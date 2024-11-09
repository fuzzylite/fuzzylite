# fuzzylite-devtools: developer tools for fuzzylite

# This file is part of fuzzylite.
# Repository: https://github.com/fuzzylite/fuzzylite/
# License: FuzzyLite License
# Copyright: FuzzyLite by Juan Rada-Vilela. All rights reserved.

"""fuzzylite-devtools: developer tools to work on fuzzylite

by Juan Rada-Vilela, PhD.

Examples:
`nox`: execute tasks to configure, build, test, and coverage of fuzzylite with default settings
`nox -l`: list available tasks
`nox -e configure -- strict=ON cxx_standard=23`: configure strict build of fuzzylite with C++23
`nox -e build -- jobs=1`: build previously configured fuzzylite without parallelization
`nox -e clean`: clean the `build/` directory
`nox --help`: show help about using nox
"""

from __future__ import annotations

import dataclasses
import datetime
import json
import os
import platform
import re
import shutil
import tempfile
import textwrap
from pathlib import Path
from typing import Any, Literal

import nox
import tqdm
from nox.command import CommandFailed

# use the virtual environment from which nox was called
nox.options.force_venv_backend = "none"
nox.options.sessions = ["version", "configure", "build", "test", "coverage"]


Build = Literal["release", "debug", "relwithdebinfo", "minsizerel"]
CxxStandard = Literal["98", "11", "14", "17", "20", "23", "26"]


@dataclasses.dataclass
class Configuration:
    """Configuration class for fuzzylite."""

    build: Build = "relwithdebinfo"
    cxx_standard: CxxStandard = "11"
    install_prefix: str = ".local"
    strict: bool = False
    tests: bool = True
    coverage: bool = True
    use_float: bool = False
    jobs: int = max((os.cpu_count() or 1) // 3, 1)
    container: str = "docker"
    _posargs: list[str] = dataclasses.field(default_factory=list)

    def posargs(self) -> str:
        return " ".join(self._posargs)

    def build_path(self) -> Path:
        """Return the build path (eg, `build/release`)

        Returns:
            the build path
        """
        return Tools.base_build() / self.build

    def convert_value(self, property: str, value: Any) -> Any:
        """Convert the value based on the current contents of the configuration
        Args:
            property: name of configuration property
            value: value of the configuration property

        Returns:
            the given value converted to the appropriate type

        Raises:
            ValueError, when the conversion fails or the type of value is not handled.
        """
        current_value = getattr(self, property)
        if isinstance(current_value, str):
            return value
        if isinstance(current_value, bool):
            boolean = {
                "true": True,
                "on": True,
                "1": True,
                "false": False,
                "off": False,
                "0": False,
            }
            return boolean[str(value).lower()]
        if isinstance(current_value, int):
            return int(value)
        raise ValueError(f"unknown type of value: {value} ({current_value})")

    def override(self, posargs: list[str]) -> Configuration:
        """Override the configuration with the session arguments
        The session arguments can be known key-value pairs, like `cxx_standard=20` or `--verbose`

        Args:
            posargs: the session arguments.

        Returns:
            self with updated properties based on the session arguments

        Raises:
            ValueError, when the conversion of known `key=value` pairs fails
        """
        for arg in posargs:
            try:
                key, value = arg.split("=")
            except ValueError:
                self._posargs.append(arg)
                continue
            if hasattr(self, key):
                value = self.convert_value(key, value)
                setattr(self, key, value)
            else:
                self._posargs.append(arg)
        return self

    def to_env(self) -> str:
        export = [
            f"{key}={value}"
            for key, value in vars(self).items()
            if not key.startswith("_")
        ]
        if self.posargs():
            export.append(f"posargs={self.posargs()}")
        return "\n".join(export)

    def to_json(self) -> str:
        export = {
            key: value for key, value in vars(self).items() if not key.startswith("_")
        }
        return json.dumps(export)

    @staticmethod
    def from_json(json_text: str) -> Configuration:
        return Configuration(**json.loads(json_text))

    def save(self) -> None:
        """Save the configuration file"""
        Tools.configuration_file().write_text(self.to_json())

    @staticmethod
    def load() -> Configuration:
        """Load the configuration file"""
        return Configuration.from_json(Tools.configuration_file().read_text())

    @staticmethod
    def for_session(
        session: nox.Session, load_env: bool = True, posargs: bool = True
    ) -> Configuration:
        """Load the default configuration,
        overriding with the configuration file
        and the session arguments.

        Args:
            session: task with optional arguments
            load_env: whether to override default configuration with the `build/fuzzylite.env` environment file
            posargs: whether to override the configuration with the session arguments

        Returns:
            a configuration with properties overriden from environment file and session
        """
        if load_env and Tools.configuration_file().exists():
            configuration = Configuration.load()
        else:
            configuration = Configuration()

        if posargs:
            configuration.override(session.posargs)
        session.log(
            "\nConfiguration:\n" + textwrap.indent(configuration.to_env(), prefix="\t")
        )
        return configuration


class Tools:
    @staticmethod
    def base_build() -> Path:
        """Return the path to the base build folder (ie, `./build`)

        Returns:
            Path to `./build`
        """
        return Path("./build")

    @staticmethod
    def configuration_file() -> Path:
        return Tools.base_build() / "fuzzylite.json"

    @staticmethod
    def poetry_directory() -> Path:
        """Return the path to the poetry project

        Returns:
            Path to `./tools.dev`
        """
        return Path("./tools/dev")

    @staticmethod
    def create_temporal_directory(name: str) -> Path:
        """Create a temporal directory in the form `com.fuzzylite.{name}.{yyyy/MM/dd}`
        Args:
            name: partial name of the temporal directory

        Returns:
            a temporal directory
        """
        now = re.sub(
            r"\W", "", datetime.datetime.now().replace(microsecond=0).isoformat()
        )
        temporal_directory = tempfile.mkdtemp(prefix=f"com.fuzzylite.{name}.{now}.")
        return Path(temporal_directory)

    @staticmethod
    def source_files() -> list[Path]:
        """Find C++ header and source files in the project

        Returns:
            C++ header and source files
        """
        paths = ["fuzzylite/", "fl/", "src/", "test/"]
        files = []
        for path in paths:
            files.extend(list(Path(path).rglob("**/*.h")))
            files.extend(list(Path(path).rglob("**/*.cpp")))
        files.sort()
        return files

    @staticmethod
    def clang_format(mode: Literal["lint", "format"], session: nox.Session) -> None:
        """Run the `clang-format` tool on each header and source file of the project.

        Args:
            mode: `lint` will check the files, and `format` will format the files in place
            session: to log progress
        """
        clang_format_cmd = "clang-format --style=file:.clang-format"
        parameters = {"format": "-i", "lint": "--dry-run --Werror"}
        message = {"format": "Formatting", "lint": "Linting"}
        files = Tools.source_files()
        errors: list[Path] = []
        session.log(f"{message[mode]} {len(files)} files...")
        with tqdm.tqdm(files, desc=f"{mode}") as bar:
            for file in bar:
                bar.set_description(f"{mode}: {file.parent}".ljust(30))
                cmd = " ".join([clang_format_cmd, parameters[mode], str(file)])
                try:
                    session.run(*cmd.split(), log=False)
                except CommandFailed:
                    errors.append(file)
                except (KeyboardInterrupt, Exception):  # CTRL+C or else
                    break
            bar.set_description(None)
        if errors:
            error_messages = "\n".join(str(file) for file in errors)
            session.error(f"The following files need formatting:\n{error_messages}")


## Sessions
@nox.session
def version(session: nox.Session) -> None:
    """Print the relevant libraries and their versions."""
    session.log(platform.python_version())
    session.run(*f"poetry show -T -C {Tools.poetry_directory()}".split())


@nox.session
def configure(session: nox.Session):
    """Prepare to build fuzzylite. Args: `build=relwithdebinfo cxx_standard=11 install_prefix=.local strict=OFF tests=ON coverage=ON use_float=OFF`"""
    c = Configuration.for_session(session, load_env=False)
    cmd = f"""\
cmake
    -S .
    -B {c.build_path()}
    -DCMAKE_BUILD_TYPE={c.build}
    -DCMAKE_CXX_STANDARD={c.cxx_standard}
    -DCMAKE_INSTALL_PREFIX={c.install_prefix}
    -DCMAKE_COMPILE_WARNING_AS_ERROR={c.strict}
    -DFL_BUILD_TESTS={c.tests}
    -DFL_BUILD_COVERAGE={c.coverage}
    -DFL_USE_FLOAT={c.use_float}
    {c.posargs()}
"""
    session.run(*cmd.split())
    c.save()


@nox.session
def build(session: nox.Session) -> None:
    """Build fuzzylite. Args: `jobs=3` and `cmake --build` options."""
    # TODO: jobs=3 --verbose won't work
    c = Configuration.for_session(session)
    cmd = f"""\
cmake 
    --build {c.build_path()} 
    --parallel {c.jobs}
    {c.posargs()}
"""
    session.run(*cmd.split())


@nox.session
def test(session: nox.Session) -> None:
    """Run tests. Args: `ctest --help` options"""
    c = Configuration.for_session(session)
    cmd = f"""\
ctest 
    --test-dir {c.build_path()} 
    --output-on-failure 
    --timeout 120
    {c.posargs()}
"""
    session.run(*cmd.split())
    session.log(
        f"""\
alternatively, for debugging information run:
{c.build_path()}/bin/fuzzylite-tests --reporter console
"""
    )


@nox.session
def coverage(session: nox.Session) -> None:
    """Report coverage. Args: `gcovr --help` options"""
    c = Configuration.for_session(session)
    cmd = f"""\
gcovr -r . 
    --filter src/ 
    --filter fuzzylite/ 
    --coveralls {c.build_path()}/coveralls.json 
    --html {c.build_path()}/coverage.html 
    --html-details 
    --html-single-page 
    --sort uncovered-percent 
    --html-theme github.dark-blue 
    --txt --txt-summary 
    {c.posargs()}
	{c.build_path()}/CMakeFiles/testTarget.dir
"""
    session.run(*cmd.split())
    session.log(f"open {c.build_path()}/coverage.html")


@nox.session
def install(session: nox.Session) -> None:
    """Install fuzzylite. Args: `cmake --build` options"""
    c = Configuration.for_session(session)
    cmd = f"""\
cmake 
    --build {c.build_path()} 
    --target install
    {c.posargs()}
"""
    session.run(*cmd.split())


@nox.session
def docs(session: nox.Session) -> None:
    """Build documentation. Args: `doxygen -h` options"""
    c = Configuration.for_session(session, load_env=False)
    session.run(*"doxygen --version".split())
    session.run(*f"doxygen Doxyfile {c.posargs()}".split())
    session.log("open docs/html/index.html")


@nox.session
def clean(session: nox.Session) -> None:
    """Clean the project (default `all`). Args: `[all|last|coverage]` to remove `build/` folder, last build (eg, `build/release`),  and *.gcda coverage files."""
    c = Configuration.for_session(session)
    args = set(c.posargs())
    error_message = (
        f"expected one of {['all', 'last', 'coverage']}, but found: {c.posargs()}"
    )
    if len(args) > 1:
        raise ValueError(error_message)

    if "coverage" in args:
        clean_coverage(session)
        return

    if not args or "all" in args:
        build_path = Tools.base_build()
    elif "last" in args:
        build_path = c.build_path()
    else:
        raise ValueError(error_message)

    if build_path.exists():
        tempdir = Tools.create_temporal_directory("build")
        shutil.move(build_path, tempdir)
        session.log(f"moved {build_path} to {tempdir}")
    else:
        session.log(f"nothing to clean: '{build_path}' does not exist")


def clean_coverage(session: nox.Session) -> None:
    """Removes the *.gcda coverage files."""
    c = Configuration.for_session(session)
    cov_files = list(Tools.base_build().rglob("*.gcda"))
    if cov_files:
        tempdir = Tools.create_temporal_directory("coverage")
        for gcda_file in cov_files:
            shutil.move(gcda_file, tempdir)
        session.log(f"moved {len(cov_files)} .gcda files to '{tempdir}'")
    else:
        session.log(f"nothing to clean: no .gcda files found in '{c.build_path()}'")


## Linting and formating
@nox.session
def lint(session: nox.Session) -> None:
    """Lint the project: CMakelists.txt, noxfile.py, Markdown files, C++ headers and sources"""
    session.notify(lint_cmake.__name__)
    session.notify(lint_py.__name__)
    session.notify(lint_md.__name__)
    session.notify(lint_cpp.__name__)


@nox.session
def lint_cmake(session: nox.Session) -> None:
    """Lint CMakeLists.txt"""
    session.run(*"cmakelint CMakeLists.txt".split())


@nox.session
def lint_cpp(session: nox.Session) -> None:
    """Lint C++ headers and sources"""
    Tools.clang_format("lint", session)


@nox.session
def lint_md(session: nox.Session) -> None:
    """Lint Markdown files"""
    poetry = Tools.poetry_directory()
    session.run(*f"pymarkdown --config {poetry}/pyproject.toml scan README.md".split())


@nox.session
def lint_py(session: nox.Session) -> None:
    """Lint noxfile.py and checks the poetry fuzzylite-devtools project"""
    # session.notify(lint_py_black.__name__)
    session.notify(lint_py_ruff.__name__)
    session.notify(lint_py_right.__name__)
    session.run(*f"poetry check -C {Tools.poetry_directory()}".split())




@nox.session
def lint_py_right(session: nox.Session) -> None:
    """Lint noxfile.py for static code analysis"""
    session.run(*"pyright noxfile.py".split())


def lint_py_ruff(session: nox.Session) -> None:
    """Lint noxfile.py for code formatting"""
    session.run(*"ruff --check noxfile.py".split())


@nox.session
def format(session: nox.Session) -> None:
    """Format the project: Markdown files, noxfile.py, and C++ header and source files"""
    session.notify(format_md.__name__)
    session.notify(format_py.__name__)
    session.notify(format_cpp.__name__)


@nox.session
def format_cpp(session: nox.Session) -> None:
    """Format C++ header and source files"""
    Tools.clang_format("format", session)


@nox.session
def format_md(session: nox.Session) -> None:
    """Format Markdown files"""
    session.run(*"pymarkdown --config pyproject.toml fix README.md".split())


@nox.session
def format_py(session: nox.Session) -> None:
    """Format noxfile.py"""
    session.run(*"ruff format noxfile.py".split())


## Other
@nox.session
def install_catch2(session: nox.Session) -> None:
    """Install the C++ testing library Catch2 v3.7.1. Args: `jobs=3 install_prefix=.local`"""
    c = Configuration.for_session(session)
    catch_src = Path(c.install_prefix) / "src" / "Catch2"
    if not catch_src.exists():
        git_clone = f"""\
git clone --single-branch -b v3.7.1 
    https://github.com/catchorg/Catch2.git {catch_src}
"""
        session.run(*git_clone.split())

    cmake_configure = f"""\
cmake 
    -B {catch_src}/build 
    -S {catch_src} 
    -DCMAKE_INSTALL_PREFIX={c.install_prefix} 
    -DCATCH_ENABLE_WERROR=OFF
"""
    session.run(*cmake_configure.split())

    cmake_build = f"""\
cmake 
    --build {catch_src}/build 
    --parallel {c.jobs} --target install
"""
    session.run(*cmake_build.split())


@nox.session
def jupyter(session: nox.Session) -> None:
    """Build and run containerized Jupyter Notebook for C++ based on Xeus. Args: `container=docker`"""
    # TODO: does not work on Apple Silicon
    c = Configuration.for_session(session)
    session.run(*f"{c.container} --version")

    # build
    tag = "fl-xeus"
    build_cmd = f"{c.container} build -f tools/notebook/Dockerfile -t {tag} ."
    session.run(*build_cmd.split())

    # run
    run_cmd = f"""\
{c.container} run --rm 
    -p 8888:8888 
    -v.:/mnt/fuzzylite 
    -it {tag} jupyter notebook 
    --allow-root --ip 0.0.0.0
"""
    session.run(*run_cmd.split())


@nox.session
def ubuntu(session: nox.Session) -> None:
    """Build and run containerized Ubuntu with default entrypoint to build fuzzylite. Args: `entrypoint` (eg, `/bin/bash` to enter the container)"""
    c = Configuration.for_session(session)
    session.run(*f"{c.container} --version".split())

    docker_path = Path("tools/docker")

    # update if necessary docker ignore of ubuntu with git files
    git_ls_files: str = session.run(  # pyright: ignore[reportAssignmentType]
        *"git ls-files".split(), silent=True, log=False
    )
    include_paths: set[Path] = set()
    for file in git_ls_files.strip().split("\n"):
        include_paths.add(Path(Path(file).parts[0]))

    docker_ignore = [
        "*",  # ignore everything, include only git ls-files
        *[
            f"!{file}"
            for file in sorted(include_paths, key=lambda p: (not p.is_dir(), p.name))
        ],
    ]
    docker_ignore_file = docker_path / "ubuntu.Dockerfile.dockerignore"
    docker_ignore_file.write_text("\n".join(docker_ignore))

    # build
    tag = "fl-ubuntu"
    build_cmd = f"{c.container} build -f {docker_path}/ubuntu.Dockerfile -t {tag} ."
    session.run(*build_cmd.split())

    # run
    entrypoint = c.posargs()
    run_cmd = f"""\
{c.container} run --rm 
    -p 8888:8888 
    -v.:/mnt/fuzzylite 
    -it {tag} 
    {entrypoint}
"""
    session.run(*run_cmd.split())

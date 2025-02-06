# fuzzylite-devtools: developer tools for fuzzylite

# This file is part of fuzzylite.
# Repository: https://github.com/fuzzylite/fuzzylite/
# License: FuzzyLite License
# Copyright: FuzzyLite by Juan Rada-Vilela. All rights reserved.

"""fuzzylite-devtools: developer tools for fuzzylite.

by Juan Rada-Vilela, PhD.

Examples:

- `nox`    execute tasks to configure, build, test, and coverage of fuzzylite with default settings
- `nox -l`    list available tasks
- `nox -e configure -- strict=ON cxx_standard=23`    configure strict build of fuzzylite with C++23
- `nox -e build -- jobs=1`    build previously configured fuzzylite without parallelization
- `nox -e clean`    removes the `build/` directory
- `nox --help`    show help about using nox
"""

from __future__ import annotations

import platform
import shutil
import webbrowser
from pathlib import Path

import nox
from fuzzylite_devtools import Configuration, Tools

# use the virtual environment from which nox was called
nox.options.force_venv_backend = "none"
nox.options.sessions = ["configure", "build", "test"]


## Sessions
@nox.session
def all(session: nox.Session) -> None:
    """Configure, build, and test fuzzylite in each of the four building types."""
    build_types = ["release", "debug", "relwithdebinfo", "minsizerel"]
    for build_type in build_types:
        session.posargs.append(f"build={build_type}")
        configure(session)
        build(session)
        test(session)


@nox.session
def configure(session: nox.Session) -> None:
    """Prepare to build fuzzylite. Args: `build=relwithdebinfo cxx_standard=11 install_prefix=.local strict=OFF tests=ON coverage=ON use_float=OFF`."""
    c = Configuration.for_session(session, from_file=False, log=True)
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
    if c.cxx_standard == "98":
        session.warn("fuzzylite 8 will drop support for standard C++98")
    session.run(*cmd.split())
    c.save()


@nox.session
def build(session: nox.Session) -> None:
    """Build fuzzylite. Args: `jobs=3` and `cmake --build` options."""
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
    """Run tests. Args: `ctest --help` options."""
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
    """Report coverage. Args: `gcovr --help` options."""
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
    url = c.build_path() / "coverage.html"
    session.log(f"open {url}")
    webbrowser.open(url.resolve().as_uri())


@nox.session
def install(session: nox.Session) -> None:
    """Install fuzzylite. Args: `cmake --build` options."""
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
    """Build documentation. Args: `doxygen -h` options."""
    c = Configuration.for_session(session, from_file=False)
    session.run(*"doxygen --version".split())
    session.run(*f"doxygen Doxyfile {c.posargs()}".split())
    session.log("open docs/html/index.html")


@nox.session
def clean(session: nox.Session) -> None:
    """Clean the project (default `all`). Args: `[all|last|coverage]`."""
    c = Configuration.for_session(session)
    args = set(c.posargs().split())
    error_message = f"expected one of {['all', 'last', 'coverage']}, but found: {c.posargs()}"
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
        session.log(f"moved '{build_path}' to '{tempdir}'")
    else:
        session.log(f"nothing to clean: '{build_path}' does not exist")


def clean_coverage(session: nox.Session) -> None:
    """Removes the *.gcda coverage files."""
    c = Configuration.for_session(session)
    cov_files = list(Tools.base_build().rglob("*.gcda"))
    if cov_files:
        trash = Tools.create_temporal_directory("coverage")
        for gcda_file in cov_files:
            shutil.move(gcda_file, trash)
        session.log(f"moved {len(cov_files)} .gcda files to '{trash}'")
    else:
        session.log(f"nothing to clean: no .gcda files found in '{c.build_path()}'")


@nox.session
def test_py(session: nox.Session) -> None:
    """Run tests for devtools. Args: `ctest --help` options."""
    if Path("tools/dev").exists():
        session.chdir("tools/dev")
    cmd = "python -m unittest discover -s tests/"
    session.run(*cmd.split())


## Linting and formating
@nox.session
def lint(session: nox.Session) -> None:
    """Lint the project: CMakelists.txt, Markdown files, devtools, C++ headers and sources."""
    session.notify(lint_cmake.__name__)
    session.notify(lint_md.__name__)
    session.notify(lint_cpp.__name__, posargs=session.posargs)


@nox.session
def lint_cmake(session: nox.Session) -> None:
    """Lint CMakeLists.txt."""
    session.run(*"cmakelint CMakeLists.txt".split())


@nox.session
def lint_cpp(session: nox.Session) -> None:
    """Lint C++ headers and sources."""
    Tools.clang_format("lint", session)


@nox.session
def lint_md(session: nox.Session) -> None:
    """Lint Markdown files."""
    session.chdir(Tools.poetry_directory())
    readme = Path("../../README.md")  # TODO: compute automatically
    session.run(*f"pymarkdown scan {readme}".split())


@nox.session
def lint_py(session: nox.Session) -> None:
    """Lint and check devtools."""
    session.notify(lint_py_ruff.__name__)
    session.notify(lint_py_right.__name__)
    session.run(*f"poetry check -C {Tools.poetry_directory()}".split())


@nox.session
def lint_py_right(session: nox.Session) -> None:
    """Lint devtools - static code analysis."""
    configuration = Tools.poetry_directory() / "pyproject.toml"
    session.run(*f"pyright -p {configuration} noxfile.py".split())


@nox.session
def lint_py_ruff(session: nox.Session) -> None:
    """Lint devtools - code formatting."""
    configuration = Tools.poetry_directory() / "pyproject.toml"
    session.run(*f"ruff --config {configuration} check noxfile.py tools/dev".split())


@nox.session
def format(session: nox.Session) -> None:
    """Format the project: Markdown files, devtools, and C++ header and source files."""
    session.notify(format_md.__name__)
    session.notify(format_cpp.__name__, posargs=session.posargs)


@nox.session
def format_cpp(session: nox.Session) -> None:
    """Format C++ header and source files."""
    Tools.clang_format("format", session)


@nox.session
def format_md(session: nox.Session) -> None:
    """Format Markdown files."""
    session.chdir(Tools.poetry_directory())
    readme = Path("../../README.md")  # TODO: compute automatically
    session.run(*f"pymarkdown fix {readme}".split())


@nox.session
def format_py(session: nox.Session) -> None:
    """Format devtools."""
    configuration = Tools.poetry_directory() / "pyproject.toml"
    session.run(*f"ruff --config {configuration} check noxfile.py tools/dev --fix".split())
    session.run(*f"ruff --config {configuration} format noxfile.py tools/dev/".split())


## Other
@nox.session
def setup_poetry(session: nox.Session) -> None:
    """Set up poetry."""
    session.log(platform.python_version())
    session.run(*"poetry config virtualenvs.create false".split())
    session.run(*f"poetry lock -C {Tools.poetry_directory()}".split())
    session.run(*f"poetry show -T -C {Tools.poetry_directory()}".split())


@nox.session
def poetry(session: nox.Session) -> None:
    """Run poetry with posargs."""
    posargs = " ".join(session.posargs)
    session.run(*f"poetry -C {Tools.poetry_directory()} {posargs}".split())


@nox.session
def install_catch2(session: nox.Session) -> None:
    """Install the C++ testing library Catch2 v3.7.1. Args: `jobs=3 install_prefix=.local` and `--force` to reinstall."""
    c = Configuration.for_session(session)

    catch_src = Path(c.install_prefix) / "src" / "Catch2"
    catch_build = catch_src / "build"

    if "--force" in c.posargs():
        if catch_src.exists():
            trash = Tools.create_temporal_directory("Catch2")
            shutil.move(catch_src, trash)
            session.log(f"moved '{catch_src}' to '{trash}'")

    if not catch_src.exists():
        git_clone = f"""\
git clone
    --depth 1
    --single-branch -b v3.7.1
    https://github.com/catchorg/Catch2.git {catch_src}
"""
        session.run(*git_clone.split())

    cmake_configure = f"""\
cmake
    -B {catch_build}
    -S {catch_src}
    -DCMAKE_INSTALL_PREFIX={c.install_prefix}
    -DCMAKE_CXX_STANDARD={c.cxx_standard}
    -DCMAKE_BUILD_TYPE={c.build}
    -DCATCH_ENABLE_WERROR=OFF
"""
    session.run(*cmake_configure.split())

    cmake_build = f"""\
cmake
    --build {catch_build}
    --parallel {c.jobs}
    --target install
"""
    session.run(*cmake_build.split())

    trash = Tools.create_temporal_directory("Catch2")
    shutil.move(catch_build, trash)
    session.log(f"moved '{catch_build}' to '{trash}'")


@nox.session
def jupyter(session: nox.Session) -> None:
    """Build and run containerized Jupyter Notebook for C++ based on Xeus. Args: `container=docker`."""
    # TODO: does not work on Apple Silicon
    c = Configuration.for_session(session)
    session.run(*f"{c.container} --version".split())

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
    """Build and run containerized Ubuntu with default entrypoint to build fuzzylite. Args: `entrypoint` (eg, `/bin/bash` to enter the container)."""
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
        *[f"!{file}" for file in sorted(include_paths, key=lambda p: (not p.is_dir(), p.name))],
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

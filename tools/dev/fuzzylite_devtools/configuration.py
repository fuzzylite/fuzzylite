"""fuzzylite-devtools: developer tools for fuzzylite.

This file is part of fuzzylite.
Repository: https://github.com/fuzzylite/fuzzylite/
License: FuzzyLite License
Copyright: FuzzyLite by Juan Rada-Vilela. All rights reserved.
"""
from __future__ import annotations

import dataclasses
import datetime
import json
import os
import re
import tempfile
import textwrap
from pathlib import Path
from typing import Any, Literal

import nox
import tqdm
from nox.command import CommandFailed

from .typing import Build, CxxStandard


@dataclasses.dataclass
class Configuration:
    """Configuration class for fuzzylite."""

    build: Build = "relwithdebinfo"
    cxx_standard: CxxStandard = "11"
    install_prefix: str = ".local"
    strict: bool = False
    tests: bool = True
    coverage: bool = False
    use_float: bool = False
    jobs: int = max((os.cpu_count() or 1) // 3, 1)
    container: str = "docker"
    _posargs: list[str] = dataclasses.field(default_factory=list)

    def posargs(self) -> str:
        """Return extra arguments as string."""
        return " ".join(self._posargs)

    def build_path(self) -> Path:
        """Return the build path (eg, `build/release`).

        Returns:
            the build path
        """
        return Tools.base_build() / self.build

    def convert_value(self, property: str, value: Any) -> Any:
        """Convert the value based on the current contents of the configuration.

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
        """Override the configuration with the session arguments.
        
        The session arguments can be `cxx_standard=20` or `--verbose`

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
            session: nox.Session, from_file: bool = True, posargs: bool = True, log: bool = False
    ) -> Configuration:
        """Load the default configuration, overriding with the configuration file and the session arguments.

        Args:
            session: task with optional arguments
            from_file: whether to override default with the configuration file
            posargs: whether to override the configuration with the session arguments
            log: whether to log the configuration

        Returns:
            a configuration with properties overriden from configuration file and session
        """
        if from_file and Tools.configuration_file().exists():
            configuration = Configuration.load()
        else:
            configuration = Configuration()

        if posargs:
            configuration.override(session.posargs)
        if log:
            session.log(
                "\nConfiguration:\n" + textwrap.indent(configuration.to_env(), prefix="\t")
            )
        return configuration


class Tools:
    @staticmethod
    def base_build() -> Path:
        """Return the path to the base build folder (ie, `./build`).

        Returns:
            Path to `./build`
        """
        return Path("./build")

    @staticmethod
    def configuration_file() -> Path:
        return Tools.base_build() / "fuzzylite.json"

    @staticmethod
    def poetry_directory() -> Path:
        """Return the path to the poetry project.

        Returns:
            Path to `./tools.dev`
        """
        return Path("./tools/dev")

    @staticmethod
    def create_temporal_directory(name: str) -> Path:
        """Create a temporal directory in the form `com.fuzzylite.{name}.{yyyy/MM/dd}`.
        
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
        """Find C++ header and source files in the project.

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
                except (KeyboardInterrupt, Exception) as error:  # CTRL+C or else
                    session.error(str(error))
            bar.set_description(None)

        if errors:
            error_messages = "\n".join(str(file) for file in errors)
            session.error(f"The following files need formatting:\n{error_messages}")

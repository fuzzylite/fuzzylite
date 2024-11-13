"""fuzzylite-devtools: developer tools for fuzzylite

This file is part of fuzzylite.
Repository: https://github.com/fuzzylite/fuzzylite/
License: FuzzyLite License
Copyright: FuzzyLite by Juan Rada-Vilela. All rights reserved.
"""

from __future__ import annotations

import shutil
import unittest
from unittest import mock
from unittest.mock import MagicMock

import nox
import nox.registry
from fuzzylite_devtools import Tools

import noxfile


class AssertThat:
    def __init__(self, session: nox.registry.F) -> None:
        self.session = session

    def runs(
        self, command: str | list[str] | None = None, *, posargs: list[str] | None = None
    ) -> MagicMock[nox.Session]:
        mock_session = mock.Mock(spec=nox.Session)
        mock_session.posargs = posargs or []
        mock_session.run = MagicMock()
        mock_session.log = MagicMock()
        self.session(mock_session)
        if isinstance(command, str):
            command = command.split()
        if command:
            mock_session.run.assert_called_with(*command)
        return mock_session


class TestSessions(unittest.TestCase):
    def setUp(self) -> None:
        Tools.base_build().mkdir(parents=True, exist_ok=True)

    def tearDown(self) -> None:
        shutil.rmtree(Tools.base_build(), ignore_errors=True)

    def test_configure(self) -> None:
        AssertThat(noxfile.configure).runs(
            """\
cmake
    -S .
    -B build/relwithdebinfo
    -DCMAKE_BUILD_TYPE=relwithdebinfo
    -DCMAKE_CXX_STANDARD=11
    -DCMAKE_INSTALL_PREFIX=.local
    -DCMAKE_COMPILE_WARNING_AS_ERROR=False
    -DFL_BUILD_TESTS=True
    -DFL_BUILD_COVERAGE=False
    -DFL_USE_FLOAT=False
"""
        )

    def test_configure_with_args(self) -> None:
        expected = """\
    cmake
        -S .
        -B build/debug
        -DCMAKE_BUILD_TYPE=debug
        -DCMAKE_CXX_STANDARD=23
        -DCMAKE_INSTALL_PREFIX=/tmp
        -DCMAKE_COMPILE_WARNING_AS_ERROR=False
        -DFL_BUILD_TESTS=False
        -DFL_BUILD_COVERAGE=True
        -DFL_USE_FLOAT=True
        --verbose
    """
        posargs = [
            "build=debug",
            "cxx_standard=23",
            "install_prefix=/tmp",
            "strict=OFF",
            "tests=off",
            "coverage=ON",
            "use_float=ON",
            "--verbose",
        ]
        AssertThat(noxfile.configure).runs(expected, posargs=posargs)

    def test_build(self) -> None:
        AssertThat(noxfile.build).runs(
            "cmake --build build/relwithdebinfo --parallel 1", posargs=["jobs=1"]
        )

    def test_build_with_configure(self) -> None:
        AssertThat(noxfile.configure).runs(posargs=["build=debug", "jobs=999"])
        AssertThat(noxfile.build).runs("cmake --build build/debug --parallel 999")

    def test_test(self) -> None:
        AssertThat(noxfile.test).runs(
            "ctest --test-dir build/relwithdebinfo --output-on-failure --timeout 120"
        )

    def test_test_with_configure_and_args(self) -> None:
        AssertThat(noxfile.configure).runs(posargs=["build=debug"])
        AssertThat(noxfile.test).runs(
            "ctest --test-dir build/debug --output-on-failure --timeout 120 --verbose",
            posargs=["--verbose"],
        )

    def test_coverage(self) -> None:
        expected = """\
gcovr -r .
    --filter src/
    --filter fuzzylite/
    --coveralls build/relwithdebinfo/coveralls.json
    --html build/relwithdebinfo/coverage.html
    --html-details
    --html-single-page
    --sort uncovered-percent
    --html-theme github.dark-blue
    --txt --txt-summary
	build/relwithdebinfo/CMakeFiles/testTarget.dir"""
        AssertThat(noxfile.coverage).runs(expected)

    def test_coverage_with_configuration_and_posargs(self) -> None:
        AssertThat(noxfile.configure).runs(posargs=["build=debug"])
        expected = """\
        gcovr -r .
            --filter src/
            --filter fuzzylite/
            --coveralls build/debug/coveralls.json
            --html build/debug/coverage.html
            --html-details
            --html-single-page
            --sort uncovered-percent
            --html-theme github.dark-blue
            --txt --txt-summary
            --verbose
        build/debug/CMakeFiles/testTarget.dir"""
        AssertThat(noxfile.coverage).runs(expected, posargs=["--verbose"])

    def test_install(self) -> None:
        AssertThat(noxfile.install).runs("cmake --build build/relwithdebinfo --target install")

    def test_install_with_configuration_and_posargs(self) -> None:
        AssertThat(noxfile.configure).runs(posargs=["build=debug"])
        AssertThat(noxfile.install).runs(
            "cmake --build build/debug --target install --verbose", posargs=["--verbose"]
        )

    def test_docs(self) -> None:
        AssertThat(noxfile.docs).runs("doxygen Doxyfile")

    def test_docs_with_args(self) -> None:
        AssertThat(noxfile.docs).runs("doxygen Doxyfile --verbose", posargs=["--verbose"])

    def test_clean(self) -> None:
        session = AssertThat(noxfile.clean).runs()
        assert session.log.call_args.startswith("moved 'build' to")  # "... to /var/tmp/...

        session = AssertThat(noxfile.clean).runs()
        assert session.log.call_args.startswith("nothing to clean: 'build/' does not exist")

    def test_clean_all(self) -> None:
        session = AssertThat(noxfile.clean).runs(posargs=["all"])
        assert session.log.call_args.startswith("moved 'build' to")  # "... to /var/tmp/...

        session = AssertThat(noxfile.clean).runs()
        assert session.log.call_args.startswith("nothing to clean: 'build/' does not exist")

    def test_clean_last(self) -> None:
        last_build = Tools.base_build() / "debug"
        last_build.mkdir(parents=True, exist_ok=True)

        session = AssertThat(noxfile.clean).runs(posargs=["build=debug", "last"])
        assert session.log.call_args.startswith("moved 'build/debug' to")  # "... to /var/tmp/...

        session = AssertThat(noxfile.clean).runs(posargs=["build=debug", "last"])
        assert session.log.call_args.startswith("nothing to clean: 'build/debug' does not exist")

    def test_clean_coverage(self) -> None:
        last_build = Tools.base_build() / "debug" / "fuzzylite"
        last_build.mkdir(parents=True, exist_ok=True)

        coverage_files = [(last_build.parent / "file1.gcda"), (last_build / "file2.gcda")]
        for file in coverage_files:
            file.touch(exist_ok=True)
            assert file.exists()

        session = AssertThat(noxfile.clean).runs(posargs=["build=debug", "last"])
        assert session.log.call_args.startswith("moved 2 .gcda files to")  # "... to /var/tmp/...
        for file in coverage_files:
            assert not file.exists()

        session = AssertThat(noxfile.clean).runs(posargs=["build=debug", "last"])
        assert session.log.call_args.startswith("nothing to clean: no .gcda files found in")

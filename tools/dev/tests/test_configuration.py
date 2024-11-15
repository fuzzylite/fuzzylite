"""fuzzylite-devtools: developer tools for fuzzylite

This file is part of fuzzylite.
Repository: https://github.com/fuzzylite/fuzzylite/
License: FuzzyLite License
Copyright: FuzzyLite by Juan Rada-Vilela. All rights reserved.
"""

import unittest

from fuzzylite_devtools import Configuration


class TestImport(unittest.TestCase):
    def test_default_configuration(self) -> None:
        expected = """\
build=relwithdebinfo
cxx_standard=11
install_prefix=.local
strict=False
tests=True
coverage=False
use_float=False
jobs=3
container=docker"""
        self.assertEqual(expected, Configuration().override(["jobs=3"]).to_env())

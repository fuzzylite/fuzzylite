"""fuzzylite-devtools: developer tools for fuzzylite.

This file is part of fuzzylite.
Repository: https://github.com/fuzzylite/fuzzylite/
License: FuzzyLite License
Copyright: FuzzyLite by Juan Rada-Vilela. All rights reserved.
"""

__all__ = ["Build", "CxxStandard"]

from typing import Literal

Build = Literal["release", "debug", "relwithdebinfo", "minsizerel"]
CxxStandard = Literal["98", "11", "14", "17", "20", "23", "26"]

Disclaimer
==========

The following folders contain examples from Matlab and Octave distributions. The examples are presented in the following formats: fuzzylite v5.0 `.cpp`, jfuzzylite v5.0 `.java`, FuzzyLite Language `.fll`, FuzzyLite Dataset `.fld`, Fuzzy Inference System `.fis`, and Fuzzy Controller Language `.fcl`. The formats were created automatically utilizing the `[Cpp|Java|Fll|Fld|Fis|Fcl]Exporter`.

The original files from Matlab and Octave distributions are contained within the folder `original`, although the following files from Matlab were modified to remove the fourth parameter of the term `gbellmf`, which is not required for its configuration.

+ [`examples/original/mamdani/matlab/mam21.fis`](/examples/original/mamdani/matlab/mam21.fis)

+ [`examples/original/mamdani/matlab/mam22.fis`](/examples/original/mamdani/matlab/mam22.fis)

In addition to the original examples, the examples are presented in the FuzzyLite Language to include scalar values with at least the same number of decimal places, and a proper configuration in the cases of Takagi-Sugeno controllers (i.e., `RuleBlock::activation = none` and `OutputVariable::accumulation = none`) to satisfy the regular operation of the controllers in `fuzzylite` v5.0.


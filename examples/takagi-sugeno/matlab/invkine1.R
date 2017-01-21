#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "invkine1"
engine.fll = "Engine: invkine1
InputVariable: input1
  enabled: true
  range: -6.287 17.000
  lock-range: false
  term: in1mf1 Bell -6.122 2.259 1.761
  term: in1mf2 Bell -2.181 2.095 2.232
  term: in1mf3 Bell 2.080 2.157 1.314
  term: in1mf4 Bell 4.962 2.790 2.508
  term: in1mf5 Bell 9.338 2.506 1.812
  term: in1mf6 Bell 13.150 2.363 2.267
  term: in1mf7 Bell 17.789 1.310 1.756
InputVariable: input2
  enabled: true
  range: 0.000 16.972
  lock-range: false
  term: in2mf1 Bell 0.621 1.741 2.454
  term: in2mf2 Bell 2.406 0.866 1.278
  term: in2mf3 Bell 4.883 1.814 2.402
  term: in2mf4 Bell 8.087 1.941 1.929
  term: in2mf5 Bell 11.428 2.333 2.022
  term: in2mf6 Bell 14.579 2.221 1.858
  term: in2mf7 Bell 17.813 0.820 1.577
OutputVariable: output
  enabled: true
  range: 0.000 1.500
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: out1mf1 Linear -0.912 2.517 0.061
  term: out1mf2 Linear -2.153 -2.204 -4.037
  term: out1mf3 Linear -0.107 -0.148 1.920
  term: out1mf4 Linear -0.088 -0.071 1.593
  term: out1mf5 Linear -0.098 -0.040 1.361
  term: out1mf6 Linear -0.068 -0.027 1.617
  term: out1mf7 Linear -1.901 -0.081 0.185
  term: out1mf8 Linear 16.651 11.713 6.803
  term: out1mf9 Linear -4.152 -1.033 -4.755
  term: out1mf10 Linear -0.123 0.004 0.861
  term: out1mf11 Linear -0.102 0.006 0.816
  term: out1mf12 Linear -0.089 0.038 0.515
  term: out1mf13 Linear -0.074 0.082 -0.061
  term: out1mf14 Linear -0.009 -0.173 4.841
  term: out1mf15 Linear -7.995 -2.818 17.910
  term: out1mf16 Linear 0.674 0.745 -2.167
  term: out1mf17 Linear -0.130 -0.004 0.869
  term: out1mf18 Linear -0.094 0.061 0.366
  term: out1mf19 Linear -0.087 0.121 -0.395
  term: out1mf20 Linear -0.061 0.162 -1.312
  term: out1mf21 Linear -0.163 0.920 -13.253
  term: out1mf22 Linear 1.417 3.072 1.881
  term: out1mf23 Linear -0.950 -0.732 3.128
  term: out1mf24 Linear -0.058 0.162 -0.530
  term: out1mf25 Linear -0.044 0.077 -0.121
  term: out1mf26 Linear -0.061 0.054 0.273
  term: out1mf27 Linear -0.068 0.099 -0.250
  term: out1mf28 Linear 0.429 0.610 -9.118
  term: out1mf29 Linear -6.661 -3.697 0.015
  term: out1mf30 Linear -3.544 8.995 0.396
  term: out1mf31 Linear 0.210 -0.137 -1.010
  term: out1mf32 Linear -0.003 0.137 -1.210
  term: out1mf33 Linear -0.030 0.018 0.107
  term: out1mf34 Linear -0.227 -0.306 6.550
  term: out1mf35 Linear 11.005 -3.375 -1.135
  term: out1mf36 Linear 0.578 0.033 -0.104
  term: out1mf37 Linear 0.895 -3.268 -0.992
  term: out1mf38 Linear 0.125 0.006 -1.733
  term: out1mf39 Linear 0.044 0.003 -0.303
  term: out1mf40 Linear -0.179 -0.093 3.458
  term: out1mf41 Linear 0.222 0.597 -10.102
  term: out1mf42 Linear 9.320 13.692 0.858
  term: out1mf43 Linear 0.161 -0.117 -1.382
  term: out1mf44 Linear 0.495 -0.833 -6.564
  term: out1mf45 Linear 0.465 -0.787 -5.610
  term: out1mf46 Linear 1.334 -3.017 -2.871
  term: out1mf47 Linear 2.561 -0.864 -0.557
  term: out1mf48 Linear 17.123 11.150 1.006
  term: out1mf49 Linear 0.220 0.154 0.010
RuleBlock: 
  enabled: true
  conjunction: AlgebraicProduct
  disjunction: none
  implication: none
  activation: General
  rule: if input1 is in1mf1 and input2 is in2mf1 then output is out1mf1
  rule: if input1 is in1mf1 and input2 is in2mf2 then output is out1mf2
  rule: if input1 is in1mf1 and input2 is in2mf3 then output is out1mf3
  rule: if input1 is in1mf1 and input2 is in2mf4 then output is out1mf4
  rule: if input1 is in1mf1 and input2 is in2mf5 then output is out1mf5
  rule: if input1 is in1mf1 and input2 is in2mf6 then output is out1mf6
  rule: if input1 is in1mf1 and input2 is in2mf7 then output is out1mf7
  rule: if input1 is in1mf2 and input2 is in2mf1 then output is out1mf8
  rule: if input1 is in1mf2 and input2 is in2mf2 then output is out1mf9
  rule: if input1 is in1mf2 and input2 is in2mf3 then output is out1mf10
  rule: if input1 is in1mf2 and input2 is in2mf4 then output is out1mf11
  rule: if input1 is in1mf2 and input2 is in2mf5 then output is out1mf12
  rule: if input1 is in1mf2 and input2 is in2mf6 then output is out1mf13
  rule: if input1 is in1mf2 and input2 is in2mf7 then output is out1mf14
  rule: if input1 is in1mf3 and input2 is in2mf1 then output is out1mf15
  rule: if input1 is in1mf3 and input2 is in2mf2 then output is out1mf16
  rule: if input1 is in1mf3 and input2 is in2mf3 then output is out1mf17
  rule: if input1 is in1mf3 and input2 is in2mf4 then output is out1mf18
  rule: if input1 is in1mf3 and input2 is in2mf5 then output is out1mf19
  rule: if input1 is in1mf3 and input2 is in2mf6 then output is out1mf20
  rule: if input1 is in1mf3 and input2 is in2mf7 then output is out1mf21
  rule: if input1 is in1mf4 and input2 is in2mf1 then output is out1mf22
  rule: if input1 is in1mf4 and input2 is in2mf2 then output is out1mf23
  rule: if input1 is in1mf4 and input2 is in2mf3 then output is out1mf24
  rule: if input1 is in1mf4 and input2 is in2mf4 then output is out1mf25
  rule: if input1 is in1mf4 and input2 is in2mf5 then output is out1mf26
  rule: if input1 is in1mf4 and input2 is in2mf6 then output is out1mf27
  rule: if input1 is in1mf4 and input2 is in2mf7 then output is out1mf28
  rule: if input1 is in1mf5 and input2 is in2mf1 then output is out1mf29
  rule: if input1 is in1mf5 and input2 is in2mf2 then output is out1mf30
  rule: if input1 is in1mf5 and input2 is in2mf3 then output is out1mf31
  rule: if input1 is in1mf5 and input2 is in2mf4 then output is out1mf32
  rule: if input1 is in1mf5 and input2 is in2mf5 then output is out1mf33
  rule: if input1 is in1mf5 and input2 is in2mf6 then output is out1mf34
  rule: if input1 is in1mf5 and input2 is in2mf7 then output is out1mf35
  rule: if input1 is in1mf6 and input2 is in2mf1 then output is out1mf36
  rule: if input1 is in1mf6 and input2 is in2mf2 then output is out1mf37
  rule: if input1 is in1mf6 and input2 is in2mf3 then output is out1mf38
  rule: if input1 is in1mf6 and input2 is in2mf4 then output is out1mf39
  rule: if input1 is in1mf6 and input2 is in2mf5 then output is out1mf40
  rule: if input1 is in1mf6 and input2 is in2mf6 then output is out1mf41
  rule: if input1 is in1mf6 and input2 is in2mf7 then output is out1mf42
  rule: if input1 is in1mf7 and input2 is in2mf1 then output is out1mf43
  rule: if input1 is in1mf7 and input2 is in2mf2 then output is out1mf44
  rule: if input1 is in1mf7 and input2 is in2mf3 then output is out1mf45
  rule: if input1 is in1mf7 and input2 is in2mf4 then output is out1mf46
  rule: if input1 is in1mf7 and input2 is in2mf5 then output is out1mf47
  rule: if input1 is in1mf7 and input2 is in2mf6 then output is out1mf48
  rule: if input1 is in1mf7 and input2 is in2mf7 then output is out1mf49"

engine.fldFile = "invkine1.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(input1, input2)) + 
    geom_tile(aes(fill=output)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=input1, y=input2, z=output), color="black") + 
    ggtitle("(input1, input2) = output")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(input2, input1)) + 
    geom_tile(aes(fill=output)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=input2, y=input1, z=output), color="black") + 
    ggtitle("(input2, input1) = output")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

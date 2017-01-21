#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "invkine2"
engine.fll = "Engine: invkine2
InputVariable: input1
  enabled: true
  range: -6.287 17.000
  lock-range: false
  term: in1mf1 Bell -5.763 3.015 1.851
  term: in1mf2 Bell -1.624 3.130 2.111
  term: in1mf3 Bell 3.552 3.193 2.104
  term: in1mf4 Bell 8.273 2.907 1.985
  term: in1mf5 Bell 13.232 2.708 2.056
  term: in1mf6 Bell 17.783 1.635 1.897
InputVariable: input2
  enabled: true
  range: 0.000 16.972
  lock-range: false
  term: in2mf1 Bell 0.005 1.877 1.995
  term: in2mf2 Bell 3.312 2.017 1.829
  term: in2mf3 Bell 6.568 2.261 1.793
  term: in2mf4 Bell 10.111 2.741 1.978
  term: in2mf5 Bell 14.952 2.045 1.783
  term: in2mf6 Bell 17.910 0.824 1.734
OutputVariable: output
  enabled: true
  range: 0.000 3.100
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: out1mf1 Linear -0.048 1.456 2.222
  term: out1mf2 Linear -0.218 -0.305 2.042
  term: out1mf3 Linear 0.026 -0.141 3.067
  term: out1mf4 Linear 0.052 -0.150 3.419
  term: out1mf5 Linear 0.113 -0.189 4.339
  term: out1mf6 Linear 2.543 0.361 -2.738
  term: out1mf7 Linear 2.517 -6.809 23.353
  term: out1mf8 Linear -0.208 -0.394 4.472
  term: out1mf9 Linear -0.046 -0.300 4.452
  term: out1mf10 Linear -0.006 -0.217 4.195
  term: out1mf11 Linear 0.089 -0.254 4.992
  term: out1mf12 Linear -0.033 0.103 -2.012
  term: out1mf13 Linear 1.355 1.228 -5.678
  term: out1mf14 Linear -0.245 -0.124 3.753
  term: out1mf15 Linear -0.099 -0.111 3.304
  term: out1mf16 Linear -0.052 -0.163 3.560
  term: out1mf17 Linear 0.099 -0.260 4.662
  term: out1mf18 Linear 0.082 -1.849 31.104
  term: out1mf19 Linear 2.180 -2.963 -0.061
  term: out1mf20 Linear -0.982 0.510 5.657
  term: out1mf21 Linear -0.087 -0.179 3.744
  term: out1mf22 Linear -0.124 -0.161 4.094
  term: out1mf23 Linear 0.383 0.007 -1.559
  term: out1mf24 Linear -8.415 2.083 5.177
  term: out1mf25 Linear 1.721 -15.079 -0.687
  term: out1mf26 Linear -1.043 -0.786 20.510
  term: out1mf27 Linear -0.249 -0.396 6.995
  term: out1mf28 Linear -0.076 -0.245 4.416
  term: out1mf29 Linear 0.765 -1.488 17.384
  term: out1mf30 Linear -21.210 -43.022 -2.522
  term: out1mf31 Linear -0.661 3.523 6.215
  term: out1mf32 Linear -1.998 1.582 33.256
  term: out1mf33 Linear -2.068 5.673 6.520
  term: out1mf34 Linear -5.044 7.093 3.516
  term: out1mf35 Linear -46.049 -35.021 -2.926
  term: out1mf36 Linear -0.448 -0.770 -0.041
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
  rule: if input1 is in1mf2 and input2 is in2mf1 then output is out1mf7
  rule: if input1 is in1mf2 and input2 is in2mf2 then output is out1mf8
  rule: if input1 is in1mf2 and input2 is in2mf3 then output is out1mf9
  rule: if input1 is in1mf2 and input2 is in2mf4 then output is out1mf10
  rule: if input1 is in1mf2 and input2 is in2mf5 then output is out1mf11
  rule: if input1 is in1mf2 and input2 is in2mf6 then output is out1mf12
  rule: if input1 is in1mf3 and input2 is in2mf1 then output is out1mf13
  rule: if input1 is in1mf3 and input2 is in2mf2 then output is out1mf14
  rule: if input1 is in1mf3 and input2 is in2mf3 then output is out1mf15
  rule: if input1 is in1mf3 and input2 is in2mf4 then output is out1mf16
  rule: if input1 is in1mf3 and input2 is in2mf5 then output is out1mf17
  rule: if input1 is in1mf3 and input2 is in2mf6 then output is out1mf18
  rule: if input1 is in1mf4 and input2 is in2mf1 then output is out1mf19
  rule: if input1 is in1mf4 and input2 is in2mf2 then output is out1mf20
  rule: if input1 is in1mf4 and input2 is in2mf3 then output is out1mf21
  rule: if input1 is in1mf4 and input2 is in2mf4 then output is out1mf22
  rule: if input1 is in1mf4 and input2 is in2mf5 then output is out1mf23
  rule: if input1 is in1mf4 and input2 is in2mf6 then output is out1mf24
  rule: if input1 is in1mf5 and input2 is in2mf1 then output is out1mf25
  rule: if input1 is in1mf5 and input2 is in2mf2 then output is out1mf26
  rule: if input1 is in1mf5 and input2 is in2mf3 then output is out1mf27
  rule: if input1 is in1mf5 and input2 is in2mf4 then output is out1mf28
  rule: if input1 is in1mf5 and input2 is in2mf5 then output is out1mf29
  rule: if input1 is in1mf5 and input2 is in2mf6 then output is out1mf30
  rule: if input1 is in1mf6 and input2 is in2mf1 then output is out1mf31
  rule: if input1 is in1mf6 and input2 is in2mf2 then output is out1mf32
  rule: if input1 is in1mf6 and input2 is in2mf3 then output is out1mf33
  rule: if input1 is in1mf6 and input2 is in2mf4 then output is out1mf34
  rule: if input1 is in1mf6 and input2 is in2mf5 then output is out1mf35
  rule: if input1 is in1mf6 and input2 is in2mf6 then output is out1mf36"

engine.fldFile = "invkine2.fld"
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

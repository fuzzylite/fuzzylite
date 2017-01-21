#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "slbb"
engine.fll = "Engine: slbb
InputVariable: in1
  enabled: true
  range: -1.500 1.500
  lock-range: false
  term: in1mf1 Bell -1.500 1.500 2.000
  term: in1mf2 Bell 1.500 1.500 2.000
InputVariable: in2
  enabled: true
  range: -1.500 1.500
  lock-range: false
  term: in2mf1 Bell -1.500 1.500 2.000
  term: in2mf2 Bell 1.500 1.500 2.000
InputVariable: in3
  enabled: true
  range: -0.200 0.200
  lock-range: false
  term: in3mf1 Bell -0.200 0.200 2.000
  term: in3mf2 Bell 0.200 0.200 2.000
InputVariable: in4
  enabled: true
  range: -0.400 0.400
  lock-range: false
  term: in4mf1 Bell -0.400 0.400 2.000
  term: in4mf2 Bell 0.400 0.400 2.000
OutputVariable: out
  enabled: true
  range: -10.000 10.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: outmf1 Linear 1.015 2.234 -12.665 -4.046 0.026
  term: outmf2 Linear 1.161 1.969 -9.396 -6.165 0.474
  term: outmf3 Linear 1.506 2.234 -12.990 -1.865 1.426
  term: outmf4 Linear 0.734 1.969 -9.381 -4.688 -0.880
  term: outmf5 Linear 0.734 2.234 -12.853 -6.110 -1.034
  term: outmf6 Linear 1.413 1.969 -9.485 -6.592 1.159
  term: outmf7 Linear 1.225 2.234 -12.801 -3.929 0.366
  term: outmf8 Linear 0.985 1.969 -9.291 -5.115 -0.195
  term: outmf9 Linear 0.985 1.969 -9.292 -5.115 0.195
  term: outmf10 Linear 1.225 2.234 -12.802 -3.929 -0.366
  term: outmf11 Linear 1.413 1.969 -9.485 -6.592 -1.159
  term: outmf12 Linear 0.734 2.234 -12.853 -6.110 1.034
  term: outmf13 Linear 0.734 1.969 -9.381 -4.688 0.880
  term: outmf14 Linear 1.506 2.234 -12.990 -1.865 -1.426
  term: outmf15 Linear 1.161 1.969 -9.396 -6.165 -0.474
  term: outmf16 Linear 1.015 2.234 -12.665 -4.046 -0.026
RuleBlock: 
  enabled: true
  conjunction: AlgebraicProduct
  disjunction: none
  implication: none
  activation: General
  rule: if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf1 then out is outmf1
  rule: if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf2 then out is outmf2
  rule: if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf1 then out is outmf3
  rule: if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf2 then out is outmf4
  rule: if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf1 then out is outmf5
  rule: if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf2 then out is outmf6
  rule: if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf1 then out is outmf7
  rule: if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf2 then out is outmf8
  rule: if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf1 then out is outmf9
  rule: if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf2 then out is outmf10
  rule: if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf1 then out is outmf11
  rule: if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf2 then out is outmf12
  rule: if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf1 then out is outmf13
  rule: if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf2 then out is outmf14
  rule: if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf1 then out is outmf15
  rule: if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf2 then out is outmf16"

engine.fldFile = "slbb.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(in1, in2)) + 
    geom_tile(aes(fill=out)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=in1, y=in2, z=out), color="black") + 
    ggtitle("(in1, in2) = out")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(in2, in1)) + 
    geom_tile(aes(fill=out)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=in2, y=in1, z=out), color="black") + 
    ggtitle("(in2, in1) = out")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

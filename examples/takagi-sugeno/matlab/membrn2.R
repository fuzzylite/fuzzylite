#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "membrn2"
engine.fll = "Engine: membrn2
InputVariable: in_n1
  enabled: true
  range: 1.000 31.000
  lock-range: false
  term: in1mf1 Bell 1.152 8.206 0.874
  term: in1mf2 Bell 15.882 7.078 0.444
  term: in1mf3 Bell 30.575 8.602 0.818
InputVariable: in_n2
  enabled: true
  range: 1.000 31.000
  lock-range: false
  term: in2mf1 Bell 1.426 8.602 0.818
  term: in2mf2 Bell 16.118 7.078 0.445
  term: in2mf3 Bell 30.847 8.206 0.875
OutputVariable: out1
  enabled: true
  range: -0.334 1.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: out1mf1 Linear -0.035 0.002 -0.352
  term: out1mf2 Linear 0.044 0.079 -0.028
  term: out1mf3 Linear -0.024 0.024 -1.599
  term: out1mf4 Linear -0.067 0.384 0.007
  term: out1mf5 Linear 0.351 -0.351 -3.663
  term: out1mf6 Linear -0.079 -0.044 3.909
  term: out1mf7 Linear 0.012 -0.012 -0.600
  term: out1mf8 Linear -0.384 0.067 10.158
  term: out1mf9 Linear -0.002 0.035 -1.402
RuleBlock: 
  enabled: true
  conjunction: AlgebraicProduct
  disjunction: none
  implication: none
  activation: General
  rule: if in_n1 is in1mf1 and in_n2 is in2mf1 then out1 is out1mf1
  rule: if in_n1 is in1mf1 and in_n2 is in2mf2 then out1 is out1mf2
  rule: if in_n1 is in1mf1 and in_n2 is in2mf3 then out1 is out1mf3
  rule: if in_n1 is in1mf2 and in_n2 is in2mf1 then out1 is out1mf4
  rule: if in_n1 is in1mf2 and in_n2 is in2mf2 then out1 is out1mf5
  rule: if in_n1 is in1mf2 and in_n2 is in2mf3 then out1 is out1mf6
  rule: if in_n1 is in1mf3 and in_n2 is in2mf1 then out1 is out1mf7
  rule: if in_n1 is in1mf3 and in_n2 is in2mf2 then out1 is out1mf8
  rule: if in_n1 is in1mf3 and in_n2 is in2mf3 then out1 is out1mf9"

engine.fldFile = "membrn2.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(in_n1, in_n2)) + 
    geom_tile(aes(fill=out1)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=in_n1, y=in_n2, z=out1), color="black") + 
    ggtitle("(in_n1, in_n2) = out1")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(in_n2, in_n1)) + 
    geom_tile(aes(fill=out1)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=in_n2, y=in_n1, z=out1), color="black") + 
    ggtitle("(in_n2, in_n1) = out1")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

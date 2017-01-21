#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "slcp1"
engine.fll = "Engine: slcp1
InputVariable: in1
  enabled: true
  range: -0.300 0.300
  lock-range: false
InputVariable: in2
  enabled: true
  range: -1.000 1.000
  lock-range: false
InputVariable: in3
  enabled: true
  range: -3.000 3.000
  lock-range: false
InputVariable: in4
  enabled: true
  range: -3.000 3.000
  lock-range: false
InputVariable: in5
  enabled: true
  range: 0.500 1.500
  lock-range: false
  term: small Gaussian 0.500 0.200
  term: medium Gaussian 1.000 0.200
  term: large Gaussian 1.500 0.200
OutputVariable: out
  enabled: true
  range: -10.000 10.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: outmf1 Linear 32.166 5.835 3.162 3.757 0.000 0.000
  term: outmf2 Linear 39.012 9.947 3.162 4.269 0.000 0.000
  term: outmf3 Linear 45.009 13.985 3.162 4.666 0.000 0.000
RuleBlock: 
  enabled: true
  conjunction: none
  disjunction: none
  implication: none
  activation: General
  rule: if in5 is small then out is outmf1
  rule: if in5 is medium then out is outmf2
  rule: if in5 is large then out is outmf3"

engine.fldFile = "slcp1.fld"
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

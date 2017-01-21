#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "slcp"
engine.fll = "Engine: slcp
InputVariable: in1
  enabled: true
  range: -0.300 0.300
  lock-range: false
  term: in1mf1 Bell -0.300 0.300 2.000
  term: in1mf2 Bell 0.300 0.300 2.000
InputVariable: in2
  enabled: true
  range: -1.000 1.000
  lock-range: false
  term: in2mf1 Bell -1.000 1.000 2.000
  term: in2mf2 Bell 1.000 1.000 2.000
InputVariable: in3
  enabled: true
  range: -3.000 3.000
  lock-range: false
  term: in3mf1 Bell -3.000 3.000 2.000
  term: in3mf2 Bell 3.000 3.000 2.000
InputVariable: in4
  enabled: true
  range: -3.000 3.000
  lock-range: false
  term: in4mf1 Bell -3.000 3.000 2.000
  term: in4mf2 Bell 3.000 3.000 2.000
OutputVariable: out
  enabled: true
  range: -10.000 10.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: outmf1 Linear 41.373 10.030 3.162 4.288 0.339
  term: outmf2 Linear 40.409 10.053 3.162 4.288 0.207
  term: outmf3 Linear 41.373 10.030 3.162 4.288 0.339
  term: outmf4 Linear 40.409 10.053 3.162 4.288 0.207
  term: outmf5 Linear 38.561 10.177 3.162 4.288 -0.049
  term: outmf6 Linear 37.596 10.154 3.162 4.288 -0.181
  term: outmf7 Linear 38.561 10.177 3.162 4.288 -0.049
  term: outmf8 Linear 37.596 10.154 3.162 4.288 -0.181
  term: outmf9 Linear 37.596 10.154 3.162 4.288 0.181
  term: outmf10 Linear 38.561 10.177 3.162 4.288 0.049
  term: outmf11 Linear 37.596 10.154 3.162 4.288 0.181
  term: outmf12 Linear 38.561 10.177 3.162 4.288 0.049
  term: outmf13 Linear 40.408 10.053 3.162 4.288 -0.207
  term: outmf14 Linear 41.373 10.030 3.162 4.288 -0.339
  term: outmf15 Linear 40.408 10.053 3.162 4.288 -0.207
  term: outmf16 Linear 41.373 10.030 3.162 4.288 -0.339
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

engine.fldFile = "slcp.fld"
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

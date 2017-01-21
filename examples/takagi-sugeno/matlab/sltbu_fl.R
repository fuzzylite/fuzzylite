#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "sltbu_fl"
engine.fll = "Engine: sltbu_fl
InputVariable: distance
  enabled: true
  range: 0.000 25.000
  lock-range: false
  term: near ZShape 1.000 2.000
  term: far SShape 1.000 2.000
InputVariable: control1
  enabled: true
  range: -0.785 0.785
  lock-range: false
InputVariable: control2
  enabled: true
  range: -0.785 0.785
  lock-range: false
OutputVariable: control
  enabled: true
  range: -0.785 0.785
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: out1mf1 Linear 0.000 0.000 1.000 0.000
  term: out1mf2 Linear 0.000 1.000 0.000 0.000
RuleBlock: 
  enabled: true
  conjunction: none
  disjunction: none
  implication: none
  activation: General
  rule: if distance is near then control is out1mf1
  rule: if distance is far then control is out1mf2"

engine.fldFile = "sltbu_fl.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(distance, control1)) + 
    geom_tile(aes(fill=control)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=distance, y=control1, z=control), color="black") + 
    ggtitle("(distance, control1) = control")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(control1, distance)) + 
    geom_tile(aes(fill=control)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=control1, y=distance, z=control), color="black") + 
    ggtitle("(control1, distance) = control")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

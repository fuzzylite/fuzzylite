#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "ObstacleAvoidance"
engine.fll = "Engine: ObstacleAvoidance
InputVariable: obstacle
  enabled: true
  range: 0.000 1.000
  lock-range: false
  term: left Ramp 1.000 0.000
  term: right Ramp 0.000 1.000
OutputVariable: mSteer
  enabled: true
  range: 0.000 1.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: Centroid 100
  default: nan
  lock-previous: false
  term: left Ramp 1.000 0.000
  term: right Ramp 0.000 1.000
OutputVariable: tsSteer
  enabled: true
  range: 0.000 1.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: WeightedAverage Automatic
  default: nan
  lock-previous: false
  term: left Constant 0.333
  term: right Constant 0.666
RuleBlock: mamdani
  enabled: true
  conjunction: none
  disjunction: none
  implication: AlgebraicProduct
  activation: General
  rule: if obstacle is left then mSteer is right
  rule: if obstacle is right then mSteer is left
RuleBlock: takagiSugeno
  enabled: true
  conjunction: none
  disjunction: none
  implication: none
  activation: General
  rule: if obstacle is left then tsSteer is right
  rule: if obstacle is right then tsSteer is left"

engine.fldFile = "ObstacleAvoidance.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1_o1 = ggplot(engine.df, aes(obstacle, mSteer)) + 
    geom_line(aes(color=mSteer), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    ggtitle("obstacle vs mSteer")

engine.plot.o1_i1 = ggplot(engine.df, aes(obstacle, mSteer)) + 
    geom_line(aes(color=mSteer), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    coord_flip() + 
    ggtitle("mSteer vs obstacle")

engine.plot.i1_o2 = ggplot(engine.df, aes(obstacle, tsSteer)) + 
    geom_line(aes(color=tsSteer), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    ggtitle("obstacle vs tsSteer")

engine.plot.o2_i1 = ggplot(engine.df, aes(obstacle, tsSteer)) + 
    geom_line(aes(color=tsSteer), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    coord_flip() + 
    ggtitle("tsSteer vs obstacle")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1_o1, engine.plot.o1_i1, engine.plot.i1_o2, engine.plot.o2_i1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

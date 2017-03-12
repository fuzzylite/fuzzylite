#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "tipper"
engine.description = "(service and food) -> (tip)"
engine.fll = "Engine: tipper
description: (service and food) -> (tip)
InputVariable: service
  description: quality of service
  enabled: true
  range: 0.000 10.000
  lock-range: true
  term: poor Trapezoid 0.000 0.000 2.500 5.000
  term: good Triangle 2.500 5.000 7.500
  term: excellent Trapezoid 5.000 7.500 10.000 10.000
InputVariable: food
  description: quality of food
  enabled: true
  range: 0.000 10.000
  lock-range: true
  term: rancid Trapezoid 0.000 0.000 2.500 7.500
  term: delicious Trapezoid 2.500 7.500 10.000 10.000
OutputVariable: mTip
  description: tip based on Mamdani inference
  enabled: true
  range: 0.000 30.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: Centroid 100
  default: nan
  lock-previous: false
  term: cheap Triangle 0.000 5.000 10.000
  term: average Triangle 10.000 15.000 20.000
  term: generous Triangle 20.000 25.000 30.000
OutputVariable: tsTip
  description: tip based on Takagi-Sugeno inference
  enabled: true
  range: 0.000 30.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: cheap Constant 5.000
  term: average Constant 15.000
  term: generous Constant 25.000
RuleBlock: mamdani
  description: Mamdani inference
  enabled: true
  conjunction: AlgebraicProduct
  disjunction: AlgebraicSum
  implication: Minimum
  activation: General
  rule: if service is poor or food is rancid then mTip is cheap
  rule: if service is good then mTip is average
  rule: if service is excellent or food is delicious then mTip is generous with 0.5
  rule: if service is excellent and food is delicious then mTip is generous with 1.0
RuleBlock: takagiSugeno
  description: Takagi-Sugeno inference
  enabled: true
  conjunction: AlgebraicProduct
  disjunction: AlgebraicSum
  implication: none
  activation: General
  rule: if service is poor or food is rancid then tsTip is cheap
  rule: if service is good then tsTip is average
  rule: if service is excellent or food is delicious then tsTip is generous with 0.5
  rule: if service is excellent and food is delicious then tsTip is generous with 1.0"

engine.fldFile = "tipper.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(service, food)) + 
    geom_tile(aes(fill=mTip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=service, y=food, z=mTip), color="black") + 
    ggtitle("(service, food) = mTip")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(food, service)) + 
    geom_tile(aes(fill=mTip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=food, y=service, z=mTip), color="black") + 
    ggtitle("(food, service) = mTip")

engine.plot.i1i2_o2 = ggplot(engine.df, aes(service, food)) + 
    geom_tile(aes(fill=tsTip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=service, y=food, z=tsTip), color="black") + 
    ggtitle("(service, food) = tsTip")

engine.plot.i2i1_o2 = ggplot(engine.df, aes(food, service)) + 
    geom_tile(aes(fill=tsTip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=food, y=service, z=tsTip), color="black") + 
    ggtitle("(food, service) = tsTip")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, engine.plot.i1i2_o2, engine.plot.i2i1_o2, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "tipper"
engine.fll = "Engine: tipper
InputVariable: service
  enabled: true
  range: 0.000 10.000
  lock-range: false
  term: poor Gaussian 0.000 1.500
  term: good Gaussian 5.000 1.500
  term: excellent Gaussian 10.000 1.500
InputVariable: food
  enabled: true
  range: 0.000 10.000
  lock-range: false
  term: rancid Trapezoid 0.000 0.000 1.000 3.000
  term: delicious Trapezoid 7.000 9.000 10.000 10.000
OutputVariable: tip
  enabled: true
  range: 0.000 30.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: Centroid 200
  default: nan
  lock-previous: false
  term: cheap Triangle 0.000 5.000 10.000
  term: average Triangle 10.000 15.000 20.000
  term: generous Triangle 20.000 25.000 30.000
RuleBlock: 
  enabled: true
  conjunction: Minimum
  disjunction: Maximum
  implication: Minimum
  activation: General
  rule: if service is poor or food is rancid then tip is cheap
  rule: if service is good then tip is average
  rule: if service is excellent or food is delicious then tip is generous"

engine.fldFile = "tipper.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(service, food)) + 
    geom_tile(aes(fill=tip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=service, y=food, z=tip), color="black") + 
    ggtitle("(service, food) = tip")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(food, service)) + 
    geom_tile(aes(fill=tip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=food, y=service, z=tip), color="black") + 
    ggtitle("(food, service) = tip")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

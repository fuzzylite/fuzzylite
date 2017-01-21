#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "tippersg"
engine.fll = "Engine: tippersg
InputVariable: service
  enabled: true
  range: 0.000 10.000
  lock-range: false
  term: poor Gaussian 0.000 1.500
  term: average Gaussian 5.000 1.500
  term: good Gaussian 10.000 1.500
InputVariable: food
  enabled: true
  range: 0.000 10.000
  lock-range: false
  term: rancid Trapezoid -5.000 0.000 1.000 3.000
  term: delicious Trapezoid 7.000 9.000 10.000 15.000
OutputVariable: tip
  enabled: true
  range: -30.000 30.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: cheap Linear 0.000 0.000 5.000
  term: average Linear 0.000 0.000 15.000
  term: generous Linear 0.000 0.000 25.000
RuleBlock: 
  enabled: true
  conjunction: none
  disjunction: Maximum
  implication: none
  activation: General
  rule: if service is poor or food is rancid then tip is cheap
  rule: if service is average then tip is average
  rule: if service is good or food is delicious then tip is generous"

engine.fldFile = "tippersg.fld"
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

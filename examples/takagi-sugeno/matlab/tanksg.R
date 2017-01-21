#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "tanksg"
engine.fll = "Engine: tanksg
InputVariable: level
  enabled: true
  range: -1.000 1.000
  lock-range: false
  term: high Gaussian -1.000 0.300
  term: okay Gaussian 0.004 0.300
  term: low Gaussian 1.000 0.300
InputVariable: rate
  enabled: true
  range: -0.100 0.100
  lock-range: false
  term: negative Gaussian -0.100 0.030
  term: none Gaussian 0.000 0.030
  term: positive Gaussian 0.100 0.030
OutputVariable: valve
  enabled: true
  range: -1.000 1.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: close_fast Constant -0.900
  term: close_slow Constant -0.500
  term: no_change Constant 0.000
  term: open_slow Constant 0.300
  term: open_fast Constant 0.900
RuleBlock: 
  enabled: true
  conjunction: AlgebraicProduct
  disjunction: none
  implication: none
  activation: General
  rule: if level is okay then valve is no_change
  rule: if level is low then valve is open_fast
  rule: if level is high then valve is close_fast
  rule: if level is okay and rate is positive then valve is close_slow
  rule: if level is okay and rate is negative then valve is open_slow"

engine.fldFile = "tanksg.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(level, rate)) + 
    geom_tile(aes(fill=valve)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=level, y=rate, z=valve), color="black") + 
    ggtitle("(level, rate) = valve")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(rate, level)) + 
    geom_tile(aes(fill=valve)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=rate, y=level, z=valve), color="black") + 
    ggtitle("(rate, level) = valve")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

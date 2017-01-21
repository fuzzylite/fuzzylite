#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "tank2"
engine.fll = "Engine: tank2
InputVariable: level
  enabled: true
  range: -1.000 1.000
  lock-range: false
  term: high Trapezoid -2.000 -1.000 -0.800 -0.001
  term: good Triangle -0.150 0.000 0.500
  term: low Trapezoid 0.001 0.800 1.000 1.500
InputVariable: change
  enabled: true
  range: -0.100 0.100
  lock-range: false
  term: falling Trapezoid -0.140 -0.100 -0.060 0.000
  term: rising Trapezoid -0.001 0.060 0.100 0.140
OutputVariable: valve
  enabled: true
  range: -1.000 1.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: Centroid 200
  default: nan
  lock-previous: false
  term: close_fast Triangle -1.000 -0.900 -0.800
  term: close_slow Triangle -0.600 -0.500 -0.400
  term: no_change Triangle -0.100 0.000 0.100
  term: open_slow Triangle 0.400 0.500 0.600
  term: open_fast Triangle 0.800 0.900 1.000
RuleBlock: 
  enabled: true
  conjunction: AlgebraicProduct
  disjunction: AlgebraicSum
  implication: AlgebraicProduct
  activation: General
  rule: if level is low then valve is open_fast
  rule: if level is high then valve is close_fast
  rule: if level is good and change is rising then valve is close_slow
  rule: if level is good and change is falling then valve is open_slow"

engine.fldFile = "tank2.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(level, change)) + 
    geom_tile(aes(fill=valve)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=level, y=change, z=valve), color="black") + 
    ggtitle("(level, change) = valve")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(change, level)) + 
    geom_tile(aes(fill=valve)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=change, y=level, z=valve), color="black") + 
    ggtitle("(change, level) = valve")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "Laundry"
engine.fll = "Engine: Laundry
InputVariable: Load
  enabled: true
  range: 0.000 6.000
  lock-range: false
  term: small Discrete 0.000 1.000 1.000 1.000 2.000 0.800 5.000 0.000
  term: normal Discrete 3.000 0.000 4.000 1.000 6.000 0.000
InputVariable: Dirt
  enabled: true
  range: 0.000 6.000
  lock-range: false
  term: low Discrete 0.000 1.000 2.000 0.800 5.000 0.000
  term: high Discrete 1.000 0.000 2.000 0.200 4.000 0.800 6.000 1.000
OutputVariable: Detergent
  enabled: true
  range: 0.000 80.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: MeanOfMaximum 500
  default: nan
  lock-previous: false
  term: less_than_usual Discrete 10.000 0.000 40.000 1.000 50.000 0.000
  term: usual Discrete 40.000 0.000 50.000 1.000 60.000 1.000 80.000 0.000
  term: more_than_usual Discrete 50.000 0.000 80.000 1.000
OutputVariable: Cycle
  enabled: true
  range: 0.000 20.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: MeanOfMaximum 500
  default: nan
  lock-previous: false
  term: short Discrete 0.000 1.000 10.000 1.000 20.000 0.000
  term: long Discrete 10.000 0.000 20.000 1.000
RuleBlock: 
  enabled: true
  conjunction: Minimum
  disjunction: Maximum
  implication: Minimum
  activation: General
  rule: if Load is small and Dirt is not high then Detergent is less_than_usual
  rule: if Load is small and Dirt is high then  Detergent is usual
  rule: if Load is normal and Dirt is low then Detergent is less_than_usual
  rule: if Load is normal and Dirt is high then Detergent is more_than_usual
  rule: if Detergent is usual  or Detergent is less_than_usual  then Cycle is short
  rule: if Detergent is more_than_usual  then Cycle is long"

engine.fldFile = "Laundry.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(Load, Dirt)) + 
    geom_tile(aes(fill=Detergent)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=Load, y=Dirt, z=Detergent), color="black") + 
    ggtitle("(Load, Dirt) = Detergent")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(Dirt, Load)) + 
    geom_tile(aes(fill=Detergent)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=Dirt, y=Load, z=Detergent), color="black") + 
    ggtitle("(Dirt, Load) = Detergent")

engine.plot.i1i2_o2 = ggplot(engine.df, aes(Load, Dirt)) + 
    geom_tile(aes(fill=Cycle)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=Load, y=Dirt, z=Cycle), color="black") + 
    ggtitle("(Load, Dirt) = Cycle")

engine.plot.i2i1_o2 = ggplot(engine.df, aes(Dirt, Load)) + 
    geom_tile(aes(fill=Cycle)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=Dirt, y=Load, z=Cycle), color="black") + 
    ggtitle("(Dirt, Load) = Cycle")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, engine.plot.i1i2_o2, engine.plot.i2i1_o2, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

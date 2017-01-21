#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "linear_tip_calculator"
engine.fll = "Engine: linear_tip_calculator
InputVariable: FoodQuality
  enabled: true
  range: 1.000 10.000
  lock-range: false
  term: Bad Trapezoid 0.000 1.000 3.000 7.000
  term: Good Trapezoid 3.000 7.000 10.000 11.000
InputVariable: Service
  enabled: true
  range: 1.000 10.000
  lock-range: false
  term: Bad Trapezoid 0.000 1.000 3.000 7.000
  term: Good Trapezoid 3.000 7.000 10.000 11.000
OutputVariable: Tip
  enabled: true
  range: 10.000 20.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: TenPercent Linear 0.000 0.000 10.000
  term: FifteenPercent Linear 0.000 0.000 15.000
  term: TwentyPercent Linear 0.000 0.000 20.000
RuleBlock: 
  enabled: true
  conjunction: Minimum
  disjunction: none
  implication: none
  activation: General
  rule: if FoodQuality is Bad and Service is Bad then Tip is TenPercent
  rule: if FoodQuality is Bad and Service is Good then Tip is FifteenPercent
  rule: if FoodQuality is Good and Service is Bad then Tip is FifteenPercent
  rule: if FoodQuality is Good and Service is Good then Tip is TwentyPercent"

engine.fldFile = "linear_tip_calculator.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(FoodQuality, Service)) + 
    geom_tile(aes(fill=Tip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=FoodQuality, y=Service, z=Tip), color="black") + 
    ggtitle("(FoodQuality, Service) = Tip")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(Service, FoodQuality)) + 
    geom_tile(aes(fill=Tip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=Service, y=FoodQuality, z=Tip), color="black") + 
    ggtitle("(Service, FoodQuality) = Tip")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

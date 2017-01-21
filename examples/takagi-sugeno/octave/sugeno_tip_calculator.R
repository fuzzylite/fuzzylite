#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "sugeno_tip_calculator"
engine.fll = "Engine: sugeno_tip_calculator
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
OutputVariable: CheapTip
  enabled: true
  range: 5.000 25.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: Low Constant 10.000
  term: Medium Constant 15.000
  term: High Constant 20.000
OutputVariable: AverageTip
  enabled: true
  range: 5.000 25.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: Low Constant 10.000
  term: Medium Constant 15.000
  term: High Constant 20.000
OutputVariable: GenerousTip
  enabled: true
  range: 5.000 25.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: Low Constant 10.000
  term: Medium Constant 15.000
  term: High Constant 20.000
RuleBlock: 
  enabled: true
  conjunction: EinsteinProduct
  disjunction: none
  implication: none
  activation: General
  rule: if FoodQuality is extremely Bad and Service is extremely Bad then CheapTip is extremely Low and AverageTip is very Low and GenerousTip is Low
  rule: if FoodQuality is Good and Service is extremely Bad then CheapTip is Low and AverageTip is Low and GenerousTip is Medium
  rule: if FoodQuality is very Good and Service is very Bad then CheapTip is Low and AverageTip is Medium and GenerousTip is High
  rule: if FoodQuality is Bad and Service is Bad then CheapTip is Low and AverageTip is Low and GenerousTip is Medium
  rule: if FoodQuality is Good and Service is Bad then CheapTip is Low and AverageTip is Medium and GenerousTip is High
  rule: if FoodQuality is extremely Good and Service is Bad then CheapTip is Low and AverageTip is Medium and GenerousTip is very High
  rule: if FoodQuality is Bad and Service is Good then CheapTip is Low and AverageTip is Medium and GenerousTip is High
  rule: if FoodQuality is Good and Service is Good then CheapTip is Medium and AverageTip is Medium and GenerousTip is very High
  rule: if FoodQuality is very Bad and Service is very Good then CheapTip is Low and AverageTip is Medium and GenerousTip is High
  rule: if FoodQuality is very very Good and Service is very very Good then CheapTip is High and AverageTip is very High and GenerousTip is extremely High"

engine.fldFile = "sugeno_tip_calculator.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(FoodQuality, Service)) + 
    geom_tile(aes(fill=CheapTip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=FoodQuality, y=Service, z=CheapTip), color="black") + 
    ggtitle("(FoodQuality, Service) = CheapTip")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(Service, FoodQuality)) + 
    geom_tile(aes(fill=CheapTip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=Service, y=FoodQuality, z=CheapTip), color="black") + 
    ggtitle("(Service, FoodQuality) = CheapTip")

engine.plot.i1i2_o2 = ggplot(engine.df, aes(FoodQuality, Service)) + 
    geom_tile(aes(fill=AverageTip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=FoodQuality, y=Service, z=AverageTip), color="black") + 
    ggtitle("(FoodQuality, Service) = AverageTip")

engine.plot.i2i1_o2 = ggplot(engine.df, aes(Service, FoodQuality)) + 
    geom_tile(aes(fill=AverageTip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=Service, y=FoodQuality, z=AverageTip), color="black") + 
    ggtitle("(Service, FoodQuality) = AverageTip")

engine.plot.i1i2_o3 = ggplot(engine.df, aes(FoodQuality, Service)) + 
    geom_tile(aes(fill=GenerousTip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=FoodQuality, y=Service, z=GenerousTip), color="black") + 
    ggtitle("(FoodQuality, Service) = GenerousTip")

engine.plot.i2i1_o3 = ggplot(engine.df, aes(Service, FoodQuality)) + 
    geom_tile(aes(fill=GenerousTip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=Service, y=FoodQuality, z=GenerousTip), color="black") + 
    ggtitle("(Service, FoodQuality) = GenerousTip")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, engine.plot.i1i2_o2, engine.plot.i2i1_o2, engine.plot.i1i2_o3, engine.plot.i2i1_o3, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

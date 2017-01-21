#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "mamdani_tip_calculator"
engine.fll = "Engine: mamdani_tip_calculator
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
  range: 0.000 30.000
  lock-range: false
  aggregation: AlgebraicSum
  defuzzifier: Centroid 200
  default: nan
  lock-previous: false
  term: AboutTenPercent Gaussian 10.000 2.000
  term: AboutFifteenPercent Gaussian 15.000 2.000
  term: AboutTwentyPercent Gaussian 20.000 2.000
OutputVariable: CheckPlusTip
  enabled: true
  range: 1.000 1.300
  lock-range: false
  aggregation: AlgebraicSum
  defuzzifier: Centroid 200
  default: nan
  lock-previous: false
  term: PlusAboutTenPercent Gaussian 1.100 0.020
  term: PlusAboutFifteenPercent Gaussian 1.150 0.020
  term: PlusAboutTwentyPercent Gaussian 1.200 0.020
RuleBlock: 
  enabled: true
  conjunction: AlgebraicProduct
  disjunction: Maximum
  implication: Minimum
  activation: General
  rule: if FoodQuality is Bad and Service is Bad then Tip is AboutTenPercent and CheckPlusTip is PlusAboutTenPercent
  rule: if FoodQuality is Bad and Service is Good then Tip is AboutFifteenPercent and CheckPlusTip is PlusAboutFifteenPercent
  rule: if FoodQuality is Good and Service is Bad then Tip is AboutFifteenPercent and CheckPlusTip is PlusAboutFifteenPercent
  rule: if FoodQuality is Good and Service is Good then Tip is AboutTwentyPercent and CheckPlusTip is PlusAboutTwentyPercent"

engine.fldFile = "mamdani_tip_calculator.fld"
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

engine.plot.i1i2_o2 = ggplot(engine.df, aes(FoodQuality, Service)) + 
    geom_tile(aes(fill=CheckPlusTip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=FoodQuality, y=Service, z=CheckPlusTip), color="black") + 
    ggtitle("(FoodQuality, Service) = CheckPlusTip")

engine.plot.i2i1_o2 = ggplot(engine.df, aes(Service, FoodQuality)) + 
    geom_tile(aes(fill=CheckPlusTip)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=Service, y=FoodQuality, z=CheckPlusTip), color="black") + 
    ggtitle("(Service, FoodQuality) = CheckPlusTip")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, engine.plot.i1i2_o2, engine.plot.i2i1_o2, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "investment_portfolio"
engine.fll = "Engine: investment_portfolio
InputVariable: Age
  enabled: true
  range: 20.000 100.000
  lock-range: false
  term: Young ZShape 30.000 90.000
  term: Old SShape 30.000 90.000
InputVariable: RiskTolerance
  enabled: true
  range: 0.000 10.000
  lock-range: false
  term: Low ZShape 2.000 8.000
  term: High SShape 2.000 8.000
OutputVariable: PercentageInStocks
  enabled: true
  range: 0.000 100.000
  lock-range: false
  aggregation: EinsteinSum
  defuzzifier: Centroid 200
  default: nan
  lock-previous: false
  term: AboutFifteen Gaussian 15.000 10.000
  term: AboutFifty Gaussian 50.000 10.000
  term: AboutEightyFive Gaussian 85.000 10.000
RuleBlock: 
  enabled: true
  conjunction: EinsteinProduct
  disjunction: EinsteinSum
  implication: EinsteinProduct
  activation: General
  rule: if Age is Young or RiskTolerance is High then PercentageInStocks is AboutEightyFive
  rule: if Age is Old or RiskTolerance is Low then PercentageInStocks is AboutFifteen
  rule: if Age is not extremely Old and RiskTolerance is not extremely Low then PercentageInStocks is AboutFifty with 0.500
  rule: if Age is not extremely Young and RiskTolerance is not extremely High then PercentageInStocks is AboutFifty with 0.500"

engine.fldFile = "investment_portfolio.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(Age, RiskTolerance)) + 
    geom_tile(aes(fill=PercentageInStocks)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=Age, y=RiskTolerance, z=PercentageInStocks), color="black") + 
    ggtitle("(Age, RiskTolerance) = PercentageInStocks")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(RiskTolerance, Age)) + 
    geom_tile(aes(fill=PercentageInStocks)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=RiskTolerance, y=Age, z=PercentageInStocks), color="black") + 
    ggtitle("(RiskTolerance, Age) = PercentageInStocks")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "heart_disease_risk"
engine.fll = "Engine: heart_disease_risk
InputVariable: LDLLevel
  enabled: true
  range: 0.000 300.000
  lock-range: false
  term: Low Trapezoid -1.000 0.000 90.000 110.000
  term: LowBorderline Trapezoid 90.000 110.000 120.000 140.000
  term: Borderline Trapezoid 120.000 140.000 150.000 170.000
  term: HighBorderline Trapezoid 150.000 170.000 180.000 200.000
  term: High Trapezoid 180.000 200.000 300.000 301.000
InputVariable: HDLLevel
  enabled: true
  range: 0.000 100.000
  lock-range: false
  term: LowHDL Trapezoid -1.000 0.000 35.000 45.000
  term: ModerateHDL Trapezoid 35.000 45.000 55.000 65.000
  term: HighHDL Trapezoid 55.000 65.000 100.000 101.000
OutputVariable: HeartDiseaseRisk
  enabled: true
  range: 0.000 10.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: NoRisk Constant 0.000
  term: LowRisk Constant 2.500
  term: MediumRisk Constant 5.000
  term: HighRisk Constant 7.500
  term: ExtremeRisk Constant 10.000
RuleBlock: 
  enabled: true
  conjunction: Minimum
  disjunction: none
  implication: none
  activation: General
  rule: if LDLLevel is Low and HDLLevel is LowHDL then HeartDiseaseRisk is MediumRisk
  rule: if LDLLevel is Low and HDLLevel is ModerateHDL then HeartDiseaseRisk is LowRisk
  rule: if LDLLevel is Low and HDLLevel is HighHDL then HeartDiseaseRisk is NoRisk
  rule: if LDLLevel is LowBorderline and HDLLevel is LowHDL then HeartDiseaseRisk is MediumRisk
  rule: if LDLLevel is LowBorderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is LowRisk
  rule: if LDLLevel is LowBorderline and HDLLevel is HighHDL then HeartDiseaseRisk is LowRisk
  rule: if LDLLevel is Borderline and HDLLevel is LowHDL then HeartDiseaseRisk is HighRisk
  rule: if LDLLevel is Borderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is MediumRisk
  rule: if LDLLevel is Borderline and HDLLevel is HighHDL then HeartDiseaseRisk is LowRisk
  rule: if LDLLevel is HighBorderline and HDLLevel is LowHDL then HeartDiseaseRisk is HighRisk
  rule: if LDLLevel is HighBorderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is HighRisk
  rule: if LDLLevel is HighBorderline and HDLLevel is HighHDL then HeartDiseaseRisk is MediumRisk
  rule: if LDLLevel is High and HDLLevel is LowHDL then HeartDiseaseRisk is ExtremeRisk
  rule: if LDLLevel is High and HDLLevel is ModerateHDL then HeartDiseaseRisk is HighRisk
  rule: if LDLLevel is High and HDLLevel is HighHDL then HeartDiseaseRisk is MediumRisk"

engine.fldFile = "heart_disease_risk.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(LDLLevel, HDLLevel)) + 
    geom_tile(aes(fill=HeartDiseaseRisk)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=LDLLevel, y=HDLLevel, z=HeartDiseaseRisk), color="black") + 
    ggtitle("(LDLLevel, HDLLevel) = HeartDiseaseRisk")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(HDLLevel, LDLLevel)) + 
    geom_tile(aes(fill=HeartDiseaseRisk)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=HDLLevel, y=LDLLevel, z=HeartDiseaseRisk), color="black") + 
    ggtitle("(HDLLevel, LDLLevel) = HeartDiseaseRisk")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "juggler"
engine.fll = "Engine: juggler
InputVariable: xHit
  enabled: true
  range: -4.000 4.000
  lock-range: false
  term: in1mf1 Bell -4.000 2.000 4.000
  term: in1mf2 Bell 0.000 2.000 4.000
  term: in1mf3 Bell 4.000 2.000 4.000
InputVariable: projectAngle
  enabled: true
  range: 0.000 3.142
  lock-range: false
  term: in2mf1 Bell 0.000 0.785 4.000
  term: in2mf2 Bell 1.571 0.785 4.000
  term: in2mf3 Bell 3.142 0.785 4.000
OutputVariable: theta
  enabled: true
  range: 0.000 0.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: out1mf Linear -0.022 -0.500 0.315
  term: out1mf Linear -0.022 -0.500 0.315
  term: out1mf Linear -0.022 -0.500 0.315
  term: out1mf Linear 0.114 -0.500 0.785
  term: out1mf Linear 0.114 -0.500 0.785
  term: out1mf Linear 0.114 -0.500 0.785
  term: out1mf Linear -0.022 -0.500 1.256
  term: out1mf Linear -0.022 -0.500 1.256
  term: out1mf Linear -0.022 -0.500 1.256
RuleBlock: 
  enabled: true
  conjunction: AlgebraicProduct
  disjunction: none
  implication: none
  activation: General
  rule: if xHit is in1mf1 and projectAngle is in2mf1 then theta is out1mf
  rule: if xHit is in1mf1 and projectAngle is in2mf2 then theta is out1mf
  rule: if xHit is in1mf1 and projectAngle is in2mf3 then theta is out1mf
  rule: if xHit is in1mf2 and projectAngle is in2mf1 then theta is out1mf
  rule: if xHit is in1mf2 and projectAngle is in2mf2 then theta is out1mf
  rule: if xHit is in1mf2 and projectAngle is in2mf3 then theta is out1mf
  rule: if xHit is in1mf3 and projectAngle is in2mf1 then theta is out1mf
  rule: if xHit is in1mf3 and projectAngle is in2mf2 then theta is out1mf
  rule: if xHit is in1mf3 and projectAngle is in2mf3 then theta is out1mf"

engine.fldFile = "juggler.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(xHit, projectAngle)) + 
    geom_tile(aes(fill=theta)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=xHit, y=projectAngle, z=theta), color="black") + 
    ggtitle("(xHit, projectAngle) = theta")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(projectAngle, xHit)) + 
    geom_tile(aes(fill=theta)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=projectAngle, y=xHit, z=theta), color="black") + 
    ggtitle("(projectAngle, xHit) = theta")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

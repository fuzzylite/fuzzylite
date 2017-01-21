#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "mam22"
engine.fll = "Engine: mam22
InputVariable: angle
  enabled: true
  range: -5.000 5.000
  lock-range: false
  term: small Bell -5.000 5.000 8.000
  term: big Bell 5.000 5.000 8.000
InputVariable: velocity
  enabled: true
  range: -5.000 5.000
  lock-range: false
  term: small Bell -5.000 5.000 2.000
  term: big Bell 5.000 5.000 2.000
OutputVariable: force
  enabled: true
  range: -5.000 5.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: Centroid 200
  default: nan
  lock-previous: false
  term: negBig Bell -5.000 1.670 8.000
  term: negSmall Bell -1.670 1.670 8.000
  term: posSmall Bell 1.670 1.670 8.000
  term: posBig Bell 5.000 1.670 8.000
OutputVariable: force2
  enabled: true
  range: -5.000 5.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: Centroid 200
  default: nan
  lock-previous: false
  term: negBig2 Bell -3.000 1.670 8.000
  term: negSmall2 Bell -1.000 1.670 8.000
  term: posSmall2 Bell 1.000 1.670 8.000
  term: posBig2 Bell 3.000 1.670 8.000
RuleBlock: 
  enabled: true
  conjunction: Minimum
  disjunction: Maximum
  implication: Minimum
  activation: General
  rule: if angle is small and velocity is small then force is negBig and force2 is posBig2
  rule: if angle is small and velocity is big then force is negSmall and force2 is posSmall2
  rule: if angle is big and velocity is small then force is posSmall and force2 is negSmall2
  rule: if angle is big and velocity is big then force is posBig and force2 is negBig2"

engine.fldFile = "mam22.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(angle, velocity)) + 
    geom_tile(aes(fill=force)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=angle, y=velocity, z=force), color="black") + 
    ggtitle("(angle, velocity) = force")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(velocity, angle)) + 
    geom_tile(aes(fill=force)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=velocity, y=angle, z=force), color="black") + 
    ggtitle("(velocity, angle) = force")

engine.plot.i1i2_o2 = ggplot(engine.df, aes(angle, velocity)) + 
    geom_tile(aes(fill=force2)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=angle, y=velocity, z=force2), color="black") + 
    ggtitle("(angle, velocity) = force2")

engine.plot.i2i1_o2 = ggplot(engine.df, aes(velocity, angle)) + 
    geom_tile(aes(fill=force2)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=velocity, y=angle, z=force2), color="black") + 
    ggtitle("(velocity, angle) = force2")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, engine.plot.i1i2_o2, engine.plot.i2i1_o2, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "tsukamoto"
engine.fll = "Engine: tsukamoto
InputVariable: X
  enabled: true
  range: -10.000 10.000
  lock-range: false
  term: small Bell -10.000 5.000 3.000
  term: medium Bell 0.000 5.000 3.000
  term: large Bell 10.000 5.000 3.000
OutputVariable: Ramps
  enabled: true
  range: 0.000 1.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage Automatic
  default: nan
  lock-previous: false
  term: b Ramp 0.600 0.400
  term: a Ramp 0.000 0.250
  term: c Ramp 0.700 1.000
OutputVariable: Sigmoids
  enabled: true
  range: 0.020 1.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage Automatic
  default: nan
  lock-previous: false
  term: b Sigmoid 0.500 -30.000
  term: a Sigmoid 0.130 30.000
  term: c Sigmoid 0.830 30.000
OutputVariable: ZSShapes
  enabled: true
  range: 0.000 1.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage Automatic
  default: nan
  lock-previous: false
  term: b ZShape 0.300 0.600
  term: a SShape 0.000 0.250
  term: c SShape 0.700 1.000
OutputVariable: Concaves
  enabled: true
  range: 0.000 1.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage Automatic
  default: nan
  lock-previous: false
  term: b Concave 0.500 0.400
  term: a Concave 0.240 0.250
  term: c Concave 0.900 1.000
RuleBlock: 
  enabled: true
  conjunction: none
  disjunction: none
  implication: none
  activation: General
  rule: if X is small then Ramps is a and Sigmoids is a and ZSShapes is a and Concaves is a
  rule: if X is medium then Ramps is b and Sigmoids is b and ZSShapes is b and Concaves is b
  rule: if X is large then Ramps is c and Sigmoids is c and ZSShapes is c and Concaves is c"

engine.fldFile = "tsukamoto.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1_o1 = ggplot(engine.df, aes(X, Ramps)) + 
    geom_line(aes(color=Ramps), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    ggtitle("X vs Ramps")

engine.plot.o1_i1 = ggplot(engine.df, aes(X, Ramps)) + 
    geom_line(aes(color=Ramps), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    coord_flip() + 
    ggtitle("Ramps vs X")

engine.plot.i1_o2 = ggplot(engine.df, aes(X, Sigmoids)) + 
    geom_line(aes(color=Sigmoids), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    ggtitle("X vs Sigmoids")

engine.plot.o2_i1 = ggplot(engine.df, aes(X, Sigmoids)) + 
    geom_line(aes(color=Sigmoids), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    coord_flip() + 
    ggtitle("Sigmoids vs X")

engine.plot.i1_o3 = ggplot(engine.df, aes(X, ZSShapes)) + 
    geom_line(aes(color=ZSShapes), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    ggtitle("X vs ZSShapes")

engine.plot.o3_i1 = ggplot(engine.df, aes(X, ZSShapes)) + 
    geom_line(aes(color=ZSShapes), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    coord_flip() + 
    ggtitle("ZSShapes vs X")

engine.plot.i1_o4 = ggplot(engine.df, aes(X, Concaves)) + 
    geom_line(aes(color=Concaves), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    ggtitle("X vs Concaves")

engine.plot.o4_i1 = ggplot(engine.df, aes(X, Concaves)) + 
    geom_line(aes(color=Concaves), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    coord_flip() + 
    ggtitle("Concaves vs X")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1_o1, engine.plot.o1_i1, engine.plot.i1_o2, engine.plot.o2_i1, engine.plot.i1_o3, engine.plot.o3_i1, engine.plot.i1_o4, engine.plot.o4_i1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

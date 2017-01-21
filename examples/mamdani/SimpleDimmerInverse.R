#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "SimpleDimmerInverse"
engine.fll = "Engine: SimpleDimmerInverse
InputVariable: Ambient
  enabled: true
  range: 0.000 1.000
  lock-range: false
  term: DARK Triangle 0.000 0.250 0.500
  term: MEDIUM Triangle 0.250 0.500 0.750
  term: BRIGHT Triangle 0.500 0.750 1.000
OutputVariable: Power
  enabled: true
  range: 0.000 1.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: Centroid 200
  default: nan
  lock-previous: false
  term: LOW Triangle 0.000 0.250 0.500
  term: MEDIUM Triangle 0.250 0.500 0.750
  term: HIGH Triangle 0.500 0.750 1.000
OutputVariable: InversePower
  enabled: true
  range: 0.000 1.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: Centroid 500
  default: nan
  lock-previous: false
  term: LOW Cosine 0.200 0.500
  term: MEDIUM Cosine 0.500 0.500
  term: HIGH Cosine 0.800 0.500
RuleBlock: 
  enabled: true
  conjunction: none
  disjunction: none
  implication: Minimum
  activation: General
  rule: if Ambient is DARK then Power is HIGH
  rule: if Ambient is MEDIUM then Power is MEDIUM
  rule: if Ambient is BRIGHT then Power is LOW
  rule: if Power is LOW then InversePower is HIGH
  rule: if Power is MEDIUM then InversePower is MEDIUM
  rule: if Power is HIGH then InversePower is LOW"

engine.fldFile = "SimpleDimmerInverse.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1_o1 = ggplot(engine.df, aes(Ambient, Power)) + 
    geom_line(aes(color=Power), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    ggtitle("Ambient vs Power")

engine.plot.o1_i1 = ggplot(engine.df, aes(Ambient, Power)) + 
    geom_line(aes(color=Power), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    coord_flip() + 
    ggtitle("Power vs Ambient")

engine.plot.i1_o2 = ggplot(engine.df, aes(Ambient, InversePower)) + 
    geom_line(aes(color=InversePower), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    ggtitle("Ambient vs InversePower")

engine.plot.o2_i1 = ggplot(engine.df, aes(Ambient, InversePower)) + 
    geom_line(aes(color=InversePower), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    coord_flip() + 
    ggtitle("InversePower vs Ambient")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1_o1, engine.plot.o1_i1, engine.plot.i1_o2, engine.plot.o2_i1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

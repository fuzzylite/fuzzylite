#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "sugeno1"
engine.fll = "Engine: sugeno1
InputVariable: input
  enabled: true
  range: -5.000 5.000
  lock-range: false
  term: low Gaussian -5.000 4.000
  term: high Gaussian 5.000 4.000
OutputVariable: output
  enabled: true
  range: 0.000 1.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: line1 Linear -1.000 -1.000
  term: line2 Linear 1.000 -1.000
RuleBlock: 
  enabled: true
  conjunction: none
  disjunction: none
  implication: none
  activation: General
  rule: if input is low then output is line1
  rule: if input is high then output is line2"

engine.fldFile = "sugeno1.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1_o1 = ggplot(engine.df, aes(input, output)) + 
    geom_line(aes(color=output), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    ggtitle("input vs output")

engine.plot.o1_i1 = ggplot(engine.df, aes(input, output)) + 
    geom_line(aes(color=output), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    coord_flip() + 
    ggtitle("output vs input")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1_o1, engine.plot.o1_i1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

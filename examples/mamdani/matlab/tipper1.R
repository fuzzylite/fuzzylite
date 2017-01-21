#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "tipper1"
engine.fll = "Engine: tipper1
InputVariable: service
  enabled: true
  range: 0.000 10.000
  lock-range: false
  term: poor Gaussian 0.000 1.500
  term: good Gaussian 5.000 1.500
  term: excellent Gaussian 10.000 1.500
OutputVariable: tip
  enabled: true
  range: 0.000 30.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: Centroid 200
  default: nan
  lock-previous: false
  term: cheap Triangle 0.000 5.000 10.000
  term: average Triangle 10.000 15.000 20.000
  term: generous Triangle 20.000 25.000 30.000
RuleBlock: 
  enabled: true
  conjunction: Minimum
  disjunction: Maximum
  implication: Minimum
  activation: General
  rule: if service is poor then tip is cheap
  rule: if service is good then tip is average
  rule: if service is excellent then tip is generous"

engine.fldFile = "tipper1.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1_o1 = ggplot(engine.df, aes(service, tip)) + 
    geom_line(aes(color=tip), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    ggtitle("service vs tip")

engine.plot.o1_i1 = ggplot(engine.df, aes(service, tip)) + 
    geom_line(aes(color=tip), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    coord_flip() + 
    ggtitle("tip vs service")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1_o1, engine.plot.o1_i1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

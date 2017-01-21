#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "shower"
engine.fll = "Engine: shower
InputVariable: temp
  enabled: true
  range: -20.000 20.000
  lock-range: false
  term: cold Trapezoid -30.000 -30.000 -15.000 0.000
  term: good Triangle -10.000 0.000 10.000
  term: hot Trapezoid 0.000 15.000 30.000 30.000
InputVariable: flow
  enabled: true
  range: -1.000 1.000
  lock-range: false
  term: soft Trapezoid -3.000 -3.000 -0.800 0.000
  term: good Triangle -0.400 0.000 0.400
  term: hard Trapezoid 0.000 0.800 3.000 3.000
OutputVariable: cold
  enabled: true
  range: -1.000 1.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: Centroid 200
  default: nan
  lock-previous: false
  term: closeFast Triangle -1.000 -0.600 -0.300
  term: closeSlow Triangle -0.600 -0.300 0.000
  term: steady Triangle -0.300 0.000 0.300
  term: openSlow Triangle 0.000 0.300 0.600
  term: openFast Triangle 0.300 0.600 1.000
OutputVariable: hot
  enabled: true
  range: -1.000 1.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: Centroid 200
  default: nan
  lock-previous: false
  term: closeFast Triangle -1.000 -0.600 -0.300
  term: closeSlow Triangle -0.600 -0.300 0.000
  term: steady Triangle -0.300 0.000 0.300
  term: openSlow Triangle 0.000 0.300 0.600
  term: openFast Triangle 0.300 0.600 1.000
RuleBlock: 
  enabled: true
  conjunction: Minimum
  disjunction: Maximum
  implication: Minimum
  activation: General
  rule: if temp is cold and flow is soft then cold is openSlow and hot is openFast
  rule: if temp is cold and flow is good then cold is closeSlow and hot is openSlow
  rule: if temp is cold and flow is hard then cold is closeFast and hot is closeSlow
  rule: if temp is good and flow is soft then cold is openSlow and hot is openSlow
  rule: if temp is good and flow is good then cold is steady and hot is steady
  rule: if temp is good and flow is hard then cold is closeSlow and hot is closeSlow
  rule: if temp is hot and flow is soft then cold is openFast and hot is openSlow
  rule: if temp is hot and flow is good then cold is openSlow and hot is closeSlow
  rule: if temp is hot and flow is hard then cold is closeSlow and hot is closeFast"

engine.fldFile = "shower.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1i2_o1 = ggplot(engine.df, aes(temp, flow)) + 
    geom_tile(aes(fill=cold)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=temp, y=flow, z=cold), color="black") + 
    ggtitle("(temp, flow) = cold")

engine.plot.i2i1_o1 = ggplot(engine.df, aes(flow, temp)) + 
    geom_tile(aes(fill=cold)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=flow, y=temp, z=cold), color="black") + 
    ggtitle("(flow, temp) = cold")

engine.plot.i1i2_o2 = ggplot(engine.df, aes(temp, flow)) + 
    geom_tile(aes(fill=hot)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=temp, y=flow, z=hot), color="black") + 
    ggtitle("(temp, flow) = hot")

engine.plot.i2i1_o2 = ggplot(engine.df, aes(flow, temp)) + 
    geom_tile(aes(fill=hot)) + 
    scale_fill_gradient(low="yellow", high="red") + 
    stat_contour(aes(x=flow, y=temp, z=hot), color="black") + 
    ggtitle("(flow, temp) = hot")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1i2_o1, engine.plot.i2i1_o1, engine.plot.i1i2_o2, engine.plot.i2i1_o2, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

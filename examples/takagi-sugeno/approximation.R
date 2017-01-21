#Code automatically generated with fuzzylite 6.0.

library(ggplot2);

engine.name = "approximation"
engine.fll = "Engine: approximation
InputVariable: inputX
  enabled: true
  range: 0.000 10.000
  lock-range: false
  term: NEAR_1 Triangle 0.000 1.000 2.000
  term: NEAR_2 Triangle 1.000 2.000 3.000
  term: NEAR_3 Triangle 2.000 3.000 4.000
  term: NEAR_4 Triangle 3.000 4.000 5.000
  term: NEAR_5 Triangle 4.000 5.000 6.000
  term: NEAR_6 Triangle 5.000 6.000 7.000
  term: NEAR_7 Triangle 6.000 7.000 8.000
  term: NEAR_8 Triangle 7.000 8.000 9.000
  term: NEAR_9 Triangle 8.000 9.000 10.000
OutputVariable: outputFx
  enabled: true
  range: -1.000 1.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: true
  term: f1 Constant 0.840
  term: f2 Constant 0.450
  term: f3 Constant 0.040
  term: f4 Constant -0.180
  term: f5 Constant -0.190
  term: f6 Constant -0.040
  term: f7 Constant 0.090
  term: f8 Constant 0.120
  term: f9 Constant 0.040
OutputVariable: trueFx
  enabled: true
  range: -1.000 1.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage Automatic
  default: nan
  lock-previous: true
  term: fx Function sin(inputX)/inputX
OutputVariable: diffFx
  enabled: true
  range: -1.000 1.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage Automatic
  default: nan
  lock-previous: false
  term: diff Function fabs(outputFx-trueFx)
RuleBlock: 
  enabled: true
  conjunction: none
  disjunction: none
  implication: none
  activation: General
  rule: if inputX is NEAR_1 then outputFx is f1
  rule: if inputX is NEAR_2 then outputFx is f2
  rule: if inputX is NEAR_3 then outputFx is f3
  rule: if inputX is NEAR_4 then outputFx is f4
  rule: if inputX is NEAR_5 then outputFx is f5
  rule: if inputX is NEAR_6 then outputFx is f6
  rule: if inputX is NEAR_7 then outputFx is f7
  rule: if inputX is NEAR_8 then outputFx is f8
  rule: if inputX is NEAR_9 then outputFx is f9
  rule: if inputX is any  then trueFx is fx and diffFx is diff"

engine.fldFile = "approximation.fld"
if (require(data.table)) {
    engine.df = data.table::fread(engine.fldFile, sep="auto", header="auto")
} else {
    engine.df = read.table(engine.fldFile, header=TRUE)
}

engine.plot.i1_o1 = ggplot(engine.df, aes(inputX, outputFx)) + 
    geom_line(aes(color=outputFx), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    ggtitle("inputX vs outputFx")

engine.plot.o1_i1 = ggplot(engine.df, aes(inputX, outputFx)) + 
    geom_line(aes(color=outputFx), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    coord_flip() + 
    ggtitle("outputFx vs inputX")

engine.plot.i1_o2 = ggplot(engine.df, aes(inputX, trueFx)) + 
    geom_line(aes(color=trueFx), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    ggtitle("inputX vs trueFx")

engine.plot.o2_i1 = ggplot(engine.df, aes(inputX, trueFx)) + 
    geom_line(aes(color=trueFx), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    coord_flip() + 
    ggtitle("trueFx vs inputX")

engine.plot.i1_o3 = ggplot(engine.df, aes(inputX, diffFx)) + 
    geom_line(aes(color=diffFx), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    ggtitle("inputX vs diffFx")

engine.plot.o3_i1 = ggplot(engine.df, aes(inputX, diffFx)) + 
    geom_line(aes(color=diffFx), size=3, lineend="round", linejoin="mitre") + 
    scale_color_gradient(low="yellow", high="red") + 
    coord_flip() + 
    ggtitle("diffFx vs inputX")

if (require(gridExtra)) {
    engine.plots = arrangeGrob(engine.plot.i1_o1, engine.plot.o1_i1, engine.plot.i1_o2, engine.plot.o2_i1, engine.plot.i1_o3, engine.plot.o3_i1, ncol=2, top=engine.name)
    ggsave(paste0(engine.name, ".pdf"), engine.plots)
    if (require(grid)) {
        grid.newpage()
        grid.draw(engine.plots)
    }
}

import com.fuzzylite.*;
import com.fuzzylite.activation.*
import com.fuzzylite.defuzzifier.*;
import com.fuzzylite.factory.*;
import com.fuzzylite.hedge.*;
import com.fuzzylite.imex.*;
import com.fuzzylite.norm.*;
import com.fuzzylite.norm.s.*;
import com.fuzzylite.norm.t.*;
import com.fuzzylite.rule.*;
import com.fuzzylite.term.*;
import com.fuzzylite.variable.*;

public class tipper{
public static void main(String[] args){
//Code automatically generated with fuzzylite 6.0.

Engine engine = new Engine();
engine.setName("tipper");

InputVariable service = new InputVariable();
service.setName("service");
service.setDescription("");
service.setEnabled(true);
service.setRange(0.000, 10.000);
service.setLockValueInRange(false);
service.addTerm(new Trapezoid("poor", 0.000, 0.000, 2.500, 5.000));
service.addTerm(new Triangle("good", 2.500, 5.000, 7.500));
service.addTerm(new Trapezoid("excellent", 5.000, 7.500, 10.000, 10.000));
engine.addInputVariable(service);

InputVariable food = new InputVariable();
food.setName("food");
food.setDescription("");
food.setEnabled(true);
food.setRange(0.000, 10.000);
food.setLockValueInRange(true);
food.addTerm(new Trapezoid("rancid", 0.000, 0.000, 2.500, 7.500));
food.addTerm(new Trapezoid("delicious", 2.500, 7.500, 10.000, 10.000));
engine.addInputVariable(food);

OutputVariable mTip = new OutputVariable();
mTip.setName("mTip");
mTip.setDescription("");
mTip.setEnabled(true);
mTip.setRange(0.000, 30.000);
mTip.setLockValueInRange(false);
mTip.setAggregation(new Maximum());
mTip.setDefuzzifier(new Centroid(100));
mTip.setDefaultValue(Double.NaN);
mTip.setLockPreviousValue(false);
mTip.addTerm(new Triangle("cheap", 0.000, 5.000, 10.000));
mTip.addTerm(new Triangle("average", 10.000, 15.000, 20.000));
mTip.addTerm(new Triangle("generous", 20.000, 25.000, 30.000));
engine.addOutputVariable(mTip);

OutputVariable tsTip = new OutputVariable();
tsTip.setName("tsTip");
tsTip.setDescription("");
tsTip.setEnabled(true);
tsTip.setRange(0.000, 30.000);
tsTip.setLockValueInRange(false);
tsTip.setAggregation(null);
tsTip.setDefuzzifier(new WeightedAverage("TakagiSugeno"));
tsTip.setDefaultValue(Double.NaN);
tsTip.setLockPreviousValue(false);
tsTip.addTerm(new Constant("cheap", 5.000));
tsTip.addTerm(new Constant("average", 15.000));
tsTip.addTerm(new Constant("generous", 25.000));
engine.addOutputVariable(tsTip);

RuleBlock mamdaniRuleBlock = new RuleBlock();
mamdaniRuleBlock.setName("mamdaniRuleBlock");
mamdaniRuleBlock.setDescription("");
mamdaniRuleBlock.setEnabled(true);
mamdaniRuleBlock.setConjunction(new AlgebraicProduct());
mamdaniRuleBlock.setDisjunction(new AlgebraicSum());
mamdaniRuleBlock.setImplication(new Minimum());
mamdaniRuleBlock.setActivation(new General());
mamdaniRuleBlock.addRule(Rule.parse("if service is poor or food is rancid then mTip is cheap", engine));
mamdaniRuleBlock.addRule(Rule.parse("if service is good then mTip is average", engine));
mamdaniRuleBlock.addRule(Rule.parse("if service is excellent or food is delicious then mTip is generous with 0.5", engine));
mamdaniRuleBlock.addRule(Rule.parse("if service is excellent and food is delicious then mTip is generous with 1.0", engine));
engine.addRuleBlock(mamdaniRuleBlock);

RuleBlock takagiSugenoRuleBlock = new RuleBlock();
takagiSugenoRuleBlock.setName("takagiSugenoRuleBlock");
takagiSugenoRuleBlock.setDescription("");
takagiSugenoRuleBlock.setEnabled(true);
takagiSugenoRuleBlock.setConjunction(new AlgebraicProduct());
takagiSugenoRuleBlock.setDisjunction(new AlgebraicSum());
takagiSugenoRuleBlock.setImplication(null);
takagiSugenoRuleBlock.setActivation(new General());
takagiSugenoRuleBlock.addRule(Rule.parse("if service is poor or food is rancid then tsTip is cheap", engine));
takagiSugenoRuleBlock.addRule(Rule.parse("if service is good then tsTip is average", engine));
takagiSugenoRuleBlock.addRule(Rule.parse("if service is excellent or food is delicious then tsTip is generous with 0.5", engine));
takagiSugenoRuleBlock.addRule(Rule.parse("if service is excellent and food is delicious then tsTip is generous with 1.0", engine));
engine.addRuleBlock(takagiSugenoRuleBlock);


}
}

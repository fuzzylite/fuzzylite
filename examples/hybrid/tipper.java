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
engine.setDescription("(service and food) -> (tip)");

InputVariable service = new InputVariable();
service.setName("service");
service.setDescription("quality of service");
service.setEnabled(true);
service.setRange(0.000, 10.000);
service.setLockValueInRange(true);
service.addTerm(new Trapezoid("poor", 0.000, 0.000, 2.500, 5.000));
service.addTerm(new Triangle("good", 2.500, 5.000, 7.500));
service.addTerm(new Trapezoid("excellent", 5.000, 7.500, 10.000, 10.000));
engine.addInputVariable(service);

InputVariable food = new InputVariable();
food.setName("food");
food.setDescription("quality of food");
food.setEnabled(true);
food.setRange(0.000, 10.000);
food.setLockValueInRange(true);
food.addTerm(new Trapezoid("rancid", 0.000, 0.000, 2.500, 7.500));
food.addTerm(new Trapezoid("delicious", 2.500, 7.500, 10.000, 10.000));
engine.addInputVariable(food);

OutputVariable mTip = new OutputVariable();
mTip.setName("mTip");
mTip.setDescription("tip based on Mamdani inference");
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
tsTip.setDescription("tip based on Takagi-Sugeno inference");
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

RuleBlock mamdani = new RuleBlock();
mamdani.setName("mamdani");
mamdani.setDescription("Mamdani inference");
mamdani.setEnabled(true);
mamdani.setConjunction(new AlgebraicProduct());
mamdani.setDisjunction(new AlgebraicSum());
mamdani.setImplication(new Minimum());
mamdani.setActivation(new General());
mamdani.addRule(Rule.parse("if service is poor or food is rancid then mTip is cheap", engine));
mamdani.addRule(Rule.parse("if service is good then mTip is average", engine));
mamdani.addRule(Rule.parse("if service is excellent or food is delicious then mTip is generous with 0.5", engine));
mamdani.addRule(Rule.parse("if service is excellent and food is delicious then mTip is generous with 1.0", engine));
engine.addRuleBlock(mamdani);

RuleBlock takagiSugeno = new RuleBlock();
takagiSugeno.setName("takagiSugeno");
takagiSugeno.setDescription("Takagi-Sugeno inference");
takagiSugeno.setEnabled(true);
takagiSugeno.setConjunction(new AlgebraicProduct());
takagiSugeno.setDisjunction(new AlgebraicSum());
takagiSugeno.setImplication(null);
takagiSugeno.setActivation(new General());
takagiSugeno.addRule(Rule.parse("if service is poor or food is rancid then tsTip is cheap", engine));
takagiSugeno.addRule(Rule.parse("if service is good then tsTip is average", engine));
takagiSugeno.addRule(Rule.parse("if service is excellent or food is delicious then tsTip is generous with 0.5", engine));
takagiSugeno.addRule(Rule.parse("if service is excellent and food is delicious then tsTip is generous with 1.0", engine));
engine.addRuleBlock(takagiSugeno);


}
}

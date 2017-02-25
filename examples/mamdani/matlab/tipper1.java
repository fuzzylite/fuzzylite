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

public class tipper1{
public static void main(String[] args){
//Code automatically generated with fuzzylite 6.0.

Engine engine = new Engine();
engine.setName("tipper1");
engine.setDescription("");

InputVariable service = new InputVariable();
service.setName("service");
service.setDescription("");
service.setEnabled(true);
service.setRange(0.000, 10.000);
service.setLockValueInRange(false);
service.addTerm(new Gaussian("poor", 0.000, 1.500));
service.addTerm(new Gaussian("good", 5.000, 1.500));
service.addTerm(new Gaussian("excellent", 10.000, 1.500));
engine.addInputVariable(service);

OutputVariable tip = new OutputVariable();
tip.setName("tip");
tip.setDescription("");
tip.setEnabled(true);
tip.setRange(0.000, 30.000);
tip.setLockValueInRange(false);
tip.setAggregation(new Maximum());
tip.setDefuzzifier(new Centroid(200));
tip.setDefaultValue(Double.NaN);
tip.setLockPreviousValue(false);
tip.addTerm(new Triangle("cheap", 0.000, 5.000, 10.000));
tip.addTerm(new Triangle("average", 10.000, 15.000, 20.000));
tip.addTerm(new Triangle("generous", 20.000, 25.000, 30.000));
engine.addOutputVariable(tip);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setName("");
ruleBlock.setDescription("");
ruleBlock.setEnabled(true);
ruleBlock.setConjunction(new Minimum());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setImplication(new Minimum());
ruleBlock.setActivation(new General());
ruleBlock.addRule(Rule.parse("if service is poor then tip is cheap", engine));
ruleBlock.addRule(Rule.parse("if service is good then tip is average", engine));
ruleBlock.addRule(Rule.parse("if service is excellent then tip is generous", engine));
engine.addRuleBlock(ruleBlock);


}
}

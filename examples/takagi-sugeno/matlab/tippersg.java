import com.fuzzylite.*;
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

public class tippersg{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("sugeno tipper");

InputVariable inputVariable1 = new InputVariable();
inputVariable1.setEnabled(true);
inputVariable1.setName("service");
inputVariable1.setRange(0.00000000, 10.00000000);
inputVariable1.addTerm(new Gaussian("poor", 0.00000000, 1.50000000));
inputVariable1.addTerm(new Gaussian("average", 5.00000000, 1.50000000));
inputVariable1.addTerm(new Gaussian("good", 10.00000000, 1.50000000));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("food");
inputVariable2.setRange(0.00000000, 10.00000000);
inputVariable2.addTerm(new Trapezoid("rancid", -5.00000000, 0.00000000, 1.00000000, 3.00000000));
inputVariable2.addTerm(new Trapezoid("delicious", 7.00000000, 9.00000000, 10.00000000, 15.00000000));
engine.addInputVariable(inputVariable2);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("tip");
outputVariable.setRange(-30.00000000, 30.00000000);
outputVariable.setLockOutputRange(false);
outputVariable.setLockValidOutput(false);
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setDefuzzifier(new WeightedAverage());
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.addTerm(Linear.create("cheap", engine.getInputVariables(), 0.00000000, 0.00000000, 5.00000000));
outputVariable.addTerm(Linear.create("average", engine.getInputVariables(), 0.00000000, 0.00000000, 15.00000000));
outputVariable.addTerm(Linear.create("generous", engine.getInputVariables(), 0.00000000, 0.00000000, 25.00000000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new Minimum());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new Minimum());
ruleBlock.addRule(Rule.parse("if service is poor or food is rancid then tip is cheap", engine));
ruleBlock.addRule(Rule.parse("if service is average then tip is average", engine));
ruleBlock.addRule(Rule.parse("if service is good or food is delicious then tip is generous", engine));
engine.addRuleBlock(ruleBlock);


}
}

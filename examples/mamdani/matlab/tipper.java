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

public class tipper{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("tipper");

InputVariable inputVariable1 = new InputVariable();
inputVariable1.setEnabled(true);
inputVariable1.setName("service");
inputVariable1.setRange(0.00000000, 10.00000000);
inputVariable1.addTerm(new Gaussian("poor", 0.00000000, 1.50000000));
inputVariable1.addTerm(new Gaussian("good", 5.00000000, 1.50000000));
inputVariable1.addTerm(new Gaussian("excellent", 10.00000000, 1.50000000));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("food");
inputVariable2.setRange(0.00000000, 10.00000000);
inputVariable2.addTerm(new Trapezoid("rancid", 0.00000000, 0.00000000, 1.00000000, 3.00000000));
inputVariable2.addTerm(new Trapezoid("delicious", 7.00000000, 9.00000000, 10.00000000, 10.00000000));
engine.addInputVariable(inputVariable2);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("tip");
outputVariable.setRange(0.00000000, 30.00000000);
outputVariable.setLockOutputRange(false);
outputVariable.setLockValidOutput(false);
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setDefuzzifier(new Centroid(200));
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.addTerm(new Triangle("cheap", 0.00000000, 5.00000000, 10.00000000));
outputVariable.addTerm(new Triangle("average", 10.00000000, 15.00000000, 20.00000000));
outputVariable.addTerm(new Triangle("generous", 20.00000000, 25.00000000, 30.00000000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new Minimum());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new Minimum());
ruleBlock.addRule(Rule.parse("if service is poor or food is rancid then tip is cheap", engine));
ruleBlock.addRule(Rule.parse("if service is good then tip is average", engine));
ruleBlock.addRule(Rule.parse("if service is excellent or food is delicious then tip is generous", engine));
engine.addRuleBlock(ruleBlock);


}
}

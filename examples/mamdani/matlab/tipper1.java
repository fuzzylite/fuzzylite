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

public class tipper1{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("tipper");

InputVariable inputVariable = new InputVariable();
inputVariable.setEnabled(true);
inputVariable.setName("service");
inputVariable.setRange(0.000, 10.000);
inputVariable.addTerm(new Gaussian("poor", 0.000, 1.500));
inputVariable.addTerm(new Gaussian("good", 5.000, 1.500));
inputVariable.addTerm(new Gaussian("excellent", 10.000, 1.500));
engine.addInputVariable(inputVariable);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("tip");
outputVariable.setRange(0.000, 30.000);
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.setDefuzzifier(new Centroid(200));
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setLockValidOutput(false);
outputVariable.setLockOutputRange(false);
outputVariable.addTerm(new Triangle("cheap", 0.000, 5.000, 10.000));
outputVariable.addTerm(new Triangle("average", 10.000, 15.000, 20.000));
outputVariable.addTerm(new Triangle("generous", 20.000, 25.000, 30.000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new Minimum());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new Minimum());
ruleBlock.addRule(Rule.parse("if service is poor then tip is cheap", engine));
ruleBlock.addRule(Rule.parse("if service is good then tip is average", engine));
ruleBlock.addRule(Rule.parse("if service is excellent then tip is generous", engine));
engine.addRuleBlock(ruleBlock);


}
}

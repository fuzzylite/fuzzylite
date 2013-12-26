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
inputVariable.setRange(0.00000000, 10.00000000);
inputVariable.addTerm(new Gaussian("poor", 0.00000000, 1.50000000));
inputVariable.addTerm(new Gaussian("good", 5.00000000, 1.50000000));
inputVariable.addTerm(new Gaussian("excellent", 10.00000000, 1.50000000));
engine.addInputVariable(inputVariable);

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
ruleBlock.addRule(Rule.parse("if service is poor then tip is cheap", engine));
ruleBlock.addRule(Rule.parse("if service is good then tip is average", engine));
ruleBlock.addRule(Rule.parse("if service is excellent then tip is generous", engine));
engine.addRuleBlock(ruleBlock);


}
}

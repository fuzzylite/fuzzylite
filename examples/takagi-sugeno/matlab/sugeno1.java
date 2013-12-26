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

public class sugeno1{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("sugeno");

InputVariable inputVariable = new InputVariable();
inputVariable.setEnabled(true);
inputVariable.setName("input");
inputVariable.setRange(-5.00000000, 5.00000000);
inputVariable.addTerm(new Gaussian("low", -5.00000000, 4.00000000));
inputVariable.addTerm(new Gaussian("high", 5.00000000, 4.00000000));
engine.addInputVariable(inputVariable);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("output");
outputVariable.setRange(0.00000000, 1.00000000);
outputVariable.setLockOutputRange(false);
outputVariable.setLockValidOutput(false);
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setDefuzzifier(new WeightedAverage());
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.addTerm(Linear.create("line1", engine.getInputVariables(), -1.00000000, -1.00000000));
outputVariable.addTerm(Linear.create("line2", engine.getInputVariables(), 1.00000000, -1.00000000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(new AlgebraicSum());
ruleBlock.setActivation(new Minimum());
ruleBlock.addRule(Rule.parse("if input is low then output is line1", engine));
ruleBlock.addRule(Rule.parse("if input is high then output is line2", engine));
engine.addRuleBlock(ruleBlock);


}
}

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

public class SimpleDimmer{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("simple-dimmer");

InputVariable inputVariable = new InputVariable();
inputVariable.setEnabled(true);
inputVariable.setName("Ambient");
inputVariable.setRange(0.00000000, 1.00000000);
inputVariable.addTerm(new Triangle("DARK", 0.00000000, 0.25000000, 0.50000000));
inputVariable.addTerm(new Triangle("MEDIUM", 0.25000000, 0.50000000, 0.75000000));
inputVariable.addTerm(new Triangle("BRIGHT", 0.50000000, 0.75000000, 1.00000000));
engine.addInputVariable(inputVariable);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("Power");
outputVariable.setRange(0.00000000, 1.00000000);
outputVariable.setLockOutputRange(false);
outputVariable.setLockValidOutput(false);
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setDefuzzifier(new Centroid(200));
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.addTerm(new Triangle("LOW", 0.00000000, 0.25000000, 0.50000000));
outputVariable.addTerm(new Triangle("MEDIUM", 0.25000000, 0.50000000, 0.75000000));
outputVariable.addTerm(new Triangle("HIGH", 0.50000000, 0.75000000, 1.00000000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(null);
ruleBlock.setDisjunction(null);
ruleBlock.setActivation(new Minimum());
ruleBlock.addRule(Rule.parse("if Ambient is DARK then Power is HIGH", engine));
ruleBlock.addRule(Rule.parse("if Ambient is MEDIUM then Power is MEDIUM", engine));
ruleBlock.addRule(Rule.parse("if Ambient is BRIGHT then Power is LOW", engine));
engine.addRuleBlock(ruleBlock);


}
}

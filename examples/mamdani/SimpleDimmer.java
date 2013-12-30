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
inputVariable.setRange(0.000, 1.000);
inputVariable.addTerm(new Triangle("DARK", 0.000, 0.250, 0.500));
inputVariable.addTerm(new Triangle("MEDIUM", 0.250, 0.500, 0.750));
inputVariable.addTerm(new Triangle("BRIGHT", 0.500, 0.750, 1.000));
engine.addInputVariable(inputVariable);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("Power");
outputVariable.setRange(0.000, 1.000);
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.setDefuzzifier(new Centroid(200));
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setLockValidOutput(false);
outputVariable.setLockOutputRange(false);
outputVariable.addTerm(new Triangle("LOW", 0.000, 0.250, 0.500));
outputVariable.addTerm(new Triangle("MEDIUM", 0.250, 0.500, 0.750));
outputVariable.addTerm(new Triangle("HIGH", 0.500, 0.750, 1.000));
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

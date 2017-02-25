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

public class sugeno1{
public static void main(String[] args){
//Code automatically generated with fuzzylite 6.0.

Engine engine = new Engine();
engine.setName("sugeno1");
engine.setDescription("");

InputVariable input = new InputVariable();
input.setName("input");
input.setDescription("");
input.setEnabled(true);
input.setRange(-5.000, 5.000);
input.setLockValueInRange(false);
input.addTerm(new Gaussian("low", -5.000, 4.000));
input.addTerm(new Gaussian("high", 5.000, 4.000));
engine.addInputVariable(input);

OutputVariable output = new OutputVariable();
output.setName("output");
output.setDescription("");
output.setEnabled(true);
output.setRange(0.000, 1.000);
output.setLockValueInRange(false);
output.setAggregation(null);
output.setDefuzzifier(new WeightedAverage("TakagiSugeno"));
output.setDefaultValue(Double.NaN);
output.setLockPreviousValue(false);
output.addTerm(Linear.create("line1", engine, -1.000, -1.000));
output.addTerm(Linear.create("line2", engine, 1.000, -1.000));
engine.addOutputVariable(output);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setName("");
ruleBlock.setDescription("");
ruleBlock.setEnabled(true);
ruleBlock.setConjunction(null);
ruleBlock.setDisjunction(null);
ruleBlock.setImplication(null);
ruleBlock.setActivation(new General());
ruleBlock.addRule(Rule.parse("if input is low then output is line1", engine));
ruleBlock.addRule(Rule.parse("if input is high then output is line2", engine));
engine.addRuleBlock(ruleBlock);


}
}

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
//Java code generated with fuzzylite 6.0.

Engine engine = new Engine();
engine.setName("SimpleDimmer");

InputVariable Ambient = new InputVariable();
Ambient.setEnabled(true);
Ambient.setName("Ambient");
Ambient.setRange(0.000, 1.000);
Ambient.setLockValueInRange(false);
Ambient.addTerm(new Triangle("DARK", 0.000, 0.250, 0.500));
Ambient.addTerm(new Triangle("MEDIUM", 0.250, 0.500, 0.750));
Ambient.addTerm(new Triangle("BRIGHT", 0.500, 0.750, 1.000));
engine.addInputVariable(Ambient);

OutputVariable Power = new OutputVariable();
Power.setEnabled(true);
Power.setName("Power");
Power.setRange(0.000, 1.000);
Power.setLockValueInRange(false);
Power.fuzzyOutput().setAggregation(null);
Power.setDefuzzifier(new WeightedAverage("TakagiSugeno"));
Power.setDefaultValue(Double.NaN);
Power.setLockPreviousValue(false);
Power.addTerm(new Constant("LOW", 0.250));
Power.addTerm(new Constant("MEDIUM", 0.500));
Power.addTerm(new Constant("HIGH", 0.750));
engine.addOutputVariable(Power);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(null);
ruleBlock.setDisjunction(null);
ruleBlock.setImplication(null);
ruleBlock.setActivation(new General());
ruleBlock.addRule(Rule.parse("if Ambient is DARK then Power is HIGH", engine));
ruleBlock.addRule(Rule.parse("if Ambient is MEDIUM then Power is MEDIUM", engine));
ruleBlock.addRule(Rule.parse("if Ambient is BRIGHT then Power is LOW", engine));
engine.addRuleBlock(ruleBlock);


}
}

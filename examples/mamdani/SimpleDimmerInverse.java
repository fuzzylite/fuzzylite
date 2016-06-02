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

public class SimpleDimmerInverse{
public static void main(String[] args){
//Java code generated with fuzzylite 6.0.

Engine engine = new Engine();
engine.setName("SimpleDimmerInverse");

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
Power.fuzzyOutput().setAggregation(new Maximum());
Power.setDefuzzifier(new Centroid(200));
Power.setDefaultValue(Double.NaN);
Power.setLockPreviousValue(false);
Power.addTerm(new Triangle("LOW", 0.000, 0.250, 0.500));
Power.addTerm(new Triangle("MEDIUM", 0.250, 0.500, 0.750));
Power.addTerm(new Triangle("HIGH", 0.500, 0.750, 1.000));
engine.addOutputVariable(Power);

OutputVariable InversePower = new OutputVariable();
InversePower.setEnabled(true);
InversePower.setName("InversePower");
InversePower.setRange(0.000, 1.000);
InversePower.setLockValueInRange(false);
InversePower.fuzzyOutput().setAggregation(new Maximum());
InversePower.setDefuzzifier(new Centroid(500));
InversePower.setDefaultValue(Double.NaN);
InversePower.setLockPreviousValue(false);
InversePower.addTerm(new Cosine("LOW", 0.200, 0.500));
InversePower.addTerm(new Cosine("MEDIUM", 0.500, 0.500));
InversePower.addTerm(new Cosine("HIGH", 0.800, 0.500));
engine.addOutputVariable(InversePower);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(null);
ruleBlock.setDisjunction(null);
ruleBlock.setImplication(new Minimum());
ruleBlock.setActivation(new General());
ruleBlock.addRule(Rule.parse("if Ambient is DARK then Power is HIGH", engine));
ruleBlock.addRule(Rule.parse("if Ambient is MEDIUM then Power is MEDIUM", engine));
ruleBlock.addRule(Rule.parse("if Ambient is BRIGHT then Power is LOW", engine));
ruleBlock.addRule(Rule.parse("if Power is LOW then InversePower is HIGH", engine));
ruleBlock.addRule(Rule.parse("if Power is MEDIUM then InversePower is MEDIUM", engine));
ruleBlock.addRule(Rule.parse("if Power is HIGH then InversePower is LOW", engine));
engine.addRuleBlock(ruleBlock);


}
}

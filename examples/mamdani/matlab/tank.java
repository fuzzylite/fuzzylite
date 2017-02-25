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

public class tank{
public static void main(String[] args){
//Code automatically generated with fuzzylite 6.0.

Engine engine = new Engine();
engine.setName("tank");
engine.setDescription("");

InputVariable level = new InputVariable();
level.setName("level");
level.setDescription("");
level.setEnabled(true);
level.setRange(-1.000, 1.000);
level.setLockValueInRange(false);
level.addTerm(new Gaussian("high", -1.000, 0.300));
level.addTerm(new Gaussian("okay", 0.000, 0.300));
level.addTerm(new Gaussian("low", 1.000, 0.300));
engine.addInputVariable(level);

InputVariable rate = new InputVariable();
rate.setName("rate");
rate.setDescription("");
rate.setEnabled(true);
rate.setRange(-0.100, 0.100);
rate.setLockValueInRange(false);
rate.addTerm(new Gaussian("negative", -0.100, 0.030));
rate.addTerm(new Gaussian("none", 0.000, 0.030));
rate.addTerm(new Gaussian("positive", 0.100, 0.030));
engine.addInputVariable(rate);

OutputVariable valve = new OutputVariable();
valve.setName("valve");
valve.setDescription("");
valve.setEnabled(true);
valve.setRange(-1.000, 1.000);
valve.setLockValueInRange(false);
valve.setAggregation(new Maximum());
valve.setDefuzzifier(new Centroid(200));
valve.setDefaultValue(Double.NaN);
valve.setLockPreviousValue(false);
valve.addTerm(new Triangle("close_fast", -1.000, -0.900, -0.800));
valve.addTerm(new Triangle("close_slow", -0.600, -0.500, -0.400));
valve.addTerm(new Triangle("no_change", -0.100, 0.000, 0.100));
valve.addTerm(new Triangle("open_slow", 0.200, 0.300, 0.400));
valve.addTerm(new Triangle("open_fast", 0.800, 0.900, 1.000));
engine.addOutputVariable(valve);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setName("");
ruleBlock.setDescription("");
ruleBlock.setEnabled(true);
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(new AlgebraicSum());
ruleBlock.setImplication(new AlgebraicProduct());
ruleBlock.setActivation(new General());
ruleBlock.addRule(Rule.parse("if level is okay then valve is no_change", engine));
ruleBlock.addRule(Rule.parse("if level is low then valve is open_fast", engine));
ruleBlock.addRule(Rule.parse("if level is high then valve is close_fast", engine));
ruleBlock.addRule(Rule.parse("if level is okay and rate is positive then valve is close_slow", engine));
ruleBlock.addRule(Rule.parse("if level is okay and rate is negative then valve is open_slow", engine));
engine.addRuleBlock(ruleBlock);


}
}

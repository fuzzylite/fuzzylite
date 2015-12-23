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

public class sltbu_fl{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("sltbu");

InputVariable distance = new InputVariable();
distance.setEnabled(true);
distance.setName("distance");
distance.setRange(0.000, 25.000);
distance.setLockValueInRange(false);
distance.addTerm(new ZShape("near", 1.000, 2.000));
distance.addTerm(new SShape("far", 1.000, 2.000));
engine.addInputVariable(distance);

InputVariable control1 = new InputVariable();
control1.setEnabled(true);
control1.setName("control1");
control1.setRange(-0.785, 0.785);
control1.setLockValueInRange(false);
engine.addInputVariable(control1);

InputVariable control2 = new InputVariable();
control2.setEnabled(true);
control2.setName("control2");
control2.setRange(-0.785, 0.785);
control2.setLockValueInRange(false);
engine.addInputVariable(control2);

OutputVariable control = new OutputVariable();
control.setEnabled(true);
control.setName("control");
control.setRange(-0.785, 0.785);
control.setLockValueInRange(false);
control.fuzzyOutput().setAggregation(null);
control.setDefuzzifier(new WeightedAverage("TakagiSugeno"));
control.setDefaultValue(Double.NaN);
control.setLockPreviousValue(false);
control.addTerm(Linear.create("out1mf1", engine, 0.000, 0.000, 1.000, 0.000));
control.addTerm(Linear.create("out1mf2", engine, 0.000, 1.000, 0.000, 0.000));
engine.addOutputVariable(control);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(null);
ruleBlock.setDisjunction(null);
ruleBlock.setImplication(null);
ruleBlock.setActivation(new General());
ruleBlock.addRule(Rule.parse("if distance is near then control is out1mf1", engine));
ruleBlock.addRule(Rule.parse("if distance is far then control is out1mf2", engine));
engine.addRuleBlock(ruleBlock);


}
}

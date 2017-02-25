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

public class mam21{
public static void main(String[] args){
//Code automatically generated with fuzzylite 6.0.

Engine engine = new Engine();
engine.setName("mam21");
engine.setDescription("");

InputVariable angle = new InputVariable();
angle.setName("angle");
angle.setDescription("");
angle.setEnabled(true);
angle.setRange(-5.000, 5.000);
angle.setLockValueInRange(false);
angle.addTerm(new Bell("small", -5.000, 5.000, 8.000));
angle.addTerm(new Bell("big", 5.000, 5.000, 8.000));
engine.addInputVariable(angle);

InputVariable velocity = new InputVariable();
velocity.setName("velocity");
velocity.setDescription("");
velocity.setEnabled(true);
velocity.setRange(-5.000, 5.000);
velocity.setLockValueInRange(false);
velocity.addTerm(new Bell("small", -5.000, 5.000, 2.000));
velocity.addTerm(new Bell("big", 5.000, 5.000, 2.000));
engine.addInputVariable(velocity);

OutputVariable force = new OutputVariable();
force.setName("force");
force.setDescription("");
force.setEnabled(true);
force.setRange(-5.000, 5.000);
force.setLockValueInRange(false);
force.setAggregation(new Maximum());
force.setDefuzzifier(new Centroid(200));
force.setDefaultValue(Double.NaN);
force.setLockPreviousValue(false);
force.addTerm(new Bell("negBig", -5.000, 1.670, 8.000));
force.addTerm(new Bell("negSmall", -1.670, 1.670, 8.000));
force.addTerm(new Bell("posSmall", 1.670, 1.670, 8.000));
force.addTerm(new Bell("posBig", 5.000, 1.670, 8.000));
engine.addOutputVariable(force);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setName("");
ruleBlock.setDescription("");
ruleBlock.setEnabled(true);
ruleBlock.setConjunction(new Minimum());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setImplication(new Minimum());
ruleBlock.setActivation(new General());
ruleBlock.addRule(Rule.parse("if angle is small and velocity is small then force is negBig", engine));
ruleBlock.addRule(Rule.parse("if angle is small and velocity is big then force is negSmall", engine));
ruleBlock.addRule(Rule.parse("if angle is big and velocity is small then force is posSmall", engine));
ruleBlock.addRule(Rule.parse("if angle is big and velocity is big then force is posBig", engine));
engine.addRuleBlock(ruleBlock);


}
}

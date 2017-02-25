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

public class juggler{
public static void main(String[] args){
//Code automatically generated with fuzzylite 6.0.

Engine engine = new Engine();
engine.setName("juggler");
engine.setDescription("");

InputVariable xHit = new InputVariable();
xHit.setName("xHit");
xHit.setDescription("");
xHit.setEnabled(true);
xHit.setRange(-4.000, 4.000);
xHit.setLockValueInRange(false);
xHit.addTerm(new Bell("in1mf1", -4.000, 2.000, 4.000));
xHit.addTerm(new Bell("in1mf2", 0.000, 2.000, 4.000));
xHit.addTerm(new Bell("in1mf3", 4.000, 2.000, 4.000));
engine.addInputVariable(xHit);

InputVariable projectAngle = new InputVariable();
projectAngle.setName("projectAngle");
projectAngle.setDescription("");
projectAngle.setEnabled(true);
projectAngle.setRange(0.000, 3.142);
projectAngle.setLockValueInRange(false);
projectAngle.addTerm(new Bell("in2mf1", 0.000, 0.785, 4.000));
projectAngle.addTerm(new Bell("in2mf2", 1.571, 0.785, 4.000));
projectAngle.addTerm(new Bell("in2mf3", 3.142, 0.785, 4.000));
engine.addInputVariable(projectAngle);

OutputVariable theta = new OutputVariable();
theta.setName("theta");
theta.setDescription("");
theta.setEnabled(true);
theta.setRange(0.000, 0.000);
theta.setLockValueInRange(false);
theta.setAggregation(null);
theta.setDefuzzifier(new WeightedAverage("TakagiSugeno"));
theta.setDefaultValue(Double.NaN);
theta.setLockPreviousValue(false);
theta.addTerm(Linear.create("out1mf", engine, -0.022, -0.500, 0.315));
theta.addTerm(Linear.create("out1mf", engine, -0.022, -0.500, 0.315));
theta.addTerm(Linear.create("out1mf", engine, -0.022, -0.500, 0.315));
theta.addTerm(Linear.create("out1mf", engine, 0.114, -0.500, 0.785));
theta.addTerm(Linear.create("out1mf", engine, 0.114, -0.500, 0.785));
theta.addTerm(Linear.create("out1mf", engine, 0.114, -0.500, 0.785));
theta.addTerm(Linear.create("out1mf", engine, -0.022, -0.500, 1.256));
theta.addTerm(Linear.create("out1mf", engine, -0.022, -0.500, 1.256));
theta.addTerm(Linear.create("out1mf", engine, -0.022, -0.500, 1.256));
engine.addOutputVariable(theta);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setName("");
ruleBlock.setDescription("");
ruleBlock.setEnabled(true);
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(null);
ruleBlock.setImplication(null);
ruleBlock.setActivation(new General());
ruleBlock.addRule(Rule.parse("if xHit is in1mf1 and projectAngle is in2mf1 then theta is out1mf", engine));
ruleBlock.addRule(Rule.parse("if xHit is in1mf1 and projectAngle is in2mf2 then theta is out1mf", engine));
ruleBlock.addRule(Rule.parse("if xHit is in1mf1 and projectAngle is in2mf3 then theta is out1mf", engine));
ruleBlock.addRule(Rule.parse("if xHit is in1mf2 and projectAngle is in2mf1 then theta is out1mf", engine));
ruleBlock.addRule(Rule.parse("if xHit is in1mf2 and projectAngle is in2mf2 then theta is out1mf", engine));
ruleBlock.addRule(Rule.parse("if xHit is in1mf2 and projectAngle is in2mf3 then theta is out1mf", engine));
ruleBlock.addRule(Rule.parse("if xHit is in1mf3 and projectAngle is in2mf1 then theta is out1mf", engine));
ruleBlock.addRule(Rule.parse("if xHit is in1mf3 and projectAngle is in2mf2 then theta is out1mf", engine));
ruleBlock.addRule(Rule.parse("if xHit is in1mf3 and projectAngle is in2mf3 then theta is out1mf", engine));
engine.addRuleBlock(ruleBlock);


}
}

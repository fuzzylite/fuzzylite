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

public class juggler{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("juggler");

InputVariable inputVariable1 = new InputVariable();
inputVariable1.setEnabled(true);
inputVariable1.setName("xHit");
inputVariable1.setRange(-4.00000000, 4.00000000);
inputVariable1.addTerm(new Bell("in1mf1", -4.00000000, 2.00000000, 4.00000000));
inputVariable1.addTerm(new Bell("in1mf2", 0.00000000, 2.00000000, 4.00000000));
inputVariable1.addTerm(new Bell("in1mf3", 4.00000000, 2.00000000, 4.00000000));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("projectAngle");
inputVariable2.setRange(0.00000000, 3.14200000);
inputVariable2.addTerm(new Bell("in2mf1", 0.00000000, 0.78500000, 4.00000000));
inputVariable2.addTerm(new Bell("in2mf2", 1.57100000, 0.78500000, 4.00000000));
inputVariable2.addTerm(new Bell("in2mf3", 3.14200000, 0.78500000, 4.00000000));
engine.addInputVariable(inputVariable2);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("theta");
outputVariable.setRange(0.00000000, 0.00000000);
outputVariable.setLockOutputRange(false);
outputVariable.setLockValidOutput(false);
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setDefuzzifier(new WeightedAverage());
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.addTerm(Linear.create("out1mf", engine.getInputVariables(), -0.02200000, -0.50000000, 0.31500000));
outputVariable.addTerm(Linear.create("out1mf", engine.getInputVariables(), -0.02200000, -0.50000000, 0.31500000));
outputVariable.addTerm(Linear.create("out1mf", engine.getInputVariables(), -0.02200000, -0.50000000, 0.31500000));
outputVariable.addTerm(Linear.create("out1mf", engine.getInputVariables(), 0.11400000, -0.50000000, 0.78500000));
outputVariable.addTerm(Linear.create("out1mf", engine.getInputVariables(), 0.11400000, -0.50000000, 0.78500000));
outputVariable.addTerm(Linear.create("out1mf", engine.getInputVariables(), 0.11400000, -0.50000000, 0.78500000));
outputVariable.addTerm(Linear.create("out1mf", engine.getInputVariables(), -0.02200000, -0.50000000, 1.25600000));
outputVariable.addTerm(Linear.create("out1mf", engine.getInputVariables(), -0.02200000, -0.50000000, 1.25600000));
outputVariable.addTerm(Linear.create("out1mf", engine.getInputVariables(), -0.02200000, -0.50000000, 1.25600000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new AlgebraicProduct());
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

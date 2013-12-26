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

public class fpeaks{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("peaks");

InputVariable inputVariable1 = new InputVariable();
inputVariable1.setEnabled(true);
inputVariable1.setName("in1");
inputVariable1.setRange(-3.00000000, 3.00000000);
inputVariable1.addTerm(new Bell("in1mf1", -2.23300000, 1.57800000, 2.15100000));
inputVariable1.addTerm(new Bell("in1mf2", -0.39400000, 0.75300000, 1.83800000));
inputVariable1.addTerm(new Bell("in1mf3", 0.49700000, 0.68900000, 1.84400000));
inputVariable1.addTerm(new Bell("in1mf4", 2.27000000, 1.52800000, 2.15600000));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("in2");
inputVariable2.setRange(-3.00000000, 3.00000000);
inputVariable2.addTerm(new Bell("in1mf1", -2.68600000, 1.26700000, 2.04400000));
inputVariable2.addTerm(new Bell("in1mf2", -0.83600000, 1.26600000, 1.79600000));
inputVariable2.addTerm(new Bell("in1mf3", 0.85900000, 1.31400000, 1.93700000));
inputVariable2.addTerm(new Bell("in1mf4", 2.72700000, 1.21400000, 2.04700000));
engine.addInputVariable(inputVariable2);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("out1");
outputVariable.setRange(-10.00000000, 10.00000000);
outputVariable.setLockOutputRange(false);
outputVariable.setLockValidOutput(false);
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setDefuzzifier(new WeightedAverage());
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.addTerm(Linear.create("out1mf1", engine.getInputVariables(), 0.15500000, -2.22800000, -8.97400000));
outputVariable.addTerm(Linear.create("out1mf2", engine.getInputVariables(), -0.31200000, -7.70500000, -9.05500000));
outputVariable.addTerm(Linear.create("out1mf3", engine.getInputVariables(), -0.45400000, -4.43700000, 6.93000000));
outputVariable.addTerm(Linear.create("out1mf4", engine.getInputVariables(), 0.24800000, -1.12200000, 5.08100000));
outputVariable.addTerm(Linear.create("out1mf5", engine.getInputVariables(), -6.27800000, 25.21100000, 99.14800000));
outputVariable.addTerm(Linear.create("out1mf6", engine.getInputVariables(), 5.53100000, 105.91600000, 157.28300000));
outputVariable.addTerm(Linear.create("out1mf7", engine.getInputVariables(), 19.51900000, 112.33300000, -127.79600000));
outputVariable.addTerm(Linear.create("out1mf8", engine.getInputVariables(), -5.07900000, 34.73800000, -143.41400000));
outputVariable.addTerm(Linear.create("out1mf9", engine.getInputVariables(), -5.88900000, 27.31100000, 116.58500000));
outputVariable.addTerm(Linear.create("out1mf10", engine.getInputVariables(), 21.51700000, 97.26600000, 93.80200000));
outputVariable.addTerm(Linear.create("out1mf11", engine.getInputVariables(), 9.19800000, 79.85300000, -118.48200000));
outputVariable.addTerm(Linear.create("out1mf12", engine.getInputVariables(), -6.57100000, 23.02600000, -87.74700000));
outputVariable.addTerm(Linear.create("out1mf13", engine.getInputVariables(), 0.09200000, -1.12600000, -4.52700000));
outputVariable.addTerm(Linear.create("out1mf14", engine.getInputVariables(), -0.30400000, -4.43400000, -6.56100000));
outputVariable.addTerm(Linear.create("out1mf15", engine.getInputVariables(), -0.16600000, -6.28400000, 7.30700000));
outputVariable.addTerm(Linear.create("out1mf16", engine.getInputVariables(), 0.10700000, -2.02800000, 8.15900000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new AlgebraicProduct());
ruleBlock.addRule(Rule.parse("if in1 is in1mf1 and in2 is in1mf1 then out1 is out1mf1", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf1 and in2 is in1mf2 then out1 is out1mf2", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf1 and in2 is in1mf3 then out1 is out1mf3", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf1 and in2 is in1mf4 then out1 is out1mf4", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf2 and in2 is in1mf1 then out1 is out1mf5", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf2 and in2 is in1mf2 then out1 is out1mf6", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf2 and in2 is in1mf3 then out1 is out1mf7", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf2 and in2 is in1mf4 then out1 is out1mf8", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf3 and in2 is in1mf1 then out1 is out1mf9", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf3 and in2 is in1mf2 then out1 is out1mf10", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf3 and in2 is in1mf3 then out1 is out1mf11", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf3 and in2 is in1mf4 then out1 is out1mf12", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf4 and in2 is in1mf1 then out1 is out1mf13", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf4 and in2 is in1mf2 then out1 is out1mf14", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf4 and in2 is in1mf3 then out1 is out1mf15", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf4 and in2 is in1mf4 then out1 is out1mf16", engine));
engine.addRuleBlock(ruleBlock);


}
}

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

public class slbb{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("slbb");

InputVariable inputVariable1 = new InputVariable();
inputVariable1.setEnabled(true);
inputVariable1.setName("in1");
inputVariable1.setRange(-1.50000000, 1.50000000);
inputVariable1.addTerm(new Bell("in1mf1", -1.50000000, 1.50000000, 2.00000000));
inputVariable1.addTerm(new Bell("in1mf2", 1.50000000, 1.50000000, 2.00000000));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("in2");
inputVariable2.setRange(-1.50000000, 1.50000000);
inputVariable2.addTerm(new Bell("in2mf1", -1.50000000, 1.50000000, 2.00000000));
inputVariable2.addTerm(new Bell("in2mf2", 1.50000000, 1.50000000, 2.00000000));
engine.addInputVariable(inputVariable2);

InputVariable inputVariable3 = new InputVariable();
inputVariable3.setEnabled(true);
inputVariable3.setName("in3");
inputVariable3.setRange(-0.20000000, 0.20000000);
inputVariable3.addTerm(new Bell("in3mf1", -0.20000000, 0.20000000, 2.00000000));
inputVariable3.addTerm(new Bell("in3mf2", 0.20000000, 0.20000000, 2.00000000));
engine.addInputVariable(inputVariable3);

InputVariable inputVariable4 = new InputVariable();
inputVariable4.setEnabled(true);
inputVariable4.setName("in4");
inputVariable4.setRange(-0.40000000, 0.40000000);
inputVariable4.addTerm(new Bell("in4mf1", -0.40000000, 0.40000000, 2.00000000));
inputVariable4.addTerm(new Bell("in4mf2", 0.40000000, 0.40000000, 2.00000000));
engine.addInputVariable(inputVariable4);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("out");
outputVariable.setRange(-10.00000000, 10.00000000);
outputVariable.setLockOutputRange(false);
outputVariable.setLockValidOutput(false);
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setDefuzzifier(new WeightedAverage());
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.addTerm(Linear.create("outmf1", engine.getInputVariables(), 1.01500000, 2.23400000, -12.66500000, -4.04600000, 0.02600000));
outputVariable.addTerm(Linear.create("outmf2", engine.getInputVariables(), 1.16100000, 1.96900000, -9.39600000, -6.16500000, 0.47400000));
outputVariable.addTerm(Linear.create("outmf3", engine.getInputVariables(), 1.50600000, 2.23400000, -12.99000000, -1.86500000, 1.42600000));
outputVariable.addTerm(Linear.create("outmf4", engine.getInputVariables(), 0.73400000, 1.96900000, -9.38100000, -4.68800000, -0.88000000));
outputVariable.addTerm(Linear.create("outmf5", engine.getInputVariables(), 0.73400000, 2.23400000, -12.85300000, -6.11000000, -1.03400000));
outputVariable.addTerm(Linear.create("outmf6", engine.getInputVariables(), 1.41300000, 1.96900000, -9.48500000, -6.59200000, 1.15900000));
outputVariable.addTerm(Linear.create("outmf7", engine.getInputVariables(), 1.22500000, 2.23400000, -12.80100000, -3.92900000, 0.36600000));
outputVariable.addTerm(Linear.create("outmf8", engine.getInputVariables(), 0.98500000, 1.96900000, -9.29100000, -5.11500000, -0.19500000));
outputVariable.addTerm(Linear.create("outmf9", engine.getInputVariables(), 0.98500000, 1.96900000, -9.29200000, -5.11500000, 0.19500000));
outputVariable.addTerm(Linear.create("outmf10", engine.getInputVariables(), 1.22500000, 2.23400000, -12.80200000, -3.92900000, -0.36600000));
outputVariable.addTerm(Linear.create("outmf11", engine.getInputVariables(), 1.41300000, 1.96900000, -9.48500000, -6.59200000, -1.15900000));
outputVariable.addTerm(Linear.create("outmf12", engine.getInputVariables(), 0.73400000, 2.23400000, -12.85300000, -6.11000000, 1.03400000));
outputVariable.addTerm(Linear.create("outmf13", engine.getInputVariables(), 0.73400000, 1.96900000, -9.38100000, -4.68800000, 0.88000000));
outputVariable.addTerm(Linear.create("outmf14", engine.getInputVariables(), 1.50600000, 2.23400000, -12.99000000, -1.86500000, -1.42600000));
outputVariable.addTerm(Linear.create("outmf15", engine.getInputVariables(), 1.16100000, 1.96900000, -9.39600000, -6.16500000, -0.47400000));
outputVariable.addTerm(Linear.create("outmf16", engine.getInputVariables(), 1.01500000, 2.23400000, -12.66500000, -4.04600000, -0.02600000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new AlgebraicProduct());
ruleBlock.addRule(Rule.parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf1 then out is outmf1", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf2 then out is outmf2", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf1 then out is outmf3", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf2 then out is outmf4", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf1 then out is outmf5", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf2 then out is outmf6", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf1 then out is outmf7", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf2 then out is outmf8", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf1 then out is outmf9", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf2 then out is outmf10", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf1 then out is outmf11", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf2 then out is outmf12", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf1 then out is outmf13", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf2 then out is outmf14", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf1 then out is outmf15", engine));
ruleBlock.addRule(Rule.parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf2 then out is outmf16", engine));
engine.addRuleBlock(ruleBlock);


}
}

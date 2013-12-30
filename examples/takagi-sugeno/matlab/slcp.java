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

public class slcp{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("slcp");

InputVariable inputVariable1 = new InputVariable();
inputVariable1.setEnabled(true);
inputVariable1.setName("in1");
inputVariable1.setRange(-0.300, 0.300);
inputVariable1.addTerm(new Bell("in1mf1", -0.300, 0.300, 2.000));
inputVariable1.addTerm(new Bell("in1mf2", 0.300, 0.300, 2.000));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("in2");
inputVariable2.setRange(-1.000, 1.000);
inputVariable2.addTerm(new Bell("in2mf1", -1.000, 1.000, 2.000));
inputVariable2.addTerm(new Bell("in2mf2", 1.000, 1.000, 2.000));
engine.addInputVariable(inputVariable2);

InputVariable inputVariable3 = new InputVariable();
inputVariable3.setEnabled(true);
inputVariable3.setName("in3");
inputVariable3.setRange(-3.000, 3.000);
inputVariable3.addTerm(new Bell("in3mf1", -3.000, 3.000, 2.000));
inputVariable3.addTerm(new Bell("in3mf2", 3.000, 3.000, 2.000));
engine.addInputVariable(inputVariable3);

InputVariable inputVariable4 = new InputVariable();
inputVariable4.setEnabled(true);
inputVariable4.setName("in4");
inputVariable4.setRange(-3.000, 3.000);
inputVariable4.addTerm(new Bell("in4mf1", -3.000, 3.000, 2.000));
inputVariable4.addTerm(new Bell("in4mf2", 3.000, 3.000, 2.000));
engine.addInputVariable(inputVariable4);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("out");
outputVariable.setRange(-10.000, 10.000);
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.setDefuzzifier(new WeightedAverage());
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setLockValidOutput(false);
outputVariable.setLockOutputRange(false);
outputVariable.addTerm(Linear.create("outmf1", engine.getInputVariables(), 41.373, 10.030, 3.162, 4.288, 0.339));
outputVariable.addTerm(Linear.create("outmf2", engine.getInputVariables(), 40.409, 10.053, 3.162, 4.288, 0.207));
outputVariable.addTerm(Linear.create("outmf3", engine.getInputVariables(), 41.373, 10.030, 3.162, 4.288, 0.339));
outputVariable.addTerm(Linear.create("outmf4", engine.getInputVariables(), 40.409, 10.053, 3.162, 4.288, 0.207));
outputVariable.addTerm(Linear.create("outmf5", engine.getInputVariables(), 38.561, 10.177, 3.162, 4.288, -0.049));
outputVariable.addTerm(Linear.create("outmf6", engine.getInputVariables(), 37.596, 10.154, 3.162, 4.288, -0.181));
outputVariable.addTerm(Linear.create("outmf7", engine.getInputVariables(), 38.561, 10.177, 3.162, 4.288, -0.049));
outputVariable.addTerm(Linear.create("outmf8", engine.getInputVariables(), 37.596, 10.154, 3.162, 4.288, -0.181));
outputVariable.addTerm(Linear.create("outmf9", engine.getInputVariables(), 37.596, 10.154, 3.162, 4.288, 0.181));
outputVariable.addTerm(Linear.create("outmf10", engine.getInputVariables(), 38.561, 10.177, 3.162, 4.288, 0.049));
outputVariable.addTerm(Linear.create("outmf11", engine.getInputVariables(), 37.596, 10.154, 3.162, 4.288, 0.181));
outputVariable.addTerm(Linear.create("outmf12", engine.getInputVariables(), 38.561, 10.177, 3.162, 4.288, 0.049));
outputVariable.addTerm(Linear.create("outmf13", engine.getInputVariables(), 40.408, 10.053, 3.162, 4.288, -0.207));
outputVariable.addTerm(Linear.create("outmf14", engine.getInputVariables(), 41.373, 10.030, 3.162, 4.288, -0.339));
outputVariable.addTerm(Linear.create("outmf15", engine.getInputVariables(), 40.408, 10.053, 3.162, 4.288, -0.207));
outputVariable.addTerm(Linear.create("outmf16", engine.getInputVariables(), 41.373, 10.030, 3.162, 4.288, -0.339));
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

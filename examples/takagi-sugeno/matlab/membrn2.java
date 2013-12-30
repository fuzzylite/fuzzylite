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

public class membrn2{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("anfis");

InputVariable inputVariable1 = new InputVariable();
inputVariable1.setEnabled(true);
inputVariable1.setName("in_n1");
inputVariable1.setRange(1.000, 31.000);
inputVariable1.addTerm(new Bell("in1mf1", 1.152, 8.206, 0.874));
inputVariable1.addTerm(new Bell("in1mf2", 15.882, 7.078, 0.444));
inputVariable1.addTerm(new Bell("in1mf3", 30.575, 8.602, 0.818));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("in_n2");
inputVariable2.setRange(1.000, 31.000);
inputVariable2.addTerm(new Bell("in2mf1", 1.426, 8.602, 0.818));
inputVariable2.addTerm(new Bell("in2mf2", 16.118, 7.078, 0.445));
inputVariable2.addTerm(new Bell("in2mf3", 30.847, 8.206, 0.875));
engine.addInputVariable(inputVariable2);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("out1");
outputVariable.setRange(-0.334, 1.000);
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.setDefuzzifier(new WeightedAverage());
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setLockValidOutput(false);
outputVariable.setLockOutputRange(false);
outputVariable.addTerm(Linear.create("out1mf1", engine.getInputVariables(), -0.035, 0.002, -0.352));
outputVariable.addTerm(Linear.create("out1mf2", engine.getInputVariables(), 0.044, 0.079, -0.028));
outputVariable.addTerm(Linear.create("out1mf3", engine.getInputVariables(), -0.024, 0.024, -1.599));
outputVariable.addTerm(Linear.create("out1mf4", engine.getInputVariables(), -0.067, 0.384, 0.007));
outputVariable.addTerm(Linear.create("out1mf5", engine.getInputVariables(), 0.351, -0.351, -3.663));
outputVariable.addTerm(Linear.create("out1mf6", engine.getInputVariables(), -0.079, -0.044, 3.909));
outputVariable.addTerm(Linear.create("out1mf7", engine.getInputVariables(), 0.012, -0.012, -0.600));
outputVariable.addTerm(Linear.create("out1mf8", engine.getInputVariables(), -0.384, 0.067, 10.158));
outputVariable.addTerm(Linear.create("out1mf9", engine.getInputVariables(), -0.002, 0.035, -1.402));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new AlgebraicProduct());
ruleBlock.addRule(Rule.parse("if in_n1 is in1mf1 and in_n2 is in2mf1 then out1 is out1mf1", engine));
ruleBlock.addRule(Rule.parse("if in_n1 is in1mf1 and in_n2 is in2mf2 then out1 is out1mf2", engine));
ruleBlock.addRule(Rule.parse("if in_n1 is in1mf1 and in_n2 is in2mf3 then out1 is out1mf3", engine));
ruleBlock.addRule(Rule.parse("if in_n1 is in1mf2 and in_n2 is in2mf1 then out1 is out1mf4", engine));
ruleBlock.addRule(Rule.parse("if in_n1 is in1mf2 and in_n2 is in2mf2 then out1 is out1mf5", engine));
ruleBlock.addRule(Rule.parse("if in_n1 is in1mf2 and in_n2 is in2mf3 then out1 is out1mf6", engine));
ruleBlock.addRule(Rule.parse("if in_n1 is in1mf3 and in_n2 is in2mf1 then out1 is out1mf7", engine));
ruleBlock.addRule(Rule.parse("if in_n1 is in1mf3 and in_n2 is in2mf2 then out1 is out1mf8", engine));
ruleBlock.addRule(Rule.parse("if in_n1 is in1mf3 and in_n2 is in2mf3 then out1 is out1mf9", engine));
engine.addRuleBlock(ruleBlock);


}
}

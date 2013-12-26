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
inputVariable1.setRange(1.00000000, 31.00000000);
inputVariable1.addTerm(new Bell("in1mf1", 1.15200000, 8.20600000, 0.87400000));
inputVariable1.addTerm(new Bell("in1mf2", 15.88200000, 7.07800000, 0.44400000));
inputVariable1.addTerm(new Bell("in1mf3", 30.57500000, 8.60200000, 0.81800000));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("in_n2");
inputVariable2.setRange(1.00000000, 31.00000000);
inputVariable2.addTerm(new Bell("in2mf1", 1.42600000, 8.60200000, 0.81800000));
inputVariable2.addTerm(new Bell("in2mf2", 16.11800000, 7.07800000, 0.44500000));
inputVariable2.addTerm(new Bell("in2mf3", 30.84700000, 8.20600000, 0.87500000));
engine.addInputVariable(inputVariable2);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("out1");
outputVariable.setRange(-0.33400000, 1.00000000);
outputVariable.setLockOutputRange(false);
outputVariable.setLockValidOutput(false);
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setDefuzzifier(new WeightedAverage());
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.addTerm(Linear.create("out1mf1", engine.getInputVariables(), -0.03500000, 0.00200000, -0.35200000));
outputVariable.addTerm(Linear.create("out1mf2", engine.getInputVariables(), 0.04400000, 0.07900000, -0.02800000));
outputVariable.addTerm(Linear.create("out1mf3", engine.getInputVariables(), -0.02400000, 0.02400000, -1.59900000));
outputVariable.addTerm(Linear.create("out1mf4", engine.getInputVariables(), -0.06700000, 0.38400000, 0.00700000));
outputVariable.addTerm(Linear.create("out1mf5", engine.getInputVariables(), 0.35100000, -0.35100000, -3.66300000));
outputVariable.addTerm(Linear.create("out1mf6", engine.getInputVariables(), -0.07900000, -0.04400000, 3.90900000));
outputVariable.addTerm(Linear.create("out1mf7", engine.getInputVariables(), 0.01200000, -0.01200000, -0.60000000));
outputVariable.addTerm(Linear.create("out1mf8", engine.getInputVariables(), -0.38400000, 0.06700000, 10.15800000));
outputVariable.addTerm(Linear.create("out1mf9", engine.getInputVariables(), -0.00200000, 0.03500000, -1.40200000));
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

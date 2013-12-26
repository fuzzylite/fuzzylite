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

public class membrn1{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("anfis");

InputVariable inputVariable1 = new InputVariable();
inputVariable1.setEnabled(true);
inputVariable1.setName("in_n1");
inputVariable1.setRange(1.00000000, 31.00000000);
inputVariable1.addTerm(new Bell("in1mf1", 2.25300000, 16.22000000, 5.05000000));
inputVariable1.addTerm(new Bell("in1mf2", 31.26000000, 15.02100000, 1.84300000));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("in_n2");
inputVariable2.setRange(1.00000000, 31.00000000);
inputVariable2.addTerm(new Bell("in2mf1", 0.74000000, 15.02100000, 1.84300000));
inputVariable2.addTerm(new Bell("in2mf2", 29.74700000, 16.22000000, 5.05000000));
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
outputVariable.addTerm(Linear.create("out1mf1", engine.getInputVariables(), 0.02600000, 0.07100000, -0.61500000));
outputVariable.addTerm(Linear.create("out1mf2", engine.getInputVariables(), -0.03600000, 0.03600000, -1.16900000));
outputVariable.addTerm(Linear.create("out1mf3", engine.getInputVariables(), -0.09400000, 0.09400000, 2.23100000));
outputVariable.addTerm(Linear.create("out1mf4", engine.getInputVariables(), -0.07100000, -0.02600000, 2.47900000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new AlgebraicProduct());
ruleBlock.addRule(Rule.parse("if in_n1 is in1mf1 and in_n2 is in2mf1 then out1 is out1mf1", engine));
ruleBlock.addRule(Rule.parse("if in_n1 is in1mf1 and in_n2 is in2mf2 then out1 is out1mf2", engine));
ruleBlock.addRule(Rule.parse("if in_n1 is in1mf2 and in_n2 is in2mf1 then out1 is out1mf3", engine));
ruleBlock.addRule(Rule.parse("if in_n1 is in1mf2 and in_n2 is in2mf2 then out1 is out1mf4", engine));
engine.addRuleBlock(ruleBlock);


}
}

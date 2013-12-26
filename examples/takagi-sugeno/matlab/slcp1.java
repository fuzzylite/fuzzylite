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

public class slcp1{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("slcp1");

InputVariable inputVariable1 = new InputVariable();
inputVariable1.setEnabled(true);
inputVariable1.setName("in1");
inputVariable1.setRange(-0.30000000, 0.30000000);
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("in2");
inputVariable2.setRange(-1.00000000, 1.00000000);
engine.addInputVariable(inputVariable2);

InputVariable inputVariable3 = new InputVariable();
inputVariable3.setEnabled(true);
inputVariable3.setName("in3");
inputVariable3.setRange(-3.00000000, 3.00000000);
engine.addInputVariable(inputVariable3);

InputVariable inputVariable4 = new InputVariable();
inputVariable4.setEnabled(true);
inputVariable4.setName("in4");
inputVariable4.setRange(-3.00000000, 3.00000000);
engine.addInputVariable(inputVariable4);

InputVariable inputVariable5 = new InputVariable();
inputVariable5.setEnabled(true);
inputVariable5.setName("in5");
inputVariable5.setRange(0.50000000, 1.50000000);
inputVariable5.addTerm(new Gaussian("small", 0.50000000, 0.20000000));
inputVariable5.addTerm(new Gaussian("medium", 1.00000000, 0.20000000));
inputVariable5.addTerm(new Gaussian("large", 1.50000000, 0.20000000));
engine.addInputVariable(inputVariable5);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("out");
outputVariable.setRange(-10.00000000, 10.00000000);
outputVariable.setLockOutputRange(false);
outputVariable.setLockValidOutput(false);
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setDefuzzifier(new WeightedAverage());
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.addTerm(Linear.create("outmf1", engine.getInputVariables(), 32.16600000, 5.83500000, 3.16200000, 3.75700000, 0.00000000, 0.00000000));
outputVariable.addTerm(Linear.create("outmf2", engine.getInputVariables(), 39.01200000, 9.94700000, 3.16200000, 4.26900000, 0.00000000, 0.00000000));
outputVariable.addTerm(Linear.create("outmf3", engine.getInputVariables(), 45.00900000, 13.98500000, 3.16200000, 4.66600000, 0.00000000, 0.00000000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new AlgebraicProduct());
ruleBlock.addRule(Rule.parse("if in5 is small then out is outmf1", engine));
ruleBlock.addRule(Rule.parse("if in5 is medium then out is outmf2", engine));
ruleBlock.addRule(Rule.parse("if in5 is large then out is outmf3", engine));
engine.addRuleBlock(ruleBlock);


}
}

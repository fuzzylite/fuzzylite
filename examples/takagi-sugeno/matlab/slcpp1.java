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

public class slcpp1{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("slcpp");

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
inputVariable5.setRange(-3.00000000, 3.00000000);
engine.addInputVariable(inputVariable5);

InputVariable inputVariable6 = new InputVariable();
inputVariable6.setEnabled(true);
inputVariable6.setName("in6");
inputVariable6.setRange(-3.00000000, 3.00000000);
engine.addInputVariable(inputVariable6);

InputVariable inputVariable7 = new InputVariable();
inputVariable7.setEnabled(true);
inputVariable7.setName("pole_length");
inputVariable7.setRange(0.50000000, 1.50000000);
inputVariable7.addTerm(new ZShape("mf1", 0.50000000, 0.60000000));
inputVariable7.addTerm(new PiShape("mf2", 0.50000000, 0.60000000, 0.60000000, 0.70000000));
inputVariable7.addTerm(new PiShape("mf3", 0.60000000, 0.70000000, 0.70000000, 0.80000000));
inputVariable7.addTerm(new PiShape("mf4", 0.70000000, 0.80000000, 0.80000000, 0.90000000));
inputVariable7.addTerm(new PiShape("mf5", 0.80000000, 0.90000000, 0.90000000, 1.00000000));
inputVariable7.addTerm(new PiShape("mf6", 0.90000000, 1.00000000, 1.00000000, 1.10000000));
inputVariable7.addTerm(new PiShape("mf7", 1.00000000, 1.10000000, 1.10000000, 1.20000000));
inputVariable7.addTerm(new PiShape("mf8", 1.10000000, 1.20000000, 1.20000000, 1.30000000));
inputVariable7.addTerm(new PiShape("mf9", 1.20000000, 1.30000000, 1.30000000, 1.40000000));
inputVariable7.addTerm(new PiShape("mf10", 1.30000000, 1.40000000, 1.40000000, 1.50000000));
inputVariable7.addTerm(new SShape("mf11", 1.40000000, 1.50000000));
engine.addInputVariable(inputVariable7);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("out");
outputVariable.setRange(-10.00000000, 10.00000000);
outputVariable.setLockOutputRange(false);
outputVariable.setLockValidOutput(false);
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setDefuzzifier(new WeightedAverage());
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.addTerm(Linear.create("outmf1", engine.getInputVariables(), 168.40000000, 31.00000000, -188.05000000, -49.25000000, -1.00000000, -2.70000000, 0.00000000, 0.00000000));
outputVariable.addTerm(Linear.create("outmf2", engine.getInputVariables(), 233.95000000, 47.19000000, -254.52000000, -66.58000000, -1.00000000, -2.74000000, 0.00000000, 0.00000000));
outputVariable.addTerm(Linear.create("outmf3", engine.getInputVariables(), 342.94000000, 74.73000000, -364.37000000, -95.23000000, -1.00000000, -2.78000000, 0.00000000, 0.00000000));
outputVariable.addTerm(Linear.create("outmf4", engine.getInputVariables(), 560.71000000, 130.67000000, -582.96000000, -152.24000000, -1.00000000, -2.81000000, 0.00000000, 0.00000000));
outputVariable.addTerm(Linear.create("outmf5", engine.getInputVariables(), 1213.88000000, 300.19000000, -1236.90000000, -322.80000000, -1.00000000, -2.84000000, 0.00000000, 0.00000000));
outputVariable.addTerm(Linear.create("outmf6", engine.getInputVariables(), 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000));
outputVariable.addTerm(Linear.create("outmf7", engine.getInputVariables(), -1399.12000000, -382.95000000, 1374.63000000, 358.34000000, -1.00000000, -2.90000000, 0.00000000, 0.00000000));
outputVariable.addTerm(Linear.create("outmf8", engine.getInputVariables(), -746.07000000, -213.42000000, 720.90000000, 187.84000000, -1.00000000, -2.93000000, 0.00000000, 0.00000000));
outputVariable.addTerm(Linear.create("outmf9", engine.getInputVariables(), -528.52000000, -157.46000000, 502.68000000, 130.92000000, -1.00000000, -2.96000000, 0.00000000, 0.00000000));
outputVariable.addTerm(Linear.create("outmf10", engine.getInputVariables(), -419.87000000, -129.89000000, 393.38000000, 102.41000000, -1.00000000, -2.98000000, 0.00000000, 0.00000000));
outputVariable.addTerm(Linear.create("outmf11", engine.getInputVariables(), -354.77000000, -113.68000000, 327.65000000, 85.27000000, -1.00000000, -3.01000000, 0.00000000, 0.00000000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new AlgebraicProduct());
ruleBlock.addRule(Rule.parse("if pole_length is mf1 then out is outmf1", engine));
ruleBlock.addRule(Rule.parse("if pole_length is mf2 then out is outmf2", engine));
ruleBlock.addRule(Rule.parse("if pole_length is mf3 then out is outmf3", engine));
ruleBlock.addRule(Rule.parse("if pole_length is mf4 then out is outmf4", engine));
ruleBlock.addRule(Rule.parse("if pole_length is mf5 then out is outmf5", engine));
ruleBlock.addRule(Rule.parse("if pole_length is mf6 then out is outmf6", engine));
ruleBlock.addRule(Rule.parse("if pole_length is mf7 then out is outmf7", engine));
ruleBlock.addRule(Rule.parse("if pole_length is mf8 then out is outmf8", engine));
ruleBlock.addRule(Rule.parse("if pole_length is mf9 then out is outmf9", engine));
ruleBlock.addRule(Rule.parse("if pole_length is mf10 then out is outmf10", engine));
ruleBlock.addRule(Rule.parse("if pole_length is mf11 then out is outmf11", engine));
engine.addRuleBlock(ruleBlock);


}
}

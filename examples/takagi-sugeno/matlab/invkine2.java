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

public class invkine2{
public static void main(String[] args){
//Code automatically generated with fuzzylite 6.0.

Engine engine = new Engine();
engine.setName("invkine2");
engine.setDescription("");

InputVariable input1 = new InputVariable();
input1.setName("input1");
input1.setDescription("");
input1.setEnabled(true);
input1.setRange(-6.287, 17.000);
input1.setLockValueInRange(false);
input1.addTerm(new Bell("in1mf1", -5.763, 3.015, 1.851));
input1.addTerm(new Bell("in1mf2", -1.624, 3.130, 2.111));
input1.addTerm(new Bell("in1mf3", 3.552, 3.193, 2.104));
input1.addTerm(new Bell("in1mf4", 8.273, 2.907, 1.985));
input1.addTerm(new Bell("in1mf5", 13.232, 2.708, 2.056));
input1.addTerm(new Bell("in1mf6", 17.783, 1.635, 1.897));
engine.addInputVariable(input1);

InputVariable input2 = new InputVariable();
input2.setName("input2");
input2.setDescription("");
input2.setEnabled(true);
input2.setRange(0.000, 16.972);
input2.setLockValueInRange(false);
input2.addTerm(new Bell("in2mf1", 0.005, 1.877, 1.995));
input2.addTerm(new Bell("in2mf2", 3.312, 2.017, 1.829));
input2.addTerm(new Bell("in2mf3", 6.568, 2.261, 1.793));
input2.addTerm(new Bell("in2mf4", 10.111, 2.741, 1.978));
input2.addTerm(new Bell("in2mf5", 14.952, 2.045, 1.783));
input2.addTerm(new Bell("in2mf6", 17.910, 0.824, 1.734));
engine.addInputVariable(input2);

OutputVariable output = new OutputVariable();
output.setName("output");
output.setDescription("");
output.setEnabled(true);
output.setRange(0.000, 3.100);
output.setLockValueInRange(false);
output.setAggregation(null);
output.setDefuzzifier(new WeightedAverage("TakagiSugeno"));
output.setDefaultValue(Double.NaN);
output.setLockPreviousValue(false);
output.addTerm(Linear.create("out1mf1", engine, -0.048, 1.456, 2.222));
output.addTerm(Linear.create("out1mf2", engine, -0.218, -0.305, 2.042));
output.addTerm(Linear.create("out1mf3", engine, 0.026, -0.141, 3.067));
output.addTerm(Linear.create("out1mf4", engine, 0.052, -0.150, 3.419));
output.addTerm(Linear.create("out1mf5", engine, 0.113, -0.189, 4.339));
output.addTerm(Linear.create("out1mf6", engine, 2.543, 0.361, -2.738));
output.addTerm(Linear.create("out1mf7", engine, 2.517, -6.809, 23.353));
output.addTerm(Linear.create("out1mf8", engine, -0.208, -0.394, 4.472));
output.addTerm(Linear.create("out1mf9", engine, -0.046, -0.300, 4.452));
output.addTerm(Linear.create("out1mf10", engine, -0.006, -0.217, 4.195));
output.addTerm(Linear.create("out1mf11", engine, 0.089, -0.254, 4.992));
output.addTerm(Linear.create("out1mf12", engine, -0.033, 0.103, -2.012));
output.addTerm(Linear.create("out1mf13", engine, 1.355, 1.228, -5.678));
output.addTerm(Linear.create("out1mf14", engine, -0.245, -0.124, 3.753));
output.addTerm(Linear.create("out1mf15", engine, -0.099, -0.111, 3.304));
output.addTerm(Linear.create("out1mf16", engine, -0.052, -0.163, 3.560));
output.addTerm(Linear.create("out1mf17", engine, 0.099, -0.260, 4.662));
output.addTerm(Linear.create("out1mf18", engine, 0.082, -1.849, 31.104));
output.addTerm(Linear.create("out1mf19", engine, 2.180, -2.963, -0.061));
output.addTerm(Linear.create("out1mf20", engine, -0.982, 0.510, 5.657));
output.addTerm(Linear.create("out1mf21", engine, -0.087, -0.179, 3.744));
output.addTerm(Linear.create("out1mf22", engine, -0.124, -0.161, 4.094));
output.addTerm(Linear.create("out1mf23", engine, 0.383, 0.007, -1.559));
output.addTerm(Linear.create("out1mf24", engine, -8.415, 2.083, 5.177));
output.addTerm(Linear.create("out1mf25", engine, 1.721, -15.079, -0.687));
output.addTerm(Linear.create("out1mf26", engine, -1.043, -0.786, 20.510));
output.addTerm(Linear.create("out1mf27", engine, -0.249, -0.396, 6.995));
output.addTerm(Linear.create("out1mf28", engine, -0.076, -0.245, 4.416));
output.addTerm(Linear.create("out1mf29", engine, 0.765, -1.488, 17.384));
output.addTerm(Linear.create("out1mf30", engine, -21.210, -43.022, -2.522));
output.addTerm(Linear.create("out1mf31", engine, -0.661, 3.523, 6.215));
output.addTerm(Linear.create("out1mf32", engine, -1.998, 1.582, 33.256));
output.addTerm(Linear.create("out1mf33", engine, -2.068, 5.673, 6.520));
output.addTerm(Linear.create("out1mf34", engine, -5.044, 7.093, 3.516));
output.addTerm(Linear.create("out1mf35", engine, -46.049, -35.021, -2.926));
output.addTerm(Linear.create("out1mf36", engine, -0.448, -0.770, -0.041));
engine.addOutputVariable(output);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setName("");
ruleBlock.setDescription("");
ruleBlock.setEnabled(true);
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(null);
ruleBlock.setImplication(null);
ruleBlock.setActivation(new General());
ruleBlock.addRule(Rule.parse("if input1 is in1mf1 and input2 is in2mf1 then output is out1mf1", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf1 and input2 is in2mf2 then output is out1mf2", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf1 and input2 is in2mf3 then output is out1mf3", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf1 and input2 is in2mf4 then output is out1mf4", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf1 and input2 is in2mf5 then output is out1mf5", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf1 and input2 is in2mf6 then output is out1mf6", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf2 and input2 is in2mf1 then output is out1mf7", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf2 and input2 is in2mf2 then output is out1mf8", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf2 and input2 is in2mf3 then output is out1mf9", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf2 and input2 is in2mf4 then output is out1mf10", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf2 and input2 is in2mf5 then output is out1mf11", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf2 and input2 is in2mf6 then output is out1mf12", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf3 and input2 is in2mf1 then output is out1mf13", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf3 and input2 is in2mf2 then output is out1mf14", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf3 and input2 is in2mf3 then output is out1mf15", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf3 and input2 is in2mf4 then output is out1mf16", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf3 and input2 is in2mf5 then output is out1mf17", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf3 and input2 is in2mf6 then output is out1mf18", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf4 and input2 is in2mf1 then output is out1mf19", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf4 and input2 is in2mf2 then output is out1mf20", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf4 and input2 is in2mf3 then output is out1mf21", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf4 and input2 is in2mf4 then output is out1mf22", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf4 and input2 is in2mf5 then output is out1mf23", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf4 and input2 is in2mf6 then output is out1mf24", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf5 and input2 is in2mf1 then output is out1mf25", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf5 and input2 is in2mf2 then output is out1mf26", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf5 and input2 is in2mf3 then output is out1mf27", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf5 and input2 is in2mf4 then output is out1mf28", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf5 and input2 is in2mf5 then output is out1mf29", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf5 and input2 is in2mf6 then output is out1mf30", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf6 and input2 is in2mf1 then output is out1mf31", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf6 and input2 is in2mf2 then output is out1mf32", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf6 and input2 is in2mf3 then output is out1mf33", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf6 and input2 is in2mf4 then output is out1mf34", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf6 and input2 is in2mf5 then output is out1mf35", engine));
ruleBlock.addRule(Rule.parse("if input1 is in1mf6 and input2 is in2mf6 then output is out1mf36", engine));
engine.addRuleBlock(ruleBlock);


}
}

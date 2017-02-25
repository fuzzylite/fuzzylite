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

public class membrn2{
public static void main(String[] args){
//Code automatically generated with fuzzylite 6.0.

Engine engine = new Engine();
engine.setName("membrn2");
engine.setDescription("");

InputVariable in_n1 = new InputVariable();
in_n1.setName("in_n1");
in_n1.setDescription("");
in_n1.setEnabled(true);
in_n1.setRange(1.000, 31.000);
in_n1.setLockValueInRange(false);
in_n1.addTerm(new Bell("in1mf1", 1.152, 8.206, 0.874));
in_n1.addTerm(new Bell("in1mf2", 15.882, 7.078, 0.444));
in_n1.addTerm(new Bell("in1mf3", 30.575, 8.602, 0.818));
engine.addInputVariable(in_n1);

InputVariable in_n2 = new InputVariable();
in_n2.setName("in_n2");
in_n2.setDescription("");
in_n2.setEnabled(true);
in_n2.setRange(1.000, 31.000);
in_n2.setLockValueInRange(false);
in_n2.addTerm(new Bell("in2mf1", 1.426, 8.602, 0.818));
in_n2.addTerm(new Bell("in2mf2", 16.118, 7.078, 0.445));
in_n2.addTerm(new Bell("in2mf3", 30.847, 8.206, 0.875));
engine.addInputVariable(in_n2);

OutputVariable out1 = new OutputVariable();
out1.setName("out1");
out1.setDescription("");
out1.setEnabled(true);
out1.setRange(-0.334, 1.000);
out1.setLockValueInRange(false);
out1.setAggregation(null);
out1.setDefuzzifier(new WeightedAverage("TakagiSugeno"));
out1.setDefaultValue(Double.NaN);
out1.setLockPreviousValue(false);
out1.addTerm(Linear.create("out1mf1", engine, -0.035, 0.002, -0.352));
out1.addTerm(Linear.create("out1mf2", engine, 0.044, 0.079, -0.028));
out1.addTerm(Linear.create("out1mf3", engine, -0.024, 0.024, -1.599));
out1.addTerm(Linear.create("out1mf4", engine, -0.067, 0.384, 0.007));
out1.addTerm(Linear.create("out1mf5", engine, 0.351, -0.351, -3.663));
out1.addTerm(Linear.create("out1mf6", engine, -0.079, -0.044, 3.909));
out1.addTerm(Linear.create("out1mf7", engine, 0.012, -0.012, -0.600));
out1.addTerm(Linear.create("out1mf8", engine, -0.384, 0.067, 10.158));
out1.addTerm(Linear.create("out1mf9", engine, -0.002, 0.035, -1.402));
engine.addOutputVariable(out1);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setName("");
ruleBlock.setDescription("");
ruleBlock.setEnabled(true);
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(null);
ruleBlock.setImplication(null);
ruleBlock.setActivation(new General());
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

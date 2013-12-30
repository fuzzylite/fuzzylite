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

public class approximation{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("approximation of sin(x)/x");

InputVariable inputVariable = new InputVariable();
inputVariable.setEnabled(true);
inputVariable.setName("inputX");
inputVariable.setRange(0.000, 10.000);
inputVariable.addTerm(new Triangle("NEAR_1", 0.000, 1.000, 2.000));
inputVariable.addTerm(new Triangle("NEAR_2", 1.000, 2.000, 3.000));
inputVariable.addTerm(new Triangle("NEAR_3", 2.000, 3.000, 4.000));
inputVariable.addTerm(new Triangle("NEAR_4", 3.000, 4.000, 5.000));
inputVariable.addTerm(new Triangle("NEAR_5", 4.000, 5.000, 6.000));
inputVariable.addTerm(new Triangle("NEAR_6", 5.000, 6.000, 7.000));
inputVariable.addTerm(new Triangle("NEAR_7", 6.000, 7.000, 8.000));
inputVariable.addTerm(new Triangle("NEAR_8", 7.000, 8.000, 9.000));
inputVariable.addTerm(new Triangle("NEAR_9", 8.000, 9.000, 10.000));
engine.addInputVariable(inputVariable);

OutputVariable outputVariable1 = new OutputVariable();
outputVariable1.setEnabled(true);
outputVariable1.setName("outputFx");
outputVariable1.setRange(-1.000, 1.000);
outputVariable1.fuzzyOutput().setAccumulation(null);
outputVariable1.setDefuzzifier(new WeightedAverage());
outputVariable1.setDefaultValue(Double.NaN);
outputVariable1.setLockValidOutput(true);
outputVariable1.setLockOutputRange(false);
outputVariable1.addTerm(new Constant("f1", 0.840));
outputVariable1.addTerm(new Constant("f2", 0.450));
outputVariable1.addTerm(new Constant("f3", 0.040));
outputVariable1.addTerm(new Constant("f4", -0.180));
outputVariable1.addTerm(new Constant("f5", -0.190));
outputVariable1.addTerm(new Constant("f6", -0.040));
outputVariable1.addTerm(new Constant("f7", 0.090));
outputVariable1.addTerm(new Constant("f8", 0.120));
outputVariable1.addTerm(new Constant("f9", 0.040));
engine.addOutputVariable(outputVariable1);

OutputVariable outputVariable2 = new OutputVariable();
outputVariable2.setEnabled(true);
outputVariable2.setName("trueFx");
outputVariable2.setRange(-1.000, 1.000);
outputVariable2.fuzzyOutput().setAccumulation(null);
outputVariable2.setDefuzzifier(new WeightedAverage());
outputVariable2.setDefaultValue(Double.NaN);
outputVariable2.setLockValidOutput(true);
outputVariable2.setLockOutputRange(false);
outputVariable2.addTerm(Function.create("fx", "sin(inputX)/inputX", engine, true));
engine.addOutputVariable(outputVariable2);

OutputVariable outputVariable3 = new OutputVariable();
outputVariable3.setEnabled(true);
outputVariable3.setName("diffFx");
outputVariable3.setRange(-1.000, 1.000);
outputVariable3.fuzzyOutput().setAccumulation(null);
outputVariable3.setDefuzzifier(new WeightedAverage());
outputVariable3.setDefaultValue(Double.NaN);
outputVariable3.setLockValidOutput(false);
outputVariable3.setLockOutputRange(false);
outputVariable3.addTerm(Function.create("diff", "fabs(outputFx-trueFx)", engine, true));
engine.addOutputVariable(outputVariable3);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(null);
ruleBlock.setDisjunction(null);
ruleBlock.setActivation(new AlgebraicProduct());
ruleBlock.addRule(Rule.parse("if inputX is NEAR_1 then outputFx is f1", engine));
ruleBlock.addRule(Rule.parse("if inputX is NEAR_2 then outputFx is f2", engine));
ruleBlock.addRule(Rule.parse("if inputX is NEAR_3 then outputFx is f3", engine));
ruleBlock.addRule(Rule.parse("if inputX is NEAR_4 then outputFx is f4", engine));
ruleBlock.addRule(Rule.parse("if inputX is NEAR_5 then outputFx is f5", engine));
ruleBlock.addRule(Rule.parse("if inputX is NEAR_6 then outputFx is f6", engine));
ruleBlock.addRule(Rule.parse("if inputX is NEAR_7 then outputFx is f7", engine));
ruleBlock.addRule(Rule.parse("if inputX is NEAR_8 then outputFx is f8", engine));
ruleBlock.addRule(Rule.parse("if inputX is NEAR_9 then outputFx is f9", engine));
ruleBlock.addRule(Rule.parse("if inputX is any  then trueFx is fx and diffFx is diff", engine));
engine.addRuleBlock(ruleBlock);


}
}

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

public class linear_tip_calculator{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("Linear-Tip-Calculator");

InputVariable inputVariable1 = new InputVariable();
inputVariable1.setEnabled(true);
inputVariable1.setName("FoodQuality");
inputVariable1.setRange(1.000, 10.000);
inputVariable1.addTerm(new Trapezoid("Bad", 0.000, 1.000, 3.000, 7.000));
inputVariable1.addTerm(new Trapezoid("Good", 3.000, 7.000, 10.000, 11.000));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("Service");
inputVariable2.setRange(1.000, 10.000);
inputVariable2.addTerm(new Trapezoid("Bad", 0.000, 1.000, 3.000, 7.000));
inputVariable2.addTerm(new Trapezoid("Good", 3.000, 7.000, 10.000, 11.000));
engine.addInputVariable(inputVariable2);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("Tip");
outputVariable.setRange(10.000, 20.000);
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.setDefuzzifier(new WeightedAverage());
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setLockValidOutput(false);
outputVariable.setLockOutputRange(false);
outputVariable.addTerm(Linear.create("TenPercent", engine.getInputVariables(), 0.000, 0.000, 10.000));
outputVariable.addTerm(Linear.create("FifteenPercent", engine.getInputVariables(), 0.000, 0.000, 15.000));
outputVariable.addTerm(Linear.create("TwentyPercent", engine.getInputVariables(), 0.000, 0.000, 20.000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new Minimum());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new Minimum());
ruleBlock.addRule(Rule.parse("if FoodQuality is Bad and Service is Bad then Tip is TenPercent", engine));
ruleBlock.addRule(Rule.parse("if FoodQuality is Bad and Service is Good then Tip is FifteenPercent", engine));
ruleBlock.addRule(Rule.parse("if FoodQuality is Good and Service is Bad then Tip is FifteenPercent", engine));
ruleBlock.addRule(Rule.parse("if FoodQuality is Good and Service is Good then Tip is TwentyPercent", engine));
engine.addRuleBlock(ruleBlock);


}
}

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

public class cubic_approximator{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("Cubic-Approximator");

InputVariable inputVariable = new InputVariable();
inputVariable.setEnabled(true);
inputVariable.setName("X");
inputVariable.setRange(-5.000, 5.000);
inputVariable.addTerm(new Triangle("AboutNegFive", -6.000, -5.000, -4.000));
inputVariable.addTerm(new Triangle("AboutNegFour", -5.000, -4.000, -3.000));
inputVariable.addTerm(new Triangle("AboutNegThree", -4.000, -3.000, -2.000));
inputVariable.addTerm(new Triangle("AboutNegTwo", -3.000, -2.000, -1.000));
inputVariable.addTerm(new Triangle("AboutNegOne", -2.000, -1.000, 0.000));
inputVariable.addTerm(new Triangle("AboutZero", -1.000, 0.000, 1.000));
inputVariable.addTerm(new Triangle("AboutOne", 0.000, 1.000, 2.000));
inputVariable.addTerm(new Triangle("AboutTwo", 1.000, 2.000, 3.000));
inputVariable.addTerm(new Triangle("AboutThree", 2.000, 3.000, 4.000));
inputVariable.addTerm(new Triangle("AboutFour", 3.000, 4.000, 5.000));
inputVariable.addTerm(new Triangle("AboutFive", 4.000, 5.000, 6.000));
engine.addInputVariable(inputVariable);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("ApproxXCubed");
outputVariable.setRange(-5.000, 5.000);
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.setDefuzzifier(new WeightedAverage());
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setLockValidOutput(false);
outputVariable.setLockOutputRange(false);
outputVariable.addTerm(Linear.create("TangentatNegFive", engine.getInputVariables(), 75.000, 250.000));
outputVariable.addTerm(Linear.create("TangentatNegFour", engine.getInputVariables(), 48.000, 128.000));
outputVariable.addTerm(Linear.create("TangentatNegThree", engine.getInputVariables(), 27.000, 54.000));
outputVariable.addTerm(Linear.create("TangentatNegTwo", engine.getInputVariables(), 12.000, 16.000));
outputVariable.addTerm(Linear.create("TangentatNegOne", engine.getInputVariables(), 3.000, 2.000));
outputVariable.addTerm(Linear.create("TangentatZero", engine.getInputVariables(), 0.000, 0.000));
outputVariable.addTerm(Linear.create("TangentatOne", engine.getInputVariables(), 3.000, -2.000));
outputVariable.addTerm(Linear.create("TangentatTwo", engine.getInputVariables(), 12.000, -16.000));
outputVariable.addTerm(Linear.create("TangentatThree", engine.getInputVariables(), 27.000, -54.000));
outputVariable.addTerm(Linear.create("TangentatFour", engine.getInputVariables(), 48.000, -128.000));
outputVariable.addTerm(Linear.create("TangentatFive", engine.getInputVariables(), 75.000, -250.000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new Minimum());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new Minimum());
ruleBlock.addRule(Rule.parse("if X is AboutNegFive then ApproxXCubed is TangentatNegFive", engine));
ruleBlock.addRule(Rule.parse("if X is AboutNegFour then ApproxXCubed is TangentatNegFour", engine));
ruleBlock.addRule(Rule.parse("if X is AboutNegThree then ApproxXCubed is TangentatNegThree", engine));
ruleBlock.addRule(Rule.parse("if X is AboutNegTwo then ApproxXCubed is TangentatNegTwo", engine));
ruleBlock.addRule(Rule.parse("if X is AboutNegOne then ApproxXCubed is TangentatNegOne", engine));
ruleBlock.addRule(Rule.parse("if X is AboutZero then ApproxXCubed is TangentatZero", engine));
ruleBlock.addRule(Rule.parse("if X is AboutOne then ApproxXCubed is TangentatOne", engine));
ruleBlock.addRule(Rule.parse("if X is AboutTwo then ApproxXCubed is TangentatTwo", engine));
ruleBlock.addRule(Rule.parse("if X is AboutThree then ApproxXCubed is TangentatThree", engine));
ruleBlock.addRule(Rule.parse("if X is AboutFour then ApproxXCubed is TangentatFour", engine));
ruleBlock.addRule(Rule.parse("if X is AboutFive then ApproxXCubed is TangentatFive", engine));
engine.addRuleBlock(ruleBlock);


}
}

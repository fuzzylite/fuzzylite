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

public class AllTerms{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("qtfuzzylite");

InputVariable inputVariable = new InputVariable();
inputVariable.setEnabled(true);
inputVariable.setName("AllInputTerms");
inputVariable.setRange(0.00000000, 6.50000000);
inputVariable.addTerm(new Sigmoid("A", 0.50000000, -20.00000000));
inputVariable.addTerm(new ZShape("B", 0.00000000, 1.00000000));
inputVariable.addTerm(new Ramp("C", 1.00000000, 0.00000000));
inputVariable.addTerm(new Triangle("D", 0.50000000, 1.00000000, 1.50000000));
inputVariable.addTerm(new Trapezoid("E", 1.00000000, 1.25000000, 1.75000000, 2.00000000));
inputVariable.addTerm(new Rectangle("F", 1.75000000, 2.25000000));
inputVariable.addTerm(Discrete.create("G", 2.00000000, 0.00000000, 2.25000000, 1.00000000, 2.50000000, 0.50000000, 2.75000000, 1.00000000, 3.00000000, 0.00000000));
inputVariable.addTerm(new Gaussian("H", 3.00000000, 0.20000000));
inputVariable.addTerm(new GaussianProduct("I", 3.50000000, 0.10000000, 3.30000000, 0.30000000));
inputVariable.addTerm(new Bell("J", 4.00000000, 0.25000000, 3.00000000));
inputVariable.addTerm(new PiShape("K", 4.00000000, 4.50000000, 4.50000000, 5.00000000));
inputVariable.addTerm(new SigmoidDifference("L", 4.75000000, 10.00000000, 30.00000000, 5.25000000));
inputVariable.addTerm(new SigmoidProduct("M", 5.25000000, 20.00000000, -10.00000000, 5.75000000));
inputVariable.addTerm(new Ramp("N", 5.50000000, 6.50000000));
inputVariable.addTerm(new SShape("O", 5.50000000, 6.50000000));
inputVariable.addTerm(new Sigmoid("P", 6.00000000, 20.00000000));
engine.addInputVariable(inputVariable);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("AllOutputTerms");
outputVariable.setRange(0.00000000, 6.50000000);
outputVariable.setLockOutputRange(false);
outputVariable.setLockValidOutput(false);
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setDefuzzifier(new Centroid(200));
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.addTerm(new Sigmoid("A", 0.50000000, -20.00000000));
outputVariable.addTerm(new ZShape("B", 0.00000000, 1.00000000));
outputVariable.addTerm(new Ramp("C", 1.00000000, 0.00000000));
outputVariable.addTerm(new Triangle("D", 0.50000000, 1.00000000, 1.50000000));
outputVariable.addTerm(new Trapezoid("E", 1.00000000, 1.25000000, 1.75000000, 2.00000000));
outputVariable.addTerm(new Rectangle("F", 1.75000000, 2.25000000));
outputVariable.addTerm(Discrete.create("G", 2.00000000, 0.00000000, 2.25000000, 1.00000000, 2.50000000, 0.50000000, 2.75000000, 1.00000000, 3.00000000, 0.00000000));
outputVariable.addTerm(new Gaussian("H", 3.00000000, 0.20000000));
outputVariable.addTerm(new GaussianProduct("I", 3.50000000, 0.10000000, 3.30000000, 0.30000000));
outputVariable.addTerm(new Bell("J", 4.00000000, 0.25000000, 3.00000000));
outputVariable.addTerm(new PiShape("K", 4.00000000, 4.50000000, 4.50000000, 5.00000000));
outputVariable.addTerm(new SigmoidDifference("L", 4.75000000, 10.00000000, 30.00000000, 5.25000000));
outputVariable.addTerm(new SigmoidProduct("M", 5.25000000, 20.00000000, -10.00000000, 5.75000000));
outputVariable.addTerm(new Ramp("N", 5.50000000, 6.50000000));
outputVariable.addTerm(new SShape("O", 5.50000000, 6.50000000));
outputVariable.addTerm(new Sigmoid("P", 6.00000000, 20.00000000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new Minimum());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new Minimum());
ruleBlock.addRule(Rule.parse("if AllInputTerms is A then AllOutputTerms is P", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is B then AllOutputTerms is O", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is C then AllOutputTerms is N", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is D then AllOutputTerms is M", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is E then AllOutputTerms is L", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is F then AllOutputTerms is K", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is G then AllOutputTerms is J", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is H then AllOutputTerms is I", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is I then AllOutputTerms is H", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is J then AllOutputTerms is G", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is K then AllOutputTerms is F", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is L then AllOutputTerms is E", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is M then AllOutputTerms is D", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is N then AllOutputTerms is C", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is O then AllOutputTerms is B", engine));
ruleBlock.addRule(Rule.parse("if AllInputTerms is P then AllOutputTerms is A", engine));
engine.addRuleBlock(ruleBlock);


}
}

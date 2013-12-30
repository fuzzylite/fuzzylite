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
inputVariable.setRange(0.000, 6.500);
inputVariable.addTerm(new Sigmoid("A", 0.500, -20.000));
inputVariable.addTerm(new ZShape("B", 0.000, 1.000));
inputVariable.addTerm(new Ramp("C", 1.000, 0.000));
inputVariable.addTerm(new Triangle("D", 0.500, 1.000, 1.500));
inputVariable.addTerm(new Trapezoid("E", 1.000, 1.250, 1.750, 2.000));
inputVariable.addTerm(new Rectangle("F", 1.750, 2.250));
inputVariable.addTerm(Discrete.create("G", 2.000, 0.000, 2.250, 1.000, 2.500, 0.500, 2.750, 1.000, 3.000, 0.000));
inputVariable.addTerm(new Gaussian("H", 3.000, 0.200));
inputVariable.addTerm(new GaussianProduct("I", 3.500, 0.100, 3.300, 0.300));
inputVariable.addTerm(new Bell("J", 4.000, 0.250, 3.000));
inputVariable.addTerm(new PiShape("K", 4.000, 4.500, 4.500, 5.000));
inputVariable.addTerm(new SigmoidDifference("L", 4.750, 10.000, 30.000, 5.250));
inputVariable.addTerm(new SigmoidProduct("M", 5.250, 20.000, -10.000, 5.750));
inputVariable.addTerm(new Ramp("N", 5.500, 6.500));
inputVariable.addTerm(new SShape("O", 5.500, 6.500));
inputVariable.addTerm(new Sigmoid("P", 6.000, 20.000));
engine.addInputVariable(inputVariable);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("AllOutputTerms");
outputVariable.setRange(0.000, 6.500);
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.setDefuzzifier(new Centroid(200));
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setLockValidOutput(false);
outputVariable.setLockOutputRange(false);
outputVariable.addTerm(new Sigmoid("A", 0.500, -20.000));
outputVariable.addTerm(new ZShape("B", 0.000, 1.000));
outputVariable.addTerm(new Ramp("C", 1.000, 0.000));
outputVariable.addTerm(new Triangle("D", 0.500, 1.000, 1.500));
outputVariable.addTerm(new Trapezoid("E", 1.000, 1.250, 1.750, 2.000));
outputVariable.addTerm(new Rectangle("F", 1.750, 2.250));
outputVariable.addTerm(Discrete.create("G", 2.000, 0.000, 2.250, 1.000, 2.500, 0.500, 2.750, 1.000, 3.000, 0.000));
outputVariable.addTerm(new Gaussian("H", 3.000, 0.200));
outputVariable.addTerm(new GaussianProduct("I", 3.500, 0.100, 3.300, 0.300));
outputVariable.addTerm(new Bell("J", 4.000, 0.250, 3.000));
outputVariable.addTerm(new PiShape("K", 4.000, 4.500, 4.500, 5.000));
outputVariable.addTerm(new SigmoidDifference("L", 4.750, 10.000, 30.000, 5.250));
outputVariable.addTerm(new SigmoidProduct("M", 5.250, 20.000, -10.000, 5.750));
outputVariable.addTerm(new Ramp("N", 5.500, 6.500));
outputVariable.addTerm(new SShape("O", 5.500, 6.500));
outputVariable.addTerm(new Sigmoid("P", 6.000, 20.000));
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

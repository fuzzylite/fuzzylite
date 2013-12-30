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

public class mamdani_tip_calculator{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("Mamdani-Tip-Calculator");

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

OutputVariable outputVariable1 = new OutputVariable();
outputVariable1.setEnabled(true);
outputVariable1.setName("Tip");
outputVariable1.setRange(0.000, 30.000);
outputVariable1.fuzzyOutput().setAccumulation(new AlgebraicSum());
outputVariable1.setDefuzzifier(new Centroid(200));
outputVariable1.setDefaultValue(Double.NaN);
outputVariable1.setLockValidOutput(false);
outputVariable1.setLockOutputRange(false);
outputVariable1.addTerm(new Gaussian("AboutTenPercent", 10.000, 2.000));
outputVariable1.addTerm(new Gaussian("AboutFifteenPercent", 15.000, 2.000));
outputVariable1.addTerm(new Gaussian("AboutTwentyPercent", 20.000, 2.000));
engine.addOutputVariable(outputVariable1);

OutputVariable outputVariable2 = new OutputVariable();
outputVariable2.setEnabled(true);
outputVariable2.setName("CheckPlusTip");
outputVariable2.setRange(1.000, 1.300);
outputVariable2.fuzzyOutput().setAccumulation(new AlgebraicSum());
outputVariable2.setDefuzzifier(new Centroid(200));
outputVariable2.setDefaultValue(Double.NaN);
outputVariable2.setLockValidOutput(false);
outputVariable2.setLockOutputRange(false);
outputVariable2.addTerm(new Gaussian("PlusAboutTenPercent", 1.100, 0.020));
outputVariable2.addTerm(new Gaussian("PlusAboutFifteenPercent", 1.150, 0.020));
outputVariable2.addTerm(new Gaussian("PlusAboutTwentyPercent", 1.200, 0.020));
engine.addOutputVariable(outputVariable2);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new Minimum());
ruleBlock.addRule(Rule.parse("if FoodQuality is Bad and Service is Bad then Tip is AboutTenPercent and CheckPlusTip is PlusAboutTenPercent", engine));
ruleBlock.addRule(Rule.parse("if FoodQuality is Bad and Service is Good then Tip is AboutFifteenPercent and CheckPlusTip is PlusAboutFifteenPercent", engine));
ruleBlock.addRule(Rule.parse("if FoodQuality is Good and Service is Bad then Tip is AboutFifteenPercent and CheckPlusTip is PlusAboutFifteenPercent", engine));
ruleBlock.addRule(Rule.parse("if FoodQuality is Good and Service is Good then Tip is AboutTwentyPercent and CheckPlusTip is PlusAboutTwentyPercent", engine));
engine.addRuleBlock(ruleBlock);


}
}

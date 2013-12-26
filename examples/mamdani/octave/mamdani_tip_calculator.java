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
inputVariable1.setRange(1.00000000, 10.00000000);
inputVariable1.addTerm(new Trapezoid("Bad", 0.00000000, 1.00000000, 3.00000000, 7.00000000));
inputVariable1.addTerm(new Trapezoid("Good", 3.00000000, 7.00000000, 10.00000000, 11.00000000));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("Service");
inputVariable2.setRange(1.00000000, 10.00000000);
inputVariable2.addTerm(new Trapezoid("Bad", 0.00000000, 1.00000000, 3.00000000, 7.00000000));
inputVariable2.addTerm(new Trapezoid("Good", 3.00000000, 7.00000000, 10.00000000, 11.00000000));
engine.addInputVariable(inputVariable2);

OutputVariable outputVariable1 = new OutputVariable();
outputVariable1.setEnabled(true);
outputVariable1.setName("Tip");
outputVariable1.setRange(0.00000000, 30.00000000);
outputVariable1.setLockOutputRange(false);
outputVariable1.setLockValidOutput(false);
outputVariable1.setDefaultValue(Double.NaN);
outputVariable1.setDefuzzifier(new Centroid(200));
outputVariable1.fuzzyOutput().setAccumulation(new AlgebraicSum());
outputVariable1.addTerm(new Gaussian("AboutTenPercent", 10.00000000, 2.00000000));
outputVariable1.addTerm(new Gaussian("AboutFifteenPercent", 15.00000000, 2.00000000));
outputVariable1.addTerm(new Gaussian("AboutTwentyPercent", 20.00000000, 2.00000000));
engine.addOutputVariable(outputVariable1);

OutputVariable outputVariable2 = new OutputVariable();
outputVariable2.setEnabled(true);
outputVariable2.setName("CheckPlusTip");
outputVariable2.setRange(1.00000000, 1.30000000);
outputVariable2.setLockOutputRange(false);
outputVariable2.setLockValidOutput(false);
outputVariable2.setDefaultValue(Double.NaN);
outputVariable2.setDefuzzifier(new Centroid(200));
outputVariable2.fuzzyOutput().setAccumulation(new AlgebraicSum());
outputVariable2.addTerm(new Gaussian("PlusAboutTenPercent", 1.10000000, 0.02000000));
outputVariable2.addTerm(new Gaussian("PlusAboutFifteenPercent", 1.15000000, 0.02000000));
outputVariable2.addTerm(new Gaussian("PlusAboutTwentyPercent", 1.20000000, 0.02000000));
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

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

InputVariable FoodQuality = new InputVariable();
FoodQuality.setEnabled(true);
FoodQuality.setName("FoodQuality");
FoodQuality.setRange(1.000, 10.000);
FoodQuality.setLockValueInRange(false);
FoodQuality.addTerm(new Trapezoid("Bad", 0.000, 1.000, 3.000, 7.000));
FoodQuality.addTerm(new Trapezoid("Good", 3.000, 7.000, 10.000, 11.000));
engine.addInputVariable(FoodQuality);

InputVariable Service = new InputVariable();
Service.setEnabled(true);
Service.setName("Service");
Service.setRange(1.000, 10.000);
Service.setLockValueInRange(false);
Service.addTerm(new Trapezoid("Bad", 0.000, 1.000, 3.000, 7.000));
Service.addTerm(new Trapezoid("Good", 3.000, 7.000, 10.000, 11.000));
engine.addInputVariable(Service);

OutputVariable Tip = new OutputVariable();
Tip.setEnabled(true);
Tip.setName("Tip");
Tip.setRange(0.000, 30.000);
Tip.setLockValueInRange(false);
Tip.fuzzyOutput().setAccumulation(new AlgebraicSum());
Tip.setDefuzzifier(new Centroid(200));
Tip.setDefaultValue(Double.NaN);
Tip.setLockPreviousValue(false);
Tip.addTerm(new Gaussian("AboutTenPercent", 10.000, 2.000));
Tip.addTerm(new Gaussian("AboutFifteenPercent", 15.000, 2.000));
Tip.addTerm(new Gaussian("AboutTwentyPercent", 20.000, 2.000));
engine.addOutputVariable(Tip);

OutputVariable CheckPlusTip = new OutputVariable();
CheckPlusTip.setEnabled(true);
CheckPlusTip.setName("CheckPlusTip");
CheckPlusTip.setRange(1.000, 1.300);
CheckPlusTip.setLockValueInRange(false);
CheckPlusTip.fuzzyOutput().setAccumulation(new AlgebraicSum());
CheckPlusTip.setDefuzzifier(new Centroid(200));
CheckPlusTip.setDefaultValue(Double.NaN);
CheckPlusTip.setLockPreviousValue(false);
CheckPlusTip.addTerm(new Gaussian("PlusAboutTenPercent", 1.100, 0.020));
CheckPlusTip.addTerm(new Gaussian("PlusAboutFifteenPercent", 1.150, 0.020));
CheckPlusTip.addTerm(new Gaussian("PlusAboutTwentyPercent", 1.200, 0.020));
engine.addOutputVariable(CheckPlusTip);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setImplication(new Minimum());
ruleBlock.addRule(Rule.parse("if FoodQuality is Bad and Service is Bad then Tip is AboutTenPercent and CheckPlusTip is PlusAboutTenPercent", engine));
ruleBlock.addRule(Rule.parse("if FoodQuality is Bad and Service is Good then Tip is AboutFifteenPercent and CheckPlusTip is PlusAboutFifteenPercent", engine));
ruleBlock.addRule(Rule.parse("if FoodQuality is Good and Service is Bad then Tip is AboutFifteenPercent and CheckPlusTip is PlusAboutFifteenPercent", engine));
ruleBlock.addRule(Rule.parse("if FoodQuality is Good and Service is Good then Tip is AboutTwentyPercent and CheckPlusTip is PlusAboutTwentyPercent", engine));
engine.addRuleBlock(ruleBlock);


}
}

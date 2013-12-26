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

public class investment_portfolio{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("Investment-Portfolio");

InputVariable inputVariable1 = new InputVariable();
inputVariable1.setEnabled(true);
inputVariable1.setName("Age");
inputVariable1.setRange(20.00000000, 100.00000000);
inputVariable1.addTerm(new ZShape("Young", 30.00000000, 90.00000000));
inputVariable1.addTerm(new SShape("Old", 30.00000000, 90.00000000));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("RiskTolerance");
inputVariable2.setRange(0.00000000, 10.00000000);
inputVariable2.addTerm(new ZShape("Low", 2.00000000, 8.00000000));
inputVariable2.addTerm(new SShape("High", 2.00000000, 8.00000000));
engine.addInputVariable(inputVariable2);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("PercentageInStocks");
outputVariable.setRange(0.00000000, 100.00000000);
outputVariable.setLockOutputRange(false);
outputVariable.setLockValidOutput(false);
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setDefuzzifier(new Centroid(200));
outputVariable.fuzzyOutput().setAccumulation(new EinsteinSum());
outputVariable.addTerm(new Gaussian("AboutFifteen", 15.00000000, 10.00000000));
outputVariable.addTerm(new Gaussian("AboutFifty", 50.00000000, 10.00000000));
outputVariable.addTerm(new Gaussian("AboutEightyFive", 85.00000000, 10.00000000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new EinsteinProduct());
ruleBlock.setDisjunction(new EinsteinSum());
ruleBlock.setActivation(new EinsteinProduct());
ruleBlock.addRule(Rule.parse("if Age is Young or RiskTolerance is High then PercentageInStocks is AboutEightyFive", engine));
ruleBlock.addRule(Rule.parse("if Age is Old or RiskTolerance is Low then PercentageInStocks is AboutFifteen", engine));
ruleBlock.addRule(Rule.parse("if Age is not extremely Old and RiskTolerance is not extremely Low then PercentageInStocks is AboutFifty with 0.50000000", engine));
ruleBlock.addRule(Rule.parse("if Age is not extremely Young and RiskTolerance is not extremely High then PercentageInStocks is AboutFifty with 0.50000000", engine));
engine.addRuleBlock(ruleBlock);


}
}

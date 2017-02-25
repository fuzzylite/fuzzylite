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

public class investment_portfolio{
public static void main(String[] args){
//Code automatically generated with fuzzylite 6.0.

Engine engine = new Engine();
engine.setName("investment_portfolio");
engine.setDescription("");

InputVariable Age = new InputVariable();
Age.setName("Age");
Age.setDescription("");
Age.setEnabled(true);
Age.setRange(20.000, 100.000);
Age.setLockValueInRange(false);
Age.addTerm(new ZShape("Young", 30.000, 90.000));
Age.addTerm(new SShape("Old", 30.000, 90.000));
engine.addInputVariable(Age);

InputVariable RiskTolerance = new InputVariable();
RiskTolerance.setName("RiskTolerance");
RiskTolerance.setDescription("");
RiskTolerance.setEnabled(true);
RiskTolerance.setRange(0.000, 10.000);
RiskTolerance.setLockValueInRange(false);
RiskTolerance.addTerm(new ZShape("Low", 2.000, 8.000));
RiskTolerance.addTerm(new SShape("High", 2.000, 8.000));
engine.addInputVariable(RiskTolerance);

OutputVariable PercentageInStocks = new OutputVariable();
PercentageInStocks.setName("PercentageInStocks");
PercentageInStocks.setDescription("");
PercentageInStocks.setEnabled(true);
PercentageInStocks.setRange(0.000, 100.000);
PercentageInStocks.setLockValueInRange(false);
PercentageInStocks.setAggregation(new EinsteinSum());
PercentageInStocks.setDefuzzifier(new Centroid(200));
PercentageInStocks.setDefaultValue(Double.NaN);
PercentageInStocks.setLockPreviousValue(false);
PercentageInStocks.addTerm(new Gaussian("AboutFifteen", 15.000, 10.000));
PercentageInStocks.addTerm(new Gaussian("AboutFifty", 50.000, 10.000));
PercentageInStocks.addTerm(new Gaussian("AboutEightyFive", 85.000, 10.000));
engine.addOutputVariable(PercentageInStocks);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setName("");
ruleBlock.setDescription("");
ruleBlock.setEnabled(true);
ruleBlock.setConjunction(new EinsteinProduct());
ruleBlock.setDisjunction(new EinsteinSum());
ruleBlock.setImplication(new EinsteinProduct());
ruleBlock.setActivation(new General());
ruleBlock.addRule(Rule.parse("if Age is Young or RiskTolerance is High then PercentageInStocks is AboutEightyFive", engine));
ruleBlock.addRule(Rule.parse("if Age is Old or RiskTolerance is Low then PercentageInStocks is AboutFifteen", engine));
ruleBlock.addRule(Rule.parse("if Age is not extremely Old and RiskTolerance is not extremely Low then PercentageInStocks is AboutFifty with 0.500", engine));
ruleBlock.addRule(Rule.parse("if Age is not extremely Young and RiskTolerance is not extremely High then PercentageInStocks is AboutFifty with 0.500", engine));
engine.addRuleBlock(ruleBlock);


}
}

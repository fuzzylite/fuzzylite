#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("Investment-Portfolio");

InputVariable* Age = new InputVariable;
Age->setEnabled(true);
Age->setName("Age");
Age->setRange(20.000, 100.000);
Age->setLockValueInRange(false);
Age->addTerm(new ZShape("Young", 30.000, 90.000));
Age->addTerm(new SShape("Old", 30.000, 90.000));
engine->addInputVariable(Age);

InputVariable* RiskTolerance = new InputVariable;
RiskTolerance->setEnabled(true);
RiskTolerance->setName("RiskTolerance");
RiskTolerance->setRange(0.000, 10.000);
RiskTolerance->setLockValueInRange(false);
RiskTolerance->addTerm(new ZShape("Low", 2.000, 8.000));
RiskTolerance->addTerm(new SShape("High", 2.000, 8.000));
engine->addInputVariable(RiskTolerance);

OutputVariable* PercentageInStocks = new OutputVariable;
PercentageInStocks->setEnabled(true);
PercentageInStocks->setName("PercentageInStocks");
PercentageInStocks->setRange(0.000, 100.000);
PercentageInStocks->setLockValueInRange(false);
PercentageInStocks->fuzzyOutput()->setAccumulation(new EinsteinSum);
PercentageInStocks->setDefuzzifier(new Centroid(200));
PercentageInStocks->setDefaultValue(fl::nan);
PercentageInStocks->setLockPreviousValue(false);
PercentageInStocks->addTerm(new Gaussian("AboutFifteen", 15.000, 10.000));
PercentageInStocks->addTerm(new Gaussian("AboutFifty", 50.000, 10.000));
PercentageInStocks->addTerm(new Gaussian("AboutEightyFive", 85.000, 10.000));
engine->addOutputVariable(PercentageInStocks);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new EinsteinProduct);
ruleBlock->setDisjunction(new EinsteinSum);
ruleBlock->setImplication(new EinsteinProduct);
ruleBlock->addRule(fl::Rule::parse("if Age is Young or RiskTolerance is High then PercentageInStocks is AboutEightyFive", engine));
ruleBlock->addRule(fl::Rule::parse("if Age is Old or RiskTolerance is Low then PercentageInStocks is AboutFifteen", engine));
ruleBlock->addRule(fl::Rule::parse("if Age is not extremely Old and RiskTolerance is not extremely Low then PercentageInStocks is AboutFifty with 0.500", engine));
ruleBlock->addRule(fl::Rule::parse("if Age is not extremely Young and RiskTolerance is not extremely High then PercentageInStocks is AboutFifty with 0.500", engine));
engine->addRuleBlock(ruleBlock);


}

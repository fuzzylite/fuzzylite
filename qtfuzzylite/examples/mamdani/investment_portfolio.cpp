/* 
 * File:   investment_portfolio.cpp
 * Author: jcrada
 *
 * Created on 12 January 2013, 11:14 PM
 */

#include <fl/Headers.h>

int main(int argc, char* argv[]) {
    fl::Engine* engine = new fl::Engine;
    engine->setName("Investment-Portfolio");
    engine->addHedge(new fl::Any);
    engine->addHedge(new fl::Extremely);
    engine->addHedge(new fl::Not);
    engine->addHedge(new fl::Seldom);
    engine->addHedge(new fl::Somewhat);
    engine->addHedge(new fl::Very);

    fl::InputVariable* inputVariable1 = new fl::InputVariable;
    inputVariable1->setName("Age");
    inputVariable1->setRange(20.000, 100.000);

    inputVariable1->addTerm(new fl::ZShape("Young", 30.000, 90.000));
    inputVariable1->addTerm(new fl::SShape("Old", 30.000, 90.000));
    engine->addInputVariable(inputVariable1);

    fl::InputVariable* inputVariable2 = new fl::InputVariable;
    inputVariable2->setName("Risk-Tolerance");
    inputVariable2->setRange(0.000, 10.000);

    inputVariable2->addTerm(new fl::ZShape("Low", 2.000, 8.000));
    inputVariable2->addTerm(new fl::SShape("High", 2.000, 8.000));
    engine->addInputVariable(inputVariable2);

    fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
    outputVariable1->setName("Percentage-In-Stocks");
    outputVariable1->setRange(0.000, 100.000);
    outputVariable1->setDefaultValue(std::numeric_limits<fl::scalar>::quiet_NaN());
    outputVariable1->setLockDefuzzifiedValue(true);
    outputVariable1->setDefuzzifier(new fl::Centroid(500));
    outputVariable1->output()->setAccumulation(new fl::EinsteinSum);

    outputVariable1->addTerm(new fl::Gaussian("About-Fifteen", 15.000, 10.000));
    outputVariable1->addTerm(new fl::Gaussian("About-Fifty", 50.000, 10.000));
    outputVariable1->addTerm(new fl::Gaussian("About-Eighty-Five", 85.000, 10.000));
    engine->addOutputVariable(outputVariable1);

    fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
    ruleblock1->setName("");
    ruleblock1->setTnorm(new fl::EinsteinProduct);
    ruleblock1->setSnorm(new fl::EinsteinSum);
    ruleblock1->setActivation(new fl::EinsteinProduct);

    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if Age is Young or Risk-Tolerance is High  then Percentage-In-Stocks is About-Eighty-Five", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if Age is Old or Risk-Tolerance is Low  then Percentage-In-Stocks is About-Fifteen", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if Age is not extremely Old and Risk-Tolerance is not extremely Low  then Percentage-In-Stocks is About-Fifty with 0.5", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if Age is not extremely Young and Risk-Tolerance is not extremely High  then Percentage-In-Stocks is About-Fifty with 0.5", engine));
    engine->addRuleBlock(ruleblock1);


}

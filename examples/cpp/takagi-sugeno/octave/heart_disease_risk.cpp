#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("Heart-Disease-Risk");
engine->addHedge(new fl::Any);
engine->addHedge(new fl::Extremely);
engine->addHedge(new fl::Not);
engine->addHedge(new fl::Seldom);
engine->addHedge(new fl::Somewhat);
engine->addHedge(new fl::Very);

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setName("LDLLevel");
inputVariable1->setRange(0.000, 300.000);

inputVariable1->addTerm(new fl::Trapezoid("Low", -1.000, 0.000, 90.000, 110.000));
inputVariable1->addTerm(new fl::Trapezoid("LowBorderline", 90.000, 110.000, 120.000, 140.000));
inputVariable1->addTerm(new fl::Trapezoid("Borderline", 120.000, 140.000, 150.000, 170.000));
inputVariable1->addTerm(new fl::Trapezoid("HighBorderline", 150.000, 170.000, 180.000, 200.000));
inputVariable1->addTerm(new fl::Trapezoid("High", 180.000, 200.000, 300.000, 301.000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setName("HDLLevel");
inputVariable2->setRange(0.000, 100.000);

inputVariable2->addTerm(new fl::Trapezoid("LowHDL", -1.000, 0.000, 35.000, 45.000));
inputVariable2->addTerm(new fl::Trapezoid("ModerateHDL", 35.000, 45.000, 55.000, 65.000));
inputVariable2->addTerm(new fl::Trapezoid("HighHDL", 55.000, 65.000, 100.000, 101.000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("HeartDiseaseRisk");
outputVariable1->setRange(0.000, 10.000);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::WeightedAverage());
outputVariable1->output()->setAccumulation(new fl::Maximum);

outputVariable1->addTerm(new fl::Constant("NoRisk", 0.000));
outputVariable1->addTerm(new fl::Constant("LowRisk", 2.500));
outputVariable1->addTerm(new fl::Constant("MediumRisk", 5.000));
outputVariable1->addTerm(new fl::Constant("HighRisk", 7.500));
outputVariable1->addTerm(new fl::Constant("ExtremeRisk", 10.000));
engine->addOutputVariable(outputVariable1);

fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
ruleblock1->setName("");
ruleblock1->setTnorm(new fl::Minimum);
ruleblock1->setSnorm(new fl::Maximum);
ruleblock1->setActivation(new fl::Minimum);

ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is Low and HDLLevel is LowHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is Low and HDLLevel is ModerateHDL then HeartDiseaseRisk is LowRisk", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is Low and HDLLevel is HighHDL then HeartDiseaseRisk is NoRisk", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is LowBorderline and HDLLevel is LowHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is LowBorderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is LowRisk", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is LowBorderline and HDLLevel is HighHDL then HeartDiseaseRisk is LowRisk", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is Borderline and HDLLevel is LowHDL then HeartDiseaseRisk is HighRisk", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is Borderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is Borderline and HDLLevel is HighHDL then HeartDiseaseRisk is LowRisk", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is HighBorderline and HDLLevel is LowHDL then HeartDiseaseRisk is HighRisk", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is HighBorderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is HighRisk", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is HighBorderline and HDLLevel is HighHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is High and HDLLevel is LowHDL then HeartDiseaseRisk is ExtremeRisk", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is High and HDLLevel is ModerateHDL then HeartDiseaseRisk is HighRisk", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if LDLLevel is High and HDLLevel is HighHDL then HeartDiseaseRisk is MediumRisk", engine));
engine->addRuleBlock(ruleblock1);


}

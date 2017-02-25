#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("heart_disease_risk");
engine->setDescription("");

InputVariable* LDLLevel = new InputVariable;
LDLLevel->setName("LDLLevel");
LDLLevel->setDescription("");
LDLLevel->setEnabled(true);
LDLLevel->setRange(0.000, 300.000);
LDLLevel->setLockValueInRange(false);
LDLLevel->addTerm(new Trapezoid("Low", -1.000, 0.000, 90.000, 110.000));
LDLLevel->addTerm(new Trapezoid("LowBorderline", 90.000, 110.000, 120.000, 140.000));
LDLLevel->addTerm(new Trapezoid("Borderline", 120.000, 140.000, 150.000, 170.000));
LDLLevel->addTerm(new Trapezoid("HighBorderline", 150.000, 170.000, 180.000, 200.000));
LDLLevel->addTerm(new Trapezoid("High", 180.000, 200.000, 300.000, 301.000));
engine->addInputVariable(LDLLevel);

InputVariable* HDLLevel = new InputVariable;
HDLLevel->setName("HDLLevel");
HDLLevel->setDescription("");
HDLLevel->setEnabled(true);
HDLLevel->setRange(0.000, 100.000);
HDLLevel->setLockValueInRange(false);
HDLLevel->addTerm(new Trapezoid("LowHDL", -1.000, 0.000, 35.000, 45.000));
HDLLevel->addTerm(new Trapezoid("ModerateHDL", 35.000, 45.000, 55.000, 65.000));
HDLLevel->addTerm(new Trapezoid("HighHDL", 55.000, 65.000, 100.000, 101.000));
engine->addInputVariable(HDLLevel);

OutputVariable* HeartDiseaseRisk = new OutputVariable;
HeartDiseaseRisk->setName("HeartDiseaseRisk");
HeartDiseaseRisk->setDescription("");
HeartDiseaseRisk->setEnabled(true);
HeartDiseaseRisk->setRange(0.000, 10.000);
HeartDiseaseRisk->setLockValueInRange(false);
HeartDiseaseRisk->setAggregation(fl::null);
HeartDiseaseRisk->setDefuzzifier(new WeightedAverage("TakagiSugeno"));
HeartDiseaseRisk->setDefaultValue(fl::nan);
HeartDiseaseRisk->setLockPreviousValue(false);
HeartDiseaseRisk->addTerm(new Constant("NoRisk", 0.000));
HeartDiseaseRisk->addTerm(new Constant("LowRisk", 2.500));
HeartDiseaseRisk->addTerm(new Constant("MediumRisk", 5.000));
HeartDiseaseRisk->addTerm(new Constant("HighRisk", 7.500));
HeartDiseaseRisk->addTerm(new Constant("ExtremeRisk", 10.000));
engine->addOutputVariable(HeartDiseaseRisk);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(new Minimum);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setImplication(fl::null);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if LDLLevel is Low and HDLLevel is LowHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleBlock->addRule(Rule::parse("if LDLLevel is Low and HDLLevel is ModerateHDL then HeartDiseaseRisk is LowRisk", engine));
ruleBlock->addRule(Rule::parse("if LDLLevel is Low and HDLLevel is HighHDL then HeartDiseaseRisk is NoRisk", engine));
ruleBlock->addRule(Rule::parse("if LDLLevel is LowBorderline and HDLLevel is LowHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleBlock->addRule(Rule::parse("if LDLLevel is LowBorderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is LowRisk", engine));
ruleBlock->addRule(Rule::parse("if LDLLevel is LowBorderline and HDLLevel is HighHDL then HeartDiseaseRisk is LowRisk", engine));
ruleBlock->addRule(Rule::parse("if LDLLevel is Borderline and HDLLevel is LowHDL then HeartDiseaseRisk is HighRisk", engine));
ruleBlock->addRule(Rule::parse("if LDLLevel is Borderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleBlock->addRule(Rule::parse("if LDLLevel is Borderline and HDLLevel is HighHDL then HeartDiseaseRisk is LowRisk", engine));
ruleBlock->addRule(Rule::parse("if LDLLevel is HighBorderline and HDLLevel is LowHDL then HeartDiseaseRisk is HighRisk", engine));
ruleBlock->addRule(Rule::parse("if LDLLevel is HighBorderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is HighRisk", engine));
ruleBlock->addRule(Rule::parse("if LDLLevel is HighBorderline and HDLLevel is HighHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleBlock->addRule(Rule::parse("if LDLLevel is High and HDLLevel is LowHDL then HeartDiseaseRisk is ExtremeRisk", engine));
ruleBlock->addRule(Rule::parse("if LDLLevel is High and HDLLevel is ModerateHDL then HeartDiseaseRisk is HighRisk", engine));
ruleBlock->addRule(Rule::parse("if LDLLevel is High and HDLLevel is HighHDL then HeartDiseaseRisk is MediumRisk", engine));
engine->addRuleBlock(ruleBlock);


}

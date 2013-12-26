#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("Heart-Disease-Risk");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("LDLLevel");
inputVariable1->setRange(0.00000000, 300.00000000);
inputVariable1->addTerm(new fl::Trapezoid("Low", -1.00000000, 0.00000000, 90.00000000, 110.00000000));
inputVariable1->addTerm(new fl::Trapezoid("LowBorderline", 90.00000000, 110.00000000, 120.00000000, 140.00000000));
inputVariable1->addTerm(new fl::Trapezoid("Borderline", 120.00000000, 140.00000000, 150.00000000, 170.00000000));
inputVariable1->addTerm(new fl::Trapezoid("HighBorderline", 150.00000000, 170.00000000, 180.00000000, 200.00000000));
inputVariable1->addTerm(new fl::Trapezoid("High", 180.00000000, 200.00000000, 300.00000000, 301.00000000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("HDLLevel");
inputVariable2->setRange(0.00000000, 100.00000000);
inputVariable2->addTerm(new fl::Trapezoid("LowHDL", -1.00000000, 0.00000000, 35.00000000, 45.00000000));
inputVariable2->addTerm(new fl::Trapezoid("ModerateHDL", 35.00000000, 45.00000000, 55.00000000, 65.00000000));
inputVariable2->addTerm(new fl::Trapezoid("HighHDL", 55.00000000, 65.00000000, 100.00000000, 101.00000000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("HeartDiseaseRisk");
outputVariable->setRange(0.00000000, 10.00000000);
outputVariable->setLockOutputRange(false);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setDefuzzifier(new fl::WeightedAverage);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->addTerm(new fl::Constant("NoRisk", 0.00000000));
outputVariable->addTerm(new fl::Constant("LowRisk", 2.50000000));
outputVariable->addTerm(new fl::Constant("MediumRisk", 5.00000000));
outputVariable->addTerm(new fl::Constant("HighRisk", 7.50000000));
outputVariable->addTerm(new fl::Constant("ExtremeRisk", 10.00000000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::Minimum);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::Minimum);
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is Low and HDLLevel is LowHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is Low and HDLLevel is ModerateHDL then HeartDiseaseRisk is LowRisk", engine));
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is Low and HDLLevel is HighHDL then HeartDiseaseRisk is NoRisk", engine));
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is LowBorderline and HDLLevel is LowHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is LowBorderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is LowRisk", engine));
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is LowBorderline and HDLLevel is HighHDL then HeartDiseaseRisk is LowRisk", engine));
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is Borderline and HDLLevel is LowHDL then HeartDiseaseRisk is HighRisk", engine));
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is Borderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is Borderline and HDLLevel is HighHDL then HeartDiseaseRisk is LowRisk", engine));
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is HighBorderline and HDLLevel is LowHDL then HeartDiseaseRisk is HighRisk", engine));
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is HighBorderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is HighRisk", engine));
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is HighBorderline and HDLLevel is HighHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is High and HDLLevel is LowHDL then HeartDiseaseRisk is ExtremeRisk", engine));
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is High and HDLLevel is ModerateHDL then HeartDiseaseRisk is HighRisk", engine));
ruleBlock->addRule(fl::Rule::parse("if LDLLevel is High and HDLLevel is HighHDL then HeartDiseaseRisk is MediumRisk", engine));
engine->addRuleBlock(ruleBlock);


}

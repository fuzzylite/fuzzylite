#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("Heart-Disease-Risk");

InputVariable* inputVariable1 = new InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("LDLLevel");
inputVariable1->setRange(0.000, 300.000);
inputVariable1->addTerm(new Trapezoid("Low", -1.000, 0.000, 90.000, 110.000));
inputVariable1->addTerm(new Trapezoid("LowBorderline", 90.000, 110.000, 120.000, 140.000));
inputVariable1->addTerm(new Trapezoid("Borderline", 120.000, 140.000, 150.000, 170.000));
inputVariable1->addTerm(new Trapezoid("HighBorderline", 150.000, 170.000, 180.000, 200.000));
inputVariable1->addTerm(new Trapezoid("High", 180.000, 200.000, 300.000, 301.000));
engine->addInputVariable(inputVariable1);

InputVariable* inputVariable2 = new InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("HDLLevel");
inputVariable2->setRange(0.000, 100.000);
inputVariable2->addTerm(new Trapezoid("LowHDL", -1.000, 0.000, 35.000, 45.000));
inputVariable2->addTerm(new Trapezoid("ModerateHDL", 35.000, 45.000, 55.000, 65.000));
inputVariable2->addTerm(new Trapezoid("HighHDL", 55.000, 65.000, 100.000, 101.000));
engine->addInputVariable(inputVariable2);

OutputVariable* outputVariable = new OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("HeartDiseaseRisk");
outputVariable->setRange(0.000, 10.000);
outputVariable->fuzzyOutput()->setAccumulation(fl::null);
outputVariable->setDefuzzifier(new WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockPreviousOutputValue(false);
outputVariable->setLockOutputValueInRange(false);
outputVariable->addTerm(new Constant("NoRisk", 0.000));
outputVariable->addTerm(new Constant("LowRisk", 2.500));
outputVariable->addTerm(new Constant("MediumRisk", 5.000));
outputVariable->addTerm(new Constant("HighRisk", 7.500));
outputVariable->addTerm(new Constant("ExtremeRisk", 10.000));
engine->addOutputVariable(outputVariable);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new Minimum);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setActivation(fl::null);
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

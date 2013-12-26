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

public class heart_disease_risk{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("Heart-Disease-Risk");

InputVariable inputVariable1 = new InputVariable();
inputVariable1.setEnabled(true);
inputVariable1.setName("LDLLevel");
inputVariable1.setRange(0.00000000, 300.00000000);
inputVariable1.addTerm(new Trapezoid("Low", -1.00000000, 0.00000000, 90.00000000, 110.00000000));
inputVariable1.addTerm(new Trapezoid("LowBorderline", 90.00000000, 110.00000000, 120.00000000, 140.00000000));
inputVariable1.addTerm(new Trapezoid("Borderline", 120.00000000, 140.00000000, 150.00000000, 170.00000000));
inputVariable1.addTerm(new Trapezoid("HighBorderline", 150.00000000, 170.00000000, 180.00000000, 200.00000000));
inputVariable1.addTerm(new Trapezoid("High", 180.00000000, 200.00000000, 300.00000000, 301.00000000));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("HDLLevel");
inputVariable2.setRange(0.00000000, 100.00000000);
inputVariable2.addTerm(new Trapezoid("LowHDL", -1.00000000, 0.00000000, 35.00000000, 45.00000000));
inputVariable2.addTerm(new Trapezoid("ModerateHDL", 35.00000000, 45.00000000, 55.00000000, 65.00000000));
inputVariable2.addTerm(new Trapezoid("HighHDL", 55.00000000, 65.00000000, 100.00000000, 101.00000000));
engine.addInputVariable(inputVariable2);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("HeartDiseaseRisk");
outputVariable.setRange(0.00000000, 10.00000000);
outputVariable.setLockOutputRange(false);
outputVariable.setLockValidOutput(false);
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setDefuzzifier(new WeightedAverage());
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.addTerm(new Constant("NoRisk", 0.00000000));
outputVariable.addTerm(new Constant("LowRisk", 2.50000000));
outputVariable.addTerm(new Constant("MediumRisk", 5.00000000));
outputVariable.addTerm(new Constant("HighRisk", 7.50000000));
outputVariable.addTerm(new Constant("ExtremeRisk", 10.00000000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new Minimum());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new Minimum());
ruleBlock.addRule(Rule.parse("if LDLLevel is Low and HDLLevel is LowHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleBlock.addRule(Rule.parse("if LDLLevel is Low and HDLLevel is ModerateHDL then HeartDiseaseRisk is LowRisk", engine));
ruleBlock.addRule(Rule.parse("if LDLLevel is Low and HDLLevel is HighHDL then HeartDiseaseRisk is NoRisk", engine));
ruleBlock.addRule(Rule.parse("if LDLLevel is LowBorderline and HDLLevel is LowHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleBlock.addRule(Rule.parse("if LDLLevel is LowBorderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is LowRisk", engine));
ruleBlock.addRule(Rule.parse("if LDLLevel is LowBorderline and HDLLevel is HighHDL then HeartDiseaseRisk is LowRisk", engine));
ruleBlock.addRule(Rule.parse("if LDLLevel is Borderline and HDLLevel is LowHDL then HeartDiseaseRisk is HighRisk", engine));
ruleBlock.addRule(Rule.parse("if LDLLevel is Borderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleBlock.addRule(Rule.parse("if LDLLevel is Borderline and HDLLevel is HighHDL then HeartDiseaseRisk is LowRisk", engine));
ruleBlock.addRule(Rule.parse("if LDLLevel is HighBorderline and HDLLevel is LowHDL then HeartDiseaseRisk is HighRisk", engine));
ruleBlock.addRule(Rule.parse("if LDLLevel is HighBorderline and HDLLevel is ModerateHDL then HeartDiseaseRisk is HighRisk", engine));
ruleBlock.addRule(Rule.parse("if LDLLevel is HighBorderline and HDLLevel is HighHDL then HeartDiseaseRisk is MediumRisk", engine));
ruleBlock.addRule(Rule.parse("if LDLLevel is High and HDLLevel is LowHDL then HeartDiseaseRisk is ExtremeRisk", engine));
ruleBlock.addRule(Rule.parse("if LDLLevel is High and HDLLevel is ModerateHDL then HeartDiseaseRisk is HighRisk", engine));
ruleBlock.addRule(Rule.parse("if LDLLevel is High and HDLLevel is HighHDL then HeartDiseaseRisk is MediumRisk", engine));
engine.addRuleBlock(ruleBlock);


}
}

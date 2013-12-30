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

public class shower{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("shower");

InputVariable inputVariable1 = new InputVariable();
inputVariable1.setEnabled(true);
inputVariable1.setName("temp");
inputVariable1.setRange(-20.000, 20.000);
inputVariable1.addTerm(new Trapezoid("cold", -30.000, -30.000, -15.000, 0.000));
inputVariable1.addTerm(new Triangle("good", -10.000, 0.000, 10.000));
inputVariable1.addTerm(new Trapezoid("hot", 0.000, 15.000, 30.000, 30.000));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("flow");
inputVariable2.setRange(-1.000, 1.000);
inputVariable2.addTerm(new Trapezoid("soft", -3.000, -3.000, -0.800, 0.000));
inputVariable2.addTerm(new Triangle("good", -0.400, 0.000, 0.400));
inputVariable2.addTerm(new Trapezoid("hard", 0.000, 0.800, 3.000, 3.000));
engine.addInputVariable(inputVariable2);

OutputVariable outputVariable1 = new OutputVariable();
outputVariable1.setEnabled(true);
outputVariable1.setName("cold");
outputVariable1.setRange(-1.000, 1.000);
outputVariable1.fuzzyOutput().setAccumulation(new Maximum());
outputVariable1.setDefuzzifier(new Centroid(200));
outputVariable1.setDefaultValue(Double.NaN);
outputVariable1.setLockValidOutput(false);
outputVariable1.setLockOutputRange(false);
outputVariable1.addTerm(new Triangle("closeFast", -1.000, -0.600, -0.300));
outputVariable1.addTerm(new Triangle("closeSlow", -0.600, -0.300, 0.000));
outputVariable1.addTerm(new Triangle("steady", -0.300, 0.000, 0.300));
outputVariable1.addTerm(new Triangle("openSlow", 0.000, 0.300, 0.600));
outputVariable1.addTerm(new Triangle("openFast", 0.300, 0.600, 1.000));
engine.addOutputVariable(outputVariable1);

OutputVariable outputVariable2 = new OutputVariable();
outputVariable2.setEnabled(true);
outputVariable2.setName("hot");
outputVariable2.setRange(-1.000, 1.000);
outputVariable2.fuzzyOutput().setAccumulation(new Maximum());
outputVariable2.setDefuzzifier(new Centroid(200));
outputVariable2.setDefaultValue(Double.NaN);
outputVariable2.setLockValidOutput(false);
outputVariable2.setLockOutputRange(false);
outputVariable2.addTerm(new Triangle("closeFast", -1.000, -0.600, -0.300));
outputVariable2.addTerm(new Triangle("closeSlow", -0.600, -0.300, 0.000));
outputVariable2.addTerm(new Triangle("steady", -0.300, 0.000, 0.300));
outputVariable2.addTerm(new Triangle("openSlow", 0.000, 0.300, 0.600));
outputVariable2.addTerm(new Triangle("openFast", 0.300, 0.600, 1.000));
engine.addOutputVariable(outputVariable2);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new Minimum());
ruleBlock.setDisjunction(new Maximum());
ruleBlock.setActivation(new Minimum());
ruleBlock.addRule(Rule.parse("if temp is cold and flow is soft then cold is openSlow and hot is openFast", engine));
ruleBlock.addRule(Rule.parse("if temp is cold and flow is good then cold is closeSlow and hot is openSlow", engine));
ruleBlock.addRule(Rule.parse("if temp is cold and flow is hard then cold is closeFast and hot is closeSlow", engine));
ruleBlock.addRule(Rule.parse("if temp is good and flow is soft then cold is openSlow and hot is openSlow", engine));
ruleBlock.addRule(Rule.parse("if temp is good and flow is good then cold is steady and hot is steady", engine));
ruleBlock.addRule(Rule.parse("if temp is good and flow is hard then cold is closeSlow and hot is closeSlow", engine));
ruleBlock.addRule(Rule.parse("if temp is hot and flow is soft then cold is openFast and hot is openSlow", engine));
ruleBlock.addRule(Rule.parse("if temp is hot and flow is good then cold is openSlow and hot is closeSlow", engine));
ruleBlock.addRule(Rule.parse("if temp is hot and flow is hard then cold is closeSlow and hot is closeFast", engine));
engine.addRuleBlock(ruleBlock);


}
}

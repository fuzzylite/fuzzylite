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

public class tank2{
public static void main(String[] args){
Engine engine = new Engine();
engine.setName("tank");

InputVariable inputVariable1 = new InputVariable();
inputVariable1.setEnabled(true);
inputVariable1.setName("level");
inputVariable1.setRange(-1.000, 1.000);
inputVariable1.addTerm(new Trapezoid("high", -2.000, -1.000, -0.800, -0.001));
inputVariable1.addTerm(new Triangle("good", -0.150, 0.000, 0.500));
inputVariable1.addTerm(new Trapezoid("low", 0.001, 0.800, 1.000, 1.500));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("change");
inputVariable2.setRange(-0.100, 0.100);
inputVariable2.addTerm(new Trapezoid("falling", -0.140, -0.100, -0.060, 0.000));
inputVariable2.addTerm(new Trapezoid("rising", -0.001, 0.060, 0.100, 0.140));
engine.addInputVariable(inputVariable2);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("valve");
outputVariable.setRange(-1.000, 1.000);
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.setDefuzzifier(new Centroid(200));
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setLockValidOutput(false);
outputVariable.setLockOutputRange(false);
outputVariable.addTerm(new Triangle("close_fast", -1.000, -0.900, -0.800));
outputVariable.addTerm(new Triangle("close_slow", -0.600, -0.500, -0.400));
outputVariable.addTerm(new Triangle("no_change", -0.100, 0.000, 0.100));
outputVariable.addTerm(new Triangle("open_slow", 0.400, 0.500, 0.600));
outputVariable.addTerm(new Triangle("open_fast", 0.800, 0.900, 1.000));
engine.addOutputVariable(outputVariable);

RuleBlock ruleBlock = new RuleBlock();
ruleBlock.setEnabled(true);
ruleBlock.setName("");
ruleBlock.setConjunction(new AlgebraicProduct());
ruleBlock.setDisjunction(new AlgebraicSum());
ruleBlock.setActivation(new AlgebraicProduct());
ruleBlock.addRule(Rule.parse("if level is low then valve is open_fast", engine));
ruleBlock.addRule(Rule.parse("if level is high then valve is close_fast", engine));
ruleBlock.addRule(Rule.parse("if level is good and change is rising then valve is close_slow", engine));
ruleBlock.addRule(Rule.parse("if level is good and change is falling then valve is open_slow", engine));
engine.addRuleBlock(ruleBlock);


}
}

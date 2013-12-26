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
inputVariable1.setRange(-1.00000000, 1.00000000);
inputVariable1.addTerm(new Trapezoid("high", -2.00000000, -1.00000000, -0.80000000, -0.00100000));
inputVariable1.addTerm(new Triangle("good", -0.15000000, 0.00000000, 0.50000000));
inputVariable1.addTerm(new Trapezoid("low", 0.00100000, 0.80000000, 1.00000000, 1.50000000));
engine.addInputVariable(inputVariable1);

InputVariable inputVariable2 = new InputVariable();
inputVariable2.setEnabled(true);
inputVariable2.setName("change");
inputVariable2.setRange(-0.10000000, 0.10000000);
inputVariable2.addTerm(new Trapezoid("falling", -0.14000000, -0.10000000, -0.06000000, 0.00000000));
inputVariable2.addTerm(new Trapezoid("rising", -0.00100000, 0.06000000, 0.10000000, 0.14000000));
engine.addInputVariable(inputVariable2);

OutputVariable outputVariable = new OutputVariable();
outputVariable.setEnabled(true);
outputVariable.setName("valve");
outputVariable.setRange(-1.00000000, 1.00000000);
outputVariable.setLockOutputRange(false);
outputVariable.setLockValidOutput(false);
outputVariable.setDefaultValue(Double.NaN);
outputVariable.setDefuzzifier(new Centroid(200));
outputVariable.fuzzyOutput().setAccumulation(new Maximum());
outputVariable.addTerm(new Triangle("close_fast", -1.00000000, -0.90000000, -0.80000000));
outputVariable.addTerm(new Triangle("close_slow", -0.60000000, -0.50000000, -0.40000000));
outputVariable.addTerm(new Triangle("no_change", -0.10000000, 0.00000000, 0.10000000));
outputVariable.addTerm(new Triangle("open_slow", 0.40000000, 0.50000000, 0.60000000));
outputVariable.addTerm(new Triangle("open_fast", 0.80000000, 0.90000000, 1.00000000));
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

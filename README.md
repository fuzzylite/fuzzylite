fuzzylite &reg; [![Build Status](https://travis-ci.org/fuzzylite/fuzzylite.svg?branch=v5.x)](https://travis-ci.org/fuzzylite/fuzzylite) <img src="https://github.com/fuzzylite/fuzzylite/raw/v5.x/fuzzylite.png" align="right" alt="fuzzylite">
===========

A Fuzzy Logic Control Library in C++
------------------------------------

By: [Juan Rada-Vilela](http://www.fuzzylite.com/jcrada), Ph.D.



***


### Table of Contents
[License](#license) &nbsp;
[Introduction](#introduction) &nbsp;
[Features](#features) &nbsp;
[Example](#example) &nbsp;
[Bulding from source](#building) &nbsp;
[Binaries](#binaries) &nbsp;
[What's next](#whatsnext) &nbsp;
[What's new](#whatsnew) : [General](#new-general), [Macros](#new-macros), [Operation](#new-operation), [Engine](#new-engine), [Input Variables and Output Variables](#new-inoutvars), [Linguistic Terms](#new-terms), [Linear and Discrete Terms](#new-linear-discrete), [Function Term](#new-function), [[T|S]Norms and Hedges](#new-norms-hedges), [Rules](#new-rules), [Rule Blocks](#new-ruleblocks), [Weighted Defuzzifiers](#new-weighted), [Integral Defuzzifiers](#new-integral), [Importers and Exporters](#new-imex), [Examples](#new-examples),  [Console](#new-console), [Fixed Bugs and Leaks](#new-fixes)


### <a name="license">License</a>
`fuzzylite` will **always** be free and open source. However, `fuzzylite` is **no** longer released under the Apache License. Since version 5.0, `fuzzylite` is **dual-licensed** under the [**GNU Lesser General Public License (LGPL) v3.0**](https://www.gnu.org/licenses/lgpl.html) and a **paid commercial license**.


In brief, an important restriction the LGPL imposes on your closed-source application is that **you are no longer allowed to statically link** against `fuzzylite`. If your application requires static linking, **you will need to purchase a commercial license** from FuzzyLite Limited. Please, contact [sales@fuzzylite.com](mailto:sales@fuzzylite.com) for commercial licenses, and refer to the [GNU LGPL](https://www.gnu.org/licenses/lgpl.html) for further information on your rights.

The change of license is an attempt to raise funds in order to be able to work  part-time in the development of the `fuzzylite` family of products, namely `fuzzylite` (C++), `jfuzzylite` (Java), `pyfuzzylite` (Python), and `QtFuzzyLite` (Windows/Linux/Mac).

**There are still many things to do!**

Besides [donations](http://www.fuzzylite.com/donations/), you can significantly contribute by **purchasing a license** of the entirely new [`QtFuzzyLite`](http://www.fuzzylite.com/QtFuzzyLite/) commercial application. In addition, if you require (paid) private support, please contact [jcrada@fuzzylite.com](mailto:jcrada@fuzzylite.com).


***




### <a name="introduction">Introduction</a>
`fuzzylite` is a free and open-source fuzzy logic control library programmed in C++ for multiple platforms (Windows, Linux, Mac, iOS).  Its goal is to allow you to easily create fuzzy logic controllers in a few steps  utilizing object-oriented programming without requiring any third-party libraries.

#### Reference
If you are using `fuzzylite`, please cite the following reference in your article:

Juan Rada-Vilela. fuzzylite: a fuzzy logic control library, 2014. URL http://www.fuzzylite.com.

```bibtex
 @misc{fl::fuzzylite,
 author={Juan Rada-Vilela},
 title={fuzzylite: a fuzzy logic control library},
 url={http://www.fuzzylite.com},
 year={2014}}
```

***

### <a name="features">Features</a>

**Controllers** *Types* (5) Mamdani, Takagi-Sugeno, Larsen, Tsukamoto, Inverse Tsukamoto

**Linguistic terms** *Basic* (4) triangle, trapezoid, rectangle, discrete. *Extended* (9) bell, cosine, gaussian, gaussian product, pi-shape, sigmoid difference, sigmoid product, spike. *Edges* (4) concave, ramp, sigmoid, s-shape, z-shape. *Functions* (3) constant, linear, function.

**Conjunction and Activation** *T-Norm* (7) minimum, algebraic product, bounded difference, drastic product, einstein product, hamacher product, nilpotent minimum.

**Disjunction and Accumulation** *S-Norm* (8) maximum, algebraic sum, bounded sum, normalized sum, drastic sum, einstein sum, hamacher sum, nilpotent maximum.

**Defuzzifiers** *Integral* (5) centroid, bisector, smallest of maximum, largest of maximum, mean of maximum, *Weighted* (2) weighted average, weighted sum.

**Hedges** *Types* (6) any, not, extremely, seldom, somewhat, very.

**Import** *Types* (3) FuzzyLite Language `fll`, Fuzzy Inference System `fis`, Fuzzy Control Language `fcl`.

**Export** *Types* (6) `C++`, `Java`, FuzzyLite Language `fll`, FuzzyLite Dataset `fld`, Fuzzy Inference System `fis`, Fuzzy Control Language `fcl`.

**Examples** (30+) of Mamdani, Takagi-Sugeno and Tsukamoto controllers from `fuzzylite`, Octave and Matlab, each included in the following formats: `C++`, `Java`, `fll`, `fld`, `fis`, and `fcl`.

***

### <a name="example">Example</a>

```cpp
#include "fl/Headers.h"

int main(int argc, char* argv[]){
    using namespace fl;
    Engine* engine = new Engine("simple-dimmer");

    InputVariable* ambient = new InputVariable;
    ambient->setName("Ambient");
    ambient->setRange(0.000, 1.000);
    ambient->addTerm(new Triangle("DARK", 0.000, 0.500));
    ambient->addTerm(new Triangle("MEDIUM", 0.250, 0.750));
    ambient->addTerm(new Triangle("BRIGHT", 0.500, 1.000));
    engine->addInputVariable(ambient);

    OutputVariable* power = new OutputVariable;
    power->setName("Power");
    power->setRange(0.000, 2.000);
    power->setDefaultValue(fl::nan);
    power->addTerm(new Triangle("LOW", 0.000, 1.000));
    power->addTerm(new Triangle("MEDIUM", 0.500, 1.500));
    power->addTerm(new Triangle("HIGH", 1.000, 2.000));
    engine->addOutputVariable(power);

    RuleBlock* ruleblock = new RuleBlock;
    ruleblock->addRule(Rule::parse("if Ambient is DARK then Power is HIGH", engine));
    ruleblock->addRule(Rule::parse("if Ambient is MEDIUM then Power is MEDIUM", engine));
    ruleblock->addRule(Rule::parse("if Ambient is BRIGHT then Power is LOW", engine));
    engine->addRuleBlock(ruleblock);

    engine->configure("", "", "Minimum", "Maximum", "Centroid");

    std::string status;
    if (not engine->isReady(&status))
        throw Exception("Engine not ready. "
            "The following errors were encountered:\n" + status, FL_AT);

    for (int i = 0; i < 50; ++i){
        scalar light = ambient->getMinimum() + i * (ambient->range() / 50);
        ambient->setValue(light);
        engine->process();
        FL_LOG("Ambient.input = " << Op::str(light) << " -> " <<
            "Power.output = " << Op::str(power->getValue()));
    }
}
```

***

### <a name="whatsnext">What's Next?</a>

+ Source code documentation
+ Type-2 Fuzzy Logic Controllers
+ Adaptive Neuro-Fuzzy Inference System (ANFIS)
+ Fuzzy C-means data clustering

***

### <a name="building">Building from Source</a>
Building from source requires you to have CMake installed.

The files [`fuzzylite/build.bat`](/fuzzylite/build.bat) and [`fuzzylite/build.sh`](/fuzzylite/build.sh) are automatic build scripts for Windows and Unix platforms, respectively. The usage of these scripts is presented as follows.

#### Windows
```bash
> build.bat help
Usage:  build.bat [options]
where   [options] can be any of the following:
    all          builds fuzzylite in debug and release mode (default)
    debug        builds fuzzylite in debug mode
    release      builds fuzzylite in release mode
    clean        erases previous builds
    help         shows this information
```

#### Unix
```bash
$ ./build.sh help
Usage:  [bash] ./build.sh [options]
where   [options] can be any of the following:
    all          builds fuzzylite in debug and release mode (default)
    debug        builds fuzzylite in debug mode
    release      builds fuzzylite in release mode
    clean        erases previous builds
    help         shows this information
```

**(important)** After executing the building script, the binaries will be built and stored in the sub-folders `release/bin` and `debug/bin`.

#### Advanced Building Options
For more advanced building options, please check the contents of [`fuzzylite/build.bat`](/fuzzylite/build.bat) or [`fuzzylite/build.sh`](/fuzzylite/build.sh), and the contents of [`fuzzylite/CMakeLists.txt`](/fuzzylite/CMakeLists.txt).

The following building options are available:

`-DFL_USE_FLOAT=ON` builds the binaries using `typedef float fl::scalar` instead of `typedef double fl::scalar` (default is OFF, i.e., double is used)

`-DFL_BACKTRACE=OFF` disables the backtrace information in case of errors (default in Unix platforms is ON, and in Windows platforms is OFF). In Windows, the backtrace information requires the library `dbghelp`, which should be available in your system.

`-DFL_CPP11=ON` builds `fuzzylite` utilizing `C++11` features (default is OFF, i.e., `C++98`)

`-DCMAKE_BUILD_TYPE=[Debug|Release]` sets the mode of your build. You can only build one mode at a time with a single CMake script.

***

### <a name="binaries">Binaries</a>

After building from source, the following are the relevant binaries that will be created in `release` mode. In `debug` mode, binaries will append a `d` at the end of the name (e.g., `fuzzylited.dll`).

#### Windows

- console application: `fuzzylite.exe`
- shared library: `fuzzylite.dll`, `fuzzylite.lib`
- static library: `fuzzylite-static.lib`

#### Linux

- console application: `fuzzylite`
- shared library: `libfuzzylite.so`
- static library: `libfuzzylite.a`

#### Mac

- console application: `fuzzylite`
- shared library: `libfuzzylite.dylib`
- static library: `libfuzzylite.a`


The console application of `fuzzylite` allows you to import and export your controllers. Its usage can be obtained executing the console binary. In addition, the FuzzyLite Interactive Console is activated  when exporting to `fld` without providing an output file. The interactive console allows you to evaluate any controller by manually providing the input the values.


***

### <a name="whatsnew">What's New?</a>
The entire `fuzzylite` library has been thoroughly revised, refactored, validated, and significantly improved. The following sections detail the changes and enhancements of version 5.0. Users of previous versions are **strongly** encouraged to carefully read the list before migrating to version 5.0. Important changes and enhancements are marked as **(important)**, **(VERY important)** and **(EXTREMELY important)**.

#### <a name="new-general">General</a>
+ **(important)** `fuzzylite v5.0` is dual-licensed under the [GNU LGPL v3.0](https://www.gnu.org/licenses/lgpl.html) and a paid commercial license.
+ Support for both `C++98` and `C++11` using the latest features.
+ Support for multiple compilers `g++`, `Clang`, `MSVC`.
+ Refactoring of many classes to improve design and performance.
+ **(important)** Performance improvements of Takagi-Sugeno controllers by over 55% with respect to v4.0 (estimate based on the average performance on the examples included).
+ Smart pointers in many classes take care of automatically deleting objects when setting new pointers (`Accumulated`, `FactoryManager`, `Function`, `OutputVariable`, `Rule`, and `RuleBlock`).
* **(important)** Exceptions are thrown when any of the following `RuleBlock::[conjunction|disjunction|activation]`, `Accumulated::accumulation`, and `OutputVariable::defuzzifier` are required but set to `fl::null`, thereby replacing the operations that would lead to `[signal 11] Segmentation fault` to operations that throw a `fl::Exception` instead.
+ Automatic build script to build `fuzzylite` in `debug` and `release` mode.
+ Binaries of debug libraries are renamed to append the letter `d`  (e.g.,`fuzzylited.dll`,  `libfuzzylited.so`).
+ **(important)** New file [`fuzzylite/src/m/compare.m`](/fuzzylite/src/m/compare.m) <!---check link--> to compare the output values of your `fuzzylite` engines with the evaluation of the same engine in Octave/Matlab.
+ **(important)** There is practically no difference between the output values obtained with `fuzzylite` and those obtained with Octave/Matlab. Based on the examples, the average mean square error (MSE)  between the output values is less than `7.3e-12` (or `0.0000000000073`) due to negligible differences in floating-point arithmetic. The results and comparison can be found in [`examples/examples.mat`](/examples/examples.mat) <!---check link-->.
+ **(important)** Source code of applications based on version 4.0 will most likely not compile with version 5.0.
+ Minor bugs and memory leaks fixed.

####<a name="new-macros">Macros</a>
* **(important)** Added support for `C++11` with smart pointers, method identifiers, move constructors and move operators as follows. For precise information, refer to file [`fuzzylite/fl/fuzzylite.h`](/fuzzylite/fl/fuzzylite.h). <!---check link-->
* **(important)** Macros for identifiers are defined as `FL_IOVERRIDE override`, `FL_IFINAL final`, `FL_IDEFAULT = default`, `FL_IDELETE = delete`, `FL_INOEXCEPT noexcept`, `FL_DEFAULT_COPY(Class)`, `FL_DEFAULT_MOVE(Class)`, and `FL_DEFAULT_COPY_AND_MOVE(Class)`.
* **(important)** Added macro `FL_unique_ptr` to refer to `std::auto_ptr` (`C++98`) or `std::unique_ptr` (`C++11`), and its respective `FL_move_ptr(x)` to move a smart pointer, albeit `FL_move_ptr(x)` is not used within the library as it is not needed.
* **(important)** Added global variables `const long fl::null = 0L` to refer to the null pointer in `C++98` and `const std::nullptr_t null = nullptr` to refer to the null pointer in `C++11`.
* **(important)** Renamed macro `FL_DEBUG` to `FL_DBG`.
* **(important)**  Renamed macros `FL_BEGIN_DEBUG_BLOCK` and `FL_END_DEBUG_BLOCK` to `FL_DEBUG_BEGIN` and `FL_DEBUG_END`, respectively.
* **(important)** Renamed macro `FL_EXPORT` to `FL_API`
* **(EXTREMELY important)** Added macro definitions `FL_EXPORT_LIBRARY` and `FL_IMPORT_LIBRARY`. If you are building `fuzzylite` as a **shared library**, you need to define `FL_EXPORT_LIBRARY`. If you are building `fuzzylite` **executable** *and* it utilizes the `fuzzylite` **shared library**, you need to define `FL_IMPORT_LIBRARY`. If you are building `fuzzylite` as a **static library** and/or building `fuzzylite` **executable** using the `fuzzylite` **static library**, then you do not need to define either `FL_[IMPORT|EXPORT]_LIBRARY`. Note that the same conditions apply for your application. This is particularly important in Windows platforms, as  `FL_IMPORT_LIBRARY` and `FL_EXPORT_LIBRARY` define `FL_API` to `__declspec(dllimport)` and `__declspec(dllexport)`, respectively. If in doubt, please check [`fuzzylite/CMakeLists.txt`](/fuzzylite/CMakeLists.txt)<!---check link-->

#### <a name="new-operation">Operation</a>
* **(important)** Added method  `Operation::toScalar(std::string x, scalar alternative) FL_INOEXCEPT` which returns `alternative` if `x` is not a valid `scalar`, and never throws an exception.
* **(important)** Added method `Operation::toScalar(std::string x)` that returns the scalar value of `x` or throws a `fl::Exception` if `x` is not a valid `scalar`.
* **(VERY important)** Marked method (from v4.0) `Operation::toScalar(std::string, bool, scalar)  FL_IDELETE;` and removed its implementation such that its usage is prevented at compile time in `C++11` and at linker time in `C++98`. Please, use the appropriate `Op::toScalar` methods mentioned above.
+ Renamed method `Op::isNan` to `Op::isNaN`.
* Added method `fl::Op::isFinite(x)` which returns `not (isNaN(x) or isInf(x))`.
* Changed `fl::Op::isEq(a,b)` to return `true` if `a == b == NaN`.
* Changed `fl::Op::isEq(a,b)` to return `true` if `a == b == Inf`.
* Changes to `fl::Op::isEq` affect other comparison methods `fl::Op::is[Lt|LEq|GEq|Gt]`.
* Added shortcuts of comparisons `fl::Op::[gt,lt,ge,le,eq,neq](scalar a, scalar b)`, mostly to provide binary versions (without `macheps`) for term `Function`.
* Deleted method `Op::repeat`.
* Removed method `fuzzylite::configuration()`.
* Changed default  `fuzzylite::_macheps = 1e-6;`.
* Renamed method `Op::makeValidId` to `Op::validName`, which now returns `"unnamed"` for empty strings.



####<a name="new-engine">Engine</a>
* **(VERY important)** Moved `Engine::hedges` (and relevant methods) to `Rule::hedges`.
* Added enumeration for the type of engine:  `enum Engine::Type{Mamdani, Larsen, TakagiSugeno, Tsukamoto, InverseTsukamoto, Hybrid, Unknown}`.
* Added method `Type Engine::type(std::string* name, std::string* reason)` to infer the type of the engine based on its configuration, additionally provides the name of the type and the inference reasons for its type.
* **(important)**  Changed method `Engine::isReady(std::string)` to satisfy the default operation of controllers. The method `Engine::isReady(std::string)` was initially conceived to provide information before a potential `[signal 11] Segmentation fault` happened due to improper configuration. However, given the improved handling of signals and exceptions mentioned in Section [General](#new-general), using method `Engine::isReady(std::string)` is not necessary except for providing suggestions of what could potentially cause an error.
* Added methods `Engine::set[Input|Output]Variables(std::vector)` and `Engine::setRuleBlocks(std::vector)`.
* Added methods `Engine::[input|output]Variables()` and `Engine::ruleBlocks()` to return mutable references.
* Added method `Engine::variables()` to retrieve a  vector (copy) containing  the `InputVariables` followed by the `OutputVariables`.
* Added method `Engine::updateReferences()` to update the references to the engine in all the necessary linguistic terms (i.e., `Linear` and `Function`).
* Added method `Engine::clone()`.
* Added copy constructors, assignment operators, and default move constructor to `Engine`.



####<a name="new-inoutvars">Input Variables and Output Variables</a>
* **(important)** Added methods `OutputVariable::[get|set]OutputValue()` to [retrieve|store] value from defuzzification
* **(important)** Changed return type of `scalar OutputVariable::defuzzify()` to `void OutputVariable::defuzzify()` because now it automatically stores the defuzzified output value, and also stores the previous *valid* output value regardless of locks. Like in version 4.0, if  `OutputVariable::lockPreviousOutputValue=true`, and the defuzzified output value is not valid (i.e., `[NaN|Inf]`) or no rules were activated, then the defuzzified output value is replaced for the previous valid output value.
* Removed method `OutputVariable::defuzzifyNoLocks()`.
* Renamed variable `OutputVariable::lastValidOutputValue` to `OutputVariable::previousOutputValue`.
* **(important)** Renamed method `OutputVariable::[get|set]LastValidOutput()` to `OutputVariable::[get|set]PreviousOutputValue()`.
* Renamed variable `OutputVariable::lockValidOutput` to `OutputVariable::lockPreviousOutputValue`.
* **(important)** Renamed method `OutputVariable::setLockValidOutput()` to `OutputVariable::setLockPreviousOutputValue()`.
* **(important)** Renamed method `OutputVariable::isLockingValidOutput()` to `OutputVariable::isLockedPreviousOutputValue()`.
* Renamed variable `OutputVariable::lockOutputRange` to `OutputVariable::lockOutputValueInRange`.
* **(important)** Renamed method `OutputVariable::setLockOutputRange()` to `OutputVariable::setLockOutputValueInRange()`.
* **(important)** Renamed method `OutputVariable::isLockingOutputRange()` to `OutputVariable::isLockedOutputValueInRange()`.
* Added methods `std::string InputVariable::fuzzyInputValue()` and `std::string  OutputVariable::fuzzyOutputValue()`.
* Added method `OutputVariable::clear()` to clear the fuzzy output, and set `OutputVariable::previousOutputValue = fl::nan` and set `OutputVariable::outputValue = fl::nan`.
* Added copy constructors, assignment operators, move constructors and move operators to [Input|Output]Variable.
* Added method `Variable::terms()` to return mutable reference.
* **(important)** Changed `OutputVariable::[defuzzifier|fuzzyOutput]` to smart pointers (`FL_unique_ptr`).


#### <a name="new-terms">Linguistic Terms</a>
* **(VERY important)** Added Term::[get|set]Height to define the height of *integral* terms, and multiply their respective membership functions accordingly.
* Added copy constructors, assignment operators, move constructors and move operators to every `Term`.
* **(VERY important)** Parameters of all terms are set by default to `fl::nan`.
* **(important)** Renamed method `Term::copy()` to `Term::clone()` in every `Term`.
* Added method `Term::updateReference(Term*, Engine*)` to ensure `Linear` and `Function` terms have updated pointers to the `Engine` (useful when cloning and copy-constructing).
+ **(important)** Added linguistic terms `Concave`, `Cosine` and `Spike`.
* **(important)** Changed `Accumulated` to take `Activated*` terms instead of `const Terms*`.
* Removed `const` from return type of method `SNorm* Accumulated::[get|set]Accumulation()`.
* Changed `Accumulated::accumulation` to a smart pointer (`FL_unique_ptr`).
* Added method `Accumulated::terms()` to return mutable reference.
* **(important)**  Renamed methods `Triangle::[set|get][A|B|C]` to `::[set|get]Vertex[A|B|C]`.
* **(important)** Renamed methods `Trapezoid::[set|get][A|B|C|D]` to `::[set|get]Vertex[A|B|C|D]`.
* **(important)** Renamed term `Thresholded` to `Activated`.
* **(important)**  Renamed methods `Thresholded::[set|get]Threshold()` to `Activated::[set|get]Degree()`.
* Added enumeration `[Ramp|Sigmoid]::Direction{ NEGATIVE, ZERO, POSITIVE }` to refer to the slope.
* Added methods `Ramp::direction()` and `Sigmoid::direction()` to retrieve direction of slope.
* Removed Exception Specification from methods in `Discrete`, `Linear` and `Function` terms.

#### <a name="new-linear-discrete">Linear and Discrete Terms</a>
* **(important)** Changed `Linear` from having pointers to the input variables to having a pointer to the `Engine`.
* Changed visibility of `Linear::coefficients` to `protected`.
* **(important)**  Added methods `Linear::coefficients()`, `Linear::setCoefficients()`.
* `Linear` term no longer throws exception when `inputVariables != |coefficients|`.
* **(important)** Removed public vector of variables `Discrete::[x|y]`.
* **(important)** Added a `typedef std::pair<scalar, scalar> Discrete::Pair`.
* **(important)** Changed representation of `Discrete::[x|y]` from `std::vector<scalar>` to `std::vector<Discrete::Pair>`.
* Added methods `Discrete::setXY()` and `Discrete::xy()` to set and get the new representation of pairs.
* Added methods `Discrete::xy(int index)` to retrieve `Discrete::Pair` at `index`.
* **(important)** Added methods `Discrete::toPairs(std::vector<scalar>)` which throws an exception if the vector is missing a value (i.e., `std::vector<scalar>.size() % 2 != 0`), and `Discrete::toPairs(std::vector<scalar>, scalar missingValue)` which adds `missingValue` in case `std::vector<scalar>.size() %2 == 1`, hence never throwing an exception.
* Added method `Discrete::toVector(std::vector<Discrete::Pair>)` to convert `std::vector<Discrete::Pair>` to a `std::vector<scalar>`.
* Added method `Discrete::formatXY()` to get pairs `(x,y)` nicely formatted.

####<a name="new-function">Function Term</a>
* **(important)** Merged structs `Function::Operator` and `Function::BuiltInFunction` into a  single `struct Function::Element`.
* **(EXTREMELY important)**  Changed the precedence of all built-in instances of `Function::Element` of type `Operator` starting from `100` and decreasing by `10`. The precedence of built-in operators  is the following: `(100)` Logical not `[!]` and Negation `[~]`; `(90)` Power `[^]`; `(80)` Multiplication `[*]`, Division `[/]` and Modulo `[%]`; `(70)` Addition `[+]` and Subtraction `[-]`; `(60)` Logical AND `[and]` and Logical OR `[or]`. If you have registered your own operators, please adjust their precedence as required.
* Added to `Function` built-in comparison functions `gt,lt,ge,le,eq` and operator logical not `!`.
* Modified typedefs `Function::Unary` and `Function::Binary` to take `scalar` instead of `double`.
* Changed `public Function::root` to `protected Function::_root` and it is now a smart pointer (`FL_unique_ptr`).
* Added method `Function::root()` to return pointer to `Function::root`.
* **(EXTREMELY important)** Moved built-in functions and operators from `Function` to a `FunctionFactory`.


#### <a name="new-norms-hedges">[T|S]Norms and Hedges</a>
+ **(important)** Fixed operation when using multiple hedges to operate from right-most to left-most, e.g. `if Ambient is not very extremely bright`, now evaluates as follows `not(very(extremely(bright)))`.
+ Added `TNorm` nilpotent minimum and `SNorm` nilpotent maximum.
* Added clone methods to every `Norm`.
* Added clone methods to every `Hedge`.
* **(VERY important)** Moved `Engine::hedges` to `Rule::hedges`.


#### <a name="new-rules">Rules</a>
* **(VERY important)** Moved `Engine::hedges` (and methods) to `Rule::hedges`.
* Added method `Rule::isLoaded()` to determine whether a rule was properly parsed and thus can be activated.
* Added method `Rule::unload()` to allow the existence of a rule in an inactive state (useful for invalid rules).
* **(important)** Removed variable `Rule::FL_ASSIGNS` and method `Rule::assignsKeyword()`, for which the symbol `=` in rules is no longer valid.
* Changed visibility of method `Rule::setText()` to `public`.
* Added method `Rule::load(const Engine*)`.
* Changed `Rule::[antecedent|consequent]` to smart pointers (`FL_unique_ptr`).
* **(important)** Renamed method `Antecedent::[get|set]Root()` to `Antecedent::[get|set]Expression()`.
* Added methods `[Antecedent|Consequent]::[get|set]Text()`.
* **(important)** Added methods `[Antecedent|Consequent]::[load|unload]()`, with the same objective as `Rule::[load|unload]()`.

#### <a name="new-ruleblocks">Rule Blocks</a>
* Added method `RuleBlock::reloadRules()`.
* Added method `RuleBlock::setRules(std::vector)`.
* Added method `RuleBlock::rules()` to return mutable reference.
* Removed `const` from `TNorm` and `SNorm` in `RuleBlock::[get|set][Conjunction|Disjunction|Activation]()`, respectively.
* Changed `RuleBlock::[conjunction|disjunction|activation]` to smart pointers (`FL_unique_ptr`).
* **(VERY important)** Added basic rule chaining such that an `OutputVariable` can be utilized in the `Antecedent` of a `Rule`. For example, considering the rule `if Power is high then InversePower is low`, where `Power` and `InversePower` are both output variables, the activation degree of the `Antecedent` will correspond to the accumulated  activation degree of the term `high` in the fuzzy output of `Power`. If `Power::accumulation = none`, the accumulated activation degree of the term `high` will be computed as the regular sum of the activation degrees of term `high` in the fuzzy output of `Power`. Otherwise, the accumulated activation degree is computed utilizing the `Power::accumulation` operator.


#### <a name="new-weighted">Weighted Defuzzifiers</a>
+ **(VERY important)** Performance improvements of Takagi-Sugeno controllers by over 55% (with respect to v4.0) based on the average performance on the examples included.
* **(important)** Created class `WeightedDefuzzifier` from which classes `Weighted[Average|Sum]` are derived.
* **(important)** Added enumeration `WeightedDefuzzifier::Type{Automatic, TakagiSugeno, Tsukamoto}` and respective methods `WeightedDefuzzifier::[get|set]Type()` and `WeightedDefuzzifer::getTypeName()`.
* Added method `WeightedDefuzzifier::inferType(Term*)` to automatically determine the `WeightedDefuzzifier::Type` based on the class of `Term`.
* **(important)** By default, `WeightedDefuzzifier::type = Automatic`, which automatically infers the type based on the `WeightedDefuzzifier::inferType()`.
* **(important)** There is a small performance penalty when using `WeightedDefuzzifier::type = Automatic` because `WeightedDefuzzifier::inferType()` performs three `dynamic_cast<>`.
* **(important)** Deleted class `Tsukamoto`. Its method `static tsukamoto()` was moved to `virtual WeightedDefuzzifier::tsukamoto()`, which allows overriding it
* Added support for `Tsukamoto` with `Concave` terms.
+ **(EXTREMELY important)** In version 5.0, the traditional operation of Takagi-Sugeno and Tsukamoto controllers is achieved by setting `OutputVariable::accumulation = none`. Unlike version 4.0, the `RuleBlock::activation` will not have any effect on Takagi-Sugeno nor Tsukamoto controllers, for which `RuleBlock::activation` should also be set to `none`. More information about the roles of the  `OutputVariable::accumulation` and `RuleBlock::activation` operators are detailed as follows. Refer to [sciweavers](http://www.sciweavers.org/free-online-latex-equation-editor) to convert LaTeX equations.
+ **(VERY important)** In version 5.0, the role of the  `RuleBlock::activation` `TNorm` on the `Weighted[Average|Sum]` always performs a regular multiplication of the weights and the values (i.e., $w_i \times z_j$) regardless of the `TNorm` chosen. In other words, selecting any `RuleBlock::activation` for `Weighted[Average|Sum]` is irrelevant, and should be set to `none` as every `TNorm` will have the same multiplication effect. This operation is different from `fuzzylite` version 4.0, where the `RuleBlock::activation` operator was utilized to multiply the weights and values (i.e. $w_i \otimes z_j$), and therefore the traditional operation of the `Weighted[Average|Sum]` was achieved when `RuleBlock::activation =  AlgebraicProduct;`.
+ **(VERY important)** In version 5.0, the role of the `OutputVariable::accumulation = none` on the `Weighted[Average|Sum]` results in a regular sum of the multiplied weights and values, i.e., $\dfrac{\sum_i^n w_i \times z_j}{\sum_i^n w_i}$. However, if the `OutputVariable::accumulation != none`, the role of the `SNorm` will be to accumulate the activation degrees of the *repeated* terms in the fuzzy output of the variable. For example, considering the rules `if Ambient is dark then Power is high` and `if Ambient is medium then Power is high`, for any  input value of `Ambient` that activates both rules, the fuzzy output of `Power` will have the term `high` activated with the degree from `Rule 1`, and the term `high` activated with the degree from `Rule 2`. Since the term `high` appears twice in the fuzzy output, the role of the accumulation operator will be to accumulate the activation degree of `high` resulting in $\dfrac{(w_1 \oplus w_2) \times z_{high}}{(w_1 \oplus w_2)}$. If another term were activated, the result would be $\dfrac{(w_1 \oplus w_2) \times z_{high} + w_i \times z_j}{(w_1 \oplus w_2) + w_i}$. In version 4.0, the accumulation operator had no effect on the `Weighted[Average|Sum]`.



#### <a name="new-integral">Integral Defuzzifiers</a>
* **(important)** Proper handling of indefinite integral defuzzification, that is, returning `fl::nan` when `[minimum|maximum]=[NaN|Inf]`.
* Default resolution of integration is defined as `static int IntegralDefuzzifier::defaultResolution=200`, and can be changed via `static IntegralDefuzzifier::setDefaultResolution()`.
+ **(important)** In `fuzzylite`, the accumulation operator has been for several versions associated with the output variables and **not** with the rule blocks, despite that the FCL format and other fuzzy logic control libraries associate the accumulation operator with the rule blocks. The argument for such a decision is that `fuzzylite` provides **coherent** support for multiple rule blocks operating on the same engine and on the same output variables. For example, if multiple rule blocks operate on the same output variables, it  only makes sense to have a single accumulation operator associated with each output variable such that the defuzzifier can naturally operate over the accumulated fuzzy output. Differently, if the accumulation operator were associated with the rule block, the possibility of having different accumulation operators in different rule blocks questions (1) **the possibility of having multiple rule blocks operating over the same output variables**; and (2) **the usage of different  accumulation operators over the accumulation and defuzzification processes**. Certainly, if (1) is not possible, i.e,  different rule blocks only operate on different output variables, then (2) is not a problem because the accumulation process and defuzzification of each variable will only have a single accumulation operator. It is therefore that the association of the accumulation operator with the output variable in `fuzzylite` provides a **better design** and an additional feature that allows having multiple rule blocks operating over the same output variables.
* Added copy constructors, assignment operators, move constructors and move operators.
* Added method `Defuzzifier::clone()`.

####<a name="new-imex">Importers and Exporters</a>
* **(EXTREMELY important)** Since terms have a new `height` property, `[Fll|Fis|Fcl]Exporter` exports terms with an additional `scalar` at the end, which indicates the `height` of the term. However, if `height=1.0`, the additional scalar is not exported.
* **(EXTREMELY important)** In `[Fll|Fis|Fcl]Importer`,  when importing terms, if there is an additional `scalar` it will be assumed as the `height` of the term. For example, `term: high Gaussian 1.0 0.5 0.75` will create a `Gaussian` term with mean `1.0`, standard deviation `0.5` and height `0.75`. This is **extremely important** because there are some examples from Matlab in `fis` format that append a useless `0.0` to some terms.
* **(EXTREMELY important)** In `FisExporter`, if the Takagi-Sugeno controller has no `activation` or `accumulation` operators (as it should generally be the case), Octave and Matlab will not be able to import the `fis` file. To overcome this issue, you will have to set `ImpMethod="min"` and `AggMethod="max"`, where `ImpMethod` and `AggMethod` are just dummy operators that can be set to any `TNorm` and `SNorm`, respectively.
+ **(important)** Improved compatibility of the exported code obtained with `[Fis|Fcl]Exporter` by exporting the additional features of `fuzzylite` only when these are different from the default operation. For example, the following features will not be exported given their values: `[Input|Output]Variable::enabled = true;`, `OutputVariable::lock-previous = false;`, `OutputVariable::lock-range = false;`, amongst others.
* **(important)** Renamed FLL property `'lock-valid'` to `'lock-previous'`.
* **(important)** Renamed FIS property `'LockValid'` to `'LockPrevious'`.
* **(important)** Renamed FCL property `'LOCK: VALID'` to `'LOCK: PREVIOUS'`.
+ **(important)** Export your controllers to files using `[Fll|Fld|Fis|Fcl]Exporter::toFile()`.
+ **(important)** Import your controllers from files using `[Fll|Fis|Fcl]Importer::fromFile()`.
+ **(important)** `FldExporter` exports the FuzzyLite Dataset of an engine utilizing the input values of another FuzzyLite Dataset.
* `FldExporter` no longer restarts the engine when exporting.
* **(important)** Renamed method `FldExporter::toWriter()` to `FldExporter::write()`.
* Removed variable and methods for property `int FldExporter::_maximum`.
* Added option in `CppExporter` to prepend the  namespace prefix `fl::` to the classes, and by default it does not prepend prefix.
* Improvement accuracy of `FisImporter` when importing `fis` files whose scalar values have more than  three decimal numbers.
* Renamed methods in `[Fis|Fcl]Importer::extract*` to `[Fis|Fcl]Importer::parse*`.



#### <a name="new-factories">Factories</a>
* Created a generic `CloningFactory<T>` to create clones of objects.
* **(important)** Created `FunctionFactory` based on `CloningFactory<Function::Element>` where function operators and methods are stored to be cloned as necessary by `Function`. Additional functions and operators can be easily registered.
* **(VERY important)** Moved built-in functions and operators from `Function` to `FunctionFactory`.
* Renamed methods `Factory<T>::[register|deregister]Class()` to `Factory<T>::[register|deregister]Constructor()`.
* **(important)** Renamed `Factory<T>` to `ConstructionFactory<T>`.
* Renamed  `typedef Factory::Creator` to `typedef Factory::Constructor`.
* Changed pointers of `FactoryManager` to smart pointers (`FL_unique_ptr`).



#### <a name="new-examples">Examples</a>
* **(important)** Added two examples for basic rule chaining: `mamdani/SimpleDimmerInverse.fll` and `mamdani/Laundry.fll`.
* Included the `original` example files in `fis` format.
* Added conversion of `examples/original/*.fis` to `examples/original/*.fll`.
* Modified `original/takagi-sugeno` examples to reflect  `activation: none; accumulation: none;`.
* Updated FLD examples produced from the `original` examples.
+ **(important)** Added file [`fuzzylite/src/m/compare.m`](/fuzzylite/src/m/compare.m) <!---check link--> to compare the output values of your `fuzzylite` engines with the evaluation of the same engine in Octave/Matlab.
+ **(important)** Added file [`examples/examples.mat`](/examples/examples.mat) <!---check link--> containing the comparison of the output values between `fuzzylite` and Matlab's Fuzzy Logic Toolbox.
* Added code to perform benchmarks in Linux.


#### <a name="new-console">Console</a>
* **(important)** Console includes option to import custom input dataset from file an export its respective output values.
* **(important)** Created the FuzzyLite Interactive Console, which can be started by specifying an input file and the output format, e.g., `fuzzylite -i SimpleDimmer.fll -of fld`.
* Console provides more information about its usage.



####<a name="new-fixes"> Fixes Bugs and Leaks</a>
+ **(important)** Fixed operation when using multiple hedges to operate from right-most to left-most, e.g. `if Ambient is not very extremely bright` evaluates as follows `not(very(extremely(bright)))`.
* **(important)** Fixed membership functions of specific cases of `Triangle` when `a=b` or `b=c`, and `Trapezoid` when `a=b` or `c=d`.
* Fixed minor memory leak at `~RuleBlock::[conjunction|disjunction|activation]`.
* Fixed minor memory leak at `~Accumulated::accumulation`.
* Fixed minor memory leak at `~OutputVariable::defuzzifier`.
* Fixed minor memory leak at `~Function::Node`.
* Fixed minor memory leak at `~FactoryManager::[factories]`.
* Fixed some rethrowing of exceptions to provide proper information about errors. Specifically, changed some rethrown exceptions from `throw ex;` to just `throw;`
* Fixed building using `-DFL_USE_FLOAT=ON`

***

For more information, visit [www.fuzzylite.com](http://www.fuzzylite.com).

fuzzylite&reg; is a registered trademark of FuzzyLite Limited.

Copyright &#xa9; 2010-2015 FuzzyLite Limited. All rights reserved.

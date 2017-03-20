fuzzylite 6.0 &reg;
=================
<img src="https://raw.githubusercontent.com/fuzzylite/fuzzylite/release/fuzzylite.png" align="right" alt="fuzzylite">


A Fuzzy Logic Control Library in C++
------------------------------------

By: [Juan Rada-Vilela](http://www.fuzzylite.com/jcrada), Ph.D.

Released: 20/March/2017

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://travis-ci.org/fuzzylite/fuzzylite.svg?branch=release)](https://travis-ci.org/fuzzylite/fuzzylite) 
[![Build status](https://ci.appveyor.com/api/projects/status/065g596yxdhkt2se/branch/release)](https://ci.appveyor.com/project/jcrada/fuzzylite/branch/release)
[![Coverity Status](https://scan.coverity.com/projects/5759/badge.svg)](https://scan.coverity.com/projects/5759) 


***


### Table of Contents
[License](#license) <br/>
[Introduction](#introduction)<br/>
[Features](#features)<br/>
[Example](#example) <br/>
[Compile, Link, and Execute](#compile-build-execute)<br/>
[Bulding from Source](#building)<br/>
[Binaries](#binaries) <br/>
[What's new](#whatsnew)<br/>
[What's next](#whatsnext) <br/>




***

### <a name="license">License</a>
`fuzzylite 6.0` is licensed under the [**GNU General Public License (GPL) 3.0**](https://www.gnu.org/licenses/gpl.html). You are **strongly** encouraged to support the development of the FuzzyLite Libraries by purchasing a license of [`QtFuzzyLite 6`](http://www.fuzzylite.com/downloads). 

[`QtFuzzyLite 6`](http://www.fuzzylite.com/downloads/) is the new and (very likely) the best graphical user interface available to  easily design and directly operate fuzzy logic controllers in real time. Available for Windows, Mac, and Linux, its goal is to significantly **speed up** the design of your fuzzy logic controllers, while providing a very **useful**, **functional** and **beautiful** user interface. 
Please, download it and check it out for free at [www.fuzzylite.com/downloads/](http://www.fuzzylite.com/downloads/).

***


### <a name="introduction">Introduction</a>


`fuzzylite` is a free and open-source fuzzy logic control library programmed in C++ for multiple platforms (e.g., Windows, Linux, Mac, iOS). [`jfuzzylite`](https://github.com/fuzzylite/jfuzzylite/) is the equivalent library for Java and Android platforms. Together, they are the FuzzyLite Libraries for Fuzzy Logic Control.

<center>
```
The goal of the FuzzyLite Libraries is to easily design and efficiently operate fuzzy logic controllers following an object-oriented programming model without relying on external libraries.
``` 
</center>

#### Reference
If you are using the FuzzyLite Libraries, please cite the following reference in your article:

Juan Rada-Vilela. fuzzylite: a fuzzy logic control library, 2017. URL http://www.fuzzylite.com/.

```bibtex
 @misc{fl::fuzzylite,
 author={Juan Rada-Vilela},
 title={fuzzylite: a fuzzy logic control library},
 url={http://www.fuzzylite.com/},
 year={2017}}
```

#### Documentation
The documentation for the `fuzzylite` library is available at: [www.fuzzylite.com/documentation/](http://www.fuzzylite.com/documentation/). 

***

### <a name="features">Features</a>

**(6) Controllers**: Mamdani, Takagi-Sugeno, Larsen, Tsukamoto, Inverse Tsukamoto, Hybrids

**(21) Linguistic terms**:  (4) *Basic*: triangle, trapezoid, rectangle, discrete. 
(9) *Extended*: bell, cosine, gaussian, gaussian product, pi-shape, sigmoid difference, sigmoid product, spike. 
(5) *Edges*: binary, concave, ramp, sigmoid, s-shape, z-shape. 
(3) *Functions*: constant, linear, function. 

**(7) Activation methods**:  general, proportional, threshold, first, last, lowest, highest.

**(8) Conjunction and Implication (T-Norms)**: minimum, algebraic product, bounded difference, drastic product, einstein product, hamacher product, nilpotent minimum, function.

**(10) Disjunction and Aggregation (S-Norms)**:  maximum, algebraic sum, bounded sum, drastic sum, einstein sum, hamacher sum, nilpotent maximum, normalized sum, unbounded sum,  function.

**(7) Defuzzifiers**:  (5) *Integral*: centroid, bisector, smallest of maximum, largest of maximum, mean of maximum. 
(2) *Weighted*: weighted average, weighted sum.

**(7) Hedges**: any, not, extremely, seldom, somewhat, very, function.

**(3) Importers**: FuzzyLite Language `fll`, Fuzzy Inference System `fis`, Fuzzy Control Language `fcl`.

**(7) Exporters**: `C++`, `Java`, FuzzyLite Language `fll`, FuzzyLite Dataset `fld`, `R` script, Fuzzy Inference System `fis`, Fuzzy Control Language `fcl`.

**(30+) Examples**  of Mamdani, Takagi-Sugeno, Tsukamoto, and Hybrid controllers from `fuzzylite`, Octave, and Matlab, each included in the following formats: `C++`, `Java`, `fll`, `fld`, `R`, `fis`, and `fcl`.




***

### <a name="example">Example</a>
#### FuzzyLite Language
```yaml
#File: ObstacleAvoidance.fll
Engine: ObstacleAvoidance
InputVariable: obstacle
  enabled: true
  range: 0.000 1.000
  lock-range: false
  term: left Ramp 1.000 0.000
  term: right Ramp 0.000 1.000
OutputVariable: mSteer
  enabled: true
  range: 0.000 1.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: Centroid 100
  default: nan
  lock-previous: false
  term: left Ramp 1.000 0.000
  term: right Ramp 0.000 1.000
RuleBlock: mamdani
  enabled: true
  conjunction: none
  disjunction: none
  implication: AlgebraicProduct
  activation: General
  rule: if obstacle is left then mSteer is right
  rule: if obstacle is right then mSteer is left
```
```cpp
//File: ObstacleAvoidance.cpp
#include "fl/Headers.h"

int main(int argc, char* argv[]){
    using namespace fl;
    Engine* engine = FllImporter().fromFile("ObstacleAvoidance.fll");
    
    std::string status;
    if (not engine->isReady(&status))
        throw Exception("[engine error] engine is not ready:\n" + status, FL_AT);

    InputVariable* obstacle = engine->getInputVariable("obstacle");
    OutputVariable* steer = engine->getOutputVariable("mSteer");

    for (int i = 0; i <= 50; ++i){
        scalar location = obstacle->getMinimum() + i * (obstacle->range() / 50);
        obstacle->setValue(location);
        engine->process();
        FL_LOG("obstacle.input = " << Op::str(location) << 
            " => " << "steer.output = " << Op::str(steer->getValue()));
    }
}
```

#### C++
```cpp
//File: ObstacleAvoidance.cpp
#include "fl/Headers.h"

int main(int argc, char* argv[]){
    using namespace fl;
    //Code automatically generated with fuzzylite 6.0.

    using namespace fl;

    Engine* engine = new Engine;
    engine->setName("ObstacleAvoidance");
    engine->setDescription("");

    InputVariable* obstacle = new InputVariable;
    obstacle->setName("obstacle");
    obstacle->setDescription("");
    obstacle->setEnabled(true);
    obstacle->setRange(0.000, 1.000);
    obstacle->setLockValueInRange(false);
    obstacle->addTerm(new Ramp("left", 1.000, 0.000));
    obstacle->addTerm(new Ramp("right", 0.000, 1.000));
    engine->addInputVariable(obstacle);

    OutputVariable* mSteer = new OutputVariable;
    mSteer->setName("mSteer");
    mSteer->setDescription("");
    mSteer->setEnabled(true);
    mSteer->setRange(0.000, 1.000);
    mSteer->setLockValueInRange(false);
    mSteer->setAggregation(new Maximum);
    mSteer->setDefuzzifier(new Centroid(100));
    mSteer->setDefaultValue(fl::nan);
    mSteer->setLockPreviousValue(false);
    mSteer->addTerm(new Ramp("left", 1.000, 0.000));
    mSteer->addTerm(new Ramp("right", 0.000, 1.000));
    engine->addOutputVariable(mSteer);

    RuleBlock* mamdani = new RuleBlock;
    mamdani->setName("mamdani");
    mamdani->setDescription("");
    mamdani->setEnabled(true);
    mamdani->setConjunction(fl::null);
    mamdani->setDisjunction(fl::null);
    mamdani->setImplication(new AlgebraicProduct);
    mamdani->setActivation(new General);
    mamdani->addRule(Rule::parse("if obstacle is left then mSteer is right", engine));
    mamdani->addRule(Rule::parse("if obstacle is right then mSteer is left", engine));
    engine->addRuleBlock(mamdani);

    std::string status;
    if (not engine->isReady(&status))
        throw Exception("[engine error] engine is not ready:\n" + status, FL_AT);

    for (int i = 0; i <= 50; ++i){
        scalar location = obstacle->getMinimum() + i * (obstacle->range() / 50);
        obstacle->setValue(location);
        engine->process();
        FL_LOG("obstacle.input = " << Op::str(location) << 
            " => " << "steer.output = " << Op::str(steer->getValue()));
    }
}
```

### <a name="compile-build-execute">Compile, Link, and Execute</a>

Once you have an engine written in C++, you can compile it to create an executable file which links to the `fuzzylite` library. The linking can be either static or dynamic. Basically, the differences between static and dynamic linking are the following.  **Static linking** includes the `fuzzylite` library into your executable file, hence increasing its size, but the executable no longer needs to  have access to the `fuzzylite` library files. **Dynamic linking** does not include the `fuzzylite` library into your executable file, hence reducing its size, but the executable needs to have access to the `fuzzylite` shared library file. When using dynamic linking, make sure that  the shared library files are  either in the same directory as the executable, or are reachable via environmental variables:

```posh
rem Windows:
set PATH="\path\to\fuzzylite\release\bin;%PATH%"
```
```bash 
#Unix:
export LD_LIBRARY_PATH="/path/to/fuzzylite/release/bin/:$LD_LIBRARY_PATH"
```

#### Windows
The commands to compile your engine in Windows are the following:

C++11 (default)
```posh
rem static linking:
cl.exe ObstacleAvoidance.cpp fuzzylite-static.lib /Ipath/to/fuzzylite /EHsc /MD
```
```posh
rem dynamic linking:
cl.exe ObstacleAvoidance.cpp fuzzylite.lib /Ipath/to/fuzzylite /DFL_IMPORT_LIBRARY /EHsc /MD 
```

C++98
```posh
rem static linking:
cl.exe ObstacleAvoidance.cpp fuzzylite-static.lib /Ipath/to/fuzzylite /DFL_CPP98=ON /EHsc /MD
```
```posh
rem dynamic linking:
cl.exe ObstacleAvoidance.cpp fuzzylite.lib /Ipath/to/fuzzylite /DFL_IMPORT_LIBRARY /DFL_CPP98=ON /EHsc /MD 
```

#### Unix
The commands to compile your engine in Unix are the following:

C++11 (default)
```bash
#static linking
g++ ObstacleAvoidance.cpp -o ObstacleAvoidance -I/path/to/fuzzylite -L/path/to/fuzzylite/release/bin -lfuzzylite-static --std=c++11
```
```bash
#dynamic linking
g++ ObstacleAvoidance.cpp -o ObstacleAvoidance -I/path/to/fuzzylite -L/path/to/fuzzylite/release/bin -lfuzzylite -Wno-non-literal-null-conversion
```

C++98
```bash
#static linking
g++ ObstacleAvoidance.cpp -o ObstacleAvoidance -I/path/to/fuzzylite -L/path/to/fuzzylite/release/bin -lfuzzylite-static -DFL_CPP98=ON
```
```bash
#dynamic linking
g++ ObstacleAvoidance.cpp -o ObstacleAvoidance -I/path/to/fuzzylite -L/path/to/fuzzylite/release/bin -lfuzzylite -DFL_CPP98=ON -Wno-non-literal-null-conversion
```

#### CMake
Alternatively, you can use CMake to build your project linking to `fuzzylite`. Please, refer to the example application available at [examples/application]([/examples/application]).



***


### <a name="building">Building  from Source</a>
You can build the `fuzzylite` library from source using  `CMake` [(cmake.org)](https://cmake.org/).

The files [`fuzzylite/build.bat`](/fuzzylite/build.bat) and [`fuzzylite/build.sh`](/fuzzylite/build.sh) are build scripts for the Windows and Unix platforms, respectively.
After building from source, the resulting binaries will be located in the sub-folders `fuzzylite/release/bin` and `fuzzylite/debug/bin`. The usage of these scripts is presented as follows.

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



#### Building Options
For advanced building options, please check the contents of [`fuzzylite/build.bat`](/fuzzylite/build.bat) or [`fuzzylite/build.sh`](/fuzzylite/build.sh), and the contents of [`fuzzylite/CMakeLists.txt`](/fuzzylite/CMakeLists.txt).

The following building options available:

* `-DFL_USE_FLOAT=ON` builds the binaries utilizing the `fl::scalar` data type as a `float` represented in 4 bytes. By default, the binaries are built utilizing `-DFL_USE_FLOAT=OFF` to utilize `fl::scalar` as a `double` represented in 8 bytes and hence providing better accuracy. If `fuzzylite` is built with `-DFL_USE_FLOAT=ON`, then the applications linking to `fuzzylite` also need to specify this compilation flag. 


* `-DFL_CPP98=ON` builds binaries utilizing `C++98` features. By default, `fuzzylite` is built with `-DFL_CPP98=OFF` to utilize `C++11` features. If compiling for `C++98`, be aware that you will not be able to benchmark the performance of your engine using the `Benchmark` class.


* `-DFL_BACKTRACE=OFF` disables the backtrace information in case of errors (default is ON). In Windows, the backtrace information requires the external library `dbghelp`, which is generally available in your system.

* `-DCMAKE_BUILD_TYPE=[Debug|Release]` sets the mode of your build. You can only build one mode at a time with a single CMake script.


#### Documentation
The source code of `fuzzylite` is very well documented using [`doxygen`](www.doxygen.org/) formatting, and the documentation is available at [fuzzylite.com/documentation](http://fuzzylite.com/documentation). If you want to generate the documentation locally, you can produce the `html` documentation from the file [Doxyfile](/Doxyfile) using the command line: `doxygen Doxyfile`. The documentation will be created in the [`documentation`](/documentation) folder.



***

### <a name="binaries">Binaries</a>

After building from source, the following are the relevant binaries that will be created in `Release` mode. In `Debug` mode, the file names end with `-debug` (e.g., `fuzzylite-debug.exe`).

#### Windows

- console application: `fuzzylite.exe`
- shared library: `fuzzylite.dll`, `fuzzylite.lib`
- static library: `fuzzylite-static.lib`

#### Linux

- console application: `fuzzylite`
- shared library: `libfuzzylite.so`
- static library: `libfuzzylite-static.a`

#### Mac

- console application: `fuzzylite`
- shared library: `libfuzzylite.dylib`
- static library: `libfuzzylite-static.a`


#### Console
The console application of `fuzzylite` allows you to import and export your engines. Its usage can be obtained executing the console binary. In addition, the console can be set in interactive mode. The `FuzzyLite Interactive Console`  allows you to evaluate a given controller by manually providing the input values. The interactive console is triggered by specifying an input file and an output format. For example, to interact with the `ObstacleAvoidance` controller, the interactive console is launched as follows:

```bash
fuzzylite -i ObstacleAvoidance.fll -of fld
```



***

### <a name="whatsnew">What's New?</a>
* The FuzzyLite Libraries, namely fuzzylite and jfuzzylite, both in version 6.0, are dual licensed under the GNU General Public License version 3 and the Apache License version 2.

* By default, fuzzylite builds using C++11 instead of C++98. 

* Important performance improvements.

* Refactored the following names for the operation of engines: from activation operator to implication operator, from accumulation operator to aggregation operator.

* Renamed the term `Accumulated` to `Aggregated`.

* New activation methods decouple the activation of rules from the rule block and provide different methods for activating rules (see Activation Methods).

* New class `ActivationFactory` provides a factory of activation methods.

* New class `Benchmark` to evaluate the performance and accuracy of engines.

* New class `Complexity` to estimate the computational complexity of an engine.

* New class `RScriptExporter` to export the surfaces of an engine using the `ggplot2` library.

* New class `Binary` term for binary edges.

* New `UnboundedSum` S-Norm in `SNormFactory`.

* New classes `SNormFunction` and `TNormFunction` to create custom functions on any two values using the `Function` class.

* Added description strings to `Engine`, `Variable` and `RuleBlock`

* Privatized previously protected members of classes and subclasses of `Term`, `Variable`, `Rule`, `Defuzzifier`, `[Cloning|Construction]Factory`, `Importer`, `Exporter`, amongst others.

* Improved portability by replacing `int` for `std::size_t` where necessary, thereby additionally removing warnings in Windows 64bit

* Deleted `Operation.cpp` and inlined its methods into `Operation.h`

* Updated `.travis.yml` to use Docker, and build using g++ (versions 6, 5, 4.9, 4.8, 4.7) and clang (versions 3.8, 3.7, 3.6, and 3.5). 

* Added `appveyor.yml` to use continuous integration in Windows under Visual Studio 2013 and 2015.

* Added some unit tests and support for future unit tests.

* Bug fixes.

* New example of hybrid engines.

* New example on obstacle avoidance for Mamdani, Takagi-Sugeno, and Hybrid engines.

* New R scripts for each example and its respective surfaces in `pdf` formats.

#### Bug fixes
* Fixed bug in `CloningFactory::deregisterObject()`. Bug: Object was deleted before removing it from the map, leaving an invalid object in the map which would cause a segmentation fault. Solution: Remove the object from the map before deleting it.
* Fixed bug causing segmentation fault when malformed term in FuzzyLite Language
* Fixed bug computing the `NormalizedSum` S-Norm.
* Fixed bug in `RuleBlock` to reset and clone the implication operator. Bug: implication operator is not copied and reset. Fix: copy and reset implication operator when cloning the `RuleBlock`.
* Fixed bug in `Function` term. Bug: given a formula = "tan(y)" and a map["y"] = 1.0, and executing `Function::load(formula)`, then the map of variables is reset because `load()` calls `unload()` first, causing the deregistration of variable `y`. Solution: Removed method `unload()` from `load()`, thereby causing future `load()` not to reset variables.
* Fixed bug in `Function` when enclosing variable in double parenthesis.


***

### <a name="whatsnext">What's Next?</a>

+ Optimization of Fuzzy Logic Controllers
+ Type-2 Fuzzy Logic Controllers
+ Adaptive Neuro-Fuzzy Inference System (ANFIS)
+ Fuzzy C-means data clustering

***

fuzzylite&reg; is a registered trademark of FuzzyLite Limited.<br>
jfuzzylite&trade; is a trademark of FuzzyLite Limited.<br>
QtFuzzyLite&trade; is a trademark of FuzzyLite Limited.<br>


Copyright &#xa9; 2010-2017 FuzzyLite Limited. All rights reserved.

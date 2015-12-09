fuzzylite&reg; v6.0
==============
A Fuzzy Logic Control Library in C++ 
------------------------------------
By [Juan Rada-Vilela](http://www.fuzzylite.com/jcrada), Ph.D.

***

### Table of Contents
[License](#license)\n
[Introduction](#introduction)\n
[Features](#features)\n
[Example](#example)\n
[Bulding from source](#building)\n
[Binaries](#binaries)\n
[What's new](#whatsnew)\n
[What's next](#whatsnext)\n


***

### <a name="license">License</a>
`fuzzylite` will **always** be free and open source. However, `fuzzylite` is **no** longer released under the Apache License. Since version 5.0, `fuzzylite` is **dual-licensed** under the [**GNU Lesser General Public License (LGPL) v3.0**](https://www.gnu.org/licenses/lgpl.html) and a **paid commercial license**.


In brief, an important restriction the LGPL imposes on your closed-source application is that **you are no longer allowed to statically link** against `fuzzylite`. If your application requires static linking, **you will need to purchase a commercial license** from FuzzyLite Limited. Please, contact [sales@fuzzylite.com](mailto:sales@fuzzylite.com) for commercial licenses, and refer to the [GNU LGPL](https://www.gnu.org/licenses/lgpl.html) for further information on your rights.

The change of license is an attempt to raise funds in order to be able to work  part-time in the development of the `fuzzylite` family of products, namely `fuzzylite` (C++), `jfuzzylite` (Java), `pyfuzzylite` (Python), and `QtFuzzyLite` (Windows/Linux/Mac).

**There are still many things to do!**

Besides [donations](http://www.fuzzylite.com/donations/), you can significantly contribute by **purchasing a license** of the entirely new [`QtFuzzyLite`](http://www.fuzzylite.com/qt/) commercial application. In addition, if you require (paid) private support, please contact [jcrada@fuzzylite.com](mailto:jcrada@fuzzylite.com).


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

**Linguistic terms** *Basic* (4) Triangle, Trapezoid, Rectangle, Discrete. *Extended* (9) Bell, Cosine, Gaussian, Gaussian Product, Pi-shape, Sigmoid Difference, Sigmoid Product, Spike. *Edges* (4) Concave, Ramp, Sigmoid, S-shape, Z-shape. *Functions* (3) Constant, Linear, Function

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
        ambient->setInputValue(light);
        engine->process();
        FL_LOG("Ambient.input = " << Op::str(light) << " -> " <<
            "Power.output = " << Op::str(power->getOutputValue()));
    }
}
```

***

### <a name="building">Building from Source</a>
Building from source requires you to have CMake installed.

The files [`fuzzylite/build.bat`](/fuzzylite/build.bat) and [`fuzzylite/build.sh`](/fuzzylite/build.sh) are automatic build scripts for Windows and Unix platforms, respectively. The usage of these scripts is presented as follows.

**Windows**

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

**Unix**
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


The console application of `fuzzylite` allows you to import and export your controllers. Its usage can be obtained executing the console binary. In addition, the FuzzyLite Interactive Console is activated  when exporting to `fld` without providing an output file. The interactive console allows you to evaluate any controller by manually providing the input values.


***

### <a name="whatsnew">What's New?</a>
The entire `fuzzylite` library has been thoroughly revised, refactored, validated, and significantly improved. The following sections detail the changes and enhancements of version 5.0. Users of previous versions are **strongly** encouraged to carefully read the list before migrating to version 5.0. Important changes and enhancements are marked as **(important)**, **(VERY important)** and **(EXTREMELY important)**.

***


### <a name="whatsnext">What's Next?</a>

+ Type-2 Fuzzy Logic Controllers
+ Adaptive Neuro-Fuzzy Inference System (ANFIS)
+ Fuzzy C-means data clustering

***

For more information, visit [www.fuzzylite.com](http://www.fuzzylite.com).

fuzzylite&reg; is a registered trademark of FuzzyLite Limited.

Copyright &copy; 2010-2015 FuzzyLite Limited. All rights reserved.

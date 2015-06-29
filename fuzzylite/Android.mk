LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := fuzzylite

LOCAL_SRC_FILES := \
src/Console.cpp \
src/defuzzifier/Bisector.cpp \
src/defuzzifier/Centroid.cpp \
src/defuzzifier/IntegralDefuzzifier.cpp \
src/defuzzifier/LargestOfMaximum.cpp \
src/defuzzifier/MeanOfMaximum.cpp \
src/defuzzifier/SmallestOfMaximum.cpp \
src/defuzzifier/WeightedAverage.cpp \
src/defuzzifier/WeightedDefuzzifier.cpp \
src/defuzzifier/WeightedSum.cpp \
src/Engine.cpp \
src/Exception.cpp \
src/factory/CloningFactory.cpp \
src/factory/ConstructionFactory.cpp \
src/factory/DefuzzifierFactory.cpp \
src/factory/FactoryManager.cpp \
src/factory/FunctionFactory.cpp \
src/factory/HedgeFactory.cpp \
src/factory/SNormFactory.cpp \
src/factory/TermFactory.cpp \
src/factory/TNormFactory.cpp \
src/fuzzylite.cpp \
src/hedge/Any.cpp \
src/hedge/Extremely.cpp \
src/hedge/Not.cpp \
src/hedge/Seldom.cpp \
src/hedge/Somewhat.cpp \
src/hedge/Very.cpp \
src/imex/CppExporter.cpp \
src/imex/Exporter.cpp \
src/imex/FclExporter.cpp \
src/imex/FclImporter.cpp \
src/imex/FisExporter.cpp \
src/imex/FisImporter.cpp \
src/imex/FldExporter.cpp \
src/imex/FllExporter.cpp \
src/imex/FllImporter.cpp \
src/imex/Importer.cpp \
src/imex/JavaExporter.cpp \
src/norm/s/AlgebraicSum.cpp \
src/norm/s/BoundedSum.cpp \
src/norm/s/DrasticSum.cpp \
src/norm/s/EinsteinSum.cpp \
src/norm/s/HamacherSum.cpp \
src/norm/s/Maximum.cpp \
src/norm/s/NilpotentMaximum.cpp \
src/norm/s/NormalizedSum.cpp \
src/norm/t/AlgebraicProduct.cpp \
src/norm/t/BoundedDifference.cpp \
src/norm/t/DrasticProduct.cpp \
src/norm/t/EinsteinProduct.cpp \
src/norm/t/HamacherProduct.cpp \
src/norm/t/Minimum.cpp \
src/norm/t/NilpotentMinimum.cpp \
src/Operation.cpp \
src/rule/Antecedent.cpp \
src/rule/Consequent.cpp \
src/rule/Expression.cpp \
src/rule/RuleBlock.cpp \
src/rule/Rule.cpp \
src/term/Accumulated.cpp \
src/term/Activated.cpp \
src/term/Bell.cpp \
src/term/Concave.cpp \
src/term/Constant.cpp \
src/term/Cosine.cpp \
src/term/Discrete.cpp \
src/term/Function.cpp \
src/term/Gaussian.cpp \
src/term/GaussianProduct.cpp \
src/term/Linear.cpp \
src/term/PiShape.cpp \
src/term/Ramp.cpp \
src/term/Rectangle.cpp \
src/term/Sigmoid.cpp \
src/term/SigmoidDifference.cpp \
src/term/SigmoidProduct.cpp \
src/term/Spike.cpp \
src/term/SShape.cpp \
src/term/Term.cpp \
src/term/Trapezoid.cpp \
src/term/Triangle.cpp \
src/term/ZShape.cpp \
src/variable/InputVariable.cpp \
src/variable/OutputVariable.cpp \
src/variable/Variable.cpp

LOCAL_CPPFLAGS += -DFL_VERSION=\"5.0\" -DFL_DATE=\"1408\" -DFL_BACKTRACE_OFF -pedantic -Wall -Wextra -DFL_EXPORT_LIBRARY

# Notice: execinfo.h only work for glibc, unsupported by Android bionic
# Add -DFL_USE_FLOAT if you need using "typedef float fl::scalar" instead of "typedef double fl::scalar"
# Add -std=c++11 -DFL_CPP11 if you need C++11 features
# TODO: Add -Wno-non-literal-null-conversion when using Clang Compiler
# Add FL_IMPORT_LIBRARY src/main.cpp instead of FL_EXPORT_LIBRARY when using for BUILD_EXECUTABLE
# Remove FL_IMPORT_LIBRARY and FL_EXPORT_LIBRARY when using for BUILD_STATIC_LIBRARY

LOCAL_CPP_FEATURES := rtti exceptions

include $(BUILD_SHARED_LIBRARY)

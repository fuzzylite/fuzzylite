################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AreaCentroidAlgorithm.cpp \
../src/CompoundTerm.cpp \
../src/DescriptiveAntecedent.cpp \
../src/DiscreteTerm.cpp \
../src/FunctionTerm.cpp \
../src/FuzzyAnd.cpp \
../src/FuzzyAntecedent.cpp \
../src/FuzzyConsequent.cpp \
../src/FuzzyDefuzzifier.cpp \
../src/FuzzyEngine.cpp \
../src/FuzzyException.cpp \
../src/FuzzyExceptions.cpp \
../src/FuzzyModulation.cpp \
../src/FuzzyOperation.cpp \
../src/FuzzyOperator.cpp \
../src/FuzzyOr.cpp \
../src/FuzzyRule.cpp \
../src/Hedge.cpp \
../src/HedgeSet.cpp \
../src/InfixToPostfix.cpp \
../src/InputLVar.cpp \
../src/LinguisticTerm.cpp \
../src/LinguisticVariable.cpp \
../src/MamdaniConsequent.cpp \
../src/MamdaniRule.cpp \
../src/OutputLVar.cpp \
../src/RectangularTerm.cpp \
../src/RuleBlock.cpp \
../src/ShoulderTerm.cpp \
../src/SingletonTerm.cpp \
../src/StrOp.cpp \
../src/TakagiSugenoConsequent.cpp \
../src/TakagiSugenoRule.cpp \
../src/TakagiSugenoTerm.cpp \
../src/TrapezoidalTerm.cpp \
../src/TriangularTerm.cpp \
../src/main.cpp \
../src/test.cpp 

OBJS += \
./src/AreaCentroidAlgorithm.o \
./src/CompoundTerm.o \
./src/DescriptiveAntecedent.o \
./src/DiscreteTerm.o \
./src/FunctionTerm.o \
./src/FuzzyAnd.o \
./src/FuzzyAntecedent.o \
./src/FuzzyConsequent.o \
./src/FuzzyDefuzzifier.o \
./src/FuzzyEngine.o \
./src/FuzzyException.o \
./src/FuzzyExceptions.o \
./src/FuzzyModulation.o \
./src/FuzzyOperation.o \
./src/FuzzyOperator.o \
./src/FuzzyOr.o \
./src/FuzzyRule.o \
./src/Hedge.o \
./src/HedgeSet.o \
./src/InfixToPostfix.o \
./src/InputLVar.o \
./src/LinguisticTerm.o \
./src/LinguisticVariable.o \
./src/MamdaniConsequent.o \
./src/MamdaniRule.o \
./src/OutputLVar.o \
./src/RectangularTerm.o \
./src/RuleBlock.o \
./src/ShoulderTerm.o \
./src/SingletonTerm.o \
./src/StrOp.o \
./src/TakagiSugenoConsequent.o \
./src/TakagiSugenoRule.o \
./src/TakagiSugenoTerm.o \
./src/TrapezoidalTerm.o \
./src/TriangularTerm.o \
./src/main.o \
./src/test.o 

CPP_DEPS += \
./src/AreaCentroidAlgorithm.d \
./src/CompoundTerm.d \
./src/DescriptiveAntecedent.d \
./src/DiscreteTerm.d \
./src/FunctionTerm.d \
./src/FuzzyAnd.d \
./src/FuzzyAntecedent.d \
./src/FuzzyConsequent.d \
./src/FuzzyDefuzzifier.d \
./src/FuzzyEngine.d \
./src/FuzzyException.d \
./src/FuzzyExceptions.d \
./src/FuzzyModulation.d \
./src/FuzzyOperation.d \
./src/FuzzyOperator.d \
./src/FuzzyOr.d \
./src/FuzzyRule.d \
./src/Hedge.d \
./src/HedgeSet.d \
./src/InfixToPostfix.d \
./src/InputLVar.d \
./src/LinguisticTerm.d \
./src/LinguisticVariable.d \
./src/MamdaniConsequent.d \
./src/MamdaniRule.d \
./src/OutputLVar.d \
./src/RectangularTerm.d \
./src/RuleBlock.d \
./src/ShoulderTerm.d \
./src/SingletonTerm.d \
./src/StrOp.d \
./src/TakagiSugenoConsequent.d \
./src/TakagiSugenoRule.d \
./src/TakagiSugenoTerm.d \
./src/TrapezoidalTerm.d \
./src/TriangularTerm.d \
./src/main.d \
./src/test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DFL_USE_LOG -DFL_USE_DEBUG -DFL_USE_ASSERT -I.. -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



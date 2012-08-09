################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DiscreteTerm.cpp \
../src/FuzzyAction.cpp \
../src/FuzzyEngine.cpp \
../src/FuzzyException.cpp \
../src/FuzzyExceptions.cpp \
../src/FuzzyOperator.cpp \
../src/FuzzyProposition.cpp \
../src/InputLVar.cpp \
../src/LinguisticVariable.cpp \
../src/MamdaniRule.cpp \
../src/OutputLVar.cpp \
../src/TrapezoidDTerm.cpp \
../src/TriangularDTerm.cpp \
../src/main.cpp \
../src/test.cpp 

OBJS += \
./src/DiscreteTerm.o \
./src/FuzzyAction.o \
./src/FuzzyEngine.o \
./src/FuzzyException.o \
./src/FuzzyExceptions.o \
./src/FuzzyOperator.o \
./src/FuzzyProposition.o \
./src/InputLVar.o \
./src/LinguisticVariable.o \
./src/MamdaniRule.o \
./src/OutputLVar.o \
./src/TrapezoidDTerm.o \
./src/TriangularDTerm.o \
./src/main.o \
./src/test.o 

CPP_DEPS += \
./src/DiscreteTerm.d \
./src/FuzzyAction.d \
./src/FuzzyEngine.d \
./src/FuzzyException.d \
./src/FuzzyExceptions.d \
./src/FuzzyOperator.d \
./src/FuzzyProposition.d \
./src/InputLVar.d \
./src/LinguisticVariable.d \
./src/MamdaniRule.d \
./src/OutputLVar.d \
./src/TrapezoidDTerm.d \
./src/TriangularDTerm.d \
./src/main.d \
./src/test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DFL_USE_LOG -DFL_USE_DEBUG -DFL_USE_INLINE -I/Users/jcrada/Development/fuzzy-lite/no-git/fuzzy-lite/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



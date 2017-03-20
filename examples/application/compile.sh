#!/bin/bash

FL_HOME="../../fuzzylite/"

#Static Linking
#--------------
#For C++11
g++ src/main.cpp -oexample-static-11 -I$FL_HOME -L$FL_HOME/release/bin -lfuzzylite-static -std=c++11

#For C++98
g++ src/main.cpp -oexample-static-98 -I$FL_HOME -L$FL_HOME/release/bin -lfuzzylite-static -DFL_CPP98=ON -Wno-non-literal-null-conversion


#Dynamic Linking
#---------------
#For C++11
g++ src/main.cpp -oexample-dynamic-11 -I$FL_HOME -L$FL_HOME/release/bin -lfuzzylite -std=c++11

#For C++98
g++ src/main.cpp -oexample-dynamic-98 -I$FL_HOME -L$FL_HOME/release/bin -lfuzzylite -DFL_CPP98=ON -Wno-non-literal-null-conversion

#Note: when using dynamic linking, the path to fuzzylite libraries must be specified. For example, run from console the following:

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$FL_HOME/release/bin;
./example-dynamic-11

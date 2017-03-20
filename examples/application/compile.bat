FL_HOME="../../fuzzylite/"

rem Static Linking
rem --------------
rem For C++11
cl.exe src/main.cpp %FL_HOME%/release/bin/fuzzylite-static.lib /I%FL_HOME% /EHsc /MD 

rem For C++98
cl.exe src/main.cpp %FL_HOME%/release/bin/fuzzylite-static.lib /I%FL_HOME% /DFL_CPP98=ON /EHsc /MD 


rem Dynamic Linking
rem ---------------
rem For C++11
rem For C++11
cl.exe src/main.cpp %FL_HOME%/release/bin/fuzzylite-static.lib /I%FL_HOME% /DFL_IMPORT_LIBRARY /EHsc /MD 

rem For C++98
cl.exe src/main.cpp %FL_HOME%/release/bin/fuzzylite-static.lib /I%FL_HOME% /DFL_CPP98=ON /DFL_IMPORT_LIBRARY /EHsc /MD 

rem Note: when using dynamic linking, the path to fuzzylite libraries must be specified. For example, run from console the following:

set PATH="%FL_HOME%\release\bin;%PATH%"
example-dynamic-11.exe

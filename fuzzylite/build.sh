#!/bin/bash

if [ -z "$FL_USE_FLOAT" ]; then
    FL_USE_FLOAT="OFF"
fi

if [ -z "$FL_CPP98" ]; then
    FL_CPP98="OFF"
fi

if [ -z "$FL_BUILD_TESTS" ]; then
    FL_BUILD_TESTS="OFF"
fi

debug(){
    set -e
    mkdir -p debug
    cd debug
    cmake .. -G"Unix Makefiles" -DFL_USE_FLOAT=${FL_USE_FLOAT} -DFL_CPP98=${FL_CPP98} -DCMAKE_BUILD_TYPE=Debug -DFL_BACKTRACE=ON  -DFL_BUILD_TESTS=${FL_BUILD_TESTS}
    make all
    if [ "${FL_BUILD_TESTS}" == "ON" ]; then
        (export CTEST_OUTPUT_ON_FAILURE=TRUE; make test)
    fi
    cd ..
}

release(){
    set -e
    mkdir -p release
    cd release
    cmake .. -G"Unix Makefiles" -DFL_USE_FLOAT=${FL_USE_FLOAT} -DFL_CPP98=${FL_CPP98} -DCMAKE_BUILD_TYPE=Release -DFL_BACKTRACE=ON -DFL_BUILD_TESTS=${FL_BUILD_TESTS}
    make all
    if [ "${FL_BUILD_TESTS}" == "ON" ]; then
        (export CTEST_OUTPUT_ON_FAILURE=TRUE; make test)
    fi
    cd ..
}

documentation(){
    set -e
    cd ..
    doxygen Doxyfile
    cd -
}

all(){
    debug
    release
}

clean(){
    rm -rf release debug CMakeFiles
}

usage(){
    printf 'Usage:\t[bash] ./build.sh [options]\n'
    printf "where\t[options] can be any of the following:\n"
    printf "\tall\t\t builds fuzzylite in debug and release mode (default)\n"
    printf "\tdebug\t\t builds fuzzylite in debug mode\n"
    printf "\trelease\t\t builds fuzzylite in release mode\n"
    printf "\tclean\t\t erases previous builds\n"
    printf "\thelp\t\t shows this information\n"
    printf "\n"
}

#############################
echo "Parameters: $@"

OPTIONS=( "all" "release" "debug" "clean" "documentation" "help")
BUILD=( )

for arg in "$@"
do
    if [[ "$arg" == "help" ]]; then usage && exit 0; fi

    if [[ "$arg" == "all" || "$arg" == "debug" || "$arg" == "release" || "$arg" == "clean" || "$arg" == "documentation" ]];
    then BUILD+=( $arg ); else echo "Invalid option: $arg" && usage && exit 2;
    fi
done

if [ ${#BUILD[@]} -eq 0 ]; then BUILD+=( "release" "debug" ); fi

echo "Building schedule: ${BUILD[@]}"
echo "Starting in 3 seconds..."
sleep 3

for option in "${BUILD[@]}"
do
    printf "\n\n"
    printf "******************************\n"
    printf "STARTING: $option\n"
    eval ${option}
    printf "\nFINISHED: $option\n"
    printf "******************************\n\n"
done




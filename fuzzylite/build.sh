#!/bin/bash

if [ -z "$FL_USE_FLOAT" ]; then
    FL_USE_FLOAT="OFF"
fi

if [ -z "$FL_CPP11" ]; then
    FL_CPP11="ON"
fi

debug(){
    set -e
    mkdir -p debug
    cd debug
    cmake .. -G"Unix Makefiles" -DFL_USE_FLOAT=${FL_USE_FLOAT} -DFL_CPP11=${FL_CPP11} -DCMAKE_BUILD_TYPE=Debug -DFL_BACKTRACE=ON 
    make ${MAKEFLAGS} all
    cd ..
}

release(){
    set -e
    mkdir -p release
    cd release
    cmake .. -G"Unix Makefiles" -DFL_USE_FLOAT=${FL_USE_FLOAT} -DFL_CPP11=${FL_CPP11} -DCMAKE_BUILD_TYPE=Release -DFL_BACKTRACE=ON 
    make ${MAKEFLAGS} all
    cd ..
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

OPTIONS=( "all" "debug" "release" "clean" "help")
BUILD=( )

for arg in "$@"
do
    if [[ "$arg" == "help" ]]; then usage && exit 0; fi

    if [[ "$arg" == "all" || "$arg" == "debug" || "$arg" == "release" || "$arg" == "clean" ]];
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



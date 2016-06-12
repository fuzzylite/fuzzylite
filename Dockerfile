FROM ubuntu:latest
MAINTAINER Juan Rada-Vilela <jcrada@fuzzylite.com>

ARG CXX_COMPILER=g++
ENV CXX_COMPILER ${CXX_COMPILER}

RUN apt-get update && apt-get -y install \
    ${CXX_COMPILER} \
    make \
    cmake \
    doxygen

RUN mkdir /build
WORKDIR /build
ENTRYPOINT [ "/build/build.sh" ]
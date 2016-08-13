FROM ubuntu:latest
MAINTAINER Juan Rada-Vilela <jcrada@fuzzylite.com>

ARG CXX_COMPILER=g++
ENV CXX_COMPILER ${CXX_COMPILER}

RUN apt-get update && apt-get -y install \
    ${CXX_COMPILER} \
    make \
    cmake 

#Create and copy Docker's context into /build
RUN mkdir /build
ADD . /build
WORKDIR /build/fuzzylite
ENTRYPOINT [ "/build/fuzzylite/build.sh" ]
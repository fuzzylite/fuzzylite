FROM ubuntu:latest
MAINTAINER Juan Rada-Vilela <jcrada@fuzzylite.com>

ARG CXX_COMPILER=g++
ENV CXX_COMPILER ${CXX_COMPILER}


#install software-properties-common to use add-apt to add repository for g++-6
RUN apt-get update && apt-get -y install software-properties-common && \
    add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && apt-get -y install \
    ${CXX_COMPILER} \
    make \
    cmake \
    doxygen \
    graphviz

#Create and copy Docker's context into /build
RUN mkdir /build
ADD . /build
WORKDIR /build/fuzzylite
ENTRYPOINT [ "/build/fuzzylite/build.sh" ]
FROM ubuntu:24.04

RUN apt-get update
RUN apt-get -y install clang-18 g++-14
RUN apt-get -y install cmake build-essential git
RUN apt-get -y install python3.12 python3-pip python3-venv

ENV CXX="/usr/bin/clang++-18"
ENV GCOV="llvm-cov-18 gcov"

COPY . /install

WORKDIR /install

CMD ["make", "COVERAGE=ON", "all", "coverage"]

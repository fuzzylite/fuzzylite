FROM ubuntu:24.04

RUN apt-get update; \
    apt-get -y install clang-18 g++-14 gdb; \
    apt-get -y install cmake build-essential git; \
    apt-get -y install python3.12 python3-pip python3-venv;

COPY . /install

WORKDIR /install

ENV CXX="clang++-18"
ENV GCOV="llvm-cov-18 gcov"

CMD ["make", "all", "coverage", "COVERAGE=ON"]

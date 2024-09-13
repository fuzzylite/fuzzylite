FROM continuumio/miniconda3

RUN conda install -y -c conda-forge bash notebook
RUN conda install -y -c conda-forge xeus-cling xtensor

RUN apt-get update && apt-get -y install g++-multilib cmake clang

COPY . /install

WORKDIR /install

RUN make configure build install

WORKDIR /mnt/fuzzylite

FROM continuumio/miniconda3

RUN conda install -y -c conda-forge bash jupyter jupyter_contrib_nbextensions
RUN conda install -y -c conda-forge xeus-cling xtensor

RUN apt-get update && apt-get -y install g++-multilib cmake clang

COPY . /install

WORKDIR /install

RUN make configure make install

WORKDIR /mnt/fuzzylite
# docker build -f test/Dockerfile -t xeus .
# docker run --rm -p 8888:8888 -v.:/mnt/fuzzylite -it xeus jupyter notebook --allow-root --ip 0.0.0.0

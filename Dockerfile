#################################################
# Dockerfile to build LHCb-simulation container #
# Based on Ubuntu xenial                        #  
#################################################
FROM ubuntu:16.04
MAINTAINER Kenenbek Arzymatov "kenenbek@gmail.com"
RUN apt-get update && apt-get install -y \
            sudo \
            git \
            build-essential \
            cmake \
            libboost-dev \
            libboost-all-dev \
            doxygen \
            python3
RUN git clone https://github.com/simgrid/simgrid.git
WORKDIR "/simgrid"
RUN cmake -Denable_documentation=OFF -Denable_coverage=OFF -Denable_java=OFF -Denable_model-checking=OFF \
    -Denable_lua=OFF -Denable_compile_optimizations=OFF -Denable_smpi=OFF -Denable_smpi_MPICH3_testsuite=OFF -Denable_compile_warnings=OFF .
RUN sudo make
RUN sudo make install
RUN mkdir -p ~/Travisci
COPY * ~/Travisci
CMD ["./run.sh"]



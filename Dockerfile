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
			unzip \
            python3 \
			wget 

# Install SimGrid
RUN git clone https://github.com/simgrid/simgrid.git
WORKDIR "/simgrid"
RUN cmake -Denable_documentation=OFF -Denable_coverage=OFF -Denable_java=OFF -Denable_model-checking=OFF \
    -Denable_lua=OFF -Denable_compile_optimizations=OFF -Denable_smpi=OFF -Denable_smpi_MPICH3_testsuite=OFF -Denable_compile_warnings=OFF .
RUN sudo sync; sudo make
RUN sudo make install

# Install parser with wget
RUN mkdir -p ~/Downloads
WORKDIR "~/Downloads"
RUN wget -O csvparser.zip https://sourceforge.net/projects/cccsvparser/files/2016-03-28/CsvParser_2016_03_28.zip/download
RUN unzip csvparser.zip
WORKDIR "~/Downloads/csvparser"
mkdir -p lib
RUN -Iinclude src/csvparser.c -fPIC -shared -o lib/libcsvparser.so
RUN sudo cp lib/libcsvparser.so /usr/lib
RUN include/csvparser.h /usr/include

# LHCb grid simulation project 
RUN sudo sysctl -w vm.max_map_count=500000
RUN mkdir -p ~/LHCb/grid_simulation
COPY ./* ~/LHCb/grid_simulation/
WORKDIR "~/LHCb/grid_simulation"
CMD ["./run.sh"]

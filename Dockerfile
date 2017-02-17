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

# Install SimGrid
RUN git clone https://github.com/simgrid/simgrid.git
WORKDIR "/simgrid"
RUN cmake -Denable_documentation=OFF -Denable_coverage=OFF -Denable_java=OFF -Denable_model-checking=OFF \
    -Denable_lua=OFF -Denable_compile_optimizations=OFF -Denable_smpi=OFF -Denable_smpi_MPICH3_testsuite=OFF -Denable_compile_warnings=OFF .
RUN sudo sync; sudo make; sudo make install;
RUN cd lib && sudo cp * /usr/lib; cd ../include && sudo cp -a * /usr/include

# Install yaml-cpp parser
RUN git clone https://github.com/jbeder/yaml-cpp.git
WORKDIR "/yaml-cpp"
RUN mkdir build && cd build && cmake -DBUILD_SHARED_LIBS=ON .. && cd .. && make && make install
 
# LHCb grid simulation project
WORKDIR "/"
RUN git clone https://github.com/skygrid/grid_simulation.git
WORKDIR "/grid_simulation"
RUN sudo sysctl -w vm.max_map_count=500000
CMD ["./run.sh"]

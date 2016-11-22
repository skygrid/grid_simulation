#################################################
# Dockerfile to build LHCb-simulation container #
# Based on Ubuntu xenial                        #  
#################################################

FROM ubuntu:16.04
MAINTAINER Kenenbek Arzymatov <kenenbek@gmail.com>
RUN apt-get update 
RUN apt-get install -y git cmake build-essential libboost-dev
RUN mkdir -p ~/LHCb/grid_simulation


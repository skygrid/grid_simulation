FROM ubuntu:16.04
MAINTAINER Kate Smith <ksmith@example.com>
RUN apt-get update && apt-get install -y git cmake build-essentials

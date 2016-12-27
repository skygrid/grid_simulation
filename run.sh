#!/bin/bash
cmake .
make
sudo ./CSim2Sim platform/platform.xml \
platform/deployment.xml \
out.txt \
simple \
--cfg=contexts/nthreads:1 \
--cfg=tracing:yes \
--cfg=tracing/platform:yes \
--cfg=tracing/filename:trace \
--cfg=maxmin/concurrency_limit:200000 \
--cfg=storage/max_file_descriptors:220000 \

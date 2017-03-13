#!/usr/bin/env bash
cmake .
make
sudo ./CSim2Sim \
--cfg=contexts/nthreads:1 \
--cfg=tracing:yes \
--cfg=tracing/platform:yes \
--cfg=tracing/filename:trace \
--cfg=maxmin/concurrency_limit:200000 \
--cfg=storage/max_file_descriptors:220000
cp out.txt trace simgridoutput/
cd simgridoutput/
# python clear_trace.py trace

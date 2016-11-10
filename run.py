import sys, os

os.system("ulimit -c unlimited")

os.system("cmake .")
os.system("make")

os.system("sudo ./CSim2Sim /home/ken/LHCb/grid_simulation/platform/platform.xml \
/home/ken/LHCb/grid_simulation/platform/deployment.xml \
/home/ken/LHCb/grid_simulation/trash/out.txt \
DAM \
--cfg=contexts/nthreads:2 \
--cfg=tracing:yes \
--cfg=tracing/platform:yes \
--cfg=tracing/filename:/home/ken/LHCb/grid_simulation/trash/trace \
--cfg=maxmin/concurrency_limit:100000 \
--cfg=storage/max_file_descriptors:220000")

# LHCb grid simulation [![Build Status](https://travis-ci.org/skygrid/grid_simulation.svg?branch=master)](https://travis-ci.org/skygrid/grid_simulation)

## Goals and problems 
Developing smarter algorithms for

1. Data management
2. Predicting anomalies
3. Job scheduling/matchning

## This research focuses on real interaction of 
1. CPU resources
2. Networks
3. Storages

## Installation process

We use [SimGrid framework](https://github.com/simgrid/simgrid). You should install it.
After installation of SimGrid, clone this project and run in command line

```
cmake . 
make .
./CSim2Sim --cfg=contexts/nthreads:2 --cfg=tracing:yes --cfg=tracing/platform:yes --cfg=tracing/filename:/home/ken/LHCb/grid_simulation/trash/trace --cfg=maxmin/concurrency_limit:100000 --cfg=storage/max_file_descriptors:220000"
```

## Job workflow

Information about jobs are contained in __input.csv__.

Topology of grid (cores, links and storages infrastructure) is contained in __Platform__ folder.

## Also

You could check [presentation](https://github.com/skygrid/grid_simulation/blob/master/results/work.pdf) about LHCb simulation and
main current results in [jupyter notebook](https://github.com/skygrid/grid_simulation/blob/master/results/Plots.ipynb).

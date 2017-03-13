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

## Job workflow

![Alt text](https://pp.vk.me/c638316/v638316287/23a2c/Z2Zl5Qtk9eg.jpg "Simulation process")

## Installation process

In this project we use two additional libraries. [SimGrid framework](https://github.com/simgrid/simgrid) is used for the purposes of simulation flow and [YAML cpp](https://github.com/jbeder/yaml-cpp) is used for parsing input files containing information about jobs and datasets.
After installation of SimGrid and YAML-cpp, clone this project:
```
git clone https://github.com/skygrid/grid_simulation.git
```
and run in the command line:

```
mkdir build
cd build
cmake .. 
make
./CSim2Sim --cfg=tracing:yes --cfg=tracing/platform:yes --cfg=tracing/filename:<path to trace file> --cfg=maxmin/concurrency_limit:100000 --cfg=storage/max_file_descriptors:220000"
```
Change `config.yml` to customize paths to the following files:
```
platform: platform/platform.xml           ## Contains grid topology
deployment: platform/test-deployment.xml  ## Defines `behaviour` of sites 
out.txt: out.txt                          ## Jobs metrics are written here 
jobs: InputData/little_jobs.yml           ## Queue of waiting jobs
input: InputData/little_data.yml          ## Datasets stored in all tiers 
```
## Docker

If you feel free with Docker you can play around with grid_simulation in container.

You just need to pull images DockerHub repository:
```
docker pull kenenbek/grid_simulation:february
```

Or you can build grid_simulation image based on `Dockerfile` on your own:
```
docker build -t <container name> .
```

This image contains all necessary software to run a simulation. For this, clone this repository. This creates **/grid_simulation** directory with scripts and input data of the simulation:

```
git clone https://github.com/skygrid/grid_simulation.git
```

The docker image has a `CMD` instruction which starts an execution of `run.sh` file. This file is in your **/grid_simulation** directory. To run the simuation use the command:
```
docker run -v $(pwd)/grid_simulation:/grid_simulation <container name>
```

Results of the simulation are in your **/grid_simulation** directory.

## Cern OpenStack Instructions Using Docker
1. Create instance on [openstack.cern.ch](https://openstack.cern.ch). Use one of the Ubuntu 14+ images. These images are available on [cloud-images.ubuntu.com](http://cloud-images.ubuntu.com/).

2. Log in your instance and run the command:

    ```bash
    sudo apt-get update
    ```

3. Install Docker using the [instructions](https://docs.docker.com/engine/installation/linux/ubuntulinux/). If your are non-root user, run the following commands:

    ```bash
    sudo groupadd docker
    sudo usermod -aG docker $USER
    sudo service docker restart
    ```

    Verify the installation by running:

    ```bash
    sudo docker run hello-world
    sudo docker ps -a
    ```

    These commands should work without errors.


## Output

In the wake of simulation you should look for `out.txt` and `trace.csv` (located in `simgridoutput directory`) files.
The first one contains all relevant information about job execution process:
```
+-----------+------+---------------+--------------+-----------+------------+-----------+--------------+
| JobId     | Done | Submission(s) | Schedule (s) | Start (s) | Finish (s) | Tier name | JobType      |
+-----------+------+---------------+--------------+-----------+------------+-----------+--------------+
| 141647523 | 1    | 0             | 10.5645      | 56.2571   | 14974.7336 | CERN-PROD | MCSimulation |
+-----------+------+---------------+--------------+-----------+------------+-----------+--------------+
```

The second one does information about resource(CPU-cores, network, etc.) usage:
```
+---------------------+----------+-----------+-----------+----------+--------------+
| Resource name       | Variable | Start     | End       | Duration | Value        |
+---------------------+----------+-----------+-----------+----------+--------------+
| CERN-RAL-LHCOPN-001 | traffic  | 17483.121 | 17535.824 | 52.702   | 342164520177 |
+---------------------+----------+-----------+-----------+----------+--------------+
```

## Also

You could check [presentation](https://github.com/skygrid/grid_simulation/blob/master/results/work.pdf) about LHCb simulation and
main current results in [jupyter notebook](https://github.com/skygrid/grid_simulation/blob/master/results/Plots.ipynb).

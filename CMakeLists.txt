cmake_minimum_required(VERSION 2.8)
project(CSim2Sim)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0")

set(SOURCE_FILES src/main.cpp src/my_structures.h src/scheduler.cpp src/data_replicator.cpp src/tier1.cpp src/myfunc_list.h src/job_requester.cpp src/writeToFile.cpp src/tracer_link.cpp src/tracer_host.cpp src/evil.cpp src/rescheduling.cpp src/tracer_storage.cpp src/executor.cpp src/matcher.cpp src/trace.cpp src/file_management.cpp src/killer.cpp src/yaml.cpp src/global_variables.cpp)
add_executable(CSim2Sim ${SOURCE_FILES})

include_directories(/home/kenenbek/SimGrid/include)
link_directories(/home/kenenbek/SimGrid/lib)
#set(HEADER_FILES)

target_link_libraries(CSim2Sim yaml-cpp)
#target_link_libraries(CSim2Sim /home/kenenbek/SimGrid/lib/libsimgrid.so)
target_link_libraries(CSim2Sim simgrid)

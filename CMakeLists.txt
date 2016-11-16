cmake_minimum_required(VERSION 3.5)
project(CSim2Sim)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0")

set(SOURCE_FILES src/main.cpp src/messages.h src/scheduler.cpp src/data_replicator.cpp src/tier1.cpp src/myfunc_list.h src/job_requester.cpp src/writeToFile.cpp src/tracer_link.cpp src/tracer_host.cpp src/evil.cpp src/rescheduling.cpp src/tracer_storage.cpp src/executor.cpp src/input.cpp src/matcher.cpp src/trace.cpp src/file_management.cpp src/killer.cpp)
add_executable(CSim2Sim ${SOURCE_FILES})

target_link_libraries(CSim2Sim csvparser)
find_library(LIB_SIMGRID NAMES simgrid libsimgrid.so libprofiler.so.V123)
target_link_libraries(CSim2Sim ${LIB_SIMGRID})

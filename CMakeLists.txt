cmake_minimum_required(VERSION 2.8)
project(CSim2Sim)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 --coverage")

set(SOURCE_FILES src/main.cpp src/messages.h src/scheduler.cpp src/data_replicator.cpp src/tier1.cpp src/myfunc_list.h src/job_requester.cpp src/writeToFile.cpp src/tracer_link.cpp src/tracer_host.cpp src/evil.cpp src/rescheduling.cpp src/tracer_storage.cpp src/executor.cpp src/input.cpp src/matcher.cpp src/trace.cpp src/file_management.cpp src/killer.cpp)
add_executable(CSim2Sim ${SOURCE_FILES})

find_library(LIB_CSVPARSER NAMES csvparser libcsvparser.so csvparser.so.V123)
target_link_libraries(CSim2Sim ${LIB_CSVPARSER})

find_library(LIB_SIMGRID NAMES simgrid libsimgrid.so libprofiler.so.V123)
target_link_libraries(CSim2Sim ${LIB_SIMGRID})



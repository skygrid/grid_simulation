cmake_minimum_required(VERSION 3.4)
project(CSim2Sim)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0")

set(SOURCE_FILES main.c messages.h dispatcher.c scheduler.c tier2.c data_replicator.c executor.c tier1.c myfunc_list.h job_requester.c str_to_enum.c)
add_executable(CSim2Sim ${SOURCE_FILES})

target_link_libraries(CSim2Sim csvparser)

#include_directories(~/SimGrid/include ~/SimGrid/bin ~/SimGrid/lib)
#link_directories(~/SimGrid/lib)
target_link_libraries(CSim2Sim simgrid)

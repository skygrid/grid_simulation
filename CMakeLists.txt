cmake_minimum_required(VERSION 3.5)
project(CSim2Sim)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp messages.h scheduler.cpp data_replicator.cpp tier1.cpp myfunc_list.h job_requester.cpp writeToFile.cpp tracer_link.cpp tracer_host.cpp evil.cpp rescheduling.cpp tracer_storage.cpp executor.cpp input.cpp matcher.cpp trace.cpp file_management.cpp killer.cpp)
add_executable(CSim2Sim ${SOURCE_FILES})

target_link_libraries(CSim2Sim csvparser)
target_link_libraries(CSim2Sim simgrid)

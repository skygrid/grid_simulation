cmake_minimum_required(VERSION 3.5)
project(CSim2Sim)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -g")

set(SOURCE_FILES main.c messages.h scheduler.c data_replicator.c tier1.c myfunc_list.h job_requester.c str_to_enum.c writeToFile.c pileOfFailedTask.c set_0_all_routes.c tracer_link.c tracer_host.c evil.c rescheduling.c main_tracer.c executor.c input.c)
add_executable(CSim2Sim ${SOURCE_FILES})
target_link_libraries(CSim2Sim csvparser)
target_link_libraries(CSim2Sim simgrid)

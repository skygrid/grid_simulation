cmake_minimum_required(VERSION 3.4)
project(CSim2Sim)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0")

set(SOURCE_FILES main.c messages.c messages.h tier_sender_downloader.c dispatcher.c scheduler.c node.c my_functions.c tier_sender_downloader.h)
add_executable(CSim2Sim ${SOURCE_FILES})

target_link_libraries(CSim2Sim csvparser)

include_directories(SimGrid/include SimGrid/bin)
link_directories(SimGrid/lib)
target_link_libraries(CSim2Sim simgrid)

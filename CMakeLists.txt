cmake_minimum_required(VERSION 2.8)
project(potatosearch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Debug)

#potato

ADD_EXECUTABLE( potatosearch ${CMAKE_CURRENT_SOURCE_DIR}/potatosearch.cc )
include_directories(potatosearch)
TARGET_LINK_LIBRARIES( potatosearch m pthread)


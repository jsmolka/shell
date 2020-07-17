cmake_minimum_required(VERSION 3.8)

project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -flto")

include_directories(../include)
include_directories(catch2/single_include)
include_directories(src)

file(GLOB_RECURSE SOURCE_FILES
  ${PROJECT_SOURCE_DIR}/src/*.h
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

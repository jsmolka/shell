cmake_minimum_required(VERSION 3.8)

project(eggcpt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -flto")

include_directories(src)
include_directories(tests)

file(GLOB_RECURSE SOURCE_FILES
  ${PROJECT_SOURCE_DIR}/src/*.h
  ${PROJECT_SOURCE_DIR}/src/*.cpp
  ${PROJECT_SOURCE_DIR}/tests/*.h
  ${PROJECT_SOURCE_DIR}/tests/*.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

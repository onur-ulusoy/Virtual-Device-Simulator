cmake_minimum_required(VERSION 3.5)

project(SpiProcessorWrapper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES SpiProcessorWrapper.cpp SpiProcessorUtil.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ../inc)

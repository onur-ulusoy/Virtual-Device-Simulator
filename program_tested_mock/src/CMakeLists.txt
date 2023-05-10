cmake_minimum_required(VERSION 3.10)
project(ProgramToBeTestedMock)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../env)

include_directories(../inc)

add_executable(ProgramToBeTestedMock
    ProgramToBeTested.cpp
)

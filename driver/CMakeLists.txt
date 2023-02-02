cmake_minimum_required(VERSION 3.24)
project(driver)

set(CMAKE_CXX_STANDARD 17)

add_executable(program main.cpp DriverLibrary.cpp DriverLibrary.h AuxiliaryTools.cpp AuxiliaryTools.h)

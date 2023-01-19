cmake_minimum_required(VERSION 3.24)
project(program)

set(CMAKE_CXX_STANDARD 17)

add_executable(program main.cpp gpiolib.cpp gpiolib.h)

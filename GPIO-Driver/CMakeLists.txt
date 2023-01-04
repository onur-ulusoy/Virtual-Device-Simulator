cmake_minimum_required(VERSION 3.24)
project(bbx15)

set(CMAKE_CXX_STANDARD 17)

add_executable(bbx15 main.cpp gpiolib.cpp gpiolib.h gpiolib.h)

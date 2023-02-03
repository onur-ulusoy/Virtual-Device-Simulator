cmake_minimum_required(VERSION 3.24)
project(driver)

set(CMAKE_CXX_STANDARD 17)

add_executable( driver drivermain.cpp
                libdriver.cpp
                libdriver.h
                libdriverutility.cpp
                libdriverutility.h
                drivercommapi.cpp
                drivercommapi.h
                driverfunctions.h )

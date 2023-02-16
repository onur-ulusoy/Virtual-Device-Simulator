cmake_minimum_required(VERSION 3.0)

project(drivertest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../")

# Set the path to the directory containing libdriver.hpp
set(LIBDRIVER_INCLUDE_DIR "../")

# Add the directory to the list of include directories
include_directories(${LIBDRIVER_INCLUDE_DIR})

# Add the source files for the driver and tester
set(drivertest_src_files
    test.cpp
    ../libdriver.cpp
    ../libdriver.hpp
    ../libdriverutility.cpp
    ../libdriverutility.hpp
    ../drivercommapi.cpp
    ../drivercommapi.hpp
)

# Define the driver executable target
add_executable(drivertest ${drivertest_src_files})

# Set the directory containing the gtest header files
set(GTEST_INCLUDE_DIR "$ENV{HOME}/Desktop/Virtual-Device-Simulator/googletest/googletest/include")

# Add the directory to the list of include directories
include_directories(${GTEST_INCLUDE_DIR})

# Set the directory containing the gtest library file
set(GTEST_LIB_DIR "$ENV{HOME}/Desktop/Virtual-Device-Simulator/googletest/build/lib")

# Add the gtest library to the test executable
target_link_libraries(drivertest ${GTEST_LIB_DIR}/libgtest.a ${GTEST_LIB_DIR}/libgtest_main.a)

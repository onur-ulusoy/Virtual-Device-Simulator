cmake_minimum_required(VERSION 3.0)

project(integritytest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../../")

# Add the source files for the driver and tester
set(integrity_src_files
    integrity_test.cpp
)

# Define the driver executable target
add_executable(integritytest ${integrity_src_files})

# Set the directory containing the gtest header files
set(GTEST_INCLUDE_DIR "$ENV{HOME}/Desktop/Virtual-Device-Simulator/googletest/googletest/include")

# Add the directory to the list of include directories
include_directories(${GTEST_INCLUDE_DIR})

# Set the directory containing the gtest library file
set(GTEST_LIB_DIR "$ENV{HOME}/Desktop/Virtual-Device-Simulator/googletest/build/lib")

# Add the gtest library to the test executable
target_link_libraries(integritytest ${GTEST_LIB_DIR}/libgtest.a ${GTEST_LIB_DIR}/libgtest_main.a)

# Set the directory containing the driver header files
set(DRIVER_INCLUDE_DIR "${CMAKE_BINARY_DIR}/../../../driver")

# Add the directory to the list of include directories
include_directories(${DRIVER_INCLUDE_DIR})

# Set the directory containing the tester header files
set(TESTER_INCLUDE_DIR "${CMAKE_BINARY_DIR}/../../../tester")

# Add the directory to the list of include directories
include_directories(${TESTER_INCLUDE_DIR})

# Specify the binary directory for the subdirectory
set(SUBDIR_BINARY_DIR "${CMAKE_BINARY_DIR}/../../")

# Include another CMakeLists.txt file in a subdirectory to compile driver and tester

add_subdirectory(${SUBDIR_BINARY_DIR} lib)

# Add a link with the driver and tester targets
set(DEV_LIB_DIR "${CMAKE_BINARY_DIR}/lib")
target_link_libraries(integritytest ${DEV_LIB_DIR}/libdriverlib.a ${DEV_LIB_DIR}/libtesterlib.a)

set_target_properties(integritytest PROPERTIES OUTPUT_NAME "integritytest.out")

add_dependencies(integritytest driverlib testerlib)
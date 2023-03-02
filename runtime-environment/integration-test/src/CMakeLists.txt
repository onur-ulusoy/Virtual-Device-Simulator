include_directories(../inc)

include_directories(../driver/inc)
include_directories(../driver/src)

include_directories(../tester/inc)
include_directories(../tester/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../")

# Add the source and include files
set(integration_src_files
    integration_test_main.cpp
    lib-integrationtest.cpp
)

set(integration_inc_files
    ../inc/lib-integrationtest.hpp
)

# Define the driver executable target
add_executable(integrationtest ${integration_src_files} ${integration_inc_files})

# Set the directory containing the gtest header files
set(GTEST_INCLUDE_DIR "$ENV{HOME}/Desktop/Virtual-Device-Simulator/googletest/googletest/include")

# Add the directory to the list of include directories
include_directories(${GTEST_INCLUDE_DIR})

# Set the directory containing the gtest library file
set(GTEST_LIB_DIR "$ENV{HOME}/Desktop/Virtual-Device-Simulator/googletest/build/lib")

# Add the gtest library to the test executable
target_link_libraries(integrationtest ${GTEST_LIB_DIR}/libgtest.a ${GTEST_LIB_DIR}/libgtest_main.a)

# Set the directory containing the driver header files
set(DRIVER_INCLUDE_DIR "${CMAKE_BINARY_DIR}/../../../driver/inc")

# Add the directory to the list of include directories
include_directories(${DRIVER_INCLUDE_DIR})

# Set the directory containing the driver source files
set(DRIVER_SOURCE_DIR "${CMAKE_BINARY_DIR}/../../../driver/src")

# Add the directory to the list of source directories
include_directories(${DRIVER_SOURCE_DIR})

# Set the directory containing the tester header files
set(TESTER_INCLUDE_DIR "${CMAKE_BINARY_DIR}/../../../tester/inc")

# Add the directory to the list of include directories
include_directories(${TESTER_INCLUDE_DIR})

# Set the directory containing the source source files
set(TESTER_SOURCE_DIR "${CMAKE_BINARY_DIR}/../../../tester/src")

# Add the directory to the list of source directories
include_directories(${TESTER_SOURCE_DIR})

# Specify the binary directory for the subdirectory
set(SUBDIR_BINARY_DIR "${CMAKE_BINARY_DIR}/../../")

# Include another CMakeLists.txt file in a subdirectory to compile driver and tester

add_subdirectory(${SUBDIR_BINARY_DIR} lib)

# Add a link with the driver and tester targets
set(DEV_LIB_DIR "${CMAKE_BINARY_DIR}/lib")
target_link_libraries(integrationtest ${DEV_LIB_DIR}/libdriverlib.a ${DEV_LIB_DIR}/libtesterlib.a)

set_target_properties(integrationtest PROPERTIES OUTPUT_NAME "integrationtest.out")

add_dependencies(integrationtest driverlib testerlib)
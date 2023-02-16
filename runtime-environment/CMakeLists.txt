cmake_minimum_required(VERSION 3.0)

project(driver_and_tester)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")

# Add the source files for the driver and tester
set(driver_src_files
    ../driver/drivermain.cpp
    ../driver/libdriver.cpp
    ../driver/libdriver.hpp
    ../driver/libdriverutility.cpp
    ../driver/libdriverutility.hpp
    ../driver/drivercommapi.cpp
    ../driver/drivercommapi.hpp
)

set(tester_src_files
    ../tester/testermain.cpp
    ../tester/libtesterutility.cpp
    ../tester/libtesterutility.hpp
    ../tester/testercommapi.cpp
    ../tester/testercommapi.hpp
)

# Define the driver executable target
add_executable(driver ${driver_src_files})
target_include_directories(driver PRIVATE ../driver)
set_target_properties(driver PROPERTIES OUTPUT_NAME "driver.out")

# Define the tester executable target
add_executable(tester ${tester_src_files})
target_include_directories(tester PRIVATE ../tester)
set_target_properties(tester PROPERTIES OUTPUT_NAME "tester.out")

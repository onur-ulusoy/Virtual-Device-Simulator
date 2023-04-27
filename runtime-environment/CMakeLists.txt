include_directories(../driver/inc)
include_directories(../driver/src)

include_directories(../tester/inc)
include_directories(../tester/src)

cmake_minimum_required(VERSION 3.0)

project(driver_and_tester)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")

# Add the source and include files for the driver and tester
set(driver_src_files
    ../driver/src/drivermain.cpp
    ../driver/src/libdriver.cpp
    ../driver/src/libdriverutility.cpp
    ../driver/src/drivercommapi.cpp
)

set(driver_inc_files
    ../driver/inc/libdriver.hpp
    ../driver/inc/libdriverutility.hpp
    ../driver/inc/drivercommapi.hpp
)

set(tester_src_files
    ../tester/src/testermain.cpp
    ../tester/src/libtesterutility.cpp
    ../tester/src/testercommapi.cpp
)

set(tester_inc_files
    ../tester/inc/libtesterutility.hpp
    ../tester/inc/testercommapi.hpp
)

# Define the driver executable target
add_executable(driver ${driver_src_files} ${driver_inc_files})
target_include_directories(driver PRIVATE ../driver)
set_target_properties(driver PROPERTIES OUTPUT_NAME "driver.out")

# Define the tester executable target
add_executable(tester ${tester_src_files} ${tester_inc_files})
target_include_directories(tester PRIVATE ../tester)
set_target_properties(tester PROPERTIES OUTPUT_NAME "tester.out")

# Include SpiProcessor library
set(SpiProcessorWrapperDir "${CMAKE_CURRENT_SOURCE_DIR}/../spi-processing/cpp_wrapper/src/build")

# Set the directory containing the spi processor header files
set(SpiProcessorWrapperIncDir "${CMAKE_CURRENT_SOURCE_DIR}/../spi-processing/cpp_wrapper/inc")

# Add the directory to the list of include directories
include_directories(${SpiProcessorWrapperIncDir})

# Include cpp_interface library
set(comm_interface_dir "${CMAKE_CURRENT_SOURCE_DIR}/process-communication/cpp_comm_interface/src/build")

# Set the directory containing the cpp interface header files
set(comm_interface_inc_dir "${CMAKE_CURRENT_SOURCE_DIR}/process-communication/cpp_comm_interface/inc")

# Add the directory to the list of include directories
include_directories(${comm_interface_inc_dir})

find_package(PkgConfig)

pkg_check_modules(PC_ZeroMQ QUIET zmq)

find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

target_include_directories(driver PUBLIC ${ZeroMQ_INCLUDE_DIR})

target_link_libraries(driver PUBLIC ${ZeroMQ_LIBRARY} ${comm_interface_dir}/libcomm_interface.a ${SpiProcessorWrapperDir}/libSpiProcessorWrapper.a)

target_link_libraries(tester PUBLIC ${ZeroMQ_LIBRARY} ${comm_interface_dir}/libcomm_interface.a ${SpiProcessorWrapperDir}/libSpiProcessorWrapper.a)

# Create libraries
add_library(driverlib STATIC ${driver_src_files} ${driver_inc_files})
set_target_properties(driverlib PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_library(testerlib STATIC ${tester_src_files} ${tester_inc_files})
set_target_properties(testerlib PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
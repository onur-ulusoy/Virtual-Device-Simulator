include_directories(../inc)

include_directories(../../inc)
include_directories(../../src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../")

# Set the path to the directory containing libdriver.hpp
set(LIBDRIVER_INCLUDE_DIR "../../")

# Add the directory to the list of include directories
include_directories(${LIBDRIVER_INCLUDE_DIR})

# Add the source files for the driver and tester
set(drivertest_src_files
    unit_test_main.cpp
    lib-unittest.cpp
    MockTests.cpp
    #AttributeGettersTest.cpp
    #ConstructorTest.cpp
    #OpenReadOnlyTest.cpp
    #OpenWriteOnlyTest.cpp
    #OpenDefaultTest.cpp
    #CloseTest.cpp
    #ReadTest.cpp
    #ParseGPIOTest.cpp
    #ConfigTest.cpp
    #WriteTest.cpp
    #ShowTest.cpp
    #ParseTest.cpp
    
    ../../src/libdriver.cpp
    ../../src/libdriverutility.cpp
    ../../src/drivercommapi.cpp
)

set(drivertest_inc_files
    ../inc/lib-unittest.hpp
    ../../inc/libdriver.hpp
    ../../inc/libdriverutility.hpp
    ../../inc/drivercommapi.hpp
)

# Define the driver executable target
add_executable(drivertest ${drivertest_src_files} ${drivertest_inc_files})

# Set the directory containing the gtest header files
set(GTEST_INCLUDE_DIR "${CMAKE_BINARY_DIR}/../../../googletest/googletest/include")

# Add the directory to the list of include directories
include_directories(${GTEST_INCLUDE_DIR})

# Set the directory containing the gmock header files
set(GMOCK_INCLUDE_DIR "${CMAKE_BINARY_DIR}/../../../googletest/googlemock/include")

# Add the directory to the list of include directories
include_directories(${GMOCK_INCLUDE_DIR})

# Set the directory containing the gtest library file
set(GTEST_LIB_DIR "${CMAKE_BINARY_DIR}/../../../googletest/build/lib")

# Add the gtest library to the test executable
target_link_libraries(drivertest ${GTEST_LIB_DIR}/libgtest.a ${GTEST_LIB_DIR}/libgtest_main.a ${GTEST_LIB_DIR}/libgmock.a ${GTEST_LIB_DIR}/libgmock_main.a)

set_target_properties(drivertest PROPERTIES OUTPUT_NAME "drivertest.out")

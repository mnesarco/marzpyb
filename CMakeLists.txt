cmake_minimum_required(VERSION 3.10)
project(marzpyb)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for language servers
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set policy if available
if(POLICY CMP0148)
    cmake_policy(SET CMP0148 NEW)
endif()

# Find Python (will use system Python or whatever is in PATH)
find_package(Python COMPONENTS Interpreter Development REQUIRED)

# Add GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# PyCXX header-only library
add_library(PyCXX INTERFACE)
target_include_directories(PyCXX INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PyCXX
)

# Enable testing
enable_testing()

add_executable(marzpyb main.cpp)
target_include_directories(marzpyb PRIVATE ${Python_INCLUDE_DIRS})
target_link_libraries(marzpyb PRIVATE ${Python_LIBRARIES} PyCXX)

# Compile time benchmark executable (no gtest overhead)
# Multiple source files to stress test compilation time
add_executable(compile_benchmark
    benchmark/compile_benchmark.cpp
    benchmark/compile_benchmark2.cpp
    benchmark/compile_benchmark3.cpp
    benchmark/compile_benchmark4.cpp
    benchmark/compile_benchmark5.cpp
    benchmark/compile_benchmark6.cpp
    benchmark/compile_benchmark7.cpp
    benchmark/compile_benchmark8.cpp
    benchmark/compile_benchmark9.cpp
    benchmark/compile_benchmark10.cpp
)
target_include_directories(compile_benchmark PRIVATE ${Python_INCLUDE_DIRS})
target_link_libraries(compile_benchmark PRIVATE ${Python_LIBRARIES} PyCXX)

# Create test executable
add_executable(test_pyarguments tests/test_pyarguments.cpp)
target_include_directories(test_pyarguments PRIVATE ${Python_INCLUDE_DIRS})
target_link_libraries(test_pyarguments PRIVATE
    ${Python_LIBRARIES}
    gtest_main
    gtest
    PyCXX
)

# Create PyCXX test executable
add_executable(test_pycxx_arguments
    3rdParty/PyCXX/CXX/Python3/cxxsupport.cxx
    3rdParty/PyCXX/CXX/Python3/cxx_exceptions.cxx
    3rdParty/PyCXX/CXX/Python3/cxx_debug_stubs.cxx
    3rdParty/PyCXX/CXX/IndirectPythonInterface.cxx
    tests/test_pycxx_arguments.cpp
)
target_include_directories(test_pycxx_arguments PRIVATE
    ${Python_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PyCXX/CXX/Python3
)
target_link_libraries(test_pycxx_arguments PRIVATE
    ${Python_LIBRARIES}
    gtest_main
    gtest
    PyCXX
)

# Add test to CTest
add_test(NAME PyArgumentsTests
    COMMAND test_pyarguments
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_test(NAME PyCxxArgumentsTests
    COMMAND test_pycxx_arguments
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

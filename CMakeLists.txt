cmake_minimum_required(VERSION 3.10)
project(marzpyb)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
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
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

add_executable(marzpyb main.cpp)
target_include_directories(marzpyb PRIVATE ${Python_INCLUDE_DIRS})
target_link_libraries(marzpyb PRIVATE ${Python_LIBRARIES})

# Create test executable
add_executable(test_pyarguments tests/test_pyarguments.cpp)
target_include_directories(test_pyarguments PRIVATE ${Python_INCLUDE_DIRS})
target_link_libraries(test_pyarguments PRIVATE
    ${Python_LIBRARIES}
    gtest_main
    gtest
)

# Add test to CTest
add_test(NAME PyArgumentsTests
    COMMAND test_pyarguments
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

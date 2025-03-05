# Specify the minimum required version of CMake for this project. 
cmake_minimum_required(VERSION 3.14)

project(AUT_AP_HW4)

# Set the C++ standard to C++23 and make it a required standard.
# If the compiler does not support C++23, CMake will produce an error.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(GTest REQUIRED)

# 添加 GoogleTest 的子目录
add_subdirectory(./googletest)

include_directories(include/)

add_executable(main
        src/main.cpp
        src/unit_test.cpp
)

# Set compiler flags for C++.
# -Wall, -Wextra, -Werror, and -Wpedantic are used for stricter warnings and error handling.
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wpedantic")


# Set compiler flags specific to the Release build type.
# -O3 enables high-level optimizations.
set(CMAKE_CXX_FLAGS_RELEASE "-O0")

target_link_libraries(main
#        GTest::GTest
#        GTest::Main
         gtest_main
)
# Minimum required CMake version
cmake_minimum_required(VERSION 3.16)

# Project information
project(ThreadSafeLRUCache
    VERSION 1.0
    LANGUAGES CXX
)

# C++ standard configuration
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # new


# add_exectuable should be below the C++ standard configuration
add_executable(lru_cache 
        include/LRUCache.h
        src/LRUCache.cpp 
        src/main.cpp
)

target_include_directories(lru_cache PRIVATE include)  # new

# Enable testing
enable_testing()

# GoogleTest executable
add_executable(lru_cache_tests
    tests/test_lru_cache.cpp
    src/LRUCache.cpp
)

target_include_directories(lru_cache_tests PRIVATE include)

# Link GoogleTest
target_link_libraries(lru_cache_tests
    PRIVATE
    GTest::gtest_main
)

# Enable automatic test discovery
include(GoogleTest)
gtest_discover_tests(lru_cache_tests)


find_package(GTest REQUIRED)

# Instruction	                    Purpose
# cmake_minimum_required()	        Minimum CMake version
# project()	                        Defines project name and version
# LANGUAGES                         CXX	Enables C++ language
# CMAKE_CXX_STANDARD 17	            Requests C++17
# CMAKE_CXX_STANDARD_REQUIRED ON	Prevents falling back to an older standard
# CMAKE_CXX_EXTENSIONS OFF	        Requests standard C++17 rather than GNU extensions
# target_include_directories        include headers 
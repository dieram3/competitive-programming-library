cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project("CP-utils" CXX C) # C is needed for FindThreads

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

find_package(Threads) # Needed for gtest

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories("include")
add_subdirectory("utils")

enable_testing()
add_subdirectory("test")

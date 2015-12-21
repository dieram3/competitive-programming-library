cmake_minimum_required(VERSION 3.1.3 FATAL_ERROR)
project("CP-utils" CXX C) # C is needed for FindThreads

# ---------------------------------------
# Initial config
# ---------------------------------------

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

find_package(Threads) # Needed for gtest
find_package(Doxygen)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# For YouCompleteMe Vim's plugin
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# ---------------------------------------
# Project options
# ---------------------------------------

option(ENABLE_CLANG_MODULES "Enables clang support for modules" OFF)

# ---------------------------------------
# Handle options
# ---------------------------------------

if(ENABLE_CLANG_MODULES)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-fmodules -fcxx-modules)
endif()
endif()

# ---------------------------------------
# Integrate utilities
# ---------------------------------------

add_subdirectory("utils")

# ---------------------------------------
# Add the CP-utils header-only library
# ---------------------------------------

add_library(CP_utils INTERFACE)
target_include_directories(CP_utils INTERFACE "include")

# The following list does not contain all features used in CP-utils
# but contain the main ones. It will induce cmake to compile with a C++ standard >= 11
target_compile_features(CP_utils INTERFACE
  cxx_auto_type
  cxx_lambdas
  cxx_nullptr
  )

# ---------------------------------------
# Add the unit tests
# ---------------------------------------

enable_testing()
add_subdirectory("test")
include(CTest)

# ---------------------------------------
# Add documentation
# ---------------------------------------

add_subdirectory("doc")

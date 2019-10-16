# Competitive Programming Library

[![Build Status](https://travis-ci.com/dieram3/competitive-programming-library.svg?branch=master)](https://travis-ci.com/dieram3/competitive-programming-library)
[![Coverage Status](https://coveralls.io/repos/github/dieram3/competitive-programming-library/badge.svg?branch=master)](https://coveralls.io/github/dieram3/competitive-programming-library?branch=master)

CPL is a header-only library that contains implementations of typical algorithms and data structures used in programming contests. It is intended to be used mainly in online contests, where you can copy and paste code easily. However, by the fact that in some programming contests you have to type everything from scratch, the source code is maintained short (but not so short because generality and ease of use is another important target of the library).

## Modules

Currently CPL has the following modules:

- Bits
- Combinatorics
- Data Structure
- Geometry
- Graph
- Math
- Number Theory
- Search
- Sorting
- Strings
- Utility

## Installation

If you want to download CPL for running the tests and/or compiling the examples, do the following:

First, you need [cmake](http://www.cmake.org) installed on your system.

Then, run the following commands on the root directory of the project:

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make && make test
```

The generated executable files will be put on a sub-directory of the build folder called *bin*.

## Dependencies
CPL requires full C++11 support:

- A fully compliant compiler.
- And a fully implemented standard library.

## Simple usage example

The following code snippet shows how to generate a suffix array and using it.

```c++
#include <iostream>
#include <string>
#include <cpl/strings/suffix_array.hpp>

int main() {
  // Given a text.
  std::string text = "mississippi";

  // Generate its suffix array.
  auto sa = cpl::make_suffix_array(text);

  // And use it to show the suffixes sorted lexicographically.
  for (size_t suffix : sa)
    std::cout << text.substr(suffix) << '\n';
}

```

## How to contribute

You can submit a pull request to improve the library. The code must be well documented, easily legible and independent of global variables.

## Read the docs

See <http://dieram3.github.io/competitive-programming-library>

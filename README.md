# Competitive Programming Utilities

[![Build Status](https://travis-ci.org/dieram3/CP-utils.svg?branch=master)](https://travis-ci.org/dieram3/CP-utils)
[![Coverage Status](https://coveralls.io/repos/dieram3/CP-utils/badge.svg?branch=master&service=github)](https://coveralls.io/github/dieram3/CP-utils?branch=master)

CP-utils is a header-only library that contains implementations of typical algorithms and data structures used in programming contests. It is intended to be used mainly in online contests, where you can copy and paste code easily. However, by the fact that in some programming contests you have to type everything from scratch, the source code is maintained short (but not so short because generality and ease of use is another important target of the library).

## Modules

Currently CP-utils has the following modules:
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

If you want to download CP-utils for running the tests and/or compiling the examples, do the following:

First, you need [cmake](http://www.cmake.org) installed on your system.

Then, run the following commands on the root directory of the project:
```Shell
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make && make test
```

The generated executable files will be put on a sub-directory of the build folder called *bin*.

## Compatibility

CP-utils needs a compliant C++11 compiler.

It has been tested with the following compilers:
- clang 3.5 on OS X
- gcc 4.9 on Linux

## Dependencies

CP-utils only depends on the Standard C++ Library.

## Simple usage example

The following code snippet shows how to generate a suffix array and using it.

```C++
#include <iostream>
#include <string>
#include <djp/strings/suffix_array.hpp>  // for djp::make_suffix_array

int main() {
  // Have a text.
  std::string text = "mississippi";

  // Generate the suffix array.
  auto sa = djp::make_suffix_array(text);

  // Use it to show the suffixes sorted lexicographically.
  for (size_t suffix : sa) 
    std::cout << text.substr(suffix) << '\n';
}

```

## How to contribute

You can submit a pull request to improve the library. The code must be well documented, easily legible and independent of global variables. Use Issues for feedback. Thanks!

## Read the docs

See http://dieram3.github.io/CP-utils


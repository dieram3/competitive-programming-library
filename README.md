# Competitive Programming Utilities

## Introduction
CP-utils is a library that contains implementations of typical algorithms and data structures used in programming contests. It is intended to be used mainly in online contests, where you can copy and paste code easily. However, by the fact that in some programming contests you have to type everything from scratch, the source code is maintained short (but not so short because generality and ease of use is another important target of the library).

## Modules

Currently CP-utils has support for the following areas:
- Combinatorics
- Data structures
- Dynamic Programming
- Geometry
- Graph Theory
- Math
- Search
- Sorting
- Strings

It also has a module called *utility* which is used by the other modules.

## Installation

If you want to download CP-utils for running the tests and/or compiling the examples, do the following:

First, you need [biicode](https://www.biicode.com) installed on your system 
including its [tools for C/C++](http://docs.biicode.com/c++/installation.html#install-c-c-tools).

Then, run the following commands on the root directory of the project:
```Shell
bii init -L
bii find
bii build
```

The generated executable files will be put on a sub-directory of the root called *bin*.

## Compatibility

CP-utils needs a compliant C++11 compiler.

It has been tested with the following compilers:
- clang 3.5 on MacOS X 64-bit

## Dependencies

CP-utils only depends on the Standard C++ Library.

## Simple usage example

The following code snippet shows how to generate a suffix array and using it.

```C++
#include <iostream>
#include <string>
#include <djp/strings/suffix_array.hpp>  // for djp::make_suffix_array

int main() {
  // We have a text
  std::string text = "mississippi";

  // We generate the suffix array
  auto sa = djp::make_suffix_array(text);

  // And then, we use it to show the suffixes sorted lexicographically.
  for (size_t suffix : sa) std::cout << text.substr(suffix) << '\n';
}

```

## How to contribute

You can submit a pull request to improve the library. The code must be well documented, easily legible and independent of global variables. Use Issues for feedback. Thanks!


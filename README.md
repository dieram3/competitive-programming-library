# Competitive Programming Utilities

## Introduction
CP-utils is a recopilation of typical algorithms and data structures used in competitive programming contests. It is intended to be used mainly in online contests, so you can copy and paste the source code. However, by the fact that in some contests you have to type everything, the source code is maintained short (but not so short, because generallity and ease of use is another target).

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

Also has a utility module which is used by the other modules.

## Installation

If you want download this repo, run the test and/or compile the examples, do the following:

First you need [biicode](https://www.biicode.com) installed on your system with [C++ tools](http://docs.biicode.com/c++/installation.html#install-c-c-tools) installed.

Then run the following commands on the root of the project:
```
bii init -L
bii find
bii build
```

This will generate a bin folder which will contain all the generated executables, including the test.

## Compatibility

This project need a compliant C++11 compiler and library.

At this moment, this has been tested with the following compilers:
- clang 3.5 on OS X 64 bit

## A simple example

The following code snippet shows a simple use of CP-utils. This uses a suffix array to show the suffixes of 'mississippi' sorted lexicographically.

```C++
#include <iostream>
#include <string>
#include <djp/strings/suffix_array.hpp>

int main() {
  // We have a text
  std::string text = "mississippi";
  std::cout << "The suffixes of '" << text
            << "' sorted lexicographically are:\n";
  // We compute the suffix array
  auto sa = djp::make_suffix_array(text);

  // And then, we show the suffixes sorted lexicographically.
  for (size_t suffix : sa) std::cout << text.substr(suffix) << '\n';
}
```

## How to contribute

You can use issues and pull requests for add, fix or improve source code. The conditions is, maintain the code legible and global variable independent.

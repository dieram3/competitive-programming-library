// If you have access to libstdc++
// #include <bits/stdc++.h> in order to include
// every header from the standard C++ library

// Take into account language extensions of your compiler

// Documentation for Clang:
// http://clang.llvm.org/docs/LanguageExtensions.html

// Documentation for GCC:
// https://gcc.gnu.org/onlinedocs/gcc/C-Extensions.html

#include <cassert>

int main() {
  unsigned long seven = 7;
  assert(__builtin_popcountl(seven) == 3);
}

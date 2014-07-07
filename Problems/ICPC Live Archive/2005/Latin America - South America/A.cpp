//
//  main.cpp
//  ACM-2005
//
//  Created by Patricio Beckmann on 05-07-14.
//  Copyright (c) 2014 Patricio Beckmann. All rights reserved.
//

#include <iostream>
#include <vector>

namespace acm {

template <typename T>
T input_value(std::istream& is = std::cin) {
  T value;
  is >> value;
  return value;
}

template <typename Func>
void repeat(std::size_t n, Func&& f) {
  while (n--)
    f();
}
}

using post_t = bool;
using fence_t = std::vector<post_t>;

std::size_t check_wooden_posts(std::size_t N) {
  std::size_t count = 0;
  std::size_t zeros = 0;
  bool needed = false;
  bool alldestroyed = true;

  post_t first = acm::input_value<post_t>();

  if (!first) {
    ++zeros;
  } else {
    alldestroyed = false;
  }

  acm::repeat(N - 1, [&] {

    post_t post = acm::input_value<post_t>();

    if (!alldestroyed) {
      if (!post && needed) {
        needed = false;
        ++count;
      } else {
        if (!post) {
          needed = true;
        } else {
          needed = false;
        }
      }
    } else {
      if (!post) {
        ++zeros;
      } else {
        alldestroyed = false;
      }
    }
  });

  if (alldestroyed) {
    return (N + 1) / 2;
  }
  
  count += (needed ? zeros + 1 : zeros) / 2;

  return count;
}

int main() {
  std::size_t N;

  while (std::cin >> N) {
    if (N == 0)
      return 0;
    std::cout << check_wooden_posts(N) << '\n';
  }
}

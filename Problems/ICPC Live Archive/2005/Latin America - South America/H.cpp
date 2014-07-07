//
//  H.cpp
//  Programming speed
//
//  Created by Patricio Beckmann on 05-07-14.
//  Copyright (c) 2014 Patricio Beckmann. All rights reserved.
//

#include <iostream>
#include <algorithm>
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

using data_t = std::size_t;
using trans_t = std::vector<data_t>;

void fill_transformation(trans_t& transformations) {
  for (auto& t : transformations) {
    t = acm::input_value<data_t>();
  }
}

bool find_repeted_num(const trans_t& trans) {
  std::size_t buffer = 0;

  for (const auto t : trans) {
    if (buffer & (1 << t))
      return true;
    buffer |= (1 << t);
  }
  return false;
}

void generate_pnet(trans_t& trans, trans_t& strokes) {
  for (std::size_t i = 0; i < trans.size(); ++i) {
    for (std::size_t j = trans.size() - 1; j > i; --j) {
      if (trans[j] < trans[j - 1]) {
        std::swap(trans[j], trans[j - 1]);
        strokes.push_back(j);
      }
    }
  }
}

void imprimir(const trans_t& trans) {
  std::cout << trans.size();
  for (const auto t : trans) {
    std::cout << ' ' << t;
  }
  std::cout << '\n';
}

int main() {
  std::size_t order;

  while (std::cin >> order) {
    if (order == 0)
      return 0;

    trans_t transformation(order);
    trans_t pnet;

    fill_transformation(transformation);
    if (find_repeted_num(transformation)) {
      std::cout << "No solution\n";
      continue;
    }
    generate_pnet(transformation, pnet);
    imprimir(pnet);
  }
}
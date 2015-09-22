//          Copyright Patricio Beckmann August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines the function \c make_dc3_suffix_array.

#ifndef DJP_STRINGS_DC3_SUFFIX_ARRAY_HPP
#define DJP_STRINGS_DC3_SUFFIX_ARRAY_HPP

#include <djp/sorting/counting_sort.hpp>
#include <algorithm> // for std::transform, std::equal
#include <iterator>  // for std::begin, std::end
#include <vector>    // for std::vector
#include <string>    // for std::string
#include <cstddef>   // for std::size_t

namespace djp {

template <typename Range>
void print_v(const Range &range) {
  for (auto &&value : range)
    std::cout << value << ' ';
  std::cout << '\n';
}

class dc3_helper {
  using vec_t = std::vector<size_t>;

  void recurse(const vec_t &rk, vec_t &sa, size_t n, size_t K) {
    const size_t n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3;
    const size_t n02 = n0 + n2;
    vec_t rk12(n02 + 3), sa12(n02 + 3);
    vec_t rk0(n0), sa0(n0);

    for (size_t i = 0, j = 0; i < n + (n0 - n1); ++i)
      if (i % 3 != 0)
        rk12.at(j++) = i;

    auto radix_pass = [&](const vec_t &a, vec_t &b, size_t len, size_t delta) {
      auto key = [&](size_t elem) { return rk[elem + delta]; };
      counting_sort_copy(a.begin(), a.begin() + len, b.begin(), K, key);
    };

    // Sort sample suffixes
    radix_pass(rk12, sa12, n02, 2);
    radix_pass(sa12, rk12, n02, 1);
    radix_pass(rk12, sa12, n02, 0);

    size_t name = 1;
    for (size_t i = 0; i < n02; ++i) {
      auto it1 = rk.begin() + sa12[i];
      auto it0 = (i == 0) ? it1 : rk.begin() + sa12[i - 1];
      if (!std::equal(it1, it1 + 3, it0))
        ++name;
      size_t offset = sa12[i] % 3 == 1 ? 0 : n0; // left half : right half
      rk12.at(sa12[i] / 3 + offset) = name;
    }

    if (name != n02) {
      recurse(rk12, sa12, n02, name + 1);
      // store unique names in rk12 using the suffix array
      for (size_t i = 0; i != n02; ++i) {
        rk12.at(sa12[i]) = i + 1;
      }
    } else // generate the suffix array of rk12 directly
      for (size_t i = 0; i != n02; ++i)
        sa12.at(rk12[i] - 1) = i;

    // Sort nonsample suffixes

    // stably sort the mod 0 suffixes from sa12 by their first character
    for (size_t i = 0, j = 0; i < n02; ++i)
      if (sa12[i] < n0)
        rk0[j++] = 3 * sa12[i];
    radix_pass(rk0, sa0, n0, 0);

    auto get_pos = [&](size_t pos) {
      return sa12.at(pos) < n0 ? (sa12.at(pos) * 3 + 1)
                               : ((sa12.at(pos) - n0) * 3 + 2);
    };

    // merge sorted SA0 suffixes and sorted SA12 suffixes
    size_t t = n0 - n1;
    size_t p = 0;

    auto leq_2 = [&](size_t a1, size_t a2, size_t b1, size_t b2) {
      return (a1 < b1 || (a1 == b1 && a2 <= b2));
    };

    auto leq_3 =
        [&](size_t a1, size_t a2, size_t a3, size_t b1, size_t b2, size_t b3) {
          return (a1 < b1 || (a1 == b1 && leq_2(a2, a3, b2, b3)));
        };

    for (size_t k = 0; k < n; ++k) {
      size_t i = get_pos(t); // pos of current offset 12 suffix
      size_t j = sa0[p];     // pos of current offset 0 suffix
      if (sa12.at(t) < n0
              ? // different compares for mod 1 and mod 2 suffixes
              leq_2(rk[i], rk12[sa12[t] + n0], rk[j], rk12[j / 3])
              : leq_3(rk[i], rk[i + 1], rk12[sa12[t] - n0 + 1], rk[j],
                      rk[j + 1],
                      rk12[j / 3 + n0])) { // suffix from SA12 is smaller
        sa[k] = i;
        t++;
        if (t == n02) // done --- only SA0 suffixes left
          for (k++; p < n0; p++, k++)
            sa[k] = sa0[p];
      } else { // suffix from SA0 is smaller
        sa[k] = j;
        p++;
        if (p == n0) // done --- only SA12 suffixes left
          for (k++; t < n02; t++, k++)
            sa[k] = get_pos(t);
      }
    }
  }

public:
  std::vector<size_t> calc(const std::string &str) {
    if (str.empty())
      return {};
    if (str.size() == 1)
      return {0};
    std::vector<size_t> sa(str.size());
    std::vector<size_t> rk(str.size() + 3);
    std::transform(str.begin(), str.end(), rk.begin(),
                   [](char elem) { return size_t(1) + elem; });
    recurse(rk, sa, str.size(), 257);
    return sa;
  }
};

/// \brief Computes the suffix array of a given string using the dc3
/// algorithm.
///
/// \param str The target string.
///
/// \returns A vector containing the all the suffixes of \p str sorted
/// lexicographically i.e the suffix array.
///
/// \par Complexity
/// <tt>O(n)</tt> time, <tt>O(n)</tt> space.
///
inline std::vector<size_t> dc3_suffix_array(const std::string &str) {
  dc3_helper helper;

  return helper.calc(str);
}

} // end namespace djp

#endif // Header guard

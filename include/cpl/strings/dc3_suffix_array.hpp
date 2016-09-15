//          Copyright Patricio Beckmann 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/// \file
/// \brief Defines the function <tt>dc3_suffix_array</tt>.

#ifndef CPL_STRINGS_DC3_SUFFIX_ARRAY_HPP
#define CPL_STRINGS_DC3_SUFFIX_ARRAY_HPP

#include <cpl/sorting/counting_sort.hpp> // counting_sort_copy
#include <algorithm>                     // transform, equal
#include <cstddef>                       // size_t
#include <functional>                    // function
#include <string>                        // string
#include <tuple>                         // tie
#include <vector>                        // vector

namespace cpl {

/// \brief Constructs the suffix array of a string using the DC3 algorithm.
///
/// \param str The input string.
///
/// \returns The suffix array of <tt>str</tt>, that is, a \c std::vector
/// which contains the indices of all the suffixes of \c str in lexicographical
/// order.
///
/// \par Complexity
/// Both the time complexity and the space complexity are linear in
/// <tt>str.size()</tt>.
///
inline std::vector<size_t> dc3_suffix_array(const std::string& str) {
  using vec_t = std::vector<size_t>;

  if (str.size() < 2)
    return vec_t(str.size());

  std::function<void(const vec_t&, vec_t&, size_t, size_t)> recurse;
  recurse = [&](const vec_t& rk, vec_t& sa, size_t n, size_t num_keys) {
    const size_t n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3;
    const size_t n02 = n0 + n2;
    vec_t rk12(n02 + 3), sa12(n02 + 3);
    vec_t rk0(n0), sa0(n0);

    for (size_t i = 0, j = 0; i < n + (n0 - n1); ++i)
      if (i % 3 != 0)
        rk12[j++] = i;

    auto radix_pass = [&](const vec_t& a, vec_t& b, size_t len, size_t delta) {
      auto key = [&](size_t elem) { return rk[elem + delta]; };
      counting_sort_copy(a.begin(), a.begin() + len, b.begin(), num_keys, key);
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
      rk12[sa12[i] / 3 + offset] = name;
    }

    if (name != n02) {
      recurse(rk12, sa12, n02, name + 1);
      // store unique names in rk12 using the suffix array
      for (size_t i = 0; i != n02; ++i)
        rk12[sa12[i]] = i + 1;
    } else // generate the suffix array of rk12 directly
      for (size_t i = 0; i != n02; ++i)
        sa12[rk12[i] - 1] = i;

    // Sort nonsample suffixes

    // stably sort the mod 0 suffixes from sa12 by their first character
    for (size_t i = 0, j = 0; i < n02; ++i)
      if (sa12[i] < n0)
        rk0[j++] = 3 * sa12[i];
    radix_pass(rk0, sa0, n0, 0);

    // merge sorted SA0 suffixes and sorted SA12 suffixes
    size_t t = n0 - n1, p = 0, k = 0;

    auto suffix12_smaller = [&](size_t i, size_t j) {
      using std::tie;
      if (sa12[t] < n0)
        return tie(rk[i], rk12[sa12[t] + n0]) < tie(rk[j], rk12[j / 3]);
      return tie(rk[i], rk[i + 1], rk12[sa12[t] - n0 + 1]) <
             tie(rk[j], rk[j + 1], rk12[j / 3 + n0]);
    };
    auto get_pos = [&](size_t pos) {
      return sa12[pos] < n0 ? (sa12[pos] * 3 + 1) : ((sa12[pos] - n0) * 3 + 2);
    };

    while (p < n0 && t < n02) {
      size_t i = get_pos(t); // pos of current offset 12 suffix
      size_t j = sa0[p];     // pos of current offset 0 suffix
      if (suffix12_smaller(i, j)) {
        sa[k++] = i;
        ++t;
      } else {
        sa[k++] = j;
        ++p;
      }
    }
    while (p < n0)
      sa[k++] = sa0[p++];
    while (t < n02)
      sa[k++] = get_pos(t++);
  };

  vec_t sa(str.size()), rk(str.size() + 3);
  auto char_key = [](unsigned char c) { return c + 1; };
  std::transform(str.begin(), str.end(), rk.begin(), char_key);
  // TODO(Patricio) should be (max_key + 1) instead of 257.
  recurse(rk, sa, str.size(), 257);
  return sa;
}

} // end namespace cpl

#endif // Header guard

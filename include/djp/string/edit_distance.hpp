#ifndef DJP_STRING_EDIT_DISTANCE_HPP
#define DJP_STRING_EDIT_DISTANCE_HPP

#include <vector>
#include <algorithm>

/// \todo document: levenshtein_distance
/// \todo analyze: damerau_levenshtein_distance
/// \todo design: longest_common_subsequence
/// \todo design: hamming_distance

namespace djp {

template <class String1, class String2>
std::size_t levenshtein_distance(const String1& str1, const String2& str2) {
  using std::size_t;
  constexpr size_t w_del = 1;  // deletion cost
  constexpr size_t w_ins = 1;  // insertion cost
  constexpr size_t w_sub = 1;  // substitution cost

  const size_t rows = str1.size() + 1;
  const size_t cols = str2.size() + 1;
  std::vector<size_t> distance(rows * cols);
  auto d = [&](size_t i, size_t j) -> size_t & {
    return distance[i * cols + j];
  };

  d(0, 0) = 0;
  for (size_t i = 1; i != rows; ++i) d(i, 0) = d(i - 1, 0) + w_del;
  for (size_t j = 1; j != cols; ++j) d(0, j) = d(0, j - 1) + w_ins;

  for (size_t i = 1; i != rows; ++i) {
    for (size_t j = 1; j != cols; ++j) {
      if (str1[i - 1] == str2[j - 1])
        d(i, j) = d(i - 1, j - 1);
      else {
        auto del_dist = d(i - 1, j) + w_del;
        auto ins_dist = d(i, j - 1) + w_ins;
        auto sub_dist = d(i - 1, j - 1) + w_sub;
        d(i, j) = std::min({del_dist, ins_dist, sub_dist});
      }
    }
  }

  return distance.back();
}

}  // namespace djp

#endif  // Header guard

#ifndef DJP_STRING_EDIT_DISTANCE_HPP
#define DJP_STRING_EDIT_DISTANCE_HPP

#include <algorithm> // for std::min, std::swap
#include <memory>    // for std::unique_ptr
#include <numeric>   // for std::iota
#include <cstddef>   // for std::size_t

/// \todo analyze: damerau_levenshtein_distance
/// \todo design: longest_common_subsequence

namespace djp {

/// Computes the edit distance between the ranges [first1, last1) and
/// [first2, last2). The set of operations taken into account are
/// deletion, insertion and substitution. Each operation costs 1.
/// Complexity: m*n comparisons of equality
/// where m == last1 - first1, n == last2 - first2
template <class RandomIt1, class RandomIt2>
std::size_t levenshtein_distance(RandomIt1 first1, RandomIt1 last1,
                                 RandomIt2 first2, RandomIt2 last2) {
  using std::size_t;
  using std::min;
  auto min3 = [](size_t x, size_t y, size_t z) { return min(x, min(y, z)); };

  const size_t rows = (last1 - first1) + 1;
  const size_t cols = (last2 - first2) + 1;
  std::unique_ptr<size_t[]> storage(new size_t[2 * cols]);
  auto *prev = storage.get(); // previous row
  auto *curr = prev + cols;   // current row

  std::iota(prev, prev + cols, size_t{0});

  for (size_t i = 1; i != rows; ++i) {
    curr[0] = i;
    for (size_t j = 1; j != cols; ++j) {
      if (first1[i - 1] == first2[j - 1])
        curr[j] = prev[j - 1];
      else
        curr[j] = min3(prev[j], curr[j - 1], prev[j - 1]) + 1;
    }
    std::swap(curr, prev);
  }
  return prev[cols - 1];
}

} // end namespace djp

#endif // Header guard

#ifndef DJP_STRING_EDIT_DISTANCE_HPP
#define DJP_STRING_EDIT_DISTANCE_HPP

#include <djp/utility/matrix.hpp>
#include <algorithm>
#include <iterator>

/// \todo analyze: damerau_levenshtein_distance
/// \todo design: longest_common_subsequence
/// \todo design: hamming_distance

namespace djp {

// Computes the edit distance between the ranges [first1, last1) and
// [first2, last2). The set of operations taken into account are
// deletion, insertion and substitution. Each operation costs 1 by default.
// Complexity: m*n comparisons of equality
// where m == last1 - first1, n == last2 - first2
template <class RandomIt1, class RandomIt2>
std::size_t levenshtein_distance(RandomIt1 first1, RandomIt1 last1,
                                 RandomIt2 first2, RandomIt2 last2) {
  using std::size_t;
  constexpr size_t w_del{1}, w_ins{1}, w_sub{1};

  const size_t rows = 1 + std::distance(first1, last1);
  const size_t cols = 1 + std::distance(first2, last2);
  matrix<size_t> dp(rows, cols);

  dp[0][0] = 0;
  for (size_t i = 1; i != rows; ++i) dp[i][0] = dp[i - 1][0] + w_del;
  for (size_t j = 1; j != cols; ++j) dp[0][j] = dp[0][j - 1] + w_ins;

  for (size_t i = 1; i != rows; ++i)
    for (size_t j = 1; j != cols; ++j) {
      if (first1[i - 1] == first2[j - 1])
        dp[i][j] = dp[i - 1][j - 1];
      else
        dp[i][j] = std::min({dp[i - 1][j] + w_del,        // deletion
                             dp[i][j - 1] + w_ins,        // insertion
                             dp[i - 1][j - 1] + w_sub});  // substitution
    }

  return dp[rows - 1][cols - 1];
}

}  // namespace djp

#endif  // Header guard

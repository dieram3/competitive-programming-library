//          Copyright Diego Ramírez May 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_NUMBER_THEORY_PRIMES_HPP
#define DJP_NUMBER_THEORY_PRIMES_HPP

#include <djp/number_theory/modular.hpp>
#include <djp/number_theory/basics.hpp>

#include <algorithm> // for std::fill_n, std::count, std::all_of
#include <array>     // for std::array
#include <iterator>  // for std::begin, std::end
#include <stdexcept> // for std::domain_error
#include <vector>    // for std::vector

#include <cassert> // for assert macro
#include <cstddef> // for std::size_t
#include <cstdint> // for std::uint64_t, UINT32_MAX

namespace djp {

/// \brief Finds all primes number less than a given number.
/// \param limit The upper bound limit.
/// \returns A sorted \c vector containing all primes numbers less than \p
/// limit.
/// \pre \p limit shall be a non-negative number.
/// \par Complexity
/// O(N * log log N) where N == \p limit.
template <class T = std::uint32_t>
std::vector<T> sieve_of_eratosthenes(size_t limit) {
  if (limit < 2)
    return {};

  std::vector<bool> is_prime(limit, true);
  is_prime[0] = is_prime[1] = false;

  for (size_t i = 2; i * i < limit; ++i) {
    if (!is_prime[i])
      continue;
    for (size_t j = i * i; j < limit; j += i)
      is_prime[j] = false;
  }

  const size_t num_primes = std::count(begin(is_prime), end(is_prime), true);
  std::vector<T> primes;
  primes.reserve(num_primes);
  for (size_t i = 2; i < limit; ++i)
    if (is_prime[i])
      primes.push_back(static_cast<T>(i));

  assert(num_primes == primes.size());
  return primes;
}

/// \brief Checks if \p n is a prime number.
/// \param n The number to be tested.
/// \note mr stands for Miller-Rabin
/// \pre T shall be an unsigned type with at most 32 bits.
template <class T>
bool is_prime_mr(const T n) {
  static_assert(std::is_unsigned<T>::value, "is_prime_mr: T must be unsigned");
  static_assert(sizeof(T) <= sizeof(std::uint32_t),
                "is_prime_mr: T must have at most 32 bits");
  if (n == 2)
    return true;
  if (n < 2 || n % 2 == 0)
    return false;

  // write n − 1 as 2^s*d with d odd by factoring powers of 2 from n − 1
  const size_t s = __builtin_ctzll(n - 1);
  const T d = (n - 1) >> s;

  auto witness = [n, s, d](const T a) {
    assert(a >= 2 && a < n);
    T x = mod_pow(a, d, n);
    if (x == 1 || x == n - 1)
      return true;

    for (size_t rep = 0; rep < s - 1; ++rep) {
      x = mod_mul(x, x, n);
      if (x == 1)
        return false;
      if (x == n - 1)
        return true;
    }
    return false;
  };

  auto test_with = [witness](std::initializer_list<uint32_t> ilist) {
    return std::all_of(begin(ilist), end(ilist), witness);
  };

  if (n < 2047)
    return test_with({2});
  if (n < 9080191)
    return test_with({31, 73});

  return test_with({2, 7, 61}); // this works if n < 4'759'123'141

  //  if (n < 1'122'004'669'633)
  //    return test_with({2, 13, 23, 1662803});
  //  if (n < 2'152'302'898'747)
  //    return test_with({2, 3, 5, 7, 11});
  //  if (n < 3'474'749'660'383)
  //    return test_with({2, 3, 5, 7, 11, 13});
  //  if (n < 341'550'071'728'321)
  //    return test_with({2, 3, 5, 7, 11, 13, 17});
  //  if (n < 3'825'123'056'546'413'051)
  //    return test_with({2, 3, 5, 7, 11, 13, 17, 19, 23});
}

} // namespace djp

#endif // Header guard

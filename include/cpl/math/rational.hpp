//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_MATH_RATIONAL_HPP
#define CPL_MATH_RATIONAL_HPP

#include <cpl/number_theory/euclid.hpp> // gcd
#include <algorithm>                    // for_each
#include <cassert>                      // assert
#include <vector>                       // vector

namespace cpl {

/// \brief Integer like class to represent rational numbers.
///
/// Objects of this class represent a rational number by storing the numerator
/// and the denominator as separated integers of type <tt>T</tt>. Rational
/// numbers are kept in its normalized form, that is, the numerator and
/// the denominator have no common factor other than 1 and the denominator is
/// positive.
///
template <typename T>
class rational {
  T num, den;
  struct no_reduction_tag {};

private:
  void set_reduced_values(const T& a, const T& b) {
    // assert(gcd(a, b) == 1);
    assert(b != 0 && "Denominator cannot be zero");
    b < 0 ? (num = -a, den = -b) : (num = a, den = b);
  }
  rational(const T& a, const T& b, no_reduction_tag /*unused*/) {
    set_reduced_values(a, b);
  }

public:
  /// \brief Constructs a rational number with the given integer.
  ///
  /// Uses the given integer as the numerator and \c 1 as the denominator.
  ///
  /// \param value The input integer.
  ///
  explicit rational(const T& value = 0) : num{value}, den{1} {}

  /// \brief Constructs a rational number with the given numerator and
  /// denominator.
  ///
  /// \param a The numerator.
  /// \param b The denominator.
  ///
  /// \pre <tt>b != 0</tt>.
  ///
  rational(const T& a, const T& b) {
    assert(b != 0 && "Denominator cannot be zero");
    const T g = gcd(a, b);
    set_reduced_values(a / g, b / g);
  }

  /// \brief Returns the stored numerator of <tt>*this</tt>.
  ///
  T numerator() const {
    return num;
  }

  /// \brief Returns the stored denominator of <tt>*this</tt>.
  ///
  T denominator() const {
    return den;
  }

  /// \brief Returns the reciprocal of a rational number.
  ///
  /// \param r The input rational number.
  ///
  /// \pre <tt>r != 0</tt>
  ///
  /// \returns The reciprocal of <tt>r</tt>.
  ///
  friend rational reciprocal(const rational& r) {
    assert(r.num != 0);
    return rational(r.den, r.num, no_reduction_tag{});
  }

  /// \brief Adds a rational number to another.
  ///
  /// \param lhs The left hand side operand.
  /// \param rhs The right hand side operand.
  ///
  /// \returns The result of <tt>(lhs + rhs)</tt>.
  ///
  friend rational operator+(const rational& lhs, const rational& rhs) {
    const T g = gcd(lhs.den, rhs.den);
    const T a = lhs.num * (rhs.den / g) + rhs.num * (lhs.den / g);
    const T b = lhs.den / g * rhs.den;
    return rational(a, b);
  }

  /// \brief Subtracts a rational number from another.
  ///
  /// \param lhs The left hand side operand.
  /// \param rhs The right hand side operand.
  ///
  /// \returns The result of <tt>(lhs - rhs)</tt>.
  ///
  friend rational operator-(const rational& lhs, const rational& rhs) {
    const T g = gcd(lhs.den, rhs.den);
    const T a = lhs.num * (rhs.den / g) - rhs.num * (lhs.den / g);
    const T b = lhs.den / g * rhs.den;
    return rational(a, b);
  }

  /// \brief Multiplies a rational number by another.
  ///
  /// \param lhs The left hand side operand.
  /// \param rhs The right hand side operand.
  ///
  /// \returns The result of <tt>(lhs * rhs)</tt>.
  ///
  friend rational operator*(const rational& lhs, const rational& rhs) {
    const T g1 = gcd(lhs.num, rhs.den);
    const T g2 = gcd(lhs.den, rhs.num);
    const T a = (lhs.num / g1) * (rhs.num / g2);
    const T b = (lhs.den / g2) * (rhs.den / g1);
    return rational(a, b, no_reduction_tag{});
  }

  /// \brief Divides a rational number by another.
  ///
  /// \param lhs The left hand side operand.
  /// \param rhs The right hand side operand.
  ///
  /// \pre <tt>rhs != 0</tt>
  ///
  /// \returns The result of <tt>(lhs / rhs)</tt>.
  ///
  friend rational operator/(const rational& lhs, const rational& rhs) {
    assert(rhs.num != 0);
    const T g1 = gcd(lhs.num, rhs.num);
    const T g2 = gcd(lhs.den, rhs.den);
    const T a = (lhs.num / g1) * (rhs.den / g2);
    const T b = (lhs.den / g2) * (rhs.num / g1);
    return rational(a, b, no_reduction_tag{});
  }
};

/// \brief Finds the continued fraction representation of a rational number.
///
/// Uses the Euclidean algorithm to find the coefficients of the continued
/// fraction representation of <tt>r</tt>. If \p r is a negative number, the
/// resultant coefficients will be the same of <tt>-r</tt> but with its values
/// negated.
///
/// \param r The input rational number.
/// \param[out] out_it The beginning of the destination range. The coefficients
/// <tt>a<sub>0</sub>, a<sub>1</sub>, ..., a<sub>n</sub></tt> of the continued
/// fraction representation will be copied to this range.
///
/// \returns Output iterator to the element past the last element copied.
///
template <typename T, typename OutputIt>
OutputIt continued_fraction(const rational<T>& r, OutputIt out_it) {
  T a = r.numerator(), b = r.denominator(), tmp;
  while (b != 0) {
    *out_it++ = a / b;
    tmp = b, b = a % b, a = tmp;
  }
  return out_it;
}

/// \brief Reduces the continued fraction representation of a rational number to
/// its normal form.
///
/// \param coeffs The coefficients of the continued fraction representation.
///
/// \pre <tt>coeffs.size() > 0</tt>.
///
/// \returns The reduced rational number.
///
template <typename T>
rational<T> evaluate_continued_fraction(const std::vector<T>& coeffs) {
  assert(coeffs.size() > 0);
  rational<T> r{coeffs.back()};
  std::for_each(coeffs.rbegin() + 1, coeffs.rend(),
                [&](const T& ai) { r = rational<T>(ai) + reciprocal(r); });
  return r;
}

} // end namespace cpl

#endif // Header guard

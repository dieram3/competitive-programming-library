//          Copyright Diego Ramírez October 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DJP_MATH_RATIONAL_HPP
#define DJP_MATH_RATIONAL_HPP

#include <djp/number_theory/euclid.hpp> // For djp::gcd
#include <algorithm>                    // For std::for_each
#include <stdexcept>                    // For std::domain_error
#include <vector>                       // For std::vector

namespace djp {

template <typename T>
class rational {
  T num, den;
  struct raw_tag {};

private:
  rational(const T &num_, const T &den_, raw_tag) : num{num_}, den{den_} {}

public:
  rational(const T &num_ = 0) : num{num_}, den{1} {}
  rational(const T &a, const T &b) {
    if (b == 0)
      throw std::domain_error("Rational: Denominator cannot be zero");
    b < 0 ? (num = -a, den = -b) : (num = a, den = b);
    const T g = gcd(num, den);
    num /= g;
    den /= g;
  }

  T numerator() const { return num; }
  T denominator() const { return den; }

  rational reciprocal() const {
    if (num == 0)
      throw std::domain_error("Rational: Zero has no reciprocal");
    return num < 0 ? rational(-den, -num, raw_tag{})
                   : rational(den, num, raw_tag{});
  }

  friend rational operator+(const rational &x, const rational &y) {
    const T g = gcd(x.den, y.den);
    const T a = x.num * (y.den / g) + y.num * (x.den / g);
    const T b = x.den / g * y.den;
    return rational(a, b);
  }

  friend rational operator-(const rational &x, const rational &y) {
    const T g = gcd(x.den, y.den);
    const T a = x.num * (y.den / g) - y.num * (x.den / g);
    const T b = x.den / g * y.den;
    return rational(a, b);
  }

  friend rational operator*(const rational &x, const rational &y) {
    const T g1 = gcd(x.num, y.den);
    const T g2 = gcd(x.den, y.num);
    const T a = (x.num / g1) * (y.num / g2);
    const T b = (x.den / g2) * (y.den / g1);
    return rational(a, b, raw_tag{});
  }

  friend rational operator/(const rational &x, const rational &y) {
    if (y.num == 0)
      throw std::domain_error("Rational: Division by zero");
    const T g1 = gcd(x.num, y.num);
    const T g2 = gcd(x.den, y.den);
    T a = (x.num / g1) * (y.den / g2);
    T b = (x.den / g2) * (y.num / g1);
    return b < 0 ? rational(-a, -b, raw_tag{}) : rational(a, b, raw_tag{});
  }
};

template <typename T>
rational<T> evaluate_continued_fraction(const std::vector<T> &coeffs) {
  rational<T> r = coeffs.back();
  std::for_each(coeffs.rbegin() + 1, coeffs.rend(),
                [&](const T &ai) { r = rational<T>(ai) + r.reciprocal(); });
  return r;
}

template <typename T, typename OutputIt>
OutputIt continued_fraction(const rational<T> &r, OutputIt out_it) {
  T a = r.numerator(), b = r.denominator(), tmp;
  while (b != 0) {
    *out_it++ = a / b;
    tmp = b, b = a % b, a = tmp;
  }
  return out_it;
}

} // end namespace djp

#endif // Header guard

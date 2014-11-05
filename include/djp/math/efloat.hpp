#ifndef DJP_UTILITY_MATRIX_HPP
#define DJP_UTILITY_MATRIX_HPP

#include <type_traits>
#include <limits>
#include <cmath>
#include <ostream>
#include <istream>

namespace djp {

// Floating point wrapper based on epsilon comparison
template <class T>
class efloat {
 public:
  static_assert(std::is_floating_point<T>::value, "T must be floating point");

  efloat() = default;
  efloat(T value) : value_{value} {}

  friend bool operator==(efloat x, efloat y) {
    return std::abs(x.value_ - y.value_) < std::numeric_limits<T>::epsilon();
  }
  friend bool operator<(efloat x, efloat y) {
    return x != y && x.value_ < y.value_;
  }
  friend bool operator!=(efloat x, efloat y) { return !(x == y); }
  friend bool operator>(efloat x, efloat y) { return y < x; }
  friend bool operator<=(efloat x, efloat y) { return !(y < x); }
  friend bool operator>=(efloat x, efloat y) { return !(x < y); }

  friend efloat operator+(efloat x) { return x.value_; }
  friend efloat operator-(efloat x) { return -x.value_; }
  friend efloat operator+(efloat x, efloat y) { return x.value_ + y.value_; }
  friend efloat operator-(efloat x, efloat y) { return x.value_ - y.value_; }
  friend efloat operator*(efloat x, efloat y) { return x.value_ * y.value_; }
  friend efloat operator/(efloat x, efloat y) { return x.value_ / y.value_; }

  efloat& operator+=(efloat d) { return *this = *this + d; }
  efloat& operator-=(efloat d) { return *this = *this - d; }
  efloat& operator*=(efloat d) { return *this = *this * d; }
  efloat& operator/=(efloat d) { return *this = *this / d; }

  friend efloat abs(efloat x) { return std::abs(x.value_); }
  friend efloat sqrt(efloat x) { return std::sqrt(x.value_); }
  friend efloat exp(efloat x) { return std::exp(x.value_); }
  friend efloat log(efloat x) { return std::log(x.value_); }
  friend efloat pow(efloat x, efloat y) { return std::pow(x.value_, y.value_); }

  template <class Arithmetic>
  explicit operator Arithmetic() const {
    return value_;
  }

  friend std::istream& operator>>(std::istream& is, efloat& x) {
    return is >> x.value_;
  }
  friend std::ostream& operator<<(std::ostream& os, efloat x) {
    return os << x.value_;
  }

 private:
  T value_;
};

}  // namespace djp

#endif  // Header guard

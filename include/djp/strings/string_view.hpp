#ifndef DJP_STRING_STRING_VIEW_HPP
#define DJP_STRING_STRING_VIEW_HPP

#include <string>
#include <algorithm>
#include <ostream>
#include <cassert>

namespace djp {

template <class CharT, class Traits = std::char_traits<char>>
class basic_string_view {
 public:
  using value_type = CharT;
  using const_pointer = const CharT*;
  using const_reference = const CharT&;
  using size_type = std::size_t;
  static constexpr size_type npos = size_type(-1);

  basic_string_view(const CharT* s, size_type count) : data_{s}, len_{count} {}

  bool empty() const { return len_ == 0; }
  size_type size() const { return len_; }
  const_pointer data() const { return data_; }
  const_pointer begin() const { return data(); }
  const_pointer end() const { return data() + size(); }
  const_reference operator[](size_type i) const { return data_[i]; }

  basic_string_view substr(size_type pos = 0, size_type count = npos) const {
    assert(pos < size());
    count = std::min(count, size() - pos);
    return basic_string_view(data() + pos, count);
  }

  int compare(basic_string_view v) const {
    auto rlen = std::min(size(), v.size());
    int cmp = Traits::compare(data(), v.data(), rlen);
    if (cmp != 0) return cmp;
    if (size() < v.size()) return -1;
    if (size() > v.size()) return 1;
    return 0;
  }

  friend bool operator<(basic_string_view lhs, basic_string_view rhs) {
    return lhs.compare(rhs) < 0;
  }

  friend bool operator==(basic_string_view lhs, basic_string_view rhs) {
    return lhs.compare(rhs) == 0;
  }

 private:
  const_pointer data_;
  size_type len_;
};

using string_view = basic_string_view<char>;
using wstring_view = basic_string_view<wchar_t>;

}  // namespace djp

#endif  // Header guard

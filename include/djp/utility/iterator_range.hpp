#ifndef DJP_UTILITY_ITERATOR_RANGE_HPP
#define DJP_UTILITY_ITERATOR_RANGE_HPP

#include <iterator>
#include <cstddef>

namespace djp {

template <class Iterator>
class iterator_range {
 public:
  using size_type = std::size_t;
  using reference = typename std::iterator_traits<Iterator>::reference;

  iterator_range() = default;
  iterator_range(Iterator first, Iterator last) : begin_{first}, end_{last} {}

  Iterator begin() const { return begin_; }
  Iterator end() const { return end_; }

  size_type size() const { return std::distance(begin_, end_); }
  reference operator[](size_type at) const { return begin_[at]; }

 private:
  Iterator begin_;
  Iterator end_;
};

template <class Iterator>
iterator_range<Iterator> make_iterator_range(Iterator first, Iterator last) {
  return {first, last};
}

}

#endif  // Header guard

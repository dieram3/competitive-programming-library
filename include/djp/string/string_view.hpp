#ifndef DJP_STRING_STRING_VIEW_HPP
#define DJP_STRING_STRING_VIEW_HPP

#include <string>
#include <algorithm>
#include <ostream>
#include <cassert>

namespace djp {

class string_view {
public:
    using const_iterator = std::string::const_iterator;
    using iterator = const_iterator;
    using size_type = std::string::size_type;
    static constexpr auto npos = std::string::npos;

    string_view(iterator first, iterator last) : begin_(first), end_(last) {}

    string_view(const std::string& str) : string_view(str.begin(), str.end()) {}

    string_view(const std::string& str, size_type pos, size_type count = npos) {
        assert(pos <= str.size());
        count = std::min(count, str.size() - pos);
        begin_ = str.begin() + pos;
        end_ = begin_ + count;
    }

    bool empty() const { return begin_ == end_; }
    size_type size() const { return end_ - begin_; }
    const char* data() const { return &*begin_; }
    iterator begin() const { return begin_; }
    iterator end() const { return end_; }
    char operator[](size_type i) const { return begin_[i]; }

private:
    iterator begin_;
    iterator end_;
};

bool operator<(const string_view& lhs, const string_view& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

bool operator==(const string_view& lhs, const string_view& rhs) {
    return lhs.size() == rhs.size() &&
            std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

std::ostream& operator<<(std::ostream& output, const string_view& sv) {
    return output.write(sv.data(), sv.size());
}


} // namespace djp

#endif // Header guard

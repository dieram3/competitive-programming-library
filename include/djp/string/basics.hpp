#ifndef DJP_STRING_BASICS_HPP
#define DJP_STRING_BASICS_HPP

#include <string>
#include <vector>
#include <djp/string/string_view.hpp>

namespace djp {

std::vector<string_view> split(const std::string& str, const std::string& sep) {
    std::vector<string_view> tokens;
    auto pos = str.find_first_not_of(sep);
    while(pos != std::string::npos) {
        auto end_pos = str.find_first_of(sep, pos+1);
        if(end_pos == std::string::npos) {
            tokens.emplace_back(str, pos);
            break;
        }
        else {
            tokens.emplace_back(str, pos, end_pos - pos);
            pos = str.find_first_not_of(sep, end_pos+1);
        }
    }
    return tokens;
}

} // namespace djp

#endif // Header guard

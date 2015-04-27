#include <string>
#include <iostream>
#include <djp/strings/suffix_array.hpp>

struct comparer {
  const std::string* text;
  bool operator()(size_t suffix, const std::string& pattern) const {
    return text->compare(suffix, pattern.size(), pattern) < 0;
  }

  bool operator()(const std::string& pattern, size_t suffix) const {
    return text->compare(suffix, pattern.size(), pattern) > 0;
  }
};

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::clog << "Usage: " << argv[0] << " <pattern>\n";
    return -1;
  }

  std::string text = "The quick brown fox jumps over the lazy dog";
  std::string pattern = argv[1];
  const auto sa = djp::make_suffix_array(text);

  std::cout << "Text: \"" << text << "\"\n\n";

  auto p = std::equal_range(sa.begin(), sa.end(), pattern, comparer{&text});
  size_t appearances = p.second - p.first;
  if (appearances == 0) {
    std::cout << "Pattern not found" << '\n';
  } else {
    std::cout << "Pattern found " << appearances << " times:\n";
    for (; p.first != p.second; ++p.first) {
      size_t suffix = *p.first;
      std::cout << "Suffix(" << suffix << "): \"" << text.substr(suffix, 10)
                << "...\"\n";
    }
  }
}

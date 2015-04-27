#include <string>
#include <iostream>
#include <djp/strings/suffix_array.hpp>

struct compare {
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
  std::cout << "Text: \"" << text << "\"\n\n";
  auto sa = djp::make_suffix_array(text);

  std::string pattern = argv[1];
  auto p = std::equal_range(sa.begin(), sa.end(), pattern, compare{&text});
  auto appearances = std::distance(p.first, p.second);
  if (appearances == 0) {
    std::cout << "Pattern not found" << '\n';
  } else {
    std::cout << "Pattern found " << appearances << " times:\n";
    std::for_each(p.first, p.second, [&text](size_t pos) {
      std::cout << "At " << pos << ": \"" << text.substr(pos, 10) << "...\"\n";
    });
  }
}

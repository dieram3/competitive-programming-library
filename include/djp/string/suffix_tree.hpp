#ifndef DJP_STRING_SUFFIX_TREE_HPP
#define DJP_STRING_SUFFIX_TREE_HPP

#include <algorithm>
#include <cstdio>
#include <map>
#include <string>

namespace djp {

class SuffixTree {
 public:
  struct Node {
    std::map<char, Node> children;
    std::size_t count;
  };

 public:
  explicit SuffixTree(const std::string &str) {
    auto first = str.begin();
    auto last = str.end();
    for (; first != last; ++first) {
      InsertSuffix(first, last);
    }
  }
  std::string LongestRepeatedSubStr() const {
    std::string longestSubStr;
    std::string currentSubStr;
    LongestRepeatedSubStrImpl(longestSubStr, currentSubStr, root_);
    return longestSubStr;
  }

 private:
  void LongestRepeatedSubStrImpl(std::string &LongestStr,
                                 std::string &currentStr,
                                 const Node &currentNode) const {
    if (currentNode.count > 1 && currentStr.size() > LongestStr.size()) {
      LongestStr = currentStr;
    }
    for (const auto &p : currentNode.children) {
      currentStr.push_back(p.first);
      LongestRepeatedSubStrImpl(LongestStr, currentStr, p.second);
      currentStr.pop_back();
    }
  }

  template <class Iterator>
  void InsertSuffix(const Iterator first, const Iterator last) {
    auto current = &root_;
    std::for_each(first, last, [&current](char c) {
      current = &current->children[c];
      ++current->count;
    });
  }

 private:
  Node root_;
};
}

#endif  // Header guard

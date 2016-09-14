//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/data_structure/lazyprop_segtree.hpp>
#include <gtest/gtest.h>

#include <algorithm>  // min, max
#include <cassert>    // assert
#include <climits>    // INT_MAX
#include <cstddef>    // size_t
#include <functional> // plus
#include <string>     // string
#include <vector>     // vector

using cpl::lazyprop_segtree;
using std::size_t;

namespace {
class LazypropSegtreeTest : public ::testing::Test {
protected:
  using vec_t = std::vector<int>;

protected:
  LazypropSegtreeTest() {
    vec = {2,  10, 3,  17, 5,  // 0 to 4
           12, 24, 3,  0,  10, // 5 to 9
           8,  3,  12, 27, 4,  // 10 to 14
           4,  13, 20, 8,  7}; // 15 to 19
    assert(vec.size() == 20);
  }

protected:
  vec_t vec;
};
} // end anonymous namespace

TEST_F(LazypropSegtreeTest, IdentityConstructorTest) {
  class op_list {
    int value_to_add;

  public:
    explicit op_list(int val = 0) : value_to_add{val} {}
    void push(const op_list& op) {
      value_to_add += op.value_to_add;
    }
    int apply(size_t /*unused*/, int reduced_val) {
      return reduced_val + value_to_add;
    }
    bool empty() const {
      return value_to_add == 0;
    }
  };

  auto minimum = [](int x, int y) { return std::min(x, y); };

  using segtree_t = lazyprop_segtree<int, decltype(minimum), op_list>;
  segtree_t segtree(size_t{35}, INT_MAX, minimum);

  EXPECT_EQ(35, segtree.size());
  EXPECT_EQ(INT_MAX, segtree.reduce(0, 35));
  segtree.apply(0, 4, op_list(-2));
  EXPECT_EQ(INT_MAX, segtree.reduce(4, 20));
  EXPECT_EQ(INT_MAX - 2, segtree.reduce(3, 20));
}

TEST_F(LazypropSegtreeTest, AdderSegtreeTest) {
  class add_ops {
    int acc_value;

  public:
    explicit add_ops(int val = 0) : acc_value{val} {}

    void push(const add_ops& ops) {
      acc_value += ops.acc_value;
    }
    int apply(size_t rsize, int reduced_val) const {
      return reduced_val + static_cast<int>(rsize) * acc_value;
    }
    bool empty() const {
      return acc_value == 0;
    }
  };

  using adder_segtree = lazyprop_segtree<int, std::plus<int>, add_ops>;
  adder_segtree segtree(vec.begin(), vec.end());

  // Trivial queries
  EXPECT_EQ(2, segtree.reduce(0, 1));
  EXPECT_EQ(10, segtree.reduce(1, 2));
  EXPECT_EQ(7, segtree.reduce(19, 20));

  // Composed queries
  EXPECT_EQ(32, segtree.reduce(0, 4));
  EXPECT_EQ(37, segtree.reduce(0, 5));
  EXPECT_EQ(49, segtree.reduce(0, 6));
  EXPECT_EQ(3, segtree.reduce(7, 9));
  EXPECT_EQ(13, segtree.reduce(7, 10));
  EXPECT_EQ(88, segtree.reduce(12, 19));
  EXPECT_EQ(35, segtree.reduce(17, 20));
  EXPECT_EQ(192, segtree.reduce(0, 20));

  // Let's to do some modifications:
  segtree.apply(0, 20, add_ops(5)); // Overall sum should increase by 100
  EXPECT_EQ(292, segtree.reduce(0, 20));
  segtree.apply(0, 10, add_ops(5)); // Overall sum should increase by 50
  EXPECT_EQ(342, segtree.reduce(0, 20));
  segtree.apply(10, 20, add_ops(5)); // Overall sum should increase by 50
  EXPECT_EQ(392, segtree.reduce(0, 20));

  // At this point, each element was increased by 10.
  // 12,20,13,27,15,22,34,13,10,20,18,13,22,37,14,14,23,30,18,17
  segtree.apply(3, 11, add_ops(7));
  // 12,20,13,(34,22,29,41,20,17,27,25),13,22,37,14,14,23,30,18,17
  segtree.apply(0, 4, add_ops(-5));
  // (7,15,8,29),22,29,41,20,17,27,25,13,22,37,14,14,23,30,18,17
  segtree.apply(18, 20, add_ops(5));
  // 7,15,8,29,22,29,41,20,17,27,25,13,22,37,14,14,23,30,(23,22)
  segtree.apply(9, 17, add_ops(11));
  // 7,15,8,29,22,29,41,20,17,(38,36,24,33,48,25,25,34),30,23,22
  segtree.apply(15, 19, add_ops(-20));
  // 7,15,8,29,22,29,41,20,17,38,36,24,33,48,25,(5,14,10,3),22
  segtree.apply(0, 2, add_ops(-10));
  // (-3,5),8,29,22,29,41,20,17,38,36,24,33,48,25,5,14,10,3,22

  EXPECT_EQ(-3, segtree.reduce(0, 1));
  EXPECT_EQ(367, segtree.reduce(3, 16));
  EXPECT_EQ(337, segtree.reduce(3, 14));
  EXPECT_EQ(88, segtree.reduce(2, 6));
  EXPECT_EQ(343, segtree.reduce(5, 19));
  EXPECT_EQ(426, segtree.reduce(0, 20));
  EXPECT_EQ(394, segtree.reduce(1, 17));
  EXPECT_EQ(158, segtree.reduce(3, 9));
  EXPECT_EQ(223, segtree.reduce(9, 17));
  EXPECT_EQ(381, segtree.reduce(3, 17));

  vec_t final_values(20);
  for (size_t i = 0; i < 20; ++i)
    final_values[i] = segtree.reduce(i, i + 1);
  const vec_t expected_final_values = {-3, 5,  8,  29, 22, 29, 41, 20, 17, 38,
                                       36, 24, 33, 48, 25, 5,  14, 10, 3,  22};
  EXPECT_EQ(expected_final_values, final_values);
}

TEST_F(LazypropSegtreeTest,
       MaxElementQueryAllowingAdditionsAndAssignmentsTest) {

  class op_list {
    int value;
    bool must_assign; // if false then the value has to be added

  public:
    explicit op_list(int v = 0, bool ma = false) : value{v}, must_assign{ma} {}

    static op_list add(int val) {
      return op_list(val);
    }
    static op_list assign(int val) {
      return op_list(val, true);
    }

    bool empty() const {
      return value == 0 && !must_assign;
    }
    int apply(size_t /*unused*/, int reduced_val) const {
      return must_assign ? value : reduced_val + value;
    }
    op_list& push(const op_list& op) {
      if (op.must_assign) {
        value = op.value;
        must_assign = true;
      } else {
        value += op.value;
      }
      return *this; // Just for convenience, not required
    }
  };
  struct maximum {
    int operator()(int x, int y) const {
      return std::max(x, y);
    }
  };

  using segtree_t = lazyprop_segtree<int, maximum, op_list>;
  segtree_t segtree(vec.begin(), vec.end());

  // Initial values:
  // 2,10,3,17,5,12,24,3,0,10,8,3,12,27,4,4,13,20,8,7

  // Trivial queries
  EXPECT_EQ(2, segtree.reduce(0, 1));
  EXPECT_EQ(10, segtree.reduce(1, 2));
  EXPECT_EQ(7, segtree.reduce(19, 20));

  // Composed queries
  EXPECT_EQ(17, segtree.reduce(0, 4));
  EXPECT_EQ(17, segtree.reduce(0, 5));
  EXPECT_EQ(17, segtree.reduce(0, 6));
  EXPECT_EQ(3, segtree.reduce(7, 9));
  EXPECT_EQ(10, segtree.reduce(7, 10));
  EXPECT_EQ(27, segtree.reduce(12, 19));
  EXPECT_EQ(20, segtree.reduce(17, 20));
  EXPECT_EQ(27, segtree.reduce(0, 20));

  // 2,10,3,17,5,12,24,3,0,10,8,3,12,27,4,4,13,20,8,7
  segtree.apply(0, 20, op_list::add(5));
  // (7,15,8,22,10,17,29,8,5,15,13,8,17,32,9,9,18,25,13,12)
  segtree.apply(0, 10, op_list::add(6));
  // (13,21,14,28,16,23,35,14,11,21),13,8,17,32,9,9,18,25,13,12
  segtree.apply(10, 20, op_list::add(7));
  // 13,21,14,28,16,23,35,14,11,21,(20,15,24,39,16,16,24,32,20,19)
  segtree.apply(15, 19, op_list::assign(12));
  // 13,21,14,28,16,23,35,14,11,21,20,15,24,39,16,(12,12,12,12),19

  EXPECT_EQ(28, segtree.reduce(2, 6));
  EXPECT_EQ(24, segtree.reduce(7, 13));
  EXPECT_EQ(35, segtree.reduce(2, 13));
  EXPECT_EQ(39, segtree.reduce(2, 14));
  EXPECT_EQ(39, segtree.reduce(13, 14));

  // 13,21,14,28,16,23,35,14,11,21,20,15,24,39,16,12,12,12,12,19
  segtree.apply(16, 20, op_list::add(8));
  // 13,21,14,28,16,23,35,14,11,21,20,15,24,39,16,12,(20,20,20,27)
  segtree.apply(0, 4, op_list::assign(4).push(op_list::add(8)));
  // (12,12,12,12),16,23,35,14,11,21,20,15,24,39,16,12,20,20,20,27
  segtree.apply(3, 7, op_list::add(18).push(op_list::assign(15)));
  // 12,12,12,(15,15,15,15),14,11,21,20,15,24,39,16,12,20,20,20,27

  EXPECT_EQ(39, segtree.reduce(0, 20));
  EXPECT_EQ(15, segtree.reduce(0, 9));
  EXPECT_EQ(21, segtree.reduce(0, 10));
  EXPECT_EQ(20, segtree.reduce(15, 19));
  EXPECT_EQ(27, segtree.reduce(15, 20));

  vec_t final_values(20);
  for (size_t i = 0; i < 20; ++i)
    final_values[i] = segtree.reduce(i, i + 1);
  const vec_t expected_final_values = {12, 12, 12, 15, 15, 15, 15, 14, 11, 21,
                                       20, 15, 24, 39, 16, 12, 20, 20, 20, 27};
  EXPECT_EQ(expected_final_values, final_values);
}

TEST_F(LazypropSegtreeTest, AssociativityTest) {
  using std::string;

  class op_list {
    int rotation_value;
    char rotate(char c) {
      return ((c - 'a') + rotation_value) % 26 + 'a';
    }

  public:
    explicit op_list(int rot = 0) : rotation_value{rot} {}
    bool empty() const {
      return rotation_value == 0;
    }
    void push(const op_list& op) {
      rotation_value = (rotation_value + op.rotation_value) % 26;
    }
    string apply(size_t /*unused*/, const string& str) {
      string outstr;
      for (char c : str)
        outstr.push_back(rotate(c));
      return outstr;
    }
  };

  using segtree_t = lazyprop_segtree<string, std::plus<string>, op_list>;

  std::vector<std::string> init_list = {
      "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
      "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
  assert(init_list.size() == 26);
  segtree_t segtree(init_list.begin(), init_list.end());

  EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", segtree.reduce(0, 26));
  segtree.apply(0, 26, op_list(1));
  EXPECT_EQ("bcdefghijklmnopqrstuvwxyza", segtree.reduce(0, 26));
  segtree.apply(0, 26, op_list(3));
  EXPECT_EQ("efghijklmnopqrstuvwxyzabcd", segtree.reduce(0, 26));
  segtree.apply(0, 13, op_list(2));
  // (ghijklmnopqrs)rstuvwxyzabcd
  segtree.apply(13, 20, op_list(4));
  // ghijklmnopqrs(vwxyzab)yzabcd
  segtree.apply(10, 15, op_list(6));
  // ghijklmnop(wxybc)xyzabyzabcd

  EXPECT_EQ("ghijklmn", segtree.reduce(0, 8));
  EXPECT_EQ("mnopwxybcxyzab", segtree.reduce(6, 20));
  EXPECT_EQ("zabyzabcd", segtree.reduce(17, 26));
  EXPECT_EQ("ghijklmnopwxybcxyzabyzabcd", segtree.reduce(0, 26));
}

//#include <random>
//#include <chrono>
// TEST_F(LazypropSegtreeTest, Benchmark) {
//  using int_t = uint64_t;
//  class add_ops {
//    int_t acc_value;
//
//  public:
//    explicit add_ops(int_t val = 0) : acc_value{val} {}
//    bool empty() const { return acc_value == 0; }
//    void push(const add_ops &ops) { acc_value += ops.acc_value; }
//    int_t apply(size_t rsize, int_t reduced_val) const {
//      return reduced_val + static_cast<int_t>(rsize) * acc_value;
//    }
//  };
//
//  using segtree_t = lazyprop_segtree<int_t, std::plus<int_t>, add_ops>;
//
//  enum class op_type { increase, reduce };
//  struct query {
//    op_type op;
//    size_t beg;
//    size_t end;
//    int_t incr_value{};
//  };
//  const size_t num_elems = 100000;
//  const size_t num_queries = 100000;
//
//  std::vector<query> queries(num_queries);
//  {
//    std::mt19937 engine;
//    std::bernoulli_distribution do_modification(0.7);
//    std::uniform_int_distribution<size_t> gen_pos;
//    std::uniform_int_distribution<int_t> gen_value(0, 1000);
//    using param_t = decltype(gen_pos)::param_type;
//    for (auto &q : queries) {
//      q.op = do_modification(engine) ? op_type::increase : op_type::reduce;
//      q.beg = gen_pos(engine, param_t(0, num_elems - 1));
//      q.end = gen_pos(engine, param_t(q.beg + 1, num_elems));
//      if (q.op == op_type::increase)
//        q.incr_value = gen_value(engine);
//    }
//  }
//
//  using namespace std::chrono;
//  auto start = steady_clock::now();
//  segtree_t segtree(num_elems, 0); // 0 is the identity value.
//  int_t checksum = 0;
//  for (const auto &q : queries) {
//    if (q.op == op_type::increase)
//      segtree.apply(q.beg, q.end, add_ops(q.incr_value));
//    else
//      checksum += segtree.reduce(q.beg, q.end);
//  }
//  auto end = steady_clock::now();
//  auto elapsed = duration_cast<milliseconds>(end - start);
//  std::clog << "Elapsed time: " << elapsed.count() << "ms\n";
//  std::clog << "Checksum: " << checksum << '\n';
//}

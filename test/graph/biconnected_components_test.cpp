//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/biconnected_components.hpp>
#include <gtest/gtest.h>

#include <djp/graph/undirected_graph.hpp>

#include <vector>        // for std::vector
#include <unordered_map> // for std::unordered_map
#include <cstddef>       // for std::size_t

using namespace djp;
using std::size_t;
using std::vector;
using std::unordered_map;

static void normalize(vector<size_t> &vec) {
  unordered_map<size_t, size_t> id_map;
  for (const size_t elem : vec)
    id_map.emplace(elem, id_map.size());
  for (size_t &elem : vec)
    elem = id_map.at(elem);
}

static void bi_comps_check(const undirected_graph &g, const size_t exp_comp_cnt,
                           const vector<size_t> &exp_bi_comps,
                           const vector<size_t> &exp_cut_set) {
  vector<size_t> bi_comp;
  vector<bool> is_cut;
  EXPECT_EQ(exp_comp_cnt, biconnected_components(g, bi_comp, is_cut));

  EXPECT_EQ(g.num_edges(), bi_comp.size());
  EXPECT_EQ(g.num_vertices(), is_cut.size());

  normalize(bi_comp);
  EXPECT_EQ(exp_bi_comps, bi_comp);

  const size_t num_vertices = g.num_vertices();
  std::vector<size_t> cut_set;
  for (size_t v = 0; v != num_vertices; ++v)
    if (is_cut[v])
      cut_set.push_back(v);

  EXPECT_EQ(exp_cut_set, cut_set);
}

TEST(BiconnectedComponentsTest, EmptyGraphTest) {
  undirected_graph g(0);
  bi_comps_check(g, 0, {}, {});
}

TEST(BiconnectedComponentsTest, SmallComponentsTest) {
  undirected_graph g(6);
  g.add_edge(0, 1); // comp 0
  g.add_edge(3, 4); // comp 1
  g.add_edge(4, 5); // comp 2

  bi_comps_check(g, 3, {0, 1, 2}, {4});
}

TEST(BiconnectedComponentsTest, ForestTest) {
  undirected_graph g(8);
  g.add_edge(0, 4);
  g.add_edge(0, 5);
  g.add_edge(1, 3);
  g.add_edge(2, 5);
  g.add_edge(3, 6);
  g.add_edge(5, 7);

  ASSERT_EQ(6, g.num_edges());
  bi_comps_check(g, 6, {0, 1, 2, 3, 4, 5}, {0, 3, 5});
}

TEST(BiconnectedComponentsTest, BiconnectedGraphTest) {
  undirected_graph g(8);
  g.add_edge(0, 2);
  g.add_edge(0, 5);
  g.add_edge(0, 7);
  g.add_edge(1, 3);
  g.add_edge(1, 6);
  g.add_edge(2, 6);
  g.add_edge(2, 7);
  g.add_edge(3, 5);
  g.add_edge(4, 6);
  g.add_edge(4, 7);

  ASSERT_EQ(10, g.num_edges());
  bi_comps_check(g, 1, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {});
}

TEST(BiconnectedComponentsTest, FourComponentsTest) {
  undirected_graph g(9);
  g.add_edge(0, 1); // comp 0
  g.add_edge(0, 2); // comp 0
  g.add_edge(1, 2); // comp 0
  g.add_edge(2, 3); // comp 1
  g.add_edge(2, 5); // comp 1
  g.add_edge(3, 4); // comp 1
  g.add_edge(4, 5); // comp 1
  g.add_edge(5, 6); // comp 2
  g.add_edge(6, 7); // comp 3
  g.add_edge(6, 8); // comp 3
  g.add_edge(7, 8); // comp 3

  ASSERT_EQ(11, g.num_edges());
  bi_comps_check(g, 4, {0, 0, 0, 1, 1, 1, 1, 2, 3, 3, 3}, {2, 5, 6});
}

TEST(BiconnectedComponentsTest, SevenComponentsTest) {
  undirected_graph g(14);
  g.add_edge(0, 1);   // comp 0
  g.add_edge(0, 2);   // comp 0
  g.add_edge(1, 2);   // comp 0
  g.add_edge(1, 8);   // comp 0
  g.add_edge(2, 9);   // comp 0
  g.add_edge(3, 4);   // comp 1
  g.add_edge(3, 5);   // comp 1
  g.add_edge(4, 6);   // comp 1
  g.add_edge(5, 6);   // comp 1
  g.add_edge(6, 10);  // comp 2
  g.add_edge(7, 8);   // comp 3
  g.add_edge(7, 10);  // comp 4
  g.add_edge(8, 11);  // comp 5
  g.add_edge(8, 12);  // comp 0
  g.add_edge(9, 12);  // comp 0
  g.add_edge(12, 13); // comp 6

  ASSERT_EQ(16, g.num_edges());
  bi_comps_check(g, 7, {0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 3, 4, 5, 0, 0, 6},
                 {6, 7, 8, 10, 12});
}

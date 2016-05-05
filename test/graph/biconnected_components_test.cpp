//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/graph/biconnected_components.hpp>
#include <gtest/gtest.h>

#include <cpl/graph/undirected_graph.hpp> // undirected_graph
#include <algorithm>                      // sort, max_element
#include <cstddef>                        // size_t
#include <unordered_map>                  // unordered_map
#include <vector>                         // vector

using cpl::biconnected_components;
using cpl::articulation_points_and_bridges;
using cpl::undirected_graph;
using std::max_element;
using std::size_t;
using std::vector;
using std::unordered_map;

static void normalize(vector<size_t>& vec) {
  unordered_map<size_t, size_t> id_map;
  for (const size_t elem : vec)
    id_map.emplace(elem, id_map.size());
  for (size_t& elem : vec)
    elem = id_map.at(elem);
}

static std::vector<size_t>
get_articulation_points(const vector<bool>& is_articulation) {
  const size_t num_v = is_articulation.size();
  std::vector<size_t> articulation_points;
  for (size_t v = 0; v != num_v; ++v)
    if (is_articulation[v])
      articulation_points.push_back(v);
  return articulation_points;
}

// Finds bridges from biconnected components map in sorted order.
static std::vector<size_t> find_bridges(const vector<size_t>& bicomp) {
  const size_t num_edges = bicomp.size();
  const size_t num_bicomps =
      bicomp.empty() ? 0 : *max_element(bicomp.begin(), bicomp.end()) + 1;

  std::vector<size_t> bicomp_size(num_bicomps);
  for (size_t e = 0; e != num_edges; ++e)
    ++bicomp_size[bicomp[e]];

  std::vector<size_t> bridges;
  for (size_t e = 0; e != num_edges; ++e)
    if (bicomp_size[bicomp[e]] == 1)
      bridges.push_back(e);
  return bridges;
}

static void
check_ap_and_bridges(const undirected_graph& g,
                     const vector<size_t>& expected_articulation_points,
                     const vector<size_t>& expected_bridges) {
  vector<size_t> articulation_points;
  vector<size_t> bridges;
  auto put_ap = [&](size_t v) { articulation_points.push_back(v); };
  auto put_bridge = [&](size_t e) { bridges.push_back(e); };

  articulation_points_and_bridges(g, put_ap, put_bridge);
  std::sort(bridges.begin(), bridges.end());

  EXPECT_EQ(expected_articulation_points, articulation_points);
  EXPECT_EQ(expected_bridges, bridges);
}

static void bi_comps_check(const undirected_graph& g, const size_t num_bicomps,
                           const vector<size_t>& expected_bicomp,
                           const vector<size_t>& expected_articulation_points) {
  vector<size_t> bicomp;
  vector<bool> is_articulation;
  EXPECT_EQ(num_bicomps, biconnected_components(g, bicomp, is_articulation));

  EXPECT_EQ(g.num_edges(), bicomp.size());
  EXPECT_EQ(g.num_vertices(), is_articulation.size());

  normalize(bicomp);
  EXPECT_EQ(expected_bicomp, bicomp);

  EXPECT_EQ(expected_articulation_points,
            get_articulation_points(is_articulation));

  check_ap_and_bridges(g, expected_articulation_points,
                       find_bridges(expected_bicomp));
}

// =========================================
// Tests section
// =========================================

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

TEST(BiconnectedComponentsTest, RootWithTwoChildrenTest) {
  undirected_graph g(9);
  g.add_edge(0, 1); // comp 0
  g.add_edge(0, 5); // comp 1
  g.add_edge(1, 2); // comp 2
  g.add_edge(1, 4); // comp 2
  g.add_edge(2, 3); // comp 2
  g.add_edge(3, 4); // comp 2
  g.add_edge(5, 6); // comp 3
  g.add_edge(5, 8); // comp 3
  g.add_edge(6, 7); // comp 3
  g.add_edge(7, 8); // comp 3
  ASSERT_EQ(10, g.num_edges());
  bi_comps_check(g, 4, {0, 1, 2, 2, 2, 2, 3, 3, 3, 3}, {0, 1, 5});
}

TEST(BiconnectedComponentsTest, RootWithOneChildTest) {
  undirected_graph g(5);
  g.add_edge(0, 1); // comp 0
  g.add_edge(1, 2); // comp 1
  g.add_edge(1, 4); // comp 1
  g.add_edge(2, 3); // comp 1
  g.add_edge(3, 4); // comp 1
  bi_comps_check(g, 2, {0, 1, 1, 1, 1}, {1});
}

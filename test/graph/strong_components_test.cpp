//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/graph/strong_components.hpp>
#include <gtest/gtest.h>

#include <cpl/graph/directed_graph.hpp> // directed_graph
#include <cstddef>                      // size_t
#include <numeric>                      // iota
#include <unordered_map>                // unordered_map
#include <vector>                       // vector

using cpl::strong_components;
using cpl::directed_graph;
using std::vector;

static std::vector<size_t> normalize(const vector<size_t>& vec) {
  std::unordered_map<size_t, size_t> id_map;
  for (const size_t elem : vec)
    id_map.emplace(elem, id_map.size());

  std::vector<size_t> norm(vec.size());
  for (size_t i = 0; i != norm.size(); ++i)
    norm[i] = id_map.at(vec[i]);
  return norm;
}

static directed_graph make_condensate(const directed_graph& g,
                                      const size_t num_comps,
                                      const vector<size_t>& comp) {
  directed_graph cg(num_comps);
  const size_t num_e = g.num_edges();
  for (size_t e = 0; e != num_e; ++e) {
    const auto src = comp[g.source(e)];
    const auto tgt = comp[g.target(e)];
    if (src == tgt)
      continue;
    cg.add_edge(src, tgt);
  }
  return cg;
}

static bool check_cg_toposort(const directed_graph& cg) {
  const size_t num_v = cg.num_vertices();
  vector<bool> ready(num_v);
  // The toposort should be num_v - 1, num_v - 2, ..., 2, 1, 0
  for (size_t v = num_v - 1; v + 1 != 0; --v) {
    for (const auto e : cg.in_edges(v))
      if (!ready[cg.source(e)])
        return false;
    ready[v] = true;
  }
  return true;
}

// Requires: expected.size() >= 1
static void check_scc(const directed_graph& g, const size_t total_scc,
                      const vector<size_t>& expected) {
  vector<size_t> comp;
  EXPECT_EQ(total_scc, strong_components(g, comp));
  for (const size_t label : comp)
    EXPECT_LT(label, total_scc) << " weird label: " << label;

  EXPECT_EQ(expected, normalize(comp));

  const auto cg = make_condensate(g, total_scc, comp);
  EXPECT_TRUE(check_cg_toposort(cg));
}

TEST(StrongComponentsTest, EmptyGraphTest) {
  directed_graph g(0);
  check_scc(g, 0, {});
}

TEST(StrongComponentsTest, SingleVertexGraphTest) {
  directed_graph g(1);
  check_scc(g, 1, {0});
  g.add_edge(0, 0);
  g.add_edge(0, 0);
  check_scc(g, 1, {0});
}

TEST(StrongComponentsTest, AloneVerticesTest) {
  directed_graph g(5);
  g.add_edge(0, 0);
  g.add_edge(3, 3);
  g.add_edge(3, 3);
  check_scc(g, 5, {0, 1, 2, 3, 4});
}

TEST(StrongComponentsTest, NonStrongConnectedGraphTest) {
  directed_graph g(8);

  g.add_edge(0, 4);
  g.add_edge(1, 0);
  g.add_edge(2, 1);
  g.add_edge(2, 3);
  g.add_edge(3, 2);
  g.add_edge(4, 1);
  g.add_edge(5, 1);
  g.add_edge(5, 4);
  g.add_edge(5, 6);
  g.add_edge(6, 2);
  g.add_edge(6, 5);
  g.add_edge(7, 3);
  g.add_edge(7, 6);
  g.add_edge(7, 7);

  EXPECT_EQ(14, g.num_edges());
  check_scc(g, 4, {0, 0, 1, 1, 0, 2, 2, 3});
}

TEST(StrongComponentsTest, StrongConnectedGraphTest) {
  directed_graph g(9);
  g.add_edge(0, 1);
  g.add_edge(1, 1);
  g.add_edge(1, 3);
  g.add_edge(2, 0);
  g.add_edge(3, 6);
  g.add_edge(4, 2);
  g.add_edge(4, 3);
  g.add_edge(5, 4);
  g.add_edge(6, 5);
  g.add_edge(6, 8);
  g.add_edge(7, 6);
  g.add_edge(8, 7);

  EXPECT_EQ(12, g.num_edges());
  check_scc(g, 1, {0, 0, 0, 0, 0, 0, 0, 0, 0});
}

TEST(StrongComponentsTest, DirectedAcyclicGraphTest) {
  directed_graph g(14);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 4);
  g.add_edge(2, 5);
  g.add_edge(3, 5);
  g.add_edge(4, 6);
  g.add_edge(4, 7);
  g.add_edge(5, 8);
  g.add_edge(5, 9);
  g.add_edge(5, 10);
  g.add_edge(6, 10);
  g.add_edge(7, 6);
  g.add_edge(11, 12);
  g.add_edge(11, 13);

  EXPECT_EQ(14, g.num_edges());
  check_scc(g, 14, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13});
}

TEST(StrongComponentsTest, PyramidalGraphTest) {
  directed_graph g(7);
  g.add_edge(0, 1);
  g.add_edge(0, 6);
  g.add_edge(1, 2);
  g.add_edge(1, 5);
  g.add_edge(2, 3);
  g.add_edge(2, 4);
  g.add_edge(4, 2);
  g.add_edge(4, 3);
  g.add_edge(5, 1);
  g.add_edge(5, 4);
  g.add_edge(6, 0);
  g.add_edge(6, 5);

  EXPECT_EQ(12, g.num_edges());
  check_scc(g, 4, {0, 1, 2, 3, 2, 1, 0});
}

TEST(StrongComponentsTest, BidirectionalTreeTest) {
  directed_graph g(5);
  auto add_edge = [&](size_t u, size_t v) {
    g.add_edge(u, v);
    g.add_edge(v, u);
  };
  add_edge(0, 1);
  add_edge(1, 3);
  add_edge(2, 3);
  add_edge(3, 4);
  EXPECT_EQ(8, g.num_edges());
  check_scc(g, 1, {0, 0, 0, 0, 0});
}

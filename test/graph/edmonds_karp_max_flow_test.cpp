//          Copyright Diego Ramírez June 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/edmonds_karp_max_flow.hpp>
#include <gtest/gtest.h>

#include <djp/graph/directed_graph.hpp>
#include <functional>
#include <cassert>
#include <cstddef>

using namespace djp;

namespace {
struct edge_data {
  long capacity;
  const edge_data *rev_edge;
  mutable long flow;
};
} // end anonymous namespace

using digraph_t = directed_graph<edge_data>;

static inline void add_edge(digraph_t &g, size_t v1, size_t v2, long c1,
                            long c2 = 0) {
  auto &edge1 = g.add_edge(v1, v2);
  auto &edge2 = g.add_edge(v2, v1);
  edge1.capacity = c1;
  edge2.capacity = c2;

  edge1.rev_edge = &edge2;
  edge2.rev_edge = &edge1;
}

TEST(EdmondsKarpMaxFlowTest, WorksOnBasicCases) {
  digraph_t g(4);
  add_edge(g, 0, 1, 20, 20);
  add_edge(g, 0, 2, 10);
  add_edge(g, 1, 2, 5);
  add_edge(g, 1, 3, 10);
  add_edge(g, 2, 3, 20, 15);

  EXPECT_EQ(20, edmonds_karp_max_flow(g, 0, 1));
  EXPECT_EQ(25, edmonds_karp_max_flow(g, 0, 2));
  EXPECT_EQ(25, edmonds_karp_max_flow(g, 0, 3));

  EXPECT_EQ(20, edmonds_karp_max_flow(g, 1, 0));
  EXPECT_EQ(25, edmonds_karp_max_flow(g, 1, 2));
  EXPECT_EQ(25, edmonds_karp_max_flow(g, 1, 3));

  EXPECT_EQ(0, edmonds_karp_max_flow(g, 2, 0));
  EXPECT_EQ(0, edmonds_karp_max_flow(g, 2, 1));
  EXPECT_EQ(20, edmonds_karp_max_flow(g, 2, 3));

  EXPECT_EQ(0, edmonds_karp_max_flow(g, 3, 0));
  EXPECT_EQ(0, edmonds_karp_max_flow(g, 3, 1));
  EXPECT_EQ(15, edmonds_karp_max_flow(g, 3, 2));
}

TEST(EdmondsKarpMaxFlowTest, WorksWhenNeedsUndoing) {
  digraph_t g(12);
  add_edge(g, 0, 1, 1);
  add_edge(g, 0, 2, 1);
  add_edge(g, 0, 3, 1);
  add_edge(g, 1, 4, 1);
  add_edge(g, 2, 5, 1);
  add_edge(g, 2, 6, 1);
  add_edge(g, 3, 7, 1);
  add_edge(g, 4, 8, 1);
  add_edge(g, 4, 9, 1);
  add_edge(g, 5, 8, 1);
  add_edge(g, 5, 10, 1);
  add_edge(g, 6, 9, 1);
  add_edge(g, 6, 10, 1);
  add_edge(g, 7, 10, 1);
  add_edge(g, 8, 11, 1);
  add_edge(g, 9, 11, 1);
  add_edge(g, 10, 11, 1);

  EXPECT_EQ(3, edmonds_karp_max_flow(g, 0, 11));
}

//#include <random>
// static graph_t make_random_graph(std::size_t num_vertices, double
// connection_p,
//                                 uint32_t seed1, uint32_t seed2) {
//  assert(num_vertices != 0);
//
//  graph_t graph(num_vertices);
//  const size_t max_parallel = 3;
//
//  std::default_random_engine gen1(seed1), gen2(seed2);
//  std::bernoulli_distribution connect_dist(connection_p / max_parallel);
//
//  std::poisson_distribution<size_t> cap_dist(5.0);
//
//  auto connect = std::bind(connect_dist, std::ref(gen1));
//  auto gen_cap = std::bind(cap_dist, std::ref(gen2));
//  for (size_t i = 0; i != num_vertices; ++i)
//    for (size_t j = i + 1; j != num_vertices; ++j)
//      for (size_t p = 0; p != max_parallel; ++p) {
//        size_t cap1 = connect() ? gen_cap() : 0;
//        size_t cap2 = connect() ? gen_cap() : 0;
//        if (cap1 || cap2)
//          add_edge(graph, i, j, cap1, cap2);
//      }
//
//  return graph;
//}
//
//#include <chrono>
//#include <iostream>
// TEST(edmonds_karp_max_flow, Benchmark) {
//  const auto g = make_random_graph(2000, 10.0 / 2000.0, 23423, 817238);
//  std::random_device rd;
//  std::mt19937 gen(34234234);
//  std::uniform_int_distribution<size_t> dist(0, g.num_vertices() - 1);
//
//  using std::chrono::steady_clock;
//  using std::chrono::duration_cast;
//  using std::chrono::milliseconds;
//
//  const auto start = steady_clock::now();
//  std::size_t total_flow = 0;
//  for (size_t rep = 0; rep < 100; ++rep) {
//    const auto source = dist(gen);
//    const auto target = dist(gen);
//    total_flow += edmonds_karp_max_flow(g, source, target);
//  }
//  const auto end = steady_clock::now();
//  const auto elapsed = duration_cast<milliseconds>(end - start);
//  std::cout << total_flow << ' ' << elapsed.count() << " ms\n";
//  // In libc++ 3.6 total_flow should be 3913
//}

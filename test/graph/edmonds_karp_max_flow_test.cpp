//          Copyright Diego Ramirez 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/graph/edmonds_karp_max_flow.hpp>
#include <gtest/gtest.h>

#include <cpl/graph/directed_graph.hpp> // directed_graph
#include <cassert>                      // assert
#include <cstddef>                      // size_t

using cpl::edmonds_karp_max_flow;
using cpl::directed_graph;
using std::size_t;

/// \todo Check residual capacity output.

TEST(EdmondsKarpMaxFlowTest, WorksOnBasicCases) {
  directed_graph g(4);
  std::vector<unsigned> capacity;
  std::vector<size_t> rev_edge;
  auto add_edge = [&](size_t s, size_t t, unsigned cap, unsigned rev_cap = 0) {
    const auto e1 = g.add_edge(s, t);
    const auto e2 = g.add_edge(t, s);
    capacity.push_back(cap);
    capacity.push_back(rev_cap);
    rev_edge.push_back(e2);
    rev_edge.push_back(e1);
  };

  add_edge(0, 1, 20, 20);
  add_edge(0, 2, 10);
  add_edge(1, 2, 5);
  add_edge(1, 3, 10);
  add_edge(2, 3, 20, 15);

  std::vector<unsigned> residual;
  auto calc_max_flow = [&](size_t src, size_t tgt) {
    return edmonds_karp_max_flow(g, src, tgt, rev_edge, capacity, residual);
  };

  EXPECT_EQ(20, calc_max_flow(0, 1));
  EXPECT_EQ(25, calc_max_flow(0, 2));
  EXPECT_EQ(25, calc_max_flow(0, 3));

  EXPECT_EQ(20, calc_max_flow(1, 0));
  EXPECT_EQ(25, calc_max_flow(1, 2));
  EXPECT_EQ(25, calc_max_flow(1, 3));

  EXPECT_EQ(0, calc_max_flow(2, 0));
  EXPECT_EQ(0, calc_max_flow(2, 1));
  EXPECT_EQ(20, calc_max_flow(2, 3));

  EXPECT_EQ(0, calc_max_flow(3, 0));
  EXPECT_EQ(0, calc_max_flow(3, 1));
  EXPECT_EQ(15, calc_max_flow(3, 2));
}

TEST(EdmondsKarpMaxFlowTest, WorksWhenNeedsUndoing) {
  directed_graph g(12);
  std::vector<unsigned> capacity;
  std::vector<size_t> rev_edge;
  auto add_edge = [&](size_t s, size_t t) {
    const auto e1 = g.add_edge(s, t);
    const auto e2 = g.add_edge(t, s);
    capacity.push_back(1);
    capacity.push_back(0);
    rev_edge.push_back(e2);
    rev_edge.push_back(e1);
  };
  add_edge(0, 1);
  add_edge(0, 2);
  add_edge(0, 3);
  add_edge(1, 4);
  add_edge(2, 5);
  add_edge(2, 6);
  add_edge(3, 7);
  add_edge(4, 8);
  add_edge(4, 9);
  add_edge(5, 8);
  add_edge(5, 10);
  add_edge(6, 9);
  add_edge(6, 10);
  add_edge(7, 10);
  add_edge(8, 11);
  add_edge(9, 11);
  add_edge(10, 11);

  std::vector<unsigned> flow;
  EXPECT_EQ(3, edmonds_karp_max_flow(g, 0, 11, rev_edge, capacity, flow));
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

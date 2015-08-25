//          Copyright Diego Ramírez August 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/floyd_warshall_shortest.hpp>
#include <gtest/gtest.h>

#include <djp/graph/directed_graph.hpp>
#include <djp/utility/matrix.hpp>

#include <iterator> // For std::back_inserter
#include <limits>   // For std::numeric_limits
#include <vector>   // For std::vector
#include <cstddef>  // For std::size_t

using namespace djp;
using std::vector;
using std::size_t;

static constexpr int inf = std::numeric_limits<int>::max();

namespace {
class FloydWarshallAPSPTest : public ::testing::Test {

public:
  FloydWarshallAPSPTest() : g(0) {}

  void build10VerticesGraph() {
    g = directed_graph(10);
    add_edge(0, 4, -3);
    add_edge(1, 2, 3);
    add_edge(1, 6, 3);
    add_edge(2, 3, 4);
    add_edge(2, 7, -2);
    add_edge(3, 1, -7);
    add_edge(3, 2, -3);
    add_edge(3, 7, -10);
    add_edge(4, 3, 12);
    add_edge(4, 8, 2);
    add_edge(5, 0, 2);
    add_edge(6, 6, 1);
    add_edge(7, 7, 0);
    add_edge(8, 1, 0);
    add_edge(8, 1, 2);
    add_edge(8, 5, 1);
    add_edge(9, 5, 4);
    add_edge(9, 8, 7);

    ASSERT_EQ(18, g.num_edges());
  }

  void add_edge(const size_t u, const size_t v, int w) {
    g.add_edge(u, v);
    weight.push_back(w);
  }

protected:
  directed_graph g;
  vector<int> weight;
};

} // end anonymous namespace

TEST_F(FloydWarshallAPSPTest, ShortestDistancesTest) {
  build10VerticesGraph();

  vector<vector<int>> expected(10);
  expected[0] = {0, -1, 2, 6, -3, 0, 2, -4, -1, inf};
  expected[1] = {inf, 0, 3, 7, inf, inf, 3, -3, inf, inf};
  expected[2] = {inf, -3, 0, 4, inf, inf, 0, -6, inf, inf};
  expected[3] = {inf, -7, -4, 0, inf, inf, -4, -10, inf, inf};
  expected[4] = {5, 2, 5, 9, 0, 3, 5, -1, 2, inf};

  expected[5] = {2, 1, 4, 8, -1, 0, 4, -2, 1, inf};
  expected[6] = {inf, inf, inf, inf, inf, inf, 0, inf, inf, inf};
  expected[7] = {inf, inf, inf, inf, inf, inf, inf, 0, inf, inf};
  expected[8] = {3, 0, 3, 7, 0, 1, 3, -3, 0, inf};
  expected[9] = {6, 5, 8, 12, 3, 4, 8, 2, 5, 0};

  matrix<int> dist({0, 0});
  matrix<size_t> next({0, 0});
  floyd_warshall_all_pairs_shortest_paths(g, weight, dist, next);

  for (size_t i = 0; i != 10; ++i)
    for (size_t j = 0; j != 10; ++j)
      EXPECT_EQ(expected[i][j], (dist[{i, j}])) << " at (" << i << ", " << j
                                                << ")";
}

TEST_F(FloydWarshallAPSPTest, PathTest) {
  build10VerticesGraph();
  matrix<int> dist({0, 0});
  matrix<size_t> next({0, 0});
  floyd_warshall_all_pairs_shortest_paths(g, weight, dist, next);

  // Long paths.

  vector<size_t> path;
  floyd_warshall_path(9, 7, next, back_inserter(path));
  EXPECT_EQ(vector<size_t>({9, 5, 0, 4, 8, 1, 2, 3, 7}), path);

  path.clear();
  floyd_warshall_path(4, 3, next, back_inserter(path));
  EXPECT_EQ(vector<size_t>({4, 8, 1, 2, 3}), path);

  path.clear();
  floyd_warshall_path(5, 2, next, back_inserter(path));
  EXPECT_EQ(vector<size_t>({5, 0, 4, 8, 1, 2}), path);

  path.resize(100);
  auto end = floyd_warshall_path(3, 2, next, path.begin());
  ASSERT_TRUE(end == path.begin() + 3);
  path.resize(3);
  EXPECT_EQ(vector<size_t>({3, 1, 2}), path);

  // Empty paths
  path.clear();
  floyd_warshall_path(3, 4, next, back_inserter(path));
  EXPECT_EQ(vector<size_t>(), path);

  path.clear();
  floyd_warshall_path(4, 9, next, back_inserter(path));
  EXPECT_EQ(vector<size_t>(), path);

  // Single vertex paths

  path.clear();
  floyd_warshall_path(8, 8, next, back_inserter(path));
  EXPECT_EQ(vector<size_t>({8}), path);

  path.clear();
  floyd_warshall_path(9, 9, next, back_inserter(path));
  EXPECT_EQ(vector<size_t>({9}), path);

  path.clear();
  floyd_warshall_path(7, 7, next, back_inserter(path));

  // Single edge paths
  path.clear();
  floyd_warshall_path(8, 1, next, back_inserter(path));
  EXPECT_EQ(vector<size_t>({8, 1}), path);

  path.clear();
  floyd_warshall_path(8, 5, next, back_inserter(path));
  EXPECT_EQ(vector<size_t>({8, 5}), path);
}

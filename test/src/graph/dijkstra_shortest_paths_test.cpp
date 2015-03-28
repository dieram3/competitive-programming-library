//          Copyright Diego Ramírez July 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/graph/dijkstra_shortest_paths.hpp>
#include <boost/test/unit_test.hpp>
#include <djp/graph/adjacency_list.hpp>
#include <array>

BOOST_AUTO_TEST_SUITE(graph)

BOOST_AUTO_TEST_CASE(dijkstra_shortest_paths) {
  djp::adjacency_list graph(6);

  graph.add_bidir_edge(0, 1, 7);
  graph.add_bidir_edge(0, 2, 9);
  graph.add_bidir_edge(0, 5, 14);
  graph.add_bidir_edge(1, 2, 10);
  graph.add_bidir_edge(1, 3, 15);
  graph.add_bidir_edge(2, 3, 11);
  graph.add_bidir_edge(2, 5, 2);
  graph.add_bidir_edge(3, 4, 6);
  graph.add_bidir_edge(4, 5, 9);

  auto spaths = djp::dijkstra_shortest_paths(graph, 0);

  std::vector<size_t> expected = {0, 7, 9, 20, 20, 11};

  BOOST_CHECK(expected == spaths);
}

BOOST_AUTO_TEST_SUITE_END()

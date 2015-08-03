set(GRAPH_TEST_SOURCES
  "bellman_ford_shortest_paths_test.cpp"
  "connected_components_test.cpp"
	"dijkstra_shortest_paths_test.cpp"
  "directed_graph_test.cpp"
  "edmonds_karp_max_flow_test.cpp"
  "hopcroft_karp_max_matching_test.cpp"
  "kruskal_minimum_spanning_tree_test.cpp"
	"lowest_common_ancestor_test.cpp"
  "topological_sort_test.cpp"
  "undirected_graph_test.cpp"
	)

add_unittest("graph" ${GRAPH_TEST_SOURCES})

set(GRAPH_TEST_SOURCES
  "bellman_ford_shortest_paths_test.cpp"
  "biconnected_components_test.cpp"
  "bipartite_test.cpp"
  "bridges_test.cpp"
  "connected_components_test.cpp"
	"dijkstra_shortest_paths_test.cpp"
  "directed_graph_test.cpp"
  "edmonds_karp_max_flow_test.cpp"
  "gusfield_all_pairs_min_cut_test.cpp"
  "hopcroft_karp_maximum_matching_test.cpp"
  "kruskal_minimum_spanning_tree_test.cpp"
	"lowest_common_ancestor_test.cpp"
  "min_st_cut_test.cpp"
  "strong_components_test.cpp"
  "topological_sort_test.cpp"
  "undirected_graph_test.cpp"
	)

add_unittest("graph" ${GRAPH_TEST_SOURCES})

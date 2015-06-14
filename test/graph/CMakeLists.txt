set(GRAPH_TEST_SOURCES
	"adjacency_graph_test"
	"adjacency_list_test.cpp"
	"dijkstra_shortest_paths_test.cpp"
  "edmons_karp_max_flow_test.cpp"
	"lowest_common_ancestor_test.cpp"
	)

add_unittest("graph" ${GRAPH_TEST_SOURCES})

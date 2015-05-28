set(GRAPH_TEST_SOURCES
	"adjacency_list_test.cpp"
	"dijkstra_shortest_paths_test.cpp"
	"lowest_common_ancestor_test.cpp"
	)

add_unittest("graph" ${GRAPH_TEST_SOURCES})

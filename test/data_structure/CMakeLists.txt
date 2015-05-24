set(DATA_STRUCTURE_TEST_SOURCES
	"disjoint_set_test.cpp"
	"fenwick_tree_test.cpp"
	"segment_tree_test.cpp"
	)

add_unittest("data_structure" ${DATA_STRUCTURE_TEST_SOURCES})

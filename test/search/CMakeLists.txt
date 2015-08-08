set(SEARCH_TEST_SOURCES
  "binary_search_test.cpp"
  "longest_increasing_subsequence_test.cpp"
	"knuth_morris_pratt_test.cpp"
	)

add_unittest("search" ${SEARCH_TEST_SOURCES})

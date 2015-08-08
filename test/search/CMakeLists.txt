set(SEARCH_TEST_SOURCES
  "longest_increasing_subsequence_test.cpp"
	"knuth_morris_pratt_test.cpp"
	)

add_unittest("search" ${SEARCH_TEST_SOURCES})

set(STRINGS_TEST_SOURCES
	"basics_test.cpp"
	"edit_distance_test.cpp"
  "lcp_array_test.cpp"
	"suffix_array_test.cpp"
	"z_function_test.cpp"
	)

add_unittest("strings" ${STRINGS_TEST_SOURCES})

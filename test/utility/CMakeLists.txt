set(UTILITY_TEST_SOURCES
  "basics_test.cpp"
	"matrix_test.cpp"
	)

add_unittest("utility" ${UTILITY_TEST_SOURCES})

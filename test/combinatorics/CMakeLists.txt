set(COMBINATORICS_TEST_SOURCES
  "basics_test.cpp"
  "binomial_coefficient_test.cpp"
  )

add_unittest("combinatorics" ${COMBINATORICS_TEST_SOURCES})

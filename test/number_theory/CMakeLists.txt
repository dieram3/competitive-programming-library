set(NUMBER_THEORY_TEST_SOURCES
	"basics_test.cpp"
  "divisor_test.cpp"
  "eratosthenes_test.cpp"
  "miller_rabin_test.cpp"
	"modular_test.cpp"
	)

add_unittest("number_theory" ${NUMBER_THEORY_TEST_SOURCES})

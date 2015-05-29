set(NUMBER_THEORY_TEST_SOURCES
	"common_factor_test.cpp"
	"modular_test.cpp"
	"primes_test.cpp"
	)

add_unittest("number_theory" ${NUMBER_THEORY_TEST_SOURCES})

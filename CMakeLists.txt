enable_testing()
include_directories("include")

file(GLOB_RECURSE TEST_SOURCES test/*.cpp)
list(REMOVE_ITEM TEST_SOURCES "test/main")
list(APPEND BII_test_main_SRC ${TEST_SOURCES})

ADD_BII_TARGETS()

TARGET_COMPILE_OPTIONS(${BII_BLOCK_TARGET} INTERFACE "-std=c++11")


#search Boost

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(BOOST_REQUIRED_COMPONENTS unit_test_framework)

find_package(Boost 1.53.0 REQUIRED COMPONENTS unit_test_framework)

#create test

set(TEST_SOURCES
"src/main"
"src/container/segment_tree_test"
"src/math/common_factor_test"
"src/math/modular_test"
"src/graph/adjacency_list_test"
"src/graph/dijkstra_shortest_paths_test"
"src/string/basics_test"
)

add_executable(djp-test ${TEST_SOURCES})

target_include_directories(djp-test PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(djp-test ${Boost_LIBRARIES})

set(GEOMETRY_TEST_SOURCES
	"convex_hull_test.cpp"
  "point_2d_test.cpp"
  "point_order_test.cpp"
	)

add_unittest("geometry" ${GEOMETRY_TEST_SOURCES})

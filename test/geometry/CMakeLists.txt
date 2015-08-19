set(GEOMETRY_TEST_SOURCES
	"convex_hull_test.cpp"
    "point_2d_test.cpp"
    "sort_points_test.cpp"
	)

add_unittest("geometry" ${GEOMETRY_TEST_SOURCES})

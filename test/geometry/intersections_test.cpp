//          Copyright Diego Ramirez 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cpl/geometry/intersections.hpp>
#include <gtest/gtest.h>

#include <cpl/geometry/point_2d.hpp> // point_2d

namespace {
class IntersectionsTest : public ::testing::Test {
protected:
  using point = cpl::point2d<float>;

  struct line {
    point p0, p1;
  };

  static point find_intersection(const line& l0, const line& l1) {
    return cpl::find_lines_intersection(l0.p0, l0.p1, l1.p0, l1.p1);
  }

  static void check_equal(const point& expected, const point& actual) {
    ASSERT_FLOAT_EQ(expected.x, actual.x) << "expected: " << expected
                                          << ", actual: " << actual;
    ASSERT_FLOAT_EQ(expected.y, actual.y) << "expected: " << expected
                                          << ", actual: " << actual;
  }
};
} // end anonymous namespace.

TEST_F(IntersectionsTest, FindLinesIntersectionTest) {
  {
    const point p0{1, 4}, p1{5, 6};
    const point q0{4, 1}, q1{2, 9};
    const point i{3, 5};

    SCOPED_TRACE("First test");
    check_equal(i, cpl::find_lines_intersection(p0, p1, q0, q1));
    check_equal(i, cpl::find_lines_intersection(p0, p1, q1, q0));
    check_equal(i, cpl::find_lines_intersection(p1, p0, q0, q1));
    check_equal(i, cpl::find_lines_intersection(p1, p0, q1, q0));
  }

  const auto a = line{{-3, 1}, {4, 1}};
  const auto b = line{{-1, -2}, {-1, 5}};
  const auto c = line{{4, 4}, {1, 3}};
  const auto d = line{{-2, 5}, {3, 0}};

  check_equal(point(-1, 1), find_intersection(a, b));
  check_equal(point(-5, 1), find_intersection(a, c));
  check_equal(point(2, 1), find_intersection(a, d));

  check_equal(point(-1, 1), find_intersection(b, a));
  check_equal(point(-1, 7.0f / 3), find_intersection(b, c));
  check_equal(point(-1, 4), find_intersection(b, d));

  check_equal(point(-5, 1), find_intersection(c, a));
  check_equal(point(-1, 7.0f / 3), find_intersection(c, b));
  check_equal(point(0.25f, 2.75f), find_intersection(c, d));

  check_equal(point(2, 1), find_intersection(d, a));
  check_equal(point(-1, 4), find_intersection(d, b));
  check_equal(point(0.25f, 2.75f), find_intersection(d, c));
}


#include <djp/geometry/segment_intersection.hpp>
#include <gtest/gtest.h>

#include <djp/geometry/point_2d.hpp>

#include <stdexcept> // For std::domain_error
#include <set>       // For std::set
#include <vector>    // For std::vector
#include <cstdint>   // For std::int32_t

using namespace djp;
using point_i = point<int32_t>;
using segment_i = segment<point_i>;
using segment_set = std::vector<segment_i>;

static bool operator==(const segment_i &s0, const segment_i &s1) {
  return s0.a == s1.a && s0.b == s1.b;
}

static void test_intersection(const segment_set &set, const segment_i &s0,
                              const segment_i &s1) {
  auto intersection = find_intersection(set.begin(), set.end());
  EXPECT_TRUE(*intersection.first == s0);
  EXPECT_TRUE(*intersection.second == s1);
}

static segment_i seg(int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
  return {{x0, y0}, {x1, y1}};
}

TEST(SegmentIntersection, SequentialSegments) {
  // vertical lines
  segment_set test_set = {seg(0, 0, 0, 1),    seg(0, 1, 0, 2),
                          seg(1, 0, 1, 1),    seg(2, 2, 3, 3),
                          seg(-1, -1, 1, -1), seg(-1, 0, -1, 3),
                          seg(-2, 0, -2, 1),  seg(-3, 2, -2, 2)};

  test_intersection(test_set, test_set[1], test_set[0]);
  // horizontal line
  test_set = {seg(0, 0, 1, 0),   seg(1, 0, 2, 0),  seg(0, 1, 1, 1),
              seg(1, -1, 2, -1), seg(3, -1, 3, 2), seg(-2, -1, -2, 2)};

  test_intersection(test_set, test_set[1], test_set[0]);
  // diagonal line
  test_set = {seg(0, 0, 1, 1), seg(1, 1, 2, 2), seg(0, 1, 1, 2),
              seg(1, -1, 2, 0), seg(3, -3, 3, 4)};

  test_intersection(test_set, test_set[1], test_set[0]);
  // perpendicular lines
  test_set = {seg(0, 0, 0, 1), seg(0, 1, 1, 1), seg(-1, -1, 2, -1),
              seg(-1, 1, -1, 0)};

  test_intersection(test_set, test_set[1], test_set[0]);
}

TEST(SegmentIntersection, OverlapedSegments) {
  // vertical lines
  segment_set test_set = {seg(0, 0, 0, 2),    seg(0, 1, 0, 3),
                          seg(1, 0, 1, 1),    seg(2, 2, 3, 3),
                          seg(-1, -1, 1, -1), seg(-1, 0, -1, 3),
                          seg(-2, 0, -2, 1),  seg(-3, 2, -2, 2)};

  test_intersection(test_set, test_set[1], test_set[0]);
  // horizontal line
  test_set = {seg(0, 0, 2, 0),   seg(1, 0, 5, 0),  seg(0, 1, 1, 1),
              seg(1, -1, 2, -1), seg(3, -1, 3, 2), seg(-2, -1, -2, 2)};
  test_intersection(test_set, test_set[1], test_set[0]);
  // diagonal line
  test_set = {seg(0, 0, 2, 2), seg(1, 1, 3, 3), seg(0, 1, 1, 2),
              seg(1, -1, 2, 0), seg(3, -3, 3, 4)};

  test_intersection(test_set, test_set[1], test_set[0]);
  // perpendicular lines
  test_set = {seg(0, 0, 0, 2), seg(0, 1, 1, 1), seg(-1, -1, 2, -1),
              seg(-1, 1, -1, 0)};

  test_intersection(test_set, test_set[1], test_set[0]);
}

// ==========================
// SimplePolygonTest
// ==========================

namespace {
class SimplePolygonTest : public ::testing::Test {

protected:
  using scalar_t = int;
  using point_t = point<scalar_t>;
  using segment_t = segment<point_t>;

  std::vector<point_t> points;

  bool has_repeated_points() const {
    std::set<point_t> set;
    for (const auto &p : points) {
      const bool inserted = set.insert(p).second;
      EXPECT_TRUE(inserted) << " Point " << p << " is repeated";
      if (!inserted)
        return true;
    }
    return false;
  }

  // Adds a point to the end of the list.
  void add(scalar_t x, scalar_t y) { points.emplace_back(x, y); }

  // Adds a point to the end of the list relative to the last point.
  void rel_add(scalar_t x_delta, scalar_t y_delta) {
    const auto prev_x = points.back().x;
    const auto prev_y = points.back().y;
    add(prev_x + x_delta, prev_y + y_delta);
  }

  // Removes the last inserted point.
  void pop_back() { points.pop_back(); }

  bool is_simple() {
    if (points.size() < 3 || has_repeated_points())
      throw std::domain_error("Invalid polygon");

    std::vector<segment_t> segments;
    segments.reserve(points.size());
    for (size_t i = 0; i + 1 != points.size(); ++i)
      segments.emplace_back(points[i], points[i + 1]);
    segments.emplace_back(points.back(), points.front());
    assert(segments.size() == points.size());
    return simple_polygon(segments.begin(), segments.end());
  }
};
} // End anonymous namespace

TEST_F(SimplePolygonTest, SquareTest) {
  add(0, 0);
  add(0, 2);
  add(2, 2);
  add(2, 0);
  EXPECT_TRUE(is_simple());
}

TEST_F(SimplePolygonTest, CrossingSquaresTest) {
  add(0, 1), add(1, 1), add(1, 0);
  EXPECT_TRUE(is_simple());
  add(0, 0);
  EXPECT_TRUE(is_simple());
  pop_back();
  add(-1, 0);
  EXPECT_TRUE(is_simple());
  add(-1, -1);
  EXPECT_FALSE(is_simple());
  add(0, -1);
  EXPECT_FALSE(is_simple());
  pop_back();
  add(1, -1);
  EXPECT_FALSE(is_simple());
  pop_back();
  add(2, -1);
  EXPECT_FALSE(is_simple());
  add(2, 0);
  EXPECT_FALSE(is_simple());
  pop_back();
  add(2, 2);
  EXPECT_TRUE(is_simple());
  add(-1, 2);
  EXPECT_TRUE(is_simple());
}

TEST_F(SimplePolygonTest, OneIntersectionTest) {
  add(0, 0);
  rel_add(3, -2), rel_add(1, 3), rel_add(-2, 1);
  rel_add(2, -5), rel_add(-2, -1), rel_add(1, 1);
  ASSERT_EQ(7, points.size());
  ASSERT_EQ(point_t(3, -3), points.back());
  EXPECT_FALSE(is_simple());

  points.clear();
  add(0, 0);
  rel_add(1, 3), rel_add(3, 0), rel_add(1, -3);
  rel_add(-2, -3), rel_add(-2, 0), rel_add(0, 5);
  EXPECT_TRUE(is_simple());
  rel_add(3, 0);
  rel_add(0, -2);
  EXPECT_FALSE(is_simple());
  rel_add(-2, 0);
  rel_add(0, -1);
  EXPECT_FALSE(is_simple());

  EXPECT_EQ(point_t(2, -1), points.back());
  EXPECT_EQ(11, points.size());
}

TEST_F(SimplePolygonTest, MultipleIntersecionsTest) {
  add(0, 0);
  rel_add(0, 3), rel_add(2, -2), rel_add(0, 2), rel_add(1, -2);
  rel_add(2, 0), rel_add(-2, -2), rel_add(2, 1), rel_add(-2, -2);
  EXPECT_TRUE(is_simple());
  rel_add(-2, 5);
  EXPECT_FALSE(is_simple());
  rel_add(-2, -2);
  rel_add(2, -1);
  EXPECT_FALSE(is_simple());

  EXPECT_EQ(point_t(1, 0), points.back());
  EXPECT_EQ(12, points.size());

  points.clear();
  add(0, 0);
  rel_add(1, 1), rel_add(1, -1), rel_add(-2, -2), rel_add(2, -2);
  rel_add(-1, -1), rel_add(-1, 1), rel_add(2, 2);
  EXPECT_EQ(point_t(2, -2), points.back());
  EXPECT_EQ(8, points.size());
  EXPECT_FALSE(is_simple());
}

TEST_F(SimplePolygonTest, NoIntersectionTest) {
  add(0, 0);
  rel_add(0, 3), rel_add(1, -2), rel_add(1, 1), rel_add(0, -3);
  rel_add(-1, 1), rel_add(0, -1), rel_add(1, -1), rel_add(1, 1);
  rel_add(0, 3), rel_add(-1, 1), rel_add(-1, -1), rel_add(0, 1);
  rel_add(1, 1), rel_add(2, -2), rel_add(0, -3), rel_add(-2, -2);
  rel_add(-3, 3), rel_add(0, 3);

  ASSERT_EQ(point_t(-1, 3), points.back());
  ASSERT_EQ(19, points.size());
  EXPECT_TRUE(is_simple());

  points.clear();
  add(0, 0);
  rel_add(0, 3), rel_add(1, -2), rel_add(-1, -2), rel_add(-1, 5);
  ASSERT_EQ(point_t(-1, 4), points.back());
  ASSERT_EQ(5, points.size());
  EXPECT_TRUE(is_simple());
}

TEST_F(SimplePolygonTest, TangentPointTest) {
  add(0, 0);
  rel_add(2, 2), rel_add(0, -3), rel_add(2, 2), rel_add(-4, 2);
  ASSERT_EQ(point_t(0, 3), points.back());
  ASSERT_EQ(5, points.size());
  EXPECT_FALSE(is_simple());

  points.clear();
  add(0, 0);
  rel_add(2, 2), rel_add(0, -3), rel_add(2, 3), rel_add(0, -4);
  rel_add(2, 0), rel_add(0, -1);
  ASSERT_EQ(point_t(6, -3), points.back());
  ASSERT_EQ(7, points.size());
  EXPECT_FALSE(is_simple());
}

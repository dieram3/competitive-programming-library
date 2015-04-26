#include <tuple>

#include <djp/geometry/convex_hull.hpp>
#include <gtest/gtest.h>

template <class T>
struct point {
  T x, y, z;

  point(T x = 0, T y = 0, T z = 0) : x{x}, y{y}, z{z} {}

  friend bool operator<(const point &p, const point &q) {
    return std::make_tuple(p.x, p.y, p.z) < std::make_tuple(q.x, q.y, q.z);
  }

  friend point operator-(const point &p, const point &q) {
    return {(p.x - q.x), (p.y - q.y), (p.z - q.z)};
  }

  friend point cross(const point &p, const point &q) {
    return {(p.y * q.z - p.z * q.y), (q.z * p.x - p.x * q.z),
            (p.x * q.y - p.y * q.x)};
  }
};

template <class ForwardIt>
bool is_ccw_sorted(ForwardIt first, ForwardIt last) {
  using point_t = typename std::iterator_traits<ForwardIt>::value_type;
  std::iter_swap(first, std::min_element(first, last));

  auto center = *first++;
  return std::is_sorted(first, last,
                        [&center](const point_t &lhs, const point_t &rhs) {
                          auto det = cross(lhs - center, rhs - center).z;
                          return det > 0;
                        });
}

TEST(convex_hull, SortsPointsInclockwise) {
  using scalar_t = int32_t;
  using point_t = point<scalar_t>;
  using vector_t = std::vector<point_t>;

  auto clockwise =
      [](const point_t &p, const point_t &q, const point_t &r) -> bool {
        return cross(q - p, r - p).z < 0;
      };

  vector_t points;
  points.emplace_back(1, 1);
  points.emplace_back(1, -1);
  points.emplace_back(-1, -1);
  points.emplace_back(-1, 1);

  std::sort(std::begin(points), std::end(points));

  auto hull = djp::convex_hull(std::begin(points), std::end(points), clockwise);
  EXPECT_EQ(is_ccw_sorted(std::begin(hull), std::end(hull)), true);

  points.erase(std::begin(points),
               djp::convex_hull_partition(std::begin(points), std::end(points),
                                          clockwise));

  EXPECT_EQ(hull.size(), points.size());

  for (auto i = 0u; i < points.size(); ++i) {
    EXPECT_EQ(hull[i].x, points[i].x);
    EXPECT_EQ(hull[i].y, points[i].y);
  }

  EXPECT_EQ(is_ccw_sorted(std::begin(points), std::end(points)), true);
}

TEST(convex_hull, withColinealPoints) {
  using scalar_t = int32_t;
  using point_t = point<scalar_t>;
  using vector_t = std::vector<point_t>;

  auto clockwise =
      [](const point_t &p, const point_t &q, const point_t &r) -> bool {
        return cross(q - p, r - p).z < 0;
      };

  vector_t points;
  // Square
  points.emplace_back(0, 0);
  points.emplace_back(0, 5);
  points.emplace_back(5, 5);
  points.emplace_back(5, 0);

  // Colineal points
  points.emplace_back(0, 2);
  points.emplace_back(2, 0);
  points.emplace_back(2, 5);
  points.emplace_back(5, 2);

  // Inner points
  points.emplace_back(1, 1);
  points.emplace_back(2, 2);
  points.emplace_back(3, 3);
  points.emplace_back(4, 4);
  points.emplace_back(1, 2);
  points.emplace_back(3, 2);

  std::sort(std::begin(points), std::end(points));

  auto hull = djp::convex_hull(std::begin(points), std::end(points), clockwise);

  points.erase(std::begin(points),
               djp::convex_hull_partition(std::begin(points), std::end(points),
                                          clockwise));

  const size_t expected_len = 8;
  EXPECT_EQ(hull.size(), expected_len);
  EXPECT_EQ(points.size(), expected_len);

  vector_t result;
  result.emplace_back(0, 0);
  result.emplace_back(2, 0);
  result.emplace_back(5, 0);
  result.emplace_back(5, 2);
  result.emplace_back(5, 5);
  result.emplace_back(2, 5);
  result.emplace_back(0, 5);
  result.emplace_back(0, 2);

  for (size_t i = 0; i < expected_len; ++i) {
    EXPECT_EQ(hull[i].x, points[i].x);
    EXPECT_EQ(hull[i].y, points[i].y);
    EXPECT_EQ(hull[i].x, result[i].x);
    EXPECT_EQ(hull[i].y, result[i].y);
  }
}

TEST(convex_hull, withoutColinealPoints) {
  using scalar_t = int32_t;
  using point_t = point<scalar_t>;
  using vector_t = std::vector<point_t>;

  auto clockwise =
      [](const point_t &p, const point_t &q, const point_t &r) -> bool {
        return cross(q - p, r - p).z <= 0;
      };

  vector_t points;
  // Square
  points.emplace_back(0, 0);
  points.emplace_back(0, 5);
  points.emplace_back(5, 5);
  points.emplace_back(5, 0);

  // Colineal points
  points.emplace_back(0, 2);
  points.emplace_back(2, 0);
  points.emplace_back(2, 5);
  points.emplace_back(5, 2);

  // Inner points
  points.emplace_back(1, 1);
  points.emplace_back(2, 2);
  points.emplace_back(3, 3);
  points.emplace_back(4, 4);
  points.emplace_back(1, 2);
  points.emplace_back(3, 2);

  std::sort(std::begin(points), std::end(points));

  auto hull = djp::convex_hull(std::begin(points), std::end(points), clockwise);

  points.erase(std::begin(points),
               djp::convex_hull_partition(std::begin(points), std::end(points),
                                          clockwise));

  const size_t expected_len = 4;
  EXPECT_EQ(hull.size(), expected_len);
  EXPECT_EQ(points.size(), expected_len);

  vector_t result;
  result.emplace_back(0, 0);
  result.emplace_back(5, 0);
  result.emplace_back(5, 5);
  result.emplace_back(0, 5);

  for (size_t i = 0; i < expected_len; ++i) {
    EXPECT_EQ(hull[i].x, points[i].x);
    EXPECT_EQ(hull[i].y, points[i].y);
    EXPECT_EQ(hull[i].x, result[i].x);
    EXPECT_EQ(hull[i].y, result[i].y);
  }
}

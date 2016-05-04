//          Copyright Jorge Aguirre 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPL_GEOMETRY_POINT_ORDER_HPP
#define CPL_GEOMETRY_POINT_ORDER_HPP

namespace cpl {

/// \brief Check if two points are in clockwise order according to a
/// center and a start angle.
///
/// \param center The reference point used as a center.
/// \param start Reference point relative to the origin used to specify the
/// start angle.
/// \param lhs The left-hand side point.
/// \param rhs The right-hand side point.
///
/// \returns \c true if \p lhs is clockwise less than \p rhs, \c false
/// otherwise.
///
/// \pre <tt>lhs != center && rhs != center && start != center</tt>, otherwise
/// the order is undetermined.
///
/// \par Complexity
/// Constant.
///
template <typename Point>
bool cw_less(const Point& center, const Point& start, const Point& lhs,
             const Point& rhs) {
  const auto u = lhs - center;
  const auto v = rhs - center;

  const auto lhs_rotation = (start ^ u);
  const auto rhs_rotation = (start ^ v);

  if (lhs_rotation <= 0 && rhs_rotation > 0)
    return true;
  if (lhs_rotation > 0 && rhs_rotation <= 0)
    return false;

  if (lhs_rotation == 0 && rhs_rotation == 0) {
    // rotate 90 degrees in clockwise order
    const Point rotated_start = {start.y, -start.x};
    return (rotated_start ^ u) > 0 && (rotated_start ^ v) < 0;
  }

  return (u ^ v) < 0;
}

/// \brief Check if two points are in counter clockwise order according to a
/// center and a start angle.
///
/// \param center The reference point used as a center.
/// \param start Reference point relative to the origin used to specify the
/// start angle.
/// \param lhs The left-hand side point.
/// \param rhs The right-hand side point.
///
/// \returns \c true if \p lhs is counter clockwise less than \p rhs, \c false
/// otherwise.
///
/// \pre <tt>lhs != center && rhs != center && start != center</tt>, otherwise
/// the order is undetermined.
///
/// \par Complexity
/// Constant.
///
template <typename Point>
bool ccw_less(const Point& center, const Point& start, const Point& lhs,
              const Point& rhs) {
  const auto u = lhs - center;
  const auto v = rhs - center;

  const auto lhs_rotation = (start ^ u);
  const auto rhs_rotation = (start ^ v);

  if (lhs_rotation >= 0 && rhs_rotation < 0)
    return true;
  if (lhs_rotation < 0 && rhs_rotation >= 0)
    return false;

  if (lhs_rotation == 0 && rhs_rotation == 0) {
    // rotate 90 degrees in clockwise order
    const Point rotated_start = {start.y, -start.x};
    return (rotated_start ^ u) > 0 && (rotated_start ^ v) < 0;
  }

  return (u ^ v) > 0;
}

} // end namespace cpl

#endif // Header guard

#include "geometry/geo_line.h"

#include <cmath>
#include <limits>

namespace sg
{
namespace geometry
{
Line_T::Line_T(const geometry::Point2D_T &point1, const geometry::Point2D_T &point2)
{
   this->a = point1.y - point2.y;
   this->b = point2.x - point1.x;
   this->c = -b * point1.y - a * point1.x;
}

Point2D_T Line_T::intersection(const Line_T &other_line) const
{
   const auto a1 = a;
   const auto b1 = b;
   const auto c1 = c;

   (void)c1; // MISRA

   const auto a2 = other_line.a;
   const auto b2 = other_line.b;
   const auto c2 = other_line.c;

   (void)c2; // MISRA

   const auto denominator_x = a2 * b1 - a1 * b2;
   const auto denominator_y = a1 * b2 - a2 * b1;

   (void)denominator_y; // MISRA

   float x0 = 0.0F;
   float y0 = 0.0F;

   if ((std::abs(denominator_x) > std::numeric_limits<float>::epsilon()) &&
       (std::abs(denominator_y) > std::numeric_limits<float>::epsilon()))
   {
      x0 = (b2 * c1 - b1 * c2) / denominator_x;
      y0 = (a2 * c1 - c2 * a1) / denominator_y;
   }
   return Point2D_T{x0, y0};
}
} // namespace geometry
} // namespace sg

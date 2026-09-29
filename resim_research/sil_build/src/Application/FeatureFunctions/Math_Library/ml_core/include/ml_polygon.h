#ifndef ML_POLYGON_H
#define ML_POLYGON_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "reuse.h"
#include "ml_vector_2d_t.h"
/**
* \defgroup polygon Polygon
* \brief Utility functions for using [polygons](https:\\en.wikipedia.org/wiki/Polygon).
* The polygons are represented as arrays of type Vector_2d_T. All functions working on polygons
* have a parameter num_polygon_corners that represents the number of Vector_2d_T in this array.
*/

/**
* Evaluates if a point is inside a polygon.
* How it works: If the polygon is convex then one can consider the polygon as a "path"
* from the first vertex. A point is on the interior of this polygons if it is always on
* the same side of all the line segments making up the path.
* IMP NOTE: This works ONLY with convex polygons.
* \return TRUE when given p_point is inside the polygon
* \ingroup polygon
* \sdd{WI-13985}
*/
boolean_T Is_Point_In_Polygon(
   const Vector_2d_T *p_polygon,           /**< [in] Pointer to array of points that form a polygon */
   const uint8_t      num_polygon_corners, /**< [in] size of polygon */
   const Vector_2d_T *p_point /**< [in] point to be checked */
);

/**
* This function evaluates if a given point is within a polygon based on the Ray Casting Method .
* Theory: Test how many times a ray, starting from the point and going in any fixed direction,
* intersects the edges of the polygon. If the point is on the outside of the polygon the ray
* will intersect its edge an even number of times. If the point is on the inside of the polygon
* then it will intersect the edge an odd number of times.
* IMP NOTES:
* 1. If the point is on the edge of the polygon, the result is non determinable. Based on which side the
*    point lies and whether the point is a convex/concave polygon, the function may return either TRUE or FALSE.
* 2. It works well when the point is inside the polygon (both convex and concave)
*
* The code is adapted from PNPOLY - Point Inclusion in Polygon Test written by W. Randolph Franklin (WRF)
* Copyright (c) 1970-2003, Wm. Randolph Franklin
* https:\\wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
*
* \return TRUE when point is inside the polygon
* \ingroup polygon
* \sdd{WI-13984}
*/
boolean_T Is_Point_In_Convex_Polygon_Ray_Casting_Method(
   const Vector_2d_T *p_polygon,           /**< [in] Pointer to array of points that form a polygon */
   const uint8_t      num_polygon_corners, /**< [in] size of polygon */
   const Vector_2d_T *p_point /**< [in] point to be checked */
);
#ifdef __cplusplus
}
#endif
#endif

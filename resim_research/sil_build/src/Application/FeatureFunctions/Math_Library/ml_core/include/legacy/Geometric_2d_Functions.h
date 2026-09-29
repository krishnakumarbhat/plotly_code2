#ifndef GEOMETRIC_2D_FUNCTIONS_H
#define GEOMETRIC_2D_FUNCTIONS_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "Vector_2d.h"
#include "reuse.h"
#include "Geometric_2d_Structs.h"

#include "st_line_parameter.h"
#include "st_line.h"
#include "st_polygon.h"

/* Since this file defines a bunch of function-like macros for backwards compatibility
* the QAC check "A function could probably be used instead of this function-like macro."
* Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

/**
* Used for backwards compatibility. Use Is_Point_In_Convex_Polygon_Ray_Casting_Method instead.
* \ingroup polygon
*/
#define Is_Point_In_Polygon_Ray_Casting_Method(a, b, c) (Is_Point_In_Convex_Polygon_Ray_Casting_Method(a, b, c))

/**
* Evaluates on which side of the line the point is
* \return Line_Side_T determining on which side of the given line the given point is located
* \ingroup line_normal
* This functionality has been deprecated! Use Line_Hesse_Get_Side_of_Point() in \ref line_hesse instead!
*/
Line_Side_T Get_Side_of_Point(
   const Line_Normal_T *p_line, /**< Line to find out on which side the given point is */
   const Vector_2d_T   *p_point /**< Point to be checked */
);

#ifdef __cplusplus
}
#endif
#endif


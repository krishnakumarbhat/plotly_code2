#ifndef ML_LINE_H
#define ML_LINE_H
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
* \defgroup line Line Utilities
* \brief Utilities to handle mathematical lines in various representations.
*/
/**
* calculate tangent with given two points
* \return slope of tangent with given two points
* \ingroup line
* \sdd{WI-13983}
*/
float32_T Get_Slope_Of_Line(
   const Vector_2d_T * const p_point0 /**< [in] input point 1 in 2-dimensional Cartesian coordinate system*/,
   const Vector_2d_T * const p_point1 /**< [in] input point 2 in 2-dimensional Cartesian coordinate system*/
);

/**
* Calculate y-value of a linear function defined by two 2-d points
* \return  y-value of a linear function defined by two 2-d points
* \ingroup line
* \sdd{WI-13982}
*/
float32_T Get_Y_Value_From_Line_Defined_By_2_Points(
   const Vector_2d_T* p_point0, /**< [in] input point 1 in 2-dimensional Cartesian coordinate system*/
   const Vector_2d_T* p_point1, /**< [in] input point 2 in 2-dimensional Cartesian coordinate system*/
   const float32_T x      /**< [in] x-value for the y-value calculation*/
);

/**
* Returns the y value corresponding to the given x of a line defined by the points (x1, y1) and (x2, y2)
* \return the y value corresponding to the given x of a line defined by the points (x1, y1) and (x2, y2)
* \ingroup line
* \sdd{WI-13981}
*/
float32_T Get_Y_Value_From_Line_By_Coordinates(
   const float32_T x1, /**< [in] x value of first point defining the line */
   const float32_T y1, /**< [in] y value of first point defining the line  */
   const float32_T x2, /**< [in] x value of second point defining the line  */
   const float32_T y2, /**< [in] y value of second point defining the line  */
   const float32_T x /**< [in] x value to return the y value for*/
);

#ifdef __cplusplus
}
#endif
#endif

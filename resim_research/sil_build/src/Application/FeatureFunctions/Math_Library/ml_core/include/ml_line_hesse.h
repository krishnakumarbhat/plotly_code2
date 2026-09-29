/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#ifndef ML_LINE_HESSE_H
#define ML_LINE_HESSE_H

#include "ml_line_hesse_t.h"
#include "ml_line_parameter_t.h"
#include "ml_line_segment_t.h"
#include "ml_vector_2d_t.h"

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * \defgroup line_hesse Hesse Line Utilities
 * \brief Utilities to handle lines in [Hesse normal form](https:\\en.wikipedia.org/wiki/Hesse_normal_form)
 * \ingroup line
 */

/**
*  Returns a Hesse normal form of a line using a point on the line and a unit vector parallel/along the line
* \return         Line_Hesse_T line
* \ingroup line_hesse
* \sdd{WI-13951}
*/
Line_Hesse_T Line_Hesse_Create(
   const float        distance_to_origin, /**< [in] Distance from line to origin */
   const Vector_2d_T *p_norm_vector       /**< [in] unit vector perpendicular to the line */
   );

/**
 *  Returns a Hesse normal form of a line using a point on the line and a unit vector parallel/along the line.
 * The normal vector is rotated counter clockwise from the given unit vector.
 * \return         Line_Hesse_T line
 * \ingroup line_hesse
 * \sdd{WI-13976}
 */
Line_Hesse_T Line_Hesse_Create_Using_Point_And_Unit_Vector(
   const Vector_2d_T *p_point,      /**< [in] a point on the line */
   const Vector_2d_T *p_unit_vector /**< [in] unit vector parallel/along the line */
   );

/**
 *  Returns a Hesse normal form of a line using a point on the line and a unit vector perpendicular to the line
 * \return         Line_Hesse_T line
 * \ingroup line_hesse
 * \sdd{WI-13974}
 */
Line_Hesse_T Line_Hesse_Create_Using_Point_And_Normal_Vector(
   const Vector_2d_T *p_point,        /**< [in] a point on the line */
   const Vector_2d_T *p_normal_vector /**< [in] unit perpendicular the line */
   );

/**
 *  Returns a Hesse normal form of a line passing through two given points
 * \return         Line_Hesse_T line
 * \ingroup line_hesse
 * \sdd{WI-13952}
 */
Line_Hesse_T Line_Hesse_Create_Using_Two_Points(
   const Vector_2d_T *p_point1, /**< [in] a point on the line */
   const Vector_2d_T *p_point2  /**< [in] another point on the same line */
   );

/**
 *  Returns a Hesse normal form equivalent to given line segment
 * \return         Line_Hesse_T line
 * \ingroup line_hesse
 * \sdd{WI-13978}
 */
Line_Hesse_T Line_Hesse_Create_Using_Line_Segment(const Line_Segment_T *p_segment /**< [in] segment to create a line in Hesse normal form from */);

/**
 *  Returns a Hesse normal form equivalent to given line in parameter form.
 * The normal vector is rotated counter clockwise from the given Line_Parameter_T direction.
 * \return         Line_Hesse_T line
 * \ingroup line_hesse
 * \sdd{WI-13970}
 */
Line_Hesse_T Line_Hesse_Create_Using_Line_Parameter(const Line_Parameter_T *p_line_param /**< [in] Line in parameter form to create a line in Hesse normal form from */);




/**
 * Finds the distance of a given point (x,y) from a given line
 * - The value will be 0 if the point (x,y) lies on the line
 * - The value will be positive/negative based on the position of point (x,y).
 * - If the point lies in the same direction as normal vector(of the line), then the value will be positive.
 * - If the point lies in the opposite direction as normal vector(of the line), then the value will be negative.
 * \return  distance
 * \ingroup line_hesse
 * \sdd{WI-13965}
 */
float Line_Hesse_Get_Distance_Of_Point(
   const Vector_2d_T  *p_point, /**< [in] a point */
   const Line_Hesse_T *p_line   /**< [in] line contains values of norm vector and distance to origin */
   );

/**
 * Returns a Line_Hesse_T based on given line parameter. The given line is adjusted so that the given point lies on the given side_of_line.
 * \return         Line_Hesse_T updated_line
 * \throws Assertion if given side_of_line is LINE_HESSE_SIDE_ON_LINE.
 * \ingroup line_hesse
 * \sdd{WI-13975}
 */
Line_Hesse_T Line_Hesse_Update_Line_Normal_Direction(
   const Vector_2d_T         *p_point,     /**< [in] a point on either side of line */
   const Line_Hesse_T        *p_line,      /**< [in] line contains values of norm vector and distance to origin */
   const Side_Of_Line_Hesse_T side_of_line /**< [in] The side given point shall be relative to given line. Cannot be LINE_HESSE_SIDE_ON_LINE */
   );

/**
 * Finds out on which side of the given line the given point lies.
 * \return the side of the line on which the given point lies
 * \ingroup line_hesse
 * \sdd{WI-13967}
 */
Side_Of_Line_Hesse_T Line_Hesse_Get_Side_of_Point(
   const Line_Hesse_T *p_line,  /**< [in] line contains values of norm vector and distance to origin */
   const Vector_2d_T  *p_point, /**< [in] a point on either side of line */
   const float margin           /**< [in] max distance to line to consider p_point on p_line */
   );

/**
 * Creates a line using a point on the line and angle made by the line to the x-axis.
 * \return         Line_Hesse_T line
 * \ingroup line_hesse
 * \sdd{WI-13959}
 */
Line_Hesse_T Line_Hesse_Create_Using_Azimuth_And_Point(
   const Vector_2d_T *p_point_on_line, /**<  [in]Point on the Line_Hesse_T to be created */
   const float        azimuth_of_line  /**<  [in]Azimuth angle relative to x axis */
   );

/**
* Returns a vector perpendicular to the given lines orientation
* \return         Vector perpendicular to the given lines orientation
* \ingroup line_hesse
* \sdd{WI-13969}
*/
Vector_2d_T Line_Hesse_Get_Norm_Vector(
   const Line_Hesse_T *p_line /**< [in] Line to get the norm vector from */
   );

/**
* Returns a vector in the given lines orientation
* \return         Vector in the given lines orientation
* \ingroup line_hesse
* \sdd{WI-13956}
*/
Vector_2d_T Line_Hesse_Get_Unit_Vector(
   const Line_Hesse_T *p_line /**< [in] Line to get the unit vector from */
   );

/**
* Returns the given lines distance to origin.
* \return  the given lines distance to origin.
* \ingroup line_hesse
* \sdd{WI-13958}
*/
float Line_Hesse_Get_Distance_To_Origin(
   const Line_Hesse_T *p_line /**< [in] Line to get the norm vector from */
   );

#ifdef __cplusplus
}
#endif
#endif


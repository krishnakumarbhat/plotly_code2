#ifndef FBK_FIELD_OF_INTEREST_FACTORY_H
#define FBK_FIELD_OF_INTEREST_FACTORY_H

/**
 * @file fbk_field_of_interest_factory.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file with functions to work with field of interest.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_field_of_interest.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Global Function Definition
\*===========================================================================*/

/**
 * @brief Returns a field of interest formed by the given x and y coordinates.
 *
 * @return void
 *
 * @SRS{SF-265}
 * @SAE{SF-2552}
 * @SDD{SF-4104}
 * @verification{}
 */
void Fbk_Create_Field_Of_Interest(Fbk_Field_Of_Interest_T *p_res_foi, /**< field of interest to create */
                                  const float32_T *p_x,               /**< x component of polygon points in vcs */
                                  const float32_T *p_y,               /**< y component of polygon points in vcs */
                                  const uint8_t field_of_interest_size /**< size of the desired field of interest */);

/**
 * @brief Resets the given field of interest to be zero. Sets size to zero.
 *
 * @return void
 *
 * @SRS{SF-266}
 * @SAE{SF-2552}
 * @SDD{SF-4105}
 * @verification{}
 */
void Fbk_Reset_Field_Of_Interest(Fbk_Field_Of_Interest_T *p_res_foi /**< Field of interest to reset */);

/**
 * @brief Calculate the area of a polygon with given n ordered vertices using the shoelace formula
 * en.wikipedia.org/wiki/Shoelace_formula
 *
 * @return area of field of interest
 *
 * @SRS{SF-267}
 * @SAE{SF-2552}
 * @SDD{SF-4106}
 * @verification{}
 */
float32_T Fbk_Get_Area_Field_Of_Interest(const Fbk_Field_Of_Interest_T *p_field_of_interest /**< Field of interest*/);

/**
 * @brief Calculates area overlapped by two bounding boxes.
 * @param Fbk_Bounding_Box_T *bounding_box,
 * @param Fbk_Bounding_Box_T *bounding_box,
 * @return overlapped area
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-27704}
 * @verification{}
 */
float32_T Fbk_Get_Bounding_Boxes_Overlapped_Area(const Fbk_Bounding_Box_T *bbox_a, const Fbk_Bounding_Box_T *bbox_b);

/**
 * @brief Calculates area of single bounding box.
 * @param Fbk_Bounding_Box_T* single bounding box
 * @return Area of single bounding box
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-27705}
 * @verification{}
 */
float32_T Fbk_Get_Area_Bounding_Box(const Fbk_Bounding_Box_T *bbox);

/**
 * @brief Calculates the rectangular field of interest for the given object data.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2552}
 * @SDD{SF-4178}
 * @verification{}
 */
void Fbk_Create_Field_Of_Interest_From_Object_Data(Fbk_Field_Of_Interest_T *p_object_foi /**< Object field of interest to create */,
                                                   const Vector_2d_T obj_center_pos /**< Object center position vector */,
                                                   const float32_T obj_length /**< Object length [m]*/,
                                                   const float32_T obj_width /**< Object width [m]*/,
                                                   const float32_T obj_heading /**< Object heading [rad]*/);

/**
 * @brief Calculates the VCS-aligned bounding box for the given field of interest.
 *
 * @return bounding box for given field of interest
 *
 * @SRS{}
 * @SAE{SF-2552}
 * @SDD{SF-4179}
 * @verification{}
 */
Fbk_Bounding_Box_T Fbk_Get_Field_Of_Interest_Bounding_Box(const Fbk_Field_Of_Interest_T *p_foi /**< Field of interest*/);

/**
 * @brief Check the overlap of the two given bounding boxes.
 *
 * @return true for bounding box overlap
 *
 * @SRS{}
 * @SAE{SF-2552}
 * @SDD{SF-4180}
 * @verification{}
 */
boolean_T Fbk_Are_Bounding_Boxes_Overlapping(const Fbk_Bounding_Box_T *p_bbox_a /**< First bounding box*/,
                                             const Fbk_Bounding_Box_T *p_bbox_b /**< Second bounding box*/);

/**
 * @brief Checks if the two given fields of interest are overlapping.
 * For fast computation only center points of rectangular FoIs and corner points are checked against each other,
 * which could lead to missed overlaps in special cases.
 *
 * @return true for overlapping fields of interest
 *
 * @SRS{}
 * @SAE{SF-2552}
 * @SDD{SF-4181}
 * @verification{}
 */
boolean_T Fbk_Are_Fields_Of_Interest_Overlapping(const Fbk_Field_Of_Interest_T *p_foi_a /**< First field of interest*/,
                                                 const Fbk_Field_Of_Interest_T *p_foi_b /**< Second field of interest*/);

/**
 * @brief Quickly check that the general direction matches using dot product.
 *
 * @return true if given point is in general direction of the given vector.
 *
 * @SRS{}
 * @SAE{SF-2552}
 * @SDD{SF-4200}
 * @verification{}
 */
boolean_T Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector(const Vector_2d_T *p_foi_point /**< Field of interest point */,
                                                                const Vector_2d_T *p_vector_origin /**< Vector origin */,
                                                                const Vector_2d_T *p_vector_dir /**< Vector direction */);

/**
 * @brief Checks whether the two given lines have an intersection point.
 * Note: The intersection can also happen outside of the given line segments.
 *
 * @return true if two lines are intersecting and gives intersection point
 *
 * @SRS{}
 * @SAE{SF-2552}
 * @SDD{SF-4199}
 * @verification{}
 */
boolean_T Fbk_Are_Two_Lines_Intersecting(Vector_2d_T *p_intersection_point /**< Intersection point */,
                                         const Vector_2d_T *p_line1_start /**< Line 1 start point */,
                                         const Vector_2d_T *p_line1_end /**< Line 1 end point */,
                                         const Vector_2d_T *p_line2_start /**< Line 2 start point */,
                                         const Vector_2d_T *p_line2_end /**< Line 2 end point */);

/**
 * @brief Calculates the approximate time it takes for the given reference point to leave the zone FoI.
 *
 * @return time to leave zone field of interest for given reference point and velocity vector
 *
 * @SRS{}
 * @SAE{SF-2552}
 * @SDD{SF-4202}
 * @verification{}
 */
float32_T Fbk_Get_Time_To_Leave_Field_Of_Interest_Given_Point_And_Velocity_Vector(
   const Fbk_Field_Of_Interest_T *p_foi /**< Field of interest */,
   const Vector_2d_T *p_vel_vector_origin /**< Velocity vector origin */,
   const Vector_2d_T *p_vel_vector_dir /**< Velocity vector direction */);

/**
 * @brief Calculates the approximate time it takes for the given object to leave the zone FoI when no overlap is left.
 *     1. Find object point opposite of velocity vector
 *     2. Loop: Calculate distance between intersection point of velocity vector (originating from point in step 1)
 *        and FoI line segments (in velocity vector direction)
 *     3. Find smallest distance from step 2
 *     4. Calculate time to leave zone based on velocity vector and closest distance to FoI edge (from step 3)
 *
 * @return time to leave zone field of interest for given object
 *
 * @SRS{}
 * @SAE{SF-2552}
 * @SDD{SF-4201}
 * @verification{}
 */
float32_T
Fbk_Get_Time_For_Object_To_Leave_Field_Of_Interest(const Fbk_Field_Of_Interest_T *p_zone_foi /**< Field of interest for zone */,
                                                   const Fbk_Field_Of_Interest_T *p_object_foi /**< Field of interest for object */,
                                                   const Vector_2d_T *p_object_position /**< Object position vector */,
                                                   const Vector_2d_T *p_object_velocity /**< Object velocity vector */,
                                                   const boolean_T f_point_shift); /**< Flag allows shifting ref point if it is out
                                                                                      of the zone */


/**
 * @brief Verifies if the point is in array of points.
 *
 * @return true if point exists in array
 *
 * @SRD{}
 * @SAD{WI-22856}
 * @SDD{CSCSA-27798}
 * @verification{}
 */
boolean_T Fbk_Is_Point_In_Array(const Vector_2d_T *p_point, const Vector_2d_T *p_array, const uint8_t size);

/**
 * @brief Calculates intersection polygon for two given polygons.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{WI-22856}
 * @SDD{CSCSA-27799}
 * @verification{}
 */
void Fbk_Get_Intersection_Polygon(Fbk_Field_Of_Interest_T *result_polygon,
                                  const Fbk_Field_Of_Interest_T *first_polygon,
                                  const Fbk_Field_Of_Interest_T *second_polygon);

/**
 * @brief Calculates points that defines intersection polygon.
 *
 * @return true if any points for intersection polygon exist and return these points
 *
 * @SRD{}
 * @SAD{WI-22856}
 * @SDD{CSCSA-27800}
 * @verification{}
 */
boolean_T Fbk_Get_Intersection_Points_For_Point_In_Polygon(Fbk_Field_Of_Interest_T *result_polygon,
                                                           const Fbk_Field_Of_Interest_T *first_polygon,
                                                           const Fbk_Field_Of_Interest_T *second_polygon);

/**
 * @brief Sort points of polygon in counterclockwise direction.
 *
 * @return void
 * @SRD{}
 * @SAD{WI-22856}
 * @SDD{CSCSA-27801}
 * @verification{}
 */
void Fbk_Sort_Polygon_Points(Fbk_Field_Of_Interest_T *p_polygon);

/**
 * @brief Checks if the point is in the specific range defined by two intersecting lines.
 *
 * @return true if point in range of two lines creating this point
 *
 * @SRD{}
 * @SAD{WI-22856}
 * @SDD{CSCSA-27803}
 * @verification{}
 */
boolean_T Fbk_Is_Intersection_Point_In_Range(Vector_2d_T *p_intersection_point,
                                             const Vector_2d_T *p_line1_start,
                                             const Vector_2d_T *p_line1_end,
                                             const Vector_2d_T *p_line2_start,
                                             const Vector_2d_T *p_line2_end);

#endif /* FBK_FIELD_OF_INTEREST_FACTORY_H */

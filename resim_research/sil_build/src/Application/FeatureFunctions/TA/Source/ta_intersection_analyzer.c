/**
 * @file ta_intersection_analyzer.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module implements logic of its header for determining if
 * two objects have a high probability to collide
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ta_intersection_analyzer.h"
#include "fbk_macros.h"
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_vector_2d.h"
#include "ta_types.h"
#include <assert.h>

/*============================================================================*\
 * LOCAL FUNCTION PROTOTYPES
\*============================================================================*/
/**
 * @brief Checks the overlap of the given ego circle type and any object circle.
 *
 * @return true if circles do overlap
 *
 * @SRS{SF-2314,SF-2313}
 * @SAE{SF-3238}
 * @SDD{SF-8615}
 * @verification{Check that a circle center distance smaller than the minimum saftety distance is evaluated as overlap.}
 */
static boolean_T
Ta_Does_Ego_Circle_Overlap_Any_Object_Circle(const Fbk_Waypoint_with_Circle_Centers_T *p_ego /**< Ego waypoint */,
                                             const Fbk_Waypoint_with_Circle_Centers_T *p_obj /**< Object waypoint */,
                                             const Ta_Circle_Type_T ego_circle_type /**< Ego circle type to check */,
                                             const float32_T min_safe_distance /**< Minimum safety distance */);

/*============================================================================*\
 * LOCAL FUNCTIONS
\*============================================================================*/

static boolean_T Ta_Does_Ego_Circle_Overlap_Any_Object_Circle(const Fbk_Waypoint_with_Circle_Centers_T *p_ego,
                                                              const Fbk_Waypoint_with_Circle_Centers_T *p_obj,
                                                              const Ta_Circle_Type_T ego_circle_type,
                                                              const float32_T min_safe_distance)
{
   /* Return value */
   boolean_T f_overlap = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_ego);
   assert(NULL != p_obj);

   if ((TA_CIRCLE_FRONT == ego_circle_type)
       && ((Vector_2d_Alg_Distance(&(p_ego->circle_center_front), &(p_obj->circle_center_front)) < min_safe_distance)
           || (Vector_2d_Alg_Distance(&(p_ego->circle_center_front), &(p_obj->circle_center_middle)) < min_safe_distance)
           || (Vector_2d_Alg_Distance(&(p_ego->circle_center_front), &(p_obj->circle_center_rear)) < min_safe_distance)))
   {
      f_overlap = FBK_TRUE;
   }
   else if ((TA_CIRCLE_MIDDLE == ego_circle_type)
            && ((Vector_2d_Alg_Distance(&(p_ego->circle_center_middle), &(p_obj->circle_center_front)) < min_safe_distance)
                || (Vector_2d_Alg_Distance(&(p_ego->circle_center_middle), &(p_obj->circle_center_middle)) < min_safe_distance)
                || (Vector_2d_Alg_Distance(&(p_ego->circle_center_middle), &(p_obj->circle_center_rear)) < min_safe_distance)))
   {
      f_overlap = FBK_TRUE;
   }
   else if ((TA_CIRCLE_REAR == ego_circle_type)
            && ((Vector_2d_Alg_Distance(&(p_ego->circle_center_rear), &(p_obj->circle_center_front)) < min_safe_distance)
                || (Vector_2d_Alg_Distance(&(p_ego->circle_center_rear), &(p_obj->circle_center_middle)) < min_safe_distance)
                || (Vector_2d_Alg_Distance(&(p_ego->circle_center_rear), &(p_obj->circle_center_rear)) < min_safe_distance)))
   {
      f_overlap = FBK_TRUE;
   }
   else
   {
      /* Circle type not valid. */
   }

   return f_overlap;
}

/*============================================================================*\
 * EXPORTED FUNCTIONS
\*============================================================================*/

boolean_T Ta_Is_Critical_Approach(const Ta_Core_Calibration_T *p_ta_cal,
                                  const Fbk_Waypoint_with_Circle_Centers_T *p_ego,
                                  const Fbk_Waypoint_with_Circle_Centers_T *p_obj)
{
   boolean_T f_critical_approach = FBK_FALSE;

   /*  Here the minimum safe distance between two waypoints is calculated.
    *  It is then used as the threshold to verify a collision probability.
    *  The calculation is based on the sum of evaluating ego and object waypoint circle radii
    *  and an added safety margin defined as calibration parameter.
    */
   const float32_T min_safe_distance =
      (p_ta_cal->k_ta_critical_approach_min_safe_distance + p_ego->circle_radius + p_obj->circle_radius);

   /* Calculate difference between yaw angles for this prediction step. */
   const float32_T approach_angle_diff = Fbk_Abs_F(Angle_Diff(&p_ego->waypoint_yaw_angle, &p_obj->waypoint_yaw_angle).angle);

   /* Asserts */
   assert(NULL != p_ta_cal);
   assert(NULL != p_ego);
   assert(NULL != p_obj);

   /* Preliminary check to verify that the collision angle is not too shallow. */
   if (approach_angle_diff >= p_ta_cal->k_ta_critical_approach_angle_diff_min)
   {
      /*  Calculating the Euclidean distances between any of the representing circles of the Ego and Object
       *  and checking if any distance is smaller than the critical threshold (min_safe_distance).
       *  Since this can be checked for all circles involved from both sides we might need to have 3 x 3 = 9 checkes
       *  which are ORed.
       */

      /* Check ego front circles */
      if (Fbk_Is_True(p_ta_cal->k_ta_critical_approach_check_ego_circles[TA_CIRCLE_FRONT]))
      {
         if (Ta_Does_Ego_Circle_Overlap_Any_Object_Circle(p_ego, p_obj, TA_CIRCLE_FRONT, min_safe_distance))
         {
            f_critical_approach = FBK_TRUE;
         }
      }

      /* Check ego middle circles */
      if (Fbk_Is_True(p_ta_cal->k_ta_critical_approach_check_ego_circles[TA_CIRCLE_MIDDLE]) && Fbk_Is_False(f_critical_approach))
      {
         if (Ta_Does_Ego_Circle_Overlap_Any_Object_Circle(p_ego, p_obj, TA_CIRCLE_MIDDLE, min_safe_distance))
         {
            f_critical_approach = FBK_TRUE;
         }
      }

      /* Check ego rear circles */
      if (Fbk_Is_True(p_ta_cal->k_ta_critical_approach_check_ego_circles[TA_CIRCLE_REAR]) && Fbk_Is_False(f_critical_approach))
      {
         if (Ta_Does_Ego_Circle_Overlap_Any_Object_Circle(p_ego, p_obj, TA_CIRCLE_REAR, min_safe_distance))
         {
            f_critical_approach = FBK_TRUE;
         }
      }
   }

   return f_critical_approach;
}

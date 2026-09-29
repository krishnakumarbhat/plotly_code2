/**
 * @file lcda_common_functions.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements functions that are shared across Lcda submodules
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_common_functions.h"
#include "fbk_field_of_interest_factory.h"
#include "fbk_macros.h"
#include "fbk_object_validation.h"
#include "fbk_ref_point.h"
#include "fbk_ref_point_calc.h"
#include "lcda_debug_interface.h"
#include "ml_math.h"
#include "ml_polygon.h"
#include "ml_saturated_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include <assert.h>

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief Checks whether the reference point is in the given zone.
 *
 * @return True when reference point is in zone.
 *
 * @SRS{SF-1015,SF-1062,SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6566}
 * @verification{Check whether true is returned for a scenario where the object is within the given zone.}
 */
static boolean_T Lcda_Is_Ref_Point_In_Zone(const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
                                           const Fbk_Field_Of_Interest_T *p_zone /**< Lcda zone */,
                                           const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Helper function for Lcda_Get_Critical_Point
 *
 * @return void
 *
 * @SRS{SF-1015,SF-1062,SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6952}
 *
 * @verification{Create a tracker object. Set the reference position position and
 * verify that it returns the expected point}
 */
static void Lcda_Set_Ref_Position_Longitudinal(Vector_2d_T *p_ref_point /**< Fbk object ref point */,
                                               Lcda_Obj_Ref_Point_T *p_ref_position /**< LCDA object ref point */,
                                               const Vector_2d_T *p_obj_center /**< Fbk object's center point */,
                                               const float32_T long_zone_min /**< LCDA zone min long point */,
                                               const float32_T long_zone_max /**< LCDA zone max long point */,
                                               const float32_T obj_heading /**< Fbk object heading */,
                                               const float32_T obj_length) /**< Fbk object length */;

/**
 * @brief Helper function for Lcda_Get_Critical_Point
 *
 * @return void
 *
 * @SRS{SF-1015,SF-1062,SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6954}
 *
 * @verification{Create a tracker object. Set the reference position position and
 * verify that it returns the expected point}
 */
static void Lcda_Set_Ref_Position_Lateral(Vector_2d_T *p_ref_point /**< Fbk object ref point */,
                                          const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                          const Vector_2d_T *p_obj_center /**< Fbk object's ref point */,
                                          const float32_T lat_zone_max /**< LCDA zone max long point */,
                                          const float32_T obj_heading /**< Fbk object heading */,
                                          const float32_T obj_width /**< Fbk object width */,
                                          const float32_T obj_length /**< Fbk object length */,
                                          const Lcda_Obj_Ref_Point_T ref_position /**< LCDA object ref point */);


/**
 * @brief Setting of longitudinal coordinate distinguishes 3 cases:
 *		  1) Objects center is between front and rear end of the zone
 *		                 -> use center of object
 *		  2) Objects center is behind the rear end of the relevant zone
 *                       -> if objects front in zone: use front of object
 *		      else: use center of zone
 *		  3) Objects center is in front of the front end of the relevant zone
 *                       -> if objects rear is in zone: use rear of object
 *		                    else: use center of zone
 *
 *        Setting of lateral coordinate distinguishes 2 cases:
 *        1) Objects center is in zone or on ego side of zone or object is heading away from zone
 *                       -> use center of object
 *        2) Objects center is on outer side of zone and object is not heading away from zone
 *                       -> use coordinate between center and inner edge determined by
 *                          k_lcda_zone_intersect_critical_point_lateral_ratio
 *
 * @return two dimensional reference point
 *
 * @SRS{SF-1015,SF-1062,SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6556}
 * @verification{Check that the reference point is returned correctly for the specified cases.}
 */
static Vector_2d_T Lcda_Get_Critical_Point(const Vector_2d_T *obj_center /**< Fbk object center point pos */,
                                           float32_T obj_heading /**< Fbk object heading */,
                                           float32_T obj_length /**< Fbk object length */,
                                           float32_T obj_width /**< Fbk object width */,
                                           const Fbk_Field_Of_Interest_T *p_zone /**< Object LCDA zone */,
                                           const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration*/);


/**
 * @brief Wrapper for a given input array. Checks whether the object alerted a arbitrary submodule of Lcda.
 *
 * @return True when object was responsible for an alert of input submodule.
 *
 * @SRS{SF-1015,SF-1062,SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6814}
 * @verification{Check that the object is only returning true when it is responsible for a submodules alert.}
 */
static boolean_T Lcda_Was_Object_Critical_Before(
   const uint8_t obj_id /**< object id */,
   const uint8_t prev_obj_id[FBK_NUMBER_OF_SIDES] /**< array of objects which were causing alerts for the input submodule */);


/**
 * @brief Dependent on the warn setting mode, a different existence probability hysteresis shall be returned
 *
 * @return existence probability threshold hysteresis.
 *
 * @SRS{SF-1015,SF-1062,SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6813}
 * @verification{Check whether the correct existence probability hysteresis is returned dependent on the warn setting mode for
 * cvw.}
 */
static float32_T Lcda_Get_Mode_Dependent_Existence_Prob_Hys(const Lcda_Core_Input_T *p_core_input /**< Lcda core input*/,
                                                            const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration*/);

/**
 * @brief Returns the mode dependent minimum existence probability threshold.
 *
 * @return existence probability threshold.
 *
 * @SRS{SF-1015,SF-1062,SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6815}
 * @verification{Check whether the correct minimum existence probability threshold is returned dependent on the warn settings.}
 */
static float32_T Lcda_Get_Mode_Dependent_Min_Threshold(const Lcda_Core_Input_T *p_core_input /**< Lcda core input*/,
                                                       const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration*/);

/**
 * @brief Makes use of the FBK functionality to create a field of interest for a given tracker object.
 * Can be used for VCS and curvi object properties.
 *
 * @return Object field of interest for given object and coordinate system.
 *
 * @SRS{SF-995,SF-1001,SF-1072}
 * @SAE{SF-2779}
 * @SDD{CSCSA-70149}
 *
 * @verification{Verify if field of interest was created properly for a given tracker object.}
 */
static void Lcda_Create_Object_Field_Of_Interest(Fbk_Field_Of_Interest_T *p_object_foi /**< Object field of interest */,
                                                 const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
                                                 const Lcda_Coordinate_System_T coordinate_system /**< Coordinate system to use */);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

void Lcda_Increment_Mature_Count_In_Zone(uint8_t *p_mature_counter, const Pa_Obj_Status_T status)
{
   /* Asserts */
   assert(NULL != p_mature_counter);

   /* Start incrementing the count in zone only when the track status is mature */
   if (PA_OBJ_STATUS_MATURE == status)
   {
      Sat_Inc_Uint8(p_mature_counter);
   }
}

Lcda_Alert_State_T Lcda_Get_Alert_State(const uint8_t side, const boolean_T f_alert_active, const Lcda_Turn_Signal_T turn_signal)
{
   Lcda_Alert_State_T alert_state = LCDA_ALERT_STATE_NONE;

   /* Assert */
   assert((FBK_SIDE_LEFT == side) || (FBK_SIDE_RIGHT == side));

   if (Fbk_Is_True(f_alert_active))
   {
      if (((FBK_SIDE_LEFT == side) && (TURN_SIGNAL_LEFT == turn_signal))
          || ((FBK_SIDE_RIGHT == side) && (TURN_SIGNAL_RIGHT == turn_signal)))
      {
         alert_state = LCDA_ALERT_STATE_LEVEL_2;
      }
      else
      {
         alert_state = LCDA_ALERT_STATE_LEVEL_1;
      }
   }

   return alert_state;
}

boolean_T Lcda_Is_Alert_On(const Lcda_Alert_State_T alert_state)
{
   return (boolean_T) ((LCDA_ALERT_STATE_LEVEL_1 == alert_state) || (LCDA_ALERT_STATE_LEVEL_2 == alert_state));
}

void Lcda_Mirror_Zone_Across_Long_Axis(Fbk_Field_Of_Interest_T *p_zone)
{
   uint8_t i;

   /* Assert */
   assert(NULL != p_zone);

   for (i = FBK_ZERO_UINT; i < p_zone->size; i++)
   {
      p_zone->points[i].y = -1.0f * p_zone->points[i].y;
   }
}

void Lcda_Multiply_Zone_Length_Behind_Ego_By_Factor(const float32_T factor, Fbk_Field_Of_Interest_T *p_zone, const float32_T ego_length)
{
   /* Assert */
   assert(NULL != p_zone);

   p_zone->points[2].x = ((p_zone->points[2].x + ego_length) * factor) - ego_length;
   p_zone->points[3].x = ((p_zone->points[3].x + ego_length) * factor) - ego_length;
}

boolean_T Lcda_Is_Object_In_Ego_Lane(const float32_T lane_width,
                                     const Fbk_Object_Data_T *p_tracker_object,
                                     const Lcda_Core_Calibration_T *p_cals,
                                     const Lcda_Coordinate_System_T coordinate_system)
{
   boolean_T f_object_in_ego_lane = FBK_FALSE;


   const float32_T lane_width_clamped        = Fbk_Clamp(lane_width, p_cals->k_lcda_min_lane_width, p_cals->k_lcda_max_lane_width);
   const float32_T effective_lane_width      = lane_width_clamped * p_cals->k_lcda_ego_lane_effective_lane_width_factor;
   const float32_T effective_lane_width_half = Fbk_Half(effective_lane_width);


   if (Fbk_Is_True(p_cals->k_lcda_ego_lane_check_center_point_only))
   {
      float32_T object_lat_pos;

      if (LCDA_USE_VCS == coordinate_system)
      {
         object_lat_pos = p_tracker_object->vcs_pos.y;
      }
      else
      {
         object_lat_pos = p_tracker_object->curvi_pos.y;
      }

      /* Only check the object center against the effective lane width to determine ego lane occupation. */
      if (Fbk_Abs_F(object_lat_pos) < effective_lane_width_half)
      {
         f_object_in_ego_lane = FBK_TRUE;
      }
   }
   else
   {
      /* Comprehensive object overlap check to determine ego lane occupation. */
      Fbk_Field_Of_Interest_T object_foi;
      Fbk_Field_Of_Interest_T ego_lane_foi;

      /* Create FoI for given object. */
      Lcda_Create_Object_Field_Of_Interest(&object_foi, p_tracker_object, coordinate_system);

      /* Create FoI for ego lane based on lane width and maximum range. */
      ego_lane_foi.size = FBK_FOI_SIZE_TETRAGON;

      ego_lane_foi.points[0].x = FBK_ZERO_F;
      ego_lane_foi.points[0].y = -effective_lane_width_half;
      ego_lane_foi.points[1].x = FBK_ZERO_F;
      ego_lane_foi.points[1].y = effective_lane_width_half;
      ego_lane_foi.points[2].x = -p_cals->k_lcda_max_range;
      ego_lane_foi.points[2].y = effective_lane_width_half;
      ego_lane_foi.points[3].x = -p_cals->k_lcda_max_range;
      ego_lane_foi.points[3].y = -effective_lane_width_half;

      f_object_in_ego_lane = Fbk_Are_Fields_Of_Interest_Overlapping(&object_foi, &ego_lane_foi);
   }

   return f_object_in_ego_lane;
}

float32_T Lcda_Get_Longitudinal_Ttc(const Fbk_Object_Data_T *p_tracker_object, const float32_T ego_length)
{
   float32_T lon_ttc = LCDA_DEFAULT_LARGE_TTC;

   /* Only calculate long TTC for objects moving towards the ego vehicle */
   if ((p_tracker_object->curvi_pos.x <= FBK_ZERO_F) && (p_tracker_object->curvi_vel_rel.x > FBK_ZERO_F))
   {
      /* Calculate the object distance of the front bumper of object to the rear bumper of the ego vehicle */
      /* To increase branch coverage, we can omit the Fbk_Abs_F(p_tracker_object->curvi_pos.x), because curvi_pos.x is always
       * negative or zero due to if condition */
      float32_T obj_distance = -p_tracker_object->curvi_pos.x - (0.5f * p_tracker_object->length) - ego_length;

      /* Calculate the ttc */
      /* To increase branch coverage, we can omit the Fbk_Abs_F(p_tracker_object->curvi_vel_rel.x), because curvi_vel_rel.x is
       * always positive due to of condition*/
      lon_ttc = obj_distance / p_tracker_object->curvi_vel_rel.x;
   }

   return lon_ttc;
}

float32_T Lcda_Get_Longitudinal_Ttp(const Fbk_Object_Data_T *p_tracker_object,
                                    const Lcda_Coordinate_System_T coordinate_system /**< Coordinate system to use */)
{
   Fbk_Object_Corners_T target_corners;
   float32_T relative_vel;
   float32_T extreme_long_point;
   float32_T lon_ttp = LCDA_DEFAULT_LARGE_TTC;

   /* Asserts */
   assert(NULL != p_tracker_object);

   /* Calculate the corners of the object */
   if (LCDA_USE_VCS == coordinate_system)
   {
      Fbk_Calculate_Target_Corners(&target_corners, &p_tracker_object->vcs_pos, &p_tracker_object->vcs_heading,
                                   &p_tracker_object->length, &p_tracker_object->width);
      relative_vel = p_tracker_object->vcs_vel_rel.x;
   }
   else
   {
      Fbk_Calculate_Target_Corners(&target_corners, &p_tracker_object->curvi_pos, &p_tracker_object->curvi_heading,
                                   &p_tracker_object->length, &p_tracker_object->width);
      relative_vel = p_tracker_object->curvi_vel_rel.x;
   }
   /* Checking whether the rear left or right corner is further away. Only SOT cases */
   if (relative_vel > EPSILON)
   {
      extreme_long_point = target_corners.points[FBK_REAR_RIGHT_CORNER].x;
      if (extreme_long_point > target_corners.points[FBK_REAR_LEFT_CORNER].x)
      {
         extreme_long_point = target_corners.points[FBK_REAR_LEFT_CORNER].x;
      }

      lon_ttp = Fbk_Clamp(Fbk_Abs_F(extreme_long_point / relative_vel), FBK_ZERO_F, LCDA_DEFAULT_LARGE_TTC);
   }
   return lon_ttp;
}

float32_T Lcda_Get_Lateral_Ttc(const Fbk_Object_Data_T *p_tracker_object, const float32_T ego_width)
{
   float32_T lat_ttc = LCDA_DEFAULT_LARGE_TTC;

   /* We are only interested in the cases where the obj is moving towards the ego so a collision with the ego is possible.
     *
    * Case 1: obj on LEFT i.e. objLatPos is -ve
    *         objRelVel +ve implies obj is moving towards the ego (possible collision)
    *         objRelVel -ve implies obj is moving away from the ego (ignore)

    * Case 2: obj on RIGHT i.e. objLatPos is +ve
    *         objRelVel +ve implies obj is moving away from the ego (ignore)
    *         objRelVel -ve implies obj is moving towards the ego (possible collision)
    *
    * Conclusion:  We calculate the ttc only when the objLatPos and objRelVel have opposite signs (possible collision cases)
    *              for all other cases including when objRelVel = 0.0, ttc is set to MAX_LARGE_TTC
    */
   if (((p_tracker_object->curvi_pos.y < FBK_ZERO_F) && (p_tracker_object->curvi_vel_rel.y > FBK_ZERO_F))
       || ((p_tracker_object->curvi_pos.y > FBK_ZERO_F) && (p_tracker_object->curvi_vel_rel.y < FBK_ZERO_F)))
   {
      float32_T obj_lat_distance = Fbk_Abs_F(p_tracker_object->curvi_pos.y) - (0.5f * (p_tracker_object->width + ego_width));

      /* Calculate the ttc */
      lat_ttc = obj_lat_distance / Fbk_Abs_F(p_tracker_object->curvi_vel_rel.y);
   }

   return lat_ttc;
}


float32_T Lcda_Get_Ttle(const Fbk_Object_Data_T *p_tracker_object,
                        const Fbk_Field_Of_Interest_T *p_zone,
                        const Lcda_Coordinate_System_T coordinate_system)
{
   Fbk_Object_Corners_T object_corners;
   float32_T nearest_corner_y;
   float32_T relative_vel;
   float32_T ttle = LCDA_DEFAULT_LARGE_TTLE;
   float32_T sign;
   uint8_t point_idx;

   if ((NULL != p_tracker_object) && (NULL != p_zone))
   {
      /* Calculate the corners of the object */
      if (LCDA_USE_VCS == coordinate_system)
      {
         Fbk_Calculate_Target_Corners(&object_corners, &p_tracker_object->vcs_pos, &p_tracker_object->vcs_heading,
                                      &p_tracker_object->length, &p_tracker_object->width);
         sign         = Fbk_Get_Obj_Side_Sign(p_tracker_object->vcs_pos.y);
         relative_vel = p_tracker_object->vcs_vel_rel.y;
      }
      else
      {
         Fbk_Calculate_Target_Corners(&object_corners, &p_tracker_object->curvi_pos, &p_tracker_object->curvi_heading,
                                      &p_tracker_object->length, &p_tracker_object->width);
         sign         = Fbk_Get_Obj_Side_Sign(p_tracker_object->curvi_pos.y);
         relative_vel = p_tracker_object->curvi_vel_rel.y;
      }

      nearest_corner_y = sign * LCDA_HUGE_LATERAL_DISTANCE;
      for (point_idx = (uint8_t) FBK_FRONT_LEFT_CORNER; point_idx < (uint8_t) FBK_NUM_OF_OBJECT_CORNERS; point_idx++)
      {
         if ((sign * object_corners.points[point_idx].y) < (sign * nearest_corner_y))
         {
            nearest_corner_y = object_corners.points[point_idx].y;
         }
      }

      /* if the object moves away from the host then ttle > 0; ttle saturates to 0 when the object leaves the zone */
      /* otherwise ttle is set to default (high) value */
      if ((sign * relative_vel) > EPSILON)
      {
         /* we assume for simplicity that the outer border of the zone is parallel to the host longitudinal axis */
         ttle = Fbk_Clamp((p_zone->points[FRONT_OUTER_SIDE].y - nearest_corner_y) / relative_vel, FBK_ZERO_F, LCDA_DEFAULT_LARGE_TTLE);
      }
   }

   return ttle;
}


uint8_t Lcda_Get_Opposite_Side(const uint8_t side)
{
   uint8_t opposite_side = FBK_SIDE_LEFT;

   if (FBK_SIDE_LEFT == side)
   {
      opposite_side = FBK_SIDE_RIGHT;
   }

   return opposite_side;
}

float32_T Lcda_Get_Lateral_Distance_Guardrail(const uint8_t side, const Lcda_Guardrail_Sources_T guardrail_data[FBK_NUMBER_OF_SIDES])
{
   float32_T lateral_guardrail_return;
   float32_T lateral_guardrail_pos_radar;
   float32_T lateral_guardrail_pos_camera;
   float32_T side_sign;

   /* Assert */
   assert(NULL != guardrail_data);

   side_sign = Fbk_Convert_Obj_Side_To_Sign(side);

   /* get lateral radar guardrail position, if available */
   if (LCDA_GUARDRAIL_VALID == guardrail_data[side].radar.status)
   {
      lateral_guardrail_pos_radar = guardrail_data[side].radar.lateral_position;
   }
   else
   {
      lateral_guardrail_pos_radar = side_sign * LCDA_HUGE_LATERAL_DISTANCE;
   }

   /* get lateral camera guardrail position, if available */
   if (LCDA_GUARDRAIL_VALID == guardrail_data[side].camera.status)
   {
      lateral_guardrail_pos_camera = guardrail_data[side].camera.lateral_position;
   }
   else
   {
      lateral_guardrail_pos_camera = side_sign * LCDA_HUGE_LATERAL_DISTANCE;
   }

   /* choose the guardrail, that is closer to host */
   if ((side_sign * lateral_guardrail_pos_radar) < (side_sign * lateral_guardrail_pos_camera))
   {
      lateral_guardrail_return = lateral_guardrail_pos_radar;
   }
   else
   {
      lateral_guardrail_return = lateral_guardrail_pos_camera;
   }

   return lateral_guardrail_return;
}

float32_T Lcda_Get_Existence_Probability_Threshold(const Lcda_Core_Input_T *p_core_input,
                                                   const uint8_t prev_obj_id[FBK_NUMBER_OF_SIDES],
                                                   const uint8_t obj_id,
                                                   const Lcda_Core_Calibration_T *p_cals)
{
   float32_T existence_prob_threshold;
   float32_T existence_prob_hyst_offset;

   if (Lcda_Was_Object_Critical_Before(obj_id, prev_obj_id))
   {
      existence_prob_hyst_offset = Lcda_Get_Mode_Dependent_Existence_Prob_Hys(p_core_input, p_cals);
   }
   else
   {
      existence_prob_hyst_offset = 0.0f;
   }

   existence_prob_threshold = Lcda_Get_Mode_Dependent_Min_Threshold(p_core_input, p_cals);

   return (existence_prob_threshold - existence_prob_hyst_offset);
}


void Lcda_Limit_Outer_Zone_Points(Fbk_Field_Of_Interest_T *p_zone, const float32_T lane_width, const Lcda_Core_Calibration_T *p_cals)
{
   assert(NULL != p_zone);
   assert(NULL != p_cals);

   /* Limit zone width */
   p_zone->points[FRONT_OUTER_SIDE].y =
      Fbk_Min(p_zone->points[FRONT_OUTER_SIDE].y, (Fbk_Half(lane_width) + p_cals->k_lcda_max_lane_width));
   p_zone->points[MIDDLE_OUTER_SIDE].y =
      Fbk_Min(p_zone->points[MIDDLE_OUTER_SIDE].y, (Fbk_Half(lane_width) + p_cals->k_lcda_max_lane_width));
   p_zone->points[REAR_OUTER_SIDE].y =
      Fbk_Min(p_zone->points[REAR_OUTER_SIDE].y, (Fbk_Half(lane_width) + p_cals->k_lcda_max_lane_width));
}

float32_T Lcda_Get_Obj_Front_Position(const Fbk_Object_Data_T *p_tracker_object)
{
   float32_T obj_front_bumper_long_pos;

   /* Assert */
   assert(NULL != p_tracker_object);

   obj_front_bumper_long_pos = p_tracker_object->curvi_pos.x + (Fbk_Half(p_tracker_object->length));

   return obj_front_bumper_long_pos;
}


float32_T Lcda_Get_Obj_Side_Distance_Lateral(const Fbk_Object_Data_T *p_tracker_object)
{
   float32_T obj_side_distance_lateral;

   /* Assert */
   assert(NULL != p_tracker_object);

   obj_side_distance_lateral = Fbk_Abs_F(p_tracker_object->curvi_pos.y) - Fbk_Half(p_tracker_object->width);

   return obj_side_distance_lateral;
}

void Lcda_Get_Zone_Maxima(float32_T *p_long_zone_min,
                          float32_T *p_long_zone_max,
                          float32_T *p_lat_zone_max,
                          const Fbk_Field_Of_Interest_T *p_zone)
{
   uint8_t i;
   /* get long min/max and lat max of zone */
   for (i = FBK_ONE_UINT; i < p_zone->size; i++)
   {
      if ((*p_long_zone_min) > p_zone->points[i].x)
      {
         (*p_long_zone_min) = p_zone->points[i].x;
      }
      else
      {
         if ((*p_long_zone_max) < p_zone->points[i].x)
         {
            (*p_long_zone_max) = p_zone->points[i].x;
         }
      }

      if (Fbk_Abs_F(*p_lat_zone_max) < Fbk_Abs_F(p_zone->points[i].y))
      {
         (*p_lat_zone_max) = p_zone->points[i].y;
      }
   }
}

void Lcda_Get_Object_Location_Data(Lcda_Object_Location_Data_T *p_obj_loc_data,
                                   const Fbk_Object_Data_T *p_obj,
                                   const Fbk_Field_Of_Interest_T *p_zone_foi,
                                   const Lcda_Core_Calibration_T *p_cals,
                                   const Lcda_Coordinate_System_T coordinate_system)

{
   Fbk_Field_Of_Interest_T obj_foi;
   boolean_T overlapped;

   /* Asserts */
   assert(NULL != p_obj_loc_data);
   assert(NULL != p_obj);
   assert(NULL != p_zone_foi);
   assert(NULL != p_cals);

   switch (p_cals->k_lcda_zone_check_method)
   {
      case (uint8_t) LCDA_ZONE_CHECK_FOI_OVERLAP:

         Lcda_Create_Object_Field_Of_Interest(&obj_foi, p_obj, coordinate_system);
         overlapped = Fbk_Are_Fields_Of_Interest_Overlapping(&obj_foi, p_zone_foi);

         if (Fbk_Is_True(overlapped))
         {
            Fbk_Field_Of_Interest_T result;
            float32_T intersection_area, object_area;
            float32_T area_ratio = FBK_ZERO_F;

            Fbk_Get_Intersection_Polygon(&result, &obj_foi, p_zone_foi);
            Fbk_Sort_Polygon_Points(&result);

            intersection_area = Fbk_Get_Area_Field_Of_Interest(&result);
            object_area       = Fbk_Get_Area_Field_Of_Interest(&obj_foi);

            if (object_area > EPSILON) /* to avoid direct comparison to 0.0f */
            {
               area_ratio = intersection_area / object_area;
            }
            p_obj_loc_data->obj_in_zone        = FBK_TRUE;
            p_obj_loc_data->area_overlap_ratio = area_ratio;
         }
         else
         {
            p_obj_loc_data->obj_in_zone        = FBK_FALSE;
            p_obj_loc_data->area_overlap_ratio = FBK_ZERO_F;
         }
         break;

      case (uint8_t) LCDA_ZONE_CHECK_REF_POINT:
      default:
         p_obj_loc_data->obj_in_zone        = Lcda_Is_Ref_Point_In_Zone(p_obj, p_zone_foi, p_cals);
         p_obj_loc_data->area_overlap_ratio = FBK_ZERO_F;
         break;
   }
}


/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/


static void Lcda_Set_Ref_Position_Longitudinal(Vector_2d_T *p_ref_point,
                                               Lcda_Obj_Ref_Point_T *p_ref_position,
                                               const Vector_2d_T *p_obj_center,
                                               const float32_T long_zone_min,
                                               const float32_T long_zone_max,
                                               const float32_T obj_heading,
                                               const float32_T obj_length)
{
   /* longitudinal check
    * check whether object is behind/at same height/in front of the zone
    * to decide which ref point to use
    * so far use lateral center of object (see lateral check below) */

   if (p_obj_center->x < long_zone_min)
   {
      /* behind zone -> use front of vehicle */
      p_ref_point->x = p_obj_center->x + Fbk_Half(obj_length * Fast_Cos(obj_heading));

      /* check whether front is in front of zone (e.g. truck in short zone (city environment)) */
      if (p_ref_point->x > long_zone_max)
      {
         (*p_ref_position) = LCDA_OBJ_USE_ZONECENTER;
         p_ref_point->x    = long_zone_max - Fbk_Half(long_zone_max - long_zone_min);
      }
      else
      {
         (*p_ref_position) = LCDA_OBJ_USE_FRONT;
      }
   }
   else if (p_obj_center->x > long_zone_max)
   {
      /* in front of zone -> use vehicles rear end */
      p_ref_point->x = p_obj_center->x - Fbk_Half(obj_length * Fast_Cos(obj_heading));

      /* check whether rear end is behind zone (e.g. truck in short zone (city environment)) */
      if (p_ref_point->x < long_zone_min)
      {
         (*p_ref_position) = LCDA_OBJ_USE_ZONECENTER;
         p_ref_point->x    = long_zone_max - Fbk_Half(long_zone_max - long_zone_min);
      }
      else
      {
         (*p_ref_position) = LCDA_OBJ_USE_REAR;
      }
   }
   else
   {
      /* longitudinally between zone min and max -> use object center as ref point */
      (*p_ref_position) = LCDA_OBJ_USE_CENTER;
      p_ref_point->x    = p_obj_center->x;
   }
}

static void Lcda_Set_Ref_Position_Lateral(Vector_2d_T *p_ref_point,
                                          const Lcda_Core_Calibration_T *p_cals,
                                          const Vector_2d_T *p_obj_center,
                                          const float32_T lat_zone_max,
                                          const float32_T obj_heading,
                                          const float32_T obj_width,
                                          const float32_T obj_length,
                                          const Lcda_Obj_Ref_Point_T ref_position)
{
   float32_T side_sign;
   float32_T lat_adjustment;

   switch (ref_position)
   {
      case LCDA_OBJ_USE_FRONT:
         p_ref_point->y = p_obj_center->y + (0.5f * obj_length * Fast_Sin(obj_heading));
         break;

      case LCDA_OBJ_USE_REAR:
         p_ref_point->y = p_obj_center->y - (0.5f * obj_length * Fast_Sin(obj_heading));
         break;

      case LCDA_OBJ_USE_CENTER:
         p_ref_point->y = p_obj_center->y;
         break;

      case LCDA_OBJ_USE_ZONECENTER:
         p_ref_point->y = p_obj_center->y + ((p_ref_point->x - p_obj_center->x) * Fast_Sin(obj_heading));
         break;

         /* coverity[dead_error_begin]  */
      default:
         assert(FBK_FALSE && "this statement cannot be reached");
         break;
   }
   /* lateral check
    * Check whether object is beside of the zone and heading towards the zone.
    * If so, adjust ref point by ratio specified by k_lcda_zone_intersect_critical_point_lateral_ratio
    * towards inner side. This enables testing of inner edge points of the object or
    * intermediate points closer to the inner edge than the center points. */

   side_sign = Fbk_Get_Obj_Side_Sign(p_ref_point->y);

   if ((Fbk_Abs_F(p_ref_point->y) >= Fbk_Abs_F(lat_zone_max)) && ((side_sign * obj_heading) <= FBK_ZERO_F))
   {
      lat_adjustment = 0.5f * p_cals->k_lcda_zone_intersect_critical_point_lateral_ratio * obj_width;
      p_ref_point->x = p_ref_point->x + (side_sign * lat_adjustment * Fast_Sin(obj_heading));
      p_ref_point->y = p_ref_point->y - (side_sign * lat_adjustment * Fast_Cos(obj_heading));
   }
}

static Vector_2d_T Lcda_Get_Critical_Point(const Vector_2d_T *obj_center,
                                           float32_T obj_heading,
                                           float32_T obj_length,
                                           float32_T obj_width,
                                           const Fbk_Field_Of_Interest_T *p_zone,
                                           const Lcda_Core_Calibration_T *p_cals)
{
   Vector_2d_T ref_point;
   float32_T long_zone_min = p_zone->points[0].x;
   float32_T long_zone_max = p_zone->points[0].x;
   float32_T lat_zone_max  = p_zone->points[0].y;
   Lcda_Obj_Ref_Point_T ref_position;

   /* Asserts */
   assert(NULL != obj_center);
   assert(NULL != p_zone);
   assert(NULL != p_cals);

   /* Reference point is initially set to the object center */
   ref_point = (*obj_center);

   /* get long min/max and lat max of zone */
   Lcda_Get_Zone_Maxima(&long_zone_min, &long_zone_max, &lat_zone_max, p_zone);

   /* longitudinal check */
   Lcda_Set_Ref_Position_Longitudinal(&ref_point, &ref_position, obj_center, long_zone_min, long_zone_max, obj_heading, obj_length);

   /* lateral check */
   Lcda_Set_Ref_Position_Lateral(&ref_point, p_cals, obj_center, lat_zone_max, obj_heading, obj_width, obj_length, ref_position);

   return ref_point;
}


static boolean_T Lcda_Was_Object_Critical_Before(const uint8_t obj_id, const uint8_t prev_obj_id[FBK_NUMBER_OF_SIDES])
{
   boolean_T f_obj_was_critical_before = FBK_FALSE;

   if ((obj_id == prev_obj_id[FBK_SIDE_LEFT]) || (obj_id == prev_obj_id[FBK_SIDE_RIGHT]))
   {
      f_obj_was_critical_before = FBK_TRUE;
   }

   return f_obj_was_critical_before;
}

static float32_T Lcda_Get_Mode_Dependent_Existence_Prob_Hys(const Lcda_Core_Input_T *p_core_input, const Lcda_Core_Calibration_T *p_cals)
{
   float32_T hysteresis_to_return;
   assert(NULL != p_core_input);
   assert(NULL != p_cals);

   if (Fbk_Is_True(p_core_input->warn_settings.f_use_cvw_lane_change_intention_zone))
   {
      hysteresis_to_return = p_cals->k_lcda_exist_prob_lc_intention_hys_offset;
   }
   else
   {
      hysteresis_to_return = p_cals->k_lcda_exist_prob_hys_offset;
   }

   return hysteresis_to_return;
}

static float32_T Lcda_Get_Mode_Dependent_Min_Threshold(const Lcda_Core_Input_T *p_core_input, const Lcda_Core_Calibration_T *p_cals)
{
   float32_T min_threshold;
   assert(NULL != p_core_input);
   assert(NULL != p_cals);

   if (Fbk_Is_True(p_core_input->warn_settings.f_use_cvw_lane_change_intention_zone))
   {
      min_threshold = p_cals->k_lcda_min_exist_prob_lc_intention;
   }
   else
   {
      min_threshold = p_cals->k_lcda_min_exist_prop;
   }

   return min_threshold;
}


static boolean_T Lcda_Is_Ref_Point_In_Zone(const Fbk_Object_Data_T *p_tracker_object,
                                           const Fbk_Field_Of_Interest_T *p_zone,
                                           const Lcda_Core_Calibration_T *p_cals)
{
   boolean_T f_intersect_temp;
   Vector_2d_T ref_point;
   Vector_2d_T obj_center;

   /* Asserts */
   assert(NULL != p_zone);
   assert(NULL != p_cals);

   obj_center.x = p_tracker_object->curvi_pos.x;
   obj_center.y = p_tracker_object->curvi_pos.y;

   ref_point = Lcda_Get_Critical_Point(&obj_center, p_tracker_object->curvi_heading, p_tracker_object->length,
                                       p_tracker_object->width, p_zone, p_cals);

   Binary_Lcda_Debug_Pass_Object_Ref_Point(&(ref_point), p_tracker_object->id);

   /* Test whether reference point is located in zone */
   f_intersect_temp = Is_Point_In_Convex_Polygon_Ray_Casting_Method(p_zone->points, p_zone->size, &ref_point);

   return f_intersect_temp;
}

static void Lcda_Create_Object_Field_Of_Interest(Fbk_Field_Of_Interest_T *p_object_foi,
                                                 const Fbk_Object_Data_T *p_tracker_object,
                                                 const Lcda_Coordinate_System_T coordinate_system)
{
   /* Create FoI based on given coordinate system */
   switch (coordinate_system)
   {
      case LCDA_USE_VCS:
         /* Create FoI for given object in VCS */
         Fbk_Create_Field_Of_Interest_From_Object_Data(p_object_foi, p_tracker_object->vcs_pos, p_tracker_object->length,
                                                       p_tracker_object->width, p_tracker_object->vcs_heading);
         break;

      case LCDA_USE_CURVI:
      default:
         /* Create FoI for given object in curvi coordinates */
         Fbk_Create_Field_Of_Interest_From_Object_Data(p_object_foi, p_tracker_object->curvi_pos, p_tracker_object->length,
                                                       p_tracker_object->width, p_tracker_object->curvi_heading);
         break;
   }
}

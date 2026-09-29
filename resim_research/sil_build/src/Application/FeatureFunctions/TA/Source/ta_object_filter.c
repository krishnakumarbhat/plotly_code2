/**
 * @file ta_object_filter.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements functions related to filter valid objects.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ta_object_filter.h"
#include "fbk_field_of_interest.h"
#include "fbk_functions.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "ml_lookup_table_2d.h"
#include "ml_polygon.h"
#include "ml_vector_2d_t.h"
#include "ta_factory.h"
#include "ta_types.h"
#include <assert.h>

/*============================================================================*\
 * LOCAL FUNCTION PROTOTYPES
\*============================================================================*/

/**
 * @brief Checks if object heading and position is within the scenario specific range.
 *
 * Calculates heading range based on host vehicle curvature and checks if
 * object heading and position is within that range.
 *
 * @return true if the objects heading and position is relevant based on host curvature
 *
 * @SRS{SF-2304,SF-2333,SF-2335,SF-2336,SF-2337,SF-2338,SF-2339,SF-2340,SF-2341,SF-2349,SF-2355,SF-2363,SF-2268}
 * @SAE{SF-3238}
 * @SDD{SF-8539}
 * @verification{Check if only for obj_heading in range and a valid position there is a positive return.}
 */
static boolean_T
Ta_Is_Obj_Relevant_Regarding_Host_Curvature(const Ta_Object_T *p_ta_object /**< TA Object */,
                                            const boolean_T f_check_extended_range /**< flag for extended range check */,
                                            const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                            const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Checks if the current vehicle state is relevant for Turn Assist (TA) overall.
 *
 * @return true if the current vehicle state is relevant for TA
 *
 * @SRS{SF-2347,SF-2348}
 * @SAE{SF-3238}
 * @SDD{SF-8730}
 * @verification{Create a test with a host vehicle whose speed and yawrate are in valid ranges. Only when both are inside the valid
 * ranges, true shall be returned.}
 */
static boolean_T Ta_Is_Vehicle_State_Relevant(const Ta_Object_T *p_ta_object /**< TA Object */,
                                              const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                              const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Updates the objects relevance for Front Turn Assist (FTA).
 *
 * @return void
 *
 * @SRS{SF-2304,SF-2333,SF-2335,SF-2336,SF-2337,SF-2338,SF-2339,SF-2340,SF-2341,SF-2349,SF-2355,SF-2363,SF-2268}
 * @SAE{SF-3238}
 * @SDD{SF-8731}
 * @verification{Create an object whose existence probability, relative velocity, velocity, heading, speed, length, width, age and
 * object class attributes are in a specified range. Also the object shall be located in the danger zone. Only then
 * f_obj_in_danger_zone shall be set to true.}
 */
static void Ta_Update_Obj_Fta_Relevance(Ta_Object_T *p_ta_object /**< TA Object */,
                                        const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                        const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Checks whether object is rta relevant.
 *
 * @return boolean_T
 *
 * @SRS{SF-2304,SF-2333,SF-2335,SF-2336,SF-2337,SF-2338,SF-2339,SF-2340,SF-2342,SF-2349,SF-2355,SF-2363}
 * @SAE{SF-3238}
 * @SDD{SF-8787}
 * @verification{Create an object whose existence probability, relative velocity, velocity, heading, speed, length, width, age and
 * object class attributes are in a specified range. Only then true shall be returned.}
 */
static boolean_T Ta_Is_Obj_Rta_Relevant(const Ta_Object_T *p_ta_object, const Ta_Core_Calibration_T *p_ta_cal);

/**
 * @brief Updates the objects relevance for Rear Turn Assist (RTA).
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8732}
 * @verification{Create a test in which k_f_rta_enable is set to True, the object is relevant and where the object is located in
 * info or warn zone. Only then the corresponding flags are allowed to be set to true.}
 */
static void Ta_Update_Obj_Rta_Relevance(Ta_Object_T *p_ta_object /**< TA Object */,
                                        const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Checks if the object with the given index is within the danger zone.
 *
 * @return true if the given object is inside the danger zone
 *
 * @SRS{SF-2304,SF-2333,SF-2335,SF-2336,SF-2337,SF-2338,SF-2339,SF-2340,SF-2341,SF-2349,SF-2355,SF-2363,SF-2268}
 * @SAE{SF-3238}
 * @SDD{SF-8727}
 * @verification{Create tests in which danger zones are enabled, in the first test case the object shall be within the left danger
 * zone in the second test the object shall be in the right danger zone. Only in those two scenarios true shall be returned.}
 */
static boolean_T Ta_Is_Obj_In_Danger_Zone(const Ta_Object_T *p_ta_object /**< TA Object */,
                                          const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Checks if the given object position is within the info zone.
 *
 * @return true if the given object is inside the info zone
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8728}
 * @verification{Create tests in which info zones are enabled, in the first test case the object curvi or vcs coordinates shall be
 * within the left info zone in the second test the object shall be in the right info zone. Only in those two scenarios true shall
 * be returned.}
 */
static boolean_T Ta_Is_Obj_In_Info_Zone(const Ta_Object_T *p_ta_object /**< TA Object */,
                                        const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Checks if the given object position is within the info zone.
 *
 * @return true if the given object is inside the wing zone
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8729}
 * @verification{Create tests in which wing zones are enabled, in the first test case the object shall be within the left wing zone
 * in the second test the object shall be in the right wing zone. Only in those two scenarios true shall be returned.}
 */
static boolean_T Ta_Is_Obj_In_Wing_Zone(const Ta_Object_T *p_ta_object /**< TA Object */,
                                        const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/*============================================================================*\
 * LOCAL FUNCTIONS
\*============================================================================*/

static boolean_T Ta_Is_Obj_Relevant_Regarding_Host_Curvature(const Ta_Object_T *p_ta_object,
                                                             const boolean_T f_check_extended_range,
                                                             const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                             const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Return flag */
   boolean_T f_obj_relevant_for_curvature;

   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ta_cal);
   assert(TA_K_TA_LOOKUP_TURNING_HOST_SPEED_ARRAY_SIZE_DIM0 == TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0);

   if (Fbk_Is_Nan(p_vehicle_data->curvature))
   {
      /* Curvature is NaN -> object shall not be considered */
      f_obj_relevant_for_curvature = FBK_FALSE;
   }
   else
   {
      boolean_T f_heading_in_range      = FBK_FALSE;
      boolean_T f_heading_rate_in_range = FBK_FALSE;
      boolean_T f_speed_in_range        = FBK_FALSE;
      boolean_T f_position_valid        = FBK_FALSE;
      boolean_T f_velocity_heading_diff_in_range;

      float32_T heading_value;
      float32_T heading_rate_abs = Fbk_Abs_F(p_ta_object->tracker_data.heading_rate);

      float32_T heading_threshold_min;
      float32_T heading_threshold_max;

      float32_T heading_threshold_min_offset = p_ta_cal->k_fta_obj_heading_ofst[TA_MIN];
      float32_T heading_threshold_max_offset = p_ta_cal->k_fta_obj_heading_ofst[TA_MAX];

      float32_T speed_threshold_min;
      float32_T speed_threshold_max;

      float32_T speed_threshold_min_offset = p_ta_cal->k_fta_obj_speed_ofst[TA_MIN];
      float32_T speed_threshold_max_offset = p_ta_cal->k_fta_obj_speed_ofst[TA_MAX];

      if (Fbk_Abs_F(p_vehicle_data->curvature) <= p_ta_cal->k_ta_straight_host_curvature_max)
      {
         /* Scenario is considered as straight -> a smaller heading range is set for object qualification */
         heading_value         = Fbk_Abs_F(p_ta_object->tracker_data.vcs_heading);
         heading_threshold_min = p_ta_cal->k_fta_obj_heading_straight[TA_MIN];
         heading_threshold_max = p_ta_cal->k_fta_obj_heading_straight[TA_MAX];

         /* Check if object heading value is in defined range */
         f_heading_in_range = Fbk_Is_Float_In_Given_Range(heading_value, heading_threshold_min, heading_threshold_max,
                                                          f_check_extended_range, heading_threshold_min_offset,
                                                          heading_threshold_max_offset);

         /* Check if object heading rate is in defined range */
         f_heading_rate_in_range = Fbk_Is_Float_In_Given_Range(
            heading_rate_abs, p_ta_cal->k_fta_obj_heading_rate_straight[TA_MIN], p_ta_cal->k_fta_obj_heading_rate_straight[TA_MAX],
            f_check_extended_range, p_ta_cal->k_fta_obj_heading_rate_ofst[TA_MIN], p_ta_cal->k_fta_obj_heading_rate_ofst[TA_MAX]);

         /* Check that objects in straight scenarios are generally positioned in front of the host vehicle */
         f_position_valid =
            (boolean_T) (Fbk_Is_True(p_ta_object->tracker_data.vcs_pos.x >= p_ta_cal->k_fta_obj_vcs_long_pos_straight_min));

         /* Check that the object is in the defined speed range for driving straight */
         speed_threshold_min = p_ta_cal->k_fta_obj_speed_straight[TA_MIN];
         speed_threshold_max = p_ta_cal->k_fta_obj_speed_straight[TA_MAX];
         f_speed_in_range = Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.speed, speed_threshold_min, speed_threshold_max,
                                                        f_check_extended_range, speed_threshold_min_offset,
                                                        speed_threshold_max_offset);
      }
      else if (Fbk_Abs_F(p_vehicle_data->curvature) >= Get_Value_From_2d_Lookup_Table(
                  p_ta_cal->k_ta_lookup_turning_host_speed, p_ta_cal->k_ta_lookup_turning_host_curvature_min,
                  TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0, p_vehicle_data->host_speed))
      {
         /* Scenario is considered as turning -> heading range for approach side and difference to ego heading is checked */
         heading_value = p_ta_object->tracker_data.vcs_heading;

         /* Extensive object position check will be done via the zone check later */
         f_position_valid = FBK_TRUE;

         /* Check if object heading rate is in defined range */
         f_heading_rate_in_range = Fbk_Is_Float_In_Given_Range(heading_rate_abs, p_ta_cal->k_fta_obj_heading_rate[TA_MIN],
                                                               p_ta_cal->k_fta_obj_heading_rate[TA_MAX], f_check_extended_range,
                                                               p_ta_cal->k_fta_obj_heading_rate_ofst[TA_MIN],
                                                               p_ta_cal->k_fta_obj_heading_rate_ofst[TA_MAX]);

         if (p_vehicle_data->curvature < FBK_ZERO_F)
         {
            /* host vehicle is doing a left turn -> heading range must be positive for object qualification */
            heading_threshold_min = p_ta_cal->k_fta_obj_heading[TA_MIN];
            heading_threshold_max = p_ta_cal->k_fta_obj_heading[TA_MAX];
         }
         else
         {
            /* host vehicle is doing a right turn -> heading range must be negative for object qualification */
            heading_threshold_min = -p_ta_cal->k_fta_obj_heading[TA_MAX];
            heading_threshold_max = -p_ta_cal->k_fta_obj_heading[TA_MIN];

            /* Heading threshold offset range also needs to be inverted */
            heading_threshold_min_offset = -p_ta_cal->k_fta_obj_heading_ofst[TA_MAX];
            heading_threshold_max_offset = -p_ta_cal->k_fta_obj_heading_ofst[TA_MIN];
         }

         /* Check if object heading value is in defined range and remains steady in world coordinates (approximated by object
          * heading difference to egos last straight section). */
         f_heading_in_range =
            (boolean_T) ((Fbk_Is_Float_In_Given_Range(heading_value, heading_threshold_min, heading_threshold_max, f_check_extended_range,
                                                      heading_threshold_min_offset, heading_threshold_max_offset))
                         && (Fbk_Is_Float_In_Given_Range(
                            p_ta_object->attributes.ego_heading_diff, p_ta_cal->k_fta_ego_obj_heading_diff[TA_MIN],
                            p_ta_cal->k_fta_ego_obj_heading_diff[TA_MAX], f_check_extended_range,
                            p_ta_cal->k_fta_ego_obj_heading_diff_ofst[TA_MIN], p_ta_cal->k_fta_ego_obj_heading_diff_ofst[TA_MAX])));

         /* Check that the object is in the defined speed range for turning */
         speed_threshold_min = p_ta_cal->k_fta_obj_speed[TA_MIN];
         speed_threshold_max = p_ta_cal->k_fta_obj_speed[TA_MAX];
         f_speed_in_range = Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.speed, speed_threshold_min, speed_threshold_max,
                                                        f_check_extended_range, speed_threshold_min_offset,
                                                        speed_threshold_max_offset);
      }
      else
      {
         /* Scenario is neither straight nor turning. */
      }

      /* Check heading against velocity heading */
      f_velocity_heading_diff_in_range =
         (boolean_T) (Fbk_Abs_F(p_ta_object->tracker_data.vcs_heading - p_ta_object->attributes.velocity_heading)
                      <= p_ta_cal->k_fta_obj_velocity_heading_diff_max);

      /* Object is relevant if heading and speed are in range and the general position is valid */
      f_obj_relevant_for_curvature = (boolean_T) (Fbk_Is_True(f_heading_in_range) && Fbk_Is_True(f_heading_rate_in_range)
                                                  && Fbk_Is_True(f_speed_in_range) && Fbk_Is_True(f_position_valid)
                                                  && Fbk_Is_True(f_velocity_heading_diff_in_range));
   }

   return f_obj_relevant_for_curvature;
}

static boolean_T Ta_Is_Vehicle_State_Relevant(const Ta_Object_T *p_ta_object,
                                              const Fbk_Vehicle_Data_T *p_vehicle_data,
                                              const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Return flag */
   boolean_T f_vehicle_state_relevant = FBK_FALSE;

   /* Object active flag */
   boolean_T f_obj_active;

   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ta_cal);

   /* Check previous object activity */
   f_obj_active = Ta_Is_Obj_Active(p_ta_object, TA_ALERT_MODE_BOTH);

   /* Check normal thresholds for new objects and extended thresholds for previously active objects */
   if ((Fbk_Is_Float_In_Given_Range(p_vehicle_data->host_speed, p_ta_cal->k_ta_ego_speed[TA_MIN], p_ta_cal->k_ta_ego_speed[TA_MAX],
                                    f_obj_active, p_ta_cal->k_ta_ego_speed_ofst[TA_MIN], p_ta_cal->k_ta_ego_speed_ofst[TA_MAX]))
       && (Fbk_Is_Float_In_Given_Range(Fbk_Abs_F(p_vehicle_data->yawrate), p_ta_cal->k_ta_ego_yawrate[TA_MIN],
                                       p_ta_cal->k_ta_ego_yawrate[TA_MAX], f_obj_active, p_ta_cal->k_ta_ego_yawrate_ofst[TA_MIN],
                                       p_ta_cal->k_ta_ego_yawrate_ofst[TA_MAX]))
       && (Fbk_Is_Float_In_Given_Range(
          p_vehicle_data->long_acc, p_ta_cal->k_ta_ego_long_acceleration[TA_MIN], p_ta_cal->k_ta_ego_long_acceleration[TA_MAX],
          f_obj_active, p_ta_cal->k_ta_ego_long_acceleration_ofst[TA_MIN], p_ta_cal->k_ta_ego_long_acceleration_ofst[TA_MAX])))
   {
      /* Vehicle State is generally relevant for TA */
      f_vehicle_state_relevant = FBK_TRUE;
   }

   return f_vehicle_state_relevant;
}

static void Ta_Update_Obj_Fta_Relevance(Ta_Object_T *p_ta_object,
                                        const Fbk_Vehicle_Data_T *p_vehicle_data,
                                        const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ta_cal);

   /* FTA Object Filter */
   if (Fbk_Is_True(p_ta_cal->k_f_fta_enable))
   {
      /* Check previous object activity */
      boolean_T f_obj_active = Ta_Is_Obj_Active(p_ta_object, TA_ALERT_MODE_FRONT);

      if ((Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.existence_probability, p_ta_cal->k_fta_obj_exist_prblty[TA_MIN],
                                       p_ta_cal->k_fta_obj_exist_prblty[TA_MAX], f_obj_active,
                                       p_ta_cal->k_fta_obj_exist_prblty_ofst[TA_MIN], p_ta_cal->k_fta_obj_exist_prblty_ofst[TA_MAX]))
          && (Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.vcs_vel_rel.x, p_ta_cal->k_fta_obj_vcs_long_vel_rel[TA_MIN],
                                          p_ta_cal->k_fta_obj_vcs_long_vel_rel[TA_MAX], f_obj_active,
                                          p_ta_cal->k_fta_obj_vcs_long_vel_rel_ofst[TA_MIN],
                                          p_ta_cal->k_fta_obj_vcs_long_vel_rel_ofst[TA_MAX]))
          && (Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.vcs_vel_rel.y, p_ta_cal->k_fta_obj_vcs_lat_vel_rel[TA_MIN],
                                          p_ta_cal->k_fta_obj_vcs_lat_vel_rel[TA_MAX], f_obj_active,
                                          p_ta_cal->k_fta_obj_vcs_lat_vel_rel_ofst[TA_MIN],
                                          p_ta_cal->k_fta_obj_vcs_lat_vel_rel_ofst[TA_MAX]))
          && (Fbk_Is_Float_In_Given_Range(
             p_ta_object->tracker_data.vcs_vel.x, p_ta_cal->k_fta_obj_vcs_long_vel[TA_MIN], p_ta_cal->k_fta_obj_vcs_long_vel[TA_MAX],
             f_obj_active, p_ta_cal->k_fta_obj_vcs_long_vel_ofst[TA_MIN], p_ta_cal->k_fta_obj_vcs_long_vel_ofst[TA_MAX]))
          && (Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.vcs_vel.y, p_ta_cal->k_fta_obj_vcs_lat_vel[TA_MIN],
                                          p_ta_cal->k_fta_obj_vcs_lat_vel[TA_MAX], f_obj_active,
                                          p_ta_cal->k_fta_obj_vcs_lat_vel_ofst[TA_MIN], p_ta_cal->k_fta_obj_vcs_lat_vel_ofst[TA_MAX]))
          && (Ta_Is_Obj_Relevant_Regarding_Host_Curvature(p_ta_object, f_obj_active, p_vehicle_data, p_ta_cal))
          && (Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.length, p_ta_cal->k_fta_obj_length[TA_MIN],
                                          p_ta_cal->k_fta_obj_length[TA_MAX], f_obj_active,
                                          p_ta_cal->k_fta_obj_length_ofst[TA_MIN], p_ta_cal->k_fta_obj_length_ofst[TA_MAX]))
          && (Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.width, p_ta_cal->k_fta_obj_width[TA_MIN],
                                          p_ta_cal->k_fta_obj_width[TA_MAX], f_obj_active, p_ta_cal->k_fta_obj_width_ofst[TA_MIN],
                                          p_ta_cal->k_fta_obj_width_ofst[TA_MAX]))
          && (Fbk_Is_Float_In_Given_Range(p_ta_object->attributes.area, p_ta_cal->k_fta_obj_area[TA_MIN],
                                          p_ta_cal->k_fta_obj_area[TA_MAX], f_obj_active, p_ta_cal->k_fta_obj_area_ofst[TA_MIN],
                                          p_ta_cal->k_fta_obj_area_ofst[TA_MAX]))
          && (Fbk_Is_Float_In_Given_Range(p_ta_object->attributes.object_class_probability_vru,
                                          p_ta_cal->k_fta_obj_vru_class_prob[TA_MIN], p_ta_cal->k_fta_obj_vru_class_prob[TA_MAX],
                                          f_obj_active, p_ta_cal->k_fta_obj_vru_class_prob_ofst[TA_MIN],
                                          p_ta_cal->k_fta_obj_vru_class_prob_ofst[TA_MAX]))
          && (Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.eclipse_value, p_ta_cal->k_fta_obj_eclipse_value[TA_MIN],
                                          p_ta_cal->k_fta_obj_eclipse_value[TA_MAX], f_obj_active,
                                          p_ta_cal->k_fta_obj_eclipse_value_ofst[TA_MIN],
                                          p_ta_cal->k_fta_obj_eclipse_value_ofst[TA_MAX]))
          && (p_ta_object->tracker_data.age >= p_ta_cal->k_fta_obj_age_min))
      {
         if (Ta_Is_Obj_In_Danger_Zone(p_ta_object, p_ta_cal))
         {
            /* Update danger zone flag */
            p_ta_object->attributes.f_obj_in_danger_zone = FBK_TRUE;
         }
      }
   }
}

static void Ta_Update_Obj_Rta_Relevance(Ta_Object_T *p_ta_object, const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_ta_cal);

   /* RTA Object Filter */
   if (Fbk_Is_True(p_ta_cal->k_f_rta_enable))
   {
      if (Ta_Is_Obj_Rta_Relevant(p_ta_object, p_ta_cal))
      {
         /* Check info zone */
         if (Ta_Is_Obj_In_Info_Zone(p_ta_object, p_ta_cal))
         {
            p_ta_object->attributes.f_obj_in_info_zone = FBK_TRUE;
         }

         /* Check wing zone */
         if (Ta_Is_Obj_In_Wing_Zone(p_ta_object, p_ta_cal))
         {
            p_ta_object->attributes.f_obj_in_wing_zone = FBK_TRUE;
         }
      }
   }
}

static boolean_T Ta_Is_Obj_Rta_Relevant(const Ta_Object_T *p_ta_object, const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Return flag */
   boolean_T f_obj_rta_relevant = FBK_FALSE;

   /* Object active flag */
   boolean_T f_obj_active;

   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_ta_cal);

   /* Check previous object activity */
   f_obj_active = Ta_Is_Obj_Active(p_ta_object, TA_ALERT_MODE_REAR);

   if ((Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.existence_probability, p_ta_cal->k_rta_obj_exist_prblty[TA_MIN],
                                    p_ta_cal->k_rta_obj_exist_prblty[TA_MAX], f_obj_active,
                                    p_ta_cal->k_rta_obj_exist_prblty_ofst[TA_MIN], p_ta_cal->k_rta_obj_exist_prblty_ofst[TA_MAX]))
       && (Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.vcs_vel_rel.x, p_ta_cal->k_rta_obj_vcs_long_vel_rel[TA_MIN],
                                       p_ta_cal->k_rta_obj_vcs_long_vel_rel[TA_MAX], f_obj_active,
                                       p_ta_cal->k_rta_obj_vcs_long_vel_rel_ofst[TA_MIN],
                                       p_ta_cal->k_rta_obj_vcs_long_vel_rel_ofst[TA_MAX]))
       && (Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.vcs_vel_rel.y, p_ta_cal->k_rta_obj_vcs_lat_vel_rel[TA_MIN],
                                       p_ta_cal->k_rta_obj_vcs_lat_vel_rel[TA_MAX], f_obj_active,
                                       p_ta_cal->k_rta_obj_vcs_lat_vel_rel_ofst[TA_MIN],
                                       p_ta_cal->k_rta_obj_vcs_lat_vel_rel_ofst[TA_MAX]))
       && (Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.vcs_vel.x, p_ta_cal->k_rta_obj_vcs_long_vel[TA_MIN],
                                       p_ta_cal->k_rta_obj_vcs_long_vel[TA_MAX], f_obj_active,
                                       p_ta_cal->k_rta_obj_vcs_long_vel_ofst[TA_MIN], p_ta_cal->k_rta_obj_vcs_long_vel_ofst[TA_MAX]))
       && (Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.vcs_vel.y, p_ta_cal->k_rta_obj_vcs_lat_vel[TA_MIN],
                                       p_ta_cal->k_rta_obj_vcs_lat_vel[TA_MAX], f_obj_active,
                                       p_ta_cal->k_rta_obj_vcs_lat_vel_ofst[TA_MIN], p_ta_cal->k_rta_obj_vcs_lat_vel_ofst[TA_MAX]))
       && (Fbk_Is_Float_In_Given_Range(Fbk_Abs_F(p_ta_object->tracker_data.vcs_heading), p_ta_cal->k_rta_obj_heading[TA_MIN],
                                       p_ta_cal->k_rta_obj_heading[TA_MAX], f_obj_active, p_ta_cal->k_rta_obj_heading_ofst[TA_MIN],
                                       p_ta_cal->k_rta_obj_heading_ofst[TA_MAX]))
       && (Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.speed, p_ta_cal->k_rta_obj_speed[TA_MIN],
                                       p_ta_cal->k_rta_obj_speed[TA_MAX], f_obj_active, p_ta_cal->k_rta_obj_speed_ofst[TA_MIN],
                                       p_ta_cal->k_rta_obj_speed_ofst[TA_MAX]))
       && (Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.length, p_ta_cal->k_rta_obj_length[TA_MIN],
                                       p_ta_cal->k_rta_obj_length[TA_MAX], f_obj_active, p_ta_cal->k_rta_obj_length_ofst[TA_MIN],
                                       p_ta_cal->k_rta_obj_length_ofst[TA_MAX]))
       && (Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.width, p_ta_cal->k_rta_obj_width[TA_MIN],
                                       p_ta_cal->k_rta_obj_width[TA_MAX], f_obj_active, p_ta_cal->k_rta_obj_width_ofst[TA_MIN],
                                       p_ta_cal->k_rta_obj_width_ofst[TA_MAX]))
       && (Fbk_Is_Float_In_Given_Range(p_ta_object->attributes.object_class_probability_vru,
                                       p_ta_cal->k_rta_obj_vru_class_prob[TA_MIN], p_ta_cal->k_rta_obj_vru_class_prob[TA_MAX],
                                       f_obj_active, p_ta_cal->k_rta_obj_vru_class_prob_ofst[TA_MIN],
                                       p_ta_cal->k_rta_obj_vru_class_prob_ofst[TA_MAX]))
       && (Fbk_Is_Float_In_Given_Range(p_ta_object->tracker_data.eclipse_value, p_ta_cal->k_rta_obj_eclipse_value[TA_MIN],
                                       p_ta_cal->k_rta_obj_eclipse_value[TA_MAX], f_obj_active,
                                       p_ta_cal->k_rta_obj_eclipse_value_ofst[TA_MIN], p_ta_cal->k_rta_obj_eclipse_value_ofst[TA_MAX]))
       && (p_ta_object->tracker_data.age >= p_ta_cal->k_rta_obj_age_min) && Fbk_Is_False(p_ta_object->tracker_data.f_reflection))
   {
      f_obj_rta_relevant = FBK_TRUE;
   }

   return f_obj_rta_relevant;
}

static boolean_T Ta_Is_Obj_In_Danger_Zone(const Ta_Object_T *p_ta_object, const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Return flag */
   boolean_T f_obj_in_danger_zone = FBK_FALSE;

   /* Initialize zone variables */
   Fbk_Field_Of_Interest_T danger_zone_left;
   Fbk_Field_Of_Interest_T danger_zone_right;

   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_ta_cal);

   if (Fbk_Is_True(p_ta_cal->k_f_fta_enable_danger_zones))
   {
      /* Construct zones */
      Ta_Create_Danger_Zones(&danger_zone_left, &danger_zone_right, p_ta_cal);

      /* Check if object center position is in field of interest and heading direction matches */
      if (Is_Point_In_Convex_Polygon_Ray_Casting_Method(danger_zone_left.points, danger_zone_left.size,
                                                        &(p_ta_object->tracker_data.vcs_pos))
          && (p_ta_object->tracker_data.vcs_heading >= FBK_ZERO_F))
      {
         f_obj_in_danger_zone = FBK_TRUE;
      }
      else if (Is_Point_In_Convex_Polygon_Ray_Casting_Method(danger_zone_right.points, danger_zone_right.size,
                                                             &(p_ta_object->tracker_data.vcs_pos))
               && (p_ta_object->tracker_data.vcs_heading <= FBK_ZERO_F))
      {
         f_obj_in_danger_zone = FBK_TRUE;
      }
      else
      {
         /* Object is not relevant for danger zones */
      }
   }

   return f_obj_in_danger_zone;
}

static boolean_T Ta_Is_Obj_In_Info_Zone(const Ta_Object_T *p_ta_object, const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Return flag */
   boolean_T f_obj_in_info_zone = FBK_FALSE;

   /* Initialize zone variables */
   Fbk_Field_Of_Interest_T info_zone_left;
   Fbk_Field_Of_Interest_T info_zone_right;
   Vector_2d_T point_to_check;

   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_ta_cal);

   if (Fbk_Is_True(p_ta_cal->k_f_rta_enable_info_zones))
   {
      /* Construct zones */
      Ta_Create_Info_Zones(&info_zone_left, &info_zone_right, p_ta_cal, Ta_Is_Obj_Active(p_ta_object, TA_ALERT_MODE_REAR));

      /* Check which object position to use */
      if (Fbk_Is_True(p_ta_object->attributes.f_curvi_available))
      {
         point_to_check = p_ta_object->tracker_data.curvi_pos;
      }
      else
      {
         point_to_check = p_ta_object->tracker_data.vcs_pos;
      }

      /* Check if point_to_check is in field of interest */
      if (Is_Point_In_Convex_Polygon_Ray_Casting_Method(info_zone_left.points, info_zone_left.size, &point_to_check)
          || Is_Point_In_Convex_Polygon_Ray_Casting_Method(info_zone_right.points, info_zone_right.size, &point_to_check))
      {
         f_obj_in_info_zone = FBK_TRUE;
      }
   }

   return f_obj_in_info_zone;
}

static boolean_T Ta_Is_Obj_In_Wing_Zone(const Ta_Object_T *p_ta_object, const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Return flag */
   boolean_T f_obj_in_wing_zone = FBK_FALSE;

   Fbk_Field_Of_Interest_T wing_zone_left;
   Fbk_Field_Of_Interest_T wing_zone_right;

   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_ta_cal);

   if (Fbk_Is_True(p_ta_cal->k_f_rta_enable_wing_zones))
   {
      /* Construct zones */
      Ta_Create_Wing_Zones(&wing_zone_left, &wing_zone_right, p_ta_cal, Ta_Is_Obj_Active(p_ta_object, TA_ALERT_MODE_REAR));

      /* Check if object center position is in field of interest */
      if (Is_Point_In_Convex_Polygon_Ray_Casting_Method(wing_zone_left.points, wing_zone_left.size, &(p_ta_object->tracker_data.vcs_pos))
          || Is_Point_In_Convex_Polygon_Ray_Casting_Method(wing_zone_right.points, wing_zone_right.size,
                                                           &(p_ta_object->tracker_data.vcs_pos)))
      {
         f_obj_in_wing_zone = FBK_TRUE;
      }
   }

   return f_obj_in_wing_zone;
}

/*============================================================================*\
 * EXPORTED FUNCTIONS
\*============================================================================*/

boolean_T Ta_Is_Obj_Active(const Ta_Object_T *p_ta_object, const Ta_Alert_Mode_T ta_alert_mode)
{
   /* Return flag */
   boolean_T f_is_active;

   /* The requested TA alert mode equals the objects alert mode */
   if (p_ta_object->attributes.ta_alert_mode == ta_alert_mode)
   {
      f_is_active = FBK_TRUE;
   }
   /* The requested TA alert mode is TA_MODE_BOTH (FTA or RTA) and the objects alert mode is not the default value */
   else if ((TA_ALERT_MODE_NONE != p_ta_object->attributes.ta_alert_mode) && (TA_ALERT_MODE_BOTH == ta_alert_mode))
   {
      f_is_active = FBK_TRUE;
   }
   /* Requested TA alert mode differs */
   else
   {
      f_is_active = FBK_FALSE;
   }

   return f_is_active;
}

void Ta_Update_Object_Relevance(Ta_Object_T *p_ta_object,
                                const Ta_Core_Input_T *p_ta_core_input,
                                const Fbk_Vehicle_Data_T *p_vehicle_data,
                                const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_ta_core_input);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ta_cal);

   /* Check if the host vehicle is in a suitable state for TA */
   if (Ta_Is_Vehicle_State_Relevant(p_ta_object, p_vehicle_data, p_ta_cal))
   {
      /* Update vehicle state flag */
      p_ta_object->attributes.f_vehicle_state_relevant = FBK_TRUE;

      /* Check enable state of FTA */
      if (Fbk_Is_True(p_ta_core_input->f_fta_enable))
      {
         /* Update the FTA specific relevance flags */
         Ta_Update_Obj_Fta_Relevance(p_ta_object, p_vehicle_data, p_ta_cal);
      }

      /* Check enable state of RTA */
      if (Fbk_Is_True(p_ta_core_input->f_rta_enable))
      {
         /* Update the RTA specific relevance flags */
         Ta_Update_Obj_Rta_Relevance(p_ta_object, p_ta_cal);
      }

      /* Update overall TA relevance flag */
      if (Fbk_Is_True(p_ta_object->attributes.f_obj_in_danger_zone) || Fbk_Is_True(p_ta_object->attributes.f_obj_in_info_zone)
          || Fbk_Is_True(p_ta_object->attributes.f_obj_in_wing_zone))
      {
         p_ta_object->attributes.f_obj_ta_relevant = FBK_TRUE;
      }
   }
}

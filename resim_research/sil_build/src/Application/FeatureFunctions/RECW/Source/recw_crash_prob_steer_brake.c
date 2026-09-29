/**
 * @file recw_crash_prob_steer_brake.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is crash probability calculation source file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "recw_crash_prob_steer_brake.h"
#include "fbk_guardrail_data_t.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_lookup_table_2d.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

/*===========================================================================*\
 * Local Defines
\*===========================================================================*/

#define RECW_ACC_MAX (20.0f)

/*===========================================================================*\
 * Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief returns the weakest longitudinal deceleration which is required for objects braking in order to avoid hitting host.
 *        Ttb is larger than Ttc, thus objects acceleration cannot be recalculated from the
 *        Ttc, thus first calculate the required objects deceleration without based on distance and velocities and then add in the
 * host acceleration.
 *
 * @return brake deceleration of target to avoid crash with host
 *
 * @SRS{SF-1705}
 * @SAE{SF-2959}
 * @SDD{SF-7882}
 * @verification{}
 */
static float32_T Recw_Get_Brake_Acc_To_Avoid_Rear_End_Collision(
   const float32_T vcs_long_acc /**< Longitudinal acceleration of ego */,
   const float32_T lon_rel_vel_obj /**< Longitudinal relative velocity of object */,
   const float32_T lon_pos_bumper_obj /**< Longitudinal front bumper position of object */);

/**
 * @brief Returns the weakest lateral acceleration which is required for
 *        objects steering in order to avoid hitting host. This is also dependend on checks
 *        whether the lanes beside the host are blocked.
 *
 * @return steering acceleration of target to avoid crash with host
 *
 * @SRS{SF-1705}
 * @SAE{SF-2959}
 * @SDD{SF-7883}
 * @verification{}
 */
static float32_T Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision(
   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
   const Fbk_Guardrail_Data_T guardrail_data[PA_OBJ_NUMBER_OF_GUARDRAILS] /**< FBK guard rail data */,
   const float32_T lon_rel_vel_obj /**< Longitudinal relative velocity of object */,
   const float32_T lat_rel_vel_obj /**< Lateral relative velocity of object */,
   const float32_T lon_pos_bumper_obj /**< Longitudinal front bumper position of object */,
   const float32_T corner_lat_pos_fl_obj /**< Lateral position of front left corner of object */,
   const float32_T corner_lat_pos_fr_obj /**< Lateral position of front right corner of object */,
   const float32_T width_obj /**< Width of object */,
   const Recw_Core_Calibration_T *p_cals /**< RECW calibrations */);

/**
 * @brief Checks if a valid guardrail is present on the corresponding side.
 *
 * @return true if valid guardrail is present, false otherwise
 *
 * @SRS{SF-1705}
 * @SAE{SF-2959}
 * @SDD{SF-8009}
 * @verification{}
 */
static boolean_T
Recw_Is_Valid_Guardrail_Present_On_Side(const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                        const Fbk_Guardrail_Data_T *p_guardrail_data /* FBK guardrail data of side to check */,
                                        const float32_T width_obj /**< Width of object */,
                                        const Recw_Core_Calibration_T *p_cals /**< RECW calibrations */);

/*===========================================================================*\
 * Global Functions Definitions
\*===========================================================================*/

void Recw_Calculate_Crash_Probabilities(Recw_Object_T *p_recw_object,
                                        const Fbk_Vehicle_Data_T *p_vehicle_data,
                                        const Fbk_Guardrail_Data_T guardrail_data[PA_OBJ_NUMBER_OF_GUARDRAILS],
                                        const Recw_Core_Calibration_T *p_cals)
{
   float32_T needed_brake_acceleration;    /* Brake Deceleration Probability */
   float32_T needed_steering_acceleration; /* Steering Away Probability */

   float32_T range_to_rear_bumper;
   float32_T corner_fl_obj;
   float32_T corner_fr_obj;
   float32_T lat_pos_front_object;
   float32_T lat_part_width_obj;

   float32_T effective_rel_lon_vel;
   float32_T effective_rel_lat_vel;

   /* Asserts */
   assert(NULL != p_recw_object);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cals);

   range_to_rear_bumper =
      (p_recw_object->tracker_data.vcs_pos.x + p_vehicle_data->host_length + (0.5f * p_recw_object->tracker_data.length));

   if (Fbk_Is_False(p_cals->k_recw_f_enable_heading_filter))
   {
      effective_rel_lon_vel = p_recw_object->tracker_data.vcs_vel_rel.x;
      effective_rel_lat_vel = p_recw_object->tracker_data.vcs_vel_rel.y;
   }
   else
   {
      effective_rel_lon_vel = p_recw_object->attributes.effective_rel_vel.x;
      effective_rel_lat_vel = p_recw_object->attributes.effective_rel_vel.y;
   }

   lat_pos_front_object = p_recw_object->tracker_data.vcs_pos.y
                          + (0.5f * Fast_Sin(p_recw_object->tracker_data.vcs_heading) * p_recw_object->tracker_data.length);
   lat_part_width_obj = 0.5f * Fast_Cos(p_recw_object->tracker_data.vcs_heading) * p_recw_object->tracker_data.width;
   corner_fl_obj      = lat_pos_front_object - lat_part_width_obj;
   corner_fr_obj      = lat_pos_front_object + lat_part_width_obj;

   needed_brake_acceleration = Recw_Get_Brake_Acc_To_Avoid_Rear_End_Collision(
      p_vehicle_data->long_acc, p_recw_object->tracker_data.vcs_vel_rel.x, range_to_rear_bumper);

   needed_steering_acceleration = Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision(
      p_vehicle_data, guardrail_data, effective_rel_lon_vel, effective_rel_lat_vel, range_to_rear_bumper, corner_fl_obj,
      corner_fr_obj, p_recw_object->tracker_data.width, p_cals);

   assert(RECW_K_RECW_LOOKUP_BRAKING_PROBABILITY_ARRAY_SIZE_DIM0 == RECW_K_RECW_LOOKUP_BRAKING_DECELERATION_ARRAY_SIZE_DIM0);

   p_recw_object->attributes.crash_prob_braking =
      Get_Value_From_2d_Lookup_Table(p_cals->k_recw_lookup_braking_deceleration, p_cals->k_recw_lookup_braking_probability,
                                     RECW_K_RECW_LOOKUP_BRAKING_PROBABILITY_ARRAY_SIZE_DIM0, needed_brake_acceleration);

   assert(RECW_K_RECW_LOOKUP_STEERING_PROBABILITY_ARRAY_SIZE_DIM0 == RECW_K_RECW_LOOKUP_STEERING_ACCELERATION_ARRAY_SIZE_DIM0);

   p_recw_object->attributes.crash_prob_steering =
      Get_Value_From_2d_Lookup_Table(p_cals->k_recw_lookup_steering_acceleration, p_cals->k_recw_lookup_steering_probability,
                                     RECW_K_RECW_LOOKUP_STEERING_PROBABILITY_ARRAY_SIZE_DIM0, needed_steering_acceleration);

   p_recw_object->attributes.crash_prob_combined =
      Min(p_recw_object->attributes.crash_prob_braking, p_recw_object->attributes.crash_prob_steering);
   p_recw_object->attributes.needed_brake_acceleration    = needed_brake_acceleration;
   p_recw_object->attributes.needed_steering_acceleration = needed_steering_acceleration;
}

/*===========================================================================*\
 * Local Functions Definitions
\*===========================================================================*/

static float32_T Recw_Get_Brake_Acc_To_Avoid_Rear_End_Collision(const float32_T vcs_long_acc,
                                                                const float32_T lon_rel_vel_obj,
                                                                const float32_T lon_pos_bumper_obj)
{
   float32_T needed_brake_acceleration;

   if ((lon_rel_vel_obj < FBK_ZERO_F) || (lon_pos_bumper_obj > FBK_ZERO_F))
   {
      needed_brake_acceleration = FBK_ZERO_F;
   }
   else if (lon_pos_bumper_obj > -EPSILON)
   {
      needed_brake_acceleration = RECW_ACC_MAX;
   }
   else
   {
      needed_brake_acceleration = -((0.5f * ((lon_rel_vel_obj * lon_rel_vel_obj) / lon_pos_bumper_obj)) + vcs_long_acc);
   }

   return needed_brake_acceleration;
}

static float32_T Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                                const Fbk_Guardrail_Data_T guardrail_data[PA_OBJ_NUMBER_OF_GUARDRAILS],
                                                                const float32_T lon_rel_vel_obj,
                                                                const float32_T lat_rel_vel_obj,
                                                                const float32_T lon_pos_bumper_obj,
                                                                const float32_T corner_lat_pos_fl_obj,
                                                                const float32_T corner_lat_pos_fr_obj,
                                                                const float32_T width_obj,
                                                                const Recw_Core_Calibration_T *p_cals)
{
   float32_T corner_lat_pos_rr_host = Fbk_Half(p_vehicle_data->host_width) * p_cals->k_recw_factor_ego_width;
   float32_T corner_lat_pos_rl_host = -corner_lat_pos_rr_host;
   float32_T needed_steering_acceleration;

   /* Asserts */
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cals);

   if ((lon_rel_vel_obj < FBK_ZERO_F) || (lon_pos_bumper_obj > FBK_ZERO_F))
   {
      needed_steering_acceleration = FBK_ZERO_F;
   }
   else if (Fbk_Abs_F(lon_pos_bumper_obj) < EPSILON)
   {
      if ((corner_lat_pos_fl_obj > corner_lat_pos_rr_host) || (corner_lat_pos_fr_obj < corner_lat_pos_rl_host))
      {
         needed_steering_acceleration = FBK_ZERO_F;
      }
      else
      {
         needed_steering_acceleration = RECW_ACC_MAX;
      }
   }
   else
   {

      float32_T pow2_reci_lon_pos_bumper_obj = 1.0f / (lon_pos_bumper_obj * lon_pos_bumper_obj);
      float32_T term1                        = 2.0f * lon_rel_vel_obj * lon_rel_vel_obj;
      float32_T term2                        = 2.0f * lon_rel_vel_obj * lon_pos_bumper_obj;
      float32_T acceleration_left;
      float32_T acceleration_right;

      if (Recw_Is_Valid_Guardrail_Present_On_Side(p_vehicle_data, &guardrail_data[PA_ENV_GUARDRAIL_SIDE_LEFT], width_obj, p_cals))
      {
         acceleration_left = -RECW_ACC_MAX;
      }
      else
      {
         acceleration_left =
            ((((corner_lat_pos_rl_host - corner_lat_pos_fr_obj) * term1) + (term2 * lat_rel_vel_obj)) * pow2_reci_lon_pos_bumper_obj)
            + p_vehicle_data->lat_acc;
      }

      if (Recw_Is_Valid_Guardrail_Present_On_Side(p_vehicle_data, &guardrail_data[PA_ENV_GUARDRAIL_SIDE_RIGHT], width_obj, p_cals))
      {
         acceleration_right = RECW_ACC_MAX;
      }
      else
      {
         acceleration_right =
            ((((corner_lat_pos_rr_host - corner_lat_pos_fl_obj) * term1) + (term2 * lat_rel_vel_obj)) * pow2_reci_lon_pos_bumper_obj)
            + p_vehicle_data->lat_acc;
      }

      needed_steering_acceleration = Min(-acceleration_left, acceleration_right);
   }

   return needed_steering_acceleration;
}

static boolean_T Recw_Is_Valid_Guardrail_Present_On_Side(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                         const Fbk_Guardrail_Data_T *p_guardrail_data,
                                                         const float32_T width_obj,
                                                         const Recw_Core_Calibration_T *p_cals)
{
   boolean_T f_valid_guardrail_present_on_side = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_vehicle_data);
   assert(NULL != p_guardrail_data);
   assert(NULL != p_cals);

   /* Check for valid guard rail */
   if ((Fbk_Is_True(p_cals->k_recw_f_make_use_of_guardrail)) && (Fbk_Is_True(p_guardrail_data->f_active))
       && (Fbk_Is_True(p_guardrail_data->f_present)) && (PA_OBJ_STATUS_MATURE == p_guardrail_data->status)
       && (Fbk_Abs_F(p_guardrail_data->lat_pos) < (width_obj + (Fbk_Half(p_vehicle_data->host_width)))))
   {
      f_valid_guardrail_present_on_side = FBK_TRUE;
   }

   return f_valid_guardrail_present_on_side;
}

/**
 * @file cta_braking_logic.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains implementation of braking logic in Cta.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Include
\*===========================================================================*/

#include "cta_braking_logic.h"
#include "cta_common_functions.h"
#include "cta_core_calibration_t.h"
#include "cta_core_input_t.h"
#include "cta_debug_interface.h"
#include "fbk_array_interpolation.h"
#include "fbk_macros.h"
#include "ml_interval.h"
#include "ml_line.h"
#include "ml_math.h"
#include "ml_saturated_math.h"
#include "pa_reuse.h"
#include <assert.h>

/*===========================================================================*\
* Local function declaration
\*===========================================================================*/


/**
 * @brief Calculates the host speed that could be changing during the response time period.
 *
 * @return host speed including acceleration induced change during response time
 *
 * @SRS{SF-242,SF-238}
 * @SAE{SF-2459}
 * @SDD{SF-3726}
 * @verification{Check that the host speed after dead time is correctly calculated and returned.}
 */
static float32_T Cta_Get_Host_Speed_After_Dead_Time(const Fbk_Vehicle_Data_T *p_vehicle_data /**<[in] host vehicle information*/,
                                                    const Cta_Core_Calibration_T *p_cta_cal /**<[in] calibration parameters*/);

/**
 * @brief Calculates braking deceleration and checks for enabling standstill flag.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-290874}
 * @verification{Check that the host speed after dead time is correctly calculated and returned.}
 */
static void Cta_Calculate_Object_Braking_Attributes(
   Cta_Object_Attributes_T *p_obj_att_highest_crit_side /**<[in] attributes of the most critical object */,
   const Cta_Core_Calibration_T *p_cta_cal,
   const float32_T host_velocity_start_braking /**<[in] host velocity*/,
   const float32_T v_max_ramp_in /**<[in] max ramp in velocity*/,
   const float32_T ramp_in_time /**<[in] ramp in time*/,
   const float32_T t_brake /**<[in] brake time */,
   const float32_T host_speed /**<[in] host speed */);

/**
 * @brief Calculates the braking time for a given, current host velocity depending on ramp in time and braking gradient.
 *
 * @return braking time depending on host velocity, ramp in time, braking gradient, and final deceleration
 *
 * @SRS{SF-242,SF-238}
 * @SAE{SF-2459}
 * @SDD{SF-3725}
 * @verification{Check that the braking time is correctly calculated and returned.}
 */
static float32_T Cta_Get_Braking_Time(const Cta_Core_Calibration_T *p_cta_cal /**<[in] calibration parameters*/,
                                      const float32_T host_velocity_start_braking /**<[in] host velocity*/,
                                      const float32_T v_max_ramp_in /**<[in] max ramp in velocity*/,
                                      const float32_T ramp_in_time /**<[in] time of ramp in phase*/);


/**
 * @brief Returns the host velocity dependent maximum safety distance threshold. This is needed so that a slow rolling host is not
 *        braked too early.
 *
 * @return upper maximum safety distance threshold in m of type float32_T
 *
 * @SRS{SF-242,SF-238}
 * @SAE{SF-2459}
 * @SDD{SF-3926}
 * @verification{Check that the upper maximum safety distance threshold is returned correctly.}
 */
static float32_T Cta_Get_Maximum_Safety_Dist_Thres(const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/,
                                                   const Cta_Core_Calibration_T *p_cta_cal /**<Cta calibration*/);

/**
 * @brief Calculates the braking distance for a given braking time, depening on braking gradient, response time.
 *
 * @return braking distance depending on host velocity, ramp in time, braking gradient, and final deceleration
 *
 * @SRS{SF-242,SF-238}
 * @SAE{SF-2459}
 * @SDD{SF-3724}
 * @verification{Check that the braking distance is correctly calculated and returned.}
 */
static float32_T Cta_Get_Braking_Distance(const Cta_Core_Calibration_T *p_cta_cal /**<[in] calibration parameters*/,
                                          const float32_T host_velocity_start_braking /**<[in] host velocity*/,
                                          const float32_T v_max_ramp_in /**<[in] max ramp in velocity*/,
                                          const float32_T braking_time /**<[in] braking time for given host velocity*/,
                                          const float32_T ramp_in_time /**<[in] time of ramp in phase*/);

/**
 * @brief Checks whether qualification cycles are sufficient and returns the resulting brake qualifier.
 *
 * @return True when qualification cycles have been reached for brake indicator
 *
 * @SRS{SF-131}
 * @SAE{SF-2459}
 * @SDD{SF-3728}
 * @verification{Check that the brake decision is correctly qualified and returned.}
 */
static boolean_T
Cta_Qualify_Brake_Decision(uint8_t *p_brake_qual_qualification_ctr /**< qualification counter for the respective side*/,
                           const Cta_Core_Calibration_T *p_cta_cal /**<calibration parameters*/,
                           const boolean_T f_input_brake_qualifier /**< brake qualifier which shall be qualified*/);

/**
 * @brief Checks whether holding logic needs to be applied to the brake qualifier and returns the resulting brake qualifier.
 *
 * @return True when brake qualifier turned to False and in the previous cycle a braking qualifier was set to True
 *
 * @SRS{SF-235}
 * @SAE{SF-2459}
 * @SDD{SF-3727}
 * @verification{Check that the brake decision is correctly held and returned.}
 */
static boolean_T Cta_Hold_Brake_Decision(uint8_t *p_brake_qual_hold_ctr /**< qualification counter for the respective side*/,
                                         boolean_T *p_last_side_brake_qualifier /**< brake qualifier of the previous cycle*/,
                                         const Cta_Core_Calibration_T *p_cta_cal /**<calibration parameters*/,
                                         const boolean_T f_input_brake_qualifier /**< brake qualifier which shall be qualified*/);


/**
 * @brief Returns the distance to driving tube after brake process.
 *
 * @return braking distance extended host length in m
 *
 * @SRS{SF-242,SF-238}
 * @SAE{SF-2459}
 * @SDD{SF-3729}
 * @verification{Check that the objects distance to the driving tube is correctly calculated and returned.}
 */
static float32_T Cta_Return_Distance_To_Driving_Tube(Cta_Object_Attributes_T *p_obj_att_highest_crit_side /**< Object attributes*/,
                                                     const float32_T braking_distance /**< brake qualifier of the previous cycle*/,
                                                     const Cta_Core_Calibration_T *p_cta_cal /**< Cta calibration*/,
                                                     const Fbk_Vehicle_Data_T *p_vehicle_data /**< Information on host vehicle*/,
                                                     const Cta_Mode_T cta_mode_idx /**< cta mode */);


/*===========================================================================*\
* Global function definition
\*===========================================================================*/

/* clang-format off */
boolean_T Cta_Shall_Brake_Qualifier_Be_Set(uint8_t *p_brake_qual_hold_ctr,
                                           uint8_t *p_brake_qual_qualification_ctr,
                                           boolean_T *p_last_side_brake_qualifier,
                                           Cta_Object_Attributes_T *p_obj_att_highest_crit_side,
                                           const Cta_Instance_T *p_cta_instance,
                                           /* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed]*/
                                           const uint8_t approach_side,
                                           const Cta_Mode_T cta_mode_idx,
                                           const Fbk_Vehicle_Data_T *p_vehicle_data)
/* clang-format on */
{
   const Cta_Core_Calibration_T *p_cta_cal = &p_cta_instance->calibration;
   boolean_T f_ctb_qualifier               = FBK_FALSE;
   float32_T host_speed_after_dead_time;
   float32_T t_brake;
   float32_T t_brake_min;
   float32_T t_brake_max;
   float32_T v_max_ramp_in;

   /* Asserts */
   assert(NULL != p_cta_cal);

   if (NULL != p_obj_att_highest_crit_side)
   {
      if (Fbk_Abs_F(p_vehicle_data->host_speed) > EPSILON)
      {
         /* Decide if use TTP or TTC to check if the braking is feasible at all. */
         /* clang-format off */
         t_brake_min = ((CTA_STOP_MODE_TTC == p_cta_instance->core_input.cta_stop_mode) ? p_obj_att_highest_crit_side->ttc : p_obj_att_highest_crit_side->ttp)
                       - p_cta_cal->k_ctb_responsetime_brake_actuation;
         /* clang-format on */
         t_brake_max = p_obj_att_highest_crit_side->ttc - p_cta_cal->k_ctb_responsetime_brake_actuation;

         if ((t_brake_min >= p_cta_cal->k_ctb_min_braking_time) && (t_brake_max <= p_cta_cal->k_ctb_max_braking_time))
         {
            boolean_T f_rctb_time_qualifier     = FBK_FALSE;
            boolean_T f_rctb_distance_qualifier = FBK_FALSE;
            float32_T distance_to_driving_tube;
            float32_T brake_distance;
            float32_T braking_gradient;
            float32_T t_event;
            float32_T maximum_safety_distance_thres;
            float32_T ramp_in_time = Max(p_cta_cal->k_ctb_ramp_in_time, p_cta_cal->k_ctb_time_to_ask_for_final_brake_decel);
            /* Consider the brake respone time to calculate the host speed at the point where the braking can actually start. */
            host_speed_after_dead_time = Cta_Get_Host_Speed_After_Dead_Time(p_vehicle_data, p_cta_cal);

            /* Check whether the braking gradient was passed as a Cal value. */
            if (Fbk_Is_True(p_cta_cal->k_cta_f_use_brake_gradient))
            {
               braking_gradient = p_cta_cal->k_ctb_braking_jerk;
            }
            else
            {
               assert(ramp_in_time > FBK_ZERO_F);
               /* If not, then at least the final deceleration and the ramp-in time must be passed. */
               braking_gradient = p_cta_cal->k_ctb_const_decel_after_ramp_in / ramp_in_time;
            }

            /* Calculate the max. ego velocity that can be braked at during the ramp in phase. */
            v_max_ramp_in = 0.5f * braking_gradient * ramp_in_time * ramp_in_time;

            /* Calculate the necessary brake time and braking distance for the current ego velocity. */
            t_brake        = Cta_Get_Braking_Time(p_cta_cal, host_speed_after_dead_time, v_max_ramp_in, ramp_in_time);
            brake_distance = Cta_Get_Braking_Distance(p_cta_cal, host_speed_after_dead_time, v_max_ramp_in, t_brake, ramp_in_time);

            /* The time for the entire braking event is the brake time plus the response time. */
            t_event = t_brake + p_cta_cal->k_ctb_responsetime_brake_actuation + p_cta_cal->k_ctb_event_time_buffer;

            /* Is it possible to brake before a crash occurs. */
            if (p_obj_att_highest_crit_side->ttc < t_event)
            {
               f_rctb_time_qualifier = FBK_TRUE;
            }

            /* Check if after braking the safety distance is ensured and that the brake is not triggered too early*/
            distance_to_driving_tube = Cta_Return_Distance_To_Driving_Tube(p_obj_att_highest_crit_side, brake_distance, p_cta_cal,
                                                                           p_vehicle_data, cta_mode_idx);

            /*Get host velocity dependent maximum safety distance*/
            maximum_safety_distance_thres = Cta_Get_Maximum_Safety_Dist_Thres(p_vehicle_data, p_cta_cal);

            if ((distance_to_driving_tube >= p_cta_cal->k_ctb_lower_safety_distance_thres)
                && (distance_to_driving_tube <= maximum_safety_distance_thres))
            {
               f_rctb_distance_qualifier = FBK_TRUE;
            }

            /* If both brake conditions are fulfilled, then we can make use of the crash probabilities to determine the best time
             * to apply the braking*/
            if (Fbk_Is_True(f_rctb_time_qualifier) && Fbk_Is_True(f_rctb_distance_qualifier))
            {
               f_ctb_qualifier = FBK_TRUE;
               Cta_Calculate_Object_Braking_Attributes(p_obj_att_highest_crit_side, p_cta_cal, host_speed_after_dead_time,
                                                       v_max_ramp_in, ramp_in_time, t_brake, p_vehicle_data->host_speed);
            }

            /*Debug conditions for the ctb qualifier*/
            Binary_Cta_Debug_Pass_Ctb_Condition_Infos(f_rctb_distance_qualifier, f_rctb_time_qualifier, distance_to_driving_tube,
                                                      t_event, p_cta_cal->k_ctb_lower_safety_distance_thres,
                                                      maximum_safety_distance_thres, p_obj_att_highest_crit_side->approach_side,
                                                      cta_mode_idx);
         }
      }
   }

   /* Qualify the ctb_qualifier with qualification cycles*/
   f_ctb_qualifier = Cta_Qualify_Brake_Decision(p_brake_qual_qualification_ctr, p_cta_cal, f_ctb_qualifier);

   /* Holding logic for brake qualifier */
   f_ctb_qualifier = Cta_Hold_Brake_Decision(p_brake_qual_hold_ctr, p_last_side_brake_qualifier, p_cta_cal, f_ctb_qualifier);

   /*Debug ctb counters and use approach side check for that so that it is used within the build.*/
   if (FBK_SIDE_UNDEFINED != approach_side)
   {
      Binary_Cta_Debug_Pass_Ctb_Counters(*p_brake_qual_hold_ctr, *p_brake_qual_qualification_ctr, approach_side, cta_mode_idx);
   }
   return f_ctb_qualifier;
}


/*===========================================================================*\
* Local function definition
\*===========================================================================*/

static float32_T Cta_Get_Host_Speed_After_Dead_Time(const Fbk_Vehicle_Data_T *p_vehicle_data, const Cta_Core_Calibration_T *p_cta_cal)
{
   float32_T host_speed_after_dead_time;
   float32_T host_accel_weighted;

   /* Asserts */
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cta_cal);

   /* Get current host acceleration and calculate the host speed after the dead time period. */
   host_accel_weighted        = p_vehicle_data->long_acc * p_cta_cal->k_ctb_host_acc_weight;
   host_speed_after_dead_time = p_vehicle_data->host_speed + (host_accel_weighted * p_cta_cal->k_ctb_responsetime_brake_actuation);

   return Fbk_Abs_F(host_speed_after_dead_time);
}

static void Cta_Calculate_Object_Braking_Attributes(Cta_Object_Attributes_T *p_obj_att_highest_crit_side,
                                                    const Cta_Core_Calibration_T *p_cta_cal,
                                                    const float32_T host_velocity_start_braking,
                                                    const float32_T v_max_ramp_in,
                                                    const float32_T ramp_in_time,
                                                    const float32_T t_brake,
                                                    const float32_T host_speed)
{
   float32_T brake_deceleration;

   assert(NULL != p_obj_att_highest_crit_side);

   /* When host velocity is higherthan max velocity that can be safely braked at during the ramp in phase,
   We trigger standstill qualifier and calculating deceleration bases on host velocity - resulting in higher deceleration value
   if host velocity is lower than max value, standstill qualifier is not triggered and deceleration has lower value that changes
   linearly  */
   if (host_velocity_start_braking > v_max_ramp_in)
   {
      brake_deceleration                                  = host_velocity_start_braking / t_brake;
      p_obj_att_highest_crit_side->f_standstill_qualifier = FBK_TRUE;
   }
   else
   {
      brake_deceleration = host_speed / (t_brake - Fbk_Half(ramp_in_time));
   }

   brake_deceleration = Enforce_Range(Fbk_Abs_F(brake_deceleration), p_cta_cal->k_cta_min_deceleration_value,
                                      p_cta_cal->k_cta_max_deceleration_value);
   p_obj_att_highest_crit_side->brake_deceleration = brake_deceleration;
}

static float32_T Cta_Get_Braking_Time(const Cta_Core_Calibration_T *p_cta_cal,
                                      const float32_T host_velocity_start_braking,
                                      const float32_T v_max_ramp_in,
                                      const float32_T ramp_in_time)
{
   float32_T braking_time;

   assert(NULL != p_cta_cal);

   if (host_velocity_start_braking > v_max_ramp_in)
   {
      assert(p_cta_cal->k_ctb_const_decel_after_ramp_in > FBK_ZERO_F);
      /* the host can't come to a standstill during the ramp_in phase, thus we need the entire ramp_in time plus the time
       * necessary to come to a standstill when the max. deceleration is reached */
      braking_time = ramp_in_time + ((host_velocity_start_braking - v_max_ramp_in) / p_cta_cal->k_ctb_const_decel_after_ramp_in);
   }
   else
   {
      assert(p_cta_cal->k_ctb_braking_jerk > FBK_ZERO_F);
      /* the host can come to a standstill during the ramp_in phase, thus we calculate that time */
      braking_time = Fast_Sqrt(2.0f * host_velocity_start_braking / p_cta_cal->k_ctb_braking_jerk);
   }

   return braking_time;
}

static float32_T Cta_Get_Maximum_Safety_Dist_Thres(const Fbk_Vehicle_Data_T *p_vehicle_data, const Cta_Core_Calibration_T *p_cta_cal)
{
   float32_T maximum_safety_dist_thres;
   float32_T saturated_host_vel;
   uint8_t index;
   /* Asserts */
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cta_cal);

   /* Interpolate within the LUTs*/
   saturated_host_vel =
      Enforce_Range(Fbk_Abs_F(p_vehicle_data->host_speed), p_cta_cal->k_ctb_safety_dist_host_vel_lut[0],
                    p_cta_cal->k_ctb_safety_dist_host_vel_lut[CTA_K_CTB_SAFETY_DIST_HOST_VEL_LUT_ARRAY_SIZE_DIM0 - FBK_ONE_UINT]);
   index = Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr(p_cta_cal->k_ctb_safety_dist_host_vel_lut,
                                              CTA_K_CTB_SAFETY_DIST_HOST_VEL_LUT_ARRAY_SIZE_DIM0, saturated_host_vel);

   maximum_safety_dist_thres = Get_Y_Value_From_Line_By_Coordinates(
      p_cta_cal->k_ctb_safety_dist_host_vel_lut[index - 1u], p_cta_cal->k_ctb_upper_safety_distance_thres_lut[index - FBK_ONE_UINT],
      p_cta_cal->k_ctb_safety_dist_host_vel_lut[index], p_cta_cal->k_ctb_upper_safety_distance_thres_lut[index], saturated_host_vel);


   return maximum_safety_dist_thres;
}


static float32_T Cta_Get_Braking_Distance(const Cta_Core_Calibration_T *p_cta_cal,
                                          const float32_T host_velocity_start_braking,
                                          const float32_T v_max_ramp_in,
                                          const float32_T braking_time,
                                          const float32_T ramp_in_time)
{
   float32_T braking_distance;
   float32_T braking_dist_ramp_in_phase;
   /* calculate braking distance offset due to response time */
   float32_T braking_distance_const;

   assert(NULL != p_cta_cal);

   braking_distance_const = host_velocity_start_braking * p_cta_cal->k_ctb_responsetime_brake_actuation;
   /*Check whether host can come to standstill within the ramp in phase*/
   if (host_velocity_start_braking > v_max_ramp_in)
   {
      float32_T braking_dist_const_decel_phase;

      /*Since we do not come to standstill in ramp in phase it is only possible to reduce the velocity */
      braking_dist_ramp_in_phase = (host_velocity_start_braking * ramp_in_time)
                                   - ((FBK_ONE_F / 6.0f) * p_cta_cal->k_ctb_braking_jerk * ramp_in_time * ramp_in_time * ramp_in_time);

      /* Braking distance until we reach the standstill of host */
      braking_dist_const_decel_phase =
         0.5f * p_cta_cal->k_ctb_const_decel_after_ramp_in * (braking_time - ramp_in_time) * (braking_time - ramp_in_time);


      /* the braking distance is the const. distance we travel during the response time plus the distance we travel during
       * the braking time. This is split up into the distance travelled during ramp in phase and the distance travelled
       * with maximum deceleration reached */
      braking_distance = braking_distance_const + braking_dist_ramp_in_phase + braking_dist_const_decel_phase;
   }
   else
   {
      /* the braking distance is the const. distance we travel during the response time plus the distance we travel during
       * the braking time in ramp in phase. */
      braking_dist_ramp_in_phase = (host_velocity_start_braking * braking_time)
                                   - ((FBK_ONE_F / 6.0f) * p_cta_cal->k_ctb_braking_jerk * braking_time * braking_time * braking_time);
      braking_distance = braking_distance_const + braking_dist_ramp_in_phase;
   }

   return braking_distance;
}


static boolean_T Cta_Qualify_Brake_Decision(uint8_t *p_brake_qual_qualification_ctr,
                                            const Cta_Core_Calibration_T *p_cta_cal,
                                            const boolean_T f_input_brake_qualifier)

{
   boolean_T f_qualified_output = f_input_brake_qualifier;

   assert(NULL != p_brake_qual_qualification_ctr);
   assert(NULL != p_cta_cal);

   /* Qualify the ctb_qualifier with qualification cycles*/
   if (Fbk_Is_True(f_input_brake_qualifier))
   {
      Sat_Inc_Uint8(p_brake_qual_qualification_ctr);
   }
   else
   {
      *p_brake_qual_qualification_ctr = FBK_ZERO_UINT;
   }

   /* Debounce due to qualification counter */
   if (*p_brake_qual_qualification_ctr <= p_cta_cal->k_ctb_min_brake_qual_ctr_thres)
   {
      f_qualified_output = FBK_FALSE;
   }

   return f_qualified_output;
}


static boolean_T Cta_Hold_Brake_Decision(uint8_t *p_brake_qual_hold_ctr /**< qualification counter for the respective side*/,
                                         boolean_T *p_last_side_brake_qualifier /**< brake qualifier of the previous cycle*/,
                                         const Cta_Core_Calibration_T *p_cta_cal /**<calibration parameters*/,
                                         const boolean_T f_input_brake_qualifier /**< brake qualifier which shall be qualified*/)
{
   boolean_T f_hold_output = f_input_brake_qualifier;

   assert(NULL != p_brake_qual_hold_ctr);
   assert(NULL != p_cta_cal);

   if (Fbk_Is_True(f_input_brake_qualifier))
   {
      *p_brake_qual_hold_ctr = FBK_ZERO_UINT;
   }
   else
   {
      if (Fbk_Is_True((*p_last_side_brake_qualifier)))
      {
         if (*p_brake_qual_hold_ctr >= p_cta_cal->k_ctb_min_brake_hold_ctr_thres)
         {
            *p_brake_qual_hold_ctr = FBK_ZERO_UINT;
            f_hold_output          = FBK_FALSE;
         }
         else
         {
            Sat_Inc_Uint8(p_brake_qual_hold_ctr);
            f_hold_output = FBK_TRUE;
         }
      }
   }
   /* Save persistent state */
   *p_last_side_brake_qualifier = f_hold_output;

   return f_hold_output;
}


static float32_T Cta_Return_Distance_To_Driving_Tube(Cta_Object_Attributes_T *p_obj_att_highest_crit_side,
                                                     const float32_T braking_distance,
                                                     const Cta_Core_Calibration_T *p_cta_cal,
                                                     const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                     const Cta_Mode_T cta_mode_idx)
{
   float32_T brake_ext_host_length;
   float32_T distance_to_driving_tube;
   float32_T min_isec_point_dist;

   /* Since heading compensation is activatable ensure that the higher feature sensitivity is used here */
   if (Fbk_Is_False(p_cta_cal->k_cta_f_apply_heading_compensation_on_intersection_point))
   {

      Cta_Adapt_Intersec_Point_To_Object_Heading(p_obj_att_highest_crit_side, p_vehicle_data, p_cta_cal, cta_mode_idx);
   }

   if (CTA_MODE_FRONT == cta_mode_idx)
   {
      brake_ext_host_length = braking_distance;
   }
   else
   {
      brake_ext_host_length = p_vehicle_data->host_length + braking_distance;
   }

   min_isec_point_dist = Fbk_Min(Fbk_Abs_F(p_obj_att_highest_crit_side->long_isect_point_candidate[FBK_SIDE_LEFT][cta_mode_idx]),
                                 Fbk_Abs_F(p_obj_att_highest_crit_side->long_isect_point_candidate[FBK_SIDE_RIGHT][cta_mode_idx]));

   distance_to_driving_tube = min_isec_point_dist - brake_ext_host_length;

   return distance_to_driving_tube;
}

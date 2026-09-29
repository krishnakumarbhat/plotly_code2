/**
 * @file ced_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the honda post run logic for CED.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_post_run.h"
#include "ced_common_functions.h"
#include "ced_core_input_t.h"
#include "ced_core_output_t.h"
#include "ced_customer_calibration_t.h"
#include "ced_types.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include <assert.h>


#ifdef BINARY_DEBUG
#include "ced_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Ced_Output(const Ced_Output_T *p_ced_output);
#endif /* BINARY_DEBUG */

/**
 * @brief Calculate Honda custom ttc
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-70413}
 * @verification{}
 */
static void Ced_Calculate_Honda_Custom_Ttc(Ced_Output_T *p_ced_output /**< CED output data */,
                                           const Ced_Core_Output_T *p_ced_core_output /**< CED core output data */,
                                           const Ced_Customer_Calibration_T *p_ced_cal /**< CED calibration data */,
                                           const Pa_Data_T *p_pa_data /**< CED context data*/,
                                           const uint8_t side_index);
/**
 * @brief Calculate Honda custom ttc, and compare it to calibration threshold
 *
 * @return true if ttc is below threshold
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{Create test to verify that the comparison and verification is correct for Honda specific ttc }
 */
static boolean_T Ced_Check_Custom_Ttc_Is_In_Range(const Ced_Output_T *p_ced_output,            /**< CED output data */
                                                  const Ced_Customer_Calibration_T *p_ced_cal, /**< CED calibration data */
                                                  uint8_t side_idx /**< side index */);

/**
 * @brief Check if longitudinal position is below threshold
 *
 * @return true if ttc is below threshold
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{Create test to verify that the comparison and verification is correct for Honda specific ttc }
 */
static boolean_T Ced_Check_Long_Position(const Pa_Data_T *p_pa_data,                  /**< CED output data */
                                         const Ced_Core_Output_T *p_ced_core_output,  /**< CED core output data */
                                         const Ced_Customer_Calibration_T *p_ced_cal, /**< CED calibration data */
                                         uint8_t side_idx /**< side index */);

/**
 * @brief Set the door eratch signals
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{Create test with ced alerts on/off and different zone elatch input signals}
 */
static void Ced_Process_Eratch_Signals(Ced_Output_T *p_ced_output /**< CED output data */,
                                       const Ced_Input_T *p_ced_input /**< CED input data */,
                                       const Ced_Core_Output_T *p_ced_core_output /**< CED core output data */,
                                       const Ced_Customer_Calibration_T *p_ced_cal /**< CED calibration data */,
                                       const Pa_Data_T *p_pa_data);

/**
 * @brief Reset CED output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2403}
 * @SDD{SF-3502}
 * @verification{}
 */
static void Ced_Reset_Output(Ced_Output_T *p_ced_output, /**< CED output data */
                             uint8_t side /**< object side*/);

/**
 * @brief Map CED object direction to Nissan specific direction enum.
 *
 * @return OSE travel direction
 *
 * @SRS{}
 * @SAE{SF-2403}
 * @SDD{SF-3501}
 * @verification{}
 */
static Ose_Target_Travel_Direction_T
Ced_Map_Travel_Direction_To_Ose(const uint8_t ced_travel_direction /**< CED travel direction */);

/**
 * @brief Map CED alert level to Nissan specific alert level enum.
 *
 * @return OSE alert level
 *
 * @SRS{}
 * @SAE{SF-2403}
 * @SDD{SF-3496}
 * @verification{Create test to check is OSE alert levels are passed correctly}
 */
static Ose_Alert_T Ced_Map_Alert_Level_To_Ose(const Ced_Alert_T ced_alert_level /**< CED alert level */);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_ced_instance" points to a non-constant type.] */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ced_Post_Run_Init(Ced_Instance_T *p_ced_instance)
{
   assert(NULL != p_ced_instance);
}

// clang-format off
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_ced_instance" points to a non-constant type] */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ced_Post_Run(Ced_Instance_T *p_ced_instance, const Ced_Input_T *p_ced_input,  Ced_Output_T *p_ced_output)
// clang-format on
{
   boolean_T f_new_alert_on;
   uint8_t side_idx;
   boolean_T f_alert_drop;
   boolean_T f_alert_too_short;
   Ose_Alert_T *p_alert;
   Ose_Target_Travel_Direction_T *p_right;
   uint8_t *p_id;
   float32_T *p_ttc;
   float32_T *p_object_predicted_lat_pos;

   const Ced_Core_Output_T *p_ced_core_output;
   const Ced_Customer_Calibration_T *p_ced_cal;
   const Pa_Data_T *p_pa_data;

   assert(NULL != p_ced_instance);
   assert(NULL != p_ced_input);
   assert(NULL != p_ced_output);

   p_ced_core_output = &p_ced_instance->core_output;
   p_ced_cal         = &p_ced_instance->customer_calibration;
   p_pa_data         = p_ced_instance->core_input.p_pa_data;

   /* Set enable flag*/
   p_ced_output->f_ced_enable = Fbk_Convert_Bool_To_Uint(p_ced_input->f_ced_enable);

   for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      /* Get pointers to side related variables*/
      p_alert = (FBK_SIDE_RIGHT == side_idx) ? (&(p_ced_output->CED_alert_right)) : (&(p_ced_output->CED_alert_left));
      p_right = (FBK_SIDE_RIGHT == side_idx) ? (&(p_ced_output->CED_dir_right)) : (&(p_ced_output->CED_dir_left));
      p_id    = (FBK_SIDE_RIGHT == side_idx) ? (&(p_ced_output->CED_id_right)) : (&(p_ced_output->CED_id_left));
      p_ttc   = (FBK_SIDE_RIGHT == side_idx) ? (&(p_ced_output->CED_ttc_right)) : (&(p_ced_output->CED_ttc_left));
      p_object_predicted_lat_pos = (FBK_SIDE_RIGHT == side_idx) ? (&(p_ced_output->CED_object_predicted_lat_pos_right))
                                                                : (&(p_ced_output->CED_object_predicted_lat_pos_left));

      /* Get alert from core and process custom Honda TTC logic*/
      Ced_Calculate_Honda_Custom_Ttc(p_ced_output, p_ced_core_output, p_ced_cal, p_pa_data, side_idx);
      f_new_alert_on = (boolean_T) ((CED_NO_ALERT != p_ced_core_output->ced_alert[side_idx])
                                    && Ced_Check_Custom_Ttc_Is_In_Range(p_ced_output, p_ced_cal, side_idx)
                                    && Ced_Check_Long_Position(p_pa_data, p_ced_core_output, p_ced_cal, side_idx));

      /* Check if alert was too short*/
      f_alert_too_short = (boolean_T) (p_ced_output->CED_current_alert_duration[side_idx] < p_ced_cal->k_honda_min_alert_duration);

      /* Check if alert drops*/
      f_alert_drop = (boolean_T) (Fbk_Is_False(f_new_alert_on) && (OSE_NO_ALERT != *p_alert));

      /* Set holding flag */
      p_ced_output->CED_f_hold_alert[side_idx] = (boolean_T) (f_alert_too_short && f_alert_drop);

      /* If alert is on, set customer output*/
      if (f_new_alert_on)
      {
         *p_alert                    = Ced_Map_Alert_Level_To_Ose(p_ced_core_output->ced_alert[side_idx]);
         *p_right                    = Ced_Map_Travel_Direction_To_Ose(p_ced_core_output->ced_object_direction[side_idx]);
         *p_ttc                      = Fbk_Max(FBK_ZERO_F, p_ced_core_output->ced_ttc[side_idx]);
         *p_id                       = p_ced_core_output->ced_id[side_idx];
         *p_object_predicted_lat_pos = p_ced_core_output->ced_object_predicted_lat_pos[side_idx];
         p_ced_output->CED_current_alert_duration[side_idx] += p_pa_data->time_diff_to_last_cycle;
      }
      /* For holding, keep the current alert and object properties*/
      else if (p_ced_output->CED_f_hold_alert[side_idx])
      {
         p_ced_output->CED_current_alert_duration[side_idx] += p_pa_data->time_diff_to_last_cycle;
      }
      /* Reset the output*/
      else
      {
         Ced_Reset_Output(p_ced_output, side_idx);
      }
   }

   /* Process e-ratch /elatch signal info*/
   Ced_Process_Eratch_Signals(p_ced_output, p_ced_input, p_ced_core_output, p_ced_cal, p_pa_data);

#ifdef BINARY_DEBUG
   Write_Ced_Output(p_ced_output);
#endif
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Ced_Calculate_Honda_Custom_Ttc(Ced_Output_T *p_ced_output,
                                           const Ced_Core_Output_T *p_ced_core_output,
                                           const Ced_Customer_Calibration_T *p_ced_cal,
                                           const Pa_Data_T *p_pa_data,
                                           const uint8_t side_index)
{
   float32_T rear_bumper_position;
   float32_T distance_to_host_rear_bumper;
   float32_T obj_long_acceleration_weighted;
   float32_T time_to_travel_distance;
   float32_T abs_velocity;

   rear_bumper_position         = -p_pa_data->vehicle_data.host_length;
   distance_to_host_rear_bumper = rear_bumper_position - p_ced_core_output->ced_front_bumper_pos_long[side_index];
   obj_long_acceleration_weighted =
      p_ced_core_output->ced_vcs_vel_rel_x[side_index] * p_ced_cal->k_ced_honda_object_acceleration_weight;
   abs_velocity = Fbk_Abs_F(p_ced_core_output->ced_vcs_vel_rel_x[side_index]);
   time_to_travel_distance =
      Ced_Get_Time_To_Travel_Given_Distance(distance_to_host_rear_bumper, abs_velocity, obj_long_acceleration_weighted);

   p_ced_output->CED_honda_custom_ttc[side_index] = Fbk_Max(FBK_ZERO_F, time_to_travel_distance);
}

static void Ced_Reset_Output(Ced_Output_T *p_ced_output, uint8_t side)
{
   if (FBK_SIDE_LEFT == side)
   {
      p_ced_output->CED_alert_left                    = OSE_NO_ALERT;
      p_ced_output->CED_id_left                       = FBK_ZERO_UINT;
      p_ced_output->CED_ttc_left                      = CED_INVALID_TIME;
      p_ced_output->CED_dir_left                      = OSE_UNDEF_DIRECTION;
      p_ced_output->CED_object_predicted_lat_pos_left = CED_INVALID_DISTANCE;
      p_ced_output->CED_honda_custom_ttc[side]        = CED_INVALID_TIME;
   }
   else
   {
      p_ced_output->CED_alert_right                    = OSE_NO_ALERT;
      p_ced_output->CED_id_right                       = FBK_ZERO_UINT;
      p_ced_output->CED_ttc_right                      = CED_INVALID_TIME;
      p_ced_output->CED_dir_right                      = OSE_UNDEF_DIRECTION;
      p_ced_output->CED_object_predicted_lat_pos_right = CED_INVALID_DISTANCE;
      p_ced_output->CED_honda_custom_ttc[side]         = CED_INVALID_TIME;
   }
   p_ced_output->CED_current_alert_duration[side] = FBK_ZERO_F;
   p_ced_output->CED_f_hold_alert[side]           = FBK_FALSE;
}

static Ose_Target_Travel_Direction_T Ced_Map_Travel_Direction_To_Ose(const uint8_t ced_travel_direction)
{
   Ose_Target_Travel_Direction_T honda_travel_direction;

   switch (ced_travel_direction)
   {
      case FBK_SIDE_FRONT:
         honda_travel_direction = OSE_FRONT_DIRECTION;
         break;

      case FBK_SIDE_REAR:
         honda_travel_direction = OSE_REAR_DIRECTION;
         break;

      case FBK_SIDE_UNDEFINED:
         honda_travel_direction = OSE_UNDEF_DIRECTION;
         break;

      default:
         honda_travel_direction = OSE_UNDEF_DIRECTION;
         break;
   }

   return honda_travel_direction;
}

static Ose_Alert_T Ced_Map_Alert_Level_To_Ose(const Ced_Alert_T ced_alert_level)
{
   Ose_Alert_T honda_alert_level;

   switch (ced_alert_level)
   {
      case CED_NO_ALERT:
         honda_alert_level = OSE_NO_ALERT;
         break;

      case CED_ALERT_ACTIVE_LEVEL_1:
         honda_alert_level = OSE_ACTIVE_ALERT;
         break;

      default:
         honda_alert_level = OSE_NO_ALERT;
         break;
   }

   return honda_alert_level;
}

static boolean_T Ced_Check_Custom_Ttc_Is_In_Range(const Ced_Output_T *p_ced_output,
                                                  const Ced_Customer_Calibration_T *p_ced_cal,
                                                  uint8_t side_idx)
{
   float32_T custom_ttc_thresh;

   assert(NULL != p_ced_output);
   assert(NULL != p_ced_cal);


   custom_ttc_thresh = p_ced_cal->k_ced_honda_srr6_custom_ttc_alert_threshold[side_idx];

   if ((FBK_SIDE_LEFT == side_idx) && Fbk_Is_True(p_ced_output->CED_alert_left))
   {
      custom_ttc_thresh += p_ced_cal->k_ced_honda_srr6_custom_ttc_alert_hysteresis[side_idx];
   }
   if ((FBK_SIDE_RIGHT == side_idx) && Fbk_Is_True(p_ced_output->CED_alert_right))
   {
      custom_ttc_thresh += p_ced_cal->k_ced_honda_srr6_custom_ttc_alert_hysteresis[side_idx];
   }


   return (boolean_T) (Fbk_Is_False(p_ced_cal->k_ced_f_honda_use_alert_ttc_threshold)
                       || (p_ced_output->CED_honda_custom_ttc[side_idx] < custom_ttc_thresh));
}

static boolean_T Ced_Check_Long_Position(const Pa_Data_T *p_pa_data,
                                         const Ced_Core_Output_T *p_ced_core_output,
                                         const Ced_Customer_Calibration_T *p_ced_cal,
                                         uint8_t side_idx)
{
   float32_T host_length, object_long_pos, threshold;
   boolean_T result;
   assert(NULL != p_pa_data);
   assert(NULL != p_ced_cal);

   host_length     = p_pa_data->vehicle_data.host_length;
   object_long_pos = Fbk_Abs_F(p_ced_core_output->ced_front_bumper_pos_long[side_idx]);
   threshold       = Fbk_Abs_F(p_ced_cal->k_ced_honda_srr6_long_dist_threshold[side_idx] + host_length);
   result          = (boolean_T) (Fbk_Is_True(object_long_pos < threshold));

   return result;
}

static void Ced_Process_Eratch_Signals(Ced_Output_T *p_ced_output /**< CED output data */,
                                       const Ced_Input_T *p_ced_input /**< CED input data */,
                                       const Ced_Core_Output_T *p_ced_core_output /**< CED core output data */,
                                       const Ced_Customer_Calibration_T *p_ced_cal /**< CED calibration data */,
                                       const Pa_Data_T *p_pa_data)
{
   boolean_T f_alert_short;
   boolean_T f_next_alert_on;
   boolean_T f_last_alert_on;
   /* get e-latch zone width*/
   float32_T elatch_zone_width = (HONDA_EW_SHORT_ZONE == p_ced_input->ew_elatch_sense_stt)
                                    ? (p_ced_cal->k_honda_elatch_zones_width_table[FBK_ONE_UINT])
                                    : (p_ced_cal->k_honda_elatch_zones_width_table[FBK_ZERO_UINT]);

   /* LeftSide */
   /* Check if e-ratch alert was on in last cycle */
   f_last_alert_on = (boolean_T) (E_RATCH_ACTIVE_ALERT == p_ced_output->CED_eratch_alert_left);

   /* Calculate if next e-ratch alert should be triggered *
    * CED core alert should be ON, but without holding and object should be inside e-ratch zone*/
   f_next_alert_on =
      (boolean_T) ((OSE_NO_ALERT != p_ced_output->CED_alert_left) && (Fbk_Is_False(p_ced_output->CED_f_hold_alert[FBK_SIDE_LEFT]))
                   && (p_ced_core_output->ced_object_closest_lat_dist_predicted[FBK_SIDE_LEFT] < elatch_zone_width));

   /* Check if alert duration is too short*/
   f_alert_short =
      (boolean_T) (p_ced_output->CED_current_eratch_alert_duration[FBK_SIDE_LEFT] < p_ced_cal->k_honda_min_eratch_alert_duration);

   /* Set the Honda e-ratch alert if it can be set in next(this) cycle or it was active in last cycle and its duration was too low
    */
   if (f_next_alert_on || (f_last_alert_on && f_alert_short))
   {
      /*Set active alert and increase time counter*/
      p_ced_output->CED_eratch_alert_left = E_RATCH_ACTIVE_ALERT;
      p_ced_output->CED_current_eratch_alert_duration[FBK_SIDE_LEFT] += p_pa_data->time_diff_to_last_cycle;
   }
   else
   {
      /* Set no active alert and reset time counter*/
      p_ced_output->CED_eratch_alert_left                            = E_RATCH_NO_ALERT;
      p_ced_output->CED_current_eratch_alert_duration[FBK_SIDE_LEFT] = FBK_ZERO_F;
   }

   /* Right Side */
   /* Check if e-ratch alert was on in last cycle */
   f_last_alert_on = (boolean_T) (E_RATCH_ACTIVE_ALERT == p_ced_output->CED_eratch_alert_right);

   /* Calculate if next e-ratch alert should be triggered *
    * CED core alert should be ON, but without holding and object should be inside e-ratch zone*/
   f_next_alert_on =
      (boolean_T) ((OSE_NO_ALERT != p_ced_output->CED_alert_right) && (Fbk_Is_False(p_ced_output->CED_f_hold_alert[FBK_SIDE_RIGHT]))
                   && (p_ced_core_output->ced_object_closest_lat_dist_predicted[FBK_SIDE_RIGHT] < elatch_zone_width));

   /* Check if alert duration is too short*/
   f_alert_short =
      (boolean_T) (p_ced_output->CED_current_eratch_alert_duration[FBK_SIDE_RIGHT] < p_ced_cal->k_honda_min_eratch_alert_duration);

   /* Set the Honda e-ratch alert if it can be set in next(this) cycle or it was active in last cycle and its duration was too low
    */
   if (f_next_alert_on || (f_last_alert_on && f_alert_short))
   {
      /*Set active alert and increase time counter*/
      p_ced_output->CED_eratch_alert_right = E_RATCH_ACTIVE_ALERT;
      p_ced_output->CED_current_eratch_alert_duration[FBK_SIDE_RIGHT] += p_pa_data->time_diff_to_last_cycle;
   }
   else
   {
      /* Set no active alert and reset time counter*/
      p_ced_output->CED_eratch_alert_right                            = E_RATCH_NO_ALERT;
      p_ced_output->CED_current_eratch_alert_duration[FBK_SIDE_RIGHT] = FBK_ZERO_F;
   }
}

#ifdef BINARY_DEBUG
static void Write_Ced_Output(const Ced_Output_T *p_ced_output)
{
   uint8_t i;
   /* check input parameters */
   assert(NULL != p_ced_output);

   /* Log the CED customer output. */
   CED_STORE_VAL_MGR_WPR("honda_srr6_f_ced_enable", p_ced_output->f_ced_enable);
   CED_STORE_VAL_MGR_WPR("honda_srr6_ced_alert_left", p_ced_output->CED_alert_left);
   CED_STORE_VAL_MGR_WPR("honda_srr6_ced_id_left", p_ced_output->CED_id_left);
   CED_STORE_VAL_MGR_WPR("honda_srr6_ced_ttc_left", p_ced_output->CED_ttc_left);
   CED_STORE_VAL_MGR_WPR("honda_srr6_ced_dir_left", p_ced_output->CED_dir_left);
   CED_STORE_VAL_MGR_WPR("honda_srr6_ced_obj_predicted_lat_pos_left", p_ced_output->CED_object_predicted_lat_pos_left);
   CED_STORE_VAL_MGR_WPR("honda_srr6_ced_alert_right", p_ced_output->CED_alert_right);
   CED_STORE_VAL_MGR_WPR("honda_srr6_ced_id_right", p_ced_output->CED_id_right);
   CED_STORE_VAL_MGR_WPR("honda_srr6_ced_ttc_right", p_ced_output->CED_ttc_right);
   CED_STORE_VAL_MGR_WPR("honda_srr6_ced_dir_right", p_ced_output->CED_dir_right);
   CED_STORE_VAL_MGR_WPR("honda_srr6_ced_obj_predicted_lat_pos_right", p_ced_output->CED_object_predicted_lat_pos_right);
   CED_STORE_VAL_MGR_WPR("honda_srr6_ced_custom_ttc_right", p_ced_output->CED_honda_custom_ttc[FBK_SIDE_RIGHT]);
   CED_STORE_VAL_MGR_WPR("honda_srr6_ced_custom_ttc_left", p_ced_output->CED_honda_custom_ttc[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("honda_srr6_ced_eratch_alert_left", p_ced_output->CED_eratch_alert_left);
   CED_STORE_VAL_MGR_WPR("honda_srr6_ced_eratch_alert_right", p_ced_output->CED_eratch_alert_right);

   for (i = 0; i < FBK_NUMBER_OF_SIDES; i++)
   {
      CED_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_ced_hold_alert", p_ced_output->CED_f_hold_alert[i], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_ced_current_alert_duration", p_ced_output->CED_current_alert_duration[i], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_ced_current_eratch_alert_duration",
                                   p_ced_output->CED_current_eratch_alert_duration[i], i);
   }
}
#endif /* BINARY_DEBUG */

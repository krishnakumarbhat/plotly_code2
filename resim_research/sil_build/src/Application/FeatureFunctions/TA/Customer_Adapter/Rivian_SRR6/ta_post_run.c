/**
 * @file ta_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Rivian_SRR6 specific post run logic for TA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

#include "ta_post_run.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "ta_constants.h"
#include "ta_core_calibration_t.h"
#include "ta_core_input_t.h"
#include "ta_core_output_t.h"
#include "ta_types.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "ta_debug_writer.h"
#endif /* BINARY_DEBUG */

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/

#ifdef BINARY_DEBUG
static void Write_Ta_Output(const Ta_Output_T *p_ta_output);
#endif /* BINARY_DEBUG */

/**
 * @brief Maps the TA TTP and TTC from core output to the Rivian specific TA TTC
 *
 * @return Rivian output TTC
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-65065}
 */
static float32_T Ta_Map_TTC_To_Rivian(const Ta_Core_Output_T *p_ta_core_output, const uint8_t side_index);

/**
 * @brief Maps the TA status from core output to the Rivian specific TA status
 *
 * @return Rivian TA status
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-8635}
 */
static Ta_Rivian_Status_T Ta_Map_Status_To_Rivian(Ta_Algorithm_State_T ta_status, boolean_T f_ta_enable, boolean_T vehicle_state_valid);

/**
 * @brief Checks speed and yaw rate of ego vehicle if are in the specified range [min,max]
 *
 * @return True if vehicle state is valid
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-8634}
 */
static boolean_T Ta_Vehicle_State_Valid(const Pa_Data_T *p_pa_data, const Ta_Core_Calibration_T *p_ta_cal);

/*============================================================================*/
/*
 * EXPORTED FUNCTIONS
 */
/*============================================================================*/

void Ta_Post_Run_Init(void)
{
}

/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation]["p_ta_instance" does not modify the object it points to] */
void Ta_Post_Run(Ta_Instance_T *p_ta_instance,
                 const Ta_Input_T *p_ta_input /**< TA Input */,
                 Ta_Output_T *p_ta_output /**< TA Output */)
{
   const Ta_Core_Output_T *p_ta_core_output;
   const Ta_Core_Calibration_T *p_ta_cal;
   const Pa_Data_T *p_pa_data;
   uint8_t side_index;
   boolean_T vehicle_state_valid;

   /* Asserts */
   assert(NULL != p_ta_output);
   assert(NULL != p_ta_instance);
   assert(NULL != p_ta_input);

   p_ta_core_output = &p_ta_instance->core_output;
   p_ta_cal         = &p_ta_instance->calibration;
   p_pa_data        = p_ta_instance->core_input.p_pa_data;

   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      /* Map object properties */
      p_ta_output->ta_waypoint_at_collision[side_index] = p_ta_core_output->ta_waypoint_at_collision[side_index];
      p_ta_output->ta_ttc[side_index]                   = Ta_Map_TTC_To_Rivian(p_ta_core_output, side_index);
      p_ta_output->ta_ttp[side_index]                   = p_ta_core_output->ta_ttp[side_index];
      p_ta_output->ta_ttb[side_index]                   = p_ta_core_output->ta_ttb[side_index];
      p_ta_output->ta_decel_estimate[side_index]        = p_ta_core_output->ta_decel_estimate[side_index];
      p_ta_output->ta_distance[side_index]              = p_ta_core_output->ta_distance[side_index];

      /* Map alert level properties */
      if (TA_ALERT_STATE_NONE != p_ta_core_output->ta_alert_level[side_index])
      {
         p_ta_output->ta_alert[side_index] = FBK_TRUE;
      }
      else
      {
         p_ta_output->ta_alert[side_index] = FBK_FALSE;
      }
      p_ta_output->ta_id[side_index]     = p_ta_core_output->ta_id[side_index];
      p_ta_output->ta_index[side_index]  = p_ta_core_output->ta_index[side_index];
      p_ta_output->ta_most_critical_side = p_ta_core_output->ta_most_critical_side;

      /* Map zone flags */
      p_ta_output->ta_f_obj_in_danger_zone[side_index] = p_ta_core_output->ta_f_obj_in_danger_zone[side_index];
      p_ta_output->ta_f_obj_in_info_zone[side_index]   = p_ta_core_output->ta_f_obj_in_info_zone[side_index];
      p_ta_output->ta_f_obj_in_wing_zone[side_index]   = p_ta_core_output->ta_f_obj_in_wing_zone[side_index];
   }

   p_ta_output->ta_most_critical_side = p_ta_core_output->ta_most_critical_side;
   p_ta_output->ta_n_valid_objects    = p_ta_core_output->ta_n_valid_objects;
   p_ta_output->ta_n_relevant_objects = p_ta_core_output->ta_n_relevant_objects;
   p_ta_output->ta_n_critical_objects = p_ta_core_output->ta_n_critical_objects;

   vehicle_state_valid = Ta_Vehicle_State_Valid(p_pa_data, p_ta_cal);

   p_ta_output->ta_status =
      Ta_Map_Status_To_Rivian(p_ta_core_output->ta_algorithm_state, p_ta_input->f_ta_enable, vehicle_state_valid);

#ifdef BINARY_DEBUG
   Write_Ta_Output(p_ta_output);
#endif
}

/*============================================================================*\
 * Local Function Definition
\*============================================================================*/

static float32_T Ta_Map_TTC_To_Rivian(const Ta_Core_Output_T *p_ta_core_output, const uint8_t side_index)
{
   float32_T ta_rivian_ttc;

   /* Asserts */
   assert(NULL != p_ta_core_output);

   /* Check if TTC is valid, if not pass to the output TTP value */
   if (TA_INVALID_TTC > p_ta_core_output->ta_ttc[side_index])
   {
      ta_rivian_ttc = p_ta_core_output->ta_ttc[side_index];
   }
   else if (TA_INVALID_TTP > p_ta_core_output->ta_ttp[side_index])
   {
      ta_rivian_ttc = p_ta_core_output->ta_ttp[side_index];
   }
   else
   {
      ta_rivian_ttc = TA_INVALID_TTC;
   }

   return ta_rivian_ttc;
}

static Ta_Rivian_Status_T Ta_Map_Status_To_Rivian(Ta_Algorithm_State_T ta_status, boolean_T f_ta_enable, boolean_T vehicle_state_valid)
{
   Ta_Rivian_Status_T ta_rivian_status;

   /* Check if TA is enabled*/
   if (Fbk_Is_False(f_ta_enable))
   {
      ta_rivian_status = RIVIAN_TA_ALGORITHM_DISABLED;
   }
   /* Check if ego vehicle meets conditions*/
   else if (Fbk_Is_False(vehicle_state_valid))
   {
      ta_rivian_status = RIVIAN_TA_VEHICLE_STATE_INVALID;
   }
   else
   /* Rewrite the TA status to Rivian*/
   {
      switch (ta_status)
      {
         case TA_STATE_NO_VALID_OBJECTS:
            ta_rivian_status = RIVIAN_TA_NO_VALID_OBJECTS;
            break;

         case TA_STATE_VEHICLE_STATE_INVALID:
            ta_rivian_status = RIVIAN_TA_VEHICLE_STATE_INVALID;
            break;

         case TA_STATE_NO_RELEVANT_OBJECTS:
            ta_rivian_status = RIVIAN_TA_NO_RELEVANT_OBJECTS;
            break;

         case TA_STATE_NO_CRITICAL_OBJECTS:
            ta_rivian_status = RIVIAN_TA_NO_CRITICAL_OBJECTS;
            break;

         case TA_STATE_CRITICAL_OBJECT_DETECTED:
            ta_rivian_status = RIVIAN_TA_CRITICAL_OBJECT_DETECTED;
            break;

         case TA_STATE_ALGORITHM_DISABLED:
         default:
            ta_rivian_status = RIVIAN_TA_ALGORITHM_DISABLED;
            break;
      }
   }

   return ta_rivian_status;
}

static boolean_T Ta_Vehicle_State_Valid(const Pa_Data_T *p_pa_data, const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Set values*/
   boolean_T vehicle_state_is_valid = FBK_FALSE;
   float32_T host_speed             = p_pa_data->vehicle_data.host_speed;
   float32_T yawrate                = p_pa_data->vehicle_data.yawrate;

   /* Check if ego speed and yawrate are within limits */
   if ((host_speed >= p_ta_cal->k_ta_ego_speed[TA_MIN]) && (host_speed <= p_ta_cal->k_ta_ego_speed[TA_MAX])
       && (Fbk_Abs_F(yawrate) >= p_ta_cal->k_ta_ego_yawrate[TA_MIN]) && (Fbk_Abs_F(yawrate) <= p_ta_cal->k_ta_ego_yawrate[TA_MAX]))
   {
      vehicle_state_is_valid = FBK_TRUE;
   }

   return vehicle_state_is_valid;
}

#ifdef BINARY_DEBUG
static void Write_Ta_Output(const Ta_Output_T *p_ta_output)
{
   /* check input parameters */
   assert(NULL != p_ta_output);

   /* Log the TA customer output. */
   TA_STORE_VAL_MGR_WPR("rivian_ta_status", p_ta_output->ta_status);
   TA_STORE_VAL_MGR_WPR("rivian_ta_alert_left", p_ta_output->ta_alert[FBK_SIDE_LEFT]);
   TA_STORE_VAL_MGR_WPR("rivian_ta_alert_right", p_ta_output->ta_alert[FBK_SIDE_RIGHT]);
   TA_STORE_VAL_MGR_WPR("rivian_ta_ttc_left", p_ta_output->ta_ttc[FBK_SIDE_LEFT]);
   TA_STORE_VAL_MGR_WPR("rivian_ta_ttc_right", p_ta_output->ta_ttc[FBK_SIDE_RIGHT]);
}
#endif /* BINARY_DEBUG */

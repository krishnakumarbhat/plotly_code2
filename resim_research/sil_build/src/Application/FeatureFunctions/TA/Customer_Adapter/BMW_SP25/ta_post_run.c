/**
 * @file ta_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW SRR5 post run logic for TA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "ta_post_run.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "ml_checked_rounding.h"
#include "ml_line.h"
#include "ml_math.h"
#include "ml_saturated_math.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "ta_bmw_diagnostic.h"
#include "ta_bmw_enums.h"
#include "ta_bmw_sp25_debug_interface.h"
#include "ta_bmw_sp25_debug_writer.h"
#include "ta_bmw_sp25_types.h"
#include "ta_constants.h"
#include "ta_core_calibration_t.h"
#include "ta_core_input_t.h"
#include "ta_core_output_t.h"
#include "ta_state_machine.h"
#include "ta_types.h"
#include <assert.h>
#include <math.h>


/* LOCAL TYPEDEF --------------------------------------------------------------*/

typedef struct
{
   float32_T pfgs_prev_cycle_brake_deceleration_request;
   float32_T ta_host_speed_at_brake_start;
   float32_T ta_host_speed_reduction_requested;
   float32_T ta_host_speed_reduction_achieved;

   uint8_t pfgs_qualification_counter;
   uint8_t pfgs_qualification_counter_min;
   uint8_t fta_prev_alert_level;

} Ta_Bmw_Persistent_T;

/* LOCAL DEFINES FOR CONSTANTS -----------------------------------------------*/

#define TA_RTA_AREA_SYSTEM_LIMITS_MASK ((uint8_t) 0x01)
#define TA_RTA_AREA_RIGHT_CAR_IN_AREA_MASK ((uint8_t) 0x02)
#define TA_RTA_AREA_RIGHT_ALERT_LEVEL_MASK ((uint8_t) 0x04)
#define TA_RTA_AREA_LEFT_CAR_IN_AREA_MASK ((uint8_t) 0x08)
#define TA_RTA_AREA_LEFT_ALERT_LEVEL_MASK ((uint8_t) 0x10)

/* Defines for threshold values to set brake conditioning output */
#define TA_BMW_BRAKE_CONDITIONING_THRESHOLD_800_MS (0.8f)
#define TA_BMW_BRAKE_CONDITIONING_THRESHOLD_600_MS (0.6f)
#define TA_BMW_BRAKE_CONDITIONING_THRESHOLD_400_MS (0.4f)
#define TA_BMW_BRAKE_CONDITIONING_THRESHOLD_200_MS (0.2f)

/* Defines for gap target output values (Limitation, Invalid) */
#define TA_BMW_TARGET_GAP_INVALID (60.0f) /* divide 60.0 by 0.25 = 240 = F0h Invalid-Gap in valid range */
#define TA_BMW_TARGET_GAP_LIMIT_MAX (60.0f)

/* Defines for id output states (Invalid) */
#define TA_BMW_TARGET_ID_INVALID (252u) /* 252 = 0xFC Invalid-ID in valid range */

/* Map degree-input value to radiant-output value */
#define TA_DEG_TO_RAD(x) ((x) *PI / 180.0f)

/* Defines TAP default TTC value */
#define TA_TAP_DEFAULT_TTC (10.0f) /* 10 sec */

/* Defines maximum PFGS qualification counter */
#define TA_PFGS_QUALIFICATION_MAX (50u) /* 2.5 sec */

/* STATIC VARIABLES --------------------------------------------------------------*/

static Ta_Bmw_Persistent_T Ta_Bmw_Persistent;

/*============================================================================*\
* LOCAL FUNCTION PROTOTYPES
\*============================================================================*/

/**
 * @brief Calculates the maneuver direction state.
 *
 * @return maneuver direction state (straight or turn)
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3239}
 * @SDD{SF-8560}
 * @verification{}
 */
static Bmw_Maneuver_Direction_State_T Ta_Get_Maneuver_Direction(const Ta_Input_T *p_ta_input /**< TA Input */,
                                                                const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

/**
 * @brief Calculates the symbol request type.
 *
 * @return symbol request type
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3239}
 * @SDD{SF-8561}
 * @verification{}
 */
static Bmw_Symbol_Request_T Ta_Get_Symbol_Request(const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */,
                                                  const boolean_T f_obj_in_zone_left /**< Flag if object is in left zone */,
                                                  const boolean_T f_obj_in_zone_right /**< Flag if object is in right zone */);

/**
 * @brief Maps the output signals for front TA.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3239}
 * @SDD{SF-8562}
 * @verification{}
 */
static void Ta_Map_Fta_Output_Signals(Ta_Output_T *p_ta_output /**< TA Output */,
                                      const Ta_Input_T *p_ta_input /**< TA Input */,
                                      const Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */,
                                      const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */,
                                      const Pa_Data_T *p_pa_data);

/**
 * @brief Maps the output signals for rear TA.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3239}
 * @SDD{SF-8563}
 * @verification{}
 */
static void Ta_Map_Rta_Output_Signals(Ta_Output_T *p_ta_output /**< TA Output */,
                                      const Ta_Input_T *p_ta_input /**< TA Input */,
                                      const Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */,
                                      const Pa_Data_T *p_pa_data /**< FBK pa data */);

/**
 * @brief Qualify PFGS alert by checking additional info about vehicle state and tracker properties.
 * Increases Pfgs_Qualification_Counter for PFGS relevant situation or resets the counter.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3239}
 * @SDD{SF-8564}
 * @verification{}
 */
static void Ta_Qualify_Pfgs_Alert(const Pa_Data_T *p_pa_data /**< PA Context */,
                                  const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                  const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */,
                                  const uint8_t fta_index /**< FTA index */,
                                  const Ta_Alert_State_T fta_alert_level /**< TA Alert state */,
                                  const Pa_Obj_Status_T object_status /**< Object status */,
                                  const float32_T fta_ttc /**< FTA TTC */);

/**
 * @brief Checks that all PFGS qualification conditions are met.
 *
 * @return flag indicating a qualified PFGS alert
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3239}
 * @SDD{SF-8614}
 * @verification{}
 */
static boolean_T Ta_Is_Pfgs_Alert_Qualified(const Ta_Alert_State_T fta_alert_level /**< FTA alert level */,
                                            const float32_T obj_speed /**< Object speed */,
                                            const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Sets the warning related signals for PFGS.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3239}
 * @SDD{SF-8565}
 * @verification{}
 */
static void Ta_Set_Pfgs_Warning_Signals(Ta_Output_T *p_ta_output /**< TA Output */,
                                        const Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */,
                                        const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Sets the braking related signals for PFGS. Braking signal will have a ramp-up if set in calibration.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3239}
 * @SDD{SF-8566}
 * @verification{}
 */
static void Ta_Set_Pfgs_Braking_Signals(Ta_Output_T *p_ta_output /**< TA Output */,
                                        const Pa_Data_T *p_pa_data /**< PA Context */,
                                        const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Sets the brake attribute for FTA.
 *
 * @return void
 *
 * @SRS{SF-2279}
 * @SAE{SF-3239}
 * @SDD{SF-8567}
 * @verification{}
 */
static void Ta_Set_Fta_Brake_Conditioning(Ta_Output_T *p_ta_output /**< TA Output */, const float32_T fta_ttb);

/**
 * @brief Fills the object list with the data of the most critical tracker object.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3239}
 * @SDD{SF-8568}
 * @verification{}
 */
static void Ta_Fill_Fta_Relevant_Object_List(Ta_Output_T *p_ta_output /**< TA Output */,
                                             const Pa_Data_T *p_pa_data /**< PA Context */,
                                             const uint8_t fta_index);

/**
 * @brief Sets area status for RTA.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3239}
 * @SDD{SF-8569}
 * @verification{}
 */
static void Ta_Set_Rta_Area_Status(Ta_Output_T *p_ta_output /**< TA Output */,
                                   const uint8_t rta_alert_state,
                                   const boolean_T f_obj_in_dynamic_area,
                                   const boolean_T f_obj_in_turning_area,
                                   const uint8_t rta_area_in_area_mask,
                                   const uint8_t rta_area_in_alert_level_mask);

/**
 * @brief Resets the customer specific output of TA.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-3239}
 * @SDD{SF-8618}
 * @verification{}
 */
static void Ta_Reset_Output(Ta_Output_T *p_ta_output /**< TA Output */);

/**
 * @brief Set the BMW specific output signals of TA based on TA State-Machine Active State.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static void Ta_Set_Output_Bus_Signals_In_Active_State(Ta_Output_T *p_ta_output);

/**
 * @brief Set the BMW specific output signals related to Right Object in Active State.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static void Ta_Set_Output_Bus_Signals_Of_Right_Object_In_Active_State(Ta_Output_T *p_ta_output);

/**
 * @brief Set the BMW specific output signals related to Left Object in Active State.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static void Ta_Set_Output_Bus_Signals_Of_Left_Object_In_Active_State(Ta_Output_T *p_ta_output);

/**
 * @brief Resets the BMW specific output signals of TA based on State-Machine.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static void Ta_Reset_Output_Bus_Signals(Ta_Output_T *p_ta_output);

/**
 * @brief Resets the BMW specific output signals related to Right Objects of TA based on State-Machine.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static void Ta_Reset_Output_Bus_Signals_Of_Right_Object(Ta_Output_T *p_ta_output);

/**
 * @brief Resets the BMW specific output signals related to Left Objects of TA based on State-Machine.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static void Ta_Reset_Output_Bus_Signals_Of_Left_Object(Ta_Output_T *p_ta_output);

/**
 * @brief Resets the customer specific output of TA.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-3239}
 * @SDD{SF-8619}
 * @verification{}
 */
static void Ta_Reset_Post_Run_Persistent(Ta_Bmw_Persistent_T *p_ta_bmw_persistent /*Bmw_Sp25 specific persistent data*/);

/**
 * @brief Map BMW specific Criticality Level of Left Object As Per Alert State.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static TA_Object_Criticality_Level_T Ta_Map_Criticality_Of_Left_Object_As_Per_Alert_State(const Ta_Output_T *p_ta_output);

/**
 * @brief Map BMW specific Criticality Level of Right Object As Per Alert State.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static TA_Object_Criticality_Level_T Ta_Map_Criticality_Of_Right_Object_As_Per_Alert_State(const Ta_Output_T *p_ta_output);

/**
 * @brief Set TA Status bit .
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static void Ta_Set_Status_Based_On_Encoding(Ta_Output_T *p_ta_output);

/**
 * @brief Map Right TA Object Type As per Object Class.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static TA_Object_Type_T Ta_Map_Right_Object_Type_As_Per_Object_Class(const Ta_Output_T *p_ta_output);

/**
 * @brief Map Left TA Object Type As per Object Class
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static TA_Object_Type_T Ta_Map_Left_Object_Type_As_Per_Object_Class(const Ta_Output_T *p_ta_output);

/*============================================================================*\
* LOCAL FUNCTIONS
\*============================================================================*/

static void Ta_Set_Rta_Area_Status(Ta_Output_T *p_ta_output,
                                   const uint8_t rta_alert_state,
                                   const boolean_T f_obj_in_dynamic_area,
                                   const boolean_T f_obj_in_turning_area,
                                   const uint8_t rta_area_in_area_mask,
                                   const uint8_t rta_area_in_alert_level_mask)
{
   /* Only update area status if TA is not in alert holding and actual zone information is available */
   if (((uint8_t) TA_ALERT_STATE_NONE != rta_alert_state)
       && (Fbk_Is_True(f_obj_in_dynamic_area) || Fbk_Is_True(f_obj_in_turning_area)))
   {
      /* Check if valid object is inside Dynamic Area zone on the right */
      if (f_obj_in_dynamic_area)
      {
         /* object in dynamic area */
         p_ta_output->rta_dynamic_area_status |= rta_area_in_area_mask;
         if (rta_alert_state >= (uint8_t) TA_ALERT_STATE_LEVEL_2)
         {
            p_ta_output->rta_dynamic_area_status |= rta_area_in_alert_level_mask;
         }
         else
         {
            p_ta_output->rta_dynamic_area_status &= (uint8_t) ~(rta_area_in_alert_level_mask);
         }
      }
      else
      {
         /* no object in dynamic area */
         p_ta_output->rta_dynamic_area_status &= (uint8_t) ~(rta_area_in_area_mask);
         p_ta_output->rta_dynamic_area_status &= (uint8_t) ~(rta_area_in_alert_level_mask);
      }

      /* Check if valid object is inside Turning Area zone on the right */
      if (f_obj_in_turning_area)
      {
         /* object in turning area */
         p_ta_output->rta_turning_area_status |= rta_area_in_area_mask;

         if (rta_alert_state >= (uint8_t) TA_ALERT_STATE_LEVEL_2)
         {
            p_ta_output->rta_turning_area_status |= rta_area_in_alert_level_mask;
         }
         else
         {
            p_ta_output->rta_turning_area_status &= (uint8_t) ~(rta_area_in_alert_level_mask);
         }
      }
      else
      {
         /* no object in turning area */
         p_ta_output->rta_turning_area_status &= (uint8_t) ~(rta_area_in_area_mask);
         p_ta_output->rta_turning_area_status &= (uint8_t) ~(rta_area_in_alert_level_mask);
      }
   }
}

static void Ta_Qualify_Pfgs_Alert(const Pa_Data_T *p_pa_data,
                                  const Fbk_Vehicle_Data_T *p_vehicle_data,
                                  const Ta_Core_Calibration_T *p_ta_cal,
                                  const uint8_t fta_index,
                                  const Ta_Alert_State_T fta_alert_level,
                                  const Pa_Obj_Status_T object_status,
                                  const float32_T fta_ttc)
{
   /* Assert */
   assert(NULL != p_pa_data);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ta_cal);

   if (fta_alert_level >= TA_ALERT_STATE_LEVEL_2)
   {
      boolean_T f_host_in_relevant_speed_range = FBK_FALSE;
      boolean_T f_obj_stationary               = p_pa_data->object_data[fta_index].f_stationary;

      if ((p_vehicle_data->host_speed >= p_ta_cal->k_pfgs_ego_speed[TA_MIN])
          && (p_vehicle_data->host_speed <= p_ta_cal->k_pfgs_ego_speed[TA_MAX]))
      {
         /* Host speed is in PFGS relevant range */
         f_host_in_relevant_speed_range = FBK_TRUE;
      }

      if (Fbk_Is_True(f_host_in_relevant_speed_range) && (PA_OBJ_STATUS_MATURE == object_status)
          && (fta_ttc >= p_ta_cal->k_pfgs_qualification_ttc_min)
          && (Fbk_Is_False(p_ta_cal->k_pfgs_qualification_check_f_stationary) || Fbk_Is_False(f_obj_stationary)))
      {
         /* Increase qualification counter if the host vehicle is inside the PFGS-relevant speed range, the object is mature,
          * the TTC is still above the threshold and the object is not flagged stationary (optional) */
         Sat_Inc_Uint8(&(Ta_Bmw_Persistent.pfgs_qualification_counter));
      }
   }
   else if ((Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request < EPSILON) || (p_vehicle_data->host_speed < EPSILON)
            || (Ta_Bmw_Persistent.pfgs_qualification_counter >= TA_PFGS_QUALIFICATION_MAX))
   {
      /* Reset qualification counter and minimum threshold */
      Ta_Bmw_Persistent.pfgs_qualification_counter     = FBK_ZERO_UINT;
      Ta_Bmw_Persistent.pfgs_qualification_counter_min = FBK_ZERO_UINT;
   }
   else if (Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request > FBK_ZERO_F)
   {
      /* Pfgs_Qualification_Counter increases further if a brake request is still active */
      Sat_Inc_Uint8(&(Ta_Bmw_Persistent.pfgs_qualification_counter));
   }
   else
   {
      /* Keep Pfgs_Qualification_Counter on current value */
   }
}

static boolean_T Ta_Is_Pfgs_Alert_Qualified(const Ta_Alert_State_T fta_alert_level,
                                            const float32_T obj_speed,
                                            const Ta_Core_Calibration_T *p_ta_cal)
{
   boolean_T f_pfgs_alert_qualified = FBK_FALSE;

   /* Assert */
   assert(NULL != p_ta_cal);
   assert(p_ta_cal->k_pfgs_qualification_counter_fast_obj <= p_ta_cal->k_pfgs_qualification_counter_slow_obj);

   /* Check that alert level 3 or greater is set */
   if (fta_alert_level >= TA_ALERT_STATE_LEVEL_3)
   {
      /* Set object speed dependent minimum counter threshold */
      Ta_Bmw_Persistent.pfgs_qualification_counter_min = Roundf_Checked_Uint8(Get_Y_Value_From_Line_By_Coordinates(
         p_ta_cal->k_fta_obj_speed[TA_MIN], (float32_T) p_ta_cal->k_pfgs_qualification_counter_slow_obj,
         p_ta_cal->k_fta_obj_speed[TA_MAX], (float32_T) p_ta_cal->k_pfgs_qualification_counter_fast_obj, obj_speed));

      /* Clamp counter threshold between minimum and maximum value */
      Ta_Bmw_Persistent.pfgs_qualification_counter_min = Fbk_Clamp(Ta_Bmw_Persistent.pfgs_qualification_counter_min,
                                                                   p_ta_cal->k_pfgs_qualification_counter_fast_obj,
                                                                   p_ta_cal->k_pfgs_qualification_counter_slow_obj);

      /* Check the PFGS qualification counter against the threshold */
      if (Ta_Bmw_Persistent.pfgs_qualification_counter >= Ta_Bmw_Persistent.pfgs_qualification_counter_min)
      {
         f_pfgs_alert_qualified = FBK_TRUE;
      }
   }

   return f_pfgs_alert_qualified;
}

static void Ta_Set_Pfgs_Warning_Signals(Ta_Output_T *p_ta_output,
                                        const Ta_Core_Output_T *p_ta_core_output,
                                        const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Assert */
   assert(NULL != p_ta_output);
   assert(NULL != p_ta_core_output);
   assert(NULL != p_ta_cal);

   /* Set alert level for acute warning */
   p_ta_output->fta_alert_level = (uint8_t) BMW_ALERT_LEVEL_ACUTE_WARNING;

   /* Set threshold reduction to highest sensitivity */
   p_ta_output->fta_brake_threshold_reduction = (uint8_t) BMW_BRAKE_THRESHOLD_REDUCTION_HIGHEST_SENSITIVITY;

   /* Set symbol request output once */
   if ((uint8_t) BMW_SYMBOL_REQUEST_NO_WARNING == p_ta_output->fta_symbol_request)
   {
      p_ta_output->fta_symbol_request = (uint8_t) Ta_Get_Symbol_Request(p_ta_cal,
                                                                        p_ta_core_output->ta_f_obj_in_danger_zone[FBK_SIDE_LEFT],
                                                                        p_ta_core_output->ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT]);
   }
}

static void Ta_Set_Pfgs_Braking_Signals(Ta_Output_T *p_ta_output, const Pa_Data_T *p_pa_data, const Ta_Core_Calibration_T *p_ta_cal)
{
   float32_T decel_request_delta_to_prev_cycle;
   float32_T decel_request_max_delta;

   /* Assert */
   assert(NULL != p_ta_output);
   assert(NULL != p_ta_cal);

   /* Delta value between the previous and the current brake request */
   decel_request_delta_to_prev_cycle =
      p_ta_output->ta_current_deceleration_estimate - Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request;

   /* Maximum delta that is allowed between the previous and the current brake request */
   decel_request_max_delta = Fbk_Abs_F(p_ta_cal->k_fta_brake_gradient * p_pa_data->time_diff_to_last_cycle);

   if ((decel_request_delta_to_prev_cycle > decel_request_max_delta) && (Fbk_Is_True(p_ta_cal->k_f_fta_enable_brake_gradient_logic)))
   {
      /* Deceleration request delta to previous cycle exceeds limit for ramping up. Value is capped. */
      p_ta_output->fta_brake_deceleration_request =
         Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request + decel_request_max_delta;
   }
   else if ((decel_request_delta_to_prev_cycle < Fbk_Half(-decel_request_max_delta))
            && (Fbk_Is_True(p_ta_cal->k_f_fta_enable_brake_gradient_logic)))
   {
      /* Deceleration request delta to previous cycle exceeds limit for ramping down. Value is capped. */
      p_ta_output->fta_brake_deceleration_request =
         Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request - Fbk_Half(decel_request_max_delta);
   }
   else
   {
      /* Set brake request to calculated deceleration value */
      p_ta_output->fta_brake_deceleration_request = p_ta_output->ta_current_deceleration_estimate;
   }
}

static void Ta_Set_Fta_Brake_Conditioning(Ta_Output_T *p_ta_output, const float32_T fta_ttb)
{
   assert(NULL != p_ta_output);

   if (p_ta_output->fta_brake_deceleration_request > FBK_ZERO_F)
   {
      /* Brake request is active: Do not send brake conditioning in this case) */
      p_ta_output->fta_brake_conditioning = (uint8_t) BMW_BRAKE_CONDITIONING_NO_REQUEST;
   }
   else
   {
      if ((fta_ttb <= TA_BMW_BRAKE_CONDITIONING_THRESHOLD_800_MS) && (fta_ttb > TA_BMW_BRAKE_CONDITIONING_THRESHOLD_600_MS))
      {
         p_ta_output->fta_brake_conditioning = (uint8_t) BMW_BRAKE_CONDITIONING_REQUEST_IN_800_MS;
      }
      else if ((fta_ttb <= TA_BMW_BRAKE_CONDITIONING_THRESHOLD_600_MS) && (fta_ttb > TA_BMW_BRAKE_CONDITIONING_THRESHOLD_400_MS))
      {
         p_ta_output->fta_brake_conditioning = (uint8_t) BMW_BRAKE_CONDITIONING_REQUEST_IN_600_MS;
      }
      else if ((fta_ttb <= TA_BMW_BRAKE_CONDITIONING_THRESHOLD_400_MS) && (fta_ttb > TA_BMW_BRAKE_CONDITIONING_THRESHOLD_200_MS))
      {
         p_ta_output->fta_brake_conditioning = (uint8_t) BMW_BRAKE_CONDITIONING_REQUEST_IN_400_MS;
      }
      else if (fta_ttb <= TA_BMW_BRAKE_CONDITIONING_THRESHOLD_200_MS)
      {
         p_ta_output->fta_brake_conditioning = (uint8_t) BMW_BRAKE_CONDITIONING_REQUEST_IN_200_MS;
      }
      else
      {
         p_ta_output->fta_brake_conditioning = (uint8_t) BMW_BRAKE_CONDITIONING_NO_REQUEST;
      }
   }
}

static void Ta_Fill_Fta_Relevant_Object_List(Ta_Output_T *p_ta_output, const Pa_Data_T *p_pa_data, const uint8_t fta_index)
{
   /* Asserts */
   assert(NULL != p_ta_output);
   assert(NULL != p_pa_data);

   /* Only fill data for first object because TA only reports the most critical object */
   if (TA_BMW_TARGET_ID_INVALID != fta_index)
   {
      uint8_t idx = FBK_ZERO_UINT;

      p_ta_output->fta_relevant_object[idx].fta_obj_list_rcs         = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_id          = (float32_T) p_pa_data->object_data[fta_index].id;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_age         = (float32_T) p_pa_data->object_data[fta_index].age;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_meas_status = (float32_T) p_pa_data->object_data[fta_index].status;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_move_status = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_exist_prob  = p_pa_data->object_data[fta_index].existence_probability;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_ref_point   = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_ref_pnt_long_posn         = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_ref_pnt_long_posn_std_dev = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_ref_pnt_lat_posn          = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_ref_pnt_lat_posn_std_dev  = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_covariance_posn           = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_yaw_angle                 = p_pa_data->object_data[fta_index].vcs_heading;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_yaw_angle_std_dev  = p_pa_data->object_data[fta_index].heading_variance;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_long_vel           = p_pa_data->object_data[fta_index].vcs_vel.x;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_long_vel_std_dev   = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_lat_vel            = p_pa_data->object_data[fta_index].vcs_vel.y;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_lat_vel_std_dev    = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_covariance_vel     = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_long_accel         = p_pa_data->object_data[fta_index].vcs_accel.x;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_long_accel_std_dev = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_lat_accel          = p_pa_data->object_data[fta_index].vcs_accel.y;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_lat_accel_std_dev  = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_covariance_accel   = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_yawrate            = p_pa_data->object_data[fta_index].heading_rate;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_yawrate_std_dev    = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_length             = p_pa_data->object_data[fta_index].length;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_length_std_dev     = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_width              = p_pa_data->object_data[fta_index].width;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_width_std_dev      = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_object_class = (float32_T) p_pa_data->object_data[fta_index].obj_class;
   }
}

static void Ta_Map_Fta_Output_Signals(Ta_Output_T *p_ta_output,
                                      const Ta_Input_T *p_ta_input,
                                      const Ta_Core_Output_T *p_ta_core_output,
                                      const Ta_Core_Calibration_T *p_ta_cal,
                                      const Pa_Data_T *p_pa_data)
{
   float32_T fta_ttc = TA_INVALID_TTC;
   float32_T fta_ttb = TA_INVALID_TTB;
   float32_T fta_gap = TA_BMW_TARGET_GAP_INVALID;

   uint8_t fta_index = TA_BMW_TARGET_ID_INVALID;
   uint8_t fta_id    = TA_BMW_TARGET_ID_INVALID;

   uint8_t fta_side;
   Ta_Alert_State_T fta_alert_level = TA_ALERT_STATE_NONE;

   Pa_Obj_Status_T fta_obj_status = PA_OBJ_STATUS_INVALID;
   float32_T fta_obj_speed        = FBK_ZERO_F;

   const boolean_T f_warning_active =
      (boolean_T) (Fbk_Is_True((uint8_t) BMW_ALERT_LEVEL_ACUTE_WARNING == Ta_Bmw_Persistent.fta_prev_alert_level));
   const boolean_T f_brake_request_active =
      (boolean_T) (Fbk_Is_True(Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request > FBK_ZERO_F));

   const Fbk_Vehicle_Data_T *p_vehicle_data;

   /* Asserts */
   assert(NULL != p_ta_output);
   assert(NULL != p_ta_input);
   assert(NULL != p_ta_core_output);
   assert(NULL != p_ta_cal);

   /* Assert */
   assert(NULL != p_pa_data);

   p_ta_output->f_fta_enable = p_ta_input->f_fta_enable;
   p_vehicle_data            = &p_pa_data->vehicle_data;

   /* Check if collision critical object is detected in danger zone */
   if ((FBK_SIDE_UNDEFINED != p_ta_core_output->ta_most_critical_side)
       && (Fbk_Is_True(p_ta_core_output->ta_f_obj_in_danger_zone[FBK_SIDE_LEFT])
           || Fbk_Is_True(p_ta_core_output->ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT]) || Fbk_Is_True(f_warning_active)))
   {
      /* Fill FTA relevant information based on most critical side */
      fta_side        = p_ta_core_output->ta_most_critical_side;
      fta_id          = p_ta_core_output->ta_id[fta_side];
      fta_index       = p_ta_core_output->ta_index[fta_side];
      fta_alert_level = p_ta_core_output->ta_alert_level[fta_side];
      fta_ttc         = p_ta_core_output->ta_ttc[fta_side];
      fta_ttb         = p_ta_core_output->ta_ttb[fta_side];
      fta_gap         = p_ta_core_output->ta_distance[fta_side];

      /* Get object properties */
      fta_obj_status = p_pa_data->object_data[fta_index].status;
      fta_obj_speed  = p_pa_data->object_data[fta_index].speed;

      /* Get current deceleration estimate value that avoids a collision (also considers brake dead time) */
      p_ta_output->ta_current_deceleration_estimate = p_ta_core_output->ta_decel_estimate[fta_side];
   }

   /* Set Target Gap output */
   p_ta_output->fta_target_gap = fta_gap;
   if (p_ta_output->fta_target_gap > TA_BMW_TARGET_GAP_LIMIT_MAX)
   {
      /* Limit Target Gap value to 60 m */
      p_ta_output->fta_target_gap = TA_BMW_TARGET_GAP_LIMIT_MAX;
   }

   /* Set Target ID output */
   p_ta_output->fta_target_id = fta_id;
   if (PA_INVALID_OBJ_ID == p_ta_output->fta_target_id)
   {
      /* Be sure not to send out invalid-ID from CORE definition */
      p_ta_output->fta_target_id = TA_BMW_TARGET_ID_INVALID;
   }

   /* Increase qualifying counter for PFGS relevant situation or reset counter. */
   Ta_Qualify_Pfgs_Alert(p_pa_data, p_vehicle_data, p_ta_cal, fta_index, fta_alert_level, fta_obj_status, fta_ttc);

   /* Check that the PFGS alert is qualified */
   if ((Fbk_Is_True(Ta_Is_Pfgs_Alert_Qualified(fta_alert_level, fta_obj_speed, p_ta_cal))) || Fbk_Is_True(f_brake_request_active))
   {
      /* Set the PFGS warning signals */
      Ta_Set_Pfgs_Warning_Signals(p_ta_output, p_ta_core_output, p_ta_cal);

      if (((TA_ALERT_STATE_LEVEL_4 == fta_alert_level) && (PA_OBJ_STATUS_MATURE == fta_obj_status))
          || Fbk_Is_True(f_brake_request_active))
      {
         /* Set host vehicle speed at start of braking */
         if (FBK_ZERO_F == Ta_Bmw_Persistent.ta_host_speed_at_brake_start)
         {
            Ta_Bmw_Persistent.ta_host_speed_at_brake_start = p_vehicle_data->host_speed;
         }

         /* Update the achieved host speed reduction */
         Ta_Bmw_Persistent.ta_host_speed_reduction_achieved =
            Ta_Bmw_Persistent.ta_host_speed_at_brake_start - p_vehicle_data->host_speed;

         /* Add TA deceleration estimates to overall requested host speed reduction */
         Ta_Bmw_Persistent.ta_host_speed_reduction_requested =
            Ta_Bmw_Persistent.ta_host_speed_reduction_requested
            + (p_ta_output->ta_current_deceleration_estimate * p_pa_data->time_diff_to_last_cycle);

         /* Set the deceleration request */
         Ta_Set_Pfgs_Braking_Signals(p_ta_output, p_pa_data, p_ta_cal);
      }
      else
      {
         /* Reset deceleration request */
         p_ta_output->fta_brake_deceleration_request = FBK_ZERO_F;
      }

      /* Set brake conditioning */
      Ta_Set_Fta_Brake_Conditioning(p_ta_output, fta_ttb);
   }
   else
   {
      p_ta_output->ta_current_deceleration_estimate       = FBK_ZERO_F;
      Ta_Bmw_Persistent.ta_host_speed_at_brake_start      = FBK_ZERO_F;
      Ta_Bmw_Persistent.ta_host_speed_reduction_requested = FBK_ZERO_F;
      Ta_Bmw_Persistent.ta_host_speed_reduction_achieved  = FBK_ZERO_F;
   }

   /* Set deceleration request for previous cycle */
   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = p_ta_output->fta_brake_deceleration_request;

   /* Set current TTC value */
   p_ta_output->fta_ttc = fta_ttc;

   /* Fill data for the relevant object list */
   Ta_Fill_Fta_Relevant_Object_List(p_ta_output, p_pa_data, fta_index);
}

static void Ta_Map_Rta_Output_Signals(Ta_Output_T *p_ta_output,
                                      const Ta_Input_T *p_ta_input,
                                      const Ta_Core_Output_T *p_ta_core_output,
                                      const Pa_Data_T *p_pa_data)
{
   boolean_T f_rta_obj_in_dynamic_area_left;
   boolean_T f_rta_obj_in_dynamic_area_right;

   boolean_T f_rta_obj_in_turning_area_left;
   boolean_T f_rta_obj_in_turning_area_right;

   const Fbk_Vehicle_Data_T *p_vehicle_data;

   /* Asserts */
   assert(NULL != p_ta_output);
   assert(NULL != p_ta_input);
   assert(NULL != p_ta_core_output);
   assert(NULL != p_pa_data);

   /* Asserts */
   assert(NULL != p_pa_data);

   p_vehicle_data = &p_pa_data->vehicle_data;

   f_rta_obj_in_dynamic_area_left  = p_ta_core_output->ta_f_obj_in_info_zone[FBK_SIDE_LEFT];
   f_rta_obj_in_dynamic_area_right = p_ta_core_output->ta_f_obj_in_info_zone[FBK_SIDE_RIGHT];

   f_rta_obj_in_turning_area_left  = p_ta_core_output->ta_f_obj_in_wing_zone[FBK_SIDE_LEFT];
   f_rta_obj_in_turning_area_right = p_ta_core_output->ta_f_obj_in_wing_zone[FBK_SIDE_RIGHT];

   p_ta_output->f_rta_enable              = p_ta_input->f_rta_enable;
   p_ta_output->f_rta_enable_turning_area = p_ta_input->f_rta_enable_turning_area;
   p_ta_output->f_rta_enable_dynamic_area = p_ta_input->f_rta_enable_dynamic_area;

   p_ta_output->rta_id_left = p_ta_core_output->ta_id[FBK_SIDE_LEFT];
   if (PA_INVALID_OBJ_ID == p_ta_output->rta_id_left)
   {
      /* Be sure not to send out invalid-ID from CORE definition */
      p_ta_output->rta_id_left = TA_BMW_TARGET_ID_INVALID;
   }
   p_ta_output->rta_id_right = p_ta_core_output->ta_id[FBK_SIDE_RIGHT];
   if (PA_INVALID_OBJ_ID == p_ta_output->rta_id_right)
   {
      /* Be sure not to send out invalid-ID from CORE definition */
      p_ta_output->rta_id_right = TA_BMW_TARGET_ID_INVALID;
   }

   p_ta_output->rta_alert_left  = (uint8_t) p_ta_core_output->ta_alert_level[FBK_SIDE_LEFT];
   p_ta_output->rta_alert_right = (uint8_t) p_ta_core_output->ta_alert_level[FBK_SIDE_RIGHT];

   /* Set first bit to always equal one, can be overwritten in state machine */
   p_ta_output->rta_dynamic_area_status |= TA_RTA_AREA_SYSTEM_LIMITS_MASK; /* Interpretation_dynamischer_Bereich_innerhalb_Systemgrenzen
                                                                            */
   p_ta_output->rta_turning_area_status |= TA_RTA_AREA_SYSTEM_LIMITS_MASK; /* Interpretation_Abbiegebereich_innerhalb_Systemgrenzen
                                                                            */

   /* Right zones */
   if ((TA_BMW_TARGET_ID_INVALID != p_ta_output->rta_id_right) && ((uint8_t) TA_ALERT_STATE_NONE != p_ta_output->rta_alert_right))
   {
      uint8_t rta_index_right = p_ta_core_output->ta_index[FBK_SIDE_RIGHT];

      Ta_Set_Rta_Area_Status(p_ta_output, p_ta_output->rta_alert_right, f_rta_obj_in_dynamic_area_right,
                             f_rta_obj_in_turning_area_right, TA_RTA_AREA_RIGHT_CAR_IN_AREA_MASK, TA_RTA_AREA_RIGHT_ALERT_LEVEL_MASK);

      /* Fill RTA object details for right side (and transform to BMW coordinate system) */
      p_ta_output->rta_long_posn_right = p_pa_data->object_data[rta_index_right].vcs_pos.x - p_vehicle_data->rear_axle_position;
      p_ta_output->rta_lat_posn_right  = -1.0f * p_pa_data->object_data[rta_index_right].vcs_pos.y;
      p_ta_output->rta_long_vel_right  = p_pa_data->object_data[rta_index_right].vcs_vel.x;
      p_ta_output->rta_lat_vel_right   = -1.0f * p_pa_data->object_data[rta_index_right].vcs_vel.y;
      p_ta_output->rta_existence_probability_right =
         floorf((100.0f * p_pa_data->object_data[rta_index_right].existence_probability) + 0.5f);
      p_ta_output->rta_right_object_width  = p_pa_data->object_data[rta_index_right].width;
      p_ta_output->rta_right_object_length = p_pa_data->object_data[rta_index_right].length;
      p_ta_output->rta_right_object_type   = (uint8_t) p_pa_data->object_data[rta_index_right].obj_class;

      if (TA_INVALID_TTC > p_ta_core_output->ta_ttc[FBK_SIDE_RIGHT])
      {
         p_ta_output->rta_ttc_right = p_ta_core_output->ta_ttc[FBK_SIDE_RIGHT];
      }
      else if (TA_INVALID_TTP > p_ta_core_output->ta_ttp[FBK_SIDE_RIGHT])
      {
         p_ta_output->rta_ttc_right = p_ta_core_output->ta_ttp[FBK_SIDE_RIGHT];
      }
      else
      {
         p_ta_output->rta_ttc_right = TA_TAP_DEFAULT_TTC;
      }
   }

   /* Left zones */
   if ((TA_BMW_TARGET_ID_INVALID != p_ta_output->rta_id_left) && ((uint8_t) TA_ALERT_STATE_NONE != p_ta_output->rta_alert_left))
   {
      uint8_t rta_index_left = p_ta_core_output->ta_index[FBK_SIDE_LEFT];

      Ta_Set_Rta_Area_Status(p_ta_output, p_ta_output->rta_alert_left, f_rta_obj_in_dynamic_area_left,
                             f_rta_obj_in_turning_area_left, TA_RTA_AREA_LEFT_CAR_IN_AREA_MASK, TA_RTA_AREA_LEFT_ALERT_LEVEL_MASK);

      /* Fill RTA object details for left side (and transform to BMW coordinate system)*/
      p_ta_output->rta_long_posn_left = p_pa_data->object_data[rta_index_left].vcs_pos.x - p_vehicle_data->rear_axle_position;
      p_ta_output->rta_lat_posn_left  = -1.0f * p_pa_data->object_data[rta_index_left].vcs_pos.y;
      p_ta_output->rta_long_vel_left  = p_pa_data->object_data[rta_index_left].vcs_vel.x;
      p_ta_output->rta_lat_vel_left   = -1.0f * p_pa_data->object_data[rta_index_left].vcs_vel.y;
      p_ta_output->rta_existence_probability_left =
         floorf((100.0f * p_pa_data->object_data[rta_index_left].existence_probability) + 0.5f);
      p_ta_output->rta_left_object_width  = p_pa_data->object_data[rta_index_left].width;
      p_ta_output->rta_left_object_length = p_pa_data->object_data[rta_index_left].length;
      p_ta_output->rta_left_object_type   = (uint8_t) p_pa_data->object_data[rta_index_left].obj_class;

      if (TA_INVALID_TTC > p_ta_core_output->ta_ttc[FBK_SIDE_LEFT])
      {
         p_ta_output->rta_ttc_left = p_ta_core_output->ta_ttc[FBK_SIDE_LEFT];
      }
      else if (TA_INVALID_TTP > p_ta_core_output->ta_ttp[FBK_SIDE_LEFT])
      {
         p_ta_output->rta_ttc_left = p_ta_core_output->ta_ttp[FBK_SIDE_LEFT];
      }
      else
      {
         p_ta_output->rta_ttc_left = TA_TAP_DEFAULT_TTC;
      }
   }
}

static Bmw_Symbol_Request_T Ta_Get_Symbol_Request(const Ta_Core_Calibration_T *p_ta_cal,
                                                  const boolean_T f_obj_in_zone_left,
                                                  const boolean_T f_obj_in_zone_right)
{
   Bmw_Symbol_Request_T symbol_request = BMW_SYMBOL_REQUEST_PERSON_CENTRAL_CLOSE;

   /* check input parameters */
   assert(NULL != p_ta_cal);

   if (Fbk_Is_True(p_ta_cal->k_pfgs_symbol_request_sides_enabled))
   {
      /* coverity[misra_c_2012_rule_15_7_violation][Else-case shall never be reached and is protected with assert-statement] */
      if (Fbk_Is_True(f_obj_in_zone_left))
      {
         symbol_request = BMW_SYMBOL_REQUEST_PERSON_LEFT;
      }
      else if (Fbk_Is_True(f_obj_in_zone_right))
      {
         symbol_request = BMW_SYMBOL_REQUEST_PERSON_RIGHT;
      }
      else
      {
         /* In case we have no info about the object position we provide the default symbol. */;
      }
   }

   return symbol_request;
}

static Bmw_Maneuver_Direction_State_T Ta_Get_Maneuver_Direction(const Ta_Input_T *p_ta_input, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /* Init output to STRAIGHT */
   Bmw_Maneuver_Direction_State_T maneuver_direction = BMW_MANEUVER_DIRECTION_STRAIGHT;

   /* check input parameters */
   assert(NULL != p_ta_input);
   assert(NULL != p_vehicle_data);

   /* Set maneuver direction to TURN */
   if ((p_vehicle_data->steering_angle <= -TA_DEG_TO_RAD((float32_T) p_ta_input->fta_steering_angle_max_left))
       || (p_vehicle_data->steering_angle >= TA_DEG_TO_RAD((float32_T) p_ta_input->fta_steering_angle_max_right)))
   {
      maneuver_direction = BMW_MANEUVER_DIRECTION_TURN;
   }

   return maneuver_direction;
}

static void Ta_Reset_Output(Ta_Output_T *p_ta_output)
{
   uint8_t idx;

   /* Assert */
   assert(NULL != p_ta_output);

   /* Reset general signals */
   p_ta_output->f_diagnostic_mode                = FBK_FALSE;
   p_ta_output->ta_current_deceleration_estimate = FBK_ZERO_F;

   /* Reset general RTA signals */
   p_ta_output->rta_alert_left            = (uint8_t) TA_ALERT_STATE_NONE;
   p_ta_output->rta_alert_right           = (uint8_t) TA_ALERT_STATE_NONE;
   p_ta_output->rta_id_left               = TA_BMW_TARGET_ID_INVALID;
   p_ta_output->rta_id_right              = TA_BMW_TARGET_ID_INVALID;
   p_ta_output->f_rta_enable              = FBK_ZERO_UINT;
   p_ta_output->f_rta_enable_turning_area = FBK_ZERO_UINT;
   p_ta_output->f_rta_enable_dynamic_area = FBK_ZERO_UINT;

   /* Reset RTA object details for left side*/
   p_ta_output->rta_dynamic_area_status &= (uint8_t) ~(TA_RTA_AREA_LEFT_CAR_IN_AREA_MASK); /* Kein_Fahrzeug_im_linken_dynamischen_Bereich
                                                                                            */
   p_ta_output->rta_turning_area_status &= (uint8_t) ~(TA_RTA_AREA_LEFT_CAR_IN_AREA_MASK); /* Kein_Fahrzeug_im_linken_Abbiegebereich
                                                                                            */
   p_ta_output->rta_dynamic_area_status &= (uint8_t) ~(TA_RTA_AREA_LEFT_ALERT_LEVEL_MASK); /* Linker_dynamischer_Bereich_Infostufe
                                                                                            */
   p_ta_output->rta_turning_area_status &= (uint8_t) ~(TA_RTA_AREA_LEFT_ALERT_LEVEL_MASK); /* Linker_Abbiegebereich_Infostufe */

   p_ta_output->rta_long_posn_left             = FBK_ZERO_F;
   p_ta_output->rta_lat_posn_left              = FBK_ZERO_F;
   p_ta_output->rta_long_vel_left              = FBK_ZERO_F;
   p_ta_output->rta_lat_vel_left               = FBK_ZERO_F;
   p_ta_output->rta_existence_probability_left = FBK_ZERO_F;
   p_ta_output->rta_ttc_left                   = TA_TAP_DEFAULT_TTC;

   /* Reset RTA object details for right side */
   p_ta_output->rta_dynamic_area_status &= (uint8_t) ~(TA_RTA_AREA_RIGHT_CAR_IN_AREA_MASK); /* Kein_Fahrzeug_im_rechten_dynamischen_Bereich
                                                                                             */
   p_ta_output->rta_turning_area_status &= (uint8_t) ~(TA_RTA_AREA_RIGHT_CAR_IN_AREA_MASK); /* Kein_Fahrzeug_im_rechten_Abbiegebereich
                                                                                             */
   p_ta_output->rta_dynamic_area_status &= (uint8_t) ~(TA_RTA_AREA_RIGHT_ALERT_LEVEL_MASK); /* Rechter_dynamischer_Bereich_Infostufe
                                                                                             */
   p_ta_output->rta_turning_area_status &= (uint8_t) ~(TA_RTA_AREA_RIGHT_ALERT_LEVEL_MASK); /* Rechter_Abbiegebereich_Infostufe */

   p_ta_output->rta_long_posn_right             = FBK_ZERO_F;
   p_ta_output->rta_lat_posn_right              = FBK_ZERO_F;
   p_ta_output->rta_long_vel_right              = FBK_ZERO_F;
   p_ta_output->rta_lat_vel_right               = FBK_ZERO_F;
   p_ta_output->rta_existence_probability_right = FBK_ZERO_F;
   p_ta_output->rta_ttc_right                   = TA_TAP_DEFAULT_TTC;

   /* Reset general FTA signals */
   p_ta_output->f_fta_enable                   = FBK_ZERO_UINT;
   p_ta_output->fta_target_gap                 = TA_BMW_TARGET_GAP_INVALID;
   p_ta_output->fta_ttc                        = TA_INVALID_TTC;
   p_ta_output->fta_target_age                 = FBK_ZERO_F;
   p_ta_output->fta_target_vel_long            = FBK_ZERO_F;
   p_ta_output->fta_target_vel_lat             = FBK_ZERO_F;
   p_ta_output->fta_target_exist_prob          = FBK_ZERO_F;
   p_ta_output->fta_target_id                  = TA_BMW_TARGET_ID_INVALID;
   p_ta_output->fta_maneuver_direction         = (uint8_t) BMW_MANEUVER_DIRECTION_STRAIGHT;
   p_ta_output->fta_symbol_request             = (uint8_t) BMW_SYMBOL_REQUEST_NO_WARNING;     /* 0h: Keine_Warnung */
   p_ta_output->fta_alert_level                = (uint8_t) BMW_ALERT_LEVEL_NO_WARNING;        /* 0h: Keine_Warnung */
   p_ta_output->fta_brake_deceleration_request = FBK_ZERO_F;                                  /* no deceleration request */
   p_ta_output->fta_brake_conditioning         = (uint8_t) BMW_BRAKE_CONDITIONING_NO_REQUEST; /* 0h: No_request */
   p_ta_output->fta_brake_threshold_reduction  = (uint8_t) BMW_BRAKE_THRESHOLD_REDUCTION_DEFAULT_PARAM_DBC;

   /* Reset FTA relevant object list */
   for (idx = FBK_ZERO_UINT; idx < TA_N_FTA_OBJECTS; idx++)
   {
      p_ta_output->fta_relevant_object[idx].fta_obj_list_rcs                       = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_id                        = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_age                       = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_meas_status               = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_move_status               = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_exist_prob                = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_ref_point                 = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_ref_pnt_long_posn         = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_ref_pnt_long_posn_std_dev = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_ref_pnt_lat_posn          = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_ref_pnt_lat_posn_std_dev  = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_covariance_posn           = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_yaw_angle                 = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_yaw_angle_std_dev         = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_long_vel                  = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_long_vel_std_dev          = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_lat_vel                   = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_lat_vel_std_dev           = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_covariance_vel            = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_long_accel                = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_long_accel_std_dev        = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_lat_accel                 = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_lat_accel_std_dev         = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_covariance_accel          = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_yawrate                   = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_yawrate_std_dev           = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_length                    = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_length_std_dev            = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_width                     = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_width_std_dev             = FBK_ZERO_F;
      p_ta_output->fta_relevant_object[idx].fta_obj_list_object_class              = FBK_ZERO_F;
   }
}

static void Ta_Reset_Output_Bus_Signals_Of_Left_Object(Ta_Output_T *p_ta_output)
{
   p_ta_output->ta_output_bus_signal.ta_object_criticality_left_output    = NOT_CRITICAL;
   p_ta_output->ta_output_bus_signal.timestamp_left_object                = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.left_object_id                       = TA_BMW_TARGET_ID_INVALID;
   p_ta_output->ta_output_bus_signal.left_object_type                     = OBJECT_TYPE_UNKNOWN;
   p_ta_output->ta_output_bus_signal.left_object_position_x               = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.left_object_position_y               = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.left_object_width                    = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.left_object_length                   = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.left_object_velocity_x               = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.left_object_velocity_y               = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.left_object_ttc                      = TA_TAP_DEFAULT_TTC;
   p_ta_output->ta_output_bus_signal.left_object_ttb                      = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.left_object_ttp                      = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.left_object_existence_probability    = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.left_object_probability_of_collision = FBK_ZERO_F;
}

static void Ta_Reset_Output_Bus_Signals_Of_Right_Object(Ta_Output_T *p_ta_output)
{
   p_ta_output->ta_output_bus_signal.ta_object_criticality_right_output    = NOT_CRITICAL;
   p_ta_output->ta_output_bus_signal.timestamp_right_object                = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.right_object_id                       = TA_BMW_TARGET_ID_INVALID;
   p_ta_output->ta_output_bus_signal.right_object_type                     = OBJECT_TYPE_UNKNOWN;
   p_ta_output->ta_output_bus_signal.right_object_position_x               = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.right_object_position_y               = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.right_object_width                    = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.right_object_length                   = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.right_object_velocity_x               = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.right_object_velocity_y               = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.right_object_ttc                      = TA_TAP_DEFAULT_TTC;
   p_ta_output->ta_output_bus_signal.right_object_ttb                      = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.right_object_ttp                      = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.right_object_existence_probability    = FBK_ZERO_F;
   p_ta_output->ta_output_bus_signal.right_object_probability_of_collision = FBK_ZERO_F;
}

static TA_Object_Criticality_Level_T Ta_Map_Criticality_Of_Right_Object_As_Per_Alert_State(const Ta_Output_T *p_ta_output)
{
   TA_Object_Criticality_Level_T ta_criticality_level = NOT_CRITICAL;
   if ((uint8_t) TA_ALERT_STATE_NONE == p_ta_output->rta_alert_right)
   {
      ta_criticality_level = NOT_CRITICAL;
   }
   else if ((uint8_t) TA_ALERT_STATE_LEVEL_1 == p_ta_output->rta_alert_right)
   {
      ta_criticality_level = INFO_LEVEL;
   }
   else if ((uint8_t) TA_ALERT_STATE_LEVEL_3 == p_ta_output->rta_alert_right)
   {
      ta_criticality_level = ACUTE_LEVEL;
   }
   else
   {
      /*do nothing*/
   }
   return ta_criticality_level;
}

static TA_Object_Criticality_Level_T Ta_Map_Criticality_Of_Left_Object_As_Per_Alert_State(const Ta_Output_T *p_ta_output)
{
   TA_Object_Criticality_Level_T ta_criticality_level = NOT_CRITICAL;
   if ((uint8_t) TA_ALERT_STATE_NONE == p_ta_output->rta_alert_left)
   {
      ta_criticality_level = NOT_CRITICAL;
   }
   else if ((uint8_t) TA_ALERT_STATE_LEVEL_1 == p_ta_output->rta_alert_left)
   {
      ta_criticality_level = INFO_LEVEL;
   }
   else if ((uint8_t) TA_ALERT_STATE_LEVEL_3 == p_ta_output->rta_alert_left)
   {
      ta_criticality_level = ACUTE_LEVEL;
   }
   else
   {
      /*do nothing*/
   }
   return ta_criticality_level;
}


static TA_Object_Type_T Ta_Map_Left_Object_Type_As_Per_Object_Class(const Ta_Output_T *p_ta_output)
{
   TA_Object_Type_T ta_object_type = OBJECT_TYPE_UNKNOWN;
   if ((uint8_t) PA_OBJ_CLASS_UNKNOWN == p_ta_output->rta_left_object_type)
   {
      ta_object_type = OBJECT_TYPE_UNKNOWN;
   }
   else if ((uint8_t) PA_OBJ_CLASS_PEDESTRIAN == p_ta_output->rta_left_object_type)
   {
      ta_object_type = OBJECT_TYPE_PEDESTRIAN;
   }
   else if ((uint8_t) PA_OBJ_CLASS_2WHEEL == p_ta_output->rta_left_object_type)
   {
      ta_object_type = OBJECT_TYPE_2_WHEEL;
   }
   else if ((uint8_t) PA_OBJ_CLASS_CAR == p_ta_output->rta_left_object_type)
   {
      ta_object_type = OBJECT_TYPE_CAR;
   }
   else if ((uint8_t) PA_OBJ_CLASS_TRUCK == p_ta_output->rta_left_object_type)
   {
      ta_object_type = OBJECT_TYPE_TRUCK;
   }
   else
   {
      /*do nothing*/
   }
   return ta_object_type;
}


static TA_Object_Type_T Ta_Map_Right_Object_Type_As_Per_Object_Class(const Ta_Output_T *p_ta_output)
{
   TA_Object_Type_T ta_object_type = OBJECT_TYPE_UNKNOWN;
   if ((uint8_t) PA_OBJ_CLASS_UNKNOWN == p_ta_output->rta_right_object_type)
   {
      ta_object_type = OBJECT_TYPE_UNKNOWN;
   }
   else if ((uint8_t) PA_OBJ_CLASS_PEDESTRIAN == p_ta_output->rta_right_object_type)
   {
      ta_object_type = OBJECT_TYPE_PEDESTRIAN;
   }
   else if ((uint8_t) PA_OBJ_CLASS_2WHEEL == p_ta_output->rta_right_object_type)
   {
      ta_object_type = OBJECT_TYPE_2_WHEEL;
   }
   else if ((uint8_t) PA_OBJ_CLASS_CAR == p_ta_output->rta_right_object_type)
   {
      ta_object_type = OBJECT_TYPE_CAR;
   }
   else if ((uint8_t) PA_OBJ_CLASS_TRUCK == p_ta_output->rta_right_object_type)
   {
      ta_object_type = OBJECT_TYPE_TRUCK;
   }
   else
   {
      /*do nothing*/
   }
   return ta_object_type;
}

static void Ta_Set_Output_Bus_Signals_Of_Left_Object_In_Active_State(Ta_Output_T *p_ta_output)
{
   if ((TA_BMW_TARGET_ID_INVALID != p_ta_output->rta_id_left) && ((uint8_t) TA_ALERT_STATE_NONE != p_ta_output->rta_alert_left))
   {
      p_ta_output->ta_output_bus_signal.ta_object_criticality_left_output =
         Ta_Map_Criticality_Of_Left_Object_As_Per_Alert_State(p_ta_output);
      p_ta_output->ta_output_bus_signal.timestamp_left_object  = FBK_ZERO_F; /*no data available from Core*/
      p_ta_output->ta_output_bus_signal.left_object_id         = p_ta_output->rta_id_left;
      p_ta_output->ta_output_bus_signal.left_object_type       = Ta_Map_Left_Object_Type_As_Per_Object_Class(p_ta_output);
      p_ta_output->ta_output_bus_signal.left_object_position_x = p_ta_output->rta_long_posn_left;
      p_ta_output->ta_output_bus_signal.left_object_position_y = p_ta_output->rta_lat_posn_left;
      p_ta_output->ta_output_bus_signal.left_object_width      = p_ta_output->rta_left_object_width;
      p_ta_output->ta_output_bus_signal.left_object_length     = p_ta_output->rta_left_object_length;
      p_ta_output->ta_output_bus_signal.left_object_velocity_x = p_ta_output->rta_long_vel_left;
      p_ta_output->ta_output_bus_signal.left_object_velocity_y = p_ta_output->rta_lat_vel_left;
      p_ta_output->ta_output_bus_signal.left_object_ttc        = p_ta_output->rta_ttc_left;
      p_ta_output->ta_output_bus_signal.left_object_ttb        = FBK_ZERO_F; /*no data available from Core*/
      p_ta_output->ta_output_bus_signal.left_object_ttp        = p_ta_output->rta_ttc_left;
      p_ta_output->ta_output_bus_signal.left_object_existence_probability    = p_ta_output->rta_existence_probability_left;
      p_ta_output->ta_output_bus_signal.left_object_probability_of_collision = FBK_ZERO_F; /*no data available from Core*/
   }
}

static void Ta_Set_Output_Bus_Signals_Of_Right_Object_In_Active_State(Ta_Output_T *p_ta_output)
{
   if ((TA_BMW_TARGET_ID_INVALID != p_ta_output->rta_id_right) && ((uint8_t) TA_ALERT_STATE_NONE != p_ta_output->rta_alert_right))
   {
      p_ta_output->ta_output_bus_signal.ta_object_criticality_right_output =
         Ta_Map_Criticality_Of_Right_Object_As_Per_Alert_State(p_ta_output);
      p_ta_output->ta_output_bus_signal.timestamp_right_object  = FBK_ZERO_F; /*no data available from Core*/
      p_ta_output->ta_output_bus_signal.right_object_id         = p_ta_output->rta_id_right;
      p_ta_output->ta_output_bus_signal.right_object_type       = Ta_Map_Right_Object_Type_As_Per_Object_Class(p_ta_output);
      p_ta_output->ta_output_bus_signal.right_object_position_x = p_ta_output->rta_long_posn_right;
      p_ta_output->ta_output_bus_signal.right_object_position_y = p_ta_output->rta_lat_posn_right;
      p_ta_output->ta_output_bus_signal.right_object_width      = p_ta_output->rta_right_object_width;
      p_ta_output->ta_output_bus_signal.right_object_length     = p_ta_output->rta_right_object_length;
      p_ta_output->ta_output_bus_signal.right_object_velocity_x = p_ta_output->rta_long_vel_right;
      p_ta_output->ta_output_bus_signal.right_object_velocity_y = p_ta_output->rta_lat_vel_right;
      p_ta_output->ta_output_bus_signal.right_object_ttc        = p_ta_output->rta_ttc_right;
      p_ta_output->ta_output_bus_signal.right_object_ttb        = FBK_ZERO_F; /*no data available from Core*/
      p_ta_output->ta_output_bus_signal.right_object_ttp        = p_ta_output->rta_ttc_right;
      p_ta_output->ta_output_bus_signal.right_object_existence_probability    = p_ta_output->rta_existence_probability_right;
      p_ta_output->ta_output_bus_signal.right_object_probability_of_collision = FBK_ZERO_F; /*no data available from Core*/
   }
}

/*
 * Need more input from Feature Function to define this function
 * Currently, just assigning bits based on current understanding
 * This will be modified further when Requirements are available in
 * Polarion for State-Machine of TAP
 */
static void Ta_Set_Status_Based_On_Encoding(Ta_Output_T *p_ta_output)
{
   /*Need to confirm about remaining Bits*/
   if (TA_STATE_ERROR == p_ta_output->ta_output_bus_signal.bmw_qualifier_ta_function_state)
   {
      p_ta_output->ta_output_bus_signal.f_ta_status |= TA_BMW_BIT_POSITION_10;
   }
   else
   {
      if ((uint8_t) TA_ALERT_STATE_NONE == p_ta_output->rta_alert_left)
      {
         p_ta_output->ta_output_bus_signal.f_ta_status |= TA_BMW_BIT_POSITION_3; /*confirm*/
      }
      else if ((uint8_t) TA_ALERT_STATE_LEVEL_1 == p_ta_output->rta_alert_left)
      {
         p_ta_output->ta_output_bus_signal.f_ta_status |= TA_BMW_BIT_POSITION_1;
      }
      else if ((uint8_t) TA_ALERT_STATE_LEVEL_3 == p_ta_output->rta_alert_left)
      {
         p_ta_output->ta_output_bus_signal.f_ta_status |= TA_BMW_BIT_POSITION_2;
      }
      else
      {
         /*do nothing*/
      }
   }
}

static void Ta_Reset_Output_Bus_Signals(Ta_Output_T *p_ta_output)
{
   /*May be Required to change*/
   p_ta_output->ta_output_bus_signal.f_ta_status |= TA_BMW_BIT_POSITION_11;
   Ta_Reset_Output_Bus_Signals_Of_Right_Object(p_ta_output);
   Ta_Reset_Output_Bus_Signals_Of_Left_Object(p_ta_output);
}

static void Ta_Set_Output_Bus_Signals_In_Active_State(Ta_Output_T *p_ta_output)
{
   Ta_Set_Output_Bus_Signals_Of_Right_Object_In_Active_State(p_ta_output);
   Ta_Set_Output_Bus_Signals_Of_Left_Object_In_Active_State(p_ta_output);
   /*To do- Call method Set_Ta_Status_Based_On_Encoding to set encoding for f_ta_status
    * More info will be required on how to set these bits
    */
   Ta_Set_Status_Based_On_Encoding(p_ta_output);
}

static void Ta_Reset_Post_Run_Persistent(Ta_Bmw_Persistent_T *p_ta_bmw_persistent)
{
   assert(NULL != p_ta_bmw_persistent);

   p_ta_bmw_persistent->pfgs_prev_cycle_brake_deceleration_request = FBK_ZERO_F;
   p_ta_bmw_persistent->ta_host_speed_at_brake_start               = FBK_ZERO_F;
   p_ta_bmw_persistent->ta_host_speed_reduction_requested          = FBK_ZERO_F;
   p_ta_bmw_persistent->ta_host_speed_reduction_achieved           = FBK_ZERO_F;
   p_ta_bmw_persistent->pfgs_qualification_counter                 = FBK_ZERO_UINT;
   p_ta_bmw_persistent->pfgs_qualification_counter_min             = FBK_ZERO_UINT;
   p_ta_bmw_persistent->fta_prev_alert_level                       = FBK_ZERO_UINT;
}

/*============================================================================*\
* EXPORTED FUNCTIONS
\*============================================================================*/

void Ta_Post_Run_Init(void)
{
   /* Reset Ta persistent post run data */
   Ta_Reset_Post_Run_Persistent(&Ta_Bmw_Persistent);
}

/* coverity[misra_c_2012_rule_8_13_violation]["p_ta_instance" does not modify the object it points to] */
void Ta_Post_Run(Ta_Instance_T *p_ta_instance,
                 const Ta_Input_T *p_ta_input /**< TA Input */,
                 Ta_Output_T *p_ta_output /**< TA Output */)
{
   const Ta_Core_Output_T *p_ta_core_output;
   const Ta_Core_Calibration_T *p_ta_cal;
   const Pa_Data_T *p_pa_data;

   Fbk_Vehicle_Data_T vehicle_data;
   const TA_FF_State_T *p_ta_state = Ta_Get_State_Output_Ptr();

   assert(NULL != p_ta_output);
   assert(NULL != p_ta_instance);
   assert(NULL != p_ta_input);
   assert(NULL != p_ta_state);

   p_ta_core_output = &p_ta_instance->core_output;
   p_ta_cal         = &p_ta_instance->calibration;
   p_pa_data        = p_ta_instance->core_input.p_pa_data;

   p_ta_output->ta_output_bus_signal.bmw_qualifier_ta_function_state = *p_ta_state;

   /* Fill vehicle data */
   vehicle_data = p_pa_data->vehicle_data;

   /* Reset TA output to default values */
   Ta_Reset_Output(p_ta_output);

   /*Reset BMW Specific Output Bus Signals*/
   Ta_Reset_Output_Bus_Signals(p_ta_output);


   /* Check if the diagnostic mode is activated */
   p_ta_output->f_diagnostic_mode = Ta_Is_Diagnostic_Mode_Enabled(p_ta_input, p_ta_cal);

   /* Check the maneuver direction (TURN/STRAIGHT) */
   p_ta_output->fta_maneuver_direction = (uint8_t) Ta_Get_Maneuver_Direction(p_ta_input, &vehicle_data);

   /* Map BMW output signals for FTA (PFGS) and RTA (TAP)*/
   Ta_Map_Fta_Output_Signals(p_ta_output, p_ta_input, p_ta_core_output, p_ta_cal, p_pa_data);
   Ta_Map_Rta_Output_Signals(p_ta_output, p_ta_input, p_ta_core_output, p_pa_data);

   /* Update persistent values */
   Ta_Bmw_Persistent.fta_prev_alert_level = p_ta_output->fta_alert_level;

   if (TA_STATE_ACTIVE != p_ta_output->ta_output_bus_signal.bmw_qualifier_ta_function_state)
   {
      Ta_Reset_Output_Bus_Signals(p_ta_output);
   }
   else
   {
      Ta_Set_Output_Bus_Signals_In_Active_State(p_ta_output);
   }


   /* Write bin file output */
   Binary_Pass_Ta_Debug_Bmw_Sp25_Various(p_ta_input, &vehicle_data, p_ta_cal, Ta_Bmw_Persistent.ta_host_speed_at_brake_start,
                                         Ta_Bmw_Persistent.ta_host_speed_reduction_requested,
                                         Ta_Bmw_Persistent.ta_host_speed_reduction_achieved,
                                         Ta_Bmw_Persistent.pfgs_qualification_counter_min,
                                         Ta_Bmw_Persistent.pfgs_qualification_counter);
   Binary_Ta_Bmw_Sp25_Fill_Debug_Data(p_ta_input, p_ta_output);
   Binary_Ta_Bmw_Sp25_Write_Bin_File();
}

/**
 * @file cta_bmw_sp25_warn_state.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Code file for BMW_SP25 specific CTA post run contents
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_bmw_sp25_warn_state.h"
#include "cta_bmw_sp25_types.h"
#include "cta_input_t.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h" // for PA_VEH_PRNDL_STATE_DRIVE

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Determine if CTA is disabled.
 *
 * @return boolean
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4041}
 * @verification{}
 */
static boolean_T Cta_Is_Disabled(const Bmw_Ctb_Input_Coding_T *p_coding_input, const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief Determine if CTA is enabled for the FRONT.
 *
 * @return boolean
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4051}
 * @verification{}
 */
static boolean_T Cta_Is_Front_Enabled(const Bmw_Ctb_Input_Coding_T *p_coding_input /**< BMW specific coding paramater inputs */,
                                      const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief Determine if CTA is enabled for the REAR.
 *
 * @return boolean
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4050}
 * @verification{}
 */
static boolean_T Cta_Is_Rear_Enabled(const Bmw_Ctb_Input_Coding_T *p_coding_input /**< BMW specific coding paramater inputs */,
                                     const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief Determine if vehicle is in standstill and CTA-Info-Warning situation is present.
 *
 * @return boolean
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4049}
 * @verification{}
 */
static boolean_T
Cta_Is_Info_Standstill_Condition(const Bmw_Ctb_Input_Coding_T *p_coding_input /**< BMW specific coding paramater inputs */,
                                 const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input /**< BMW specific bus signal inputs */,
                                 const Cta_Crit_Level_T *cta_alert_level /**< CTA Core output alert level */);

/**
 * @brief Determine if vehicle is moving forward and CTA-Info-Warning situation is present.
 *
 * @return boolean
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4048}
 * @verification{}
 */
static boolean_T
Cta_Is_Info_Forward_Condition(const Bmw_Ctb_Input_Coding_T *p_coding_input /**< BMW specific coding paramater inputs */,
                              const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input /**< BMW specific bus signal inputs */,
                              const Cta_Crit_Level_T *cta_alert_level /**< CTA Core output alert level */);

/**
 * @brief Determine if vehicle is moving backwards and CTA-Info-Warning situation is present.
 *
 * @return boolean
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4047}
 * @verification{}
 */
static boolean_T
Cta_Is_Info_Backwards_Condition(const Bmw_Ctb_Input_Coding_T *p_coding_input /**< BMW specific coding paramater inputs */,
                                const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input /**< BMW specific bus signal inputs */,
                                const Cta_Crit_Level_T *cta_alert_level /**< CTA Core output alert level */);

/**
 * @brief Determine if CTA-Acute-Warning situation for FRONT is present when configured for early Acute warning.
 *
 * @return boolean
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4046}
 * @verification{}
 */
static boolean_T
Cta_Is_Acute_Same_Time_Front_Condition(const Bmw_Ctb_Input_Coding_T *p_coding_input /**< BMW specific coding paramater inputs */,
                                       const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input /**< BMW specific bus signal inputs */,
                                       const Cta_Crit_Level_T *cta_alert_level /**< CTA Core output alert level */);

/**
 * @brief Determine if CTA-Acute-Warning situation for REAR is present when configured for early Acute warning.
 *
 * @return boolean
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4045}
 * @verification{}
 */
static boolean_T
Cta_Is_Acute_Same_Time_Rear_Condition(const Bmw_Ctb_Input_Coding_T *p_coding_input /**< BMW specific coding paramater inputs */,
                                      const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input /**< BMW specific bus signal inputs */,
                                      const Cta_Crit_Level_T *cta_alert_level /**< CTA Core output alert level */);

/**
 * @brief Determine if vehicle is moving forward and CTA-Acute-Warning situation is present
 *
 * @return boolean
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4042}
 * @verification{}
 */
static boolean_T
Cta_Is_Acute_Forward_Condition(const Bmw_Ctb_Input_Coding_T *p_coding_input /**< BMW specific coding paramater inputs */,
                               const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input /**< BMW specific bus signal inputs */,
                               const Cta_Crit_Level_T *cta_alert_level /**< CTA Core output alert level */);

/**
 * @brief Determine if vehicle is moving backwards and CTA-Acute-Warning situation is present.
 *
 * @return boolean
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4044}
 * @verification{}
 */
static boolean_T
Cta_Is_Acute_Backwards_Condition(const Bmw_Ctb_Input_Coding_T *p_coding_input /**< BMW specific coding paramater inputs */,
                                 const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input /**< BMW specific bus signal inputs */,
                                 const Cta_Crit_Level_T *cta_alert_level /**< CTA Core output alert level */);
static boolean_T Cta_Is_front_warning_state_condition(const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input);
static boolean_T Cta_Is_rear_warning_state_condition(const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input);
static Bmw_Ctb_Warn_Control_Output_State_T Cta_Check_For_Warning(const boolean_T *front_rear_disabled,
                                                                 const boolean_T *front_enabled,
                                                                 const boolean_T *front_warning,
                                                                 const boolean_T *rear_enabled,
                                                                 const boolean_T *rear_warning,
                                                                 const boolean_T *acute_warning_forward_condition,
                                                                 const boolean_T *acute_warning_same_time_front_condition,
                                                                 const boolean_T *acute_warning_backwards_condition,
                                                                 const boolean_T *acute_warning_same_time_rear_condition);
/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

Bmw_Ctb_Warn_Control_Output_State_T Cta_Bmw_Ctb_Get_Warn_Output_Control_State(const Cta_Input_T *p_cta_input,
                                                                              const Cta_Crit_Level_T *cta_alert_level,
                                                                              const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   Bmw_Ctb_Warn_Control_Output_State_T warn_output_state; /* Return value for current evaluated warning state */

   /* Disabled warning flag definition is duplicate (to front&rear enabled) but additional safety mechanism */
   boolean_T front_rear_disabled = Cta_Is_Disabled(&(p_cta_input->bmw_ctb_coding_parameters), p_vehicle_data);

   /* Enable flag for Front-Mode (Main-CTB-Codings & Gear) */
   boolean_T front_enabled = Cta_Is_Front_Enabled(&(p_cta_input->bmw_ctb_coding_parameters), p_vehicle_data);

   /* Enable flag for Rear-Mode (Main-CTB-Codings & Gear) */
   boolean_T rear_enabled = Cta_Is_Rear_Enabled(&(p_cta_input->bmw_ctb_coding_parameters), p_vehicle_data);

   /* Enable flag for an Info-Warning */
   boolean_T info_warning_standstill_condition = Cta_Is_Info_Standstill_Condition(
      &(p_cta_input->bmw_ctb_coding_parameters), &(p_cta_input->bmw_ctb_input_signals), cta_alert_level);

   /* Enable flag for a Front Info-Warning */
   boolean_T info_warning_forward_condition = Cta_Is_Info_Forward_Condition(&(p_cta_input->bmw_ctb_coding_parameters),
                                                                            &(p_cta_input->bmw_ctb_input_signals), cta_alert_level);

   /* Enable flag for a Rear Info-Warning */
   boolean_T info_warning_backwards_condition = Cta_Is_Info_Backwards_Condition(
      &(p_cta_input->bmw_ctb_coding_parameters), &(p_cta_input->bmw_ctb_input_signals), cta_alert_level);

   /* Enable flag for a Front Acute-Warning together with Info-Warning */
   boolean_T acute_warning_same_time_front_condition = Cta_Is_Acute_Same_Time_Front_Condition(
      &(p_cta_input->bmw_ctb_coding_parameters), &(p_cta_input->bmw_ctb_input_signals), cta_alert_level);

   /* Enable flag for a Rear Acute-Warning together with Info-Warning */
   boolean_T acute_warning_same_time_rear_condition = Cta_Is_Acute_Same_Time_Rear_Condition(
      &(p_cta_input->bmw_ctb_coding_parameters), &(p_cta_input->bmw_ctb_input_signals), cta_alert_level);
   /* Enable flag for a Forward Acute-Warning */
   boolean_T acute_warning_forward_condition = Cta_Is_Acute_Forward_Condition(
      &(p_cta_input->bmw_ctb_coding_parameters), &(p_cta_input->bmw_ctb_input_signals), cta_alert_level);
   /* Enable flag for a Backwards Acute-Warning */
   boolean_T acute_warning_backwards_condition = Cta_Is_Acute_Backwards_Condition(
      &(p_cta_input->bmw_ctb_coding_parameters), &(p_cta_input->bmw_ctb_input_signals), cta_alert_level);
   boolean_T rear_warning_state_condition  = Cta_Is_rear_warning_state_condition(&(p_cta_input->bmw_ctb_input_signals));
   boolean_T front_warning_state_condition = Cta_Is_front_warning_state_condition(&(p_cta_input->bmw_ctb_input_signals));
   boolean_T front_warning =
      (boolean_T) ((((Fbk_Is_True(info_warning_standstill_condition)) || (Fbk_Is_True(info_warning_forward_condition)))
                    && (front_warning_state_condition)));
   boolean_T rear_warning =
      (boolean_T) ((((Fbk_Is_True(info_warning_standstill_condition)) || (Fbk_Is_True(info_warning_backwards_condition)))
                    && (rear_warning_state_condition)));
   warn_output_state = Cta_Check_For_Warning(&front_rear_disabled, &front_enabled, &front_warning, &rear_enabled, &rear_warning,
                                             &acute_warning_forward_condition, &acute_warning_same_time_front_condition,
                                             &acute_warning_backwards_condition, &acute_warning_same_time_rear_condition);
   return warn_output_state;
}
/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static boolean_T Cta_Is_Disabled(const Bmw_Ctb_Input_Coding_T *p_coding_input, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /* CTB is disabled by overall Coding-Parameter */
   /* CTB is disbaled for both sides Front and Rear */
   /* Ego vehicle is NOT in Forward gear (Drive) */
   /* Ego vehicle is NOT in Backwards gear (Reverse) */
   boolean_T f_gear_not_rear_front =
      (boolean_T) ((PA_VEH_PRNDL_STATE_DRIVE != p_vehicle_data->prndl) && (PA_VEH_PRNDL_STATE_REVERSE != p_vehicle_data->prndl));
   return (boolean_T) (f_gear_not_rear_front || (BMW_CTB_VARIANT_NO_CTB == p_coding_input->c_ctb_variant)
                       || Fbk_Is_False(p_coding_input->c_ctb_enabled));
}

static boolean_T Cta_Is_Front_Enabled(const Bmw_Ctb_Input_Coding_T *p_coding_input, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /* CTB is enabled by overall Coding-Parameter */
   /* CTB is enabled for both sides Front and Rear */
   /* Ego vehicle is in Forward gear (Drive) */
   return (boolean_T) (Fbk_Is_True(p_coding_input->c_ctb_enabled)
                       && (BMW_CTB_VARIANT_CTB_REAR_AND_FRONT == p_coding_input->c_ctb_variant)
                       && (PA_VEH_PRNDL_STATE_DRIVE == p_vehicle_data->prndl));
}

static boolean_T Cta_Is_Rear_Enabled(const Bmw_Ctb_Input_Coding_T *p_coding_input, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /* CTB is enabled by overall Coding-Parameter */
   /* CTB is enabled at least for Rear */
   /* Ego vehicle is in Backwards gear (Reverse) */
   return (boolean_T) (Fbk_Is_True(p_coding_input->c_ctb_enabled)
                       && ((BMW_CTB_VARIANT_CTB_REAR_AND_FRONT == p_coding_input->c_ctb_variant)
                           || (BMW_CTB_VARIANT_CTB_REAR == p_coding_input->c_ctb_variant))
                       && (PA_VEH_PRNDL_STATE_REVERSE == p_vehicle_data->prndl));
}
static boolean_T Cta_Is_Info_Standstill_Condition(const Bmw_Ctb_Input_Coding_T *p_coding_input,
                                                  const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input,
                                                  const Cta_Crit_Level_T *cta_alert_level)
{
   /* Acute warning is NOT same time with info warning */
   /* Ego vehicle is in standstill */
   /* Algo-Core-Alert is active at any level */
   return (boolean_T) (Fbk_Is_False(p_coding_input->c_ctb_acute_warning_same_time_with_advance_warning)
                       && (BMW_CTB_VEHICLE_IS_IN_STANDSTILL == p_bus_signal_input->vehicle_moving_direction)
                       && (CTA_CRIT_LEVEL_1 <= *cta_alert_level));
}
static boolean_T Cta_Is_Info_Forward_Condition(const Bmw_Ctb_Input_Coding_T *p_coding_input,
                                               const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input,
                                               const Cta_Crit_Level_T *cta_alert_level)
{
   /* Acute warning is NOT same time with info warning */
   /* Ego vehicle is moving forward */
   /* Algo-Core-Alert is active at low criticallity level (Level1) */
   return (boolean_T) (Fbk_Is_False(p_coding_input->c_ctb_acute_warning_same_time_with_advance_warning)
                       && (BMW_CTB_VEHICLE_IS_MOVING_FORWARDS == p_bus_signal_input->vehicle_moving_direction)
                       && (CTA_CRIT_LEVEL_1 == *cta_alert_level));
}


static boolean_T Cta_Is_Info_Backwards_Condition(const Bmw_Ctb_Input_Coding_T *p_coding_input,
                                                 const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input,
                                                 const Cta_Crit_Level_T *cta_alert_level)
{
   /* Acute warning is NOT same time with info warning */
   /* Ego vehicle is moving backwards */
   /* Algo-Core-Alert is active at low criticallity level (Level1) */
   return (boolean_T) (Fbk_Is_False(p_coding_input->c_ctb_acute_warning_same_time_with_advance_warning)
                       && (BMW_CTB_VEHICLE_IS_MOVING_BACKWARDS == p_bus_signal_input->vehicle_moving_direction)
                       && (CTA_CRIT_LEVEL_1 == *cta_alert_level));
}
static boolean_T Cta_Is_Acute_Same_Time_Front_Condition(const Bmw_Ctb_Input_Coding_T *p_coding_input,
                                                        const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input,
                                                        const Cta_Crit_Level_T *cta_alert_level)
{
   /* Acute warning is same time with info warning */
   /* Ego vehicle is moving forward or at standstill */
   /* Algo-Core-Alert is active at any level */
   return (boolean_T) (Fbk_Is_True(p_coding_input->c_ctb_acute_warning_same_time_with_advance_warning)
                       && ((BMW_CTB_VEHICLE_IS_MOVING_FORWARDS == p_bus_signal_input->vehicle_moving_direction)
                           || (BMW_CTB_VEHICLE_IS_IN_STANDSTILL == p_bus_signal_input->vehicle_moving_direction))
                       && (CTA_CRIT_LEVEL_1 <= *cta_alert_level));
}
static boolean_T Cta_Is_Acute_Same_Time_Rear_Condition(const Bmw_Ctb_Input_Coding_T *p_coding_input,
                                                       const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input,
                                                       const Cta_Crit_Level_T *cta_alert_level)
{
   /* Acute warning is same time with info warning */
   /* Ego vehicle is moving backwards or at standstill */
   /* Algo-Core-Alert is active at any level */
   return (boolean_T) (Fbk_Is_True(p_coding_input->c_ctb_acute_warning_same_time_with_advance_warning)
                       && ((BMW_CTB_VEHICLE_IS_MOVING_BACKWARDS == p_bus_signal_input->vehicle_moving_direction)
                           || (BMW_CTB_VEHICLE_IS_IN_STANDSTILL == p_bus_signal_input->vehicle_moving_direction))
                       && (CTA_CRIT_LEVEL_1 <= *cta_alert_level));
}

static boolean_T Cta_Is_Acute_Forward_Condition(const Bmw_Ctb_Input_Coding_T *p_coding_input,
                                                const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input,
                                                const Cta_Crit_Level_T *cta_alert_level)
{
   /* Acute warning is NOT same time with info warning */
   /* Ego vehicle is moving forward */
   /* Algo-Core-Alert is active at high criticallity level */
   return (boolean_T) (Fbk_Is_False(p_coding_input->c_ctb_acute_warning_same_time_with_advance_warning)
                       && (BMW_CTB_VEHICLE_IS_MOVING_FORWARDS == p_bus_signal_input->vehicle_moving_direction)
                       && (CTA_CRIT_LEVEL_1 < *cta_alert_level));
}

static boolean_T Cta_Is_Acute_Backwards_Condition(const Bmw_Ctb_Input_Coding_T *p_coding_input,
                                                  const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input,
                                                  const Cta_Crit_Level_T *cta_alert_level)
{
   /* Acute warning is NOT same time with info warning */
   /* Ego vehicle is moving backwards */
   /* Algo-Core-Alert is active at high criticallity level */
   return (boolean_T) (Fbk_Is_False(p_coding_input->c_ctb_acute_warning_same_time_with_advance_warning)
                       && (BMW_CTB_VEHICLE_IS_MOVING_BACKWARDS == p_bus_signal_input->vehicle_moving_direction)
                       && (CTA_CRIT_LEVEL_1 < *cta_alert_level));
}
static boolean_T Cta_Is_rear_warning_state_condition(const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input)
{
   return (boolean_T) ((Fbk_Is_True(p_bus_signal_input->control_cross_traffic_alert_rear))
                       || (Fbk_Is_True(p_bus_signal_input->control_cross_traffic_alert_rear_braking)));
}
static boolean_T Cta_Is_front_warning_state_condition(const Bmw_Ctb_Input_Bus_Signals_T *p_bus_signal_input)
{
   return (boolean_T) ((Fbk_Is_True(p_bus_signal_input->control_cross_traffic_alert_front)));
}
static Bmw_Ctb_Warn_Control_Output_State_T Cta_Check_For_Warning(const boolean_T *front_rear_disabled,
                                                                 const boolean_T *front_enabled,
                                                                 const boolean_T *front_warning,
                                                                 const boolean_T *rear_enabled,
                                                                 const boolean_T *rear_warning,
                                                                 const boolean_T *acute_warning_forward_condition,
                                                                 const boolean_T *acute_warning_same_time_front_condition,
                                                                 const boolean_T *acute_warning_backwards_condition,
                                                                 const boolean_T *acute_warning_same_time_rear_condition)
{
   Bmw_Ctb_Warn_Control_Output_State_T warn_state;
   if (*front_rear_disabled)
   {
      /* This condition should be a safety step to assure deactivation when no warning is expected */
      warn_state = BMW_CTB_WARN_STATE_NO_WARNING;
   }
   else if ((Fbk_Is_True(*front_enabled)) && (*front_warning))
   {
      warn_state = BMW_CTB_WARN_STATE_INFO_WARNING_FRONT;
   }
   else if ((Fbk_Is_True(*rear_enabled)) && (*rear_warning))
   {
      warn_state = BMW_CTB_WARN_STATE_INFO_WARNING_REAR;
   }
   else if (Fbk_Is_True(*front_enabled)
            && (Fbk_Is_True(*acute_warning_forward_condition) || Fbk_Is_True(*acute_warning_same_time_front_condition)))
   {
      warn_state = BMW_CTB_WARN_STATE_ACUTE_WARNING_FRONT;
   }
   else if (Fbk_Is_True(*rear_enabled)
            && (Fbk_Is_True(*acute_warning_backwards_condition) || Fbk_Is_True(*acute_warning_same_time_rear_condition)))
   {
      warn_state = BMW_CTB_WARN_STATE_ACUTE_WARNING_REAR;
   }
   else
   {
      /* This condition could be reached for Gear=Drive && Mov_Direction=Backwards or similar cases */
      warn_state = BMW_CTB_WARN_STATE_NO_WARNING;
   }
   return warn_state;
}

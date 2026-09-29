#ifndef CTA_STATE_MACHINE_H
#define CTA_STATE_MACHINE_H
/**
 * @file cta_state_machine.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for the BMW SP25 State Machine logic for CTB.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */
/*===========================================================================
 * Includes
 *=========================================================================*/
#include "cta_bmw_sp25_types.h"
#include "cta_core_calibration_t.h"     // for Cta_Core_Calibration_T
#include "cta_customer_calibration_t.h" // for Cta_Customer_Calibration_T
#include "cta_input_t.h"                // for Cta_Input_T
#include "fbk_vehicle_data_t.h"         // for Fbk_Vehicle_Data_T
#include "pa_reuse.h"                   // for boolean_T
/**
 * Ctb_Flag_Output_T structure
 */
typedef struct
{
   boolean_T f_ctb_flag_ctb_enabled;   /**< BMW CTB Enabled Flag*/
   boolean_T f_ctb_flag_ctb_activated; /**< BMW CTB Activation Flag*/
   boolean_T f_ctb_test_mode;          /**< BMW Test Mode Enabled Flag*/
   boolean_T f_speed_check;            /**< BMW CTB Activation Speed Check Pass/Fail*/
   boolean_T f_speed_check_hys;        /**< BMW CTB deactivation Speed Check Pass/Fail*/
   boolean_T f_fcta_activate;          /**< BMW CTB front mode activation*/
   boolean_T f_rcta_activate;          /**< BMW CTB rear mode activation*/
   boolean_T f_brake_override;         /**< brake override for braking*/
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Ctb_Flag_Output_T;
/*===========================================================================
 * External Function Prototypes
 *=========================================================================*/
/**
 * Cta_State_Output_T Get Pointer Function
 */
Ctb_State_Output_T *Cta_Get_State_Output_Ptr(void);
/**
 * Cta_State_Output_T structure
 */
/**
 * @brief State Machine Flag Computation for CTB.
 *
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
void Cta_Update_Flags(const Cta_Input_T *p_cta_input,
                      const Cta_Core_Calibration_T *p_cta_cal,
                      const Cta_Customer_Calibration_T *p_cta_custom_cal,
                      Ctb_Flag_Output_T *p_ctb_state_machine_flags,
                      const Fbk_Vehicle_Data_T *p_vehicle_data);
/**
 * @brief State Machine for CTB.
 *
 * @return void
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
void Cta_State_Machine(Ctb_State_Output_T *p_ctb_current_state,
                       const Ctb_Function_Error_T *p_cta_errors,
                       const Ctb_Flag_Output_T ctb_state_machine_flags);
#endif /* CTA_STATE_MACHINE_H*/

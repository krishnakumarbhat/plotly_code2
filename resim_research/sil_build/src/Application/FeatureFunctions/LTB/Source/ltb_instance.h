#ifndef LTB_INSTANCE_H
#define LTB_INSTANCE_H

/**
 * @file ltb_instance.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the LTB instance data types.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

#include "fbk_ego_traj_predictor_instance.h"
#include "ltb_core_calibration_t.h"
#include "ltb_core_input_t.h"
#include "ltb_core_output_t.h"
#include "ltb_customer_calibration_t.h"
#include "ltb_persistent_t.h"

/**
 * @brief Ltb_Persistent_T structure
 *
 * @SDD{}
 * @verification{}
 */
/* coverity[misra_c_2012_rule_2_4_violation][Type has tag "Ltb_Instance_T" but that tag is never used] */
typedef struct Ltb_Instance_T
{
   Ltb_Persistent_T persistent;
   Ltb_Core_Calibration_T calibration;
   Ltb_Customer_Calibration_T customer_calibration;
   Ltb_Core_Input_T core_input;
   Ltb_Core_Output_T core_output;
   Fbk_Ego_Traj_Predictor_Instance_T ego_traj_predictor_instance;
} Ltb_Instance_T;

#endif /* LTB_INSTANCE_H */

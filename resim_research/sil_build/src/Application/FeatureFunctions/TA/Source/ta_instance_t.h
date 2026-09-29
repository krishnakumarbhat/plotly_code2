#ifndef TA_INSTANCE_H
#define TA_INSTANCE_H

/**
 * @file ta_instance.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the TA instance data types.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

#include "fbk_ego_traj_predictor_instance.h"
#include "ta_core_calibration_t.h"
#include "ta_core_input_t.h"
#include "ta_core_output_t.h"
#include "ta_customer_calibration_t.h"
#include "ta_persistent_t.h"
/**
 * @brief Ta_Persistent_T structure
 *
 * @SDD{}
 * @verification{}
 */
/* coverity[misra_c_2012_rule_2_4_violation][Type has tag "Ta_Instance_T" but that tag is never used] */
typedef struct Ta_Instance_T
{
   Ta_Persistent_T persistent;
   Ta_Core_Calibration_T calibration;
   Ta_Customer_Calibration_T customer_calibration;
   Ta_Core_Input_T core_input;
   Ta_Core_Output_T core_output;
   Fbk_Ego_Traj_Predictor_Instance_T ego_traj_predictor_instance;
} Ta_Instance_T;

#endif /* TA_INSTANCE_H */

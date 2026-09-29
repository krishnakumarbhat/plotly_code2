#ifndef SCW_INSTANCE_T_H
#define SCW_INSTANCE_T_H

/**
 * @file scw_instance_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the SCW instance data types.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "scw_core_calibration_t.h"
#include "scw_core_input_t.h"
#include "scw_core_output_t.h"
#include "scw_customer_calibration_t.h"
#include "scw_persistent_t.h"
/**
 * @brief Scw_Instance_T structure
 *
 * @SDD{}
 * @verification{}
 */
/* coverity[misra_c_2012_rule_2_4_violation][Type has tag "Scw_Instance_T" but that tag is never used] */
typedef struct Scw_Instance_T
{
   Scw_Persistent_T persistent;
   Scw_Core_Calibration_T calibration;
   Scw_Customer_Calibration_T customer_calibration;
   Scw_Core_Input_T core_input;
   Scw_Core_Output_T core_output;
} Scw_Instance_T;

#endif /* SCW_INSTANCE_T_H */

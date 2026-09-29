#ifndef ESA_INSTANCE_T_H
#define ESA_INSTANCE_T_H

/**
 * @file esa_instance_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the ESA instance data types.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "esa_core_calibration_t.h"
#include "esa_core_input_t.h"
#include "esa_core_output_t.h"
#include "esa_customer_calibration_t.h"
#include "esa_persistent_t.h"
/**
 * @brief Esa_Instance_T structure
 *
 * @SDD{}
 * @verification{}
 */
/* coverity[misra_c_2012_rule_2_4_violation][Type has tag "Esa_Instance_T" but that tag is never used] */
typedef struct Esa_Instance_T
{
   Esa_Persistent_T persistent;
   Esa_Core_Calibration_T calibration;
   Esa_Customer_Calibration_T customer_calibration;

   Esa_Core_Input_T core_input;
   Esa_Core_Output_T core_output;
} Esa_Instance_T;

#endif /* ESA_INSTANCE_T_H */

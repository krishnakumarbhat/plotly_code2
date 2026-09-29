#ifndef CED_INSTANCE_H
#define CED_INSTANCE_H

/**
 * @file ced_instance.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the CED instance data types.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */


#include "ced_core_calibration_t.h"
#include "ced_core_input_t.h"
#include "ced_core_output_t.h"
#include "ced_customer_calibration_t.h"
#include "ced_persistent_t.h"

/* coverity[misra_c_2012_rule_2_4_violation][Type has tag "Ced_Instance_T" but that tag is never used] */
typedef struct Ced_Instance_T
{
   Ced_Persistent_T persistance;

   Ced_Core_Calibration_T calibration;
   Ced_Customer_Calibration_T customer_calibration;

   Ced_Core_Input_T core_input;
   Ced_Core_Output_T core_output;
} Ced_Instance_T;

#endif /* CED_INSTANCE_H */

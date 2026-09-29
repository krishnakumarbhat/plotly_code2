#ifndef LCDA_INSTANCE_H
#define LCDA_INSTANCE_H

/**
 * @file lcda_instance.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the LCDA instance data types.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_customer_calibration_t.h"
#include "lcda_persistent_t.h"
/**
 * @brief Lcda_Persistent_T structure
 *
 * @SDD{}
 * @verification{}
 */
/* coverity[misra_c_2012_rule_2_4_violation][Type has tag "Lcda_Instance_T" but that tag is never used] */
typedef struct Lcda_Instance_T
{
   Lcda_Persistent_T persistent;
   Lcda_Core_Calibration_T calibration;
   Lcda_Customer_Calibration_T customer_calibration;
   Lcda_Core_Input_T core_input;
   Lcda_Core_Output_T core_output;
   Lcda_Bsw_Persistent_T bsw_persistent;
   Lcda_Cvw_Persistent_T cvw_persistent;
   Lcda_Elc_Persistent_T elc_persistent;
   Lcda_Slc_Persistent_T slc_persistent;
} Lcda_Instance_T;

#endif /* LCDA_INSTANCE_H */

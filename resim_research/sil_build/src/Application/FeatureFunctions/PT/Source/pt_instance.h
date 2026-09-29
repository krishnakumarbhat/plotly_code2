#ifndef PT_INSTANCE_H
#define PT_INSTANCE_H
/**
 * @file pt_instance.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the PT instance data types.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

#include "fbk_output.h"
#include "pt_core_calibration_t.h"
#include "pt_customer_calibration_t.h"
#include "pt_input_t.h"
#include "pt_persistent_t.h"
/**
 * @brief Pt_Persistent_T structure
 *
 * @SDD{}
 * @verification{}
 */

/* coverity[misra_c_2012_rule_2_4_violation][Type has tag "Mois_Instance_T" but that tag is never used] */
typedef struct Pt_Instance_T
{
   Pt_Persistent_T persistent;
   Pt_Core_Calibration_T calibration;
   Pt_Customer_Calibration_T customer_calibration;
   Pt_Input_T pt_core_input;
} Pt_Instance_T;


#endif

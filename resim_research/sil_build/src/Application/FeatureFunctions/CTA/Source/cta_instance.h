#ifndef CTA_INSTANCE_H
#define CTA_INSTANCE_H

/**
 * @file cta_instance.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the CTA instance data types.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "cta_core_calibration_t.h"
#include "cta_core_input_t.h"
#include "cta_core_output_t.h"
#include "cta_customer_calibration_t.h"
#include "cta_persistent_t.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"

/**
 * @brief Cta_Persistent_T structure
 *
 * @SDD{}
 * @verification{}
 */
/* coverity[misra_c_2012_rule_2_4_violation][Type has tag "Cta_Instance_T" but that tag is never used] */
typedef struct Cta_Instance_T
{
   Cta_Persistent_T persistent;
   Cta_Core_Calibration_T calibration;
   Cta_Customer_Calibration_T customer_calibration;
   Cta_Core_Input_T core_input;
   Cta_Core_Output_T core_output;
   Fbk_Object_Data_T cta_obj_tracker_high_crit[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];
   Cta_Object_Attributes_T obj_attributes_array[PA_OBJ_NUMBER_OF_OBJECTS];
   Cta_Object_Persistent_T obj_persistent_array[CTA_OBJ_MAX_ARRAY_SIZE];
} Cta_Instance_T;

#endif /* CTA_INSTANCE_H */

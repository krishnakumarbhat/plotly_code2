#ifndef RECW_INSTANCE_H
#define RECW_INSTANCE_H

/**
 * @file recw_instance.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the RECW instance data types.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

#include "recw_core_calibration_t.h"
#include "recw_core_input_t.h"
#include "recw_core_output_t.h"
#include "recw_customer_calibration_t.h"
#include "recw_persistent_t.h"

/*===========================================================================*\
* Defines
\*===========================================================================*/

/**
 * @brief Recw_Persistent_T structure
 *
 * @SDD{}
 * @verification{}
 */
/* coverity[misra_c_2012_rule_2_4_violation][Type has tag "Recw_Instance_T" but that tag is never used] */
typedef struct Recw_Instance_T
{
   Recw_Persistent_T persistent;
   Recw_Core_Calibration_T calibration;
   Recw_Customer_Calibration_T customer_calibration;
   Recw_Core_Input_T core_input;
   Recw_Core_Output_T core_output;
} Recw_Instance_T;

#endif /* RECW_INSTANCE_H */

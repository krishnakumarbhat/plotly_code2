#ifndef CED_CORE_INPUT_T_H
#define CED_CORE_INPUT_T_H

/**
 * @file ced_core_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the core input data structure for CED.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_types.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pt_output_t.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Ced_Core_Input_T structure
 *
 * @SRS{SF-50}
 * @SAE{SF-2453}
 * @SDD{SF-3556}
 */
typedef struct
{
   boolean_T f_ced_enable;     /* Flag indicating the status of the CED function */
   boolean_T f_ced_front_mode; /* Flag indicating the front mode is enabled */
   boolean_T f_ced_rear_mode;  /* Flag indicating the rear mode is enabled */

   const Pa_Data_T *p_pa_data;     /* Data from abstraction layer consumed by CED */
   const Pt_Output_T *p_pt_output; /* Information on path to object relations, from Path Tracking */
} Ced_Core_Input_T;

#endif /* CED_CORE_INPUT_T_H */

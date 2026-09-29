#ifndef ESA_CORE_INPUT_T_H
#define ESA_CORE_INPUT_T_H

/**
 * @file esa_core_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the core input data structure for ESA.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "esa_types.h"
#include "pa_data.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Esa_Core_Input_T structure
 *
 * @SRD{}
 * @SAD{}
 * @SDD{CSCSA-65123}
 */
typedef struct
{
   boolean_T f_esa_enabled; /* Flag indicating the status of the ESA function */

   float32_T lane_width;         /**< [m] Lane width from external sources for example from camera info */
   float32_T lane_center_offset; /**< [m] Lane center offset in VCS co-ord (positive when to the right of the ego, negative
                                  when to the left of the ego) */
   Esa_Trailer_Object_T trailer; /**< Information about the trailer that might be attached to the host vehicle */

   const Pa_Data_T *p_pa_data; /**< Abstraction layer consumed by ESA */
} Esa_Core_Input_T;

#endif /* ESA_CORE_INPUT_T_H */

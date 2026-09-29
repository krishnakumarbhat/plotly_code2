#ifndef RECW_CORE_INPUT_T_H
#define RECW_CORE_INPUT_T_H

/**
 * @file recw_core_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the RECW core input tag header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_data.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Recw_Core_Input_T structure
 *
 * Defines core input interface data
 *
 * @SRS{SF-1659}
 * @SAE{SF-2984,SF-2960,SF-2987,SF-2988}
 * @SDD{SF-7821}
 */

typedef struct
{
   const Pa_Data_T *p_pa_data; /**< Platform abstraction data */
   boolean_T f_enable_recw;    /**< Flag indicating if RECW is enabled */
} Recw_Core_Input_T;

#endif /* RECW_CORE_INPUT_T_H */

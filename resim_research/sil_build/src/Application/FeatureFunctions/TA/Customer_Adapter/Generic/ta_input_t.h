#ifndef TA_INPUT_T_H
#define TA_INPUT_T_H

/**
 * @file ta_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the input data structure for TA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_data.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

/**
 * @brief Ta_Input_T structure
 *
 * @SDD{}
 */
typedef struct
{
   boolean_T f_ta_enable;  /**< Enable flag for TA */
   boolean_T f_fta_enable; /**< Enable flag for FTA */
   boolean_T f_rta_enable; /**< Enable flag for RTA */
} Ta_Input_T;

#endif /* TA_INPUT_T_H */

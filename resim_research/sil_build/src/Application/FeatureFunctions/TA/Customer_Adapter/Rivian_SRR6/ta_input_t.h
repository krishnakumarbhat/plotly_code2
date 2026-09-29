#ifndef TA_INPUT_T_H
#define TA_INPUT_T_H

/**
 * @file ta_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the Rivian_SRR6 specific input data structure for TA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_data.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

/* Defines the HMI warntrigger switches so that warn settings can be set according to the respective situation for the core. */
typedef enum
{
   TA_RIVIAN_SRR6_WARNTRIGGER_LATE   = (0),
   TA_RIVIAN_SRR6_WARNTRIGGER_NORMAL = (1),
   TA_RIVIAN_SRR6_WARNTRIGGER_EARLY  = (2)
} Ta_Rivian_Srr6_Hmi_Warntrigger_T;

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

   Ta_Rivian_Srr6_Hmi_Warntrigger_T ta_warntrigger_hmi; /**< TA warntrigger HMI */
} Ta_Input_T;

#endif /* TA_INPUT_T_H */

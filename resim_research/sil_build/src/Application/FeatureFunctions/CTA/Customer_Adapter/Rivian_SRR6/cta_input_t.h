#ifndef CTA_INPUT_T_H
#define CTA_INPUT_T_H

/**
 * @file cta_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Rivian_SRR6 customer input declaration.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_types.h"
#include "pa_context.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/* Defines the HMI warntrigger switches so that warn settings can be set according to the respective situation for the core. */
typedef enum
{
   CTA_RIVIAN_SRR6_WARNTRIGGER_LATE   = (0),
   CTA_RIVIAN_SRR6_WARNTRIGGER_NORMAL = (1),
   CTA_RIVIAN_SRR6_WARNTRIGGER_EARLY  = (2)
} Cta_Rivian_Srr6_Hmi_Warntrigger_T;

typedef struct
{
   boolean_T f_cta_enable;                                /**< Flag indicating if overall CTA shall be enabled */
   boolean_T f_front_cta_enable;                          /**< Flag indicating if front CTA shall be enabled */
   boolean_T f_rear_cta_enable;                           /**< Flag indicating if rear CTA shall be enabled */
   Cta_Rivian_Srr6_Hmi_Warntrigger_T cta_warntrigger_hmi; /**< CTA warntrigger HMI */
   Pa_Context_T *p_pa_data;                               /**< Pointer to Context data */
   Pt_Output_T *p_pt_output;                              /**< Pointer to Path Tracking Output */

} Cta_Input_T;

#endif

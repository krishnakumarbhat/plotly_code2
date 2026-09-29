#ifndef LCDA_INPUT_T_H
#define LCDA_INPUT_T_H

/**
 * @file lcda_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the Generic input data structure for LCDA.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "lcda_types.h"
#include "pa_data.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

typedef enum
{
   CVW_DYN_TTC_DISABLED = (0),
   CVW_DYN_TTC_EARLY    = (1),
   CVW_DYN_TTC_NORMAL   = (2),
   CVW_DYN_TTC_LATE     = (3)
} HMI_CVW_DYN_TTC;

/**
 * @brief Lcda_Input_T structure
 *
 * @SRS{CSCSA-122825}
 * @SAE{CSCSA-122811}
 * @SDD{CSCSA-122801}
 */
typedef struct
{
   boolean_T f_lcda_enable; /**< Flag indicating that LCDA shoud be enabled */

   boolean_T f_bsw_enable; /**< Flag indicating that BSW should be enabled */
   boolean_T f_cvw_enable; /**< Flag indicating that CVW should be enabled */
   boolean_T f_slc_enable; /**< Flag indicating that Sim Lane Change should be enabled */
   boolean_T f_elc_enable; /**< Flag indicating that Evasive Lane Change should be enabled */

   boolean_T f_dropback_enable; /**< Flag indicating that dropback handler should be enabled */
   boolean_T f_fallback_enable; /**< Flag indicating that fallback handler should be enabled */

   HMI_CVW_DYN_TTC hmi_cvw_dyn_ttc; /**< Input signal for the TTC threshold level for the CVW objects */
} Lcda_Input_T;

#endif /* LCDA_INPUT_T_H */

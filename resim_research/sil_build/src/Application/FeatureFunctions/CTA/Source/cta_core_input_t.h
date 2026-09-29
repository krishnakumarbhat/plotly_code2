#ifndef CTA_CORE_INPUT_T_H
#define CTA_CORE_INPUT_T_H

/**
 * @file cta_core_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Core input declaration.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "cta_types.h"
#include "fbk_field_of_interest.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pt_output_t.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Structure summarizing the CTA core input
 *
 * @SRS{SF-164,SF-158}
 * @SAE{SF-2503}
 * @SDD{SF-3677}
 */
typedef struct
{
   const Pt_Output_T *p_pt_output; /**< Information on path to object relations, from Path Tracking */

   Fbk_Field_Of_Interest_T cta_zone; /**< CTA zone represented by Vectors. This is expected to be a convex polygon and expected to
                                        be  defined for RCTA, since flipping occurs in core.*/
   float32_T ttc_criticality_level[CTA_NUM_MODES][CTA_NUM_CRIT_LEVEL]; /**< TTC thresholds for criticality levels for front/rear
                                                                          mode. */
   boolean_T f_cta_switch; /**< Flag indicating if the CTA feature should be operational */

   Cta_Stop_Mode_T cta_stop_mode; /**< Determines if the TTC or TTP time of critical object is used to suppress the alert. */

   const Pa_Data_T *p_pa_data; /** overall PA data */
} Cta_Core_Input_T;


#endif /* CTA_CORE_INPUT_T_H */

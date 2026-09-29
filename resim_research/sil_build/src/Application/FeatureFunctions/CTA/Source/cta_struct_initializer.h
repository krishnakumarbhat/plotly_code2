#ifndef CTA_STRUCT_INITIALIZER_H
#define CTA_STRUCT_INITIALIZER_H

/**
 * @file cta_struct_initializer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Exports struct initialization functions of Cta.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_core_output_t.h"
#include "cta_persistent_t.h"

/*===========================================================================*\
* Global Function Definition
\*===========================================================================*/

/**
 * @brief Resets the CTA core output to its default values.
 *
 * @return void
 *
 * @SRS{SF-190}
 * @SAE{SF-2459}
 * @SDD{SF-3882}
 * @verification{Check that the CTA core output struct is reset.}
 */
void Cta_Reset_Core_Output(Cta_Core_Output_T *p_cta_core_output /**< CTA core output*/);

/**
 * @brief This function resets the CTA persistent struct to its default values.
 *
 * @return void
 *
 * @SRS{SF-193}
 * @SAE{SF-2459}
 * @SDD{SF-3883}
 * @verification{Check that the CTA persistent struct is reset.}
 */
void Cta_Reset_Persistent(Cta_Persistent_T *p_cta_persistent /**< persistent variables*/);

#endif /* CTA_STRUCT_INITIALIZER_H*/

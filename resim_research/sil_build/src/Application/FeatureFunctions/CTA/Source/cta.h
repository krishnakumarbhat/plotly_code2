#ifndef CTA_H
#define CTA_H

/**
 * @file cta.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Provides main functions for the CTA feature.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "cta_instance.h"
#include "ml_version.h"

/* Check shared toolbox preconditions */
#if Ml_Math_Library_Version_Insufficient(19, 11, 8, 0)
#error The shared toolbox used is too old. Please update the shared toolbox. http://pep.usinkok.northamerica.delphiauto.net/projectdb/public/?page=project-tasks&pid=2850.6859
#endif


/*===========================================================================*\
* Global Function Prototypess
\*===========================================================================*/

/**
 * @brief This function fills CTA_persistent, CTA_output and CTA_core_output with default values.
 *
 * @return void
 *
 * @SRS{SF-193}
 * @SAE{SF-2459}
 * @SDD{SF-3721}
 * @verification{Check that the CTA core output is reset.}
 */
void Cta_Reset(Cta_Instance_T *p_cta_instance /**< Cta Instance Pointer*/);

/**
 * @brief This main function calls Cta_Algorithm subrountine which implements CTA feature
 * - get feature enable states
 * - reset CTA params when function disabled
 * - check CTA when function enabled
 *
 * @return void
 *
 * @SRS{SF-192,SF-193}
 * @SAE{SF-2459}
 * @SDD{SF-3720}
 * @verification{Test that no active alert is set when no valid object data is provided.}
 */
void Cta_Core_Run(Cta_Instance_T *p_cta_instance /**< Cta instance pointer*/);


#endif /*CTA_H*/

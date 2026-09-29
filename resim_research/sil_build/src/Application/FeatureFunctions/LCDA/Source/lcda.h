#ifndef LCDA_H
#define LCDA_H

/**
 * @file lcda.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Exported interface of Lcda main module
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
  * Includes
\*===========================================================================*/

#include "fbk_output.h"
#include "lcda_instance.h"
#include "lcda_persistent_t.h"
#include "pa_data.h"

/*===========================================================================*\
 * Global Functions	Definition
\*===========================================================================*/

/**
 * @brief Main routine of the lcda core.
 *
 * @return void
 *
 * @SRS{SF-981,SF-982}
 * @SAE{SF-2779}
 * @SDD{SF-6552}
 * @verification{Create tests where lcda shall not trigger any warning for default tracker data and also a test where lcda trigers
 * any warning for a valid object.}
 */
void Lcda_Core_Run(Lcda_Instance_T *p_lcda_instance, const Fbk_Output_T *p_fbk_output);

/**
 * @brief Resets all persistent values of LCDA.
 *
 * @return void
 *
 * @SRS{SF-982}
 * @SAE{SF-2779}
 * @SDD{SF-6553}
 * @verification{Create a test to ceck whether persistent values are set correctly to their default.}
 */
void Lcda_Reset(Lcda_Instance_T *p_lcda_instance, const Pa_Data_T *p_pa_data);

/**
 * @brief  Resets Lcda persistent to its default.
 *
 * @return void
 *
 * @SRS{SF-982}
 * @SAE{SF-2779}
 * @SDD{SF-6537}
 * @verification{Check whether lcda persistent values are correctly set to their defaults.}
 */
void Lcda_Init_Persistent(Lcda_Persistent_T *p_lcda_persistent);

#endif /* LCDA_H */

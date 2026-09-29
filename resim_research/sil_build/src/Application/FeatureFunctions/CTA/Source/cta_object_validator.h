#ifndef CTA_OBJECT_VALIDATOR_H
#define CTA_OBJECT_VALIDATOR_H

/**
 * @file cta_object_validator.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Exports main function of object validator of Cta.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */


/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_core_calibration_t.h"
#include "cta_types.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Global Function Declaration
\*===========================================================================*/

/**
 * @brief Checks if the current object is a valid CTA candidate with
 * means of the activated functions.
 *
 * @return True if the object is a valid candidate for CTA
 *
 * @SRS{SF-199,SF-200,SF-201,SF-132,SF-202,SF-203,SF-204,SF-205,SF-206,SF-207,SF-208,SF-209}
 * @SAE{SF-2459}
 * @SDD{SF-3872}
 * @verification{Check that this function correctly detects the validity of the provided object.}
 */
boolean_T Cta_Is_Object_Valid(const Cta_Object_Data_T *p_object /**<[in,out] CTA object data*/,
                              const Cta_Core_Calibration_T *p_cta_cal /**<[in] calibration parameters*/);


#endif /*CTA_OBJECT_VALIDATOR_H*/

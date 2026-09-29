#ifndef CTA_COMMON_FUNCTIONS_H
#define CTA_COMMON_FUNCTIONS_H

/**
 * @file cta_common_functions.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the export of common functions for Cta.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */
/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_core_calibration_t.h"
#include "cta_types.h"
#include "fbk_vehicle_data_t.h"

/*===========================================================================*\
* Global Function Declaration
\*===========================================================================*/

/**
 * @brief Adapts the intersection point considering the geometric constraints of target heading and ego width.
 *
 * @return void
 *
 * @SRS{SF-236,SF-198}
 * @SAE{SF-2459}
 * @SDD{SF-3737}
 * @verification{Check that the intersection point is adapted correctly.}
 */
void Cta_Adapt_Intersec_Point_To_Object_Heading(Cta_Object_Attributes_T *p_attributes /**< cta object attributes*/,
                                                const Fbk_Vehicle_Data_T *p_vehicle_data /**< information on host vehicle*/,
                                                const Cta_Core_Calibration_T *p_cta_cal /**< Cta calibrations */,
                                                const Cta_Mode_T cta_mode /**<indicating whether we are in rear or front mode*/);
#endif /*CTA_COMMON_FUNCTIONS*/

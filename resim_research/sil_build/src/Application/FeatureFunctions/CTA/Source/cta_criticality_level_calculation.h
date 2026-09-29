#ifndef CTA_CRITICALITY_LEVEL_CALCULATION_H
#define CTA_CRITICALITY_LEVEL_CALCULATION_H

/**
 * @file cta_criticality_level_calculation.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the criticality level calculation functions definitions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_core_calibration_t.h"
#include "cta_persistent_t.h"
#include "cta_types.h"
#include "fbk_vehicle_data_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Global Function Prototypess
\*===========================================================================*/

/**
 * This superordinate function checks if current object is the one with the highest criticality level.
 * The call of subroutines is as follows:
 *	- Initialization of the criticality criteria: TTC, radial distance, intersection point with x axis
 *	- Adapt the thresholds when the corresponding criticality level is activated in the previous cycle
 *	- Evaluate the criticality level of the target_id
 *	- Determine the criticality level in the current cycle
 * Function returns true if:
 *	- current criticality level is higher than the previous cycle
 *	- current criticality level is equal to the previous cycle, two cases are crucial: \n
 *		1) TTC value shall be no larger than the previous cycle \n
 *		2) when TTC value equals, radial distance shall be no larger than the previous cycle
 *
 * @return void
 *
 * @SRS{SF-231}
 * @SAE{SF-2459}
 * @SDD{SF-3791}
 * @verification{Check that the criticality level is determined correctly.}
 */
void Cta_Check_All_Level(Cta_Crit_Level_Calibration_T *p_extended_crit_level /**< extended criticality level calibration */,
                         Cta_Comparison_Data_T *p_cta_comparison_data /**< comparison data for criticality level*/,
                         const Cta_Persistent_T *p_persistent /**< persistent variables*/,
                         const Cta_Object_Data_T *p_object /**< CTA object data*/,
                         const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/,
                         const boolean_T f_target_in_zone /**< flag indicating whether object is in zone*/,
                         const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/,
                         const Cta_Mode_T cta_mode /**< mode of cta. 0 - rear cta, 1 - front cta */);


#endif /*CTA_CRITICALITY_LEVEL_CALCULATION_H*/

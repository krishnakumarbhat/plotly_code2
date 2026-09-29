#ifndef CTA_CONFLICT_ZONE_ADAPTER_H
#define CTA_CONFLICT_ZONE_ADAPTER_H

/**
 * @file cta_conflict_zone_adapter.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the CTA conflict zone adapter function definitions.
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
 * @brief Calculates the host dependend extension factors of the conflict zone adaption
 *
 * @return void
 *
 * @SRS{SF-230}
 * @SAE{SF-2459}
 * @SDD{SF-3748}
 * @verification{Check that the host dependend extension factors are calculated correctly.}
 */
void Cta_Calc_Host_Dep_Ext_Fac(
   Cta_Inters_Zone_Ext_Param_T *p_confl_zone_ext_fac /**< zone extension which has to be applied to the conflict zone*/,
   const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/,
   const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/);


/**
 * @brief Calculates the conflict zone extension which is based on the target object.
 * Also Merges the conflict zone extensions which are caused by the host vehicle on the first side
 * and by the target on the other side.
 *
 * @return void
 *
 * @SRS{SF-230}
 * @SAE{SF-2459}
 * @SDD{SF-3747}
 * @verification{Check that the conflict zone extension is calculated correctly.}
 */
void Cta_Adapt_Long_Crit_Level_Ranges(Cta_Crit_Level_Calibration_T *p_crit_level_calibration /**< extended level calibration*/,
                                      Cta_Inters_Zone_Ext_Param_T *p_confl_zone_ext_params /**< conflict zone extension*/,
                                      const Cta_Object_Data_T *p_object /**<  cta object*/,
                                      const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/,
                                      const Cta_Mode_T cta_mode /**< mode of cta */);


#endif /*CTA_CONFLICT_ZONE_ADAPTER_H*/

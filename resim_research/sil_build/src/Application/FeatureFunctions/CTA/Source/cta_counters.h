#ifndef CTA_COUNTERS_H
#define CTA_COUNTERS_H

/**
 * @file cta_counters.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for CTA holding logic of object criticality level.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_core_calibration_t.h"
#include "cta_persistent_t.h"
#include "cta_types.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

/**
 * @brief Processes the current CTA warning level and also
 * makes use of an holding logic.
 *
 * @return warn level for a given side and an object with highest criticality of type uint8_t
 *
 * @SRS{SF-222,SF-226}
 * @SAE{SF-2459}
 * @SDD{SF-3761}
 * @verification{Check that the current CTA alert level is processed correctly.}
 */
Cta_Crit_Level_T Cta_Process_Current_Alert_Level(Cta_Persistent_T *p_cta_persistent /**< holding counter*/,
                                                 const Cta_Crit_Level_T warning_level /**<warning level*/,
                                                 const Cta_Mode_T cta_mode /**<cta mode*/,
                                                 const uint8_t approach_direction /**<approach direction*/,
                                                 const Cta_Object_Data_T *p_object_data /**< object data*/,
                                                 const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/);


/**
 * @brief Checks if CTA shall stop the level holding procedure and sets the holding
 * counter to its maximum value.
 *
 * @return void
 *
 * @SRS{SF-236,SF-198}
 * @SAE{SF-2459}
 * @SDD{SF-3760}
 * @verification{Check that a stop of the level holding is detected correctly.}
 */
void Cta_Check_Stop_Level_Holding(
   Cta_Persistent_T *p_cta_persistent /**< holding counter*/,
   const Cta_Object_Data_T *p_object /**< object data*/,
   const boolean_T f_within_field_of_interest /**<flag indicating if object is within field of interest*/);


#endif /* CTA_COUNTERS_H */

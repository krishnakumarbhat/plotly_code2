#ifndef LTB_WARN_LOGIC_H
#define LTB_WARN_LOGIC_H

/**
 * @file ltb_warn_logic.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements logic for determining if a warning or info is determined by the LTB algorithm.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ltb_core_calibration_t.h"
#include "ltb_core_output_t.h"
#include "ltb_persistent_t.h"
#include "ltb_types.h"

/**
 * @brief Sets the object criticality based on calibrated alert level thresholds.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53942}
 *
 * @verification{Create tests where either ttc or ttp based alert levels are set. Only when subconditions are fulfilled, an alert
 * level shall be set.}
 */
void Ltb_Set_Object_Criticality(Ltb_Object_T *p_ltb_object /**< LTB Object */,
                                const Ltb_Core_Calibration_T *p_ltb_cal /**< LTB Calibration */);

/**
 * @brief Sets the most critical object for each vehicle side.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53949}
 *
 * @verification{Create tests where the passed p_ltb_object is checked against the currently stored one in the core output.
 * Scenario shall be tested: When the ttc of p_ltb_object is less than the one in core output, p_ltb_object is more critical.}
 */
void Ltb_Set_Most_Critical_Object_Per_Side(Ltb_Core_Output_T *p_ltb_core_output /**< LTB Core Output */,
                                           const Ltb_Object_T *p_ltb_object /**< LTB Object */);

/**
 * @brief Sets the most critical side for this cycle.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53950}
 *
 * @verification{Create tests where the logic of this unit is verified: 1.) When alert level are set, the higher alert level
 * specifies the most critical side. 2.) When alert level is equals on both sides, the smaller ttc specifies the most critical
 * side. 3.) When alert level are not set and ttcs are in valid boundaries, the smaller distance to host specifies the most
 * critical side.}
 */
void Ltb_Set_Most_Critical_Side(Ltb_Core_Output_T *p_ltb_core_output /**< LTB Core Output */);

/**
 * @brief Debounces the alert levels using qualifying and holding counters.
 *
 * @return void
 *
 * @SRS{CSCSA-30421}
 * @SAE{CSCSA-70554}
 * @SDD{CSCSA-63525}
 *
 * @verification{Create tests where outputs need to qualify for an alert or where outputs are held for a successive amount of
 * cycles. Only if conditions are not fulfilled, the LTB core output shall be reset.}
 */
void Ltb_Debounce_Alert_Level(Ltb_Core_Output_T *p_ltb_core_output /**< LTB Core Output */,
                              Ltb_Persistent_T *p_ltb_persistent /**< LTB Persistent */,
                              const Ltb_Core_Calibration_T *p_ltb_cal /**< LTB Calibration */);


#endif /* LTB_WARN_LOGIC_H */

#ifndef TA_WARN_LOGIC_H
#define TA_WARN_LOGIC_H

/**
 * @file ta_warn_logic.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements logic for determining if a warning or info is determined by the TA algorithm.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ta_core_calibration_t.h"
#include "ta_core_input_t.h"
#include "ta_core_output_t.h"
#include "ta_persistent_t.h"
#include "ta_types.h"

/**
 * @brief Sets the object criticality based on calibrated alert level thresholds.
 *
 * @return void
 *
 * @SRS{SF-2320,SF-2317,SF-2299}
 * @SAE{SF-3238}
 * @SDD{SF-8766}
 * @verification{Create tests where either ttc or ttp based alert levels are set. Only when subconditions are fulfilled, an alert
 * level shall be set.}
 */
void Ta_Set_Object_Criticality(Ta_Object_T *p_ta_object /**< TA Object */,
                               const Ta_Core_Input_T *p_ta_core_input /**< TA Core Input */,
                               const Ta_Persistent_T *p_ta_persistent /**< Ta persistent data */,
                               const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Sets the most critical object for each vehicle side.
 *
 * @return void
 *
 * @SRS{SF-2321}
 * @SAE{SF-3238}
 * @SDD{SF-8764}
 * @verification{Create tests where the passed p_ta_object is checked against the currently stored one in the core output. Multiple
 * scenarios shall be tested: 1.) When the ttp of p_ta_object is less than the one in core output, p_ta_object is more
 * critical. 2.) When the ttc of p_ta_object is less than the one in core output, p_ta_object is more critical. 3.) When both ttcs
 * are equal, the object closer to the host, is more critical.}
 */
void Ta_Set_Most_Critical_Object_Per_Side(Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */,
                                          const Ta_Object_T *p_ta_object /**< TA Object */,
                                          const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Sets the most critical side for this cycle.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8765}
 * @verification{Create tests where the logic of this unit is verified: 1.) When alert level are set, the higher alert level
 * specifies the most critical side. 2.) When alert level is equals on both sides, the smaller ttc specifies the most critical
 * side. 3.) When alert level are not set and ttcs are in valid boundaries, the smaller distance to host specifies the most
 * critical side. 4.) When alert level are not set and ttcs are not within valid boundaries, the smaller ttp specifies the most
 * critical side.}
 */
void Ta_Set_Most_Critical_Side(Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */);


#endif /* TA_WARN_LOGIC_H */

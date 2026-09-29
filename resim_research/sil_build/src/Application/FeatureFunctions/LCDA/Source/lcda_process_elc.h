#ifndef LCDA_PROCESS_ELC_H
#define LCDA_PROCESS_ELC_H

/**
 * @file lcda_process_elc.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Exports functions for evasive lane change (ELC) module
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_object_data_t.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_persistent_t.h"
#include "lcda_types.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

/**
 * @brief Returns the Elc object specific zone which might be either the Elc zone or the Elc zone with hysteresis applied.
 *
 * @return void
 *
 * @SRS{SF-1085,SF-1089,SF-1092,SF-1093,SF-1097}
 * @SAE{SF-2779}
 * @SDD{SF-6717}
 * @verification{Create tests where either the object was Elc relevant in the last cycle and where an object was Elc bsw relevant
 * before. In the first case the hysteresis zone is expected. In the second the bare Elc zone shall be returned.}
 */
void Lcda_Create_Elc_Object_Zone(Elc_Object_T *p_elc_object /**< Elc object */,
                                 const uint8_t mature_count_in_elc_zone /**< Elc object maturity count */,
                                 const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                                 const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Resets Elc Core Output and persistent data to default values.
 *
 * @return void
 *
 * @SRS{SF-992}
 * @SAE{SF-2779}
 * @SDD{SF-6721}
 * @verification{Check that the Elc core output and persistent data is reset correctly to its default.}
 */
void Lcda_Reset_Elc_Core(Lcda_Elc_Core_Output_T *p_elc_core_output /**< Elc core output */, Lcda_Elc_Persistent_T *p_elc_persistent);

/**
 * @brief Resets Elc Core Output to default values.
 *
 * @return void
 *
 * @SRS{SF-1105}
 * @SAE{SF-2779}
 * @SDD{SF-6719}
 * @verification{Check that the Elc core output is reset correctly to its default.}
 */
void Lcda_Preprocess_Elc(Lcda_Elc_Core_Output_T *p_elc_core_output /**< Elc core output */,
                         const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                         const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Main routine for Elc calculations for each object.
 *
 * @return void
 *
 * @SRS{SF-1095,SF-1109}
 * @SAE{SF-2779}
 * @SDD{SF-6720}
 * @verification{Checks whether a Elc relevant candidate object is causing a Elc alert. Also check that default data of tracker is
 * not causing any Elc alert.}
 */
void Lcda_Process_Elc_Object(Lcda_Elc_Core_Output_T *p_elc_core_output /**< Elc core output */,
                             const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
                             const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                             const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                             Lcda_Elc_Persistent_T *p_elc_persistent);

/**
 * @brief Fills the Elc persistent data with Elc core output.
 *
 * @return void
 *
 * @SRS{SF-1102,SF-1103}
 * @SAE{SF-2779}
 * @SDD{SF-6718}
 * @verification{Check that Elc persistent data is filled correctly.}
 */
void Lcda_Postprocess_Elc(Lcda_Elc_Core_Output_T *p_elc_core_output /**< Elc core output */,
                          const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                          Lcda_Elc_Persistent_T *p_elc_persistent);

/**
 * @brief Initializes Elc object persistent data to its default.
 *
 * @return void
 *
 * @SRS{SF-1105}
 * @SAE{SF-2779}
 * @SDD{SF-6705}
 * @verification{Check whether Elc object persistent data is reset correctly.}
 */
void Lcda_Clear_Elc_Persistent(Lcda_Elc_Persistent_T *p_elc_persistent /**< ELc persistent data */);

#endif /* LCDA_PROCESS_ELC_H */

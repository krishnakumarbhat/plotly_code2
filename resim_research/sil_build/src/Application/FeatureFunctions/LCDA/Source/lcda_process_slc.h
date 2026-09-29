#ifndef LCDA_PROCESS_SLC_H
#define LCDA_PROCESS_SLC_H

/**
 * @file lcda_process_slc.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Exports functions for the simultaneous lane change (Slc) module
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
* Global Function Prototypess
\*===========================================================================*/

/**
 * @brief Returns the Slc object specific zone which might be either the Slc zone or the Slc zone with hysteresis applied.
 *
 * @return void
 *
 * @SRS{SF-1072,SF-1074}
 * @SAE{SF-2779}
 * @SDD{SF-6742}
 * @verification{Create tests where either the object was Slc relevant in the last cycle and where an object was Slc bsw relevant
 * before. In the first case the hysteresis zone is expected. In the second the bare Slc zone shall be returned.}
 */
void Lcda_Create_Slc_Object_Zone(Slc_Object_T *p_slc_object /**<Slc object*/,
                                 const uint8_t mature_count_in_slc_zone /**<maturity count of object in zone*/,
                                 const Lcda_Core_Input_T *p_core_input /**<Lcda core input*/,
                                 const Lcda_Core_Calibration_T *p_cals /**<Lcda calibrations*/);

/**
 * @brief Resets Slc Core Output and persistent data to default values.
 *
 * @return void
 *
 * @SRS{SF-992}
 * @SAE{SF-2779}
 * @SDD{SF-6746}
 * @verification{Check that the Slc core output and persistent data is reset correctly to its default.}
 */
void Lcda_Reset_Slc_Core(Lcda_Slc_Core_Output_T *p_slc_core_output /**< Slc core output*/, Lcda_Slc_Persistent_T *p_slc_persistent);

/**
 * @brief Resets Slc Core Output to default values.
 *
 * @return void
 *
 * @SRS{SF-1107}
 * @SAE{SF-2779}
 * @SDD{SF-6744}
 * @verification{Check that the Slc core output is reset correctly to its default.}
 */
void Lcda_Preprocess_Slc(Lcda_Slc_Core_Output_T *p_slc_core_output /**< Slc core output */,
                         const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                         const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Main routine for Slc calculations for each object.
 *
 * @return void
 *
 * @SRS{SF-1076}
 * @SAE{SF-2779}
 * @SDD{SF-6745}
 * @verification{Checks whether a Slc relevant candidate object is causing a Slc alert. Also check that default data of tracker is
 * not causing any Slc alert.}
 */
void Lcda_Process_Slc_Object(Lcda_Slc_Core_Output_T *p_slc_core_output /**< Slc core output */,
                             const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
                             const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                             const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                             Lcda_Slc_Persistent_T *p_slc_persistent);

/**
 * @brief Fills the Slc persistent data with Slc core output.
 *
 * @return void
 *
 * @SRS{SF-1099,SF-1106}
 * @SAE{SF-2779}
 * @SDD{SF-6743}
 * @verification{Check that Slc persistent data is filled correctly.}
 */
void Lcda_Postprocess_Slc(Lcda_Slc_Core_Output_T *p_slc_core_output /**< Slc core output*/,
                          const Lcda_Core_Calibration_T *p_cals /**<Lcda calibrations*/,
                          Lcda_Slc_Persistent_T *p_slc_persistent);

/**
 * @brief Initializes Slc object persistent data to its default.
 *
 * @return void
 *
 * @SRS{SF-1099}
 * @SAE{SF-2779}
 * @SDD{SF-6729}
 * @verification{Check whether Slc object persistent data is reset correctly.}
 */
void Lcda_Clear_Slc_Persistent(Lcda_Slc_Persistent_T *p_slc_persistent /**< Slc persistent data */);

#endif /* LCDA_PROCESS_SLC_H */

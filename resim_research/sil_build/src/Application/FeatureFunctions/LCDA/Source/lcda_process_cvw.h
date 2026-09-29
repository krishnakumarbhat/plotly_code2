#ifndef LCDA_PROCESS_CVW_H
#define LCDA_PROCESS_CVW_H

/**
 * @file lcda_process_cvw.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Exported functions for warnings in case of an object approaches on the adjacent lane
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_persistent_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

/**
 * @brief Resets Cvw core output as well as Cvw persistent data.
 *
 * @return void
 *
 * @SRS{SF-992}
 * @SAE{SF-2779}
 * @SDD{SF-6698}
 * @verification{Check that Cvw persistent data and core output is reset correctly.}
 */
void Lcda_Reset_Cvw_Core(Lcda_Cvw_Core_Output_T *p_cvw_core_output /**< Cvw core output */,
                         Lcda_Persistent_T *p_lcda_persistent /**< Lcda persistent data */,
                         Lcda_Cvw_Persistent_T *p_cvw_persistent);

/**
 * @brief Prepares information for merged objects and resets the cvw core output.
 *
 * @return void
 *
 * @SRS{SF-1019}
 * @SAE{SF-2779}
 * @SDD{SF-6696}
 * @verification{Check that cvw core output information is reset to its default and that merged objects information are prepared
 * correctly.}
 */
void Lcda_Preprocess_Cvw(Lcda_Cvw_Core_Output_T *p_cvw_core_output /**< Cvw core output */,
                         float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES] /**< cvw ttc on each side*/,
                         boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] /**< Flags indicating which zone to use */,
                         const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                         const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations*/,
                         const Fbk_Output_T *p_fbk_output,
                         Lcda_Cvw_Persistent_T *p_cvw_persistent);

/**
 * @brief Main routine for Cvw calculations for each object.
 *
 * @return void
 *
 * @SRS{SF-1015,SF-1079}
 * @SAE{SF-2779}
 * @SDD{SF-6697}
 * @verification{Checks whether a cvw relevant candidate object is causing a bsw alert. Also check that default data of tracker is
 * not causing any cvw alert.}
 */
void Lcda_Process_Cvw_Object(
   Lcda_Cvw_Core_Output_T *p_cvw_core_output /**< Cvw core output */,
   const float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES] /**< cvw ttc on each side*/,
   const boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] /**< Flags indicating which zone to use */,
   const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
   const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
   const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations*/,
   const Lcda_Persistent_T *p_lcda_persistent /**< Lcda persistent data */,
   Lcda_Cvw_Persistent_T *p_cvw_persistent);

/**
 * @brief Fills the Cvw core output with bsw persistent data.
 *
 * @return void
 *
 * @SRS{SF-1116,SF-1020,SF-1066}
 * @SAE{SF-2779}
 * @SDD{SF-6695}
 * @verification{Check that Cvw core output is filled correctly.}
 */
void Lcda_Postprocess_Cvw(
   Lcda_Cvw_Core_Output_T *p_cvw_core_output /**< Cvw core output */,
   const boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] /**< Flags indicating which zone to use */,
   const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
   const Lcda_Persistent_T *p_lcda_persistent /**< Lcda persistent data */,
   Lcda_Cvw_Persistent_T *p_cvw_persistent);

/**
 * @brief Checks whether in previous cycle a cvw alert was triggered by the given object identifier.
 *
 * @return True when object triggered a cvw warning before
 *
 * @SRS{SF-1114}
 * @SAE{SF-2779}
 * @SDD{SF-6694}
 * @verification{Verify that true is returned when the given object caused a cvw warning in the last cycle.}
 */
boolean_T Lcda_Is_Prev_Cvw_Alert_Active_On_Obj(const Lcda_Cvw_Persistent_T *p_cvw_persistent,
                                               const uint8_t obj_id /**< object identifier */,
                                               const uint8_t side /**< side index */);

/**
 * @brief Stores the given curve zone factor for the given side.
 *
 * @return void
 *
 * @SRS{SF-1114}
 * @SAE{SF-2779}
 * @SDD{SF-6699}
 * @verification{Check that the curve zone factor is stored correctly on the given side.}
 */
void Lcda_Store_Pers_Cvw_Curve_Zone_Factor(Lcda_Cvw_Persistent_T *p_cvw_persistent,
                                           const float32_T factor /**< Cvw curve zone factor */,
                                           const uint8_t side /**< side index */);

/**
 * @brief Returns Cvw curve zone factor.
 *
 * @return curve zone factor of type float32_T
 *
 * @SRS{SF-1114}
 * @SAE{SF-2779}
 * @SDD{SF-6692}
 * @verification{Check whether the cprrect curve zone factor of the respective side is returned.}
 */
float32_T Lcda_Get_Pers_Prev_Cvw_Curve_Zone_Factor(const uint8_t side /**< side index */,
                                                   const Lcda_Cvw_Persistent_T *p_cvw_persistent);

/**
 * @brief Returns identifier of the object responsible for the cvw warning in the last cycle for the respective side.
 *
 * @return identifier of object of type uint8_t
 *
 * @SRS{SF-1020}
 * @SAE{SF-2779}
 * @SDD{SF-6693}
 * @verification{Check whether the index is returned correctly when in the last cycle a warning was given.}
 */
uint8_t Lcda_Get_Prev_Cvw_Alert_Object_Id_On_Side(const uint8_t side /**< side index */,
                                                  const Lcda_Cvw_Persistent_T *p_cvw_persistent);

/**
 * @brief Resets all Cvw persistent data.
 *
 * @return void
 *
 * @SRS{SF-1020,SF-1015}
 * @SAE{SF-2779}
 * @SDD{SF-6684}
 * @verification{Check whether persistent cvw data is correctly reset to its default.}
 */
void Lcda_Reset_Cvw_Persistent_Data(Lcda_Cvw_Persistent_T *p_cvw_persistent /**<Cvw persistent data*/);

#endif /* LCDA_PROCESS_CVW_H */

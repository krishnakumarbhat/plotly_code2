#ifndef LCDA_PROCESS_BSW_H
#define LCDA_PROCESS_BSW_H

/**
 * @file lcda_process_bsw.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Exported function for blindspot warning detection.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_persistent_t.h"

/*===========================================================================*\
* Global Function Prototypess
\*===========================================================================*/

/**
 * @brief Prepares information for merged objects and resets the bsw core output.
 *
 * @return void
 *
 * @SRS{SF-1069}
 * @SAE{SF-2779}
 * @SDD{SF-6666}
 * @verification{Check that bsw core output information is reset to its default and that merged objects information are prepared
 * correctly.}
 */
void Lcda_Preprocess_Bsw(Lcda_Bsw_Core_Output_T *p_bsw_core_output /**< Bsw core output */,
                         const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                         const Lcda_Core_Calibration_T *p_cals /**<Lcda calibrations*/,
                         const Fbk_Output_T *p_fbk_output,
                         Lcda_Bsw_Persistent_T *p_bsw_persistent);


/**
 * @brief Main routine for Bsw calculations for each object.
 *
 * @return void
 *
 * @SRS{SF-1062}
 * @SAE{SF-2779}
 * @SDD{SF-6667}
 * @verification{Checks whether a bsw relevant candidate object is causing a bsw alert. Also check that default data of tracker is
 * not causing any bsw alert.}
 */
void Lcda_Process_Bsw_Object(Lcda_Bsw_Core_Output_T *p_bsw_core_output /**< Bsw core output */,
                             const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
                             const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                             const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                             const Lcda_Persistent_T *p_lcda_persistent /**< Lcda persistent data */,
                             Lcda_Bsw_Persistent_T *p_bsw_persistent /**< Bsw persistent data */,
                             const Lcda_Cvw_Persistent_T *p_cvw_persistent);

/**
 * @brief Fills the bsw core output with bsw persistent data.
 *
 * @return void
 *
 * @SRS{SF-1067,SF-1068,SF-1115,}
 * @SAE{SF-2779}
 * @SDD{SF-6665}
 * @verification{Check that bsw core output is filled correctly.}
 */
void Lcda_Postprocess_Bsw(Lcda_Bsw_Core_Output_T *p_bsw_core_output /**< Bsw core output */,
                          Lcda_Bsw_Persistent_T *p_bsw_persistent /**< Bsw persistent data */,
                          const Lcda_Core_Input_T *p_core_input /**<Lcda core input */,
                          const Lcda_Core_Calibration_T *p_cals /**<Lcda calibrations*/,
                          const Lcda_Persistent_T *p_lcda_persistent /**<Lcda persistent data */,
                          const Fbk_Output_T *p_fbk_output);

/**
 * @brief Resets bsw internal and related persistent data as well as the bsw core output.
 *
 * @return void
 *
 * @SRS{SF-992}
 * @SAE{SF-2779}
 * @SDD{SF-6668}
 * @verification{Check whether persistent data and core output is correctly reset to its default.}
 */
void Lcda_Reset_Bsw_Core(Lcda_Bsw_Core_Output_T *p_bsw_core_output /**< Bsw core output */,
                         Lcda_Bsw_Persistent_T *p_bsw_persistent /**< Bsw persistent data */,
                         Lcda_Persistent_T *p_lcda_persistent /**<Lcda persistent data */,
                         const Lcda_Core_Input_T *p_core_input /** Lcda core input*/);
/**
 * @brief Resets persistent bsw data to its default.
 *
 * @return void
 *
 * @SRS{SF-1067}
 * @SAE{SF-2779}
 * @SDD{SF-6655}
 * @verification{Create a test to check whether bsw persistent data is reset correctly.}
 */
void Lcda_Reset_Bsw_Persistent_Data(Lcda_Bsw_Persistent_T *p_bsw_persistent /**< Bsw persistent data */);

#endif /* LCDA_PROCESS_BSW_H */

#ifndef TA_FACTORY_H
#define TA_FACTORY_H

/**
 * @file ta_factory.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module makes the global variables accessible to other modules.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_field_of_interest.h"
#include "fbk_traj_predictor_t.h"
#include "pa_reuse.h"
#include "ta_core_calibration_t.h"
#include "ta_persistent_t.h"

/*============================================================================*\
* EXPORTED FUNCTIONS PROTOTYPES
\*============================================================================*/

/**
 * @brief Resets the TA trajectory struct.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8699}
 * @verification{Create a test to check that trajectory properties are reset to their default.}
 */
void Ta_Reset_Trajectory(Fbk_Trajectory_T *p_ta_trajectory /**< TA trajectory */,
                         const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Creates the danger zones for FTA
 *
 * @return void
 *
 * @SRS{SF-2304,SF-2333,SF-2335,SF-2336,SF-2337,SF-2338,SF-2339,SF-2340,SF-2341,SF-2349,SF-2355,SF-2363,SF-2268}
 * @SAE{SF-3238}
 * @SDD{SF-8696}
 * @verification{Create a test to check that trajectory properties are reset to their default.}
 */
void Ta_Create_Danger_Zones(Fbk_Field_Of_Interest_T *p_danger_zone_left /**< Danger zone on ego left side */,
                            Fbk_Field_Of_Interest_T *p_danger_zone_right /**< Danger zone on ego right side */,
                            const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Creates the info zones for RTA
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8697}
 * @verification{Create a test to check that info zone properties are reset to their default.}
 */
void Ta_Create_Info_Zones(Fbk_Field_Of_Interest_T *p_info_zone_left /**< Info zone on ego left side */,
                          Fbk_Field_Of_Interest_T *p_info_zone_right /**< Info zone on ego right side */,
                          const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */,
                          const boolean_T f_zone_hysteresis /**< Flag to indicate zone hysteresis */);

/**
 * @brief Creates the wing zones for RTA
 *
 * @return     void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8698}
 * @verification{Create a test to check that wing zone properties are reset to their default.}
 */
void Ta_Create_Wing_Zones(Fbk_Field_Of_Interest_T *p_wing_zone_left /**< Wing zone on ego left side */,
                          Fbk_Field_Of_Interest_T *p_wing_zone_right /**< Wing zone on ego right side */,
                          const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */,
                          const boolean_T f_zone_hysteresis /**< Flag to indicate zone hysteresis */);

/**
 * @brief Fills the FBK data struct about host parameters, using calibraiton and persistent data.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-8630}
 * @verification{Check that the object information is updated correctly.}
 */
void Ta_Fill_Fbk_Predict_Ego_Struct(Fbk_Ego_Predict_Data_T *p_fbk_ego_data /**< Host data*/,
                                    const Ta_Persistent_T *p_ta_persistent /**< TA Persistent */,
                                    const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);
/**
 * @brief Fills the FBK data struct using calibraiton and persistent data.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-8631}
 * @verification{Check that the object information is updated correctly.}
 */
void Ta_Fill_Fbk_Predict_Obj_Struct(Fbk_Object_Predict_Data_T *p_fbk_obj_data /**< Object data*/,
                                    const Ta_Persistent_T *p_ta_persistent /**< TA Persistent */,
                                    const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

#endif /* TA_FACTORY_H */

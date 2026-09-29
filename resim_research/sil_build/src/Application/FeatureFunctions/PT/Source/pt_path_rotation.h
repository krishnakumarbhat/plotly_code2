#ifndef PT_PATH_ROTATION_H
#define PT_PATH_ROTATION_H
/**
 * @file pt_path_rotation.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains main function for the path rotation module.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_vehicle_data_t.h"
#include "pt_core_calibration_t.h"
#include "pt_input_t.h"
#include "pt_persistent_t.h"

/*===========================================================================*\
* Global Function Prototypess
\*===========================================================================*/

/**
 * @brief This superordinate function calls all subroutines which are needed for rotation of paths.
 *
 * @return void
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{SF-2918}
 * @SDD{SF-7527}
 * @verification{}
 */
void Pt_Rotate_Recorded_Paths(Pt_Persistent_T *p_pt_persistent /**< persistent data*/,
                              const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
                              const Pt_Input_T *p_pt_input /**< pt input data */,
                              const Fbk_Vehicle_Data_T *p_vehicle_data /**< vehicle data*/);

#endif

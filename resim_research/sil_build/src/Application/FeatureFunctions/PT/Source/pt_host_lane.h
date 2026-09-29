#ifndef PT_HOST_LANE_H
#define PT_HOST_LANE_H

/**
 * @file pt_host_lane.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains exported routines for creation of trail information and tranformation of those into path information.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

/*Internal includes*/
#include "fbk_iface_types.h"
#include "fbk_vehicle_data_t.h"
#include "pt_core_calibration_t.h"
#include "pt_input_t.h"
#include "pt_persistent_t.h"

/*===========================================================================*\
* Global Functions Declaration
\*===========================================================================*/

/**
 * @brief Transforms recorded trail to path information useable within path tracking algorithm.
 *
 * @return void.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7629}
 * @verification{}
 */
void Pt_Transform_Trail_To_Path(Pt_Persistent_T *p_pt_persistent /**< persistent data*/,
                                const Pt_Core_Calibration_T *p_cals /**< calibration data */,
                                const Pt_Input_T *p_pt_input /**< path tracking input*/,
                                const Fbk_Vehicle_Data_T *p_vehicle_data /**< vehicle data */,
                                const Fbk_Host_Trail_T *p_fbk_host_trail);

#endif /* PT_HOST_LANE_H */

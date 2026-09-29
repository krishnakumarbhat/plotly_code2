#ifndef PT_OBJECT_MATCHING_H
#define PT_OBJECT_MATCHING_H
/**
 * @file pt_object_matching.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the main function for the object to path matching module of path tracking.
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
#include "pt_output_t.h"
#include "pt_persistent_t.h"

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

/**
 * @brief iterates through given objects and calls subroutines needed for
 * object to path matching
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7478}
 * @verification{}
 */
void Pt_Objects_To_Path_Matching(Pt_Persistent_T *p_pt_persistent /**<persistent data of Path Tracking*/,
                                 Pt_Output_T *pt_output /**< path output struct*/,
                                 const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
                                 const Pt_Input_T *p_pt_input /**< path tracking input*/,
                                 const Fbk_Vehicle_Data_T *p_vehicle_data /**<vehicle data*/);

#endif

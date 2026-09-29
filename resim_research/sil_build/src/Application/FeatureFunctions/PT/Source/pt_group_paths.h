#ifndef PT_GROUP_PATHS_H
#define PT_GROUP_PATHS_H
/**
 * @file pt_group_paths.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains main function of the grouping module of path tracking.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

/* Internal includes */
#include "pt_core_calibration_t.h"
#include "pt_input_t.h"
#include "pt_persistent_t.h"

/*===========================================================================*\
* Global Function Prototypess
\*===========================================================================*/

/**
 * @brief This superordinate function calls the procedures necessary for the path
 * grouping module.
 *
 * @return void
 *
 * @SRS{SF-1575}
 * @SAE{SF-2918}
 * @SDD{SF-7401}
 * @verification{}
 */
void Pt_Group_Paths(Pt_Persistent_T *p_pt_persistent /**<persistent data of Path tracking*/,
                    Pt_Path_T *path_to_check /**< one path to check against all other paths*/,
                    const Pt_Core_Calibration_T *p_cals /**<calibration parameters*/,
                    const Pt_Input_T *p_pt_input /**<path tracking input*/);


#endif

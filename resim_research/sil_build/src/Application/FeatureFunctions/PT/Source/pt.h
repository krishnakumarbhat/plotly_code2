#ifndef PT_H
#define PT_H

/**
 * @file pt.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Main functions for the path creation management.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pt_core_calibration_t.h"
#include "pt_input_t.h"
#include "pt_output_t.h"
#include "pt_persistent_t.h"
/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

/**
 * This function calls the main step procedures needed for the path algorithm.
 * At first the cal values which depend on the absolute num of grid points are adapted to the
 * grid point array range which is considered. Then Paths are created within Path_Track. And at
 * the end objects are matched to those paths within the Pt_Objects_To_Path_Matching function.
 *
 * @return     void
 *
 * @SRS{SF-1522,SF-1520,SF-1521}
 * @SAE{SF-2918}
 * @SDD{SF-7643}
 * @verification{}
 *
 */
void Pt_Core_Run(Pt_Output_T *p_pt_output,
                 Pt_Persistent_T *p_pt_persistent,
                 const Pt_Input_T *p_pt_input,
                 const Pt_Core_Calibration_T *p_cals);
#endif

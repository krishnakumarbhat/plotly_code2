#ifndef PT_MODIFY_GRID_CALS_H
#define PT_MODIFY_GRID_CALS_H
/**
 * @file pt_modify_grid_cals.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the modification of grid dependend cals of path tracking algorithm.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

/* Internal includes */
#include "pt_constants.h"
#include "pt_core_calibration_t.h"
#include "pt_input_t.h"

/*Fbk includes*/
#include "pa_reuse.h"

/*===========================================================================*\
* Global Functions Prototypes
\*===========================================================================*/

/**
 * @brief adapts every cal value which is dependend on the num of grid points provided. For that
 * a cal was introduced that describes the default range of the tracking zone (60m).
 *
 * @return Pt_Num_Grid_Pts_Dep_Cals_T with adapted cals which are dependend on amount of grid points
 *
 * @SRS{SF-1522,SF-1520,SF-1521}
 * @SAE{SF-2918}
 * @SDD{SF-7437}
 * @verification{}
 */
void Pt_Set_Num_Grid_Pts_Dependend_Cals(
   const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid point array*/,
   Pt_Num_Grid_Pts_Dep_Cals_T *p_num_grid_pts_dep_cals /**< calibration parameters depending on amount of grid points*/);

#endif

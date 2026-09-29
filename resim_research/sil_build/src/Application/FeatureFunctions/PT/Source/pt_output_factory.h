#ifndef PT_OBJECT_FACTORY_H
#define PT_OBJECT_FACTORY_H
/**
 * @file pt_output_factory.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Main function for the path-object pair property calculation and constructor for the path tracking output.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_vehicle_data_t.h"
#include "pt_common_functions.h"
#include "pt_core_calibration_t.h"
#include "pt_directions.h"
#include "pt_input_t.h"
#include "pt_output_t.h"
#include "pt_persistent_t.h"
#include "pt_types.h"

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

/**
 * @brief  calls subroutines which are needed to fill the attributes of the p_path_out struct for the path with which the object
 * gets matched to.
 *
 * @return void
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7499}
 * @verification{}
 */
void Pt_Set_Best_Matching_Path_Output(
   Pt_Output_T *p_pt_output /**<pt output struct*/,
   Pt_Persistent_T *p_pt_persistent /**<pt persistent data*/,
   const Pt_Path_Obj_Pair_Consumer_Info_T *p_path_consumer_info /**<consumer info for a not necessarily match*/,
   const Pt_Input_T *p_pt_input /**<path tracking input*/,
   const Pt_Object_T *p_object /**<object data*/,
   const Pt_Core_Calibration_T *p_cals /**<calibration parameters*/,
   const Pt_Object_Mov_Direction_T obj_move_dir /**<moving direction of object*/,
   const Pt_Point_Indices_Dir_Indep_Obj_T
      *p_next_point_idx /**<grid point indices for the next point independent of object direction*/,
   const Fbk_Vehicle_Data_T *p_vehicle_data);

#endif

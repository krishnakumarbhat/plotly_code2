#ifndef PT_RESET_H
#define PT_RESET_H
/**
 * @file pt_reset.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains declarations of reset functions for path tracking types.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_const_macros.h"
#include "pt_constants.h"
#include "pt_output_t.h"
#include "pt_persistent_t.h"
#include "pt_types.h"

/*===========================================================================*\
* Global Function Prototypess
\*===========================================================================*/

/**
 * @brief resets all paths and path output structs.
 *
 * @return void
 *
 * @SRS{SF-1521}
 * @SAE{SF-2918}
 * @SDD{SF-7547}
 * @verification{}
 */
void Pt_Reset_All_Paths(
   Pt_Path_T p_paths[PT_NUMBER_OF_PATHS] /**<all paths*/,
   Pt_Output_T *p_path_output /**< path output struct*/,
   Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS] /**< best path object pairs*/);

/**
 * @brief Ensures that all associations to a path are reset before
 * the path gets reset.
 *
 * @return void
 *
 * @SRS{SF-1572}
 * @SAE{SF-2918}
 * @SDD{SF-7548}
 * @verification{}
 */
void Pt_Reset_Path_And_Associations_To_It(
   Pt_Path_T *p_path /**<respective path*/,
   Pt_Best_Path_Obj_Pair_Persistent_T
      best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS] /**< Pairs which might need to be reset when associated to reset candidate*/,
   const Pt_Path_Reset_Reason_T path_reset_reason /**< Reason for the path reset (debug signal)*/);


/**
 * @brief resets a single path_output struct to its default values.
 *
 * @return void
 *
 * @SRS{SF-1521}
 * @SAE{SF-2918}
 * @SDD{SF-7549}
 * @verification{}
 */
void Pt_Reset_Single_Path_Output(Pt_Path_Object_Pair_Output_T *p_path_obj_pair_output /**< path output struct for match*/,
                                 Pt_Nearest_Path_T *p_nearest_path_output /**< path output struct for match*/);

/**
 * @brief resets all path_output structs to its default values.
 *
 * @return void
 *
 * @SRS{SF-1521}
 * @SAE{SF-2918}
 * @SDD{SF-7642}
 * @verification{}
 */
void Pt_Reset_Path_Output(Pt_Output_T *p_pt_output);


#endif

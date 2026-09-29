#ifndef PT_PERSISTENT_HANDLER_H
#define PT_PERSISTENT_HANDLER_H
/**
 * @file pt_persistent_handler.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains declarations for persistent data structure handling of path tracking.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pt_persistent_t.h"
#include "pt_types.h"

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

/**
 * @brief updates the persistent path Tracking struct.
 *
 * @return void
 *
 * @SRS{SF-1522,SF-1520,SF-1521}
 * @SAE{SF-2918}
 * @SDD{SF-7537}
 * @verification{}
 */
void Pt_Update_Persistent(Pt_Persistent_T *p_pt_persistent /**< persistent variable of Path Tracking algorithm*/);

/**
 * @brief This function resets the persistent path Tracking struct to its default values.
 *
 * @return void
 *
 * @SRS{SF-1521}
 * @SAE{SF-2918}
 * @SDD{SF-7535}
 * @verification{}
 */
void Pt_Reset_Persistent(Pt_Persistent_T *p_pt_persistent /**< persistent variable of Path Tracking algorithm*/);

/**
 * @brief resets all attributes of all path object pairs.
 *
 * @return void
 *
 * @SRS{SF-1521}
 * @SAE{SF-2918}
 * @SDD{SF-7534}
 * @verification{}
 */
void Pt_Reset_All_Matching_Pairs(
   Pt_Best_Path_Obj_Pair_Persistent_T matching_pairs[PT_OBJ_MAX_ARRAY_SIZE] /**< all path-object pairs*/);

/**
 * @brief resets all attributes of a selected path object pair.
 *
 * @return void
 *
 * @SRS{SF-1521}
 * @SAE{SF-2918}
 * @SDD{SF-7536}
 * @verification{}
 */
void Pt_Reset_Single_Matching_Pair(Pt_Best_Path_Obj_Pair_Persistent_T *p_matching_pair /**< path-object pair*/);

#endif

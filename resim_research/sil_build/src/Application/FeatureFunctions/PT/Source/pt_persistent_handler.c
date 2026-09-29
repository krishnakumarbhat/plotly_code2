/**
 * @file pt_persistent_handler.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function definitions handling of persistent data structures of path tracking.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pt_persistent_handler.h"
#include "fbk_macros.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"
#include "pt_output_t.h"
#include <assert.h>

/*===========================================================================*\
* Defines
\*===========================================================================*/

#define PT_HUGE_DISTANCE_TO_PATH (100.0f)

/*===========================================================================*\
* Global Functions Prototypes
\*===========================================================================*/

void Pt_Update_Persistent(Pt_Persistent_T *p_pt_persistent)
{
   /* Asserts */
   assert(NULL != p_pt_persistent);

   p_pt_persistent->f_was_pt_executed = FBK_TRUE;
}

void Pt_Reset_Persistent(Pt_Persistent_T *p_pt_persistent)
{
   uint8_t index;

   /* Assert */
   assert(NULL != p_pt_persistent);

   p_pt_persistent->f_was_pt_executed = FBK_FALSE;

   for (index = FBK_ZERO_UINT; index <= PA_OBJ_NUMBER_OF_OBJECTS; index++)
   {
      p_pt_persistent->path_index_last_cycle[index] = PT_DEFAULT_MATCH_INDEX;
   }
}

void Pt_Reset_All_Matching_Pairs(Pt_Best_Path_Obj_Pair_Persistent_T matching_pairs[PT_OBJ_MAX_ARRAY_SIZE])
{
   uint8_t index;

   /* Assert */
   assert(NULL != matching_pairs);

   for (index = FBK_ZERO_UINT; index <= PA_OBJ_NUMBER_OF_OBJECTS; index++)
   {
      Pt_Reset_Single_Matching_Pair(&(matching_pairs[index]));
   }
}

void Pt_Reset_Single_Matching_Pair(Pt_Best_Path_Obj_Pair_Persistent_T *p_matching_pair)
{
   /* Assert */
   assert(NULL != p_matching_pair);

   p_matching_pair->confidence_factor           = FBK_ZERO_F;
   p_matching_pair->relevant_dist_comp_matching = PT_HUGE_DISTANCE_TO_PATH;
   p_matching_pair->path_index                  = PT_DEFAULT_MATCH_INDEX;
}

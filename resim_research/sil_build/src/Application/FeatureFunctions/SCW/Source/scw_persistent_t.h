#ifndef SCW_PERSISTENT_T_H
#define SCW_PERSISTENT_T_H

/**
 * @file scw_persistent_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the persistent data of the SCW module.
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"
#include "scw_types.h"

/*============================================================================*\
* EXPORTED TYPEDEF DECLARATIONS
\*============================================================================*/

/* STRUCTS -------------------------------------------------------------------*/

/**
 * @brief Scw_Persistent_T structure
 *
 * Stores persistent data
 */
typedef struct
{
   boolean_T prev_feature_activated; /**< Flag indicating if the SCW feature was activated by ego speed in the previous cycle */
   Scw_Persist_Obj_Data_T dyn_obj_data[PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT]; /**< Processed dynamic object data */
   uint8_t count_in_zone_grail[PA_OBJ_NUMBER_OF_GUARDRAILS]; /**< Number of cycles a guardrail is in the SCW zone */
   float32_T core_grail_lat_position[PA_OBJ_NUMBER_OF_GUARDRAILS][SCW_LAT_POS_BUFFER_SIZE]; /**< Lateral position of the guardrail,
                                                                                          shift buffer used in core */
   float32_T grail_lat_position[PA_OBJ_NUMBER_OF_GUARDRAILS]; /**< Lateral position of the guardrail, used in pre_run */
   float32_T grail_confidence[PA_OBJ_NUMBER_OF_GUARDRAILS];   /**< Confidence of the guardrail, used in pre_run */
   uint8_t grail_freeze_counter[PA_OBJ_NUMBER_OF_GUARDRAILS]; /**< Down counter to control calibrated freeze period for the
                                                                 guardrail */

   Scw_Object_Type_T prev_obj_type[FBK_NUMBER_OF_SIDES]; /**< Object type of critical object in the last cycle */
   uint8_t prev_obj_index[FBK_NUMBER_OF_SIDES];          /**< Object index in the last cycle */
   uint8_t prev_obj_id[FBK_NUMBER_OF_SIDES];             /**< Object ID of critical object in the last cycle */
   uint32_t prev_obj_unique_id[FBK_NUMBER_OF_SIDES];     /**< Object Unique ID of critical object in the last cycle */

   boolean_T prev_was_below_min_lat_ttc[FBK_NUMBER_OF_SIDES]; /**< critical object's lat ttc was below min critical threshold in
                                                                 last cycle */
   boolean_T prev_was_below_max_lat_ttc[FBK_NUMBER_OF_SIDES]; /**< critical object's lat ttc was below max critical threshold in
                                                                 last cycle */
   boolean_T prev_was_below_min_lat_distance[FBK_NUMBER_OF_SIDES]; /**< critical object's lat distance was below min critical
                                                                      threshold in last cycle */
   boolean_T prev_was_below_max_lat_distance[FBK_NUMBER_OF_SIDES]; /**< critical object's lat distance was below max critical
                                                                      threshold in last cycle */

} Scw_Persistent_T;

#endif /* SCW_PERSISTENT_T_H */

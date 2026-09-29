#ifndef SCW_CORE_OUTPUT_T_H
#define SCW_CORE_OUTPUT_T_H

/**
 * @file scw_core_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the core output data structure for SCW.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "fbk_macros.h"
#include "scw_types.h"

/**
 * @brief Summarizes interesting object types for Scw
 */
typedef enum
{
   SCW_OBJECT_TYPE_NONE      = (0),
   SCW_OBJECT_TYPE_DYNAMIC   = (1),
   SCW_OBJECT_TYPE_GUARDRAIL = (2)
} Scw_Object_Type_T;

/**
 * @brief Summarizes Alert level of Scw
 */
typedef enum
{
   SCW_NO_ALERT      = (0),
   SCW_ALERT_LEVEL_1 = (1),
   SCW_ALERT_LEVEL_2 = (2)
} Scw_Alert_Level_T;

/**
 * @brief Scw_Core_Output_T structure
 */
typedef struct
{
   Scw_Object_Type_T obj_type[FBK_NUMBER_OF_SIDES];    /**< Type of critical objects dependening on their side. */
   Scw_Alert_Level_T alert_level[FBK_NUMBER_OF_SIDES]; /**< Alert level of critical objects dependening on their side */
   uint8_t obj_id[FBK_NUMBER_OF_SIDES];         /**< ID of critical object. This is the id of the object in the tracker output. */
   uint32_t obj_unique_id[FBK_NUMBER_OF_SIDES]; /**< Unique ID of critical object. This is the id of the object in the tracker
                                                   output. */
   uint8_t obj_index[FBK_NUMBER_OF_SIDES];      /**< The index of the critical object, per side. */
   float32_T obj_lateral_distance[FBK_NUMBER_OF_SIDES]; /**< [m] Lateral distance (metal to metal) between the host and critical
                                                           object, per side. */
   float32_T obj_lateral_velocity[FBK_NUMBER_OF_SIDES]; /**< [m/s] Lateral velocity of the dynamic critical object or Host velocity
                                                           to the critical guardrail */
   float32_T obj_lateral_acceleration[FBK_NUMBER_OF_SIDES]; /**< [m/s^2] Lateral acceleration of the dynamic critical object or
                                                               Host acceleration to the critical guardrail */
   float32_T obj_lateral_ttc[FBK_NUMBER_OF_SIDES];          /**< [s] Lateral TTC of critical object, per side. */
   float32_T obj_ttle[FBK_NUMBER_OF_SIDES];                 /**< [s] TTLE - Time to Lateral Exit - of critical object, per side. */
   float32_T obj_ttp[FBK_NUMBER_OF_SIDES];                  /**< [s] TTP - Time to Pass - of critical object, per side. */
} Scw_Core_Output_T;

#endif /* SCW_CORE_OUTPUT_T_H */

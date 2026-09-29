#ifndef LTB_TYPES_H
#define LTB_TYPES_H

/**
 * @file ltb_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the LTB specific data types.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_traj_predictor_t.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Zone defines */
#define LTB_NUMBER_OF_ZONE_POINTS (4u)

/**
 * @brief TTC (Time-To-Collision) value indicating an invalid or no TTC
 *
 * @SDD{CSCSA-53961}
 * @verification{}
 */
#define LTB_INVALID_TTC (100.0f)

/**
 * @brief TTB (Time-To-Brake) value indicating an invalid TTB
 *
 * @SDD{CSCSA-53958}
 * @verification{}
 */

#define LTB_INVALID_TTB (100.0f)
/**
 * @brief Value indicating an invalid distance calculation
 *
 * @SDD{CSCSA-53959}
 * @verification{}
 */
#define LTB_INVALID_DISTANCE (100.0f)


/*============================================================================*\
 * EXPORTED TYPEDEF DECLARATIONS
\*============================================================================*/

/* STRUCTS -------------------------------------------------------------------*/

/**
 * @brief Contains the LTB alert states
 *
 * @SDD{CSCSA-53957}
 */
typedef enum
{
   NO_ALERT             = (0), /**< No alert level */
   ALERT_ACTIVE_LEVEL_1 = (1), /**< Alert level 1 - Info Level */
   ALERT_ACTIVE_LEVEL_2 = (2), /**< Alert level 2 - Warning level - possible collision course */
   ALERT_ACTIVE_LEVEL_3 = (3)  /**< Alert level 3 - Warning level - collision imminent */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Ltb_Alert_State_T;

/**
 * @brief Ltb_Object_T structure
 *
 * Stores the LTB object information
 *
 * @SDD{CSCSA-53953}
 */
typedef struct
{
   /* Object trajectory information */
   Fbk_Trajectory_T trajectory; /**< object trajectory */

   /* Tracker-based additional info */
   boolean_T f_curvi_available; /**< flag indicating the availablity of curvi information */

   /* LTB specific flags*/
   boolean_T f_vehicle_state_relevant; /**< flag indicating that the vehicle state is relevant for this object */
   boolean_T f_obj_ltb_relevant;       /**< flag indicating that the object is relevant for LTB */
   boolean_T f_obj_in_zone;            /**< flag indicating that the object is inside the zone */


   /* Object criticality */
   Vector_2d_T waypoint_at_collision; /**< Coordinates of predicted collision waypoint */
   float32_T ttc;                     /**< [s] Time-To-Collision */
   float32_T distance_to_ego;         /**< [m] Object distance to VCS origin */
   Ltb_Alert_State_T alert_level;     /**< LTB alert level */
   uint8_t alert_side;                /**< LTB alert side */

   /* Brake related */
   float32_T decel_to_avoid_coll; /**< [m/s^2] Ego deceleration that would prevent a collision with this object */
   float32_T ttb;                 /**< [s] Time-To-Brake */

} Ltb_Object_Attributes_T;


/**
 * @brief LTB object
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53954}
 * @verification{}
 */
typedef struct
{
   Fbk_Object_Data_T tracker_data;     /**< Tracker object data */
   Ltb_Object_Attributes_T attributes; /**< LTB object attributes */
} Ltb_Object_T;

#endif

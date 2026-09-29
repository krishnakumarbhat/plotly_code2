#ifndef TA_TYPES_H
#define TA_TYPES_H

/**
 * @file ta_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the TA specific data types.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_traj_predictor_t.h"
#include "ml_angle_t.h"
#include "ml_vector_2d_t.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"
#include "ta_core_calibration_t.h"

/* EXPORTED DEFINES FOR CONSTANTS --------------------------------------------*/

/* Defines for accessing min and max values in arrays */
#define TA_MIN (0) /* WI-6211 */
#define TA_MAX (1) /* WI-6210 */

/*============================================================================*\
 * EXPORTED TYPEDEF DECLARATIONS
\*============================================================================*/

/* STRUCTS -------------------------------------------------------------------*/

/**
 * @brief Contains the TA alert states
 *
 * @SDD{SF-8758}
 */
typedef enum
{
   TA_ALERT_STATE_NONE    = (0), /**< No Alert */
   TA_ALERT_STATE_LEVEL_1 = (1), /**< Info Level */
   TA_ALERT_STATE_LEVEL_2 = (2), /**< Warning level - possible collision course */
   TA_ALERT_STATE_LEVEL_3 = (3), /**< Warning level - collision confirmed (acute driver warning) */
   TA_ALERT_STATE_LEVEL_4 = (4)  /**< Warning level - collision imminent  (initiate braking request) */
} Ta_Alert_State_T;


/**
 * @brief Determines if an alert was raised for FTA, RTA, or BOTH.
 * If set via cal value, any alert will raise BOTH.
 *
 * @SDD{}
 */
typedef enum
{
   TA_ALERT_MODE_NONE  = (0),
   TA_ALERT_MODE_FRONT = (1),
   TA_ALERT_MODE_REAR  = (2),
   TA_ALERT_MODE_BOTH  = (3)
} Ta_Alert_Mode_T;

/**
 * @brief Contains the different circle types
 *
 * @SDD{}
 */
typedef enum
{
   TA_CIRCLE_FRONT  = (0),
   TA_CIRCLE_MIDDLE = (1),
   TA_CIRCLE_REAR   = (2)
} Ta_Circle_Type_T;

/**
 * @brief Contains the TA algorithm states
 *
 * @SDD{}
 */
typedef enum
{
   TA_STATE_ALGORITHM_DISABLED       = (0),
   TA_STATE_NO_VALID_OBJECTS         = (1),
   TA_STATE_VEHICLE_STATE_INVALID    = (2),
   TA_STATE_NO_RELEVANT_OBJECTS      = (3),
   TA_STATE_NO_CRITICAL_OBJECTS      = (4),
   TA_STATE_CRITICAL_OBJECT_DETECTED = (5)
} Ta_Algorithm_State_T;

/**
 * @brief Ta_Object_Attributes_T structure
 *
 * Stores the TA object attributes
 *
 * @SDD{SF-8617}
 */
typedef struct
{
   /* Object trajectory information */
   Fbk_Trajectory_T trajectory; /**< object trajectory */

   /* Tracker-based additional info */
   float32_T area;                         /**< [m^2] object area */
   float32_T object_class_probability_vru; /**< [%] object class probability for vulnerable road user (pedestrian or 2wheel)*/
   float32_T velocity_heading;             /**< [rad] object velocity vector heading */
   boolean_T f_curvi_available;            /**< flag indicating the availablity of curvi information */

   /* TA specific flags*/
   boolean_T f_vehicle_state_relevant; /**< flag indicating that the vehicle state is relevant for this object */
   boolean_T f_obj_ta_relevant;        /**< flag indicating that the object is relevant for TA */
   boolean_T f_obj_in_danger_zone;     /**< flag indicating that the object is inside the danger zone */
   boolean_T f_obj_in_info_zone;       /**< flag indicating that the object is inside the info zone */
   boolean_T f_obj_in_wing_zone;       /**< flag indicating that the object is inside the wing zone */

   /* TA specific information */
   Ta_Alert_Mode_T ta_alert_mode; /**< indicates if previous cycle had active alert for FTA, RTA, or both */
   float32_T ego_heading_diff;    /**< [rad] difference between object heading and ego yaw angle to last straight */

   /* Object criticality */
   Vector_2d_T waypoint_at_collision; /**< Coordinates of predicted collision waypoint */
   float32_T ttc;                     /**< [s] Time-To-Collision */
   float32_T ttp;                     /**< [s] Time-To-Pass */
   float32_T distance_to_ego;         /**< [m] Object distance to VCS origin */
   Ta_Alert_State_T alert_level;      /**< TA alert level */
   uint8_t alert_side;                /**< TA alert side */

   /* Brake related */
   float32_T decel_to_avoid_coll; /**< [m/s^2] Ego deceleration that would prevent a collision with this object */
   float32_T ttb;                 /**< [s] Time-To-Brake */

} Ta_Object_Attributes_T;

/**
 * @brief Ta_Object_T structure
 *
 * Stores the TA object and tracker information
 *
 * @SDD{SF-8639}
 */
typedef struct
{
   Fbk_Object_Data_T tracker_data;    /* FBK Tracker info */
   Ta_Object_Attributes_T attributes; /* TA Object attributes */

} Ta_Object_T;

#endif /* TA_TYPES_H */

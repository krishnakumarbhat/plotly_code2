#ifndef CED_TYPES_H
#define CED_TYPES_H

/**
 * @file ced_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the CED specific data types.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"
#include "pt_output_t.h"

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Zone defines */
#define CED_NUMBER_OF_ZONE_POINTS (4u)

/* Invalid values */
#define CED_INVALID_TIME (100.0f)
#define CED_INVALID_DISTANCE (100.0f)
#define CED_ALERT_LEVELS (4u)

/* Number of maximum array size for all available objects */
#define CED_OBJ_MAX_ARRAY_SIZE ((uint8_t) PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT)

/*============================================================================*\
 * EXPORTED TYPEDEF DECLARATIONS
\*============================================================================*/

/* STRUCTS -------------------------------------------------------------------*/

/**
 * @brief Contains the CED alert states
 *
 * @SDD{SF-3649}
 */
typedef enum
{
   CED_NO_ALERT             = (0), /**< No alert level */
   CED_ALERT_ACTIVE_LEVEL_1 = (1), /**< Alert level 1 */
   CED_ALERT_ACTIVE_LEVEL_2 = (2), /**< Alert level 2 */
   CED_ALERT_ACTIVE_LEVEL_3 = (3), /**< Alert level 3 */
   CED_ALERT_QUALIFICATION  = (4)  /**< Internal status for alert qualification */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Ced_Alert_T;

/**
 * @brief Contains the CED intersection sides
 *
 * @SDD{SF-3650}
 */
typedef enum
{
   INTERSEC_UNDEF_SIDE = (0), /**< Undefined side */
   INTERSEC_BOTH_SIDES = (1), /**< Both sides */
   INTERSEC_LEFT_SIDE  = (2), /**< Left side */
   INTERSEC_RIGHT_SIDE = (3)  /**< Right side */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Ced_Intersection_Side_T;

/**
 * @brief Contains the CED ego sides
 *
 * @SDD{SF-3651}
 */
typedef enum
{
   UNDEF_SIDE     = (0), /**< Undefined side */
   EGO_LANE       = (1), /**< Ego lane */
   EGO_LEFT_SIDE  = (2), /**< Ego left side */
   EGO_RIGHT_SIDE = (3)  /**< Ego right side */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Ced_Object_Side_T;


/**
 * @brief Contains the CED states for opposite side alerts
 *
 * @SDD{SF-3652}
 */
typedef enum
{
   OPPOSITE_SIDE_ALERT_NOT_ALLOWED                  = (0), /**< Opposite side alert not allowed */
   OPPOSITE_SIDE_ALERT_ONLY_ALLOWED_WITH_PATH_MATCH = (1), /**< Opposite side alert onyl allowed with path match */
   OPPOSITE_SIDE_ALERT_ALLOWED                      = (2)  /**< Opposite side alert allowed */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Ced_Opposite_Side_Alert_T;

/**
 * @brief Contains the CED points of the zone check
 *
 * @SDD{SF-3653}
 */
typedef enum
{
   CED_POINT_FRONT_LEFT          = (0), /**< Front left corner of zone */
   CED_POINT_FRONT_RIGHT         = (1), /**< Front right corner of zone */
   CED_POINT_REAR_RIGHT          = (2), /**< Rear right corner of zone */
   CED_POINT_REAR_LEFT           = (3), /**< Rear left corner of zone */
   CED_POINT_MIDDLE              = (4), /**< Middle point of zone */
   CED_POINT_MIDDLE_LEFT         = (5), /**< Middle of left side of zone */
   CED_POINT_MIDDLE_RIGHT        = (6), /**< Middle of right side of zone */
   CED_NUMBER_OF_POINTS_TO_CHECK = (7)  /**< Number of points to check */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Ced_Points_To_Check_T;

/**
 * @brief Contains the CED alert suppression options
 *
 * @SDD{SF-3654}
 */
typedef enum
{
   CED_SUPPRESS_NO_ALERT                   = (0), /**< Object alert level should not be suppressed */
   CED_SUPPRESS_OPPOSITE_SIDE_ALERT        = (1), /**< Object on opposite side of alert side will be suppressed */
   CED_SUPPRESS_EGO_LANE_ALERT             = (2), /**< Object in ego lane alert will be suppressed */
   CED_SUPPRESS_COASTED_OBJECT_ALERT       = (3), /**< Coasted object alerts will be suppressed */
   CED_SUPPRESS_BASED_ON_NEAREST_PATH_INFO = (4), /**< New Object behaving differently from recorded trajectory will be suppressed*/
   CED_SUPPRESS_CROSS_BORDER               = (5)  /**< Object located laterally on two sides of a specific line */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Ced_Alert_Suppression_T;

/**
 * @brief Contains the CED funnel zone reference points
 *
 * @SDD{}
 */
typedef enum
{
   CED_DEFAULT_REFERENCE        = (0), /**< default reference point */
   CED_FRONT_BUMPER_REFERENCE   = (1), /**< nearest corner reference point */
   CED_NEAREST_CORNER_REFERENCE = (2)  /**< nearest corner reference point */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Ced_Reference_Point_T;

/**
 * @brief Ced_Object_T structure
 *
 * Stores the CED object information
 *
 * @SDD{CSCSA-87641}
 */
typedef struct
{
   /* Path tracking info */
   const Pt_Path_Object_Pair_Output_T *p_pt_match_info; /**<pt output which gives information about the matched path*/
   const Pt_Nearest_Path_T *p_pt_nearest_path_info;     /**<pt output which gives information about the nearest path*/

   /* Object status */
   Ced_Object_Side_T ego_side; /**< object ego side */

   /* Object rear bumper position (longitudinally) */
   float32_T front_bumper_pos_long; /**< [m] Object front bumper position (longitudinally) */
   float32_T rear_bumper_pos_long;  /**< [m] Object rear bumper position (longitudinally) */

   /* Position prediction at crash line */
   Vector_2d_T position_predicted;       /**< [m] predicted object position in VCS */
   float32_T heading_predicted;          /**< [RAD] predicted object heading */
   float32_T length_predicted;           /**< [m] predicted object length */
   float32_T width_predicted;            /**< [m] predicted object width */
   float32_T closest_lat_dist_predicted; /**< [m] Closest lateral distance of the predicted object */

   /* Distance and Time to crash line */
   float32_T distance_to_crash_line;  /**< [m] Object distance to crash line */
   float32_T time_to_crash_line;      /**< [s] Time to reach the crash line */
   float32_T time_to_pass_crash_line; /**< [s] Time to pass the crash line */

   /* Alert */
   Ced_Alert_T alert_level;                    /**< CED alert level */
   Ced_Intersection_Side_T alert_side;         /**< CED alert side */
   uint8_t direction;                          /**< CED alert direction */
   boolean_T f_skip_alert_holding;             /**< Flag indicating that no alert should be held for this object */
   float32_T zone_width_hys[CED_ALERT_LEVELS]; /**< Array containing hysteresis of individual alert levels, if they were triggered
                                                  in the previous cycle. For alert LVL_1 the collision zone width is extended, for
                                                  others closest_lat_dist_predicted */

} Ced_Object_Attributes_T;


/**
 * @brief CED object
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{CSCSA-87642}
 * @verification{}
 */
typedef struct
{
   Fbk_Object_Data_T tracker_data;     /**< Tracker object data */
   Ced_Object_Attributes_T attributes; /**< CED object attributes */
} Ced_Object_T;

#endif

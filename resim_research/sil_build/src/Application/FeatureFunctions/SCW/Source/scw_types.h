#ifndef SCW_TYPES_H
#define SCW_TYPES_H

/**
 * @file scw_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the SCW specific data types.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"

/*==========================================================================*\
* Defines
\*===========================================================================*/

#define SCW_OUTPUT_OBJ_ID_GUARDRAIL (0u)       /** Value used as object ID for guardrail objects */
#define SCW_BIG_VALUE (1000.0f)                /** Usefull max default value */
#define SCW_MIN_POSITION_X (-SCW_BIG_VALUE)    /** Max default value of object's longitudinal position */
#define SCW_MAX_LATERAL_DISTANCE SCW_BIG_VALUE /** Max default value of object's lateral distance */

/* Zone defines */
#define SCW_ZONE_FRONT_RIGHT (0u)      /** Index of front right zone point */
#define SCW_ZONE_MIDDLE_RIGHT (1u)     /** Index of middle right zone point */
#define SCW_ZONE_REAR_RIGHT (2u)       /** Index of rear right zone point */
#define SCW_ZONE_REAR_LEFT (3u)        /** Index of rear left zone point */
#define SCW_ZONE_MIDDLE_LEFT (4u)      /** Index of middle left zone point */
#define SCW_ZONE_FRONT_LEFT (5u)       /** Index of front left zone point */
#define SCW_NUMBER_OF_ZONE_POINTS (6u) /** Number of zone points */

/* Defines for accessing min and max values in arrays */
#define SCW_MIN (0) /* Index of minimum value of calibration ranges */
#define SCW_MAX (1) /* Index of maximum value of calibration ranges */

/* Defines for calculation of guardrail lateral velocity and acceleration */
#define SCW_VELOCITY (1u)
#define SCW_ACCELERATION (2u)
#define SCW_LAT_POS_BUFFER_SIZE SCW_ACCELERATION

#define Fbk_GE_F(a, b) (((a) > (b)) || Fbk_Equal_F((a), (b)))

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * Stores all object specific information from the tracker and vehicle required by SCW
 */
typedef struct
{
   uint8_t index;
   float32_T position_x;
   float32_T lateral_distance;
   float32_T lateral_velocity;
   float32_T lateral_acceleration;
   float32_T lateral_ttc;
   float32_T ttp;
   float32_T ttle;
} Scw_Critical_Object_T;


#define SCW_TTX_MODE_0 FBK_ZERO_UINT
#define SCW_TTX_MODE_1 FBK_ONE_UINT


typedef enum
{
   LAT_TTC_LEFT  = SCW_TTX_MODE_0,
   LAT_TTC_RIGHT = SCW_TTX_MODE_1,
   TTLE_LEFT     = SCW_TTX_MODE_1,
   TTLE_RIGHT    = SCW_TTX_MODE_0,
   TTP_MODE      = SCW_TTX_MODE_0
} Scw_TTx_Calculation_Mode_T;


typedef struct
{
   Fbk_Object_Data_T tracker_data;      /* Tracker data provided by PA */
   Scw_Critical_Object_T extended_data; /* Lateral distance to the Host and TTx parameters, lateral velocity and acceleration in
                                           case of guardrail */
   float32_T rearmost_corner_x;         /* x coordinate of the Target rearmost corner, used to calculate TTP */
   float32_T nearest_corner_y; /* y coordinate of the Target corner laterally nearest to the Host, used to calculate lateral
                                  distance and TTLE */
} Scw_Object_T;


typedef struct
{
   uint8_t mature_in_zone_count;    /**< Count of the number of cycles an object was in the SCW zone with status mature */
   boolean_T f_relevant_last_cycle; /**< Flag indicating whether the object was relevant in the previous cycle */
   boolean_T f_in_speed_range;      /**< Flag indicating whether the object was in speed range in the previous cycle */
   boolean_T f_in_relative_long_velocity_range; /**< Flag indicating whether the object was in relative longitudinal velocity range
                                                   in the previous cycle */
   boolean_T f_in_heading_range; /**< Flag indicating whether the object was in the heading range in the previous cycle */
   boolean_T f_in_yawrate_range; /**< Flag indicating whether the object was in the yawrate range in the previous cycle */
} Scw_Persist_Obj_Data_T;


typedef enum
{
   SCW_GUARDRAIL_INVALID = 0,
   SCW_GUARDRAIL_VALID   = 1
} SCW_Guardrail_status_T;


typedef struct
{
   float32_T lateral_position;
   float32_T confidence;
   SCW_Guardrail_status_T type;
} Scw_Guardrail_Information_T;


typedef struct
{
   Scw_Guardrail_Information_T radar;
   Scw_Guardrail_Information_T camera;
} Scw_Guardrail_Sources_T;


/**
 * @brief Scw_Trailer_Object_T summarizes object properties of a trailer attached to the host vehicle.
 *
 * @SDD{}
 */
typedef struct
{
   boolean_T f_present; /**< Flag indicating whether a trailer is attached to the host vehicle */
   float32_T length;    /**< [m] Length of the trailer (Back edge of the trailer will be host length plus trailer length when going
                        straight) */
   float32_T width;     /**< [m] Width of the trailer */
   float32_T angle;     /**< [rad] Angle of the trailer */

} Scw_Trailer_Object_T;


#endif /* SCW_TYPES*/

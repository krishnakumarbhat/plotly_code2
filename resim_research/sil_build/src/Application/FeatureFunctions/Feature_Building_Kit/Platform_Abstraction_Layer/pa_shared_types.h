#ifndef PA_SHARED_TYPES_H
#define PA_SHARED_TYPES_H

/**
 * @file pa_shared_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains shares types used in Platform abstraction.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*============================================================================*\
 * Defines
\*============================================================================*/

/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define PA_INVALID_OBJ_INDEX (255u)

/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define PA_INVALID_OBJ_ID (0u)

/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define PA_ENV_GUARDRAIL_SIDE_LEFT (0u)

/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define PA_ENV_GUARDRAIL_SIDE_RIGHT (1u)

/*============================================================================*\
 * EXPORTED TYPEDEF DECLARATIONS
\*============================================================================*/

/**
 * @brief Defines the state of a track unified for all trackers
 *
 * @SRS{SF-287}
 * @SAE{}
 * @SDD{}
 */
typedef enum
{
   PA_OBJ_STATUS_INVALID             = (0), /**< track not existing */
   PA_OBJ_STATUS_NEW                 = (1), /**< track created this cycle */
   PA_OBJ_STATUS_MATURE              = (2), /**< track was confirmed by succeeding measurements of detections */
   PA_OBJ_STATUS_COASTED             = (3), /**< track was MATURE, but is currently no longer supported by measured detections. */
   PA_OBJ_STATUS_COASTED_IMPLAUSIBLE = (4)  /**< track was MATURE, is no longer supported by detections but also not in sensor
                                                     field of view. */
} Pa_Obj_Status_T;

/**
 * @brief Defines the class of a track unified for all trackers
 *
 * @SRS{SF-287}
 * @SAE{}
 * @SDD{}
 */
typedef enum
{
   PA_OBJ_CLASS_UNKNOWN    = (0), /**< unknown object class */
   PA_OBJ_CLASS_PEDESTRIAN = (1), /**< pedestrians */
   PA_OBJ_CLASS_2WHEEL     = (2), /**< bicycles, motorbikes, etc */
   PA_OBJ_CLASS_CAR        = (3), /**< cars */
   PA_OBJ_CLASS_TRUCK      = (4)  /**< trucks */
} Pa_Obj_Class_T;


/**
 * @brief Defines the curvi coordinates method
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
typedef enum
{
   PA_OBJ_CURVI_COORDINATES_UNKNOWN                  = (0), /**< unknown */
   PA_OBJ_CURVI_COORDINATES_BASED_ON_VCS             = (1), /**< based on VCS */
   PA_OBJ_CURVI_COORDINATES_SNAIL_TRAIL              = (2), /**< based on snail trail */
   PA_OBJ_CURVI_COORDINATES_DISTANCE_BASED_CURVATURE = (3)  /**< distance based curvature */
} Pa_Obj_Curvi_Calc_Method_T;

/**
 * @brief Defines the vehicle PRNDL state
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
typedef enum
{
   PA_VEH_PRNDL_STATE_PARK    = (0), /**< park state */
   PA_VEH_PRNDL_STATE_REVERSE = (1), /**< reverse state */
   PA_VEH_PRNDL_STATE_NEUTRAL = (2), /**< neutral state */
   PA_VEH_PRNDL_STATE_DRIVE   = (3), /**< drive state */
   PA_VEH_PRNDL_STATE_LOW     = (4)  /**< low state */
} Pa_Veh_Prndl_State_T;


#endif /* PA_SHARED_TYPES_H */

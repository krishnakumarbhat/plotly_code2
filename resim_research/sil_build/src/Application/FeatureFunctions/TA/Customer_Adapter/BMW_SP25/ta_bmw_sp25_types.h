#ifndef TA_BMW_SP25_TYPES_H
#define TA_BMW_SP25_TYPES_H
/**
 * @file ta_bmw_sp25_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for the BMW SP2025 State Machine logic for TA.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */
/*===========================================================================
 * Includes
 *=========================================================================*/
#include "pa_reuse.h"
/* #define TA_BMW_BIT_POSITION_0(0x0u) < Critical object in the left TAP zone - Bitposition 0x00 0000 0000 0000*/
#define TA_BMW_BIT_POSITION_1                                                                         \
   (0x1u) /**< Critical object with info level in the left TAP zone - Bitposition 0x00 0000 0000 0001 \
           */
#define TA_BMW_BIT_POSITION_2 \
   (0x2u) /**< Critical object with acute level in the left TAP zone - Bitposition 0x00 0000 0000 0010 */
#define TA_BMW_BIT_POSITION_3 (0x4u) /**< No critical object in the left TAP zone - Bitposition 0x00 0000 0000 0100 */
/* #define TA_BMW_BIT_POSITION_4(0x8u) < Critical object in the right TAP zone - Bitposition 0x00 0000 0000 1000 */
/** #define TA_BMW_BIT_POSITION_5 \
   (0x16u)  Critical object with info level in the right TAP zone - Bitposition 0x00 0000 0001 0000 */
/** #define TA_BMW_BIT_POSITION_6 \
   (0x32u)  Critical object with acute level in the right TAP zone - Bitposition 0x00 0000 0010 0000 */
/** #define TA_BMW_BIT_POSITION_7 (0x64u)  No critical object in the right TAP zone - Bitposition 0x00 0000 0100 1000 */
/* #define TA_BMW_BIT_POSITION_8(0x128u)  TAP zone outside system boundaries - Bitposition 0x00 0000 1000 0000 */
/* #define TA_BMW_BIT_POSITION_9(0x256u) TAP zone within system boundaries - Bitposition 0x00 0001 0000 1000 */
#define TA_BMW_BIT_POSITION_10 (0x512u)  /**< Function reports error - Bitposition 0x00 0010 0000 1000 */
#define TA_BMW_BIT_POSITION_11 (0x1024u) /** Function interface is not available - Bitposition 0x00 0100 0000 1000 */
/*===========================================================================*\
* typedefs
\*===========================================================================*/
/**
 * @brief Contains the BMW SP25 State Outputs for TA
 */
typedef enum
{
   TA_STATE_NOT_AVAILABLE, /**< BMW TA state: Not Available */
   TA_STATE_AVAILABLE,     /**< BMW TA state: Available */
   TA_STATE_INACTIVE,      /**< BMW TA state: InActive*/
   TA_STATE_ACTIVE,        /**< BMW TA state: Active*/
   TA_STATE_ERROR          /**< BMW TA state: Error */
} TA_FF_State_T;
typedef enum
{
   TA_PARKENBN_NIO                            = (1u),
   TA_PARKENBN_IO                             = (2u),
   TA_STANDFUNKTIONEN_KUNDE_NICHT_IM_FAHRZEUG = (3u),
   TA_WOHNEN                                  = (5u),
   TA_PRUEFEN_ANALYSE_DIAGNOSE                = (7u),
   TA_FAHRBEREITSCHAFT_HERSTELLEN             = (8u),
   TA_FAHREN                                  = (10u),
   TA_FAHRBEREITSCHAFT_BEENDEN                = (12u)
} PWF_State_T;
typedef enum
{
   TA_VEHICLE_STANDSTILL     = (0u),
   TA_VEHICLE_MOVES_FORWARD  = (1u),
   TA_VEHICLE_MOVES_BACKWARD = (2u),
   TA_VEHICLE_IS_MOVING      = (3u),
   TA_FUNCTION_REPORTS_ERROR = (14u),
   TA_SIGNAL_UNFILLED        = (15u)
} TA_Vehicle_Driving_Direction_T;
typedef enum
{
   NO_DYNAMOMETER                             = (0u),
   FRONT_AXLE_ON_DYNAMOMETER                  = (1u),
   BACK_AXLE_ON_DYNAMOMETER                   = (2u),
   TWO_AXLE_ON_DYNAMOMETER                    = (3u),
   DYNAMOMETER_MODE_TA_FUNCTION_REPORTS_ERROR = (14u),
   DYNAMOMETER_MODE_TA_SIGNAL_UNFILLED        = (15u)
} DYNAMOMETER_MODE_T;
typedef enum
{
   TA_END_OF_LINE_MODE_NOT_SET                   = (0u),
   TA_END_OF_LINE_MODE_SET                       = (1u),
   TA_END_OF_LINE_MODE_TA_FUNCTION_REPORTS_ERROR = (14u),
   TA_END_OF_LINE_MODE_TA_SIGNAL_UNFILLED        = (15u)
} END_OF_LINE_MODE_T;
typedef enum
{
   NOT_CRITICAL = (0u),
   INFO_LEVEL   = (1u),
   ACUTE_LEVEL  = (2u)
} TA_Object_Criticality_Level_T;
typedef enum
{
   OBJECT_TYPE_UNKNOWN    = (0u),
   OBJECT_TYPE_PEDESTRIAN = (1u),
   OBJECT_TYPE_2_WHEEL    = (2u),
   OBJECT_TYPE_CAR        = (3u),
   OBJECT_TYPE_TRUCK      = (4u)
} TA_Object_Type_T;
typedef struct
{
   boolean_T c_f_ta_enabled;
   boolean_T c_f_dynamic_ta_enabled;
   boolean_T c_f_static_ta_enabled;
   boolean_T c_f_cross_ta_enabled;
   float32_T c_ta_min_vel_lower_limit;
   float32_T c_ta_max_vel_lower_limit;
   float32_T c_ta_min_vel_upper_limit;
   float32_T c_ta_max_vel_upper_limit;
} Ta_Coding_Parameters_T;
typedef struct
{
   PWF_State_T pwf_state;
   TA_Vehicle_Driving_Direction_T vehicle_driving_direction;
   boolean_T ta_function_error;
   DYNAMOMETER_MODE_T status_dynamometer_mode;
   END_OF_LINE_MODE_T status_end_of_line_mode;
} Bmw_TA_Input_Bus_Signals_T;
typedef struct
{
   TA_FF_State_T bmw_qualifier_ta_function_state;
   uint16_t f_ta_status;
   boolean_T ta_error_status;
   TA_Object_Criticality_Level_T ta_object_criticality_left_output;
   float32_T timestamp_left_object; /*confirm about type of it*/
   uint8_t left_object_id;
   /*Object Type needs to be here but no info is available either in requirement(Polarion) or from Core.
   Currently creating dummy.mocked data*/
   TA_Object_Type_T left_object_type;
   float32_T left_object_position_x;               /*long position of Left Object*/
   float32_T left_object_position_y;               /*lat position Left Object*/
   float32_T left_object_width;                    /*width of left object*/
   float32_T left_object_length;                   /*length of left object*/
   float32_T left_object_velocity_x;               /*long velocity of Left Object*/
   float32_T left_object_velocity_y;               /*lat velocity Left Object*/
   float32_T left_object_ttc;                      /*TTC of Left Object*/
   float32_T left_object_ttb;                      /*TTB of Left Object*/
   float32_T left_object_ttp;                      /*TTP of Left Object*/
   float32_T left_object_existence_probability;    /*Left Object Existence Probability*/
   float32_T left_object_probability_of_collision; /*Left Object Probability of Collision*/
   TA_Object_Criticality_Level_T ta_object_criticality_right_output;
   float32_T timestamp_right_object; /*confirm about type of it*/
   uint8_t right_object_id;
   /*Object Type needs to be here but no info is available either in requirement(Polarion) or from Core.
   Currently creating dummy.mocked data*/
   TA_Object_Type_T right_object_type;
   float32_T right_object_position_x;               /*long position of Right Object*/
   float32_T right_object_position_y;               /*lat position of Right Object*/
   float32_T right_object_width;                    /*width of right object*/
   float32_T right_object_length;                   /*length of right object*/
   float32_T right_object_velocity_x;               /*long velocity of Right Object*/
   float32_T right_object_velocity_y;               /*lat velocity of Right Object*/
   float32_T right_object_ttc;                      /*TTC of Right Object*/
   float32_T right_object_ttb;                      /*TTB of Right Object*/
   float32_T right_object_ttp;                      /*TTP of Right Object*/
   float32_T right_object_existence_probability;    /*Right Object Existence Probability*/
   float32_T right_object_probability_of_collision; /*Right Object Probability of Collision*/
} Bmw_TA_Output_Bus_Signals_T;
#endif /* TA_BMW_SP25_TYPES_H */

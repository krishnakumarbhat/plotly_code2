#ifndef LCDA_BMW_SP25_TYPES_H
#define LCDA_BMW_SP25_TYPES_H

#include "pa_reuse.h"
/**
 * @file lcda_bmw_sp25_types.h
 * @author SFL (Side Feature Logic) scrum team
 *  @brief This file contains the BMW SP2025 specific types for LCDA.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Local Defines
\*===========================================================================*/

#define LCDA_BMW_DEFAULT_BSW_ADJUSTMENT_FACTOR (1.0f)
#define LCDA_GUARDRAIL_LOW_EXIST_PROB (0.7f)
#define LCDA_GUARDRAIL_HIGH_EXIST_PROB (0.9f)
#define LCDA_CONVERT_FROM_PERCENTAGE(val) (0.01f * (val)) /* Conversion from percentage to SI*/

#define LCDA_MOCKED_TIMESTAMP_VALUE (0.0f)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Contains the BMW SP25 State Outputs
 */
typedef enum
{
   LCDA_STATE_NOT_AVAILABLE, /**< BMW LCDA state: Not Available */
   LCDA_STATE_AVAILABLE,     /**< BMW LCDA state: Not Available */
   LCDA_STATE_INACTIVE,      /**< BMW LCDA state: InActive*/
   LCDA_STATE_ACTIVE,        /**< BMW LCDA state: Active*/
   LCDA_STATE_ERROR          /**< BMW LCDA state: Error */
} LCDA_FF_State_T;

typedef enum
{
   PARKENBN_NIO                            = (1u),
   PARKENBN_IO                             = (2u),
   STANDFUNKTIONEN_KUNDE_NICHT_IM_FAHRZEUG = (3u),
   WOHNEN                                  = (5u),
   PRUEFEN_ANALYSE_DIAGNOSE                = (7u),
   FAHRBEREITSCHAFT_HERSTELLEN             = (8u),
   FAHREN                                  = (10u),
   FAHRBEREITSCHAFT_BEENDEN                = (12u)
} Vehicle_Condition_T;

typedef enum
{
   VEHICLE_STANDSTILL                             = (0u),
   VEHICLE_MOVES_FORWARD                          = (1u),
   VEHICLE_MOVES_BACKWARD                         = (2u),
   VEHICLE_IS_MOVING                              = (3u),
   ERROR                                          = (14u),
   LCDA_VEHICLE_DRIVING_DIRECTION_SIGNAL_UNFILLED = (15u)
} Vehicle_Driving_Direction_T;

typedef enum
{
   LEVEL0 = (0u),
   LEVEL1 = (1u),
   LEVEL2 = (2u)
} Function_LCDA_Error_T;

/*Describes the current functional state of LCDA*/
typedef enum
{
   EVENT_DATA_AVAILABLE          = (0u),
   EVENT_DATA_AVAILABLE_REDUCED  = (1u),
   EVENT_DATA_NOT_AVAILABLE      = (2u),
   EVENT_DATA_INVALID_OR_TIMEOUT = (3u)
} LCDA_EVENT_DATA_QUALIFIER_T;

/*Describes some more detailed information on the current functional state of LCDA*/
typedef enum
{
   NORMAL_OPERATION_MODE                            = (0u),
   POWER_UP_OR_DOWN                                 = (1u),
   SENSOR_NOT_CALIBRATED                            = (2u),
   SENSOR_BLOCKED                                   = (3u),
   SENSOR_MISALIGNED                                = (4u),
   BAD_SENSOR_ENVIRONMENTAL_CONDITION               = (5u),
   REDUCED_FIELD_OF_VIEW                            = (6u),
   INPUT_NOT_AVAILABLE                              = (7u),
   INTERNAL_REASON                                  = (8u),
   EXTERNAL_DESTORTION                              = (9u),
   BEGINNING_BLOCKAGE                               = (10u),
   SELF_TEST                                        = (11u),
   STAND_BY                                         = (12u),
   EXTERNAL_FAILURE                                 = (13u),
   EXTENDED_QUALIFIER_EVENT_DATA_INVALID_OR_TIMEOUT = (255u)
} LCDA_PRIVATE_EXTENDED_QUALIFIER_T;

typedef enum
{
   NO_VEHICLE_IN_THE_RIGHT_AND_LEFT_BSW_ZONE = (0u),
   VEHICLE_IN_THE_LEFT_BSW_ZONE              = (1u),
   VEHICLE_IN_THE_RIGHT_BSW_ZONE             = (2u),
   VEHICLE_IN_THE_RIGHT_AND_LEFT_BSW_ZONE    = (3u),
   BSW_ZONE_OUTSIDE_SYSTEM_BOUNDARIES        = (4u),
   BSW_FUNCTION_INTERFACE_IS_NOT_AVAILABLE   = (5u),
   BSW_FUNCTION_REPORTS_ERROR                = (6u)
} LCDA_OBJECT_STATUS_BSW_T;

typedef enum
{
   NO_VEHICLE_IN_THE_RIGHT_AND_LEFT_CVW_ZONE = (0u),
   VEHICLE_IN_THE_LEFT_CVW_ZONE              = (1u),
   VEHICLE_IN_THE_RIGHT_CVW_ZONE             = (2u),
   VEHICLE_IN_THE_RIGHT_AND_LEFT_CVW_ZONE    = (3u),
   CVW_ZONE_OUTSIDE_SYSTEM_BOUNDARIES        = (4u),
   CVW_FUNCTION_INTERFACE_IS_NOT_AVAILABLE   = (5u),
   CVW_FUNCTION_REPORTS_ERROR                = (6u)
} LCDA_OBJECT_STATUS_CVW_T;

typedef enum
{
   NO_VEHICLE_IN_THE_RIGHT_AND_LEFT_SLC_ZONE = (0u),
   VEHICLE_IN_THE_LEFT_SLC_ZONE              = (1u),
   VEHICLE_IN_THE_RIGHT_SLC_ZONE             = (2u),
   VEHICLE_IN_THE_RIGHT_AND_LEFT_SLC_ZONE    = (3u),
   SLC_ZONE_OUTSIDE_SYSTEM_BOUNDARIES        = (4u),
   SLC_FUNCTION_INTERFACE_IS_NOT_AVAILABLE   = (5u),
   SLC_FUNCTION_REPORTS_ERROR                = (6u)
} LCDA_OBJECT_STATUS_SLC_T;

typedef struct
{
   float32_T hour;
   float32_T minute;
   float32_T second;
} LCDA_OBJECT_TIMESTAMP_T;

typedef struct
{
   Vehicle_Condition_T vehicle_condition;
   Vehicle_Driving_Direction_T vehicle_driving_direction;
   float32_T curve_radii;
   Function_LCDA_Error_T lcda_function_error;
} Bmw_LCDA_Input_Bus_Signals_T;


typedef struct
{
   LCDA_FF_State_T bmw_qualifier_lcda_function_state;
   LCDA_EVENT_DATA_QUALIFIER_T bmw_lcda_event_data_qualifier;
   LCDA_PRIVATE_EXTENDED_QUALIFIER_T bmw_lcda_extended_qualifier;
   LCDA_OBJECT_STATUS_BSW_T bmw_lcda_object_status_bsw;
   LCDA_OBJECT_STATUS_CVW_T bmw_lcda_object_status_cvw;
   LCDA_OBJECT_STATUS_SLC_T bmw_lcda_object_status_slc;
   uint8_t bmw_lcda_heartbeat;
   /*BSW Object Properties*/
   LCDA_OBJECT_TIMESTAMP_T bmw_lcda_object_timestamp_left;
   uint8_t bmw_lcda_object_id_left;
   float32_T bmw_lcda_object_position_x_left;
   float32_T bmw_lcda_object_position_y_left;
   float32_T bmw_lcda_object_width_left;
   float32_T bmw_lcda_object_length_left;
   float32_T bmw_lcda_object_ttc_left;
   float32_T bmw_lcda_object_ttp_left;
   float32_T bmw_lcda_object_ttle_left;
   float32_T bmw_lcda_object_velocity_x_left;
   float32_T bmw_lcda_object_velocity_y_left;
   uint16_t bmw_lcda_object_existance_probability_left;
   uint8_t bmw_lcda_object_lane_change_probability_left;

   LCDA_OBJECT_TIMESTAMP_T bmw_lcda_object_timestamp_right;
   uint8_t bmw_lcda_object_id_right;
   float32_T bmw_lcda_object_position_x_right;
   float32_T bmw_lcda_object_position_y_right;
   float32_T bmw_lcda_object_width_right;
   float32_T bmw_lcda_object_length_right;
   float32_T bmw_lcda_object_ttc_right;
   float32_T bmw_lcda_object_ttp_right;
   float32_T bmw_lcda_object_ttle_right;
   float32_T bmw_lcda_object_velocity_x_right;
   float32_T bmw_lcda_object_velocity_y_right;
   uint16_t bmw_lcda_object_existance_probability_right;
   uint8_t bmw_lcda_object_lane_change_probability_right;
   /*CVW Object Properties*/
   LCDA_OBJECT_TIMESTAMP_T bmw_lcda_object_timestamp_left_cvw;
   uint8_t bmw_lcda_object_id_left_cvw;
   float32_T bmw_lcda_object_position_x_left_cvw;
   float32_T bmw_lcda_object_position_y_left_cvw;
   float32_T bmw_lcda_object_width_left_cvw;
   float32_T bmw_lcda_object_length_left_cvw;
   float32_T bmw_lcda_object_ttc_left_cvw;
   float32_T bmw_lcda_object_ttp_left_cvw;
   float32_T bmw_lcda_object_ttle_left_cvw;
   float32_T bmw_lcda_object_velocity_x_left_cvw;
   float32_T bmw_lcda_object_velocity_y_left_cvw;
   uint16_t bmw_lcda_object_existance_probability_left_cvw;
   uint8_t bmw_lcda_object_lane_change_probability_left_cvw;

   LCDA_OBJECT_TIMESTAMP_T bmw_lcda_object_timestamp_right_cvw;
   uint8_t bmw_lcda_object_id_right_cvw;
   float32_T bmw_lcda_object_position_x_right_cvw;
   float32_T bmw_lcda_object_position_y_right_cvw;
   float32_T bmw_lcda_object_width_right_cvw;
   float32_T bmw_lcda_object_length_right_cvw;
   float32_T bmw_lcda_object_ttc_right_cvw;
   float32_T bmw_lcda_object_ttp_right_cvw;
   float32_T bmw_lcda_object_ttle_right_cvw;
   float32_T bmw_lcda_object_velocity_x_right_cvw;
   float32_T bmw_lcda_object_velocity_y_right_cvw;
   uint16_t bmw_lcda_object_existance_probability_right_cvw;
   uint8_t bmw_lcda_object_lane_change_probability_right_cvw;
   /*SLC Object Properties*/
   LCDA_OBJECT_TIMESTAMP_T bmw_lcda_object_timestamp_left_slc;
   uint8_t bmw_lcda_object_id_left_slc;
   float32_T bmw_lcda_object_position_x_left_slc;
   float32_T bmw_lcda_object_position_y_left_slc;
   float32_T bmw_lcda_object_width_left_slc;
   float32_T bmw_lcda_object_length_left_slc;
   float32_T bmw_lcda_object_ttc_left_slc;
   float32_T bmw_lcda_object_ttp_left_slc;
   float32_T bmw_lcda_object_ttle_left_slc;
   float32_T bmw_lcda_object_velocity_x_left_slc;
   float32_T bmw_lcda_object_velocity_y_left_slc;
   uint16_t bmw_lcda_object_existance_probability_left_slc;
   uint8_t bmw_lcda_object_lane_change_probability_left_slc;

   LCDA_OBJECT_TIMESTAMP_T bmw_lcda_object_timestamp_right_slc;
   uint8_t bmw_lcda_object_id_right_slc;
   float32_T bmw_lcda_object_position_x_right_slc;
   float32_T bmw_lcda_object_position_y_right_slc;
   float32_T bmw_lcda_object_width_right_slc;
   float32_T bmw_lcda_object_length_right_slc;
   float32_T bmw_lcda_object_ttc_right_slc;
   float32_T bmw_lcda_object_ttp_right_slc;
   float32_T bmw_lcda_object_ttle_right_slc;
   float32_T bmw_lcda_object_velocity_x_right_slc;
   float32_T bmw_lcda_object_velocity_y_right_slc;
   uint16_t bmw_lcda_object_existance_probability_right_slc;
   uint8_t bmw_lcda_object_lane_change_probability_right_slc;

} Bmw_LCDA_Output_Bus_Signals_T;


typedef struct
{
   boolean_T c_f_lcda_enabled;
   float32_T c_lcda_min_vel_lower_limit;
   float32_T c_lcda_max_vel_upper_limit;
   float32_T c_min_curve_radii;
   uint8_t c_f_lcda_enable_bsw; /* LCDA BSW subfunction */
   uint8_t c_f_lcda_enable_cvw; /* LCDA CVW subfunction */
   uint8_t c_f_lcda_enable_slc; /* LCDA SLC subfunction */
   uint8_t c_f_lcda_enable_awa; /* LCDA AWA subfunction */
} Lcda_Coding_Parameters_T;


#endif /* LCDA_BMW_SP25_TYPES_H */

#ifndef CED_BMW_SP25_TYPES_H
#define CED_BMW_SP25_TYPES_H

#include "pa_reuse.h"
/**
 * @file ced_bmw_sp25_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the BMW SRR5 specific types for CED.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Defines
\*===========================================================================*/

#define CED_BMW_NUMBER_OF_SEATS (9u)
#define CED_BMW_NUMBER_OF_DOORS (4u)
#define CED_BMW_NUMBER_OF_SIDES (2u)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef enum
{
   CED_DOOR_POSITION_FRONT_LEFT  = (0u),
   CED_DOOR_POSITION_FRONT_RIGHT = (1u),
   CED_DOOR_POSITION_REAR_LEFT   = (2u),
   CED_DOOR_POSITION_REAR_RIGHT  = (3u),
   CED_DOOR_POSITION_UNKNOWN     = (4u)
} Ced_Door_Position_T;

typedef enum
{
   CED_SIDE_LEFT  = (0u),
   CED_SIDE_RIGHT = (1u)
} Ced_Sides_T;


/**
 * @brief Contains the CED target directions
 *
 * @SDD{n/a}
 */
typedef enum
{
   UNDEF_DIRECTION = (0), /**< Undefined direction */
   REAR_DIRECTION  = (1), /**< Rear direction */
   FRONT_DIRECTION = (2)  /**< Front direction */
} Ced_Target_Travel_Direction_T;


/**
 * @brief C_Sfe_Mirror_Light_Warning_Type_T enumerator.
 *
 *
 */
typedef enum
{
   CED_MIRROR_LIGHT_WARNING_OFF            = (0u),  /**< =Anzeigesegment_AUS */
   CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING = (1u),  /**< =Anzeigesegment_AN_kein_Blinken */
   CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1  = (2u),  /**< =Anzeigesegment_AN_Blinken_Stufe_1 */
   CED_MIRROR_LIGHT_WARNING_ON_FLASHING_2  = (3u),  /**< =Anzeigesegment_AN_Blinken_Stufe_2 */
   CED_MIRROR_LIGHT_WARNING_ON_FLASHING_3  = (4u),  /**< =Anzeigesegment_AN_Blinken_Stufe_3 */
   CED_MIRROR_LIGHT_WARNING_RESERVED_1     = (5u),  /**< Reserved */
   CED_MIRROR_LIGHT_WARNING_RESERVED_2     = (6u),  /**< Reserved */
   CED_MIRROR_LIGHT_WARNING_RESERVED_3     = (7u),  /**< Reserved */
   CED_MIRROR_LIGHT_WARNING_RESERVED_4     = (8u),  /**< Reserved */
   CED_MIRROR_LIGHT_WARNING_RESERVED_5     = (9u),  /**< Reserved */
   CED_MIRROR_LIGHT_WARNING_RESERVED_6     = (10u), /**< Reserved */
   CED_MIRROR_LIGHT_WARNING_RESERVED_7     = (11u), /**< Reserved */
   CED_MIRROR_LIGHT_WARNING_RESERVED_8     = (12u), /**< Reserved */
   CED_MIRROR_LIGHT_WARNING_NOT_AVAILABLE  = (13u), /**< =Funktionsschnittstelle_ist_nicht_verfuegbar */
   CED_MIRROR_LIGHT_WARNING_ERROR          = (14u), /**< =Funktion_meldet_Fehle */
   CED_MIRROR_LIGHT_WARNING_NOT_FILLED     = (15u)  /**< =Signal_unbefuellt */
} Ced_Mirror_Light_Warning_Type_T;

/**
 * @brief C_Sfe_Interior_Light_Type_T enumerator.
 *
 *
 */
typedef enum
{
   CED_DOOR_WARNING_LEVEL_NO_WARNING = (0u), /**< Not filled */
   CED_DOOR_WARNING_LEVEL_1          = (1u), /**< Interior Light Type 1 shall be set */
   CED_DOOR_WARNING_LEVEL_2          = (2u)  /**< Interior Light Type 2 shall be set */
} Ced_Door_Warning_Levels_T;

/*Indicates if a optical warning shall be raised for an vehicle approachingand the direction(front / rear) from were it approaches
 * from.*/
typedef enum
{
   NOT_ACTIVE            = (0u),
   ACTIVE_APPROACH_REAR  = (1u),
   ACTIVE_APPROACH_FRONT = (2u)
} CED_OUTPUT_WARNING_OPTICAL_T;

/*indicates whether the ambiend light shall be switched on and with which mode caused by a safe exit warning.*/
typedef enum
{
   AMBIENT_LIGHTS_NO_WARNING      = (0u),
   AMBIENT_LIGHTS_WARNING_LEVEL_1 = (1u),
   AMBIENT_LIGHTS_WARNING_LEVEL_2 = (2u)
} CED_OUTPUT_WARNING_AMBIENT_LIGHTS_T;


/* Needs to be updated
Indicates whether the acoustic warning shall be raised caused by a safe exit warning and from were the vehicle approaches.*/
typedef enum
{
   ACOUSTIC_NO_WARNING = (0u),
   ACOUSTIC_WARNING    = (1u)
} CED_OUTPUT_WARNING_ACOUSTIC_T;

/*Confirm it*/
typedef enum
{
   DISPLAY_DOOR_NO_WARNING = (0u),
   DISPLAY_DOOR_WARNING    = (1u)
} CED_OUTPUT_DISPLAY_DOOR_T;

typedef enum
{
   BMW_CED_NO_DYNAMOMETER                     = (0U),  /**< BMW Dynamometer status : No dynamometer */
   BMW_CED_FRONT_AXLE_ON_DYNAMOMETER          = (1U),  /**< BMW Dynamometer status : Front Axle dynamometer */
   BMW_CED_BACK_AXLE_ON_DYNAMOMETER           = (2U),  /**< BMW Dynamometer status : Rear Axle dynamometer */
   BMW_CED_TWO_AXLE_DYNAMOMETER               = (3U),  /**< BMW Dynamometer status : Two Axle dynamometer */
   BMW_CED_FUNCTION_DYNAMOMETER_REPORTS_ERROR = (14U), /**< BMW Dynamometer status : Dynamometer Error */
   BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED = (15U)  /**< BMW Dynamometer status : Signal Unfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ced_Status_Roller_Dynamometer_T;

typedef enum
{
   BMW_CED_END_OF_LINE_MODE_NOT_SET           = (0U),  /**< BMW : Vehicle on End of Line Roller */
   BMW_CED_END_OF_LINE_MODE_SET               = (1U),  /**< BMW : Vehicle not on End of Line Roller */
   BMW_CED_FUNCTION_END_OF_LINE_REPORTS_ERROR = (14U), /**< BMW : Signal Error */
   BMW_CED_END_OF_LINE_SIGNAL_UNFILLED        = (15U)  /**< BMW : Signal Unfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ced_Status_End_Of_Line_T;

/*============================================================================*\
 * ENUM values for CED state machine states
\*============================================================================*/
typedef enum
{
   CED_STATE_NOTAVAILABLE = 0,
   CED_STATE_READY        = 1,
   CED_STATE_ACTIVE       = 2,
   CED_STATE_DEGRADED     = 3,
   CED_STATE_ERROR        = 4
} CED_FF_STATE_T;

typedef struct
{
   boolean_T f_ced_function_fault_state; /**< Feature Function Fault State */
   boolean_T f_ced_function_activation;  /**< Feature Function Activation */
   boolean_T f_ced_function_degraded;    /**< Feature Function degraded */

   boolean_T f_ced_parental_controls_driver_rear;
   boolean_T f_ced_parental_controls_passenger_rear;

   boolean_T f_ced_vehicle_door_lock_rear_left;
   boolean_T f_ced_vehicle_door_lock_rear_right;

   boolean_T f_ced_vehicle_door_lock_request_rear_left;
   boolean_T f_ced_vehicle_door_lock_request_rear_right;

   Bmw_Ced_Status_Roller_Dynamometer_T ced_status_roller_dynamometer; /**< Provides information about the roller dynamometer test
                                                                      bench operating mode*/
   Bmw_Ced_Status_End_Of_Line_T ced_status_end_of_line; /**< provides information about whether the vehicle is on the end-of-line
                                                        roller following assembly*/
} Bmw_Ced_Input_Bus_Signals_T;


typedef struct
{
   CED_FF_STATE_T qualifier_ced_function_state; /**< Contains the RUN/Functional-State of CED Statemachine */

   CED_OUTPUT_WARNING_OPTICAL_T ced_output_warning_optical_front_right;
   CED_OUTPUT_WARNING_OPTICAL_T ced_output_warning_optical_front_left;
   CED_OUTPUT_WARNING_OPTICAL_T ced_output_warning_optical_rear_right;
   CED_OUTPUT_WARNING_OPTICAL_T ced_output_warning_optical_rear_left; /**< Indicates if a optical warning shall be raised for an
                                                                         vehicle approaching and the direction (front/rear) from
                                                                         were it approaches from.*/

   Ced_Mirror_Light_Warning_Type_T ced_output_warning_mirror_led_right; /**< Indicates if the right mirror LED shall be switched on
                                                                           and which mode (flashing or permanent light) it should
                                                                           be raised with.*/
   Ced_Mirror_Light_Warning_Type_T ced_output_warning_mirror_led_left;  /**< Indicates if the left mirror LED shall be switched on
                                                                           and which mode (flashing or permanent light) it should be
                                                                           raised with.*/

   CED_OUTPUT_WARNING_AMBIENT_LIGHTS_T ced_output_warning_ambient_lights_front_right; /**< indicates whether the ambiend light
                                                                                         shall be switched on and with which mode
                                                                                         caused by a safe exit warning.*/
   CED_OUTPUT_WARNING_AMBIENT_LIGHTS_T ced_output_warning_ambient_lights_front_left; /**< indicates whether the ambiend light shall
                                                                                        be switched on and with which mode caused
                                                                                        by a safe exit warning.*/
   CED_OUTPUT_WARNING_AMBIENT_LIGHTS_T ced_output_warning_ambient_lights_rear_right; /**< indicates whether the ambiend light shall
                                                                                        be switched on and with which mode caused
                                                                                        by a safe exit warning.*/
   CED_OUTPUT_WARNING_AMBIENT_LIGHTS_T ced_output_warning_ambient_lights_rear_left;  /**< indicates whether the ambiend light shall
                                                                                        be switched on and with which mode caused by
                                                                                        a safe exit warning.*/

   CED_OUTPUT_WARNING_ACOUSTIC_T ced_output_warning_acoustic_front_right; /**< Indicates whether the acoustic warning shall be
                                                                             raised caused by a safe exit warning and from were the
                                                                             vehicle appraches. */
   CED_OUTPUT_WARNING_ACOUSTIC_T ced_output_warning_acoustic_front_left;  /**< Indicates whether the acoustic warning shall be
                                                                             raised caused by a safe exit warning and from were the
                                                                             vehicle appraches. */
   CED_OUTPUT_WARNING_ACOUSTIC_T ced_output_warning_acoustic_rear_right;  /**< Indicates whether the acoustic warning shall be
                                                                             raised caused by a safe exit warning and from were the
                                                                             vehicle appraches. */
   CED_OUTPUT_WARNING_ACOUSTIC_T ced_output_warning_acoustic_rear_left; /**< Indicates whether the acoustic warning shall be raised
                                                                           caused by a safe exit warning and from were the vehicle
                                                                           appraches. */


   boolean_T ced_output_door_stop_automatic_opening_right;
   boolean_T ced_output_door_stop_automatic_opening_left;

   boolean_T ced_output_door_lock_electronic_front_right;
   boolean_T ced_output_door_lock_electronic_rear_right;
   boolean_T ced_output_door_lock_electronic_front_left;
   boolean_T ced_output_door_lock_electronic_rear_left;

   /*Confirm it*/
   CED_OUTPUT_DISPLAY_DOOR_T ced_output_display_door;
} Bmw_Ced_Output_Bus_Signals_T;

#endif /* CED_BMW_SP25_TYPES_H */

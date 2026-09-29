#ifndef LTB_BMW_SP25_TYPES_H
#define LTB_BMW_SP25_TYPES_H
/**
 * @file ltb_bmw_sp25_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the BMW_SP25 specific input data structure for LTB.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */
/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "ltb_types.h"
#include "pa_reuse.h"
/*===========================================================================*\
* typedefs
\*===========================================================================*/
typedef enum
{
   LTB_STATE_NOT_AVAILABLE = (0U),
   LTB_STATE_READY         = (1U),
   LTB_STATE_ACTIVE        = (2U),
   LTB_STATE_ERROR         = (3U)
} Ltb_State_T;
/**
 * @brief Contains the BMW input states for the Vehicle_Moving_Direction signal.
 */
typedef enum
{
   BMW_CTB_VEH_MOVING_DIR_STANDSTILL      = (0U),  /**< BMW Input signal state: Vehicle_standstill */
   BMW_CTB_VEH_MOVING_DIR_MOVES_FORWARD   = (1U),  /**< BMW Input signal state: Vehicle_moves_forward */
   BMW_CTB_VEH_MOVING_DIR_MOVES_BACKWARDS = (2U),  /**< BMW Input signal state: Vehicle_moves_backwards */
   BMW_CTB_VEH_MOVING_DIR_MOVING          = (3U),  /**< BMW Input signal state: Vehicle_is_moving */
   BMW_CTB_VEH_MOVING_DIR_ERROR           = (14U), /**< BMW Input signal state: LTB_FUNCTION_REPORTS_ERROR */
   BMW_CTB_VEH_MOVING_DIR_UNFILLED        = (15U)  /**< BMW Input signal state: LTB_SIGNAL_UNFILLED */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ltb_Vehicle_Moving_Direction_T;
typedef enum
{
   BMW_LTB_NO_DYNAMOMETER             = (0U),  /**< BMW Dynamometer status : No dynamometer */
   BMW_LTB_FRONT_AXLE_ON_DYNAMOMETER  = (1U),  /**< BMW Dynamometer status : Front Axle dynamometer */
   BMW_LTB_BACK_AXLE_ON_DYNAMOMETER   = (2U),  /**< BMW Dynamometer status : Rear Axle dynamometer */
   BMW_LTB_TWO_AXLE_DYNAMOMETER       = (3U),  /**< BMW Dynamometer status : Two Axle dynamometer */
   BMW_LTB_LTB_FUNCTION_REPORTS_ERROR = (14U), /**< BMW Dynamometer status : Dynamometer Error */
   BMW_LTB_LTB_SIGNAL_UNFILLED        = (15U)  /**< BMW Dynamometer status : Signal Unfilled */
} Bmw_Ltb_Status_Roller_Dynamometer_T;
typedef enum
{
   LTB_END_OF_LINE_MODE_NOT_SET = (0U),  /**< BMW : Vehicle on End of Line Roller */
   LTB_END_OF_LINE_MODE_SET     = (1U),  /**< BMW : Vehicle not on End of Line Roller */
   LTB_FUNCTION_REPORTS_ERROR   = (14U), /**< BMW : Signal Error */
   LTB_SIGNAL_UNFILLED          = (15U)  /**< BMW : Signal Unfilled */
} Bmw_Ltb_Status_End_Of_Line_T;
typedef enum
{
   PARKING_BORDNET_NOT_IN_ORDER              = (1),  /**< Parking bordnet not available*/
   PARKING_BORDNET_IN_ORDER                  = (2),  /**< Parking bordnet in order*/
   STANDING_FUNCTION_CUSTOMER_NOT_IN_VEHICLE = (3),  /**< Standing Function customer not in vehicle*/
   DWELLING                                  = (5),  /**< Dwelling */
   TESTING                                   = (7),  /**< Testing */
   ESTABLISH_DRIVING_AVAILABILITY            = (8),  /**< Establish Driving Availability*/
   DRIVING                                   = (10), /**< Driving */
   END_DRIVING_AVAILABILITY                  = (12)  /**<End Driving Availability */
} Ltb_Pwf_State_T;
typedef enum
{
   NO_TRAILER_AVAILABLE = (0U),
   TRAILER_AVAILABLE    = (1U)
} Bmw_Ltb_Trailer_Status_T;
typedef enum
{
   LTB_NO_ERROR = (0U),
   LTB_ERROR    = (2U)
} Bmw_Ltb_Error_T;
typedef struct
{
   boolean_T Ltb_Enable_Check;
   boolean_T Ltb_Speed_Check_Flag;
   boolean_T Ltb_Speed_Check_Hys_Flag;
   boolean_T Ltb_Check_Forward_Driving_Direction_Flag;
   boolean_T Ltb_Check_Pwf_State_Driving_Flag;
   boolean_T Ltb_Test_Mode_Check_Flag;
   boolean_T Ltb_Error_Check_Flag;
} Ltb_State_Flags_T;
/**
 * @brief LTB BMW Vehicle Inputs
 *
 *
 * @SRD{}
 * @SAD{}
 * @SDD{n/a}
 */
typedef struct
{
   Ltb_Pwf_State_T pwf_state;
   Bmw_Ltb_Trailer_Status_T status_trailer;
   Bmw_Ltb_Status_Roller_Dynamometer_T status_roller_dynamometer;
   Bmw_Ltb_Status_End_Of_Line_T status_end_of_lne;
   Bmw_Ltb_Vehicle_Moving_Direction_T vehicle_moving_direction;
} Ltb_Vehicle_Inputs_T;
#endif /* LTB_BMW_SP25_TYPES_H */

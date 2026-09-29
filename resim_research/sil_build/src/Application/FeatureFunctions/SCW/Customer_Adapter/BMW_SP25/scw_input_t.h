#ifndef SCW_INPUT_T_H
#define SCW_INPUT_T_H

/**
 * @file scw_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the BMW SP25 input data structure for SCW.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

#include "pa_reuse.h"
#include "scw_types.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Contains the BMW input states for the Vehicle_Moving_Direction signal.
 */
typedef enum
{
   SCW_BMW_VEH_MOVING_DIR_STANDSTILL      = (0U),  /**< BMW Input signal state: Vehicle_standstill */
   SCW_BMW_VEH_MOVING_DIR_MOVES_FORWARD   = (1U),  /**< BMW Input signal state: Vehicle_moves_forward */
   SCW_BMW_VEH_MOVING_DIR_MOVES_BACKWARDS = (2U),  /**< BMW Input signal state: Vehicle_moves_backwards */
   SCW_BMW_VEH_MOVING_DIR_MOVING          = (3U),  /**< BMW Input signal state: Vehicle_is_moving */
   SCW_BMW_VEH_MOVING_DIR_ERROR           = (14U), /**< BMW Input signal state: Function_reports_error */
   SCW_BMW_VEH_MOVING_DIR_UNFILLED        = (15U)  /**< BMW Input signal state: Signal_unfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Scw_Vehicle_Moving_Direction_T;
typedef enum
{
   SCW_BMW_STATUS_ROLLER_DYNAMOMETER_NO_DYNAMOMETER            = (0U),  /**< BMW Dynamometer status : No dynamometer */
   SCW_BMW_STATUS_ROLLER_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER = (1U),  /**< BMW Dynamometer status : Front Axle dynamometer */
   SCW_BMW_STATUS_ROLLER_DYNAMOMETER_BACK_AXLE_ON_DYNAMOMETER  = (2U),  /**< BMW Dynamometer status : Rear Axle dynamometer */
   SCW_BMW_STATUS_ROLLER_DYNAMOMETER_TWO_AXLE_DYNAMOMETER      = (3U),  /**< BMW Dynamometer status : Two Axle dynamometer */
   SCW_BMW_STATUS_ROLLER_DYNAMOMETER_FUNCTION_REPORTS_ERROR    = (14U), /**< BMW Dynamometer status : Dynamometer Error */
   SCW_BMW_STATUS_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED           = (15U)  /**< BMW Dynamometer status : Signal Unfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Scw_Status_Roller_Dynamometer_T;
typedef enum
{
   SCW_BMW_STATUS_END_OF_LINE_MODE_NOT_SET           = (0U),  /**< BMW : Vehicle on End of Line Roller */
   SCW_BMW_STATUS_END_OF_LINE_MODE_SET               = (1U),  /**< BMW : Vehicle not on End of Line Roller */
   SCW_BMW_STATUS_END_OF_LINE_FUNCTION_REPORTS_ERROR = (14U), /**< BMW : Signal Error */
   SCW_BMW_STATUS_END_OF_LINE_SIGNAL_UNFILLED        = (15U)  /**< BMW : Signal Unfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Scw_Status_End_Of_Line_T;
typedef enum
{
   SCW_BMW_COUNTRY_VARIANT_NOT_AVAILABLE = (0U),
   SCW_BMW_COUNTRY_VARIANT_EUROPE        = (1U),
   SCW_BMW_COUNTRY_VARIANT_USA           = (2U),
   SCW_BMW_COUNTRY_VARIANT_CHINA         = (3U)
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Scw_Country_Variant_T;
typedef enum
{
   SCW_BMW_NO_TRAILER_AVAILABLE = (0U),
   SCW_BMW_TRAILER_AVAILABLE    = (1U)
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Scw_Trailer_Status_T;
typedef enum
{
   SCW_BMW_PWF_STATE_PARKING_BORDNET_NOT_IN_ORDER              = (1),  /**< Parking bordnet not available*/
   SCW_BMW_PWF_STATE_PARKING_BORDNET_IN_ORDER                  = (2),  /**< Parking bordnet in order*/
   SCW_BMW_PWF_STATE_STANDING_FUNCTION_CUSTOMER_NOT_IN_VEHICLE = (3),  /**< Standing Function customer not in vehicle*/
   SCW_BMW_PWF_STATE_DWELLING                                  = (5),  /**< Dwelling */
   SCW_BMW_PWF_STATE_TESTING                                   = (7),  /**< Testing */
   SCW_BMW_PWF_STATE_ESTABLISH_DRIVING_AVAILABILITY            = (8),  /**< Establish Driving Availability*/
   SCW_BMW_PWF_STATE_DRIVING                                   = (10), /**< Driving */
   SCW_BMW_PWF_STATE_END_DRIVING_AVAILABILITY                  = (12)  /**< End Driving Availability */
} Scw_Pwf_State_T;
/**
 * @brief BMW SP25 specific SCW coding parameters
 *
 * @return void
 *
 * @SAE{SF-2993}
 * @SRS{SF-8176}
 */
typedef struct
{
   boolean_T c_scw_bike_carrier_mode;         /**< SCW bike carrier mode activation*/
   Bmw_Scw_Country_Variant_T country_variant; /**< SCW Country Variant */
   float32_T c_scw_min_vel_lower_limit;       /**< Min Velocity Limit*/
   float32_T c_scw_min_vel_upper_limit;       /**< Min Velocity Limit*/
   float32_T c_scw_max_vel_lower_limit;       /**< Max Velocity Limit*/
   float32_T c_scw_max_vel_upper_limit;       /**< Max Velocity Limit*/
} Bmw_Scw_Coding_Parameters_T;

/**
 * @brief BMW SP25 specific SCW vehicle input signals
 *
 * @return void
 *
 * @SAE{SF-2993}
 * @SRS{SF-8176}
 */
typedef struct
{
   Scw_Pwf_State_T vehicle_condition;                              /**< Vehicle PWF signals */
   Bmw_Scw_Vehicle_Moving_Direction_T vehicle_driving_direction;   /**< Vehicle Movement Direction */
   Bmw_Scw_Trailer_Status_T vehicle_trailer_status;                /**< Trailer Attachment Status */
   Bmw_Scw_Status_Roller_Dynamometer_T vehicle_dynamometer_status; /**< Test Mode Activation via Dynamometer Status*/
   Bmw_Scw_Status_End_Of_Line_T vehicle_end_of_line_status;        /**< End of Line Status*/
} Bmw_Scw_Vehicle_Bus_Inputs_T;

/**
 * @brief BMW SP25 specific SCW input structure
 *
 * @return void
 *
 * @SAE{SF-2993}
 * @SRS{SF-8176}
 */
typedef struct
{
   uint8_t f_scw_enable;                              /**< enable/disable SCW feature */
   uint8_t f_scw_enable_dynamic;                      /**< enable/disable SCW dynamic object subfeature */
   uint8_t f_scw_enable_guardrail;                    /**< enable/disable SCW guardrail subfeature */
   Bmw_Scw_Coding_Parameters_T scw_coding_parameters; /**< Coding parameters for SCW */
   Bmw_Scw_Vehicle_Bus_Inputs_T scw_vehicle_input;    /**< Vehicle Inputs for SCW*/

   boolean_T f_trailer_present; /**< Flag indicating that a trailer is attached */
   float32_T trailer_length;    /**< [m] Trailer length */
   float32_T trailer_width;     /**< [m] Trailer width */
   float32_T trailer_angle;     /**< [rad] Trailer angle */
} Scw_Input_T;

#endif /* SCW_INPUT_T_H */

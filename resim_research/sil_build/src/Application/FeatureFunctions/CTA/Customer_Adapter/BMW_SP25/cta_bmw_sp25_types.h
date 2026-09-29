#ifndef CTA_BMW_SP25_TYPES_H
#define CTA_BMW_SP25_TYPES_H
/**
 * @file cta_bmw_sp25_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for BMW_SP25 specific CTA types, enums and defines
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */
/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "cta_types.h"
#define CTA_BMW_ALERT_POSITION_COUNT (4u) /**< Amount of alert positions (FL, FR, RL, RR) */
#define CTA_BMW_SIDE_COUNT (2u)           /**< Amount of sides (Left, Right) */
#define CTA_BMW_BIT_POSITION_0 (0x0u)     /**< Bitposition 0x0000 0000*/
#define CTA_BMW_BIT_POSITION_1 (0x1u)     /**< Bitposition 0x0000 0001 */
#define CTA_BMW_BIT_POSITION_2 (0x2u)     /**< Bitposition 0x0000 0010 */
#define CTA_BMW_BIT_POSITION_3 (0x4u)     /**< Bitposition 0x0000 0100 */
#define CTA_BMW_BIT_POSITION_4 (0x8u)     /**< Bitposition 0x0000 1000 */
#define CTA_ACCELERATION_PRIORITIZED (7u)
#define CTA_FASOCLI_ACCELERATION_PRIORITIZED (1u)
/*============================================================================*\
* BMW SPECIFIC TYPE DEFINITIONS
\*============================================================================*/
/**
 * @brief Contains the BMW SP25 State Outputs
 */
typedef enum
{
   CTB_STATE_READY          = (1U), /**< BMW CTB state: Ready */
   CTB_STATE_RCTA_ACTIVE    = (5U),
   CTB_STATE_ERROR          = (6U),  /**< BMW CTB state: Error */
   CTB_STATE_ACTIVE         = (9U),  /**< BMW CTB state: Active*/
   CTB_STATE_ACTIVE_BRAKING = (10U), /**< BMW CTB state: Active Braking */
   CTB_STATE_FCTA_ACTIVE    = (12U),
   CTB_STATE_NOT_AVAILABLE  = (14U), /**< BMW CTB state: Not Available */
   CTB_STATE_UNFILLED       = (15U)  /**< BMW CTB state: Unfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Ctb_State_Output_T;
/**
 * @brief Contains the BMW input states for the Vehicle_Moving_Direction signal.
 */
typedef enum
{
   BMW_CTB_VEHICLE_IS_IN_STANDSTILL       = (0U),  /**< BMW Input signal state: Vehicle_standstill */
   BMW_CTB_VEHICLE_IS_MOVING_FORWARDS     = (1U),  /**< BMW Input signal state: Vehicle_moves_forward */
   BMW_CTB_VEHICLE_IS_MOVING_BACKWARDS    = (2U),  /**< BMW Input signal state: Vehicle_moves_backwards */
   BMW_CTB_VEHICLE_IS_MOVING              = (3U),  /**< BMW Input signal state: Vehicle_is_moving */
   BMW_CTB_VEHICLE_FUNCTION_REPORTS_ERROR = (14U), /**< BMW Input signal state: Function_reports_error */
   BMW_CTB_VEHICLE_SIGNAL_UNFILLED        = (15U)  /**< BMW Input signal state: Signal_unfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Vehicle_Moving_Direction_T;
/**
 * @brief Contains the BMW input states for the Qualifier_Gradient_Angle_AcceleratorPedal signal.
 */
typedef enum
{
   BMW_CTB_VALUE_VALID_HIGH_QUALITY    = (1U),  /**< BMW Input signal state: Value_valid_high_quality */
   BMW_CTB_VALUE_VALID_SIMPLE_QUALITY  = (2U),  /**< BMW Input signal state: Value_valid_simple_quality */
   BMW_CTB_VALUE_VALID_LIMITED_QUALITY = (3U),  /**< BMW Input signal state: Value_valid_limited_quality */
   BMW_CTB_VALUE_VALID_SUBSTITUDE      = (4U),  /**< BMW Input signal state: Value_valid_substitute */
   BMW_CTB_VALUE_NOT_AVAILABLE_ERROR   = (6U),  /**< BMW Input signal state: Value_not_available_error */
   BMW_CTB_VALUE_VALID_MEDIUM_QUALITY  = (9U),  /**< BMW Input signal state: Value_valid_medium_quality */
   BMW_CTB_VALUE_NOT_AVAILABLE         = (14U), /**< BMW Input signal state: Value_not_available */
   BMW_CTB_UNFILLED                    = (15U)  /**< BMW Input signal state: Unfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Qu_Gradient_Angle_Acc_Pedal_T;
/**
 * @brief Contains the BMW input states for the c_ctb_ttc_ttp_use coding parameter.
 */
typedef enum
{
   BMW_CTB_TTC_TTP_USE_TTC = (0U), /**< BMW Input coding parameter state: TTC */
   BMW_CTB_TTC_TTP_USE_TTP = (1U)  /**< BMW Input coding parameter state: TTP */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Ttc_Ttp_Use_T;
/**
 * @brief Contains the BMW input states for the c_ctb_Variant coding parameter.
 */
typedef enum
{
   BMW_CTB_VARIANT_NO_CTB             = (0U), /**< BMW Input coding parameter state: No_CTB */
   BMW_CTB_VARIANT_CTB_REAR           = (1U), /**< BMW Input coding parameter state: CTB_Rear */
   BMW_CTB_VARIANT_CTB_REAR_AND_FRONT = (2U)  /**< BMW Input coding parameter state: CTB_Rear_And_Front */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Variant_T;
/**
 * @brief Contains the BMW output states for the various Warning output signals.
 */
typedef enum
{
   BMW_CTB_NO_WARNING = (0u), /**< BMW Output for the state No_Warning */
   BMW_CTB_WARNING    = (1u)  /**< BMW Output for the state Warning */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Warning_T;
/**
 * @brief Contains the BMW output states for the Request_Ext_Mirror_Warning (Left/Right) signal.
 */
typedef enum
{
   BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF          = (0U), /**< BMW Output ExtMirror for the state Segment_Off */
   BMW_CTB_REQUEST_MIRROR_DISPLAY_SEGMENT_ON_NO_FLASHING = (1U), /**< BMW Output ExtMirror for the state Segment_On_No_Flashing */
   BMW_CTB_REQUEST_MIRROR_DISPLAY_SEGMENT_ON_FLASHING_LEVEL_1 = (2U), /**< BMW Output ExtMirror for the state Segment_Level1 */
   BMW_CTB_REQUEST_MIRROR_INTERFACE_NOT_AVAILABLE = (13U), /**< BMW Output ExtMirror for the state Segment_Interface NotAvailable */
   BMW_CTB_REQUEST_MIRROR_FUNCTION_REPORTS_ERROR = (14U), /**< BMW Output ExtMirror for the state Segment_Function Reports Error */
   BMW_CTB_REQUEST_MIRROR_SIGNAL_UNFILLED        = (15U)  /**< BMW Output ExtMirror for the state Segment_SignalUnfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Request_Ext_Mirror_Warning_T;
/**
 * @brief Contains the BMW output states for the Status_Brake_Requirement signal.
 */
typedef enum
{
   BMW_CTB_STATUS_BRAKE_REQ_NO_BRAKING = (0U), /**< BMW Output Status_Brake_Requirement for the state No_Braking */
   BMW_CTB_STATUS_BRAKE_REQ_BRAKING    = (1U)  /**< BMW Output Status_Brake_Requirement for the state Braking */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Status_Brake_Req_T;
/**
 * @brief Contains the BMW output states for the Braking signal.
 */
typedef enum
{
   BMW_CTB_NO_BRAKING                      = (0U),  /**< BMW Output Braking for the state No_Braking */
   BMW_CTB_BRAKING_AT_FRONT                = (1U),  /**< BMW Output Braking for the state Braking_At_Front */
   BMW_CTB_BRAKING_AT_REAR                 = (2U),  /**< BMW Output Braking for the state Braking_At_Rear */
   BMW_CTB_BRAKING_INTERFACE_NOT_AVAILABLE = (13U), /**< BMW Output Braking for the state Braking_Interface not available */
   BMW_CTB_BRAKING_FUNCTION_REPORTS_ERROR  = (14U), /**< BMW Output Braking for the state Braking_Function Reports Error */
   BMW_CTB_BRAKING_SIGNAL_UNFILLED         = (15U)  /**< BMW Output Braking for the state Braking Signal Unfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Braking_T;
/**
 * @brief Contains the BMW alert positions used for multiple output signals.
 */
typedef enum
{
   BMW_CTB_ALERT_POSITION_FRONT_RIGHT = (0U), /**< BMW alert position for Front_Right */
   BMW_CTB_ALERT_POSITION_FRONT_LEFT  = (1U), /**< BMW alert position for Front_Left */
   BMW_CTB_ALERT_POSITION_REAR_RIGHT  = (2U), /**< BMW alert position for Rear_Right */
   BMW_CTB_ALERT_POSITION_REAR_LEFT   = (3U)  /**< BMW alert position for Rear_Left */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Alert_Position_T;
/**
 * @brief Contains the BMW alert side used for Ext-Mirror signal.
 */
typedef enum
{
   BMW_CTB_ALERT_SIDE_LEFT  = (0U), /**< BMW alert position for Left side */
   BMW_CTB_ALERT_SIDE_RIGHT = (1U)  /**< BMW alert position for Right side */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Alert_Side_T;
/**
 * @brief Contains the BMW Variant Selection Status
 */
typedef enum
{
   BMW_CTB_NOT_CONFIGURABLE = (0U), /**< BMW CTB status: Not configurable */
   BMW_CTB_DEACTIVATED      = (1U), /**< BMW CTB status: De activated */
   BMW_CTB_ACTIVATED        = (2U)  /**< BMW CTB status: Activated */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Status_T;
/**
 * @brief Contains the BMW specific Warning-Output-Alert-State.
 */
typedef enum
{
   BMW_CTB_WARN_STATE_NO_WARNING          = (0U), /**< BMW No Warning at Rear or Front */
   BMW_CTB_WARN_STATE_INFO_WARNING_FRONT  = (1U), /**< BMW Warning at the Front */
   BMW_CTB_WARN_STATE_INFO_WARNING_REAR   = (2U), /**< BMW Warning at the Rear */
   BMW_CTB_WARN_STATE_ACUTE_WARNING_FRONT = (3U), /**< BMW Acute Warning at the Front */
   BMW_CTB_WARN_STATE_ACUTE_WARNING_REAR  = (4U)  /**< BMW Acute Warning at the Rear */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Warn_Control_Output_State_T;
typedef enum
{
   BMW_CTB_NO_DYNAMOMETER            = (0U),  /**< BMW Dynamometer status : No dynamometer */
   BMW_CTB_FRONT_AXLE_ON_DYNAMOMETER = (1U),  /**< BMW Dynamometer status : Front Axle dynamometer */
   BMW_CTB_BACK_AXLE_ON_DYNAMOMETER  = (2U),  /**< BMW Dynamometer status : Rear Axle dynamometer */
   BMW_CTB_TWO_AXLE_DYNAMOMETER      = (3U),  /**< BMW Dynamometer status : Two Axle dynamometer */
   BMW_CTB_FUNCTION_REPORTS_ERROR    = (14U), /**< BMW Dynamometer status : Dynamometer Error */
   BMW_CTB_SIGNAL_UNFILLED           = (15U)  /**< BMW Dynamometer status : Signal Unfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Status_Roller_Dynamometer_T;
typedef enum
{
   END_OF_LINE_MODE_NOT_SET = (0U),  /**< BMW : Vehicle on End of Line Roller */
   END_OF_LINE_MODE_SET     = (1U),  /**< BMW : Vehicle not on End of Line Roller */
   FUNCTION_REPORTS_ERROR   = (14U), /**< BMW : Signal Error */
   SIGNAL_UNFILLED          = (15U)  /**< BMW : Signal Unfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Status_End_Of_Line_T;
typedef enum
{
   BMW_CTA_NO_ERROR = (0U), /**< Error Set to FALSE */
   BMW_CTA_ERROR    = (1U)  /**< Error Set to Critical */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Ctb_Function_Error_T;
/**
 * @brief Contains the BMW CTB Status_Brake_Request output signals.
 */
typedef enum
{
   BMW_CTB_TARGET_VALUE_NOT_IMPLEMENTED     = (1U),  /**< BMW : Target Value Not Implemented*/
   BMW_CTB_TARGET_VALUE_IMPLEMENTED         = (2U),  /**< BMW : Target Value Implemented */
   BMW_CTB_TARGET_VALUE_NOT_AVAILABLE       = (6U),  /**< BMW : Target Value Not Available */
   BMW_CTB_TARGET_VALUE_NOT_AVAILABLE_ERROR = (14U), /**< BMW : Target Value Not Available Error */
   BMW_CTB_TARGET_SIGNAL_UNFILLED           = (15U)  /**< BMW : Signal Unfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Status_Brake_Request_T;
/**
 * @brief Contains the BMW CTB Display_Warning_Graphically output signals.
 */
typedef enum
{
   CTB_NO_WARNING                      = (0U),   /**< BMW :No Warning */
   CTB_WARNING_FRONT_RIGHT             = (1U),   /**< BMW : CTB Front Right Warning */
   CTB_WARNING_FRONT_LEFT              = (2U),   /**< BMW : CTB Front Left Warning */
   CTB_WARNING_FRONT_RIGHT_AND_LEFT    = (3U),   /**< BMW : CTB Front Right and Left Warning*/
   CTB_WARNING_BACK_RIGHT              = (4U),   /**< BMW : CTB Back Right Warning*/
   CTB_WARNING_BACK_LEFT               = (8U),   /**< BMW : CTB Back Left Warning */
   CTB_WARNING_BACK_RIGHT_AND_LEFT     = (12U),  /**< BMW : CTB Back Right and Left Warning */
   CTB_WARNING_FRONT_NOT_AVAILABLE     = (16U),  /**< BMW :CTB Front Not Available  */
   CTB_WARNING_BACK_NOT_AVAILABLE      = (32U),  /**< BMW : CTB Back not Available*/
   CTB_TARGETVALUE_NOT_AVAILABLE       = (253U), /**< BMW: Target Value not Available*/
   CTB_TARGETVALUE_NOT_AVAILABLE_ERROR = (254U), /**< BMW: Target Value not Available Error*/
   CTB_SIGNAL_UNFILLED                 = (255U)  /**< BMW : Signal Unfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Display_Warning_Graphically_T;
/**
 * @brief Contains the BMW Ctb_Warning_Acoustics output signals.
 */
typedef struct
{
   uint16_t ctb_earconId;         /**< BMW : earConID */
   uint32_t ctb_replicasCount;    /**< BMW : replicasCount*/
   float32_T ctb_horizontalAngle; /**< BMW : horizontalAngle */
   float32_T ctb_verticalAngle;   /**< BMW : verticalAngle*/
   uint32_t ctb_presentationTime; /**BMW : presentationTime*/
   uint32_t ctb_period;           /**< BMW : Period*/
   uint32_t ctb_handleID;         /** <BMW :Handle ID*/
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Warning_Acoustics_T;
/**
 * @brief Contains the BMW output states for the various Warning output signals.
 */
typedef enum
{
   BMW_OUTPUT_CTB_NO_WARNING              = (0u),   /**< BMW Output for the state No_Warning */
   BMW_OUTPUT_CTB_WARNING_AT_FRONT_RIGHT  = (1U),   /**< BMW : CTB Front Right Warning */
   BMW_OUTPUT_CTB_WARNING_AT_FRONT_LEFT   = (2U),   /**< BMW : CTB Front Left Warning */
   BMW_OUTPUT_CTB_WARNING_AT_REAR_RIGHT   = (4U),   /**< BMW : CTB Rear Right Warning*/
   BMW_OUTPUT_CTB_WARNING_AT_REAR_LEFT    = (8U),   /**< BMW : CTB Rear Left Warning */
   BMW_OUTPUT_CTB_INTERFACE_NOT_AVAILABLE = (253U), /**< BMW: INTERFACE_NOT_AVAILABLE*/
   BMW_OUTPUT_CTB_FUNCTION_REPORTS_ERROR  = (254U), /**< BMW: FUNCTION_REPORTS_ERROR*/
   BMW_OUTPUT_CTB_SIGNAL_UNFILLED         = (255U)  /**< BMW : Signal Unfilled */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Output_Ctb_Warning_T;
/**
 * @brief Contains the BMW output states for the various Warning output signals.
 */
typedef enum
{
   BMW_CTB_SIGNAL_VALUE_VALID_SECURED_CHECKED_FOR_PLAUSIBILITY = (1u),    /**< BMW Output
                                                                             Signal_value_valid_secured_checked_for_plausibility */
   BMW_CTB_INITIALIZATION                                        = (8U),  /**< BMW :  Initialization */
   BMW_CTB_SIGNAL_VALUE_VALID_STATUS_TEMPORARY                   = (10U), /**< BMW : Signal_value_valid_status_temporary */
   BMW_CTB_SIGNAL_QUALITY_OR_MONITORING_LIMITED_STATUS_TEMPORARY = (11U), /**< BMW :
                                                                             Signal_quality_or_monitoring_limited_status_temporary*/
   BMW_CTB_SIGNAL_VALUE_INVALID_STATUS_TEMPORARY = (14U),                 /**< BMW : Signal_value_invalid_status_temporary*/
   BMW_CTB_SIGNAL_INVALID                        = (15)                   /**< BMW : Signal_invalid */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Input_Ctb_Qualifier_Vehicle_Speed_T;
/**
 * @brief Contains the BMW input status trailer.
 */
typedef enum
{
   BMW_CTB_NO_TRAILER_AVAILABLE = (0u), /**< BMW input for the status Trailer Not Available */
   BMW_CTB_TRAILER_AVAILABLE    = (1u)  /**< BMW input for the status Trailer Not Available */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Ctb_Status_Trailer_T;
/**
 * @brief Contains all customer specific input signals from the vehicle bus.
 */
typedef struct
{
   Bmw_Ctb_Vehicle_Moving_Direction_T vehicle_moving_direction; /**< Host vehicle moving direction */
   float32_T gradient_angle_acceleratorpedal;                   /**< Current gradient of the accelerator pedal */
   Bmw_Ctb_Qu_Gradient_Angle_Acc_Pedal_T qualifier_gradient_angle_acceleratorpedal; /**< Qualifier for the
                                                                                       gradient_Angle_AcceleratorPedal signal */
   Bmw_Ctb_Status_Trailer_T status_trailer;                                         /**< Trailer connected flag */
   Bmw_Ctb_Status_T setting_cross_traffic_brake; /**< Describes whether CTB (warning only) is activated by the driver / parking
                                                    master */
   Bmw_Ctb_Status_T setting_cross_traffic_brake_with_braking; /**< Describes whether CTB (warning and braking) is activated by the
                                                                 driver / parking master */
   Bmw_Ctb_Status_Roller_Dynamometer_T status_roller_dynamometer; /**< Provides information about the roller dynamometer test bench
                                                                     operating mode*/
   Bmw_Ctb_Status_End_Of_Line_T status_end_of_line;       /**< provides information about whether the vehicle is on the end-of-line
                                                             roller following assembly */
   uint8_t status_arbitration_longitudinal_low_integrity; /**< Describes whether the braking request from CTB was actually piped
                                                             through by other modules.*/
   uint8_t status_acceleration_long_prioritization; /**< Describes whether the braking request from CTB was actually piped through
                                                       by other modules. */
   Bmw_Input_Ctb_Qualifier_Vehicle_Speed_T qualifier_vehicle_speed;
   Ctb_Function_Error_T ctb_function_error;
   boolean_T parking_context_active;
   boolean_T control_cross_traffic_alert_front;
   boolean_T control_cross_traffic_alert_rear;
   boolean_T control_cross_traffic_alert_rear_braking;
} Bmw_Ctb_Input_Bus_Signals_T;
/**
 * @brief Contains all customer specific inputs from the coding parameter list.
 */
typedef struct
{
   boolean_T c_rctb_extended_pre_alert_zone; /**< Flag indicating if the warning zone shall be extended - only for rear */
   Bmw_Ctb_Ttc_Ttp_Use_T c_ctb_ttc_ttp_use;  /**< Flag indicating if TTC or TTP shall be used */
   boolean_T c_ctb_acute_warning_same_time_with_advance_warning; /**< Flag indicating if the Acute-Warning shall be present already
                                                                    with Pre-Warning */
   boolean_T c_ctb_enabled;                                      /**< Overall availability of CTB */
   Bmw_Ctb_Variant_T c_ctb_variant;       /**< Provides the information of availability Front, Rear or both */
   boolean_T c_ctb_braking_enabled_rear;  /**< Flag indicating if CTB is braking for rear */
   boolean_T c_ctb_braking_enabled_front; /**< Flag indicating if CTB is braking for front */
} Bmw_Ctb_Input_Coding_T;
/**
 * @brief Contains the algorithm outputs of CTA - in an easy to read/understand type (Algo-Post-Run).
 */
typedef struct
{
   uint32_t unique_id[CTA_BMW_ALERT_POSITION_COUNT];

   Bmw_Ctb_Warn_Control_Output_State_T warn_control_output_state[CTA_BMW_SIDE_COUNT];
   Bmw_Ctb_Warning_T display_warning_graphically[CTA_BMW_ALERT_POSITION_COUNT];        /**< Content of the BMW-Output
                                                                                          display_Warning_Graphically for all positions */
   Bmw_Ctb_Request_Ext_Mirror_Warning_T request_extmirror_warning[CTA_BMW_SIDE_COUNT]; /**< Content of the BMW-Output
                                                                                         request_ExtMirror_Warning for both sides */
   Bmw_Ctb_Warning_T warning_acoustics[CTA_BMW_ALERT_POSITION_COUNT]; /**< Content of the BMW-Output warning_Acoustics for all
                                                                         positions */
   Bmw_Ctb_Status_Brake_Req_T status_brake_requirement;               /**< Flag indicating if a braking is active or not */
   float32_T target_longitudinal_acceleration; /**< Contains the desired ramp with the delay for control of the brake */
   Bmw_Ctb_Warning_T ctb_warning[CTA_BMW_ALERT_POSITION_COUNT];       /**< Content of the BMW-Output warning for all positions */
   Bmw_Ctb_Warning_T ctb_acute_warning[CTA_BMW_ALERT_POSITION_COUNT]; /**< Content of the BMW-Output acute_warning for all
                                                                         positions */
   Bmw_Ctb_Braking_T ctb_braking; /**< Flag indicating if a braking is active at FRONT, REAR or NONE  */
} Bmw_Ctb_Output_Algo_State_T;
/**
 * @brief Contains the final output signals of CTA - already transferred to BUS content (Statemachine-Post-Run).
 */
typedef struct
{
   Bmw_Output_Ctb_Warning_T warning_acoustics; /**< Bitfield indicating if an acoustical warning is active at FL, FR, RR, RL or
                                                  NONE */
   Bmw_Output_Ctb_Warning_T ctb_warning;       /**< Bitfield indicating if a warning is active at FL, FR, RR, RL or NONE */
   Bmw_Output_Ctb_Warning_T ctb_acute_warning; /**< Bitfield indicating if an acute warning is active at FL, FR, RR, RL or NONE */
   Ctb_State_Output_T qualifier_function_ctb;  /**< Contains the RUN/Functional-State of CTB Statemachine */
   Bmw_Ctb_Display_Warning_Graphically_T display_warning_graphically; /**< Bitfield indicating which corner of the vehicle shall be
                                          warned optically - FL, FR, RR, RL or NONE */
   Bmw_Ctb_Status_Brake_Req_T status_brake_requirement;               /**< Flag indicating if a braking is active or not */
   Bmw_Ctb_Braking_T ctb_braking;              /**< Flag indicating if a braking is active at FRONT, REAR or NONE  */
   float32_T target_longitudinal_acceleration; /**< Contains the desired target longitudinal acceleration*/
   Bmw_Ctb_Request_Ext_Mirror_Warning_T request_extmirror_warning_left;  /**< Flag indicating how to visually warn the relevant
                                            object in the exterior mirror LED
                                           on the LEFT side */
   Bmw_Ctb_Request_Ext_Mirror_Warning_T request_extmirror_warning_right; /**< Flag indicating how to visually warn the relevant
                                               object in the exterior mirror
                                               LED on the RIGHT side */
   Bmw_Ctb_Status_Brake_Request_T ctb_status_braking_request;            /**< Contains the desired ctb status of brake request*/
   Bmw_Ctb_Warning_Acoustics_T Bmw_ctb_warning_acoustics; /**< Contains the desired properties of ctb warning acoustics */
   boolean_T Bmw_Ctb_error_status;
   float32_T ctb_ttc_front_left;
   float32_T ctb_ttc_front_right;
   float32_T ctb_ttc_rear_left;
   float32_T ctb_ttc_rear_right;
} Bmw_Ctb_Output_Bus_Signals_T;
/**
 * @brief Contains Event-Data-Recorder output signals of CTA (Not yet finalized).
 */
typedef struct
{
   float32_T bmw_ctb_timestamp;
   float32_T bmw_ctb_f_acute_warning;
   float32_T bmw_ctb_f_acute_braking;
   float32_T bmw_ctb_ego_speed;
   float32_T bmw_ctb_object_speed;
   float32_T bmw_ctb_object_direction_of_motion_angle;
   float32_T bmw_ctb_ttc;
   float32_T bmw_ctb_ttp;
   float32_T bmw_ctb_object_distance_x;
   float32_T bmw_ctb_object_distance_y;
   float32_T bmw_ctb_warning;
} Bmw_Ctb_Output_Edr_T;
#endif /* CTA_BMW_SP25_TYPES_H */

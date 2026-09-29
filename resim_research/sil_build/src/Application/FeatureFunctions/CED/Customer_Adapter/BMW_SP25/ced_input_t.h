#ifndef CED_INPUT_T_H
#define CED_INPUT_T_H

/**
 * @file ced_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the BMW_SP25 input data structure for CED.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_bmw_sp25_types.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pt_output_t.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Ced_Coding_Parameters_T structure.
 */
typedef struct
{
   boolean_T c_f_sfe_doors_automatic_availability;                    /**< Indicates whether automatic doors are available */
   boolean_T c_f_sfe_door_locks_electronic_controllable_availability; /**< Indicates whether electric doors are available */
   boolean_T c_f_sfe_function_activation;                             /**< Indicates whether the fucntion shall be active */
   boolean_T c_f_sfe_function_enabled;                                /**< Indicates whether the fucntion shall be enabled */

   boolean_T c_f_sfe_door_control_activation;      /* still needed? */
   boolean_T c_f_sfe_door_lock_control_activation; /**< Indicates if the door locks shall be controled with the SFE function */
   boolean_T c_f_sfe_door_actuator_activation;     /**< Indicates the the actuators for the automated door opening/closing shall be
                                                      conbtrolled by the sfe function */
   boolean_T c_f_sfe_mirror_light_activation; /**< Indicates whether the mirror light shall be controlled by the sfe function */
   boolean_T c_f_sfe_mirror_light_information_activation;   /**< Indicates whether the mirror light shall be controlled by the sfe
                                                               function */
   boolean_T c_f_sfe_mirror_light_warning_activation;       /**< Indicates whether the mirror light shall be controlled by the sfe
                                                               function */
   boolean_T c_f_sfe_mirror_light_acute_warning_activation; /**< Indicates whether the mirror light shall be controlled by the sfe
                                                               function */
   Ced_Mirror_Light_Warning_Type_T c_sfe_mirror_light_information_type; /**< Controlles with which type the mirror LED shall
                                                                               indicate an information */
   Ced_Mirror_Light_Warning_Type_T c_sfe_mirror_light_warning_type; /**< Controlles with which type the mirror LED shall indicate a
                                                                       warning/information ??? */
   Ced_Mirror_Light_Warning_Type_T c_sfe_mirror_light_acute_warning_type; /**< Controlles with which type the mirror LED shall
                                                                             indicate a warning */
   boolean_T c_f_sfe_interior_light_activation; /**< Indicates whether the interior light shall be activated by the sfe function */
   Ced_Door_Warning_Levels_T c_sfe_interior_light_type_information; /**< Controlles how the interior light shall behave in case of
                                                                       an information */
   Ced_Door_Warning_Levels_T c_sfe_interior_light_type_warning; /**< Controlles how the interior light shall behave in case of a
                                                                   warning */
   Ced_Door_Warning_Levels_T c_sfe_interior_light_type_acute_warning; /**< Controlles how the interior light shall behave in case
                                                                         of an acute warning */
   boolean_T c_f_sfe_sound_activation; /**< Indicates whether the warning sound shall be controlles by the SFE function */
   boolean_T c_f_sfe_optical_display_activation; /**< Indicates if the optical display at the door shall be controlled by the sfe
                                                    function */
   float32_T c_f_sfe_object_minimum_relative_velocity; /**< a coding parameter that can be used to define the minimum relative
                                                          speed of an object to be warned. */
   boolean_T c_f_sfe_fault_monitoring_activation; /**< a coding parameter that can be used to turn fault monitoring on and off. */
   boolean_T c_f_sfe_urgent_warning_during_automatic_door_opening_in_ready; /**< an encoding parameter to activated/disabled the
                                                                               urgent warning during automatic door opening in the
                                                                               Ready state. */
} Ced_Coding_Parameters_T;

typedef struct
{
   boolean_T cal_inject_seat_occupancy_front_right;   /**< Inject that the right front seat is occupied */
   boolean_T cal_inject_seat_occupancy_front_left;    /**< Inject that the left front seat is occupied */
   boolean_T cal_inject_seat_occupancy_rear_right;    /**< Inject that the right rear seat is occupied */
   boolean_T cal_inject_seat_occupancy_rear_left;     /**< Inject that the left rear seat is occupied */
   boolean_T cal_inject_seat_belt_buckle_front_right; /**< Inject that the right front seat belt buckle is opened/closed */
   boolean_T cal_inject_seat_belt_buckle_front_left;  /**< Inject that the left front seat belt buckle is opened/closed */
   boolean_T cal_inject_seat_belt_buckle_rear_right;  /**< Inject that the right rear seat belt buckle is opened/closed */
   boolean_T cal_inject_seat_belt_buckle_rear_left;   /**< Inject that the left rear seat belt buckle is opened/closed */
   boolean_T cal_inject_door_opened_front_right;      /**< Inject that the right front door is opened */
   boolean_T cal_inject_door_opened_front_left;       /**< Inject that the left front door is opened */
   boolean_T cal_inject_door_opened_rear_right;       /**< Inject that the right rear door is opened */
   boolean_T cal_inject_door_opened_rear_left;        /**< Inject that the left rea door is opened */
   float32_T cal_velocity_ego_max; /**< application parameter to manipulate the maximum speed when the SafeExit function is active.
                                    */
   float32_T cal_availability_max; /**< ?? */
   float32_T cal_information_distance;                             /**< ?? */
   uint8_t par_sfe_acute_warning_memory_number_of_stored_warnings; /**< Information about the trigger time of the last 15 acute
                                                         warning shall be readable via a diagnostic job. */
   float32_T cal_time_to_object_is_passed_by;                      /**< ???? */
   float32_T cal_information_ttc;                                  /**< Min TTC for an information */
   float32_T cal_information_zone_lateral;                         /**< lateral zone for an information */
   float32_T cal_information_ttp;                                  /**< Max TTP for an information */
   float32_T cal_warning_ttc;                                      /**< Min TTC for a warning */
   float32_T cal_warning_zone_lateral;                             /**< lateral zone for a warning */
   float32_T cal_warning_ttp;                                      /**< Max TTP for a warning */
   float32_T cal_warning_min_hold_time;                            /**< Minimum time for a warning to be hold */
   float32_T cal_acute_warning_ttc;                                /**< Min TTC for an actue warning */
   float32_T cal_acute_warning_zone_lateral;                       /**< lateral zone for an actue warning */
   float32_T cal_acute_warning_ttp;                                /**< Max TTP for an actue warning */
   float32_T cal_acute_warning_min_hold_time;                      /**< Minimum time for an acture warning to be hold */
   float32_T cal_information_min_hold_time;                        /**< Minimum time for an informaiton to be hold */
} Ced_Calibration_Parameters_T;


/** @brief SeatOccupancyEnum from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 SeatOccupancyEnum (72) */
typedef enum
{
   SEAT_OCCUPANCY_SEAT_OCCUPANCY_SEAT_EMPTY             = (0u),  /**< No value comment available in database */
   SEAT_OCCUPANCY_SEAT_OCCUPIED_PERSON                  = (1u),  /**< No value comment available in database */
   SEAT_OCCUPANCY_CHILDSEAT_OCCUPIED_PERSON             = (2u),  /**< No value comment available in database */
   SEAT_OCCUPANCY_CHILDSEAT_OCCUPATION_UNKNOWN          = (3u),  /**< No value comment available in database */
   SEAT_OCCUPANCY_SEAT_OCCUPIED_OBJECT                  = (4u),  /**< No value comment available in database */
   SEAT_OCCUPANCY_SEAT_NOT_EMPTY_OCCUPATION_UNKNOWN     = (5u),  /**< No value comment available in database */
   SEAT_OCCUPANCY_SEAT_NOT_AVAILABLE_WITHIN_THE_VEHICLE = (6u),  /**< No value comment available in database */
   SEAT_OCCUPANCY_UNKNOWN                               = (7u),  /**< No value comment available in database */
   SEAT_OCCUPANCY_CHILDSEAT_EMPTY                       = (8u),  /**< No value comment available in database */
   SEAT_OCCUPANCY_LIVING_BEING                          = (9u),  /**< No value comment available in database */
   SEAT_OCCUPANCY_ANIMAL                                = (10u), /**< No value comment available in database */
   SEAT_OCCUPANCY_SEAT_IN_NON_USE_POSITION              = (11u), /**< No value comment available in database */
   SEAT_OCCUPANCY_ERROR                                 = (12u)  /**< No value comment available in database */
} Seat_Occupancy_T;

/** @brief PersonAgeCategoryEnum from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 PersonAgeCategoryEnum (2) */
typedef enum
{
   PERSON_AGE_CATEGORY_UNKNOWN = (0u), /**< Initial value, no age information available */
   PERSON_AGE_CATEGORY_BABY    = (1u), /**< No value comment available in database */
   PERSON_AGE_CATEGORY_CHILD   = (2u), /**< No value comment available in database */
   PERSON_AGE_CATEGORY_TEEN    = (3u), /**< No value comment available in database */
   PERSON_AGE_CATEGORY_ADULT   = (4u), /**< No value comment available in database */
   PERSON_AGE_CATEGORY_ERROR   = (5u)  /**< 	function report failure */
} Person_Age_Category_T;

/** @brief PersonEntryExitEnum from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 PersonEntryExitEnum (2) */
typedef enum
{
   PERSON_ENTRY_EXIT_UNKNOWN                      = (0u), /**< Initial value, no entry/exit information available */
   PERSON_ENTRY_EXIT_ENTRY_PREDICTED              = (1u), /**< Entry is predicted */
   PERSON_ENTRY_EXIT_ENTRY_ONGOING_PERSON_INSIDE  = (2u), /**< Entry is ongoing, person is mostly inside */
   PERSON_ENTRY_EXIT_ENTRY_ONGOING_PERSON_OUTSIDE = (3u), /**< Entry is ongoing, person is mostly outside */
   PERSON_ENTRY_EXIT_EXIT_EXPECTED                = (4u), /**< Exit is expected */
   PERSON_ENTRY_EXIT_EXIT_POSSIBLE                = (5u), /**< Exit is possible */
   PERSON_ENTRY_EXIT_EXIT_ONGOING_PERSON_INSIDE   = (6u), /**< Exit is ongoing, person is mostly inside */
   PERSON_ENTRY_EXIT_EXIT_ONGOING_PERSON_OUTSIDE  = (7u), /**< Exit is ongoing, person is mostly outside */
   PERSON_ENTRY_EXIT_NO_EVENT                     = (8u), /**< No entry/exit event */
   PERSON_ENTRY_EXIT_ERROR                        = (9u)  /**< Entry/Exit information cannot be calcluated */
} Person_Entry_Exit_T;

/** @brief Qualifier_Container_T from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 Qualifier_Container_T (1) */
typedef enum
{
   QUALIFIER_CONTAINER_OK       = (0u), /**< All the contained values of data type element are up to date and valid. */
   QUALIFIER_CONTAINER_DEBOUNCE = (7u), /**< All the contained values of data type element are recently frozen by receiver. Data
                       age since last reception is shorter than network error debounce time. Sender is not allowed to use this
                       state in any type of communication. */
   QUALIFIER_CONTAINER_INIT = (9u), /**< At least one value of data type element is set with an initial value since start up. Data
                                       age is undefined. */
   QUALIFIER_CONTAINER_NOT_AVAILABLE = (12u), /**< At least one value of data type element is not available, no error evaluation is
                             active on the sender side. Value is set with a default or last valid frozen value. Data age is
                             undefined. */
   QUALIFIER_CONTAINER_ERROR = (15u) /**< At least one value of data type element is not available because of en error confirmed on
                     the sender side. Value is set with a default or last valid frozen value. Data age is undefined. */
} Qualifier_Container_T;


/** @brief AbsoluteSeatLocationEnum from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 AbsoluteSeatLocationEnum (77) */
typedef enum
{
   ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW1  = (1u),  /**< No value comment available in database */
   ABSOLUTE_SEAT_LOCATION_SEAT_MID_ROW1   = (2u),  /**< No value comment available in database */
   ABSOLUTE_SEAT_LOCATION_SEAT_RIGHT_ROW1 = (3u),  /**< No value comment available in database */
   ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW2  = (4u),  /**< No value comment available in database */
   ABSOLUTE_SEAT_LOCATION_SEAT_MID_ROW2   = (5u),  /**< No value comment available in database */
   ABSOLUTE_SEAT_LOCATION_SEAT_RIGHT_ROW2 = (6u),  /**< No value comment available in database */
   ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW3  = (7u),  /**< No value comment available in database */
   ABSOLUTE_SEAT_LOCATION_SEAT_MID_ROW3   = (8u),  /**< No value comment available in database */
   ABSOLUTE_SEAT_LOCATION_SEAT_RIGHT_ROW3 = (9u),  /**< No value comment available in database */
   ABSOLUTE_SEAT_LOCATION_UNKNOWN         = (10u), /**< No value comment available in database */
   ABSOLUTE_SEAT_LOCATION_INVALID         = (11u)  /**< No value comment available in database */
} Absolute_Seat_Location_T;

/** @brief AbsoluteSeatLocationEnum from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 AbsoluteSeatLocationEnum (77) */
typedef enum
{
   RELATIVE_SEAT_LOCATION_DRIVER_SEAT_ROW1             = (1u),  /**< No value comment available in database */
   RELATIVE_SEAT_LOCATION_PASSENGER_MID_SEAT_ROW1      = (2u),  /**< No value comment available in database */
   RELATIVE_SEAT_LOCATION_PASSENGER_SEAT_ROW1          = (3u),  /**< No value comment available in database */
   RELATIVE_SEAT_LOCATION_DRIVER_REAR_SEAT_ROW2        = (4u),  /**< No value comment available in database */
   RELATIVE_SEAT_LOCATION_PASSENGER_MID_REAR_SEAT_ROW2 = (5u),  /**< No value comment available in database */
   RELATIVE_SEAT_LOCATION_PASSENGER_REAR_SEAT_ROW2     = (6u),  /**< No value comment available in database */
   RELATIVE_SEAT_LOCATION_DRIVER_REAR_SEAT_ROW3        = (7u),  /**< No value comment available in database */
   RELATIVE_SEAT_LOCATION_PASSENGER_MID_REAR_SEAT_ROW3 = (8u),  /**< No value comment available in database */
   RELATIVE_SEAT_LOCATION_PASSENGER_REAR_SEAT_ROW3     = (9u),  /**< No value comment available in database */
   RELATIVE_SEAT_LOCATION_UNKNOWN                      = (10u), /**< No value comment available in database */
   RELATIVE_SEAT_LOCATION_INVALID                      = (11u)  /**< No value comment available in database */
} Relative_Seat_Location_T;

/** @brief StatusBeltBuckleSwitchEnum from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 StatusBeltBuckleSwitchEnum (2) */
typedef enum
{
   STATUS_BELT_BUCKLE_SWITCH_NOT_BUCKLED  = (0u), /**< not buckled */
   STATUS_BELT_BUCKLE_SWITCH_BUCKLED      = (1u), /**< buckled */
   STATUS_BELT_BUCKLE_SWITCH_NOT_EQUIPPED = (12u) /**< not equipped */
} Status_Belt_Buckle_Switch_T;

/** @brief Qualifier_Value_T from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 Qualifier_Value_T (11) */
typedef enum
{
   QUALIFIER_VALUE_OK       = (0u), /**< Value is up to date and valid. */
   QUALIFIER_VALUE_DEBOUNCE = (7u), /**< Last valid value is recently frozen by receiver. Data age since last reception is shorter
                     than network error debounce time. Sender is not allowed to use this state in any type of communication. */
   QUALIFIER_VALUE_INIT          = (9u),  /**< Value is set with an initial value since start up. Data age is undefined. */
   QUALIFIER_VALUE_NOT_AVAILABLE = (12u), /**< A valid value is not available, no error evaluation is active on the sender side.
                          Value is set with default or last valid frozen value. Data age is undefined. */
   QUALIFIER_VALUE_ERROR = (15u) /**< A valid value is not available because of a confirmed error on the sender side. Value is set
                     with default or last valid frozen value. Data age is undefined. */

} Qualifier_Value_T;

/** @brief SettingSafeExitValueEnum from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 SettingSafeExitValueEnum (71) */
typedef enum
{
   SETTING_SAFE_EXIT_VALUE_NOT_CONFIGURABLE  = (0u), /**< Do_not_use */
   SETTING_SAFE_EXIT_VALUE_DEACTIVATED       = (1u), /**< No value comment available in database */
   SETTING_SAFE_EXIT_VALUE_ACTIVATED         = (2u), /**< No value comment available in database */
   SETTING_SAFE_EXIT_VALUE_ACTIVATED_REDUCED = (3u)  /**< No value comment available in database */
} Setting_Safe_Exit_Value_T;

/**
 * @brief SettingDrvAssFctAvailabilityEnum from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 SettingDrvAssFctAvailabilityEnum (71)
 * If corresponding member is "AvailabilityDeactivated" this is used to grey out the setting "deactivate function". The
 * customer can not deactivate the function (but he can change the attribute defined in "value"). When this field/attribute is sent
 * by the client, the client shall use value Operable. However for robustness the server shall not evaluate this member, i.e. on
 * setting this member shall be discarded.
 */
typedef enum
{
   SETTING_DRV_ASS_FCT_AVAILABILITY_NOT_AVAILABLE = (0u), /**< If "Not_Available" is sent, the value list shall not be displayed in
                                                             the UI */
   SETTING_DRV_ASS_FCT_AVAILABILITY_NOT_OPERABLE = (1u),  /**< If "Not_Operable" is sent, the value list shall be displayed in the
                            UI but not be operable (e.g.  greyed out) */
   SETTING_DRV_ASS_FCT_AVAILABILITY_OPERABLE = (2u) /**< If "Operable" is sent (default) by the server, the value list shall be
                                                       displayed and operable.  */
} Setting_Drv_Ass_Fct_Availability_T;

/** @brief SettingActivityValueEnum from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 SettingActivityValueEnum (71) */
typedef enum
{
   SETTING_ACTIVITY_VALUE_NOT_CONFIGURABLE = (0u), /**< Do_not_use */
   SETTING_ACTIVITY_VALUE_DEACTIVATED      = (1u), /**< No value comment available in database */
   SETTING_ACTIVITY_VALUE_ACTIVATED        = (2u)  /**< No value comment available in database */
} Setting_Activity_Value_T;

/** @brief SeatOccupancyStatusStruct from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 SeatOccupancyStatusStruct (74) */
typedef struct
{
   Seat_Occupancy_T seat_occupancy;           /**< Position 0 - Index N/A - Signal N/A - Bitgap N/A - Base DataType Length 8 */
   uint8_t confidence_seat_occupancy;         /**< Position 1 - Index N/A - Signal N/A - Bitgap N/A - Base DataType Length  */
   Person_Age_Category_T person_age_category; /**< Position 2 - Index N/A - Signal N/A - Bitgap N/A - Base DataType Length 8 */
   Person_Entry_Exit_T person_entry_exit;     /**< Position 3 - Index N/A - Signal N/A - Bitgap N/A - Base DataType Length 8 */
   Qualifier_Container_T qualifier_container_seat_occupancy_status; /**< Position 4 - Index N/A - Signal N/A - Bitgap N/A -
                                                                         Base DataType Length 8 */

} Seat_Occupancy_Status_T;

/** @brief SeatLocationStruct from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 SeatLocationStruct (74) */
typedef struct
{
   Absolute_Seat_Location_T absolute_seat_location; /**< Position 0 - Index N/A - Signal N/A - Bitgap N/A - Base DataType Length
                                                          8 */
   Relative_Seat_Location_T relative_seat_location; /**< Position 1 - Index N/A - Signal N/A - Bitgap N/A - Base DataType Length
                                                          8 */

} Seat_Location_T;

/** @brief StatusBeltStruct from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 StatusBeltStruct (1) */
typedef struct
{
   Status_Belt_Buckle_Switch_T status_belt_buckle_switch; /**< Position 0 - Index N/A - Signal N/A - Bitgap N/A - Base DataType
                                                         Length 8 */
   Qualifier_Value_T qualifier_value_status_belt_buckle_switch; /**< Position 1 - Index N/A - Signal N/A - Bitgap N/A - Base
                                                              DataType Length 8 */

} Status_Belt_T;

/** @brief SeatStatusForEachSeatStruct from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 SeatStatusForEachSeatStruct (1) */
typedef struct
{
   Seat_Occupancy_Status_T seat_occupancy_status; /**< Position 0 - Index N/A - Signal N/A - Bitgap N/A - Base DataType Length */
   Seat_Location_T seat_location;                 /**< Position 1 - Index N/A - Signal N/A - Bitgap N/A - Base DataType Length  */
   Status_Belt_T status_belt; /**< Position N/A - Index N/A - Signal N/A - Bitgap N/A - Base DataType Length  */
} Seat_Status_For_Each_Seat_T;

/** @brief SettingSafeExitStruct from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 SettingSafeExitStruct (71) */
typedef struct
{
   Setting_Safe_Exit_Value_T value;                 /**< Position 0 - Index  - Signal  - Bitgap  - Base DataType Length 8 */
   Setting_Drv_Ass_Fct_Availability_T availability; /**< Position 1 - Index  - Signal  - Bitgap  - Base DataType Length 8 */
   Setting_Drv_Ass_Fct_Availability_T availability_deactivated; /**< Position 2 - Index  - Signal  - Bitgap  - Base DataType Length
                                                                  8 */
} Setting_Safe_Exit_T;

/** @brief SettingActivityStruct from BMW SK_EESystem25_22KW43_V12_Ethernet4_V12 SettingActivityStruct (71) */
typedef struct
{
   Setting_Activity_Value_T value;                  /**< Position 0 - Index  - Signal  - Bitgap  - Base DataType Length 8 */
   Setting_Drv_Ass_Fct_Availability_T availability; /**< Position 1 - Index  - Signal  - Bitgap  - Base DataType Length 8 */
} Setting_Activity_T;

/** @brief containing all the information needed for the output adoptation */
typedef struct
{
   float32_T ego_displayed_speed;
   Setting_Activity_T setting_safe_exit_sound;      /**< Setting information of EUF (End User Function) Safe Exit */
   Setting_Activity_T setting_safe_exit_delay_door; /**< Setting information of EUF (End User Function) Safe Exit */
   Setting_Safe_Exit_T setting_safe_exit;           /**< Setting information of EUF (End User Function) Safe Exit */
   Seat_Status_For_Each_Seat_T seat_status_for_each_seat[CED_BMW_NUMBER_OF_SEATS]; /**< No value comment available in database */
} Ced_Bmw_Boardnet_T;

/**
 * @brief Ced_Input_T structure.
 *
 *
 * @SRS{SF-56}
 * @SAE{SF-2407}
 * @SDD{SF-3429}
 */
typedef struct
{
   boolean_T f_ced_enable;     /**< Flag indicating the status of the CED function */
   boolean_T f_ced_front_mode; /**< Flag indicating the front mode is enabled */
   boolean_T f_ced_rear_mode;  /**< Flag indicating the rear mode is enabled */

   Bmw_Ced_Input_Bus_Signals_T ced_input_bus_signals;       /**< Contains all BMW specific input signals from the vehicle bus */
   Ced_Bmw_Boardnet_T bmw_boardnet_signals;                 /**< BMW SP25 boardnet signals */
   Ced_Calibration_Parameters_T ced_calibration_parameters; /**< BMW SP25 calibration parameters */
   Ced_Coding_Parameters_T ced_coding_parameters;           /**< BMW SP25 coding parameters */

} Ced_Input_T;


#endif /* CED_INPUT_T_H */

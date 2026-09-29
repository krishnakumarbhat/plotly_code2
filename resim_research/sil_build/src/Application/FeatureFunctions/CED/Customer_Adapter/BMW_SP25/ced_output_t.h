#ifndef CED_OUTPUT_T_H
#define CED_OUTPUT_T_H

/**
 * @file ced_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the BMW_SP25 output data structure for CED.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_bmw_sp25_types.h"
#include "ced_types.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/


/** @brief this struct contains the output for the occupant detection @SRS{n/a} */
typedef struct
{
   boolean_T f_occupant_left_front_not_wearing_seatbelt; /**<indicates whether there is a occupant on the front left seat who might
                                                          leave the ego vehicle */
   boolean_T f_occupant_right_front_not_wearing_seatbelt; /**<indicates whether there is a occupant on the front right seat who
                                                          might leave the ego vehicle */
   boolean_T f_occupant_left_rear_not_wearing_seatbelt;   /**<indicates whether there is a occupant on the rear left seat who might
                                                           leave the ego vehicle */
   boolean_T f_occupant_right_rear_not_wearing_seatbelt; /**<indicates whether there is a occupant on the rear right seat who might
                                                           leave the ego vehicle */
} Ced_Output_Occupant_Detection_T;


/** @brief this struct contains the output for the warning notifications  */
typedef struct
{
   boolean_T f_ced_output_occupant_not_wearing_seat_belt[CED_BMW_NUMBER_OF_DOORS];
   Ced_Target_Travel_Direction_T ced_output_warning_optical[CED_BMW_NUMBER_OF_DOORS];
   Ced_Mirror_Light_Warning_Type_T ced_output_warning_mirror[CED_BMW_NUMBER_OF_SIDES];
   Ced_Door_Warning_Levels_T ced_output_warning_ambient[CED_BMW_NUMBER_OF_DOORS];
   Ced_Target_Travel_Direction_T ced_output_warning_acoustic[CED_BMW_NUMBER_OF_DOORS];
   uint8_t ced_output_door_stop_automatic_opening[CED_BMW_NUMBER_OF_DOORS];
   uint8_t ced_output_door_lock_electronic[CED_BMW_NUMBER_OF_DOORS];
} Ced_Output_Warning_Indicators_T;


/**
 * @brief Ced_Output_T structure.
 *
 *
 * @SRS{SF-55}
 * @SAE{SF-2408}
 * @SDD{SF-3426}
 */
typedef struct
{
   /* BMW SP25 Safe Exit Outputs for State Machine and UDP Logging */
   uint8_t SFE_CED_Status;      /**Status of SFE*/
   uint8_t SFE_CED_alert_right; /**Alert indicator for left side (0 -> NO_WARNING, 1 -> LEVEL_1, 2 -> LEVEL_2)*/
   uint8_t SFE_CED_alert_left;  /**Alert indicator for right side (0 -> NO_WARNING, 1 -> LEVEL_1, 2 -> LEVEL_2)*/
   uint8_t SFE_CED_dir_right;   /**Direction indicator for right side for most severe object (Enum Sfe_Target_Travel_Direction_T)*/
   uint8_t SFE_CED_dir_left;    /**Direction indicator for left side for most severe object (Enum Sfe_Target_Travel_Direction_T)*/
   float32_T SFE_CED_ttc_left;  /**[s] TTC for most severe object on left side*/
   float32_T SFE_CED_ttc_right; /**[s] TTC for most severe object on right side*/
   float32_T SFE_CED_ttp_left;  /**[s] TTP for most severe object on left side*/
   float32_T SFE_CED_ttp_right; /**[s] TTP for most severe object on right side*/
   uint8_t SFE_CED_id_right;    /**Id of object with highest severity on right side*/
   uint8_t SFE_CED_id_left;     /**Id of object with highest severity on left side*/
   uint32_t SFE_CED_unique_id_right;        /**Unique id of object with highest severity on right side*/
   uint32_t SFE_CED_unique_id_left;         /**Unique id of object with highest severity on left side*/
   uint8_t SFE_CED_obj_type_right;          /**Type of object with highest severity on right side (Enum object_class_T)*/
   uint8_t SFE_CED_obj_type_left;           /**Type of object with highest severity on left side (Enum object_calss_T)*/
   float32_T SFE_CED_obj_speed_right;       /**[m/s] Speed of object with highest severity on right side*/
   float32_T SFE_CED_obj_speed_left;        /**[m/s] Speed of object with highest severity on left side*/
   float32_T SFE_CED_obj_heading_right;     /**[rad] Heading of object with highest severity on right side*/
   float32_T SFE_CED_obj_heading_left;      /**[rad] heading of object with highest severity on left side*/
   float32_T SFE_CED_obj_lateral_pos_right; /**[m] Lateral position of object with highest severity on right side*/
   float32_T SFE_CED_obj_lateral_pos_left;  /**[m] Lateral position of object with highest severity on left side*/
   float32_T SFE_CED_obj_long_pos_right;    /**[m] Longitudinal position of object with highest severity on right side*/
   float32_T SFE_CED_obj_long_pos_left;     /**[m] Longitudinal position of object with highest severity on left side*/

   /* pCAN Signals used for Debugging */
   uint8_t SFE_CED_rear_status;           /**Flag indicating the status of the CED rear function (PCAN Logging)*/
   uint8_t SFE_CED_rear_alert_right;      /**Warning level for the right side (0 -> NO_WARNING, 1 -> LEVEL_1, 2 -> LEVEL_2) (PCAN
                                             Logging)*/
   uint8_t SFE_CED_rear_alert_left;       /**Warning level for the left side (0 -> NO_WARNING, 1 -> LEVEL_1, 2 -> LEVEL_2) (PCAN
                                             Logging)*/
   uint8_t SFE_CED_rear_id_right;         /**ID of object with highest severity on right side (PCAN Logging)*/
   uint8_t SFE_CED_rear_id_left;          /**ID of object with highest severity on left side (PCAN Logging)*/
   uint8_t SFE_CED_rear_path_match_right; /**Flag indicating if path tracking information is used for warning on rear right (PCAN
                                             Logging)*/
   uint8_t SFE_CED_rear_path_match_left;  /**Flag indicating if path tracking information is used for warning on rear left (PCAN
                                             Logging)*/

   uint8_t SFE_CED_front_status;           /**Flag indicating the status of the CED front function (PCAN Logging)*/
   uint8_t SFE_CED_front_alert_right;      /**Warning level for the right side (0 -> NO_WARNING, 1 -> LEVEL_1, 2 -> LEVEL_2) (PCAN
                                              Logging)*/
   uint8_t SFE_CED_front_alert_left;       /**Warning level for the left side (0 -> NO_WARNING, 1 -> LEVEL_1, 2 -> LEVEL_2) (PCAN
                                              Logging)*/
   uint8_t SFE_CED_front_id_right;         /**ID of object with highest severity on right side (PCAN Logging)*/
   uint8_t SFE_CED_front_id_left;          /**ID of object with highest severity on left side (PCAN Logging)*/
   uint8_t SFE_CED_front_path_match_right; /**Flag indicating if path tracking information is used for warning on front right (PCAN
                                              Logging)*/
   uint8_t SFE_CED_front_path_match_left;  /**Flag indicating if path tracking information is used for warning on front left (PCAN
                                              Logging)*/

   float32_T SFE_CED_rear_ttc_right; /**[s] Time to collision (TTC) for most severe object on right side in seconds (PCAN Logging)*/
   float32_T SFE_CED_rear_ttc_left;  /**[s] Time to collision (TTC) for most severe object on left side in seconds (PCAN Logging)*/
   float32_T SFE_CED_rear_ttp_right; /**[s] Time to pass (TTP) for most severe object on right side in seconds (PCAN Logging)*/
   float32_T SFE_CED_rear_ttp_left;  /**[s] Time to pass (TTP) for most severe object on left side in seconds (PCAN Logging)*/
   float32_T SFE_CED_rear_lat_right; /**[m] Lateral intersection point for most severe object on right side (PCAN Logging)*/
   float32_T SFE_CED_rear_lat_left;  /**[m] Lateral intersection point for most severe object on left side (PCAN Logging)*/

   float32_T SFE_CED_front_ttc_right; /**[s] Time to collision (TTC) for most severe object on right side in seconds (PCAN
                                         Logging)*/
   float32_T SFE_CED_front_ttc_left; /**[s] Time to collision (TTC) for most severe object on left side in seconds (PCAN Logging)*/
   float32_T SFE_CED_front_ttp_right; /**[s] Time to pass (TTP) for most severe object on right side in seconds (PCAN Logging)*/
   float32_T SFE_CED_front_ttp_left;  /**[s] Time to pass(TTP) for most severe object on left side in seconds (PCAN Logging)*/
   float32_T SFE_CED_front_lat_right; /**[m] Lateral intersection point for most severe object on right side (PCAN Logging)*/
   float32_T SFE_CED_front_lat_left;  /**[m] Lateral intersection point for most severe object on left side (PCAN Logging)*/

   Bmw_Ced_Output_Bus_Signals_T ced_output_bus_signals;

   Ced_Output_Occupant_Detection_T ced_output_occupant_detection;

   Ced_Output_Warning_Indicators_T ced_output_warning_indicators;

} Ced_Output_T;


#endif /* CED_OUTPUT_T_H */

#ifndef TA_INPUT_T_H
#define TA_INPUT_T_H

/**
 * @file ta_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the input data structure for TA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* ta internal includes */
#include "ta_bmw_boardnet_t.h"
#include "ta_bmw_sp25_types.h"

/**
 * @brief BMW SRR5 specific TA input structure
 *
 * @SRS{SF-2276}
 * @SAE{SF-3240}
 * @SDD{SF-8600}
 */
typedef struct
{
   /* Turn-Assist (PFGS / TAP) generic content */
   Ta_BMW_Boardnet_T *bmw_boardnet_signals; /**< BMW bordnet signals TA is required to read in - Not used*/

   /* Front-Turn-Assist PFGS specific content */
   float32_T fta_obj_offset_x_positive; /**< Diagnostic Job: Shift object in +x direction - Range: 0 -> 5000 cm */
   float32_T fta_obj_offset_x_negative; /**< Diagnostic Job: Shift object in -x direction - Range: -5000 -> 0 cm */
   float32_T fta_obj_offset_y_positive; /**< Diagnostic Job: Shift object in +y direction - Range: 0 -> 1000 cm */
   float32_T fta_obj_offset_y_negative; /**< Diagnostic Job: Shift object in -y direction - Range: -1000 -> 0 cm */

   /* Rear-Turn-Assist TAP specific content */
   uint8_t f_rta_enable;              /**< enable/disable Rear Turn Assist RTA/TAP feature - Range: 0 -> 1 */
   uint8_t f_rta_enable_turning_area; /**< enable/disable RTA Turning area subfeature - Not used*/
   uint8_t f_rta_enable_dynamic_area; /**< enable/disable RTA Dynamic area subfeature - Not used*/

   /* Front-Turn-Assist PFGS specific content */
   uint8_t f_fta_enable; /**< enable/disable Front Turn Assist FTA/PFGS feature - Range: 0 -> 1 */

   uint8_t fta_steering_angle_max_left;  /**< Steering angle threshold to diff. between turn/straight - Range 0 -> 20 deg */
   uint8_t fta_steering_angle_max_right; /**< Steering angle threshold to diff. between turn/straight - Range 0 -> 20 deg */

   Bmw_TA_Input_Bus_Signals_T ta_input_signals; /* Contains all customer specific input signals from the vehicle bus */
   Ta_Coding_Parameters_T ta_coding_parameters; /* Contains all customer specific inputs from the coding parameter list */
} Ta_Input_T;

#endif /* TA_INPUT_T_H */

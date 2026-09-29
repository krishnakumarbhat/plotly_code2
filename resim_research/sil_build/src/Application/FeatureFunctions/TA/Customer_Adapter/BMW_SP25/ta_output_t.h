#ifndef TA_OUTPUT_T_H
#define TA_OUTPUT_T_H

/**
 * @file ta_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the output data structure for TA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* fbk includes */
#include "pa_reuse.h"
#include "ta_bmw_sp25_types.h"

#define TA_N_FTA_OBJECTS (8u)

typedef struct
{
   float32_T fta_obj_list_rcs;
   float32_T fta_obj_list_id;
   float32_T fta_obj_list_age;
   float32_T fta_obj_list_meas_status;
   float32_T fta_obj_list_move_status;
   float32_T fta_obj_list_exist_prob;
   float32_T fta_obj_list_ref_point;
   float32_T fta_obj_list_ref_pnt_long_posn;
   float32_T fta_obj_list_ref_pnt_long_posn_std_dev;
   float32_T fta_obj_list_ref_pnt_lat_posn;
   float32_T fta_obj_list_ref_pnt_lat_posn_std_dev;
   float32_T fta_obj_list_covariance_posn;
   float32_T fta_obj_list_yaw_angle;
   float32_T fta_obj_list_yaw_angle_std_dev;
   float32_T fta_obj_list_long_vel;
   float32_T fta_obj_list_long_vel_std_dev;
   float32_T fta_obj_list_lat_vel;
   float32_T fta_obj_list_lat_vel_std_dev;
   float32_T fta_obj_list_covariance_vel;
   float32_T fta_obj_list_long_accel;
   float32_T fta_obj_list_long_accel_std_dev;
   float32_T fta_obj_list_lat_accel;
   float32_T fta_obj_list_lat_accel_std_dev;
   float32_T fta_obj_list_covariance_accel;
   float32_T fta_obj_list_yawrate;
   float32_T fta_obj_list_yawrate_std_dev;
   float32_T fta_obj_list_length;
   float32_T fta_obj_list_length_std_dev;
   float32_T fta_obj_list_width;
   float32_T fta_obj_list_width_std_dev;
   float32_T fta_obj_list_object_class;
} FTA_Object_List_T;

/**
 * @brief BMW SRR5 specific TA output structure
 *
 * @SRS{SF-2277}
 * @SAE{SF-3241}
 * @SDD{SF-8601}
 */
typedef struct
{
   /* FTA Object list */
   FTA_Object_List_T fta_relevant_object[TA_N_FTA_OBJECTS];

   /* Front Turn Assist (FTA, also PFGS) */
   float32_T fta_brake_deceleration_request;
   float32_T fta_target_gap;
   float32_T fta_ttc;

   float32_T fta_target_age;        /* Not filled from core side */
   float32_T fta_target_vel_long;   /* Not filled from core side */
   float32_T fta_target_vel_lat;    /* Not filled from core side*/
   float32_T fta_target_exist_prob; /* Not filled from core side*/

   /* Rear Turn Assist (RTA, also TAP) - Tracker-Signals*/
   float32_T rta_long_posn_left;
   float32_T rta_lat_posn_left;
   float32_T rta_long_posn_right;
   float32_T rta_lat_posn_right;
   float32_T rta_ttc_left;
   float32_T rta_ttc_right;
   float32_T rta_long_vel_left;
   float32_T rta_lat_vel_left;
   float32_T rta_long_vel_right;
   float32_T rta_lat_vel_right;
   float32_T rta_existence_probability_left;
   float32_T rta_existence_probability_right;
   float32_T rta_right_object_width;
   float32_T rta_right_object_length;
   float32_T rta_left_object_width;
   float32_T rta_left_object_length;

   /* Front Turn Assist (FTA, also PFGS) */
   uint8_t fta_alert_level;
   uint8_t fta_symbol_request;
   uint8_t fta_brake_threshold_reduction;
   uint8_t fta_brake_conditioning;
   uint8_t fta_target_id;
   uint8_t fta_maneuver_direction; /* maneuver direction (TURN / STRAIGHT) */
   boolean_T f_diagnostic_mode;    /* flag indicates the activation of the BMW diagnostic mode */

   /* Rear Turn Assist (RTA, also TAP) */
   uint8_t rta_dynamic_area_status;
   uint8_t rta_turning_area_status;
   uint8_t rta_alert_left;
   uint8_t rta_alert_right;
   uint8_t rta_id_left;
   uint8_t rta_id_right;
   uint8_t rta_left_object_type;
   uint8_t rta_right_object_type;

   /* Enable flags */
   uint8_t f_fta_enable;              /* flag indicating status of Front Turn Assist (FTA, also PFGS) feature */
   uint8_t f_rta_enable;              /* flag indicating status of Rear Turn Assist (RTA, also TAP) feature */
   uint8_t f_rta_enable_turning_area; /* flag indicating status of RTA Turning area subfeature */
   uint8_t f_rta_enable_dynamic_area; /* flag indicating status of RTA Dynamic area subfeature */

   /* Debug signals */
   float32_T ta_current_deceleration_estimate; /* currently required deceleration of ego vehicle to avoid collision */
   /*Added for BMW SP25*/
   Bmw_TA_Output_Bus_Signals_T ta_output_bus_signal;
} Ta_Output_T;

#endif /* TA_OUTPUT_T_H */

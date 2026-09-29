#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H
/*===========================================================================*\
 * FILE: Event_Logger.h
 *===========================================================================
 * Copyright 2015 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
 *---------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the header files for RESIM Interface in VC++.
 *
 * ABBREVIATIONS:
 *   TODO: List of abbreviations used, or reference(s) to external document(s)
 *
 * TRACEABILITY INFO:
 *   Design Document(s):
 *     TODO: Update list of design document(s)
 *
 *   Requirements Document(s):
 *     TODO: Update list of requirements document(s)
 *
 *   Applicable Standards (in order of precedence: highest first):
 *     SW REF 264.15D "Delphi C Coding Standards" [12-Mar-2006]
 *
 * DEVIATIONS FROM STANDARDS:
 *   None.
 *
\*===========================================================================*/

#include "fixmac.h"
#include <vector>

#define MAX_OBJ_PROPERTIES_COUNT         10
#define MAX_RECW_OUTPUT_PROPERTIES_COUNT 5
#define MAX_CTA_OUTPUT_PROPERTIES_COUNT  5

#define MAX_ARRAY_FILE_PATH_LEN 4200

#ifdef SRR_DC
#define MAX_SENSORS 4
#elif MRR_DC
#define MAX_SENSORS 1
#endif

/*To be used to allow the RECU Debug files with this ranges*/
#ifdef _WIN32
#define MAX_FILE_PATH_LEN 255
#elif __GNUC__
#define MAX_FILE_PATH_LEN 4096
#endif

/* specific constants for SP2025 SRR tracker*/
#define NUMBER_OF_SENSORS (4)
#include <iostream>
#include <string>

typedef enum Statistic_Call_Tag {
   NONE,
   FEATURE_INPUT_DATA,
   FEATURE_OUTPUT_DATA,
   TRACKER_INPUT_DATA,
   TRACKER_OUTPUT_DATA
} Statistic_Call_T;

typedef enum CTA_Warn_Alert_Brake_Tag {
   CTA_Left_Warn,
   CTA_Right_Warn,
   CTA_Left_Alert,
   CTA_Right_Alert,
   CTA_Brake,
   Max_CTA_Warn_Alert_Status
} CTA_Warn_Alert_Brake_T;
typedef struct FF_alerts_warns_statistics_Tag {
   unsigned8_T ESA_left_id;
   unsigned8_T ESA_left_index;
   unsigned8_T ESA_right_id;
   unsigned8_T ESA_right_index;
   unsigned16_T ESA_right_alert;
   unsigned16_T ESA_left_alert;
   unsigned16_T ESA_left_alert_count;
   unsigned16_T ESA_right_alert_count;
   unsigned16_T LTB_Left_alert_count;
   unsigned16_T LTB_Right_alert_count;
   unsigned16_T LTB_Left_alert_level1_count;
   unsigned16_T LTB_Left_alert_level2_count;
   unsigned16_T LTB_Right_alert_level1_count;
   unsigned16_T LTB_Right_alert_level2_count;
   float32_T ESA_left_ttc_s;
   float32_T ESA_left_ttp_s;
   float32_T ESA_left_width_m;
   float32_T ESA_left_length_m;
   float32_T ESA_left_long_pos_m;
   float32_T ESA_left_lat_pos_m;
   float32_T ESA_left_long_speed_mps;
   float32_T ESA_left_lat_speed_mps;
   float32_T ESA_left_long_distance_m;
   float32_T ESA_left_existence_prob;
   float32_T ESA_right_ttc_s;
   float32_T ESA_right_ttp_s;
   float32_T ESA_right_width_m;
   float32_T ESA_right_length_m;
   float32_T ESA_right_long_pos_m;
   float32_T ESA_right_lat_pos_m;
   float32_T ESA_right_long_speed_mps;
   float32_T ESA_right_lat_speed_mps;
   float32_T ESA_right_long_distance_m;
   float32_T ESA_right_existence_prob;
} FF_alerts_warns_statistics_T;

typedef struct RECU_FF_alerts_warns_Tag {
   unsigned16_T CTA_left_alert_count;
   unsigned16_T CTA_right_alert_count;
   unsigned16_T CTA_left_warn_count;
   unsigned16_T CTA_right_warn_count;
   unsigned16_T RCTB_Brake_qualifier_count;
   unsigned16_T CED_left_alert_level1_count;
   unsigned16_T CED_left_alert_level2_count;
   unsigned16_T CED_right_alert_level1_count;
   unsigned16_T CED_right_alert_level2_count;
   unsigned16_T CED_FL_alert_level1_count;
   unsigned16_T CED_FL_alert_level2_count;
   unsigned16_T CED_FR_alert_level1_count;
   unsigned16_T CED_FR_alert_level2_count;
   unsigned16_T CED_RL_alert_level1_count;
   unsigned16_T CED_RL_alert_level2_count;
   unsigned16_T CED_RR_alert_level1_count;
   unsigned16_T CED_RR_alert_level2_count;
   unsigned16_T RECW_Acute_warn_count;
   unsigned16_T RECW_Alert_count;
   unsigned16_T lcda_bsw_Left_alert_Level1_count;
   unsigned16_T lcda_bsw_Left_alert_Level2_count;
   unsigned16_T lcda_bsw_Right_alert_Level1_count;
   unsigned16_T lcda_bsw_Right_alert_Level2_count;
   unsigned16_T lcda_cvw_Left_alert_Level1_count;
   unsigned16_T lcda_cvw_Left_alert_Level2_count;
   unsigned16_T lcda_cvw_Right_alert_Level1_count;
   unsigned16_T lcda_cvw_Right_alert_Level2_count;
   unsigned16_T lcda_slc_Left_alert_Level1_count;
   unsigned16_T lcda_slc_Left_alert_Level2_count;
   unsigned16_T lcda_slc_Right_alert_Level1_count;
   unsigned16_T lcda_slc_Right_alert_Level2_count;
   unsigned16_T scw_left_dyn_alert_count;
   unsigned16_T scw_left_Gaud_alert_count;
   unsigned16_T scw_right_dyn_alert_count;
   unsigned16_T scw_right_Gaud_alert_count;
   unsigned16_T TAP_left_alert_level1_count;
   unsigned16_T TAP_left_alert_level2_count;
   unsigned16_T TAP_right_alert_level1_count;
   unsigned16_T TAP_right_alert_level2_count;
   unsigned16_T rta_dynamic_area_info_left_count;
   unsigned16_T rta_dynamic_area_acute_left_count;
   unsigned16_T rta_dynamic_area_info_right_count;
   unsigned16_T rta_dynamic_area_acute_right_count;
   unsigned16_T rta_turning_area_info_left_count;
   unsigned16_T rta_turning_area_acute_left_count;
   unsigned16_T rta_turning_area_info_right_count;
   unsigned16_T rta_turning_area_acute_right_count;
   unsigned16_T PFGS_alert_level1_count;
   unsigned16_T PFGS_alert_level2_count;
   unsigned16_T PFGS_alert_level3_count;
   unsigned16_T PFGS_alert_level4_count;
   unsigned16_T PFGS_alert_level4_sequence_count;
   unsigned16_T fta_brake_deceleration_request_stage1_count;
   unsigned16_T fta_brake_deceleration_request_stage2_count;
   unsigned16_T fta_brake_deceleration_request_sequence_stage1_count;
   unsigned16_T fta_brake_deceleration_request_sequence_stage2_count;
   unsigned16_T fta_brake_deceleration_request_below_2_Count;
   unsigned16_T fta_brake_deceleration_request_below_4_Count;
   unsigned16_T fta_brake_deceleration_request_below_6_Count;
   unsigned16_T fta_brake_deceleration_request_below_10_Count;
   unsigned16_T fta_brake_deceleration_request_above_10_Count;
   unsigned16_T ff_overrun_count;
   unsigned16_T rdd_unset_count[MAX_SENSORS];
   unsigned16_T rdd_no_degradation_count[MAX_SENSORS];
   unsigned16_T rdd_degraded_count[MAX_SENSORS];
} DC_FF_alerts_warns_T;

typedef struct RECU_Statistic_Tracker_Tag {
   unsigned16_T unknownTrack_count;
   unsigned16_T pedestrainTrack_count;
   unsigned16_T TwoWheelTrack_count;
   unsigned16_T Car_count;
   unsigned16_T Truck_count;
   unsigned16_T Invalid_count;
   unsigned16_T totalTrackCount;
} DC_Statistic_Tracker_T;

typedef struct RECU_Statistic_VSE_Output_Tag {
   unsigned32_T filt_veh_speed_over_ground_qf_count;
   unsigned32_T comp_yaw_rate_qf_count;
   unsigned32_T raw_lat_accel_qf_count;
   unsigned32_T raw_long_accel_qf_count;
} DC_Statistic_VSE_Output_T;

typedef struct RECU_Statistic_Tracker_Diagnostic_Tag {
   unsigned32_T internal_tracker_error_fault_Present_Count;
} DC_Statistic_Tracker_Diagnostic_T;

typedef struct Vehicle_Speed_Data_Tag {
   unsigned16_T Zero_kph_speed_range_count;
   unsigned16_T Zero_to_Three_kph_speed_range_count;
   unsigned16_T Three_to_Five_kph_speed_range_count;
   unsigned16_T Five_to_Ten_kph_speed_range_count;
   unsigned16_T Ten_to_Fifteen_kph_speed_range_count;
   unsigned16_T Fifteen_to_Twety_kph_speed_range_count;
   unsigned16_T Twety_to_TwetyFive_kph_speed_range_count;
   unsigned16_T TwetyFive_to_thirty_kph_speed_range_count;
   unsigned16_T Thirty_to_ThirtyFive_kph_speed_range_count;
   unsigned16_T ThirtyFive_to_Fifty_kph_speed_range_count;
   unsigned16_T Fifty_to_Seventy_kph_speed_range_count;
   unsigned16_T Seventy_to_EightyFive_kph_speed_range_count;
   unsigned16_T EightyFive_to_OneHundred_kph_speed_range_count;
   unsigned16_T OneHundred_to_OneHundredFifty_kph_speed_range_count;
   unsigned16_T OneHundredFifty_to_TwoHundred_kph_speed_range_count;
   unsigned16_T Greater_Than_TwoHundred_kph_speed_range_count;
} Vehicle_Speed_Data_T;

typedef struct Vehicle_Dynamic_Data_Tag {
   float32_T min_vehspeed_m;
   float32_T max_vehspeed_m;
   float32_T avgSpeedLog_m;
   float32_T min_yawrate;
   float32_T max_yawrate;
   float32_T avg_yawrate;
   float32_T TotalDistLog_m;
   float32_T TotalDistLog_km;
   unsigned32_T inputcycle_count;
   unsigned16_T reverseGearcnt;
   unsigned16_T noreverseGearcnt;
   unsigned16_T invalid_gear;

} Vehicle_Dynamic_Data_T;

typedef struct PFGS_Tracker_Output_Tag {
   float32_T vcs_long_posn;
   float32_T vcs_lat_posn;
   float32_T vcs_long_vel;
   float32_T vcs_lat_vel;
   float32_T speed;
   float32_T vcs_long_accel;
   float32_T vcs_lat_accel;
   float32_T heading;
   float32_T probability_unknown;    //!< probability that the object class is unknown
   float32_T probability_pedestrian; //!< probability that the object is a pedestrian
   float32_T probability_2wheel;     //!< probability that the object is a 2wheel
   float32_T probability_car;        //!< probability that the object is a car
   float32_T probability_truck;      //!< probability that the object is a truck
   float32_T existence_probability;
   float32_T length; /**< [m] length of the target vehicle */
   float32_T width;  /**< [m] width of the target vehicle */
   float32_T eclipse_value;
   uint8_t f_stationary;
   uint8_t f_moveable;
   uint8_t object_class;
} PFGS_Tracker_Output_T;

typedef struct PFGS_Feature_Output_Tag {
   float32_T fta_ttc;
   float32_T fta_brake_deceleration_request;
   unsigned8_T fta_target_id;
   unsigned8_T fta_brake_conditioning;
} PFGS_Feature_Output_T;

typedef struct PFGS_Vehicle_Output_Tag {
   float32_T host_curvature_fast;
   float32_T vcs_long_vel;
   float32_T vcs_lat_vel;
   float32_T speed;
   float32_T vcs_long_acc;
   float32_T vcs_lat_acc;
   float32_T yawrate;
   float32_T steering_angle;
} PFGS_Vehicle_Output_T;

typedef struct PFGS_Object_Properties_Tag {
   PFGS_Tracker_Output_T PFGS_Traker_Output_Data;
   PFGS_Vehicle_Output_T PFGS_Vehicle_Output_Data;
   PFGS_Feature_Output_T PFGS_Feature_Output_Data;
} PFGS_Object_Properties_T;

typedef struct RECW_Output_Properties_Tag {
   float32_T Ego_speed;
   float32_T recw_ttc;
   float32_T recw_crash_probability;
   float32_T recw_obj_speed;
   float32_T recw_obj_long_pos;
   float32_T recw_obj_lat_pos;
   float32_T recw_obj_heading;
   float32_T recw_overlap;
   float32_T recw_ttc_warning_threshold;
} RECW_Output_Properties_T;

typedef struct CTA_Output_Properties_Tag {
   float32_T Ego_Speed;
   float32_T most_critical_object_ttc;
   float32_T Intersectionpoint_x_left;
   float32_T Intersectionpoint_x_right;
   float32_T RCTB_brake_request;
   int32_t cta_id_left;
   int32_t cta_id_right;
} CTA_Output_Properties_T;

typedef struct Snow_Blockage_Statistic_Tag {
   signed32_T Ip_f_SB_Blockage[NUMBER_OF_SENSORS];
   signed32_T Op_f_SB_Blockage[NUMBER_OF_SENSORS];
   signed32_T Ip_FP_Count[NUMBER_OF_SENSORS];
   signed32_T Op_FP_Count[NUMBER_OF_SENSORS];
} Snow_Blockage_Statistic_T;

typedef struct RECU_Statistic_Tag {
   Vehicle_Dynamic_Data_T Vehicle_Dynamic_Data;
   Vehicle_Dynamic_Data_T Overll_Vehicle_Dynamic_Data;
   DC_FF_alerts_warns_T Features_Input_Data;
   DC_FF_alerts_warns_T Features_Output_Data;
   DC_FF_alerts_warns_T Overall_Input_F_Count;
   DC_FF_alerts_warns_T Overall_Output_F_count;
   DC_Statistic_Tracker_T Tracker_Input_Data;
   DC_Statistic_Tracker_T Tracker_Output_Data;
   DC_Statistic_Tracker_T Overall_Input_T_Count;
   DC_Statistic_Tracker_T Overall_Output_T_Count;
   DC_Statistic_VSE_Output_T Vse_Output_Count;
   DC_Statistic_Tracker_Diagnostic_T Tracker_Diagnostic_Count;
   Vehicle_Speed_Data_T Vehicle_Speed;
   PFGS_Object_Properties_T PFGS_Object_Properties[MAX_OBJ_PROPERTIES_COUNT];
   RECW_Output_Properties_T RECW_Output_Properties[MAX_RECW_OUTPUT_PROPERTIES_COUNT];
   CTA_Output_Properties_T CTA_Output_Properties[Max_CTA_Warn_Alert_Status][MAX_CTA_OUTPUT_PROPERTIES_COUNT];
   Snow_Blockage_Statistic_T Snow_Blockage_Count;
} DC_Statistic_T;

void Write_statistic_Data(void *pLogFolder, bool endFile);
void Reset_Statstic_data();
void Run_Dia_Statistic();
FF_alerts_warns_statistics_T *Get_FF_alerts_warns_statistics_ptr();
void Update_Vehicle_Dynamic(Vehicle_Dynamic_Data_T *Vehicle_Dynamic_ptr);
void Load_Vehicle_Dyn_into_Statistic();
void ConvertToHr_sec_msee(unsigned32_T Speed_count, std::string *Speed_Range_in_hr_min_sec);
void Statistic_Tracker(DC_Statistic_Tracker_T *trackerPtr);
void Vse_Output(DC_Statistic_VSE_Output_T *vsePtr);
void Update_Feature_Statistic_Data(DC_FF_alerts_warns_T *Features_Count);
uint8_T Check_Alert_Status_changes_from_zero_to_one(unsigned8_T Alert_signal, unsigned8_T *Alert_Status_flag);
void Store_CTA_Output_properties_to_Statistic_Buff(CTA_Warn_Alert_Brake_T CTA_status);

#endif
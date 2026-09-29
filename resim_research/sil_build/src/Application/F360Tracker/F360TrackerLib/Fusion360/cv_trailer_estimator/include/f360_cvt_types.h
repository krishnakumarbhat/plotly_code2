#ifndef F360_CVT_TYPES_H
#define F360_CVT_TYPES_H
/******************************************************************************
 * Copyright 2024 Aptiv, All Rights Reserved.
 * Aptiv Confidential
 ******************************************************************************/
#include "f360_reuse.h"
#include "f360_variant_definition.h"

namespace f360_variant_A
{
   static const int32_t MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER = 40;
   static const int32_t CVT_MAX_NUMBER_OF_DETECTIONS = (MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER > static_cast<int32_t>(NUMBER_OF_SRR_DETECTIONS)) ? MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER : static_cast<int32_t>(NUMBER_OF_SRR_DETECTIONS);

   typedef enum F360_CVT_Filter_State_Tag
   {
      TRAILER_FILTER_STATE_NOT_STARTED = 0, // Default value
      TRAILER_FILTER_STATE_INIT,
      TRAILER_FILTER_STATE_ACTIVE,
   }F360_CVT_Filter_State_T;

   typedef enum F360_CVT_Model_Tag
   {
      TRAILER_MODEL_NOT_AVAILABLE = 0, // Default value
      TRAILER_MODEL_ONE_LINK,
      TRAILER_MODEL_TWO_LINK
   }F360_CVT_Model_T;

   /******************
   * Input data iface
   ******************/
   typedef struct F360_CVT_Detection_Info_Tag
   {
      float32_t range_rate;   // [m/s] Detection range rate
      float32_t vcs_latpos;   // [m]   lateral position
      float32_t vcs_longpos;  // [m]   longitudinal position
   }F360_CVT_Detection_Info_T;

   typedef struct F360_CVT_Input_Data_Tag
   {
      F360_CVT_Detection_Info_T detections[CVT_MAX_NUMBER_OF_DETECTIONS];
      int32_t n_detections;
      float32_t host_speed;                  // [m/s] Host speed over ground
      float32_t host_yawrate;                // [rad/s] Host yawrate
      float32_t host_side_slip_vcs;          // [rad] Host slip angle in 
      float32_t host_rear_axle_vcs_longpos;  // [m] Host center of rotation in VCS
      uint32_t tracker_index;
   }F360_CVT_Input_Data_T;

   /***************************
   * Initialization data iface
   ***************************/
   typedef struct F360_CVT_One_Link_Initialization_Data_Tag
   {
      int32_t n_updates;
      float32_t link1_angle;
      float32_t joint1_vcs_longpos;  // Position of the first joint in VCS
      float32_t link1_wheelbase;     // Distance from the joint to the trailer wheels for the first trailer link
      float32_t full_vehicle_length;
   }F360_CVT_One_Link_Initialization_Data_T;

   typedef struct F360_CVT_Two_Link_Initialization_Data_T
   {
      int32_t n_updates;
      float32_t link1_angle;
      float32_t link2_angle;
      float32_t joint1_vcs_longpos;  // Position of the first joint in VCS
      float32_t link1_wheelbase;     // Distance from the joint to the trailer wheels for the first trailer link
      float32_t link2_wheelbase;     // Distance from the joint to the trailer wheels for the first trailer link
      float32_t full_vehicle_length;
   }F360_CVT_Two_Link_Initialization_Data_T;

   typedef struct F360_CVT_Initialization_Data_Tag
   {
      float32_t host_length;  // [m] Host length
      float32_t host_width;   // [m] Host width
      float32_t host_rear_axle_vcs_longpos; //[m]
      float32_t radar_vcs_longpos; // [m] 
      float32_t radar_vcs_latpos; // [m] 
      uint32_t radar_id;

      float32_t joint1_vcs_longpos_min;  // Minimum position of the first joint in VCS
      float32_t joint1_vcs_longpos_max;  // Maximum position of the first joint in VCS
      float32_t link1_wheelbase_min;     // Minimum distance from the joint to the trailer wheels for the first trailer link
      float32_t link1_wheelbase_max;     // Maximum distance from the joint to the trailer wheels for the first trailer link
      float32_t link2_wheelbase_min;     // Minimum distance from the joint to the trailer wheels for the second trailer link
      float32_t link2_wheelbase_max;     // Maximum distance from the joint to the trailer wheels for the second trailer link

      F360_CVT_One_Link_Initialization_Data_T one_link_model;
      F360_CVT_Two_Link_Initialization_Data_T two_link_model;
   }F360_CVT_Initialization_Data_T;

   /******************
   * Internal CVT data
   *******************/
   typedef struct Trailer_Measurement_Info_Tag
   {
      int32_t selected_detids[MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER];
      int32_t n_selected_dets;                             // [-]   Number of valid detections used to estimate this line
      float32_t line_slope;                                // [-]   ransac output in vcs_longpos = slope * vcs_latpos + intersect, most visible line
      float32_t line_intersect;                            // [-]   ransac output in vcs_longpos = slope * vcs_latpos + intersect, most visible line
      float32_t trailer_angle_vcs;                         // [rad] Line through center of trailer
      float32_t trailer_intersect_vcs_long;                // [m]   Line through center of trailer
      bool f_line_valid;                                   // Flag indicating that a line was found
      bool f_msmt_valid;                                   // Flag indicating succesful trailer measurement
   }Trailer_Measurement_Info_T;

   typedef struct F360_CVT_One_Link_Trailer_State_Tag
   {
      F360_CVT_Filter_State_T filter_state;
      int32_t n_updates;
      float32_t joint_vcs_latpos;
      float32_t joint_vcs_longpos;
      float32_t joint_dist_to_center;
      float32_t joint_dist_to_wheels;
      float32_t full_vehicle_length;
      float32_t trailer_length;
      float32_t trailer_width;
      float32_t longvel;
      float32_t angle_rate;
      float32_t ekf_state[3]; // 0: trailer angle, 1: rear axle to joint (a1), 2: joint2wheels (b2)
      float32_t ekf_state_errcov[3][3];
      float32_t prev_trailer_angle;
   } F360_CVT_One_Link_Trailer_State_T;

   typedef struct F360_CVT_Two_Link_Trailer_State_Tag
   {
      F360_CVT_Filter_State_T filter_state;
      int32_t n_updates;
      float32_t joint1_vcs_latpos;
      float32_t joint1_vcs_longpos;
      float32_t joint1_dist_to_center;
      float32_t joint1_dist_to_wheels;
      float32_t joint2_vcs_latpos;
      float32_t joint2_vcs_longpos;
      float32_t joint2_dist_to_center;
      float32_t joint2_dist_to_wheels;
      float32_t full_vehicle_length;
      float32_t trailer1_length;
      float32_t trailer1_width;
      float32_t trailer2_length;
      float32_t trailer2_width;
      float32_t longvel1;
      float32_t longvel2;
      float32_t angle_rate1;
      float32_t angle_rate2;
      float32_t ekf_state[5];  // 0: trailer angle vcs, 1: rear axle to joint (a1), 2: joint2wheels (b2), 3: trailer angle vcs , 4: joint2wheels (b3)
      float32_t ekf_state_errcov[5][5];
      float32_t prev_trailer1_angle;
      float32_t prev_trailer2_angle;
      float32_t confidence;
   } F360_CVT_Two_Link_Trailer_State_T;

   typedef struct F360_CVT_State_Constraints_Tag
   {
      float32_t max_x0;    // Maximum constraint of the first joint angle
      float32_t min_x0;    // Minimum constraint of the first joint angle
      float32_t max_x1;    // Maximum constraint of the distance to the first joint
      float32_t min_x1;    // Minimum constraint of the distance to the first joint
      float32_t max_x2;    // Maximum constraint of the distance from the first joint to the second.
      float32_t min_x2;    // Minimum constraint of the distance from the first joint to the second.
      float32_t max_x3;    // Maximum constraint of the second joint angle
      float32_t min_x3;    // Minimum constraint of the second joint angle
      float32_t max_x4;    // Maximum constraint of the distance from the second joint to the trailer wheelbase.
      float32_t min_x4;    // Minimum constraint of the distance from the second joint to the trailer wheelbase.
   }F360_CVT_State_Constraints_T;

   typedef struct F360_CVT_State_Tag
   {
      Trailer_Measurement_Info_T primary_measurement;
      Trailer_Measurement_Info_T secondary_measurement;
      F360_CVT_State_Constraints_T state_constraints;
      F360_CVT_One_Link_Trailer_State_T one_link;
      F360_CVT_Two_Link_Trailer_State_T two_link;
      int32_t valid_det_idx[MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER];
      int32_t n_valid_dets;
      float32_t saturated_odometer_reversing_countermeasures;  // [m] Saturated bidirectional odometer for reversing countermeasures hysteresis Range: [-2.5m, +50.0m]. Resets on direction change. Triggers countermeasures at -2.5m, releases at +50.0m
      float32_t radar_vcs_longpos;
      float32_t radar_vcs_latpos;
      float32_t host_width;
      float32_t host_length;
      uint32_t radar_id;
      uint16_t best_model_change_counter;
      F360_CVT_Model_T best_trailer_model;
      bool f_init_complete;
      bool f_initiated_from_states;
      bool f_reversing_countermeasures_active;
   } F360_CVT_State_T;
}
#endif

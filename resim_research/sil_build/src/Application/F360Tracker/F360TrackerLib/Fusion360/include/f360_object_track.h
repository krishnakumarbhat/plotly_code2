/*===================================================================================*\
* FILE: f360_object_track.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains F360_Object_Track_T structure declaration
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_OBJECT_TRACK_H
#define F360_OBJECT_TRACK_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_velocity.h"
#include "f360_point.h"
#include "f360_accel.h"
#include "f360_object_status.h"
#include "f360_occlusion_types.h"
#include "f360_look_type.h"
#include "f360_track_init.h"
#include "f360_trk_fltr_type.h"
#include "f360_aeb_confidence.h"
#include "f360_object_class.h"
#include "f360_conf.h"
#include "f360_math.h"
#include "f360_dead_zone_status.h"
#include "f360_angle.h"
#include "f360_bounding_box.h"
#include "f360_reference_point.h"

#include "ocg_underdrivability_enum.h"
#include "sg_drivability_class.h"

namespace f360_variant_A
{
   struct F360_Object_Track_T
   {
      // Longitudinal Static Curves (LSC)
      F360_Object_Track_T* lsc_next_in_cluster; // Pointer to the next VCS-longitudinal sorted object in a certain LSC cluster
      F360_Object_Track_T* lsc_prev_in_cluster; // Pointer to the previous VCS-longitudinal sorted object in a certain LSC cluster

      F360_Object_Track_T* p_higher_priority_track;      // pointer to the track with next greater priority [-]
      F360_Object_Track_T* p_lower_priority_track;       // pointer to the track with next lower priority [-]

      Point pseudo_vcs_position;  // position with pseudo-measurement for measurement update in VCS [m]
      float32_t prev_predicted_vcs_y_pos; // Time updated VCS Y position of object in previous tracker iteratation. Only used for CCA non moveable objects
      float32_t speed;                   // object Over-The-Ground (OTG) [m/s]; estimation point at object rear center in CTCA model
      float32_t predicted_speed;         // predicted object Over-The-Ground (OTG) [m/s]
      float32_t hdg_ptng_disagmt;        // disagreement between heading angle and pointing angle [rad]
      float32_t curvature;               // object movement curvature [1/m]; estimation point at object rear center in CTCA model
      float32_t heading_rate;               // object heading rate [rad/s]; 
      float32_t tang_accel;              // object tangential acceleration [m/s^2]; estimation point at object rear center in CTCA model
      Point vcs_position;   // object position in VCS [m]
      Point predicted_vcs_position; // predicted object position in VCS [m]
      Point average_grid_search_tcs_position; // Average Pseudo pos grid serach tcs position. The average is over 5 scans
      float32_t otg_height; // over the ground height mean of the object, accumulated over object's lifetime [m]
      float32_t otg_height_raw;
      F360_VCS_Velocity_T vcs_velocity;   // object velocity in VCS [m/s]; estimation point at object rear center in CTCA model
      F360_VCS_Velocity_T predicted_vcs_velocity; // predicted object velocity in VCS [m/s]
      F360_VCS_Accel_T vcs_accel;         // object acceleration in VCS [m/s^2]
      Angle vcs_heading;             // object velocity heading in VCS [rad]; estimation point at object rear center in CTCA model
      BoundingBox bbox;                   // bounding box - stores position of center, pointing angle, length and width
      float32_t length_processed;         // object length that is outputed by the tracker (don't have to be same as bbox.length)
      float32_t width_processed;          // object width that is outputed by the tracker (don't have to be same as bbox.width)
      float32_t orientation_std;          // orientation std [rad]
      float32_t time_since_cluster_created;  // time since creation of a cluster, from which the track is derived (based on tracker execution time) [s]
      float32_t time_since_track_updated;    // time since latest detection associated to the object (based on tracker execution time) [s]
      float32_t time_since_downselected;    // time since downselected [s]
      float32_t time_since_split;            // time since object was involved in a split [s]
      float32_t errcov[STATE_DIMENSION][STATE_DIMENSION]; // error covariance matrix for current state representation [multiple]
      float32_t cca_pnt_filter_cov[2][2]; // covariance matrix used in object cca pointing filter
      uint32_t ndets;                 // number of detections associated to object [-]
      uint32_t detids[MAX_DETS_IN_OBJ_TRK]; // list of detections identifiers associated to the object [-]
      uint32_t num_rr_inlier_dets;              // number of associated detections which range-rate closely matches the predicted range-rate for the object [-]
      uint32_t num_dets_used_in_rr_msmt_update; // number of reduced detections which was used in the measurement rr update [-]
      uint32_t slow_moving_cluster_id;          // id of slow moving cluster the object is part of, 0 if part of no cluster [-]
      uint32_t num_members_in_slow_moving_obj_cluster; // number of objects in the slow moving cluster the object is part of, including the object itself [-]
      float32_t length_of_slow_moving_obj_cluster; // [m] Difference between max and min cordinates (in average heading direction) of corners of bboxes for objects in the slow moving cluster

      float32_t number_of_events_of_multipath_with_forgetting_factor; // number of multipath signal on object for entire time with forgetting alpha factor [-]
      float32_t time_since_initialization_with_forgetting_factor; // time since initalization with forgetting alpha factor [s]

      // Underdrivability for moving objects
      float32_t  ud_mov_historic_ndets;  // total number of detections used to calculate historic height mean, associated to the object over time [-]
      uint32_t ud_mov_cnt_underdrivable;  // number of scans during which object historic height mean was above the threshold determining ud status [-]
      float32_t probability_underdrivable_ocg; // Probability (0-1) that the object is underdrivable [-]
      float32_t ud_overdrivable_det_pct;  // percentage of detections that are overdrivable (have positive z position which mean that they are below the host) [-]

      float32_t mirror_prob;            // Probability (0-1) that this track is a mirror of multi path detections (where mirror is guardrail or other object) [-]
      float32_t filtered_combined_nosep_mirror_prob; // Probability (0-1) that this object is a mirror of host, where mirror is not SEP or other object [-]
      float32_t historic_num_db_dets_with_forgetting_factor; // Number of historic double bounce detections associated to this object with forgetting factor [-]
      uint8_t num_db_dets;               // Number of double bounce detections associated ot this object [-]
      float32_t length_uncertainty;      // Uncertainty scalar for updating object size in length direction [m]
      float32_t width_uncertainty;       // Uncertainty scalar for updating object size in width direction [m]
      int32_t id;                    // object track identifier (id = index_in_array + 1; ) [-]
      uint32_t unique_id;             // unique object track identifier [-]
      int32_t reduced_id;            // down-selected tracked object identifier (0 = invalid object) [-]
      int32_t cntConsecutiveAmbiguous; // counter of consecutive tracked object update by ambiguous object motion status (incremented once per tracker cycle) [tracker execution cycles]
      int32_t cntConsecutiveMoving;    // counter of consecutive tracked object update by moving object motion status (incremented once per tracker cycle) [tracker execution cycles]
      int32_t cntConsecutiveStopped;   // counter of consecutive tracked object update by stopped object motion status (incremented once per tracker cycle) [tracker execution cycles]
      int32_t cntHostTurnForMirrorProb; // Counter of host in straight driving and there is a guardrail between host and the object, which was flagged as mirror (incremented once per tracker cycle) [tracker execution cycles]
      float32_t raw_confidence_level;    // instantaneous object confidence level (based on current measurement) [-], range: <0, 1>
      float32_t confidenceLevel;         // overall confidence that object state is valid [-], range: <0, 1>
      float32_t prev_avrg_conf_level;    // average confidence level calculated based on previous and current confidence  [-]
      float32_t time_since_stage_start;        // tracker time since last change in track status [s]
      int32_t num_types_of_dets[2];  // number of detections associated to the track (split by detection motion status: 0 - moving, 1 - other) [-]
      float32_t meascov[F360_PSEUDO_MEAS_DIM][F360_PSEUDO_MEAS_DIM]; // track measurement covariance for x, y pseudo position [m^2]
      float32_t long_buffer_zone_len1;    // length dimension added to len1 of the bounding box for detection association [m]
      float32_t long_buffer_zone_len2;    // length dimension added to the len2 of the bounding box for detection association [m]
      float32_t lat_buffer_zone_wid1;     // lateral dimension added to wid1 bounding box for detection association [m]
      float32_t lat_buffer_zone_wid2;     // lateral dimension added to wid2 bounding box for detection association [m]
      float32_t time_since_initialization;     // time since object initialization (based on tracker execution time) [s]
      float32_t time_since_last_stop;    // time since last time the object comes to a stop from moving motion classification [s]
      float32_t time_since_started_move; // time since object started to move [s]
      uint32_t total_reduced_dets;    // number of down-selected detection associated to the track through its lifespan [-]
      float32_t filtered_dets;           // filtered over the time number of detection (can be fraction) [-]
      float32_t time_since_measurement;  // Time since object was updated [s]
      float32_t priority;                // priority used for object vs clusters prioritization during new object initialization when object list is saturated, range <0, 1> (higher priority means that object is more important) [-]
      float32_t bbox_center_otg_altitude;// estimation of bbox center altitude for obstacle probability calculation [m]
      float32_t bbox_height;             // estimation of bbox height for obstacle probability calculation [m]
      float32_t obstacle_prob;           // probability that object is an obstacle  - cannot be drived over or under

      // existance probability filter
      float32_t exist_prob;              // object existence probability [-], range: <0, 1>
      float32_t p_track_state;           // probability of precise estimation of object state, range: <0, 1>

      // Object class probability vectors
      float32_t probability_pedestrian;  // probability that object is a pedestrian [-]
      float32_t probability_car;         // probability that object is a car [-]
      float32_t probability_bicycle;     // probability that object is a bicycle [-]
      float32_t probability_motorcycle;   // probability that object is a motorcycle [-]
      float32_t probability_truck;       // probability that object is a track [-]
      float32_t probability_undet;       // probability that object is a grounded UFO [-]

      float32_t time_since_obj_considered_veh_for_class_freeze; // time since object was lastly considered as a vehicle for non-VRU class freeze logic [s]

      float32_t movable_prob;     // probability indicating how likely an object is movable [0.0, 1.0]

      // To Select Detections To Mark As Inliers
      float32_t filtered_hist_assoc_det_rr_err_mean; // adjusted mean difference between speeds of a detection and and object it is assigned to, accumulated over object's lifetime.
      float32_t filtered_hist_assoc_det_rr_err_var; // adjusted variance difference between speeds of a detection and and object it is assigned to, accumulated over object's lifetime.
      float32_t filtered_hist_assoc_n_dets; // adjusted total number of detections associated to the object over time

      float32_t average_rcs; // low-pass filtered RCS value based on average rcs of associated detections [dB/m^2] 
      float32_t maximum_rcs; // filtered maximum RCS value based on associated detections [dB/m^2] 

      // Split logic signals
      float32_t orth_delta_filtered; // Low pass filtered orthogonal delta distance between detections on object (maximum orth distance between all detections) [m]
      float32_t orth_gap_filtered; // Low pass filtered orthogonal distance gap between detections on object (maximum orth gap between two detections). Note that gap <= delta based on its definition [m]
      float32_t orth_range_rate_diff_filtered; // Low pass filtered difference between means of range rates of detections on left and right side of an object. Note that it needs to be signed to work properly [m]
      Point prev_vcs_center_pos; // VCS position of object center position in previous tracker iteratation. Only used for CTCA objects. Transformed to current VCS in time update module [m]
      float32_t filtered_pos_diff_heading; // Low pass filtered VCS heading solely based on object centroid position delta between tracker iterations [rad]. Only used for CTCA objects.
      float32_t filtered_rr_err_max_gap; // Low pass filtered max gap between range rate errors of detections associated to the object. [m/s]
      float32_t filtered_mean_tcs_y_pos_of_lower_rr_err_bin; // Low pass filtered mean of tcs y pos of detections from lower range rate error bin. [m] 
      float32_t filtered_mean_tcs_y_pos_of_higher_rr_err_bin; // Low pass filtered mean of tcs y pos of detections from higher range rate error bin. [m] 
      uint8_t split_type; // Type of split that is to be performed on the object. [-]

      int32_t pseudo_pos_cov_outlier_count_orth; // Number of consecutive pseudo position outliers in orth direction. Positive or negative value depending on which direction object is being pulled [-]

      CONF9_T conf_longitudinal_position; // Internal state confidence of vcs_position longitudinal used for determining overall confidence
      CONF9_T conf_lateral_position;      // Internal state confidence of vcs_position lateral used for determining overall confidence
      CONF9_T conf_speed;                 // Internal state confidence of speed used for determining overall confidence
      CONF3_T conf_overall;               // Overall state confidence based on "smoothness" of all states

      F360_Object_Status_T status;              // object lifespan status [-]
      F360_Occlusion_Status_T occlusion_status; // occlusion status of various points on the object
      F360_Track_Init_T init_scheme;            // object initialization method [-]
      F360_Object_Status_T reduced_status; // down-selected tracked object lifespan status [-]
      F360_Trk_Fltr_Type_T trk_fltr_type;  // track motion filter method [-]
      F360_AEB_Confidence_T aeb_confidence; // Radar-only AEB confidence signal [-]
      F360_Reference_Point_T reference_point; // Enumeration of which object point (corners, side midpoints or center) that is most likely seen from center of host.
      F360_Reference_Point_T min_projection_reference_point; // Enumeration of which object point (corners, side midpoints or center) that is most likely seen from center of host regardless of visiblity
      F360_Object_Class_T object_class; // object class [-]
      F360_Dead_Zone_Status_T dead_zone_status; // Enumeration identifying whether object is in left or right dead zone
      
      // Underdrivibility class
      ocg::OCG_Underdrivable_Status_T underdrivable_status_ocg; // enum identifying whether host can pass under track
      sg::SG_Drivability_Class_T drivable_status_sg; // enum identifying whether host can pass under track

      bool f_moving;              // flag indicating that object is moving [-]
      bool f_oncoming;            // flag indicating that object is oncoming to the host [-]
      bool f_hide_occluded_track_behind_host; // flag indicating that object is occluded by another object behind host (likely a multipath ghost) [-]
      bool f_vehicular_trk;       // flag indicating that object is a vehicle [-]
      bool f_ghost_NU_2_C;        // flag indicating change of object status from NEW to COASTED [-]
      bool f_overlapping_with_object; // flag indicating that object bounding box overlaps with different bounding box [-]
      bool f_prevent_orientation_std_decrease;          // flag indicating that object is in orientation turbulent period after merge, used by orientation variance
      bool f_changed_direction_after_start; // flag indicating whether object changed direction when it started moving
      bool f_shrink_fast; // flag indicating that the length filter became faster for shrinkage 
      bool f_suspectable_for_det_drop; // flag indicating whether object is suspectable for detection drop [-]

      bool f_moveable;            // flag indicating that object is ever seen moving (cannot be reset and intend for external usage)  [-]
      int8_t direction_before_stopped; // Integer indicating whether object was driving in reverse before it came to stop
      int8_t cnt_consecutive_visible_from_rear; // Counter of consecutive scans where the object is visible to rear sensors. It is in conjunction with f_hide_occluded_track_behind_host

      // Static Environment Polynomials
      bool f_behind_sep_ambiguous; // Flag indicating that the object is partially behind an SEP
      uint8_t behind_sep_id; // Id of highest prioritized SEP that this object is behind. Tracks that are "on" will never be flagged as "behind". Only moving object are considered, all other tracks will have behind_sep_id = F360_INVALID_UNSIGNED_ID by default.
      uint8_t on_sep_id;  // Id of highest prioritized SEP which this object is "on".
      Point sep_intersection_point; // Point where a straight line from VCS origin to the object centroid intersects the closest SEP 
      
      uint8_t num_updates_since_init; // number of times an upbject has been KF measurement updated since it was first born
      uint8_t low_rcs_dets_cnt; // Counter for identifying suspicious low rcs tracks is close proximity to host that should not be downselected. Since the track will be initialied close to host we start collecting information already in the cluster stages. The counter can only be set while the cluster and subsequent merged clusters have only a single associated detection per tracker iteration.

      uint8_t cca_cross_moving_buffer_index; // Index used for cca_cross_moving_buffer
      uint8_t drivable_confidence_sg; // Confidence in the SG underdrivable status from 0 to 100 [-]
      float32_t drivable_sg_dist_to_segment_sq; // [m^2] squared distance to the SG segment that assigned current drivable_status_sg
      int8_t cca_cross_moving_buffer[F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE];    // buffer that records the sign of change in vcs position for non moveable objects

      float32_t pseudo_hdg_state_vec[6]; // pseudo heading based on detection trail
      float32_t pseudo_hdg; // pseudo heading state vector

      float32_t assoc_dets_pct_filtered; // percentage of associated detections within the extended bbox of an object [-]
      uint16_t num_dets_in_ext_bbox; // number of all the detections (including detections that are not associated) in object's extended bbox [-]

      float32_t idm_det_fraction; // fraction of IDM detections associated to the object [-]

      void Update_Bbox_Size(const float32_t &length, const float32_t &width); // Update bbox to be aligned with size related signals
      void Set_Bbox_Orientation(const Angle& new_orientation);    // Set new orientation of bbox
      void Update_Bbox_Center();    // Recalculate bbox center to be aligned with current state of track

   };
}
#endif

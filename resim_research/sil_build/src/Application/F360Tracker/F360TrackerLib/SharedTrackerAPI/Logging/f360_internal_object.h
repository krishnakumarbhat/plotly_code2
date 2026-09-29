#ifndef F360_INTERNAL_OBJECT_H
#define F360_INTERNAL_OBJECT_H

#include "f360_reuse.h"

// Add pragmas to throw error if struct is padded
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(error : 4820)
#elif 0
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wpadded"
#endif

static const int32_t F360_Internal_Object_Stream_Num = 150;
static const int32_t F360_Internal_Object_Stream_Ver = 35;
static const int32_t F360_Internal_Object_Max_Payload_Size = 2000;

typedef struct F360_Internal_Object_Tag
{
   float other_state_covariance[12];       /* states in error cov that is not part of logged state_variance[6] or supplemental_state_covariance[3]. Note: For stream version 11 and above tracking is done in VCS instead of in WCS */
   float orth_delta_filtered;              /* low pass filtered max orthogonal delta of associated detections */
   float orth_gap_filtered;                /* low pass filtered max orthogonal gap of associated detections */
   float orth_range_rate_diff_filtered;		   /* low pass filtered range-rate difference between left and right detections */
   float filtered_pos_diff_heading;        /* low pass filtered heading solely based on position delta between tracker iterations */
   float time_since_initialization;        /* time since object initialization (based on tracker execution time) [s] */
   float time_since_last_stop;             /* time since last time the object comes to a stop from moving motion classification [s] */
   float filtered_dets;                    /* filtered over the time number of detection (can be fraction) [-] */
   float prev_avrg_conf_level;             /* average confidence level calculated based on previous and current confidence  [-] */
   float length_uncertainty;
   float width_uncertainty;
   float mirror_prob;                      /* Probability (0-1) that this track is a mirror of multi path detections (where mirror is guardrail or other object) [-] */
   float filtered_combined_nosep_mirror_prob; /* Probability(0 - 1) that this object is a mirror of host, where mirror is not SEP or other object[-] */
   float historic_num_db_dets_with_forgetting_factor; /* Number of historic double bounce detections associated to this object with forgetting factor[-] */
   float average_rcs;                      /* low-pass filtered RCS value based on average rcs of associated detections [dB/m^2] */
   float maximum_rcs;                      /* filtered max rcs value */
   float hdg_ptng_disagmt;                 /* filtered difference between heading and pointing angles */
   float cca_pnt_filter_cov[3];            /*  covariance matrix used in object cca pointing filter */
   float filtered_hist_assoc_det_rr_err_mean; /* adjusted mean difference between speeds of a detectionandand object it is assigned to, accumulated over object's lifetime. */
   float filtered_hist_assoc_det_rr_err_var;  /* adjusted variance difference between speeds of a detection and and object it is assigned to, accumulated over object's lifetime. */
   float filtered_hist_assoc_n_dets;          /* adjusted total number of detections associated to the object over time */
   float ud_mov_historic_height_mean;      /* height mean of the object, accumulated over object's lifetime. The height mean is weighted with forgetting factor and decays by 3% if there are no detections assigned to the object [m] */
   float ud_mov_historic_ndets;            /* total number of detections used to calculate historic height mean, associated to the object over time[-] */
   float ud_overdrivable_det_pct;          /* percentage of detections that are overdrivable (have positive z position which mean that they are below the host) [-] */
   float time_since_split;                 /* time since object was involved in a split[s] */
   float prev_predicted_vcs_y_pos;         /* Time updated VCS Y position of object in previous tracker iteratation. Only used for CCA non moveable objects */
   float orientation_std;                  /* orientation variance [rad] */
   float number_of_events_of_multipath_with_forgetting_factor; // number of multipath signal on object for entire time with forgeting alpha factor [-]
   float time_since_initialization_with_forgetting_factor; // time since initalization with forgeting alpha factor [s]
   float bbox_width;                      // width of internal bounding box
   float bbox_length;                     // length of internal bounding box
   float time_since_started_move;          // time since object started to move [s]
   float otg_height_raw;                   /* over the ground height */
   float pseudo_hdg;                       // pseudo heading based on detection trail
   float pseudo_hdg_state_vec[6];          // pseudo heading state vector
   float assoc_dets_pct_filtered;          // percentage of associated detections within the extended bbox of an object [-]
   float bbox_center_otg_altitude;         // estimation of bbox center altitude for obstacle probability calculation [m]
   float bbox_height;                      // estimation of bbox height for obstacle probability calculation [m]
   float filtered_rr_err_max_gap;         // Low pass filtered max gap between range rate errors of detections associated to the object. [m/s]
   float filtered_mean_tcs_y_pos_of_lower_rr_err_bin; // Low pass filtered mean of tcs y pos of detections from lower range rate error bin. [m]
   float filtered_mean_tcs_y_pos_of_higher_rr_err_bin; // Low pass filtered mean of tcs y pos of detections from higher range rate error bin. [m]
   float time_since_obj_considered_veh_for_class_freeze; // time since object was lastly considered as a vehicle for non-VRU class freeze logic [s]
   float predicted_vcs_position_y;         // predicted VCS Y position of the object
   int32_t cntConsecutiveAmbiguous;        /* counter of consecutive tracked object update by ambiguous object motion status (incremented once per tracker cycle) [tracker execution cycles] */
   int32_t cntConsecutiveMoving;           /* counter of consecutive tracked object update by moving object motion status (incremented once per tracker cycle) [tracker execution cycles] */
   int32_t cntConsecutiveStopped;          /* counter of consecutive tracked object update by stopped object motion status (incremented once per tracker cycle) [tracker execution cycles] */
   int32_t cntHostTurnForMirrorProb;       /* Counter of host not in straight driving and there is a guardrail between host and the object (incremented once per tracker cycle) [tracker execution cycles] */
   int32_t total_reduced_dets;             /* number of down-selected detection associated to the track through its lifespan [-] */
   int32_t pseudo_pos_cov_outlier_count_orth; /* Number of consecutive pseudo position outliers in orth direction. Positive or negative value depending on which direction object is being pulled [-] */
   uint32_t ud_mov_cnt_underdrivable;      /* number of scans during which object historic height mean was above the threshold determining ud status[-] */
   uint16_t id;
   uint8_t num_updates_since_init;         /* number of times an upbject has been KF measurement updated since it was first born */
   uint8_t min_projection_reference_point; /* Enumeration of which object point (corners, side midpoints or center) that is most likely seen from center of host regardless of visiblity */
   uint8_t behind_sep_id;                  /* id of highest prioritized SEP that this object is behind. Tracks that are "on" will never be flagged as "behind". Only CTCA and fast moving CCA tracks are flagged as behind whereas slow moving CCA tracks have behind_lsc_id = F360_INVALID_UNSIGNED_ID by default */
   uint8_t on_sep_id;                      /* id of highest prioritized SEP which this object is "on" */
   uint8_t conf_longitudinal_position;     /* [CONF9_T] Internal state confidence of vcs_position longitudinal used for determining overall confidence */
   uint8_t conf_lateral_position;          /* [CONF9_T] Internal state confidence of vcs_position lateral used for determining overall confidence */
   uint8_t conf_speed;                     /* [CONF9_T] Internal state confidence of speed used for determining overall confidence */
   uint8_t conf_overall;                   /* [CONF3_T] Overall state confidence based on "smoothness" of all states */
   uint8_t low_rcs_dets_cnt;               /* Counter for blocking overall confidence increase */
   uint8_t f_ghost_NU_2_C;                 /* flag indicating change of object status from NEW to COASTED [-] */
   uint8_t f_overlapping_with_object;      /* flag indicating that object bounding box overlaps with different bounding box [-] */
   uint8_t f_prevent_orientation_std_decrease;                   /* flag indicating that object is in orientation turbulent period after merge, used by orientation variance */
   uint8_t f_changed_direction_after_start;  // flag indicating whether object changed direction when it started moving
   uint8_t f_hide_occluded_track_behind_host; // flag indicating that object is occluded by another object behind host (likely a multipath ghost)
   uint8_t f_shrink_fast;                       // flag indicating that the length filter became faster for shrinkage
   uint8_t f_suspectable_for_det_drop;         // flag indicating whether object is suspectable for detection drop [-]
   uint8_t cca_cross_moving_buffer_index; // Index used for cca_cross_moving_buffer
   int8_t direction_before_stopped;       // Direction of object's movement before it came to a stop
   int8_t cnt_consecutive_visible_from_rear; // Counter of conseuctive scans where the object is visible to rear sensors. It is in conjunction with f_hide_occluded_track_behind_host
   int8_t cca_cross_moving_buffer[20];    // buffer that records the sign of change in vcs position for non moveable objects
   uint8_t padding[3];
} F360_Internal_Object_T;

// Restore MSVC and GCC warning settings
#if defined _MSC_VER
#pragma warning(pop)
#elif 0
#pragma GCC diagnostic pop
#endif
#endif

#include "highfive.hpp"
#include "fixmac.h"
#include "Tracker_VariantA_Wrapper.h"
#include "sil_input.h"
#include "sfl_wrapper.h"
#include "f360_tracker.h"
#include "lcda_instance.h"
#include "recu_stream_log.h"
#include "sg_output.h"

extern Tracker_Info_Log_T *GetF360TrackerInfo();

using namespace HighFive;
using ds = HighFive::DataSpace;
using namespace std;

struct HDF_Scan_Index_T {
   std::vector<uint32_t> scan_index;
};

struct HDF_Raw_Detection_Info {
   std::vector<std::vector<float32_T>> timestamp;
   std::vector<std::vector<unsigned32_T>> hdrTimestamp_fractionalSec;
   std::vector<std::vector<unsigned32_T>> hdrTimestamp_Sec;
   std::vector<std::vector<float32_T>> AutoAlignElevation;
   std::vector<std::vector<unsigned32_T>> Count;
   std::vector<std::vector<unsigned32_T>> ScanIndex;
   std::vector<std::vector<unsigned8_T>> AutoAlignAzimuthQF;
   std::vector<std::vector<unsigned8_T>> AutoAlignElevationQF;
   std::vector<std::vector<unsigned8_T>> timestamp_consistency;
   std::vector<std::vector<unsigned8_T>> LookID;
   std::vector<std::vector<unsigned8_T>> LookType;

   std::vector<std::vector<std::vector<float32_T>>> elevation;
   std::vector<std::vector<std::vector<float32_T>>> azimuth;
   std::vector<std::vector<std::vector<float32_T>>> range_rate;
   std::vector<std::vector<std::vector<float32_T>>> range;
   std::vector<std::vector<std::vector<float32_T>>> amplitude;
   std::vector<std::vector<std::vector<float32_T>>> snr;
   std::vector<std::vector<std::vector<float32_T>>> std_elevation;
   std::vector<std::vector<std::vector<float32_T>>> std_azimuth;
   std::vector<std::vector<std::vector<float32_T>>> std_range_rate;
   std::vector<std::vector<std::vector<float32_T>>> std_range;
   std::vector<std::vector<std::vector<float32_T>>> std_rcs;
   std::vector<std::vector<std::vector<float32_T>>> multi_target_probability;
   std::vector<std::vector<std::vector<float32_T>>> existence_probability;
   std::vector<std::vector<std::vector<unsigned8_T>>> azimuth_confidence;
   std::vector<std::vector<std::vector<unsigned8_T>>> elevation_confidence;
   std::vector<std::vector<std::vector<unsigned8_T>>> valid;
   std::vector<std::vector<std::vector<boolean_T>>> host_veh_clutter;
   std::vector<std::vector<std::vector<boolean_T>>> nd_target;
   std::vector<std::vector<std::vector<boolean_T>>> bistatic;
};

struct HDF_GT {
   std::vector<std::vector<unsigned8_T>> id;
   std::vector<std::vector<signed8_T>> status;
   std::vector<std::vector<unsigned8_T>> age;
   std::vector<std::vector<unsigned8_T>> stage_age;
   std::vector<std::vector<float32_T>> vcs_long_posn;
   std::vector<std::vector<float32_T>> vcs_long_vel;
   std::vector<std::vector<float32_T>> vcs_long_accel;
   std::vector<std::vector<float32_T>> vcs_lat_posn;
   std::vector<std::vector<float32_T>> vcs_lat_vel;
   std::vector<std::vector<float32_T>> vcs_lat_accel;
   std::vector<std::vector<float32_T>> vcs_long_vel_rel;
   std::vector<std::vector<float32_T>> vcs_lat_vel_rel;
   std::vector<std::vector<float32_T>> speed;
   std::vector<std::vector<float32_T>> tangential_accel;
   std::vector<std::vector<float32_T>> heading;
   std::vector<std::vector<float32_T>> heading_rate;
   std::vector<std::vector<float32_T>> length;
   std::vector<std::vector<float32_T>> width;
   std::vector<std::vector<signed8_T>> object_class;
};

struct HDF_MountPosInfo {
   std::vector<std::vector<float32_T>> azimuth_polarity;
   std::vector<std::vector<float32_T>> boresight_angle;
   std::vector<std::vector<float32_T>> vcs_lat_position;
   std::vector<std::vector<float32_T>> vcs_lon_position;
   std::vector<std::vector<float32_T>> vcs_z_position;
};

struct HDF_InVehicleInfo {
   std::vector<float32_T> abs_speed;
   std::vector<float32_T> yawrate;
   std::vector<float32_T> steering_angle;
   std::vector<float32_T> rear_axle_steering_angle;
   std::vector<float32_T> rear_axle_position;
   std::vector<float32_T> lane_width_external;
   std::vector<float32_T> lane_center_offset_external;
   std::vector<float32_T> host_vehicle_length;
   std::vector<float32_T> host_vehicle_width;
   std::vector<float32_T> host_vehicle_height;
   std::vector<float32_T> curve_radius;
   std::vector<float32_T> vcs_long_acc;
   std::vector<float32_T> vcs_lat_acc;
   std::vector<float32_T> bb_center_to_rear_x;
   std::vector<float32_T> bb_center_to_rear_y;
   std::vector<float32_T> bb_center_to_rear_z;
   std::vector<unsigned32_T> vehicle_data_buff_timestamp;
   std::vector<unsigned8_T> prndl;
   std::vector<unsigned8_T> turn_signal;
   std::vector<unsigned8_T> f_reverse_gear;
   std::vector<unsigned8_T> f_trailer_present;
   std::vector<unsigned8_T> f_traffic_side;

   std::vector<std::vector<unsigned32_T>> ego_Sec;
   std::vector<std::vector<unsigned32_T>> ego_FractionalSec;
   std::vector<std::vector<float32_T>> ego_vcs_long_acc;
   std::vector<std::vector<unsigned32_T>> ego_valueQEgoAccelerationLongitudinalCog;
   std::vector<std::vector<float32_T>> ego_vcs_lat_acc;
   std::vector<std::vector<unsigned32_T>> ego_valueQEgoAccelerationLateralCog;
   std::vector<std::vector<float32_T>> ego_yawrate;
   std::vector<std::vector<unsigned32_T>> ego_valueQYawRateVehicleBody;
   std::vector<std::vector<float32_T>> ego_steering_angle;
   std::vector<std::vector<float32_T>> ego_abs_speed;
   std::vector<std::vector<unsigned32_T>> ego_valueQEgoSpeedCog;
   std::vector<std::vector<unsigned32_T>> ego_drivingDirectionConfirmed;
};
struct HDF_Detection_Info {

   std::vector<unsigned32_T> number_of_valid_detections;
   std::vector<signed16_T> vcslong_det_idx_min;
   std::vector<signed16_T> vcslong_det_idx_max;
   std::vector<std::vector<signed16_T>> vcslong_sorted_ref_det_idx;

   std::vector<std::vector<float32_t>> RD_range;
   std::vector<std::vector<float32_t>> RD_std_range;
   std::vector<std::vector<float32_t>> RD_range_rate;
   std::vector<std::vector<float32_t>> RD_std_range_rate;
   std::vector<std::vector<float32_t>> RD_azimuth;
   std::vector<std::vector<float32_t>> RD_std_azimuth;
   std::vector<std::vector<float32_t>> RD_elevation;
   std::vector<std::vector<float32_t>> RD_std_elevation;
   std::vector<std::vector<float32_t>> RD_snr;
   std::vector<std::vector<float32_t>> RD_rcs;
   std::vector<std::vector<float32_t>> RD_prob_1stazhypo;
   std::vector<std::vector<signed32_T>> RD_sensor_id;
   std::vector<std::vector<signed32_T>> RD_det_id;
   std::vector<std::vector<signed8_T>> RD_confid_azimuth;
   std::vector<std::vector<signed8_T>> RD_confid_elevation;
   std::vector<std::vector<boolean_T>> RD_f_super_res;
   std::vector<std::vector<boolean_T>> RD_f_host_veh_clutter;
   std::vector<std::vector<boolean_T>> RD_f_nd_target;
   std::vector<std::vector<boolean_T>> RD_f_bistatic;
   std::vector<std::vector<boolean_T>> RD_f_ci_det;
   std::vector<std::vector<boolean_T>> RD_f_idm_det;
   std::vector<std::vector<boolean_T>> RD_f_below_rain_thold;

   std::vector<std::vector<float32_t>> PD_vcs_position_x;
   std::vector<std::vector<float32_t>> PD_vcs_position_y;
   std::vector<std::vector<float32_t>> PD_vcs_position_z;
   std::vector<std::vector<std::vector<float32_t>>> PD_vcs_position_cov_scm;
   std::vector<std::vector<float32_t>> PD_range_rate_compensated;
   std::vector<std::vector<float32_t>> PD_vcs_az;
   std::vector<std::vector<float32_t>> PD_vcs_el;
   std::vector<std::vector<float32_t>> PD_cos_vcs_az;
   std::vector<std::vector<float32_t>> PD_sin_vcs_az;
   std::vector<std::vector<float32_t>> PD_std_vcs_az_scm;
   std::vector<std::vector<std::vector<float32_t>>> PD_vcs_cross_covariances_scm;
   std::vector<std::vector<signed16_T>> PD_next_sorted_idx;
   std::vector<std::vector<signed16_T>> PD_prev_sorted_idx;
   std::vector<std::vector<signed32_T>> PD_global_id;
   std::vector<std::vector<signed8_T>> PD_motion_status;
   std::vector<std::vector<boolean_T>> PD_f_ok_to_use;
   std::vector<std::vector<boolean_T>> PD_f_azimuth_error_stat_mov;
   std::vector<std::vector<boolean_T>> PD_f_double_bounce;
};

struct HDF_All_Objects_Log_T {
   std::vector<std::vector<unsigned32_T>> unique_id;
   std::vector<std::vector<unsigned8_T>> reference_point;
   std::vector<std::vector<unsigned8_T>> drivable_status_sg;
   std::vector<std::vector<unsigned8_T>> drivable_confidence_sg;
   std::vector<std::vector<float32_T>> otg_height;
   std::vector<std::vector<float32_T>> probability_underdrivable_ocg;
   std::vector<std::vector<float32_T>> radar_cross_section;
   std::vector<std::vector<unsigned16_T>> num_rr_inlier_dets;
   std::vector<std::vector<unsigned16_T>> num_dets_used_in_rr_msmt_update;

   std::vector<unsigned8_T> version;
   std::vector<unsigned32_T> num_elements;
   std::vector<unsigned64_T> data_timstamp_us;
   std::vector<std::vector<float32_T>> vcs_xposn;
   std::vector<std::vector<float32_T>> vcs_yposn;
   std::vector<std::vector<float32_T>> vcs_xvel;
   std::vector<std::vector<float32_T>> vcs_yvel;
   std::vector<std::vector<float32_T>> vcs_xaccel;
   std::vector<std::vector<float32_T>> vcs_yaccel;
   std::vector<std::vector<float32_T>> vcs_heading;
   std::vector<std::vector<float32_T>> vcs_pointing;
   std::vector<std::vector<float32_T>> speed;
   std::vector<std::vector<float32_T>> curvature;
   std::vector<std::vector<float32_T>> tang_accel;
   std::vector<std::vector<std::vector<float32_T>>> state_variance;
   std::vector<std::vector<std::vector<float32_T>>> supplemental_state_covariance;
   std::vector<std::vector<float32_T>> time_since_measurement;
   std::vector<std::vector<float32_T>> time_since_cluster_created;
   std::vector<std::vector<float32_T>> time_since_track_updated;
   std::vector<std::vector<float32_T>> len1;
   std::vector<std::vector<float32_T>> len2;
   std::vector<std::vector<float32_T>> wid1;
   std::vector<std::vector<float32_T>> wid2;
   std::vector<std::vector<float32_T>> confidenceLevel;
   std::vector<std::vector<float32_T>> time_since_stage_start;
   std::vector<std::vector<float32_T>> existence_probability;
   std::vector<std::vector<float32_T>> accuracy_length;
   std::vector<std::vector<float32_T>> accuracy_width;
   std::vector<std::vector<float32_T>> probability_pedestrian;
   std::vector<std::vector<float32_T>> probability_car;
   std::vector<std::vector<float32_T>> probability_motorcycle;
   std::vector<std::vector<float32_T>> probability_bicycle;
   std::vector<std::vector<float32_T>> probability_truck;
   std::vector<std::vector<float32_T>> probability_undet;
   std::vector<std::vector<unsigned16_T>> trkID;
   std::vector<std::vector<unsigned16_T>> ndets;
   std::vector<std::vector<unsigned16_T>> num_reduced_dets;
   std::vector<std::vector<unsigned8_T>> reducedID;
   std::vector<std::vector<unsigned8_T>> trk_fltr_type;
   std::vector<std::vector<unsigned8_T>> status;
   std::vector<std::vector<unsigned8_T>> reducedStatus;
   std::vector<std::vector<unsigned8_T>> init_scheme;
   std::vector<std::vector<unsigned8_T>> object_class;
   std::vector<std::vector<unsigned8_T>> f_crossing;
   std::vector<std::vector<unsigned8_T>> f_moving;
   std::vector<std::vector<unsigned8_T>> f_moveable;
   std::vector<std::vector<unsigned8_T>> f_oncoming;
   std::vector<std::vector<unsigned8_T>> f_vehicular_trk;
   std::vector<std::vector<unsigned8_T>> f_onguardrail;
   std::vector<std::vector<unsigned8_T>> f_fast_moving;
   std::vector<std::vector<unsigned8_T>> underdrivable_status;
};

struct HDF_Core_Info_T {
   std::vector<unsigned64_T> time_us;
   std::vector<unsigned64_T> prev_time_us;
   std::vector<float32_T> elapsed_time_s;
   std::vector<unsigned32_T> cnt_loops;
};

struct HDF_Tracker_Info_Log_T {
   std::vector<unsigned64_T> timestamp_us;
   std::vector<unsigned64_T> object_list_timestamp;
   std::vector<float32_T> elapsed_time_s;
   std::vector<float32_T> nr_suspected_stat_angle_jump_dets_filtered;
   std::vector<unsigned32_T> sw_version_buildID;
   std::vector<unsigned32_T> tracker_index;
   std::vector<unsigned32_T> vehicle_index;
   std::vector<unsigned32_T> num_unique_objs;
   std::vector<unsigned8_T> f_severe_angle_jump_detected;
   std::vector<std::vector<unsigned16_T>> active_obj_ids;
   std::vector<std::vector<unsigned16_T>> inactive_obj_ids;
   std::vector<unsigned16_T> num_active_objs;
   std::vector<std::vector<unsigned16_T>> reduced_active_obj_ids;
   std::vector<std::vector<unsigned16_T>> reduced_inactive_obj_ids;
   std::vector<std::vector<unsigned16_T>> reduced_obj_ids;
   std::vector<unsigned16_T> reduced_num_active_objs;
   std::vector<unsigned16_T> num_active_clusters;
   std::vector<unsigned16_T> number_of_historic_detections;
   std::vector<unsigned8_T> sw_version_major;
   std::vector<unsigned8_T> sw_version_minor;
   std::vector<unsigned8_T> sw_version_patch;
};

struct HDF_Vehicle_Info_Log_T {
   std::vector<unsigned32_T> vehicle_index;
   std::vector<float32_T> speed;
   std::vector<float32_T> vcs_speed;
   std::vector<float32_T> acceleration;
   std::vector<float32_T> vcs_lat_acceleration;
   std::vector<float32_T> vcs_long_acceleration;
   std::vector<float32_T> yaw_rate_rad;
   std::vector<float32_T> vcs_sideslip;
   std::vector<float32_T> curvature_rear;
   std::vector<float32_T> dist_rear_axle_to_vcs_m;
   std::vector<float32_T> vehicle_length;
   std::vector<float32_T> vehicle_width;
   std::vector<float32_T> rear_cornering_compliance;
   std::vector<float32_T> speed_correction_factor;
   std::vector<unsigned8_T> host_type;
   std::vector<boolean_T> f_trailer_presence_hardware;
   std::vector<unsigned8_T> speed_qf;
   std::vector<unsigned8_T> yaw_rate_qf;
   std::vector<unsigned8_T> lat_accel_qf;
   std::vector<unsigned8_T> long_accel_qf;
   std::vector<float32_T> steer_gear_ratio;
   std::vector<float32_T> wheelbase_m;
   std::vector<float32_T> understeer_coefficient;
   std::vector<float32_T> cog_x;
   std::vector<float32_T> cog_y;
   std::vector<float32_T> front_wheel_radius_m;
   std::vector<float32_T> front_track_width_m;
   std::vector<unsigned32_T> raw_host_signal_latency_ms;
   std::vector<boolean_T> f_enable_internal_reflections_func;
   std::vector<boolean_T> f_enable_internal_reflections_func_trailer;
};

struct HDF_Sensor_Info_Log_T {
   std::vector<std::vector<float32_T>> ant_sens_SCS_azim;
   std::vector<std::vector<float32_T>> ant_sens_SCS_sq_rng_90;
   std::vector<std::vector<float32_T>> ant_sens_SCS_sq_rng_50;
   std::vector<std::vector<float32_T>> min_host_vel;
   std::vector<std::vector<float32_T>> occurrence_lowerlimit;
   std::vector<std::vector<float32_T>> occurrence_threshold;
   std::vector<std::vector<float32_T>> rcs_tolerance;
   std::vector<std::vector<float32_T>> azimuth_tolerance;
   std::vector<std::vector<float32_T>> range_tolerance;
   std::vector<std::vector<float32_T>> rcs_max;
   std::vector<std::vector<float32_T>> range_max;
   std::vector<std::vector<unsigned16_T>> age_threshold;
   std::vector<std::vector<boolean_T>> f_enable;
   std::vector<std::vector<float32_T>> interior_fov;
   std::vector<std::vector<float32_T>> left_fov_normal;
   std::vector<std::vector<float32_T>> right_fov_normal;
   std::vector<std::vector<float32_T>> vcs_Longitude;
   std::vector<std::vector<float32_T>> vcs_lateral;
   std::vector<std::vector<float32_T>> vcs_height;
   std::vector<std::vector<float32_T>> vcs_boresight_azimuth_angle;
   std::vector<std::vector<float32_T>> vcs_boresight_elevation_angle;
   std::vector<std::vector<float32_T>> range_limits;
   std::vector<std::vector<float32_T>> fov_min_az_rad;
   std::vector<std::vector<float32_T>> fov_max_az_rad;
   std::vector<std::vector<float32_T>> fov_min_el_rad;
   std::vector<std::vector<float32_T>> fov_max_el_rad;
   std::vector<std::vector<float32_T>> min_aliaised_range_rate;
   std::vector<std::vector<float32_T>> v_wrapping;
   std::vector<std::vector<float32_T>> r_wrapping;
   std::vector<std::vector<unsigned32_T>> id;
   std::vector<std::vector<signed32_T>> polarity;
   std::vector<std::vector<int32_T>> useful_FOV;
   std::vector<unsigned32_T> sensor_sw_version;
   std::vector<boolean_T> f_read_cdc_data;
   std::vector<std::vector<int8_T>> mounting_location;
   std::vector<std::vector<int8_T>> sensor_type;
   std::vector<std::vector<unsigned64_T>> timestamp_us;
   std::vector<std::vector<float32_T>> vcs_velocityLat;
   std::vector<std::vector<float32_T>> vcs_velocityLong;
   std::vector<std::vector<float32_T>> vacs_boresight_az_estimated;
   std::vector<std::vector<float32_T>> vacs_boresight_el_estimated;
   std::vector<std::vector<unsigned32_T>> number_of_valid_detections;
   std::vector<float32_T> yaw_rate_calc_dps;
   std::vector<float32_T> vehicle_speed_calc_mps;
   std::vector<float32_T> time_since_measurement_s;
   std::vector<signed32_T> first_detection_list_idx;
   std::vector<std::vector<unsigned16_T>> overall_rain_level;
   std::vector<std::vector<unsigned16_T>> look_index;
   std::vector<std::vector<boolean_T>> is_valid;
   std::vector<std::vector<boolean_T>> f_sensor_fault_detected;
   std::vector<boolean_T> f_ant_sens_available;
   std::vector<boolean_T> f_ant_sens_degraded;
   std::vector<std::vector<int8_T>> look_id;
};

struct HDF_VSE_Output_Log_T {
   std::vector<unsigned64_T> timestamp_us;
   std::vector<float32_T> raw_speed_mps;
   std::vector<float32_T> speed_compensation_factor;
   std::vector<float32_T> filt_veh_speed_over_ground;
   std::vector<float32_T> raw_lat_accel;
   std::vector<float32_T> raw_long_accel;
   std::vector<float32_T> raw_yaw_rate_rps;
   std::vector<float32_T> raw_steering_angle_deg;
   std::vector<float32_T> road_wheel_angle_deg;
   std::vector<float32_T> yaw_rate_sa;
   std::vector<float32_T> yaw_rate_raw_bias;
   std::vector<float32_T> comp_yaw_rate_unfiltered;
   std::vector<float32_T> comp_yaw_rate_filtered;
   std::vector<float32_T> curvature_rear_axle;
   std::vector<float32_T> sideslip_rear_axle;
   std::vector<float32_T> vcs_sideslip;
   std::vector<float32_T> vcs_long_velocity;
   std::vector<float32_T> vcs_lat_velocity;
   std::vector<float32_T> sensor_sideslip;
   std::vector<float32_T> sensor_long_velocity;
   std::vector<float32_T> sensor_lat_velocity;
   std::vector<float32_T> k_dist_rear_axle_to_vcs;
   std::vector<float32_T> vcs_lat_accel;
   std::vector<float32_T> vcs_long_accel;
   std::vector<float32_T> accel_rear_axle;
   std::vector<float32_T> signed_filt_veh_speed_over_ground;
   std::vector<unsigned32_T> veh_index;
   std::vector<unsigned8_T> raw_speed_qf;
   std::vector<unsigned8_T> speed_compensation_factor_qf;
   std::vector<unsigned8_T> filt_veh_speed_over_ground_qf;
   std::vector<unsigned8_T> raw_lat_accel_qf;
   std::vector<unsigned8_T> raw_long_accel_qf;
   std::vector<unsigned8_T> raw_yaw_rate_qf;
   std::vector<unsigned8_T> raw_steering_angle_qf;
   std::vector<unsigned8_T> road_wheel_angle_qf;
   std::vector<unsigned8_T> yaw_rate_sa_qf;
   std::vector<unsigned8_T> yaw_rate_bias_qf;
   std::vector<unsigned8_T> comp_yaw_rate_qf;
   std::vector<unsigned8_T> stationary;
   std::vector<unsigned8_T> vcs_lat_accel_qf;
   std::vector<unsigned8_T> vcs_long_accel_qf;
};

struct HDF_ROT_T {
   std::vector<unsigned64_T> rot_object_list_timestamp;
   std::vector<unsigned64_T> tracker_start_timestamp;
   std::vector<float32_T> tracker_elapsed_time;
   std::vector<unsigned32_T> tracker_index;
   std::vector<unsigned16_T> number_of_objects;

   std::vector<std::vector<float32_T>> vcs_x_posn;
   std::vector<std::vector<float32_T>> vcs_y_posn;
   std::vector<std::vector<float32_T>> vcs_x_vel;
   std::vector<std::vector<float32_T>> vcs_y_vel;
   std::vector<std::vector<float32_T>> vcs_x_acc;
   std::vector<std::vector<float32_T>> vcs_y_acc;
   std::vector<std::vector<float32_T>> vcs_heading;
   std::vector<std::vector<float32_T>> vcs_pointing;
   std::vector<std::vector<std::vector<float32_T>>> vcs_state_variance;
   std::vector<std::vector<std::vector<float32_T>>> vcs_supplemental_state_covariance;
   std::vector<std::vector<float32_T>> vcs_curvature;

   std::vector<std::vector<float32_T>> iso_x_posn;
   std::vector<std::vector<float32_T>> iso_y_posn;
   std::vector<std::vector<float32_T>> iso_x_vel;
   std::vector<std::vector<float32_T>> iso_relative_x_vel;
   std::vector<std::vector<float32_T>> iso_y_vel;
   std::vector<std::vector<float32_T>> iso_relative_y_vel;
   std::vector<std::vector<float32_T>> iso_x_acc;
   std::vector<std::vector<float32_T>> iso_relative_x_acc;
   std::vector<std::vector<float32_T>> iso_y_acc;
   std::vector<std::vector<float32_T>> iso_relative_y_acc;

   std::vector<std::vector<float32_T>> iso_orientation;
   std::vector<std::vector<float32_T>> iso_orientation_var;
   std::vector<std::vector<float32_T>> iso_orientation_rate;
   std::vector<std::vector<float32_T>> iso_orientation_rate_var;

   std::vector<std::vector<float32_T>> iso_x_posn_var;
   std::vector<std::vector<float32_T>> iso_y_posn_var;
   std::vector<std::vector<float32_T>> iso_xy_posn_cov;
   std::vector<std::vector<float32_T>> iso_x_vel_var;
   std::vector<std::vector<float32_T>> iso_y_vel_var;
   std::vector<std::vector<float32_T>> iso_xy_vel_cov;
   std::vector<std::vector<float32_T>> iso_x_acc_var;
   std::vector<std::vector<float32_T>> iso_y_acc_var;
   std::vector<std::vector<float32_T>> iso_xy_acc_cov;

   std::vector<std::vector<float32_T>> speed;
   std::vector<std::vector<float32_T>> tang_accel;
   std::vector<std::vector<float32_T>> length;
   std::vector<std::vector<float32_T>> length_var;
   std::vector<std::vector<float32_T>> width;
   std::vector<std::vector<float32_T>> width_var;
   std::vector<std::vector<float32_T>> time_since_created;
   std::vector<std::vector<float32_T>> time_since_published;
   std::vector<std::vector<float32_T>> time_since_stage_start;
   std::vector<std::vector<float32_T>> existence_probability;
   std::vector<std::vector<float32_T>> mirror_prob;
   std::vector<std::vector<float32_T>> radar_cross_section;
   std::vector<std::vector<float32_T>> otg_height;
   std::vector<std::vector<float32_T>> confidence_level;

   std::vector<std::vector<float32_T>> probability_pedestrian;
   std::vector<std::vector<float32_T>> probability_car;
   std::vector<std::vector<float32_T>> probability_motorcycle;
   std::vector<std::vector<float32_T>> probability_bicycle;
   std::vector<std::vector<float32_T>> probability_truck;
   std::vector<std::vector<float32_T>> probability_undet;
   std::vector<std::vector<float32_T>> probability_underdrivable_ocg;
   std::vector<std::vector<float32_T>> movable_prob;

   std::vector<std::vector<int32_T>> id;
   std::vector<std::vector<unsigned32_T>> unique_id;
   std::vector<std::vector<unsigned32_T>> ndets;
   std::vector<std::vector<unsigned32_T>> num_dets_used_in_rr_msmt_update;
   std::vector<std::vector<unsigned16_T>> sensor_src;

   std::vector<std::vector<unsigned8_T>> reference_point;
   std::vector<std::vector<unsigned8_T>> object_status;
   std::vector<std::vector<unsigned8_T>> object_class;
   std::vector<std::vector<unsigned8_T>> movement_status;
   std::vector<std::vector<unsigned8_T>> occlusion_status;
   std::vector<std::vector<unsigned8_T>> underdrivable_status_ocg;
   std::vector<std::vector<unsigned8_T>> drivable_status_sg;
   std::vector<std::vector<unsigned8_T>> drivable_confidence_sg;
   std::vector<std::vector<unsigned8_T>> f_onguardrail;
   std::vector<std::vector<unsigned8_T>> trk_fltr_type;
};

struct HDF_OLP_OUPUT_Log_T {
   std::vector<std::vector<uint8_t>> id;
   std::vector<std::vector<uint32_t>> unique_id;
   std::vector<std::vector<uint8_t>> index;
   std::vector<std::vector<std::string>> status;
   std::vector<std::vector<uint8_t>> age;
   std::vector<std::vector<uint8_t>> stage_age;
   std::vector<std::vector<uint8_t>> fbk_stage_age;
   std::vector<std::vector<float32_t>> existence_probability;
   std::vector<std::vector<float32_t>> speed;
   std::vector<std::vector<float32_t>> vcs_pos_x;
   std::vector<std::vector<float32_t>> vcs_pos_y;
   std::vector<std::vector<float32_t>> vcs_vel_x;
   std::vector<std::vector<float32_t>> vcs_vel_y;
   std::vector<std::vector<float32_t>> vcs_vel_rel_x;
   std::vector<std::vector<float32_t>> vcs_vel_rel_y;
   std::vector<std::vector<float32_t>> vcs_accel_x;
   std::vector<std::vector<float32_t>> vcs_accel_y;
   std::vector<std::vector<float32_t>> vcs_heading;
   std::vector<std::vector<float32_t>> heading_rate;
   std::vector<std::vector<float32_t>> heading_variance;
   std::vector<std::vector<float32_t>> accuracy_heading;
   std::vector<std::vector<float32_t>> eclipse_value;
   std::vector<std::vector<float32_t>> length;
   std::vector<std::vector<float32_t>> width;
   std::vector<std::vector<float32_t>> obj_distance;
   std::vector<std::vector<float32_t>> obstruction_prob;
   std::vector<std::vector<uint8_t>> obj_class;
   std::vector<std::vector<float32_t>> class_prob_pedestrian;
   std::vector<std::vector<float32_t>> class_prob_2wheel;
   std::vector<std::vector<float32_t>> class_prob_car;
   std::vector<std::vector<float32_t>> class_prob_truck;
   std::vector<std::vector<uint8_t>> id_merged_obj;
   std::vector<std::vector<uint8_t>> f_merge_occured;
   std::vector<std::vector<uint8_t>> curvi_coordinates_calc_method;
   std::vector<std::vector<float32_t>> curvi_pos_x;
   std::vector<std::vector<float32_t>> curvi_pos_y;
   std::vector<std::vector<float32_t>> curvi_vel_x;
   std::vector<std::vector<float32_t>> curvi_vel_y;
   std::vector<std::vector<float32_t>> curvi_vel_rel_x;
   std::vector<std::vector<float32_t>> curvi_vel_rel_y;
   std::vector<std::vector<float32_t>> curvi_heading;
   std::vector<std::vector<uint8_t>> f_reflection;
   std::vector<std::vector<uint8_t>> f_stationary;
   std::vector<std::vector<uint8_t>> f_moveable;
   std::vector<std::vector<uint8_t>> f_stationary_clutter;
   std::vector<std::vector<uint8_t>> f_is_fl_origin_sensor;
   std::vector<std::vector<uint8_t>> f_is_fr_origin_sensor;
   std::vector<std::vector<uint8_t>> f_is_rl_origin_sensor;
   std::vector<std::vector<uint8_t>> f_is_rr_origin_sensor;
   std::vector<std::vector<uint8_t>> f_is_in_fl_sensor_fov;
   std::vector<std::vector<uint8_t>> f_is_in_fr_sensor_fov;
   std::vector<std::vector<uint8_t>> f_is_in_rl_sensor_fov;
   std::vector<std::vector<uint8_t>> f_is_in_rr_sensor_fov;
   std::vector<uint32_t> n_valid_objects;
};

struct HDF_Lcda_Output {
   std::vector<int> lcda_status;
   std::vector<boolean_T> f_bsw_enabled;
   std::vector<int> bsw_alert_left;
   std::vector<int> bsw_alert_right;
   std::vector<uint8_t> bsw_id_left;
   std::vector<uint8_t> bsw_id_right;
   std::vector<uint32_t> bsw_unique_id_left;
   std::vector<uint32_t> bsw_unique_id_right;

   std::vector<boolean_T> f_cvw_enabled;
   std::vector<int> cvw_alert_left;
   std::vector<int> cvw_alert_right;
   std::vector<uint8_t> cvw_id_left;
   std::vector<uint8_t> cvw_id_right;
   std::vector<uint32_t> cvw_unique_id_left;
   std::vector<uint32_t> cvw_unique_id_right;
   std::vector<float32_T> cvw_ttc_s_left;
   std::vector<float32_T> cvw_ttc_s_right;
   std::vector<boolean_T> f_slc_enabled;
   std::vector<boolean_T> slc_alert_left;
   std::vector<boolean_T> slc_alert_right;
   std::vector<uint8_t> slc_id_left;
   std::vector<uint8_t> slc_id_right;
   std::vector<uint32_t> slc_unique_id_left;
   std::vector<uint32_t> slc_unique_id_right;
   std::vector<float32_T> slc_ttc_s_left;
   std::vector<float32_T> slc_ttc_s_right;
   std::vector<float32_T> slc_lane_change_probability_left;
   std::vector<float32_T> slc_lane_change_probability_right;
};

struct HDF_Ced_Output {
   std::vector<boolean_T> f_ced_enable;
   std::vector<int> ced_alert_left;
   std::vector<int> ced_alert_right;

   std::vector<uint8_t> ced_object_ced_id_left;
   std::vector<uint8_t> ced_object_unique_id_left;

   std::vector<int> ced_object_type_left;
   std::vector<float32_T> ced_object_length_m_left;
   std::vector<float32_T> ced_object_width_m_left;
   std::vector<float32_T> ced_object_lat_pos_m_left;
   std::vector<float32_T> ced_object_long_pos_m_left;
   std::vector<float32_T> ced_object_speed_mps_left;
   std::vector<float32_T> ced_object_heading_rad_left;

   std::vector<int> ced_object_direction_left;
   std::vector<float32_T> ced_object_predicted_lat_pos_m_left;
   std::vector<float32_T> ced_object_ttc_s_left;
   std::vector<float32_T> ced_object_ttp_s_left;

   std::vector<uint8_t> ced_object_ced_id_right;
   std::vector<uint8_t> ced_object_unique_id_right;

   std::vector<int> ced_object_type_right;
   std::vector<float32_T> ced_object_length_m_right;
   std::vector<float32_T> ced_object_width_m_right;
   std::vector<float32_T> ced_object_lat_pos_m_right;
   std::vector<float32_T> ced_object_long_pos_m_right;
   std::vector<float32_T> ced_object_speed_mps_right;
   std::vector<float32_T> ced_object_heading_rad_right;

   vector<int> ced_object_direction_right;
   vector<float32_T> ced_object_predicted_lat_pos_m_right;
   vector<float32_T> ced_object_ttc_s_right;
   vector<float32_T> ced_object_ttp_s_right;
};

struct HDF_Esa_Output {
   std::vector<int> esa_status;

   std::vector<boolean_T> f_esa_alert_left;
   std::vector<boolean_T> f_esa_alert_right;

   // Esa_Critical_Object_T for left side
   std::vector<uint8_t> esa_object_id_left;
   std::vector<uint8_t> esa_object_index_left;

   std::vector<float32_T> esa_object_width_m_left;
   std::vector<float32_T> esa_object_length_m_left;

   std::vector<float32_T> esa_object_long_pos_m_left;
   std::vector<float32_T> esa_object_lat_pos_m_left;
   std::vector<float32_T> esa_object_long_speed_mps_left;
   std::vector<float32_T> esa_object_lat_speed_mps_left;

   std::vector<float32_T> esa_object_ttc_s_left;
   std::vector<float32_T> esa_object_ttp_s_left;
   std::vector<float32_T> esa_object_decel_to_reach_host_speed_mps2_left;
   std::vector<float32_T> esa_object_long_distance_m_left;
   std::vector<float32_T> esa_object_existence_prob_left;

   // Esa_Critical_Object_T for right side
   std::vector<uint8_t> esa_object_id_right;
   std::vector<uint8_t> esa_object_index_right;

   std::vector<float32_T> esa_object_width_m_right;
   std::vector<float32_T> esa_object_length_m_right;

   std::vector<float32_T> esa_object_long_pos_m_right;
   std::vector<float32_T> esa_object_lat_pos_m_right;
   std::vector<float32_T> esa_object_long_speed_mps_right;
   std::vector<float32_T> esa_object_lat_speed_mps_right;

   std::vector<float32_T> esa_object_ttc_s_right;
   std::vector<float32_T> esa_object_ttp_s_right;
   std::vector<float32_T> esa_object_decel_to_reach_host_speed_mps2_right;
   std::vector<float32_T> esa_object_long_distance_m_right;
   std::vector<float32_T> esa_object_existence_prob_right;
};
struct Hdf_Ltb_Output {
   // Ltb_Critical_Object_T for left side
   std::vector<uint8_t> ltb_object_ltb_id_left;
   std::vector<float32_T> ltb_object_ltb_ttc_s_left;
   std::vector<float32_T> ltb_object_ltb_ttb_s_left;
   std::vector<float32_T> ltb_object_ltb_decel_estimate_mps2_left;
   std::vector<float32_T> ltb_object_ltb_distance_m_left;

   // Ltb_Critical_Object_T for right side
   std::vector<uint8_t> ltb_object_ltb_id_right;
   std::vector<float32_T> ltb_object_ltb_ttc_s_right;
   std::vector<float32_T> ltb_object_ltb_ttb_s_right;
   std::vector<float32_T> ltb_object_ltb_decel_estimate_mps2_right;
   std::vector<float32_T> ltb_object_ltb_distance_m_right;

   std::vector<int> ltb_alert_level_left;
   std::vector<int> ltb_alert_level_right;

   std::vector<uint8_t> ltb_most_critical_side;
};
struct HDF_Recw_Output {
   std::vector<float32_T> recw_crash_probability;
   std::vector<float32_T> recw_ttc_s;
   std::vector<uint8_t> recw_id;
   std::vector<uint32_t> recw_unique_id;
   std::vector<int> recw_alert_level;
   std::vector<float32_T> ttc_threshold_alert_level_1_s;
   std::vector<float32_T> ttc_threshold_alert_level_2_s;
};
struct HDF_Ta_Output {
   std::vector<boolean_T> f_ta_enable;
   std::vector<boolean_T> ta_f_vehicle_state_relevant;

   std::vector<uint8_t> ta_most_critical_side;
   std::vector<uint8_t> ta_n_valid_objects;
   std::vector<uint8_t> ta_n_relevant_objects;
   std::vector<uint8_t> ta_n_critical_objects;

   std::vector<int> ta_algorithm_state;
   std::vector<int> ta_alert_level_left;
   std::vector<int> ta_alert_level_right;

   // Ta_Critical_Object_T data vector for left side
   std::vector<float32_T> ta_object_ta_waypoint_at_collision_m_x_left;
   std::vector<float32_T> ta_object_ta_waypoint_at_collision_m_y_left;

   std::vector<float32_T> ta_object_ta_ttc_s_left;
   std::vector<float32_T> ta_object_ta_ttp_s_left;
   std::vector<float32_T> ta_object_ta_ttb_s_left;
   std::vector<float32_T> ta_object_ta_decel_estimate_mps2_left;
   std::vector<float32_T> ta_object_ta_distance_m_left;

   std::vector<uint8_t> ta_object_ta_id_left;
   std::vector<uint8_t> ta_object_ta_index_left;

   std::vector<boolean_T> ta_object_ta_f_obj_in_danger_zone_left;
   std::vector<boolean_T> ta_object_ta_f_obj_in_info_zone_left;
   std::vector<boolean_T> ta_object_ta_f_obj_in_wing_zone_left;

   // Ta_Critical_Object_T data vector for right side
   std::vector<float32_T> ta_object_ta_waypoint_at_collision_m_x_right;
   std::vector<float32_T> ta_object_ta_waypoint_at_collision_m_y_right;

   std::vector<float32_T> ta_object_ta_ttc_s_right;
   std::vector<float32_T> ta_object_ta_ttp_s_right;
   std::vector<float32_T> ta_object_ta_ttb_s_right;
   std::vector<float32_T> ta_object_ta_decel_estimate_mps2_right;
   std::vector<float32_T> ta_object_ta_distance_m_right;

   std::vector<uint8_t> ta_object_ta_id_right;
   std::vector<uint8_t> ta_object_ta_index_right;

   std::vector<boolean_T> ta_object_ta_f_obj_in_danger_zone_right;
   std::vector<boolean_T> ta_object_ta_f_obj_in_info_zone_right;
   std::vector<boolean_T> ta_object_ta_f_obj_in_wing_zone_right;
};

struct HDF_Pt_Output {
   std::vector<std::vector<float32_T>> range_vcs_proj_to_path_segment;
   std::vector<std::vector<float32_T>> segment_heading_diff;
   std::vector<std::vector<uint8_t>> track_idx_nearest_path;
   std::vector<std::vector<float32_T>> range_at_zero;
   std::vector<std::vector<float32_T>> range_to_current_path_part;
   std::vector<std::vector<float32_T>> range_at_host_edge;
   std::vector<std::vector<float32_T>> length_of_trajectory;
   std::vector<std::vector<float32_T>> path_heading;
   std::vector<std::vector<int>> path_direction;
   std::vector<std::vector<uint8_t>> track_match;
   std::vector<std::vector<uint8_t>> track_match_age;
   std::vector<std::vector<uint8_t>> track_match_last_cycle;
   std::vector<boolean_T> f_pt_operational;
};

struct HDF_Scw_Output {
   std::vector<boolean_T> f_scw_enabled;
   std::vector<boolean_T> f_scw_dyn_enabled;
   std::vector<boolean_T> f_scw_guardrail_enabled;

   // Scw_Output_Object_T data for left side
   std::vector<int> scw_object_alert_level_left;
   std::vector<uint8_t> scw_object_id_left;
   std::vector<uint8_t> scw_object_unique_id_left;
   std::vector<int> scw_object_type_left;
   std::vector<float32_T> scw_object_lateral_ttc_s_left;
   std::vector<float32_T> scw_object_lateral_distance_m_left;
   std::vector<float32_T> scw_object_position_m_x_left;
   std::vector<float32_T> scw_object_position_m_y_left;
   std::vector<float32_T> scw_object_velocity_mps_x_left;
   std::vector<float32_T> scw_object_velocity_mps_y_left;
   std::vector<float32_T> scw_object_acceleration_mps2_x_left;
   std::vector<float32_T> scw_object_acceleration_mps2_y_left;
   std::vector<float32_T> scw_object_width_m_left;
   std::vector<float32_T> scw_object_length_m_left;
   std::vector<float32_T> scw_object_heading_rad_left;
   std::vector<float32_T> scw_object_existence_probability_left;
   std::vector<uint16_t> scw_object_age_left;

   // Scw_Output_Object_T data for right side
   std::vector<int> scw_object_alert_level_right;
   std::vector<uint8_t> scw_object_id_right;
   std::vector<uint8_t> scw_object_unique_id_right;
   std::vector<int> scw_object_type_right;
   std::vector<float32_T> scw_object_lateral_ttc_s_right;
   std::vector<float32_T> scw_object_lateral_distance_m_right;
   std::vector<float32_T> scw_object_position_m_x_right;
   std::vector<float32_T> scw_object_position_m_y_right;
   std::vector<float32_T> scw_object_velocity_mps_x_right;
   std::vector<float32_T> scw_object_velocity_mps_y_right;
   std::vector<float32_T> scw_object_acceleration_mps2_x_right;
   std::vector<float32_T> scw_object_acceleration_mps2_y_right;
   std::vector<float32_T> scw_object_width_m_right;
   std::vector<float32_T> scw_object_length_m_right;
   std::vector<float32_T> scw_object_heading_rad_right;
   std::vector<float32_T> scw_object_existence_probability_right;
   std::vector<uint16_t> scw_object_age_right;
};

struct HDF_Cta_Output {
   std::vector<boolean_T> f_cta_enabled;

   // Cta_Critical_Object_T data for each 2 for left side
   std::vector<uint8_t> most_critical_object_by_sides_id_left;
   std::vector<uint32_t> most_critical_object_by_sides_unique_id_left;
   std::vector<float32_T> most_critical_object_by_sides_objPoseX_m_left;
   std::vector<float32_T> most_critical_object_by_sides_objPoseY_m_left;
   std::vector<float32_T> most_critical_object_by_sides_objVelocityX_mps_left;
   std::vector<float32_T> most_critical_object_by_sides_objVelocityY_mps_left;
   std::vector<float32_T> most_critical_object_by_sides_heading_rad_left;
   std::vector<int> most_critical_object_by_sides_alert_level_left;
   std::vector<float32_T> most_critical_object_by_sides_ttc_s_left;
   std::vector<float32_T> most_critical_object_by_sides_intersection_point_x_m_left;
   std::vector<boolean_T> most_critical_object_by_sides_f_brake_qualifier_left;

   // Cta_Critical_Object_T data for each 2 for right side
   std::vector<uint8_t> most_critical_object_by_sides_id_right;
   std::vector<uint32_t> most_critical_object_by_sides_unique_id_right;
   std::vector<float32_T> most_critical_object_by_sides_objPoseX_m_right;
   std::vector<float32_T> most_critical_object_by_sides_objPoseY_m_right;
   std::vector<float32_T> most_critical_object_by_sides_objVelocityX_mps_right;
   std::vector<float32_T> most_critical_object_by_sides_objVelocityY_mps_right;
   std::vector<float32_T> most_critical_object_by_sides_heading_rad_right;
   std::vector<int> most_critical_object_by_sides_alert_level_right;
   std::vector<float32_T> most_critical_object_by_sides_ttc_s_right;
   std::vector<float32_T> most_critical_object_by_sides_intersection_point_x_m_right;
   std::vector<boolean_T> most_critical_object_by_sides_f_brake_qualifier_right;
};

struct HDF_SG_Output {
   std::vector<unsigned64_T> execution_timestamp_us;
   std::vector<unsigned64_T> measurement_timestamp_us;
   std::vector<unsigned32_T> cycle_index;
   std::vector<unsigned32_T> num_contours;
   std::vector<boolean_T> f_valid;

   std::vector<std::vector<float32_T>> position_x;
   std::vector<std::vector<float32_T>> position_y;
   std::vector<std::vector<float32_T>> position_variance_x;
   std::vector<std::vector<float32_T>> position_variance_y;
   std::vector<std::vector<float32_T>> position_covariance_xy;
   std::vector<std::vector<unsigned16_T>> cycles_since_created;
   std::vector<std::vector<unsigned16_T>> cycles_since_coasted;
   std::vector<std::vector<unsigned8_T>> drivability_confidence;
   std::vector<std::vector<unsigned8_T>> drivability;

   std::vector<std::vector<unsigned32_T>> unique_id;
   std::vector<std::vector<unsigned16_T>> num_vertices;
   std::vector<std::vector<unsigned8_T>> type;

   std::vector<unsigned16_T> major;
   std::vector<unsigned16_T> minor;
   std::vector<unsigned16_T> patch;
};

struct HDF_Faults {
   std::vector<uint8_t> input_core_time_us_no_increase;
   std::vector<uint8_t> input_core_cnt_loops_no_increase;
   std::vector<uint8_t> input_core_elapsed_time_below_lower_limit;
   std::vector<uint8_t> input_core_elapsed_time_above_upper_limit;

   std::vector<uint8_t> input_host_vehicle_index_no_increase;
   std::vector<uint8_t> input_host_speed_invalid;
   std::vector<uint8_t> input_host_yawrate_invalid;
   std::vector<uint8_t> input_host_longitudinal_acceleration_invalid;
   std::vector<uint8_t> input_host_lateral_acceleration_invalid;

   std::vector<uint8_t> input_raw_detection_range_is_invalid;
   std::vector<uint8_t> input_raw_detection_range_rate_is_invalid;
   std::vector<uint8_t> input_raw_detection_azimuth_is_invalid;
   std::vector<uint8_t> input_raw_detection_elevation_is_invalid;

   std::vector<std::vector<uint8_t>> input_sensors_calib_mounting_pos_is_invalid;
   std::vector<std::vector<uint8_t>> input_sensors_calib_polarity_is_invalid;
   std::vector<std::vector<uint8_t>> input_sensors_calib_boresight_angle_is_invalid;

   std::vector<std::vector<uint8_t>> input_sensors_look_index_no_increase;
   std::vector<std::vector<uint8_t>> input_sensors_sensor_vs_tracker_timestamp_divergence;

   std::vector<uint8_t> output_track_positions_faulty;
   std::vector<uint8_t> output_track_velocities_faulty;
   std::vector<uint8_t> output_track_accelerations_faulty;
   std::vector<uint8_t> output_severe_angle_jump_presence_fault;

   std::vector<std::vector<uint8_t>> scl_sensors_calibs_fault_status;
   std::vector<std::vector<uint8_t>> scl_sensors_fault_status;
   std::vector<uint8_t> scl_core_info_fault_status;
   std::vector<uint8_t> scl_host_info_fault_status;
   std::vector<uint8_t> scl_raw_detection_fault_status;
   std::vector<uint8_t> scl_object_track_fault_status;
   std::vector<uint8_t> scl_angle_jump_fault_status;
   std::vector<uint8_t> scl_overall_fault_status;
   std::vector<uint8_t> scl_should_reset;
};

class HDFWriteClass {

 private:
   HDFWriteClass()                                 = default;
   ~HDFWriteClass()                                = default;
   HDFWriteClass(const HDFWriteClass &)            = delete;
   HDFWriteClass &operator=(const HDFWriteClass &) = delete;
   string HDF_Path;
   string HDF_IPath;

   HDF_Raw_Detection_Info hdf_raw_detection_info_ptr;
   HDF_Detection_Info hdf_processed_info_ptr;
   HDF_All_Objects_Log_T hdf_all_objects_ptr;
   HDF_All_Objects_Log_T hdf_all_objectsGT_ptr;
   HDF_Vehicle_Info_Log_T hdf_vehicle_info_ptr;
   HDF_Tracker_Info_Log_T hdf_tracker_info_ptr;
   HDF_Sensor_Info_Log_T hdf_sensor_info_ptr;
   HDF_MountPosInfo hdf_MountInfo_ptr;
   HDF_InVehicleInfo hdf_InVehicleInfo_ptr;
   HDF_Core_Info_T hdf_core_info_ptr;
   HDF_ROT_T hdf_ROT_info_ptr;
   HDF_GT hdf_GT_ptr;
   HDF_OLP_OUPUT_Log_T hdf_olp_output_ptr;
   DC_INPUT_DATA_T *dc_input;
   HDF_Lcda_Output hdf_lcda_ptr;
   HDF_Ced_Output hdf_ced_ptr;
   HDF_Esa_Output hdf_esa_ptr;
   Hdf_Ltb_Output hdf_ltb_ptr;
   HDF_Recw_Output hdf_recw_ptr;
   HDF_Ta_Output hdf_ta_ptr;
   HDF_Scw_Output hdf_scw_ptr;
   HDF_Pt_Output hdf_pt_ptr;
   HDF_Cta_Output hdf_cta_ptr;
   HDF_Scan_Index_T hdf_scan_index_ptr;
   HDF_Scan_Index_T hdf_scan_indexIp_ptr;
   HDF_SG_Output hdf_sg_ptr;
   HDF_Faults hdf_faults_ptr;

   HDF_VSE_Output_Log_T hdf_vse_output_ptr;
   Tracker_Info_Log_T *trackInfo                             = nullptr;
   f360_variant_A::F360_Object_Log_Output_T *all_objects     = nullptr;
   rspp_variant_A::F360_Radar_Sensor_T *SensInfo             = nullptr;
   f360_variant_A::F360_Detection_Log_Output_T *DetLog       = nullptr;
   rspp_variant_A::RSPP_Detection_List_T *DetList            = nullptr;
   f360_variant_A::F360_Core_Info_T *CoreInfo                = nullptr;
   ROT_Object_List_Info_T *ROT_Obj                           = nullptr;
   VSE_OUT *VsePtr                                           = nullptr;
   f360_variant_A::F360_Host_Calib_T *ptr_hostCalib          = nullptr;
   f360_variant_A::F360_Sensor_Calib_Log_Output_T *SensCalib = nullptr;
   f360_variant_A::F360_Host_T *ptr_host                     = nullptr;
   SFL_Olp_Objects_Log_T *olp_ptr                            = nullptr;
   sg::SG_Output_T *sg_ptr                                   = nullptr;

   Lcda_Output_T *lcda_ptr                = nullptr;
   Ced_Output_T *ced_ptr                  = nullptr;
   Esa_Output_T *esa_ptr                  = nullptr;
   Ltb_Output_T *ltb_ptr                  = nullptr;
   Recw_Output_T *recw_ptr                = nullptr;
   Ta_Output_T *ta_ptr                    = nullptr;
   Scw_Output_T *scw_ptr                  = nullptr;
   Pt_Output_T *pt_ptr                    = nullptr;
   Cta_Output_T *cta_ptr                  = nullptr;
   Functional_Safety_Faults_Log_T *faults = nullptr;

 public:
   static HDFWriteClass *instance;
   void Init();
   void UpdateHDFbuffers();
   void UpdateHDFIpbuffers();
   void HDF_Write();
   void HDF_IpWrite();
   void Set_HDF_Output_Path(const char *Output_Path);
   void Set_HDF_Input_Path(const char *Input_Path);
   string Get_HDF_Output_Path();
   string Get_HDF_Input_Path();
   static HDFWriteClass *getInstance();
   void Reset_Scan_Index();
   void Reset_All_Object_Info();
   void Reset_Vehicle_Info();
   void Reset_Tracker_Info();
   void Reset_Processed_Detection_Info();
   void Reset_Vse_Output_Info();
   void Reset_Raw_Detection_Info();
   void Reset_Sensor_Info();
   void Reset_ROT_Info();
   void Reset_MountInfo_ptr();
   void Reset_InVehicleInfo_ptr();
   void Reset_GT_Info();
   void Reset_IpScanIdx_Info();
   void Reset_AllObjGtPtr_Info();
   void Reset_OLP_Object_Output();
   void Reset_Lcda_Feature_Output();
   void Reset_Ced_Feature_Output();
   void Reset_Esa_Feature_Output();
   void Reset_Ltb_Feature_Output();
   void Reset_Recw_Feature_Output();
   void Reset_Ta_Feature_Output();
   void Reset_Scw_Feature_Output();
   void Reset_Cta_Feature_Output();
   void Reset_Pt_Feature_Output();
   void Reset_SG_Info();
   void Reset_Faults_Info();
   void Reset_HDF_Buffers();
};

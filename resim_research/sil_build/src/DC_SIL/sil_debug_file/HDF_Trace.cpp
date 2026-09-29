#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#include <direct.h> // for _mkdir
#endif
#ifdef __GNUC__
#include <sys/stat.h>
#endif
#include <cstring>
#include <string>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "HDF_Trace.h"
#include "dc_app_version.h"
namespace fs = std::filesystem;
#define MAX_HDF_FILE_PATH (4200)

// locate versions.txt next to the binaries
#if defined(_WIN32)
#include <windows.h>
static std::string get_exe_dir() {
   char path[MAX_PATH] = {0};
   DWORD len           = GetModuleFileNameA(nullptr, path, MAX_PATH);
   if (len == 0 || len >= MAX_PATH)
      return ".";
   std::string p(path);
   auto pos = p.find_last_of("\\/");
   return (pos == std::string::npos) ? "." : p.substr(0, pos);
}
#else
#include <unistd.h>
#include <limits.h>
static std::string get_exe_dir() {
   char path[PATH_MAX] = {0};
   ssize_t len         = readlink("/proc/self/exe", path, sizeof(path) - 1);
   if (len <= 0)
      return ".";
   path[len] = '\0';
   std::string p(path);
   auto pos = p.find_last_of("\\/");
   return (pos == std::string::npos) ? "." : p.substr(0, pos);
}
#endif

// trim & parsing utilities
static inline void ltrim(std::string &s) {
   s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                   [](unsigned char ch) { return !std::isspace(ch); }));
}
static inline void rtrim(std::string &s) {
   s.erase(std::find_if(s.rbegin(), s.rend(),
                        [](unsigned char ch) { return !std::isspace(ch); })
               .base(),
           s.end());
}
static inline void trim(std::string &s) {
   ltrim(s);
   rtrim(s);
}

static std::unordered_map<std::string, std::string>
read_versions_txt(const std::string &full_path) {
   std::unordered_map<std::string, std::string> out;
   std::ifstream ifs(full_path);
   if (!ifs) {
      std::cerr << "[versions] Could not open: " << full_path << "\n";
      return out;
   }

   std::string line;
   while (std::getline(ifs, line)) {
      // skip comments / empty lines
      std::string tmp = line;
      trim(tmp);
      if (tmp.empty() || tmp.rfind("#", 0) == 0)
         continue;

      auto pos = line.find(':');
      if (pos == std::string::npos)
         continue;

      std::string key = line.substr(0, pos);
      std::string val = line.substr(pos + 1);
      trim(key);
      trim(val);
      if (!key.empty())
         out[key] = val;
   }
   return out;
}

static std::string get_exe_versions_path() {
   std::string dir = get_exe_dir();
#if defined(_WIN32)
   return dir + "\\versions.txt";
#else
   return dir + "/versions.txt";
#endif
}

HDFWriteClass *HDFWriteClass::instance = nullptr;

void HDFWriteClass::Init() {
   dc_input      = GetDCInputdata();
   trackInfo     = GetF360TrackerInfo();
   all_objects   = GetF360TrackerObject();
   SensInfo      = GetF360SensorInfo();
   DetLog        = GetF360DetObject();
   CoreInfo      = GetF360CoreInfo();
   ROT_Obj       = GetF360TrackerROTObject();
   SensCalib     = GetSensCalibPtr();
   ptr_host      = GetHostInfo();
   ptr_hostCalib = GetF360HostCalib();
   VsePtr        = GetVSEOutput();
   DetList       = GetDetectionList();
   olp_ptr       = GetSFLObjectPtr();
   lcda_ptr      = Lcda_Get_Output_Ptr();
   ced_ptr       = Ced_Get_Output_Ptr();
   esa_ptr       = Esa_Get_Output_Ptr();
   pt_ptr        = Pt_Get_Output_Ptr();
   ltb_ptr       = Ltb_Get_Output_Ptr();
   recw_ptr      = Recw_Get_Output_Ptr();
   ta_ptr        = Ta_Get_Output_Ptr();
   scw_ptr       = Scw_Get_Output_Ptr();
   cta_ptr       = Cta_Get_Output_Ptr();
   sg_ptr        = GetSGOutputPtr();
   faults        = GetFunctionalSafetyFaultsLogPtr();
}

HDFWriteClass *HDFWriteClass::getInstance() {
   if (instance == nullptr) {
      instance = new HDFWriteClass;
   }
   return instance;
}

void HDFWriteClass::Reset_Lcda_Feature_Output() {
   hdf_lcda_ptr.lcda_status.clear();
   hdf_lcda_ptr.f_bsw_enabled.clear();
   hdf_lcda_ptr.bsw_alert_left.clear();
   hdf_lcda_ptr.bsw_alert_right.clear();
   hdf_lcda_ptr.bsw_id_left.clear();
   hdf_lcda_ptr.bsw_id_right.clear();
   hdf_lcda_ptr.bsw_unique_id_left.clear();
   hdf_lcda_ptr.bsw_unique_id_right.clear();

   hdf_lcda_ptr.f_cvw_enabled.clear();
   hdf_lcda_ptr.cvw_alert_left.clear();
   hdf_lcda_ptr.cvw_alert_right.clear();
   hdf_lcda_ptr.cvw_id_left.clear();
   hdf_lcda_ptr.cvw_id_right.clear();
   hdf_lcda_ptr.cvw_unique_id_left.clear();
   hdf_lcda_ptr.cvw_unique_id_right.clear();
   hdf_lcda_ptr.cvw_ttc_s_left.clear();
   hdf_lcda_ptr.cvw_ttc_s_right.clear();
   hdf_lcda_ptr.f_slc_enabled.clear();
   hdf_lcda_ptr.slc_alert_left.clear();
   hdf_lcda_ptr.slc_alert_right.clear();
   hdf_lcda_ptr.slc_id_left.clear();
   hdf_lcda_ptr.slc_id_right.clear();
   hdf_lcda_ptr.slc_unique_id_left.clear();
   hdf_lcda_ptr.slc_unique_id_right.clear();
   hdf_lcda_ptr.slc_ttc_s_left.clear();
   hdf_lcda_ptr.slc_ttc_s_right.clear();

   hdf_lcda_ptr.slc_lane_change_probability_left.clear();
   hdf_lcda_ptr.slc_lane_change_probability_right.clear();
}

void HDFWriteClass::Reset_Scan_Index() {
   hdf_scan_index_ptr.scan_index.clear();
}

void HDFWriteClass::Reset_IpScanIdx_Info() {
   hdf_scan_indexIp_ptr.scan_index.clear();
}

void HDFWriteClass::Reset_Tracker_Info() {
   hdf_tracker_info_ptr.timestamp_us.clear();
   hdf_tracker_info_ptr.object_list_timestamp.clear();
   hdf_tracker_info_ptr.nr_suspected_stat_angle_jump_dets_filtered.clear();
   hdf_tracker_info_ptr.elapsed_time_s.clear();
   hdf_tracker_info_ptr.sw_version_buildID.clear();
   hdf_tracker_info_ptr.tracker_index.clear();
   hdf_tracker_info_ptr.vehicle_index.clear();
   hdf_tracker_info_ptr.active_obj_ids.clear();
   hdf_tracker_info_ptr.inactive_obj_ids.clear();
   hdf_tracker_info_ptr.num_active_objs.clear();
   hdf_tracker_info_ptr.reduced_active_obj_ids.clear();
   hdf_tracker_info_ptr.reduced_inactive_obj_ids.clear();
   hdf_tracker_info_ptr.reduced_obj_ids.clear();
   hdf_tracker_info_ptr.reduced_num_active_objs.clear();
   hdf_tracker_info_ptr.num_unique_objs.clear();
   hdf_tracker_info_ptr.f_severe_angle_jump_detected.clear();
   hdf_tracker_info_ptr.num_active_clusters.clear();
   hdf_tracker_info_ptr.number_of_historic_detections.clear();
   hdf_tracker_info_ptr.sw_version_major.clear();
   hdf_tracker_info_ptr.sw_version_minor.clear();
   hdf_tracker_info_ptr.sw_version_patch.clear();
}

void HDFWriteClass::Reset_GT_Info() {
   hdf_GT_ptr.id.clear();
   hdf_GT_ptr.status.clear();
   hdf_GT_ptr.age.clear();
   hdf_GT_ptr.stage_age.clear();
   hdf_GT_ptr.vcs_long_posn.clear();
   hdf_GT_ptr.vcs_long_vel.clear();
   hdf_GT_ptr.vcs_long_accel.clear();
   hdf_GT_ptr.vcs_lat_posn.clear();
   hdf_GT_ptr.vcs_lat_vel.clear();
   hdf_GT_ptr.vcs_lat_accel.clear();
   hdf_GT_ptr.vcs_long_vel_rel.clear();
   hdf_GT_ptr.vcs_lat_vel_rel.clear();
   hdf_GT_ptr.speed.clear();
   hdf_GT_ptr.tangential_accel.clear();
   hdf_GT_ptr.heading.clear();
   hdf_GT_ptr.heading_rate.clear();
   hdf_GT_ptr.length.clear();
   hdf_GT_ptr.width.clear();
   hdf_GT_ptr.object_class.clear();
}

void HDFWriteClass::Reset_Sensor_Info() {
   hdf_sensor_info_ptr.min_host_vel.clear();
   hdf_sensor_info_ptr.occurrence_lowerlimit.clear();
   hdf_sensor_info_ptr.occurrence_threshold.clear();
   hdf_sensor_info_ptr.rcs_tolerance.clear();
   hdf_sensor_info_ptr.azimuth_tolerance.clear();
   hdf_sensor_info_ptr.range_tolerance.clear();
   hdf_sensor_info_ptr.rcs_max.clear();
   hdf_sensor_info_ptr.range_max.clear();
   hdf_sensor_info_ptr.age_threshold.clear();
   hdf_sensor_info_ptr.f_enable.clear();
   hdf_sensor_info_ptr.useful_FOV.clear();
   hdf_sensor_info_ptr.vcs_lateral.clear();
   hdf_sensor_info_ptr.vcs_Longitude.clear();
   hdf_sensor_info_ptr.vcs_height.clear();
   hdf_sensor_info_ptr.vcs_boresight_azimuth_angle.clear();
   hdf_sensor_info_ptr.vcs_boresight_elevation_angle.clear();
   hdf_sensor_info_ptr.range_limits.clear();
   hdf_sensor_info_ptr.fov_min_az_rad.clear();
   hdf_sensor_info_ptr.fov_max_az_rad.clear();
   hdf_sensor_info_ptr.fov_min_el_rad.clear();
   hdf_sensor_info_ptr.fov_max_el_rad.clear();
   hdf_sensor_info_ptr.min_aliaised_range_rate.clear();
   hdf_sensor_info_ptr.v_wrapping.clear();
   hdf_sensor_info_ptr.r_wrapping.clear();
   hdf_sensor_info_ptr.id.clear();
   hdf_sensor_info_ptr.polarity.clear();
   hdf_sensor_info_ptr.mounting_location.clear();
   hdf_sensor_info_ptr.sensor_type.clear();
   hdf_sensor_info_ptr.timestamp_us.clear();
   hdf_sensor_info_ptr.vcs_velocityLat.clear();
   hdf_sensor_info_ptr.vcs_velocityLong.clear();
   hdf_sensor_info_ptr.vacs_boresight_az_estimated.clear();
   hdf_sensor_info_ptr.vacs_boresight_el_estimated.clear();
   hdf_sensor_info_ptr.number_of_valid_detections.clear();
   hdf_sensor_info_ptr.overall_rain_level.clear();
   hdf_sensor_info_ptr.look_index.clear();
   hdf_sensor_info_ptr.is_valid.clear();
   hdf_sensor_info_ptr.f_sensor_fault_detected.clear();
   hdf_sensor_info_ptr.look_id.clear();
}
void HDFWriteClass::Reset_MountInfo_ptr() {
   hdf_MountInfo_ptr.azimuth_polarity.clear();
   hdf_MountInfo_ptr.boresight_angle.clear();
   hdf_MountInfo_ptr.vcs_lat_position.clear();
   hdf_MountInfo_ptr.vcs_lon_position.clear();
   hdf_MountInfo_ptr.vcs_z_position.clear();
}

void HDFWriteClass::Reset_InVehicleInfo_ptr() {

   hdf_InVehicleInfo_ptr.abs_speed.clear();
   hdf_InVehicleInfo_ptr.yawrate.clear();
   hdf_InVehicleInfo_ptr.steering_angle.clear();
   hdf_InVehicleInfo_ptr.rear_axle_steering_angle.clear();
   hdf_InVehicleInfo_ptr.rear_axle_position.clear();
   hdf_InVehicleInfo_ptr.lane_width_external.clear();
   hdf_InVehicleInfo_ptr.lane_center_offset_external.clear();
   hdf_InVehicleInfo_ptr.host_vehicle_length.clear();
   hdf_InVehicleInfo_ptr.host_vehicle_width.clear();
   hdf_InVehicleInfo_ptr.host_vehicle_height.clear();
   hdf_InVehicleInfo_ptr.curve_radius.clear();
   hdf_InVehicleInfo_ptr.vcs_long_acc.clear();
   hdf_InVehicleInfo_ptr.vcs_lat_acc.clear();
   hdf_InVehicleInfo_ptr.bb_center_to_rear_x.clear();
   hdf_InVehicleInfo_ptr.bb_center_to_rear_y.clear();
   hdf_InVehicleInfo_ptr.bb_center_to_rear_z.clear();

   // 1D unsigned vectors
   hdf_InVehicleInfo_ptr.vehicle_data_buff_timestamp.clear();
   hdf_InVehicleInfo_ptr.prndl.clear();
   hdf_InVehicleInfo_ptr.turn_signal.clear();
   hdf_InVehicleInfo_ptr.f_reverse_gear.clear();
   hdf_InVehicleInfo_ptr.f_trailer_present.clear();
   hdf_InVehicleInfo_ptr.f_traffic_side.clear();

   // 2D vectors (nested)
   hdf_InVehicleInfo_ptr.ego_Sec.clear();
   hdf_InVehicleInfo_ptr.ego_FractionalSec.clear();
   hdf_InVehicleInfo_ptr.ego_vcs_long_acc.clear();
   hdf_InVehicleInfo_ptr.ego_valueQEgoAccelerationLongitudinalCog.clear();
   hdf_InVehicleInfo_ptr.ego_vcs_lat_acc.clear();
   hdf_InVehicleInfo_ptr.ego_valueQEgoAccelerationLateralCog.clear();
   hdf_InVehicleInfo_ptr.ego_yawrate.clear();
   hdf_InVehicleInfo_ptr.ego_valueQYawRateVehicleBody.clear();
   hdf_InVehicleInfo_ptr.ego_steering_angle.clear();
   hdf_InVehicleInfo_ptr.ego_abs_speed.clear();
   hdf_InVehicleInfo_ptr.ego_valueQEgoSpeedCog.clear();
   hdf_InVehicleInfo_ptr.ego_drivingDirectionConfirmed.clear();
}
void HDFWriteClass::Reset_ROT_Info() {
   hdf_ROT_info_ptr.rot_object_list_timestamp.clear();
   hdf_ROT_info_ptr.tracker_start_timestamp.clear();
   hdf_ROT_info_ptr.tracker_elapsed_time.clear();
   hdf_ROT_info_ptr.tracker_index.clear();
   hdf_ROT_info_ptr.number_of_objects.clear();

   hdf_ROT_info_ptr.vcs_x_posn.clear();
   hdf_ROT_info_ptr.vcs_y_posn.clear();
   hdf_ROT_info_ptr.vcs_x_vel.clear();
   hdf_ROT_info_ptr.vcs_y_vel.clear();
   hdf_ROT_info_ptr.vcs_x_acc.clear();
   hdf_ROT_info_ptr.vcs_y_acc.clear();
   hdf_ROT_info_ptr.vcs_heading.clear();
   hdf_ROT_info_ptr.vcs_pointing.clear();
   hdf_ROT_info_ptr.vcs_state_variance.clear();
   hdf_ROT_info_ptr.vcs_supplemental_state_covariance.clear();
   hdf_ROT_info_ptr.vcs_curvature.clear();

   hdf_ROT_info_ptr.iso_x_posn.clear();
   hdf_ROT_info_ptr.iso_y_posn.clear();
   hdf_ROT_info_ptr.iso_x_vel.clear();
   hdf_ROT_info_ptr.iso_relative_x_vel.clear();
   hdf_ROT_info_ptr.iso_y_vel.clear();
   hdf_ROT_info_ptr.iso_relative_y_vel.clear();
   hdf_ROT_info_ptr.iso_x_acc.clear();
   hdf_ROT_info_ptr.iso_relative_x_acc.clear();
   hdf_ROT_info_ptr.iso_y_acc.clear();
   hdf_ROT_info_ptr.iso_relative_y_acc.clear();

   hdf_ROT_info_ptr.iso_orientation.clear();
   hdf_ROT_info_ptr.iso_orientation_var.clear();
   hdf_ROT_info_ptr.iso_orientation_rate.clear();
   hdf_ROT_info_ptr.iso_orientation_rate_var.clear();

   hdf_ROT_info_ptr.iso_x_posn_var.clear();
   hdf_ROT_info_ptr.iso_y_posn_var.clear();
   hdf_ROT_info_ptr.iso_xy_posn_cov.clear();
   hdf_ROT_info_ptr.iso_x_vel_var.clear();
   hdf_ROT_info_ptr.iso_y_vel_var.clear();
   hdf_ROT_info_ptr.iso_xy_vel_cov.clear();
   hdf_ROT_info_ptr.iso_x_acc_var.clear();
   hdf_ROT_info_ptr.iso_y_acc_var.clear();
   hdf_ROT_info_ptr.iso_xy_acc_cov.clear();

   hdf_ROT_info_ptr.speed.clear();
   hdf_ROT_info_ptr.tang_accel.clear();
   hdf_ROT_info_ptr.length.clear();
   hdf_ROT_info_ptr.length_var.clear();
   hdf_ROT_info_ptr.width.clear();
   hdf_ROT_info_ptr.width_var.clear();
   hdf_ROT_info_ptr.time_since_created.clear();
   hdf_ROT_info_ptr.time_since_published.clear();
   hdf_ROT_info_ptr.time_since_stage_start.clear();
   hdf_ROT_info_ptr.existence_probability.clear();
   hdf_ROT_info_ptr.mirror_prob.clear();
   hdf_ROT_info_ptr.radar_cross_section.clear();
   hdf_ROT_info_ptr.otg_height.clear();
   hdf_ROT_info_ptr.confidence_level.clear();

   hdf_ROT_info_ptr.probability_pedestrian.clear();
   hdf_ROT_info_ptr.probability_car.clear();
   hdf_ROT_info_ptr.probability_motorcycle.clear();
   hdf_ROT_info_ptr.probability_bicycle.clear();
   hdf_ROT_info_ptr.probability_truck.clear();
   hdf_ROT_info_ptr.probability_undet.clear();
   hdf_ROT_info_ptr.probability_underdrivable_ocg.clear();
   hdf_ROT_info_ptr.movable_prob.clear();

   hdf_ROT_info_ptr.id.clear();
   hdf_ROT_info_ptr.unique_id.clear();
   hdf_ROT_info_ptr.ndets.clear();
   hdf_ROT_info_ptr.num_dets_used_in_rr_msmt_update.clear();
   hdf_ROT_info_ptr.sensor_src.clear();

   hdf_ROT_info_ptr.reference_point.clear();
   hdf_ROT_info_ptr.object_status.clear();
   hdf_ROT_info_ptr.object_class.clear();
   hdf_ROT_info_ptr.movement_status.clear();
   hdf_ROT_info_ptr.occlusion_status.clear();
   hdf_ROT_info_ptr.underdrivable_status_ocg.clear();
   hdf_ROT_info_ptr.drivable_status_sg.clear();
   hdf_ROT_info_ptr.drivable_confidence_sg.clear();
   hdf_ROT_info_ptr.f_onguardrail.clear();
   hdf_ROT_info_ptr.trk_fltr_type.clear();
}
void HDFWriteClass::Reset_Raw_Detection_Info() {
   hdf_raw_detection_info_ptr.timestamp.clear();
   hdf_raw_detection_info_ptr.hdrTimestamp_fractionalSec.clear();
   hdf_raw_detection_info_ptr.hdrTimestamp_Sec.clear();
   hdf_raw_detection_info_ptr.AutoAlignElevation.clear();
   hdf_raw_detection_info_ptr.Count.clear();
   hdf_raw_detection_info_ptr.ScanIndex.clear();
   hdf_raw_detection_info_ptr.AutoAlignAzimuthQF.clear();
   hdf_raw_detection_info_ptr.AutoAlignElevationQF.clear();
   hdf_raw_detection_info_ptr.timestamp_consistency.clear();
   hdf_raw_detection_info_ptr.LookID.clear();
   hdf_raw_detection_info_ptr.LookType.clear();

   hdf_raw_detection_info_ptr.elevation.clear();
   hdf_raw_detection_info_ptr.azimuth.clear();
   hdf_raw_detection_info_ptr.range_rate.clear();
   hdf_raw_detection_info_ptr.range.clear();
   hdf_raw_detection_info_ptr.amplitude.clear();
   hdf_raw_detection_info_ptr.snr.clear();
   hdf_raw_detection_info_ptr.std_elevation.clear();
   hdf_raw_detection_info_ptr.std_azimuth.clear();
   hdf_raw_detection_info_ptr.std_rcs.clear();
   hdf_raw_detection_info_ptr.std_range_rate.clear();
   hdf_raw_detection_info_ptr.std_range.clear();
   hdf_raw_detection_info_ptr.std_rcs.clear();
   hdf_raw_detection_info_ptr.multi_target_probability.clear();
   hdf_raw_detection_info_ptr.existence_probability.clear();
   hdf_raw_detection_info_ptr.azimuth_confidence.clear();
   hdf_raw_detection_info_ptr.elevation_confidence.clear();
   hdf_raw_detection_info_ptr.valid.clear();
   hdf_raw_detection_info_ptr.host_veh_clutter.clear();
   hdf_raw_detection_info_ptr.nd_target.clear();
   hdf_raw_detection_info_ptr.bistatic.clear();
}

void HDFWriteClass::Reset_Processed_Detection_Info() {
   hdf_processed_info_ptr.number_of_valid_detections.clear();
   hdf_processed_info_ptr.vcslong_det_idx_min.clear();
   hdf_processed_info_ptr.vcslong_det_idx_max.clear();
   hdf_processed_info_ptr.vcslong_sorted_ref_det_idx.clear();

   // RD_* fields
   hdf_processed_info_ptr.RD_range.clear();
   hdf_processed_info_ptr.RD_std_range.clear();
   hdf_processed_info_ptr.RD_range_rate.clear();
   hdf_processed_info_ptr.RD_std_range_rate.clear();
   hdf_processed_info_ptr.RD_azimuth.clear();
   hdf_processed_info_ptr.RD_std_azimuth.clear();
   hdf_processed_info_ptr.RD_elevation.clear();
   hdf_processed_info_ptr.RD_std_elevation.clear();
   hdf_processed_info_ptr.RD_snr.clear();
   hdf_processed_info_ptr.RD_rcs.clear();
   hdf_processed_info_ptr.RD_prob_1stazhypo.clear();
   hdf_processed_info_ptr.RD_sensor_id.clear();
   hdf_processed_info_ptr.RD_det_id.clear();
   hdf_processed_info_ptr.RD_confid_azimuth.clear();
   hdf_processed_info_ptr.RD_confid_elevation.clear();
   hdf_processed_info_ptr.RD_f_super_res.clear();
   hdf_processed_info_ptr.RD_f_host_veh_clutter.clear();
   hdf_processed_info_ptr.RD_f_nd_target.clear();
   hdf_processed_info_ptr.RD_f_bistatic.clear();
   hdf_processed_info_ptr.RD_f_ci_det.clear();
   hdf_processed_info_ptr.RD_f_idm_det.clear();
   hdf_processed_info_ptr.RD_f_below_rain_thold.clear();

   // PD_* fields
   hdf_processed_info_ptr.PD_vcs_position_x.clear();
   hdf_processed_info_ptr.PD_vcs_position_y.clear();
   hdf_processed_info_ptr.PD_vcs_position_z.clear();
   hdf_processed_info_ptr.PD_range_rate_compensated.clear();
   hdf_processed_info_ptr.PD_vcs_az.clear();
   hdf_processed_info_ptr.PD_vcs_el.clear();
   hdf_processed_info_ptr.PD_cos_vcs_az.clear();
   hdf_processed_info_ptr.PD_sin_vcs_az.clear();
   hdf_processed_info_ptr.PD_next_sorted_idx.clear();
   hdf_processed_info_ptr.PD_prev_sorted_idx.clear();
   hdf_processed_info_ptr.PD_motion_status.clear();
   hdf_processed_info_ptr.PD_f_ok_to_use.clear();
}

void HDFWriteClass::Reset_Vehicle_Info() {
   hdf_vehicle_info_ptr.vehicle_index.clear();
   hdf_vehicle_info_ptr.speed.clear();
   hdf_vehicle_info_ptr.acceleration.clear();
   hdf_vehicle_info_ptr.vcs_lat_acceleration.clear();
   hdf_vehicle_info_ptr.vcs_long_acceleration.clear();
   hdf_vehicle_info_ptr.yaw_rate_rad.clear();
   hdf_vehicle_info_ptr.vcs_sideslip.clear();
   hdf_vehicle_info_ptr.curvature_rear.clear();
   hdf_vehicle_info_ptr.dist_rear_axle_to_vcs_m.clear();
   hdf_vehicle_info_ptr.vehicle_length.clear();
   hdf_vehicle_info_ptr.vehicle_width.clear();
   hdf_vehicle_info_ptr.rear_cornering_compliance.clear();
   hdf_vehicle_info_ptr.speed_correction_factor.clear();
   hdf_vehicle_info_ptr.host_type.clear();
   hdf_vehicle_info_ptr.f_trailer_presence_hardware.clear();
   hdf_vehicle_info_ptr.speed_qf.clear();
   hdf_vehicle_info_ptr.yaw_rate_qf.clear();
   hdf_vehicle_info_ptr.lat_accel_qf.clear();
   hdf_vehicle_info_ptr.long_accel_qf.clear();
   hdf_vehicle_info_ptr.dist_rear_axle_to_vcs_m.clear();
   hdf_vehicle_info_ptr.steer_gear_ratio.clear();
   hdf_vehicle_info_ptr.wheelbase_m.clear();
   hdf_vehicle_info_ptr.understeer_coefficient.clear();
   hdf_vehicle_info_ptr.cog_x.clear();
   hdf_vehicle_info_ptr.cog_y.clear();
   hdf_vehicle_info_ptr.front_wheel_radius_m.clear();
   hdf_vehicle_info_ptr.front_track_width_m.clear();
   hdf_vehicle_info_ptr.raw_host_signal_latency_ms.clear();
   hdf_vehicle_info_ptr.f_enable_internal_reflections_func.clear();
}

void HDFWriteClass::Reset_All_Object_Info() {
   hdf_all_objects_ptr.unique_id.clear();
   hdf_all_objects_ptr.num_elements.clear();
   hdf_all_objects_ptr.reference_point.clear();
   hdf_all_objects_ptr.drivable_status_sg.clear();
   hdf_all_objects_ptr.version.clear();
   hdf_all_objects_ptr.num_elements.clear();
   hdf_all_objects_ptr.data_timstamp_us.clear();
   hdf_all_objects_ptr.drivable_confidence_sg.clear();
   hdf_all_objects_ptr.otg_height.clear();
   hdf_all_objects_ptr.probability_underdrivable_ocg.clear();
   hdf_all_objects_ptr.radar_cross_section.clear();
   hdf_all_objects_ptr.num_rr_inlier_dets.clear();
   hdf_all_objects_ptr.data_timstamp_us.clear();
   hdf_all_objects_ptr.num_dets_used_in_rr_msmt_update.clear();
   hdf_all_objects_ptr.vcs_xposn.clear();
   hdf_all_objects_ptr.vcs_yposn.clear();
   hdf_all_objects_ptr.vcs_xvel.clear();
   hdf_all_objects_ptr.vcs_yvel.clear();
   hdf_all_objects_ptr.vcs_xaccel.clear();
   hdf_all_objects_ptr.vcs_yaccel.clear();
   hdf_all_objects_ptr.vcs_heading.clear();
   hdf_all_objects_ptr.vcs_pointing.clear();
   hdf_all_objects_ptr.speed.clear();
   hdf_all_objects_ptr.curvature.clear();
   hdf_all_objects_ptr.tang_accel.clear();
   hdf_all_objects_ptr.state_variance.clear();
   hdf_all_objects_ptr.supplemental_state_covariance.clear();
   hdf_all_objects_ptr.time_since_measurement.clear();
   hdf_all_objects_ptr.time_since_cluster_created.clear();
   hdf_all_objects_ptr.time_since_track_updated.clear();
   hdf_all_objects_ptr.len1.clear();
   hdf_all_objects_ptr.len2.clear();
   hdf_all_objects_ptr.wid1.clear();
   hdf_all_objects_ptr.wid2.clear();
   hdf_all_objects_ptr.confidenceLevel.clear();
   hdf_all_objects_ptr.time_since_stage_start.clear();
   hdf_all_objects_ptr.existence_probability.clear();
   hdf_all_objects_ptr.accuracy_length.clear();
   hdf_all_objects_ptr.accuracy_width.clear();
   hdf_all_objects_ptr.probability_pedestrian.clear();
   hdf_all_objects_ptr.probability_car.clear();
   hdf_all_objects_ptr.probability_motorcycle.clear();
   hdf_all_objects_ptr.probability_bicycle.clear();
   hdf_all_objects_ptr.probability_truck.clear();
   hdf_all_objects_ptr.probability_undet.clear();
   hdf_all_objects_ptr.trkID.clear();
   hdf_all_objects_ptr.ndets.clear();
   hdf_all_objects_ptr.num_reduced_dets.clear();
   hdf_all_objects_ptr.reducedID.clear();
   hdf_all_objects_ptr.trk_fltr_type.clear();
   hdf_all_objects_ptr.status.clear();
   hdf_all_objects_ptr.reducedStatus.clear();
   hdf_all_objects_ptr.init_scheme.clear();
   hdf_all_objects_ptr.object_class.clear();
   hdf_all_objects_ptr.f_crossing.clear();
   hdf_all_objects_ptr.f_moving.clear();
   hdf_all_objects_ptr.f_moveable.clear();
   hdf_all_objects_ptr.f_oncoming.clear();
   hdf_all_objects_ptr.f_vehicular_trk.clear();
   hdf_all_objects_ptr.f_onguardrail.clear();
   hdf_all_objects_ptr.f_fast_moving.clear();
   hdf_all_objects_ptr.underdrivable_status.clear();
}

void HDFWriteClass::Reset_AllObjGtPtr_Info() {

   hdf_all_objectsGT_ptr.unique_id.clear();
   hdf_all_objectsGT_ptr.vcs_xposn.clear();
   hdf_all_objectsGT_ptr.vcs_yposn.clear();
   hdf_all_objectsGT_ptr.vcs_xvel.clear();
   hdf_all_objectsGT_ptr.vcs_yvel.clear();
   hdf_all_objectsGT_ptr.vcs_xaccel.clear();
   hdf_all_objectsGT_ptr.vcs_yaccel.clear();
   hdf_all_objectsGT_ptr.vcs_heading.clear();
   hdf_all_objectsGT_ptr.speed.clear();
   hdf_all_objectsGT_ptr.tang_accel.clear();
   hdf_all_objectsGT_ptr.len1.clear();
   hdf_all_objectsGT_ptr.wid1.clear();
   hdf_all_objectsGT_ptr.object_class.clear();
   hdf_all_objectsGT_ptr.underdrivable_status.clear();
}

void HDFWriteClass::Reset_Vse_Output_Info() {
   hdf_vse_output_ptr.timestamp_us.clear();
   hdf_vse_output_ptr.raw_speed_mps.clear();
   hdf_vse_output_ptr.speed_compensation_factor.clear();
   hdf_vse_output_ptr.filt_veh_speed_over_ground.clear();
   hdf_vse_output_ptr.raw_lat_accel.clear();
   hdf_vse_output_ptr.raw_long_accel.clear();
   hdf_vse_output_ptr.raw_yaw_rate_rps.clear();
   hdf_vse_output_ptr.raw_steering_angle_deg.clear();
   hdf_vse_output_ptr.road_wheel_angle_deg.clear();
   hdf_vse_output_ptr.yaw_rate_sa.clear();
   hdf_vse_output_ptr.yaw_rate_raw_bias.clear();
   hdf_vse_output_ptr.comp_yaw_rate_unfiltered.clear();
   hdf_vse_output_ptr.comp_yaw_rate_filtered.clear();
   hdf_vse_output_ptr.curvature_rear_axle.clear();
   hdf_vse_output_ptr.sideslip_rear_axle.clear();
   hdf_vse_output_ptr.vcs_sideslip.clear();
   hdf_vse_output_ptr.vcs_long_velocity.clear();
   hdf_vse_output_ptr.vcs_lat_velocity.clear();
   hdf_vse_output_ptr.sensor_sideslip.clear();
   hdf_vse_output_ptr.sensor_long_velocity.clear();
   hdf_vse_output_ptr.sensor_lat_velocity.clear();
   hdf_vse_output_ptr.k_dist_rear_axle_to_vcs.clear();
   hdf_vse_output_ptr.vcs_lat_accel.clear();
   hdf_vse_output_ptr.vcs_long_accel.clear();
   hdf_vse_output_ptr.accel_rear_axle.clear();
   hdf_vse_output_ptr.signed_filt_veh_speed_over_ground.clear();
   hdf_vse_output_ptr.veh_index.clear();
   hdf_vse_output_ptr.raw_speed_qf.clear();
   hdf_vse_output_ptr.speed_compensation_factor_qf.clear();
   hdf_vse_output_ptr.filt_veh_speed_over_ground_qf.clear();
   hdf_vse_output_ptr.raw_lat_accel_qf.clear();
   hdf_vse_output_ptr.raw_long_accel_qf.clear();
   hdf_vse_output_ptr.raw_yaw_rate_qf.clear();
   hdf_vse_output_ptr.raw_steering_angle_qf.clear();
   hdf_vse_output_ptr.road_wheel_angle_qf.clear();
   hdf_vse_output_ptr.yaw_rate_sa_qf.clear();
   hdf_vse_output_ptr.yaw_rate_bias_qf.clear();
   hdf_vse_output_ptr.comp_yaw_rate_qf.clear();
   hdf_vse_output_ptr.stationary.clear();
   hdf_vse_output_ptr.vcs_lat_accel_qf.clear();
   hdf_vse_output_ptr.vcs_long_accel_qf.clear();
}

void HDFWriteClass::Reset_OLP_Object_Output() {
   hdf_olp_output_ptr.n_valid_objects.clear();
   hdf_olp_output_ptr.id.clear();
   hdf_olp_output_ptr.unique_id.clear();
   hdf_olp_output_ptr.index.clear();
   hdf_olp_output_ptr.status.clear();
   hdf_olp_output_ptr.age.clear();
   hdf_olp_output_ptr.stage_age.clear();
   hdf_olp_output_ptr.fbk_stage_age.clear();
   hdf_olp_output_ptr.existence_probability.clear();
   hdf_olp_output_ptr.speed.clear();
   hdf_olp_output_ptr.vcs_pos_x.clear();
   hdf_olp_output_ptr.vcs_pos_y.clear();
   hdf_olp_output_ptr.vcs_vel_x.clear();
   hdf_olp_output_ptr.vcs_vel_y.clear();
   hdf_olp_output_ptr.vcs_vel_rel_x.clear();
   hdf_olp_output_ptr.vcs_vel_rel_y.clear();
   hdf_olp_output_ptr.vcs_accel_x.clear();
   hdf_olp_output_ptr.vcs_accel_y.clear();
   hdf_olp_output_ptr.vcs_heading.clear();
   hdf_olp_output_ptr.heading_rate.clear();
   hdf_olp_output_ptr.heading_variance.clear();
   hdf_olp_output_ptr.accuracy_heading.clear();
   hdf_olp_output_ptr.eclipse_value.clear();
   hdf_olp_output_ptr.length.clear();
   hdf_olp_output_ptr.width.clear();
   hdf_olp_output_ptr.obj_distance.clear();
   hdf_olp_output_ptr.obstruction_prob.clear();
   hdf_olp_output_ptr.obj_class.clear();
   hdf_olp_output_ptr.class_prob_pedestrian.clear();
   hdf_olp_output_ptr.class_prob_2wheel.clear();
   hdf_olp_output_ptr.class_prob_car.clear();
   hdf_olp_output_ptr.class_prob_truck.clear();
   hdf_olp_output_ptr.id_merged_obj.clear();
   hdf_olp_output_ptr.f_merge_occured.clear();
   hdf_olp_output_ptr.curvi_coordinates_calc_method.clear();
   hdf_olp_output_ptr.curvi_pos_x.clear();
   hdf_olp_output_ptr.curvi_pos_y.clear();
   hdf_olp_output_ptr.curvi_vel_x.clear();
   hdf_olp_output_ptr.curvi_vel_y.clear();
   hdf_olp_output_ptr.curvi_vel_rel_x.clear();
   hdf_olp_output_ptr.curvi_vel_rel_y.clear();
   hdf_olp_output_ptr.curvi_heading.clear();
   hdf_olp_output_ptr.f_reflection.clear();
   hdf_olp_output_ptr.f_stationary.clear();
   hdf_olp_output_ptr.f_moveable.clear();
   hdf_olp_output_ptr.f_stationary_clutter.clear();
   hdf_olp_output_ptr.f_is_fl_origin_sensor.clear();
   hdf_olp_output_ptr.f_is_fr_origin_sensor.clear();
   hdf_olp_output_ptr.f_is_rl_origin_sensor.clear();
   hdf_olp_output_ptr.f_is_rr_origin_sensor.clear();
   hdf_olp_output_ptr.f_is_in_fl_sensor_fov.clear();
   hdf_olp_output_ptr.f_is_in_fr_sensor_fov.clear();
   hdf_olp_output_ptr.f_is_in_rl_sensor_fov.clear();
   hdf_olp_output_ptr.f_is_in_rr_sensor_fov.clear();
}

void HDFWriteClass::Reset_Ced_Feature_Output() {
   // Clear CED enable flag
   hdf_ced_ptr.f_ced_enable.clear();

   // Clear CED alerts
   hdf_ced_ptr.ced_alert_left.clear();
   hdf_ced_ptr.ced_alert_right.clear();

   // Clear left side object data
   hdf_ced_ptr.ced_object_ced_id_left.clear();
   hdf_ced_ptr.ced_object_unique_id_left.clear();
   hdf_ced_ptr.ced_object_type_left.clear();
   hdf_ced_ptr.ced_object_length_m_left.clear();
   hdf_ced_ptr.ced_object_width_m_left.clear();
   hdf_ced_ptr.ced_object_lat_pos_m_left.clear();
   hdf_ced_ptr.ced_object_long_pos_m_left.clear();
   hdf_ced_ptr.ced_object_speed_mps_left.clear();
   hdf_ced_ptr.ced_object_heading_rad_left.clear();
   hdf_ced_ptr.ced_object_direction_left.clear();
   hdf_ced_ptr.ced_object_predicted_lat_pos_m_left.clear();
   hdf_ced_ptr.ced_object_ttc_s_left.clear();
   hdf_ced_ptr.ced_object_ttp_s_left.clear();

   // Clear right side object data
   hdf_ced_ptr.ced_object_ced_id_right.clear();
   hdf_ced_ptr.ced_object_unique_id_right.clear();
   hdf_ced_ptr.ced_object_type_right.clear();
   hdf_ced_ptr.ced_object_length_m_right.clear();
   hdf_ced_ptr.ced_object_width_m_right.clear();
   hdf_ced_ptr.ced_object_lat_pos_m_right.clear();
   hdf_ced_ptr.ced_object_long_pos_m_right.clear();
   hdf_ced_ptr.ced_object_speed_mps_right.clear();
   hdf_ced_ptr.ced_object_heading_rad_right.clear();
   hdf_ced_ptr.ced_object_direction_right.clear();
   hdf_ced_ptr.ced_object_predicted_lat_pos_m_right.clear();
   hdf_ced_ptr.ced_object_ttc_s_right.clear();
   hdf_ced_ptr.ced_object_ttp_s_right.clear();
}

void HDFWriteClass::Reset_Esa_Feature_Output() {
   // Clear ESA status flag
   hdf_esa_ptr.esa_status.clear();

   // Clear ESA alerts
   hdf_esa_ptr.f_esa_alert_left.clear();
   hdf_esa_ptr.f_esa_alert_right.clear();

   // Clear left side object data
   hdf_esa_ptr.esa_object_id_left.clear();
   hdf_esa_ptr.esa_object_index_left.clear();
   hdf_esa_ptr.esa_object_width_m_left.clear();
   hdf_esa_ptr.esa_object_length_m_left.clear();
   hdf_esa_ptr.esa_object_long_pos_m_left.clear();
   hdf_esa_ptr.esa_object_lat_pos_m_left.clear();
   hdf_esa_ptr.esa_object_long_speed_mps_left.clear();
   hdf_esa_ptr.esa_object_lat_speed_mps_left.clear();
   hdf_esa_ptr.esa_object_ttc_s_left.clear();
   hdf_esa_ptr.esa_object_ttp_s_left.clear();
   hdf_esa_ptr.esa_object_decel_to_reach_host_speed_mps2_left.clear();
   hdf_esa_ptr.esa_object_long_distance_m_left.clear();
   hdf_esa_ptr.esa_object_existence_prob_left.clear();

   // Clear right side object data
   hdf_esa_ptr.esa_object_id_right.clear();
   hdf_esa_ptr.esa_object_index_right.clear();
   hdf_esa_ptr.esa_object_width_m_right.clear();
   hdf_esa_ptr.esa_object_length_m_right.clear();
   hdf_esa_ptr.esa_object_long_pos_m_right.clear();
   hdf_esa_ptr.esa_object_lat_pos_m_right.clear();
   hdf_esa_ptr.esa_object_long_speed_mps_right.clear();
   hdf_esa_ptr.esa_object_lat_speed_mps_right.clear();
   hdf_esa_ptr.esa_object_ttc_s_right.clear();
   hdf_esa_ptr.esa_object_ttp_s_right.clear();
   hdf_esa_ptr.esa_object_decel_to_reach_host_speed_mps2_right.clear();
   hdf_esa_ptr.esa_object_long_distance_m_right.clear();
   hdf_esa_ptr.esa_object_existence_prob_right.clear();
}

void HDFWriteClass::Reset_Ltb_Feature_Output() {
   // Clear left side object data vectors
   hdf_ltb_ptr.ltb_object_ltb_id_left.clear();
   hdf_ltb_ptr.ltb_object_ltb_ttc_s_left.clear();
   hdf_ltb_ptr.ltb_object_ltb_ttb_s_left.clear();
   hdf_ltb_ptr.ltb_object_ltb_decel_estimate_mps2_left.clear();
   hdf_ltb_ptr.ltb_object_ltb_distance_m_left.clear();

   // Clear right side object data vectors
   hdf_ltb_ptr.ltb_object_ltb_id_right.clear();
   hdf_ltb_ptr.ltb_object_ltb_ttc_s_right.clear();
   hdf_ltb_ptr.ltb_object_ltb_ttb_s_right.clear();
   hdf_ltb_ptr.ltb_object_ltb_decel_estimate_mps2_right.clear();
   hdf_ltb_ptr.ltb_object_ltb_distance_m_right.clear();

   // Clear alert levels and critical side vectors
   hdf_ltb_ptr.ltb_alert_level_left.clear();
   hdf_ltb_ptr.ltb_alert_level_right.clear();
   hdf_ltb_ptr.ltb_most_critical_side.clear();
}

void HDFWriteClass::Reset_Recw_Feature_Output() {
   hdf_recw_ptr.recw_crash_probability.clear();
   hdf_recw_ptr.recw_ttc_s.clear();
   hdf_recw_ptr.recw_id.clear();
   hdf_recw_ptr.recw_unique_id.clear();
   hdf_recw_ptr.recw_alert_level.clear();
   hdf_recw_ptr.ttc_threshold_alert_level_1_s.clear();
   hdf_recw_ptr.ttc_threshold_alert_level_2_s.clear();
}

void HDFWriteClass::Reset_Ta_Feature_Output() {
   hdf_ta_ptr.f_ta_enable.clear();
   hdf_ta_ptr.ta_f_vehicle_state_relevant.clear();
   hdf_ta_ptr.ta_most_critical_side.clear();
   hdf_ta_ptr.ta_n_valid_objects.clear();
   hdf_ta_ptr.ta_n_relevant_objects.clear();
   hdf_ta_ptr.ta_n_critical_objects.clear();
   hdf_ta_ptr.ta_algorithm_state.clear();
   hdf_ta_ptr.ta_alert_level_left.clear();
   hdf_ta_ptr.ta_alert_level_right.clear();

   // Clear left side critical object data
   hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_x_left.clear();
   hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_y_left.clear();
   hdf_ta_ptr.ta_object_ta_ttc_s_left.clear();
   hdf_ta_ptr.ta_object_ta_ttp_s_left.clear();
   hdf_ta_ptr.ta_object_ta_ttb_s_left.clear();
   hdf_ta_ptr.ta_object_ta_decel_estimate_mps2_left.clear();
   hdf_ta_ptr.ta_object_ta_distance_m_left.clear();
   hdf_ta_ptr.ta_object_ta_id_left.clear();
   hdf_ta_ptr.ta_object_ta_index_left.clear();
   hdf_ta_ptr.ta_object_ta_f_obj_in_danger_zone_left.clear();
   hdf_ta_ptr.ta_object_ta_f_obj_in_info_zone_left.clear();
   hdf_ta_ptr.ta_object_ta_f_obj_in_wing_zone_left.clear();

   // Clear right side critical object data
   hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_x_right.clear();
   hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_y_right.clear();
   hdf_ta_ptr.ta_object_ta_ttc_s_right.clear();
   hdf_ta_ptr.ta_object_ta_ttp_s_right.clear();
   hdf_ta_ptr.ta_object_ta_ttb_s_right.clear();
   hdf_ta_ptr.ta_object_ta_decel_estimate_mps2_right.clear();
   hdf_ta_ptr.ta_object_ta_distance_m_right.clear();
   hdf_ta_ptr.ta_object_ta_id_right.clear();
   hdf_ta_ptr.ta_object_ta_index_right.clear();
   hdf_ta_ptr.ta_object_ta_f_obj_in_danger_zone_right.clear();
   hdf_ta_ptr.ta_object_ta_f_obj_in_info_zone_right.clear();
   hdf_ta_ptr.ta_object_ta_f_obj_in_wing_zone_right.clear();
}

void HDFWriteClass::Reset_Scw_Feature_Output() {
   hdf_scw_ptr.f_scw_enabled.clear();
   hdf_scw_ptr.f_scw_dyn_enabled.clear();
   hdf_scw_ptr.f_scw_guardrail_enabled.clear();

   // Clear left side object data
   hdf_scw_ptr.scw_object_alert_level_left.clear();
   hdf_scw_ptr.scw_object_id_left.clear();
   hdf_scw_ptr.scw_object_unique_id_left.clear();
   hdf_scw_ptr.scw_object_type_left.clear();
   hdf_scw_ptr.scw_object_lateral_ttc_s_left.clear();
   hdf_scw_ptr.scw_object_lateral_distance_m_left.clear();
   hdf_scw_ptr.scw_object_position_m_x_left.clear();
   hdf_scw_ptr.scw_object_position_m_y_left.clear();
   hdf_scw_ptr.scw_object_velocity_mps_x_left.clear();
   hdf_scw_ptr.scw_object_velocity_mps_y_left.clear();
   hdf_scw_ptr.scw_object_acceleration_mps2_x_left.clear();
   hdf_scw_ptr.scw_object_acceleration_mps2_y_left.clear();
   hdf_scw_ptr.scw_object_width_m_left.clear();
   hdf_scw_ptr.scw_object_length_m_left.clear();
   hdf_scw_ptr.scw_object_heading_rad_left.clear();
   hdf_scw_ptr.scw_object_existence_probability_left.clear();
   hdf_scw_ptr.scw_object_age_left.clear();

   // Clear right side object data
   hdf_scw_ptr.scw_object_alert_level_right.clear();
   hdf_scw_ptr.scw_object_id_right.clear();
   hdf_scw_ptr.scw_object_unique_id_right.clear();
   hdf_scw_ptr.scw_object_type_right.clear();
   hdf_scw_ptr.scw_object_lateral_ttc_s_right.clear();
   hdf_scw_ptr.scw_object_lateral_distance_m_right.clear();
   hdf_scw_ptr.scw_object_position_m_x_right.clear();
   hdf_scw_ptr.scw_object_position_m_y_right.clear();
   hdf_scw_ptr.scw_object_velocity_mps_x_right.clear();
   hdf_scw_ptr.scw_object_velocity_mps_y_right.clear();
   hdf_scw_ptr.scw_object_acceleration_mps2_x_right.clear();
   hdf_scw_ptr.scw_object_acceleration_mps2_y_right.clear();
   hdf_scw_ptr.scw_object_width_m_right.clear();
   hdf_scw_ptr.scw_object_length_m_right.clear();
   hdf_scw_ptr.scw_object_heading_rad_right.clear();
   hdf_scw_ptr.scw_object_existence_probability_right.clear();
   hdf_scw_ptr.scw_object_age_right.clear();
}

void HDFWriteClass::Reset_Pt_Feature_Output() {
   hdf_pt_ptr.range_vcs_proj_to_path_segment.clear();
   hdf_pt_ptr.segment_heading_diff.clear();
   hdf_pt_ptr.track_idx_nearest_path.clear();
   hdf_pt_ptr.range_at_zero.clear();
   hdf_pt_ptr.range_to_current_path_part.clear();
   hdf_pt_ptr.range_at_host_edge.clear();
   hdf_pt_ptr.length_of_trajectory.clear();
   hdf_pt_ptr.path_heading.clear();
   hdf_pt_ptr.path_direction.clear();
   hdf_pt_ptr.track_match.clear();
   hdf_pt_ptr.track_match_age.clear();
   hdf_pt_ptr.track_match_last_cycle.clear();
   hdf_pt_ptr.f_pt_operational.clear();
}

void HDFWriteClass::Reset_Cta_Feature_Output() {
   hdf_cta_ptr.f_cta_enabled.clear();

   // Clear left side object data
   hdf_cta_ptr.most_critical_object_by_sides_id_left.clear();
   hdf_cta_ptr.most_critical_object_by_sides_unique_id_left.clear();
   hdf_cta_ptr.most_critical_object_by_sides_objPoseX_m_left.clear();
   hdf_cta_ptr.most_critical_object_by_sides_objPoseY_m_left.clear();
   hdf_cta_ptr.most_critical_object_by_sides_objVelocityX_mps_left.clear();
   hdf_cta_ptr.most_critical_object_by_sides_objVelocityY_mps_left.clear();
   hdf_cta_ptr.most_critical_object_by_sides_heading_rad_left.clear();
   hdf_cta_ptr.most_critical_object_by_sides_alert_level_left.clear();
   hdf_cta_ptr.most_critical_object_by_sides_ttc_s_left.clear();
   hdf_cta_ptr.most_critical_object_by_sides_intersection_point_x_m_left.clear();
   hdf_cta_ptr.most_critical_object_by_sides_f_brake_qualifier_left.clear();

   // Clear right side object data
   hdf_cta_ptr.most_critical_object_by_sides_id_right.clear();
   hdf_cta_ptr.most_critical_object_by_sides_unique_id_right.clear();
   hdf_cta_ptr.most_critical_object_by_sides_objPoseX_m_right.clear();
   hdf_cta_ptr.most_critical_object_by_sides_objPoseY_m_right.clear();
   hdf_cta_ptr.most_critical_object_by_sides_objVelocityX_mps_right.clear();
   hdf_cta_ptr.most_critical_object_by_sides_objVelocityY_mps_right.clear();
   hdf_cta_ptr.most_critical_object_by_sides_heading_rad_right.clear();
   hdf_cta_ptr.most_critical_object_by_sides_alert_level_right.clear();
   hdf_cta_ptr.most_critical_object_by_sides_ttc_s_right.clear();
   hdf_cta_ptr.most_critical_object_by_sides_intersection_point_x_m_right.clear();
   hdf_cta_ptr.most_critical_object_by_sides_f_brake_qualifier_right.clear();
}

void HDFWriteClass::Reset_SG_Info() {
   hdf_sg_ptr.execution_timestamp_us.clear();
   hdf_sg_ptr.measurement_timestamp_us.clear();
   hdf_sg_ptr.cycle_index.clear();
   hdf_sg_ptr.num_contours.clear();
   hdf_sg_ptr.f_valid.clear();

   hdf_sg_ptr.position_x.clear();
   hdf_sg_ptr.position_y.clear();
   hdf_sg_ptr.position_variance_x.clear();
   hdf_sg_ptr.position_variance_y.clear();
   hdf_sg_ptr.position_covariance_xy.clear();
   hdf_sg_ptr.cycles_since_created.clear();
   hdf_sg_ptr.cycles_since_coasted.clear();
   hdf_sg_ptr.drivability_confidence.clear();
   hdf_sg_ptr.drivability.clear();

   hdf_sg_ptr.unique_id.clear();
   hdf_sg_ptr.num_vertices.clear();
   hdf_sg_ptr.type.clear();

   hdf_sg_ptr.major.clear();
   hdf_sg_ptr.minor.clear();
   hdf_sg_ptr.patch.clear();
}

void HDFWriteClass::Reset_Faults_Info() {
   hdf_faults_ptr.input_core_time_us_no_increase.clear();
   hdf_faults_ptr.input_core_cnt_loops_no_increase.clear();
   hdf_faults_ptr.input_core_elapsed_time_below_lower_limit.clear();
   hdf_faults_ptr.input_core_elapsed_time_above_upper_limit.clear();

   hdf_faults_ptr.input_host_vehicle_index_no_increase.clear();
   hdf_faults_ptr.input_host_speed_invalid.clear();
   hdf_faults_ptr.input_host_yawrate_invalid.clear();
   hdf_faults_ptr.input_host_longitudinal_acceleration_invalid.clear();
   hdf_faults_ptr.input_host_lateral_acceleration_invalid.clear();

   hdf_faults_ptr.input_raw_detection_range_is_invalid.clear();
   hdf_faults_ptr.input_raw_detection_range_rate_is_invalid.clear();
   hdf_faults_ptr.input_raw_detection_azimuth_is_invalid.clear();
   hdf_faults_ptr.input_raw_detection_elevation_is_invalid.clear();

   hdf_faults_ptr.input_sensors_calib_mounting_pos_is_invalid.clear();
   hdf_faults_ptr.input_sensors_calib_polarity_is_invalid.clear();
   hdf_faults_ptr.input_sensors_calib_boresight_angle_is_invalid.clear();
   hdf_faults_ptr.input_sensors_look_index_no_increase.clear();
   hdf_faults_ptr.input_sensors_sensor_vs_tracker_timestamp_divergence.clear();

   hdf_faults_ptr.output_track_positions_faulty.clear();
   hdf_faults_ptr.output_track_velocities_faulty.clear();
   hdf_faults_ptr.output_track_accelerations_faulty.clear();
   hdf_faults_ptr.output_severe_angle_jump_presence_fault.clear();

   hdf_faults_ptr.scl_sensors_calibs_fault_status.clear();
   hdf_faults_ptr.scl_sensors_fault_status.clear();
   hdf_faults_ptr.scl_core_info_fault_status.clear();
   hdf_faults_ptr.scl_host_info_fault_status.clear();
   hdf_faults_ptr.scl_raw_detection_fault_status.clear();
   hdf_faults_ptr.scl_object_track_fault_status.clear();
   hdf_faults_ptr.scl_angle_jump_fault_status.clear();
   hdf_faults_ptr.scl_overall_fault_status.clear();
   hdf_faults_ptr.scl_should_reset.clear();
}

void HDFWriteClass::Reset_HDF_Buffers() {

   HDFWriteClass::Reset_Scan_Index();
   HDFWriteClass::Reset_All_Object_Info();
   HDFWriteClass::Reset_Vehicle_Info();
   HDFWriteClass::Reset_Tracker_Info();
   HDFWriteClass::Reset_Processed_Detection_Info();
   HDFWriteClass::Reset_Vse_Output_Info();
   HDFWriteClass::Reset_Raw_Detection_Info();
   HDFWriteClass::Reset_Sensor_Info();
   HDFWriteClass::Reset_ROT_Info();
   HDFWriteClass::Reset_MountInfo_ptr();
   HDFWriteClass::Reset_InVehicleInfo_ptr();
   HDFWriteClass::Reset_GT_Info();
   HDFWriteClass::Reset_OLP_Object_Output();
   HDFWriteClass::Reset_Lcda_Feature_Output();
   HDFWriteClass::Reset_Ced_Feature_Output();
   HDFWriteClass::Reset_Esa_Feature_Output();
   HDFWriteClass::Reset_Ltb_Feature_Output();
   HDFWriteClass::Reset_Recw_Feature_Output();
   HDFWriteClass::Reset_Ta_Feature_Output();
   HDFWriteClass::Reset_Scw_Feature_Output();
   HDFWriteClass::Reset_Cta_Feature_Output();
   HDFWriteClass::Reset_Pt_Feature_Output();
   HDFWriteClass::Reset_SG_Info();
   HDFWriteClass::Reset_Faults_Info();
}
void HDFWriteClass::UpdateHDFbuffers() {

   hdf_scan_index_ptr.scan_index.push_back(GetScanIndex());

   std::vector<DetectionObj_Data_Input_T *> Radar_Input{
       &dc_input->sym_detection_fl_radar,
       &dc_input->sym_detection_fr_radar,
       &dc_input->sym_detection_rl_radar,
       &dc_input->sym_detection_rr_radar,
       &dc_input->sym_detection_fc_radar,
       &dc_input->sym_detection_rc_radar,
       &dc_input->sym_detection_bpil_bl_radar,
       &dc_input->sym_detection_bpil_br_radar};

   std::vector<MOUNTING_VALUES_T *> MountPos_Input{
       &dc_input->Mounting_Values_FL,
       &dc_input->Mounting_Values_FR,
       &dc_input->Mounting_Values_RL,
       &dc_input->Mounting_Values_RR,
       &dc_input->Mounting_Values_FC,
       &dc_input->Mounting_Values_RC,
       &dc_input->Mounting_Values_BL,
       &dc_input->Mounting_Values_BR,
   };

   std::vector<float32_T> timestamp;
   std::vector<unsigned32_T> hdrTimestamp_fractionalSec;
   std::vector<unsigned32_T> hdrTimestamp_Sec;
   std::vector<float32_T> AutoAlignElevation;
   std::vector<unsigned32_T> Count;
   std::vector<unsigned32_T> ScanIndex;
   std::vector<unsigned8_T> AutoAlignAzimuthQF;
   std::vector<unsigned8_T> AutoAlignElevationQF;
   std::vector<unsigned8_T> timestamp_consistency;
   std::vector<unsigned8_T> LookID;
   std::vector<unsigned8_T> LookType;

   std::vector<float32_T> azimuth_polarity;
   std::vector<float32_T> boresight_angle;
   std::vector<float32_T> vcs_lat_position;
   std::vector<float32_T> vcs_lon_position;
   std::vector<float32_T> vcs_z_position;

   for (int sensor_index = 0; sensor_index < 8; sensor_index++) {
      timestamp.push_back(Radar_Input[sensor_index]->dets_info.timestamp);
      hdrTimestamp_fractionalSec.push_back(Radar_Input[sensor_index]->dets_info.headerTimestamp.fractional_seconds);
      hdrTimestamp_Sec.push_back(Radar_Input[sensor_index]->dets_info.headerTimestamp.seconds);
      AutoAlignElevation.push_back(Radar_Input[sensor_index]->dets_info.AutoAlignElevation);
      Count.push_back(Radar_Input[sensor_index]->dets_info.Count);
      ScanIndex.push_back(Radar_Input[sensor_index]->dets_info.ScanIndex);
      AutoAlignAzimuthQF.push_back(Radar_Input[sensor_index]->dets_info.AutoAlignAzimuthQF);
      AutoAlignElevationQF.push_back(Radar_Input[sensor_index]->dets_info.AutoAlignElevationQF);
      timestamp_consistency.push_back(Radar_Input[sensor_index]->dets_info.timestamp_consistency);
      LookID.push_back(Radar_Input[sensor_index]->dets_info.LookID);
      LookType.push_back(Radar_Input[sensor_index]->dets_info.LookType);
      azimuth_polarity.push_back(MountPos_Input[sensor_index]->azimuth_polarity);
      boresight_angle.push_back(MountPos_Input[sensor_index]->boresight_angle);
      vcs_lat_position.push_back(MountPos_Input[sensor_index]->vcs_lat_position);
      vcs_lon_position.push_back(MountPos_Input[sensor_index]->vcs_lon_position);
      vcs_z_position.push_back(MountPos_Input[sensor_index]->vcs_z_position);
   }

   hdf_raw_detection_info_ptr.timestamp.push_back(timestamp);
   hdf_raw_detection_info_ptr.hdrTimestamp_fractionalSec.push_back(hdrTimestamp_fractionalSec);
   hdf_raw_detection_info_ptr.hdrTimestamp_Sec.push_back(hdrTimestamp_Sec);
   hdf_raw_detection_info_ptr.AutoAlignElevation.push_back(AutoAlignElevation);
   hdf_raw_detection_info_ptr.Count.push_back(Count);
   hdf_raw_detection_info_ptr.ScanIndex.push_back(ScanIndex);
   hdf_raw_detection_info_ptr.AutoAlignAzimuthQF.push_back(AutoAlignAzimuthQF);
   hdf_raw_detection_info_ptr.AutoAlignElevationQF.push_back(AutoAlignElevationQF);
   hdf_raw_detection_info_ptr.timestamp_consistency.push_back(timestamp_consistency);
   hdf_raw_detection_info_ptr.LookID.push_back(LookID);
   hdf_raw_detection_info_ptr.LookType.push_back(LookType);

   hdf_MountInfo_ptr.azimuth_polarity.push_back(azimuth_polarity);
   hdf_MountInfo_ptr.boresight_angle.push_back(boresight_angle);
   hdf_MountInfo_ptr.vcs_lat_position.push_back(vcs_lat_position);
   hdf_MountInfo_ptr.vcs_lon_position.push_back(vcs_lon_position);
   hdf_MountInfo_ptr.vcs_z_position.push_back(vcs_z_position);

   std::vector<std::vector<float32_T>> elevation_2d;
   std::vector<std::vector<float32_T>> azimuth_2d;
   std::vector<std::vector<float32_T>> range_rate_2d;
   std::vector<std::vector<float32_T>> range_2d;
   std::vector<std::vector<float32_T>> amplitude_2d;
   std::vector<std::vector<float32_T>> snr_2d;
   std::vector<std::vector<float32_T>> std_elevation_2d;
   std::vector<std::vector<float32_T>> std_azimuth_2d;
   std::vector<std::vector<float32_T>> std_range_rate_2d;
   std::vector<std::vector<float32_T>> std_range_2d;
   std::vector<std::vector<float32_T>> std_rcs_2d;
   std::vector<std::vector<float32_T>> multi_target_probability_2d;
   std::vector<std::vector<float32_T>> existence_probability_2d;
   std::vector<std::vector<unsigned8_T>> azimuth_confidence_2d;
   std::vector<std::vector<unsigned8_T>> elevation_confidence_2d;
   std::vector<std::vector<unsigned8_T>> valid_2d;
   std::vector<std::vector<boolean_T>> host_veh_clutter_2d;
   std::vector<std::vector<boolean_T>> nd_target_2d;
   std::vector<std::vector<boolean_T>> bistatic_2d;

   for (int i = 0; i < NUMBER_OF_SRR_DETECTIONS_LOG; i++) {
      std::vector<float32_T> elevation;
      std::vector<float32_T> azimuth;
      std::vector<float32_T> range_rate;
      std::vector<float32_T> range;
      std::vector<float32_T> amplitude;
      std::vector<float32_T> snr;
      std::vector<float32_T> std_elevation;
      std::vector<float32_T> std_azimuth;
      std::vector<float32_T> std_range_rate;
      std::vector<float32_T> std_range;
      std::vector<float32_T> std_rcs;
      std::vector<float32_T> multi_target_probability;
      std::vector<float32_T> existence_probability;
      std::vector<unsigned8_T> azimuth_confidence;
      std::vector<unsigned8_T> elevation_confidence;
      std::vector<unsigned8_T> valid;
      std::vector<boolean_T> host_veh_clutter;
      std::vector<boolean_T> nd_target;
      std::vector<boolean_T> bistatic;

      for (int sensor_index = 0; sensor_index < 8; sensor_index++) {
         elevation.push_back(Radar_Input[sensor_index]->dets_input[i].elevation);
         azimuth.push_back(Radar_Input[sensor_index]->dets_input[i].azimuth);
         range_rate.push_back(Radar_Input[sensor_index]->dets_input[i].range_rate);
         range.push_back(Radar_Input[sensor_index]->dets_input[i].range);
         amplitude.push_back(Radar_Input[sensor_index]->dets_input[i].amplitude);
         snr.push_back(Radar_Input[sensor_index]->dets_input[i].snr);
         std_elevation.push_back(Radar_Input[sensor_index]->dets_input[i].std_elevation);
         std_azimuth.push_back(Radar_Input[sensor_index]->dets_input[i].std_azimuth);
         std_range_rate.push_back(Radar_Input[sensor_index]->dets_input[i].std_range_rate);
         std_range.push_back(Radar_Input[sensor_index]->dets_input[i].std_range);
         std_rcs.push_back(Radar_Input[sensor_index]->dets_input[i].std_rcs);
         multi_target_probability.push_back(Radar_Input[sensor_index]->dets_input[i].multi_target_probability);
         existence_probability.push_back(Radar_Input[sensor_index]->dets_input[i].existence_probability);
         azimuth_confidence.push_back(Radar_Input[sensor_index]->dets_input[i].azimuth_confidence);
         elevation_confidence.push_back(Radar_Input[sensor_index]->dets_input[i].elevation_confidence);
         valid.push_back(Radar_Input[sensor_index]->dets_input[i].valid);
         host_veh_clutter.push_back(Radar_Input[sensor_index]->dets_input[i].host_veh_clutter);
         nd_target.push_back(Radar_Input[sensor_index]->dets_input[i].nd_target);
         bistatic.push_back(Radar_Input[sensor_index]->dets_input[i].bistatic);
      }

      elevation_2d.push_back(elevation);
      azimuth_2d.push_back(azimuth);
      range_rate_2d.push_back(range_rate);
      range_2d.push_back(range);
      amplitude_2d.push_back(amplitude);
      snr_2d.push_back(snr);
      std_elevation_2d.push_back(std_elevation);
      std_azimuth_2d.push_back(std_azimuth);
      std_range_rate_2d.push_back(std_range_rate);
      std_range_2d.push_back(std_range);
      std_rcs_2d.push_back(std_rcs);
      multi_target_probability_2d.push_back(multi_target_probability);
      existence_probability_2d.push_back(existence_probability);
      azimuth_confidence_2d.push_back(azimuth_confidence);
      elevation_confidence_2d.push_back(elevation_confidence);
      valid_2d.push_back(valid);
      host_veh_clutter_2d.push_back(host_veh_clutter);
      nd_target_2d.push_back(nd_target);
      bistatic_2d.push_back(bistatic);
   }
   hdf_raw_detection_info_ptr.elevation.push_back(elevation_2d);
   hdf_raw_detection_info_ptr.azimuth.push_back(azimuth_2d);
   hdf_raw_detection_info_ptr.range_rate.push_back(range_rate_2d);
   hdf_raw_detection_info_ptr.range.push_back(range_2d);
   hdf_raw_detection_info_ptr.amplitude.push_back(amplitude_2d);
   hdf_raw_detection_info_ptr.snr.push_back(snr_2d);
   hdf_raw_detection_info_ptr.std_elevation.push_back(std_elevation_2d);
   hdf_raw_detection_info_ptr.std_azimuth.push_back(std_azimuth_2d);
   hdf_raw_detection_info_ptr.std_rcs.push_back(std_rcs_2d);
   hdf_raw_detection_info_ptr.std_range_rate.push_back(std_range_rate_2d);
   hdf_raw_detection_info_ptr.std_range.push_back(std_range_2d);
   hdf_raw_detection_info_ptr.std_rcs.push_back(std_rcs_2d);
   hdf_raw_detection_info_ptr.multi_target_probability.push_back(multi_target_probability_2d);
   hdf_raw_detection_info_ptr.existence_probability.push_back(existence_probability_2d);
   hdf_raw_detection_info_ptr.azimuth_confidence.push_back(azimuth_confidence_2d);
   hdf_raw_detection_info_ptr.elevation_confidence.push_back(elevation_confidence_2d);
   hdf_raw_detection_info_ptr.valid.push_back(valid_2d);
   hdf_raw_detection_info_ptr.host_veh_clutter.push_back(host_veh_clutter_2d);
   hdf_raw_detection_info_ptr.nd_target.push_back(nd_target_2d);
   hdf_raw_detection_info_ptr.bistatic.push_back(bistatic_2d);

   hdf_InVehicleInfo_ptr.abs_speed.push_back(dc_input->vehicle_inputs.abs_speed);
   hdf_InVehicleInfo_ptr.bb_center_to_rear_x.push_back(dc_input->vehicle_inputs.bb_center_to_rear_x);
   hdf_InVehicleInfo_ptr.bb_center_to_rear_y.push_back(dc_input->vehicle_inputs.bb_center_to_rear_y);
   hdf_InVehicleInfo_ptr.bb_center_to_rear_z.push_back(dc_input->vehicle_inputs.bb_center_to_rear_z);
   hdf_InVehicleInfo_ptr.curve_radius.push_back(dc_input->vehicle_inputs.curve_radius);
   hdf_InVehicleInfo_ptr.f_reverse_gear.push_back(dc_input->vehicle_inputs.f_reverse_gear);
   hdf_InVehicleInfo_ptr.f_traffic_side.push_back(dc_input->vehicle_inputs.f_traffic_side);
   hdf_InVehicleInfo_ptr.f_trailer_present.push_back(dc_input->vehicle_inputs.f_trailer_present);
   hdf_InVehicleInfo_ptr.host_vehicle_height.push_back(dc_input->vehicle_inputs.host_vehicle_height);
   hdf_InVehicleInfo_ptr.host_vehicle_length.push_back(dc_input->vehicle_inputs.host_vehicle_length);
   hdf_InVehicleInfo_ptr.host_vehicle_width.push_back(dc_input->vehicle_inputs.host_vehicle_width);
   hdf_InVehicleInfo_ptr.lane_center_offset_external.push_back(dc_input->vehicle_inputs.lane_center_offset_external);
   hdf_InVehicleInfo_ptr.lane_width_external.push_back(dc_input->vehicle_inputs.lane_width_external);
   hdf_InVehicleInfo_ptr.prndl.push_back(dc_input->vehicle_inputs.prndl);
   hdf_InVehicleInfo_ptr.rear_axle_position.push_back(dc_input->vehicle_inputs.rear_axle_position);
   hdf_InVehicleInfo_ptr.rear_axle_steering_angle.push_back(dc_input->vehicle_inputs.rear_axle_steering_angle);
   hdf_InVehicleInfo_ptr.steering_angle.push_back(dc_input->vehicle_inputs.steering_angle);
   hdf_InVehicleInfo_ptr.turn_signal.push_back(dc_input->vehicle_inputs.turn_signal);
   hdf_InVehicleInfo_ptr.vcs_lat_acc.push_back(dc_input->vehicle_inputs.vcs_lat_acc);
   hdf_InVehicleInfo_ptr.vcs_long_acc.push_back(dc_input->vehicle_inputs.vcs_long_acc);
   hdf_InVehicleInfo_ptr.vehicle_data_buff_timestamp.push_back(dc_input->vehicle_inputs.vehicle_data_buff_timestamp);
   hdf_InVehicleInfo_ptr.yawrate.push_back(dc_input->vehicle_inputs.yawrate);

   std::vector<unsigned32_T> ego_Sec;
   std::vector<unsigned32_T> ego_FractionalSec;
   std::vector<float32_T> ego_vcs_long_acc;
   std::vector<unsigned32_T> ego_valueQEgoAccelerationLongitudinalCog;
   std::vector<float32_T> ego_vcs_lat_acc;
   std::vector<unsigned32_T> ego_valueQEgoAccelerationLateralCog;
   std::vector<float32_T> ego_yawrate;
   std::vector<unsigned32_T> ego_valueQYawRateVehicleBody;
   std::vector<float32_T> ego_steering_angle;
   std::vector<float32_T> ego_abs_speed;
   std::vector<unsigned32_T> ego_valueQEgoSpeedCog;
   std::vector<unsigned32_T> ego_drivingDirectionConfirmed;

   for (int j = 0; j < EGO_DATA_ARRAY_SIZE; j++) {
      ego_Sec.push_back(dc_input->vehicle_inputs.egomotionData[j].timeStamp.seconds);
      ego_FractionalSec.push_back(dc_input->vehicle_inputs.egomotionData[j].timeStamp.fractional_seconds);
      ego_vcs_long_acc.push_back(dc_input->vehicle_inputs.egomotionData[j].vcs_long_acc);
      ego_valueQEgoAccelerationLongitudinalCog.push_back(dc_input->vehicle_inputs.egomotionData[j].valueQEgoAccelerationLongitudinalCog);
      ego_vcs_lat_acc.push_back(dc_input->vehicle_inputs.egomotionData[j].vcs_lat_acc);
      ego_valueQEgoAccelerationLateralCog.push_back(dc_input->vehicle_inputs.egomotionData[j].valueQEgoAccelerationLateralCog);
      ego_yawrate.push_back(dc_input->vehicle_inputs.egomotionData[j].yawrate);
      ego_valueQYawRateVehicleBody.push_back(dc_input->vehicle_inputs.egomotionData[j].valueQYawRateVehicleBody);
      ego_steering_angle.push_back(dc_input->vehicle_inputs.egomotionData[j].steering_angle);
      ego_abs_speed.push_back(dc_input->vehicle_inputs.egomotionData[j].abs_speed);
      ego_valueQEgoSpeedCog.push_back(dc_input->vehicle_inputs.egomotionData[j].valueQEgoSpeedCog);
      ego_drivingDirectionConfirmed.push_back(dc_input->vehicle_inputs.egomotionData[j].drivingDirectionConfirmed);
   }
   hdf_InVehicleInfo_ptr.ego_abs_speed.push_back(ego_abs_speed);
   hdf_InVehicleInfo_ptr.ego_drivingDirectionConfirmed.push_back(ego_drivingDirectionConfirmed);
   hdf_InVehicleInfo_ptr.ego_FractionalSec.push_back(ego_FractionalSec);
   hdf_InVehicleInfo_ptr.ego_Sec.push_back(ego_Sec);
   hdf_InVehicleInfo_ptr.ego_steering_angle.push_back(ego_steering_angle);
   hdf_InVehicleInfo_ptr.ego_valueQEgoAccelerationLateralCog.push_back(ego_valueQEgoAccelerationLateralCog);
   hdf_InVehicleInfo_ptr.ego_valueQEgoAccelerationLongitudinalCog.push_back(ego_valueQEgoAccelerationLongitudinalCog);
   hdf_InVehicleInfo_ptr.ego_valueQEgoSpeedCog.push_back(ego_valueQEgoSpeedCog);
   hdf_InVehicleInfo_ptr.ego_valueQYawRateVehicleBody.push_back(ego_valueQYawRateVehicleBody);
   hdf_InVehicleInfo_ptr.ego_vcs_lat_acc.push_back(ego_vcs_lat_acc);
   hdf_InVehicleInfo_ptr.ego_vcs_long_acc.push_back(ego_vcs_long_acc);
   hdf_InVehicleInfo_ptr.ego_yawrate.push_back(ego_yawrate);

   // Ground Truth

   std::vector<unsigned8_T> GT_id;
   std::vector<signed8_T> GT_status;
   std::vector<unsigned8_T> GT_age;
   std::vector<unsigned8_T> GT_stage_age;
   std::vector<float32_T> GT_vcs_long_posn;
   std::vector<float32_T> GT_vcs_long_vel;
   std::vector<float32_T> GT_vcs_long_accel;
   std::vector<float32_T> GT_vcs_lat_posn;
   std::vector<float32_T> GT_vcs_lat_vel;
   std::vector<float32_T> GT_vcs_lat_accel;
   std::vector<float32_T> GT_vcs_long_vel_rel;
   std::vector<float32_T> GT_vcs_lat_vel_rel;
   std::vector<float32_T> GT_speed;
   std::vector<float32_T> GT_tangential_accel;
   std::vector<float32_T> GT_heading;
   std::vector<float32_T> GT_heading_rate;
   std::vector<float32_T> GT_length;
   std::vector<float32_T> GT_width;
   std::vector<signed8_T> GT_object_class;

   for (int gt = 0; gt < TRACKER_NUMBER_OF_OBJECTS; gt++) {
      GT_id.push_back(dc_input->object_rl[gt].id);
      GT_status.push_back(dc_input->object_rl[gt].status);
      GT_age.push_back(dc_input->object_rl[gt].age);
      GT_stage_age.push_back(dc_input->object_rl[gt].stage_age);
      GT_vcs_long_posn.push_back(dc_input->object_rl[gt].vcs_long_posn);
      GT_vcs_long_vel.push_back(dc_input->object_rl[gt].vcs_long_vel);
      GT_vcs_long_accel.push_back(dc_input->object_rl[gt].vcs_long_accel);
      GT_vcs_lat_posn.push_back(dc_input->object_rl[gt].vcs_lat_posn);
      GT_vcs_lat_vel.push_back(dc_input->object_rl[gt].vcs_lat_vel);
      GT_vcs_lat_accel.push_back(dc_input->object_rl[gt].vcs_lat_accel);
      GT_speed.push_back(dc_input->object_rl[gt].speed);
      if (dc_input->object_rl[gt].speed != 0) {
         GT_vcs_long_vel_rel.push_back(dc_input->object_rl[gt].vcs_long_vel - VsePtr->VsVSE_mps_VCSLongVel);
         GT_vcs_lat_vel_rel.push_back(dc_input->object_rl[gt].vcs_lat_vel - VsePtr->VsVSE_mps_VCSLatVel);
      } else {
         GT_vcs_long_vel_rel.push_back(dc_input->object_rl[gt].vcs_long_vel);
         GT_vcs_lat_vel_rel.push_back(dc_input->object_rl[gt].vcs_lat_vel);
      }
      GT_tangential_accel.push_back(dc_input->object_rl[gt].tangential_accel);
      GT_heading.push_back(dc_input->object_rl[gt].heading);
      GT_heading_rate.push_back(dc_input->object_rl[gt].heading_rate);
      GT_length.push_back(dc_input->object_rl[gt].length);
      GT_width.push_back(dc_input->object_rl[gt].width);
      GT_object_class.push_back(dc_input->object_rl[gt].object_class);
   }

   hdf_GT_ptr.id.push_back(GT_id);
   hdf_GT_ptr.status.push_back(GT_status);
   hdf_GT_ptr.age.push_back(GT_age);
   hdf_GT_ptr.stage_age.push_back(GT_stage_age);
   hdf_GT_ptr.vcs_long_posn.push_back(GT_vcs_long_posn);
   hdf_GT_ptr.vcs_long_vel.push_back(GT_vcs_long_vel);
   hdf_GT_ptr.vcs_long_accel.push_back(GT_vcs_long_accel);
   hdf_GT_ptr.vcs_lat_posn.push_back(GT_vcs_lat_posn);
   hdf_GT_ptr.vcs_lat_vel.push_back(GT_vcs_lat_vel);
   hdf_GT_ptr.vcs_lat_accel.push_back(GT_vcs_lat_accel);
   hdf_GT_ptr.vcs_long_vel_rel.push_back(GT_vcs_long_vel_rel);
   hdf_GT_ptr.vcs_lat_vel_rel.push_back(GT_vcs_lat_vel_rel);
   hdf_GT_ptr.speed.push_back(GT_speed);
   hdf_GT_ptr.tangential_accel.push_back(GT_tangential_accel);
   hdf_GT_ptr.heading.push_back(GT_heading);
   hdf_GT_ptr.heading_rate.push_back(GT_heading_rate);
   hdf_GT_ptr.length.push_back(GT_length);
   hdf_GT_ptr.width.push_back(GT_width);
   hdf_GT_ptr.object_class.push_back(GT_object_class);

   // Core  info

   hdf_core_info_ptr.cnt_loops.push_back(CoreInfo->cnt_loops);
   hdf_core_info_ptr.elapsed_time_s.push_back(CoreInfo->elapsed_time_s);
   hdf_core_info_ptr.prev_time_us.push_back(CoreInfo->prev_time_us);
   hdf_core_info_ptr.time_us.push_back(CoreInfo->time_us);

   // Populate vehicle information

   hdf_vehicle_info_ptr.vehicle_index.push_back(ptr_host->vehicle_index);
   hdf_vehicle_info_ptr.speed.push_back(ptr_host->speed);
   hdf_vehicle_info_ptr.vcs_speed.push_back(ptr_host->vcs_speed);
   hdf_vehicle_info_ptr.acceleration.push_back(ptr_host->acceleration);
   hdf_vehicle_info_ptr.vcs_lat_acceleration.push_back(ptr_host->vcs_lat_acceleration);
   hdf_vehicle_info_ptr.vcs_long_acceleration.push_back(ptr_host->vcs_long_acceleration);
   hdf_vehicle_info_ptr.yaw_rate_rad.push_back(ptr_host->yaw_rate_rad);
   hdf_vehicle_info_ptr.vcs_sideslip.push_back(ptr_host->vcs_sideslip);
   hdf_vehicle_info_ptr.curvature_rear.push_back(ptr_host->curvature_rear);
   hdf_vehicle_info_ptr.dist_rear_axle_to_vcs_m.push_back(ptr_host->dist_rear_axle_to_vcs_m);
   hdf_vehicle_info_ptr.vehicle_length.push_back(ptr_host->vehicle_length);
   hdf_vehicle_info_ptr.vehicle_width.push_back(ptr_host->vehicle_width);
   hdf_vehicle_info_ptr.rear_cornering_compliance.push_back(ptr_host->rear_cornering_compliance);
   hdf_vehicle_info_ptr.speed_correction_factor.push_back(ptr_host->speed_correction_factor);
   hdf_vehicle_info_ptr.host_type.push_back(ptr_host->host_type);
   hdf_vehicle_info_ptr.f_trailer_presence_hardware.push_back(ptr_host->f_trailer_presence_hardware);
   hdf_vehicle_info_ptr.speed_qf.push_back(ptr_host->speed_qf);
   hdf_vehicle_info_ptr.yaw_rate_qf.push_back(ptr_host->yaw_rate_qf);
   hdf_vehicle_info_ptr.lat_accel_qf.push_back(ptr_host->lat_accel_qf);
   hdf_vehicle_info_ptr.long_accel_qf.push_back(ptr_host->long_accel_qf);
   hdf_vehicle_info_ptr.dist_rear_axle_to_vcs_m.push_back(ptr_hostCalib->dist_rear_axle_to_vcs_m);
   hdf_vehicle_info_ptr.steer_gear_ratio.push_back(ptr_hostCalib->steer_gear_ratio);
   hdf_vehicle_info_ptr.wheelbase_m.push_back(ptr_hostCalib->wheelbase_m);
   hdf_vehicle_info_ptr.understeer_coefficient.push_back(ptr_hostCalib->understeer_coefficient);
   hdf_vehicle_info_ptr.cog_x.push_back(ptr_hostCalib->cog_x);
   hdf_vehicle_info_ptr.cog_y.push_back(ptr_hostCalib->cog_y);
   hdf_vehicle_info_ptr.front_wheel_radius_m.push_back(ptr_hostCalib->front_wheel_radius_m);
   hdf_vehicle_info_ptr.front_track_width_m.push_back(ptr_hostCalib->front_track_width_m);
   hdf_vehicle_info_ptr.raw_host_signal_latency_ms.push_back(ptr_hostCalib->raw_host_signal_latency_ms);
   hdf_vehicle_info_ptr.f_enable_internal_reflections_func.push_back(ptr_hostCalib->f_enable_internal_reflections_func);
   hdf_vehicle_info_ptr.f_enable_internal_reflections_func_trailer.push_back(ptr_hostCalib->f_enable_internal_reflections_func_trailer);

   // Populate Tracker information
   hdf_tracker_info_ptr.timestamp_us.push_back(trackInfo->timestamp_us);
   hdf_tracker_info_ptr.object_list_timestamp.push_back(trackInfo->timestamp_us);
   hdf_tracker_info_ptr.elapsed_time_s.push_back(trackInfo->elapsed_time_s);
   hdf_tracker_info_ptr.sw_version_buildID.push_back(static_cast<unsigned32_T>(trackInfo->sw_version_buildID));
   hdf_tracker_info_ptr.tracker_index.push_back(trackInfo->tracker_index);
   hdf_tracker_info_ptr.nr_suspected_stat_angle_jump_dets_filtered.push_back(trackInfo->nr_suspected_stat_angle_jump_dets_filtered);
   hdf_tracker_info_ptr.num_unique_objs.push_back(trackInfo->num_unique_objs);
   hdf_tracker_info_ptr.f_severe_angle_jump_detected.push_back(trackInfo->f_severe_angle_jump_detected);
   std::vector<unsigned16_T> active_obj_ids;
   std::vector<unsigned16_T> inactive_obj_ids;
   for (int i = 0; i < MAX_F360_OBJECTS_LOG; i++) {
      active_obj_ids.push_back(trackInfo->active_obj_ids[i]);
      inactive_obj_ids.push_back(trackInfo->inactive_obj_ids[i]);
   }
   hdf_tracker_info_ptr.active_obj_ids.push_back(active_obj_ids);
   hdf_tracker_info_ptr.inactive_obj_ids.push_back(inactive_obj_ids);
   hdf_tracker_info_ptr.num_active_objs.push_back(trackInfo->num_active_objs);
   std::vector<unsigned16_T> reduced_active_obj_ids;
   std::vector<unsigned16_T> reduced_inactive_obj_ids;
   std::vector<unsigned16_T> reduced_obj_ids;
   for (int i = 0; i < MAX_REDUCED_OBJECTS_LOG; i++) {
      reduced_active_obj_ids.push_back(trackInfo->reduced_active_obj_ids[i]);
      reduced_inactive_obj_ids.push_back(trackInfo->reduced_inactive_obj_ids[i]);
      reduced_obj_ids.push_back(trackInfo->reduced_obj_ids[i]);
   }
   hdf_tracker_info_ptr.reduced_active_obj_ids.push_back(reduced_active_obj_ids);
   hdf_tracker_info_ptr.reduced_inactive_obj_ids.push_back(reduced_inactive_obj_ids);
   hdf_tracker_info_ptr.reduced_obj_ids.push_back(reduced_obj_ids);
   hdf_tracker_info_ptr.reduced_num_active_objs.push_back(trackInfo->reduced_num_active_objs);
   hdf_tracker_info_ptr.num_active_clusters.push_back(trackInfo->num_active_clusters);
   hdf_tracker_info_ptr.number_of_historic_detections.push_back(trackInfo->number_of_historic_detections);
   hdf_tracker_info_ptr.sw_version_major.push_back(trackInfo->sw_version_major);
   hdf_tracker_info_ptr.sw_version_minor.push_back(trackInfo->sw_version_minor);
   hdf_tracker_info_ptr.sw_version_patch.push_back(trackInfo->sw_version_patch);

   const Functional_Safety_Faults_Log_T default_faults{};
   const Functional_Safety_Faults_Log_T &faults_log = (faults != nullptr) ? *faults : default_faults;

   hdf_faults_ptr.input_core_time_us_no_increase.push_back(faults_log.input_faults.core_info.time_us_no_increase);
   hdf_faults_ptr.input_core_cnt_loops_no_increase.push_back(faults_log.input_faults.core_info.cnt_loops_no_increase);
   hdf_faults_ptr.input_core_elapsed_time_below_lower_limit.push_back(faults_log.input_faults.core_info.elapsed_time_below_lower_limit);
   hdf_faults_ptr.input_core_elapsed_time_above_upper_limit.push_back(faults_log.input_faults.core_info.elapsed_time_above_upper_limit);

   hdf_faults_ptr.input_host_vehicle_index_no_increase.push_back(faults_log.input_faults.host_info.vehicle_index_no_increase);
   hdf_faults_ptr.input_host_speed_invalid.push_back(faults_log.input_faults.host_info.host_speed_invalid);
   hdf_faults_ptr.input_host_yawrate_invalid.push_back(faults_log.input_faults.host_info.host_yawrate_invalid);
   hdf_faults_ptr.input_host_longitudinal_acceleration_invalid.push_back(faults_log.input_faults.host_info.host_longitudinal_acceleration_invalid);
   hdf_faults_ptr.input_host_lateral_acceleration_invalid.push_back(faults_log.input_faults.host_info.host_lateral_acceleration_invalid);

   hdf_faults_ptr.input_raw_detection_range_is_invalid.push_back(faults_log.input_faults.raw_detection.range_is_invalid);
   hdf_faults_ptr.input_raw_detection_range_rate_is_invalid.push_back(faults_log.input_faults.raw_detection.range_rate_is_invalid);
   hdf_faults_ptr.input_raw_detection_azimuth_is_invalid.push_back(faults_log.input_faults.raw_detection.azimuth_is_invalid);
   hdf_faults_ptr.input_raw_detection_elevation_is_invalid.push_back(faults_log.input_faults.raw_detection.elevation_is_invalid);

   std::vector<uint8_t> input_sensors_calib_mounting_pos_is_invalid;
   std::vector<uint8_t> input_sensors_calib_polarity_is_invalid;
   std::vector<uint8_t> input_sensors_calib_boresight_angle_is_invalid;
   std::vector<uint8_t> input_sensors_look_index_no_increase;
   std::vector<uint8_t> input_sensors_sensor_vs_tracker_timestamp_divergence;
   std::vector<uint8_t> scl_sensors_calibs_fault_status;
   std::vector<uint8_t> scl_sensors_fault_status;

   for (const auto &sensor_calib_fault : faults_log.input_faults.sensors_calibs) {
      input_sensors_calib_mounting_pos_is_invalid.push_back(sensor_calib_fault.mounting_pos_is_invalid);
      input_sensors_calib_polarity_is_invalid.push_back(sensor_calib_fault.polarity_is_invalid);
      input_sensors_calib_boresight_angle_is_invalid.push_back(sensor_calib_fault.boresight_angle_is_invalid);
   }

   for (const auto &sensor_fault : faults_log.input_faults.sensors) {
      input_sensors_look_index_no_increase.push_back(sensor_fault.look_index_no_increase);
      input_sensors_sensor_vs_tracker_timestamp_divergence.push_back(sensor_fault.sensor_vs_tracker_timestamp_divergence);
   }

   for (const auto status : faults_log.scl_output_faults.sensors_calibs_fault_status) {
      scl_sensors_calibs_fault_status.push_back(status);
   }

   for (const auto status : faults_log.scl_output_faults.sensors_fault_status) {
      scl_sensors_fault_status.push_back(status);
   }

   hdf_faults_ptr.input_sensors_calib_mounting_pos_is_invalid.push_back(input_sensors_calib_mounting_pos_is_invalid);
   hdf_faults_ptr.input_sensors_calib_polarity_is_invalid.push_back(input_sensors_calib_polarity_is_invalid);
   hdf_faults_ptr.input_sensors_calib_boresight_angle_is_invalid.push_back(input_sensors_calib_boresight_angle_is_invalid);
   hdf_faults_ptr.input_sensors_look_index_no_increase.push_back(input_sensors_look_index_no_increase);
   hdf_faults_ptr.input_sensors_sensor_vs_tracker_timestamp_divergence.push_back(input_sensors_sensor_vs_tracker_timestamp_divergence);

   hdf_faults_ptr.output_track_positions_faulty.push_back(faults_log.output_faults.f_track_positions_faulty);
   hdf_faults_ptr.output_track_velocities_faulty.push_back(faults_log.output_faults.f_track_velocities_faulty);
   hdf_faults_ptr.output_track_accelerations_faulty.push_back(faults_log.output_faults.f_track_accelerations_faulty);
   hdf_faults_ptr.output_severe_angle_jump_presence_fault.push_back(faults_log.output_faults.f_severe_angle_jump_presence_fault);

   hdf_faults_ptr.scl_sensors_calibs_fault_status.push_back(scl_sensors_calibs_fault_status);
   hdf_faults_ptr.scl_sensors_fault_status.push_back(scl_sensors_fault_status);
   hdf_faults_ptr.scl_core_info_fault_status.push_back(faults_log.scl_output_faults.core_info_fault_status);
   hdf_faults_ptr.scl_host_info_fault_status.push_back(faults_log.scl_output_faults.host_info_fault_status);
   hdf_faults_ptr.scl_raw_detection_fault_status.push_back(faults_log.scl_output_faults.raw_detection_fault_status);
   hdf_faults_ptr.scl_object_track_fault_status.push_back(faults_log.scl_output_faults.object_track_fault_status);
   hdf_faults_ptr.scl_angle_jump_fault_status.push_back(faults_log.scl_output_faults.angle_jump_fault_status);
   hdf_faults_ptr.scl_overall_fault_status.push_back(faults_log.scl_output_faults.overall_fault_status);
   hdf_faults_ptr.scl_should_reset.push_back(faults_log.scl_output_faults.should_reset);

   // Populate Sensor Info

   std::vector<float32_T> min_host_vel;
   std::vector<float32_T> occurrence_lowerlimit;
   std::vector<float32_T> occurrence_threshold;
   std::vector<float32_T> rcs_tolerance;
   std::vector<float32_T> azimuth_tolerance;
   std::vector<float32_T> range_tolerance;
   std::vector<float32_T> rcs_max;
   std::vector<float32_T> range_max;
   std::vector<unsigned16_T> age_threshold;
   std::vector<boolean_T> f_enable_flags;

   std::vector<float32_T> range_limits;
   std::vector<float32_T> fov_min_az_rad;
   std::vector<float32_T> fov_max_az_rad;
   std::vector<float32_T> fov_min_el_rad;
   std::vector<float32_T> fov_max_el_rad;
   std::vector<float32_T> min_aliaised_range_rate;
   std::vector<float32_T> v_wrapping;
   std::vector<float32_T> r_wrapping;

   // RSPP_DET_NUM_LOOK_ID dependent arrays
   for (int i = 0; i < RSPP_DET_NUM_LOOK_ID; i++) {
      range_limits.push_back(SensInfo->constant.range_limits[i]);
      fov_min_az_rad.push_back(SensInfo->constant.fov_min_az_rad[i]);
      fov_max_az_rad.push_back(SensInfo->constant.fov_max_az_rad[i]);
      fov_min_el_rad.push_back(SensInfo->constant.fov_min_el_rad[i]);
      fov_max_el_rad.push_back(SensInfo->constant.fov_max_el_rad[i]);
      min_aliaised_range_rate.push_back(SensInfo->constant.min_aliaised_range_rate[i]);
      v_wrapping.push_back(SensInfo->constant.v_wrapping[i]);
      r_wrapping.push_back(SensInfo->constant.r_wrapping[i]);
   }

   hdf_sensor_info_ptr.range_limits.push_back(range_limits);
   hdf_sensor_info_ptr.fov_min_az_rad.push_back(fov_min_az_rad);
   hdf_sensor_info_ptr.fov_max_az_rad.push_back(fov_max_az_rad);
   hdf_sensor_info_ptr.fov_min_el_rad.push_back(fov_min_el_rad);
   hdf_sensor_info_ptr.fov_max_el_rad.push_back(fov_max_el_rad);
   hdf_sensor_info_ptr.min_aliaised_range_rate.push_back(min_aliaised_range_rate);
   hdf_sensor_info_ptr.v_wrapping.push_back(v_wrapping);
   hdf_sensor_info_ptr.r_wrapping.push_back(r_wrapping);

   // For per-sensor values, build a vector across sensors and push that vector (one entry per scan)

   std::vector<unsigned32_T> ids;
   std::vector<signed32_T> polarities;
   std::vector<int8_T> mount_locs;
   std::vector<int8_T> sensor_types;
   std::vector<unsigned64_T> timestamps;
   std::vector<float32_T> vcs_vel_lat;
   std::vector<float32_T> vcs_vel_long;
   std::vector<float32_T> vcs_lon;
   std::vector<float32_T> vcs_lat;
   std::vector<float32_T> vcs_height;
   std::vector<float32_T> vcs_boresight_az;
   std::vector<float32_T> vcs_boresight_el;
   std::vector<float32_T> vacs_boresight_az_est;
   std::vector<float32_T> vacs_boresight_el_est;
   std::vector<unsigned32_T> num_valid_dets;
   std::vector<unsigned16_T> rain_level;
   std::vector<unsigned16_T> look_idx;
   std::vector<boolean_T> valid_flag;
   std::vector<boolean_T> sensor_fault_flag;
   std::vector<int8_T> look_ids;
   std::vector<int32_T> useful_FOV(8, 0);
   for (size_t i = 0; i < 5; ++i) {
      useful_FOV[i] = 75;
   }

   for (int i = 0; i < 8; ++i) {
      ids.push_back(SensInfo[i].constant.id);
      polarities.push_back(SensInfo[i].constant.polarity);
      mount_locs.push_back(SensInfo[i].constant.mounting_location);
      sensor_types.push_back(SensInfo[i].constant.sensor_type);
      timestamps.push_back(SensInfo[i].variable.timestamp_us);
      vcs_vel_lat.push_back(SensInfo[i].variable.vcs_velocity.lateral);
      vcs_vel_long.push_back(SensInfo[i].variable.vcs_velocity.longitudinal);
      vcs_lon.push_back(SensInfo[i].constant.mounting_position.vcs_position.longitudinal);
      vcs_lat.push_back(SensInfo[i].constant.mounting_position.vcs_position.lateral);
      vcs_height.push_back(SensInfo[i].constant.mounting_position.vcs_position.height);
      vcs_boresight_az.push_back(SensInfo[i].constant.mounting_position.vcs_boresight_azimuth_angle);
      vcs_boresight_el.push_back(SensInfo[i].constant.mounting_position.vcs_boresight_elevation_angle);
      vacs_boresight_az_est.push_back(SensInfo[i].variable.vacs_boresight_az_estimated);
      vacs_boresight_el_est.push_back(SensInfo[i].variable.vacs_boresight_el_estimated);
      num_valid_dets.push_back(SensInfo[i].variable.number_of_valid_detections);
      rain_level.push_back(SensInfo[i].variable.overall_rain_level);
      look_idx.push_back(SensInfo[i].variable.look_index);
      valid_flag.push_back(SensInfo[i].variable.is_valid);
      sensor_fault_flag.push_back(SensInfo[i].variable.f_sensor_fault_detected);
      // look_id may be a scalar per sensor; store as single-element vector per sensor
      look_ids.push_back(SensInfo[i].variable.look_id);
      min_host_vel.push_back(SensInfo[i].constant.internal_reflections.min_host_vel);
      occurrence_lowerlimit.push_back(SensInfo[i].constant.internal_reflections.occurrence_lowerlimit);
      occurrence_threshold.push_back(SensInfo[i].constant.internal_reflections.occurrence_threshold);
      rcs_tolerance.push_back(SensInfo[i].constant.internal_reflections.rcs_tolerance);
      azimuth_tolerance.push_back(SensInfo[i].constant.internal_reflections.azimuth_tolerance);
      range_tolerance.push_back(SensInfo[i].constant.internal_reflections.range_tolerance);
      rcs_max.push_back(SensInfo[i].constant.internal_reflections.rcs_max);
      range_max.push_back(SensInfo[i].constant.internal_reflections.range_max);
      age_threshold.push_back(SensInfo[i].constant.internal_reflections.age_threshold);
      f_enable_flags.push_back(SensInfo[i].constant.internal_reflections.f_enable);
   }

   hdf_sensor_info_ptr.min_host_vel.push_back(min_host_vel);
   hdf_sensor_info_ptr.occurrence_lowerlimit.push_back(occurrence_lowerlimit);
   hdf_sensor_info_ptr.occurrence_threshold.push_back(occurrence_threshold);
   hdf_sensor_info_ptr.rcs_tolerance.push_back(rcs_tolerance);
   hdf_sensor_info_ptr.azimuth_tolerance.push_back(azimuth_tolerance);
   hdf_sensor_info_ptr.range_tolerance.push_back(range_tolerance);
   hdf_sensor_info_ptr.rcs_max.push_back(rcs_max);
   hdf_sensor_info_ptr.range_max.push_back(range_max);
   hdf_sensor_info_ptr.age_threshold.push_back(age_threshold);
   hdf_sensor_info_ptr.f_enable.push_back(f_enable_flags);
   hdf_sensor_info_ptr.useful_FOV.push_back(useful_FOV);

   hdf_sensor_info_ptr.id.push_back(ids);
   hdf_sensor_info_ptr.polarity.push_back(polarities);
   hdf_sensor_info_ptr.mounting_location.push_back(mount_locs);
   hdf_sensor_info_ptr.sensor_type.push_back(sensor_types);
   hdf_sensor_info_ptr.timestamp_us.push_back(timestamps);
   hdf_sensor_info_ptr.vcs_velocityLat.push_back(vcs_vel_lat);
   hdf_sensor_info_ptr.vcs_velocityLong.push_back(vcs_vel_long);
   hdf_sensor_info_ptr.vcs_Longitude.push_back(vcs_lon);
   hdf_sensor_info_ptr.vcs_lateral.push_back(vcs_lat);
   hdf_sensor_info_ptr.vcs_height.push_back(vcs_height);
   hdf_sensor_info_ptr.vcs_boresight_azimuth_angle.push_back(vcs_boresight_az);
   hdf_sensor_info_ptr.vcs_boresight_elevation_angle.push_back(vcs_boresight_el);
   hdf_sensor_info_ptr.vacs_boresight_az_estimated.push_back(vacs_boresight_az_est);
   hdf_sensor_info_ptr.vacs_boresight_el_estimated.push_back(vacs_boresight_el_est);
   hdf_sensor_info_ptr.number_of_valid_detections.push_back(num_valid_dets);
   hdf_sensor_info_ptr.overall_rain_level.push_back(rain_level);
   hdf_sensor_info_ptr.look_index.push_back(look_idx);
   hdf_sensor_info_ptr.is_valid.push_back(valid_flag);
   hdf_sensor_info_ptr.f_sensor_fault_detected.push_back(sensor_fault_flag);
   hdf_sensor_info_ptr.look_id.push_back(look_ids);

   // Populate all object information
   hdf_all_objects_ptr.data_timstamp_us.push_back(all_objects->f360header.data_timstamp_us);
   hdf_all_objects_ptr.num_elements.push_back(all_objects->f360header.num_elements);
   hdf_all_objects_ptr.version.push_back(all_objects->f360header.version);
   std::vector<float32_T> vcs_xposn;
   std::vector<float32_T> vcs_yposn;
   std::vector<float32_T> vcs_xvel;
   std::vector<float32_T> vcs_yvel;
   std::vector<float32_T> vcs_xaccel;
   std::vector<float32_T> vcs_yaccel;
   std::vector<float32_T> vcs_heading;
   std::vector<float32_T> vcs_pointing;
   std::vector<float32_T> speed_1;
   std::vector<float32_T> curvature;
   std::vector<float32_T> tang_accel;
   std::vector<std::vector<float32_T>> state_variance_2d;
   std::vector<std::vector<float32_T>> supplemental_state_covariance_2d;
   std::vector<float32_T> time_since_measurement;
   std::vector<float32_T> time_since_cluster_created;
   std::vector<float32_T> time_since_track_updated;
   std::vector<float32_T> len1;
   std::vector<float32_T> len2;
   std::vector<float32_T> wid1;
   std::vector<float32_T> wid2;
   std::vector<float32_T> confidenceLevel;
   std::vector<float32_T> time_since_stage_start;
   std::vector<float32_T> existence_probability;
   std::vector<float32_T> accuracy_length;
   std::vector<float32_T> accuracy_width;
   std::vector<float32_T> probability_pedestrian;
   std::vector<float32_T> probability_car;
   std::vector<float32_T> probability_motorcycle;
   std::vector<float32_T> probability_bicycle;
   std::vector<float32_T> probability_truck;
   std::vector<float32_T> probability_undet;
   std::vector<unsigned16_T> trkID;
   std::vector<unsigned16_T> ndets;
   std::vector<unsigned16_T> num_reduced_dets;
   std::vector<unsigned8_T> reducedID;
   std::vector<unsigned8_T> trk_fltr_type;
   std::vector<unsigned8_T> status;
   std::vector<unsigned8_T> reducedStatus;
   std::vector<unsigned8_T> init_scheme;
   std::vector<unsigned8_T> object_class;
   std::vector<unsigned8_T> f_crossing;
   std::vector<unsigned8_T> f_moving;
   std::vector<unsigned8_T> f_moveable;
   std::vector<unsigned8_T> f_oncoming;
   std::vector<unsigned8_T> f_vehicular_trk;
   std::vector<unsigned8_T> f_onguardrail;
   std::vector<unsigned8_T> f_fast_moving;
   std::vector<unsigned8_T> underdrivable_status;
   std::vector<unsigned32_T> unique_id;
   std::vector<unsigned8_T> reference_pointAll;
   std::vector<unsigned8_T> drivable_status_sgAll;
   std::vector<unsigned8_T> drivable_confidence_sgAll;
   std::vector<float32_T> otg_heightAll;
   std::vector<float32_T> probability_underdrivable_ocgAll;
   std::vector<float32_T> radar_cross_sectionAll;
   std::vector<unsigned16_T> num_rr_inlier_dets;
   std::vector<unsigned16_T> num_dets_used_in_rr_msmt_updateAll;
   for (int i = 0; i < MAX_F360_OBJECTS_LOG; i++) {
      unique_id.push_back(all_objects->object[i].unique_id);
      reference_pointAll.push_back(all_objects->object[i].reference_point);
      drivable_status_sgAll.push_back(all_objects->object[i].drivable_status_sg);
      otg_heightAll.push_back(all_objects->object[i].otg_height);
      probability_underdrivable_ocgAll.push_back(all_objects->object[i].probability_underdrivable_ocg);
      radar_cross_sectionAll.push_back(all_objects->object[i].radar_cross_section);
      num_rr_inlier_dets.push_back(all_objects->object[i].num_rr_inlier_dets);
      num_dets_used_in_rr_msmt_updateAll.push_back(all_objects->object[i].num_dets_used_in_rr_msmt_update);
      vcs_xposn.push_back(all_objects->object[i].vcs_xposn);
      vcs_yposn.push_back(all_objects->object[i].vcs_yposn);
      vcs_xvel.push_back(all_objects->object[i].vcs_xvel);
      vcs_yvel.push_back(all_objects->object[i].vcs_yvel);
      vcs_xaccel.push_back(all_objects->object[i].vcs_xaccel);
      vcs_yaccel.push_back(all_objects->object[i].vcs_yaccel);
      vcs_heading.push_back(all_objects->object[i].vcs_heading);
      vcs_pointing.push_back(all_objects->object[i].vcs_pointing);
      speed_1.push_back(all_objects->object[i].speed);
      curvature.push_back(all_objects->object[i].curvature);
      tang_accel.push_back(all_objects->object[i].tang_accel);
      std::vector<float32_T> state_variance_1d;
      std::vector<float32_T> supplemental_state_covariance_1d;
      for (int j = 0; j < STATE_VARIANCE_ARRAY_SIZE; j++) {
         state_variance_1d.push_back(all_objects->object[i].state_variance[j]);
      }
      for (int k = 0; k < SUPPLEMENTAL_STATE_COVARIANCE_ARRAY_SIZE; k++) {
         supplemental_state_covariance_1d.push_back(all_objects->object[i].supplemental_state_covariance[k]);
      }
      state_variance_2d.push_back(state_variance_1d);
      supplemental_state_covariance_2d.push_back(supplemental_state_covariance_1d);
      time_since_measurement.push_back(all_objects->object[i].time_since_measurement);
      time_since_cluster_created.push_back(all_objects->object[i].time_since_cluster_created);
      time_since_track_updated.push_back(all_objects->object[i].time_since_track_updated);
      len1.push_back(all_objects->object[i].len1);
      len2.push_back(all_objects->object[i].len2);
      wid1.push_back(all_objects->object[i].wid1);
      wid2.push_back(all_objects->object[i].wid2);
      confidenceLevel.push_back(all_objects->object[i].confidenceLevel);
      time_since_stage_start.push_back(all_objects->object[i].time_since_stage_start);
      existence_probability.push_back(all_objects->object[i].existence_probability);
      accuracy_length.push_back(all_objects->object[i].accuracy_length);
      accuracy_width.push_back(all_objects->object[i].accuracy_width);
      probability_pedestrian.push_back(all_objects->object[i].probability_pedestrian);
      probability_car.push_back(all_objects->object[i].probability_car);
      probability_motorcycle.push_back(all_objects->object[i].probability_motorcycle);
      probability_bicycle.push_back(all_objects->object[i].probability_bicycle);
      probability_truck.push_back(all_objects->object[i].probability_truck);
      probability_undet.push_back(all_objects->object[i].probability_undet);
      trkID.push_back(all_objects->object[i].trkID);
      ndets.push_back(all_objects->object[i].ndets);
      num_reduced_dets.push_back(static_cast<unsigned16_T>(f360_variant_A::NUMBER_OF_REDUCED_OBJECT_TRACKS));
      reducedID.push_back(static_cast<unsigned8_T>(all_objects->object[i].reducedID));
      trk_fltr_type.push_back(all_objects->object[i].trk_fltr_type);
      status.push_back(all_objects->object[i].status);
      reducedStatus.push_back(all_objects->object[i].reducedStatus);
      init_scheme.push_back(all_objects->object[i].init_scheme);
      object_class.push_back(all_objects->object[i].object_class);
      f_crossing.push_back(all_objects->object[i].f_crossing);
      f_moving.push_back(all_objects->object[i].f_moving);
      f_moveable.push_back(all_objects->object[i].f_moveable);
      f_oncoming.push_back(all_objects->object[i].f_oncoming);
      f_vehicular_trk.push_back(all_objects->object[i].f_vehicular_trk);
      f_onguardrail.push_back(all_objects->object[i].f_onguardrail);
      f_fast_moving.push_back(all_objects->object[i].f_fast_moving);
      underdrivable_status.push_back(all_objects->object[i].underdrivable_status_ocg);
   }
   hdf_all_objects_ptr.unique_id.push_back(unique_id);
   hdf_all_objects_ptr.reference_point.push_back(reference_pointAll);
   hdf_all_objects_ptr.drivable_status_sg.push_back(drivable_status_sgAll);
   hdf_all_objects_ptr.drivable_confidence_sg.push_back(drivable_confidence_sgAll);
   hdf_all_objects_ptr.otg_height.push_back(otg_heightAll);
   hdf_all_objects_ptr.probability_underdrivable_ocg.push_back(probability_underdrivable_ocgAll);
   hdf_all_objects_ptr.radar_cross_section.push_back(radar_cross_sectionAll);
   hdf_all_objects_ptr.num_rr_inlier_dets.push_back(num_rr_inlier_dets);
   hdf_all_objects_ptr.num_dets_used_in_rr_msmt_update.push_back(num_dets_used_in_rr_msmt_updateAll);

   hdf_all_objects_ptr.vcs_xposn.push_back(vcs_xposn);
   hdf_all_objects_ptr.vcs_yposn.push_back(vcs_yposn);
   hdf_all_objects_ptr.vcs_xvel.push_back(vcs_xvel);
   hdf_all_objects_ptr.vcs_yvel.push_back(vcs_yvel);
   hdf_all_objects_ptr.vcs_xaccel.push_back(vcs_xaccel);
   hdf_all_objects_ptr.vcs_yaccel.push_back(vcs_yaccel);
   hdf_all_objects_ptr.vcs_heading.push_back(vcs_heading);
   hdf_all_objects_ptr.vcs_pointing.push_back(vcs_pointing);
   hdf_all_objects_ptr.speed.push_back(speed_1);
   hdf_all_objects_ptr.curvature.push_back(curvature);
   hdf_all_objects_ptr.tang_accel.push_back(tang_accel);
   hdf_all_objects_ptr.state_variance.push_back(state_variance_2d);
   hdf_all_objects_ptr.supplemental_state_covariance.push_back(supplemental_state_covariance_2d);
   hdf_all_objects_ptr.time_since_measurement.push_back(time_since_measurement);
   hdf_all_objects_ptr.time_since_cluster_created.push_back(time_since_cluster_created);
   hdf_all_objects_ptr.time_since_track_updated.push_back(time_since_track_updated);
   hdf_all_objects_ptr.len1.push_back(len1);
   hdf_all_objects_ptr.len2.push_back(len2);
   hdf_all_objects_ptr.wid1.push_back(wid1);
   hdf_all_objects_ptr.wid2.push_back(wid2);
   hdf_all_objects_ptr.confidenceLevel.push_back(confidenceLevel);
   hdf_all_objects_ptr.time_since_stage_start.push_back(time_since_stage_start);
   hdf_all_objects_ptr.existence_probability.push_back(existence_probability);
   hdf_all_objects_ptr.accuracy_length.push_back(accuracy_length);
   hdf_all_objects_ptr.accuracy_width.push_back(accuracy_width);
   hdf_all_objects_ptr.probability_pedestrian.push_back(probability_pedestrian);
   hdf_all_objects_ptr.probability_car.push_back(probability_car);
   hdf_all_objects_ptr.probability_motorcycle.push_back(probability_motorcycle);
   hdf_all_objects_ptr.probability_bicycle.push_back(probability_bicycle);
   hdf_all_objects_ptr.probability_truck.push_back(probability_truck);
   hdf_all_objects_ptr.probability_undet.push_back(probability_undet);
   hdf_all_objects_ptr.trkID.push_back(trkID);
   hdf_all_objects_ptr.ndets.push_back(ndets);
   hdf_all_objects_ptr.num_reduced_dets.push_back(num_reduced_dets);
   hdf_all_objects_ptr.reducedID.push_back(reducedID);
   hdf_all_objects_ptr.trk_fltr_type.push_back(trk_fltr_type);
   hdf_all_objects_ptr.status.push_back(status);
   hdf_all_objects_ptr.reducedStatus.push_back(reducedStatus);
   hdf_all_objects_ptr.init_scheme.push_back(init_scheme);
   hdf_all_objects_ptr.object_class.push_back(object_class);
   hdf_all_objects_ptr.f_crossing.push_back(f_crossing);
   hdf_all_objects_ptr.f_moving.push_back(f_moving);
   hdf_all_objects_ptr.f_moveable.push_back(f_moveable);
   hdf_all_objects_ptr.f_oncoming.push_back(f_oncoming);
   hdf_all_objects_ptr.f_vehicular_trk.push_back(f_vehicular_trk);
   hdf_all_objects_ptr.f_onguardrail.push_back(f_onguardrail);
   hdf_all_objects_ptr.f_fast_moving.push_back(f_fast_moving);
   hdf_all_objects_ptr.underdrivable_status.push_back(underdrivable_status);

   // Populate detection information
   hdf_processed_info_ptr.number_of_valid_detections.push_back(DetList->number_of_valid_detections);
   hdf_processed_info_ptr.vcslong_det_idx_max.push_back(DetList->vcslong_det_idx_max);
   hdf_processed_info_ptr.vcslong_det_idx_min.push_back(DetList->vcslong_det_idx_min);

   std::vector<signed16_T> vcslong_sorted_ref_det_idx;

   for (int j = 0; j < f360_variant_A::MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; j++) {
      vcslong_sorted_ref_det_idx.push_back(DetList->vcslong_sorted_ref_det_idx[j]);
   }
   hdf_processed_info_ptr.vcslong_sorted_ref_det_idx.push_back(vcslong_sorted_ref_det_idx);

   std::vector<float32_t> RD_range;
   std::vector<float32_t> RD_std_range;
   std::vector<float32_t> RD_range_rate;
   std::vector<float32_t> RD_std_range_rate;
   std::vector<float32_t> RD_azimuth;
   std::vector<float32_t> RD_std_azimuth;
   std::vector<float32_t> RD_elevation;
   std::vector<float32_t> RD_std_elevation;
   std::vector<float32_t> RD_snr;
   std::vector<float32_t> RD_rcs;
   std::vector<float32_t> RD_prob_1stazhypo;
   std::vector<int32_t> RD_sensor_id;
   std::vector<int32_t> RD_det_id;
   std::vector<int8_t> RD_confid_azimuth;
   std::vector<int8_t> RD_confid_elevation;
   std::vector<boolean_T> RD_f_super_res;
   std::vector<boolean_T> RD_f_host_veh_clutter;
   std::vector<boolean_T> RD_f_nd_target;
   std::vector<boolean_T> RD_f_bistatic;
   std::vector<boolean_T> RD_f_ci_det;
   std::vector<boolean_T> RD_f_idm_det;
   std::vector<boolean_T> RD_f_below_rain_thold;

   std::vector<float32_t> PD_vcs_position_x;
   std::vector<float32_t> PD_vcs_position_y;
   std::vector<float32_t> PD_vcs_position_z;
   std::vector<float32_t> PD_range_rate_compensated;
   std::vector<float32_t> PD_vcs_az;
   std::vector<float32_t> PD_vcs_el;
   std::vector<float32_t> PD_cos_vcs_az;
   std::vector<float32_t> PD_sin_vcs_az;
   std::vector<signed16_T> PD_next_sorted_idx;
   std::vector<signed16_T> PD_prev_sorted_idx;
   std::vector<signed8_T> PD_motion_status;
   std::vector<boolean_T> PD_f_ok_to_use;

   for (int i = 0; i < f360_variant_A::MAX_NUMBER_OF_DETECTIONS; i++) {
      RD_range.push_back(DetList->detections[i].raw.range);
      RD_std_range.push_back(DetList->detections[i].raw.std_range);
      RD_range_rate.push_back(DetList->detections[i].raw.range_rate);
      RD_std_range_rate.push_back(DetList->detections[i].raw.std_range_rate);
      RD_azimuth.push_back(DetList->detections[i].raw.azimuth);
      RD_std_azimuth.push_back(DetList->detections[i].raw.std_azimuth);
      RD_elevation.push_back(DetList->detections[i].raw.elevation);
      RD_std_elevation.push_back(DetList->detections[i].raw.std_elevation);
      RD_snr.push_back(DetList->detections[i].raw.snr);
      RD_rcs.push_back(DetList->detections[i].raw.rcs);
      RD_prob_1stazhypo.push_back(DetList->detections[i].raw.prob_1stazhypo);
      RD_sensor_id.push_back(DetList->detections[i].raw.sensor_id);
      RD_det_id.push_back(DetList->detections[i].raw.det_id);
      RD_confid_azimuth.push_back(DetList->detections[i].raw.confid_azimuth);
      RD_confid_elevation.push_back(DetList->detections[i].raw.confid_elevation);
      RD_f_super_res.push_back(DetList->detections[i].raw.f_super_res);
      RD_f_host_veh_clutter.push_back(DetList->detections[i].raw.f_host_veh_clutter);
      RD_f_nd_target.push_back(DetList->detections[i].raw.f_nd_target);
      RD_f_bistatic.push_back(DetList->detections[i].raw.f_bistatic);
      RD_f_ci_det.push_back(DetList->detections[i].raw.f_ci_det);
      RD_f_idm_det.push_back(DetList->detections[i].raw.f_idm_det);
      RD_f_below_rain_thold.push_back(DetList->detections[i].raw.f_below_rain_thold);

      PD_vcs_position_x.push_back(DetList->detections[i].processed.vcs_position_x);
      PD_vcs_position_y.push_back(DetList->detections[i].processed.vcs_position_y);
      PD_vcs_position_z.push_back(DetList->detections[i].processed.vcs_position_z);

      PD_range_rate_compensated.push_back(DetList->detections[i].processed.range_rate_compensated);
      PD_vcs_az.push_back(DetList->detections[i].processed.vcs_az);
      PD_vcs_el.push_back(DetList->detections[i].processed.vcs_el);
      PD_cos_vcs_az.push_back(DetList->detections[i].processed.cos_vcs_az);
      PD_sin_vcs_az.push_back(DetList->detections[i].processed.sin_vcs_az);
      PD_next_sorted_idx.push_back(DetList->detections[i].processed.next_sorted_idx);
      PD_prev_sorted_idx.push_back(DetList->detections[i].processed.prev_sorted_idx);
      PD_motion_status.push_back(DetList->detections[i].processed.motion_status);
      PD_f_ok_to_use.push_back(DetList->detections[i].processed.f_ok_to_use);
   }

   hdf_processed_info_ptr.RD_range.push_back(RD_range);
   hdf_processed_info_ptr.RD_std_range.push_back(RD_std_range);
   hdf_processed_info_ptr.RD_range_rate.push_back(RD_range_rate);
   hdf_processed_info_ptr.RD_std_range_rate.push_back(RD_std_range_rate);
   hdf_processed_info_ptr.RD_azimuth.push_back(RD_azimuth);
   hdf_processed_info_ptr.RD_std_azimuth.push_back(RD_std_azimuth);
   hdf_processed_info_ptr.RD_elevation.push_back(RD_elevation);
   hdf_processed_info_ptr.RD_std_elevation.push_back(RD_std_elevation);
   hdf_processed_info_ptr.RD_snr.push_back(RD_snr);
   hdf_processed_info_ptr.RD_rcs.push_back(RD_rcs);
   hdf_processed_info_ptr.RD_prob_1stazhypo.push_back(RD_prob_1stazhypo);
   hdf_processed_info_ptr.RD_sensor_id.push_back(RD_sensor_id);
   hdf_processed_info_ptr.RD_det_id.push_back(RD_det_id);
   hdf_processed_info_ptr.RD_confid_azimuth.push_back(RD_confid_azimuth);
   hdf_processed_info_ptr.RD_confid_elevation.push_back(RD_confid_elevation);
   hdf_processed_info_ptr.RD_f_super_res.push_back(RD_f_super_res);
   hdf_processed_info_ptr.RD_f_host_veh_clutter.push_back(RD_f_host_veh_clutter);
   hdf_processed_info_ptr.RD_f_nd_target.push_back(RD_f_nd_target);
   hdf_processed_info_ptr.RD_f_bistatic.push_back(RD_f_bistatic);
   hdf_processed_info_ptr.RD_f_ci_det.push_back(RD_f_ci_det);
   hdf_processed_info_ptr.RD_f_idm_det.push_back(RD_f_idm_det);
   hdf_processed_info_ptr.RD_f_below_rain_thold.push_back(RD_f_below_rain_thold);

   hdf_processed_info_ptr.PD_vcs_position_x.push_back(PD_vcs_position_x);
   hdf_processed_info_ptr.PD_vcs_position_y.push_back(PD_vcs_position_y);
   hdf_processed_info_ptr.PD_vcs_position_z.push_back(PD_vcs_position_z);
   hdf_processed_info_ptr.PD_range_rate_compensated.push_back(PD_range_rate_compensated);
   hdf_processed_info_ptr.PD_vcs_az.push_back(PD_vcs_az);
   hdf_processed_info_ptr.PD_vcs_el.push_back(PD_vcs_el);
   hdf_processed_info_ptr.PD_cos_vcs_az.push_back(PD_cos_vcs_az);
   hdf_processed_info_ptr.PD_sin_vcs_az.push_back(PD_sin_vcs_az);
   hdf_processed_info_ptr.PD_next_sorted_idx.push_back(PD_next_sorted_idx);
   hdf_processed_info_ptr.PD_prev_sorted_idx.push_back(PD_prev_sorted_idx);
   hdf_processed_info_ptr.PD_motion_status.push_back(PD_motion_status);
   hdf_processed_info_ptr.PD_f_ok_to_use.push_back(PD_f_ok_to_use);
   // Populate vse output information

   hdf_vse_output_ptr.timestamp_us.push_back(VsePtr->VsVSE_us_Timestamp);
   hdf_vse_output_ptr.raw_speed_mps.push_back(VsePtr->VsVSE_mps_VehRawSpd);
   hdf_vse_output_ptr.speed_compensation_factor.push_back(VsePtr->VeVSE_VehRawSpdQF);
   hdf_vse_output_ptr.filt_veh_speed_over_ground.push_back(VsePtr->VsVSE_mps_VehFiltSpdOverGround);
   hdf_vse_output_ptr.raw_lat_accel.push_back(VsePtr->VsVSE_mps2_RawLatAccel);
   hdf_vse_output_ptr.raw_long_accel.push_back(VsePtr->VsVSE_mps2_RawLongAccel);
   hdf_vse_output_ptr.raw_yaw_rate_rps.push_back(VsePtr->VsVSE_rps_RawYawRate);
   hdf_vse_output_ptr.raw_steering_angle_deg.push_back(VsePtr->VsVSE_deg_RawSteeringAngle);
   hdf_vse_output_ptr.road_wheel_angle_deg.push_back(VsePtr->VsVSE_deg_RoadWhlAngle);
   hdf_vse_output_ptr.yaw_rate_sa.push_back(VsePtr->VsVSE_rps_YawRateSA);
   hdf_vse_output_ptr.yaw_rate_raw_bias.push_back(VsePtr->VsVSE_rps_YawRateBias);
   hdf_vse_output_ptr.comp_yaw_rate_unfiltered.push_back(VsePtr->VsVSE_rps_CompYawRateUnfilt);
   hdf_vse_output_ptr.comp_yaw_rate_filtered.push_back(VsePtr->VsVSE_rps_CompYawRateFilt);
   hdf_vse_output_ptr.curvature_rear_axle.push_back(VsePtr->VsVSE_CurvatureRearAxle);
   hdf_vse_output_ptr.sideslip_rear_axle.push_back(VsePtr->VsVSE_rad_SideslipRearAxle);
   hdf_vse_output_ptr.vcs_sideslip.push_back(VsePtr->VsVSE_rad_VCSSideslip);
   hdf_vse_output_ptr.vcs_long_velocity.push_back(VsePtr->VsVSE_mps_VCSLongVel);
   hdf_vse_output_ptr.vcs_lat_velocity.push_back(VsePtr->VsVSE_mps_VCSLatVel);
   hdf_vse_output_ptr.sensor_sideslip.push_back(VsePtr->VsVSE_rad_SensorSideslip);
   hdf_vse_output_ptr.sensor_long_velocity.push_back(VsePtr->VsVSE_mps_SensorLongVel);
   hdf_vse_output_ptr.sensor_lat_velocity.push_back(VsePtr->VsVSE_mps_SensorLatVel);
   hdf_vse_output_ptr.k_dist_rear_axle_to_vcs.push_back(VsePtr->KsVSE_m_DistRearAxleToVCS);
   hdf_vse_output_ptr.vcs_lat_accel.push_back(VsePtr->VsVSE_mps2_VCSLatAccel);
   hdf_vse_output_ptr.vcs_long_accel.push_back(VsePtr->VsVSE_mps2_VCSLongAccel);
   hdf_vse_output_ptr.accel_rear_axle.push_back(VsePtr->VsVSE_mps2_COGLongAccel);
   hdf_vse_output_ptr.signed_filt_veh_speed_over_ground.push_back(VsePtr->VsVSE_mps_VehFiltSignedSpdOverGround);
   hdf_vse_output_ptr.veh_index.push_back(VsePtr->VsVSE_VehIndex);
   hdf_vse_output_ptr.raw_speed_qf.push_back(VsePtr->VeVSE_VehRawSpdQF);
   hdf_vse_output_ptr.speed_compensation_factor_qf.push_back(VsePtr->VeVSE_SpdCompFactorQF);
   hdf_vse_output_ptr.filt_veh_speed_over_ground_qf.push_back(VsePtr->VeVSE_VehFiltSpdOverGroundQF);
   hdf_vse_output_ptr.raw_lat_accel_qf.push_back(VsePtr->VeVSE_RawLatAccelQF);
   hdf_vse_output_ptr.raw_long_accel_qf.push_back(VsePtr->VeVSE_RawLongAccelQF);
   hdf_vse_output_ptr.raw_yaw_rate_qf.push_back(VsePtr->VeVSE_RawYawRateQF);
   hdf_vse_output_ptr.raw_steering_angle_qf.push_back(VsePtr->VeVSE_RawSteeringAngleQF);
   hdf_vse_output_ptr.road_wheel_angle_qf.push_back(VsePtr->VeVSE_RoadWhlAngleQF);
   hdf_vse_output_ptr.yaw_rate_sa_qf.push_back(VsePtr->VeVSE_YawRateSAQF);
   hdf_vse_output_ptr.yaw_rate_bias_qf.push_back(VsePtr->VeVSE_YawRateBiasQF);
   hdf_vse_output_ptr.comp_yaw_rate_qf.push_back(VsePtr->VeVSE_CompYawRateQF);
   hdf_vse_output_ptr.stationary.push_back(VsePtr->VsVSE_b_VehStationary);
   hdf_vse_output_ptr.vcs_lat_accel_qf.push_back(VsePtr->VeVSE_VCSLatAccelQF);
   hdf_vse_output_ptr.vcs_long_accel_qf.push_back(VsePtr->VeVSE_VCSLongAccelQF);

   // Populate ROT info

   hdf_ROT_info_ptr.rot_object_list_timestamp.push_back(ROT_Obj->rot_object_list_timestamp);
   hdf_ROT_info_ptr.tracker_start_timestamp.push_back(ROT_Obj->tracker_start_timestamp);
   hdf_ROT_info_ptr.tracker_elapsed_time.push_back(ROT_Obj->tracker_elapsed_time);
   hdf_ROT_info_ptr.tracker_index.push_back(ROT_Obj->tracker_index);
   hdf_ROT_info_ptr.number_of_objects.push_back(ROT_Obj->number_of_objects);

   std::vector<float32_T> vcs_x_posn;
   std::vector<float32_T> vcs_y_posn;
   std::vector<float32_T> vcs_x_vel;
   std::vector<float32_T> vcs_y_vel;
   std::vector<float32_T> vcs_x_acc;
   std::vector<float32_T> vcs_y_acc;
   std::vector<float32_T> vcs_heading1;
   std::vector<float32_T> vcs_pointing1;
   std::vector<float32_T> vcs_curvature;
   std::vector<std::vector<float32_T>> vcs_state_variance_2d;
   std::vector<std::vector<float32_T>> vcs_supplemental_state_covariance_2d;

   std::vector<float32_T> iso_x_posn;
   std::vector<float32_T> iso_y_posn;
   std::vector<float32_T> iso_x_vel;
   std::vector<float32_T> iso_relative_x_vel;
   std::vector<float32_T> iso_y_vel;
   std::vector<float32_T> iso_relative_y_vel;
   std::vector<float32_T> iso_x_acc;
   std::vector<float32_T> iso_relative_x_acc;
   std::vector<float32_T> iso_y_acc;
   std::vector<float32_T> iso_relative_y_acc;

   std::vector<float32_T> iso_orientation;
   std::vector<float32_T> iso_orientation_var;
   std::vector<float32_T> iso_orientation_rate;
   std::vector<float32_T> iso_orientation_rate_var;

   std::vector<float32_T> iso_x_posn_var;
   std::vector<float32_T> iso_y_posn_var;
   std::vector<float32_T> iso_xy_posn_cov;
   std::vector<float32_T> iso_x_vel_var;
   std::vector<float32_T> iso_y_vel_var;
   std::vector<float32_T> iso_xy_vel_cov;
   std::vector<float32_T> iso_x_acc_var;
   std::vector<float32_T> iso_y_acc_var;
   std::vector<float32_T> iso_xy_acc_cov;

   std::vector<float32_T> speed;
   std::vector<float32_T> tang_accel1;
   std::vector<float32_T> length;
   std::vector<float32_T> length_var;
   std::vector<float32_T> width;
   std::vector<float32_T> width_var;
   std::vector<float32_T> time_since_created;
   std::vector<float32_T> time_since_published;
   std::vector<float32_T> time_since_stage_start1;
   std::vector<float32_T> existence_probability1;
   std::vector<float32_T> mirror_prob;
   std::vector<float32_T> radar_cross_section;
   std::vector<float32_T> otg_height;
   std::vector<float32_T> confidence_level;

   std::vector<float32_T> probability_pedestrian1;
   std::vector<float32_T> probability_car1;
   std::vector<float32_T> probability_motorcycle1;
   std::vector<float32_T> probability_bicycle1;
   std::vector<float32_T> probability_truck1;
   std::vector<float32_T> probability_undet1;
   std::vector<float32_T> probability_underdrivable_ocg;
   std::vector<float32_T> movable_prob;

   std::vector<signed32_T> id_ROT;
   std::vector<unsigned32_T> unique_id_ROT;
   std::vector<unsigned32_T> ndets1;
   std::vector<unsigned32_T> num_dets_used_in_rr_msmt_update;
   std::vector<unsigned16_T> sensor_src;

   std::vector<unsigned8_T> reference_point;
   std::vector<unsigned8_T> object_status;
   std::vector<unsigned8_T> object_class1;
   std::vector<unsigned8_T> movement_status;
   std::vector<unsigned8_T> occlusion_status;
   std::vector<unsigned8_T> underdrivable_status_ocg;
   std::vector<unsigned8_T> drivable_status_sg;
   std::vector<unsigned8_T> drivable_confidence_sg;
   std::vector<unsigned8_T> f_onguardrail1;
   std::vector<unsigned8_T> trk_fltr_type1;

   for (int i = 0; i < f360_variant_A::NUMBER_OF_REDUCED_OBJECT_TRACKS; i++) {
      vcs_x_posn.push_back(ROT_Obj->rot_object_list[i].vcs_x_posn);
      vcs_y_posn.push_back(ROT_Obj->rot_object_list[i].vcs_y_posn);
      vcs_x_vel.push_back(ROT_Obj->rot_object_list[i].vcs_x_vel);
      vcs_y_vel.push_back(ROT_Obj->rot_object_list[i].vcs_y_vel);
      vcs_x_acc.push_back(ROT_Obj->rot_object_list[i].vcs_x_acc);
      vcs_y_acc.push_back(ROT_Obj->rot_object_list[i].vcs_y_acc);
      vcs_heading1.push_back(ROT_Obj->rot_object_list[i].vcs_heading);
      vcs_pointing1.push_back(ROT_Obj->rot_object_list[i].vcs_pointing);
      vcs_curvature.push_back(ROT_Obj->rot_object_list[i].vcs_curvature);
      std::vector<float32_T> vcs_state_variance;
      std::vector<float32_T> vcs_supplemental_state_covariance;

      for (int j = 0; j < 6; j++) {
         vcs_state_variance.push_back(ROT_Obj->rot_object_list[i].vcs_state_variance[j]);
      }
      for (int k = 0; k < 3; k++) {
         vcs_supplemental_state_covariance.push_back(ROT_Obj->rot_object_list[i].vcs_supplemental_state_covariance[k]);
      }

      vcs_state_variance_2d.push_back(vcs_state_variance);
      vcs_supplemental_state_covariance_2d.push_back(vcs_supplemental_state_covariance);
      iso_x_posn.push_back(ROT_Obj->rot_object_list[i].iso_x_posn);
      iso_y_posn.push_back(ROT_Obj->rot_object_list[i].iso_y_posn);
      iso_x_vel.push_back(ROT_Obj->rot_object_list[i].iso_x_vel);
      iso_relative_x_vel.push_back(ROT_Obj->rot_object_list[i].iso_relative_x_vel);
      iso_y_vel.push_back(ROT_Obj->rot_object_list[i].iso_y_vel);
      iso_relative_y_vel.push_back(ROT_Obj->rot_object_list[i].iso_relative_y_vel);
      iso_x_acc.push_back(ROT_Obj->rot_object_list[i].iso_x_acc);
      iso_relative_x_acc.push_back(ROT_Obj->rot_object_list[i].iso_relative_x_acc);
      iso_y_acc.push_back(ROT_Obj->rot_object_list[i].iso_y_acc);
      iso_relative_y_acc.push_back(ROT_Obj->rot_object_list[i].iso_relative_y_acc);
      iso_orientation.push_back(ROT_Obj->rot_object_list[i].iso_orientation);
      iso_orientation_var.push_back(ROT_Obj->rot_object_list[i].iso_orientation_var);
      iso_orientation_rate.push_back(ROT_Obj->rot_object_list[i].iso_orientation_rate);
      iso_orientation_rate_var.push_back(ROT_Obj->rot_object_list[i].iso_orientation_rate_var);
      iso_x_posn_var.push_back(ROT_Obj->rot_object_list[i].iso_x_posn_var);
      iso_y_posn_var.push_back(ROT_Obj->rot_object_list[i].iso_y_posn_var);
      iso_xy_posn_cov.push_back(ROT_Obj->rot_object_list[i].iso_xy_posn_cov);
      iso_x_vel_var.push_back(ROT_Obj->rot_object_list[i].iso_x_vel_var);
      iso_y_vel_var.push_back(ROT_Obj->rot_object_list[i].iso_y_vel_var);
      iso_xy_vel_cov.push_back(ROT_Obj->rot_object_list[i].iso_xy_vel_cov);
      iso_x_acc_var.push_back(ROT_Obj->rot_object_list[i].iso_x_acc_var);
      iso_y_acc_var.push_back(ROT_Obj->rot_object_list[i].iso_y_acc_var);
      iso_xy_acc_cov.push_back(ROT_Obj->rot_object_list[i].iso_xy_acc_cov);
      speed.push_back(ROT_Obj->rot_object_list[i].speed);
      tang_accel1.push_back(ROT_Obj->rot_object_list[i].tang_accel);
      length.push_back(ROT_Obj->rot_object_list[i].length);
      length_var.push_back(ROT_Obj->rot_object_list[i].length_var);
      width.push_back(ROT_Obj->rot_object_list[i].width);
      width_var.push_back(ROT_Obj->rot_object_list[i].width_var);
      time_since_created.push_back(ROT_Obj->rot_object_list[i].time_since_created);
      time_since_published.push_back(ROT_Obj->rot_object_list[i].time_since_published);
      time_since_stage_start1.push_back(ROT_Obj->rot_object_list[i].time_since_stage_start);
      existence_probability1.push_back(ROT_Obj->rot_object_list[i].existence_probability);
      mirror_prob.push_back(ROT_Obj->rot_object_list[i].mirror_prob);
      radar_cross_section.push_back(ROT_Obj->rot_object_list[i].radar_cross_section);
      otg_height.push_back(ROT_Obj->rot_object_list[i].otg_height);
      confidence_level.push_back(ROT_Obj->rot_object_list[i].confidence_level);
      probability_pedestrian1.push_back(ROT_Obj->rot_object_list[i].probability_pedestrian);
      probability_car1.push_back(ROT_Obj->rot_object_list[i].probability_car);
      probability_motorcycle1.push_back(ROT_Obj->rot_object_list[i].probability_motorcycle);
      probability_bicycle1.push_back(ROT_Obj->rot_object_list[i].probability_bicycle);
      probability_truck1.push_back(ROT_Obj->rot_object_list[i].probability_truck);
      probability_undet1.push_back(ROT_Obj->rot_object_list[i].probability_undet);
      probability_underdrivable_ocg.push_back(ROT_Obj->rot_object_list[i].probability_underdrivable_ocg);
      movable_prob.push_back(ROT_Obj->rot_object_list[i].movable_prob);
      id_ROT.push_back(ROT_Obj->rot_object_list[i].id);
      unique_id_ROT.push_back(ROT_Obj->rot_object_list[i].unique_id);
      ndets1.push_back(ROT_Obj->rot_object_list[i].ndets);
      num_dets_used_in_rr_msmt_update.push_back(ROT_Obj->rot_object_list[i].num_dets_used_in_rr_msmt_update);
      sensor_src.push_back(ROT_Obj->rot_object_list[i].sensor_src);
      reference_point.push_back(ROT_Obj->rot_object_list[i].reference_point);
      object_status.push_back(ROT_Obj->rot_object_list[i].object_status);
      object_class1.push_back(ROT_Obj->rot_object_list[i].object_class);
      movement_status.push_back(ROT_Obj->rot_object_list[i].movement_status);
      occlusion_status.push_back(ROT_Obj->rot_object_list[i].occlusion_status);
      underdrivable_status_ocg.push_back(ROT_Obj->rot_object_list[i].underdrivable_status_ocg);
      drivable_status_sg.push_back(ROT_Obj->rot_object_list[i].drivable_status_sg);
      drivable_confidence_sg.push_back(ROT_Obj->rot_object_list[i].drivable_confidence_sg);
      f_onguardrail1.push_back(ROT_Obj->rot_object_list[i].f_onguardrail);
      trk_fltr_type1.push_back(ROT_Obj->rot_object_list[i].trk_fltr_type);
   }
   hdf_ROT_info_ptr.vcs_x_posn.push_back(vcs_x_posn);
   hdf_ROT_info_ptr.vcs_y_posn.push_back(vcs_y_posn);
   hdf_ROT_info_ptr.vcs_x_vel.push_back(vcs_x_vel);
   hdf_ROT_info_ptr.vcs_y_vel.push_back(vcs_y_vel);
   hdf_ROT_info_ptr.vcs_x_acc.push_back(vcs_x_acc);
   hdf_ROT_info_ptr.vcs_y_acc.push_back(vcs_y_acc);
   hdf_ROT_info_ptr.vcs_heading.push_back(vcs_heading1);
   hdf_ROT_info_ptr.vcs_pointing.push_back(vcs_pointing1);
   hdf_ROT_info_ptr.vcs_curvature.push_back(vcs_curvature);
   hdf_ROT_info_ptr.vcs_state_variance.push_back(vcs_state_variance_2d);
   hdf_ROT_info_ptr.vcs_supplemental_state_covariance.push_back(vcs_supplemental_state_covariance_2d);

   // --- ISO kinematics ---
   hdf_ROT_info_ptr.iso_x_posn.push_back(iso_x_posn);
   hdf_ROT_info_ptr.iso_y_posn.push_back(iso_y_posn);
   hdf_ROT_info_ptr.iso_x_vel.push_back(iso_x_vel);
   hdf_ROT_info_ptr.iso_relative_x_vel.push_back(iso_relative_x_vel);
   hdf_ROT_info_ptr.iso_y_vel.push_back(iso_y_vel);
   hdf_ROT_info_ptr.iso_relative_y_vel.push_back(iso_relative_y_vel);
   hdf_ROT_info_ptr.iso_x_acc.push_back(iso_x_acc);
   hdf_ROT_info_ptr.iso_relative_x_acc.push_back(iso_relative_x_acc);
   hdf_ROT_info_ptr.iso_y_acc.push_back(iso_y_acc);
   hdf_ROT_info_ptr.iso_relative_y_acc.push_back(iso_relative_y_acc);

   hdf_ROT_info_ptr.iso_orientation.push_back(iso_orientation);
   hdf_ROT_info_ptr.iso_orientation_var.push_back(iso_orientation_var);
   hdf_ROT_info_ptr.iso_orientation_rate.push_back(iso_orientation_rate);
   hdf_ROT_info_ptr.iso_orientation_rate_var.push_back(iso_orientation_rate_var);

   hdf_ROT_info_ptr.iso_x_posn_var.push_back(iso_x_posn_var);
   hdf_ROT_info_ptr.iso_y_posn_var.push_back(iso_y_posn_var);
   hdf_ROT_info_ptr.iso_xy_posn_cov.push_back(iso_xy_posn_cov);
   hdf_ROT_info_ptr.iso_x_vel_var.push_back(iso_x_vel_var);
   hdf_ROT_info_ptr.iso_y_vel_var.push_back(iso_y_vel_var);
   hdf_ROT_info_ptr.iso_xy_vel_cov.push_back(iso_xy_vel_cov);
   hdf_ROT_info_ptr.iso_x_acc_var.push_back(iso_x_acc_var);
   hdf_ROT_info_ptr.iso_y_acc_var.push_back(iso_y_acc_var);
   hdf_ROT_info_ptr.iso_xy_acc_cov.push_back(iso_xy_acc_cov);

   // --- Shape and dynamics ---
   hdf_ROT_info_ptr.speed.push_back(speed);
   hdf_ROT_info_ptr.tang_accel.push_back(tang_accel1);
   hdf_ROT_info_ptr.length.push_back(length);
   hdf_ROT_info_ptr.length_var.push_back(length_var);
   hdf_ROT_info_ptr.width.push_back(width);
   hdf_ROT_info_ptr.width_var.push_back(width_var);
   hdf_ROT_info_ptr.time_since_created.push_back(time_since_created);
   hdf_ROT_info_ptr.time_since_published.push_back(time_since_published);
   hdf_ROT_info_ptr.time_since_stage_start.push_back(time_since_stage_start1);
   hdf_ROT_info_ptr.existence_probability.push_back(existence_probability1);
   hdf_ROT_info_ptr.mirror_prob.push_back(mirror_prob);
   hdf_ROT_info_ptr.radar_cross_section.push_back(radar_cross_section);
   hdf_ROT_info_ptr.otg_height.push_back(otg_height);
   hdf_ROT_info_ptr.confidence_level.push_back(confidence_level);

   // --- Probabilities ---
   hdf_ROT_info_ptr.probability_pedestrian.push_back(probability_pedestrian1);
   hdf_ROT_info_ptr.probability_car.push_back(probability_car1);
   hdf_ROT_info_ptr.probability_motorcycle.push_back(probability_motorcycle1);
   hdf_ROT_info_ptr.probability_bicycle.push_back(probability_bicycle1);
   hdf_ROT_info_ptr.probability_truck.push_back(probability_truck1);
   hdf_ROT_info_ptr.probability_undet.push_back(probability_undet1);
   hdf_ROT_info_ptr.probability_underdrivable_ocg.push_back(probability_underdrivable_ocg);
   hdf_ROT_info_ptr.movable_prob.push_back(movable_prob);

   // --- IDs & counters ---
   hdf_ROT_info_ptr.id.push_back(id_ROT);
   hdf_ROT_info_ptr.unique_id.push_back(unique_id_ROT);
   hdf_ROT_info_ptr.ndets.push_back(ndets1);
   hdf_ROT_info_ptr.num_dets_used_in_rr_msmt_update.push_back(num_dets_used_in_rr_msmt_update);
   hdf_ROT_info_ptr.sensor_src.push_back(sensor_src);

   // --- Status fields ---
   hdf_ROT_info_ptr.reference_point.push_back(reference_point);
   hdf_ROT_info_ptr.object_status.push_back(object_status);
   hdf_ROT_info_ptr.object_class.push_back(object_class1);
   hdf_ROT_info_ptr.movement_status.push_back(movement_status);
   hdf_ROT_info_ptr.occlusion_status.push_back(occlusion_status);
   hdf_ROT_info_ptr.underdrivable_status_ocg.push_back(underdrivable_status_ocg);
   hdf_ROT_info_ptr.drivable_status_sg.push_back(drivable_status_sg);
   hdf_ROT_info_ptr.drivable_confidence_sg.push_back(drivable_confidence_sg);
   hdf_ROT_info_ptr.f_onguardrail.push_back(f_onguardrail1);
   hdf_ROT_info_ptr.trk_fltr_type.push_back(trk_fltr_type1);

   if (olp_ptr == nullptr) {
      std::cerr << "Error: olp_ptr() returned nullptr" << std::endl;
      return;
   }

   std::vector<uint8_t> id;
   std::vector<uint32_t> unique_id_olp;
   std::vector<uint8_t> index;
   std::vector<std::string> olp_status;
   std::vector<uint8_t> age;
   std::vector<uint8_t> stage_age;
   std::vector<uint8_t> fbk_stage_age;
   std::vector<float32_t> olp_existence_probability;
   std::vector<float32_t> olp_speed;
   std::vector<float32_t> vcs_pos_x;
   std::vector<float32_t> vcs_pos_y;
   std::vector<float32_t> vcs_vel_x;
   std::vector<float32_t> vcs_vel_y;
   std::vector<float32_t> vcs_vel_rel_x;
   std::vector<float32_t> vcs_vel_rel_y;
   std::vector<float32_t> vcs_accel_x;
   std::vector<float32_t> vcs_accel_y;
   std::vector<float32_t> olp_vcs_heading;
   std::vector<float32_t> heading_rate;
   std::vector<float32_t> heading_variance;
   std::vector<float32_t> accuracy_heading;
   std::vector<float32_t> eclipse_value;
   std::vector<float32_t> olp_length;
   std::vector<float32_t> olp_width;
   std::vector<float32_t> obj_distance;
   std::vector<float32_t> obstruction_prob;
   std::vector<uint8_t> obj_class;
   std::vector<float32_t> class_prob_pedestrian;
   std::vector<float32_t> class_prob_2wheel;
   std::vector<float32_t> class_prob_car;
   std::vector<float32_t> class_prob_truck;
   std::vector<uint8_t> id_merged_obj;
   std::vector<uint8_t> f_merge_occured;
   std::vector<uint8_t> curvi_coordinates_calc_method;
   std::vector<float32_t> curvi_pos_x;
   std::vector<float32_t> curvi_pos_y;
   std::vector<float32_t> curvi_vel_x;
   std::vector<float32_t> curvi_vel_y;
   std::vector<float32_t> curvi_vel_rel_x;
   std::vector<float32_t> curvi_vel_rel_y;
   std::vector<float32_t> curvi_heading;
   std::vector<uint8_t> f_reflection;
   std::vector<uint8_t> f_stationary;
   std::vector<uint8_t> olp_f_moveable;
   std::vector<uint8_t> f_stationary_clutter;
   std::vector<uint8_t> f_is_fl_origin_sensor;
   std::vector<uint8_t> f_is_fr_origin_sensor;
   std::vector<uint8_t> f_is_rl_origin_sensor;
   std::vector<uint8_t> f_is_rr_origin_sensor;
   std::vector<uint8_t> f_is_in_fl_sensor_fov;
   std::vector<uint8_t> f_is_in_fr_sensor_fov;
   std::vector<uint8_t> f_is_in_rl_sensor_fov;
   std::vector<uint8_t> f_is_in_rr_sensor_fov;

   auto obj_status_str = [](uint8_t status) -> std::string {
      if (status == 0)
         return "OBJ_STATUS_INVALID";
      else if (status == 1)
         return "OBJ_STATUS_NEW";
      else if (status == 2)
         return "OBJ_STATUS_MATURE";
      else if (status == 3)
         return "OBJ_STATUS_COASTED";
      else if (status == 4)
         return "OBJ_STATUS_COASTED_IMPLAUSIBLE";
      else
         return "OBJ_STATUS_INVALID";
   };

   auto obj_class_str = [](uint8_t obj_class) -> std::string {
      if (obj_class == 0)
         return "OBJ_CLASS_UNKNOWN";
      else if (obj_class == 1)
         return "OBJ_CLASS_PEDESTRIAN";
      else if (obj_class == 2)
         return "OBJ_CLASS_2WHEEL";
      else if (obj_class == 3)
         return "OBJ_CLASS_CAR";
      else if (obj_class == 4)
         return "OBJ_CLASS_TRUCK";
      else
         return "OBJ_CLASS_UNKNOWN";
   };

   for (int i = 0; i < SFL_OBJ_NUMBER_OF_OBJECTS; i++) {
      id.push_back(olp_ptr->obj[i].id);
      unique_id_olp.push_back(olp_ptr->obj[i].unique_id);
      index.push_back(olp_ptr->obj[i].index);
      olp_status.push_back(obj_status_str(olp_ptr->obj[i].status));
      age.push_back(olp_ptr->obj[i].age);
      stage_age.push_back(olp_ptr->obj[i].stage_age);
      fbk_stage_age.push_back(olp_ptr->obj[i].fbk_stage_age);
      olp_existence_probability.push_back(olp_ptr->obj[i].existence_probability);
      olp_speed.push_back(olp_ptr->obj[i].speed);
      vcs_pos_x.push_back(olp_ptr->obj[i].vcs_pos.x);
      vcs_pos_y.push_back(olp_ptr->obj[i].vcs_pos.y);
      vcs_vel_x.push_back(olp_ptr->obj[i].vcs_vel.x);
      vcs_vel_y.push_back(olp_ptr->obj[i].vcs_vel.y);
      vcs_vel_rel_x.push_back(olp_ptr->obj[i].vcs_vel_rel.x);
      vcs_vel_rel_y.push_back(olp_ptr->obj[i].vcs_vel_rel.y);
      vcs_accel_x.push_back(olp_ptr->obj[i].vcs_accel.x);
      vcs_accel_y.push_back(olp_ptr->obj[i].vcs_accel.y);
      olp_vcs_heading.push_back(olp_ptr->obj[i].vcs_heading);
      heading_rate.push_back(olp_ptr->obj[i].heading_rate);
      heading_variance.push_back(olp_ptr->obj[i].heading_variance);
      accuracy_heading.push_back(olp_ptr->obj[i].accuracy_heading);
      eclipse_value.push_back(olp_ptr->obj[i].eclipse_value);
      olp_length.push_back(olp_ptr->obj[i].length);
      olp_width.push_back(olp_ptr->obj[i].width);
      obj_distance.push_back(olp_ptr->obj[i].obj_distance);
      obstruction_prob.push_back(olp_ptr->obj[i].obstruction_prob);
      obj_class.push_back(olp_ptr->obj[i].obj_class);
      class_prob_pedestrian.push_back(olp_ptr->obj[i].class_prob_pedestrian);
      class_prob_2wheel.push_back(olp_ptr->obj[i].class_prob_2wheel);
      class_prob_car.push_back(olp_ptr->obj[i].class_prob_car);
      class_prob_truck.push_back(olp_ptr->obj[i].class_prob_truck);
      id_merged_obj.push_back(olp_ptr->obj[i].id_merged_obj);
      f_merge_occured.push_back(olp_ptr->obj[i].f_merge_occured);
      curvi_coordinates_calc_method.push_back(olp_ptr->obj[i].curvi_coordinates_calc_method);
      curvi_pos_x.push_back(olp_ptr->obj[i].curvi_pos.x);
      curvi_pos_y.push_back(olp_ptr->obj[i].curvi_pos.y);
      curvi_vel_x.push_back(olp_ptr->obj[i].curvi_vel.x);
      curvi_vel_y.push_back(olp_ptr->obj[i].curvi_vel.y);
      curvi_vel_rel_x.push_back(olp_ptr->obj[i].curvi_vel_rel.x);
      curvi_vel_rel_y.push_back(olp_ptr->obj[i].curvi_vel_rel.y);
      curvi_heading.push_back(olp_ptr->obj[i].curvi_heading);
      f_reflection.push_back(olp_ptr->obj[i].f_reflection);
      f_stationary.push_back(olp_ptr->obj[i].f_stationary);
      olp_f_moveable.push_back(olp_ptr->obj[i].f_moveable);
      f_stationary_clutter.push_back(olp_ptr->obj[i].f_stationary_clutter);
      f_is_fl_origin_sensor.push_back(olp_ptr->obj[i].f_is_fl_origin_sensor);
      f_is_fr_origin_sensor.push_back(olp_ptr->obj[i].f_is_fr_origin_sensor);
      f_is_rl_origin_sensor.push_back(olp_ptr->obj[i].f_is_rl_origin_sensor);
      f_is_rr_origin_sensor.push_back(olp_ptr->obj[i].f_is_rr_origin_sensor);
      f_is_in_fl_sensor_fov.push_back(olp_ptr->obj[i].f_is_in_fl_sensor_fov);
      f_is_in_fr_sensor_fov.push_back(olp_ptr->obj[i].f_is_in_fr_sensor_fov);
      f_is_in_rl_sensor_fov.push_back(olp_ptr->obj[i].f_is_in_rl_sensor_fov);
      f_is_in_rr_sensor_fov.push_back(olp_ptr->obj[i].f_is_in_rr_sensor_fov);
   }

   hdf_olp_output_ptr.n_valid_objects.push_back(olp_ptr->n_valid_objects);
   hdf_olp_output_ptr.id.push_back(id);
   hdf_olp_output_ptr.unique_id.push_back(unique_id_olp);
   hdf_olp_output_ptr.index.push_back(index);
   hdf_olp_output_ptr.status.push_back(olp_status);
   hdf_olp_output_ptr.age.push_back(age);
   hdf_olp_output_ptr.stage_age.push_back(stage_age);
   hdf_olp_output_ptr.fbk_stage_age.push_back(fbk_stage_age);
   hdf_olp_output_ptr.existence_probability.push_back(olp_existence_probability);
   hdf_olp_output_ptr.speed.push_back(olp_speed);
   hdf_olp_output_ptr.vcs_pos_x.push_back(vcs_pos_x);
   hdf_olp_output_ptr.vcs_pos_y.push_back(vcs_pos_y);
   hdf_olp_output_ptr.vcs_vel_x.push_back(vcs_vel_x);
   hdf_olp_output_ptr.vcs_vel_y.push_back(vcs_vel_y);
   hdf_olp_output_ptr.vcs_vel_rel_x.push_back(vcs_vel_rel_x);
   hdf_olp_output_ptr.vcs_vel_rel_y.push_back(vcs_vel_rel_y);
   hdf_olp_output_ptr.vcs_accel_x.push_back(vcs_accel_x);
   hdf_olp_output_ptr.vcs_accel_y.push_back(vcs_accel_y);
   hdf_olp_output_ptr.vcs_heading.push_back(olp_vcs_heading);
   hdf_olp_output_ptr.heading_rate.push_back(heading_rate);
   hdf_olp_output_ptr.heading_variance.push_back(heading_variance);
   hdf_olp_output_ptr.accuracy_heading.push_back(accuracy_heading);
   hdf_olp_output_ptr.eclipse_value.push_back(eclipse_value);
   hdf_olp_output_ptr.length.push_back(olp_length);
   hdf_olp_output_ptr.width.push_back(olp_width);
   hdf_olp_output_ptr.obj_distance.push_back(obj_distance);
   hdf_olp_output_ptr.obstruction_prob.push_back(obstruction_prob);
   hdf_olp_output_ptr.obj_class.push_back(obj_class);
   hdf_olp_output_ptr.class_prob_pedestrian.push_back(class_prob_pedestrian);
   hdf_olp_output_ptr.class_prob_2wheel.push_back(class_prob_2wheel);
   hdf_olp_output_ptr.class_prob_car.push_back(class_prob_car);
   hdf_olp_output_ptr.class_prob_truck.push_back(class_prob_truck);
   hdf_olp_output_ptr.id_merged_obj.push_back(id_merged_obj);
   hdf_olp_output_ptr.f_merge_occured.push_back(f_merge_occured);
   hdf_olp_output_ptr.curvi_coordinates_calc_method.push_back(curvi_coordinates_calc_method);
   hdf_olp_output_ptr.curvi_pos_x.push_back(curvi_pos_x);
   hdf_olp_output_ptr.curvi_pos_y.push_back(curvi_pos_y);
   hdf_olp_output_ptr.curvi_vel_x.push_back(curvi_vel_x);
   hdf_olp_output_ptr.curvi_vel_y.push_back(curvi_vel_y);
   hdf_olp_output_ptr.curvi_vel_rel_x.push_back(curvi_vel_rel_x);
   hdf_olp_output_ptr.curvi_vel_rel_y.push_back(curvi_vel_rel_y);
   hdf_olp_output_ptr.curvi_heading.push_back(curvi_heading);
   hdf_olp_output_ptr.f_reflection.push_back(f_reflection);
   hdf_olp_output_ptr.f_stationary.push_back(f_stationary);
   hdf_olp_output_ptr.f_moveable.push_back(olp_f_moveable);
   hdf_olp_output_ptr.f_stationary_clutter.push_back(f_stationary_clutter);
   hdf_olp_output_ptr.f_is_fl_origin_sensor.push_back(f_is_fl_origin_sensor);
   hdf_olp_output_ptr.f_is_fr_origin_sensor.push_back(f_is_fr_origin_sensor);
   hdf_olp_output_ptr.f_is_rl_origin_sensor.push_back(f_is_rl_origin_sensor);
   hdf_olp_output_ptr.f_is_rr_origin_sensor.push_back(f_is_rr_origin_sensor);
   hdf_olp_output_ptr.f_is_in_fl_sensor_fov.push_back(f_is_in_fl_sensor_fov);
   hdf_olp_output_ptr.f_is_in_fr_sensor_fov.push_back(f_is_in_fr_sensor_fov);
   hdf_olp_output_ptr.f_is_in_rl_sensor_fov.push_back(f_is_in_rl_sensor_fov);
   hdf_olp_output_ptr.f_is_in_rr_sensor_fov.push_back(f_is_in_rr_sensor_fov);

   if (lcda_ptr == nullptr) {
      std::cerr << "Error: Lcda_Get_Output_Ptr() returned nullptr" << std::endl;
      return;
   }
   hdf_lcda_ptr.lcda_status.push_back(lcda_ptr->lcda_status);
   hdf_lcda_ptr.f_bsw_enabled.push_back(lcda_ptr->f_bsw_enabled);
   hdf_lcda_ptr.bsw_alert_left.push_back(lcda_ptr->bsw_alert[0]);
   hdf_lcda_ptr.bsw_alert_right.push_back(lcda_ptr->bsw_alert[1]);
   hdf_lcda_ptr.bsw_id_left.push_back(lcda_ptr->bsw_id[0]);
   hdf_lcda_ptr.bsw_id_right.push_back(lcda_ptr->bsw_id[1]);
   hdf_lcda_ptr.bsw_unique_id_left.push_back(lcda_ptr->bsw_unique_id[0]);
   hdf_lcda_ptr.bsw_unique_id_right.push_back(lcda_ptr->bsw_unique_id[1]);

   hdf_lcda_ptr.f_cvw_enabled.push_back(lcda_ptr->f_cvw_enabled);
   hdf_lcda_ptr.cvw_alert_left.push_back(lcda_ptr->cvw_alert[0]);
   hdf_lcda_ptr.cvw_alert_right.push_back(lcda_ptr->cvw_alert[1]);
   hdf_lcda_ptr.cvw_id_left.push_back(lcda_ptr->cvw_id[0]);
   hdf_lcda_ptr.cvw_id_right.push_back(lcda_ptr->cvw_id[1]);
   hdf_lcda_ptr.cvw_unique_id_left.push_back(lcda_ptr->cvw_unique_id[0]);
   hdf_lcda_ptr.cvw_unique_id_right.push_back(lcda_ptr->cvw_unique_id[1]);
   hdf_lcda_ptr.cvw_ttc_s_left.push_back(lcda_ptr->cvw_ttc_s[0]);
   hdf_lcda_ptr.cvw_ttc_s_right.push_back(lcda_ptr->cvw_ttc_s[1]);

   hdf_lcda_ptr.f_slc_enabled.push_back(lcda_ptr->f_slc_enabled);
   hdf_lcda_ptr.slc_alert_left.push_back(lcda_ptr->slc_alert[0]);
   hdf_lcda_ptr.slc_alert_right.push_back(lcda_ptr->slc_alert[1]);
   hdf_lcda_ptr.slc_id_left.push_back(lcda_ptr->cvw_id[0]);
   hdf_lcda_ptr.slc_id_right.push_back(lcda_ptr->slc_id[1]);
   hdf_lcda_ptr.slc_unique_id_left.push_back(lcda_ptr->slc_unique_id[0]);
   hdf_lcda_ptr.slc_unique_id_right.push_back(lcda_ptr->slc_unique_id[1]);
   hdf_lcda_ptr.slc_ttc_s_left.push_back(lcda_ptr->slc_ttc_s[0]);
   hdf_lcda_ptr.slc_ttc_s_right.push_back(lcda_ptr->slc_ttc_s[1]);

   hdf_lcda_ptr.slc_lane_change_probability_left.push_back(lcda_ptr->slc_lane_change_probability[0]);
   hdf_lcda_ptr.slc_lane_change_probability_right.push_back(lcda_ptr->slc_lane_change_probability[1]);

   // Verify if ced_ptr data populated
   if (ced_ptr == nullptr) {
      std::cerr << "Error: Ced_Get_Output_Ptr() returned nullptr" << std::endl;
      return;
   }
   hdf_ced_ptr.f_ced_enable.push_back(ced_ptr->f_ced_enable);

   // CED alerts for left and right sides
   hdf_ced_ptr.ced_alert_left.push_back(ced_ptr->ced_alert[0]);
   hdf_ced_ptr.ced_alert_right.push_back(ced_ptr->ced_alert[1]);

   // Left side Ced_Critical_Object_T  data
   hdf_ced_ptr.ced_object_ced_id_left.push_back(ced_ptr->ced_object[0].id);
   hdf_ced_ptr.ced_object_unique_id_left.push_back(ced_ptr->ced_object[0].unique_id);
   hdf_ced_ptr.ced_object_type_left.push_back(ced_ptr->ced_object[0].type);
   hdf_ced_ptr.ced_object_length_m_left.push_back(ced_ptr->ced_object[0].length_m);
   hdf_ced_ptr.ced_object_width_m_left.push_back(ced_ptr->ced_object[0].width_m);
   hdf_ced_ptr.ced_object_lat_pos_m_left.push_back(ced_ptr->ced_object[0].lat_pos_m);
   hdf_ced_ptr.ced_object_long_pos_m_left.push_back(ced_ptr->ced_object[0].long_pos_m);
   hdf_ced_ptr.ced_object_speed_mps_left.push_back(ced_ptr->ced_object[0].speed_mps);
   hdf_ced_ptr.ced_object_heading_rad_left.push_back(ced_ptr->ced_object[0].heading_rad);
   hdf_ced_ptr.ced_object_direction_left.push_back(ced_ptr->ced_object[0].direction);
   hdf_ced_ptr.ced_object_predicted_lat_pos_m_left.push_back(ced_ptr->ced_object[0].predicted_lat_pos_m);
   hdf_ced_ptr.ced_object_ttc_s_left.push_back(ced_ptr->ced_object[0].ttc_s);
   hdf_ced_ptr.ced_object_ttp_s_left.push_back(ced_ptr->ced_object[0].ttp_s);

   // Right side Ced_Critical_Object_T data
   hdf_ced_ptr.ced_object_ced_id_right.push_back(ced_ptr->ced_object[1].id);
   hdf_ced_ptr.ced_object_unique_id_right.push_back(ced_ptr->ced_object[1].unique_id);
   hdf_ced_ptr.ced_object_type_right.push_back(ced_ptr->ced_object[1].type);
   hdf_ced_ptr.ced_object_length_m_right.push_back(ced_ptr->ced_object[1].length_m);
   hdf_ced_ptr.ced_object_width_m_right.push_back(ced_ptr->ced_object[1].width_m);
   hdf_ced_ptr.ced_object_lat_pos_m_right.push_back(ced_ptr->ced_object[1].lat_pos_m);
   hdf_ced_ptr.ced_object_long_pos_m_right.push_back(ced_ptr->ced_object[1].long_pos_m);
   hdf_ced_ptr.ced_object_speed_mps_right.push_back(ced_ptr->ced_object[1].speed_mps);
   hdf_ced_ptr.ced_object_heading_rad_right.push_back(ced_ptr->ced_object[1].heading_rad);
   hdf_ced_ptr.ced_object_direction_right.push_back(ced_ptr->ced_object[1].direction);
   hdf_ced_ptr.ced_object_predicted_lat_pos_m_right.push_back(ced_ptr->ced_object[1].predicted_lat_pos_m);
   hdf_ced_ptr.ced_object_ttc_s_right.push_back(ced_ptr->ced_object[1].ttc_s);
   hdf_ced_ptr.ced_object_ttp_s_right.push_back(ced_ptr->ced_object[1].ttp_s);

   // Verify if esa_ptr data populated
   if (esa_ptr == nullptr) {
      std::cerr << "Error: Esa_Get_Output_Ptr() returned nullptr" << std::endl;
      return;
   }

   // ESA status
   hdf_esa_ptr.esa_status.push_back(esa_ptr->esa_status);

   // ESA alerts for left and right sides
   hdf_esa_ptr.f_esa_alert_left.push_back(esa_ptr->f_esa_alert[0]);
   hdf_esa_ptr.f_esa_alert_right.push_back(esa_ptr->f_esa_alert[1]);

   // ESA Critical Object for left side
   hdf_esa_ptr.esa_object_id_left.push_back(esa_ptr->esa_object[0].id);
   hdf_esa_ptr.esa_object_index_left.push_back(esa_ptr->esa_object[0].index);
   hdf_esa_ptr.esa_object_width_m_left.push_back(esa_ptr->esa_object[0].width_m);
   hdf_esa_ptr.esa_object_length_m_left.push_back(esa_ptr->esa_object[0].length_m);
   hdf_esa_ptr.esa_object_long_pos_m_left.push_back(esa_ptr->esa_object[0].long_pos_m);
   hdf_esa_ptr.esa_object_lat_pos_m_left.push_back(esa_ptr->esa_object[0].lat_pos_m);
   hdf_esa_ptr.esa_object_long_speed_mps_left.push_back(esa_ptr->esa_object[0].long_speed_mps);
   hdf_esa_ptr.esa_object_lat_speed_mps_left.push_back(esa_ptr->esa_object[0].lat_speed_mps);
   hdf_esa_ptr.esa_object_ttc_s_left.push_back(esa_ptr->esa_object[0].ttc_s);
   hdf_esa_ptr.esa_object_ttp_s_left.push_back(esa_ptr->esa_object[0].ttp_s);
   hdf_esa_ptr.esa_object_decel_to_reach_host_speed_mps2_left.push_back(esa_ptr->esa_object[0].decel_to_reach_host_speed_mps2);
   hdf_esa_ptr.esa_object_long_distance_m_left.push_back(esa_ptr->esa_object[0].long_distance_m);
   hdf_esa_ptr.esa_object_existence_prob_left.push_back(esa_ptr->esa_object[0].existence_prob);

   // ESA Critical Object for right side
   hdf_esa_ptr.esa_object_id_right.push_back(esa_ptr->esa_object[1].id);
   hdf_esa_ptr.esa_object_index_right.push_back(esa_ptr->esa_object[1].index);
   hdf_esa_ptr.esa_object_width_m_right.push_back(esa_ptr->esa_object[1].width_m);
   hdf_esa_ptr.esa_object_length_m_right.push_back(esa_ptr->esa_object[1].length_m);
   hdf_esa_ptr.esa_object_long_pos_m_right.push_back(esa_ptr->esa_object[1].long_pos_m);
   hdf_esa_ptr.esa_object_lat_pos_m_right.push_back(esa_ptr->esa_object[1].lat_pos_m);
   hdf_esa_ptr.esa_object_long_speed_mps_right.push_back(esa_ptr->esa_object[1].long_speed_mps);
   hdf_esa_ptr.esa_object_lat_speed_mps_right.push_back(esa_ptr->esa_object[1].lat_speed_mps);
   hdf_esa_ptr.esa_object_ttc_s_right.push_back(esa_ptr->esa_object[1].ttc_s);
   hdf_esa_ptr.esa_object_ttp_s_right.push_back(esa_ptr->esa_object[1].ttp_s);
   hdf_esa_ptr.esa_object_decel_to_reach_host_speed_mps2_right.push_back(esa_ptr->esa_object[1].decel_to_reach_host_speed_mps2);
   hdf_esa_ptr.esa_object_long_distance_m_right.push_back(esa_ptr->esa_object[1].long_distance_m);
   hdf_esa_ptr.esa_object_existence_prob_right.push_back(esa_ptr->esa_object[1].existence_prob);

   // Verify if ltb_ptr populated
   if (ltb_ptr == nullptr) {
      std::cerr << "Error: Ltb_Get_Output_Ptr() returned nullptr" << std::endl;
      return;
   }

   // Populate left side object data
   hdf_ltb_ptr.ltb_object_ltb_id_left.push_back(ltb_ptr->ltb_object[0].ltb_id);
   hdf_ltb_ptr.ltb_object_ltb_ttc_s_left.push_back(ltb_ptr->ltb_object[0].ltb_ttc_s);
   hdf_ltb_ptr.ltb_object_ltb_ttb_s_left.push_back(ltb_ptr->ltb_object[0].ltb_ttb_s);
   hdf_ltb_ptr.ltb_object_ltb_decel_estimate_mps2_left.push_back(ltb_ptr->ltb_object[0].ltb_decel_estimate_mps2);
   hdf_ltb_ptr.ltb_object_ltb_distance_m_left.push_back(ltb_ptr->ltb_object[0].ltb_distance_m);

   // Populate right side object data
   hdf_ltb_ptr.ltb_object_ltb_id_right.push_back(ltb_ptr->ltb_object[1].ltb_id);
   hdf_ltb_ptr.ltb_object_ltb_ttc_s_right.push_back(ltb_ptr->ltb_object[1].ltb_ttc_s);
   hdf_ltb_ptr.ltb_object_ltb_ttb_s_right.push_back(ltb_ptr->ltb_object[1].ltb_ttb_s);
   hdf_ltb_ptr.ltb_object_ltb_decel_estimate_mps2_right.push_back(ltb_ptr->ltb_object[1].ltb_decel_estimate_mps2);
   hdf_ltb_ptr.ltb_object_ltb_distance_m_right.push_back(ltb_ptr->ltb_object[1].ltb_distance_m);

   // Populate alert levels and critical side
   hdf_ltb_ptr.ltb_alert_level_left.push_back(ltb_ptr->ltb_alert_level[0]);
   hdf_ltb_ptr.ltb_alert_level_right.push_back(ltb_ptr->ltb_alert_level[1]);
   hdf_ltb_ptr.ltb_most_critical_side.push_back(ltb_ptr->ltb_most_critical_side);

   // Verify if recw_ptr populated
   if (recw_ptr == nullptr) {
      std::cerr << "Error: Recw_Get_Output_Ptr() returned nullptr" << std::endl;
      return;
   }

   hdf_recw_ptr.recw_crash_probability.push_back(recw_ptr->recw_crash_probability);
   hdf_recw_ptr.recw_ttc_s.push_back(recw_ptr->recw_ttc_s);
   hdf_recw_ptr.recw_id.push_back(recw_ptr->recw_id);
   hdf_recw_ptr.recw_unique_id.push_back(recw_ptr->recw_unique_id);
   hdf_recw_ptr.recw_alert_level.push_back(recw_ptr->recw_alert_level);
   hdf_recw_ptr.ttc_threshold_alert_level_1_s.push_back(recw_ptr->ttc_threshold_alert_level_1_s);
   hdf_recw_ptr.ttc_threshold_alert_level_2_s.push_back(recw_ptr->ttc_threshold_alert_level_2_s);

   // Verify if ta_ptr populated
   if (ta_ptr == nullptr) {
      std::cerr << "Error: Ta_Get_Output_Ptr() returned nullptr" << std::endl;
      return;
   }

   hdf_ta_ptr.f_ta_enable.push_back(ta_ptr->f_ta_enable);
   hdf_ta_ptr.ta_f_vehicle_state_relevant.push_back(ta_ptr->ta_f_vehicle_state_relevant);
   hdf_ta_ptr.ta_most_critical_side.push_back(ta_ptr->ta_most_critical_side);
   hdf_ta_ptr.ta_n_valid_objects.push_back(ta_ptr->ta_n_valid_objects);
   hdf_ta_ptr.ta_n_relevant_objects.push_back(ta_ptr->ta_n_relevant_objects);
   hdf_ta_ptr.ta_n_critical_objects.push_back(ta_ptr->ta_n_critical_objects);
   hdf_ta_ptr.ta_algorithm_state.push_back(ta_ptr->ta_algorithm_state);

   hdf_ta_ptr.ta_alert_level_left.push_back(ta_ptr->ta_alert_level[0]);
   hdf_ta_ptr.ta_alert_level_right.push_back(ta_ptr->ta_alert_level[1]);

   // Populate left side critical object data
   hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_x_left.push_back(ta_ptr->ta_object[0].ta_waypoint_at_collision_m.x);
   hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_y_left.push_back(ta_ptr->ta_object[0].ta_waypoint_at_collision_m.y);
   hdf_ta_ptr.ta_object_ta_ttc_s_left.push_back(ta_ptr->ta_object[0].ta_ttc_s);
   hdf_ta_ptr.ta_object_ta_ttp_s_left.push_back(ta_ptr->ta_object[0].ta_ttp_s);
   hdf_ta_ptr.ta_object_ta_ttb_s_left.push_back(ta_ptr->ta_object[0].ta_ttb_s);
   hdf_ta_ptr.ta_object_ta_decel_estimate_mps2_left.push_back(ta_ptr->ta_object[0].ta_decel_estimate_mps2);
   hdf_ta_ptr.ta_object_ta_distance_m_left.push_back(ta_ptr->ta_object[0].ta_distance_m);
   hdf_ta_ptr.ta_object_ta_id_left.push_back(ta_ptr->ta_object[0].ta_id);
   hdf_ta_ptr.ta_object_ta_index_left.push_back(ta_ptr->ta_object[0].ta_index);
   hdf_ta_ptr.ta_object_ta_f_obj_in_danger_zone_left.push_back(ta_ptr->ta_object[0].ta_f_obj_in_danger_zone);
   hdf_ta_ptr.ta_object_ta_f_obj_in_info_zone_left.push_back(ta_ptr->ta_object[0].ta_f_obj_in_info_zone);
   hdf_ta_ptr.ta_object_ta_f_obj_in_wing_zone_left.push_back(ta_ptr->ta_object[0].ta_f_obj_in_wing_zone);

   // Populate right side critical object data
   hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_x_right.push_back(ta_ptr->ta_object[1].ta_waypoint_at_collision_m.x);
   hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_y_right.push_back(ta_ptr->ta_object[1].ta_waypoint_at_collision_m.y);
   hdf_ta_ptr.ta_object_ta_ttc_s_right.push_back(ta_ptr->ta_object[1].ta_ttc_s);
   hdf_ta_ptr.ta_object_ta_ttp_s_right.push_back(ta_ptr->ta_object[1].ta_ttp_s);
   hdf_ta_ptr.ta_object_ta_ttb_s_right.push_back(ta_ptr->ta_object[1].ta_ttb_s);
   hdf_ta_ptr.ta_object_ta_decel_estimate_mps2_right.push_back(ta_ptr->ta_object[1].ta_decel_estimate_mps2);
   hdf_ta_ptr.ta_object_ta_distance_m_right.push_back(ta_ptr->ta_object[1].ta_distance_m);
   hdf_ta_ptr.ta_object_ta_id_right.push_back(ta_ptr->ta_object[1].ta_id);
   hdf_ta_ptr.ta_object_ta_index_right.push_back(ta_ptr->ta_object[1].ta_index);
   hdf_ta_ptr.ta_object_ta_f_obj_in_danger_zone_right.push_back(ta_ptr->ta_object[1].ta_f_obj_in_danger_zone);
   hdf_ta_ptr.ta_object_ta_f_obj_in_info_zone_right.push_back(ta_ptr->ta_object[1].ta_f_obj_in_info_zone);
   hdf_ta_ptr.ta_object_ta_f_obj_in_wing_zone_right.push_back(ta_ptr->ta_object[1].ta_f_obj_in_wing_zone);

   // Verify if scw_ptr populated
   if (scw_ptr == nullptr) {
      std::cerr << "Error: Scw_Get_Output_Ptr() returned nullptr" << std::endl;
      return;
   }

   hdf_scw_ptr.f_scw_enabled.push_back(scw_ptr->f_scw_enabled);
   hdf_scw_ptr.f_scw_dyn_enabled.push_back(scw_ptr->f_scw_dyn_enabled);
   hdf_scw_ptr.f_scw_guardrail_enabled.push_back(scw_ptr->f_scw_guardrail_enabled);

   // Populate left side object data
   hdf_scw_ptr.scw_object_alert_level_left.push_back(scw_ptr->scw_object[0].alert_level);
   hdf_scw_ptr.scw_object_id_left.push_back(scw_ptr->scw_object[0].id);
   hdf_scw_ptr.scw_object_unique_id_left.push_back(scw_ptr->scw_object[0].unique_id);
   hdf_scw_ptr.scw_object_type_left.push_back(scw_ptr->scw_object[0].type);
   hdf_scw_ptr.scw_object_lateral_ttc_s_left.push_back(scw_ptr->scw_object[0].lateral_ttc_s);
   hdf_scw_ptr.scw_object_lateral_distance_m_left.push_back(scw_ptr->scw_object[0].lateral_distance_m);
   hdf_scw_ptr.scw_object_position_m_x_left.push_back(scw_ptr->scw_object[0].position_m.x);
   hdf_scw_ptr.scw_object_position_m_y_left.push_back(scw_ptr->scw_object[0].position_m.y);
   hdf_scw_ptr.scw_object_velocity_mps_x_left.push_back(scw_ptr->scw_object[0].velocity_mps.x);
   hdf_scw_ptr.scw_object_velocity_mps_y_left.push_back(scw_ptr->scw_object[0].velocity_mps.y);
   hdf_scw_ptr.scw_object_acceleration_mps2_x_left.push_back(scw_ptr->scw_object[0].acceleration_mps2.x);
   hdf_scw_ptr.scw_object_acceleration_mps2_y_left.push_back(scw_ptr->scw_object[0].acceleration_mps2.y);
   hdf_scw_ptr.scw_object_width_m_left.push_back(scw_ptr->scw_object[0].width_m);
   hdf_scw_ptr.scw_object_length_m_left.push_back(scw_ptr->scw_object[0].length_m);
   hdf_scw_ptr.scw_object_heading_rad_left.push_back(scw_ptr->scw_object[0].heading_rad);
   hdf_scw_ptr.scw_object_existence_probability_left.push_back(scw_ptr->scw_object[0].existence_probability);
   hdf_scw_ptr.scw_object_age_left.push_back(scw_ptr->scw_object[0].age);

   // Populate right side object data
   hdf_scw_ptr.scw_object_alert_level_right.push_back(scw_ptr->scw_object[1].alert_level);
   hdf_scw_ptr.scw_object_id_right.push_back(scw_ptr->scw_object[1].id);
   hdf_scw_ptr.scw_object_unique_id_right.push_back(scw_ptr->scw_object[1].unique_id);
   hdf_scw_ptr.scw_object_type_right.push_back(scw_ptr->scw_object[1].type);
   hdf_scw_ptr.scw_object_lateral_ttc_s_right.push_back(scw_ptr->scw_object[1].lateral_ttc_s);
   hdf_scw_ptr.scw_object_lateral_distance_m_right.push_back(scw_ptr->scw_object[1].lateral_distance_m);
   hdf_scw_ptr.scw_object_position_m_x_right.push_back(scw_ptr->scw_object[1].position_m.x);
   hdf_scw_ptr.scw_object_position_m_y_right.push_back(scw_ptr->scw_object[1].position_m.y);
   hdf_scw_ptr.scw_object_velocity_mps_x_right.push_back(scw_ptr->scw_object[1].velocity_mps.x);
   hdf_scw_ptr.scw_object_velocity_mps_y_right.push_back(scw_ptr->scw_object[1].velocity_mps.y);
   hdf_scw_ptr.scw_object_acceleration_mps2_x_right.push_back(scw_ptr->scw_object[1].acceleration_mps2.x);
   hdf_scw_ptr.scw_object_acceleration_mps2_y_right.push_back(scw_ptr->scw_object[1].acceleration_mps2.y);
   hdf_scw_ptr.scw_object_width_m_right.push_back(scw_ptr->scw_object[1].width_m);
   hdf_scw_ptr.scw_object_length_m_right.push_back(scw_ptr->scw_object[1].length_m);
   hdf_scw_ptr.scw_object_heading_rad_right.push_back(scw_ptr->scw_object[1].heading_rad);
   hdf_scw_ptr.scw_object_existence_probability_right.push_back(scw_ptr->scw_object[1].existence_probability);
   hdf_scw_ptr.scw_object_age_right.push_back(scw_ptr->scw_object[1].age);

   // Verify if pt_ptr populated
   if (pt_ptr == nullptr) {
      std::cerr << "Error: Pt_Get_Output_Ptr() returned nullptr" << std::endl;
      return;
   }
   std::vector<float32_T> range_vcs_proj_to_path_segment;
   std::vector<float32_T> segment_heading_diff;
   std::vector<uint8_t> track_idx_nearest_path;
   std::vector<float32_T> range_at_zero;
   std::vector<float32_T> range_to_current_path_part;
   std::vector<float32_T> range_at_host_edge;
   std::vector<float32_T> length_of_trajectory;
   std::vector<float32_T> path_heading;
   std::vector<int> path_direction;
   std::vector<uint8_t> track_match;
   std::vector<uint8_t> track_match_age;
   std::vector<uint8_t> track_match_last_cycle;
   for (int i = 0; i < PA_OBJ_NUMBER_OF_OBJECTS; i++) {
      range_vcs_proj_to_path_segment.push_back(pt_ptr->nearest_path_output[i].range_vcs_proj_to_path_segment);
      segment_heading_diff.push_back(pt_ptr->nearest_path_output[i].segment_heading_diff);
      track_idx_nearest_path.push_back(pt_ptr->nearest_path_output[i].track_idx_nearest_path);
      range_at_zero.push_back(pt_ptr->path_obj_pair_output[i].range_at_zero);
      range_to_current_path_part.push_back(pt_ptr->path_obj_pair_output[i].range_to_current_path_part);
      range_at_host_edge.push_back(pt_ptr->path_obj_pair_output[i].range_at_host_edge);
      length_of_trajectory.push_back(pt_ptr->path_obj_pair_output[i].length_of_trajectory);
      path_heading.push_back(pt_ptr->path_obj_pair_output[i].path_heading);
      path_direction.push_back(pt_ptr->path_obj_pair_output[i].path_direction);
      track_match.push_back(pt_ptr->path_obj_pair_output[i].track_match);
      track_match_age.push_back(pt_ptr->path_obj_pair_output[i].track_match_age);
      track_match_last_cycle.push_back(pt_ptr->path_obj_pair_output[i].track_match_last_cycle);
   }
   hdf_pt_ptr.range_vcs_proj_to_path_segment.push_back(range_vcs_proj_to_path_segment);
   hdf_pt_ptr.segment_heading_diff.push_back(segment_heading_diff);
   hdf_pt_ptr.track_idx_nearest_path.push_back(track_idx_nearest_path);
   hdf_pt_ptr.range_at_zero.push_back(range_at_zero);
   hdf_pt_ptr.range_to_current_path_part.push_back(range_to_current_path_part);
   hdf_pt_ptr.range_at_host_edge.push_back(range_at_host_edge);
   hdf_pt_ptr.length_of_trajectory.push_back(length_of_trajectory);
   hdf_pt_ptr.path_heading.push_back(path_heading);
   hdf_pt_ptr.path_direction.push_back(path_direction);
   hdf_pt_ptr.track_match.push_back(track_match);
   hdf_pt_ptr.track_match_age.push_back(track_match_age);
   hdf_pt_ptr.track_match_last_cycle.push_back(track_match_last_cycle);
   hdf_pt_ptr.f_pt_operational.push_back(pt_ptr->f_pt_operational);

   // Verify if cta_ptr populated
   if (cta_ptr == nullptr) {
      std::cerr << "Error: Cta_Get_Output_Ptr() returned nullptr" << std::endl;
      return;
   }

   hdf_cta_ptr.f_cta_enabled.push_back(cta_ptr->f_cta_enabled);

   // Populate left side object data
   hdf_cta_ptr.most_critical_object_by_sides_id_left.push_back(cta_ptr->most_critical_object_by_sides[2][0].id);
   hdf_cta_ptr.most_critical_object_by_sides_unique_id_left.push_back(cta_ptr->most_critical_object_by_sides[2][0].unique_id);
   hdf_cta_ptr.most_critical_object_by_sides_objPoseX_m_left.push_back(cta_ptr->most_critical_object_by_sides[2][0].objPoseX_m);
   hdf_cta_ptr.most_critical_object_by_sides_objPoseY_m_left.push_back(cta_ptr->most_critical_object_by_sides[2][0].objPoseY_m);
   hdf_cta_ptr.most_critical_object_by_sides_objVelocityX_mps_left.push_back(cta_ptr->most_critical_object_by_sides[2][0].objVelocityX_mps);
   hdf_cta_ptr.most_critical_object_by_sides_objVelocityY_mps_left.push_back(cta_ptr->most_critical_object_by_sides[2][0].objVelocityY_mps);
   hdf_cta_ptr.most_critical_object_by_sides_heading_rad_left.push_back(cta_ptr->most_critical_object_by_sides[2][0].heading_rad);
   hdf_cta_ptr.most_critical_object_by_sides_alert_level_left.push_back(cta_ptr->most_critical_object_by_sides[2][0].alert_level);
   hdf_cta_ptr.most_critical_object_by_sides_ttc_s_left.push_back(cta_ptr->most_critical_object_by_sides[2][0].ttc_s);
   hdf_cta_ptr.most_critical_object_by_sides_intersection_point_x_m_left.push_back(cta_ptr->most_critical_object_by_sides[2][0].intersection_point_x_m);
   hdf_cta_ptr.most_critical_object_by_sides_f_brake_qualifier_left.push_back(cta_ptr->most_critical_object_by_sides[2][0].f_brake_qualifier);

   // Populate right side object data
   hdf_cta_ptr.most_critical_object_by_sides_id_right.push_back(cta_ptr->most_critical_object_by_sides[2][1].id);
   hdf_cta_ptr.most_critical_object_by_sides_unique_id_right.push_back(cta_ptr->most_critical_object_by_sides[2][1].unique_id);
   hdf_cta_ptr.most_critical_object_by_sides_objPoseX_m_right.push_back(cta_ptr->most_critical_object_by_sides[2][1].objPoseX_m);
   hdf_cta_ptr.most_critical_object_by_sides_objPoseY_m_right.push_back(cta_ptr->most_critical_object_by_sides[2][1].objPoseY_m);
   hdf_cta_ptr.most_critical_object_by_sides_objVelocityX_mps_right.push_back(cta_ptr->most_critical_object_by_sides[2][1].objVelocityX_mps);
   hdf_cta_ptr.most_critical_object_by_sides_objVelocityY_mps_right.push_back(cta_ptr->most_critical_object_by_sides[2][1].objVelocityY_mps);
   hdf_cta_ptr.most_critical_object_by_sides_heading_rad_right.push_back(cta_ptr->most_critical_object_by_sides[2][1].heading_rad);
   hdf_cta_ptr.most_critical_object_by_sides_alert_level_right.push_back(cta_ptr->most_critical_object_by_sides[2][1].alert_level);
   hdf_cta_ptr.most_critical_object_by_sides_ttc_s_right.push_back(cta_ptr->most_critical_object_by_sides[2][1].ttc_s);
   hdf_cta_ptr.most_critical_object_by_sides_intersection_point_x_m_right.push_back(cta_ptr->most_critical_object_by_sides[2][1].intersection_point_x_m);
   hdf_cta_ptr.most_critical_object_by_sides_f_brake_qualifier_right.push_back(cta_ptr->most_critical_object_by_sides[2][1].f_brake_qualifier);

   if (sg_ptr == nullptr) {
      std::cerr << "Error: GetSGOutputPtr() returned nullptr" << std::endl;
      return;
   }
   hdf_sg_ptr.execution_timestamp_us.push_back(sg_ptr->execution_timestamp_us);
   hdf_sg_ptr.measurement_timestamp_us.push_back(sg_ptr->measurement_timestamp_us);
   hdf_sg_ptr.cycle_index.push_back(sg_ptr->cycle_index);
   hdf_sg_ptr.num_contours.push_back(sg_ptr->num_contours);
   hdf_sg_ptr.f_valid.push_back(sg_ptr->f_valid);

   std::vector<float32_T> posn_x;
   std::vector<float32_T> posn_y;
   std::vector<float32_T> posn_variance_x;
   std::vector<float32_T> posn_variance_y;
   std::vector<float32_T> posn_covariance_xy;
   std::vector<unsigned16_T> cyc_since_created;
   std::vector<unsigned16_T> cyc_since_coasted;
   std::vector<unsigned8_T> drivability_class;
   std::vector<unsigned8_T> driv_confidence;

   for (int i = 0; i < SG_MAX_NUM_OUTPUT_VERTICES; i++) {
      posn_x.push_back(sg_ptr->vertices[i].position_x);
      posn_y.push_back(sg_ptr->vertices[i].position_y);
      posn_variance_x.push_back(sg_ptr->vertices[i].position_variance_x);
      posn_variance_y.push_back(sg_ptr->vertices[i].position_variance_y);
      posn_covariance_xy.push_back(sg_ptr->vertices[i].position_covariance_xy);
      cyc_since_created.push_back(sg_ptr->vertices[i].cycles_since_created);
      cyc_since_coasted.push_back(sg_ptr->vertices[i].cycles_since_coasted);
      drivability_class.push_back(static_cast<unsigned8_T>(sg_ptr->vertices[i].drivability));
      driv_confidence.push_back(sg_ptr->vertices[i].drivability_confidence);
   }

   hdf_sg_ptr.position_x.push_back(posn_x);
   hdf_sg_ptr.position_y.push_back(posn_y);
   hdf_sg_ptr.position_variance_x.push_back(posn_variance_x);
   hdf_sg_ptr.position_variance_y.push_back(posn_variance_y);
   hdf_sg_ptr.position_covariance_xy.push_back(posn_covariance_xy);
   hdf_sg_ptr.cycles_since_created.push_back(cyc_since_created);
   hdf_sg_ptr.cycles_since_coasted.push_back(cyc_since_coasted);
   hdf_sg_ptr.drivability.push_back(drivability_class);
   hdf_sg_ptr.drivability_confidence.push_back(driv_confidence);

   std::vector<unsigned32_T> u_id;
   std::vector<unsigned16_T> no_of_vertices;
   std::vector<unsigned8_T> contour_type;

   for (int j = 0; j < SG_MAX_NUM_OUTPUT_CONTOURS; j++) {
      u_id.push_back(sg_ptr->contours[j].unique_id);
      no_of_vertices.push_back(sg_ptr->contours[j].num_vertices);
      contour_type.push_back(static_cast<unsigned8_T>(sg_ptr->contours[j].type));
   }
   hdf_sg_ptr.unique_id.push_back(u_id);
   hdf_sg_ptr.num_vertices.push_back(no_of_vertices);
   hdf_sg_ptr.type.push_back(contour_type);

   hdf_sg_ptr.major.push_back(sg_ptr->software_version.major);
   hdf_sg_ptr.minor.push_back(sg_ptr->software_version.minor);
   hdf_sg_ptr.patch.push_back(sg_ptr->software_version.patch);
}

void HDFWriteClass::UpdateHDFIpbuffers() {

   hdf_scan_indexIp_ptr.scan_index.push_back(GetScanIndex());

   std::vector<float32_T> vcs_xposn;
   std::vector<float32_T> vcs_yposn;
   std::vector<float32_T> vcs_xvel;
   std::vector<float32_T> vcs_yvel;
   std::vector<float32_T> vcs_xaccel;
   std::vector<float32_T> vcs_yaccel;
   std::vector<float32_T> vcs_heading;
   std::vector<float32_T> speed_1;
   std::vector<float32_T> tang_accel;
   std::vector<float32_T> len1;
   std::vector<float32_T> wid1;
   std::vector<unsigned8_T> object_class;
   std::vector<unsigned32_T> unique_id;
   std::vector<unsigned8_T> status;

   for (int i = 0; i < TRACKER_NUMBER_OF_OBJECTS; i++) {
      unique_id.push_back(dc_input->object_rl[i].id);
      vcs_xposn.push_back(dc_input->object_rl[i].vcs_long_posn);
      vcs_yposn.push_back(dc_input->object_rl[i].vcs_lat_posn);
      vcs_xvel.push_back(dc_input->object_rl[i].vcs_long_vel);
      vcs_yvel.push_back(dc_input->object_rl[i].vcs_lat_vel);
      vcs_xaccel.push_back(dc_input->object_rl[i].vcs_long_accel);
      vcs_yaccel.push_back(dc_input->object_rl[i].vcs_lat_accel);
      vcs_heading.push_back(dc_input->object_rl[i].heading);
      speed_1.push_back(dc_input->object_rl[i].speed);
      tang_accel.push_back(dc_input->object_rl[i].tangential_accel);
      len1.push_back(dc_input->object_rl[i].length);
      wid1.push_back(dc_input->object_rl[i].width);
      status.push_back(dc_input->object_rl[i].status);
      object_class.push_back(dc_input->object_rl[i].object_class);
   }
   hdf_all_objectsGT_ptr.unique_id.push_back(unique_id);
   hdf_all_objectsGT_ptr.vcs_xposn.push_back(vcs_xposn);
   hdf_all_objectsGT_ptr.vcs_yposn.push_back(vcs_yposn);
   hdf_all_objectsGT_ptr.vcs_xvel.push_back(vcs_xvel);
   hdf_all_objectsGT_ptr.vcs_yvel.push_back(vcs_yvel);
   hdf_all_objectsGT_ptr.vcs_xaccel.push_back(vcs_xaccel);
   hdf_all_objectsGT_ptr.vcs_yaccel.push_back(vcs_yaccel);
   hdf_all_objectsGT_ptr.vcs_heading.push_back(vcs_heading);
   hdf_all_objectsGT_ptr.speed.push_back(speed_1);
   hdf_all_objectsGT_ptr.tang_accel.push_back(tang_accel);
   hdf_all_objectsGT_ptr.len1.push_back(len1);
   hdf_all_objectsGT_ptr.wid1.push_back(wid1);
   hdf_all_objectsGT_ptr.status.push_back(status);
   hdf_all_objectsGT_ptr.object_class.push_back(object_class);
}

void HDFWriteClass::Set_HDF_Output_Path(const char *filename) {
   if (filename == nullptr) {
      std::cerr << "Error: filename is nullptr" << std::endl;
   }

   char path_name[MAX_HDF_FILE_PATH] = "";

   fs::path nfilepath      = filename;
   std::string nfile_name  = nfilepath.stem().string();
   std::string HdfFileName = nfile_name + "_HDF_Report.h5";
   std::string npath_name  = nfilepath.parent_path().string() + "/HDF_Output/";
   HDF_Path                = npath_name + HdfFileName;
   std::copy(npath_name.begin(), npath_name.end(), path_name);

#ifdef _WIN32
   _mkdir(path_name);
#endif
#ifdef __GNUC__
   mkdir(path_name, ACCESSPERMS);
#endif
}

void HDFWriteClass::Set_HDF_Input_Path(const char *filename) {
   if (filename == nullptr) {
      std::cerr << "Error: filename is nullptr" << std::endl;
   }

   char path_name[MAX_HDF_FILE_PATH] = "";

   fs::path nfilepath      = filename;
   std::string nfile_name  = nfilepath.stem().string();
   std::string HdfFileName = nfile_name + "_HDF_Report.h5";
   std::string npath_name  = nfilepath.parent_path().string() + "/HDF_Input/";
   HDF_IPath               = npath_name + HdfFileName;
   std::copy(npath_name.begin(), npath_name.end(), path_name);

#ifdef _WIN32
   _mkdir(path_name);
#endif
#ifdef __GNUC__
   mkdir(path_name, ACCESSPERMS);
#endif
}

string HDFWriteClass::Get_HDF_Output_Path() {
   return HDF_Path;
}

string HDFWriteClass::Get_HDF_Input_Path() {
   return HDF_IPath;
}

void HDFWriteClass::HDF_Write(void) {

   HighFive::File file(this->HDF_Path, HighFive::File::Truncate);

   auto gcpl = HighFive::GroupCreateProps::Default();
   gcpl.add(HighFive::AttributePhaseChange(0, 0));

   size_t vecsize  = hdf_scan_index_ptr.scan_index.size();
   size_t vecsize1 = hdf_raw_detection_info_ptr.elevation[0].size();

   if (vecsize != 0 && vecsize1 != 0) {
      HighFive::DataSetCreateProps props1D;
      props1D.add(HighFive::Chunking{vecsize});
      props1D.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props2D;
      props2D.add(HighFive::Chunking{vecsize, 8});
      props2D.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_2d;
      props_2d.add(HighFive::Chunking{vecsize, vecsize1, 8});
      props_2d.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_egod;
      props_egod.add(HighFive::Chunking{vecsize, 10});
      props_egod.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_GT;
      props_GT.add(HighFive::Chunking{vecsize, hdf_GT_ptr.id[0].size()});
      props_GT.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_SrtRefId;
      props_SrtRefId.add(HighFive::Chunking{vecsize, f360_variant_A::MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS});
      props_SrtRefId.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_MaxDet;
      props_MaxDet.add(HighFive::Chunking{vecsize, f360_variant_A::MAX_NUMBER_OF_DETECTIONS});
      props_MaxDet.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_crossCov;
      props_crossCov.add(HighFive::Chunking{vecsize, f360_variant_A::MAX_NUMBER_OF_DETECTIONS, 5});
      props_crossCov.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_posCov;
      props_posCov.add(HighFive::Chunking{vecsize, f360_variant_A::MAX_NUMBER_OF_DETECTIONS, 3});
      props_posCov.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_Ant;
      props_Ant.add(HighFive::Chunking{vecsize, 18});
      props_Ant.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_Rspp;
      props_Rspp.add(HighFive::Chunking{vecsize, RSPP_DET_NUM_LOOK_ID});
      props_Rspp.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_ObjLog;
      props_ObjLog.add(HighFive::Chunking{vecsize, MAX_F360_OBJECTS_LOG});
      props_ObjLog.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_AllobjVar;
      props_AllobjVar.add(HighFive::Chunking{vecsize, MAX_F360_OBJECTS_LOG, STATE_VARIANCE_ARRAY_SIZE});
      props_AllobjVar.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_AllobjCovVar;
      props_AllobjCovVar.add(HighFive::Chunking{vecsize, MAX_F360_OBJECTS_LOG, SUPPLEMENTAL_STATE_COVARIANCE_ARRAY_SIZE});
      props_AllobjCovVar.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_RedObj;
      props_RedObj.add(HighFive::Chunking{vecsize, MAX_REDUCED_OBJECTS_LOG});
      props_RedObj.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_ROTObj;
      props_ROTObj.add(HighFive::Chunking{vecsize, f360_variant_A::NUMBER_OF_REDUCED_OBJECT_TRACKS});
      props_ROTObj.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_OLP;
      props_OLP.add(HighFive::Chunking{vecsize, SFL_OBJ_NUMBER_OF_OBJECTS});
      props_OLP.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_pt;
      props_pt.add(HighFive::Chunking{vecsize, PA_OBJ_NUMBER_OF_OBJECTS});
      props_pt.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_sg;
      props_sg.add(HighFive::Chunking{vecsize, SG_MAX_NUM_OUTPUT_VERTICES});
      props_sg.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_sg_contours;
      props_sg_contours.add(HighFive::Chunking{vecsize, SG_MAX_NUM_OUTPUT_CONTOURS});
      props_sg_contours.add(HighFive::Deflate(9));

      using Clock = std::chrono::system_clock;
      auto now    = Clock::now();

      std::time_t tt = Clock::to_time_t(now);
      std::tm local_tm{};
#ifdef _WIN32
      localtime_s(&local_tm, &tt);
#else
      localtime_r(&tt, &local_tm);
#endif
      char ts_buf[32];

      std::strftime(ts_buf, sizeof(ts_buf), "%d-%m-%Y %H:%M:%S", &local_tm);
      std::string created_datetime(ts_buf);
      std::string DC_version  = std::to_string(APPLICATION_MAJOR_VERSION) + "." + std::to_string(APPLICATION_MINOR_VERSION) + "." + std::to_string(APPLICATION_PATCH_VERSION);
      std::string sfl_version = "3.3.0";

      // read versions.txt at runtime
      const auto versions_kv = read_versions_txt(get_exe_versions_path());

      // fallback values if keys are missing
      auto get_or = [&](const char *key, const char *fallback) -> std::string {
         auto it = versions_kv.find(key);
         return (it != versions_kv.end() && !it->second.empty()) ? it->second : std::string(fallback);
      };

      std::string tracker_version = get_or("Tracker_version", "unknown");
      std::string ocg_version     = get_or("OCG_version", "unknown");
      std::string olp_version     = get_or("OLP_version", "unknown");

      // ... proceed to write these into your HDF5 group/datasets as before

      auto meta_grp = file.createGroup("00_metadata", gcpl);
      meta_grp.createDataSet<std::string>("Created_datetime", ds::From(created_datetime)).write(created_datetime);
      meta_grp.createDataSet<std::string>("DC_version", ds::From(DC_version)).write(DC_version);
      meta_grp.createDataSet<std::string>("Tracker_version", ds::From(tracker_version)).write(tracker_version);
      meta_grp.createDataSet<std::string>("OCG_version", ds::From(ocg_version)).write(ocg_version);
      meta_grp.createDataSet<std::string>("OLP_version", ds::From(olp_version)).write(olp_version);
      meta_grp.createDataSet<std::string>("SFL_version", ds::From(sfl_version)).write(sfl_version);

      auto scan_grp = file.createGroup("01_Scan_Index", gcpl);
      (scan_grp.createDataSet<uint32_t>("scan_index", ds::From(hdf_scan_index_ptr.scan_index), props1D).write(hdf_scan_index_ptr.scan_index));

      auto g1  = file.createGroup("02_Input_DC_Data", gcpl);
      auto s13 = g1.createGroup("Detections");
      auto s1  = s13.createGroup("Detection_Header");
      auto s14 = g1.createGroup("MountingPosition");
      auto s15 = g1.createGroup("VehicleInfo");
      auto s20 = g1.createGroup("GroundTruth");

      (s1.createDataSet<float32_T>("timestamp", ds::From(hdf_raw_detection_info_ptr.timestamp), props2D).write(hdf_raw_detection_info_ptr.timestamp));
      (s1.createDataSet<unsigned32_T>("hdrTimestamp_fractionalSec", ds::From(hdf_raw_detection_info_ptr.hdrTimestamp_fractionalSec), props2D).write(hdf_raw_detection_info_ptr.hdrTimestamp_fractionalSec));
      (s1.createDataSet<unsigned32_T>("hdrTimestamp_Sec", ds::From(hdf_raw_detection_info_ptr.hdrTimestamp_Sec), props2D).write(hdf_raw_detection_info_ptr.hdrTimestamp_Sec));
      (s1.createDataSet<float32_T>("AutoAlignElevation", ds::From(hdf_raw_detection_info_ptr.AutoAlignElevation), props2D).write(hdf_raw_detection_info_ptr.AutoAlignElevation));
      (s1.createDataSet<unsigned32_T>("Count", ds::From(hdf_raw_detection_info_ptr.Count), props2D).write(hdf_raw_detection_info_ptr.Count));
      (s1.createDataSet<unsigned32_T>("ScanIndex", ds::From(hdf_raw_detection_info_ptr.ScanIndex), props2D).write(hdf_raw_detection_info_ptr.ScanIndex));
      (s1.createDataSet<unsigned8_T>("AutoAlignAzimuthQF", ds::From(hdf_raw_detection_info_ptr.AutoAlignAzimuthQF), props2D).write(hdf_raw_detection_info_ptr.AutoAlignAzimuthQF));
      (s1.createDataSet<unsigned8_T>("AutoAlignElevationQF", ds::From(hdf_raw_detection_info_ptr.AutoAlignElevationQF), props2D).write(hdf_raw_detection_info_ptr.AutoAlignElevationQF));
      (s1.createDataSet<unsigned8_T>("timestamp_consistency", ds::From(hdf_raw_detection_info_ptr.timestamp_consistency), props2D).write(hdf_raw_detection_info_ptr.timestamp_consistency));
      (s1.createDataSet<unsigned8_T>("LookID", ds::From(hdf_raw_detection_info_ptr.LookID), props2D).write(hdf_raw_detection_info_ptr.LookID));
      (s1.createDataSet<unsigned8_T>("LookType", ds::From(hdf_raw_detection_info_ptr.LookType), props2D).write(hdf_raw_detection_info_ptr.LookType));

      auto s2 = s13.createGroup("Detection_Info");
      (s2.createDataSet<float32_T>("elevation", ds::From(hdf_raw_detection_info_ptr.elevation), props_2d).write(hdf_raw_detection_info_ptr.elevation));
      (s2.createDataSet<float32_T>("azimuth", ds::From(hdf_raw_detection_info_ptr.azimuth), props_2d).write(hdf_raw_detection_info_ptr.azimuth));
      (s2.createDataSet<float32_T>("range_rate", ds::From(hdf_raw_detection_info_ptr.range_rate), props_2d).write(hdf_raw_detection_info_ptr.range_rate));
      (s2.createDataSet<float32_T>("range", ds::From(hdf_raw_detection_info_ptr.range), props_2d).write(hdf_raw_detection_info_ptr.range));
      (s2.createDataSet<float32_T>("amplitude", ds::From(hdf_raw_detection_info_ptr.amplitude), props_2d).write(hdf_raw_detection_info_ptr.amplitude));
      (s2.createDataSet<float32_T>("snr", ds::From(hdf_raw_detection_info_ptr.snr), props_2d).write(hdf_raw_detection_info_ptr.snr));
      (s2.createDataSet<float32_T>("std_elevation", ds::From(hdf_raw_detection_info_ptr.std_elevation), props_2d).write(hdf_raw_detection_info_ptr.std_elevation));
      (s2.createDataSet<float32_T>("std_azimuth", ds::From(hdf_raw_detection_info_ptr.std_azimuth), props_2d).write(hdf_raw_detection_info_ptr.std_azimuth));
      (s2.createDataSet<float32_T>("std_range", ds::From(hdf_raw_detection_info_ptr.std_range), props_2d).write(hdf_raw_detection_info_ptr.std_range));
      (s2.createDataSet<float32_T>("std_range_rate", ds::From(hdf_raw_detection_info_ptr.std_range_rate), props_2d).write(hdf_raw_detection_info_ptr.std_range_rate));
      (s2.createDataSet<float32_T>("std_rcs", ds::From(hdf_raw_detection_info_ptr.std_rcs), props_2d).write(hdf_raw_detection_info_ptr.std_rcs));
      (s2.createDataSet<float32_T>("multi_target_probability", ds::From(hdf_raw_detection_info_ptr.multi_target_probability), props_2d).write(hdf_raw_detection_info_ptr.multi_target_probability));
      (s2.createDataSet<float32_T>("existence_probability ", ds::From(hdf_raw_detection_info_ptr.existence_probability), props_2d).write(hdf_raw_detection_info_ptr.existence_probability));
      (s2.createDataSet<unsigned8_T>("azimuth_confidence", ds::From(hdf_raw_detection_info_ptr.azimuth_confidence), props_2d).write(hdf_raw_detection_info_ptr.azimuth_confidence));
      (s2.createDataSet<unsigned8_T>("elevation_confidence", ds::From(hdf_raw_detection_info_ptr.elevation_confidence), props_2d).write(hdf_raw_detection_info_ptr.elevation_confidence));
      (s2.createDataSet<unsigned8_T>("valid", ds::From(hdf_raw_detection_info_ptr.valid), props_2d).write(hdf_raw_detection_info_ptr.valid));
      (s2.createDataSet<boolean_T>("host_veh_clutter", ds::From(hdf_raw_detection_info_ptr.host_veh_clutter), props_2d).write(hdf_raw_detection_info_ptr.host_veh_clutter));
      (s2.createDataSet<boolean_T>("nd_target", ds::From(hdf_raw_detection_info_ptr.nd_target), props_2d).write(hdf_raw_detection_info_ptr.nd_target));
      (s2.createDataSet<boolean_T>("bistatic", ds::From(hdf_raw_detection_info_ptr.bistatic), props_2d).write(hdf_raw_detection_info_ptr.bistatic));

      (s14.createDataSet<float32_T>("azimuth_polarity", ds::From(hdf_MountInfo_ptr.azimuth_polarity), props2D).write(hdf_MountInfo_ptr.azimuth_polarity));
      (s14.createDataSet<float32_T>("boresight_angle", ds::From(hdf_MountInfo_ptr.boresight_angle), props2D).write(hdf_MountInfo_ptr.boresight_angle));
      (s14.createDataSet<float32_T>("vcs_lat_position", ds::From(hdf_MountInfo_ptr.vcs_lat_position), props2D).write(hdf_MountInfo_ptr.vcs_lat_position));
      (s14.createDataSet<float32_T>("vcs_lon_position", ds::From(hdf_MountInfo_ptr.vcs_lon_position), props2D).write(hdf_MountInfo_ptr.vcs_lon_position));
      (s14.createDataSet<float32_T>("vcs_z_position", ds::From(hdf_MountInfo_ptr.vcs_z_position), props2D).write(hdf_MountInfo_ptr.vcs_z_position));

      (s15.createDataSet<float32_T>("abs_speed", ds::From(hdf_InVehicleInfo_ptr.abs_speed), props1D).write(hdf_InVehicleInfo_ptr.abs_speed));
      (s15.createDataSet<float32_T>("yawrate", ds::From(hdf_InVehicleInfo_ptr.yawrate), props1D).write(hdf_InVehicleInfo_ptr.yawrate));
      (s15.createDataSet<float32_T>("steering_angle", ds::From(hdf_InVehicleInfo_ptr.steering_angle), props1D).write(hdf_InVehicleInfo_ptr.steering_angle));
      (s15.createDataSet<float32_T>("rear_axle_steering_angle", ds::From(hdf_InVehicleInfo_ptr.rear_axle_steering_angle), props1D).write(hdf_InVehicleInfo_ptr.rear_axle_steering_angle));
      (s15.createDataSet<float32_T>("rear_axle_position", ds::From(hdf_InVehicleInfo_ptr.rear_axle_position), props1D).write(hdf_InVehicleInfo_ptr.rear_axle_position));
      (s15.createDataSet<float32_T>("lane_width_external", ds::From(hdf_InVehicleInfo_ptr.lane_width_external), props1D).write(hdf_InVehicleInfo_ptr.lane_width_external));
      (s15.createDataSet<float32_T>("lane_center_offset_external", ds::From(hdf_InVehicleInfo_ptr.lane_center_offset_external), props1D).write(hdf_InVehicleInfo_ptr.lane_center_offset_external));
      (s15.createDataSet<float32_T>("host_vehicle_length", ds::From(hdf_InVehicleInfo_ptr.host_vehicle_length), props1D).write(hdf_InVehicleInfo_ptr.host_vehicle_length));
      (s15.createDataSet<float32_T>("host_vehicle_width", ds::From(hdf_InVehicleInfo_ptr.host_vehicle_width), props1D).write(hdf_InVehicleInfo_ptr.host_vehicle_width));
      (s15.createDataSet<float32_T>("host_vehicle_height", ds::From(hdf_InVehicleInfo_ptr.host_vehicle_height), props1D).write(hdf_InVehicleInfo_ptr.host_vehicle_height));
      (s15.createDataSet<float32_T>("curve_radius", ds::From(hdf_InVehicleInfo_ptr.curve_radius), props1D).write(hdf_InVehicleInfo_ptr.curve_radius));
      (s15.createDataSet<float32_T>("vcs_long_acc", ds::From(hdf_InVehicleInfo_ptr.vcs_long_acc), props1D).write(hdf_InVehicleInfo_ptr.vcs_long_acc));
      (s15.createDataSet<float32_T>("vcs_lat_acc", ds::From(hdf_InVehicleInfo_ptr.vcs_lat_acc), props1D).write(hdf_InVehicleInfo_ptr.vcs_lat_acc));
      (s15.createDataSet<float32_T>("bb_center_to_rear_x", ds::From(hdf_InVehicleInfo_ptr.bb_center_to_rear_x), props1D).write(hdf_InVehicleInfo_ptr.bb_center_to_rear_x));
      (s15.createDataSet<float32_T>("bb_center_to_rear_y", ds::From(hdf_InVehicleInfo_ptr.bb_center_to_rear_y), props1D).write(hdf_InVehicleInfo_ptr.bb_center_to_rear_y));
      (s15.createDataSet<float32_T>("bb_center_to_rear_z", ds::From(hdf_InVehicleInfo_ptr.bb_center_to_rear_z), props1D).write(hdf_InVehicleInfo_ptr.bb_center_to_rear_z));
      (s15.createDataSet<unsigned32_T>("vehicle_data_buff_timestamp", ds::From(hdf_InVehicleInfo_ptr.vehicle_data_buff_timestamp), props1D).write(hdf_InVehicleInfo_ptr.vehicle_data_buff_timestamp));
      (s15.createDataSet<unsigned8_T>("prndl", ds::From(hdf_InVehicleInfo_ptr.prndl), props1D).write(hdf_InVehicleInfo_ptr.prndl));
      (s15.createDataSet<unsigned8_T>("turn_signal", ds::From(hdf_InVehicleInfo_ptr.turn_signal), props1D).write(hdf_InVehicleInfo_ptr.turn_signal));
      (s15.createDataSet<unsigned8_T>("f_reverse_gear", ds::From(hdf_InVehicleInfo_ptr.f_reverse_gear), props1D).write(hdf_InVehicleInfo_ptr.f_reverse_gear));
      (s15.createDataSet<unsigned8_T>("f_trailer_present", ds::From(hdf_InVehicleInfo_ptr.f_trailer_present), props1D).write(hdf_InVehicleInfo_ptr.f_trailer_present));
      (s15.createDataSet<unsigned8_T>("f_traffic_side", ds::From(hdf_InVehicleInfo_ptr.f_traffic_side), props1D).write(hdf_InVehicleInfo_ptr.f_traffic_side));

      (s15.createDataSet<unsigned32_T>("ego_Sec", ds::From(hdf_InVehicleInfo_ptr.ego_Sec), props_egod).write(hdf_InVehicleInfo_ptr.ego_Sec));
      (s15.createDataSet<unsigned32_T>("ego_FractionalSec", ds::From(hdf_InVehicleInfo_ptr.ego_FractionalSec), props_egod).write(hdf_InVehicleInfo_ptr.ego_FractionalSec));
      (s15.createDataSet<float32_T>("ego_vcs_long_acc", ds::From(hdf_InVehicleInfo_ptr.ego_vcs_long_acc), props_egod).write(hdf_InVehicleInfo_ptr.ego_vcs_long_acc));
      (s15.createDataSet<unsigned32_T>("ego_valueQEgoAccelerationLongitudinalCog", ds::From(hdf_InVehicleInfo_ptr.ego_valueQEgoAccelerationLongitudinalCog), props_egod).write(hdf_InVehicleInfo_ptr.ego_valueQEgoAccelerationLongitudinalCog));
      (s15.createDataSet<float32_T>("ego_vcs_lat_acc", ds::From(hdf_InVehicleInfo_ptr.ego_vcs_lat_acc), props_egod).write(hdf_InVehicleInfo_ptr.ego_vcs_lat_acc));
      (s15.createDataSet<unsigned32_T>("ego_valueQEgoAccelerationLateralCog", ds::From(hdf_InVehicleInfo_ptr.ego_valueQEgoAccelerationLateralCog), props_egod).write(hdf_InVehicleInfo_ptr.ego_valueQEgoAccelerationLateralCog));
      (s15.createDataSet<float32_T>("ego_yawrate", ds::From(hdf_InVehicleInfo_ptr.ego_yawrate), props_egod).write(hdf_InVehicleInfo_ptr.ego_yawrate));
      (s15.createDataSet<unsigned32_T>("ego_valueQYawRateVehicleBody", ds::From(hdf_InVehicleInfo_ptr.ego_valueQYawRateVehicleBody), props_egod).write(hdf_InVehicleInfo_ptr.ego_valueQYawRateVehicleBody));
      (s15.createDataSet<float32_T>("ego_steering_angle", ds::From(hdf_InVehicleInfo_ptr.ego_steering_angle), props_egod).write(hdf_InVehicleInfo_ptr.ego_steering_angle));
      (s15.createDataSet<float32_T>("ego_abs_speed", ds::From(hdf_InVehicleInfo_ptr.ego_abs_speed), props_egod).write(hdf_InVehicleInfo_ptr.ego_abs_speed));
      (s15.createDataSet<unsigned32_T>("ego_valueQEgoSpeedCog", ds::From(hdf_InVehicleInfo_ptr.ego_valueQEgoSpeedCog), props_egod).write(hdf_InVehicleInfo_ptr.ego_valueQEgoSpeedCog));
      (s15.createDataSet<unsigned32_T>("ego_drivingDirectionConfirmed", ds::From(hdf_InVehicleInfo_ptr.ego_drivingDirectionConfirmed), props_egod).write(hdf_InVehicleInfo_ptr.ego_drivingDirectionConfirmed));

      (s20.createDataSet<unsigned8_T>("ID", ds::From(hdf_GT_ptr.id), props_GT).write(hdf_GT_ptr.id));
      (s20.createDataSet<signed8_T>("status", ds::From(hdf_GT_ptr.status), props_GT).write(hdf_GT_ptr.status));
      (s20.createDataSet<unsigned8_T>("age", ds::From(hdf_GT_ptr.age), props_GT).write(hdf_GT_ptr.age));
      (s20.createDataSet<unsigned8_T>("stage_age", ds::From(hdf_GT_ptr.stage_age), props_GT).write(hdf_GT_ptr.stage_age));
      (s20.createDataSet<float32_T>("vcs_long_posn", ds::From(hdf_GT_ptr.vcs_long_posn), props_GT).write(hdf_GT_ptr.vcs_long_posn));
      (s20.createDataSet<float32_T>("vcs_long_vel", ds::From(hdf_GT_ptr.vcs_long_vel), props_GT).write(hdf_GT_ptr.vcs_long_vel));
      (s20.createDataSet<float32_T>("vcs_long_accel", ds::From(hdf_GT_ptr.vcs_long_accel), props_GT).write(hdf_GT_ptr.vcs_long_accel));
      (s20.createDataSet<float32_T>("vcs_lat_posn", ds::From(hdf_GT_ptr.vcs_lat_posn), props_GT).write(hdf_GT_ptr.vcs_lat_posn));
      (s20.createDataSet<float32_T>("vcs_lat_vel", ds::From(hdf_GT_ptr.vcs_lat_vel), props_GT).write(hdf_GT_ptr.vcs_lat_vel));
      (s20.createDataSet<float32_T>("vcs_lat_accel", ds::From(hdf_GT_ptr.vcs_lat_accel), props_GT).write(hdf_GT_ptr.vcs_lat_accel));
      (s20.createDataSet<float32_T>("vcs_long_vel_rel", ds::From(hdf_GT_ptr.vcs_long_vel_rel), props_GT).write(hdf_GT_ptr.vcs_long_vel_rel));
      (s20.createDataSet<float32_T>("vcs_lat_vel_rel", ds::From(hdf_GT_ptr.vcs_lat_vel_rel), props_GT).write(hdf_GT_ptr.vcs_lat_vel_rel));
      (s20.createDataSet<float32_T>("speed", ds::From(hdf_GT_ptr.speed), props_GT).write(hdf_GT_ptr.speed));
      (s20.createDataSet<float32_T>("tangential_accel", ds::From(hdf_GT_ptr.tangential_accel), props_GT).write(hdf_GT_ptr.tangential_accel));
      (s20.createDataSet<float32_T>("heading", ds::From(hdf_GT_ptr.heading), props_GT).write(hdf_GT_ptr.heading));
      (s20.createDataSet<float32_T>("heading_rate", ds::From(hdf_GT_ptr.heading_rate), props_GT).write(hdf_GT_ptr.heading_rate));
      (s20.createDataSet<float32_T>("length", ds::From(hdf_GT_ptr.length), props_GT).write(hdf_GT_ptr.length));
      (s20.createDataSet<float32_T>("width", ds::From(hdf_GT_ptr.width), props_GT).write(hdf_GT_ptr.width));
      (s20.createDataSet<signed8_T>("object_class", ds::From(hdf_GT_ptr.object_class), props_GT).write(hdf_GT_ptr.object_class));

      // Write Tracker information
      auto g2 = file.createGroup("03_TrackerInfo", gcpl);
      auto s3 = g2.createGroup("Tracker_Input");
      auto s4 = s3.createGroup("CoreInfo");
      // auto s5 = s3.createGroup("Vehicle_Info");
      auto s6  = s3.createGroup("TrackInfo");
      auto s12 = s3.createGroup("SensorInfo");
      auto s7  = g2.createGroup("Tracker_Output");
      auto s8  = s3.createGroup("DetectionsList");
      auto s9  = s3.createGroup("VSE");
      auto s10 = s7.createGroup("AllObjects");
      auto s11 = s7.createGroup("ROT");

      /* Tracker Info*/
      (s6.createDataSet<unsigned64_T>("timestamp_us", ds::From(hdf_tracker_info_ptr.timestamp_us), props1D).write(hdf_tracker_info_ptr.timestamp_us));
      (s6.createDataSet<unsigned64_T>("object_list_timestamp", ds::From(hdf_tracker_info_ptr.object_list_timestamp), props1D).write(hdf_tracker_info_ptr.object_list_timestamp));
      (s6.createDataSet<float32_T>("nr_suspected_stat_angle_jump_dets_filtered", ds::From(hdf_tracker_info_ptr.nr_suspected_stat_angle_jump_dets_filtered), props1D).write(hdf_tracker_info_ptr.nr_suspected_stat_angle_jump_dets_filtered));
      (s6.createDataSet<float32_T>("elapsed_time_s", ds::From(hdf_tracker_info_ptr.elapsed_time_s), props1D).write(hdf_tracker_info_ptr.elapsed_time_s));
      (s6.createDataSet<unsigned32_T>("sw_version_buildID", ds::From(hdf_tracker_info_ptr.sw_version_buildID), props1D).write(hdf_tracker_info_ptr.sw_version_buildID));
      (s6.createDataSet<unsigned32_T>("tracker_index", ds::From(hdf_tracker_info_ptr.tracker_index), props1D).write(hdf_tracker_info_ptr.tracker_index));
      (s6.createDataSet<unsigned32_T>("vehicle_index", ds::From(hdf_tracker_info_ptr.vehicle_index), props1D).write(hdf_tracker_info_ptr.vehicle_index));
      (s6.createDataSet<unsigned16_T>("active_obj_ids", ds::From(hdf_tracker_info_ptr.active_obj_ids), props_ObjLog).write(hdf_tracker_info_ptr.active_obj_ids));
      (s6.createDataSet<unsigned16_T>("inactive_obj_ids", ds::From(hdf_tracker_info_ptr.inactive_obj_ids), props_ObjLog).write(hdf_tracker_info_ptr.inactive_obj_ids));
      (s6.createDataSet<unsigned16_T>("num_active_objs", ds::From(hdf_tracker_info_ptr.num_active_objs), props1D).write(hdf_tracker_info_ptr.num_active_objs));
      (s6.createDataSet<unsigned16_T>("reduced_active_obj_ids", ds::From(hdf_tracker_info_ptr.reduced_active_obj_ids), props_RedObj).write(hdf_tracker_info_ptr.reduced_active_obj_ids));
      (s6.createDataSet<unsigned16_T>("reduced_inactive_obj_ids", ds::From(hdf_tracker_info_ptr.reduced_inactive_obj_ids), props_RedObj).write(hdf_tracker_info_ptr.reduced_inactive_obj_ids));
      (s6.createDataSet<unsigned16_T>("reduced_obj_ids", ds::From(hdf_tracker_info_ptr.reduced_obj_ids), props_RedObj).write(hdf_tracker_info_ptr.reduced_obj_ids));
      (s6.createDataSet<unsigned16_T>("reduced_num_active_objs", ds::From(hdf_tracker_info_ptr.reduced_num_active_objs), props1D).write(hdf_tracker_info_ptr.reduced_num_active_objs));
      (s6.createDataSet<unsigned32_T>("num_unique_objs", ds::From(hdf_tracker_info_ptr.num_unique_objs), props1D).write(hdf_tracker_info_ptr.num_unique_objs));
      (s6.createDataSet<unsigned16_T>("num_active_clusters", ds::From(hdf_tracker_info_ptr.num_active_clusters), props1D).write(hdf_tracker_info_ptr.num_active_clusters));
      (s6.createDataSet<unsigned16_T>("number_of_historic_detections", ds::From(hdf_tracker_info_ptr.number_of_historic_detections), props1D).write(hdf_tracker_info_ptr.number_of_historic_detections));
      (s6.createDataSet<unsigned8_T>("sw_version_major", ds::From(hdf_tracker_info_ptr.sw_version_major), props1D).write(hdf_tracker_info_ptr.sw_version_major));
      (s6.createDataSet<unsigned8_T>("sw_version_minor", ds::From(hdf_tracker_info_ptr.sw_version_minor), props1D).write(hdf_tracker_info_ptr.sw_version_minor));
      (s6.createDataSet<unsigned8_T>("sw_version_patch", ds::From(hdf_tracker_info_ptr.sw_version_patch), props1D).write(hdf_tracker_info_ptr.sw_version_patch));
      (s6.createDataSet<unsigned8_T>("f_severe_angle_jump_detected", ds::From(hdf_tracker_info_ptr.f_severe_angle_jump_detected), props1D).write(hdf_tracker_info_ptr.f_severe_angle_jump_detected));

      /* Detection List Info*/

      auto s16 = s8.createGroup("DetectionListHeader");
      auto s17 = s8.createGroup("RawDetectionList");
      auto s18 = s8.createGroup("ProcessedDetectionList");

      (s16.createDataSet<unsigned64_T>("number_of_valid_detections", ds::From(hdf_processed_info_ptr.number_of_valid_detections), props1D).write(hdf_processed_info_ptr.number_of_valid_detections));
      (s16.createDataSet<signed16_T>("vcslong_det_idx_min", ds::From(hdf_processed_info_ptr.vcslong_det_idx_min), props1D).write(hdf_processed_info_ptr.vcslong_det_idx_min));
      (s16.createDataSet<signed16_T>("vcslong_det_idx_max", ds::From(hdf_processed_info_ptr.vcslong_det_idx_max), props1D).write(hdf_processed_info_ptr.vcslong_det_idx_max));
      (s16.createDataSet<signed16_T>("vcslong_sorted_ref_det_idx", ds::From(hdf_processed_info_ptr.vcslong_sorted_ref_det_idx), props_SrtRefId).write(hdf_processed_info_ptr.vcslong_sorted_ref_det_idx));

      (s17.createDataSet<float32_T>("range", ds::From(hdf_processed_info_ptr.RD_range), props_MaxDet).write(hdf_processed_info_ptr.RD_range));
      (s17.createDataSet<float32_T>("std_range", ds::From(hdf_processed_info_ptr.RD_std_range), props_MaxDet).write(hdf_processed_info_ptr.RD_std_range));
      (s17.createDataSet<float32_T>("range_rate", ds::From(hdf_processed_info_ptr.RD_range_rate), props_MaxDet).write(hdf_processed_info_ptr.RD_range_rate));
      (s17.createDataSet<float32_T>("std_range_rate", ds::From(hdf_processed_info_ptr.RD_std_range_rate), props_MaxDet).write(hdf_processed_info_ptr.RD_std_range_rate));
      (s17.createDataSet<float32_T>("azimuth", ds::From(hdf_processed_info_ptr.RD_azimuth), props_MaxDet).write(hdf_processed_info_ptr.RD_azimuth));
      (s17.createDataSet<float32_T>("std_azimuth", ds::From(hdf_processed_info_ptr.RD_std_azimuth), props_MaxDet).write(hdf_processed_info_ptr.RD_std_azimuth));
      (s17.createDataSet<float32_T>("elevation", ds::From(hdf_processed_info_ptr.RD_elevation), props_MaxDet).write(hdf_processed_info_ptr.RD_elevation));
      (s17.createDataSet<float32_T>("std_elevation", ds::From(hdf_processed_info_ptr.RD_std_elevation), props_MaxDet).write(hdf_processed_info_ptr.RD_std_elevation));
      (s17.createDataSet<float32_T>("snr", ds::From(hdf_processed_info_ptr.RD_snr), props_MaxDet).write(hdf_processed_info_ptr.RD_snr));
      (s17.createDataSet<float32_T>("rcs", ds::From(hdf_processed_info_ptr.RD_rcs), props_MaxDet).write(hdf_processed_info_ptr.RD_rcs));
      (s17.createDataSet<float32_T>("prob_1stazhypo", ds::From(hdf_processed_info_ptr.RD_prob_1stazhypo), props_MaxDet).write(hdf_processed_info_ptr.RD_prob_1stazhypo));
      (s17.createDataSet<signed32_T>("sensor_id", ds::From(hdf_processed_info_ptr.RD_sensor_id), props_MaxDet).write(hdf_processed_info_ptr.RD_sensor_id));
      (s17.createDataSet<signed32_T>("det_id", ds::From(hdf_processed_info_ptr.RD_det_id), props_MaxDet).write(hdf_processed_info_ptr.RD_det_id));
      (s17.createDataSet<signed8_T>("confid_azimuth", ds::From(hdf_processed_info_ptr.RD_confid_azimuth), props_MaxDet).write(hdf_processed_info_ptr.RD_confid_azimuth));
      (s17.createDataSet<signed8_T>("confid_elevation", ds::From(hdf_processed_info_ptr.RD_confid_elevation), props_MaxDet).write(hdf_processed_info_ptr.RD_confid_elevation));
      (s17.createDataSet<boolean_T>("f_super_res", ds::From(hdf_processed_info_ptr.RD_f_super_res), props_MaxDet).write(hdf_processed_info_ptr.RD_f_super_res));
      (s17.createDataSet<boolean_T>("f_host_veh_clutter", ds::From(hdf_processed_info_ptr.RD_f_host_veh_clutter), props_MaxDet).write(hdf_processed_info_ptr.RD_f_host_veh_clutter));
      (s17.createDataSet<boolean_T>("f_nd_target", ds::From(hdf_processed_info_ptr.RD_f_nd_target), props_MaxDet).write(hdf_processed_info_ptr.RD_f_nd_target));
      (s17.createDataSet<boolean_T>("f_bistatic", ds::From(hdf_processed_info_ptr.RD_f_bistatic), props_MaxDet).write(hdf_processed_info_ptr.RD_f_bistatic));
      (s17.createDataSet<boolean_T>("f_ci_det", ds::From(hdf_processed_info_ptr.RD_f_ci_det), props_MaxDet).write(hdf_processed_info_ptr.RD_f_ci_det));
      (s17.createDataSet<boolean_T>("f_idm_det", ds::From(hdf_processed_info_ptr.RD_f_idm_det), props_MaxDet).write(hdf_processed_info_ptr.RD_f_idm_det));
      (s17.createDataSet<boolean_T>("f_below_rain_thold", ds::From(hdf_processed_info_ptr.RD_f_below_rain_thold), props_MaxDet).write(hdf_processed_info_ptr.RD_f_below_rain_thold));

      (s18.createDataSet<float32_T>("vcs_position_x", ds::From(hdf_processed_info_ptr.PD_vcs_position_x), props_MaxDet).write(hdf_processed_info_ptr.PD_vcs_position_x));
      (s18.createDataSet<float32_T>("vcs_position_y", ds::From(hdf_processed_info_ptr.PD_vcs_position_y), props_MaxDet).write(hdf_processed_info_ptr.PD_vcs_position_y));
      (s18.createDataSet<float32_T>("vcs_position_z", ds::From(hdf_processed_info_ptr.PD_vcs_position_z), props_MaxDet).write(hdf_processed_info_ptr.PD_vcs_position_z));
      (s18.createDataSet<float32_T>("range_rate_compensated", ds::From(hdf_processed_info_ptr.PD_range_rate_compensated), props_MaxDet).write(hdf_processed_info_ptr.PD_range_rate_compensated));
      (s18.createDataSet<float32_T>("vcs_az", ds::From(hdf_processed_info_ptr.PD_vcs_az), props_MaxDet).write(hdf_processed_info_ptr.PD_vcs_az));
      (s18.createDataSet<float32_T>("vcs_el", ds::From(hdf_processed_info_ptr.PD_vcs_el), props_MaxDet).write(hdf_processed_info_ptr.PD_vcs_el));
      (s18.createDataSet<float32_T>("cos_vcs_az", ds::From(hdf_processed_info_ptr.PD_cos_vcs_az), props_MaxDet).write(hdf_processed_info_ptr.PD_cos_vcs_az));
      (s18.createDataSet<float32_T>("sin_vcs_az", ds::From(hdf_processed_info_ptr.PD_sin_vcs_az), props_MaxDet).write(hdf_processed_info_ptr.PD_sin_vcs_az));
      (s18.createDataSet<signed16_T>("next_sorted_idx", ds::From(hdf_processed_info_ptr.PD_next_sorted_idx), props_MaxDet).write(hdf_processed_info_ptr.PD_next_sorted_idx));
      (s18.createDataSet<signed16_T>("prev_sorted_idx", ds::From(hdf_processed_info_ptr.PD_prev_sorted_idx), props_MaxDet).write(hdf_processed_info_ptr.PD_prev_sorted_idx));
      (s18.createDataSet<signed8_T>("motion_status", ds::From(hdf_processed_info_ptr.PD_motion_status), props_MaxDet).write(hdf_processed_info_ptr.PD_motion_status));
      (s18.createDataSet<boolean_T>("f_ok_to_use", ds::From(hdf_processed_info_ptr.PD_f_ok_to_use), props_MaxDet).write(hdf_processed_info_ptr.PD_f_ok_to_use));
      /*Core Info*/
      (s4.createDataSet<unsigned32_T>("time_us", ds::From(hdf_core_info_ptr.time_us), props1D).write(hdf_core_info_ptr.time_us));
      (s4.createDataSet<unsigned64_T>("prev_time_us", ds::From(hdf_core_info_ptr.prev_time_us), props1D).write(hdf_core_info_ptr.prev_time_us));
      (s4.createDataSet<float32_T>("elapsed_time_s", ds::From(hdf_core_info_ptr.elapsed_time_s), props1D).write(hdf_core_info_ptr.elapsed_time_s));
      (s4.createDataSet<unsigned32_T>("cnt_loops", ds::From(hdf_core_info_ptr.cnt_loops), props1D).write(hdf_core_info_ptr.cnt_loops));

      /*
     (s5.createDataSet<unsigned32_T>("vehicle_index", ds::From(hdf_vehicle_info_ptr.vehicle_index)).write(hdf_vehicle_info_ptr.vehicle_index));
     (s5.createDataSet<float32_T>("speed", ds::From(hdf_vehicle_info_ptr.speed)).write(hdf_vehicle_info_ptr.speed));
     (s5.createDataSet<float32_T>("vcs_speed", ds::From(hdf_vehicle_info_ptr.vcs_speed)).write(hdf_vehicle_info_ptr.vcs_speed));
     (s5.createDataSet<float32_T>("acceleration", ds::From(hdf_vehicle_info_ptr.acceleration)).write(hdf_vehicle_info_ptr.acceleration));
     (s5.createDataSet<float32_T>("vcs_lat_acceleration", ds::From(hdf_vehicle_info_ptr.vcs_lat_acceleration)).write(hdf_vehicle_info_ptr.vcs_lat_acceleration));
     (s5.createDataSet<float32_T>("vcs_long_acceleration", ds::From(hdf_vehicle_info_ptr.vcs_long_acceleration)).write(hdf_vehicle_info_ptr.vcs_long_acceleration));
     (s5.createDataSet<float32_T>("yaw_rate_rad", ds::From(hdf_vehicle_info_ptr.yaw_rate_rad)).write(hdf_vehicle_info_ptr.yaw_rate_rad));
     (s5.createDataSet<float32_T>("vcs_sideslip", ds::From(hdf_vehicle_info_ptr.vcs_sideslip)).write(hdf_vehicle_info_ptr.vcs_sideslip));
     (s5.createDataSet<float32_T>("curvature_rear", ds::From(hdf_vehicle_info_ptr.curvature_rear)).write(hdf_vehicle_info_ptr.curvature_rear));
     (s5.createDataSet<float32_T>("dist_rear_axle_to_vcs_m", ds::From(hdf_vehicle_info_ptr.dist_rear_axle_to_vcs_m)).write(hdf_vehicle_info_ptr.dist_rear_axle_to_vcs_m));
     (s5.createDataSet<float32_T>("vehicle_length", ds::From(hdf_vehicle_info_ptr.vehicle_length)).write(hdf_vehicle_info_ptr.vehicle_length));
     (s5.createDataSet<float32_T>("vehicle_width", ds::From(hdf_vehicle_info_ptr.vehicle_width)).write(hdf_vehicle_info_ptr.vehicle_width));
     (s5.createDataSet<float32_T>("rear_cornering_compliance", ds::From(hdf_vehicle_info_ptr.rear_cornering_compliance)).write(hdf_vehicle_info_ptr.rear_cornering_compliance));
     (s5.createDataSet<float32_T>("speed_correction_factor", ds::From(hdf_vehicle_info_ptr.speed_correction_factor)).write(hdf_vehicle_info_ptr.speed_correction_factor));
     (s5.createDataSet<unsigned8_T>("host_type", ds::From(hdf_vehicle_info_ptr.host_type)).write(hdf_vehicle_info_ptr.host_type));
     (s5.createDataSet<boolean_T>("f_trailer_presence_hardware", ds::From(hdf_vehicle_info_ptr.f_trailer_presence_hardware)).write(hdf_vehicle_info_ptr.f_trailer_presence_hardware));
     (s5.createDataSet<unsigned8_T>("speed_qf", ds::From(hdf_vehicle_info_ptr.speed_qf)).write(hdf_vehicle_info_ptr.speed_qf));
     (s5.createDataSet<unsigned8_T>("yaw_rate_qf", ds::From(hdf_vehicle_info_ptr.yaw_rate_qf)).write(hdf_vehicle_info_ptr.yaw_rate_qf));
     (s5.createDataSet<unsigned8_T>("lat_accel_qf", ds::From(hdf_vehicle_info_ptr.lat_accel_qf)).write(hdf_vehicle_info_ptr.lat_accel_qf));
     (s5.createDataSet<unsigned8_T>("long_accel_qf", ds::From(hdf_vehicle_info_ptr.long_accel_qf)).write(hdf_vehicle_info_ptr.long_accel_qf));
     (s5.createDataSet<float32_T>("steer_gear_ratio", ds::From(hdf_vehicle_info_ptr.steer_gear_ratio)).write(hdf_vehicle_info_ptr.steer_gear_ratio));
     (s5.createDataSet<float32_T>("wheelbase_m", ds::From(hdf_vehicle_info_ptr.wheelbase_m)).write(hdf_vehicle_info_ptr.wheelbase_m));
     (s5.createDataSet<float32_T>("understeer_coefficient", ds::From(hdf_vehicle_info_ptr.understeer_coefficient)).write(hdf_vehicle_info_ptr.understeer_coefficient));
     (s5.createDataSet<float32_T>("cog_x", ds::From(hdf_vehicle_info_ptr.cog_x)).write(hdf_vehicle_info_ptr.cog_x));
     (s5.createDataSet<float32_T>("cog_y", ds::From(hdf_vehicle_info_ptr.cog_y)).write(hdf_vehicle_info_ptr.cog_y));
     (s5.createDataSet<float32_T>("front_wheel_radius_m", ds::From(hdf_vehicle_info_ptr.front_wheel_radius_m)).write(hdf_vehicle_info_ptr.front_wheel_radius_m));
     (s5.createDataSet<float32_T>("front_track_width_m", ds::From(hdf_vehicle_info_ptr.front_track_width_m)).write(hdf_vehicle_info_ptr.front_track_width_m));
     (s5.createDataSet<unsigned32_T>("raw_host_signal_latency_ms", ds::From(hdf_vehicle_info_ptr.raw_host_signal_latency_ms)).write(hdf_vehicle_info_ptr.raw_host_signal_latency_ms));
     (s5.createDataSet<boolean_T>("f_enable_internal_reflections_func", ds::From(hdf_vehicle_info_ptr.f_enable_internal_reflections_func)).write(hdf_vehicle_info_ptr.f_enable_internal_reflections_func));
     (s5.createDataSet<boolean_T>("f_enable_internal_reflections_func_trailer", ds::From(hdf_vehicle_info_ptr.f_enable_internal_reflections_func_trailer)).write(hdf_vehicle_info_ptr.f_enable_internal_reflections_func_trailer));

     /* Write Sensor Info*/
      (s12.createDataSet<float32_T>("range_limits", ds::From(hdf_sensor_info_ptr.range_limits), props_Rspp).write(hdf_sensor_info_ptr.range_limits));
      (s12.createDataSet<float32_T>("fov_min_az_rad", ds::From(hdf_sensor_info_ptr.fov_min_az_rad), props_Rspp).write(hdf_sensor_info_ptr.fov_min_az_rad));
      (s12.createDataSet<float32_T>("fov_max_az_rad", ds::From(hdf_sensor_info_ptr.fov_max_az_rad), props_Rspp).write(hdf_sensor_info_ptr.fov_max_az_rad));
      (s12.createDataSet<float32_T>("fov_min_el_rad", ds::From(hdf_sensor_info_ptr.fov_min_el_rad), props_Rspp).write(hdf_sensor_info_ptr.fov_min_el_rad));
      (s12.createDataSet<float32_T>("fov_max_el_rad", ds::From(hdf_sensor_info_ptr.fov_max_el_rad), props_Rspp).write(hdf_sensor_info_ptr.fov_max_el_rad));
      (s12.createDataSet<float32_T>("min_aliaised_range_rate", ds::From(hdf_sensor_info_ptr.min_aliaised_range_rate), props_Rspp).write(hdf_sensor_info_ptr.min_aliaised_range_rate));
      (s12.createDataSet<float32_T>("v_wrapping", ds::From(hdf_sensor_info_ptr.v_wrapping), props_Rspp).write(hdf_sensor_info_ptr.v_wrapping));
      (s12.createDataSet<float32_T>("r_wrapping", ds::From(hdf_sensor_info_ptr.r_wrapping), props_Rspp).write(hdf_sensor_info_ptr.r_wrapping));
      (s12.createDataSet<float32_T>("min_host_vel", ds::From(hdf_sensor_info_ptr.min_host_vel), props2D).write(hdf_sensor_info_ptr.min_host_vel));
      (s12.createDataSet<float32_T>("occurrence_lowerlimit", ds::From(hdf_sensor_info_ptr.occurrence_lowerlimit), props2D).write(hdf_sensor_info_ptr.occurrence_lowerlimit));
      (s12.createDataSet<float32_T>("occurrence_threshold", ds::From(hdf_sensor_info_ptr.occurrence_threshold), props2D).write(hdf_sensor_info_ptr.occurrence_threshold));
      (s12.createDataSet<float32_T>("rcs_tolerance", ds::From(hdf_sensor_info_ptr.rcs_tolerance), props2D).write(hdf_sensor_info_ptr.rcs_tolerance));
      (s12.createDataSet<float32_T>("azimuth_tolerance", ds::From(hdf_sensor_info_ptr.azimuth_tolerance), props2D).write(hdf_sensor_info_ptr.azimuth_tolerance));
      (s12.createDataSet<float32_T>("range_tolerance", ds::From(hdf_sensor_info_ptr.range_tolerance), props2D).write(hdf_sensor_info_ptr.range_tolerance));
      (s12.createDataSet<float32_T>("rcs_max", ds::From(hdf_sensor_info_ptr.rcs_max), props2D).write(hdf_sensor_info_ptr.rcs_max));
      (s12.createDataSet<float32_T>("range_max", ds::From(hdf_sensor_info_ptr.range_max), props2D).write(hdf_sensor_info_ptr.range_max));
      (s12.createDataSet<unsigned16_T>("age_threshold", ds::From(hdf_sensor_info_ptr.age_threshold), props2D).write(hdf_sensor_info_ptr.age_threshold));
      (s12.createDataSet<boolean_T>("f_enable", ds::From(hdf_sensor_info_ptr.f_enable), props2D).write(hdf_sensor_info_ptr.f_enable));
      (s12.createDataSet<int32_T>("useful_FOV", ds::From(hdf_sensor_info_ptr.useful_FOV), props2D).write(hdf_sensor_info_ptr.useful_FOV));
      (s12.createDataSet<float32_T>("vcs_Longitude", ds::From(hdf_sensor_info_ptr.vcs_Longitude), props2D).write(hdf_sensor_info_ptr.vcs_Longitude));
      (s12.createDataSet<float32_T>("vcs_lateral", ds::From(hdf_sensor_info_ptr.vcs_lateral), props2D).write(hdf_sensor_info_ptr.vcs_lateral));
      (s12.createDataSet<float32_T>("vcs_height", ds::From(hdf_sensor_info_ptr.vcs_height), props2D).write(hdf_sensor_info_ptr.vcs_height));
      (s12.createDataSet<float32_T>("vcs_boresight_azimuth_angle", ds::From(hdf_sensor_info_ptr.vcs_boresight_azimuth_angle), props2D).write(hdf_sensor_info_ptr.vcs_boresight_azimuth_angle));
      (s12.createDataSet<float32_T>("vcs_boresight_elevation_angle", ds::From(hdf_sensor_info_ptr.vcs_boresight_elevation_angle), props2D).write(hdf_sensor_info_ptr.vcs_boresight_elevation_angle));
      (s12.createDataSet<unsigned32_T>("id", ds::From(hdf_sensor_info_ptr.id), props2D).write(hdf_sensor_info_ptr.id));
      (s12.createDataSet<signed32_T>("polarity", ds::From(hdf_sensor_info_ptr.polarity), props2D).write(hdf_sensor_info_ptr.polarity));
      (s12.createDataSet<int8_T>("mounting_location", ds::From(hdf_sensor_info_ptr.mounting_location), props2D).write(hdf_sensor_info_ptr.mounting_location));
      (s12.createDataSet<int8_T>("sensor_type", ds::From(hdf_sensor_info_ptr.sensor_type), props2D).write(hdf_sensor_info_ptr.sensor_type));
      (s12.createDataSet<unsigned64_T>("timestamp_us", ds::From(hdf_sensor_info_ptr.timestamp_us), props2D).write(hdf_sensor_info_ptr.timestamp_us));
      (s12.createDataSet<float32_T>("vcs_velocityLat", ds::From(hdf_sensor_info_ptr.vcs_velocityLat), props2D).write(hdf_sensor_info_ptr.vcs_velocityLat));
      (s12.createDataSet<float32_T>("vcs_velocityLong", ds::From(hdf_sensor_info_ptr.vcs_velocityLong), props2D).write(hdf_sensor_info_ptr.vcs_velocityLong));
      (s12.createDataSet<float32_T>("vacs_boresight_az_estimated", ds::From(hdf_sensor_info_ptr.vacs_boresight_az_estimated), props2D).write(hdf_sensor_info_ptr.vacs_boresight_az_estimated));
      (s12.createDataSet<float32_T>("vacs_boresight_el_estimated", ds::From(hdf_sensor_info_ptr.vacs_boresight_el_estimated), props2D).write(hdf_sensor_info_ptr.vacs_boresight_el_estimated));
      (s12.createDataSet<unsigned32_T>("number_of_valid_detections", ds::From(hdf_sensor_info_ptr.number_of_valid_detections), props2D).write(hdf_sensor_info_ptr.number_of_valid_detections));
      (s12.createDataSet<unsigned16_T>("overall_rain_level", ds::From(hdf_sensor_info_ptr.overall_rain_level), props2D).write(hdf_sensor_info_ptr.overall_rain_level));
      (s12.createDataSet<unsigned16_T>("look_index", ds::From(hdf_sensor_info_ptr.look_index), props2D).write(hdf_sensor_info_ptr.look_index));
      (s12.createDataSet<boolean_T>("is_valid", ds::From(hdf_sensor_info_ptr.is_valid), props2D).write(hdf_sensor_info_ptr.is_valid));
      (s12.createDataSet<boolean_T>("f_sensor_fault_detected", ds::From(hdf_sensor_info_ptr.f_sensor_fault_detected), props2D).write(hdf_sensor_info_ptr.f_sensor_fault_detected));
      (s12.createDataSet<int8_T>("look_id", ds::From(hdf_sensor_info_ptr.look_id), props2D).write(hdf_sensor_info_ptr.look_id));

      // Write All objects information

      (s10.createDataSet<unsigned8_T>("version", ds::From(hdf_all_objects_ptr.version), props1D).write(hdf_all_objects_ptr.version));
      (s10.createDataSet<unsigned8_T>("num_elements", ds::From(hdf_all_objects_ptr.num_elements), props1D).write(hdf_all_objects_ptr.num_elements));
      (s10.createDataSet<unsigned8_T>("data_timstamp_us", ds::From(hdf_all_objects_ptr.data_timstamp_us), props1D).write(hdf_all_objects_ptr.data_timstamp_us));
      (s10.createDataSet<unsigned32_T>("unique_id", ds::From(hdf_all_objects_ptr.unique_id), props_ObjLog).write(hdf_all_objects_ptr.unique_id));
      (s10.createDataSet<unsigned8_T>("reference_point", ds::From(hdf_all_objects_ptr.reference_point), props_ObjLog).write(hdf_all_objects_ptr.reference_point));
      (s10.createDataSet<unsigned8_T>("drivable_status_sg", ds::From(hdf_all_objects_ptr.drivable_status_sg), props_ObjLog).write(hdf_all_objects_ptr.drivable_status_sg));
      (s10.createDataSet<unsigned8_T>("drivable_confidence_sg", ds::From(hdf_all_objects_ptr.drivable_confidence_sg), props_ObjLog).write(hdf_all_objects_ptr.drivable_confidence_sg));
      (s10.createDataSet<float32_T>("otg_height", ds::From(hdf_all_objects_ptr.otg_height), props_ObjLog).write(hdf_all_objects_ptr.otg_height));
      (s10.createDataSet<float32_T>("probability_underdrivable_ocg", ds::From(hdf_all_objects_ptr.otg_height), props_ObjLog).write(hdf_all_objects_ptr.probability_underdrivable_ocg));
      (s10.createDataSet<float32_T>("radar_cross_section", ds::From(hdf_all_objects_ptr.radar_cross_section), props_ObjLog).write(hdf_all_objects_ptr.radar_cross_section));
      (s10.createDataSet<unsigned16_T>("num_rr_inlier_dets", ds::From(hdf_all_objects_ptr.num_rr_inlier_dets), props_ObjLog).write(hdf_all_objects_ptr.num_rr_inlier_dets));
      (s10.createDataSet<unsigned16_T>("num_dets_used_in_rr_msmt_update", ds::From(hdf_all_objects_ptr.num_dets_used_in_rr_msmt_update), props_ObjLog).write(hdf_all_objects_ptr.num_dets_used_in_rr_msmt_update));
      (s10.createDataSet<float32_T>("vcs_xposn", ds::From(hdf_all_objects_ptr.vcs_xposn), props_ObjLog).write(hdf_all_objects_ptr.vcs_xposn));
      (s10.createDataSet<float32_T>("vcs_yposn", ds::From(hdf_all_objects_ptr.vcs_yposn), props_ObjLog).write(hdf_all_objects_ptr.vcs_yposn));
      (s10.createDataSet<float32_T>("vcs_xvel", ds::From(hdf_all_objects_ptr.vcs_xvel), props_ObjLog).write(hdf_all_objects_ptr.vcs_xvel));
      (s10.createDataSet<float32_T>("vcs_yvel", ds::From(hdf_all_objects_ptr.vcs_yvel), props_ObjLog).write(hdf_all_objects_ptr.vcs_yvel));
      (s10.createDataSet<float32_T>("vcs_xaccel", ds::From(hdf_all_objects_ptr.vcs_xaccel), props_ObjLog).write(hdf_all_objects_ptr.vcs_xaccel));
      (s10.createDataSet<float32_T>("vcs_yaccel", ds::From(hdf_all_objects_ptr.vcs_yaccel), props_ObjLog).write(hdf_all_objects_ptr.vcs_yaccel));
      (s10.createDataSet<float32_T>("vcs_heading", ds::From(hdf_all_objects_ptr.vcs_heading), props_ObjLog).write(hdf_all_objects_ptr.vcs_heading));
      (s10.createDataSet<float32_T>("vcs_pointing", ds::From(hdf_all_objects_ptr.vcs_pointing), props_ObjLog).write(hdf_all_objects_ptr.vcs_pointing));
      (s10.createDataSet<float32_T>("speed", ds::From(hdf_all_objects_ptr.speed), props_ObjLog).write(hdf_all_objects_ptr.speed));
      (s10.createDataSet<float32_T>("curvature", ds::From(hdf_all_objects_ptr.curvature), props_ObjLog).write(hdf_all_objects_ptr.curvature));
      (s10.createDataSet<float32_T>("tang_accel", ds::From(hdf_all_objects_ptr.tang_accel), props_ObjLog).write(hdf_all_objects_ptr.tang_accel));
      (s10.createDataSet<float32_T>("state_variance", ds::From(hdf_all_objects_ptr.state_variance), props_AllobjVar).write(hdf_all_objects_ptr.state_variance));
      (s10.createDataSet<float32_T>("supplemental_state_covariance", ds::From(hdf_all_objects_ptr.supplemental_state_covariance), props_AllobjCovVar).write(hdf_all_objects_ptr.supplemental_state_covariance));
      (s10.createDataSet<float32_T>("time_since_measurement", ds::From(hdf_all_objects_ptr.time_since_measurement), props_ObjLog).write(hdf_all_objects_ptr.time_since_measurement));
      (s10.createDataSet<float32_T>("time_since_cluster_created", ds::From(hdf_all_objects_ptr.time_since_cluster_created), props_ObjLog).write(hdf_all_objects_ptr.time_since_cluster_created));
      (s10.createDataSet<float32_T>("time_since_track_updated", ds::From(hdf_all_objects_ptr.time_since_track_updated), props_ObjLog).write(hdf_all_objects_ptr.time_since_track_updated));
      (s10.createDataSet<float32_T>("len1", ds::From(hdf_all_objects_ptr.len1), props_ObjLog).write(hdf_all_objects_ptr.len1));
      (s10.createDataSet<float32_T>("len2", ds::From(hdf_all_objects_ptr.len2), props_ObjLog).write(hdf_all_objects_ptr.len2));
      (s10.createDataSet<float32_T>("wid1", ds::From(hdf_all_objects_ptr.wid1), props_ObjLog).write(hdf_all_objects_ptr.wid1));
      (s10.createDataSet<float32_T>("wid2", ds::From(hdf_all_objects_ptr.wid2), props_ObjLog).write(hdf_all_objects_ptr.wid2));
      (s10.createDataSet<float32_T>("confidenceLevel", ds::From(hdf_all_objects_ptr.confidenceLevel), props_ObjLog).write(hdf_all_objects_ptr.confidenceLevel));
      (s10.createDataSet<float32_T>("time_since_stage_start", ds::From(hdf_all_objects_ptr.time_since_stage_start), props_ObjLog).write(hdf_all_objects_ptr.time_since_stage_start));
      (s10.createDataSet<float32_T>("existence_probability", ds::From(hdf_all_objects_ptr.existence_probability), props_ObjLog).write(hdf_all_objects_ptr.existence_probability));
      (s10.createDataSet<float32_T>("accuracy_length", ds::From(hdf_all_objects_ptr.accuracy_length), props_ObjLog).write(hdf_all_objects_ptr.accuracy_length));
      (s10.createDataSet<float32_T>("accuracy_width", ds::From(hdf_all_objects_ptr.accuracy_width), props_ObjLog).write(hdf_all_objects_ptr.accuracy_width));
      (s10.createDataSet<float32_T>("probability_pedestrian", ds::From(hdf_all_objects_ptr.probability_pedestrian), props_ObjLog).write(hdf_all_objects_ptr.probability_pedestrian));
      (s10.createDataSet<float32_T>("probability_car", ds::From(hdf_all_objects_ptr.probability_car), props_ObjLog).write(hdf_all_objects_ptr.probability_car));
      (s10.createDataSet<float32_T>("probability_motorcycle", ds::From(hdf_all_objects_ptr.probability_motorcycle), props_ObjLog).write(hdf_all_objects_ptr.probability_motorcycle));
      (s10.createDataSet<float32_T>("probability_bicycle", ds::From(hdf_all_objects_ptr.probability_bicycle), props_ObjLog).write(hdf_all_objects_ptr.probability_bicycle));
      (s10.createDataSet<float32_T>("probability_truck", ds::From(hdf_all_objects_ptr.probability_truck), props_ObjLog).write(hdf_all_objects_ptr.probability_truck));
      (s10.createDataSet<float32_T>("probability_undet", ds::From(hdf_all_objects_ptr.probability_undet), props_ObjLog).write(hdf_all_objects_ptr.probability_undet));
      (s10.createDataSet<unsigned16_T>("trkID", ds::From(hdf_all_objects_ptr.trkID), props_ObjLog).write(hdf_all_objects_ptr.trkID));
      (s10.createDataSet<unsigned16_T>("ndets", ds::From(hdf_all_objects_ptr.ndets), props_ObjLog).write(hdf_all_objects_ptr.ndets));
      (s10.createDataSet<unsigned16_T>("num_reduced_dets", ds::From(hdf_all_objects_ptr.num_reduced_dets), props_ObjLog).write(hdf_all_objects_ptr.num_reduced_dets));
      (s10.createDataSet<unsigned8_T>("reducedID", ds::From(hdf_all_objects_ptr.reducedID), props_ObjLog).write(hdf_all_objects_ptr.reducedID));
      (s10.createDataSet<unsigned8_T>("trk_fltr_type", ds::From(hdf_all_objects_ptr.trk_fltr_type), props_ObjLog).write(hdf_all_objects_ptr.trk_fltr_type));
      (s10.createDataSet<unsigned8_T>("status", ds::From(hdf_all_objects_ptr.status), props_ObjLog).write(hdf_all_objects_ptr.status));
      (s10.createDataSet<unsigned8_T>("reducedStatus", ds::From(hdf_all_objects_ptr.reducedStatus), props_ObjLog).write(hdf_all_objects_ptr.reducedStatus));
      (s10.createDataSet<unsigned8_T>("init_scheme", ds::From(hdf_all_objects_ptr.init_scheme), props_ObjLog).write(hdf_all_objects_ptr.init_scheme));
      (s10.createDataSet<unsigned8_T>("object_class", ds::From(hdf_all_objects_ptr.object_class), props_ObjLog).write(hdf_all_objects_ptr.object_class));
      (s10.createDataSet<unsigned8_T>("f_crossing", ds::From(hdf_all_objects_ptr.f_crossing), props_ObjLog).write(hdf_all_objects_ptr.f_crossing));
      (s10.createDataSet<unsigned8_T>("f_moving", ds::From(hdf_all_objects_ptr.f_moving), props_ObjLog).write(hdf_all_objects_ptr.f_moving));
      (s10.createDataSet<unsigned8_T>("f_moveable", ds::From(hdf_all_objects_ptr.f_moveable), props_ObjLog).write(hdf_all_objects_ptr.f_moveable));
      (s10.createDataSet<unsigned8_T>("f_oncoming", ds::From(hdf_all_objects_ptr.f_oncoming), props_ObjLog).write(hdf_all_objects_ptr.f_oncoming));
      (s10.createDataSet<unsigned8_T>("f_vehicular_trk", ds::From(hdf_all_objects_ptr.f_vehicular_trk), props_ObjLog).write(hdf_all_objects_ptr.f_vehicular_trk));
      (s10.createDataSet<unsigned8_T>("f_onguardrail", ds::From(hdf_all_objects_ptr.f_onguardrail), props_ObjLog).write(hdf_all_objects_ptr.f_onguardrail));
      (s10.createDataSet<unsigned8_T>("f_fast_moving", ds::From(hdf_all_objects_ptr.f_fast_moving), props_ObjLog).write(hdf_all_objects_ptr.f_fast_moving));
      (s10.createDataSet<unsigned8_T>("underdrivable_status", ds::From(hdf_all_objects_ptr.underdrivable_status), props_ObjLog).write(hdf_all_objects_ptr.underdrivable_status));

      // Write processed detection information

      /* write VSE output information*/

      (s9.createDataSet<unsigned64_T>("timestamp_us", ds::From(hdf_vse_output_ptr.timestamp_us), props1D).write(hdf_vse_output_ptr.timestamp_us));
      (s9.createDataSet<float32_T>("raw_speed_mps", ds::From(hdf_vse_output_ptr.raw_speed_mps), props1D).write(hdf_vse_output_ptr.raw_speed_mps));
      (s9.createDataSet<float32_T>("speed_compensation_factor", ds::From(hdf_vse_output_ptr.speed_compensation_factor), props1D).write(hdf_vse_output_ptr.speed_compensation_factor));
      (s9.createDataSet<float32_T>("filt_veh_speed_over_ground", ds::From(hdf_vse_output_ptr.filt_veh_speed_over_ground), props1D).write(hdf_vse_output_ptr.filt_veh_speed_over_ground));
      (s9.createDataSet<float32_T>("raw_lat_accel", ds::From(hdf_vse_output_ptr.raw_lat_accel), props1D).write(hdf_vse_output_ptr.raw_lat_accel));
      (s9.createDataSet<float32_T>("raw_long_accel", ds::From(hdf_vse_output_ptr.raw_long_accel), props1D).write(hdf_vse_output_ptr.raw_long_accel));
      (s9.createDataSet<float32_T>("raw_yaw_rate_rps", ds::From(hdf_vse_output_ptr.raw_yaw_rate_rps), props1D).write(hdf_vse_output_ptr.raw_yaw_rate_rps));
      (s9.createDataSet<float32_T>("raw_steering_angle_deg", ds::From(hdf_vse_output_ptr.raw_steering_angle_deg), props1D).write(hdf_vse_output_ptr.raw_steering_angle_deg));
      (s9.createDataSet<float32_T>("road_wheel_angle_deg", ds::From(hdf_vse_output_ptr.road_wheel_angle_deg), props1D).write(hdf_vse_output_ptr.road_wheel_angle_deg));
      (s9.createDataSet<float32_T>("yaw_rate_sa", ds::From(hdf_vse_output_ptr.yaw_rate_sa), props1D).write(hdf_vse_output_ptr.yaw_rate_sa));
      (s9.createDataSet<float32_T>("yaw_rate_raw_bias", ds::From(hdf_vse_output_ptr.yaw_rate_raw_bias), props1D).write(hdf_vse_output_ptr.yaw_rate_raw_bias));
      (s9.createDataSet<float32_T>("comp_yaw_rate_unfiltered", ds::From(hdf_vse_output_ptr.comp_yaw_rate_unfiltered), props1D).write(hdf_vse_output_ptr.comp_yaw_rate_unfiltered));
      (s9.createDataSet<float32_T>("comp_yaw_rate_filtered", ds::From(hdf_vse_output_ptr.comp_yaw_rate_filtered), props1D).write(hdf_vse_output_ptr.comp_yaw_rate_filtered));
      (s9.createDataSet<float32_T>("curvature_rear_axle", ds::From(hdf_vse_output_ptr.curvature_rear_axle), props1D).write(hdf_vse_output_ptr.curvature_rear_axle));
      (s9.createDataSet<float32_T>("sideslip_rear_axle", ds::From(hdf_vse_output_ptr.sideslip_rear_axle), props1D).write(hdf_vse_output_ptr.sideslip_rear_axle));
      (s9.createDataSet<float32_T>("vcs_sideslip", ds::From(hdf_vse_output_ptr.vcs_sideslip), props1D).write(hdf_vse_output_ptr.vcs_sideslip));
      (s9.createDataSet<float32_T>("vcs_long_velocity", ds::From(hdf_vse_output_ptr.vcs_long_velocity), props1D).write(hdf_vse_output_ptr.vcs_long_velocity));
      (s9.createDataSet<float32_T>("vcs_lat_velocity", ds::From(hdf_vse_output_ptr.vcs_lat_velocity), props1D).write(hdf_vse_output_ptr.vcs_lat_velocity));
      (s9.createDataSet<float32_T>("sensor_sideslip", ds::From(hdf_vse_output_ptr.sensor_sideslip), props1D).write(hdf_vse_output_ptr.sensor_sideslip));
      (s9.createDataSet<float32_T>("sensor_long_velocity", ds::From(hdf_vse_output_ptr.sensor_long_velocity), props1D).write(hdf_vse_output_ptr.sensor_long_velocity));
      (s9.createDataSet<float32_T>("sensor_lat_velocity", ds::From(hdf_vse_output_ptr.sensor_lat_velocity), props1D).write(hdf_vse_output_ptr.sensor_lat_velocity));
      (s9.createDataSet<float32_T>("k_dist_rear_axle_to_vcs", ds::From(hdf_vse_output_ptr.k_dist_rear_axle_to_vcs), props1D).write(hdf_vse_output_ptr.k_dist_rear_axle_to_vcs));
      (s9.createDataSet<float32_T>("vcs_lat_accel", ds::From(hdf_vse_output_ptr.vcs_lat_accel), props1D).write(hdf_vse_output_ptr.vcs_lat_accel));
      (s9.createDataSet<float32_T>("vcs_long_accel", ds::From(hdf_vse_output_ptr.vcs_long_accel), props1D).write(hdf_vse_output_ptr.vcs_long_accel));
      (s9.createDataSet<float32_T>("accel_rear_axle", ds::From(hdf_vse_output_ptr.accel_rear_axle), props1D).write(hdf_vse_output_ptr.accel_rear_axle));
      (s9.createDataSet<float32_T>("signed_filt_veh_speed_over_ground", ds::From(hdf_vse_output_ptr.signed_filt_veh_speed_over_ground), props1D).write(hdf_vse_output_ptr.signed_filt_veh_speed_over_ground));
      (s9.createDataSet<unsigned32_T>("veh_index", ds::From(hdf_vse_output_ptr.veh_index), props1D).write(hdf_vse_output_ptr.veh_index));
      (s9.createDataSet<unsigned8_T>("raw_speed_qf", ds::From(hdf_vse_output_ptr.raw_speed_qf), props1D).write(hdf_vse_output_ptr.raw_speed_qf));
      (s9.createDataSet<unsigned8_T>("speed_compensation_factor_qf", ds::From(hdf_vse_output_ptr.speed_compensation_factor_qf), props1D).write(hdf_vse_output_ptr.speed_compensation_factor_qf));
      (s9.createDataSet<unsigned8_T>("filt_veh_speed_over_ground_qf", ds::From(hdf_vse_output_ptr.filt_veh_speed_over_ground_qf), props1D).write(hdf_vse_output_ptr.filt_veh_speed_over_ground_qf));
      (s9.createDataSet<unsigned8_T>("raw_lat_accel_qf", ds::From(hdf_vse_output_ptr.raw_lat_accel_qf), props1D).write(hdf_vse_output_ptr.raw_lat_accel_qf));
      (s9.createDataSet<unsigned8_T>("raw_long_accel_qf", ds::From(hdf_vse_output_ptr.raw_long_accel_qf), props1D).write(hdf_vse_output_ptr.raw_long_accel_qf));
      (s9.createDataSet<unsigned8_T>("raw_yaw_rate_qf", ds::From(hdf_vse_output_ptr.raw_yaw_rate_qf), props1D).write(hdf_vse_output_ptr.raw_yaw_rate_qf));
      (s9.createDataSet<unsigned8_T>("raw_steering_angle_qf", ds::From(hdf_vse_output_ptr.raw_steering_angle_qf), props1D).write(hdf_vse_output_ptr.raw_steering_angle_qf));
      (s9.createDataSet<unsigned8_T>("road_wheel_angle_qf", ds::From(hdf_vse_output_ptr.road_wheel_angle_qf), props1D).write(hdf_vse_output_ptr.road_wheel_angle_qf));
      (s9.createDataSet<unsigned8_T>("yaw_rate_sa_qf", ds::From(hdf_vse_output_ptr.yaw_rate_sa_qf), props1D).write(hdf_vse_output_ptr.yaw_rate_sa_qf));
      (s9.createDataSet<unsigned8_T>("yaw_rate_bias_qf", ds::From(hdf_vse_output_ptr.yaw_rate_bias_qf), props1D).write(hdf_vse_output_ptr.yaw_rate_bias_qf));
      (s9.createDataSet<unsigned8_T>("comp_yaw_rate_qf", ds::From(hdf_vse_output_ptr.comp_yaw_rate_qf), props1D).write(hdf_vse_output_ptr.comp_yaw_rate_qf));
      (s9.createDataSet<unsigned8_T>("stationary", ds::From(hdf_vse_output_ptr.stationary), props1D).write(hdf_vse_output_ptr.stationary));
      (s9.createDataSet<unsigned8_T>("vcs_lat_accel_qf", ds::From(hdf_vse_output_ptr.vcs_lat_accel_qf), props1D).write(hdf_vse_output_ptr.vcs_lat_accel_qf));
      (s9.createDataSet<unsigned8_T>("vcs_long_accel_qf", ds::From(hdf_vse_output_ptr.vcs_long_accel_qf), props1D).write(hdf_vse_output_ptr.vcs_long_accel_qf));

      // Write ROT Objects

      (s11.createDataSet<unsigned64_T>("rot_object_list_timestamp", ds::From(hdf_ROT_info_ptr.rot_object_list_timestamp), props1D).write(hdf_ROT_info_ptr.rot_object_list_timestamp));
      (s11.createDataSet<unsigned64_T>("tracker_start_timestamp", ds::From(hdf_ROT_info_ptr.tracker_start_timestamp), props1D).write(hdf_ROT_info_ptr.tracker_start_timestamp));
      (s11.createDataSet<float32_T>("tracker_elapsed_time", ds::From(hdf_ROT_info_ptr.tracker_elapsed_time), props1D).write(hdf_ROT_info_ptr.tracker_elapsed_time));
      (s11.createDataSet<unsigned32_T>("tracker_index", ds::From(hdf_ROT_info_ptr.tracker_index), props1D).write(hdf_ROT_info_ptr.tracker_index));
      (s11.createDataSet<unsigned16_T>("number_of_objects", ds::From(hdf_ROT_info_ptr.number_of_objects), props1D).write(hdf_ROT_info_ptr.number_of_objects));
      (s11.createDataSet<float32_T>("vcs_x_posn", ds::From(hdf_ROT_info_ptr.vcs_x_posn), props_ROTObj).write(hdf_ROT_info_ptr.vcs_x_posn));
      (s11.createDataSet<float32_T>("vcs_y_posn", ds::From(hdf_ROT_info_ptr.vcs_y_posn), props_ROTObj).write(hdf_ROT_info_ptr.vcs_y_posn));
      (s11.createDataSet<float32_T>("vcs_x_vel", ds::From(hdf_ROT_info_ptr.vcs_x_vel), props_ROTObj).write(hdf_ROT_info_ptr.vcs_x_vel));
      (s11.createDataSet<float32_T>("vcs_y_vel", ds::From(hdf_ROT_info_ptr.vcs_y_vel), props_ROTObj).write(hdf_ROT_info_ptr.vcs_y_vel));
      (s11.createDataSet<float32_T>("vcs_x_acc", ds::From(hdf_ROT_info_ptr.vcs_x_acc), props_ROTObj).write(hdf_ROT_info_ptr.vcs_x_acc));
      (s11.createDataSet<float32_T>("vcs_y_acc", ds::From(hdf_ROT_info_ptr.vcs_y_acc), props_ROTObj).write(hdf_ROT_info_ptr.vcs_y_acc));
      (s11.createDataSet<float32_T>("vcs_heading", ds::From(hdf_ROT_info_ptr.vcs_heading), props_ROTObj).write(hdf_ROT_info_ptr.vcs_heading));
      (s11.createDataSet<float32_T>("vcs_pointing", ds::From(hdf_ROT_info_ptr.vcs_pointing), props_ROTObj).write(hdf_ROT_info_ptr.vcs_pointing));
      (s11.createDataSet<float32_T>("vcs_state_variance", ds::From(hdf_ROT_info_ptr.vcs_state_variance)).write(hdf_ROT_info_ptr.vcs_state_variance));
      (s11.createDataSet<float32_T>("vcs_supplemental_state_covariance", ds::From(hdf_ROT_info_ptr.vcs_supplemental_state_covariance)).write(hdf_ROT_info_ptr.vcs_supplemental_state_covariance));
      (s11.createDataSet<float32_T>("vcs_curvature", ds::From(hdf_ROT_info_ptr.vcs_curvature), props_ROTObj).write(hdf_ROT_info_ptr.vcs_curvature));
      (s11.createDataSet<float32_T>("iso_x_posn", ds::From(hdf_ROT_info_ptr.iso_x_posn), props_ROTObj).write(hdf_ROT_info_ptr.iso_x_posn));
      (s11.createDataSet<float32_T>("iso_y_posn", ds::From(hdf_ROT_info_ptr.iso_y_posn), props_ROTObj).write(hdf_ROT_info_ptr.iso_y_posn));
      (s11.createDataSet<float32_T>("iso_x_vel", ds::From(hdf_ROT_info_ptr.iso_x_vel), props_ROTObj).write(hdf_ROT_info_ptr.iso_x_vel));
      (s11.createDataSet<float32_T>("iso_relative_x_vel", ds::From(hdf_ROT_info_ptr.iso_relative_x_vel), props_ROTObj).write(hdf_ROT_info_ptr.iso_relative_x_vel));
      (s11.createDataSet<float32_T>("iso_y_vel", ds::From(hdf_ROT_info_ptr.iso_y_vel), props_ROTObj).write(hdf_ROT_info_ptr.iso_y_vel));
      (s11.createDataSet<float32_T>("iso_relative_y_vel", ds::From(hdf_ROT_info_ptr.iso_relative_y_vel), props_ROTObj).write(hdf_ROT_info_ptr.iso_relative_y_vel));
      (s11.createDataSet<float32_T>("iso_x_acc", ds::From(hdf_ROT_info_ptr.iso_x_acc), props_ROTObj).write(hdf_ROT_info_ptr.iso_x_acc));
      (s11.createDataSet<float32_T>("iso_relative_x_acc", ds::From(hdf_ROT_info_ptr.iso_relative_x_acc), props_ROTObj).write(hdf_ROT_info_ptr.iso_relative_x_acc));
      (s11.createDataSet<float32_T>("iso_y_acc", ds::From(hdf_ROT_info_ptr.iso_y_acc), props_ROTObj).write(hdf_ROT_info_ptr.iso_y_acc));
      (s11.createDataSet<float32_T>("iso_relative_y_acc", ds::From(hdf_ROT_info_ptr.iso_relative_y_acc), props_ROTObj).write(hdf_ROT_info_ptr.iso_relative_y_acc));
      (s11.createDataSet<float32_T>("iso_orientation", ds::From(hdf_ROT_info_ptr.iso_orientation), props_ROTObj).write(hdf_ROT_info_ptr.iso_orientation));
      (s11.createDataSet<float32_T>("iso_orientation_var", ds::From(hdf_ROT_info_ptr.iso_orientation_var), props_ROTObj).write(hdf_ROT_info_ptr.iso_orientation_var));
      (s11.createDataSet<float32_T>("iso_orientation_rate", ds::From(hdf_ROT_info_ptr.iso_orientation_rate), props_ROTObj).write(hdf_ROT_info_ptr.iso_orientation_rate));
      (s11.createDataSet<float32_T>("iso_orientation_rate_var", ds::From(hdf_ROT_info_ptr.iso_orientation_rate_var), props_ROTObj).write(hdf_ROT_info_ptr.iso_orientation_rate_var));
      (s11.createDataSet<float32_T>("iso_x_posn_var", ds::From(hdf_ROT_info_ptr.iso_x_posn_var), props_ROTObj).write(hdf_ROT_info_ptr.iso_x_posn_var));
      (s11.createDataSet<float32_T>("iso_y_posn_var", ds::From(hdf_ROT_info_ptr.iso_y_posn_var), props_ROTObj).write(hdf_ROT_info_ptr.iso_y_posn_var));
      (s11.createDataSet<float32_T>("iso_xy_posn_cov", ds::From(hdf_ROT_info_ptr.iso_xy_posn_cov), props_ROTObj).write(hdf_ROT_info_ptr.iso_xy_posn_cov));
      (s11.createDataSet<float32_T>("iso_x_vel_var", ds::From(hdf_ROT_info_ptr.iso_x_vel_var), props_ROTObj).write(hdf_ROT_info_ptr.iso_x_vel_var));
      (s11.createDataSet<float32_T>("iso_y_vel_var", ds::From(hdf_ROT_info_ptr.iso_y_vel_var), props_ROTObj).write(hdf_ROT_info_ptr.iso_y_vel_var));
      (s11.createDataSet<float32_T>("iso_xy_vel_cov", ds::From(hdf_ROT_info_ptr.iso_xy_vel_cov), props_ROTObj).write(hdf_ROT_info_ptr.iso_xy_vel_cov));
      (s11.createDataSet<float32_T>("iso_x_acc_var", ds::From(hdf_ROT_info_ptr.iso_x_acc_var), props_ROTObj).write(hdf_ROT_info_ptr.iso_x_acc_var));
      (s11.createDataSet<float32_T>("iso_y_acc_var", ds::From(hdf_ROT_info_ptr.iso_y_acc_var), props_ROTObj).write(hdf_ROT_info_ptr.iso_y_acc_var));
      (s11.createDataSet<float32_T>("iso_xy_acc_cov", ds::From(hdf_ROT_info_ptr.iso_xy_acc_cov), props_ROTObj).write(hdf_ROT_info_ptr.iso_xy_acc_cov));
      (s11.createDataSet<float32_T>("speed", ds::From(hdf_ROT_info_ptr.speed), props_ROTObj).write(hdf_ROT_info_ptr.speed));
      (s11.createDataSet<float32_T>("tang_accel", ds::From(hdf_ROT_info_ptr.tang_accel), props_ROTObj).write(hdf_ROT_info_ptr.tang_accel));
      (s11.createDataSet<float32_T>("length", ds::From(hdf_ROT_info_ptr.length), props_ROTObj).write(hdf_ROT_info_ptr.length));
      (s11.createDataSet<float32_T>("length_var", ds::From(hdf_ROT_info_ptr.length_var), props_ROTObj).write(hdf_ROT_info_ptr.length_var));
      (s11.createDataSet<float32_T>("width", ds::From(hdf_ROT_info_ptr.width), props_ROTObj).write(hdf_ROT_info_ptr.width));
      (s11.createDataSet<float32_T>("width_var", ds::From(hdf_ROT_info_ptr.width_var), props_ROTObj).write(hdf_ROT_info_ptr.width_var));
      (s11.createDataSet<float32_T>("time_since_created", ds::From(hdf_ROT_info_ptr.time_since_created), props_ROTObj).write(hdf_ROT_info_ptr.time_since_created));
      (s11.createDataSet<float32_T>("time_since_published", ds::From(hdf_ROT_info_ptr.time_since_published), props_ROTObj).write(hdf_ROT_info_ptr.time_since_published));
      (s11.createDataSet<float32_T>("time_since_stage_start", ds::From(hdf_ROT_info_ptr.time_since_stage_start), props_ROTObj).write(hdf_ROT_info_ptr.time_since_stage_start));
      (s11.createDataSet<float32_T>("existence_probability", ds::From(hdf_ROT_info_ptr.existence_probability), props_ROTObj).write(hdf_ROT_info_ptr.existence_probability));
      (s11.createDataSet<float32_T>("mirror_prob", ds::From(hdf_ROT_info_ptr.mirror_prob), props_ROTObj).write(hdf_ROT_info_ptr.mirror_prob));
      (s11.createDataSet<float32_T>("radar_cross_section", ds::From(hdf_ROT_info_ptr.radar_cross_section), props_ROTObj).write(hdf_ROT_info_ptr.radar_cross_section));
      (s11.createDataSet<float32_T>("otg_height", ds::From(hdf_ROT_info_ptr.otg_height), props_ROTObj).write(hdf_ROT_info_ptr.otg_height));
      (s11.createDataSet<float32_T>("confidence_level", ds::From(hdf_ROT_info_ptr.confidence_level), props_ROTObj).write(hdf_ROT_info_ptr.confidence_level));
      (s11.createDataSet<float32_T>("probability_pedestrian", ds::From(hdf_ROT_info_ptr.probability_pedestrian), props_ROTObj).write(hdf_ROT_info_ptr.probability_pedestrian));
      (s11.createDataSet<float32_T>("probability_car", ds::From(hdf_ROT_info_ptr.probability_car), props_ROTObj).write(hdf_ROT_info_ptr.probability_car));
      (s11.createDataSet<float32_T>("probability_motorcycle", ds::From(hdf_ROT_info_ptr.probability_motorcycle), props_ROTObj).write(hdf_ROT_info_ptr.probability_motorcycle));
      (s11.createDataSet<float32_T>("probability_bicycle", ds::From(hdf_ROT_info_ptr.probability_bicycle), props_ROTObj).write(hdf_ROT_info_ptr.probability_bicycle));
      (s11.createDataSet<float32_T>("probability_truck", ds::From(hdf_ROT_info_ptr.probability_truck), props_ROTObj).write(hdf_ROT_info_ptr.probability_truck));
      (s11.createDataSet<float32_T>("probability_undet", ds::From(hdf_ROT_info_ptr.probability_undet), props_ROTObj).write(hdf_ROT_info_ptr.probability_undet));
      (s11.createDataSet<float32_T>("probability_underdrivable_ocg", ds::From(hdf_ROT_info_ptr.probability_underdrivable_ocg), props_ROTObj).write(hdf_ROT_info_ptr.probability_underdrivable_ocg));
      (s11.createDataSet<float32_T>("movable_prob", ds::From(hdf_ROT_info_ptr.movable_prob), props_ROTObj).write(hdf_ROT_info_ptr.movable_prob));
      (s11.createDataSet<signed32_T>("id", ds::From(hdf_ROT_info_ptr.id), props_ROTObj).write(hdf_ROT_info_ptr.id));
      (s11.createDataSet<unsigned32_T>("unique_id", ds::From(hdf_ROT_info_ptr.unique_id), props_ROTObj).write(hdf_ROT_info_ptr.unique_id));
      (s11.createDataSet<unsigned32_T>("ndets", ds::From(hdf_ROT_info_ptr.ndets), props_ROTObj).write(hdf_ROT_info_ptr.ndets));
      (s11.createDataSet<unsigned32_T>("num_dets_used_in_rr_msmt_update", ds::From(hdf_ROT_info_ptr.num_dets_used_in_rr_msmt_update), props_ROTObj).write(hdf_ROT_info_ptr.num_dets_used_in_rr_msmt_update));
      (s11.createDataSet<unsigned16_T>("sensor_src", ds::From(hdf_ROT_info_ptr.sensor_src), props_ROTObj).write(hdf_ROT_info_ptr.sensor_src));
      (s11.createDataSet<unsigned8_T>("reference_point", ds::From(hdf_ROT_info_ptr.reference_point), props_ROTObj).write(hdf_ROT_info_ptr.reference_point));
      (s11.createDataSet<unsigned8_T>("object_status", ds::From(hdf_ROT_info_ptr.object_status), props_ROTObj).write(hdf_ROT_info_ptr.object_status));
      (s11.createDataSet<unsigned8_T>("object_class", ds::From(hdf_ROT_info_ptr.object_class), props_ROTObj).write(hdf_ROT_info_ptr.object_class));
      (s11.createDataSet<unsigned8_T>("movement_status", ds::From(hdf_ROT_info_ptr.movement_status), props_ROTObj).write(hdf_ROT_info_ptr.movement_status));
      (s11.createDataSet<unsigned8_T>("occlusion_status", ds::From(hdf_ROT_info_ptr.occlusion_status), props_ROTObj).write(hdf_ROT_info_ptr.occlusion_status));
      (s11.createDataSet<unsigned8_T>("underdrivable_status_ocg", ds::From(hdf_ROT_info_ptr.underdrivable_status_ocg), props_ROTObj).write(hdf_ROT_info_ptr.underdrivable_status_ocg));
      (s11.createDataSet<unsigned8_T>("drivable_status_sg", ds::From(hdf_ROT_info_ptr.drivable_status_sg), props_ROTObj).write(hdf_ROT_info_ptr.drivable_status_sg));
      (s11.createDataSet<unsigned8_T>("drivable_confidence_sg", ds::From(hdf_ROT_info_ptr.drivable_confidence_sg), props_ROTObj).write(hdf_ROT_info_ptr.drivable_confidence_sg));
      (s11.createDataSet<unsigned8_T>("f_onguardrail", ds::From(hdf_ROT_info_ptr.f_onguardrail), props_ROTObj).write(hdf_ROT_info_ptr.f_onguardrail));
      (s11.createDataSet<unsigned8_T>("trk_fltr_type", ds::From(hdf_ROT_info_ptr.trk_fltr_type), props_ROTObj).write(hdf_ROT_info_ptr.trk_fltr_type));

      auto s30 = file.createGroup("04_OLP", gcpl);
      // auto s30 = g4.createGroup("OLP");
      s30.createDataSet<uint8_t>("id", ds::From(hdf_olp_output_ptr.id), props_OLP).write(hdf_olp_output_ptr.id);
      s30.createDataSet<uint32_t>("unique_id", ds::From(hdf_olp_output_ptr.unique_id), props_OLP).write(hdf_olp_output_ptr.unique_id);
      s30.createDataSet<std::string>("status", ds::From(hdf_olp_output_ptr.status), props_OLP).write(hdf_olp_output_ptr.status);
      s30.createDataSet<uint8_t>("age", ds::From(hdf_olp_output_ptr.age), props_OLP).write(hdf_olp_output_ptr.age);
      s30.createDataSet<uint8_t>("stage_age", ds::From(hdf_olp_output_ptr.stage_age), props_OLP).write(hdf_olp_output_ptr.stage_age);
      s30.createDataSet<uint8_t>("fbk_stage_age", ds::From(hdf_olp_output_ptr.fbk_stage_age), props_OLP).write(hdf_olp_output_ptr.fbk_stage_age);
      s30.createDataSet<float32_t>("existence_probability", ds::From(hdf_olp_output_ptr.existence_probability), props_OLP).write(hdf_olp_output_ptr.existence_probability);
      s30.createDataSet<float32_t>("speed", ds::From(hdf_olp_output_ptr.speed), props_OLP).write(hdf_olp_output_ptr.speed);
      s30.createDataSet<float32_t>("vcs_pos_x", ds::From(hdf_olp_output_ptr.vcs_pos_x), props_OLP).write(hdf_olp_output_ptr.vcs_pos_x);
      s30.createDataSet<float32_t>("vcs_pos_y", ds::From(hdf_olp_output_ptr.vcs_pos_y), props_OLP).write(hdf_olp_output_ptr.vcs_pos_y);
      s30.createDataSet<float32_t>("vcs_vel_x", ds::From(hdf_olp_output_ptr.vcs_vel_x), props_OLP).write(hdf_olp_output_ptr.vcs_vel_x);
      s30.createDataSet<float32_t>("vcs_vel_y", ds::From(hdf_olp_output_ptr.vcs_vel_y), props_OLP).write(hdf_olp_output_ptr.vcs_vel_y);
      s30.createDataSet<float32_t>("vcs_vel_rel_x", ds::From(hdf_olp_output_ptr.vcs_vel_rel_x), props_OLP).write(hdf_olp_output_ptr.vcs_vel_rel_x);
      s30.createDataSet<float32_t>("vcs_vel_rel_y", ds::From(hdf_olp_output_ptr.vcs_vel_rel_y), props_OLP).write(hdf_olp_output_ptr.vcs_vel_rel_y);
      s30.createDataSet<float32_t>("vcs_accel_x", ds::From(hdf_olp_output_ptr.vcs_accel_x), props_OLP).write(hdf_olp_output_ptr.vcs_accel_x);
      s30.createDataSet<float32_t>("vcs_accel_y", ds::From(hdf_olp_output_ptr.vcs_accel_y), props_OLP).write(hdf_olp_output_ptr.vcs_accel_y);
      s30.createDataSet<float32_t>("vcs_heading", ds::From(hdf_olp_output_ptr.vcs_heading), props_OLP).write(hdf_olp_output_ptr.vcs_heading);
      s30.createDataSet<float32_t>("heading_rate", ds::From(hdf_olp_output_ptr.heading_rate), props_OLP).write(hdf_olp_output_ptr.heading_rate);
      s30.createDataSet<float32_t>("heading_variance", ds::From(hdf_olp_output_ptr.heading_variance), props_OLP).write(hdf_olp_output_ptr.heading_variance);
      s30.createDataSet<float32_t>("accuracy_heading", ds::From(hdf_olp_output_ptr.accuracy_heading), props_OLP).write(hdf_olp_output_ptr.accuracy_heading);
      s30.createDataSet<float32_t>("eclipse_value", ds::From(hdf_olp_output_ptr.eclipse_value), props_OLP).write(hdf_olp_output_ptr.eclipse_value);
      s30.createDataSet<float32_t>("length", ds::From(hdf_olp_output_ptr.length), props_OLP).write(hdf_olp_output_ptr.length);
      s30.createDataSet<float32_t>("width", ds::From(hdf_olp_output_ptr.width), props_OLP).write(hdf_olp_output_ptr.width);
      s30.createDataSet<float32_t>("obj_distance", ds::From(hdf_olp_output_ptr.obj_distance), props_OLP).write(hdf_olp_output_ptr.obj_distance);
      s30.createDataSet<float32_t>("obstruction_prob", ds::From(hdf_olp_output_ptr.obstruction_prob), props_OLP).write(hdf_olp_output_ptr.obstruction_prob);
      s30.createDataSet<uint8_t>("obj_class", ds::From(hdf_olp_output_ptr.obj_class), props_OLP).write(hdf_olp_output_ptr.obj_class);
      s30.createDataSet<float32_t>("class_prob_pedestrian", ds::From(hdf_olp_output_ptr.class_prob_pedestrian), props_OLP).write(hdf_olp_output_ptr.class_prob_pedestrian);
      s30.createDataSet<float32_t>("class_prob_2wheel", ds::From(hdf_olp_output_ptr.class_prob_2wheel), props_OLP).write(hdf_olp_output_ptr.class_prob_2wheel);
      s30.createDataSet<float32_t>("class_prob_car", ds::From(hdf_olp_output_ptr.class_prob_car), props_OLP).write(hdf_olp_output_ptr.class_prob_car);
      s30.createDataSet<float32_t>("class_prob_truck", ds::From(hdf_olp_output_ptr.class_prob_truck), props_OLP).write(hdf_olp_output_ptr.class_prob_truck);
      s30.createDataSet<uint8_t>("id_merged_obj", ds::From(hdf_olp_output_ptr.id_merged_obj), props_OLP).write(hdf_olp_output_ptr.id_merged_obj);
      s30.createDataSet<uint8_t>("f_merge_occured", ds::From(hdf_olp_output_ptr.f_merge_occured), props_OLP).write(hdf_olp_output_ptr.f_merge_occured);
      s30.createDataSet<uint8_t>("curvi_coordinates_calc_method", ds::From(hdf_olp_output_ptr.curvi_coordinates_calc_method), props_OLP).write(hdf_olp_output_ptr.curvi_coordinates_calc_method);
      s30.createDataSet<float32_t>("curvi_pos_x", ds::From(hdf_olp_output_ptr.curvi_pos_x), props_OLP).write(hdf_olp_output_ptr.curvi_pos_x);
      s30.createDataSet<float32_t>("curvi_pos_y", ds::From(hdf_olp_output_ptr.curvi_pos_y), props_OLP).write(hdf_olp_output_ptr.curvi_pos_y);
      s30.createDataSet<float32_t>("curvi_vel_x", ds::From(hdf_olp_output_ptr.curvi_vel_x), props_OLP).write(hdf_olp_output_ptr.curvi_vel_x);
      s30.createDataSet<float32_t>("curvi_vel_y", ds::From(hdf_olp_output_ptr.curvi_vel_y), props_OLP).write(hdf_olp_output_ptr.curvi_vel_y);
      s30.createDataSet<float32_t>("curvi_vel_rel_x", ds::From(hdf_olp_output_ptr.curvi_vel_rel_x), props_OLP).write(hdf_olp_output_ptr.curvi_vel_rel_x);
      s30.createDataSet<float32_t>("curvi_vel_rel_y", ds::From(hdf_olp_output_ptr.curvi_vel_rel_y), props_OLP).write(hdf_olp_output_ptr.curvi_vel_rel_y);
      s30.createDataSet<float32_t>("curvi_heading", ds::From(hdf_olp_output_ptr.curvi_heading), props_OLP).write(hdf_olp_output_ptr.curvi_heading);
      s30.createDataSet<uint8_t>("f_reflection", ds::From(hdf_olp_output_ptr.f_reflection), props_OLP).write(hdf_olp_output_ptr.f_reflection);
      s30.createDataSet<uint8_t>("f_stationary", ds::From(hdf_olp_output_ptr.f_stationary), props_OLP).write(hdf_olp_output_ptr.f_stationary);
      s30.createDataSet<uint8_t>("f_moveable", ds::From(hdf_olp_output_ptr.f_moveable), props_OLP).write(hdf_olp_output_ptr.f_moveable);
      s30.createDataSet<uint8_t>("f_stationary_clutter", ds::From(hdf_olp_output_ptr.f_stationary_clutter), props_OLP).write(hdf_olp_output_ptr.f_stationary_clutter);
      s30.createDataSet<uint8_t>("f_is_fl_origin_sensor", ds::From(hdf_olp_output_ptr.f_is_fl_origin_sensor), props_OLP).write(hdf_olp_output_ptr.f_is_fl_origin_sensor);
      s30.createDataSet<uint8_t>("f_is_fr_origin_sensor", ds::From(hdf_olp_output_ptr.f_is_fr_origin_sensor), props_OLP).write(hdf_olp_output_ptr.f_is_fr_origin_sensor);
      s30.createDataSet<uint8_t>("f_is_rl_origin_sensor", ds::From(hdf_olp_output_ptr.f_is_rl_origin_sensor), props_OLP).write(hdf_olp_output_ptr.f_is_rl_origin_sensor);
      s30.createDataSet<uint8_t>("f_is_rr_origin_sensor", ds::From(hdf_olp_output_ptr.f_is_rr_origin_sensor), props_OLP).write(hdf_olp_output_ptr.f_is_rr_origin_sensor);
      s30.createDataSet<uint8_t>("f_is_in_fl_sensor_fov", ds::From(hdf_olp_output_ptr.f_is_in_fl_sensor_fov), props_OLP).write(hdf_olp_output_ptr.f_is_in_fl_sensor_fov);
      s30.createDataSet<uint8_t>("f_is_in_fr_sensor_fov", ds::From(hdf_olp_output_ptr.f_is_in_fr_sensor_fov), props_OLP).write(hdf_olp_output_ptr.f_is_in_fr_sensor_fov);
      s30.createDataSet<uint8_t>("f_is_in_rl_sensor_fov", ds::From(hdf_olp_output_ptr.f_is_in_rl_sensor_fov), props_OLP).write(hdf_olp_output_ptr.f_is_in_rl_sensor_fov);
      s30.createDataSet<uint8_t>("f_is_in_rr_sensor_fov", ds::From(hdf_olp_output_ptr.f_is_in_rr_sensor_fov), props_OLP).write(hdf_olp_output_ptr.f_is_in_rr_sensor_fov);
      s30.createDataSet<uint32_t>("n_valid_objects", ds::From(hdf_olp_output_ptr.n_valid_objects), props1D).write(hdf_olp_output_ptr.n_valid_objects);

      auto g5  = file.createGroup("05_FeatureFunctions", gcpl);
      auto lcd = g5.createGroup("LCDA");
      // Write LCDA feature output
      (lcd.createDataSet<int>("lcda_status", ds::From(hdf_lcda_ptr.lcda_status), props1D).write(hdf_lcda_ptr.lcda_status));
      (lcd.createDataSet<boolean_T>("f_bsw_enabled", ds::From(hdf_lcda_ptr.f_bsw_enabled), props1D).write(hdf_lcda_ptr.f_bsw_enabled));
      (lcd.createDataSet<int>("bsw_alert_left", ds::From(hdf_lcda_ptr.bsw_alert_left), props1D).write(hdf_lcda_ptr.bsw_alert_left));
      (lcd.createDataSet<int>("bsw_alert_right", ds::From(hdf_lcda_ptr.bsw_alert_right), props1D).write(hdf_lcda_ptr.bsw_alert_right));
      (lcd.createDataSet<uint8_t>("bsw_id_left", ds::From(hdf_lcda_ptr.bsw_id_left), props1D).write(hdf_lcda_ptr.bsw_id_left));
      (lcd.createDataSet<uint8_t>("bsw_id_right", ds::From(hdf_lcda_ptr.bsw_id_right), props1D).write(hdf_lcda_ptr.bsw_id_right));
      (lcd.createDataSet<uint32_t>("bsw_unique_id_left", ds::From(hdf_lcda_ptr.bsw_unique_id_left), props1D).write(hdf_lcda_ptr.bsw_unique_id_left));
      (lcd.createDataSet<uint32_t>("bsw_unique_id_right", ds::From(hdf_lcda_ptr.bsw_unique_id_right), props1D).write(hdf_lcda_ptr.bsw_unique_id_right));
      (lcd.createDataSet<boolean_T>("f_cvw_enabled", ds::From(hdf_lcda_ptr.f_cvw_enabled), props1D).write(hdf_lcda_ptr.f_cvw_enabled));
      (lcd.createDataSet<int>("cvw_alert_left", ds::From(hdf_lcda_ptr.cvw_alert_left), props1D).write(hdf_lcda_ptr.cvw_alert_left));
      (lcd.createDataSet<int>("cvw_alert_right", ds::From(hdf_lcda_ptr.cvw_alert_right), props1D).write(hdf_lcda_ptr.cvw_alert_right));
      (lcd.createDataSet<uint8_t>("cvw_id_left", ds::From(hdf_lcda_ptr.cvw_id_left), props1D).write(hdf_lcda_ptr.cvw_id_left));
      (lcd.createDataSet<uint8_t>("cvw_id_right", ds::From(hdf_lcda_ptr.cvw_id_right), props1D).write(hdf_lcda_ptr.cvw_id_right));
      (lcd.createDataSet<uint32_t>("cvw_unique_id_left", ds::From(hdf_lcda_ptr.cvw_unique_id_left), props1D).write(hdf_lcda_ptr.cvw_unique_id_left));
      (lcd.createDataSet<uint32_t>("cvw_unique_id_right", ds::From(hdf_lcda_ptr.cvw_unique_id_right), props1D).write(hdf_lcda_ptr.cvw_unique_id_right));
      (lcd.createDataSet<float32_T>("cvw_ttc_s_left", ds::From(hdf_lcda_ptr.cvw_ttc_s_left), props1D).write(hdf_lcda_ptr.cvw_ttc_s_left));
      (lcd.createDataSet<float32_T>("cvw_ttc_s_right", ds::From(hdf_lcda_ptr.cvw_ttc_s_right), props1D).write(hdf_lcda_ptr.cvw_ttc_s_right));
      (lcd.createDataSet<boolean_T>("f_slc_enabled", ds::From(hdf_lcda_ptr.f_slc_enabled), props1D).write(hdf_lcda_ptr.f_slc_enabled));
      (lcd.createDataSet<boolean_T>("slc_alert_left", ds::From(hdf_lcda_ptr.slc_alert_left), props1D).write(hdf_lcda_ptr.slc_alert_left));
      (lcd.createDataSet<boolean_T>("slc_alert_right", ds::From(hdf_lcda_ptr.slc_alert_right), props1D).write(hdf_lcda_ptr.slc_alert_right));
      (lcd.createDataSet<uint8_t>("slc_id_left", ds::From(hdf_lcda_ptr.slc_id_left), props1D).write(hdf_lcda_ptr.slc_id_left));
      (lcd.createDataSet<uint8_t>("slc_id_right", ds::From(hdf_lcda_ptr.slc_id_right), props1D).write(hdf_lcda_ptr.slc_id_right));
      (lcd.createDataSet<uint32_t>("slc_unique_id_left", ds::From(hdf_lcda_ptr.slc_unique_id_left), props1D).write(hdf_lcda_ptr.slc_unique_id_left));
      (lcd.createDataSet<uint32_t>("slc_unique_id_right", ds::From(hdf_lcda_ptr.slc_unique_id_right), props1D).write(hdf_lcda_ptr.slc_unique_id_right));
      (lcd.createDataSet<float32_T>("slc_ttc_s_left", ds::From(hdf_lcda_ptr.slc_ttc_s_left), props1D).write(hdf_lcda_ptr.slc_ttc_s_left));
      (lcd.createDataSet<float32_T>("slc_ttc_s_right", ds::From(hdf_lcda_ptr.slc_ttc_s_right), props1D).write(hdf_lcda_ptr.slc_ttc_s_right));
      (lcd.createDataSet<float32_T>("slc_lane_change_probability_left", ds::From(hdf_lcda_ptr.slc_lane_change_probability_left), props1D).write(hdf_lcda_ptr.slc_lane_change_probability_left));
      (lcd.createDataSet<float32_T>("slc_lane_change_probability_right", ds::From(hdf_lcda_ptr.slc_lane_change_probability_right), props1D).write(hdf_lcda_ptr.slc_lane_change_probability_right));

      /* LCDA calibrations*/
      Lcda_Instance_T *lcda_generic = Lcda_Get_Instance_Ptr();

      auto lcda_cals = lcd.createGroup("Calibration");
      (lcda_cals.createDataSet<float32_T>("k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone", ds::From(lcda_generic->calibration.k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone)).write(lcda_generic->calibration.k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone));
      (lcda_cals.createDataSet<float32_T>("k_bsw_lane_change_intention_vel_lat_hys", ds::From(lcda_generic->calibration.k_bsw_lane_change_intention_vel_lat_hys)).write(lcda_generic->calibration.k_bsw_lane_change_intention_vel_lat_hys));
      (lcda_cals.createDataSet<float32_T>("k_bsw_warntrigger_early", ds::From(lcda_generic->calibration.k_bsw_warntrigger_early)).write(lcda_generic->calibration.k_bsw_warntrigger_early));
      (lcda_cals.createDataSet<float32_T>("k_bsw_warntrigger_late", ds::From(lcda_generic->calibration.k_bsw_warntrigger_late)).write(lcda_generic->calibration.k_bsw_warntrigger_late));
      (lcda_cals.createDataSet<float32_T>("k_bsw_trailer_zone_min_width", ds::From(lcda_generic->calibration.k_bsw_trailer_zone_min_width)).write(lcda_generic->calibration.k_bsw_trailer_zone_min_width));
      (lcda_cals.createDataSet<float32_T>("k_bsw_guardrail_distance_safety_margin", ds::From(lcda_generic->calibration.k_bsw_guardrail_distance_safety_margin)).write(lcda_generic->calibration.k_bsw_guardrail_distance_safety_margin));
      (lcda_cals.createDataSet<float32_T>("k_cvw_warntrigger_early", ds::From(lcda_generic->calibration.k_cvw_warntrigger_early)).write(lcda_generic->calibration.k_cvw_warntrigger_early));
      (lcda_cals.createDataSet<float32_T>("k_cvw_warntrigger_late", ds::From(lcda_generic->calibration.k_cvw_warntrigger_late)).write(lcda_generic->calibration.k_cvw_warntrigger_late));
      (lcda_cals.createDataSet<float32_T>("k_zone_hys_obj_width_correction", ds::From(lcda_generic->calibration.k_zone_hys_obj_width_correction)).write(lcda_generic->calibration.k_zone_hys_obj_width_correction));
      (lcda_cals.createDataSet<float32_T>("k_cvw_curve_zone_factor_outer", ds::From(lcda_generic->calibration.k_cvw_curve_zone_factor_outer)).write(lcda_generic->calibration.k_cvw_curve_zone_factor_outer));
      (lcda_cals.createDataSet<float32_T>("k_cvw_curve_zone_factor_inner", ds::From(lcda_generic->calibration.k_cvw_curve_zone_factor_inner)).write(lcda_generic->calibration.k_cvw_curve_zone_factor_inner));
      (lcda_cals.createDataSet<float32_T>("k_cvw_time_obj_start_decel_after_lane_change", ds::From(lcda_generic->calibration.k_cvw_time_obj_start_decel_after_lane_change)).write(lcda_generic->calibration.k_cvw_time_obj_start_decel_after_lane_change));
      (lcda_cals.createDataSet<float32_T>("k_cvw_time_diff_after_obj_decel", ds::From(lcda_generic->calibration.k_cvw_time_diff_after_obj_decel)).write(lcda_generic->calibration.k_cvw_time_diff_after_obj_decel));
      (lcda_cals.createDataSet<float32_T>("k_cvw_crit_dist_hys_factor", ds::From(lcda_generic->calibration.k_cvw_crit_dist_hys_factor)).write(lcda_generic->calibration.k_cvw_crit_dist_hys_factor));
      (lcda_cals.createDataSet<float32_T>("k_cvw_crit_dist_additive_hys", ds::From(lcda_generic->calibration.k_cvw_crit_dist_additive_hys)).write(lcda_generic->calibration.k_cvw_crit_dist_additive_hys));
      (lcda_cals.createDataSet<float32_T>("k_cvw_critical_obj_decel_after_lane_change", ds::From(lcda_generic->calibration.k_cvw_critical_obj_decel_after_lane_change)).write(lcda_generic->calibration.k_cvw_critical_obj_decel_after_lane_change));
      (lcda_cals.createDataSet<float32_T>("k_cvw_lane_change_intention_zone_small_x", ds::From(lcda_generic->calibration.k_cvw_lane_change_intention_zone_small_x)).write(lcda_generic->calibration.k_cvw_lane_change_intention_zone_small_x));
      (lcda_cals.createDataSet<float32_T>("k_cvw_lane_change_intention_zone_small_y", ds::From(lcda_generic->calibration.k_cvw_lane_change_intention_zone_small_y)).write(lcda_generic->calibration.k_cvw_lane_change_intention_zone_small_y));
      (lcda_cals.createDataSet<float32_T>("k_cvw_lane_change_intention_zone_hys_y", ds::From(lcda_generic->calibration.k_cvw_lane_change_intention_zone_hys_y)).write(lcda_generic->calibration.k_cvw_lane_change_intention_zone_hys_y));
      (lcda_cals.createDataSet<float32_T>("k_bsw_y1_hys", ds::From(lcda_generic->calibration.k_bsw_y1_hys)).write(lcda_generic->calibration.k_bsw_y1_hys));
      (lcda_cals.createDataSet<float32_T>("k_bsw_y0_hys", ds::From(lcda_generic->calibration.k_bsw_y0_hys)).write(lcda_generic->calibration.k_bsw_y0_hys));
      (lcda_cals.createDataSet<float32_T>("k_bsw_x1_hys", ds::From(lcda_generic->calibration.k_bsw_x1_hys)).write(lcda_generic->calibration.k_bsw_x1_hys));
      (lcda_cals.createDataSet<float32_T>("k_bsw_x0_hys", ds::From(lcda_generic->calibration.k_bsw_x0_hys)).write(lcda_generic->calibration.k_bsw_x0_hys));
      (lcda_cals.createDataSet<float32_T>("k_slc_zone_y_hys", ds::From(lcda_generic->calibration.k_slc_zone_y_hys)).write(lcda_generic->calibration.k_slc_zone_y_hys));
      (lcda_cals.createDataSet<float32_T>("k_slc_max_obj_eclipse", ds::From(lcda_generic->calibration.k_slc_max_obj_eclipse)).write(lcda_generic->calibration.k_slc_max_obj_eclipse));
      (lcda_cals.createDataSet<float32_T>("k_slc_critical_lat_ttc_hys", ds::From(lcda_generic->calibration.k_slc_critical_lat_ttc_hys)).write(lcda_generic->calibration.k_slc_critical_lat_ttc_hys));
      (lcda_cals.createDataSet<float32_T>("k_slc_critical_lon_ttc_hys", ds::From(lcda_generic->calibration.k_slc_critical_lon_ttc_hys)).write(lcda_generic->calibration.k_slc_critical_lon_ttc_hys));
      (lcda_cals.createDataSet<float32_T>("k_slc_warntrigger_TTC_lat_late", ds::From(lcda_generic->calibration.k_slc_warntrigger_TTC_lat_late)).write(lcda_generic->calibration.k_slc_warntrigger_TTC_lat_late));
      (lcda_cals.createDataSet<float32_T>("k_slc_warntrigger_TTC_lat_early", ds::From(lcda_generic->calibration.k_slc_warntrigger_TTC_lat_early)).write(lcda_generic->calibration.k_slc_warntrigger_TTC_lat_early));
      (lcda_cals.createDataSet<float32_T>("k_slc_warntrigger_TTC_lon_late", ds::From(lcda_generic->calibration.k_slc_warntrigger_TTC_lon_late)).write(lcda_generic->calibration.k_slc_warntrigger_TTC_lon_late));
      (lcda_cals.createDataSet<float32_T>("k_slc_warntrigger_TTC_lon_early", ds::From(lcda_generic->calibration.k_slc_warntrigger_TTC_lon_early)).write(lcda_generic->calibration.k_slc_warntrigger_TTC_lon_early));
      (lcda_cals.createDataSet<float32_T>("k_slc_obj_lc_effective_speed_min", ds::From(lcda_generic->calibration.k_slc_obj_lc_effective_speed_min)).write(lcda_generic->calibration.k_slc_obj_lc_effective_speed_min));
      (lcda_cals.createDataSet<float32_T>("k_elc_zone_x", ds::From(lcda_generic->calibration.k_elc_zone_x)).write(lcda_generic->calibration.k_elc_zone_x));
      (lcda_cals.createDataSet<float32_T>("k_elc_zone_y", ds::From(lcda_generic->calibration.k_elc_zone_y)).write(lcda_generic->calibration.k_elc_zone_y));
      (lcda_cals.createDataSet<float32_T>("k_elc_zone_y_hys", ds::From(lcda_generic->calibration.k_elc_zone_y_hys)).write(lcda_generic->calibration.k_elc_zone_y_hys));
      (lcda_cals.createDataSet<float32_T>("k_elc_max_curvi_heading_abs", ds::From(lcda_generic->calibration.k_elc_max_curvi_heading_abs)).write(lcda_generic->calibration.k_elc_max_curvi_heading_abs));
      (lcda_cals.createDataSet<float32_T>("k_elc_min_obj_curvi_long_vel_abs", ds::From(lcda_generic->calibration.k_elc_min_obj_curvi_long_vel_abs)).write(lcda_generic->calibration.k_elc_min_obj_curvi_long_vel_abs));
      (lcda_cals.createDataSet<float32_T>("k_elc_critical_longitudinal_ttc", ds::From(lcda_generic->calibration.k_elc_critical_longitudinal_ttc)).write(lcda_generic->calibration.k_elc_critical_longitudinal_ttc));
      (lcda_cals.createDataSet<float32_T>("k_elc_critical_longitudinal_ttc_hys", ds::From(lcda_generic->calibration.k_elc_critical_longitudinal_ttc_hys)).write(lcda_generic->calibration.k_elc_critical_longitudinal_ttc_hys));
      (lcda_cals.createDataSet<float32_T>("k_elc_obj_safe_deceleration_threshold", ds::From(lcda_generic->calibration.k_elc_obj_safe_deceleration_threshold)).write(lcda_generic->calibration.k_elc_obj_safe_deceleration_threshold));
      (lcda_cals.createDataSet<float32_T>("k_elc_obj_safe_deceleration_threshold_hys", ds::From(lcda_generic->calibration.k_elc_obj_safe_deceleration_threshold_hys)).write(lcda_generic->calibration.k_elc_obj_safe_deceleration_threshold_hys));
      (lcda_cals.createDataSet<float32_T>("k_lm_lane_width_city", ds::From(lcda_generic->calibration.k_lm_lane_width_city)).write(lcda_generic->calibration.k_lm_lane_width_city));
      (lcda_cals.createDataSet<float32_T>("k_lm_lane_width_highway", ds::From(lcda_generic->calibration.k_lm_lane_width_highway)).write(lcda_generic->calibration.k_lm_lane_width_highway));
      (lcda_cals.createDataSet<float32_T>("k_lcda_lm_lane_width_us_default", ds::From(lcda_generic->calibration.k_lcda_lm_lane_width_us_default)).write(lcda_generic->calibration.k_lcda_lm_lane_width_us_default));
      (lcda_cals.createDataSet<float32_T>("k_lcda_lm_lane_width_japan_default", ds::From(lcda_generic->calibration.k_lcda_lm_lane_width_japan_default)).write(lcda_generic->calibration.k_lcda_lm_lane_width_japan_default));
      (lcda_cals.createDataSet<float32_T>("k_lcda_lm_lane_width_china_default", ds::From(lcda_generic->calibration.k_lcda_lm_lane_width_china_default)).write(lcda_generic->calibration.k_lcda_lm_lane_width_china_default));
      (lcda_cals.createDataSet<float32_T>("k_lcda_lm_lane_width_korea_default", ds::From(lcda_generic->calibration.k_lcda_lm_lane_width_korea_default)).write(lcda_generic->calibration.k_lcda_lm_lane_width_korea_default));
      (lcda_cals.createDataSet<float32_T>("k_lcda_lm_lane_width_germany_default", ds::From(lcda_generic->calibration.k_lcda_lm_lane_width_germany_default)).write(lcda_generic->calibration.k_lcda_lm_lane_width_germany_default));
      (lcda_cals.createDataSet<float32_T>("k_lcda_lm_lane_width_defaultcountry_default", ds::From(lcda_generic->calibration.k_lcda_lm_lane_width_defaultcountry_default)).write(lcda_generic->calibration.k_lcda_lm_lane_width_defaultcountry_default));
      (lcda_cals.createDataSet<float32_T>("k_lm_lane_center_offset_default", ds::From(lcda_generic->calibration.k_lm_lane_center_offset_default)).write(lcda_generic->calibration.k_lm_lane_center_offset_default));
      (lcda_cals.createDataSet<float32_T>("k_lm_min_speed_hway", ds::From(lcda_generic->calibration.k_lm_min_speed_hway)).write(lcda_generic->calibration.k_lm_min_speed_hway));
      (lcda_cals.createDataSet<float32_T>("k_lm_hys_delta_speed_hway", ds::From(lcda_generic->calibration.k_lm_hys_delta_speed_hway)).write(lcda_generic->calibration.k_lm_hys_delta_speed_hway));
      (lcda_cals.createDataSet<float32_T>("k_lm_min_yawrate_city_abs", ds::From(lcda_generic->calibration.k_lm_min_yawrate_city_abs)).write(lcda_generic->calibration.k_lm_min_yawrate_city_abs));
      (lcda_cals.createDataSet<float32_T>("k_lm_hys_delta_yawrate_city_abs", ds::From(lcda_generic->calibration.k_lm_hys_delta_yawrate_city_abs)).write(lcda_generic->calibration.k_lm_hys_delta_yawrate_city_abs));
      (lcda_cals.createDataSet<float32_T>("k_lm_min_lane_exist_prob_percent", ds::From(lcda_generic->calibration.k_lm_min_lane_exist_prob_percent)).write(lcda_generic->calibration.k_lm_min_lane_exist_prob_percent));
      (lcda_cals.createDataSet<float32_T>("k_lm_min_plausible_lane_width", ds::From(lcda_generic->calibration.k_lm_min_plausible_lane_width)).write(lcda_generic->calibration.k_lm_min_plausible_lane_width));
      (lcda_cals.createDataSet<float32_T>("k_lm_max_plausible_lane_width", ds::From(lcda_generic->calibration.k_lm_max_plausible_lane_width)).write(lcda_generic->calibration.k_lm_max_plausible_lane_width));
      (lcda_cals.createDataSet<float32_T>("k_lm_max_plausible_lc_offset_factor", ds::From(lcda_generic->calibration.k_lm_max_plausible_lc_offset_factor)).write(lcda_generic->calibration.k_lm_max_plausible_lc_offset_factor));
      (lcda_cals.createDataSet<float32_T>("k_lcda_min_exist_prop", ds::From(lcda_generic->calibration.k_lcda_min_exist_prop)).write(lcda_generic->calibration.k_lcda_min_exist_prop));
      (lcda_cals.createDataSet<float32_T>("k_lcda_host_activation_speed_min", ds::From(lcda_generic->calibration.k_lcda_host_activation_speed_min)).write(lcda_generic->calibration.k_lcda_host_activation_speed_min));
      (lcda_cals.createDataSet<float32_T>("k_lcda_host_activation_speed_min_hys", ds::From(lcda_generic->calibration.k_lcda_host_activation_speed_min_hys)).write(lcda_generic->calibration.k_lcda_host_activation_speed_min_hys));
      (lcda_cals.createDataSet<float32_T>("k_lcda_host_activation_speed_max", ds::From(lcda_generic->calibration.k_lcda_host_activation_speed_max)).write(lcda_generic->calibration.k_lcda_host_activation_speed_max));
      (lcda_cals.createDataSet<float32_T>("k_lcda_host_activation_speed_max_hys", ds::From(lcda_generic->calibration.k_lcda_host_activation_speed_max_hys)).write(lcda_generic->calibration.k_lcda_host_activation_speed_max_hys));
      (lcda_cals.createDataSet<float32_T>("k_lcda_distance_traveled_scale_factor", ds::From(lcda_generic->calibration.k_lcda_distance_traveled_scale_factor)).write(lcda_generic->calibration.k_lcda_distance_traveled_scale_factor));
      (lcda_cals.createDataSet<float32_T>("k_lcda_min_curve_radius", ds::From(lcda_generic->calibration.k_lcda_min_curve_radius)).write(lcda_generic->calibration.k_lcda_min_curve_radius));
      (lcda_cals.createDataSet<float32_T>("k_lcda_min_curve_radius_hys", ds::From(lcda_generic->calibration.k_lcda_min_curve_radius_hys)).write(lcda_generic->calibration.k_lcda_min_curve_radius_hys));
      (lcda_cals.createDataSet<float32_T>("k_lcda_curve_radius_threshold_for_zone_adaptation", ds::From(lcda_generic->calibration.k_lcda_curve_radius_threshold_for_zone_adaptation)).write(lcda_generic->calibration.k_lcda_curve_radius_threshold_for_zone_adaptation));
      (lcda_cals.createDataSet<float32_T>("k_lcda_min_lane_width", ds::From(lcda_generic->calibration.k_lcda_min_lane_width)).write(lcda_generic->calibration.k_lcda_min_lane_width));
      (lcda_cals.createDataSet<float32_T>("k_lcda_max_lane_width", ds::From(lcda_generic->calibration.k_lcda_max_lane_width)).write(lcda_generic->calibration.k_lcda_max_lane_width));
      (lcda_cals.createDataSet<float32_T>("k_lcda_min_ego_vehicle_width", ds::From(lcda_generic->calibration.k_lcda_min_ego_vehicle_width)).write(lcda_generic->calibration.k_lcda_min_ego_vehicle_width));
      (lcda_cals.createDataSet<float32_T>("k_lcda_max_ego_vehicle_width", ds::From(lcda_generic->calibration.k_lcda_max_ego_vehicle_width)).write(lcda_generic->calibration.k_lcda_max_ego_vehicle_width));
      (lcda_cals.createDataSet<float32_T>("k_lcda_min_ego_vehicle_length", ds::From(lcda_generic->calibration.k_lcda_min_ego_vehicle_length)).write(lcda_generic->calibration.k_lcda_min_ego_vehicle_length));
      (lcda_cals.createDataSet<float32_T>("k_lcda_max_ego_vehicle_length", ds::From(lcda_generic->calibration.k_lcda_max_ego_vehicle_length)).write(lcda_generic->calibration.k_lcda_max_ego_vehicle_length));
      (lcda_cals.createDataSet<float32_T>("k_lcda_zone_intersect_critical_point_lateral_ratio", ds::From(lcda_generic->calibration.k_lcda_zone_intersect_critical_point_lateral_ratio)).write(lcda_generic->calibration.k_lcda_zone_intersect_critical_point_lateral_ratio));
      (lcda_cals.createDataSet<float32_T>("k_lcda_exist_prob_lc_intention_hys_offset", ds::From(lcda_generic->calibration.k_lcda_exist_prob_lc_intention_hys_offset)).write(lcda_generic->calibration.k_lcda_exist_prob_lc_intention_hys_offset));
      (lcda_cals.createDataSet<float32_T>("k_lcda_exist_prob_hys_offset", ds::From(lcda_generic->calibration.k_lcda_exist_prob_hys_offset)).write(lcda_generic->calibration.k_lcda_exist_prob_hys_offset));
      (lcda_cals.createDataSet<float32_T>("k_lcda_lane_change_intention_vel_lat_thresh", ds::From(lcda_generic->calibration.k_lcda_lane_change_intention_vel_lat_thresh)).write(lcda_generic->calibration.k_lcda_lane_change_intention_vel_lat_thresh));
      (lcda_cals.createDataSet<float32_T>("k_bsw_lane_change_intention_pos_lat_thres", ds::From(lcda_generic->calibration.k_bsw_lane_change_intention_pos_lat_thres)).write(lcda_generic->calibration.k_bsw_lane_change_intention_pos_lat_thres));
      (lcda_cals.createDataSet<float32_T>("k_bsw_lane_change_intention_pos_long_thres", ds::From(lcda_generic->calibration.k_bsw_lane_change_intention_pos_long_thres)).write(lcda_generic->calibration.k_bsw_lane_change_intention_pos_long_thres));
      (lcda_cals.createDataSet<float32_T>("k_lcda_ego_lane_effective_lane_width_factor", ds::From(lcda_generic->calibration.k_lcda_ego_lane_effective_lane_width_factor)).write(lcda_generic->calibration.k_lcda_ego_lane_effective_lane_width_factor));
      (lcda_cals.createDataSet<float32_T>("k_lcda_min_exist_prob_lc_intention", ds::From(lcda_generic->calibration.k_lcda_min_exist_prob_lc_intention)).write(lcda_generic->calibration.k_lcda_min_exist_prob_lc_intention));
      (lcda_cals.createDataSet<float32_T>("k_bsw_zone_x", ds::From(lcda_generic->calibration.k_bsw_zone_x)).write(lcda_generic->calibration.k_bsw_zone_x));
      (lcda_cals.createDataSet<float32_T>("k_bsw_zone_y", ds::From(lcda_generic->calibration.k_bsw_zone_y)).write(lcda_generic->calibration.k_bsw_zone_y));
      (lcda_cals.createDataSet<float32_T>("k_bsw_zone_x_hys", ds::From(lcda_generic->calibration.k_bsw_zone_x_hys)).write(lcda_generic->calibration.k_bsw_zone_x_hys));
      (lcda_cals.createDataSet<float32_T>("k_bsw_zone_y_hys", ds::From(lcda_generic->calibration.k_bsw_zone_y_hys)).write(lcda_generic->calibration.k_bsw_zone_y_hys));
      (lcda_cals.createDataSet<float32_T>("k_bsw_zone_y_hys_min", ds::From(lcda_generic->calibration.k_bsw_zone_y_hys_min)).write(lcda_generic->calibration.k_bsw_zone_y_hys_min));
      (lcda_cals.createDataSet<float32_T>("k_bsw_zone_y_hys_max", ds::From(lcda_generic->calibration.k_bsw_zone_y_hys_max)).write(lcda_generic->calibration.k_bsw_zone_y_hys_max));
      (lcda_cals.createDataSet<float32_T>("k_bsw_fixed_zone_x", ds::From(lcda_generic->calibration.k_bsw_fixed_zone_x)).write(lcda_generic->calibration.k_bsw_fixed_zone_x));
      (lcda_cals.createDataSet<float32_T>("k_bsw_fixed_zone_y", ds::From(lcda_generic->calibration.k_bsw_fixed_zone_y)).write(lcda_generic->calibration.k_bsw_fixed_zone_y));
      (lcda_cals.createDataSet<float32_T>("k_bsw_fixed_zone_x_hys", ds::From(lcda_generic->calibration.k_bsw_fixed_zone_x_hys)).write(lcda_generic->calibration.k_bsw_fixed_zone_x_hys));
      (lcda_cals.createDataSet<float32_T>("k_bsw_fixed_zone_y_hys", ds::From(lcda_generic->calibration.k_bsw_fixed_zone_y_hys)).write(lcda_generic->calibration.k_bsw_fixed_zone_y_hys));
      (lcda_cals.createDataSet<float32_T>("k_bsw_lateral_distance_zone", ds::From(lcda_generic->calibration.k_bsw_lateral_distance_zone)).write(lcda_generic->calibration.k_bsw_lateral_distance_zone));
      (lcda_cals.createDataSet<float32_T>("k_bsw_overlap_area_threshold", ds::From(lcda_generic->calibration.k_bsw_overlap_area_threshold)).write(lcda_generic->calibration.k_bsw_overlap_area_threshold));
      (lcda_cals.createDataSet<float32_T>("k_bsw_fallback_rel_vel_thres", ds::From(lcda_generic->calibration.k_bsw_fallback_rel_vel_thres)).write(lcda_generic->calibration.k_bsw_fallback_rel_vel_thres));
      (lcda_cals.createDataSet<float32_T>("k_bsw_fallback_rel_vel_thres_hys", ds::From(lcda_generic->calibration.k_bsw_fallback_rel_vel_thres_hys)).write(lcda_generic->calibration.k_bsw_fallback_rel_vel_thres_hys));
      (lcda_cals.createDataSet<float32_T>("k_bsw_dynzone_speed", ds::From(lcda_generic->calibration.k_bsw_dynzone_speed)).write(lcda_generic->calibration.k_bsw_dynzone_speed));
      (lcda_cals.createDataSet<float32_T>("k_bsw_dynzone_range", ds::From(lcda_generic->calibration.k_bsw_dynzone_range)).write(lcda_generic->calibration.k_bsw_dynzone_range));
      (lcda_cals.createDataSet<float32_T>("k_min_exist_prob_radar_guardrail", ds::From(lcda_generic->calibration.k_min_exist_prob_radar_guardrail)).write(lcda_generic->calibration.k_min_exist_prob_radar_guardrail));
      (lcda_cals.createDataSet<float32_T>("k_min_exist_prob_camera_guardrail", ds::From(lcda_generic->calibration.k_min_exist_prob_camera_guardrail)).write(lcda_generic->calibration.k_min_exist_prob_camera_guardrail));
      (lcda_cals.createDataSet<float32_T>("k_bsw_dynzone_speed_dropback", ds::From(lcda_generic->calibration.k_bsw_dynzone_speed_dropback)).write(lcda_generic->calibration.k_bsw_dynzone_speed_dropback));
      (lcda_cals.createDataSet<float32_T>("k_bsw_dynzone_speed_dropback_max", ds::From(lcda_generic->calibration.k_bsw_dynzone_speed_dropback_max)).write(lcda_generic->calibration.k_bsw_dynzone_speed_dropback_max));
      (lcda_cals.createDataSet<float32_T>("k_bsw_dynzone_range_dropback", ds::From(lcda_generic->calibration.k_bsw_dynzone_range_dropback)).write(lcda_generic->calibration.k_bsw_dynzone_range_dropback));
      (lcda_cals.createDataSet<float32_T>("k_bsw_n_line_position_for_long_object_sot_scenario", ds::From(lcda_generic->calibration.k_bsw_n_line_position_for_long_object_sot_scenario)).write(lcda_generic->calibration.k_bsw_n_line_position_for_long_object_sot_scenario));
      (lcda_cals.createDataSet<float32_T>("k_bsw_line_to_stop_TOS_alert", ds::From(lcda_generic->calibration.k_bsw_line_to_stop_TOS_alert)).write(lcda_generic->calibration.k_bsw_line_to_stop_TOS_alert));
      (lcda_cals.createDataSet<float32_T>("k_bsw_min_length_long_object", ds::From(lcda_generic->calibration.k_bsw_min_length_long_object)).write(lcda_generic->calibration.k_bsw_min_length_long_object));
      (lcda_cals.createDataSet<float32_T>("k_bsw_min_length_long_object_hys", ds::From(lcda_generic->calibration.k_bsw_min_length_long_object_hys)).write(lcda_generic->calibration.k_bsw_min_length_long_object_hys));
      (lcda_cals.createDataSet<float32_T>("k_bsw_suppress_late_warning_max_time_till_leave", ds::From(lcda_generic->calibration.k_bsw_suppress_late_warning_max_time_till_leave)).write(lcda_generic->calibration.k_bsw_suppress_late_warning_max_time_till_leave));
      (lcda_cals.createDataSet<float32_T>("k_bsw_suppress_late_warning_min_pos_behind_host", ds::From(lcda_generic->calibration.k_bsw_suppress_late_warning_min_pos_behind_host)).write(lcda_generic->calibration.k_bsw_suppress_late_warning_min_pos_behind_host));
      (lcda_cals.createDataSet<float32_T>("k_bsw_suppress_late_warning_max_rel_vel", ds::From(lcda_generic->calibration.k_bsw_suppress_late_warning_max_rel_vel)).write(lcda_generic->calibration.k_bsw_suppress_late_warning_max_rel_vel));
      (lcda_cals.createDataSet<float32_T>("k_bsw_max_heading_abs", ds::From(lcda_generic->calibration.k_bsw_max_heading_abs)).write(lcda_generic->calibration.k_bsw_max_heading_abs));
      (lcda_cals.createDataSet<float32_T>("k_bsw_max_heading_abs_hysteresis", ds::From(lcda_generic->calibration.k_bsw_max_heading_abs_hysteresis)).write(lcda_generic->calibration.k_bsw_max_heading_abs_hysteresis));
      (lcda_cals.createDataSet<float32_T>("k_bsw_min_obj_long_vel", ds::From(lcda_generic->calibration.k_bsw_min_obj_long_vel)).write(lcda_generic->calibration.k_bsw_min_obj_long_vel));
      (lcda_cals.createDataSet<float32_T>("k_bsw_max_obj_long_vel", ds::From(lcda_generic->calibration.k_bsw_max_obj_long_vel)).write(lcda_generic->calibration.k_bsw_max_obj_long_vel));
      (lcda_cals.createDataSet<float32_T>("k_bsw_min_obj_long_vel_hysteresis", ds::From(lcda_generic->calibration.k_bsw_min_obj_long_vel_hysteresis)).write(lcda_generic->calibration.k_bsw_min_obj_long_vel_hysteresis));
      (lcda_cals.createDataSet<float32_T>("k_bsw_max_obj_long_vel_hysteresis", ds::From(lcda_generic->calibration.k_bsw_max_obj_long_vel_hysteresis)).write(lcda_generic->calibration.k_bsw_max_obj_long_vel_hysteresis));
      (lcda_cals.createDataSet<float32_T>("k_bsw_trailer_zone_ext_safety_margin", ds::From(lcda_generic->calibration.k_bsw_trailer_zone_ext_safety_margin)).write(lcda_generic->calibration.k_bsw_trailer_zone_ext_safety_margin));
      (lcda_cals.createDataSet<float32_T>("k_bsw_trailer_zone_ext_safety_margin_hys", ds::From(lcda_generic->calibration.k_bsw_trailer_zone_ext_safety_margin_hys)).write(lcda_generic->calibration.k_bsw_trailer_zone_ext_safety_margin_hys));
      (lcda_cals.createDataSet<float32_T>("k_cvw_zone_x", ds::From(lcda_generic->calibration.k_cvw_zone_x)).write(lcda_generic->calibration.k_cvw_zone_x));
      (lcda_cals.createDataSet<float32_T>("k_cvw_zone_y", ds::From(lcda_generic->calibration.k_cvw_zone_y)).write(lcda_generic->calibration.k_cvw_zone_y));
      (lcda_cals.createDataSet<float32_T>("k_cvw_zone_y_hys", ds::From(lcda_generic->calibration.k_cvw_zone_y_hys)).write(lcda_generic->calibration.k_cvw_zone_y_hys));
      (lcda_cals.createDataSet<float32_T>("k_cvw_zone_y_hys_max", ds::From(lcda_generic->calibration.k_cvw_zone_y_hys_max)).write(lcda_generic->calibration.k_cvw_zone_y_hys_max));
      (lcda_cals.createDataSet<float32_T>("k_cvw_zone_y_hys_min", ds::From(lcda_generic->calibration.k_cvw_zone_y_hys_min)).write(lcda_generic->calibration.k_cvw_zone_y_hys_min));
      (lcda_cals.createDataSet<float32_T>("k_lcda_max_range", ds::From(lcda_generic->calibration.k_lcda_max_range)).write(lcda_generic->calibration.k_lcda_max_range));
      (lcda_cals.createDataSet<float32_T>("k_cvw_gap_bridge", ds::From(lcda_generic->calibration.k_cvw_gap_bridge)).write(lcda_generic->calibration.k_cvw_gap_bridge));
      (lcda_cals.createDataSet<float32_T>("k_cvw_candidate_ttc", ds::From(lcda_generic->calibration.k_cvw_candidate_ttc)).write(lcda_generic->calibration.k_cvw_candidate_ttc));
      (lcda_cals.createDataSet<float32_T>("k_cvw_ttc", ds::From(lcda_generic->calibration.k_cvw_ttc)).write(lcda_generic->calibration.k_cvw_ttc));
      (lcda_cals.createDataSet<float32_T>("k_cvw_ttc_hys", ds::From(lcda_generic->calibration.k_cvw_ttc_hys)).write(lcda_generic->calibration.k_cvw_ttc_hys));
      (lcda_cals.createDataSet<float32_T>("k_cvw_max_curvi_heading_abs", ds::From(lcda_generic->calibration.k_cvw_max_curvi_heading_abs)).write(lcda_generic->calibration.k_cvw_max_curvi_heading_abs));
      (lcda_cals.createDataSet<float32_T>("k_cvw_min_obj_curvi_long_vel", ds::From(lcda_generic->calibration.k_cvw_min_obj_curvi_long_vel)).write(lcda_generic->calibration.k_cvw_min_obj_curvi_long_vel));
      (lcda_cals.createDataSet<float32_T>("k_cvw_lane_change_intention_zone_x", ds::From(lcda_generic->calibration.k_cvw_lane_change_intention_zone_x)).write(lcda_generic->calibration.k_cvw_lane_change_intention_zone_x));
      (lcda_cals.createDataSet<float32_T>("k_cvw_lane_change_intention_zone_y", ds::From(lcda_generic->calibration.k_cvw_lane_change_intention_zone_y)).write(lcda_generic->calibration.k_cvw_lane_change_intention_zone_y));
      (lcda_cals.createDataSet<float32_T>("k_cvw_y_width1", ds::From(lcda_generic->calibration.k_cvw_y_width1)).write(lcda_generic->calibration.k_cvw_y_width1));
      (lcda_cals.createDataSet<float32_T>("k_cvw_x_length1", ds::From(lcda_generic->calibration.k_cvw_x_length1)).write(lcda_generic->calibration.k_cvw_x_length1));
      (lcda_cals.createDataSet<float32_T>("k_cvw_y1", ds::From(lcda_generic->calibration.k_cvw_y1)).write(lcda_generic->calibration.k_cvw_y1));
      (lcda_cals.createDataSet<float32_T>("k_cvw_x_length0", ds::From(lcda_generic->calibration.k_cvw_x_length0)).write(lcda_generic->calibration.k_cvw_x_length0));
      (lcda_cals.createDataSet<float32_T>("k_cvw_y_width0", ds::From(lcda_generic->calibration.k_cvw_y_width0)).write(lcda_generic->calibration.k_cvw_y_width0));
      (lcda_cals.createDataSet<float32_T>("k_cvw_y0", ds::From(lcda_generic->calibration.k_cvw_y0)).write(lcda_generic->calibration.k_cvw_y0));
      (lcda_cals.createDataSet<float32_T>("k_cvw_x0", ds::From(lcda_generic->calibration.k_cvw_x0)).write(lcda_generic->calibration.k_cvw_x0));
      (lcda_cals.createDataSet<float32_T>("k_bsw_y_width", ds::From(lcda_generic->calibration.k_bsw_y_width)).write(lcda_generic->calibration.k_bsw_y_width));
      (lcda_cals.createDataSet<float32_T>("k_bsw_y0", ds::From(lcda_generic->calibration.k_bsw_y0)).write(lcda_generic->calibration.k_bsw_y0));
      (lcda_cals.createDataSet<float32_T>("k_bsw_x_length", ds::From(lcda_generic->calibration.k_bsw_x_length)).write(lcda_generic->calibration.k_bsw_x_length));
      (lcda_cals.createDataSet<float32_T>("k_bsw_x0", ds::From(lcda_generic->calibration.k_bsw_x0)).write(lcda_generic->calibration.k_bsw_x0));
      (lcda_cals.createDataSet<float32_T>("k_lka_ov_zone_width", ds::From(lcda_generic->calibration.k_lka_ov_zone_width)).write(lcda_generic->calibration.k_lka_ov_zone_width));
      (lcda_cals.createDataSet<float32_T>("k_slc_zone_x", ds::From(lcda_generic->calibration.k_slc_zone_x)).write(lcda_generic->calibration.k_slc_zone_x));
      (lcda_cals.createDataSet<float32_T>("k_slc_zone_y", ds::From(lcda_generic->calibration.k_slc_zone_y)).write(lcda_generic->calibration.k_slc_zone_y));
      (lcda_cals.createDataSet<float32_T>("k_slc_max_curvi_heading_abs", ds::From(lcda_generic->calibration.k_slc_max_curvi_heading_abs)).write(lcda_generic->calibration.k_slc_max_curvi_heading_abs));
      (lcda_cals.createDataSet<float32_T>("k_slc_min_obj_curvi_long_vel_abs", ds::From(lcda_generic->calibration.k_slc_min_obj_curvi_long_vel_abs)).write(lcda_generic->calibration.k_slc_min_obj_curvi_long_vel_abs));
      (lcda_cals.createDataSet<float32_T>("k_slc_critical_lat_ttc", ds::From(lcda_generic->calibration.k_slc_critical_lat_ttc)).write(lcda_generic->calibration.k_slc_critical_lat_ttc));
      (lcda_cals.createDataSet<float32_T>("k_slc_critical_lon_ttc", ds::From(lcda_generic->calibration.k_slc_critical_lon_ttc)).write(lcda_generic->calibration.k_slc_critical_lon_ttc));
      (lcda_cals.createDataSet<float32_T>("k_slc_lateral_ttc_lookup", ds::From(lcda_generic->calibration.k_slc_lateral_ttc_lookup)).write(lcda_generic->calibration.k_slc_lateral_ttc_lookup));
      (lcda_cals.createDataSet<float32_T>("k_slc_lane_change_prob_lookup", ds::From(lcda_generic->calibration.k_slc_lane_change_prob_lookup)).write(lcda_generic->calibration.k_slc_lane_change_prob_lookup));
      (lcda_cals.createDataSet<float32_T>("k_slc_lookup_ego_speed", ds::From(lcda_generic->calibration.k_slc_lookup_ego_speed)).write(lcda_generic->calibration.k_slc_lookup_ego_speed));
      (lcda_cals.createDataSet<float32_T>("k_slc_lookup_ego_overlap_offset", ds::From(lcda_generic->calibration.k_slc_lookup_ego_overlap_offset)).write(lcda_generic->calibration.k_slc_lookup_ego_overlap_offset));
      (lcda_cals.createDataSet<float32_T>("k_bsw_obj_max_rel_vel_thresh", ds::From(lcda_generic->calibration.k_bsw_obj_max_rel_vel_thresh)).write(lcda_generic->calibration.k_bsw_obj_max_rel_vel_thresh));
      (lcda_cals.createDataSet<float32_T>("k_bsw_obj_max_rel_vel_hys", ds::From(lcda_generic->calibration.k_bsw_obj_max_rel_vel_hys)).write(lcda_generic->calibration.k_bsw_obj_max_rel_vel_hys));
      (lcda_cals.createDataSet<float32_T>("k_bsw_zone_front_ego_side_x", ds::From(lcda_generic->calibration.k_bsw_zone_front_ego_side_x)).write(lcda_generic->calibration.k_bsw_zone_front_ego_side_x));
      (lcda_cals.createDataSet<float32_T>("k_bsw_zone_front_ego_side_y", ds::From(lcda_generic->calibration.k_bsw_zone_front_ego_side_y)).write(lcda_generic->calibration.k_bsw_zone_front_ego_side_y));
      (lcda_cals.createDataSet<float32_T>("k_bsw_zone_rear_outer_side_x", ds::From(lcda_generic->calibration.k_bsw_zone_rear_outer_side_x)).write(lcda_generic->calibration.k_bsw_zone_rear_outer_side_x));
      (lcda_cals.createDataSet<float32_T>("k_bsw_zone_rear_outer_side_y", ds::From(lcda_generic->calibration.k_bsw_zone_rear_outer_side_y)).write(lcda_generic->calibration.k_bsw_zone_rear_outer_side_y));
      (lcda_cals.createDataSet<float32_T>("k_bsw_zone_front_ego_side_x_hys", ds::From(lcda_generic->calibration.k_bsw_zone_front_ego_side_x_hys)).write(lcda_generic->calibration.k_bsw_zone_front_ego_side_x_hys));
      (lcda_cals.createDataSet<float32_T>("k_bsw_zone_front_ego_side_y_hys", ds::From(lcda_generic->calibration.k_bsw_zone_front_ego_side_y_hys)).write(lcda_generic->calibration.k_bsw_zone_front_ego_side_y_hys));
      (lcda_cals.createDataSet<float32_T>("k_bsw_zone_rear_outer_side_x_hys", ds::From(lcda_generic->calibration.k_bsw_zone_rear_outer_side_x_hys)).write(lcda_generic->calibration.k_bsw_zone_rear_outer_side_x_hys));
      (lcda_cals.createDataSet<float32_T>("k_bsw_zone_rear_outer_side_y_hys", ds::From(lcda_generic->calibration.k_bsw_zone_rear_outer_side_y_hys)).write(lcda_generic->calibration.k_bsw_zone_rear_outer_side_y_hys));
      (lcda_cals.createDataSet<float32_T>("k_lcda_pedestrian_min_size", ds::From(lcda_generic->calibration.k_lcda_pedestrian_min_size)).write(lcda_generic->calibration.k_lcda_pedestrian_min_size));
      (lcda_cals.createDataSet<float32_T>("k_lcda_pedestrian_min_speed", ds::From(lcda_generic->calibration.k_lcda_pedestrian_min_speed)).write(lcda_generic->calibration.k_lcda_pedestrian_min_speed));
      (lcda_cals.createDataSet<float32_T>("k_lcda_2wheel_min_size", ds::From(lcda_generic->calibration.k_lcda_2wheel_min_size)).write(lcda_generic->calibration.k_lcda_2wheel_min_size));
      (lcda_cals.createDataSet<float32_T>("k_lcda_2wheel_min_speed", ds::From(lcda_generic->calibration.k_lcda_2wheel_min_speed)).write(lcda_generic->calibration.k_lcda_2wheel_min_speed));
      (lcda_cals.createDataSet<float32_T>("k_bsw_object_position_correction_delay_time", ds::From(lcda_generic->calibration.k_bsw_object_position_correction_delay_time)).write(lcda_generic->calibration.k_bsw_object_position_correction_delay_time));
      (lcda_cals.createDataSet<float32_T>("k_bsw_min_speed_for_tos_scenario", ds::From(lcda_generic->calibration.k_bsw_min_speed_for_tos_scenario)).write(lcda_generic->calibration.k_bsw_min_speed_for_tos_scenario));
      (lcda_cals.createDataSet<float32_T>("k_cvw_min_object_curvi_relative_speed", ds::From(lcda_generic->calibration.k_cvw_min_object_curvi_relative_speed)).write(lcda_generic->calibration.k_cvw_min_object_curvi_relative_speed));
      (lcda_cals.createDataSet<float32_T>("k_cvw_max_object_curvi_relative_speed", ds::From(lcda_generic->calibration.k_cvw_max_object_curvi_relative_speed)).write(lcda_generic->calibration.k_cvw_max_object_curvi_relative_speed));
      (lcda_cals.createDataSet<float32_T>("k_cvw_object_curvi_relative_speed_hys", ds::From(lcda_generic->calibration.k_cvw_object_curvi_relative_speed_hys)).write(lcda_generic->calibration.k_cvw_object_curvi_relative_speed_hys));
      (lcda_cals.createDataSet<float32_T>("k_lcda_cvw_ttc_const", ds::From(lcda_generic->calibration.k_lcda_cvw_ttc_const)).write(lcda_generic->calibration.k_lcda_cvw_ttc_const));
      (lcda_cals.createDataSet<float32_T>("k_lcda_cvw_ttc_accel", ds::From(lcda_generic->calibration.k_lcda_cvw_ttc_accel)).write(lcda_generic->calibration.k_lcda_cvw_ttc_accel));
      (lcda_cals.createDataSet<float32_T>("k_lcda_dyn_cvw_ttc_speed_parameter", ds::From(lcda_generic->calibration.k_lcda_dyn_cvw_ttc_speed_parameter)).write(lcda_generic->calibration.k_lcda_dyn_cvw_ttc_speed_parameter));
      (lcda_cals.createDataSet<float32_T>("k_bsw_object_position_correction_threshold", ds::From(lcda_generic->calibration.k_bsw_object_position_correction_threshold)).write(lcda_generic->calibration.k_bsw_object_position_correction_threshold));
      (lcda_cals.createDataSet<float32_T>("k_lcda_dyn_cvw_ttc_compens_rel_vel_thresh", ds::From(lcda_generic->calibration.k_lcda_dyn_cvw_ttc_compens_rel_vel_thresh)).write(lcda_generic->calibration.k_lcda_dyn_cvw_ttc_compens_rel_vel_thresh));
      (lcda_cals.createDataSet<float32_T>("k_lcda_dyn_cvw_ttc_compens_time", ds::From(lcda_generic->calibration.k_lcda_dyn_cvw_ttc_compens_time)).write(lcda_generic->calibration.k_lcda_dyn_cvw_ttc_compens_time));
      (lcda_cals.createDataSet<float32_T>("k_bsw_dynzone_object_rel_vel", ds::From(lcda_generic->calibration.k_bsw_dynzone_object_rel_vel)).write(lcda_generic->calibration.k_bsw_dynzone_object_rel_vel));
      (lcda_cals.createDataSet<float32_T>("k_bsw_dynzone_object_range", ds::From(lcda_generic->calibration.k_bsw_dynzone_object_range)).write(lcda_generic->calibration.k_bsw_dynzone_object_range));
      (lcda_cals.createDataSet<uint16_t>("k_lm_min_count_in_state", ds::From(lcda_generic->calibration.k_lm_min_count_in_state)).write(lcda_generic->calibration.k_lm_min_count_in_state));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present", ds::From(lcda_generic->calibration.k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present)).write(lcda_generic->calibration.k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_f_zone_extension_by_diff_width_host_vs_trailer", ds::From(lcda_generic->calibration.k_bsw_f_zone_extension_by_diff_width_host_vs_trailer)).write(lcda_generic->calibration.k_bsw_f_zone_extension_by_diff_width_host_vs_trailer));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_f_enable_trailer_zone_adjustment_on_ego_side", ds::From(lcda_generic->calibration.k_bsw_f_enable_trailer_zone_adjustment_on_ego_side)).write(lcda_generic->calibration.k_bsw_f_enable_trailer_zone_adjustment_on_ego_side));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_f_enable_trailer_zone_adjustment_on_outer_side", ds::From(lcda_generic->calibration.k_bsw_f_enable_trailer_zone_adjustment_on_outer_side)).write(lcda_generic->calibration.k_bsw_f_enable_trailer_zone_adjustment_on_outer_side));
      (lcda_cals.createDataSet<boolean_T>("k_elc_enable", ds::From(lcda_generic->calibration.k_elc_enable)).write(lcda_generic->calibration.k_elc_enable));
      (lcda_cals.createDataSet<boolean_T>("k_elc_enable_via_cal", ds::From(lcda_generic->calibration.k_elc_enable_via_cal)).write(lcda_generic->calibration.k_elc_enable_via_cal));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_use_default_warntrigger_hmi", ds::From(lcda_generic->calibration.k_lcda_use_default_warntrigger_hmi)).write(lcda_generic->calibration.k_lcda_use_default_warntrigger_hmi));
      (lcda_cals.createDataSet<boolean_T>("k_lm_use_default_lane_information", ds::From(lcda_generic->calibration.k_lm_use_default_lane_information)).write(lcda_generic->calibration.k_lm_use_default_lane_information));
      (lcda_cals.createDataSet<boolean_T>("k_lm_enable_use_navigation_data", ds::From(lcda_generic->calibration.k_lm_enable_use_navigation_data)).write(lcda_generic->calibration.k_lm_enable_use_navigation_data));
      (lcda_cals.createDataSet<boolean_T>("k_lm_enable_use_camera_data", ds::From(lcda_generic->calibration.k_lm_enable_use_camera_data)).write(lcda_generic->calibration.k_lm_enable_use_camera_data));
      (lcda_cals.createDataSet<boolean_T>("k_lm_enable_use_vehicle_dyn", ds::From(lcda_generic->calibration.k_lm_enable_use_vehicle_dyn)).write(lcda_generic->calibration.k_lm_enable_use_vehicle_dyn));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_enable", ds::From(lcda_generic->calibration.k_lcda_enable)).write(lcda_generic->calibration.k_lcda_enable));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_enable_via_cal", ds::From(lcda_generic->calibration.k_lcda_enable_via_cal)).write(lcda_generic->calibration.k_lcda_enable_via_cal));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_f_disable_due_to_small_curve_radius", ds::From(lcda_generic->calibration.k_lcda_f_disable_due_to_small_curve_radius)).write(lcda_generic->calibration.k_lcda_f_disable_due_to_small_curve_radius));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_allow_track_status_new", ds::From(lcda_generic->calibration.k_lcda_allow_track_status_new)).write(lcda_generic->calibration.k_lcda_allow_track_status_new));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_f_enable_obj_in_ego_lane_check", ds::From(lcda_generic->calibration.k_lcda_f_enable_obj_in_ego_lane_check)).write(lcda_generic->calibration.k_lcda_f_enable_obj_in_ego_lane_check));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_f_enable_suppress_alert_object_no_lane_change_intention", ds::From(lcda_generic->calibration.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention)).write(lcda_generic->calibration.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge", ds::From(lcda_generic->calibration.k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge)).write(lcda_generic->calibration.k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_f_enable_adv_pos_data_lane_change_intention", ds::From(lcda_generic->calibration.k_bsw_f_enable_adv_pos_data_lane_change_intention)).write(lcda_generic->calibration.k_bsw_f_enable_adv_pos_data_lane_change_intention));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_f_enable_alert_obj_in_ego_lane", ds::From(lcda_generic->calibration.k_lcda_f_enable_alert_obj_in_ego_lane)).write(lcda_generic->calibration.k_lcda_f_enable_alert_obj_in_ego_lane));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_ego_lane_check_center_point_only", ds::From(lcda_generic->calibration.k_lcda_ego_lane_check_center_point_only)).write(lcda_generic->calibration.k_lcda_ego_lane_check_center_point_only));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_uses_cvw_alert_state_enabled", ds::From(lcda_generic->calibration.k_bsw_uses_cvw_alert_state_enabled)).write(lcda_generic->calibration.k_bsw_uses_cvw_alert_state_enabled));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_overlap_area_check_enable", ds::From(lcda_generic->calibration.k_bsw_overlap_area_check_enable)).write(lcda_generic->calibration.k_bsw_overlap_area_check_enable));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_use_curvi_coordinates", ds::From(lcda_generic->calibration.k_bsw_use_curvi_coordinates)).write(lcda_generic->calibration.k_bsw_use_curvi_coordinates));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_enable", ds::From(lcda_generic->calibration.k_bsw_enable)).write(lcda_generic->calibration.k_bsw_enable));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_enable_via_cal", ds::From(lcda_generic->calibration.k_bsw_enable_via_cal)).write(lcda_generic->calibration.k_bsw_enable_via_cal));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_enable_zone_front_boundary_specific_conditions", ds::From(lcda_generic->calibration.k_bsw_enable_zone_front_boundary_specific_conditions)).write(lcda_generic->calibration.k_bsw_enable_zone_front_boundary_specific_conditions));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_enable_specific_front_sot_conditions", ds::From(lcda_generic->calibration.k_bsw_enable_specific_front_sot_conditions)).write(lcda_generic->calibration.k_bsw_enable_specific_front_sot_conditions));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_hold_alert_long_object", ds::From(lcda_generic->calibration.k_bsw_hold_alert_long_object)).write(lcda_generic->calibration.k_bsw_hold_alert_long_object));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_enable_factor_based_host_speed_adjustment", ds::From(lcda_generic->calibration.k_bsw_enable_factor_based_host_speed_adjustment)).write(lcda_generic->calibration.k_bsw_enable_factor_based_host_speed_adjustment));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_enable_dynspeed_zone", ds::From(lcda_generic->calibration.k_bsw_enable_dynspeed_zone)).write(lcda_generic->calibration.k_bsw_enable_dynspeed_zone));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_enable_trailer_zone_extension", ds::From(lcda_generic->calibration.k_bsw_enable_trailer_zone_extension)).write(lcda_generic->calibration.k_bsw_enable_trailer_zone_extension));
      (lcda_cals.createDataSet<boolean_T>("k_cvw_enable", ds::From(lcda_generic->calibration.k_cvw_enable)).write(lcda_generic->calibration.k_cvw_enable));
      (lcda_cals.createDataSet<boolean_T>("k_cvw_enable_via_cal", ds::From(lcda_generic->calibration.k_cvw_enable_via_cal)).write(lcda_generic->calibration.k_cvw_enable_via_cal));
      (lcda_cals.createDataSet<boolean_T>("k_enable_cvw_curve_zone_adaptation", ds::From(lcda_generic->calibration.k_enable_cvw_curve_zone_adaptation)).write(lcda_generic->calibration.k_enable_cvw_curve_zone_adaptation));
      (lcda_cals.createDataSet<boolean_T>("k_cvw_f_most_crit_obj_must_be_closest_relevant_obj", ds::From(lcda_generic->calibration.k_cvw_f_most_crit_obj_must_be_closest_relevant_obj)).write(lcda_generic->calibration.k_cvw_f_most_crit_obj_must_be_closest_relevant_obj));
      (lcda_cals.createDataSet<boolean_T>("k_slc_enable", ds::From(lcda_generic->calibration.k_slc_enable)).write(lcda_generic->calibration.k_slc_enable));
      (lcda_cals.createDataSet<boolean_T>("k_slc_enable_via_cal", ds::From(lcda_generic->calibration.k_slc_enable_via_cal)).write(lcda_generic->calibration.k_slc_enable_via_cal));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_f_enable_rel_vel_logic_in_ego_lane_check", ds::From(lcda_generic->calibration.k_lcda_f_enable_rel_vel_logic_in_ego_lane_check)).write(lcda_generic->calibration.k_lcda_f_enable_rel_vel_logic_in_ego_lane_check));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_f_fallback_default_status_slow", ds::From(lcda_generic->calibration.k_bsw_f_fallback_default_status_slow)).write(lcda_generic->calibration.k_bsw_f_fallback_default_status_slow));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_f_enable_obj_reflection_flag_check", ds::From(lcda_generic->calibration.k_lcda_f_enable_obj_reflection_flag_check)).write(lcda_generic->calibration.k_lcda_f_enable_obj_reflection_flag_check));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_f_enable_fallback_handler", ds::From(lcda_generic->calibration.k_lcda_f_enable_fallback_handler)).write(lcda_generic->calibration.k_lcda_f_enable_fallback_handler));
      (lcda_cals.createDataSet<boolean_T>("k_lcda_f_enable_dyn_cvw_ttc_threshold", ds::From(lcda_generic->calibration.k_lcda_f_enable_dyn_cvw_ttc_threshold)).write(lcda_generic->calibration.k_lcda_f_enable_dyn_cvw_ttc_threshold));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_shrink_zone_method", ds::From(lcda_generic->calibration.k_bsw_shrink_zone_method)).write(lcda_generic->calibration.k_bsw_shrink_zone_method));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_f_use_front_zone_as_n_line", ds::From(lcda_generic->calibration.k_bsw_f_use_front_zone_as_n_line)).write(lcda_generic->calibration.k_bsw_f_use_front_zone_as_n_line));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_f_enable_object_rel_vel_dynzone", ds::From(lcda_generic->calibration.k_bsw_f_enable_object_rel_vel_dynzone)).write(lcda_generic->calibration.k_bsw_f_enable_object_rel_vel_dynzone));
      (lcda_cals.createDataSet<boolean_T>("k_bsw_f_use_zone_without_hysteresis_lane_change_intention", ds::From(lcda_generic->calibration.k_bsw_f_use_zone_without_hysteresis_lane_change_intention)).write(lcda_generic->calibration.k_bsw_f_use_zone_without_hysteresis_lane_change_intention));
      (lcda_cals.createDataSet<uint8_t>("k_elc_alert_holding_cycles", ds::From(lcda_generic->calibration.k_elc_alert_holding_cycles)).write(lcda_generic->calibration.k_elc_alert_holding_cycles));
      (lcda_cals.createDataSet<uint8_t>("k_lcda_turn_signal_coast_cycles", ds::From(lcda_generic->calibration.k_lcda_turn_signal_coast_cycles)).write(lcda_generic->calibration.k_lcda_turn_signal_coast_cycles));
      (lcda_cals.createDataSet<uint8_t>("k_lcda_lc_intention_cycles_for_zone_change_threshold", ds::From(lcda_generic->calibration.k_lcda_lc_intention_cycles_for_zone_change_threshold)).write(lcda_generic->calibration.k_lcda_lc_intention_cycles_for_zone_change_threshold));
      (lcda_cals.createDataSet<uint8_t>("k_cvw_min_mature_cycles_lc_intention", ds::From(lcda_generic->calibration.k_cvw_min_mature_cycles_lc_intention)).write(lcda_generic->calibration.k_cvw_min_mature_cycles_lc_intention));
      (lcda_cals.createDataSet<uint8_t>("k_elc_min_mature_cycles", ds::From(lcda_generic->calibration.k_elc_min_mature_cycles)).write(lcda_generic->calibration.k_elc_min_mature_cycles));
      (lcda_cals.createDataSet<uint8_t>("k_lcda_default_warntrigger_hmi", ds::From(lcda_generic->calibration.k_lcda_default_warntrigger_hmi)).write(lcda_generic->calibration.k_lcda_default_warntrigger_hmi));
      (lcda_cals.createDataSet<uint8_t>("k_lm_min_qualification_cycle_count", ds::From(lcda_generic->calibration.k_lm_min_qualification_cycle_count)).write(lcda_generic->calibration.k_lm_min_qualification_cycle_count));
      (lcda_cals.createDataSet<uint8_t>("k_lm_min_output_hold_cycles", ds::From(lcda_generic->calibration.k_lm_min_output_hold_cycles)).write(lcda_generic->calibration.k_lm_min_output_hold_cycles));
      (lcda_cals.createDataSet<uint8_t>("k_unused_padding_byte_0", ds::From(lcda_generic->calibration.k_unused_padding_byte_0)).write(lcda_generic->calibration.k_unused_padding_byte_0));
      (lcda_cals.createDataSet<uint8_t>("k_bsw_alert_holding_cycles", ds::From(lcda_generic->calibration.k_bsw_alert_holding_cycles)).write(lcda_generic->calibration.k_bsw_alert_holding_cycles));
      (lcda_cals.createDataSet<uint8_t>("k_cvw_alert_holding_cycles", ds::From(lcda_generic->calibration.k_cvw_alert_holding_cycles)).write(lcda_generic->calibration.k_cvw_alert_holding_cycles));
      (lcda_cals.createDataSet<uint8_t>("k_slc_alert_holding_cycles", ds::From(lcda_generic->calibration.k_slc_alert_holding_cycles)).write(lcda_generic->calibration.k_slc_alert_holding_cycles));
      (lcda_cals.createDataSet<uint8_t>("k_lcda_zone_check_method", ds::From(lcda_generic->calibration.k_lcda_zone_check_method)).write(lcda_generic->calibration.k_lcda_zone_check_method));
      (lcda_cals.createDataSet<uint8_t>("k_bsw_fallback_fast_to_slow_qual_thres", ds::From(lcda_generic->calibration.k_bsw_fallback_fast_to_slow_qual_thres)).write(lcda_generic->calibration.k_bsw_fallback_fast_to_slow_qual_thres));
      (lcda_cals.createDataSet<uint8_t>("k_bsw_min_mature_cycles", ds::From(lcda_generic->calibration.k_bsw_min_mature_cycles)).write(lcda_generic->calibration.k_bsw_min_mature_cycles));
      (lcda_cals.createDataSet<uint8_t>("k_lcda_min_track_age", ds::From(lcda_generic->calibration.k_lcda_min_track_age)).write(lcda_generic->calibration.k_lcda_min_track_age));
      (lcda_cals.createDataSet<uint8_t>("k_bsw_alert_track_age", ds::From(lcda_generic->calibration.k_bsw_alert_track_age)).write(lcda_generic->calibration.k_bsw_alert_track_age));
      (lcda_cals.createDataSet<uint8_t>("k_bsw_stop_alert_reaching_front_custom_limit_mode", ds::From(lcda_generic->calibration.k_bsw_stop_alert_reaching_front_custom_limit_mode)).write(lcda_generic->calibration.k_bsw_stop_alert_reaching_front_custom_limit_mode));
      (lcda_cals.createDataSet<uint8_t>("k_cvw_min_mature_cycles", ds::From(lcda_generic->calibration.k_cvw_min_mature_cycles)).write(lcda_generic->calibration.k_cvw_min_mature_cycles));
      (lcda_cals.createDataSet<uint8_t>("k_cvw_zone_calculation_mode", ds::From(lcda_generic->calibration.k_cvw_zone_calculation_mode)).write(lcda_generic->calibration.k_cvw_zone_calculation_mode));
      (lcda_cals.createDataSet<uint8_t>("k_slc_min_mature_cycles", ds::From(lcda_generic->calibration.k_slc_min_mature_cycles)).write(lcda_generic->calibration.k_slc_min_mature_cycles));
      (lcda_cals.createDataSet<uint8_t>("k_slc_alert_qualifying_counter", ds::From(lcda_generic->calibration.k_slc_alert_qualifying_counter)).write(lcda_generic->calibration.k_slc_alert_qualifying_counter));
      (lcda_cals.createDataSet<uint8_t>("k_lcda_f_enable_camera_based_guardrail", ds::From(lcda_generic->calibration.k_lcda_f_enable_camera_based_guardrail)).write(lcda_generic->calibration.k_lcda_f_enable_camera_based_guardrail));

      // Write CED feature output
      auto ced = g5.createGroup("CED");
      (ced.createDataSet<boolean_T>("f_ced_enable", ds::From(hdf_ced_ptr.f_ced_enable), props1D).write(hdf_ced_ptr.f_ced_enable));
      (ced.createDataSet<int>("ced_alert_left", ds::From(hdf_ced_ptr.ced_alert_left), props1D).write(hdf_ced_ptr.ced_alert_left));
      (ced.createDataSet<int>("ced_alert_right", ds::From(hdf_ced_ptr.ced_alert_right), props1D).write(hdf_ced_ptr.ced_alert_right));
      (ced.createDataSet<uint8_t>("ced_object_ced_id_left", ds::From(hdf_ced_ptr.ced_object_ced_id_left), props1D).write(hdf_ced_ptr.ced_object_ced_id_left));
      (ced.createDataSet<uint8_t>("ced_object_unique_id_left", ds::From(hdf_ced_ptr.ced_object_unique_id_left), props1D).write(hdf_ced_ptr.ced_object_unique_id_left));
      (ced.createDataSet<int>("ced_object_type_left", ds::From(hdf_ced_ptr.ced_object_type_left), props1D).write(hdf_ced_ptr.ced_object_type_left));
      (ced.createDataSet<float32_T>("ced_object_length_m_left", ds::From(hdf_ced_ptr.ced_object_length_m_left), props1D).write(hdf_ced_ptr.ced_object_length_m_left));
      (ced.createDataSet<float32_T>("ced_object_width_m_left", ds::From(hdf_ced_ptr.ced_object_width_m_left), props1D).write(hdf_ced_ptr.ced_object_width_m_left));
      (ced.createDataSet<float32_T>("ced_object_lat_pos_m_left", ds::From(hdf_ced_ptr.ced_object_lat_pos_m_left), props1D).write(hdf_ced_ptr.ced_object_lat_pos_m_left));
      (ced.createDataSet<float32_T>("ced_object_long_pos_m_left", ds::From(hdf_ced_ptr.ced_object_long_pos_m_left), props1D).write(hdf_ced_ptr.ced_object_long_pos_m_left));
      (ced.createDataSet<float32_T>("ced_object_speed_mps_left", ds::From(hdf_ced_ptr.ced_object_speed_mps_left), props1D).write(hdf_ced_ptr.ced_object_speed_mps_left));
      (ced.createDataSet<float32_T>("ced_object_heading_rad_left", ds::From(hdf_ced_ptr.ced_object_heading_rad_left), props1D).write(hdf_ced_ptr.ced_object_heading_rad_left));
      (ced.createDataSet<int>("ced_object_direction_left", ds::From(hdf_ced_ptr.ced_object_direction_left), props1D).write(hdf_ced_ptr.ced_object_direction_left));
      (ced.createDataSet<float32_T>("ced_object_predicted_lat_pos_m_left", ds::From(hdf_ced_ptr.ced_object_predicted_lat_pos_m_left), props1D).write(hdf_ced_ptr.ced_object_predicted_lat_pos_m_left));
      (ced.createDataSet<float32_T>("ced_object_ttc_s_left", ds::From(hdf_ced_ptr.ced_object_ttc_s_left), props1D).write(hdf_ced_ptr.ced_object_ttc_s_left));
      (ced.createDataSet<float32_T>("ced_object_ttp_s_left", ds::From(hdf_ced_ptr.ced_object_ttp_s_left), props1D).write(hdf_ced_ptr.ced_object_ttp_s_left));
      (ced.createDataSet<uint8_t>("ced_object_ced_id_right", ds::From(hdf_ced_ptr.ced_object_ced_id_right), props1D).write(hdf_ced_ptr.ced_object_ced_id_right));
      (ced.createDataSet<uint8_t>("ced_object_unique_id_right", ds::From(hdf_ced_ptr.ced_object_unique_id_right), props1D).write(hdf_ced_ptr.ced_object_unique_id_right));
      (ced.createDataSet<int>("ced_object_type_right", ds::From(hdf_ced_ptr.ced_object_type_right), props1D).write(hdf_ced_ptr.ced_object_type_right));
      (ced.createDataSet<float32_T>("ced_object_length_m_right", ds::From(hdf_ced_ptr.ced_object_length_m_right), props1D).write(hdf_ced_ptr.ced_object_length_m_right));
      (ced.createDataSet<float32_T>("ced_object_width_m_right", ds::From(hdf_ced_ptr.ced_object_width_m_right), props1D).write(hdf_ced_ptr.ced_object_width_m_right));
      (ced.createDataSet<float32_T>("ced_object_lat_pos_m_right", ds::From(hdf_ced_ptr.ced_object_lat_pos_m_right), props1D).write(hdf_ced_ptr.ced_object_lat_pos_m_right));
      (ced.createDataSet<float32_T>("ced_object_long_pos_m_right", ds::From(hdf_ced_ptr.ced_object_long_pos_m_right), props1D).write(hdf_ced_ptr.ced_object_long_pos_m_right));
      (ced.createDataSet<float32_T>("ced_object_speed_mps_right", ds::From(hdf_ced_ptr.ced_object_speed_mps_right), props1D).write(hdf_ced_ptr.ced_object_speed_mps_right));
      (ced.createDataSet<float32_T>("ced_object_heading_rad_right", ds::From(hdf_ced_ptr.ced_object_heading_rad_right), props1D).write(hdf_ced_ptr.ced_object_heading_rad_right));
      (ced.createDataSet<int>("ced_object_direction_right", ds::From(hdf_ced_ptr.ced_object_direction_right), props1D).write(hdf_ced_ptr.ced_object_direction_right));
      (ced.createDataSet<float32_T>("ced_object_predicted_lat_pos_m_right", ds::From(hdf_ced_ptr.ced_object_predicted_lat_pos_m_right), props1D).write(hdf_ced_ptr.ced_object_predicted_lat_pos_m_right));
      (ced.createDataSet<float32_T>("ced_object_ttc_s_right", ds::From(hdf_ced_ptr.ced_object_ttc_s_right), props1D).write(hdf_ced_ptr.ced_object_ttc_s_right));

      // CED Calibrations
      Ced_Instance_T *ced_generic = Ced_Get_Instance_Ptr();
      auto ced_cals               = ced.createGroup("Calibration");
      (ced_cals.createDataSet<float32_T>("k_ced_object_acceleration_weight", ds::From(ced_generic->calibration.k_ced_object_acceleration_weight)).write(ced_generic->calibration.k_ced_object_acceleration_weight));
      (ced_cals.createDataSet<float32_T>("k_ced_object_heading_predicted_weight", ds::From(ced_generic->calibration.k_ced_object_heading_predicted_weight)).write(ced_generic->calibration.k_ced_object_heading_predicted_weight));
      (ced_cals.createDataSet<float32_T>("k_ced_object_existence_probability_min", ds::From(ced_generic->calibration.k_ced_object_existence_probability_min)).write(ced_generic->calibration.k_ced_object_existence_probability_min));
      (ced_cals.createDataSet<float32_T>("k_ced_object_heading_abs_angle_max", ds::From(ced_generic->calibration.k_ced_object_heading_abs_angle_max)).write(ced_generic->calibration.k_ced_object_heading_abs_angle_max));
      (ced_cals.createDataSet<float32_T>("k_ced_object_long_vel_rel_min", ds::From(ced_generic->calibration.k_ced_object_long_vel_rel_min)).write(ced_generic->calibration.k_ced_object_long_vel_rel_min));
      (ced_cals.createDataSet<float32_T>("k_ced_object_long_vel_min", ds::From(ced_generic->calibration.k_ced_object_long_vel_min)).write(ced_generic->calibration.k_ced_object_long_vel_min));
      (ced_cals.createDataSet<float32_T>("k_ced_object_lat_vel_max", ds::From(ced_generic->calibration.k_ced_object_lat_vel_max)).write(ced_generic->calibration.k_ced_object_lat_vel_max));
      (ced_cals.createDataSet<float32_T>("k_ced_object_max_width_increase_factor_with_path_match", ds::From(ced_generic->calibration.k_ced_object_max_width_increase_factor_with_path_match)).write(ced_generic->calibration.k_ced_object_max_width_increase_factor_with_path_match));
      (ced_cals.createDataSet<float32_T>("k_ced_object_max_width_increase_factor_without_path_match", ds::From(ced_generic->calibration.k_ced_object_max_width_increase_factor_without_path_match)).write(ced_generic->calibration.k_ced_object_max_width_increase_factor_without_path_match));
      (ced_cals.createDataSet<float32_T>("k_ced_object_heading_exp_moving_average_alpha", ds::From(ced_generic->calibration.k_ced_object_heading_exp_moving_average_alpha)).write(ced_generic->calibration.k_ced_object_heading_exp_moving_average_alpha));
      (ced_cals.createDataSet<float32_T>("k_ced_object_ftm_existence_probability_min", ds::From(ced_generic->calibration.k_ced_object_ftm_existence_probability_min)).write(ced_generic->calibration.k_ced_object_ftm_existence_probability_min));
      (ced_cals.createDataSet<float32_T>("k_ced_object_ftm_heading_abs_angle_min", ds::From(ced_generic->calibration.k_ced_object_ftm_heading_abs_angle_min)).write(ced_generic->calibration.k_ced_object_ftm_heading_abs_angle_min));
      (ced_cals.createDataSet<float32_T>("k_ced_object_ftm_long_vel_rel_min", ds::From(ced_generic->calibration.k_ced_object_ftm_long_vel_rel_min)).write(ced_generic->calibration.k_ced_object_ftm_long_vel_rel_min));
      (ced_cals.createDataSet<float32_T>("k_ced_object_ftm_long_vel_min", ds::From(ced_generic->calibration.k_ced_object_ftm_long_vel_min)).write(ced_generic->calibration.k_ced_object_ftm_long_vel_min));
      (ced_cals.createDataSet<float32_T>("k_ced_object_ftm_lat_vel_max", ds::From(ced_generic->calibration.k_ced_object_ftm_lat_vel_max)).write(ced_generic->calibration.k_ced_object_ftm_lat_vel_max));
      (ced_cals.createDataSet<float32_T>("k_ced_first_warning_ttc_threshold", ds::From(ced_generic->calibration.k_ced_first_warning_ttc_threshold)).write(ced_generic->calibration.k_ced_first_warning_ttc_threshold));
      (ced_cals.createDataSet<float32_T>("k_ced_second_warning_ttc_threshold", ds::From(ced_generic->calibration.k_ced_second_warning_ttc_threshold)).write(ced_generic->calibration.k_ced_second_warning_ttc_threshold));
      (ced_cals.createDataSet<float32_T>("k_ced_third_warning_ttc_threshold", ds::From(ced_generic->calibration.k_ced_third_warning_ttc_threshold)).write(ced_generic->calibration.k_ced_third_warning_ttc_threshold));
      (ced_cals.createDataSet<float32_T>("k_ced_second_warning_pred_lat_dist_max", ds::From(ced_generic->calibration.k_ced_second_warning_pred_lat_dist_max)).write(ced_generic->calibration.k_ced_second_warning_pred_lat_dist_max));
      (ced_cals.createDataSet<float32_T>("k_ced_third_warning_pred_lat_dist_max", ds::From(ced_generic->calibration.k_ced_third_warning_pred_lat_dist_max)).write(ced_generic->calibration.k_ced_third_warning_pred_lat_dist_max));
      (ced_cals.createDataSet<float32_T>("k_ced_alert_ttp_min", ds::From(ced_generic->calibration.k_ced_alert_ttp_min)).write(ced_generic->calibration.k_ced_alert_ttp_min));
      (ced_cals.createDataSet<float32_T>("k_ced_ego_abs_speed_max", ds::From(ced_generic->calibration.k_ced_ego_abs_speed_max)).write(ced_generic->calibration.k_ced_ego_abs_speed_max));
      (ced_cals.createDataSet<float32_T>("k_ced_crash_line_host_length_percentage", ds::From(ced_generic->calibration.k_ced_crash_line_host_length_percentage)).write(ced_generic->calibration.k_ced_crash_line_host_length_percentage));
      (ced_cals.createDataSet<float32_T>("k_ced_object_width_safety_margin_for_active_alert", ds::From(ced_generic->calibration.k_ced_object_width_safety_margin_for_active_alert)).write(ced_generic->calibration.k_ced_object_width_safety_margin_for_active_alert));
      (ced_cals.createDataSet<float32_T>("k_ced_object_width_safety_margin_for_critical_path_match", ds::From(ced_generic->calibration.k_ced_object_width_safety_margin_for_critical_path_match)).write(ced_generic->calibration.k_ced_object_width_safety_margin_for_critical_path_match));
      (ced_cals.createDataSet<float32_T>("k_ced_object_min_dist_to_crash_line_for_path_match", ds::From(ced_generic->calibration.k_ced_object_min_dist_to_crash_line_for_path_match)).write(ced_generic->calibration.k_ced_object_min_dist_to_crash_line_for_path_match));
      (ced_cals.createDataSet<float32_T>("k_ced_offset_to_path_weight", ds::From(ced_generic->calibration.k_ced_offset_to_path_weight)).write(ced_generic->calibration.k_ced_offset_to_path_weight));
      (ced_cals.createDataSet<float32_T>("k_ced_collision_zone_width", ds::From(ced_generic->calibration.k_ced_collision_zone_width)).write(ced_generic->calibration.k_ced_collision_zone_width));
      (ced_cals.createDataSet<float32_T>("k_ced_funnel_zone_length", ds::From(ced_generic->calibration.k_ced_funnel_zone_length)).write(ced_generic->calibration.k_ced_funnel_zone_length));
      (ced_cals.createDataSet<float32_T>("k_ced_funnel_zone_width", ds::From(ced_generic->calibration.k_ced_funnel_zone_width)).write(ced_generic->calibration.k_ced_funnel_zone_width));
      (ced_cals.createDataSet<float32_T>("k_ced_slow_objects_long_vel_max", ds::From(ced_generic->calibration.k_ced_slow_objects_long_vel_max)).write(ced_generic->calibration.k_ced_slow_objects_long_vel_max));
      (ced_cals.createDataSet<float32_T>("k_ced_ego_lane_width", ds::From(ced_generic->calibration.k_ced_ego_lane_width)).write(ced_generic->calibration.k_ced_ego_lane_width));
      (ced_cals.createDataSet<float32_T>("k_ced_ego_lane_parking_range", ds::From(ced_generic->calibration.k_ced_ego_lane_parking_range)).write(ced_generic->calibration.k_ced_ego_lane_parking_range));
      (ced_cals.createDataSet<float32_T>("k_ced_ego_lane_parking_maneuver_speed", ds::From(ced_generic->calibration.k_ced_ego_lane_parking_maneuver_speed)).write(ced_generic->calibration.k_ced_ego_lane_parking_maneuver_speed));
      (ced_cals.createDataSet<float32_T>("k_ced_alert_holding_obj_abs_heading_max", ds::From(ced_generic->calibration.k_ced_alert_holding_obj_abs_heading_max)).write(ced_generic->calibration.k_ced_alert_holding_obj_abs_heading_max));
      (ced_cals.createDataSet<float32_T>("k_ced_alert_holding_obj_long_vel_min", ds::From(ced_generic->calibration.k_ced_alert_holding_obj_long_vel_min)).write(ced_generic->calibration.k_ced_alert_holding_obj_long_vel_min));
      (ced_cals.createDataSet<float32_T>("k_ced_suppress_pt_heading_diff_ced_alert_max", ds::From(ced_generic->calibration.k_ced_suppress_pt_heading_diff_ced_alert_max)).write(ced_generic->calibration.k_ced_suppress_pt_heading_diff_ced_alert_max));
      (ced_cals.createDataSet<float32_T>("k_ced_suppress_range_to_nearest_path_max", ds::From(ced_generic->calibration.k_ced_suppress_range_to_nearest_path_max)).write(ced_generic->calibration.k_ced_suppress_range_to_nearest_path_max));
      (ced_cals.createDataSet<float32_T>("k_ced_object_long_vel_rel_max", ds::From(ced_generic->calibration.k_ced_object_long_vel_rel_max)).write(ced_generic->calibration.k_ced_object_long_vel_rel_max));
      (ced_cals.createDataSet<float32_T>("k_ced_object_ftm_long_vel_rel_max", ds::From(ced_generic->calibration.k_ced_object_ftm_long_vel_rel_max)).write(ced_generic->calibration.k_ced_object_ftm_long_vel_rel_max));
      (ced_cals.createDataSet<float32_T>("k_ced_object_vel_max", ds::From(ced_generic->calibration.k_ced_object_vel_max)).write(ced_generic->calibration.k_ced_object_vel_max));
      (ced_cals.createDataSet<float32_T>("k_ced_slight_turn_lat_vel_table", ds::From(ced_generic->calibration.k_ced_slight_turn_lat_vel_table)).write(ced_generic->calibration.k_ced_slight_turn_lat_vel_table));
      (ced_cals.createDataSet<float32_T>("k_ced_slight_turn_position_limits", ds::From(ced_generic->calibration.k_ced_slight_turn_position_limits)).write(ced_generic->calibration.k_ced_slight_turn_position_limits));
      (ced_cals.createDataSet<float32_T>("k_ced_lat_pos_of_border", ds::From(ced_generic->calibration.k_ced_lat_pos_of_border)).write(ced_generic->calibration.k_ced_lat_pos_of_border));
      (ced_cals.createDataSet<float32_T>("k_ced_lat_pos_max_shift", ds::From(ced_generic->calibration.k_ced_lat_pos_max_shift)).write(ced_generic->calibration.k_ced_lat_pos_max_shift));
      (ced_cals.createDataSet<float32_T>("k_ced_lat_pos_shift_long_dist_thresholds", ds::From(ced_generic->calibration.k_ced_lat_pos_shift_long_dist_thresholds)).write(ced_generic->calibration.k_ced_lat_pos_shift_long_dist_thresholds));
      (ced_cals.createDataSet<float32_T>("k_ced_lat_pos_shift_lat_dist_thresholds", ds::From(ced_generic->calibration.k_ced_lat_pos_shift_lat_dist_thresholds)).write(ced_generic->calibration.k_ced_lat_pos_shift_lat_dist_thresholds));
      (ced_cals.createDataSet<float32_T>("k_bmw_ced_speed_max_hysteresis", ds::From(ced_generic->calibration.k_bmw_ced_speed_max_hysteresis)).write(ced_generic->calibration.k_bmw_ced_speed_max_hysteresis));
      (ced_cals.createDataSet<float32_T>("k_ced_object_predicted_max_width_slope_reduce_factor", ds::From(ced_generic->calibration.k_ced_object_predicted_max_width_slope_reduce_factor)).write(ced_generic->calibration.k_ced_object_predicted_max_width_slope_reduce_factor));
      (ced_cals.createDataSet<float32_T>("k_ced_object_predicted_max_width_slope_offset", ds::From(ced_generic->calibration.k_ced_object_predicted_max_width_slope_offset)).write(ced_generic->calibration.k_ced_object_predicted_max_width_slope_offset));
      (ced_cals.createDataSet<float32_T>("k_ced_lat_pos_shift_width_thresh", ds::From(ced_generic->calibration.k_ced_lat_pos_shift_width_thresh)).write(ced_generic->calibration.k_ced_lat_pos_shift_width_thresh));
      (ced_cals.createDataSet<float32_T>("k_ced_warning_pred_lat_dist_max_histeresis", ds::From(ced_generic->calibration.k_ced_warning_pred_lat_dist_max_histeresis)).write(ced_generic->calibration.k_ced_warning_pred_lat_dist_max_histeresis));
      (ced_cals.createDataSet<boolean_T>("k_ced_f_enable_heading_exp_moving_average", ds::From(ced_generic->calibration.k_ced_f_enable_heading_exp_moving_average)).write(ced_generic->calibration.k_ced_f_enable_heading_exp_moving_average));
      (ced_cals.createDataSet<boolean_T>("k_ced_f_second_warning_level_enable", ds::From(ced_generic->calibration.k_ced_f_second_warning_level_enable)).write(ced_generic->calibration.k_ced_f_second_warning_level_enable));
      (ced_cals.createDataSet<boolean_T>("k_ced_f_third_warning_level_enable", ds::From(ced_generic->calibration.k_ced_f_third_warning_level_enable)).write(ced_generic->calibration.k_ced_f_third_warning_level_enable));
      (ced_cals.createDataSet<boolean_T>("k_ced_f_suppress_alert_holding_for_uncritical_objects", ds::From(ced_generic->calibration.k_ced_f_suppress_alert_holding_for_uncritical_objects)).write(ced_generic->calibration.k_ced_f_suppress_alert_holding_for_uncritical_objects));
      (ced_cals.createDataSet<boolean_T>("k_ced_f_suppress_alert_holding_for_obj_below_min_ttp", ds::From(ced_generic->calibration.k_ced_f_suppress_alert_holding_for_obj_below_min_ttp)).write(ced_generic->calibration.k_ced_f_suppress_alert_holding_for_obj_below_min_ttp));
      (ced_cals.createDataSet<boolean_T>("k_ced_f_path_tracking_enable", ds::From(ced_generic->calibration.k_ced_f_path_tracking_enable)).write(ced_generic->calibration.k_ced_f_path_tracking_enable));
      (ced_cals.createDataSet<boolean_T>("k_ced_f_handle_both_side_alerts_as_object_side", ds::From(ced_generic->calibration.k_ced_f_handle_both_side_alerts_as_object_side)).write(ced_generic->calibration.k_ced_f_handle_both_side_alerts_as_object_side));
      (ced_cals.createDataSet<boolean_T>("k_ced_f_allow_ego_lane_alerts", ds::From(ced_generic->calibration.k_ced_f_allow_ego_lane_alerts)).write(ced_generic->calibration.k_ced_f_allow_ego_lane_alerts));
      (ced_cals.createDataSet<boolean_T>("k_ced_f_use_only_mature_paths", ds::From(ced_generic->calibration.k_ced_f_use_only_mature_paths)).write(ced_generic->calibration.k_ced_f_use_only_mature_paths));
      (ced_cals.createDataSet<boolean_T>("k_ced_f_allow_coasted_object_alerts", ds::From(ced_generic->calibration.k_ced_f_allow_coasted_object_alerts)).write(ced_generic->calibration.k_ced_f_allow_coasted_object_alerts));
      (ced_cals.createDataSet<boolean_T>("k_ced_f_adapt_heading_ego_lane", ds::From(ced_generic->calibration.k_ced_f_adapt_heading_ego_lane)).write(ced_generic->calibration.k_ced_f_adapt_heading_ego_lane));
      (ced_cals.createDataSet<boolean_T>("k_ced_f_object_lat_on_one_side_of_border", ds::From(ced_generic->calibration.k_ced_f_object_lat_on_one_side_of_border)).write(ced_generic->calibration.k_ced_f_object_lat_on_one_side_of_border));
      (ced_cals.createDataSet<boolean_T>("k_ced_lat_pos_shift_enable", ds::From(ced_generic->calibration.k_ced_lat_pos_shift_enable)).write(ced_generic->calibration.k_ced_lat_pos_shift_enable));
      (ced_cals.createDataSet<uint8_t>("k_ced_alert_qualifying_cycles", ds::From(ced_generic->calibration.k_ced_alert_qualifying_cycles)).write(ced_generic->calibration.k_ced_alert_qualifying_cycles));
      (ced_cals.createDataSet<uint8_t>("k_ced_alert_qualifying_cycles_slow_objects", ds::From(ced_generic->calibration.k_ced_alert_qualifying_cycles_slow_objects)).write(ced_generic->calibration.k_ced_alert_qualifying_cycles_slow_objects));
      (ced_cals.createDataSet<uint8_t>("k_ced_alert_holding_cycles", ds::From(ced_generic->calibration.k_ced_alert_holding_cycles)).write(ced_generic->calibration.k_ced_alert_holding_cycles));
      (ced_cals.createDataSet<uint8_t>("k_ced_allow_opposite_side_alerts", ds::From(ced_generic->calibration.k_ced_allow_opposite_side_alerts)).write(ced_generic->calibration.k_ced_allow_opposite_side_alerts));
      (ced_cals.createDataSet<uint8_t>("k_ced_suppress_alert_object_age_max", ds::From(ced_generic->calibration.k_ced_suppress_alert_object_age_max)).write(ced_generic->calibration.k_ced_suppress_alert_object_age_max));
      (ced_cals.createDataSet<uint8_t>("k_ced_min_cycles_for_path_match_for_no_suppress", ds::From(ced_generic->calibration.k_ced_min_cycles_for_path_match_for_no_suppress)).write(ced_generic->calibration.k_ced_min_cycles_for_path_match_for_no_suppress));
      (ced_cals.createDataSet<uint8_t>("k_ced_object_age_min", ds::From(ced_generic->calibration.k_ced_object_age_min)).write(ced_generic->calibration.k_ced_object_age_min));
      (ced_cals.createDataSet<uint8_t>("k_ced_object_ftm_age_min", ds::From(ced_generic->calibration.k_ced_object_ftm_age_min)).write(ced_generic->calibration.k_ced_object_ftm_age_min));
      (ced_cals.createDataSet<uint8_t>("k_ced_f_choose_ref_point_funnel_check", ds::From(ced_generic->calibration.k_ced_f_choose_ref_point_funnel_check)).write(ced_generic->calibration.k_ced_f_choose_ref_point_funnel_check));

      auto esa = g5.createGroup("ESA");
      (esa.createDataSet<int>("esa_status", ds::From(hdf_esa_ptr.esa_status), props1D).write(hdf_esa_ptr.esa_status));
      (esa.createDataSet<boolean_T>("f_esa_alert_left", ds::From(hdf_esa_ptr.f_esa_alert_left), props1D).write(hdf_esa_ptr.f_esa_alert_left));
      (esa.createDataSet<boolean_T>("f_esa_alert_right", ds::From(hdf_esa_ptr.f_esa_alert_right), props1D).write(hdf_esa_ptr.f_esa_alert_right));
      (esa.createDataSet<uint8_t>("esa_object_id_left", ds::From(hdf_esa_ptr.esa_object_id_left), props1D).write(hdf_esa_ptr.esa_object_id_left));
      (esa.createDataSet<uint8_t>("esa_object_index_left", ds::From(hdf_esa_ptr.esa_object_index_left), props1D).write(hdf_esa_ptr.esa_object_index_left));
      (esa.createDataSet<float32_T>("esa_object_width_m_left", ds::From(hdf_esa_ptr.esa_object_width_m_left), props1D).write(hdf_esa_ptr.esa_object_width_m_left));
      (esa.createDataSet<float32_T>("esa_object_length_m_left", ds::From(hdf_esa_ptr.esa_object_length_m_left), props1D).write(hdf_esa_ptr.esa_object_length_m_left));
      (esa.createDataSet<float32_T>("esa_object_long_pos_m_left", ds::From(hdf_esa_ptr.esa_object_long_pos_m_left), props1D).write(hdf_esa_ptr.esa_object_long_pos_m_left));
      (esa.createDataSet<float32_T>("esa_object_lat_pos_m_left", ds::From(hdf_esa_ptr.esa_object_lat_pos_m_left), props1D).write(hdf_esa_ptr.esa_object_lat_pos_m_left));
      (esa.createDataSet<float32_T>("esa_object_long_speed_mps_left", ds::From(hdf_esa_ptr.esa_object_long_speed_mps_left), props1D).write(hdf_esa_ptr.esa_object_long_speed_mps_left));
      (esa.createDataSet<float32_T>("esa_object_lat_speed_mps_left", ds::From(hdf_esa_ptr.esa_object_lat_speed_mps_left), props1D).write(hdf_esa_ptr.esa_object_lat_speed_mps_left));
      (esa.createDataSet<float32_T>("esa_object_ttc_s_left", ds::From(hdf_esa_ptr.esa_object_ttc_s_left), props1D).write(hdf_esa_ptr.esa_object_ttc_s_left));
      (esa.createDataSet<float32_T>("esa_object_ttp_s_left", ds::From(hdf_esa_ptr.esa_object_ttp_s_left), props1D).write(hdf_esa_ptr.esa_object_ttp_s_left));
      (esa.createDataSet<float32_T>("esa_object_decel_to_reach_host_speed_mps2_left", ds::From(hdf_esa_ptr.esa_object_decel_to_reach_host_speed_mps2_left), props1D).write(hdf_esa_ptr.esa_object_decel_to_reach_host_speed_mps2_left));
      (esa.createDataSet<float32_T>("esa_object_long_distance_m_left", ds::From(hdf_esa_ptr.esa_object_long_distance_m_left), props1D).write(hdf_esa_ptr.esa_object_long_distance_m_left));
      (esa.createDataSet<float32_T>("esa_object_existence_prob_left", ds::From(hdf_esa_ptr.esa_object_existence_prob_left), props1D).write(hdf_esa_ptr.esa_object_existence_prob_left));
      (esa.createDataSet<uint8_t>("esa_object_id_right", ds::From(hdf_esa_ptr.esa_object_id_right), props1D).write(hdf_esa_ptr.esa_object_id_right));
      (esa.createDataSet<uint8_t>("esa_object_index_right", ds::From(hdf_esa_ptr.esa_object_index_right), props1D).write(hdf_esa_ptr.esa_object_index_right));
      (esa.createDataSet<float32_T>("esa_object_width_m_right", ds::From(hdf_esa_ptr.esa_object_width_m_right), props1D).write(hdf_esa_ptr.esa_object_width_m_right));
      (esa.createDataSet<float32_T>("esa_object_length_m_right", ds::From(hdf_esa_ptr.esa_object_length_m_right), props1D).write(hdf_esa_ptr.esa_object_length_m_right));
      (esa.createDataSet<float32_T>("esa_object_long_pos_m_right", ds::From(hdf_esa_ptr.esa_object_long_pos_m_right), props1D).write(hdf_esa_ptr.esa_object_long_pos_m_right));
      (esa.createDataSet<float32_T>("esa_object_lat_pos_m_right", ds::From(hdf_esa_ptr.esa_object_lat_pos_m_right), props1D).write(hdf_esa_ptr.esa_object_lat_pos_m_right));
      (esa.createDataSet<float32_T>("esa_object_long_speed_mps_right", ds::From(hdf_esa_ptr.esa_object_long_speed_mps_right), props1D).write(hdf_esa_ptr.esa_object_long_speed_mps_right));
      (esa.createDataSet<float32_T>("esa_object_lat_speed_mps_right", ds::From(hdf_esa_ptr.esa_object_lat_speed_mps_right), props1D).write(hdf_esa_ptr.esa_object_lat_speed_mps_right));
      (esa.createDataSet<float32_T>("esa_object_ttc_s_right", ds::From(hdf_esa_ptr.esa_object_ttc_s_right), props1D).write(hdf_esa_ptr.esa_object_ttc_s_right));
      (esa.createDataSet<float32_T>("esa_object_ttp_s_right", ds::From(hdf_esa_ptr.esa_object_ttp_s_right), props1D).write(hdf_esa_ptr.esa_object_ttp_s_right));
      (esa.createDataSet<float32_T>("esa_object_decel_to_reach_host_speed_mps2_right", ds::From(hdf_esa_ptr.esa_object_decel_to_reach_host_speed_mps2_right), props1D).write(hdf_esa_ptr.esa_object_decel_to_reach_host_speed_mps2_right));
      (esa.createDataSet<float32_T>("esa_object_long_distance_m_right", ds::From(hdf_esa_ptr.esa_object_long_distance_m_right), props1D).write(hdf_esa_ptr.esa_object_long_distance_m_right));
      (esa.createDataSet<float32_T>("esa_object_existence_prob_right", ds::From(hdf_esa_ptr.esa_object_existence_prob_right), props1D).write(hdf_esa_ptr.esa_object_existence_prob_right));

      auto esa_cals = esa.createGroup("Calibration");
      // ESA Calibrations
      Esa_Instance_T *esa_generic = Esa_Get_Instance_Ptr();
      (esa_cals.createDataSet<float32_T>("k_esa_zone_x", ds::From(esa_generic->calibration.k_esa_zone_x)).write(esa_generic->calibration.k_esa_zone_x));
      (esa_cals.createDataSet<float32_T>("k_esa_zone_x_hys", ds::From(esa_generic->calibration.k_esa_zone_x_hys)).write(esa_generic->calibration.k_esa_zone_x_hys));
      (esa_cals.createDataSet<float32_T>("k_esa_zone_y", ds::From(esa_generic->calibration.k_esa_zone_y)).write(esa_generic->calibration.k_esa_zone_y));
      (esa_cals.createDataSet<float32_T>("k_esa_zone_y_hys", ds::From(esa_generic->calibration.k_esa_zone_y_hys)).write(esa_generic->calibration.k_esa_zone_y_hys));
      (esa_cals.createDataSet<float32_T>("k_esa_max_range", ds::From(esa_generic->calibration.k_esa_max_range)).write(esa_generic->calibration.k_esa_max_range));
      (esa_cals.createDataSet<float32_T>("k_esa_min_lane_width", ds::From(esa_generic->calibration.k_esa_min_lane_width)).write(esa_generic->calibration.k_esa_min_lane_width));
      (esa_cals.createDataSet<float32_T>("k_esa_max_lane_width", ds::From(esa_generic->calibration.k_esa_max_lane_width)).write(esa_generic->calibration.k_esa_max_lane_width));
      (esa_cals.createDataSet<float32_T>("k_esa_min_exist_prob", ds::From(esa_generic->calibration.k_esa_min_exist_prob)).write(esa_generic->calibration.k_esa_min_exist_prob));
      (esa_cals.createDataSet<float32_T>("k_esa_min_curve_radius", ds::From(esa_generic->calibration.k_esa_min_curve_radius)).write(esa_generic->calibration.k_esa_min_curve_radius));
      (esa_cals.createDataSet<float32_T>("k_esa_min_curve_radius_hys", ds::From(esa_generic->calibration.k_esa_min_curve_radius_hys)).write(esa_generic->calibration.k_esa_min_curve_radius_hys));
      (esa_cals.createDataSet<float32_T>("k_esa_max_curvi_heading_abs", ds::From(esa_generic->calibration.k_esa_max_curvi_heading_abs)).write(esa_generic->calibration.k_esa_max_curvi_heading_abs));
      (esa_cals.createDataSet<float32_T>("k_esa_min_obj_curvi_long_vel_abs", ds::From(esa_generic->calibration.k_esa_min_obj_curvi_long_vel_abs)).write(esa_generic->calibration.k_esa_min_obj_curvi_long_vel_abs));
      (esa_cals.createDataSet<float32_T>("k_esa_critical_longitudinal_ttc", ds::From(esa_generic->calibration.k_esa_critical_longitudinal_ttc)).write(esa_generic->calibration.k_esa_critical_longitudinal_ttc));
      (esa_cals.createDataSet<float32_T>("k_esa_critical_longitudinal_ttc_hys", ds::From(esa_generic->calibration.k_esa_critical_longitudinal_ttc_hys)).write(esa_generic->calibration.k_esa_critical_longitudinal_ttc_hys));
      (esa_cals.createDataSet<float32_T>("k_esa_obj_safe_deceleration_threshold", ds::From(esa_generic->calibration.k_esa_obj_safe_deceleration_threshold)).write(esa_generic->calibration.k_esa_obj_safe_deceleration_threshold));
      (esa_cals.createDataSet<float32_T>("k_esa_obj_safe_deceleration_threshold_hys", ds::From(esa_generic->calibration.k_esa_obj_safe_deceleration_threshold_hys)).write(esa_generic->calibration.k_esa_obj_safe_deceleration_threshold_hys));
      (esa_cals.createDataSet<float32_T>("k_esa_host_activation_speed_min", ds::From(esa_generic->calibration.k_esa_host_activation_speed_min)).write(esa_generic->calibration.k_esa_host_activation_speed_min));
      (esa_cals.createDataSet<float32_T>("k_esa_host_activation_speed_min_hys", ds::From(esa_generic->calibration.k_esa_host_activation_speed_min_hys)).write(esa_generic->calibration.k_esa_host_activation_speed_min_hys));
      (esa_cals.createDataSet<float32_T>("k_esa_host_activation_speed_max", ds::From(esa_generic->calibration.k_esa_host_activation_speed_max)).write(esa_generic->calibration.k_esa_host_activation_speed_max));
      (esa_cals.createDataSet<float32_T>("k_esa_host_activation_speed_max_hys", ds::From(esa_generic->calibration.k_esa_host_activation_speed_max_hys)).write(esa_generic->calibration.k_esa_host_activation_speed_max_hys));
      (esa_cals.createDataSet<boolean_T>("k_esa_f_enable_via_cal", ds::From(esa_generic->calibration.k_esa_f_enable_via_cal)).write(esa_generic->calibration.k_esa_f_enable_via_cal));
      (esa_cals.createDataSet<boolean_T>("k_esa_f_enable", ds::From(esa_generic->calibration.k_esa_f_enable)).write(esa_generic->calibration.k_esa_f_enable));
      (esa_cals.createDataSet<boolean_T>("k_esa_f_allow_min_curve_radius", ds::From(esa_generic->calibration.k_esa_f_allow_min_curve_radius)).write(esa_generic->calibration.k_esa_f_allow_min_curve_radius));
      (esa_cals.createDataSet<boolean_T>("k_esa_f_allow_obj_critical_ttc_and_deceleration", ds::From(esa_generic->calibration.k_esa_f_allow_obj_critical_ttc_and_deceleration)).write(esa_generic->calibration.k_esa_f_allow_obj_critical_ttc_and_deceleration));
      (esa_cals.createDataSet<boolean_T>("k_esa_f_allow_obj_selection_ttc", ds::From(esa_generic->calibration.k_esa_f_allow_obj_selection_ttc)).write(esa_generic->calibration.k_esa_f_allow_obj_selection_ttc));
      (esa_cals.createDataSet<boolean_T>("k_esa_f_allow_obj_selection_deceleration", ds::From(esa_generic->calibration.k_esa_f_allow_obj_selection_deceleration)).write(esa_generic->calibration.k_esa_f_allow_obj_selection_deceleration));
      (esa_cals.createDataSet<boolean_T>("k_esa_f_allow_obj_selection_long_distance", ds::From(esa_generic->calibration.k_esa_f_allow_obj_selection_long_distance)).write(esa_generic->calibration.k_esa_f_allow_obj_selection_long_distance));
      (esa_cals.createDataSet<uint8_t>("k_esa_min_track_age", ds::From(esa_generic->calibration.k_esa_min_track_age)).write(esa_generic->calibration.k_esa_min_track_age));
      (esa_cals.createDataSet<uint8_t>("k_esa_min_mature_cycles", ds::From(esa_generic->calibration.k_esa_min_mature_cycles)).write(esa_generic->calibration.k_esa_min_mature_cycles));
      (esa_cals.createDataSet<uint8_t>("k_esa_alert_holding_cycles", ds::From(esa_generic->calibration.k_esa_alert_holding_cycles)).write(esa_generic->calibration.k_esa_alert_holding_cycles));

      auto ltb = g5.createGroup("LTB");
      (ltb.createDataSet<uint8_t>("ltb_object_ltb_id_left", ds::From(hdf_ltb_ptr.ltb_object_ltb_id_left), props1D)).write(hdf_ltb_ptr.ltb_object_ltb_id_left);
      (ltb.createDataSet<float32_T>("ltb_object_ltb_ttc_s_left", ds::From(hdf_ltb_ptr.ltb_object_ltb_ttc_s_left), props1D)).write(hdf_ltb_ptr.ltb_object_ltb_ttc_s_left);
      (ltb.createDataSet<float32_T>("ltb_object_ltb_ttb_s_left", ds::From(hdf_ltb_ptr.ltb_object_ltb_ttb_s_left), props1D)).write(hdf_ltb_ptr.ltb_object_ltb_ttb_s_left);
      (ltb.createDataSet<float32_T>("ltb_object_ltb_decel_estimate_mps2_left", ds::From(hdf_ltb_ptr.ltb_object_ltb_decel_estimate_mps2_left), props1D)).write(hdf_ltb_ptr.ltb_object_ltb_decel_estimate_mps2_left);
      (ltb.createDataSet<float32_T>("ltb_object_ltb_distance_m_left", ds::From(hdf_ltb_ptr.ltb_object_ltb_distance_m_left), props1D)).write(hdf_ltb_ptr.ltb_object_ltb_distance_m_left);
      (ltb.createDataSet<uint8_t>("ltb_object_ltb_id_right", ds::From(hdf_ltb_ptr.ltb_object_ltb_id_right), props1D)).write(hdf_ltb_ptr.ltb_object_ltb_id_right);
      (ltb.createDataSet<float32_T>("ltb_object_ltb_ttc_s_right", ds::From(hdf_ltb_ptr.ltb_object_ltb_ttc_s_right), props1D)).write(hdf_ltb_ptr.ltb_object_ltb_ttc_s_right);
      (ltb.createDataSet<float32_T>("ltb_object_ltb_ttb_s_right", ds::From(hdf_ltb_ptr.ltb_object_ltb_ttb_s_right), props1D)).write(hdf_ltb_ptr.ltb_object_ltb_ttb_s_right);
      (ltb.createDataSet<float32_T>("ltb_object_ltb_decel_estimate_mps2_right", ds::From(hdf_ltb_ptr.ltb_object_ltb_decel_estimate_mps2_right), props1D)).write(hdf_ltb_ptr.ltb_object_ltb_decel_estimate_mps2_right);
      (ltb.createDataSet<float32_T>("ltb_object_ltb_distance_m_right", ds::From(hdf_ltb_ptr.ltb_object_ltb_distance_m_right), props1D)).write(hdf_ltb_ptr.ltb_object_ltb_distance_m_right);
      (ltb.createDataSet<int>("ltb_alert_level_left", ds::From(hdf_ltb_ptr.ltb_alert_level_left), props1D)).write(hdf_ltb_ptr.ltb_alert_level_left);
      (ltb.createDataSet<int>("ltb_alert_level_right", ds::From(hdf_ltb_ptr.ltb_alert_level_right), props1D)).write(hdf_ltb_ptr.ltb_alert_level_right);
      (ltb.createDataSet<uint8_t>("ltb_most_critical_side", ds::From(hdf_ltb_ptr.ltb_most_critical_side), props1D)).write(hdf_ltb_ptr.ltb_most_critical_side);

      auto ltb_cals = ltb.createGroup("Calibration");
      // LTB Calibrations
      Ltb_Instance_T *ltb_generic = Ltb_Get_Instance_Ptr();
      (ltb_cals.createDataSet<float32_T>("k_ltb_zone_length", ds::From(ltb_generic->calibration.k_ltb_zone_length)).write(ltb_generic->calibration.k_ltb_zone_length));
      (ltb_cals.createDataSet<float32_T>("k_ltb_zone_width", ds::From(ltb_generic->calibration.k_ltb_zone_width)).write(ltb_generic->calibration.k_ltb_zone_width));
      (ltb_cals.createDataSet<float32_T>("k_ltb_ego_acceleration_weight", ds::From(ltb_generic->calibration.k_ltb_ego_acceleration_weight)).write(ltb_generic->calibration.k_ltb_ego_acceleration_weight));
      (ltb_cals.createDataSet<float32_T>("k_ltb_ego_shape_gain_fixed", ds::From(ltb_generic->calibration.k_ltb_ego_shape_gain_fixed)).write(ltb_generic->calibration.k_ltb_ego_shape_gain_fixed));
      (ltb_cals.createDataSet<float32_T>("k_ltb_ego_circle_offset", ds::From(ltb_generic->calibration.k_ltb_ego_circle_offset)).write(ltb_generic->calibration.k_ltb_ego_circle_offset));
      (ltb_cals.createDataSet<float32_T>("k_ltb_ego_circle_host_length_factor", ds::From(ltb_generic->calibration.k_ltb_ego_circle_host_length_factor)).write(ltb_generic->calibration.k_ltb_ego_circle_host_length_factor));
      (ltb_cals.createDataSet<float32_T>("k_ltb_ego_deceleration_weight", ds::From(ltb_generic->calibration.k_ltb_ego_deceleration_weight)).write(ltb_generic->calibration.k_ltb_ego_deceleration_weight));
      (ltb_cals.createDataSet<float32_T>("k_ltb_ego_max_pred_yaw_angle", ds::From(ltb_generic->calibration.k_ltb_ego_max_pred_yaw_angle)).write(ltb_generic->calibration.k_ltb_ego_max_pred_yaw_angle));
      (ltb_cals.createDataSet<float32_T>("k_ltb_ego_yawangle_integration_yawrate_min", ds::From(ltb_generic->calibration.k_ltb_ego_yawangle_integration_yawrate_min)).write(ltb_generic->calibration.k_ltb_ego_yawangle_integration_yawrate_min));
      (ltb_cals.createDataSet<float32_T>("k_ltb_ego_shape_gain_per_pred_step", ds::From(ltb_generic->calibration.k_ltb_ego_shape_gain_per_pred_step)).write(ltb_generic->calibration.k_ltb_ego_shape_gain_per_pred_step));
      (ltb_cals.createDataSet<float32_T>("k_ltb_obj_pred_speed_min", ds::From(ltb_generic->calibration.k_ltb_obj_pred_speed_min)).write(ltb_generic->calibration.k_ltb_obj_pred_speed_min));
      (ltb_cals.createDataSet<float32_T>("k_ltb_obj_shape_gain_fixed", ds::From(ltb_generic->calibration.k_ltb_obj_shape_gain_fixed)).write(ltb_generic->calibration.k_ltb_obj_shape_gain_fixed));
      (ltb_cals.createDataSet<float32_T>("k_ltb_obj_shape_gain_per_pred_step", ds::From(ltb_generic->calibration.k_ltb_obj_shape_gain_per_pred_step)).write(ltb_generic->calibration.k_ltb_obj_shape_gain_per_pred_step));
      (ltb_cals.createDataSet<float32_T>("k_ltb_critical_approach_min_safe_distance", ds::From(ltb_generic->calibration.k_ltb_critical_approach_min_safe_distance)).write(ltb_generic->calibration.k_ltb_critical_approach_min_safe_distance));
      (ltb_cals.createDataSet<float32_T>("k_ltb_critical_approach_angle_diff_min", ds::From(ltb_generic->calibration.k_ltb_critical_approach_angle_diff_min)).write(ltb_generic->calibration.k_ltb_critical_approach_angle_diff_min));
      (ltb_cals.createDataSet<float32_T>("k_ltb_alert_lvl_1_ttc_threshold", ds::From(ltb_generic->calibration.k_ltb_alert_lvl_1_ttc_threshold)).write(ltb_generic->calibration.k_ltb_alert_lvl_1_ttc_threshold));
      (ltb_cals.createDataSet<float32_T>("k_ltb_alert_lvl_2_ttc_threshold", ds::From(ltb_generic->calibration.k_ltb_alert_lvl_2_ttc_threshold)).write(ltb_generic->calibration.k_ltb_alert_lvl_2_ttc_threshold));
      (ltb_cals.createDataSet<float32_T>("k_ltb_alert_lvl_2_ttb_threshold", ds::From(ltb_generic->calibration.k_ltb_alert_lvl_2_ttb_threshold)).write(ltb_generic->calibration.k_ltb_alert_lvl_2_ttb_threshold));
      (ltb_cals.createDataSet<float32_T>("k_ltb_alert_lvl_3_ttc_threshold", ds::From(ltb_generic->calibration.k_ltb_alert_lvl_3_ttc_threshold)).write(ltb_generic->calibration.k_ltb_alert_lvl_3_ttc_threshold));
      (ltb_cals.createDataSet<float32_T>("k_ltb_alert_lvl_3_decel_threshold", ds::From(ltb_generic->calibration.k_ltb_alert_lvl_3_decel_threshold)).write(ltb_generic->calibration.k_ltb_alert_lvl_3_decel_threshold));
      (ltb_cals.createDataSet<float32_T>("k_ltb_brake_deceleration_max", ds::From(ltb_generic->calibration.k_ltb_brake_deceleration_max)).write(ltb_generic->calibration.k_ltb_brake_deceleration_max));
      (ltb_cals.createDataSet<float32_T>("k_ltb_brake_dead_time", ds::From(ltb_generic->calibration.k_ltb_brake_dead_time)).write(ltb_generic->calibration.k_ltb_brake_dead_time));
      (ltb_cals.createDataSet<float32_T>("k_ltb_brake_gradient", ds::From(ltb_generic->calibration.k_ltb_brake_gradient)).write(ltb_generic->calibration.k_ltb_brake_gradient));
      (ltb_cals.createDataSet<float32_T>("k_ltb_object_long_vel_min", ds::From(ltb_generic->calibration.k_ltb_object_long_vel_min)).write(ltb_generic->calibration.k_ltb_object_long_vel_min));
      (ltb_cals.createDataSet<float32_T>("k_ltb_bmw_sp25_v_ego_max", ds::From(ltb_generic->calibration.k_ltb_bmw_sp25_v_ego_max)).write(ltb_generic->calibration.k_ltb_bmw_sp25_v_ego_max));
      (ltb_cals.createDataSet<float32_T>("k_ltb_bmw_sp25_v_ego_max_hys", ds::From(ltb_generic->calibration.k_ltb_bmw_sp25_v_ego_max_hys)).write(ltb_generic->calibration.k_ltb_bmw_sp25_v_ego_max_hys));
      (ltb_cals.createDataSet<boolean_T>("k_ltb_f_only_allow_consecutive_ttc_based_alert_levels", ds::From(ltb_generic->calibration.k_ltb_f_only_allow_consecutive_ttc_based_alert_levels)).write(ltb_generic->calibration.k_ltb_f_only_allow_consecutive_ttc_based_alert_levels));
      (ltb_cals.createDataSet<boolean_T>("k_ltb_f_skip_holding_for_single_alert_level_drop", ds::From(ltb_generic->calibration.k_ltb_f_skip_holding_for_single_alert_level_drop)).write(ltb_generic->calibration.k_ltb_f_skip_holding_for_single_alert_level_drop));
      (ltb_cals.createDataSet<boolean_T>("k_f_ltb_enable_brake_gradient_logic", ds::From(ltb_generic->calibration.k_f_ltb_enable_brake_gradient_logic)).write(ltb_generic->calibration.k_f_ltb_enable_brake_gradient_logic));
      (ltb_cals.createDataSet<uint8_t>("k_ltb_ego_pred_const_velocity_pred_steps_min", ds::From(ltb_generic->calibration.k_ltb_ego_pred_const_velocity_pred_steps_min)).write(ltb_generic->calibration.k_ltb_ego_pred_const_velocity_pred_steps_min));
      (ltb_cals.createDataSet<uint8_t>("k_ltb_prediction_steps_max", ds::From(ltb_generic->calibration.k_ltb_prediction_steps_max)).write(ltb_generic->calibration.k_ltb_prediction_steps_max));
      (ltb_cals.createDataSet<uint8_t>("k_ltb_alert_qualifying_cycles", ds::From(ltb_generic->calibration.k_ltb_alert_qualifying_cycles)).write(ltb_generic->calibration.k_ltb_alert_qualifying_cycles));
      (ltb_cals.createDataSet<uint8_t>("k_ltb_alert_holding_cycles", ds::From(ltb_generic->calibration.k_ltb_alert_holding_cycles)).write(ltb_generic->calibration.k_ltb_alert_holding_cycles));

      // Write RECW feature output
      auto recw = g5.createGroup("RECW");
      (recw.createDataSet<float32_T>("recw_crash_probability", ds::From(hdf_recw_ptr.recw_crash_probability), props1D)).write(hdf_recw_ptr.recw_crash_probability);
      (recw.createDataSet<float32_T>("recw_ttc_s", ds::From(hdf_recw_ptr.recw_ttc_s), props1D)).write(hdf_recw_ptr.recw_ttc_s);
      (recw.createDataSet<uint8_t>("recw_id", ds::From(hdf_recw_ptr.recw_id), props1D)).write(hdf_recw_ptr.recw_id);
      (recw.createDataSet<uint32_t>("recw_unique_id", ds::From(hdf_recw_ptr.recw_unique_id), props1D)).write(hdf_recw_ptr.recw_unique_id);
      (recw.createDataSet<int>("recw_alert_level", ds::From(hdf_recw_ptr.recw_alert_level), props1D)).write(hdf_recw_ptr.recw_alert_level);
      (recw.createDataSet<float32_T>("ttc_threshold_alert_level_1_s", ds::From(hdf_recw_ptr.ttc_threshold_alert_level_1_s), props1D)).write(hdf_recw_ptr.ttc_threshold_alert_level_1_s);
      (recw.createDataSet<float32_T>("ttc_threshold_alert_level_2_s", ds::From(hdf_recw_ptr.ttc_threshold_alert_level_2_s), props1D)).write(hdf_recw_ptr.ttc_threshold_alert_level_2_s);

      auto recw_cals = recw.createGroup("Calibration");
      // RECW Calibrations
      Recw_Instance_T *recw_generic = Recw_Get_Instance_Ptr();
      (recw_cals.createDataSet<float32_T>("k_recw_min_rel_velocity", ds::From(recw_generic->calibration.k_recw_min_rel_velocity)).write(recw_generic->calibration.k_recw_min_rel_velocity));
      (recw_cals.createDataSet<float32_T>("k_recw_min_rel_velocity_hys", ds::From(recw_generic->calibration.k_recw_min_rel_velocity_hys)).write(recw_generic->calibration.k_recw_min_rel_velocity_hys));
      (recw_cals.createDataSet<float32_T>("k_recw_max_heading", ds::From(recw_generic->calibration.k_recw_max_heading)).write(recw_generic->calibration.k_recw_max_heading));
      (recw_cals.createDataSet<float32_T>("k_recw_max_heading_hys", ds::From(recw_generic->calibration.k_recw_max_heading_hys)).write(recw_generic->calibration.k_recw_max_heading_hys));
      (recw_cals.createDataSet<float32_T>("k_recw_factor_ego_width", ds::From(recw_generic->calibration.k_recw_factor_ego_width)).write(recw_generic->calibration.k_recw_factor_ego_width));
      (recw_cals.createDataSet<float32_T>("k_recw_average_sensor_latency", ds::From(recw_generic->calibration.k_recw_average_sensor_latency)).write(recw_generic->calibration.k_recw_average_sensor_latency));
      (recw_cals.createDataSet<float32_T>("k_recw_min_crash_prob", ds::From(recw_generic->calibration.k_recw_min_crash_prob)).write(recw_generic->calibration.k_recw_min_crash_prob));
      (recw_cals.createDataSet<float32_T>("k_recw_min_existence_prob", ds::From(recw_generic->calibration.k_recw_min_existence_prob)).write(recw_generic->calibration.k_recw_min_existence_prob));
      (recw_cals.createDataSet<float32_T>("k_recw_min_existence_prob_hys", ds::From(recw_generic->calibration.k_recw_min_existence_prob_hys)).write(recw_generic->calibration.k_recw_min_existence_prob_hys));
      (recw_cals.createDataSet<float32_T>("k_recw_min_ttc_for_alert_level", ds::From(recw_generic->calibration.k_recw_min_ttc_for_alert_level)).write(recw_generic->calibration.k_recw_min_ttc_for_alert_level));
      (recw_cals.createDataSet<float32_T>("k_recw_min_overlap_for_alert_level", ds::From(recw_generic->calibration.k_recw_min_overlap_for_alert_level)).write(recw_generic->calibration.k_recw_min_overlap_for_alert_level));
      (recw_cals.createDataSet<float32_T>("k_recw_min_host_speed", ds::From(recw_generic->calibration.k_recw_min_host_speed)).write(recw_generic->calibration.k_recw_min_host_speed));
      (recw_cals.createDataSet<float32_T>("k_recw_min_host_speed_hys", ds::From(recw_generic->calibration.k_recw_min_host_speed_hys)).write(recw_generic->calibration.k_recw_min_host_speed_hys));
      (recw_cals.createDataSet<float32_T>("k_recw_max_host_speed", ds::From(recw_generic->calibration.k_recw_max_host_speed)).write(recw_generic->calibration.k_recw_max_host_speed));
      (recw_cals.createDataSet<float32_T>("k_recw_max_host_speed_hys", ds::From(recw_generic->calibration.k_recw_max_host_speed_hys)).write(recw_generic->calibration.k_recw_max_host_speed_hys));
      (recw_cals.createDataSet<float32_T>("k_recw_min_rel_velocity_for_max_ttc_threshold", ds::From(recw_generic->calibration.k_recw_min_rel_velocity_for_max_ttc_threshold)).write(recw_generic->calibration.k_recw_min_rel_velocity_for_max_ttc_threshold));
      (recw_cals.createDataSet<float32_T>("k_recw_max_ttc_threshold", ds::From(recw_generic->calibration.k_recw_max_ttc_threshold)).write(recw_generic->calibration.k_recw_max_ttc_threshold));
      (recw_cals.createDataSet<float32_T>("k_recw_min_dist_for_young_slow_targets", ds::From(recw_generic->calibration.k_recw_min_dist_for_young_slow_targets)).write(recw_generic->calibration.k_recw_min_dist_for_young_slow_targets));
      (recw_cals.createDataSet<float32_T>("k_recw_min_abs_speed_for_young_close_targets", ds::From(recw_generic->calibration.k_recw_min_abs_speed_for_young_close_targets)).write(recw_generic->calibration.k_recw_min_abs_speed_for_young_close_targets));
      (recw_cals.createDataSet<float32_T>("k_recw_max_rel_velocity", ds::From(recw_generic->calibration.k_recw_max_rel_velocity)).write(recw_generic->calibration.k_recw_max_rel_velocity));
      (recw_cals.createDataSet<float32_T>("k_recw_max_rel_velocity_hys", ds::From(recw_generic->calibration.k_recw_max_rel_velocity_hys)).write(recw_generic->calibration.k_recw_max_rel_velocity_hys));
      (recw_cals.createDataSet<float32_T>("k_recw_max_rel_lon_vel_release_car_wash", ds::From(recw_generic->calibration.k_recw_max_rel_lon_vel_release_car_wash)).write(recw_generic->calibration.k_recw_max_rel_lon_vel_release_car_wash));
      (recw_cals.createDataSet<float32_T>("k_recw_max_lon_distance_car_wash", ds::From(recw_generic->calibration.k_recw_max_lon_distance_car_wash)).write(recw_generic->calibration.k_recw_max_lon_distance_car_wash));
      (recw_cals.createDataSet<float32_T>("k_recw_max_lat_distance_car_wash", ds::From(recw_generic->calibration.k_recw_max_lat_distance_car_wash)).write(recw_generic->calibration.k_recw_max_lat_distance_car_wash));
      (recw_cals.createDataSet<float32_T>("k_recw_min_rel_lon_vel_car_wash", ds::From(recw_generic->calibration.k_recw_min_rel_lon_vel_car_wash)).write(recw_generic->calibration.k_recw_min_rel_lon_vel_car_wash));
      (recw_cals.createDataSet<float32_T>("k_recw_max_speed_ego_car_wash", ds::From(recw_generic->calibration.k_recw_max_speed_ego_car_wash)).write(recw_generic->calibration.k_recw_max_speed_ego_car_wash));
      (recw_cals.createDataSet<float32_T>("k_recw_lane_filter_width", ds::From(recw_generic->calibration.k_recw_lane_filter_width)).write(recw_generic->calibration.k_recw_lane_filter_width));
      (recw_cals.createDataSet<float32_T>("k_recw_lane_filter_width_hys", ds::From(recw_generic->calibration.k_recw_lane_filter_width_hys)).write(recw_generic->calibration.k_recw_lane_filter_width_hys));
      (recw_cals.createDataSet<float32_T>("k_recw_lane_filter_max_abs_ego_speed_vcs_coord", ds::From(recw_generic->calibration.k_recw_lane_filter_max_abs_ego_speed_vcs_coord)).write(recw_generic->calibration.k_recw_lane_filter_max_abs_ego_speed_vcs_coord));
      (recw_cals.createDataSet<float32_T>("k_recw_lane_width_slope", ds::From(recw_generic->calibration.k_recw_lane_width_slope)).write(recw_generic->calibration.k_recw_lane_width_slope));
      (recw_cals.createDataSet<float32_T>("k_recw_lookup_braking_deceleration", ds::From(recw_generic->calibration.k_recw_lookup_braking_deceleration)).write(recw_generic->calibration.k_recw_lookup_braking_deceleration));
      (recw_cals.createDataSet<float32_T>("k_recw_lookup_braking_probability", ds::From(recw_generic->calibration.k_recw_lookup_braking_probability)).write(recw_generic->calibration.k_recw_lookup_braking_probability));
      (recw_cals.createDataSet<float32_T>("k_recw_lookup_steering_acceleration", ds::From(recw_generic->calibration.k_recw_lookup_steering_acceleration)).write(recw_generic->calibration.k_recw_lookup_steering_acceleration));
      (recw_cals.createDataSet<float32_T>("k_recw_lookup_steering_probability", ds::From(recw_generic->calibration.k_recw_lookup_steering_probability)).write(recw_generic->calibration.k_recw_lookup_steering_probability));
      (recw_cals.createDataSet<float32_T>("k_recw_max_eclipse_value_for_valid_object", ds::From(recw_generic->calibration.k_recw_max_eclipse_value_for_valid_object)).write(recw_generic->calibration.k_recw_max_eclipse_value_for_valid_object));
      (recw_cals.createDataSet<float32_T>("k_recw_max_object_width_warn_on", ds::From(recw_generic->calibration.k_recw_max_object_width_warn_on)).write(recw_generic->calibration.k_recw_max_object_width_warn_on));
      (recw_cals.createDataSet<float32_T>("k_recw_max_allowed_rel_vel_long_diff", ds::From(recw_generic->calibration.k_recw_max_allowed_rel_vel_long_diff)).write(recw_generic->calibration.k_recw_max_allowed_rel_vel_long_diff));
      (recw_cals.createDataSet<float32_T>("k_recw_max_allowed_rel_vel_lat_diff", ds::From(recw_generic->calibration.k_recw_max_allowed_rel_vel_lat_diff)).write(recw_generic->calibration.k_recw_max_allowed_rel_vel_lat_diff));
      (recw_cals.createDataSet<float32_T>("k_recw_max_allowed_heading_diff", ds::From(recw_generic->calibration.k_recw_max_allowed_heading_diff)).write(recw_generic->calibration.k_recw_max_allowed_heading_diff));
      (recw_cals.createDataSet<float32_T>("k_recw_rear_blockage_speed_threshold", ds::From(recw_generic->calibration.k_recw_rear_blockage_speed_threshold)).write(recw_generic->calibration.k_recw_rear_blockage_speed_threshold));
      (recw_cals.createDataSet<float32_T>("k_recw_rear_blockage_width", ds::From(recw_generic->calibration.k_recw_rear_blockage_width)).write(recw_generic->calibration.k_recw_rear_blockage_width));
      (recw_cals.createDataSet<float32_T>("k_recw_rear_blockage_length", ds::From(recw_generic->calibration.k_recw_rear_blockage_length)).write(recw_generic->calibration.k_recw_rear_blockage_length));
      (recw_cals.createDataSet<float32_T>("k_recw_rear_blockage_ego_speed_threshold", ds::From(recw_generic->calibration.k_recw_rear_blockage_ego_speed_threshold)).write(recw_generic->calibration.k_recw_rear_blockage_ego_speed_threshold));
      (recw_cals.createDataSet<float32_T>("k_recw_heading_accuracy_threshold", ds::From(recw_generic->calibration.k_recw_heading_accuracy_threshold)).write(recw_generic->calibration.k_recw_heading_accuracy_threshold));
      (recw_cals.createDataSet<float32_T>("k_recw_min_speed_not_stationary", ds::From(recw_generic->calibration.k_recw_min_speed_not_stationary)).write(recw_generic->calibration.k_recw_min_speed_not_stationary));
      (recw_cals.createDataSet<float32_T>("k_recb_max_host_speed", ds::From(recw_generic->calibration.k_recb_max_host_speed)).write(recw_generic->calibration.k_recb_max_host_speed));
      (recw_cals.createDataSet<float32_T>("k_recb_max_host_speed_hys", ds::From(recw_generic->calibration.k_recb_max_host_speed_hys)).write(recw_generic->calibration.k_recb_max_host_speed_hys));
      (recw_cals.createDataSet<float32_T>("k_recb_max_velocity_host_standstill", ds::From(recw_generic->calibration.k_recb_max_velocity_host_standstill)).write(recw_generic->calibration.k_recb_max_velocity_host_standstill));
      (recw_cals.createDataSet<float32_T>("k_recb_min_rel_velocity", ds::From(recw_generic->calibration.k_recb_min_rel_velocity)).write(recw_generic->calibration.k_recb_min_rel_velocity));
      (recw_cals.createDataSet<float32_T>("k_recb_min_rel_velocity_hys", ds::From(recw_generic->calibration.k_recb_min_rel_velocity_hys)).write(recw_generic->calibration.k_recb_min_rel_velocity_hys));
      (recw_cals.createDataSet<float32_T>("k_recb_max_ttc", ds::From(recw_generic->calibration.k_recb_max_ttc)).write(recw_generic->calibration.k_recb_max_ttc));
      (recw_cals.createDataSet<float32_T>("k_recb_nominal_acceleration_applied", ds::From(recw_generic->calibration.k_recb_nominal_acceleration_applied)).write(recw_generic->calibration.k_recb_nominal_acceleration_applied));
      (recw_cals.createDataSet<float32_T>("k_recb_accelerator_pedal_gradient_threshold", ds::From(recw_generic->calibration.k_recb_accelerator_pedal_gradient_threshold)).write(recw_generic->calibration.k_recb_accelerator_pedal_gradient_threshold));
      (recw_cals.createDataSet<boolean_T>("k_recw_f_make_use_of_guardrail", ds::From(recw_generic->calibration.k_recw_f_make_use_of_guardrail)).write(recw_generic->calibration.k_recw_f_make_use_of_guardrail));
      (recw_cals.createDataSet<boolean_T>("k_recw_f_only_allow_consecutive_alert_levels", ds::From(recw_generic->calibration.k_recw_f_only_allow_consecutive_alert_levels)).write(recw_generic->calibration.k_recw_f_only_allow_consecutive_alert_levels));
      (recw_cals.createDataSet<boolean_T>("k_recw_f_allow_alert_on_coasted_objects", ds::From(recw_generic->calibration.k_recw_f_allow_alert_on_coasted_objects)).write(recw_generic->calibration.k_recw_f_allow_alert_on_coasted_objects));
      (recw_cals.createDataSet<boolean_T>("k_recw_f_use_rear_blockage", ds::From(recw_generic->calibration.k_recw_f_use_rear_blockage)).write(recw_generic->calibration.k_recw_f_use_rear_blockage));
      (recw_cals.createDataSet<boolean_T>("k_recw_f_apply_lane_filter", ds::From(recw_generic->calibration.k_recw_f_apply_lane_filter)).write(recw_generic->calibration.k_recw_f_apply_lane_filter));
      (recw_cals.createDataSet<boolean_T>("k_recw_f_enable_heading_filter", ds::From(recw_generic->calibration.k_recw_f_enable_heading_filter)).write(recw_generic->calibration.k_recw_f_enable_heading_filter));
      (recw_cals.createDataSet<boolean_T>("k_recw_f_enable_traffic_light_ghost_detection", ds::From(recw_generic->calibration.k_recw_f_enable_traffic_light_ghost_detection)).write(recw_generic->calibration.k_recw_f_enable_traffic_light_ghost_detection));
      (recw_cals.createDataSet<boolean_T>("k_recb_f_enable_recb", ds::From(recw_generic->calibration.k_recb_f_enable_recb)).write(recw_generic->calibration.k_recb_f_enable_recb));
      (recw_cals.createDataSet<uint8_t>("k_recw_alert_holding_cycles", ds::From(recw_generic->calibration.k_recw_alert_holding_cycles)).write(recw_generic->calibration.k_recw_alert_holding_cycles));
      (recw_cals.createDataSet<uint8_t>("k_recw_alert_qualifying_cycles", ds::From(recw_generic->calibration.k_recw_alert_qualifying_cycles)).write(recw_generic->calibration.k_recw_alert_qualifying_cycles));
      (recw_cals.createDataSet<uint8_t>("k_recw_min_stage_age_for_alert_level", ds::From(recw_generic->calibration.k_recw_min_stage_age_for_alert_level)).write(recw_generic->calibration.k_recw_min_stage_age_for_alert_level));
      (recw_cals.createDataSet<uint8_t>("k_recw_max_cycles_alert_duration", ds::From(recw_generic->calibration.k_recw_max_cycles_alert_duration)).write(recw_generic->calibration.k_recw_max_cycles_alert_duration));
      (recw_cals.createDataSet<uint8_t>("k_recw_min_age_for_close_slow_targets", ds::From(recw_generic->calibration.k_recw_min_age_for_close_slow_targets)).write(recw_generic->calibration.k_recw_min_age_for_close_slow_targets));
      (recw_cals.createDataSet<uint8_t>("k_recw_min_object_age", ds::From(recw_generic->calibration.k_recw_min_object_age)).write(recw_generic->calibration.k_recw_min_object_age));
      (recw_cals.createDataSet<uint8_t>("k_recw_min_cycles_with_min_crash_prob", ds::From(recw_generic->calibration.k_recw_min_cycles_with_min_crash_prob)).write(recw_generic->calibration.k_recw_min_cycles_with_min_crash_prob));
      (recw_cals.createDataSet<uint8_t>("k_recw_en_active_car_wash_logic", ds::From(recw_generic->calibration.k_recw_en_active_car_wash_logic)).write(recw_generic->calibration.k_recw_en_active_car_wash_logic));
      (recw_cals.createDataSet<uint8_t>("k_recw_lane_filter_num_consecutive_cycles", ds::From(recw_generic->calibration.k_recw_lane_filter_num_consecutive_cycles)).write(recw_generic->calibration.k_recw_lane_filter_num_consecutive_cycles));
      (recw_cals.createDataSet<uint8_t>("k_recw_max_allowed_consecutive_coasted_cycles", ds::From(recw_generic->calibration.k_recw_max_allowed_consecutive_coasted_cycles)).write(recw_generic->calibration.k_recw_max_allowed_consecutive_coasted_cycles));
      (recw_cals.createDataSet<uint8_t>("k_recw_rear_blockage_qualifying_cycles", ds::From(recw_generic->calibration.k_recw_rear_blockage_qualifying_cycles)).write(recw_generic->calibration.k_recw_rear_blockage_qualifying_cycles));
      (recw_cals.createDataSet<uint8_t>("k_recb_min_cycles_host_standstill", ds::From(recw_generic->calibration.k_recb_min_cycles_host_standstill)).write(recw_generic->calibration.k_recb_min_cycles_host_standstill));
      (recw_cals.createDataSet<uint8_t>("k_recb_ssm_braking", ds::From(recw_generic->calibration.k_recb_ssm_braking)).write(recw_generic->calibration.k_recb_ssm_braking));
      (recw_cals.createDataSet<uint8_t>("k_recb_ssm_request_cancelled", ds::From(recw_generic->calibration.k_recb_ssm_request_cancelled)).write(recw_generic->calibration.k_recb_ssm_request_cancelled));
      (recw_cals.createDataSet<uint8_t>("k_recb_integrity", ds::From(recw_generic->calibration.k_recb_integrity)).write(recw_generic->calibration.k_recb_integrity));
      (recw_cals.createDataSet<uint8_t>("k_recb_qualifier_nominal_acceleration", ds::From(recw_generic->calibration.k_recb_qualifier_nominal_acceleration)).write(recw_generic->calibration.k_recb_qualifier_nominal_acceleration));
      (recw_cals.createDataSet<uint8_t>("k_recb_max_cycles_braking_request_duration", ds::From(recw_generic->calibration.k_recb_max_cycles_braking_request_duration)).write(recw_generic->calibration.k_recb_max_cycles_braking_request_duration));

      // Write TA feature output
      auto ta = g5.createGroup("TA");
      (ta.createDataSet<boolean_T>("f_ta_enable", ds::From(hdf_ta_ptr.f_ta_enable), props1D)).write(hdf_ta_ptr.f_ta_enable);
      (ta.createDataSet<boolean_T>("ta_f_vehicle_state_relevant", ds::From(hdf_ta_ptr.ta_f_vehicle_state_relevant), props1D)).write(hdf_ta_ptr.ta_f_vehicle_state_relevant);
      (ta.createDataSet<uint8_t>("ta_most_critical_side", ds::From(hdf_ta_ptr.ta_most_critical_side), props1D)).write(hdf_ta_ptr.ta_most_critical_side);
      (ta.createDataSet<uint8_t>("ta_n_valid_objects", ds::From(hdf_ta_ptr.ta_n_valid_objects), props1D)).write(hdf_ta_ptr.ta_n_valid_objects);
      (ta.createDataSet<uint8_t>("ta_n_relevant_objects", ds::From(hdf_ta_ptr.ta_n_relevant_objects), props1D)).write(hdf_ta_ptr.ta_n_relevant_objects);
      (ta.createDataSet<uint8_t>("ta_n_critical_objects", ds::From(hdf_ta_ptr.ta_n_critical_objects), props1D)).write(hdf_ta_ptr.ta_n_critical_objects);
      (ta.createDataSet<int>("ta_algorithm_state", ds::From(hdf_ta_ptr.ta_algorithm_state), props1D)).write(hdf_ta_ptr.ta_algorithm_state);
      (ta.createDataSet<int>("ta_alert_level_left", ds::From(hdf_ta_ptr.ta_alert_level_left), props1D)).write(hdf_ta_ptr.ta_alert_level_left);
      (ta.createDataSet<int>("ta_alert_level_right", ds::From(hdf_ta_ptr.ta_alert_level_right), props1D)).write(hdf_ta_ptr.ta_alert_level_right);
      (ta.createDataSet<float32_T>("ta_object_ta_waypoint_at_collision_m_x_left", ds::From(hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_x_left), props1D)).write(hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_x_left);
      (ta.createDataSet<float32_T>("ta_object_ta_waypoint_at_collision_m_y_left", ds::From(hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_y_left), props1D)).write(hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_y_left);
      (ta.createDataSet<float32_T>("ta_object_ta_ttc_s_left", ds::From(hdf_ta_ptr.ta_object_ta_ttc_s_left), props1D)).write(hdf_ta_ptr.ta_object_ta_ttc_s_left);
      (ta.createDataSet<float32_T>("ta_object_ta_ttp_s_left", ds::From(hdf_ta_ptr.ta_object_ta_ttp_s_left), props1D)).write(hdf_ta_ptr.ta_object_ta_ttp_s_left);
      (ta.createDataSet<float32_T>("ta_object_ta_ttb_s_left", ds::From(hdf_ta_ptr.ta_object_ta_ttb_s_left), props1D)).write(hdf_ta_ptr.ta_object_ta_ttb_s_left);
      (ta.createDataSet<float32_T>("ta_object_ta_decel_estimate_mps2_left", ds::From(hdf_ta_ptr.ta_object_ta_decel_estimate_mps2_left), props1D)).write(hdf_ta_ptr.ta_object_ta_decel_estimate_mps2_left);
      (ta.createDataSet<float32_T>("ta_object_ta_distance_m_left", ds::From(hdf_ta_ptr.ta_object_ta_distance_m_left), props1D)).write(hdf_ta_ptr.ta_object_ta_distance_m_left);
      (ta.createDataSet<uint8_t>("ta_object_ta_id_left", ds::From(hdf_ta_ptr.ta_object_ta_id_left), props1D)).write(hdf_ta_ptr.ta_object_ta_id_left);
      (ta.createDataSet<uint8_t>("ta_object_ta_index_left", ds::From(hdf_ta_ptr.ta_object_ta_index_left), props1D)).write(hdf_ta_ptr.ta_object_ta_index_left);
      (ta.createDataSet<boolean_T>("ta_object_ta_f_obj_in_danger_zone_left", ds::From(hdf_ta_ptr.ta_object_ta_f_obj_in_danger_zone_left), props1D)).write(hdf_ta_ptr.ta_object_ta_f_obj_in_danger_zone_left);
      (ta.createDataSet<boolean_T>("ta_object_ta_f_obj_in_info_zone_left", ds::From(hdf_ta_ptr.ta_object_ta_f_obj_in_info_zone_left), props1D)).write(hdf_ta_ptr.ta_object_ta_f_obj_in_info_zone_left);
      (ta.createDataSet<boolean_T>("ta_object_ta_f_obj_in_wing_zone_left", ds::From(hdf_ta_ptr.ta_object_ta_f_obj_in_wing_zone_left), props1D)).write(hdf_ta_ptr.ta_object_ta_f_obj_in_wing_zone_left);
      (ta.createDataSet<float32_T>("ta_object_ta_waypoint_at_collision_m_x_right", ds::From(hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_x_right), props1D)).write(hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_x_right);
      (ta.createDataSet<float32_T>("ta_object_ta_waypoint_at_collision_m_y_right", ds::From(hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_y_right), props1D)).write(hdf_ta_ptr.ta_object_ta_waypoint_at_collision_m_y_right);
      (ta.createDataSet<float32_T>("ta_object_ta_ttc_s_right", ds::From(hdf_ta_ptr.ta_object_ta_ttc_s_right), props1D)).write(hdf_ta_ptr.ta_object_ta_ttc_s_right);
      (ta.createDataSet<float32_T>("ta_object_ta_ttp_s_right", ds::From(hdf_ta_ptr.ta_object_ta_ttp_s_right), props1D)).write(hdf_ta_ptr.ta_object_ta_ttp_s_right);
      (ta.createDataSet<float32_T>("ta_object_ta_ttb_s_right", ds::From(hdf_ta_ptr.ta_object_ta_ttb_s_right), props1D)).write(hdf_ta_ptr.ta_object_ta_ttb_s_right);
      (ta.createDataSet<float32_T>("ta_object_ta_decel_estimate_mps2_right", ds::From(hdf_ta_ptr.ta_object_ta_decel_estimate_mps2_right), props1D)).write(hdf_ta_ptr.ta_object_ta_decel_estimate_mps2_right);
      (ta.createDataSet<float32_T>("ta_object_ta_distance_m_right", ds::From(hdf_ta_ptr.ta_object_ta_distance_m_right), props1D)).write(hdf_ta_ptr.ta_object_ta_distance_m_right);
      (ta.createDataSet<uint8_t>("ta_object_ta_id_right", ds::From(hdf_ta_ptr.ta_object_ta_id_right), props1D)).write(hdf_ta_ptr.ta_object_ta_id_right);
      (ta.createDataSet<uint8_t>("ta_object_ta_index_right", ds::From(hdf_ta_ptr.ta_object_ta_index_right), props1D)).write(hdf_ta_ptr.ta_object_ta_index_right);
      (ta.createDataSet<boolean_T>("ta_object_ta_f_obj_in_danger_zone_right", ds::From(hdf_ta_ptr.ta_object_ta_f_obj_in_danger_zone_right), props1D)).write(hdf_ta_ptr.ta_object_ta_f_obj_in_danger_zone_right);
      (ta.createDataSet<boolean_T>("ta_object_ta_f_obj_in_info_zone_right", ds::From(hdf_ta_ptr.ta_object_ta_f_obj_in_info_zone_right), props1D)).write(hdf_ta_ptr.ta_object_ta_f_obj_in_info_zone_right);
      (ta.createDataSet<boolean_T>("ta_object_ta_f_obj_in_wing_zone_right", ds::From(hdf_ta_ptr.ta_object_ta_f_obj_in_wing_zone_right), props1D)).write(hdf_ta_ptr.ta_object_ta_f_obj_in_wing_zone_right);

      auto ta_cals = ta.createGroup("Calibration");
      // TA Calibrations
      Ta_Instance_T *ta_generic = Ta_Get_Instance_Ptr();
      (ta_cals.createDataSet<float32_T>("k_ta_alert_lvl_1_ttp_threshold", ds::From(ta_generic->calibration.k_ta_alert_lvl_1_ttp_threshold)).write(ta_generic->calibration.k_ta_alert_lvl_1_ttp_threshold));
      (ta_cals.createDataSet<float32_T>("k_ta_active_obj_ttp_offset", ds::From(ta_generic->calibration.k_ta_active_obj_ttp_offset)).write(ta_generic->calibration.k_ta_active_obj_ttp_offset));
      (ta_cals.createDataSet<float32_T>("k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max", ds::From(ta_generic->calibration.k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max)).write(ta_generic->calibration.k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max));
      (ta_cals.createDataSet<float32_T>("k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max", ds::From(ta_generic->calibration.k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max)).write(ta_generic->calibration.k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max));
      (ta_cals.createDataSet<float32_T>("k_ta_alert_lvl_2_ttc_threshold", ds::From(ta_generic->calibration.k_ta_alert_lvl_2_ttc_threshold)).write(ta_generic->calibration.k_ta_alert_lvl_2_ttc_threshold));
      (ta_cals.createDataSet<float32_T>("k_ta_alert_lvl_3_ttc_threshold", ds::From(ta_generic->calibration.k_ta_alert_lvl_3_ttc_threshold)).write(ta_generic->calibration.k_ta_alert_lvl_3_ttc_threshold));
      (ta_cals.createDataSet<float32_T>("k_ta_alert_lvl_3_ttb_threshold", ds::From(ta_generic->calibration.k_ta_alert_lvl_3_ttb_threshold)).write(ta_generic->calibration.k_ta_alert_lvl_3_ttb_threshold));
      (ta_cals.createDataSet<float32_T>("k_ta_alert_lvl_4_ttc_threshold", ds::From(ta_generic->calibration.k_ta_alert_lvl_4_ttc_threshold)).write(ta_generic->calibration.k_ta_alert_lvl_4_ttc_threshold));
      (ta_cals.createDataSet<float32_T>("k_ta_alert_lvl_4_decel_threshold", ds::From(ta_generic->calibration.k_ta_alert_lvl_4_decel_threshold)).write(ta_generic->calibration.k_ta_alert_lvl_4_decel_threshold));
      (ta_cals.createDataSet<float32_T>("k_ta_critical_approach_min_safe_distance", ds::From(ta_generic->calibration.k_ta_critical_approach_min_safe_distance)).write(ta_generic->calibration.k_ta_critical_approach_min_safe_distance));
      (ta_cals.createDataSet<float32_T>("k_ta_critical_approach_angle_diff_min", ds::From(ta_generic->calibration.k_ta_critical_approach_angle_diff_min)).write(ta_generic->calibration.k_ta_critical_approach_angle_diff_min));
      (ta_cals.createDataSet<float32_T>("k_ta_ego_max_pred_yaw_angle", ds::From(ta_generic->calibration.k_ta_ego_max_pred_yaw_angle)).write(ta_generic->calibration.k_ta_ego_max_pred_yaw_angle));
      (ta_cals.createDataSet<float32_T>("k_ta_obj_pred_speed_min", ds::From(ta_generic->calibration.k_ta_obj_pred_speed_min)).write(ta_generic->calibration.k_ta_obj_pred_speed_min));
      (ta_cals.createDataSet<float32_T>("k_ta_obj_acceleration_long_weight", ds::From(ta_generic->calibration.k_ta_obj_acceleration_long_weight)).write(ta_generic->calibration.k_ta_obj_acceleration_long_weight));
      (ta_cals.createDataSet<float32_T>("k_ta_obj_acceleration_lat_weight", ds::From(ta_generic->calibration.k_ta_obj_acceleration_lat_weight)).write(ta_generic->calibration.k_ta_obj_acceleration_lat_weight));
      (ta_cals.createDataSet<float32_T>("k_ta_ego_shape_gain_per_pred_step", ds::From(ta_generic->calibration.k_ta_ego_shape_gain_per_pred_step)).write(ta_generic->calibration.k_ta_ego_shape_gain_per_pred_step));
      (ta_cals.createDataSet<float32_T>("k_ta_obj_shape_gain_per_pred_step", ds::From(ta_generic->calibration.k_ta_obj_shape_gain_per_pred_step)).write(ta_generic->calibration.k_ta_obj_shape_gain_per_pred_step));
      (ta_cals.createDataSet<float32_T>("k_ta_ego_shape_gain_fixed", ds::From(ta_generic->calibration.k_ta_ego_shape_gain_fixed)).write(ta_generic->calibration.k_ta_ego_shape_gain_fixed));
      (ta_cals.createDataSet<float32_T>("k_ta_obj_shape_gain_fixed", ds::From(ta_generic->calibration.k_ta_obj_shape_gain_fixed)).write(ta_generic->calibration.k_ta_obj_shape_gain_fixed));
      (ta_cals.createDataSet<float32_T>("k_ta_ego_speed", ds::From(ta_generic->calibration.k_ta_ego_speed)).write(ta_generic->calibration.k_ta_ego_speed));
      (ta_cals.createDataSet<float32_T>("k_ta_ego_speed_ofst", ds::From(ta_generic->calibration.k_ta_ego_speed_ofst)).write(ta_generic->calibration.k_ta_ego_speed_ofst));
      (ta_cals.createDataSet<float32_T>("k_ta_ego_yawrate", ds::From(ta_generic->calibration.k_ta_ego_yawrate)).write(ta_generic->calibration.k_ta_ego_yawrate));
      (ta_cals.createDataSet<float32_T>("k_ta_ego_yawrate_ofst", ds::From(ta_generic->calibration.k_ta_ego_yawrate_ofst)).write(ta_generic->calibration.k_ta_ego_yawrate_ofst));
      (ta_cals.createDataSet<float32_T>("k_ta_ego_long_acceleration", ds::From(ta_generic->calibration.k_ta_ego_long_acceleration)).write(ta_generic->calibration.k_ta_ego_long_acceleration));
      (ta_cals.createDataSet<float32_T>("k_ta_ego_long_acceleration_ofst", ds::From(ta_generic->calibration.k_ta_ego_long_acceleration_ofst)).write(ta_generic->calibration.k_ta_ego_long_acceleration_ofst));
      (ta_cals.createDataSet<float32_T>("k_ta_ego_acceleration_weight", ds::From(ta_generic->calibration.k_ta_ego_acceleration_weight)).write(ta_generic->calibration.k_ta_ego_acceleration_weight));
      (ta_cals.createDataSet<float32_T>("k_ta_ego_deceleration_weight", ds::From(ta_generic->calibration.k_ta_ego_deceleration_weight)).write(ta_generic->calibration.k_ta_ego_deceleration_weight));
      (ta_cals.createDataSet<float32_T>("k_ta_ego_circle_offset", ds::From(ta_generic->calibration.k_ta_ego_circle_offset)).write(ta_generic->calibration.k_ta_ego_circle_offset));
      (ta_cals.createDataSet<float32_T>("k_ta_ego_circle_host_length_factor", ds::From(ta_generic->calibration.k_ta_ego_circle_host_length_factor)).write(ta_generic->calibration.k_ta_ego_circle_host_length_factor));
      (ta_cals.createDataSet<float32_T>("k_ta_ego_yawangle_integration_yawrate_min", ds::From(ta_generic->calibration.k_ta_ego_yawangle_integration_yawrate_min)).write(ta_generic->calibration.k_ta_ego_yawangle_integration_yawrate_min));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_exist_prblty", ds::From(ta_generic->calibration.k_fta_obj_exist_prblty)).write(ta_generic->calibration.k_fta_obj_exist_prblty));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_exist_prblty_ofst", ds::From(ta_generic->calibration.k_fta_obj_exist_prblty_ofst)).write(ta_generic->calibration.k_fta_obj_exist_prblty_ofst));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_vcs_long_vel_rel", ds::From(ta_generic->calibration.k_fta_obj_vcs_long_vel_rel)).write(ta_generic->calibration.k_fta_obj_vcs_long_vel_rel));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_vcs_long_vel_rel_ofst", ds::From(ta_generic->calibration.k_fta_obj_vcs_long_vel_rel_ofst)).write(ta_generic->calibration.k_fta_obj_vcs_long_vel_rel_ofst));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_vcs_lat_vel_rel", ds::From(ta_generic->calibration.k_fta_obj_vcs_lat_vel_rel)).write(ta_generic->calibration.k_fta_obj_vcs_lat_vel_rel));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_vcs_lat_vel_rel_ofst", ds::From(ta_generic->calibration.k_fta_obj_vcs_lat_vel_rel_ofst)).write(ta_generic->calibration.k_fta_obj_vcs_lat_vel_rel_ofst));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_vcs_long_vel", ds::From(ta_generic->calibration.k_fta_obj_vcs_long_vel)).write(ta_generic->calibration.k_fta_obj_vcs_long_vel));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_vcs_long_vel_ofst", ds::From(ta_generic->calibration.k_fta_obj_vcs_long_vel_ofst)).write(ta_generic->calibration.k_fta_obj_vcs_long_vel_ofst));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_vcs_lat_vel", ds::From(ta_generic->calibration.k_fta_obj_vcs_lat_vel)).write(ta_generic->calibration.k_fta_obj_vcs_lat_vel));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_vcs_lat_vel_ofst", ds::From(ta_generic->calibration.k_fta_obj_vcs_lat_vel_ofst)).write(ta_generic->calibration.k_fta_obj_vcs_lat_vel_ofst));
      (ta_cals.createDataSet<float32_T>("k_ta_straight_host_curvature_max", ds::From(ta_generic->calibration.k_ta_straight_host_curvature_max)).write(ta_generic->calibration.k_ta_straight_host_curvature_max));
      (ta_cals.createDataSet<float32_T>("k_ta_lookup_turning_host_speed", ds::From(ta_generic->calibration.k_ta_lookup_turning_host_speed)).write(ta_generic->calibration.k_ta_lookup_turning_host_speed));
      (ta_cals.createDataSet<float32_T>("k_ta_lookup_turning_host_curvature_min", ds::From(ta_generic->calibration.k_ta_lookup_turning_host_curvature_min)).write(ta_generic->calibration.k_ta_lookup_turning_host_curvature_min));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_vcs_long_pos_straight_min", ds::From(ta_generic->calibration.k_fta_obj_vcs_long_pos_straight_min)).write(ta_generic->calibration.k_fta_obj_vcs_long_pos_straight_min));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_heading", ds::From(ta_generic->calibration.k_fta_obj_heading)).write(ta_generic->calibration.k_fta_obj_heading));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_heading_straight", ds::From(ta_generic->calibration.k_fta_obj_heading_straight)).write(ta_generic->calibration.k_fta_obj_heading_straight));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_heading_ofst", ds::From(ta_generic->calibration.k_fta_obj_heading_ofst)).write(ta_generic->calibration.k_fta_obj_heading_ofst));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_heading_rate", ds::From(ta_generic->calibration.k_fta_obj_heading_rate)).write(ta_generic->calibration.k_fta_obj_heading_rate));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_heading_rate_straight", ds::From(ta_generic->calibration.k_fta_obj_heading_rate_straight)).write(ta_generic->calibration.k_fta_obj_heading_rate_straight));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_heading_rate_ofst", ds::From(ta_generic->calibration.k_fta_obj_heading_rate_ofst)).write(ta_generic->calibration.k_fta_obj_heading_rate_ofst));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_speed", ds::From(ta_generic->calibration.k_fta_obj_speed)).write(ta_generic->calibration.k_fta_obj_speed));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_speed_straight", ds::From(ta_generic->calibration.k_fta_obj_speed_straight)).write(ta_generic->calibration.k_fta_obj_speed_straight));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_speed_ofst", ds::From(ta_generic->calibration.k_fta_obj_speed_ofst)).write(ta_generic->calibration.k_fta_obj_speed_ofst));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_length", ds::From(ta_generic->calibration.k_fta_obj_length)).write(ta_generic->calibration.k_fta_obj_length));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_length_ofst", ds::From(ta_generic->calibration.k_fta_obj_length_ofst)).write(ta_generic->calibration.k_fta_obj_length_ofst));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_width", ds::From(ta_generic->calibration.k_fta_obj_width)).write(ta_generic->calibration.k_fta_obj_width));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_width_ofst", ds::From(ta_generic->calibration.k_fta_obj_width_ofst)).write(ta_generic->calibration.k_fta_obj_width_ofst));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_area", ds::From(ta_generic->calibration.k_fta_obj_area)).write(ta_generic->calibration.k_fta_obj_area));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_area_ofst", ds::From(ta_generic->calibration.k_fta_obj_area_ofst)).write(ta_generic->calibration.k_fta_obj_area_ofst));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_vru_class_prob", ds::From(ta_generic->calibration.k_fta_obj_vru_class_prob)).write(ta_generic->calibration.k_fta_obj_vru_class_prob));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_vru_class_prob_ofst", ds::From(ta_generic->calibration.k_fta_obj_vru_class_prob_ofst)).write(ta_generic->calibration.k_fta_obj_vru_class_prob_ofst));
      (ta_cals.createDataSet<float32_T>("k_fta_ego_obj_heading_diff", ds::From(ta_generic->calibration.k_fta_ego_obj_heading_diff)).write(ta_generic->calibration.k_fta_ego_obj_heading_diff));
      (ta_cals.createDataSet<float32_T>("k_fta_ego_obj_heading_diff_ofst", ds::From(ta_generic->calibration.k_fta_ego_obj_heading_diff_ofst)).write(ta_generic->calibration.k_fta_ego_obj_heading_diff_ofst));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_eclipse_value", ds::From(ta_generic->calibration.k_fta_obj_eclipse_value)).write(ta_generic->calibration.k_fta_obj_eclipse_value));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_eclipse_value_ofst", ds::From(ta_generic->calibration.k_fta_obj_eclipse_value_ofst)).write(ta_generic->calibration.k_fta_obj_eclipse_value_ofst));
      (ta_cals.createDataSet<float32_T>("k_fta_obj_velocity_heading_diff_max", ds::From(ta_generic->calibration.k_fta_obj_velocity_heading_diff_max)).write(ta_generic->calibration.k_fta_obj_velocity_heading_diff_max));
      (ta_cals.createDataSet<float32_T>("k_fta_brake_deceleration_max", ds::From(ta_generic->calibration.k_fta_brake_deceleration_max)).write(ta_generic->calibration.k_fta_brake_deceleration_max));
      (ta_cals.createDataSet<float32_T>("k_fta_brake_dead_time", ds::From(ta_generic->calibration.k_fta_brake_dead_time)).write(ta_generic->calibration.k_fta_brake_dead_time));
      (ta_cals.createDataSet<float32_T>("k_fta_brake_gradient", ds::From(ta_generic->calibration.k_fta_brake_gradient)).write(ta_generic->calibration.k_fta_brake_gradient));
      (ta_cals.createDataSet<float32_T>("k_fta_danger_zone_left_long", ds::From(ta_generic->calibration.k_fta_danger_zone_left_long)).write(ta_generic->calibration.k_fta_danger_zone_left_long));
      (ta_cals.createDataSet<float32_T>("k_fta_danger_zone_left_lat", ds::From(ta_generic->calibration.k_fta_danger_zone_left_lat)).write(ta_generic->calibration.k_fta_danger_zone_left_lat));
      (ta_cals.createDataSet<float32_T>("k_fta_danger_zone_right_long", ds::From(ta_generic->calibration.k_fta_danger_zone_right_long)).write(ta_generic->calibration.k_fta_danger_zone_right_long));
      (ta_cals.createDataSet<float32_T>("k_fta_danger_zone_right_lat", ds::From(ta_generic->calibration.k_fta_danger_zone_right_lat)).write(ta_generic->calibration.k_fta_danger_zone_right_lat));
      (ta_cals.createDataSet<float32_T>("k_pfgs_ego_speed", ds::From(ta_generic->calibration.k_pfgs_ego_speed)).write(ta_generic->calibration.k_pfgs_ego_speed));
      (ta_cals.createDataSet<float32_T>("k_pfgs_qualification_ttc_min", ds::From(ta_generic->calibration.k_pfgs_qualification_ttc_min)).write(ta_generic->calibration.k_pfgs_qualification_ttc_min));
      (ta_cals.createDataSet<float32_T>("k_tap_lvl_2_host_curvature_min", ds::From(ta_generic->calibration.k_tap_lvl_2_host_curvature_min)).write(ta_generic->calibration.k_tap_lvl_2_host_curvature_min));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_exist_prblty", ds::From(ta_generic->calibration.k_rta_obj_exist_prblty)).write(ta_generic->calibration.k_rta_obj_exist_prblty));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_exist_prblty_ofst", ds::From(ta_generic->calibration.k_rta_obj_exist_prblty_ofst)).write(ta_generic->calibration.k_rta_obj_exist_prblty_ofst));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_vcs_long_vel_rel", ds::From(ta_generic->calibration.k_rta_obj_vcs_long_vel_rel)).write(ta_generic->calibration.k_rta_obj_vcs_long_vel_rel));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_vcs_long_vel_rel_ofst", ds::From(ta_generic->calibration.k_rta_obj_vcs_long_vel_rel_ofst)).write(ta_generic->calibration.k_rta_obj_vcs_long_vel_rel_ofst));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_vcs_lat_vel_rel", ds::From(ta_generic->calibration.k_rta_obj_vcs_lat_vel_rel)).write(ta_generic->calibration.k_rta_obj_vcs_lat_vel_rel));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_vcs_lat_vel_rel_ofst", ds::From(ta_generic->calibration.k_rta_obj_vcs_lat_vel_rel_ofst)).write(ta_generic->calibration.k_rta_obj_vcs_lat_vel_rel_ofst));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_vcs_long_vel", ds::From(ta_generic->calibration.k_rta_obj_vcs_long_vel)).write(ta_generic->calibration.k_rta_obj_vcs_long_vel));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_vcs_long_vel_ofst", ds::From(ta_generic->calibration.k_rta_obj_vcs_long_vel_ofst)).write(ta_generic->calibration.k_rta_obj_vcs_long_vel_ofst));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_vcs_lat_vel", ds::From(ta_generic->calibration.k_rta_obj_vcs_lat_vel)).write(ta_generic->calibration.k_rta_obj_vcs_lat_vel));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_vcs_lat_vel_ofst", ds::From(ta_generic->calibration.k_rta_obj_vcs_lat_vel_ofst)).write(ta_generic->calibration.k_rta_obj_vcs_lat_vel_ofst));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_heading", ds::From(ta_generic->calibration.k_rta_obj_heading)).write(ta_generic->calibration.k_rta_obj_heading));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_heading_ofst", ds::From(ta_generic->calibration.k_rta_obj_heading_ofst)).write(ta_generic->calibration.k_rta_obj_heading_ofst));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_speed", ds::From(ta_generic->calibration.k_rta_obj_speed)).write(ta_generic->calibration.k_rta_obj_speed));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_speed_ofst", ds::From(ta_generic->calibration.k_rta_obj_speed_ofst)).write(ta_generic->calibration.k_rta_obj_speed_ofst));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_length", ds::From(ta_generic->calibration.k_rta_obj_length)).write(ta_generic->calibration.k_rta_obj_length));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_length_ofst", ds::From(ta_generic->calibration.k_rta_obj_length_ofst)).write(ta_generic->calibration.k_rta_obj_length_ofst));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_width", ds::From(ta_generic->calibration.k_rta_obj_width)).write(ta_generic->calibration.k_rta_obj_width));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_width_ofst", ds::From(ta_generic->calibration.k_rta_obj_width_ofst)).write(ta_generic->calibration.k_rta_obj_width_ofst));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_vru_class_prob", ds::From(ta_generic->calibration.k_rta_obj_vru_class_prob)).write(ta_generic->calibration.k_rta_obj_vru_class_prob));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_vru_class_prob_ofst", ds::From(ta_generic->calibration.k_rta_obj_vru_class_prob_ofst)).write(ta_generic->calibration.k_rta_obj_vru_class_prob_ofst));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_eclipse_value", ds::From(ta_generic->calibration.k_rta_obj_eclipse_value)).write(ta_generic->calibration.k_rta_obj_eclipse_value));
      (ta_cals.createDataSet<float32_T>("k_rta_obj_eclipse_value_ofst", ds::From(ta_generic->calibration.k_rta_obj_eclipse_value_ofst)).write(ta_generic->calibration.k_rta_obj_eclipse_value_ofst));
      (ta_cals.createDataSet<float32_T>("k_rta_ttp_obj_abs_lat_vel_rel_max", ds::From(ta_generic->calibration.k_rta_ttp_obj_abs_lat_vel_rel_max)).write(ta_generic->calibration.k_rta_ttp_obj_abs_lat_vel_rel_max));
      (ta_cals.createDataSet<float32_T>("k_rta_ttp_obj_abs_heading_diff_max", ds::From(ta_generic->calibration.k_rta_ttp_obj_abs_heading_diff_max)).write(ta_generic->calibration.k_rta_ttp_obj_abs_heading_diff_max));
      (ta_cals.createDataSet<float32_T>("k_rta_ttp_curve_suppression_obj_distance_min", ds::From(ta_generic->calibration.k_rta_ttp_curve_suppression_obj_distance_min)).write(ta_generic->calibration.k_rta_ttp_curve_suppression_obj_distance_min));
      (ta_cals.createDataSet<float32_T>("k_rta_info_zone_left_long", ds::From(ta_generic->calibration.k_rta_info_zone_left_long)).write(ta_generic->calibration.k_rta_info_zone_left_long));
      (ta_cals.createDataSet<float32_T>("k_rta_info_zone_left_lat", ds::From(ta_generic->calibration.k_rta_info_zone_left_lat)).write(ta_generic->calibration.k_rta_info_zone_left_lat));
      (ta_cals.createDataSet<float32_T>("k_rta_info_zone_right_long", ds::From(ta_generic->calibration.k_rta_info_zone_right_long)).write(ta_generic->calibration.k_rta_info_zone_right_long));
      (ta_cals.createDataSet<float32_T>("k_rta_info_zone_right_lat", ds::From(ta_generic->calibration.k_rta_info_zone_right_lat)).write(ta_generic->calibration.k_rta_info_zone_right_lat));
      (ta_cals.createDataSet<float32_T>("k_rta_info_zone_left_long_hys", ds::From(ta_generic->calibration.k_rta_info_zone_left_long_hys)).write(ta_generic->calibration.k_rta_info_zone_left_long_hys));
      (ta_cals.createDataSet<float32_T>("k_rta_info_zone_left_lat_hys", ds::From(ta_generic->calibration.k_rta_info_zone_left_lat_hys)).write(ta_generic->calibration.k_rta_info_zone_left_lat_hys));
      (ta_cals.createDataSet<float32_T>("k_rta_info_zone_right_long_hys", ds::From(ta_generic->calibration.k_rta_info_zone_right_long_hys)).write(ta_generic->calibration.k_rta_info_zone_right_long_hys));
      (ta_cals.createDataSet<float32_T>("k_rta_info_zone_right_lat_hys", ds::From(ta_generic->calibration.k_rta_info_zone_right_lat_hys)).write(ta_generic->calibration.k_rta_info_zone_right_lat_hys));
      (ta_cals.createDataSet<float32_T>("k_rta_wing_zone_left_long", ds::From(ta_generic->calibration.k_rta_wing_zone_left_long)).write(ta_generic->calibration.k_rta_wing_zone_left_long));
      (ta_cals.createDataSet<float32_T>("k_rta_wing_zone_left_lat", ds::From(ta_generic->calibration.k_rta_wing_zone_left_lat)).write(ta_generic->calibration.k_rta_wing_zone_left_lat));
      (ta_cals.createDataSet<float32_T>("k_rta_wing_zone_right_long", ds::From(ta_generic->calibration.k_rta_wing_zone_right_long)).write(ta_generic->calibration.k_rta_wing_zone_right_long));
      (ta_cals.createDataSet<float32_T>("k_rta_wing_zone_right_lat", ds::From(ta_generic->calibration.k_rta_wing_zone_right_lat)).write(ta_generic->calibration.k_rta_wing_zone_right_lat));
      (ta_cals.createDataSet<float32_T>("k_rta_wing_zone_left_long_hys", ds::From(ta_generic->calibration.k_rta_wing_zone_left_long_hys)).write(ta_generic->calibration.k_rta_wing_zone_left_long_hys));
      (ta_cals.createDataSet<float32_T>("k_rta_wing_zone_left_lat_hys", ds::From(ta_generic->calibration.k_rta_wing_zone_left_lat_hys)).write(ta_generic->calibration.k_rta_wing_zone_left_lat_hys));
      (ta_cals.createDataSet<float32_T>("k_rta_wing_zone_right_long_hys", ds::From(ta_generic->calibration.k_rta_wing_zone_right_long_hys)).write(ta_generic->calibration.k_rta_wing_zone_right_long_hys));
      (ta_cals.createDataSet<float32_T>("k_rta_wing_zone_right_lat_hys", ds::From(ta_generic->calibration.k_rta_wing_zone_right_lat_hys)).write(ta_generic->calibration.k_rta_wing_zone_right_lat_hys));
      (ta_cals.createDataSet<boolean_T>("k_ta_always_overwrite_ta_mode_to_both", ds::From(ta_generic->calibration.k_ta_always_overwrite_ta_mode_to_both)).write(ta_generic->calibration.k_ta_always_overwrite_ta_mode_to_both));
      (ta_cals.createDataSet<boolean_T>("k_ta_f_only_allow_consecutive_ttc_based_alert_levels", ds::From(ta_generic->calibration.k_ta_f_only_allow_consecutive_ttc_based_alert_levels)).write(ta_generic->calibration.k_ta_f_only_allow_consecutive_ttc_based_alert_levels));
      (ta_cals.createDataSet<boolean_T>("k_ta_f_skip_holding_for_single_alert_level_drop", ds::From(ta_generic->calibration.k_ta_f_skip_holding_for_single_alert_level_drop)).write(ta_generic->calibration.k_ta_f_skip_holding_for_single_alert_level_drop));
      (ta_cals.createDataSet<boolean_T>("k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj", ds::From(ta_generic->calibration.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj)).write(ta_generic->calibration.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj));
      (ta_cals.createDataSet<boolean_T>("k_ta_f_apply_ttp_hysteresis_globally", ds::From(ta_generic->calibration.k_ta_f_apply_ttp_hysteresis_globally)).write(ta_generic->calibration.k_ta_f_apply_ttp_hysteresis_globally));
      (ta_cals.createDataSet<boolean_T>("k_f_ta_enable_debug_mode", ds::From(ta_generic->calibration.k_f_ta_enable_debug_mode)).write(ta_generic->calibration.k_f_ta_enable_debug_mode));
      (ta_cals.createDataSet<boolean_T>("k_f_fta_enable", ds::From(ta_generic->calibration.k_f_fta_enable)).write(ta_generic->calibration.k_f_fta_enable));
      (ta_cals.createDataSet<boolean_T>("k_f_fta_enable_brake_gradient_logic", ds::From(ta_generic->calibration.k_f_fta_enable_brake_gradient_logic)).write(ta_generic->calibration.k_f_fta_enable_brake_gradient_logic));
      (ta_cals.createDataSet<boolean_T>("k_f_fta_enable_danger_zones", ds::From(ta_generic->calibration.k_f_fta_enable_danger_zones)).write(ta_generic->calibration.k_f_fta_enable_danger_zones));
      (ta_cals.createDataSet<boolean_T>("k_pfgs_symbol_request_sides_enabled", ds::From(ta_generic->calibration.k_pfgs_symbol_request_sides_enabled)).write(ta_generic->calibration.k_pfgs_symbol_request_sides_enabled));
      (ta_cals.createDataSet<boolean_T>("k_pfgs_qualification_check_f_stationary", ds::From(ta_generic->calibration.k_pfgs_qualification_check_f_stationary)).write(ta_generic->calibration.k_pfgs_qualification_check_f_stationary));
      (ta_cals.createDataSet<boolean_T>("k_f_rta_enable", ds::From(ta_generic->calibration.k_f_rta_enable)).write(ta_generic->calibration.k_f_rta_enable));
      (ta_cals.createDataSet<boolean_T>("k_f_rta_enable_info_zones", ds::From(ta_generic->calibration.k_f_rta_enable_info_zones)).write(ta_generic->calibration.k_f_rta_enable_info_zones));
      (ta_cals.createDataSet<boolean_T>("k_rta_f_higher_obj_crit_based_on_lower_ttp", ds::From(ta_generic->calibration.k_rta_f_higher_obj_crit_based_on_lower_ttp)).write(ta_generic->calibration.k_rta_f_higher_obj_crit_based_on_lower_ttp));
      (ta_cals.createDataSet<boolean_T>("k_f_rta_enable_wing_zones", ds::From(ta_generic->calibration.k_f_rta_enable_wing_zones)).write(ta_generic->calibration.k_f_rta_enable_wing_zones));
      (ta_cals.createDataSet<uint8_t>("k_ta_alert_qualifying_cycles", ds::From(ta_generic->calibration.k_ta_alert_qualifying_cycles)).write(ta_generic->calibration.k_ta_alert_qualifying_cycles));
      (ta_cals.createDataSet<uint8_t>("k_ta_alert_holding_cycles", ds::From(ta_generic->calibration.k_ta_alert_holding_cycles)).write(ta_generic->calibration.k_ta_alert_holding_cycles));
      (ta_cals.createDataSet<uint8_t>("k_ta_prediction_steps_max", ds::From(ta_generic->calibration.k_ta_prediction_steps_max)).write(ta_generic->calibration.k_ta_prediction_steps_max));
      (ta_cals.createDataSet<uint8_t>("k_ta_critical_approach_check_ego_circles", ds::From(ta_generic->calibration.k_ta_critical_approach_check_ego_circles)).write(ta_generic->calibration.k_ta_critical_approach_check_ego_circles));
      (ta_cals.createDataSet<uint8_t>("k_ta_ego_pred_const_velocity_pred_steps_min", ds::From(ta_generic->calibration.k_ta_ego_pred_const_velocity_pred_steps_min)).write(ta_generic->calibration.k_ta_ego_pred_const_velocity_pred_steps_min));
      (ta_cals.createDataSet<uint8_t>("k_fta_obj_age_min", ds::From(ta_generic->calibration.k_fta_obj_age_min)).write(ta_generic->calibration.k_fta_obj_age_min));
      (ta_cals.createDataSet<uint8_t>("k_fta_danger_zone_point_size", ds::From(ta_generic->calibration.k_fta_danger_zone_point_size)).write(ta_generic->calibration.k_fta_danger_zone_point_size));
      (ta_cals.createDataSet<uint8_t>("k_pfgs_qualification_counter_fast_obj", ds::From(ta_generic->calibration.k_pfgs_qualification_counter_fast_obj)).write(ta_generic->calibration.k_pfgs_qualification_counter_fast_obj));
      (ta_cals.createDataSet<uint8_t>("k_pfgs_qualification_counter_slow_obj", ds::From(ta_generic->calibration.k_pfgs_qualification_counter_slow_obj)).write(ta_generic->calibration.k_pfgs_qualification_counter_slow_obj));
      (ta_cals.createDataSet<uint8_t>("k_rta_obj_age_min", ds::From(ta_generic->calibration.k_rta_obj_age_min)).write(ta_generic->calibration.k_rta_obj_age_min));
      (ta_cals.createDataSet<uint8_t>("k_rta_info_zone_point_size", ds::From(ta_generic->calibration.k_rta_info_zone_point_size)).write(ta_generic->calibration.k_rta_info_zone_point_size));
      (ta_cals.createDataSet<uint8_t>("k_rta_wing_zone_point_size", ds::From(ta_generic->calibration.k_rta_wing_zone_point_size)).write(ta_generic->calibration.k_rta_wing_zone_point_size));

      // Write SCW feature output
      auto scw = g5.createGroup("SCW");
      (scw.createDataSet<boolean_T>("f_scw_enabled", ds::From(hdf_scw_ptr.f_scw_enabled), props1D)).write(hdf_scw_ptr.f_scw_enabled);
      (scw.createDataSet<boolean_T>("f_scw_dyn_enabled", ds::From(hdf_scw_ptr.f_scw_dyn_enabled), props1D)).write(hdf_scw_ptr.f_scw_dyn_enabled);
      (scw.createDataSet<boolean_T>("f_scw_guardrail_enabled", ds::From(hdf_scw_ptr.f_scw_guardrail_enabled), props1D)).write(hdf_scw_ptr.f_scw_guardrail_enabled);
      (scw.createDataSet<int>("scw_object_alert_level_left", ds::From(hdf_scw_ptr.scw_object_alert_level_left), props1D)).write(hdf_scw_ptr.scw_object_alert_level_left);
      (scw.createDataSet<uint8_t>("scw_object_id_left", ds::From(hdf_scw_ptr.scw_object_id_left), props1D)).write(hdf_scw_ptr.scw_object_id_left);
      (scw.createDataSet<uint32_t>("scw_object_unique_id_left", ds::From(hdf_scw_ptr.scw_object_unique_id_left), props1D)).write(hdf_scw_ptr.scw_object_unique_id_left);
      (scw.createDataSet<int>("scw_object_type_left", ds::From(hdf_scw_ptr.scw_object_type_left), props1D)).write(hdf_scw_ptr.scw_object_type_left);
      (scw.createDataSet<float32_T>("scw_object_lateral_ttc_s_left", ds::From(hdf_scw_ptr.scw_object_lateral_ttc_s_left), props1D)).write(hdf_scw_ptr.scw_object_lateral_ttc_s_left);
      (scw.createDataSet<float32_T>("scw_object_lateral_distance_m_left", ds::From(hdf_scw_ptr.scw_object_lateral_distance_m_left), props1D)).write(hdf_scw_ptr.scw_object_lateral_distance_m_left);
      (scw.createDataSet<float32_T>("scw_object_position_m_x_left", ds::From(hdf_scw_ptr.scw_object_position_m_x_left), props1D)).write(hdf_scw_ptr.scw_object_position_m_x_left);
      (scw.createDataSet<float32_T>("scw_object_position_m_y_left", ds::From(hdf_scw_ptr.scw_object_position_m_y_left), props1D)).write(hdf_scw_ptr.scw_object_position_m_y_left);
      (scw.createDataSet<float32_T>("scw_object_velocity_mps_x_left", ds::From(hdf_scw_ptr.scw_object_velocity_mps_x_left), props1D)).write(hdf_scw_ptr.scw_object_velocity_mps_x_left);
      (scw.createDataSet<float32_T>("scw_object_velocity_mps_y_left", ds::From(hdf_scw_ptr.scw_object_velocity_mps_y_left), props1D)).write(hdf_scw_ptr.scw_object_velocity_mps_y_left);
      (scw.createDataSet<float32_T>("scw_object_acceleration_mps2_x_left", ds::From(hdf_scw_ptr.scw_object_acceleration_mps2_x_left), props1D)).write(hdf_scw_ptr.scw_object_acceleration_mps2_x_left);
      (scw.createDataSet<float32_T>("scw_object_acceleration_mps2_y_left", ds::From(hdf_scw_ptr.scw_object_acceleration_mps2_y_left), props1D)).write(hdf_scw_ptr.scw_object_acceleration_mps2_y_left);
      (scw.createDataSet<float32_T>("scw_object_width_m_left", ds::From(hdf_scw_ptr.scw_object_width_m_left), props1D)).write(hdf_scw_ptr.scw_object_width_m_left);
      (scw.createDataSet<float32_T>("scw_object_length_m_left", ds::From(hdf_scw_ptr.scw_object_length_m_left), props1D)).write(hdf_scw_ptr.scw_object_length_m_left);
      (scw.createDataSet<float32_T>("scw_object_heading_rad_left", ds::From(hdf_scw_ptr.scw_object_heading_rad_left), props1D)).write(hdf_scw_ptr.scw_object_heading_rad_left);
      (scw.createDataSet<float32_T>("scw_object_existence_probability_left", ds::From(hdf_scw_ptr.scw_object_existence_probability_left), props1D)).write(hdf_scw_ptr.scw_object_existence_probability_left);
      (scw.createDataSet<uint16_t>("scw_object_age_left", ds::From(hdf_scw_ptr.scw_object_age_left), props1D)).write(hdf_scw_ptr.scw_object_age_left);
      (scw.createDataSet<int>("scw_object_alert_level_right", ds::From(hdf_scw_ptr.scw_object_alert_level_right), props1D)).write(hdf_scw_ptr.scw_object_alert_level_right);
      (scw.createDataSet<uint8_t>("scw_object_id_right", ds::From(hdf_scw_ptr.scw_object_id_right), props1D)).write(hdf_scw_ptr.scw_object_id_right);
      (scw.createDataSet<uint32_t>("scw_object_unique_id_right", ds::From(hdf_scw_ptr.scw_object_unique_id_right), props1D)).write(hdf_scw_ptr.scw_object_unique_id_right);
      (scw.createDataSet<int>("scw_object_type_right", ds::From(hdf_scw_ptr.scw_object_type_right), props1D)).write(hdf_scw_ptr.scw_object_type_right);
      (scw.createDataSet<float32_T>("scw_object_lateral_ttc_s_right", ds::From(hdf_scw_ptr.scw_object_lateral_ttc_s_right), props1D)).write(hdf_scw_ptr.scw_object_lateral_ttc_s_right);
      (scw.createDataSet<float32_T>("scw_object_lateral_distance_m_right", ds::From(hdf_scw_ptr.scw_object_lateral_distance_m_right), props1D)).write(hdf_scw_ptr.scw_object_lateral_distance_m_right);
      (scw.createDataSet<float32_T>("scw_object_position_m_x_right", ds::From(hdf_scw_ptr.scw_object_position_m_x_right), props1D)).write(hdf_scw_ptr.scw_object_position_m_x_right);
      (scw.createDataSet<float32_T>("scw_object_position_m_y_right", ds::From(hdf_scw_ptr.scw_object_position_m_y_right), props1D)).write(hdf_scw_ptr.scw_object_position_m_y_right);
      (scw.createDataSet<float32_T>("scw_object_velocity_mps_x_right", ds::From(hdf_scw_ptr.scw_object_velocity_mps_x_right), props1D)).write(hdf_scw_ptr.scw_object_velocity_mps_x_right);
      (scw.createDataSet<float32_T>("scw_object_velocity_mps_y_right", ds::From(hdf_scw_ptr.scw_object_velocity_mps_y_right), props1D)).write(hdf_scw_ptr.scw_object_velocity_mps_y_right);
      (scw.createDataSet<float32_T>("scw_object_acceleration_mps2_x_right", ds::From(hdf_scw_ptr.scw_object_acceleration_mps2_x_right), props1D)).write(hdf_scw_ptr.scw_object_acceleration_mps2_x_right);
      (scw.createDataSet<float32_T>("scw_object_acceleration_mps2_y_right", ds::From(hdf_scw_ptr.scw_object_acceleration_mps2_y_right), props1D)).write(hdf_scw_ptr.scw_object_acceleration_mps2_y_right);
      (scw.createDataSet<float32_T>("scw_object_width_m_right", ds::From(hdf_scw_ptr.scw_object_width_m_right), props1D)).write(hdf_scw_ptr.scw_object_width_m_right);
      (scw.createDataSet<float32_T>("scw_object_length_m_right", ds::From(hdf_scw_ptr.scw_object_length_m_right), props1D)).write(hdf_scw_ptr.scw_object_length_m_right);
      (scw.createDataSet<float32_T>("scw_object_heading_rad_right", ds::From(hdf_scw_ptr.scw_object_heading_rad_right), props1D)).write(hdf_scw_ptr.scw_object_heading_rad_right);
      (scw.createDataSet<float32_T>("scw_object_existence_probability_right", ds::From(hdf_scw_ptr.scw_object_existence_probability_right), props1D)).write(hdf_scw_ptr.scw_object_existence_probability_right);
      (scw.createDataSet<uint16_t>("scw_object_age_right", ds::From(hdf_scw_ptr.scw_object_age_right), props1D)).write(hdf_scw_ptr.scw_object_age_right);

      auto scw_cals = scw.createGroup("Calibration");
      // SCW Calibrations
      Scw_Instance_T *scw_generic = Scw_Get_Instance_Ptr();
      (scw_cals.createDataSet<float32_T>("k_scw_min_host_speed", ds::From(scw_generic->calibration.k_scw_min_host_speed)).write(scw_generic->calibration.k_scw_min_host_speed));
      (scw_cals.createDataSet<float32_T>("k_scw_min_host_speed_hys", ds::From(scw_generic->calibration.k_scw_min_host_speed_hys)).write(scw_generic->calibration.k_scw_min_host_speed_hys));
      (scw_cals.createDataSet<float32_T>("k_scw_max_lat_pos_ratio", ds::From(scw_generic->calibration.k_scw_max_lat_pos_ratio)).write(scw_generic->calibration.k_scw_max_lat_pos_ratio));
      (scw_cals.createDataSet<float32_T>("k_scw_candidate_heading", ds::From(scw_generic->calibration.k_scw_candidate_heading)).write(scw_generic->calibration.k_scw_candidate_heading));
      (scw_cals.createDataSet<float32_T>("k_scw_candidate_heading_hys", ds::From(scw_generic->calibration.k_scw_candidate_heading_hys)).write(scw_generic->calibration.k_scw_candidate_heading_hys));
      (scw_cals.createDataSet<float32_T>("k_scw_candidate_yawrate", ds::From(scw_generic->calibration.k_scw_candidate_yawrate)).write(scw_generic->calibration.k_scw_candidate_yawrate));
      (scw_cals.createDataSet<float32_T>("k_scw_candidate_yawrate_hys", ds::From(scw_generic->calibration.k_scw_candidate_yawrate_hys)).write(scw_generic->calibration.k_scw_candidate_yawrate_hys));
      (scw_cals.createDataSet<float32_T>("k_scw_candidate_velocity", ds::From(scw_generic->calibration.k_scw_candidate_velocity)).write(scw_generic->calibration.k_scw_candidate_velocity));
      (scw_cals.createDataSet<float32_T>("k_scw_candidate_velocity_hys", ds::From(scw_generic->calibration.k_scw_candidate_velocity_hys)).write(scw_generic->calibration.k_scw_candidate_velocity_hys));
      (scw_cals.createDataSet<float32_T>("k_scw_candidate_relative_velocity", ds::From(scw_generic->calibration.k_scw_candidate_relative_velocity)).write(scw_generic->calibration.k_scw_candidate_relative_velocity));
      (scw_cals.createDataSet<float32_T>("k_scw_candidate_relative_vel_hys", ds::From(scw_generic->calibration.k_scw_candidate_relative_vel_hys)).write(scw_generic->calibration.k_scw_candidate_relative_vel_hys));
      (scw_cals.createDataSet<float32_T>("k_scw_min_candidate_existence_probability", ds::From(scw_generic->calibration.k_scw_min_candidate_existence_probability)).write(scw_generic->calibration.k_scw_min_candidate_existence_probability));
      (scw_cals.createDataSet<float32_T>("k_scw_min_exist_prob_radar_guardrail", ds::From(scw_generic->calibration.k_scw_min_exist_prob_radar_guardrail)).write(scw_generic->calibration.k_scw_min_exist_prob_radar_guardrail));
      (scw_cals.createDataSet<float32_T>("k_scw_min_dynamic_lat_ttc", ds::From(scw_generic->calibration.k_scw_min_dynamic_lat_ttc)).write(scw_generic->calibration.k_scw_min_dynamic_lat_ttc));
      (scw_cals.createDataSet<float32_T>("k_scw_max_dynamic_lat_ttc", ds::From(scw_generic->calibration.k_scw_max_dynamic_lat_ttc)).write(scw_generic->calibration.k_scw_max_dynamic_lat_ttc));
      (scw_cals.createDataSet<float32_T>("k_scw_min_guardrail_lat_ttc", ds::From(scw_generic->calibration.k_scw_min_guardrail_lat_ttc)).write(scw_generic->calibration.k_scw_min_guardrail_lat_ttc));
      (scw_cals.createDataSet<float32_T>("k_scw_max_guardrail_lat_ttc", ds::From(scw_generic->calibration.k_scw_max_guardrail_lat_ttc)).write(scw_generic->calibration.k_scw_max_guardrail_lat_ttc));
      (scw_cals.createDataSet<float32_T>("k_scw_trailer_lat_ttc_extension", ds::From(scw_generic->calibration.k_scw_trailer_lat_ttc_extension)).write(scw_generic->calibration.k_scw_trailer_lat_ttc_extension));
      (scw_cals.createDataSet<float32_T>("k_scw_min_dynamic_lat_distance", ds::From(scw_generic->calibration.k_scw_min_dynamic_lat_distance)).write(scw_generic->calibration.k_scw_min_dynamic_lat_distance));
      (scw_cals.createDataSet<float32_T>("k_scw_max_dynamic_lat_distance", ds::From(scw_generic->calibration.k_scw_max_dynamic_lat_distance)).write(scw_generic->calibration.k_scw_max_dynamic_lat_distance));
      (scw_cals.createDataSet<float32_T>("k_scw_min_guardrail_lat_distance", ds::From(scw_generic->calibration.k_scw_min_guardrail_lat_distance)).write(scw_generic->calibration.k_scw_min_guardrail_lat_distance));
      (scw_cals.createDataSet<float32_T>("k_scw_max_guardrail_lat_distance", ds::From(scw_generic->calibration.k_scw_max_guardrail_lat_distance)).write(scw_generic->calibration.k_scw_max_guardrail_lat_distance));
      (scw_cals.createDataSet<float32_T>("k_scw_critical_lat_ttc_hys", ds::From(scw_generic->calibration.k_scw_critical_lat_ttc_hys)).write(scw_generic->calibration.k_scw_critical_lat_ttc_hys));
      (scw_cals.createDataSet<float32_T>("k_scw_critical_lat_distance_hys", ds::From(scw_generic->calibration.k_scw_critical_lat_distance_hys)).write(scw_generic->calibration.k_scw_critical_lat_distance_hys));
      (scw_cals.createDataSet<float32_T>("k_scw_initial_zone_x", ds::From(scw_generic->calibration.k_scw_initial_zone_x)).write(scw_generic->calibration.k_scw_initial_zone_x));
      (scw_cals.createDataSet<float32_T>("k_scw_initial_zone_y", ds::From(scw_generic->calibration.k_scw_initial_zone_y)).write(scw_generic->calibration.k_scw_initial_zone_y));
      (scw_cals.createDataSet<float32_T>("k_scw_hys_zone_x_offset", ds::From(scw_generic->calibration.k_scw_hys_zone_x_offset)).write(scw_generic->calibration.k_scw_hys_zone_x_offset));
      (scw_cals.createDataSet<float32_T>("k_scw_hys_zone_y_offset", ds::From(scw_generic->calibration.k_scw_hys_zone_y_offset)).write(scw_generic->calibration.k_scw_hys_zone_y_offset));
      (scw_cals.createDataSet<float32_T>("k_scw_lateral_distance_default", ds::From(scw_generic->calibration.k_scw_lateral_distance_default)).write(scw_generic->calibration.k_scw_lateral_distance_default));
      (scw_cals.createDataSet<float32_T>("k_scw_lateral_ttc_max", ds::From(scw_generic->calibration.k_scw_lateral_ttc_max)).write(scw_generic->calibration.k_scw_lateral_ttc_max));
      (scw_cals.createDataSet<float32_T>("k_scw_lateral_ttc_default", ds::From(scw_generic->calibration.k_scw_lateral_ttc_default)).write(scw_generic->calibration.k_scw_lateral_ttc_default));
      (scw_cals.createDataSet<float32_T>("k_scw_ttle_max", ds::From(scw_generic->calibration.k_scw_ttle_max)).write(scw_generic->calibration.k_scw_ttle_max));
      (scw_cals.createDataSet<float32_T>("k_scw_ttle_default", ds::From(scw_generic->calibration.k_scw_ttle_default)).write(scw_generic->calibration.k_scw_ttle_default));
      (scw_cals.createDataSet<float32_T>("k_scw_ttp_max", ds::From(scw_generic->calibration.k_scw_ttp_max)).write(scw_generic->calibration.k_scw_ttp_max));
      (scw_cals.createDataSet<float32_T>("k_scw_ttp_default", ds::From(scw_generic->calibration.k_scw_ttp_default)).write(scw_generic->calibration.k_scw_ttp_default));
      (scw_cals.createDataSet<float32_T>("k_scw_trailer_zone_ext_safety_margin", ds::From(scw_generic->calibration.k_scw_trailer_zone_ext_safety_margin)).write(scw_generic->calibration.k_scw_trailer_zone_ext_safety_margin));
      (scw_cals.createDataSet<float32_T>("k_scw_trailer_zone_ext_safety_margin_lat", ds::From(scw_generic->calibration.k_scw_trailer_zone_ext_safety_margin_lat)).write(scw_generic->calibration.k_scw_trailer_zone_ext_safety_margin_lat));
      (scw_cals.createDataSet<float32_T>("k_scw_max_zone_length", ds::From(scw_generic->calibration.k_scw_max_zone_length)).write(scw_generic->calibration.k_scw_max_zone_length));
      (scw_cals.createDataSet<float32_T>("k_scw_max_zone_width", ds::From(scw_generic->calibration.k_scw_max_zone_width)).write(scw_generic->calibration.k_scw_max_zone_width));
      (scw_cals.createDataSet<boolean_T>("k_scw_f_adjust_zones_to_ego_size", ds::From(scw_generic->calibration.k_scw_f_adjust_zones_to_ego_size)).write(scw_generic->calibration.k_scw_f_adjust_zones_to_ego_size));
      (scw_cals.createDataSet<boolean_T>("k_scw_f_enable_via_cal", ds::From(scw_generic->calibration.k_scw_f_enable_via_cal)).write(scw_generic->calibration.k_scw_f_enable_via_cal));
      (scw_cals.createDataSet<boolean_T>("k_scw_f_dynamic_enable_via_cal", ds::From(scw_generic->calibration.k_scw_f_dynamic_enable_via_cal)).write(scw_generic->calibration.k_scw_f_dynamic_enable_via_cal));
      (scw_cals.createDataSet<boolean_T>("k_scw_f_guardrail_enable_via_cal", ds::From(scw_generic->calibration.k_scw_f_guardrail_enable_via_cal)).write(scw_generic->calibration.k_scw_f_guardrail_enable_via_cal));
      (scw_cals.createDataSet<boolean_T>("k_scw_f_enable", ds::From(scw_generic->calibration.k_scw_f_enable)).write(scw_generic->calibration.k_scw_f_enable));
      (scw_cals.createDataSet<boolean_T>("k_scw_f_enable_dynamic", ds::From(scw_generic->calibration.k_scw_f_enable_dynamic)).write(scw_generic->calibration.k_scw_f_enable_dynamic));
      (scw_cals.createDataSet<boolean_T>("k_scw_f_enable_guardrail", ds::From(scw_generic->calibration.k_scw_f_enable_guardrail)).write(scw_generic->calibration.k_scw_f_enable_guardrail));
      (scw_cals.createDataSet<boolean_T>("k_scw_f_enable_trailer_zone_extension", ds::From(scw_generic->calibration.k_scw_f_enable_trailer_zone_extension)).write(scw_generic->calibration.k_scw_f_enable_trailer_zone_extension));
      (scw_cals.createDataSet<boolean_T>("k_scw_f_enable_trailer_ttc_extension", ds::From(scw_generic->calibration.k_scw_f_enable_trailer_ttc_extension)).write(scw_generic->calibration.k_scw_f_enable_trailer_ttc_extension));
      (scw_cals.createDataSet<uint8_t>("k_scw_min_guardrail_age", ds::From(scw_generic->calibration.k_scw_min_guardrail_age)).write(scw_generic->calibration.k_scw_min_guardrail_age));
      (scw_cals.createDataSet<uint8_t>("k_scw_guardrail_freeze_period", ds::From(scw_generic->calibration.k_scw_guardrail_freeze_period)).write(scw_generic->calibration.k_scw_guardrail_freeze_period));
      (scw_cals.createDataSet<uint8_t>("k_scw_min_candidate_age", ds::From(scw_generic->calibration.k_scw_min_candidate_age)).write(scw_generic->calibration.k_scw_min_candidate_age));
      (scw_cals.createDataSet<uint8_t>("k_scw_candidate_mature_cycles_in_zone_threshold", ds::From(scw_generic->calibration.k_scw_candidate_mature_cycles_in_zone_threshold)).write(scw_generic->calibration.k_scw_candidate_mature_cycles_in_zone_threshold));
      (scw_cals.createDataSet<uint8_t>("k_scw_guardrail_cycles_in_zone_threshold", ds::From(scw_generic->calibration.k_scw_guardrail_cycles_in_zone_threshold)).write(scw_generic->calibration.k_scw_guardrail_cycles_in_zone_threshold));

      // Write PT Feature output
      auto pt = g5.createGroup("PT");
      (pt.createDataSet<float32_T>("range_vcs_proj_to_path_segment", ds::From(hdf_pt_ptr.range_vcs_proj_to_path_segment), props_pt)).write(hdf_pt_ptr.range_vcs_proj_to_path_segment);
      (pt.createDataSet<float32_T>("segment_heading_diff", ds::From(hdf_pt_ptr.segment_heading_diff), props_pt)).write(hdf_pt_ptr.segment_heading_diff);
      (pt.createDataSet<uint8_t>("track_idx_nearest_path", ds::From(hdf_pt_ptr.track_idx_nearest_path), props_pt)).write(hdf_pt_ptr.track_idx_nearest_path);
      (pt.createDataSet<float32_T>("range_at_zero", ds::From(hdf_pt_ptr.range_at_zero), props_pt)).write(hdf_pt_ptr.range_at_zero);
      (pt.createDataSet<float32_T>("range_to_current_path_part", ds::From(hdf_pt_ptr.range_to_current_path_part), props_pt)).write(hdf_pt_ptr.range_to_current_path_part);
      (pt.createDataSet<float32_T>("range_at_host_edge", ds::From(hdf_pt_ptr.range_at_host_edge), props_pt)).write(hdf_pt_ptr.range_at_host_edge);
      (pt.createDataSet<float32_T>("length_of_trajectory", ds::From(hdf_pt_ptr.length_of_trajectory), props_pt)).write(hdf_pt_ptr.length_of_trajectory);
      (pt.createDataSet<float32_T>("path_heading", ds::From(hdf_pt_ptr.path_heading), props_pt)).write(hdf_pt_ptr.path_heading);
      (pt.createDataSet<int>("path_direction", ds::From(hdf_pt_ptr.path_direction), props_pt)).write(hdf_pt_ptr.path_direction);
      (pt.createDataSet<uint8_t>("track_match", ds::From(hdf_pt_ptr.track_match), props_pt)).write(hdf_pt_ptr.track_match);
      (pt.createDataSet<uint8_t>("track_match_age", ds::From(hdf_pt_ptr.track_match_age), props_pt)).write(hdf_pt_ptr.track_match_age);
      (pt.createDataSet<uint8_t>("track_match_last_cycle", ds::From(hdf_pt_ptr.track_match_last_cycle), props_pt)).write(hdf_pt_ptr.track_match_last_cycle);
      (pt.createDataSet<boolean_T>("f_pt_operational", ds::From(hdf_pt_ptr.f_pt_operational), props1D)).write(hdf_pt_ptr.f_pt_operational);

      auto pt_cals = pt.createGroup("Calibration");
      // PT Calibrations
      Pt_Instance_T *pt_generic = Pt_Get_Instance_Ptr();
      (pt_cals.createDataSet<float32_T>("k_pt_zone_max_posn", ds::From(pt_generic->calibration.k_pt_zone_max_posn)).write(pt_generic->calibration.k_pt_zone_max_posn));
      (pt_cals.createDataSet<float32_T>("k_pt_min_obj_speed", ds::From(pt_generic->calibration.k_pt_min_obj_speed)).write(pt_generic->calibration.k_pt_min_obj_speed));
      (pt_cals.createDataSet<float32_T>("k_pt_overlap_max_match_value", ds::From(pt_generic->calibration.k_pt_overlap_max_match_value)).write(pt_generic->calibration.k_pt_overlap_max_match_value));
      (pt_cals.createDataSet<float32_T>("k_pt_move_max_value", ds::From(pt_generic->calibration.k_pt_move_max_value)).write(pt_generic->calibration.k_pt_move_max_value));
      (pt_cals.createDataSet<float32_T>("k_pt_group_max_match_value", ds::From(pt_generic->calibration.k_pt_group_max_match_value)).write(pt_generic->calibration.k_pt_group_max_match_value));
      (pt_cals.createDataSet<float32_T>("k_pt_group_max_match_value_ad", ds::From(pt_generic->calibration.k_pt_group_max_match_value_ad)).write(pt_generic->calibration.k_pt_group_max_match_value_ad));
      (pt_cals.createDataSet<float32_T>("k_pt_group_dir_max_diff_value", ds::From(pt_generic->calibration.k_pt_group_dir_max_diff_value)).write(pt_generic->calibration.k_pt_group_dir_max_diff_value));
      (pt_cals.createDataSet<float32_T>("k_pt_group_dir_max_avg_diff_value", ds::From(pt_generic->calibration.k_pt_group_dir_max_avg_diff_value)).write(pt_generic->calibration.k_pt_group_dir_max_avg_diff_value));
      (pt_cals.createDataSet<float32_T>("k_pt_find_max_lat_posn", ds::From(pt_generic->calibration.k_pt_find_max_lat_posn)).write(pt_generic->calibration.k_pt_find_max_lat_posn));
      (pt_cals.createDataSet<float32_T>("k_pt_find_min_speed", ds::From(pt_generic->calibration.k_pt_find_min_speed)).write(pt_generic->calibration.k_pt_find_min_speed));
      (pt_cals.createDataSet<float32_T>("k_pt_find_max_long_posn", ds::From(pt_generic->calibration.k_pt_find_max_long_posn)).write(pt_generic->calibration.k_pt_find_max_long_posn));
      (pt_cals.createDataSet<float32_T>("k_pt_path_change_match_hyst_default", ds::From(pt_generic->calibration.k_pt_path_change_match_hyst_default)).write(pt_generic->calibration.k_pt_path_change_match_hyst_default));
      (pt_cals.createDataSet<float32_T>("k_pt_path_change_differing_states_hyst_default", ds::From(pt_generic->calibration.k_pt_path_change_differing_states_hyst_default)).write(pt_generic->calibration.k_pt_path_change_differing_states_hyst_default));
      (pt_cals.createDataSet<float32_T>("k_pt_path_change_match_hyst_more_established", ds::From(pt_generic->calibration.k_pt_path_change_match_hyst_more_established)).write(pt_generic->calibration.k_pt_path_change_match_hyst_more_established));
      (pt_cals.createDataSet<float32_T>("k_pt_path_change_one_grouped_one_mature", ds::From(pt_generic->calibration.k_pt_path_change_one_grouped_one_mature)).write(pt_generic->calibration.k_pt_path_change_one_grouped_one_mature));
      (pt_cals.createDataSet<float32_T>("k_pt_path_change_one_grouped_one_creation", ds::From(pt_generic->calibration.k_pt_path_change_one_grouped_one_creation)).write(pt_generic->calibration.k_pt_path_change_one_grouped_one_creation));
      (pt_cals.createDataSet<float32_T>("k_pt_kill_path_exceed_dist_thres", ds::From(pt_generic->calibration.k_pt_kill_path_exceed_dist_thres)).write(pt_generic->calibration.k_pt_kill_path_exceed_dist_thres));
      (pt_cals.createDataSet<float32_T>("k_pt_kill_path_max_diff_posn", ds::From(pt_generic->calibration.k_pt_kill_path_max_diff_posn)).write(pt_generic->calibration.k_pt_kill_path_max_diff_posn));
      (pt_cals.createDataSet<float32_T>("k_pt_lower_lim_obj_orient_lat", ds::From(pt_generic->calibration.k_pt_lower_lim_obj_orient_lat)).write(pt_generic->calibration.k_pt_lower_lim_obj_orient_lat));
      (pt_cals.createDataSet<float32_T>("k_pt_upper_lim_obj_orient_lat", ds::From(pt_generic->calibration.k_pt_upper_lim_obj_orient_lat)).write(pt_generic->calibration.k_pt_upper_lim_obj_orient_lat));
      (pt_cals.createDataSet<float32_T>("k_pt_apply_move_point_min_speed", ds::From(pt_generic->calibration.k_pt_apply_move_point_min_speed)).write(pt_generic->calibration.k_pt_apply_move_point_min_speed));
      (pt_cals.createDataSet<float32_T>("k_pt_apply_move_point_min_yaw_rate", ds::From(pt_generic->calibration.k_pt_apply_move_point_min_yaw_rate)).write(pt_generic->calibration.k_pt_apply_move_point_min_yaw_rate));
      (pt_cals.createDataSet<float32_T>("k_pt_move_point_yaw_rate_thres_calc_ego_shift", ds::From(pt_generic->calibration.k_pt_move_point_yaw_rate_thres_calc_ego_shift)).write(pt_generic->calibration.k_pt_move_point_yaw_rate_thres_calc_ego_shift));
      (pt_cals.createDataSet<float32_T>("k_pt_group_paths_min_interval_dist", ds::From(pt_generic->calibration.k_pt_group_paths_min_interval_dist)).write(pt_generic->calibration.k_pt_group_paths_min_interval_dist));
      (pt_cals.createDataSet<float32_T>("k_pt_path_track_long_range_limit", ds::From(pt_generic->calibration.k_pt_path_track_long_range_limit)).write(pt_generic->calibration.k_pt_path_track_long_range_limit));
      (pt_cals.createDataSet<float32_T>("k_pt_path_track_lat_range_limit", ds::From(pt_generic->calibration.k_pt_path_track_lat_range_limit)).write(pt_generic->calibration.k_pt_path_track_lat_range_limit));
      (pt_cals.createDataSet<float32_T>("k_pt_group_overlap_paths_min_diff", ds::From(pt_generic->calibration.k_pt_group_overlap_paths_min_diff)).write(pt_generic->calibration.k_pt_group_overlap_paths_min_diff));
      (pt_cals.createDataSet<float32_T>("k_pt_en_algo_min_vel_inactive", ds::From(pt_generic->calibration.k_pt_en_algo_min_vel_inactive)).write(pt_generic->calibration.k_pt_en_algo_min_vel_inactive));
      (pt_cals.createDataSet<float32_T>("k_pt_en_algo_max_val_active", ds::From(pt_generic->calibration.k_pt_en_algo_max_val_active)).write(pt_generic->calibration.k_pt_en_algo_max_val_active));
      (pt_cals.createDataSet<float32_T>("k_pt_point_diff_weighting_factor_lut", ds::From(pt_generic->calibration.k_pt_point_diff_weighting_factor_lut)).write(pt_generic->calibration.k_pt_point_diff_weighting_factor_lut));
      (pt_cals.createDataSet<float32_T>("k_pt_default_range_of_tracking_zone", ds::From(pt_generic->calibration.k_pt_default_range_of_tracking_zone)).write(pt_generic->calibration.k_pt_default_range_of_tracking_zone));
      (pt_cals.createDataSet<float32_T>("k_pt_min_exist_prob_to_be_valid", ds::From(pt_generic->calibration.k_pt_min_exist_prob_to_be_valid)).write(pt_generic->calibration.k_pt_min_exist_prob_to_be_valid));
      (pt_cals.createDataSet<float32_T>("k_pt_dist_obj_to_border_conf_lut", ds::From(pt_generic->calibration.k_pt_dist_obj_to_border_conf_lut)).write(pt_generic->calibration.k_pt_dist_obj_to_border_conf_lut));
      (pt_cals.createDataSet<float32_T>("k_pt_dist_border_to_isect_conf_lut", ds::From(pt_generic->calibration.k_pt_dist_border_to_isect_conf_lut)).write(pt_generic->calibration.k_pt_dist_border_to_isect_conf_lut));
      (pt_cals.createDataSet<float32_T>("k_pt_dist_obj_to_path_lut", ds::From(pt_generic->calibration.k_pt_dist_obj_to_path_lut)).write(pt_generic->calibration.k_pt_dist_obj_to_path_lut));
      (pt_cals.createDataSet<float32_T>("k_pt_dist_obj_to_path_conf_lut", ds::From(pt_generic->calibration.k_pt_dist_obj_to_path_conf_lut)).write(pt_generic->calibration.k_pt_dist_obj_to_path_conf_lut));
      (pt_cals.createDataSet<float32_T>("k_pt_similarity_trail_path_lut", ds::From(pt_generic->calibration.k_pt_similarity_trail_path_lut)).write(pt_generic->calibration.k_pt_similarity_trail_path_lut));
      (pt_cals.createDataSet<float32_T>("k_pt_similarity_trail_path_conf_lut", ds::From(pt_generic->calibration.k_pt_similarity_trail_path_conf_lut)).write(pt_generic->calibration.k_pt_similarity_trail_path_conf_lut));
      (pt_cals.createDataSet<float32_T>("k_pt_weight_of_last_trail_point", ds::From(pt_generic->calibration.k_pt_weight_of_last_trail_point)).write(pt_generic->calibration.k_pt_weight_of_last_trail_point));
      (pt_cals.createDataSet<float32_T>("k_pt_weight_of_sec_last_trail_point", ds::From(pt_generic->calibration.k_pt_weight_of_sec_last_trail_point)).write(pt_generic->calibration.k_pt_weight_of_sec_last_trail_point));
      (pt_cals.createDataSet<float32_T>("k_pt_heading_diff_lut", ds::From(pt_generic->calibration.k_pt_heading_diff_lut)).write(pt_generic->calibration.k_pt_heading_diff_lut));
      (pt_cals.createDataSet<float32_T>("k_pt_heading_diff_conf_lut", ds::From(pt_generic->calibration.k_pt_heading_diff_conf_lut)).write(pt_generic->calibration.k_pt_heading_diff_conf_lut));
      (pt_cals.createDataSet<float32_T>("k_pt_weight_heading_diff_confidence", ds::From(pt_generic->calibration.k_pt_weight_heading_diff_confidence)).write(pt_generic->calibration.k_pt_weight_heading_diff_confidence));
      (pt_cals.createDataSet<float32_T>("k_pt_min_confidence_valid_match", ds::From(pt_generic->calibration.k_pt_min_confidence_valid_match)).write(pt_generic->calibration.k_pt_min_confidence_valid_match));
      (pt_cals.createDataSet<float32_T>("k_pt_dist_betw_paths_similarity_matching", ds::From(pt_generic->calibration.k_pt_dist_betw_paths_similarity_matching)).write(pt_generic->calibration.k_pt_dist_betw_paths_similarity_matching));
      (pt_cals.createDataSet<float32_T>("k_pt_host_implausibilty_range", ds::From(pt_generic->calibration.k_pt_host_implausibilty_range)).write(pt_generic->calibration.k_pt_host_implausibilty_range));
      (pt_cals.createDataSet<float32_T>("k_pt_trail_max_speed_trail_to_path_conv", ds::From(pt_generic->calibration.k_pt_trail_max_speed_trail_to_path_conv)).write(pt_generic->calibration.k_pt_trail_max_speed_trail_to_path_conv));
      (pt_cals.createDataSet<float32_T>("k_pt_minimum_host_trail_length", ds::From(pt_generic->calibration.k_pt_minimum_host_trail_length)).write(pt_generic->calibration.k_pt_minimum_host_trail_length));
      (pt_cals.createDataSet<float32_T>("k_pt_max_heading_diff_valid_interval", ds::From(pt_generic->calibration.k_pt_max_heading_diff_valid_interval)).write(pt_generic->calibration.k_pt_max_heading_diff_valid_interval));
      (pt_cals.createDataSet<boolean_T>("k_pt_f_apply_move_point", ds::From(pt_generic->calibration.k_pt_f_apply_move_point)).write(pt_generic->calibration.k_pt_f_apply_move_point));
      (pt_cals.createDataSet<boolean_T>("k_pt_f_check_object_age_plausibility", ds::From(pt_generic->calibration.k_pt_f_check_object_age_plausibility)).write(pt_generic->calibration.k_pt_f_check_object_age_plausibility));
      (pt_cals.createDataSet<boolean_T>("k_pt_enable_host_trail", ds::From(pt_generic->calibration.k_pt_enable_host_trail)).write(pt_generic->calibration.k_pt_enable_host_trail));
      (pt_cals.createDataSet<uint8_t>("k_pt_max_diff_num_path_point", ds::From(pt_generic->calibration.k_pt_max_diff_num_path_point)).write(pt_generic->calibration.k_pt_max_diff_num_path_point));
      (pt_cals.createDataSet<uint8_t>("k_pt_group_path_min_overlap_count", ds::From(pt_generic->calibration.k_pt_group_path_min_overlap_count)).write(pt_generic->calibration.k_pt_group_path_min_overlap_count));
      (pt_cals.createDataSet<uint8_t>("k_pt_group_path_min_overlap_count_ad", ds::From(pt_generic->calibration.k_pt_group_path_min_overlap_count_ad)).write(pt_generic->calibration.k_pt_group_path_min_overlap_count_ad));
      (pt_cals.createDataSet<uint8_t>("k_pt_find_min_diff_path_points", ds::From(pt_generic->calibration.k_pt_find_min_diff_path_points)).write(pt_generic->calibration.k_pt_find_min_diff_path_points));
      (pt_cals.createDataSet<uint8_t>("k_pt_find_max_diff_path_points", ds::From(pt_generic->calibration.k_pt_find_max_diff_path_points)).write(pt_generic->calibration.k_pt_find_max_diff_path_points));
      (pt_cals.createDataSet<uint8_t>("k_pt_kill_lane_change_min_diff_path_point", ds::From(pt_generic->calibration.k_pt_kill_lane_change_min_diff_path_point)).write(pt_generic->calibration.k_pt_kill_lane_change_min_diff_path_point));
      (pt_cals.createDataSet<uint8_t>("k_pt_min_diff_num_path_points", ds::From(pt_generic->calibration.k_pt_min_diff_num_path_points)).write(pt_generic->calibration.k_pt_min_diff_num_path_points));
      (pt_cals.createDataSet<uint8_t>("k_pt_min_path_length_proc_lane_change", ds::From(pt_generic->calibration.k_pt_min_path_length_proc_lane_change)).write(pt_generic->calibration.k_pt_min_path_length_proc_lane_change));
      (pt_cals.createDataSet<uint8_t>("k_pt_point_diff_grouping_borders_lut", ds::From(pt_generic->calibration.k_pt_point_diff_grouping_borders_lut)).write(pt_generic->calibration.k_pt_point_diff_grouping_borders_lut));
      (pt_cals.createDataSet<uint8_t>("k_pt_cond_kill_implaus_path", ds::From(pt_generic->calibration.k_pt_cond_kill_implaus_path)).write(pt_generic->calibration.k_pt_cond_kill_implaus_path));
      (pt_cals.createDataSet<uint8_t>("k_pt_min_path_length_after_rot", ds::From(pt_generic->calibration.k_pt_min_path_length_after_rot)).write(pt_generic->calibration.k_pt_min_path_length_after_rot));
      (pt_cals.createDataSet<uint8_t>("k_pt_dist_obj_to_border_lut", ds::From(pt_generic->calibration.k_pt_dist_obj_to_border_lut)).write(pt_generic->calibration.k_pt_dist_obj_to_border_lut));
      (pt_cals.createDataSet<uint8_t>("k_pt_dist_border_to_isect_lut", ds::From(pt_generic->calibration.k_pt_dist_border_to_isect_lut)).write(pt_generic->calibration.k_pt_dist_border_to_isect_lut));
      (pt_cals.createDataSet<uint8_t>("k_pt_min_path_length_obj_trail", ds::From(pt_generic->calibration.k_pt_min_path_length_obj_trail)).write(pt_generic->calibration.k_pt_min_path_length_obj_trail));
      (pt_cals.createDataSet<uint8_t>("k_pt_range_nearest_border_impl_path", ds::From(pt_generic->calibration.k_pt_range_nearest_border_impl_path)).write(pt_generic->calibration.k_pt_range_nearest_border_impl_path));
      (pt_cals.createDataSet<uint8_t>("k_pt_start_of_lane_change_processing", ds::From(pt_generic->calibration.k_pt_start_of_lane_change_processing)).write(pt_generic->calibration.k_pt_start_of_lane_change_processing));
      (pt_cals.createDataSet<uint8_t>("k_pt_end_of_lane_change_processing", ds::From(pt_generic->calibration.k_pt_end_of_lane_change_processing)).write(pt_generic->calibration.k_pt_end_of_lane_change_processing));
      (pt_cals.createDataSet<uint8_t>("k_pt_minimum_amount_of_trail_points", ds::From(pt_generic->calibration.k_pt_minimum_amount_of_trail_points)).write(pt_generic->calibration.k_pt_minimum_amount_of_trail_points));

      // Write Cta Feature output
      auto cta = g5.createGroup("CTA");
      cta.createDataSet<boolean_T>("f_cta_enabled", ds::From(hdf_cta_ptr.f_cta_enabled), props1D).write(hdf_cta_ptr.f_cta_enabled);
      cta.createDataSet<uint8_t>("most_critical_object_by_sides_id_left", ds::From(hdf_cta_ptr.most_critical_object_by_sides_id_left), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_id_left);
      cta.createDataSet<uint32_t>("most_critical_object_by_sides_unique_id_left", ds::From(hdf_cta_ptr.most_critical_object_by_sides_unique_id_left), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_unique_id_left);
      cta.createDataSet<float32_T>("most_critical_object_by_sides_objPoseX_m_left", ds::From(hdf_cta_ptr.most_critical_object_by_sides_objPoseX_m_left), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_objPoseX_m_left);
      cta.createDataSet<float32_T>("most_critical_object_by_sides_objPoseY_m_left", ds::From(hdf_cta_ptr.most_critical_object_by_sides_objPoseY_m_left), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_objPoseY_m_left);
      cta.createDataSet<float32_T>("most_critical_object_by_sides_objVelocityX_mps_left", ds::From(hdf_cta_ptr.most_critical_object_by_sides_objVelocityX_mps_left), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_objVelocityX_mps_left);
      cta.createDataSet<float32_T>("most_critical_object_by_sides_objVelocityY_mps_left", ds::From(hdf_cta_ptr.most_critical_object_by_sides_objVelocityY_mps_left), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_objVelocityY_mps_left);
      cta.createDataSet<float32_T>("most_critical_object_by_sides_heading_rad_left", ds::From(hdf_cta_ptr.most_critical_object_by_sides_heading_rad_left), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_heading_rad_left);
      cta.createDataSet<int>("most_critical_object_by_sides_alert_level_left", ds::From(hdf_cta_ptr.most_critical_object_by_sides_alert_level_left), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_alert_level_left);
      cta.createDataSet<float32_T>("most_critical_object_by_sides_ttc_s_left", ds::From(hdf_cta_ptr.most_critical_object_by_sides_ttc_s_left), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_ttc_s_left);
      cta.createDataSet<float32_T>("most_critical_object_by_sides_intersection_point_x_m_left", ds::From(hdf_cta_ptr.most_critical_object_by_sides_intersection_point_x_m_left), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_intersection_point_x_m_left);
      cta.createDataSet<boolean_T>("most_critical_object_by_sides_f_brake_qualifier_left", ds::From(hdf_cta_ptr.most_critical_object_by_sides_f_brake_qualifier_left), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_f_brake_qualifier_left);
      cta.createDataSet<uint8_t>("most_critical_object_by_sides_id_right", ds::From(hdf_cta_ptr.most_critical_object_by_sides_id_right), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_id_right);
      cta.createDataSet<uint32_t>("most_critical_object_by_sides_unique_id_right", ds::From(hdf_cta_ptr.most_critical_object_by_sides_unique_id_right), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_unique_id_right);
      cta.createDataSet<float32_T>("most_critical_object_by_sides_objPoseX_m_right", ds::From(hdf_cta_ptr.most_critical_object_by_sides_objPoseX_m_right), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_objPoseX_m_right);
      cta.createDataSet<float32_T>("most_critical_object_by_sides_objPoseY_m_right", ds::From(hdf_cta_ptr.most_critical_object_by_sides_objPoseY_m_right), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_objPoseY_m_right);
      cta.createDataSet<float32_T>("most_critical_object_by_sides_objVelocityX_mps_right", ds::From(hdf_cta_ptr.most_critical_object_by_sides_objVelocityX_mps_right), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_objVelocityX_mps_right);
      cta.createDataSet<float32_T>("most_critical_object_by_sides_objVelocityY_mps_right", ds::From(hdf_cta_ptr.most_critical_object_by_sides_objVelocityY_mps_right), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_objVelocityY_mps_right);
      cta.createDataSet<float32_T>("most_critical_object_by_sides_heading_rad_right", ds::From(hdf_cta_ptr.most_critical_object_by_sides_heading_rad_right), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_heading_rad_right);
      cta.createDataSet<int>("most_critical_object_by_sides_alert_level_right", ds::From(hdf_cta_ptr.most_critical_object_by_sides_alert_level_right), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_alert_level_right);
      cta.createDataSet<float32_T>("most_critical_object_by_sides_ttc_s_right", ds::From(hdf_cta_ptr.most_critical_object_by_sides_ttc_s_right), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_ttc_s_right);
      cta.createDataSet<float32_T>("most_critical_object_by_sides_intersection_point_x_m_right", ds::From(hdf_cta_ptr.most_critical_object_by_sides_intersection_point_x_m_right), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_intersection_point_x_m_right);
      cta.createDataSet<boolean_T>("most_critical_object_by_sides_f_brake_qualifier_right", ds::From(hdf_cta_ptr.most_critical_object_by_sides_f_brake_qualifier_right), props1D).write(hdf_cta_ptr.most_critical_object_by_sides_f_brake_qualifier_right);

      auto cta_cals = cta.createGroup("Calibration");
      // CTA Calibrations
      Cta_Instance_T *cta_generic = Cta_Get_Instance_Ptr();
      (cta_cals.createDataSet<float32_T>("k_cta_min_park_angle", ds::From(cta_generic->calibration.k_cta_min_park_angle)).write(cta_generic->calibration.k_cta_min_park_angle));
      (cta_cals.createDataSet<float32_T>("k_cta_fcta_steer_angle_table", ds::From(cta_generic->calibration.k_cta_fcta_steer_angle_table)).write(cta_generic->calibration.k_cta_fcta_steer_angle_table));
      (cta_cals.createDataSet<float32_T>("k_cta_fcta_steer_factor_table", ds::From(cta_generic->calibration.k_cta_fcta_steer_factor_table)).write(cta_generic->calibration.k_cta_fcta_steer_factor_table));
      (cta_cals.createDataSet<float32_T>("k_cta_rcta_steer_angle_table", ds::From(cta_generic->calibration.k_cta_rcta_steer_angle_table)).write(cta_generic->calibration.k_cta_rcta_steer_angle_table));
      (cta_cals.createDataSet<float32_T>("k_cta_rcta_steer_factor_table", ds::From(cta_generic->calibration.k_cta_rcta_steer_factor_table)).write(cta_generic->calibration.k_cta_rcta_steer_factor_table));
      (cta_cals.createDataSet<float32_T>("k_cta_max_obstruction_probability", ds::From(cta_generic->calibration.k_cta_max_obstruction_probability)).write(cta_generic->calibration.k_cta_max_obstruction_probability));
      (cta_cals.createDataSet<float32_T>("k_cta_max_object_eclipse_for_level_qualification", ds::From(cta_generic->calibration.k_cta_max_object_eclipse_for_level_qualification)).write(cta_generic->calibration.k_cta_max_object_eclipse_for_level_qualification));
      (cta_cals.createDataSet<float32_T>("k_cta_accelerationpedal_gradient_threshold_ctb", ds::From(cta_generic->calibration.k_cta_accelerationpedal_gradient_threshold_ctb)).write(cta_generic->calibration.k_cta_accelerationpedal_gradient_threshold_ctb));
      (cta_cals.createDataSet<float32_T>("k_cta_host_width_sensor_fov_suppr_factor", ds::From(cta_generic->calibration.k_cta_host_width_sensor_fov_suppr_factor)).write(cta_generic->calibration.k_cta_host_width_sensor_fov_suppr_factor));
      (cta_cals.createDataSet<float32_T>("k_cta_sensor_fov_border", ds::From(cta_generic->calibration.k_cta_sensor_fov_border)).write(cta_generic->calibration.k_cta_sensor_fov_border));
      (cta_cals.createDataSet<float32_T>("k_cta_ttc_warntrigger_early", ds::From(cta_generic->calibration.k_cta_ttc_warntrigger_early)).write(cta_generic->calibration.k_cta_ttc_warntrigger_early));
      (cta_cals.createDataSet<float32_T>("k_cta_ttc_warntrigger_late", ds::From(cta_generic->calibration.k_cta_ttc_warntrigger_late)).write(cta_generic->calibration.k_cta_ttc_warntrigger_late));
      (cta_cals.createDataSet<float32_T>("k_cta_ego_abs_speed_max", ds::From(cta_generic->calibration.k_cta_ego_abs_speed_max)).write(cta_generic->calibration.k_cta_ego_abs_speed_max));
      (cta_cals.createDataSet<float32_T>("k_cta_stop_alert_ttc", ds::From(cta_generic->calibration.k_cta_stop_alert_ttc)).write(cta_generic->calibration.k_cta_stop_alert_ttc));
      (cta_cals.createDataSet<float32_T>("k_cta_stop_alert_ttp", ds::From(cta_generic->calibration.k_cta_stop_alert_ttp)).write(cta_generic->calibration.k_cta_stop_alert_ttp));
      (cta_cals.createDataSet<float32_T>("k_cta_min_speed", ds::From(cta_generic->calibration.k_cta_min_speed)).write(cta_generic->calibration.k_cta_min_speed));
      (cta_cals.createDataSet<float32_T>("k_cta_butterfly_long", ds::From(cta_generic->calibration.k_cta_butterfly_long)).write(cta_generic->calibration.k_cta_butterfly_long));
      (cta_cals.createDataSet<float32_T>("k_cta_butterfly_lat", ds::From(cta_generic->calibration.k_cta_butterfly_lat)).write(cta_generic->calibration.k_cta_butterfly_lat));
      (cta_cals.createDataSet<float32_T>("k_cta_ttc_criticality_level", ds::From(cta_generic->calibration.k_cta_ttc_criticality_level)).write(cta_generic->calibration.k_cta_ttc_criticality_level));
      (cta_cals.createDataSet<float32_T>("k_cta_speed_criticality_level", ds::From(cta_generic->calibration.k_cta_speed_criticality_level)).write(cta_generic->calibration.k_cta_speed_criticality_level));
      (cta_cals.createDataSet<float32_T>("k_cta_max_long_point_criticality_level", ds::From(cta_generic->calibration.k_cta_max_long_point_criticality_level)).write(cta_generic->calibration.k_cta_max_long_point_criticality_level));
      (cta_cals.createDataSet<float32_T>("k_cta_min_long_point_criticality_level", ds::From(cta_generic->calibration.k_cta_min_long_point_criticality_level)).write(cta_generic->calibration.k_cta_min_long_point_criticality_level));
      (cta_cals.createDataSet<float32_T>("k_cta_min_lateral_approach_speed", ds::From(cta_generic->calibration.k_cta_min_lateral_approach_speed)).write(cta_generic->calibration.k_cta_min_lateral_approach_speed));
      (cta_cals.createDataSet<float32_T>("k_cta_min_rel_existence_probability", ds::From(cta_generic->calibration.k_cta_min_rel_existence_probability)).write(cta_generic->calibration.k_cta_min_rel_existence_probability));
      (cta_cals.createDataSet<float32_T>("k_cta_rel_warning_hysteresis", ds::From(cta_generic->calibration.k_cta_rel_warning_hysteresis)).write(cta_generic->calibration.k_cta_rel_warning_hysteresis));
      (cta_cals.createDataSet<float32_T>("k_cta_rcta_host_speed_factor", ds::From(cta_generic->calibration.k_cta_rcta_host_speed_factor)).write(cta_generic->calibration.k_cta_rcta_host_speed_factor));
      (cta_cals.createDataSet<float32_T>("k_cta_max_speed", ds::From(cta_generic->calibration.k_cta_max_speed)).write(cta_generic->calibration.k_cta_max_speed));
      (cta_cals.createDataSet<float32_T>("k_cta_max_length_fov", ds::From(cta_generic->calibration.k_cta_max_length_fov)).write(cta_generic->calibration.k_cta_max_length_fov));
      (cta_cals.createDataSet<float32_T>("k_cta_heading_range", ds::From(cta_generic->calibration.k_cta_heading_range)).write(cta_generic->calibration.k_cta_heading_range));
      (cta_cals.createDataSet<float32_T>("k_cta_angles_zone_definition", ds::From(cta_generic->calibration.k_cta_angles_zone_definition)).write(cta_generic->calibration.k_cta_angles_zone_definition));
      (cta_cals.createDataSet<float32_T>("k_cta_min_host_speed_to_discard_pt_info", ds::From(cta_generic->calibration.k_cta_min_host_speed_to_discard_pt_info)).write(cta_generic->calibration.k_cta_min_host_speed_to_discard_pt_info));
      (cta_cals.createDataSet<float32_T>("k_cta_obj_dist_to_discard_pt_info", ds::From(cta_generic->calibration.k_cta_obj_dist_to_discard_pt_info)).write(cta_generic->calibration.k_cta_obj_dist_to_discard_pt_info));
      (cta_cals.createDataSet<float32_T>("k_cta_ghost_condition_max_heading_diff_path_tracker", ds::From(cta_generic->calibration.k_cta_ghost_condition_max_heading_diff_path_tracker)).write(cta_generic->calibration.k_cta_ghost_condition_max_heading_diff_path_tracker));
      (cta_cals.createDataSet<float32_T>("k_cta_min_ttc_additional_mature_qualification", ds::From(cta_generic->calibration.k_cta_min_ttc_additional_mature_qualification)).write(cta_generic->calibration.k_cta_min_ttc_additional_mature_qualification));
      (cta_cals.createDataSet<float32_T>("k_cta_intersection_line_host_width_percentage", ds::From(cta_generic->calibration.k_cta_intersection_line_host_width_percentage)).write(cta_generic->calibration.k_cta_intersection_line_host_width_percentage));
      (cta_cals.createDataSet<float32_T>("k_ctb_lower_safety_distance_thres", ds::From(cta_generic->calibration.k_ctb_lower_safety_distance_thres)).write(cta_generic->calibration.k_ctb_lower_safety_distance_thres));
      (cta_cals.createDataSet<float32_T>("k_ctb_upper_safety_distance_thres_lut", ds::From(cta_generic->calibration.k_ctb_upper_safety_distance_thres_lut)).write(cta_generic->calibration.k_ctb_upper_safety_distance_thres_lut));
      (cta_cals.createDataSet<float32_T>("k_ctb_safety_dist_host_vel_lut", ds::From(cta_generic->calibration.k_ctb_safety_dist_host_vel_lut)).write(cta_generic->calibration.k_ctb_safety_dist_host_vel_lut));
      (cta_cals.createDataSet<float32_T>("k_ctb_event_time_buffer", ds::From(cta_generic->calibration.k_ctb_event_time_buffer)).write(cta_generic->calibration.k_ctb_event_time_buffer));
      (cta_cals.createDataSet<float32_T>("k_ctb_min_braking_time", ds::From(cta_generic->calibration.k_ctb_min_braking_time)).write(cta_generic->calibration.k_ctb_min_braking_time));
      (cta_cals.createDataSet<float32_T>("k_ctb_max_braking_time", ds::From(cta_generic->calibration.k_ctb_max_braking_time)).write(cta_generic->calibration.k_ctb_max_braking_time));
      (cta_cals.createDataSet<float32_T>("k_ctb_responsetime_brake_actuation", ds::From(cta_generic->calibration.k_ctb_responsetime_brake_actuation)).write(cta_generic->calibration.k_ctb_responsetime_brake_actuation));
      (cta_cals.createDataSet<float32_T>("k_ctb_ramp_in_time", ds::From(cta_generic->calibration.k_ctb_ramp_in_time)).write(cta_generic->calibration.k_ctb_ramp_in_time));
      (cta_cals.createDataSet<float32_T>("k_ctb_const_decel_after_ramp_in", ds::From(cta_generic->calibration.k_ctb_const_decel_after_ramp_in)).write(cta_generic->calibration.k_ctb_const_decel_after_ramp_in));
      (cta_cals.createDataSet<float32_T>("k_ctb_braking_jerk", ds::From(cta_generic->calibration.k_ctb_braking_jerk)).write(cta_generic->calibration.k_ctb_braking_jerk));
      (cta_cals.createDataSet<float32_T>("k_ctb_host_acc_weight", ds::From(cta_generic->calibration.k_ctb_host_acc_weight)).write(cta_generic->calibration.k_ctb_host_acc_weight));
      (cta_cals.createDataSet<float32_T>("k_ctb_time_to_ask_for_final_brake_decel", ds::From(cta_generic->calibration.k_ctb_time_to_ask_for_final_brake_decel)).write(cta_generic->calibration.k_ctb_time_to_ask_for_final_brake_decel));
      (cta_cals.createDataSet<float32_T>("k_cta_dist_thres_crit_level_reset", ds::From(cta_generic->calibration.k_cta_dist_thres_crit_level_reset)).write(cta_generic->calibration.k_cta_dist_thres_crit_level_reset));
      (cta_cals.createDataSet<float32_T>("k_cta_max_heading_variance", ds::From(cta_generic->calibration.k_cta_max_heading_variance)).write(cta_generic->calibration.k_cta_max_heading_variance));
      (cta_cals.createDataSet<float32_T>("k_cta_max_seg_heading_diff_no_ghost", ds::From(cta_generic->calibration.k_cta_max_seg_heading_diff_no_ghost)).write(cta_generic->calibration.k_cta_max_seg_heading_diff_no_ghost));
      (cta_cals.createDataSet<float32_T>("k_cta_range_to_path_segment_ghost_qualif", ds::From(cta_generic->calibration.k_cta_range_to_path_segment_ghost_qualif)).write(cta_generic->calibration.k_cta_range_to_path_segment_ghost_qualif));
      (cta_cals.createDataSet<float32_T>("k_cta_object_heading_exp_moving_average_alpha", ds::From(cta_generic->calibration.k_cta_object_heading_exp_moving_average_alpha)).write(cta_generic->calibration.k_cta_object_heading_exp_moving_average_alpha));
      (cta_cals.createDataSet<float32_T>("k_cta_pedestrian_min_size", ds::From(cta_generic->calibration.k_cta_pedestrian_min_size)).write(cta_generic->calibration.k_cta_pedestrian_min_size));
      (cta_cals.createDataSet<float32_T>("k_cta_pedestrian_min_speed", ds::From(cta_generic->calibration.k_cta_pedestrian_min_speed)).write(cta_generic->calibration.k_cta_pedestrian_min_speed));
      (cta_cals.createDataSet<float32_T>("k_cta_2wheel_min_size", ds::From(cta_generic->calibration.k_cta_2wheel_min_size)).write(cta_generic->calibration.k_cta_2wheel_min_size));
      (cta_cals.createDataSet<float32_T>("k_cta_2wheel_min_speed", ds::From(cta_generic->calibration.k_cta_2wheel_min_speed)).write(cta_generic->calibration.k_cta_2wheel_min_speed));
      (cta_cals.createDataSet<float32_T>("k_cta_min_deceleration_value", ds::From(cta_generic->calibration.k_cta_min_deceleration_value)).write(cta_generic->calibration.k_cta_min_deceleration_value));
      (cta_cals.createDataSet<float32_T>("k_cta_max_deceleration_value", ds::From(cta_generic->calibration.k_cta_max_deceleration_value)).write(cta_generic->calibration.k_cta_max_deceleration_value));
      (cta_cals.createDataSet<float32_T>("k_cta_ttc_calc_positive_ref_point", ds::From(cta_generic->calibration.k_cta_ttc_calc_positive_ref_point)).write(cta_generic->calibration.k_cta_ttc_calc_positive_ref_point));
      (cta_cals.createDataSet<float32_T>("k_cta_speed_thresh_for_rel_vel_calc", ds::From(cta_generic->calibration.k_cta_speed_thresh_for_rel_vel_calc)).write(cta_generic->calibration.k_cta_speed_thresh_for_rel_vel_calc));
      (cta_cals.createDataSet<uint16_t>("k_cta_DEBUG_MODE", ds::From(cta_generic->calibration.k_cta_DEBUG_MODE)).write(cta_generic->calibration.k_cta_DEBUG_MODE));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_check_reflection_signal", ds::From(cta_generic->calibration.k_cta_f_check_reflection_signal)).write(cta_generic->calibration.k_cta_f_check_reflection_signal));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_check_obstruction_probability_signal", ds::From(cta_generic->calibration.k_cta_f_check_obstruction_probability_signal)).write(cta_generic->calibration.k_cta_f_check_obstruction_probability_signal));
      (cta_cals.createDataSet<boolean_T>("k_cta_switch", ds::From(cta_generic->calibration.k_cta_switch)).write(cta_generic->calibration.k_cta_switch));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_adapt_intersect_lines_by_steering_angle", ds::From(cta_generic->calibration.k_cta_f_adapt_intersect_lines_by_steering_angle)).write(cta_generic->calibration.k_cta_f_adapt_intersect_lines_by_steering_angle));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_adapt_intersect_lines_by_obj_heading", ds::From(cta_generic->calibration.k_cta_f_adapt_intersect_lines_by_obj_heading)).write(cta_generic->calibration.k_cta_f_adapt_intersect_lines_by_obj_heading));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_adapt_intersect_lines_by_host_speed", ds::From(cta_generic->calibration.k_cta_f_adapt_intersect_lines_by_host_speed)).write(cta_generic->calibration.k_cta_f_adapt_intersect_lines_by_host_speed));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_prevent_fall_back_to_critlevel_1", ds::From(cta_generic->calibration.k_cta_f_prevent_fall_back_to_critlevel_1)).write(cta_generic->calibration.k_cta_f_prevent_fall_back_to_critlevel_1));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_apply_heading_compensation_on_intersection_point", ds::From(cta_generic->calibration.k_cta_f_apply_heading_compensation_on_intersection_point)).write(cta_generic->calibration.k_cta_f_apply_heading_compensation_on_intersection_point));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_calc_ttc_ego_side_enabled", ds::From(cta_generic->calibration.k_cta_f_calc_ttc_ego_side_enabled)).write(cta_generic->calibration.k_cta_f_calc_ttc_ego_side_enabled));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_calc_ttp_ego_side_enabled", ds::From(cta_generic->calibration.k_cta_f_calc_ttp_ego_side_enabled)).write(cta_generic->calibration.k_cta_f_calc_ttp_ego_side_enabled));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_use_heading_for_relative_velocity_calculation", ds::From(cta_generic->calibration.k_cta_f_use_heading_for_relative_velocity_calculation)).write(cta_generic->calibration.k_cta_f_use_heading_for_relative_velocity_calculation));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_apply_path_tracking", ds::From(cta_generic->calibration.k_cta_f_apply_path_tracking)).write(cta_generic->calibration.k_cta_f_apply_path_tracking));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_discard_pt_heading_when_moving", ds::From(cta_generic->calibration.k_cta_f_discard_pt_heading_when_moving)).write(cta_generic->calibration.k_cta_f_discard_pt_heading_when_moving));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_use_object_supress_counter", ds::From(cta_generic->calibration.k_cta_f_use_object_supress_counter)).write(cta_generic->calibration.k_cta_f_use_object_supress_counter));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_use_ghost_detector", ds::From(cta_generic->calibration.k_cta_f_use_ghost_detector)).write(cta_generic->calibration.k_cta_f_use_ghost_detector));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_use_object_min_object_age_in_cycles", ds::From(cta_generic->calibration.k_cta_f_use_object_min_object_age_in_cycles)).write(cta_generic->calibration.k_cta_f_use_object_min_object_age_in_cycles));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_use_rel_vel_isect_point_calc", ds::From(cta_generic->calibration.k_cta_f_use_rel_vel_isect_point_calc)).write(cta_generic->calibration.k_cta_f_use_rel_vel_isect_point_calc));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_enable_thres_crit_level_reset", ds::From(cta_generic->calibration.k_cta_f_enable_thres_crit_level_reset)).write(cta_generic->calibration.k_cta_f_enable_thres_crit_level_reset));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_use_front_corners_dist_stop", ds::From(cta_generic->calibration.k_cta_f_use_front_corners_dist_stop)).write(cta_generic->calibration.k_cta_f_use_front_corners_dist_stop));
      (cta_cals.createDataSet<boolean_T>("k_cta_enable_ctb", ds::From(cta_generic->calibration.k_cta_enable_ctb)).write(cta_generic->calibration.k_cta_enable_ctb));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_brake_overriding_ctb", ds::From(cta_generic->calibration.k_cta_f_brake_overriding_ctb)).write(cta_generic->calibration.k_cta_f_brake_overriding_ctb));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_use_brake_gradient", ds::From(cta_generic->calibration.k_cta_f_use_brake_gradient)).write(cta_generic->calibration.k_cta_f_use_brake_gradient));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_enable_heading_exp_moving_average", ds::From(cta_generic->calibration.k_cta_f_enable_heading_exp_moving_average)).write(cta_generic->calibration.k_cta_f_enable_heading_exp_moving_average));
      (cta_cals.createDataSet<boolean_T>("k_cta_f_stop_mode_ttp", ds::From(cta_generic->calibration.k_cta_f_stop_mode_ttp)).write(cta_generic->calibration.k_cta_f_stop_mode_ttp));
      (cta_cals.createDataSet<uint8_t>("k_cta_addit_mature_cycles_outside_sensor_fov", ds::From(cta_generic->calibration.k_cta_addit_mature_cycles_outside_sensor_fov)).write(cta_generic->calibration.k_cta_addit_mature_cycles_outside_sensor_fov));
      (cta_cals.createDataSet<uint8_t>("k_cta_min_age_obj_outside_sensor_fov", ds::From(cta_generic->calibration.k_cta_min_age_obj_outside_sensor_fov)).write(cta_generic->calibration.k_cta_min_age_obj_outside_sensor_fov));
      (cta_cals.createDataSet<uint8_t>("k_cta_amount_butterfly_points_in_use", ds::From(cta_generic->calibration.k_cta_amount_butterfly_points_in_use)).write(cta_generic->calibration.k_cta_amount_butterfly_points_in_use));
      (cta_cals.createDataSet<uint8_t>("k_cta_enable_modes", ds::From(cta_generic->calibration.k_cta_enable_modes)).write(cta_generic->calibration.k_cta_enable_modes));
      (cta_cals.createDataSet<uint8_t>("k_cta_cycle_count_suppress_true_warning", ds::From(cta_generic->calibration.k_cta_cycle_count_suppress_true_warning)).write(cta_generic->calibration.k_cta_cycle_count_suppress_true_warning));
      (cta_cals.createDataSet<uint8_t>("k_cta_cycle_count_hold_true_warning", ds::From(cta_generic->calibration.k_cta_cycle_count_hold_true_warning)).write(cta_generic->calibration.k_cta_cycle_count_hold_true_warning));
      (cta_cals.createDataSet<uint8_t>("k_cta_min_object_age_check_valid", ds::From(cta_generic->calibration.k_cta_min_object_age_check_valid)).write(cta_generic->calibration.k_cta_min_object_age_check_valid));
      (cta_cals.createDataSet<uint8_t>("k_cta_min_object_age_thres", ds::From(cta_generic->calibration.k_cta_min_object_age_thres)).write(cta_generic->calibration.k_cta_min_object_age_thres));
      (cta_cals.createDataSet<uint8_t>("k_cta_cycles_coasted_to_ignore", ds::From(cta_generic->calibration.k_cta_cycles_coasted_to_ignore)).write(cta_generic->calibration.k_cta_cycles_coasted_to_ignore));
      (cta_cals.createDataSet<uint8_t>("k_cta_object_supress_counter", ds::From(cta_generic->calibration.k_cta_object_supress_counter)).write(cta_generic->calibration.k_cta_object_supress_counter));
      (cta_cals.createDataSet<uint8_t>("k_cta_ghost_validation_min_age", ds::From(cta_generic->calibration.k_cta_ghost_validation_min_age)).write(cta_generic->calibration.k_cta_ghost_validation_min_age));
      (cta_cals.createDataSet<uint8_t>("k_cta_ghost_validation_min_mature", ds::From(cta_generic->calibration.k_cta_ghost_validation_min_mature)).write(cta_generic->calibration.k_cta_ghost_validation_min_mature));
      (cta_cals.createDataSet<uint8_t>("k_cta_min_mature_cycles_level_qualifiction", ds::From(cta_generic->calibration.k_cta_min_mature_cycles_level_qualifiction)).write(cta_generic->calibration.k_cta_min_mature_cycles_level_qualifiction));
      (cta_cals.createDataSet<uint8_t>("k_cta_additional_qualification_mature_cycles", ds::From(cta_generic->calibration.k_cta_additional_qualification_mature_cycles)).write(cta_generic->calibration.k_cta_additional_qualification_mature_cycles));
      (cta_cals.createDataSet<uint8_t>("k_ctb_min_brake_qual_ctr_thres", ds::From(cta_generic->calibration.k_ctb_min_brake_qual_ctr_thres)).write(cta_generic->calibration.k_ctb_min_brake_qual_ctr_thres));
      (cta_cals.createDataSet<uint8_t>("k_ctb_min_brake_hold_ctr_thres", ds::From(cta_generic->calibration.k_ctb_min_brake_hold_ctr_thres)).write(cta_generic->calibration.k_ctb_min_brake_hold_ctr_thres));
      (cta_cals.createDataSet<uint8_t>("k_cta_min_qual_age_obj_crossing_paths", ds::From(cta_generic->calibration.k_cta_min_qual_age_obj_crossing_paths)).write(cta_generic->calibration.k_cta_min_qual_age_obj_crossing_paths));
      (cta_cals.createDataSet<uint8_t>("k_cta_cycles_valid_match_of_pot_ghost", ds::From(cta_generic->calibration.k_cta_cycles_valid_match_of_pot_ghost)).write(cta_generic->calibration.k_cta_cycles_valid_match_of_pot_ghost));
      (cta_cals.createDataSet<uint8_t>("k_cta_age_for_new_creation_below_long_intersection", ds::From(cta_generic->calibration.k_cta_age_for_new_creation_below_long_intersection)).write(cta_generic->calibration.k_cta_age_for_new_creation_below_long_intersection));

      auto s31 = file.createGroup("06_Faults", gcpl);
      (s31.createDataSet<uint8_t>("input_core_time_us_no_increase", ds::From(hdf_faults_ptr.input_core_time_us_no_increase), props1D).write(hdf_faults_ptr.input_core_time_us_no_increase));
      (s31.createDataSet<uint8_t>("input_core_cnt_loops_no_increase", ds::From(hdf_faults_ptr.input_core_cnt_loops_no_increase), props1D).write(hdf_faults_ptr.input_core_cnt_loops_no_increase));
      (s31.createDataSet<uint8_t>("input_core_elapsed_time_below_lower_limit", ds::From(hdf_faults_ptr.input_core_elapsed_time_below_lower_limit), props1D).write(hdf_faults_ptr.input_core_elapsed_time_below_lower_limit));
      (s31.createDataSet<uint8_t>("input_core_elapsed_time_above_upper_limit", ds::From(hdf_faults_ptr.input_core_elapsed_time_above_upper_limit), props1D).write(hdf_faults_ptr.input_core_elapsed_time_above_upper_limit));

      (s31.createDataSet<uint8_t>("input_host_vehicle_index_no_increase", ds::From(hdf_faults_ptr.input_host_vehicle_index_no_increase), props1D).write(hdf_faults_ptr.input_host_vehicle_index_no_increase));
      (s31.createDataSet<uint8_t>("input_host_speed_invalid", ds::From(hdf_faults_ptr.input_host_speed_invalid), props1D).write(hdf_faults_ptr.input_host_speed_invalid));
      (s31.createDataSet<uint8_t>("input_host_yawrate_invalid", ds::From(hdf_faults_ptr.input_host_yawrate_invalid), props1D).write(hdf_faults_ptr.input_host_yawrate_invalid));
      (s31.createDataSet<uint8_t>("input_host_longitudinal_acceleration_invalid", ds::From(hdf_faults_ptr.input_host_longitudinal_acceleration_invalid), props1D).write(hdf_faults_ptr.input_host_longitudinal_acceleration_invalid));
      (s31.createDataSet<uint8_t>("input_host_lateral_acceleration_invalid", ds::From(hdf_faults_ptr.input_host_lateral_acceleration_invalid), props1D).write(hdf_faults_ptr.input_host_lateral_acceleration_invalid));

      (s31.createDataSet<uint8_t>("input_raw_detection_range_is_invalid", ds::From(hdf_faults_ptr.input_raw_detection_range_is_invalid), props1D).write(hdf_faults_ptr.input_raw_detection_range_is_invalid));
      (s31.createDataSet<uint8_t>("input_raw_detection_range_rate_is_invalid", ds::From(hdf_faults_ptr.input_raw_detection_range_rate_is_invalid), props1D).write(hdf_faults_ptr.input_raw_detection_range_rate_is_invalid));
      (s31.createDataSet<uint8_t>("input_raw_detection_azimuth_is_invalid", ds::From(hdf_faults_ptr.input_raw_detection_azimuth_is_invalid), props1D).write(hdf_faults_ptr.input_raw_detection_azimuth_is_invalid));
      (s31.createDataSet<uint8_t>("input_raw_detection_elevation_is_invalid", ds::From(hdf_faults_ptr.input_raw_detection_elevation_is_invalid), props1D).write(hdf_faults_ptr.input_raw_detection_elevation_is_invalid));

      (s31.createDataSet<uint8_t>("input_sensors_calib_mounting_pos_is_invalid", ds::From(hdf_faults_ptr.input_sensors_calib_mounting_pos_is_invalid), props2D).write(hdf_faults_ptr.input_sensors_calib_mounting_pos_is_invalid));
      (s31.createDataSet<uint8_t>("input_sensors_calib_polarity_is_invalid", ds::From(hdf_faults_ptr.input_sensors_calib_polarity_is_invalid), props2D).write(hdf_faults_ptr.input_sensors_calib_polarity_is_invalid));
      (s31.createDataSet<uint8_t>("input_sensors_calib_boresight_angle_is_invalid", ds::From(hdf_faults_ptr.input_sensors_calib_boresight_angle_is_invalid), props2D).write(hdf_faults_ptr.input_sensors_calib_boresight_angle_is_invalid));
      (s31.createDataSet<uint8_t>("input_sensors_look_index_no_increase", ds::From(hdf_faults_ptr.input_sensors_look_index_no_increase), props2D).write(hdf_faults_ptr.input_sensors_look_index_no_increase));
      (s31.createDataSet<uint8_t>("input_sensors_sensor_vs_tracker_timestamp_divergence", ds::From(hdf_faults_ptr.input_sensors_sensor_vs_tracker_timestamp_divergence), props2D).write(hdf_faults_ptr.input_sensors_sensor_vs_tracker_timestamp_divergence));

      (s31.createDataSet<uint8_t>("output_track_positions_faulty", ds::From(hdf_faults_ptr.output_track_positions_faulty), props1D).write(hdf_faults_ptr.output_track_positions_faulty));
      (s31.createDataSet<uint8_t>("output_track_velocities_faulty", ds::From(hdf_faults_ptr.output_track_velocities_faulty), props1D).write(hdf_faults_ptr.output_track_velocities_faulty));
      (s31.createDataSet<uint8_t>("output_track_accelerations_faulty", ds::From(hdf_faults_ptr.output_track_accelerations_faulty), props1D).write(hdf_faults_ptr.output_track_accelerations_faulty));
      (s31.createDataSet<uint8_t>("output_severe_angle_jump_presence_fault", ds::From(hdf_faults_ptr.output_severe_angle_jump_presence_fault), props1D).write(hdf_faults_ptr.output_severe_angle_jump_presence_fault));

      (s31.createDataSet<uint8_t>("scl_sensors_calibs_fault_status", ds::From(hdf_faults_ptr.scl_sensors_calibs_fault_status), props2D).write(hdf_faults_ptr.scl_sensors_calibs_fault_status));
      (s31.createDataSet<uint8_t>("scl_sensors_fault_status", ds::From(hdf_faults_ptr.scl_sensors_fault_status), props2D).write(hdf_faults_ptr.scl_sensors_fault_status));
      (s31.createDataSet<uint8_t>("scl_core_info_fault_status", ds::From(hdf_faults_ptr.scl_core_info_fault_status), props1D).write(hdf_faults_ptr.scl_core_info_fault_status));
      (s31.createDataSet<uint8_t>("scl_host_info_fault_status", ds::From(hdf_faults_ptr.scl_host_info_fault_status), props1D).write(hdf_faults_ptr.scl_host_info_fault_status));
      (s31.createDataSet<uint8_t>("scl_raw_detection_fault_status", ds::From(hdf_faults_ptr.scl_raw_detection_fault_status), props1D).write(hdf_faults_ptr.scl_raw_detection_fault_status));
      (s31.createDataSet<uint8_t>("scl_object_track_fault_status", ds::From(hdf_faults_ptr.scl_object_track_fault_status), props1D).write(hdf_faults_ptr.scl_object_track_fault_status));
      (s31.createDataSet<uint8_t>("scl_angle_jump_fault_status", ds::From(hdf_faults_ptr.scl_angle_jump_fault_status), props1D).write(hdf_faults_ptr.scl_angle_jump_fault_status));
      (s31.createDataSet<uint8_t>("scl_overall_fault_status", ds::From(hdf_faults_ptr.scl_overall_fault_status), props1D).write(hdf_faults_ptr.scl_overall_fault_status));
      (s31.createDataSet<uint8_t>("scl_should_reset", ds::From(hdf_faults_ptr.scl_should_reset), props1D).write(hdf_faults_ptr.scl_should_reset));

      auto s32 = file.createGroup("07_StationaryGeometry", gcpl);
      (s32.createDataSet<uint64_t>("execution_timestamp", ds::From(hdf_sg_ptr.execution_timestamp_us), props1D).write(hdf_sg_ptr.execution_timestamp_us));
      (s32.createDataSet<uint64_t>("measurement_timestamp", ds::From(hdf_sg_ptr.measurement_timestamp_us), props1D).write(hdf_sg_ptr.measurement_timestamp_us));
      (s32.createDataSet<float32_T>("xposn", ds::From(hdf_sg_ptr.position_x), props_sg).write(hdf_sg_ptr.position_x));
      (s32.createDataSet<float32_T>("yposn", ds::From(hdf_sg_ptr.position_y), props_sg).write(hdf_sg_ptr.position_y));
      (s32.createDataSet<float32_T>("xposnvar", ds::From(hdf_sg_ptr.position_variance_x), props_sg).write(hdf_sg_ptr.position_variance_x));
      (s32.createDataSet<float32_T>("yposnvar", ds::From(hdf_sg_ptr.position_variance_y), props_sg).write(hdf_sg_ptr.position_variance_y));
      (s32.createDataSet<float32_T>("xyposncov", ds::From(hdf_sg_ptr.position_covariance_xy), props_sg).write(hdf_sg_ptr.position_covariance_xy));
      (s32.createDataSet<uint16_t>("cycles_since_created", ds::From(hdf_sg_ptr.cycles_since_created), props_sg).write(hdf_sg_ptr.cycles_since_created));
      (s32.createDataSet<uint16_t>("cycles_since_coasted", ds::From(hdf_sg_ptr.cycles_since_coasted), props_sg).write(hdf_sg_ptr.cycles_since_coasted));
      (s32.createDataSet<uint8_t>("drivability", ds::From(hdf_sg_ptr.drivability), props_sg).write(hdf_sg_ptr.drivability));
      (s32.createDataSet<uint8_t>("drivability_confidence", ds::From(hdf_sg_ptr.drivability_confidence), props_sg).write(hdf_sg_ptr.drivability_confidence));
      (s32.createDataSet<uint32_t>("unique_id", ds::From(hdf_sg_ptr.unique_id), props_sg_contours).write(hdf_sg_ptr.unique_id));
      (s32.createDataSet<uint16_t>("num_vertices", ds::From(hdf_sg_ptr.num_vertices), props_sg_contours).write(hdf_sg_ptr.num_vertices));
      (s32.createDataSet<uint8_t>("type", ds::From(hdf_sg_ptr.type), props_sg_contours).write(hdf_sg_ptr.type));
      (s32.createDataSet<uint16_t>("major_version", ds::From(hdf_sg_ptr.major), props1D).write(hdf_sg_ptr.major));
      (s32.createDataSet<uint16_t>("minor_version", ds::From(hdf_sg_ptr.minor), props1D).write(hdf_sg_ptr.minor));
      (s32.createDataSet<uint16_t>("patch_version", ds::From(hdf_sg_ptr.patch), props1D).write(hdf_sg_ptr.patch));
      (s32.createDataSet<uint32_t>("cycle_index", ds::From(hdf_sg_ptr.cycle_index), props1D).write(hdf_sg_ptr.cycle_index));
      (s32.createDataSet<uint32_t>("num_contours", ds::From(hdf_sg_ptr.num_contours), props1D).write(hdf_sg_ptr.num_contours));
      (s32.createDataSet<boolean_T>("f_valid", ds::From(hdf_sg_ptr.f_valid), props1D).write(hdf_sg_ptr.f_valid));
   }
}

void HDFWriteClass::HDF_IpWrite(void) {

   HighFive::File file(this->HDF_IPath, HighFive::File::Truncate);
   auto gcpl = HighFive::GroupCreateProps::Default();
   gcpl.add(HighFive::AttributePhaseChange(0, 0));

   size_t vecsize = hdf_scan_indexIp_ptr.scan_index.size();

   if (vecsize != 0) {

      HighFive::DataSetCreateProps props1D;
      props1D.add(HighFive::Chunking{vecsize});
      props1D.add(HighFive::Deflate(9));

      HighFive::DataSetCreateProps props_GTA;
      props_GTA.add(HighFive::Chunking{vecsize, hdf_all_objectsGT_ptr.unique_id[0].size()});
      props_GTA.add(HighFive::Deflate(9));

      using Clock = std::chrono::system_clock;
      auto now    = Clock::now();

      std::time_t tt = Clock::to_time_t(now);
      std::tm local_tm{};
#ifdef _WIN32
      localtime_s(&local_tm, &tt);
#else
      localtime_r(&tt, &local_tm);
#endif
      char ts_buf[32];

      std::strftime(ts_buf, sizeof(ts_buf), "%d-%m-%Y %H:%M:%S", &local_tm);
      std::string created_datetime(ts_buf);
      std::string DC_version      = std::to_string(APPLICATION_MAJOR_VERSION) + "." + std::to_string(APPLICATION_MINOR_VERSION) + "." + std::to_string(APPLICATION_PATCH_VERSION);
      std::string tracker_version = "10.35.0";
      std::string ocg_version     = "1.10.1";
      std::string olp_version     = "5.1.1";
      std::string sfl_version     = "3.3.0";

      auto meta_grp = file.createGroup("00_metadata", gcpl);
      meta_grp.createDataSet<std::string>("Created_datetime", ds::From(created_datetime)).write(created_datetime);
      meta_grp.createDataSet<std::string>("DC_version", ds::From(DC_version)).write(DC_version);
      meta_grp.createDataSet<std::string>("Tracker_version", ds::From(tracker_version)).write(tracker_version);
      meta_grp.createDataSet<std::string>("OCG_version", ds::From(ocg_version)).write(ocg_version);
      meta_grp.createDataSet<std::string>("OLP_version", ds::From(olp_version)).write(olp_version);
      meta_grp.createDataSet<std::string>("SFL_version", ds::From(sfl_version)).write(sfl_version);

      auto scan_grp = file.createGroup("01_Scan_Index", gcpl);
      (scan_grp.createDataSet<uint32_t>("scan_index", ds::From(hdf_scan_indexIp_ptr.scan_index), props1D).write(hdf_scan_indexIp_ptr.scan_index));

      // Write Tracker information
      auto TrkInfo = file.createGroup("03_TrackerInfo", gcpl);
      auto TrkOut  = TrkInfo.createGroup("Tracker_Output");
      auto AllObj  = TrkOut.createGroup("AllObjects");

      (AllObj.createDataSet<unsigned32_T>("unique_id", ds::From(hdf_all_objectsGT_ptr.unique_id), props_GTA).write(hdf_all_objectsGT_ptr.unique_id));
      (AllObj.createDataSet<float32_T>("vcs_xposn", ds::From(hdf_all_objectsGT_ptr.vcs_xposn), props_GTA).write(hdf_all_objectsGT_ptr.vcs_xposn));
      (AllObj.createDataSet<float32_T>("vcs_yposn", ds::From(hdf_all_objectsGT_ptr.vcs_yposn), props_GTA).write(hdf_all_objectsGT_ptr.vcs_yposn));
      (AllObj.createDataSet<float32_T>("vcs_xvel", ds::From(hdf_all_objectsGT_ptr.vcs_xvel), props_GTA).write(hdf_all_objectsGT_ptr.vcs_xvel));
      (AllObj.createDataSet<float32_T>("vcs_yvel", ds::From(hdf_all_objectsGT_ptr.vcs_yvel), props_GTA).write(hdf_all_objectsGT_ptr.vcs_yvel));
      (AllObj.createDataSet<float32_T>("vcs_xaccel", ds::From(hdf_all_objectsGT_ptr.vcs_xaccel), props_GTA).write(hdf_all_objectsGT_ptr.vcs_xaccel));
      (AllObj.createDataSet<float32_T>("vcs_yaccel", ds::From(hdf_all_objectsGT_ptr.vcs_yaccel), props_GTA).write(hdf_all_objectsGT_ptr.vcs_yaccel));
      (AllObj.createDataSet<float32_T>("vcs_heading", ds::From(hdf_all_objectsGT_ptr.vcs_heading), props_GTA).write(hdf_all_objectsGT_ptr.vcs_heading));
      (AllObj.createDataSet<float32_T>("speed", ds::From(hdf_all_objectsGT_ptr.speed), props_GTA).write(hdf_all_objectsGT_ptr.speed));
      (AllObj.createDataSet<float32_T>("tang_accel", ds::From(hdf_all_objectsGT_ptr.tang_accel), props_GTA).write(hdf_all_objectsGT_ptr.tang_accel));
      (AllObj.createDataSet<float32_T>("len1", ds::From(hdf_all_objectsGT_ptr.len1), props_GTA).write(hdf_all_objectsGT_ptr.len1));
      (AllObj.createDataSet<float32_T>("wid1", ds::From(hdf_all_objectsGT_ptr.wid1), props_GTA).write(hdf_all_objectsGT_ptr.wid1));
      (AllObj.createDataSet<unsigned8_T>("status", ds::From(hdf_all_objectsGT_ptr.status), props_GTA).write(hdf_all_objectsGT_ptr.status));
      (AllObj.createDataSet<unsigned8_T>("object_class", ds::From(hdf_all_objectsGT_ptr.object_class), props_GTA).write(hdf_all_objectsGT_ptr.object_class));
   }
}
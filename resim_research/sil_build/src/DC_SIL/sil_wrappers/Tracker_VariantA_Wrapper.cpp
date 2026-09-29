/*===========================================================================*\
 * Standard Header Includes
\*===========================================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <filesystem>
/*===========================================================================*\
 * Project Header Includes
\*===========================================================================*/
#include "f360_host_calib.h"
#include "f360_input_diagnostics.h"
#include "f360_output_diagnostics.h"
#include "f360_safety_control_logic.h"
#include "f360_types.h"
#include "f360_tracker.h"
#include "rspp_core_info.h"
#include "rspp_host.h"
#include "rspp.h"
#include "State_Manager.h"
#include "AutoGenSMCParameter.h"
#include "SMCParameters.h"
#include "DCMacros.h"
#include "VSE_Core.h"
#include "Tracker_VariantA_Wrapper.h"
#include "ocg_occupancy_grid_types.h"
#include "ocg_occupancy_grid.h"
#include "sg_stationary_geometries.h"
#include "sg_output.h"
#include "AS_bin_writer_wrapper.h"

// pointers
static f360_variant_A::F360_Tracker *ptr_f360tracker;
static f360_variant_A::Input_Diagnostics *ptr_input_diag;
static f360_variant_A::Output_Diagnostics *ptr_output_diag;
static f360_variant_A::F360_Host_T *ptr_host;
static f360_variant_A::F360_Host_Calib_T *ptr_host_calib;
static f360_variant_A::F360_Host_Raw_T *ptr_host_raw;
static rspp_variant_A::RSPP_Detection_List_T *ptr_detection_list;
static f360_variant_A::F360_Core_Info_T *ptr_core_info;
static f360_variant_A::F360_Object_Log_Output_T *ptr_obj_log;
static Tracker_Info_Log_T *ptr_tracker_info_log;
static RSPP_Core_Info_T *ptr_rspp_core_info;
static RSPP_Host_T *ptr_rspp_host;
static f360_variant_A::SafetyControlLogic *ptr_SCL;
static f360_variant_A::State_Manager *ptr_SM;
static vse_core::VSE_CORE *ptr_core_vse;
static ocg::OCG_Inputs_T *OCG_inputs;
static ocg::OCG_Outputs_T *OCG_outputs;
static ocg::Occupancy_Grid *OCG_grid;
static f360_variant_A::F360_Sensor_Calib_Log_Output_T *ptr_sensor_calib;
/*SG Inputs*/
static sg::SG_Internals_Dump_T *SG_internal;
static sg::Stationary_Geometries *SG_geometries;
static sg::SG_Output_T *sg_output;

// variables
static f360_variant_A::F360_Detection_Log_Output_T log_dets;
static Functional_Safety_Faults_Log_T faults;
static ROT_Object_List_Info_T rot_obj;
static vse_core::VSE_CORE core_vse;
static DCSMCParameters smc_parameters;
static rspp_variant_A::F360_Radar_Sensor_T sensors[rspp_variant_A::MAX_NUMBER_OF_SENSORS]{};
static bool run_first_time = true;
static VSE_OUT vse_info;

const APT_ECU_Vehicle_Data_Input_T *veh_data = nullptr;
std::vector<DetectionObj_Data_Input_T *> Det_Data;
std::vector<MOUNTING_VALUES_T *> mounting_values;
namespace fs = std::filesystem;
static std::string xtrk_flag;
static std::string XTRK_Path               = "";
static std::string previous_xtrk_file_name = "";
static double tracker_timestamp_us         = 0;

f360_variant_A::F360_Object_Log_Output_T *GetF360TrackerObject() {
   return ptr_obj_log;
}
f360_variant_A::F360_Detection_Log_Output_T *GetF360DetObject() {
   return &log_dets;
}
rspp_variant_A::F360_Radar_Sensor_T *GetF360SensorInfo() {
   return sensors;
}
f360_variant_A::F360_Core_Info_T *GetF360CoreInfo() {
   return ptr_core_info;
}
ROT_Object_List_Info_T *GetF360TrackerROTObject() {
   return &rot_obj;
}
Tracker_Info_Log_T *GetF360TrackerInfo() {
   return ptr_tracker_info_log;
}

f360_variant_A::F360_Host_Calib_T *GetF360HostCalib() {
   return ptr_host_calib;
}
f360_variant_A::F360_Host_T *GetHostInfo() {
   return ptr_host;
}
VSE_OUT *GetVSEOutput() {
   return &vse_info;
}
f360_variant_A::F360_Sensor_Calib_Log_Output_T *GetSensCalibPtr() {
   return ptr_sensor_calib;
}
rspp_variant_A::RSPP_Detection_List_T *GetDetectionList() {
   return ptr_detection_list;
}
f360_variant_A::F360_Host_Raw_T *GetHostRawInfo() {
   return ptr_host_raw;
}
sg::SG_Output_T *GetSGOutputPtr() {
   return sg_output;
}
Functional_Safety_Faults_Log_T *GetFunctionalSafetyFaultsLogPtr() {
   return &faults;
}

template <typename RSPP_T, typename Host_T>
static void Set_RSPP_Host_Info(RSPP_T &ptr_rspp_host, Host_T &ptr_host) {
   ptr_rspp_host.vehicle_index             = ptr_host.vehicle_index;
   ptr_rspp_host.speed                     = ptr_host.speed;
   ptr_rspp_host.vcs_speed                 = ptr_host.vcs_speed;
   ptr_rspp_host.acceleration              = ptr_host.acceleration;
   ptr_rspp_host.vcs_lat_acceleration      = ptr_host.vcs_lat_acceleration;
   ptr_rspp_host.vcs_long_acceleration     = ptr_host.vcs_long_acceleration;
   ptr_rspp_host.yaw_rate_rad              = ptr_host.yaw_rate_rad;
   ptr_rspp_host.vcs_sideslip              = ptr_host.vcs_sideslip;
   ptr_rspp_host.curvature_rear            = ptr_host.curvature_rear;
   ptr_rspp_host.dist_rear_axle_to_vcs_m   = ptr_host.dist_rear_axle_to_vcs_m;
   ptr_rspp_host.rear_cornering_compliance = ptr_host.rear_cornering_compliance;
   ptr_rspp_host.speed_correction_factor   = ptr_host.speed_correction_factor;
   ptr_rspp_host.speed_qf                  = ptr_host.speed_qf;
   ptr_rspp_host.yaw_rate_qf               = ptr_host.yaw_rate_qf;
   ptr_rspp_host.lat_accel_qf              = ptr_host.lat_accel_qf;
   ptr_rspp_host.long_accel_qf             = ptr_host.long_accel_qf;
}

template <typename RSPP_Core_T, typename Core_Info_T>
static void Set_RSPP_Core_Info(RSPP_Core_T &ptr_rspp_core_info, Core_Info_T &ptr_core_info) {
   ptr_rspp_core_info.cnt_loops      = ptr_core_info.cnt_loops;
   ptr_rspp_core_info.elapsed_time_s = ptr_core_info.elapsed_time_s;
   ptr_rspp_core_info.prev_time_us   = ptr_core_info.prev_time_us;
   ptr_rspp_core_info.time_us        = ptr_core_info.time_us;
}
template <typename OCG_Inputs_T, typename F360_Host_T>
static void Set_OCG_Host_Info(OCG_Inputs_T &OCG_inputs, F360_Host_T &ptr_host) {
   OCG_inputs.host.dist_rear_axle_to_vcs_m     = ptr_host.dist_rear_axle_to_vcs_m;
   OCG_inputs.host.vehicle_index               = ptr_host.vehicle_index;
   OCG_inputs.host.speed                       = ptr_host.speed;
   OCG_inputs.host.vcs_speed                   = ptr_host.vcs_speed;
   OCG_inputs.host.acceleration                = ptr_host.acceleration;
   OCG_inputs.host.vcs_lat_acceleration        = ptr_host.vcs_lat_acceleration;
   OCG_inputs.host.vcs_long_acceleration       = ptr_host.vcs_long_acceleration;
   OCG_inputs.host.yaw_rate_rad                = ptr_host.yaw_rate_rad;
   OCG_inputs.host.vcs_sideslip                = ptr_host.vcs_sideslip;
   OCG_inputs.host.curvature_rear              = ptr_host.curvature_rear;
   OCG_inputs.host.dist_rear_axle_to_vcs_m     = ptr_host.dist_rear_axle_to_vcs_m;
   OCG_inputs.host.rear_cornering_compliance   = ptr_host.rear_cornering_compliance;
   OCG_inputs.host.speed_correction_factor     = ptr_host.speed_correction_factor;
   OCG_inputs.host.f_trailer_presence_hardware = ptr_host.f_trailer_presence_hardware;
   OCG_inputs.host.speed_qf                    = ptr_host.speed_qf;
   OCG_inputs.host.yaw_rate_qf                 = ptr_host.yaw_rate_qf;
   OCG_inputs.host.lat_accel_qf                = ptr_host.lat_accel_qf;
   OCG_inputs.host.long_accel_qf               = ptr_host.long_accel_qf;
}

static void UpdateSensorConstants(int sensortype) {
   for (int i = 0; i < rspp_variant_A::MAX_NUMBER_OF_SENSORS && i < MAX_DC_SENSORS; i++) {
      switch (sensortype) {
      case 0:
         sensors[i].constant.sensor_type = RSPP_SENSOR_TYPE_SRR5_RADAR;
         break;
      case 1:
         sensors[i].constant.sensor_type = RSPP_SENSOR_TYPE_MRR3_RADAR;
         break;
      case 2:
         sensors[i].constant.sensor_type = RSPP_SENSOR_TYPE_SRR5_RADAR;
         break;
      case 3:
         sensors[i].constant.sensor_type = RSPP_SENSOR_TYPE_SRR4_RADAR;
         break;
      case 4:
         sensors[i].constant.sensor_type = RSPP_SENSOR_TYPE_FLR4_RADAR;
         break;
      case 5:
         sensors[i].constant.sensor_type = RSPP_SENSOR_TYPE_SRR6_PLUS_RADAR;
         break;
      case 6:
         sensors[i].constant.sensor_type = RSPP_SENSOR_TYPE_SRR6_RADAR;
         break;
      case 7:
         sensors[i].constant.sensor_type = RSPP_SENSOR_TYPE_FLR4_PLUS_RADAR;
         break;
      case 8:
         sensors[i].constant.sensor_type = RSPP_SENSOR_TYPE_SRR7_PLUS_RADAR;
         break;
      case 9:
         sensors[i].constant.sensor_type = RSPP_SENSOR_TYPE_FLR7_RADAR;
         break;
      }

      sensors[i].constant.range_limits[0] = static_cast<float>(smc_parameters.look_parameters_LRLL[0]);
      sensors[i].constant.range_limits[1] = static_cast<float>(smc_parameters.look_parameters_LRML[0]);
      sensors[i].constant.range_limits[2] = static_cast<float>(smc_parameters.look_parameters_MRLL[0]);
      sensors[i].constant.range_limits[3] = static_cast<float>(smc_parameters.look_parameters_MRML[0]);

      // v_wrapping and r_wrapping are temporarily hardcoded with values from emblib.To be updated later.

      sensors[i].constant.v_wrapping[0] = smc_parameters.look_parameters_LRLL[3] - smc_parameters.look_parameters_LRLL[2];
      sensors[i].constant.v_wrapping[1] = smc_parameters.look_parameters_LRML[3] - smc_parameters.look_parameters_LRML[2];
      sensors[i].constant.v_wrapping[2] = smc_parameters.look_parameters_MRLL[3] - smc_parameters.look_parameters_MRLL[2];
      sensors[i].constant.v_wrapping[3] = smc_parameters.look_parameters_MRML[3] - smc_parameters.look_parameters_MRML[2];

#ifdef SRR_DC
      sensors[i].constant.r_wrapping[0] = 0.316389962F;
      sensors[i].constant.r_wrapping[1] = 0.295186833F;
      sensors[i].constant.r_wrapping[2] = 0.330654609F;
      sensors[i].constant.r_wrapping[3] = 0.287792106F;

#elif MRR_DC
      sensors[i].constant.r_wrapping[0] = 0.375258014F;
      sensors[i].constant.r_wrapping[1] = 0.298435793F;
      sensors[i].constant.r_wrapping[2] = 0.370417108F;
      sensors[i].constant.r_wrapping[3] = 0.293496303F;
#endif

      sensors[i].constant.fov_min_az_rad[0] = static_cast<float>(smc_parameters.fov_parameters[0]);
      sensors[i].constant.fov_min_az_rad[1] = static_cast<float>(smc_parameters.fov_parameters[0]);
      sensors[i].constant.fov_min_az_rad[2] = static_cast<float>(smc_parameters.fov_parameters[0]);
      sensors[i].constant.fov_min_az_rad[3] = static_cast<float>(smc_parameters.fov_parameters[0]);
      sensors[i].constant.fov_max_az_rad[0] = static_cast<float>(smc_parameters.fov_parameters[1]);
      sensors[i].constant.fov_max_az_rad[1] = static_cast<float>(smc_parameters.fov_parameters[1]);
      sensors[i].constant.fov_max_az_rad[2] = static_cast<float>(smc_parameters.fov_parameters[1]);
      sensors[i].constant.fov_max_az_rad[3] = static_cast<float>(smc_parameters.fov_parameters[1]);

      sensors[i].constant.fov_min_el_rad[0] = static_cast<float>(smc_parameters.fov_parameters[2]);
      sensors[i].constant.fov_min_el_rad[1] = static_cast<float>(smc_parameters.fov_parameters[2]);
      sensors[i].constant.fov_min_el_rad[2] = static_cast<float>(smc_parameters.fov_parameters[2]);
      sensors[i].constant.fov_min_el_rad[3] = static_cast<float>(smc_parameters.fov_parameters[2]);

      sensors[i].constant.fov_max_el_rad[0] = static_cast<float>(smc_parameters.fov_parameters[3]);
      sensors[i].constant.fov_max_el_rad[1] = static_cast<float>(smc_parameters.fov_parameters[3]);
      sensors[i].constant.fov_max_el_rad[2] = static_cast<float>(smc_parameters.fov_parameters[3]);
      sensors[i].constant.fov_max_el_rad[3] = static_cast<float>(smc_parameters.fov_parameters[3]);

      sensors[i].constant.min_aliaised_range_rate[0] = static_cast<float>(smc_parameters.look_parameters_LRLL[2]);
      sensors[i].constant.min_aliaised_range_rate[1] = static_cast<float>(smc_parameters.look_parameters_LRML[2]);
      sensors[i].constant.min_aliaised_range_rate[2] = static_cast<float>(smc_parameters.look_parameters_MRLL[2]);
      sensors[i].constant.min_aliaised_range_rate[3] = static_cast<float>(smc_parameters.look_parameters_MRML[2]);

      // Version info
      sensors[i].constant.id = i + 1;
#ifdef SRR_DC
      sensors[0].constant.mounting_location = RSPP_MOUNTING_LOCATION_LEFT_FORWARD;
      sensors[1].constant.mounting_location = RSPP_MOUNTING_LOCATION_RIGHT_FORWARD;
      sensors[2].constant.mounting_location = RSPP_MOUNTING_LOCATION_LEFT_REAR;
      sensors[3].constant.mounting_location = RSPP_MOUNTING_LOCATION_RIGHT_REAR;
#elif MRR_DC
      sensors[0].constant.mounting_location = RSPP_MOUNTING_LOCATION_CENTER_FORWARD;
#endif
   }
}

static void InitializeHostCalib() {
   /* Init Host Calib */

   ptr_host_calib->host_type                                  = f360_variant_A::F360_HOST_TYPE_PASSENGER_VEHICLE; // set as F360_HOST_TYPE_COMMERCIAL_VEHICLE for commercial vehicles
   ptr_host_calib->dist_rear_axle_to_vcs_m                    = std::abs(veh_data->rear_axle_position);
   ptr_host_calib->vehicle_length_m                           = veh_data->host_vehicle_length;
   ptr_host_calib->vehicle_width_m                            = veh_data->host_vehicle_width;
   ptr_host_calib->rear_cornering_compliance                  = 0.0053F;
   ptr_host_calib->steer_gear_ratio                           = 14.8F; // TODO: Where to get this value? The ratio between the turn of the steering wheel and the turn of the wheels
   ptr_host_calib->wheelbase_m                                = 2.65F;
   ptr_host_calib->understeer_coefficient                     = 0.0053F; // According to VSE team, this can be used as default value
   ptr_host_calib->cog_x                                      = -1.0F;   // Currently not used
   ptr_host_calib->cog_y                                      = -1.0F;   // Currently not used
   ptr_host_calib->front_wheel_radius_m                       = -1.0F;   // Currently not used
   ptr_host_calib->front_track_width_m                        = -1.0F;   // Currently not used
   ptr_host_calib->raw_host_signal_latency_ms                 = 30;
   ptr_host_calib->f_enable_internal_reflections_func         = 0; // CAF_Parameter.C_Internal_Reflections_Enable_Function;
   ptr_host_calib->f_enable_internal_reflections_func_trailer = 0; // CAF_Parameter.C_LCW_trailer_mode ;
}

static void UpdateMountingInfo() {

   for (int i = 0; i < rspp_variant_A::MAX_NUMBER_OF_SENSORS && i < MAX_DC_SENSORS; i++) {

      MOUNTING_VALUES_T *mounting_posn                                    = mounting_values[DC_SENSOR + i];
      sensors[i].constant.polarity                                        = static_cast<int>(mounting_posn->azimuth_polarity);
      sensors[i].constant.mounting_position.vcs_position.lateral          = mounting_posn->vcs_lat_position;
      sensors[i].constant.mounting_position.vcs_position.longitudinal     = mounting_posn->vcs_lon_position;
      sensors[i].constant.mounting_position.vcs_boresight_azimuth_angle   = mounting_posn->boresight_angle;
      sensors[i].constant.mounting_position.vcs_boresight_elevation_angle = 0.0f;
      sensors[i].constant.mounting_position.vcs_position.height           = (mounting_posn->vcs_z_position == 0.0f) ? 0.5f : mounting_posn->vcs_z_position;
   }
}
// latest RSPP code expects all sensors calibration data to be passed, hence copying the first sensor calibration data to all other sensors.
static void CopySensor0CalibrationToInactiveSensors() {
#ifdef SRR_DC
   for (int i = 4; i < rspp_variant_A::MAX_NUMBER_OF_SENSORS; i++) {
      sensors[i]             = sensors[0];
      sensors[i].constant.id = static_cast<uint8_t>(i + 1);
   }
#elif MRR_DC
   for (int i = 1; i < rspp_variant_A::MAX_NUMBER_OF_SENSORS; i++) {
      sensors[i]             = sensors[0];
      sensors[i].constant.id = static_cast<uint8_t>(i + 1);
   }
#endif
}

static void UpdateSensorVariables(double timestamp_us) {
   for (int i = 0; i < rspp_variant_A::MAX_NUMBER_OF_SENSORS && i < MAX_DC_SENSORS; i++) {
      DetectionObj_Data_Input_T *dets                 = Det_Data[DC_SENSOR + i];
      sensors[i].variable.is_valid                    = true;
      sensors[i].variable.number_of_valid_detections  = dets->dets_info.Count;
      sensors[i].variable.look_index                  = dets->dets_info.ScanIndex;
      sensors[i].variable.look_id                     = RSPP_Det_Look_ID_T(dets->dets_info.LookType);
      sensors[i].variable.timestamp_us                = static_cast<uint64_t>(timestamp_us);
      sensors[i].variable.vacs_boresight_az_estimated = sensors[i].constant.mounting_position.vcs_boresight_azimuth_angle;   // considering no misalignment
      sensors[i].variable.vacs_boresight_el_estimated = sensors[i].constant.mounting_position.vcs_boresight_elevation_angle; // considering no misalignment
   }
}

static void UpdateSensorDetections() {
   memset(ptr_detection_list, 0, sizeof(rspp_variant_A::RSPP_Detection_List_T));
   int det_index = 0;
   for (int i = 0; i < rspp_variant_A::MAX_NUMBER_OF_SENSORS && i < MAX_DC_SENSORS; i++) {
      DetectionObj_Data_Input_T *dets = Det_Data[DC_SENSOR + i];
      int num_valid_detections        = dets->dets_info.Count;

#ifdef SRR_DC
      bool f_max_dets_reached = (det_index >= static_cast<int>(rspp_variant_A::MAX_NUMBER_OF_SRR_SENSORS * rspp_variant_A::NUMBER_OF_SRR_DETECTIONS));
#elif MRR_DC
      bool f_max_dets_reached = (det_index >= static_cast<int>(rspp_variant_A::MAX_NUMBER_OF_MRR_SENSORS * rspp_variant_A::NUMBER_OF_MRR_DETECTIONS));
#endif
      ptr_detection_list->number_of_valid_detections += num_valid_detections;
      for (int j = 0; j < num_valid_detections && !f_max_dets_reached; j++) {
         ptr_detection_list->detections[det_index + j].raw.range      = dets->dets_input[j].range;
         ptr_detection_list->detections[det_index + j].raw.range_rate = dets->dets_input[j].range_rate;
         ptr_detection_list->detections[det_index + j].raw.azimuth    = dets->dets_input[j].azimuth;
         ptr_detection_list->detections[det_index + j].raw.elevation  = dets->dets_input[j].elevation;
         ptr_detection_list->detections[det_index + j].raw.snr        = dets->dets_input[j].snr;
         ptr_detection_list->detections[det_index + j].raw.det_id     = j + 1;
         ptr_detection_list->detections[det_index + j].raw.sensor_id  = i + 1;
      }
      det_index += num_valid_detections;
   }
}

static void UpdateHostInfo(double timestamp_us) {
   ptr_host_raw->global_time_sync_s       = static_cast<float>(timestamp_us * 1E-6);
   ptr_host_raw->timestamp_s              = static_cast<float>(timestamp_us * 1E-6);
   ptr_host_raw->raw_speed                = (veh_data->f_reverse_gear == 1) ? -1.0F * veh_data->abs_speed : veh_data->abs_speed;
   ptr_host_raw->speed_qf                 = 3;
   ptr_host_raw->raw_yaw_rate_rad         = veh_data->yawrate;
   ptr_host_raw->yaw_rate_qf              = 3;
   ptr_host_raw->steering_wheel_angle_rad = veh_data->steering_angle;
   ptr_host_raw->steering_wheel_angle_qf  = 3;
   ptr_host_raw->lat_accel                = veh_data->vcs_lat_acc;
   ptr_host_raw->long_accel               = veh_data->vcs_long_acc;
   ptr_host_raw->lat_accel_qf             = 3;
   ptr_host_raw->long_accel_qf            = 3;
   if (ptr_host_raw->raw_speed < 0.0f) {
      ptr_host_raw->prndl        = F360_PRNDL_STATE_REVERSE;
      ptr_host_raw->reverse_gear = 1;
   } else {
      ptr_host_raw->prndl        = F360_PRNDL_STATE_DRIVE;
      ptr_host_raw->reverse_gear = 0;
   }
   ptr_host->speed_correction_factor = 1.0;
}

static void UpdateSensorMotion() {
   auto xsens         = 0.0f;
   auto ysens         = 0.0f;
   auto xdotvcs       = 0.0f;
   auto ydotvcs       = 0.0f;
   float cb           = 0.0f;
   float sb           = 0.0f;
   auto rear_sideslip = static_cast<float>(-(0.0053f) * (0.01 * ptr_host->speed * ptr_host->speed));
   cb                 = cos(rear_sideslip);
   sb                 = sin(rear_sideslip);

   for (int i = 0; i < rspp_variant_A::MAX_NUMBER_OF_SENSORS && i < MAX_DC_SENSORS; i++) {
      /* Sensor position relative to center of rear axle (in VCS-aligned coordinates) */
      xsens = ptr_host->dist_rear_axle_to_vcs_m + sensors[i].constant.mounting_position.vcs_position.longitudinal;
      ysens = sensors[i].constant.mounting_position.vcs_position.lateral;

      /* VCS components of OTG velocity of sensor */
      xdotvcs = (ptr_host->speed * cb) - (ptr_host->yaw_rate_rad * ysens);
      ydotvcs = (ptr_host->speed * sb) + (ptr_host->yaw_rate_rad * xsens);

      sensors[i].variable.vcs_velocity.longitudinal = xdotvcs;
      sensors[i].variable.vcs_velocity.lateral      = ydotvcs;
   }
}

static void UpdateCoreInfo(double timestamp_us) {
   ptr_core_info->cnt_loops++;
   ptr_core_info->prev_time_us   = (1 == ptr_core_info->cnt_loops) ? static_cast<uint64_t>(timestamp_us) : ptr_core_info->time_us;
   ptr_core_info->time_us        = static_cast<uint64_t>(timestamp_us);
   ptr_core_info->elapsed_time_s = (F360_FPN_T)((F360_DPN_T)(ptr_core_info->time_us - ptr_core_info->prev_time_us) * 1e-6);
   if (ptr_core_info->elapsed_time_s < 0.04F) {
      ptr_core_info->elapsed_time_s = 0.05F;
   }
}

void InitTracker(int customer, int sensortype, DC_INPUT_DATA_T *sil_input_buffer, std::string XTRKConfig) {
   ptr_host_calib       = new f360_variant_A::F360_Host_Calib_T();
   ptr_sensor_calib     = new f360_variant_A::F360_Sensor_Calib_Log_Output_T();
   ptr_core_vse         = &core_vse;
   sg_output            = new sg::SG_Output_T();
   SG_geometries        = new sg::Stationary_Geometries();
   SG_internal          = new sg::SG_Internals_Dump_T();
   OCG_inputs           = new ocg::OCG_Inputs_T();
   OCG_outputs          = new ocg::OCG_Outputs_T();
   OCG_grid             = new ocg::Occupancy_Grid();
   ptr_f360tracker      = new f360_variant_A::F360_Tracker();
   ptr_input_diag       = new f360_variant_A::Input_Diagnostics();
   ptr_output_diag      = new f360_variant_A::Output_Diagnostics();
   ptr_host             = new f360_variant_A::F360_Host_T();
   ptr_host_raw         = new f360_variant_A::F360_Host_Raw_T();
   ptr_detection_list   = new rspp_variant_A::RSPP_Detection_List_T();
   ptr_core_info        = new f360_variant_A::F360_Core_Info_T();
   ptr_obj_log          = new f360_variant_A::F360_Object_Log_Output_T();
   ptr_tracker_info_log = new Tracker_Info_Log_T();
   ptr_rspp_core_info   = new RSPP_Core_Info_T();
   ptr_rspp_host        = new RSPP_Host_T();
   ptr_SCL              = new f360_variant_A::SafetyControlLogic(*ptr_input_diag, *ptr_output_diag);
   ptr_SM               = new f360_variant_A::State_Manager(*ptr_SCL, *ptr_f360tracker);
   smc_parameters.initDCSMCParameters(DCCustomerVariant(DC_GET_VARIANT(customer, sensortype)));
   memset(&sensors[0], 0, sizeof(rspp_variant_A::F360_Radar_Sensor_T) * rspp_variant_A::MAX_NUMBER_OF_SENSORS);
   run_first_time = true;
   UpdateSensorConstants(sensortype);

   veh_data = &sil_input_buffer->vehicle_inputs;
   Det_Data.push_back(&sil_input_buffer->sym_detection_fl_radar);
   Det_Data.push_back(&sil_input_buffer->sym_detection_fr_radar);
   Det_Data.push_back(&sil_input_buffer->sym_detection_rl_radar);
   Det_Data.push_back(&sil_input_buffer->sym_detection_rr_radar);
   Det_Data.push_back(&sil_input_buffer->sym_detection_fc_radar);

   mounting_values.push_back(&sil_input_buffer->Mounting_Values_FL);
   mounting_values.push_back(&sil_input_buffer->Mounting_Values_FR);
   mounting_values.push_back(&sil_input_buffer->Mounting_Values_RL);
   mounting_values.push_back(&sil_input_buffer->Mounting_Values_RR);
   mounting_values.push_back(&sil_input_buffer->Mounting_Values_FC);

   xtrk_flag = XTRKConfig;
}

void RunTracker() {
   /*timestamp*/
   static double previous_timestamp = 0;
   static double seconds            = 0;
   double timestamp_us              = 0;
#ifdef SRR_DC
   // Prefer a detection buffer that has a non-zero ScanIndex (some variants
   // populate rear sensors only and DC_SENSOR==0 may be empty).
   // DetectionObj_Data_Input_T *det = nullptr;
   // for (size_t k = 0; k < Det_Data.size(); ++k) {
   //    if (Det_Data[k] && Det_Data[k]->dets_info.ScanIndex != 0) {
   //       det = Det_Data[k];
   //       break;
   //    }
   // }
   DetectionObj_Data_Input_T *det = Det_Data[3]; // temp fix for R8.1.4 emblib
#elif MRR_DC
   DetectionObj_Data_Input_T *det = Det_Data[DC_SENSOR];
#endif

   /*Logic add to prevent det->dets_info.timestamp overrun since the det->dets_info.timestamp goes from 0-999ms and then repeat */
   if (previous_timestamp != 0 && previous_timestamp > det->dets_info.timestamp) {
      seconds++;
   }
   timestamp_us       = seconds * 1E6 + det->dets_info.timestamp * 1E3;
   previous_timestamp = det->dets_info.timestamp;

   setTrackerTimestamp(timestamp_us);

   if (run_first_time) {
      /* Initialize I/O Structures */
      InitializeHostCalib();

      /* Initialize VSE */
      ptr_core_vse->Initialize(*ptr_host_calib);

      /* Initialize F360 Tracker */
      ptr_f360tracker->Initialize();

      /* Initialize RSPP Calibs */
      rspp_variant_A::RSPP_Initialize();

#ifdef SRR_DC
      /* Initialise OCG */
      OCG_inputs->host.dist_rear_axle_to_vcs_m = ptr_host_calib->dist_rear_axle_to_vcs_m;
      OCG_grid->initialize(*OCG_inputs);
#elif MRR_DC
      SG_geometries->initialize(*SG_internal);
#endif
      if (!XTRK_Path.empty() && xtrk_flag == "ENABLE") {
         ptr_f360tracker->Open_Debug_Info(XTRK_Path.c_str());
      }
      previous_xtrk_file_name = XTRK_Path;
      UpdateMountingInfo();
      CopySensor0CalibrationToInactiveSensors();
      rspp_variant_A::RSPP_Set_Sensor_Calibrations(sensors);

      run_first_time = false;
   } else {
      if (XTRK_Path != previous_xtrk_file_name) {
         if (!XTRK_Path.empty() && xtrk_flag == "ENABLE") {
            ptr_f360tracker->Close_Debug_Info();
            ptr_f360tracker->Open_Debug_Info(XTRK_Path.c_str());
         }
         previous_xtrk_file_name = XTRK_Path;
      }
   }

   UpdateSensorVariables(timestamp_us);
   UpdateSensorDetections();

   /* Set F360 inputs */
   UpdateHostInfo(timestamp_us);

   /* Run VSE Core */
   const uint64_t middle_sensor_timestamp = vse_core::F360_Get_Middle_Sensor_Timestamp(reinterpret_cast<f360_variant_A::F360_Radar_Sensor_T(&)[rspp_variant_A::MAX_NUMBER_OF_SENSORS]>(sensors));
   ptr_core_vse->Step(static_cast<uint64_t>(timestamp_us), ptr_host->speed_correction_factor, *ptr_host_raw);
   vse_info = ptr_core_vse->Get_VSE_Output(middle_sensor_timestamp);
   vse_core::F360_Map_VSE_OUT_to_Host_T(vse_info, ptr_host_calib->rear_cornering_compliance, *ptr_host); // Map VSE output to tracker input
   /* update host length and width */
   ptr_host->vehicle_length = ptr_host_calib->vehicle_length_m;
   ptr_host->vehicle_width  = ptr_host_calib->vehicle_width_m;

   /* Update F360 Sensor Motion */
   UpdateSensorMotion();

   /* Update core info data */
   UpdateCoreInfo(timestamp_us);

   Set_RSPP_Host_Info(*ptr_rspp_host, *ptr_host);
   Set_RSPP_Core_Info(*ptr_rspp_core_info, *ptr_core_info);
   rspp_variant_A::RSPP_Process_Detections(sensors, *ptr_detection_list, *ptr_rspp_host, *ptr_rspp_core_info);

#ifdef SRR_DC
   Set_OCG_Host_Info(*OCG_inputs, *ptr_host);
   // execute OCG and get output
   OCG_grid->step(static_cast<double>(sensors[0].variable.timestamp_us) / 1000.0, *OCG_inputs);
   OCG_grid->get_output(*OCG_outputs);
   ptr_SM->execute(*ptr_core_info, *ptr_host, *OCG_outputs, *ptr_detection_list, reinterpret_cast<f360_variant_A::F360_Radar_Sensor_T(&)[rspp_variant_A::MAX_NUMBER_OF_SENSORS]>(sensors), *ptr_tracker_info_log, *ptr_obj_log);
#elif MRR_DC
   ptr_SM->execute(*ptr_core_info, *ptr_host, sg_output, *ptr_detection_list, reinterpret_cast<f360_variant_A::F360_Radar_Sensor_T(&)[rspp_variant_A::MAX_NUMBER_OF_SENSORS]>(sensors), *ptr_tracker_info_log, *ptr_obj_log);
#endif
   ptr_SM->Log_Functional_Safety_Faults(faults);
   ptr_f360tracker->Log_Tracker_Info(ptr_tracker_info_log);
   ptr_f360tracker->Fill_ROT_Object_Output(*ptr_detection_list, *ptr_host, reinterpret_cast<f360_variant_A::F360_Radar_Sensor_T(&)[rspp_variant_A::MAX_NUMBER_OF_SENSORS]>(sensors), faults, rot_obj);
   ptr_f360tracker->Log_Detections(&log_dets, *ptr_detection_list);
   ptr_f360tracker->Log_Sensor_Calibs(*ptr_sensor_calib, reinterpret_cast<f360_variant_A::F360_Radar_Sensor_T(&)[rspp_variant_A::MAX_NUMBER_OF_SENSORS]>(sensors));

#ifdef MRR_DC
   // execute sg and get output
   SG_geometries->step(sensors[0].variable.timestamp_us, log_dets, *ptr_detection_list, sensors, *ptr_rspp_host);
   SG_geometries->get_output(*sg_output);
#endif
   ptr_f360tracker->Write_Debug_Info(ptr_host_calib, ptr_host_raw, ptr_host, ptr_detection_list, reinterpret_cast<f360_variant_A::F360_Radar_Sensor_T(&)[rspp_variant_A::MAX_NUMBER_OF_SENSORS]>(sensors), OCG_outputs, sg_output);
}

void setTrackerTimestamp(double timestamp_us) {
   tracker_timestamp_us = timestamp_us;
}

double GetTrackerTimestamp() {
   return tracker_timestamp_us;
}

void SetXTRKFiles(const char *OpFilepath) {
   if (OpFilepath == nullptr) {
      std::cerr << "Error: filename is nullptr" << std::endl;
   }

   std::string file_name(OpFilepath);

   auto lastSlashIndex  = file_name.find_last_of("/\\");
   std::string log_name = file_name.substr(lastSlashIndex + 1);

   size_t dotIndex = log_name.find_last_of(".");
   if (dotIndex != std::string::npos) {
      log_name = log_name.substr(0, dotIndex);
   }

   std::string path_name = file_name.substr(0, lastSlashIndex);
   path_name += "/XTRK";

   fs::path dir_path(path_name);
   if (!fs::exists(dir_path)) {
      fs::create_directories(dir_path);
   }

   std::string absolute_log_name = path_name + "/" + log_name + "_XTRK";
   fs::path log_path(absolute_log_name);

   if (fs::exists(log_path)) {
      fs::remove(log_path);
   }

   XTRK_Path = absolute_log_name;
}

void TrackerReset() {
   if (ptr_SM) {
      delete ptr_SM;
      ptr_SM = nullptr;
   }
   // if (ptr_SCL) { delete ptr_SCL; ptr_SCL = nullptr; }
   if (ptr_f360tracker) {
      delete ptr_f360tracker;
      ptr_f360tracker         = nullptr;
      run_first_time          = true;
      previous_xtrk_file_name = "";
   }
   if (ptr_host_calib) {
      delete ptr_host_calib;
      ptr_host_calib = nullptr;
   }
   if (ptr_sensor_calib) {
      delete ptr_sensor_calib;
      ptr_sensor_calib = nullptr;
   }
   if (ptr_core_vse) {
      ptr_core_vse = nullptr; // core_vse is a static object, do not delete
   }
   if (sg_output) {
      delete sg_output;
      sg_output = nullptr;
   }
   if (SG_geometries) {
      delete SG_geometries;
      SG_geometries = nullptr;
   }
   if (SG_internal) {
      delete SG_internal;
      SG_internal = nullptr;
   }
   if (OCG_inputs) {
      delete OCG_inputs;
      OCG_inputs = nullptr;
   }
   if (OCG_outputs) {
      delete OCG_outputs;
      OCG_outputs = nullptr;
   }
   if (OCG_grid) {
      delete OCG_grid;
      OCG_grid = nullptr;
   }
   if (ptr_input_diag) {
      delete ptr_input_diag;
      ptr_input_diag = nullptr;
   }
   if (ptr_output_diag) {
      delete ptr_output_diag;
      ptr_output_diag = nullptr;
   }
   if (ptr_host) {
      delete ptr_host;
      ptr_host = nullptr;
   }
   if (ptr_host_raw) {
      delete ptr_host_raw;
      ptr_host_raw = nullptr;
   }
   if (ptr_detection_list) {
      delete ptr_detection_list;
      ptr_detection_list = nullptr;
   }
   if (ptr_core_info) {
      delete ptr_core_info;
      ptr_core_info = nullptr;
   }
   if (ptr_obj_log) {
      delete ptr_obj_log;
      ptr_obj_log = nullptr;
   }
   if (ptr_tracker_info_log) {
      delete ptr_tracker_info_log;
      ptr_tracker_info_log = nullptr;
   }
   if (ptr_rspp_core_info) {
      delete ptr_rspp_core_info;
      ptr_rspp_core_info = nullptr;
   }
   if (ptr_rspp_host) {
      delete ptr_rspp_host;
      ptr_rspp_host = nullptr;
   }
}
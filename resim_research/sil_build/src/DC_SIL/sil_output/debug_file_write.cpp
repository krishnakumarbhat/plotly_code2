#include "AS_bin_writer_wrapper.h"
#include "dc_read_config.h"
#include "debug_file_write.h"
#include "DCMacros.h"
#include "HDF_Trace.h"
#include "DGPS_Decoder.h"
#include "Event_Logger.h"
#include "sil_output.h"

/* HDF Write logic irrespective of API's */
static std::string current_hdf_file_name  = "";
static std::string previous_hdf_file_name = "";

static std::string current_hdfIn_file_name  = "";
static std::string previous_hdfIn_file_name = "";

/* Names of Detection bins, radar_param bins */
static const char *AS_bww_Tracker_Detections_OutputString  = "Detections";
static const char *AS_bww_Tracker_RadarParams_OutputString = "RadarParams";

uint32_t GetSensorTimestampFraction(unsigned64_T fraction) {
   unsigned int timestamp;
   static unsigned int temp_time_stamp_prev = 0;
   static unsigned int temp_time_stamp_base = 0;
   unsigned int temp_time_stamp_max         = 0;

   unsigned32_T sensor_timestamp_ms;
   sensor_timestamp_ms = ((fraction) % 1000);

   if (sensor_timestamp_ms < temp_time_stamp_prev) {
      temp_time_stamp_max = (temp_time_stamp_prev + 40);
      temp_time_stamp_base += temp_time_stamp_max;
      timestamp = temp_time_stamp_base;
      temp_time_stamp_base -= sensor_timestamp_ms;
   } else
      timestamp = temp_time_stamp_base + sensor_timestamp_ms;
   temp_time_stamp_prev = sensor_timestamp_ms;
   return timestamp;
}

void InitDebugFiles(Func_Call_LogFile_T LogFile) {
   switch (LogFile) {
   case Func_Call_LogFile_T::HDF_FILE_LOG: {
      std::string HDFConfig = getConfigParameters().HDF_Files_Output;
      if (HDFConfig == "ENABLE") {
         HDFWriteClass *hdf_write_instance = HDFWriteClass::getInstance();
         hdf_write_instance->Init();
      }
      std::string HDFConfigIn = getConfigParameters().HDF_Files_Input;
      if (HDFConfigIn == "ENABLE") {
         HDFWriteClass *hdf_write_instanceIn = HDFWriteClass::getInstance();
         hdf_write_instanceIn->Init();
      }
      break;
   }
   case Func_Call_LogFile_T::BIN_FILE_LOG: {
      std::string BINConfig = getConfigParameters().BIN_Files;
      if (BINConfig == "DISABLE") {
         AS_BWW_SUPPRESS_ALL_BIN_FILES(true);
      }
      break;
   }
   }
}

void WriteDebugFiles(Func_Call_LogFile_T LogFile) {
   switch (LogFile) {
   case Func_Call_LogFile_T::HDF_FILE_LOG: {
      std::string HDFConfig = getConfigParameters().HDF_Files_Output;
      if (HDFConfig == "ENABLE") {
         HDFWriteClass *hdf_write_instance = HDFWriteClass::getInstance();
         hdf_write_instance->UpdateHDFbuffers();
      }
      std::string HDFConfigIn = getConfigParameters().HDF_Files_Input;
      if (HDFConfigIn == "ENABLE") {
         HDFWriteClass *hdf_write_instanceIn = HDFWriteClass::getInstance();
         hdf_write_instanceIn->UpdateHDFIpbuffers();
      }
      break;
   }
   case Func_Call_LogFile_T::STATISTIC_FILE_LOG: {
      Run_Dia_Statistic();
      break;
   }
   case Func_Call_LogFile_T::BIN_FILE_LOG: {
      std::string BINConfig = getConfigParameters().BIN_Files;
      if (BINConfig == "ENABLE") {
         AS_BWW_SET_MOUNTING_RP_MAC(AS_BWW_MOUNTING_DOMAIN_CONTROLLER_UNIT);
         uint64_t timestamp_us  = static_cast<uint64_t>(GetTrackerTimestamp()); // explicit cast to silence conversion warning
         uint32_t timestamp_bin = GetSensorTimestampFraction(timestamp_us / 1000);
         AS_BWW_SET_TIME_STAMP_MAC(timestamp_bin);
         AS_BWW_SET_SCAN_INDEX_MAC(GetScanIndex());
         AS_BWW_WRITE_LINE_MAC(timestamp_bin, GetScanIndex(), AS_BWW_MOUNTING_DOMAIN_CONTROLLER_UNIT);
         // write processed_dets bin, radar_params bin
         writeF360Bin();
      }
      break;
   }
   }
}

void ResetDebugFiles(Func_Call_LogFile_T LogFile, void *pLogFolder, bool endFile) {
   switch (LogFile) {
   case Func_Call_LogFile_T::HDF_FILE_LOG: {
      std::string HDFConfig = getConfigParameters().HDF_Files_Output;
      if (HDFConfig == "ENABLE") {
         HDFWriteClass *hdf_write_instance = HDFWriteClass::getInstance();
         if (current_hdf_file_name != previous_hdf_file_name) {
            hdf_write_instance->HDF_Write();
            hdf_write_instance->Reset_HDF_Buffers();
            previous_hdf_file_name = current_hdf_file_name;
         }
      }
      std::string HDFConfigIn = getConfigParameters().HDF_Files_Input;
      if (HDFConfigIn == "ENABLE") {
         HDFWriteClass *hdf_write_instanceIn = HDFWriteClass::getInstance();
         if (current_hdfIn_file_name != previous_hdfIn_file_name) {
            hdf_write_instanceIn->HDF_IpWrite();
            hdf_write_instanceIn->Reset_AllObjGtPtr_Info();
            hdf_write_instanceIn->Reset_IpScanIdx_Info();
            previous_hdfIn_file_name = current_hdfIn_file_name;
         }
      }
      break;
   }
   case Func_Call_LogFile_T::STATISTIC_FILE_LOG: {
      Write_statistic_Data(pLogFolder, endFile);
      Reset_Statstic_data();
      break;
   }
   }
}

void SetPathDebugFiles(Func_Call_LogFile_T LogFile, const char *OpFilepath) {
   switch (LogFile) {
   case Func_Call_LogFile_T::HDF_FILE_LOG: {
      std::string HDFConfig = getConfigParameters().HDF_Files_Output;
      if (HDFConfig == "ENABLE" && OpFilepath != nullptr) {
         HDFWriteClass *hdf_write_instance = HDFWriteClass::getInstance();
         hdf_write_instance->Set_HDF_Output_Path(OpFilepath);
         current_hdf_file_name = hdf_write_instance->Get_HDF_Output_Path();
      }
      std::string HDFConfigIn = getConfigParameters().HDF_Files_Input;
      if (HDFConfigIn == "ENABLE" && OpFilepath != nullptr) {
         HDFWriteClass *hdf_write_instanceIn = HDFWriteClass::getInstance();
         hdf_write_instanceIn->Set_HDF_Input_Path(OpFilepath);
         current_hdfIn_file_name = hdf_write_instanceIn->Get_HDF_Input_Path();
      }
      break;
   }
   case Func_Call_LogFile_T::BIN_FILE_LOG: {
      std::string BINConfig = getConfigParameters().BIN_Files;
      if (BINConfig == "ENABLE" && OpFilepath != nullptr) {
         AS_BWW_CLOSE_FILE_MAC(AS_BWW_MOUNTING_DOMAIN_CONTROLLER_UNIT);
         AS_BWW_INIT_MAC(OpFilepath);
      }
      break;
   }
   case Func_Call_LogFile_T::DGPS_FILE_LOG: {
      std::string DGPSConfig = getConfigParameters().DGPS_Decode_Status;
      if (DGPSConfig == "ENABLE") {
         Set_DGPS_Output_Path(OpFilepath);
      }
      break;
   }
   }
}

/* Detection bins, Radarparams bins */
void writeF360Bin() {
   rspp_variant_A::RSPP_Detection_List_T *ptr_detection_list = GetDetectionList();
   rspp_variant_A::F360_Radar_Sensor_T *sensors              = GetF360SensorInfo();
   if (ptr_detection_list != nullptr) {
      for (uint32_t i = 0; i < MAX_DC_SENSORS * 200; i++) {
         int32_t det_id = ptr_detection_list->detections[i].raw.det_id;
         int32_t sen_id = 0;
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "f_valid", ptr_detection_list->detections[i].processed.f_ok_to_use, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "id", det_id, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "amplitude", ptr_detection_list->detections[i].raw.rcs, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "azimuth", ptr_detection_list->detections[i].raw.azimuth, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "azimuth_vcs", ptr_detection_list->detections[i].processed.vcs_az, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "elevation", ptr_detection_list->detections[i].raw.elevation, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "elevation_vcs", ptr_detection_list->detections[i].processed.vcs_el, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "confid_azimuth", ptr_detection_list->detections[i].raw.confid_azimuth, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "confid_elevation", ptr_detection_list->detections[i].raw.confid_elevation, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "range", ptr_detection_list->detections[i].raw.range, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "range_rate", ptr_detection_list->detections[i].raw.range_rate, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "range_rate_comp", ptr_detection_list->detections[i].processed.range_rate_compensated, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "x_vcs", ptr_detection_list->detections[i].processed.vcs_position_x, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "y_vcs", ptr_detection_list->detections[i].processed.vcs_position_y, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "motion_status", ptr_detection_list->detections[i].processed.motion_status, i, sen_id);
         STORE_ARRAY_ELEM_MGR_WPR_INDEXED(AS_bww_Tracker_Detections_OutputString, "valid_level", ((i < ptr_detection_list->number_of_valid_detections) ? 1 : 0), i, sen_id);
      }
   }
   for (uint8_t i = 0; i < MAX_DC_SENSORS; i++) {
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "id", i + 1, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "f_active", 1, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "mount_loc", sensors[i].constant.mounting_location, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "alignment", 0, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "alignment_quality_factor", 0, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "arm_throughput", 0, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "azimuth_polarity", sensors[i].constant.polarity, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "boresight_angle", sensors[i].constant.mounting_position.vcs_boresight_azimuth_angle, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "boresight_elevation_angle", sensors[i].constant.mounting_position.vcs_boresight_elevation_angle, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "long_posn", sensors[i].constant.mounting_position.vcs_position.longitudinal, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "lat_posn", sensors[i].constant.mounting_position.vcs_position.lateral, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "height_offset", sensors[i].constant.mounting_position.vcs_position.height, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "long_vel", sensors[i].variable.vcs_velocity.longitudinal, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "lat_vel", sensors[i].variable.vcs_velocity.lateral, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "look_id", sensors[i].variable.look_id, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "look_index", sensors[i].variable.look_index, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "max_range_current_look", sensors[i].constant.range_limits[sensors[i].variable.look_id], i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "max_az_non_ex_zone", 1.308997, i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "fov_minaz_d", sensors[i].constant.fov_min_az_rad[sensors[i].variable.look_id], i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "fov_maxaz_d", sensors[i].constant.fov_max_az_rad[sensors[i].variable.look_id], i);
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "fov_minaz_nx", 0, i); /* Stubbed */
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "fov_minaz_ny", 0, i); /* Stubbed */
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "fov_maxaz_nx", 0, i); /* Stubbed */
      STORE_VAL_MGR_WPR_INDEXED(AS_bww_Tracker_RadarParams_OutputString, "fov_maxaz_ny", 0, i); /* Stubbed */
   }
}
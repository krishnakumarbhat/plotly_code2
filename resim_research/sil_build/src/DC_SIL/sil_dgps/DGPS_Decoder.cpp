#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "SMValidationInterface.h"
#include "DGPS_Decoder.h"
#include "dc_read_config.h"
#include "sil_input.h"
#include <cstring>
#include <fstream>
#include <string>
#include <filesystem>
#ifdef _WIN32
#include <direct.h> // for _mkdir
#else
#include <dlfcn.h> // for RTLD_LOCAL | RTLD_DEEPBIND
#endif
#ifdef __GNUC__
#include <sys/stat.h>
#endif
namespace fs = std::filesystem;

// SRR/MRR library isolation is achieved via distinct SONAME in libSMValidation_MRR.so.

// SRR instance (4 sensors: FL, FR, RL, RR)
static DgpsFunction_T g_dgpsContainerSRR;
static SMValidationInputConfig_T *input_config_SRR = nullptr;
static SMValidationRunConfig_T *run_config_SRR     = nullptr;
static SMValidationOutput_T *output_SRR            = nullptr;
static boolean_T FirstExecSRR                      = true;
static unsigned int zero_timestamp_count_SRR       = 0;

// MRR instance (1 sensor: FC)
static DgpsFunction_T g_dgpsContainerMRR;
static SMValidationInputConfig_T *input_config_MRR = nullptr;
static SMValidationRunConfig_T *run_config_MRR     = nullptr;
static SMValidationOutput_T *output_MRR            = nullptr;
static boolean_T FirstExecMRR                      = true;
static unsigned int zero_timestamp_count_MRR       = 0;

static std::ofstream txt_file;
static char FilePathSRR[SMVALIDATION_MAX_PATH];
static char FilePathMRR[SMVALIDATION_MAX_PATH];

DGPS_Data_T gen7_DGPS_object_SRR;
DGPS_Data_T gen7_DGPS_object_MRR;

static void loadDgpsFunctions(DgpsFunction_T &container, const std::string &libPath) {
   container.handler = SMValidation_LoadLibrary(libPath.c_str());
   if (container.handler) {
      container.dgps_init  = (DGPSinit)SMValidation_GetProcAddress(container.handler, "SMValidationInit");
      container.dgps_run   = (DGPSRun)SMValidation_GetProcAddress(container.handler, "SMValidationRun");
      container.dgps_exit  = (DGPSExit)SMValidation_GetProcAddress(container.handler, "SMValidationExit");
      container.dgps_reset = (DGPSReset)SMValidation_GetProcAddress(container.handler, "SMValidationReset");
      container.dgps_data  = (DGPSData)SMValidation_GetProcAddress(container.handler, "SMValidationDGPSData");
      if (!(container.dgps_init && container.dgps_run && container.dgps_exit && container.dgps_reset)) {
         std::cout << "[DGPS]: Failed to resolve one or more SMValidation functions from " << libPath << std::endl;
      }
   } else {
      std::cout << "[DGPS]: Unable to load library at " << libPath << std::endl;
   }
}

// Load a library in an isolated namespace so its embedded libprotobuf
// does NOT share the global descriptor registry with any other loaded copy.
// This prevents "duplicate proto registration" / repeated_field crashes
// when two copies of libSMValidation.so are loaded in the same process.
static void *loadIsolated(const std::string &libPath) {
#ifdef _WIN32
   return SMValidation_LoadLibrary(libPath.c_str());
#else
   void *handle = dlopen(libPath.c_str(), RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND);
   if (!handle) {
      std::cout << "[DGPS]: dlopen (isolated) failed for " << libPath << ": " << dlerror() << std::endl;
   }
   return handle;
#endif
}

static void loadDgpsFunctionsIsolated(DgpsFunction_T &container, const std::string &libPath) {
#ifdef _WIN32
   loadDgpsFunctions(container, libPath);
#else
   container.handler = loadIsolated(libPath);
   if (container.handler) {
      container.dgps_init  = (DGPSinit)dlsym(container.handler, "SMValidationInit");
      container.dgps_run   = (DGPSRun)dlsym(container.handler, "SMValidationRun");
      container.dgps_exit  = (DGPSExit)dlsym(container.handler, "SMValidationExit");
      container.dgps_reset = (DGPSReset)dlsym(container.handler, "SMValidationReset");
      container.dgps_data  = (DGPSData)dlsym(container.handler, "SMValidationDGPSData");
      if (!(container.dgps_init && container.dgps_run && container.dgps_exit && container.dgps_reset)) {
         std::cout << "[DGPS]: Failed to resolve SMValidation symbols from " << libPath << std::endl;
      }
   }
#endif
}

void initDgpsLibrary() {
   std::string libPathSRR = std::string(SMVALIDATION_SHARED_LIBRARY);

#if defined(SRR_DC)
   input_config_SRR = new SMValidationInputConfig_T;
   run_config_SRR   = new SMValidationRunConfig_T;
   output_SRR       = new SMValidationOutput_T;
   loadDgpsFunctions(g_dgpsContainerSRR, libPathSRR);
   std::cout << "[DGPS]: SRR instance loaded from " << libPathSRR << std::endl;
#endif

#if defined(MRR_DC)
   // Use a separate physical copy of the .so for MRR to isolate protobuf symbols.
   // Define SMVALIDATION_SHARED_LIBRARY_MRR in CMake pointing to the copy; falls back to same .so if not set.
#if defined(SMVALIDATION_SHARED_LIBRARY_MRR)
   std::string libPathMRR = std::string(SMVALIDATION_SHARED_LIBRARY_MRR);
#else
   std::string libPathMRR = libPathSRR; // fallback: same file, isolated via RTLD_DEEPBIND
#endif
   input_config_MRR = new SMValidationInputConfig_T;
   run_config_MRR   = new SMValidationRunConfig_T;
   output_MRR       = new SMValidationOutput_T;
   loadDgpsFunctionsIsolated(g_dgpsContainerMRR, libPathMRR);
   std::cout << "[DGPS]: MRR instance loaded (isolated) from " << libPathMRR << std::endl;
#endif
}

void updateInputConfig() {
   DC_INPUT_DATA_T *ptr_dc_input_data = GetDCInputdata();

#if defined(SRR_DC)
   {
      memset(input_config_SRR, 0, sizeof(SMValidationInputConfig_T));
      input_config_SRR->header.version_major          = OUTPUT_VERSION_MAJOR;
      input_config_SRR->header.version_minor          = OUTPUT_VERSION_MINOR;
      input_config_SRR->header.version_patch          = OUTPUT_VERSION_PATCH;
      input_config_SRR->header.size                   = sizeof(SMValidationInputConfig_T);
      input_config_SRR->customer                      = SMVALIDATION_CUSTOMER_CEER;
      input_config_SRR->bus_type                      = DGPS_ETHERNET;
      input_config_SRR->decode_ncom_packets_from_rcom = true;
      input_config_SRR->number_of_sensors             = 4;

      // converting VCS to OSI
      input_config_SRR->sensor_info[0].mounting_pose.osi_position_x                       = (ptr_dc_input_data->Mounting_Values_FL.vcs_lon_position + (ptr_dc_input_data->vehicle_inputs.host_vehicle_length / 2));
      input_config_SRR->sensor_info[0].mounting_pose.osi_position_y                       = -ptr_dc_input_data->Mounting_Values_FL.vcs_lat_position;
      input_config_SRR->sensor_info[0].mounting_pose.osi_position_z                       = ptr_dc_input_data->Mounting_Values_FL.vcs_z_position;
      input_config_SRR->sensor_info[0].mounting_pose.osi_orientation_pitch                = 0;
      input_config_SRR->sensor_info[0].mounting_pose.osi_orientation_roll                 = 0;
      input_config_SRR->sensor_info[0].mounting_pose.osi_orientation_yaw                  = -ptr_dc_input_data->Mounting_Values_FL.boresight_angle;
      input_config_SRR->sensor_info[0].sensor_id                                          = SMVALIDATION_FRONT_LEFT;
      input_config_SRR->sensor_info[0].sensortype                                         = SMVALIDATION_SENSOR_TYPE_SRR7_PLUS;
      input_config_SRR->sensor_info[0].radar_alignment_angle.vcs_boresight_az_align_angle = 0;
      input_config_SRR->sensor_info[0].radar_alignment_angle.vcs_boresight_el_align_angle = 0;

      // converting VCS to OSI
      input_config_SRR->sensor_info[1].mounting_pose.osi_position_x                       = (ptr_dc_input_data->Mounting_Values_FR.vcs_lon_position + (ptr_dc_input_data->vehicle_inputs.host_vehicle_length / 2));
      input_config_SRR->sensor_info[1].mounting_pose.osi_position_y                       = -ptr_dc_input_data->Mounting_Values_FR.vcs_lat_position;
      input_config_SRR->sensor_info[1].mounting_pose.osi_position_z                       = ptr_dc_input_data->Mounting_Values_FR.vcs_z_position;
      input_config_SRR->sensor_info[1].mounting_pose.osi_orientation_pitch                = 0;
      input_config_SRR->sensor_info[1].mounting_pose.osi_orientation_roll                 = 3.14;
      input_config_SRR->sensor_info[1].mounting_pose.osi_orientation_yaw                  = -ptr_dc_input_data->Mounting_Values_FR.boresight_angle;
      input_config_SRR->sensor_info[1].sensor_id                                          = SMVALIDATION_FRONT_RIGHT;
      input_config_SRR->sensor_info[1].sensortype                                         = SMVALIDATION_SENSOR_TYPE_SRR7_PLUS;
      input_config_SRR->sensor_info[1].radar_alignment_angle.vcs_boresight_az_align_angle = 0;
      input_config_SRR->sensor_info[1].radar_alignment_angle.vcs_boresight_el_align_angle = 0;

      // converting VCS to OSI
      input_config_SRR->sensor_info[2].mounting_pose.osi_position_x                       = (ptr_dc_input_data->Mounting_Values_RL.vcs_lon_position + (ptr_dc_input_data->vehicle_inputs.host_vehicle_length / 2));
      input_config_SRR->sensor_info[2].mounting_pose.osi_position_y                       = -ptr_dc_input_data->Mounting_Values_RL.vcs_lat_position;
      input_config_SRR->sensor_info[2].mounting_pose.osi_position_z                       = ptr_dc_input_data->Mounting_Values_RL.vcs_z_position;
      input_config_SRR->sensor_info[2].mounting_pose.osi_orientation_pitch                = 0;
      input_config_SRR->sensor_info[2].mounting_pose.osi_orientation_roll                 = 3.14;
      input_config_SRR->sensor_info[2].mounting_pose.osi_orientation_yaw                  = -ptr_dc_input_data->Mounting_Values_RL.boresight_angle;
      input_config_SRR->sensor_info[2].sensor_id                                          = SMVALIDATION_REAR_LEFT;
      input_config_SRR->sensor_info[2].sensortype                                         = SMVALIDATION_SENSOR_TYPE_SRR7_PLUS;
      input_config_SRR->sensor_info[2].radar_alignment_angle.vcs_boresight_az_align_angle = 0;
      input_config_SRR->sensor_info[2].radar_alignment_angle.vcs_boresight_el_align_angle = 0;

      // converting VCS to OSI
      input_config_SRR->sensor_info[3].mounting_pose.osi_position_x                       = (ptr_dc_input_data->Mounting_Values_RR.vcs_lon_position + (ptr_dc_input_data->vehicle_inputs.host_vehicle_length / 2));
      input_config_SRR->sensor_info[3].mounting_pose.osi_position_y                       = -ptr_dc_input_data->Mounting_Values_RR.vcs_lat_position;
      input_config_SRR->sensor_info[3].mounting_pose.osi_position_z                       = ptr_dc_input_data->Mounting_Values_RR.vcs_z_position;
      input_config_SRR->sensor_info[3].mounting_pose.osi_orientation_pitch                = 0;
      input_config_SRR->sensor_info[3].mounting_pose.osi_orientation_roll                 = 0;
      input_config_SRR->sensor_info[3].mounting_pose.osi_orientation_yaw                  = -ptr_dc_input_data->Mounting_Values_RR.boresight_angle;
      input_config_SRR->sensor_info[3].sensor_id                                          = SMVALIDATION_REAR_RIGHT;
      input_config_SRR->sensor_info[3].sensortype                                         = SMVALIDATION_SENSOR_TYPE_SRR7_PLUS;
      input_config_SRR->sensor_info[3].radar_alignment_angle.vcs_boresight_az_align_angle = 0;
      input_config_SRR->sensor_info[3].radar_alignment_angle.vcs_boresight_el_align_angle = 0;

      strncpy(input_config_SRR->output_path, FilePathSRR, SMVALIDATION_MAX_PATH - 1);
      input_config_SRR->output_path[SMVALIDATION_MAX_PATH - 1] = '\0';
      strncpy(input_config_SRR->sm_config_path, getConfigParameters().DGPS_SM_Config.c_str(), SMVALIDATION_MAX_PATH - 1);
      input_config_SRR->sm_config_path[SMVALIDATION_MAX_PATH - 1] = '\0';
      strncpy(input_config_SRR->binary_path, "./", SMVALIDATION_MAX_PATH - 1);
      input_config_SRR->binary_path[SMVALIDATION_MAX_PATH - 1] = '\0';
   }
#endif

#if defined(MRR_DC)
   {
      memset(input_config_MRR, 0, sizeof(SMValidationInputConfig_T));
      input_config_MRR->header.version_major          = OUTPUT_VERSION_MAJOR;
      input_config_MRR->header.version_minor          = OUTPUT_VERSION_MINOR;
      input_config_MRR->header.version_patch          = OUTPUT_VERSION_PATCH;
      input_config_MRR->header.size                   = sizeof(SMValidationInputConfig_T);
      input_config_MRR->customer                      = SMVALIDATION_CUSTOMER_CEER;
      input_config_MRR->bus_type                      = DGPS_ETHERNET;
      input_config_MRR->decode_ncom_packets_from_rcom = true;
      input_config_MRR->number_of_sensors             = 1;

      input_config_MRR->sensor_info[0].mounting_pose.osi_position_x                       = (ptr_dc_input_data->Mounting_Values_FC.vcs_lon_position + (ptr_dc_input_data->vehicle_inputs.host_vehicle_length / 2));
      input_config_MRR->sensor_info[0].mounting_pose.osi_position_y                       = -ptr_dc_input_data->Mounting_Values_FC.vcs_lat_position;
      input_config_MRR->sensor_info[0].mounting_pose.osi_position_z                       = ptr_dc_input_data->Mounting_Values_FC.vcs_z_position;
      input_config_MRR->sensor_info[0].mounting_pose.osi_orientation_pitch                = 0;
      input_config_MRR->sensor_info[0].mounting_pose.osi_orientation_roll                 = 0;
      input_config_MRR->sensor_info[0].mounting_pose.osi_orientation_yaw                  = -ptr_dc_input_data->Mounting_Values_FC.boresight_angle;
      input_config_MRR->sensor_info[0].sensor_id                                          = SMVALIDATION_FRONT_CENTER;
      input_config_MRR->sensor_info[0].sensortype                                         = SMVALIDATION_SENSOR_TYPE_FLR7;
      input_config_MRR->sensor_info[0].radar_alignment_angle.vcs_boresight_az_align_angle = 0;
      input_config_MRR->sensor_info[0].radar_alignment_angle.vcs_boresight_el_align_angle = 0;

      strncpy(input_config_MRR->output_path, FilePathMRR, SMVALIDATION_MAX_PATH - 1);
      input_config_MRR->output_path[SMVALIDATION_MAX_PATH - 1] = '\0';
      strncpy(input_config_MRR->sm_config_path, getConfigParameters().DGPS_SM_Config.c_str(), SMVALIDATION_MAX_PATH - 1);
      input_config_MRR->sm_config_path[SMVALIDATION_MAX_PATH - 1] = '\0';
      strncpy(input_config_MRR->binary_path, "./", SMVALIDATION_MAX_PATH - 1);
      input_config_MRR->binary_path[SMVALIDATION_MAX_PATH - 1] = '\0';
   }
#endif
}

bool updateRunConfig() {

   DC_INPUT_DATA_T *ptr_dc_input_data                      = GetDCInputdata();
   static char previous_mdffile_SRR[SMVALIDATION_MAX_PATH] = {0};
   static char previous_mdffile_MRR[SMVALIDATION_MAX_PATH] = {0};

#if defined(SRR_DC)
   {
      {

         if (FirstExecSRR) {
            std::cout << "[DGPS SRR]: First execution, initializing." << std::endl;
            updateInputConfig();
            g_dgpsContainerSRR.dgps_init(input_config_SRR);
            std::cout << "[DGPS SRR]: Initialized successfully." << std::endl;
            FirstExecSRR = false;
         } else {
            strncpy(input_config_SRR->output_path, FilePathSRR, SMVALIDATION_MAX_PATH - 1);
            input_config_SRR->output_path[SMVALIDATION_MAX_PATH - 1] = '\0';
         }
      }
      memset(run_config_SRR, 0, sizeof(SMValidationRunConfig_T));
      strncpy(run_config_SRR->next_mdffile, ptr_dc_input_data->fPath_SRR_Ref_Next, sizeof(run_config_SRR->next_mdffile) - 1);
      run_config_SRR->next_mdffile[sizeof(run_config_SRR->next_mdffile) - 1] = '\0';
      strncpy(run_config_SRR->current_mdffile, ptr_dc_input_data->fPath_SRR_Ref, sizeof(run_config_SRR->current_mdffile) - 1);
      run_config_SRR->current_mdffile[sizeof(run_config_SRR->current_mdffile) - 1] = '\0';

      // Reset zero timestamp counter if processing a new file
      if (std::strcmp(run_config_SRR->current_mdffile, previous_mdffile_SRR) != 0) {
         zero_timestamp_count_SRR = 0;
         std::strncpy(previous_mdffile_SRR, run_config_SRR->current_mdffile, sizeof(previous_mdffile_SRR) - 1);
         previous_mdffile_SRR[sizeof(previous_mdffile_SRR) - 1] = '\0';
         std::cout << "[DGPS SRR]: New file detected, resetting zero timestamp counter." << std::endl;
      }

      run_config_SRR->look_id[0] = ptr_dc_input_data->sym_detection_fl_radar.dets_info.LookType;
      run_config_SRR->look_id[1] = ptr_dc_input_data->sym_detection_fr_radar.dets_info.LookType;
      run_config_SRR->look_id[2] = ptr_dc_input_data->sym_detection_rl_radar.dets_info.LookType;
      run_config_SRR->look_id[3] = ptr_dc_input_data->sym_detection_rr_radar.dets_info.LookType;

      run_config_SRR->timestamp[0].seconds              = 0;
      run_config_SRR->timestamp[0].fractional_seconds   = static_cast<uint64_t>(ptr_dc_input_data->sym_detection_fl_radar.dets_info.timestamp);
      run_config_SRR->filetimestamp[0].abs_timeStamp_ns = ptr_dc_input_data->sim_cycle_start_time_info[0].abs_timeStamp_ns;

      run_config_SRR->timestamp[1].seconds              = 0;
      run_config_SRR->timestamp[1].fractional_seconds   = static_cast<uint64_t>(ptr_dc_input_data->sym_detection_fr_radar.dets_info.timestamp);
      run_config_SRR->filetimestamp[1].abs_timeStamp_ns = ptr_dc_input_data->sim_cycle_start_time_info[1].abs_timeStamp_ns;

      run_config_SRR->timestamp[2].seconds              = 0;
      run_config_SRR->timestamp[2].fractional_seconds   = static_cast<uint64_t>(ptr_dc_input_data->sym_detection_rl_radar.dets_info.timestamp);
      run_config_SRR->filetimestamp[2].abs_timeStamp_ns = ptr_dc_input_data->sim_cycle_start_time_info[2].abs_timeStamp_ns;

      run_config_SRR->timestamp[3].seconds              = 0;
      run_config_SRR->timestamp[3].fractional_seconds   = static_cast<uint64_t>(ptr_dc_input_data->sym_detection_rr_radar.dets_info.timestamp);
      run_config_SRR->filetimestamp[3].abs_timeStamp_ns = ptr_dc_input_data->sim_cycle_start_time_info[3].abs_timeStamp_ns;

      // Check if any sensor timestamp is zero
      for (int i = 0; i < 4; i++) {
         if (ptr_dc_input_data->sim_cycle_start_time_info[i].abs_timeStamp_ns == 0) {
            zero_timestamp_count_SRR++;
            std::cout << "[DGPS SRR]: Skipping dgps_run - sensor [" << i << "] timestamp is zero. [Count: " << zero_timestamp_count_SRR << "]" << std::endl;
            return false;
         }
      }

      if (run_config_SRR->current_mdffile[0] == '\0') {
         std::cout << "[DGPS]: Skipping dgps_run - current_mdffile path is empty." << std::endl;
         return true;
      }
      if (!fs::exists(run_config_SRR->current_mdffile)) {
         std::cout << "[DGPS]: Skipping dgps_run - current_mdffile does not exist: " << run_config_SRR->current_mdffile << std::endl;
         return true;
      }

      // If next_mdffile is provided, verify it exists too
      if (run_config_SRR->next_mdffile[0] != '\0' && !fs::exists(run_config_SRR->next_mdffile)) {
         std::cout << "[DGPS]: Warning - next_mdffile does not exist, clearing it: " << run_config_SRR->next_mdffile << std::endl;
         memset(run_config_SRR->next_mdffile, 0, sizeof(run_config_SRR->next_mdffile));
      }

      g_dgpsContainerSRR.dgps_run(run_config_SRR, output_SRR);
      g_dgpsContainerSRR.dgps_data(&gen7_DGPS_object_SRR);

      std::cout << "SRR API output" << gen7_DGPS_object_SRR.Latitude_Longitude_Host.Poslat << std::endl;
      std::cout << "SRR API output" << gen7_DGPS_object_SRR.Latitude_Longitude_Host.PosLon << std::endl;

      ptr_dc_input_data->sym_detection_fl_radar.dets_info.LookID   = static_cast<unsigned8_T>(output_SRR->detections[0].LookID);
      ptr_dc_input_data->sym_detection_fl_radar.dets_info.LookType = output_SRR->detections[0].LookType;
      ptr_dc_input_data->sym_detection_fl_radar.dets_info.Count    = static_cast<unsigned8_T>(output_SRR->detections[0].num_of_dets);

      ptr_dc_input_data->sym_detection_fr_radar.dets_info.LookID   = static_cast<unsigned8_T>(output_SRR->detections[1].LookID);
      ptr_dc_input_data->sym_detection_fr_radar.dets_info.LookType = output_SRR->detections[1].LookType;
      ptr_dc_input_data->sym_detection_fr_radar.dets_info.Count    = static_cast<unsigned8_T>(output_SRR->detections[1].num_of_dets);

      ptr_dc_input_data->sym_detection_rl_radar.dets_info.LookID   = static_cast<unsigned8_T>(output_SRR->detections[2].LookID);
      ptr_dc_input_data->sym_detection_rl_radar.dets_info.LookType = output_SRR->detections[2].LookType;
      ptr_dc_input_data->sym_detection_rl_radar.dets_info.Count    = static_cast<unsigned8_T>(output_SRR->detections[2].num_of_dets);

      ptr_dc_input_data->sym_detection_rr_radar.dets_info.LookID   = static_cast<unsigned8_T>(output_SRR->detections[3].LookID);
      ptr_dc_input_data->sym_detection_rr_radar.dets_info.LookType = output_SRR->detections[3].LookType;
      ptr_dc_input_data->sym_detection_rr_radar.dets_info.Count    = static_cast<unsigned8_T>(output_SRR->detections[3].num_of_dets);

      memset(ptr_dc_input_data->sym_detection_fl_radar.dets_input, 0, sizeof(ptr_dc_input_data->sym_detection_fl_radar.dets_input));
      for (int flCnt = 0; flCnt < SMVALIDATION_MAX_NUMBER_OF_DETECTIONS; flCnt++) {
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].amplitude                = static_cast<float32_T>(output_SRR->detections[0].dets_float[flCnt].amplitude);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].snr                      = static_cast<float32_T>(output_SRR->detections[0].dets_float[flCnt].snr);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].range                    = static_cast<float32_T>(output_SRR->detections[0].dets_float[flCnt].range);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].range_rate               = static_cast<float32_T>(output_SRR->detections[0].dets_float[flCnt].range_rate);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].azimuth                  = static_cast<float32_T>(output_SRR->detections[0].dets_float[flCnt].azimuth);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].elevation                = static_cast<float32_T>(output_SRR->detections[0].dets_float[flCnt].elevation);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].azimuth_confidence       = static_cast<unsigned8_T>(output_SRR->detections[0].dets_float[flCnt].azimuth_confidence);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].bistatic                 = static_cast<unsigned8_T>(output_SRR->detections[0].dets_float[flCnt].bistatic);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].elevation_confidence     = static_cast<unsigned8_T>(output_SRR->detections[0].dets_float[flCnt].elevation_confidence);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].existence_probability    = static_cast<float32_T>(output_SRR->detections[0].dets_float[flCnt].existence_probability);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].multi_target_probability = static_cast<float32_T>(output_SRR->detections[0].dets_float[flCnt].multi_target_probability);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].nd_target                = static_cast<unsigned8_T>(output_SRR->detections[0].dets_float[flCnt].nd_target);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].std_azimuth              = static_cast<float32_T>(output_SRR->detections[0].dets_float[flCnt].std_azimuth);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].std_elevation            = static_cast<float32_T>(output_SRR->detections[0].dets_float[flCnt].std_elevation);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].std_range                = static_cast<float32_T>(output_SRR->detections[0].dets_float[flCnt].std_range);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].std_range_rate           = static_cast<float32_T>(output_SRR->detections[0].dets_float[flCnt].std_range_rate);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].std_rcs                  = static_cast<float32_T>(output_SRR->detections[0].dets_float[flCnt].std_rcs);
         ptr_dc_input_data->sym_detection_fl_radar.dets_input[flCnt].valid                    = output_SRR->detections[0].dets_float[flCnt].valid;
      }

      memset(ptr_dc_input_data->sym_detection_fr_radar.dets_input, 0, sizeof(ptr_dc_input_data->sym_detection_fr_radar.dets_input));

      for (int frCnt = 0; frCnt < SMVALIDATION_MAX_NUMBER_OF_DETECTIONS; frCnt++) {
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].amplitude                = static_cast<float32_T>(output_SRR->detections[1].dets_float[frCnt].amplitude);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].snr                      = static_cast<float32_T>(output_SRR->detections[1].dets_float[frCnt].snr);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].range                    = static_cast<float32_T>(output_SRR->detections[1].dets_float[frCnt].range);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].range_rate               = static_cast<float32_T>(output_SRR->detections[1].dets_float[frCnt].range_rate);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].azimuth                  = static_cast<float32_T>(output_SRR->detections[1].dets_float[frCnt].azimuth);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].elevation                = static_cast<float32_T>(output_SRR->detections[1].dets_float[frCnt].elevation);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].azimuth_confidence       = static_cast<unsigned8_T>(output_SRR->detections[1].dets_float[frCnt].azimuth_confidence);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].bistatic                 = static_cast<unsigned8_T>(output_SRR->detections[1].dets_float[frCnt].bistatic);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].elevation_confidence     = static_cast<unsigned8_T>(output_SRR->detections[1].dets_float[frCnt].elevation_confidence);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].existence_probability    = static_cast<float32_T>(output_SRR->detections[1].dets_float[frCnt].existence_probability);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].multi_target_probability = static_cast<float32_T>(output_SRR->detections[1].dets_float[frCnt].multi_target_probability);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].nd_target                = static_cast<unsigned8_T>(output_SRR->detections[1].dets_float[frCnt].nd_target);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].std_azimuth              = static_cast<float32_T>(output_SRR->detections[1].dets_float[frCnt].std_azimuth);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].std_elevation            = static_cast<float32_T>(output_SRR->detections[1].dets_float[frCnt].std_elevation);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].std_range                = static_cast<float32_T>(output_SRR->detections[1].dets_float[frCnt].std_range);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].std_range_rate           = static_cast<float32_T>(output_SRR->detections[1].dets_float[frCnt].std_range_rate);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].std_rcs                  = static_cast<float32_T>(output_SRR->detections[1].dets_float[frCnt].std_rcs);
         ptr_dc_input_data->sym_detection_fr_radar.dets_input[frCnt].valid                    = static_cast<unsigned8_T>(output_SRR->detections[1].dets_float[frCnt].valid);
      }

      memset(ptr_dc_input_data->sym_detection_rl_radar.dets_input, 0, sizeof(ptr_dc_input_data->sym_detection_rl_radar.dets_input));
      for (int rlCnt = 0; rlCnt < SMVALIDATION_MAX_NUMBER_OF_DETECTIONS; rlCnt++) {
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].amplitude                = static_cast<float32_T>(output_SRR->detections[2].dets_float[rlCnt].amplitude);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].snr                      = static_cast<float32_T>(output_SRR->detections[2].dets_float[rlCnt].snr);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].range                    = static_cast<float32_T>(output_SRR->detections[2].dets_float[rlCnt].range);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].range_rate               = static_cast<float32_T>(output_SRR->detections[2].dets_float[rlCnt].range_rate);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].azimuth                  = static_cast<float32_T>(output_SRR->detections[2].dets_float[rlCnt].azimuth);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].elevation                = static_cast<float32_T>(output_SRR->detections[2].dets_float[rlCnt].elevation);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].azimuth_confidence       = static_cast<unsigned8_T>(output_SRR->detections[2].dets_float[rlCnt].azimuth_confidence);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].bistatic                 = static_cast<unsigned8_T>(output_SRR->detections[2].dets_float[rlCnt].bistatic);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].elevation_confidence     = static_cast<unsigned8_T>(output_SRR->detections[2].dets_float[rlCnt].elevation_confidence);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].existence_probability    = static_cast<float32_T>(output_SRR->detections[2].dets_float[rlCnt].existence_probability);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].multi_target_probability = static_cast<float32_T>(output_SRR->detections[2].dets_float[rlCnt].multi_target_probability);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].nd_target                = static_cast<unsigned8_T>(output_SRR->detections[2].dets_float[rlCnt].nd_target);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].std_azimuth              = static_cast<float32_T>(output_SRR->detections[2].dets_float[rlCnt].std_azimuth);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].std_elevation            = static_cast<float32_T>(output_SRR->detections[2].dets_float[rlCnt].std_elevation);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].std_range                = static_cast<float32_T>(output_SRR->detections[2].dets_float[rlCnt].std_range);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].std_range_rate           = static_cast<float32_T>(output_SRR->detections[2].dets_float[rlCnt].std_range_rate);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].std_rcs                  = static_cast<float32_T>(output_SRR->detections[2].dets_float[rlCnt].std_rcs);
         ptr_dc_input_data->sym_detection_rl_radar.dets_input[rlCnt].valid                    = static_cast<unsigned8_T>(output_SRR->detections[2].dets_float[rlCnt].valid);
      }
      memset(ptr_dc_input_data->sym_detection_rr_radar.dets_input, 0, sizeof(ptr_dc_input_data->sym_detection_rr_radar.dets_input));
      for (int rrCnt = 0; rrCnt < SMVALIDATION_MAX_NUMBER_OF_DETECTIONS; rrCnt++) {
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].amplitude                = static_cast<float32_T>(output_SRR->detections[3].dets_float[rrCnt].amplitude);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].snr                      = static_cast<float32_T>(output_SRR->detections[3].dets_float[rrCnt].snr);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].range                    = static_cast<float32_T>(output_SRR->detections[3].dets_float[rrCnt].range);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].range_rate               = static_cast<float32_T>(output_SRR->detections[3].dets_float[rrCnt].range_rate);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].azimuth                  = static_cast<float32_T>(output_SRR->detections[3].dets_float[rrCnt].azimuth);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].elevation                = static_cast<float32_T>(output_SRR->detections[3].dets_float[rrCnt].elevation);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].azimuth_confidence       = static_cast<unsigned8_T>(output_SRR->detections[3].dets_float[rrCnt].azimuth_confidence);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].bistatic                 = static_cast<unsigned8_T>(output_SRR->detections[3].dets_float[rrCnt].bistatic);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].elevation_confidence     = static_cast<unsigned8_T>(output_SRR->detections[3].dets_float[rrCnt].elevation_confidence);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].existence_probability    = static_cast<float32_T>(output_SRR->detections[3].dets_float[rrCnt].existence_probability);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].multi_target_probability = static_cast<float32_T>(output_SRR->detections[3].dets_float[rrCnt].multi_target_probability);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].nd_target                = static_cast<unsigned8_T>(output_SRR->detections[3].dets_float[rrCnt].nd_target);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].std_azimuth              = static_cast<float32_T>(output_SRR->detections[3].dets_float[rrCnt].std_azimuth);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].std_elevation            = static_cast<float32_T>(output_SRR->detections[3].dets_float[rrCnt].std_elevation);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].std_range                = static_cast<float32_T>(output_SRR->detections[3].dets_float[rrCnt].std_range);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].std_range_rate           = static_cast<float32_T>(output_SRR->detections[3].dets_float[rrCnt].std_range_rate);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].std_rcs                  = static_cast<float32_T>(output_SRR->detections[3].dets_float[rrCnt].std_rcs);
         ptr_dc_input_data->sym_detection_rr_radar.dets_input[rrCnt].valid                    = static_cast<unsigned8_T>(output_SRR->detections[3].dets_float[rrCnt].valid);
      }

      // gtCnt to start from 1, as 0th index holds the Host information.
      for (int gtCnt = 1; gtCnt < SMVALIDATION_MAX_NUMBER_OF_TARGET; gtCnt++) {
         int gtCntTrgt                                        = gtCnt - 1;
         ptr_dc_input_data->object_rl[gtCntTrgt].age          = output_SRR->object[gtCnt].age;
         ptr_dc_input_data->object_rl[gtCntTrgt].heading      = static_cast<float>(output_SRR->object[gtCnt].heading);
         ptr_dc_input_data->object_rl[gtCntTrgt].heading_rate = static_cast<float>(output_SRR->object[gtCnt].heading_rate);
         ptr_dc_input_data->object_rl[gtCntTrgt].id           = static_cast<unsigned8_T>(output_SRR->object[gtCnt].id);
         ptr_dc_input_data->object_rl[gtCntTrgt].length       = static_cast<float>(output_SRR->object[gtCnt].length);
         switch (output_SRR->object[gtCnt].object_class) {
         case 1:
            ptr_dc_input_data->object_rl[gtCntTrgt].object_class = DEBUG_OBJECT_CLASS_PEDESTRIAN;
            break;
         case 2:
            ptr_dc_input_data->object_rl[gtCntTrgt].object_class = DEBUG_OBJECT_CLASS_2WHEEL;
            break;
         case 3:
            ptr_dc_input_data->object_rl[gtCntTrgt].object_class = DEBUG_OBJECT_CLASS_CAR;
            break;
         case 4:
            ptr_dc_input_data->object_rl[gtCntTrgt].object_class = DEBUG_OBJECT_CLASS_TRUCK;
            break;
         default:
            ptr_dc_input_data->object_rl[gtCntTrgt].object_class = DEBUG_OBJECT_CLASS_UNKNOWN;
         }

         ptr_dc_input_data->object_rl[gtCntTrgt].speed     = static_cast<float>(output_SRR->object[gtCnt].speed);
         ptr_dc_input_data->object_rl[gtCntTrgt].stage_age = output_SRR->object[gtCnt].stage_age;
         // ptr->object_rl[gtCntTrgt].status           = output->object[gtCnt].status;
         ptr_dc_input_data->object_rl[gtCntTrgt].tangential_accel = static_cast<float>(output_SRR->object[gtCnt].tangential_accel);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_lat_accel    = static_cast<float>(output_SRR->object[gtCnt].vcs_lat_accel);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_lat_posn     = static_cast<float>(output_SRR->object[gtCnt].vcs_lat_posn);

         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_lat_vel      = static_cast<float>(output_SRR->object[gtCnt].vcs_lat_vel);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_lat_vel_rel  = static_cast<float>(output_SRR->object[gtCnt].vcs_lat_vel_rel);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_long_accel   = static_cast<float>(output_SRR->object[gtCnt].vcs_long_accel);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_long_posn    = static_cast<float>(output_SRR->object[gtCnt].vcs_long_posn);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_long_vel     = static_cast<float>(output_SRR->object[gtCnt].vcs_long_vel);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_long_vel_rel = static_cast<float>(output_SRR->object[gtCnt].vcs_long_vel_rel);
         ptr_dc_input_data->object_rl[gtCntTrgt].width            = static_cast<float>(output_SRR->object[gtCnt].width);
      }
   }
#endif

#if defined(MRR_DC)
   {
      {

         if (FirstExecMRR) {
            std::cout << "[DGPS MRR]: First execution, initializing." << std::endl;
            updateInputConfig();
            g_dgpsContainerMRR.dgps_init(input_config_MRR);
            std::cout << "[DGPS MRR]: Initialized successfully." << std::endl;
            FirstExecMRR = false;
         } else {
            strncpy(input_config_MRR->output_path, FilePathMRR, SMVALIDATION_MAX_PATH - 1);
            input_config_MRR->output_path[SMVALIDATION_MAX_PATH - 1] = '\0';
         }
      }
      memset(run_config_MRR, 0, sizeof(SMValidationRunConfig_T));
      strncpy(run_config_MRR->next_mdffile, ptr_dc_input_data->fPath_SRR_Ref_Next, sizeof(run_config_MRR->next_mdffile) - 1);
      run_config_MRR->next_mdffile[sizeof(run_config_MRR->next_mdffile) - 1] = '\0';
      strncpy(run_config_MRR->current_mdffile, ptr_dc_input_data->fPath_SRR_Ref, sizeof(run_config_MRR->current_mdffile) - 1);
      run_config_MRR->current_mdffile[sizeof(run_config_MRR->current_mdffile) - 1] = '\0';

      // Reset zero timestamp counter if processing a new file
      if (std::strcmp(run_config_MRR->current_mdffile, previous_mdffile_MRR) != 0) {
         zero_timestamp_count_MRR = 0;
         std::strncpy(previous_mdffile_MRR, run_config_MRR->current_mdffile, sizeof(previous_mdffile_MRR) - 1);
         previous_mdffile_MRR[sizeof(previous_mdffile_MRR) - 1] = '\0';
         std::cout << "[DGPS MRR]: New file detected, resetting zero timestamp counter." << std::endl;
      }

      run_config_MRR->look_id[0]                        = ptr_dc_input_data->sym_detection_fc_radar.dets_info.LookType;
      run_config_MRR->timestamp[0].seconds              = 0;
      run_config_MRR->timestamp[0].fractional_seconds   = static_cast<uint64_t>(ptr_dc_input_data->sym_detection_fc_radar.dets_info.timestamp);
      run_config_MRR->filetimestamp[0].abs_timeStamp_ns = ptr_dc_input_data->sim_cycle_start_time_info[4].abs_timeStamp_ns;

      // Check if sensor timestamp is zero
      if (ptr_dc_input_data->sim_cycle_start_time_info[4].abs_timeStamp_ns == 0) {
         zero_timestamp_count_MRR++;
         std::cout << "[DGPS MRR]: Skipping dgps_run - sensor timestamp is zero. [Count: " << zero_timestamp_count_MRR << "]" << std::endl;
         return false;
      }

      if (run_config_MRR->current_mdffile[0] == '\0') {
         std::cout << "[DGPS MRR]: Skipping dgps_run - current_mdffile path is empty." << std::endl;
         return true;
      }
      if (!fs::exists(run_config_MRR->current_mdffile)) {
         std::cout << "[DGPS MRR]: Skipping dgps_run - current_mdffile does not exist: " << run_config_MRR->current_mdffile << std::endl;
         return true;
      }
      if (run_config_MRR->next_mdffile[0] != '\0' && !fs::exists(run_config_MRR->next_mdffile)) {
         std::cout << "[DGPS MRR]: Warning - next_mdffile does not exist, clearing it." << std::endl;
         memset(run_config_MRR->next_mdffile, 0, sizeof(run_config_MRR->next_mdffile));
      }

      g_dgpsContainerMRR.dgps_run(run_config_MRR, output_MRR);
      g_dgpsContainerMRR.dgps_data(&gen7_DGPS_object_MRR);

      std::cout << "MRR API Lat =" << gen7_DGPS_object_MRR.Latitude_Longitude_Host.Poslat
                << " MRR API Lon =" << gen7_DGPS_object_MRR.Latitude_Longitude_Host.PosLon << std::endl;

      ptr_dc_input_data->sym_detection_fc_radar.dets_info.LookID   = static_cast<unsigned8_T>(output_MRR->detections[0].LookID);
      ptr_dc_input_data->sym_detection_fc_radar.dets_info.LookType = output_MRR->detections[0].LookType;
      ptr_dc_input_data->sym_detection_fc_radar.dets_info.Count    = static_cast<unsigned8_T>(output_MRR->detections[0].num_of_dets);

      memset(ptr_dc_input_data->sym_detection_fc_radar.dets_input, 0, sizeof(ptr_dc_input_data->sym_detection_fc_radar.dets_input));
      for (int fcCnt = 0; fcCnt < SMVALIDATION_MAX_NUMBER_OF_DETECTIONS; fcCnt++) {
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].amplitude                = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].amplitude);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].snr                      = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].snr);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].range                    = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].range);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].range_rate               = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].range_rate);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].azimuth                  = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].azimuth);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].elevation                = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].elevation);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].azimuth_confidence       = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].azimuth_confidence);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].bistatic                 = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].bistatic);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].elevation_confidence     = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].elevation_confidence);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].existence_probability    = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].existence_probability);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].multi_target_probability = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].multi_target_probability);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].nd_target                = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].nd_target);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].std_azimuth              = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].std_azimuth);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].std_elevation            = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].std_elevation);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].std_range                = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].std_range);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].std_range_rate           = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].std_range_rate);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].std_rcs                  = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].std_rcs);
         ptr_dc_input_data->sym_detection_fc_radar.dets_input[fcCnt].valid                    = static_cast<float32_T>(output_MRR->detections[0].dets_float[fcCnt].valid);
      }

      // gtCnt to start from 1, as 0th index holds the Host information.
      for (int gtCnt = 1; gtCnt < SMVALIDATION_MAX_NUMBER_OF_TARGET; gtCnt++) {
         int gtCntTrgt                                            = gtCnt - 1;
         ptr_dc_input_data->object_rl[gtCntTrgt].age              = output_MRR->object[gtCnt].age;
         ptr_dc_input_data->object_rl[gtCntTrgt].heading          = static_cast<float>(output_MRR->object[gtCnt].heading);
         ptr_dc_input_data->object_rl[gtCntTrgt].heading_rate     = static_cast<float>(output_MRR->object[gtCnt].heading_rate);
         ptr_dc_input_data->object_rl[gtCntTrgt].id               = static_cast<unsigned8_T>(output_MRR->object[gtCnt].id);
         ptr_dc_input_data->object_rl[gtCntTrgt].length           = static_cast<float>(output_MRR->object[gtCnt].length);
         ptr_dc_input_data->object_rl[gtCntTrgt].speed            = static_cast<float>(output_MRR->object[gtCnt].speed);
         ptr_dc_input_data->object_rl[gtCntTrgt].stage_age        = output_MRR->object[gtCnt].stage_age;
         ptr_dc_input_data->object_rl[gtCntTrgt].tangential_accel = static_cast<float>(output_MRR->object[gtCnt].tangential_accel);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_lat_accel    = static_cast<float>(output_MRR->object[gtCnt].vcs_lat_accel);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_lat_posn     = static_cast<float>(output_MRR->object[gtCnt].vcs_lat_posn);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_lat_vel      = static_cast<float>(output_MRR->object[gtCnt].vcs_lat_vel);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_lat_vel_rel  = static_cast<float>(output_MRR->object[gtCnt].vcs_lat_vel_rel);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_long_accel   = static_cast<float>(output_MRR->object[gtCnt].vcs_long_accel);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_long_posn    = static_cast<float>(output_MRR->object[gtCnt].vcs_long_posn);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_long_vel     = static_cast<float>(output_MRR->object[gtCnt].vcs_long_vel);
         ptr_dc_input_data->object_rl[gtCntTrgt].vcs_long_vel_rel = static_cast<float>(output_MRR->object[gtCnt].vcs_long_vel_rel);
         ptr_dc_input_data->object_rl[gtCntTrgt].width            = static_cast<float>(output_MRR->object[gtCnt].width);
         switch (output_MRR->object[gtCnt].object_class) {
         case 1:
            ptr_dc_input_data->object_rl[gtCntTrgt].object_class = DEBUG_OBJECT_CLASS_PEDESTRIAN;
            break;
         case 2:
            ptr_dc_input_data->object_rl[gtCntTrgt].object_class = DEBUG_OBJECT_CLASS_2WHEEL;
            break;
         case 3:
            ptr_dc_input_data->object_rl[gtCntTrgt].object_class = DEBUG_OBJECT_CLASS_CAR;
            break;
         case 4:
            ptr_dc_input_data->object_rl[gtCntTrgt].object_class = DEBUG_OBJECT_CLASS_TRUCK;
            break;
         default:
            ptr_dc_input_data->object_rl[gtCntTrgt].object_class = DEBUG_OBJECT_CLASS_UNKNOWN;
            break;
         }
      }
   }
#endif
   return true;
}

void clearOutputFile() {

   if (txt_file.is_open()) {
      txt_file.close();
   }

   // Delete and re-allocate configs so the next log starts fresh.
   // FirstExec flags are also reset so updateRunConfig() re-initializes
   // with the new output path set by Set_DGPS_Output_Path().
#if defined(SRR_DC)
   if (g_dgpsContainerSRR.dgps_exit) {
      g_dgpsContainerSRR.dgps_exit();
   }
   delete input_config_SRR;
   input_config_SRR = new SMValidationInputConfig_T;
   delete run_config_SRR;
   run_config_SRR = new SMValidationRunConfig_T;
   delete output_SRR;
   output_SRR   = new SMValidationOutput_T;
   FirstExecSRR = true;
#endif

#if defined(MRR_DC)
   if (g_dgpsContainerMRR.dgps_exit) {
      g_dgpsContainerMRR.dgps_exit();
   }
   delete input_config_MRR;
   input_config_MRR = new SMValidationInputConfig_T;
   delete run_config_MRR;
   run_config_MRR = new SMValidationRunConfig_T;
   delete output_MRR;
   output_MRR   = new SMValidationOutput_T;
   FirstExecMRR = true;
#endif
}

void writeDetectionOfAllSensor() {
   int detectionId = 0;
#if defined(SRR_DC)
   SMValidationInputConfig_T *w_input_config = input_config_SRR;
   SMValidationOutput_T *w_output            = output_SRR;
#elif defined(MRR_DC)
   SMValidationInputConfig_T *w_input_config = input_config_MRR;
   SMValidationOutput_T *w_output            = output_MRR;
#else
   SMValidationInputConfig_T *w_input_config = nullptr;
   SMValidationOutput_T *w_output            = nullptr;
#endif
   if (txt_file.is_open() && w_input_config != nullptr && w_output != nullptr) {
      for (int j = 0; j < w_input_config->number_of_sensors; j++) {
         auto &detection_info = w_output->detections[j];
         if (detection_info.num_of_dets != 0) {
            txt_file << "#Timestamp in ms\n";
            txt_file << "Timestamp " << w_output->output_timestamp[j].fractional_seconds << "\n";
            txt_file << "#Id LookType NumOfDets\n";
            txt_file << "Sensor " << static_cast<int>(detection_info.sensorid) << " "
                     << static_cast<int>(detection_info.LookID) << " "
                     << static_cast<int>(detection_info.num_of_dets) << "\n";
            txt_file << "#Id ObjectID RCS SNR Range Range_Rate Azimuth Elevation\n";
            for (size_t i = 0; i < detection_info.num_of_dets; i++) {
               auto &det = detection_info.dets_float[i];
               txt_file << "Detection " << detectionId++ << " "
                        << static_cast<int>(det.objectid) << " "
                        << det.amplitude << " " << det.snr << " "
                        << det.range << " " << det.range_rate << " "
                        << det.azimuth << " " << det.elevation << "\n";
            }
         }
      }
   }
}

DGPS_Data_T *GetDGPSDataPtr() {
#if defined(SRR_DC)
   return &gen7_DGPS_object_SRR;
#elif defined(MRR_DC)
   return &gen7_DGPS_object_MRR;
#else
   return nullptr;
#endif
}

unsigned int GetZeroTimestampCountSRR() {
   return zero_timestamp_count_SRR;
}

unsigned int GetZeroTimestampCountMRR() {
   return zero_timestamp_count_MRR;
}

void Set_DGPS_Output_Path(const char *filename) {

   if (filename == nullptr) {
      std::cerr << "Error: filename is nullptr" << std::endl;
      return;
   }

   fs::path nfilepath        = filename;
   std::string nfile_name    = nfilepath.stem().string();
   std::string npath_nameSRR = nfilepath.parent_path().string() + "/DGPS_Output/SRR/";
   std::string npath_nameMRR = nfilepath.parent_path().string() + "/DGPS_Output/MRR/";

#if defined(SRR_DC)
   if (FirstExecSRR) {
      memset(FilePathSRR, 0, sizeof(FilePathSRR));
      strncpy(FilePathSRR, npath_nameSRR.c_str(), sizeof(FilePathSRR) - 1);
      FilePathSRR[sizeof(FilePathSRR) - 1] = '\0';
      std::error_code ec;
      fs::create_directories(FilePathSRR, ec);
      if (ec) {
         std::cerr << "[DGPS SRR]: Failed to create output directory: " << FilePathSRR << " (" << ec.message() << ")" << std::endl;
      } else {
         std::cout << "[DGPS SRR]: Output path set to: " << FilePathSRR << std::endl;
      }
   }
#endif
#if defined(MRR_DC)
   if (FirstExecMRR) {
      memset(FilePathMRR, 0, sizeof(FilePathMRR));
      strncpy(FilePathMRR, npath_nameMRR.c_str(), sizeof(FilePathMRR) - 1);
      FilePathMRR[sizeof(FilePathMRR) - 1] = '\0';
      std::error_code ec;
      fs::create_directories(FilePathMRR, ec);
      if (ec) {
         std::cerr << "[DGPS MRR]: Failed to create output directory: " << FilePathMRR << " (" << ec.message() << ")" << std::endl;
      } else {
         std::cout << "[DGPS MRR]: Output path set to: " << FilePathMRR << std::endl;
      }
   }
#endif
}

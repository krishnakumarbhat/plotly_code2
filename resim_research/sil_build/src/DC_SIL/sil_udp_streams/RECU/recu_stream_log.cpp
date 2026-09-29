
#include <vector>
#include "recu_stream_log.h"
#include "dc_output_data.h"
#include "f360_core_info.h"
#include "sil_ecu_input_ext.h"
#include "DCMacros.h"
#include "rspp_detection_list.h"
#include "rspp_radar_sensor.h"
#include "rspp_constants.h"
#include "olp_iface.h"
#include "olp_wrapper.h"
#include "recw_iface.h"
#include "cta_iface.h"
#include "lcda_iface.h"
#include "ced_iface.h"
#include "pt_iface.h"
#include "scw_iface.h"
#include "ta_iface.h"
#include "sfl_wrapper.h"

/* Initialising memory to core's */
Radar_ECU_CORE0_T core0_udp_buff                   = {0};
Radar_ECU_CORE1_T core1_udp_buff                   = {0};
Radar_ECU_CORE3_T core3_udp_buff                   = {0};
Radar_ECU_calibration_T ecu_cal_buff               = {0};
Radar_ECU_Internal_CORE1_T ecu_internal_buff       = {0};
Radar_ECU_VRU_T vru_udp_buff                       = {0};
Ref_Logging_Data_T OSI_Object_input                = {0};
DSPACE_Common_Logging_Data_T dspace_input_buff_ptr = {0};

/* buffer holding input data from each core's */
static logging_data_buffer_T logging_data_buff[RECU_UDP_LOGGING_DATA_BUFF_INDEX] = {{0}};
/* Mux_table used to fill cut data into packets */
static MUX_ID_Table_Struct_Logging_T mux_id_table_udp[UDP_MUX_TAB_SIZE] = {{0}};
/* Buffer into which the logging data is stored for transmission */
UDP_PACKAGE_POINTER_TABLE_T udp_data_log = {0};
/* timing buffer for udp */
static uint32_t time_ms = 0;
/* static counter for incrementing scans */
static uint16_t scan_counter = 0;

/* Farward declarations */
static void RecuLoggingDataBuffInit(void);
static uint32_t RunTimer(void);
static void ResetTimer(void);
static void ResetCounter(void);
static void GetRecwOutputLog(void);
static void GetCtaOutputLog(void);
static void CopyTaOutputToUdpLog(RECU_Ta_Output_Udp_Log_T *ptr_ta_output_udp_log, Ta_Output_T *ptr_ta_output);
static void CopyCedOutputToUdpLog(RECU_Ced_Output_Udp_Log_T *ptr_ced_output_udp_log, Ced_Output_T *ptr_ced_output);
static void CopyScwOutputToUdpLog(RECU_Scw_Output_Udp_Log_T *ptr_scw_output_udp_log, Scw_Output_T *ptr_scw_output);
static void CopyPathTrackingOutputToUdpLog(RECU_Path_Tracking_Output_Udp_Log_T *ptr_pt_output_udp_log, Pt_Output_T *ptr_pt_output);
static void CopyLcdaOutputToUdpLog(RECU_Lcda_Output_Udp_Log_T *ptr_lcda_output_udp_log, Lcda_Output_T *ptr_lcda_output);

uint16_t swap_uint16(uint16_t);
uint32_t swap_uint32(uint32_t);
SIL_DC_Output_Data_T *GetOutputDataPtr();

/* Funtion definitions */
void RECUUDPLogInit() {
   uint8_t idx = 0;
   RecuLoggingDataBuffInit();
   for (int i = 0; i < RECU_UDP_LOGGING_DATA_BUFF_INDEX; i++) {
      uint8_t no_of_chunks = 0;
      if (logging_data_buff[i].Total_size > RECU_UDP_MAX_DATA_SIZE) {
         no_of_chunks = static_cast<uint8_t>((logging_data_buff[i].Total_size % RECU_UDP_MAX_DATA_SIZE == 0) ? (logging_data_buff[i].Total_size / RECU_UDP_MAX_DATA_SIZE) : (logging_data_buff[i].Total_size / RECU_UDP_MAX_DATA_SIZE) + 1);
      } else {
         no_of_chunks = 1;
      }
      uint8_t chunk_index_temp = 0;
      for (; (idx < UDP_MUX_TAB_SIZE) && (chunk_index_temp < no_of_chunks); idx++) {
         mux_id_table_udp[idx].mux_id                = idx;
         mux_id_table_udp[idx].logging_array_address = logging_data_buff[i].data_Buff_Ptr + (RECU_UDP_MAX_DATA_SIZE * chunk_index_temp);
         mux_id_table_udp[idx].logging_source        = logging_data_buff[i].Log_Data_Source;
         mux_id_table_udp[idx].Logging_Version       = logging_data_buff[i].Struct_version;
         mux_id_table_udp[idx].total_no_chunks       = no_of_chunks;
         mux_id_table_udp[idx].chunk_number          = chunk_index_temp;
         if (chunk_index_temp != no_of_chunks - 1) {
            mux_id_table_udp[idx].logging_array_size = RECU_UDP_MAX_DATA_SIZE;
            mux_id_table_udp[idx].frame_diagostic    = FRAME_FULL;
         } else {
            if (logging_data_buff[i].Total_size % RECU_UDP_MAX_DATA_SIZE == 0) {
               mux_id_table_udp[idx].logging_array_size = RECU_UDP_MAX_DATA_SIZE;
               mux_id_table_udp[idx].frame_diagostic    = FRAME_FULL;
            } else {
               mux_id_table_udp[idx].logging_array_size = logging_data_buff[i].Total_size % RECU_UDP_MAX_DATA_SIZE;
               mux_id_table_udp[idx].frame_diagostic    = FRAME_PARTIAL_FULL;
            }
         }
         chunk_index_temp++;
      }
   }
   udp_data_log.frame_address = (unsigned32_T *)&udp_data_log.frame;
}

void RECUUDPLogSendFrames() {
   UDP_FRAME_STRUCTURE_T *frame_ptr     = &udp_data_log.frame;
   SIL_DC_Output_Data_T *Output_Str_Ptr = GetOutputDataPtr();
   uint8_t chunk_id                     = 0;
   uint32_t crc                         = 0;
   static uint32_t streamRefIndexCnt    = 0;
   for (chunk_id = 0; chunk_id < UDP_MUX_TAB_SIZE; chunk_id++) {
      memcpy(frame_ptr->data, mux_id_table_udp[chunk_id].logging_array_address, mux_id_table_udp[chunk_id].logging_array_size);
      // for last frame fill the partially filled frame to 0's
      if (mux_id_table_udp[chunk_id].chunk_number == mux_id_table_udp[chunk_id].total_no_chunks - 1) {
         uint16_t data_ptr_last_byte = mux_id_table_udp[chunk_id].logging_array_size;
         if (mux_id_table_udp[chunk_id].logging_array_size < RECU_UDP_MAX_DATA_SIZE) {
            memset(&frame_ptr->data[data_ptr_last_byte], 0, RECU_UDP_MAX_DATA_SIZE - mux_id_table_udp[chunk_id].logging_array_size);
         }
      }
      // fill frame header
      frame_ptr->frame_header.versionInfo    = UDP_RECORD_VERSIONINFO;
      frame_ptr->frame_header.sourceTxTime   = RunTimer();
      frame_ptr->frame_header.sourceTxCnt    = streamRefIndexCnt; // GetScanIndex();
      frame_ptr->frame_header.Platform       = RECU_UDP_PLATFORM_SRR5;
      frame_ptr->frame_header.Radar_Position = RECU_RADAR_ECU_POS;
      frame_ptr->frame_header.streamRefIndex = GetScanIndex();
      frame_ptr->frame_header.streamDataLen  = mux_id_table_udp[chunk_id].logging_array_size;
      frame_ptr->frame_header.streamTxCnt    = mux_id_table_udp[chunk_id].chunk_number;
      frame_ptr->frame_header.streamNumber   = mux_id_table_udp[chunk_id].logging_source;
      frame_ptr->frame_header.streamVersion  = mux_id_table_udp[chunk_id].Logging_Version;
      frame_ptr->frame_header.streamChunks   = mux_id_table_udp[chunk_id].total_no_chunks;
      frame_ptr->frame_header.streamChunkIdx = mux_id_table_udp[chunk_id].chunk_number;
      frame_ptr->frame_header.customerId     = RECU_CUSTOMER_ID_BMW_SAT_SRR5;
      // fill customer frame header
      frame_ptr->cust_frame_header.sensorid     = RECU_RADAR_ECU_POS;
      frame_ptr->cust_frame_header.packetnumber = streamRefIndexCnt++;
      ;
      frame_ptr->cust_frame_header.Cal_Running_Cnt     = chunk_id;
      frame_ptr->cust_frame_header.Total_Cal_Chunk_Cnt = UDP_MUX_TAB_SIZE;
      // Temporary fix for OSI
      if (mux_id_table_udp[chunk_id].logging_source == OSI_LOGGING_DATA_ID) {
         frame_ptr->frame_header.Radar_Position = 1;
         frame_ptr->cust_frame_header.sensorid  = 1;
         frame_ptr->frame_header.versionInfo    = swap_uint16(frame_ptr->frame_header.versionInfo);
         frame_ptr->frame_header.streamRefIndex = swap_uint32(frame_ptr->frame_header.streamRefIndex);
         frame_ptr->frame_header.streamDataLen  = swap_uint16(frame_ptr->frame_header.streamDataLen);
      } else if (mux_id_table_udp[chunk_id].logging_source == DSPACE) {
         /*frame_ptr->frame_header.Platform = 0x3F;
         frame_ptr->frame_header.customerId = 0xA1;
         frame_ptr->frame_header.Radar_Position = 0x00;*/
         frame_ptr->cust_frame_header.packetnumber = 0x3F;
         frame_ptr->frame_header.versionInfo       = swap_uint16(frame_ptr->frame_header.versionInfo);
         frame_ptr->frame_header.streamRefIndex    = swap_uint32(frame_ptr->frame_header.streamRefIndex);
         frame_ptr->frame_header.streamDataLen     = swap_uint16(frame_ptr->frame_header.streamDataLen);
      }
      memcpy(&Output_Str_Ptr->UDP_Tx_Buff[chunk_id], &udp_data_log.frame, sizeof(UDP_FRAME_STRUCTURE_T));
   }
   Output_Str_Ptr->UDP_Buff_Len      = chunk_id;
   Output_Str_Ptr->f_dvsu_buff_valid = 1;
}

static void RecuLoggingDataBuffInit() {
   /* CORE 0 logging structure */
   logging_data_buff[0].data_Buff_Ptr   = (uint8_t *)GetRadarCore0DataPtr();
   logging_data_buff[0].Total_size      = sizeof(Radar_ECU_CORE0_T);
   logging_data_buff[0].Log_Data_Source = CORE0_LOGGING_DATA;
   logging_data_buff[0].Struct_version  = RADAR_ECU_CORE0_VERSION;
   /* CORE 1 logging structure */
   logging_data_buff[1].data_Buff_Ptr   = (uint8_t *)GetRadarCore1DataPtr();
   logging_data_buff[1].Total_size      = sizeof(Radar_ECU_CORE1_T);
   logging_data_buff[1].Log_Data_Source = CORE1_LOGGING_DATA;
   logging_data_buff[1].Struct_version  = RADAR_ECU_CORE1_VERSION;
   /* CORE3 logging structure */
   logging_data_buff[2].data_Buff_Ptr   = (uint8_t *)GetRadarCore3DataPtr();
   logging_data_buff[2].Total_size      = sizeof(Radar_ECU_CORE3_T);
   logging_data_buff[2].Log_Data_Source = CORE3_LOGGING_DATA;
   logging_data_buff[2].Struct_version  = RADAR_ECU_CORE3_VERSION;
   /* CAL logging structure */
   logging_data_buff[3].data_Buff_Ptr   = (uint8_t *)GetRadarCalDataPtr();
   logging_data_buff[3].Total_size      = sizeof(Radar_ECU_calibration_T);
   logging_data_buff[3].Log_Data_Source = CAL_LOGGING_DATA;
   logging_data_buff[3].Struct_version  = RADAR_ECU_CALIBRATION_VERSION;
   /* Tracker Internal logging structure */
   logging_data_buff[4].data_Buff_Ptr   = (uint8_t *)GetRadarInternalDataPtr();
   logging_data_buff[4].Total_size      = sizeof(Radar_ECU_Internal_CORE1_T);
   logging_data_buff[4].Log_Data_Source = ECU_INTERNAL_LOGGING_DATA;
   logging_data_buff[4].Struct_version  = RADAR_ECU_INTERNAL_CORE1_VERSION;
   /* VRU Classifier logging data */
   logging_data_buff[5].data_Buff_Ptr   = (uint8_t *)GetRadarVruDataPtr();
   logging_data_buff[5].Total_size      = sizeof(Radar_ECU_VRU_T);
   logging_data_buff[5].Log_Data_Source = VRU_CLASSIFIER_LOGGING_DATA;
   logging_data_buff[5].Struct_version  = RADAR_ECU_VRU_CLASSIFIER_VERSION;
   /* OSI logging structure */
   logging_data_buff[6].data_Buff_Ptr   = (uint8_t *)GetRadarOsiDataPtr();
   logging_data_buff[6].Total_size      = sizeof(Ref_Logging_Data_T);
   logging_data_buff[6].Log_Data_Source = OSI_LOGGING_DATA;
   logging_data_buff[6].Struct_version  = OSI_INPUT_STRUCTURE_VERSION;
   /* DSPACE Input logging structure */
   logging_data_buff[7].data_Buff_Ptr   = (uint8_t *)GetRadarDspaceInputDataPtr();
   logging_data_buff[7].Total_size      = sizeof(DSPACE_Common_Logging_Data_T);
   logging_data_buff[7].Log_Data_Source = DSPACE_INPUT_LOGGING_DATA;
   logging_data_buff[7].Struct_version  = DSPACE_COMMON_INPUT_STRUCTURE_VERSION;
}

/* get core data pointers definition */
Radar_ECU_CORE0_T *GetRadarCore0DataPtr() {
   return &core0_udp_buff;
}

Radar_ECU_CORE1_T *GetRadarCore1DataPtr() {
   return &core1_udp_buff;
}

Radar_ECU_CORE3_T *GetRadarCore3DataPtr() {
   return &core3_udp_buff;
}

Radar_ECU_calibration_T *GetRadarCalDataPtr() {
   return &ecu_cal_buff;
}

Radar_ECU_Internal_CORE1_T *GetRadarInternalDataPtr() {
   return &ecu_internal_buff;
}

Radar_ECU_VRU_T *GetRadarVruDataPtr() {
   return &vru_udp_buff;
}

Ref_Logging_Data_T *GetRadarOsiDataPtr() {
   return &OSI_Object_input;
}

DSPACE_Common_Logging_Data_T *GetRadarDspaceInputDataPtr() {
   return &dspace_input_buff_ptr;
}

static uint32_t RunTimer() {
   time_ms += 50;
   return time_ms;
}

static void ResetTimer() {
   time_ms = 0;
}

uint16_t GetScanIndex() {
   return scan_counter;
}

void SetRECUMUDPScanIndex(uint16_T in_val) {
   scan_counter = in_val;
}

static void ResetCounter() {
   scan_counter = 0;
}

uint16_t swap_uint16(uint16_t val) {
   return (val << 8) | (val >> 8);
}

//! Byte swap unsigned int
uint32_t swap_uint32(uint32_t val) {
   uint32_t output = 0;

   output = (((val & 0xFF000000) >> 24) |
             ((val & 0x00FF0000) >> 8) |
             ((val & 0x0000FF00) << 8) |
             ((val & 0x000000FF) << 24));

   return output;
}

void PopulateRECUStreamData(f360_variant_A::F360_Object_Log_Output_T *obj, DC_INPUT_DATA_T *sil_input_buffer, f360_variant_A::F360_Detection_Log_Output_T *log_dets, rspp_variant_A::F360_Radar_Sensor_T *sens, f360_variant_A::F360_Core_Info_T *core_info) {
   Radar_ECU_CORE3_T *core3_ptr = GetRadarCore3DataPtr();
   Radar_ECU_CORE1_T *core1_ptr = GetRadarCore1DataPtr();
   Radar_ECU_CORE0_T *core0_ptr = GetRadarCore0DataPtr();
   uint8_t det_Index            = 0;
   int j = 0, valid_i = 0;

   const APT_ECU_Vehicle_Data_Input_T *vehicle_data = &sil_input_buffer->vehicle_inputs;
   std::vector<DetectionObj_Data_Input_T *> Radar_Input{
       &sil_input_buffer->sym_detection_fl_radar,
       &sil_input_buffer->sym_detection_fr_radar,
       &sil_input_buffer->sym_detection_rl_radar,
       &sil_input_buffer->sym_detection_rr_radar,
       &sil_input_buffer->sym_detection_fc_radar,
   };

   memset(core0_ptr, 0, sizeof(Radar_ECU_CORE0_T));
   core0_ptr->ego_vehicle_info_Core0.abs_speed             = vehicle_data->abs_speed;
   core0_ptr->ego_vehicle_info_Core0.speed                 = vehicle_data->abs_speed;
   core0_ptr->ego_vehicle_info_Core0.raw_speed             = vehicle_data->abs_speed;
   core0_ptr->ego_vehicle_info_Core0.yaw_rate              = vehicle_data->yawrate;
   core0_ptr->ego_vehicle_info_Core0.reverse_gear          = vehicle_data->f_reverse_gear;
   core0_ptr->ego_vehicle_info_Core0.f_reverse             = (vehicle_data->f_reverse_gear == 1) ? 1 : 0;
   core0_ptr->ego_vehicle_info_Core0.f_trailer_present     = vehicle_data->f_trailer_present;
   core0_ptr->ego_vehicle_info_Core0.steering_angle        = vehicle_data->steering_angle;
   core0_ptr->ego_vehicle_info_Core0.dist_rear_axle_to_vcs = std::abs(vehicle_data->rear_axle_position);
   core0_ptr->ego_vehicle_info_Core0.vcs_lat_acc           = vehicle_data->vcs_lat_acc;
   core0_ptr->ego_vehicle_info_Core0.vcs_long_acc          = vehicle_data->vcs_long_acc;
   core0_ptr->ego_vehicle_info_Core0.prndl                 = vehicle_data->prndl;
   core0_ptr->ego_vehicle_info_Core0.curve_radius          = vehicle_data->curve_radius;
   core0_ptr->ego_vehicle_info_Core0.host_vehicle_length   = vehicle_data->host_vehicle_length;
   core0_ptr->ego_vehicle_info_Core0.host_vehicle_width    = vehicle_data->host_vehicle_width;
   if (vehicle_data->f_reverse_gear == 1) {
      core0_ptr->ego_vehicle_info_Core0.speed *= -1.0F;
   }

   for (int i = 0; i < rspp_variant_A::MAX_NUMBER_OF_SENSORS && i < MAX_DC_SENSORS; i++) {

      DetectionObj_Data_Input_T *det_ip                       = Radar_Input[i + DC_SENSOR];
      core0_ptr->raw_srr_dets[i].timestamp                    = det_ip->dets_info.timestamp;
      core0_ptr->raw_srr_dets[i].look_id                      = det_ip->dets_info.LookID;
      core0_ptr->raw_srr_dets[i].look_index                   = det_ip->dets_info.ScanIndex;
      core0_ptr->raw_srr_dets[i].look_type                    = det_ip->dets_info.LookType;
      core0_ptr->raw_srr_dets[i].sensor_id                    = i + 1;
      core0_ptr->raw_srr_dets[i].vcs_boresight_az_align_angle = det_ip->dets_info.AutoAlignAzimuth;
      core0_ptr->raw_srr_dets[i].vcs_boresight_el_align_angle = det_ip->dets_info.AutoAlignElevation;
      core0_ptr->raw_srr_dets[i].veh_speed_comp_factor        = 1;
      core0_ptr->tracker_timestamp_us                         = static_cast<unsigned64_T>(core0_ptr->raw_srr_dets[0].timestamp * 1000.0);

      for (j = 0, valid_i = 0; j < NUMBER_OF_SRR_DETECTIONS_LOG; j++) {
         if (det_ip->dets_input[j].valid == 1) {
            core0_ptr->raw_srr_dets[i].raw_dets[valid_i].amplitude        = det_ip->dets_input[j].amplitude;
            core0_ptr->raw_srr_dets[i].raw_dets[valid_i].range            = det_ip->dets_input[j].range;
            core0_ptr->raw_srr_dets[i].raw_dets[valid_i].range_rate       = det_ip->dets_input[j].range_rate;
            core0_ptr->raw_srr_dets[i].raw_dets[valid_i].azimuth          = det_ip->dets_input[j].azimuth;
            core0_ptr->raw_srr_dets[i].raw_dets[valid_i].elevation        = det_ip->dets_input[j].elevation;
            core0_ptr->raw_srr_dets[i].raw_dets[valid_i].valid            = det_ip->dets_input[j].valid;
            core0_ptr->raw_srr_dets[i].raw_dets[valid_i].confid_azimuth   = det_ip->dets_input[j].azimuth_confidence;
            core0_ptr->raw_srr_dets[i].raw_dets[valid_i].confid_elevation = det_ip->dets_input[j].elevation_confidence;
            core0_ptr->raw_srr_dets[i].raw_dets[valid_i].host_clutter     = det_ip->dets_input[j].host_veh_clutter;
            core0_ptr->raw_srr_dets[i].raw_dets[valid_i].snr              = det_ip->dets_input[j].snr;
            core0_ptr->raw_srr_dets[i].raw_dets[valid_i].super_res_target = det_ip->dets_input[j].super_res_target;
            core0_ptr->raw_srr_dets[i].raw_dets[valid_i].nd_target        = det_ip->dets_input[j].nd_target;
            core0_ptr->raw_srr_dets[i].raw_dets[valid_i].bistatic         = det_ip->dets_input[j].bistatic;
            valid_i++;
         }
      }
      core0_ptr->raw_srr_dets[i].num_dets = valid_i;
   }
   // core 1
   memset(core1_ptr, 0, sizeof(Radar_ECU_CORE1_T));
   core1_ptr->tracker_oal_timestamp_us = core_info->time_us;
   core1_ptr->tracker_index            = core_info->cnt_loops;

   for (int i = 0; i < rspp_variant_A::MAX_NUMBER_OF_SENSORS && i < MAX_DC_SENSORS; i++) {

      core1_ptr->processed_detections.sensorInfoLog[i].sensorID                 = sens[i].constant.id;
      core1_ptr->processed_detections.sensorInfoLog[i].f_sens_valid             = sens[i].variable.is_valid;
      core1_ptr->processed_detections.sensorInfoLog[i].mount_location           = sens[i].constant.mounting_location;
      core1_ptr->processed_detections.sensorInfoLog[i].sensor_type              = sens[i].constant.sensor_type;
      core1_ptr->processed_detections.sensorInfoLog[i].radar_polarity           = sens[i].constant.polarity;
      core1_ptr->processed_detections.sensorInfoLog[i].vcs_long_posn            = sens[i].constant.mounting_position.vcs_position.longitudinal;
      core1_ptr->processed_detections.sensorInfoLog[i].vcs_lat_posn             = sens[i].constant.mounting_position.vcs_position.lateral;
      core1_ptr->processed_detections.sensorInfoLog[i].vcs_height_offset_m      = sens[i].constant.mounting_position.vcs_position.height;
      core1_ptr->processed_detections.sensorInfoLog[i].vcs_boresight_az_angle   = sens[i].constant.mounting_position.vcs_boresight_azimuth_angle;
      core1_ptr->processed_detections.sensorInfoLog[i].vcs_boresight_elev_angle = sens[i].constant.mounting_position.vcs_boresight_elevation_angle;
      core1_ptr->processed_detections.sensorInfoLog[i].align_angle_az_rad       = 0;
      core1_ptr->processed_detections.sensorInfoLog[i].align_angle_el_rad       = 0;
      core1_ptr->processed_detections.sensorInfoLog[i].timestamp_us             = sens[i].variable.timestamp_us;
      core1_ptr->processed_detections.sensorInfoLog[i].look_index               = sens[i].variable.look_index;
      core1_ptr->processed_detections.sensorInfoLog[i].new_measurement_update   = 0U < sens[i].variable.number_of_valid_detections;
      core1_ptr->processed_detections.sensorInfoLog[i].look_id                  = sens[i].variable.look_id;
      core1_ptr->processed_detections.sensorInfoLog[i].vcs_long_vel             = sens[i].variable.vcs_velocity.longitudinal;
      core1_ptr->processed_detections.sensorInfoLog[i].vcs_lat_vel              = sens[i].variable.vcs_velocity.lateral;
   }

   int dets = (log_dets->f360header.num_elements);
   for (int k = 0; k < dets; k++) {
      core1_ptr->processed_detections.detsLog.vcs_y[k]             = log_dets->detection[k].vcs_y;
      core1_ptr->processed_detections.detsLog.rngrate_dealiased[k] = log_dets->detection[k].rngrate_dealiased;
      core1_ptr->processed_detections.detsLog.rngrate_comp[k]      = log_dets->detection[k].rngrate_comp;
      core1_ptr->processed_detections.detsLog.raw_det_id[k]        = log_dets->detection[k].raw_det_id;
      core1_ptr->processed_detections.detsLog.vcs_x[k]             = log_dets->detection[k].vcs_x;
      core1_ptr->processed_detections.detsLog.objTrkID[k]          = log_dets->detection[k].objTrkID;
      core1_ptr->processed_detections.detsLog.clusterID[k]         = log_dets->detection[k].clusterID;
      core1_ptr->processed_detections.detsLog.sensorID[k]          = log_dets->detection[k].sensorID;
      core1_ptr->processed_detections.detsLog.motion_status[k]     = log_dets->detection[k].motion_status;
      core1_ptr->processed_detections.detsLog.wheel_spin[k]        = log_dets->detection[k].wheel_spin;
      core1_ptr->processed_detections.detsLog.f_dealiased[k]       = log_dets->detection[k].f_dealiased;
      core1_ptr->processed_detections.detsLog.f_double_bounce[k]   = log_dets->detection[k].f_dealiased;
      core1_ptr->processed_detections.detsLog.f_FOV_edge[k]        = log_dets->detection[k].f_FOV_edge;
      core1_ptr->processed_detections.detsLog.f_close_target[k]    = log_dets->detection[k].f_close_target;
      core1_ptr->processed_detections.detsLog.f_inside_gate[k]     = log_dets->detection[k].f_inside_gate;
      core1_ptr->processed_detections.detsLog.f_ok_to_use[k]       = log_dets->detection[k].f_ok_to_use;
      core1_ptr->processed_detections.detsLog.f_on_guardrail[k]    = log_dets->detection[k].f_on_guardrail;
      core1_ptr->processed_detections.detsLog.f_selected_by_trk[k] = log_dets->detection[k].f_used_in_rr_msmt_update;
      core1_ptr->processed_detections.detsLog.f_azimuth_error[k]   = 0;
   }
   core1_ptr->processed_detections.detsLog.num_dets = log_dets->f360header.num_elements;

   Olp_Data_T *Olp_Data_GPtr = GetOLPObjectsData();
   for (uint32_t i = 0; i < SFL_OBJ_NUMBER_OF_OBJECTS; i++) {
      core1_ptr->all_objects.obj[i].vcs_xposn    = Olp_Data_GPtr->olp_inout_obj_data[i].vcs_pos.x;
      core1_ptr->all_objects.obj[i].vcs_yposn    = Olp_Data_GPtr->olp_inout_obj_data[i].vcs_pos.y;
      core1_ptr->all_objects.obj[i].vcs_xvel     = Olp_Data_GPtr->olp_inout_obj_data[i].vcs_vel.x;
      core1_ptr->all_objects.obj[i].vcs_yvel     = Olp_Data_GPtr->olp_inout_obj_data[i].vcs_vel.y;
      core1_ptr->all_objects.obj[i].vcs_xaccel   = Olp_Data_GPtr->olp_inout_obj_data[i].vcs_accel.x;
      core1_ptr->all_objects.obj[i].vcs_yaccel   = Olp_Data_GPtr->olp_inout_obj_data[i].vcs_accel.y;
      core1_ptr->all_objects.obj[i].vcs_heading  = Olp_Data_GPtr->olp_inout_obj_data[i].vcs_heading;
      core1_ptr->all_objects.obj[i].vcs_pointing = Olp_Data_GPtr->olp_inout_obj_data[i].vcs_heading;
      core1_ptr->all_objects.obj[i].speed        = Olp_Data_GPtr->olp_inout_obj_data[i].speed;

      core1_ptr->all_objects.obj[i].len1                     = Olp_Data_GPtr->olp_inout_obj_data[i].length;
      core1_ptr->all_objects.obj[i].wid1                     = Olp_Data_GPtr->olp_inout_obj_data[i].width;
      core1_ptr->all_objects.obj[i].time_since_track_updated = Olp_Data_GPtr->olp_inout_obj_data[i].age;
      core1_ptr->all_objects.obj[i].existence_probability    = Olp_Data_GPtr->olp_inout_obj_data[i].existence_probability;

      core1_ptr->all_objects.obj[i].probability_bicycle    = Olp_Data_GPtr->olp_inout_obj_data[i].class_prob_2wheel;
      core1_ptr->all_objects.obj[i].probability_car        = Olp_Data_GPtr->olp_inout_obj_data[i].class_prob_car;
      core1_ptr->all_objects.obj[i].probability_motorcycle = Olp_Data_GPtr->olp_inout_obj_data[i].class_prob_2wheel;
      core1_ptr->all_objects.obj[i].probability_pedestrian = Olp_Data_GPtr->olp_inout_obj_data[i].class_prob_pedestrian;
      core1_ptr->all_objects.obj[i].probability_truck      = Olp_Data_GPtr->olp_inout_obj_data[i].class_prob_truck;
      float max_prob                                       = Olp_Data_GPtr->olp_inout_obj_data[i].class_prob_2wheel;
      if (max_prob < Olp_Data_GPtr->olp_inout_obj_data[i].class_prob_car)
         max_prob = Olp_Data_GPtr->olp_inout_obj_data[i].class_prob_car;
      if (max_prob < Olp_Data_GPtr->olp_inout_obj_data[i].class_prob_pedestrian)
         max_prob = Olp_Data_GPtr->olp_inout_obj_data[i].class_prob_pedestrian;
      if (max_prob < Olp_Data_GPtr->olp_inout_obj_data[i].class_prob_truck)
         max_prob = Olp_Data_GPtr->olp_inout_obj_data[i].class_prob_truck;
      core1_ptr->all_objects.obj[i].probability_undet = 1 - max_prob;

      core1_ptr->all_objects.obj[i].trkID         = Olp_Data_GPtr->olp_inout_obj_data[i].id;
      core1_ptr->all_objects.obj[i].reducedID     = Olp_Data_GPtr->olp_inout_obj_data[i].id;
      core1_ptr->all_objects.obj[i].status        = Olp_Data_GPtr->olp_inout_obj_data[i].status;
      core1_ptr->all_objects.obj[i].reducedStatus = Olp_Data_GPtr->olp_inout_obj_data[i].status;
      core1_ptr->all_objects.obj[i].object_class  = Olp_Data_GPtr->olp_inout_obj_data[i].obj_class;

      core1_ptr->all_objects.obj[i].f_fast_moving = Olp_Data_GPtr->olp_inout_obj_data[i].f_moveable;
      core1_ptr->all_objects.obj[i].f_moveable    = Olp_Data_GPtr->olp_inout_obj_data[i].f_moveable;
      core1_ptr->all_objects.obj[i].f_moving      = Olp_Data_GPtr->olp_inout_obj_data[i].f_moveable;
   }
   // core1_ptr->all_objects.object_list_timestamp = g_tracker_OAL->tracker_info.object_list_timestamp; // g_f360->core_info.time_us
   core1_ptr->all_objects.tracker_index = core_info->cnt_loops;

   /* Host RAW */
   /*core1_ptr->host_raw_data_log.reverse_gear =
   core1_ptr->host_raw_data_log.prndl =
   core1_ptr->host_raw_data_log.speed_qf =
   core1_ptr->host_raw_data_log.yaw_rate_qf =
   core1_ptr->host_raw_data_log.steering_wheel_angle_qf =
   core1_ptr->host_raw_data_log.lat_accel_qf =
   core1_ptr->host_raw_data_log.long_accel_qf =
   core1_ptr->host_raw_data_log.global_time_sync_s =
   core1_ptr->host_raw_data_log.timestamp_s =
   core1_ptr->host_raw_data_log.raw_speed =
   core1_ptr->host_raw_data_log.steering_wheel_angle_rad =
   core1_ptr->host_raw_data_log.raw_yaw_rate_rad =
   core1_ptr->host_raw_data_log.lat_accel =
   core1_ptr->host_raw_data_log.long_accel =
   core1_ptr->host_raw_data_log.speed_correction_factor =
   core1_ptr->host_raw_data_log.f_trailer_present =
   /* Host Calibs
   core1_ptr->host_calib_log.dist_rear_axle_to_vcs_m =
   core1_ptr->host_calib_log.rear_cornering_compliance =
   core1_ptr->host_calib_log.steer_gear_ratio =
   core1_ptr->host_calib_log.wheelbase_m =
   core1_ptr->host_calib_log.understeer_coefficient =
   core1_ptr->host_calib_log.vehicle_width_m =
   core1_ptr->host_calib_log.cog_x =
   core1_ptr->host_calib_log.cog_y =
   core1_ptr->host_calib_log.front_wheel_radius_m =
   core1_ptr->host_calib_log.front_track_width_m =
   core1_ptr->host_calib_log.raw_host_signal_latency_ms =
   core1_ptr->host_calib_log.f_enable_internal_reflections_func =
   core1_ptr->host_calib_log.f_enable_internal_reflections_func_trailer =*/
}
void CopyFFOutputToUDPBuffer(f360_variant_A::F360_Object_Log_Output_T *obj) {
   uint32_t i                   = 0U;
   Radar_ECU_CORE3_T *core3_ptr = GetRadarCore3DataPtr();
   memset(core3_ptr, 0, sizeof(Radar_ECU_CORE3_T));
   /*Copy core output to Feature Functions */
   GetRecwOutputLog();
   GetCtaOutputLog();
   CopyLcdaOutputToUdpLog(&(core3_ptr->FF_OUTPUT_Logging_Data.lcda_log.lcda_output), Lcda_Get_Output_Ptr());
   CopyCedOutputToUdpLog(&(core3_ptr->FF_OUTPUT_Logging_Data.ced_log.ced_output), Ced_Get_Output_Ptr());
   CopyPathTrackingOutputToUdpLog(&(core3_ptr->FF_OUTPUT_Logging_Data.path_tracking_log.Path_Tracking_Output_Udp_Log), Pt_Get_Output_Ptr());
   CopyScwOutputToUdpLog(&(core3_ptr->FF_OUTPUT_Logging_Data.scw_log.scw_output), Scw_Get_Output_Ptr());
   CopyTaOutputToUdpLog(&(core3_ptr->FF_OUTPUT_Logging_Data.ta_log.ta_output), Ta_Get_Output_Ptr());

   for (int i = 0; i < 50; i++) {
      if (obj->object[i].reducedID > 0U) {
         core3_ptr->gdsr_tracker_log_output.object_id[i]     = static_cast<unsigned8_T>(obj->object[i].reducedID);
         core3_ptr->gdsr_tracker_log_output.status[i]        = obj->object[i].reducedStatus;
         core3_ptr->gdsr_tracker_log_output.age[i]           = obj->object[i].time_since_track_updated;
         core3_ptr->gdsr_tracker_log_output.length[i]        = obj->object[i].len1 + obj->object[i].len2;
         core3_ptr->gdsr_tracker_log_output.width[i]         = obj->object[i].wid1 + obj->object[i].wid2;
         core3_ptr->gdsr_tracker_log_output.heading[i]       = obj->object[i].vcs_heading;
         core3_ptr->gdsr_tracker_log_output.object_class[i]  = obj->object[i].object_class;
         core3_ptr->gdsr_tracker_log_output.f_stationary[i]  = (obj->object[i].f_moving == 0U) ? 1U : 0U;
         core3_ptr->gdsr_tracker_log_output.f_moveable[i]    = obj->object[i].f_moveable;
         core3_ptr->gdsr_tracker_log_output.vcs_long_posn[i] = obj->object[i].vcs_xposn;
         core3_ptr->gdsr_tracker_log_output.vcs_lat_posn[i]  = obj->object[i].vcs_yposn;
         core3_ptr->gdsr_tracker_log_output.vcs_long_vel[i]  = obj->object[i].vcs_xvel;
         core3_ptr->gdsr_tracker_log_output.vcs_lat_vel[i]   = obj->object[i].vcs_yvel;
         core3_ptr->data_valid                               = 1;
      }
   }
}

static void GetRecwOutputLog(void) {

   Recw_Output_T *ptr_recw_output;
   ptr_recw_output              = Recw_Get_Output_Ptr();
   Radar_ECU_CORE3_T *core3_ptr = GetRadarCore3DataPtr();

   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_ttc                            = ptr_recw_output->recw_ttc_s;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_obj_distance                   = 0;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_obj_speed                      = 0;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_obj_lat_pos                    = 0;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_obj_long_pos                   = 0;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_obj_heading                    = 0;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_crash_probability              = ptr_recw_output->recw_crash_probability;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_overlap                        = 0;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_ttc_warning_threshold          = ptr_recw_output->ttc_threshold_alert_level_1_s;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recb_nominal_acceleration_applied   = 0;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_alert_signal                   = ptr_recw_output->recw_alert_level;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_acute_alert_signal             = ptr_recw_output->recw_alert_level;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_status                         = 1;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_obj_id                         = ptr_recw_output->recw_unique_id;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_obj_class                      = 0;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recw_obj_class_cdc                  = 0;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recb_braking_signal                 = 0;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recb_qualifier_nominal_acceleration = 0;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recb_SSM                            = 0;
   core3_ptr->FF_OUTPUT_Logging_Data.recw_Log.recw_output.recb_integrity                      = 0;

} /* End of GetRecwOutputLog() */

static void GetCtaOutputLog(void) {
   Cta_Output_T *p_cta_output   = Cta_Get_Output_Ptr();
   Radar_ECU_CORE3_T *core3_ptr = GetRadarCore3DataPtr();

   core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.f_cta_enabled = p_cta_output->f_cta_enabled;
   if (p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_LEFT].alert_level >= 1 || p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_RIGHT].alert_level >= 1) {
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_radarposition = 2;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_id_left       = p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_LEFT].id;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_ttc_left      = p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_LEFT].ttc_s;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.f_cta_alert_left  = p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_LEFT].alert_level;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.f_cta_warn_left   = p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_LEFT].alert_level;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_id_right      = p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_RIGHT].id;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_ttc_right     = p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_RIGHT].ttc_s;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.f_cta_alert_right = p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_RIGHT].alert_level;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.f_cta_warn_right  = p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_RIGHT].alert_level;
   }
   if (p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_LEFT].alert_level >= 1 || p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_RIGHT].alert_level >= 1) {
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_radarposition = 2;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_id_left       = p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_LEFT].id;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_ttc_left      = p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_LEFT].ttc_s;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.f_cta_alert_left  = p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_LEFT].alert_level;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.f_cta_warn_left   = p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_LEFT].alert_level;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_id_right      = p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_RIGHT].id;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_ttc_right     = p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_RIGHT].ttc_s;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.f_cta_alert_right = p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_RIGHT].alert_level;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.f_cta_warn_right  = p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_RIGHT].alert_level;
   }
   if ((p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_LEFT].alert_level == 0 && p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_RIGHT].alert_level == 0) && (p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_LEFT].alert_level == 0 && p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_RIGHT].alert_level == 0)) {
      // reset all CTA/CTB left and right
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_radarposition = 0;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_id_left       = 0;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_ttc_left      = 0;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.f_cta_alert_left  = 0;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.f_cta_warn_left   = 0;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_id_right      = 0;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.cta_ttc_right     = 0;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.f_cta_alert_right = 0;
      core3_ptr->FF_OUTPUT_Logging_Data.cta_log.cta_output_log.f_cta_warn_right  = 0;
   }
} /* GetCtaOutputLog*/

static void CopyLcdaOutputToUdpLog(RECU_Lcda_Output_Udp_Log_T *ptr_lcda_output_udp_log,
                                   Lcda_Output_T *ptr_lcda_output) {
   uint8_t index = 0;

   ptr_lcda_output_udp_log->f_lcda_enabled = 1; // by default enable working sfl in orcas.
   ptr_lcda_output_udp_log->f_bsw_enabled  = ptr_lcda_output->f_bsw_enabled;
   ptr_lcda_output_udp_log->f_cvw_enabled  = ptr_lcda_output->f_cvw_enabled;
   ptr_lcda_output_udp_log->f_slc_enabled  = ptr_lcda_output->f_slc_enabled;
   // ptr_lcda_output_udp_log->f_awa_enabled = ptr_lcda_output->f_awa_enabled;

   for (index = 0; index < LCDA_NUMBER_OF_SIDES; index++) {
      ptr_lcda_output_udp_log->bsw_alert[index] = ptr_lcda_output->bsw_alert[index];
      ptr_lcda_output_udp_log->bsw_id[index]    = ptr_lcda_output->bsw_id[index];

      ptr_lcda_output_udp_log->cvw_alert[index] = ptr_lcda_output->cvw_alert[index];
      ptr_lcda_output_udp_log->cvw_id[index]    = ptr_lcda_output->cvw_id[index];
      ptr_lcda_output_udp_log->cvw_ttc[index]   = ptr_lcda_output->cvw_ttc_s[index];

      ptr_lcda_output_udp_log->slc_alert[index]                   = ptr_lcda_output->slc_alert[index];
      ptr_lcda_output_udp_log->slc_id[index]                      = ptr_lcda_output->slc_id[index];
      ptr_lcda_output_udp_log->slc_ttc[index]                     = ptr_lcda_output->slc_ttc_s[index];
      ptr_lcda_output_udp_log->slc_lane_change_probability[index] = ptr_lcda_output->slc_lane_change_probability[index];
   }

} /* End of CopyLcdaOutputToUdpLog() */

static void CopyCedOutputToUdpLog(RECU_Ced_Output_Udp_Log_T *ptr_ced_output_udp_log,
                                  Ced_Output_T *ptr_ced_output) {
   ptr_ced_output_udp_log->SFE_CED_Status                = ptr_ced_output->f_ced_enable;
   ptr_ced_output_udp_log->SFE_CED_alert_left            = ptr_ced_output->ced_alert[FBK_SIDE_LEFT];
   ptr_ced_output_udp_log->SFE_CED_dir_left              = ptr_ced_output->ced_object[FBK_SIDE_LEFT].direction;
   ptr_ced_output_udp_log->SFE_CED_ttc_left              = ptr_ced_output->ced_object[FBK_SIDE_LEFT].ttc_s;
   ptr_ced_output_udp_log->SFE_CED_id_left               = ptr_ced_output->ced_object[FBK_SIDE_LEFT].id;
   ptr_ced_output_udp_log->SFE_CED_obj_type_left         = ptr_ced_output->ced_object[FBK_SIDE_LEFT].type;
   ptr_ced_output_udp_log->SFE_CED_obj_speed_left        = ptr_ced_output->ced_object[FBK_SIDE_LEFT].speed_mps;
   ptr_ced_output_udp_log->SFE_CED_obj_heading_left      = ptr_ced_output->ced_object[FBK_SIDE_LEFT].heading_rad;
   ptr_ced_output_udp_log->SFE_CED_obj_lateral_pos_left  = ptr_ced_output->ced_object[FBK_SIDE_LEFT].lat_pos_m;
   ptr_ced_output_udp_log->SFE_CED_obj_long_pos_left     = ptr_ced_output->ced_object[FBK_SIDE_LEFT].long_pos_m;
   ptr_ced_output_udp_log->SFE_CED_alert_right           = ptr_ced_output->ced_alert[FBK_SIDE_RIGHT];
   ptr_ced_output_udp_log->SFE_CED_dir_right             = ptr_ced_output->ced_object[FBK_SIDE_RIGHT].direction;
   ptr_ced_output_udp_log->SFE_CED_ttc_right             = ptr_ced_output->ced_object[FBK_SIDE_RIGHT].ttc_s;
   ptr_ced_output_udp_log->SFE_CED_id_right              = ptr_ced_output->ced_object[FBK_SIDE_RIGHT].id;
   ptr_ced_output_udp_log->SFE_CED_obj_type_right        = ptr_ced_output->ced_object[FBK_SIDE_RIGHT].type;
   ptr_ced_output_udp_log->SFE_CED_obj_speed_right       = ptr_ced_output->ced_object[FBK_SIDE_RIGHT].speed_mps;
   ptr_ced_output_udp_log->SFE_CED_obj_heading_right     = ptr_ced_output->ced_object[FBK_SIDE_RIGHT].heading_rad;
   ptr_ced_output_udp_log->SFE_CED_obj_lateral_pos_right = ptr_ced_output->ced_object[FBK_SIDE_RIGHT].lat_pos_m;
   ptr_ced_output_udp_log->SFE_CED_obj_long_pos_right    = ptr_ced_output->ced_object[FBK_SIDE_RIGHT].long_pos_m;
   ptr_ced_output_udp_log->SFE_CED_rear_status           = ((ptr_ced_output->ced_object[FBK_SIDE_LEFT].direction == 1) || (ptr_ced_output->ced_object[FBK_SIDE_RIGHT].direction == 1));

   if (ptr_ced_output_udp_log->SFE_CED_rear_status == 1) {
      ptr_ced_output_udp_log->SFE_CED_rear_alert_right = ptr_ced_output->ced_alert[FBK_SIDE_RIGHT];
      ptr_ced_output_udp_log->SFE_CED_rear_alert_left  = ptr_ced_output->ced_alert[FBK_SIDE_LEFT];
      ptr_ced_output_udp_log->SFE_CED_rear_id_right    = ptr_ced_output->ced_object[FBK_SIDE_RIGHT].id;
      ptr_ced_output_udp_log->SFE_CED_rear_id_left     = ptr_ced_output->ced_object[FBK_SIDE_LEFT].id;
      ptr_ced_output_udp_log->SFE_CED_rear_ttc_right   = ptr_ced_output->ced_object[FBK_SIDE_RIGHT].ttc_s;
      ptr_ced_output_udp_log->SFE_CED_rear_ttc_left    = ptr_ced_output->ced_object[FBK_SIDE_LEFT].ttc_s;
      ptr_ced_output_udp_log->SFE_CED_rear_lat_right   = ptr_ced_output->ced_object[FBK_SIDE_RIGHT].predicted_lat_pos_m;
      ptr_ced_output_udp_log->SFE_CED_rear_lat_left    = ptr_ced_output->ced_object[FBK_SIDE_LEFT].predicted_lat_pos_m;
   }
   ptr_ced_output_udp_log->SFE_CED_front_status = ((ptr_ced_output->ced_object[FBK_SIDE_LEFT].direction == 2) || (ptr_ced_output->ced_object[FBK_SIDE_RIGHT].direction == 2));
   if (ptr_ced_output_udp_log->SFE_CED_front_status == 2) {
      ptr_ced_output_udp_log->SFE_CED_front_alert_right = ptr_ced_output->ced_alert[FBK_SIDE_RIGHT];
      ptr_ced_output_udp_log->SFE_CED_front_alert_left  = ptr_ced_output->ced_alert[FBK_SIDE_LEFT];
      ptr_ced_output_udp_log->SFE_CED_front_id_right    = ptr_ced_output->ced_object[FBK_SIDE_RIGHT].id;
      ptr_ced_output_udp_log->SFE_CED_front_id_left     = ptr_ced_output->ced_object[FBK_SIDE_LEFT].id;
      ptr_ced_output_udp_log->SFE_CED_front_ttc_right   = ptr_ced_output->ced_object[FBK_SIDE_RIGHT].ttc_s;
      ptr_ced_output_udp_log->SFE_CED_front_ttc_left    = ptr_ced_output->ced_object[FBK_SIDE_LEFT].ttc_s;
      ptr_ced_output_udp_log->SFE_CED_front_lat_right   = ptr_ced_output->ced_object[FBK_SIDE_RIGHT].predicted_lat_pos_m;
      ptr_ced_output_udp_log->SFE_CED_front_lat_left    = ptr_ced_output->ced_object[FBK_SIDE_LEFT].predicted_lat_pos_m;
   }
} /* End of CopyCedOutputToUdpLog() */

static void CopyPathTrackingOutputToUdpLog(
    RECU_Path_Tracking_Output_Udp_Log_T *ptr_pt_output_udp_log,
    Pt_Output_T *ptr_pt_output) {
   uint8_t index = 0;
   for (index = 0; index < PA_OBJ_NUMBER_OF_OBJECTS_UDP_LOG; index++) {
      ptr_pt_output_udp_log->nearest_path_output[index].range_vcs_proj_to_path_segment = ptr_pt_output->nearest_path_output[index].range_vcs_proj_to_path_segment;
      ptr_pt_output_udp_log->nearest_path_output[index].segment_heading_diff           = ptr_pt_output->nearest_path_output[index].segment_heading_diff;
      ptr_pt_output_udp_log->nearest_path_output[index].track_idx_nearest_path         = ptr_pt_output->nearest_path_output[index].track_idx_nearest_path;
      ptr_pt_output_udp_log->path_obj_pair_output[index].range_at_zero                 = ptr_pt_output->path_obj_pair_output[index].range_at_zero;
      ptr_pt_output_udp_log->path_obj_pair_output[index].range_to_current_path_part    = ptr_pt_output->path_obj_pair_output[index].range_to_current_path_part;
      ptr_pt_output_udp_log->path_obj_pair_output[index].range_at_host_edge            = ptr_pt_output->path_obj_pair_output[index].range_at_host_edge;
      ptr_pt_output_udp_log->path_obj_pair_output[index].length_of_trajectory          = ptr_pt_output->path_obj_pair_output[index].length_of_trajectory;
      ptr_pt_output_udp_log->path_obj_pair_output[index].path_heading                  = ptr_pt_output->path_obj_pair_output[index].path_heading;
      ptr_pt_output_udp_log->path_obj_pair_output[index].path_direction                = ptr_pt_output->path_obj_pair_output[index].path_direction;
      ptr_pt_output_udp_log->path_obj_pair_output[index].track_match                   = ptr_pt_output->path_obj_pair_output[index].track_match;
      ptr_pt_output_udp_log->path_obj_pair_output[index].track_match_age               = ptr_pt_output->path_obj_pair_output[index].track_match_age;
      ptr_pt_output_udp_log->path_obj_pair_output[index].track_match_last_cycle        = ptr_pt_output->path_obj_pair_output[index].track_match_last_cycle;
      ptr_pt_output_udp_log->f_pt_operational                                          = ptr_pt_output->f_pt_operational;
   }
} /* end of copy_path_tracking_output_to_udp_log() */

static void CopyScwOutputToUdpLog(RECU_Scw_Output_Udp_Log_T *ptr_scw_output_udp_log,
                                  Scw_Output_T *ptr_scw_output) {

   ptr_scw_output_udp_log->scw_object_px_left                     = ptr_scw_output->scw_object[FBK_SIDE_LEFT].position_m.x;
   ptr_scw_output_udp_log->scw_object_py_left                     = ptr_scw_output->scw_object[FBK_SIDE_LEFT].position_m.y;
   ptr_scw_output_udp_log->scw_object_width_left                  = ptr_scw_output->scw_object[FBK_SIDE_LEFT].width_m;
   ptr_scw_output_udp_log->scw_object_length_left                 = ptr_scw_output->scw_object[FBK_SIDE_LEFT].length_m;
   ptr_scw_output_udp_log->scw_object_heading_left                = ptr_scw_output->scw_object[FBK_SIDE_LEFT].heading_rad;
   ptr_scw_output_udp_log->scw_object_ttc_left                    = ptr_scw_output->scw_object[FBK_SIDE_LEFT].lateral_ttc_s;
   ptr_scw_output_udp_log->scw_object_vx_left                     = ptr_scw_output->scw_object[FBK_SIDE_LEFT].velocity_mps.x;
   ptr_scw_output_udp_log->scw_object_vy_left                     = ptr_scw_output->scw_object[FBK_SIDE_LEFT].velocity_mps.y;
   ptr_scw_output_udp_log->scw_object_ax_left                     = ptr_scw_output->scw_object[FBK_SIDE_LEFT].acceleration_mps2.x;
   ptr_scw_output_udp_log->scw_object_ay_left                     = ptr_scw_output->scw_object[FBK_SIDE_LEFT].acceleration_mps2.y;
   ptr_scw_output_udp_log->scw_object_px_right                    = ptr_scw_output->scw_object[FBK_SIDE_RIGHT].position_m.x;
   ptr_scw_output_udp_log->scw_object_py_right                    = ptr_scw_output->scw_object[FBK_SIDE_RIGHT].position_m.y;
   ptr_scw_output_udp_log->scw_object_width_right                 = ptr_scw_output->scw_object[FBK_SIDE_RIGHT].width_m;
   ptr_scw_output_udp_log->scw_object_length_right                = ptr_scw_output->scw_object[FBK_SIDE_RIGHT].length_m;
   ptr_scw_output_udp_log->scw_object_heading_right               = ptr_scw_output->scw_object[FBK_SIDE_RIGHT].heading_rad;
   ptr_scw_output_udp_log->scw_object_ttc_right                   = ptr_scw_output->scw_object[FBK_SIDE_RIGHT].lateral_ttc_s;
   ptr_scw_output_udp_log->scw_object_vx_right                    = ptr_scw_output->scw_object[FBK_SIDE_RIGHT].velocity_mps.x;
   ptr_scw_output_udp_log->scw_object_vy_right                    = ptr_scw_output->scw_object[FBK_SIDE_RIGHT].velocity_mps.y;
   ptr_scw_output_udp_log->scw_object_ax_right                    = ptr_scw_output->scw_object[FBK_SIDE_RIGHT].acceleration_mps2.x;
   ptr_scw_output_udp_log->scw_object_ay_right                    = ptr_scw_output->scw_object[FBK_SIDE_RIGHT].acceleration_mps2.y;
   ptr_scw_output_udp_log->scw_object_existance_probability_left  = static_cast<unsigned16_T>(ptr_scw_output->scw_object[FBK_SIDE_LEFT].existence_probability);
   ptr_scw_output_udp_log->scw_object_age_left                    = ptr_scw_output->scw_object[FBK_SIDE_LEFT].age;
   ptr_scw_output_udp_log->scw_object_existance_probability_right = static_cast<unsigned16_T>(ptr_scw_output->scw_object[FBK_SIDE_RIGHT].existence_probability);
   ptr_scw_output_udp_log->scw_object_age_right                   = ptr_scw_output->scw_object[FBK_SIDE_RIGHT].age;
   if (ptr_scw_output->scw_object[FBK_SIDE_LEFT].alert_level > 0 || ptr_scw_output->scw_object[FBK_SIDE_RIGHT].alert_level > 0) {
      ptr_scw_output_udp_log->f_scw_enabled           = true;
      ptr_scw_output_udp_log->f_scw_dyn_enabled       = true;
      ptr_scw_output_udp_log->f_scw_guardrail_enabled = true;
   } else {
      ptr_scw_output_udp_log->f_scw_enabled           = false;
      ptr_scw_output_udp_log->f_scw_dyn_enabled       = false;
      ptr_scw_output_udp_log->f_scw_guardrail_enabled = false;
   }
   ptr_scw_output_udp_log->scw_object_type_left  = ptr_scw_output->scw_object[FBK_SIDE_LEFT].type;
   ptr_scw_output_udp_log->scw_object_id_left    = ptr_scw_output->scw_object[FBK_SIDE_LEFT].id;
   ptr_scw_output_udp_log->scw_object_type_right = ptr_scw_output->scw_object[FBK_SIDE_RIGHT].type;
   ptr_scw_output_udp_log->scw_object_id_right   = ptr_scw_output->scw_object[FBK_SIDE_RIGHT].id;

} /* End of CopyScwOutputToUdpLog() */

static void CopyTaOutputToUdpLog(RECU_Ta_Output_Udp_Log_T *ptr_ta_output_udp_log,
                                 Ta_Output_T *ptr_ta_output) {

   if (ptr_ta_output->ta_alert_level[FBK_SIDE_LEFT] > 0) {
      ptr_ta_output_udp_log->fta_ttc                          = ptr_ta_output->ta_object[FBK_SIDE_LEFT].ta_ttc_s;
      ptr_ta_output_udp_log->ta_current_deceleration_estimate = ptr_ta_output->ta_object[FBK_SIDE_LEFT].ta_decel_estimate_mps2;
      ptr_ta_output_udp_log->fta_alert_level                  = ptr_ta_output->ta_alert_level[FBK_SIDE_LEFT];
      ptr_ta_output_udp_log->fta_target_id                    = ptr_ta_output->ta_object[FBK_SIDE_LEFT].ta_id;
   } else if (ptr_ta_output->ta_alert_level[FBK_SIDE_RIGHT] > 0) {
      ptr_ta_output_udp_log->fta_ttc                          = ptr_ta_output->ta_object[FBK_SIDE_RIGHT].ta_ttc_s;
      ptr_ta_output_udp_log->ta_current_deceleration_estimate = ptr_ta_output->ta_object[FBK_SIDE_RIGHT].ta_decel_estimate_mps2;
      ptr_ta_output_udp_log->fta_alert_level                  = ptr_ta_output->ta_alert_level[FBK_SIDE_RIGHT];
      ptr_ta_output_udp_log->fta_target_id                    = ptr_ta_output->ta_object[FBK_SIDE_RIGHT].ta_id;
   } else {
      ptr_ta_output_udp_log->fta_ttc                          = 0;
      ptr_ta_output_udp_log->ta_current_deceleration_estimate = 0;
      ptr_ta_output_udp_log->fta_alert_level                  = 0;
      ptr_ta_output_udp_log->fta_target_id                    = 0;
   }
   ptr_ta_output_udp_log->fta_brake_deceleration_request  = 0;
   ptr_ta_output_udp_log->ta_ttp_left                     = ptr_ta_output->ta_object[FBK_SIDE_LEFT].ta_ttp_s;
   ptr_ta_output_udp_log->ta_ttp_right                    = ptr_ta_output->ta_object[FBK_SIDE_RIGHT].ta_ttp_s;
   ptr_ta_output_udp_log->fta_target_gap                  = 0;
   ptr_ta_output_udp_log->rta_long_posn_left              = 0;
   ptr_ta_output_udp_log->rta_lat_posn_left               = 0;
   ptr_ta_output_udp_log->rta_long_posn_right             = 0;
   ptr_ta_output_udp_log->rta_lat_posn_right              = 0;
   ptr_ta_output_udp_log->rta_ttc_left                    = ptr_ta_output->ta_object[FBK_SIDE_LEFT].ta_ttc_s;
   ptr_ta_output_udp_log->rta_ttc_right                   = ptr_ta_output->ta_object[FBK_SIDE_RIGHT].ta_ttc_s;
   ptr_ta_output_udp_log->rta_long_vel_left               = 0;
   ptr_ta_output_udp_log->rta_lat_vel_left                = 0;
   ptr_ta_output_udp_log->rta_long_vel_right              = 0;
   ptr_ta_output_udp_log->rta_lat_vel_right               = 0;
   ptr_ta_output_udp_log->rta_existence_probability_left  = 0;
   ptr_ta_output_udp_log->rta_existence_probability_right = 0;
   ptr_ta_output_udp_log->ta_ttc_left                     = ptr_ta_output->ta_object[FBK_SIDE_LEFT].ta_ttc_s;
   ptr_ta_output_udp_log->ta_collision_probability_left   = 0;
   ptr_ta_output_udp_log->ta_ttc_right                    = ptr_ta_output->ta_object[FBK_SIDE_RIGHT].ta_ttc_s;
   ptr_ta_output_udp_log->fta_target_age                  = 0;
   ptr_ta_output_udp_log->fta_target_vel_long             = 0;
   ptr_ta_output_udp_log->fta_target_vel_lat              = 0;
   ptr_ta_output_udp_log->fta_target_exist_prob           = 0;
   ptr_ta_output_udp_log->fta_symbol_request              = 0;
   ptr_ta_output_udp_log->fta_brake_threshold_reduction   = 0;
   ptr_ta_output_udp_log->fta_brake_conditioning          = 0;
   ptr_ta_output_udp_log->f_fta_enable                    = ptr_ta_output->f_ta_enable;
   ptr_ta_output_udp_log->f_rta_enable                    = ptr_ta_output->f_ta_enable;
   ptr_ta_output_udp_log->f_rta_enable_turning_area       = 0;
   ptr_ta_output_udp_log->f_rta_enable_dynamic_area       = 0;
   ptr_ta_output_udp_log->fta_maneuver_direction          = 0;
   ptr_ta_output_udp_log->rta_dynamic_area_status         = 0;
   ptr_ta_output_udp_log->rta_turning_area_status         = 0;
   ptr_ta_output_udp_log->rta_alert_left                  = 0;
   ptr_ta_output_udp_log->rta_alert_right                 = 0;
   ptr_ta_output_udp_log->rta_id_left                     = ptr_ta_output->ta_object[FBK_SIDE_LEFT].ta_id;
   ptr_ta_output_udp_log->rta_id_right                    = ptr_ta_output->ta_object[FBK_SIDE_RIGHT].ta_id;
   ptr_ta_output_udp_log->ta_alert_left                   = 0;
   ptr_ta_output_udp_log->ta_id_left                      = ptr_ta_output->ta_object[FBK_SIDE_LEFT].ta_id;
   ptr_ta_output_udp_log->ta_alert_right                  = 0;
   ptr_ta_output_udp_log->ta_id_right                     = ptr_ta_output->ta_object[FBK_SIDE_LEFT].ta_id;
   ptr_ta_output_udp_log->ta_turn_maneuver                = 0;
   ptr_ta_output_udp_log->unused1                         = 0;
   ptr_ta_output_udp_log->lcda_warntrigger_hmi            = 0;
   ptr_ta_output_udp_log->f_diagnostic_mode               = 0;
   ptr_ta_output_udp_log->K_Unused16_1                    = 0;

   uint8_t index = 0;
   for (index = 0; index < N_FTA_OBJECTS; index++) {
      RECU_FTA_Object_List_Udp_Log_T *l_ptr_ta_output_udp_log = &(ptr_ta_output_udp_log->fta_relevant_object[index]);

      l_ptr_ta_output_udp_log->fta_obj_list_rcs                       = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_id                        = ptr_ta_output_udp_log->fta_target_id;
      l_ptr_ta_output_udp_log->fta_obj_list_age                       = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_meas_status               = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_move_status               = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_exist_prob                = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_ref_point                 = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_ref_pnt_long_posn         = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_ref_pnt_long_posn_std_dev = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_ref_pnt_lat_posn          = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_ref_pnt_lat_posn_std_dev  = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_covariance_posn           = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_yaw_angle                 = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_yaw_angle_std_dev         = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_long_vel                  = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_long_vel_std_dev          = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_lat_vel                   = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_lat_vel_std_dev           = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_covariance_vel            = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_long_accel                = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_long_accel_std_dev        = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_lat_accel                 = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_lat_accel_std_dev         = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_covariance_accel          = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_yawrate                   = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_yawrate_std_dev           = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_length                    = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_length_std_dev            = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_width                     = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_width_std_dev             = 0;
      l_ptr_ta_output_udp_log->fta_obj_list_object_class              = 0;
   }
} /* End of CopyTaOutputToUdpLog */

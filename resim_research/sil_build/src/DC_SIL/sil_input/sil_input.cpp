#include "sil_input.h"
#include <map>

static DC_INPUT_DATA_T dc_input_object;
static bool isFirstCall = true;
static DC_INPUT_DATA_T PrevIP;
static int seqIdx_fl = -1;
static int seqIdx_fr = -1;
static int seqIdx_rl = -1;
static int seqIdx_rr = -1;
static int seqIdx_fc = -1;

#define CHECK_AND_BUFFER(vecField, scanId, result)                                  \
   do {                                                                             \
      if (!(result)) {                                                              \
         if (getConfigParameters().DC_IP_DQ_Chk_Print == "ENABLE") {                \
            std::cout << "[ScanId: " << scanId << "] " << #vecField << " failed\n"; \
         }                                                                          \
         vecField.push_back(scanId);                                                \
      }                                                                             \
   } while (0)

DC_ValidChecks_T DataQualityChk = {};

std::map<ErrorCode, std::string> errorMessages = {
    {ErrHeaderSize, "HeaderSizeMismatch"},
    {ErrHeaderChecksum, "HeaderChecksumMismatch"},
    {ErrHeaderVersion, "HeaderVersionMismatch"}};

boolean_T ProcessInputData(SIL_DC_Input_Data_T *Input_Data_ptr, Run_Mode_T run_mode) {
   boolean_T ret_val = false;
   boolean_T isValid = true;
   std::vector<std::string> errors;

   if (Input_Data_ptr->Symbol_Record.raw_stream[SYM_BMW_DATA].payload != NULL) // add header size check and header checksum and structure version check
   {
      DC_INPUT_DATA_T *ptr     = (DC_INPUT_DATA_T *)(Input_Data_ptr->Symbol_Record.raw_stream[SYM_BMW_DATA].payload);
      auto header_size         = ptr->sil_ecu_data_input_hdr.ecu_sym_Size;
      auto header_checksum     = ptr->sil_ecu_data_input_hdr.ecu_sym_Checksum;
      auto version             = ptr->sil_ecu_data_input_hdr.ecu_sym_version;
      unsigned int offset      = sizeof(ptr->sil_ecu_data_input_hdr);
      auto calculated_checksum = CalcSum16_Z2((const unsigned8_T *)&ptr->SIL_Mode, ptr->sil_ecu_data_input_hdr.ecu_sym_Size - offset);
      const float deg2rad      = 0.0174532939f;

      if (header_size != sizeof(DC_INPUT_DATA_T)) {
         errors.push_back(errorMessages[ErrHeaderSize]);
         isValid = false;
      }
      if (header_checksum != calculated_checksum) {
         errors.push_back(errorMessages[ErrHeaderChecksum]);
         isValid = false;
      }
      if (version != SIL_ECU_DEBUG_INPUT_STRUCTURE_VERSION) {
         errors.push_back(errorMessages[ErrHeaderVersion]);
         isValid = false;
      }

      if (isValid) {
         memcpy((&dc_input_object), (Input_Data_ptr->Symbol_Record.raw_stream[SYM_BMW_DATA].payload), sizeof(DC_INPUT_DATA_T));

         if (run_mode == DETECTIONS_UDP) {

            float rear_axle_offset = std::abs(dc_input_object.vehicle_inputs.rear_axle_position);

            dc_input_object.Mounting_Values_FL.boresight_angle  = -dc_input_object.Mounting_Values_FL.boresight_angle * deg2rad;
            dc_input_object.Mounting_Values_FL.azimuth_polarity = -dc_input_object.Mounting_Values_FL.azimuth_polarity;
            dc_input_object.Mounting_Values_FL.vcs_lat_position = -dc_input_object.Mounting_Values_FL.vcs_lat_position;
            dc_input_object.Mounting_Values_FL.vcs_lon_position = dc_input_object.Mounting_Values_FL.vcs_lon_position - rear_axle_offset;

            dc_input_object.Mounting_Values_FR.boresight_angle  = -dc_input_object.Mounting_Values_FR.boresight_angle * deg2rad;
            dc_input_object.Mounting_Values_FR.azimuth_polarity = -dc_input_object.Mounting_Values_FR.azimuth_polarity;
            dc_input_object.Mounting_Values_FR.vcs_lat_position = -dc_input_object.Mounting_Values_FR.vcs_lat_position;
            dc_input_object.Mounting_Values_FR.vcs_lon_position = dc_input_object.Mounting_Values_FR.vcs_lon_position - rear_axle_offset;

            dc_input_object.Mounting_Values_RL.boresight_angle  = -dc_input_object.Mounting_Values_RL.boresight_angle * deg2rad;
            dc_input_object.Mounting_Values_RL.azimuth_polarity = -dc_input_object.Mounting_Values_RL.azimuth_polarity;
            dc_input_object.Mounting_Values_RL.vcs_lat_position = -dc_input_object.Mounting_Values_RL.vcs_lat_position;
            dc_input_object.Mounting_Values_RL.vcs_lon_position = dc_input_object.Mounting_Values_RL.vcs_lon_position - rear_axle_offset;

            dc_input_object.Mounting_Values_RR.boresight_angle  = -dc_input_object.Mounting_Values_RR.boresight_angle * deg2rad;
            dc_input_object.Mounting_Values_RR.azimuth_polarity = -dc_input_object.Mounting_Values_RR.azimuth_polarity;
            dc_input_object.Mounting_Values_RR.vcs_lat_position = -dc_input_object.Mounting_Values_RR.vcs_lat_position;
            dc_input_object.Mounting_Values_RR.vcs_lon_position = dc_input_object.Mounting_Values_RR.vcs_lon_position - rear_axle_offset;

            dc_input_object.Mounting_Values_FC.boresight_angle  = -dc_input_object.Mounting_Values_FC.boresight_angle * deg2rad;
            dc_input_object.Mounting_Values_FC.azimuth_polarity = -dc_input_object.Mounting_Values_FC.azimuth_polarity;
            dc_input_object.Mounting_Values_FC.vcs_lat_position = -dc_input_object.Mounting_Values_FC.vcs_lat_position;
            dc_input_object.Mounting_Values_FC.vcs_lon_position = dc_input_object.Mounting_Values_FC.vcs_lon_position - rear_axle_offset;
         } else {
            dc_input_object.Mounting_Values_FL.boresight_angle = dc_input_object.Mounting_Values_FL.boresight_angle * deg2rad;
            dc_input_object.Mounting_Values_FR.boresight_angle = dc_input_object.Mounting_Values_FR.boresight_angle * deg2rad;
            dc_input_object.Mounting_Values_RL.boresight_angle = dc_input_object.Mounting_Values_RL.boresight_angle * deg2rad;
            dc_input_object.Mounting_Values_RR.boresight_angle = dc_input_object.Mounting_Values_RR.boresight_angle * deg2rad;
            dc_input_object.Mounting_Values_FC.boresight_angle = dc_input_object.Mounting_Values_FC.boresight_angle * deg2rad;
         }
#ifdef SRR_DC
         memset(&dc_input_object.sym_detection_fc_radar, 0, sizeof(dc_input_object.sym_detection_fc_radar));
         if (dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex != 0) {
            SetRECUMUDPScanIndex(dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex);
            ret_val = true;
         } else if (dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex != 0) {
            SetRECUMUDPScanIndex(dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex);
            ret_val = true;
         } else if (dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex != 0) {
            SetRECUMUDPScanIndex(dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex);
            ret_val = true;
         } else if (dc_input_object.sym_detection_rr_radar.dets_info.ScanIndex != 0) {
            SetRECUMUDPScanIndex(dc_input_object.sym_detection_rr_radar.dets_info.ScanIndex);
            ret_val = true;
         } else {
            ret_val = false;
         }

#elif MRR_DC
         memset(&dc_input_object.sym_detection_fl_radar, 0, sizeof(dc_input_object.sym_detection_fl_radar));
         memset(&dc_input_object.sym_detection_fr_radar, 0, sizeof(dc_input_object.sym_detection_fr_radar));
         memset(&dc_input_object.sym_detection_rl_radar, 0, sizeof(dc_input_object.sym_detection_rl_radar));
         memset(&dc_input_object.sym_detection_rr_radar, 0, sizeof(dc_input_object.sym_detection_rr_radar));
         if (dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex != 0) {
            SetRECUMUDPScanIndex(dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex);
            ret_val = true;
         } else {
            ret_val = false;
         }
#endif
      }
   } else {
      ret_val = false;
   }

   if (!isValid && !errors.empty()) {
      for (const auto &message : errors) {
         std::cout << message << " ";
      }
   }

   ValidateScanIP();
   return ret_val;
}

void ValidateScanIP() {

   const int seq[]                                               = {3, 1, 2, 0};
   f360_variant_A::F360_Sensor_Calib_Log_Output_T *ptr_SensCalib = GetSensCalibPtr();
   int senstype                                                  = getConfigParameters().senstype;
   bool result;

   if (isFirstCall) {

      PrevIP      = dc_input_object;
      isFirstCall = false;
#ifdef SRR_DC

      for (int seqIdx = 0; seqIdx < 4; seqIdx++) {
         if (seq[seqIdx] == dc_input_object.sym_detection_fl_radar.dets_info.LookType) {
            seqIdx_fl = seqIdx;
            break;
         }
      }

      for (int seqIdx = 0; seqIdx < 4; seqIdx++) {
         if (seq[seqIdx] == dc_input_object.sym_detection_fr_radar.dets_info.LookType) {
            seqIdx_fr = seqIdx;
            break;
         }
      }

      for (int seqIdx = 0; seqIdx < 4; seqIdx++) {
         if (seq[seqIdx] == dc_input_object.sym_detection_rl_radar.dets_info.LookType) {
            seqIdx_rl = seqIdx;
            break;
         }
      }
      for (int seqIdx = 0; seqIdx < 4; seqIdx++) {
         if (seq[seqIdx] == dc_input_object.sym_detection_rr_radar.dets_info.LookType) {
            seqIdx_rr = seqIdx;
            break;
         }
      }
#elif MRR_DC
      for (int seqIdx = 0; seqIdx < 4; seqIdx++) {
         if (seq[seqIdx] == dc_input_object.sym_detection_fc_radar.dets_info.LookType) {
            seqIdx_fc = seqIdx;
            break;
         }
      }
#endif

      return;
   }

#ifdef SRR_DC
   int fl_ScanIdDiff = dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex - PrevIP.sym_detection_fl_radar.dets_info.ScanIndex;
   int fr_ScanIdDiff = dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex - PrevIP.sym_detection_fr_radar.dets_info.ScanIndex;
   int rl_ScanIdDiff = dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex - PrevIP.sym_detection_rl_radar.dets_info.ScanIndex;
   int rr_ScanIdDiff = dc_input_object.sym_detection_rr_radar.dets_info.ScanIndex - PrevIP.sym_detection_rr_radar.dets_info.ScanIndex;

   // ScanIdCheck
   CHECK_AND_BUFFER(DataQualityChk.ScanIdChk_fl, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, (fl_ScanIdDiff == 1) ? true : false);
   CHECK_AND_BUFFER(DataQualityChk.ScanIdChk_fr, dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex, (fr_ScanIdDiff == 1) ? true : false);
   CHECK_AND_BUFFER(DataQualityChk.ScanIdChk_rr, dc_input_object.sym_detection_rr_radar.dets_info.ScanIndex, (rr_ScanIdDiff == 1) ? true : false);
   CHECK_AND_BUFFER(DataQualityChk.ScanIdChk_rl, dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex, (rl_ScanIdDiff == 1) ? true : false);

   // Stale Data Check

   CHECK_AND_BUFFER(DataQualityChk.NoStaleDataChk_fl, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, (fl_ScanIdDiff == 0) ? false : true);
   CHECK_AND_BUFFER(DataQualityChk.NoStaleDataChk_fr, dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex, (fr_ScanIdDiff == 0) ? false : true);
   CHECK_AND_BUFFER(DataQualityChk.NoStaleDataChk_rl, dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex, (rl_ScanIdDiff == 0) ? false : true);
   CHECK_AND_BUFFER(DataQualityChk.NoStaleDataChk_rr, dc_input_object.sym_detection_rr_radar.dets_info.ScanIndex, (rr_ScanIdDiff == 0) ? false : true);

   // Look Idx Check

   result = GetLookIdCheck(dc_input_object.sym_detection_fl_radar.dets_info.LookType, dc_input_object.sym_detection_fl_radar.dets_info.LookID);
   CHECK_AND_BUFFER(DataQualityChk.LookIdChk_fl, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);
   result = GetLookIdCheck(dc_input_object.sym_detection_fr_radar.dets_info.LookType, dc_input_object.sym_detection_fr_radar.dets_info.LookID);
   CHECK_AND_BUFFER(DataQualityChk.LookIdChk_fr, dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex, result);
   result = GetLookIdCheck(dc_input_object.sym_detection_rl_radar.dets_info.LookType, dc_input_object.sym_detection_rl_radar.dets_info.LookID);
   CHECK_AND_BUFFER(DataQualityChk.LookIdChk_rl, dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex, result);
   result = GetLookIdCheck(dc_input_object.sym_detection_rr_radar.dets_info.LookType, dc_input_object.sym_detection_rr_radar.dets_info.LookID);
   CHECK_AND_BUFFER(DataQualityChk.LookIdChk_rr, dc_input_object.sym_detection_rr_radar.dets_info.ScanIndex, result);

   // ZeroDetectCheck
   result = (dc_input_object.sym_detection_fl_radar.dets_info.Count > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.ZeroDetectChk_fl, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);
   result = (dc_input_object.sym_detection_fr_radar.dets_info.Count > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.ZeroDetectChk_fr, dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex, result);
   result = (dc_input_object.sym_detection_rl_radar.dets_info.Count > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.ZeroDetectChk_rl, dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex, result);
   result = (dc_input_object.sym_detection_rr_radar.dets_info.Count > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.ZeroDetectChk_rr, dc_input_object.sym_detection_rr_radar.dets_info.ScanIndex, result);

   // LookType Check
   int expected_fl = seq[(seqIdx_fl + 1) % 4];
   int expected_fr = seq[(seqIdx_fr + 1) % 4];
   int expected_rl = seq[(seqIdx_rl + 1) % 4];
   int expected_rr = seq[(seqIdx_rr + 1) % 4];

   // printf("TimeInHeader = %d\n",dc_input_object.sym_detection_fl_radar.dets_info.headerTimestamp.seconds);
   seqIdx_fl = (seqIdx_fl + 1) % 4;
   seqIdx_fr = (seqIdx_fr + 1) % 4;
   seqIdx_rl = (seqIdx_rl + 1) % 4;
   seqIdx_rr = (seqIdx_rr + 1) % 4;

   result = (dc_input_object.sym_detection_fl_radar.dets_info.LookType == expected_fl) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.LookTypeChk_fl, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);
   result = (dc_input_object.sym_detection_fr_radar.dets_info.LookType == expected_fr) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.LookTypeChk_fr, dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex, result);
   result = (dc_input_object.sym_detection_rl_radar.dets_info.LookType == expected_rl) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.LookTypeChk_rl, dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex, result);
   result = (dc_input_object.sym_detection_rr_radar.dets_info.LookType == expected_rr) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.LookTypeChk_rr, dc_input_object.sym_detection_rr_radar.dets_info.ScanIndex, result);

   // FOV and vua check

   result = (dc_input_object.sym_detection_fl_radar.dets_input->range_rate < (ptr_SensCalib->sensor[senstype].min_aliaised_range_rate[0])) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VuaChk_fl, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);
   result = (dc_input_object.sym_detection_fl_radar.dets_input->range < ptr_SensCalib->sensor[senstype].range_limits[0]) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.FOVChk_fl, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);

   result = (dc_input_object.sym_detection_fr_radar.dets_input->range_rate < (ptr_SensCalib->sensor[senstype].min_aliaised_range_rate[1])) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VuaChk_fr, dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex, result);
   result = (dc_input_object.sym_detection_fr_radar.dets_input->range < ptr_SensCalib->sensor[senstype].range_limits[1]) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.FOVChk_fr, dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex, result);

   result = (dc_input_object.sym_detection_rl_radar.dets_input->range_rate < (ptr_SensCalib->sensor[senstype].min_aliaised_range_rate[2])) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VuaChk_rl, dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex, result);
   result = (dc_input_object.sym_detection_rl_radar.dets_input->range < ptr_SensCalib->sensor[senstype].range_limits[2]) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.FOVChk_rl, dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex, result);

   result = (dc_input_object.sym_detection_rr_radar.dets_input->range_rate < (ptr_SensCalib->sensor[senstype].min_aliaised_range_rate[3])) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VuaChk_rr, dc_input_object.sym_detection_rr_radar.dets_info.ScanIndex, result);
   result = (dc_input_object.sym_detection_rr_radar.dets_input->range < ptr_SensCalib->sensor[senstype].range_limits[3]) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.FOVChk_rr, dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex, result);

   // Elev Check
   result = ((dc_input_object.sym_detection_fl_radar.dets_input->elevation > ptr_SensCalib->sensor[senstype].fov_min_el_rad[0]) &&
             (dc_input_object.sym_detection_fl_radar.dets_input->elevation < ptr_SensCalib->sensor[senstype].fov_max_el_rad[0]))
                ? true
                : false;

   CHECK_AND_BUFFER(DataQualityChk.ElevChk_fl, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);

   result = ((dc_input_object.sym_detection_fr_radar.dets_input->elevation > ptr_SensCalib->sensor[senstype].fov_min_el_rad[1]) &&
             (dc_input_object.sym_detection_fr_radar.dets_input->elevation < ptr_SensCalib->sensor[senstype].fov_max_el_rad[1]))
                ? true
                : false;

   CHECK_AND_BUFFER(DataQualityChk.ElevChk_fr, dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex, result);

   result = ((dc_input_object.sym_detection_rl_radar.dets_input->elevation > ptr_SensCalib->sensor[senstype].fov_min_el_rad[2]) &&
             (dc_input_object.sym_detection_rl_radar.dets_input->elevation < ptr_SensCalib->sensor[senstype].fov_max_el_rad[2]))
                ? true
                : false;

   CHECK_AND_BUFFER(DataQualityChk.ElevChk_rl, dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex, result);

   result = ((dc_input_object.sym_detection_rr_radar.dets_input->elevation > ptr_SensCalib->sensor[senstype].fov_min_el_rad[3]) &&
             (dc_input_object.sym_detection_rr_radar.dets_input->elevation < ptr_SensCalib->sensor[senstype].fov_max_el_rad[3]))
                ? true
                : false;

   CHECK_AND_BUFFER(DataQualityChk.ElevChk_rr, dc_input_object.sym_detection_rr_radar.dets_info.ScanIndex, result);

   // Azimuth Check
   result = ((dc_input_object.sym_detection_fl_radar.dets_input->azimuth > ptr_SensCalib->sensor[senstype].fov_min_az_rad[0]) &&
             (dc_input_object.sym_detection_fl_radar.dets_input->azimuth < ptr_SensCalib->sensor[senstype].fov_max_az_rad[0]))
                ? true
                : false;

   CHECK_AND_BUFFER(DataQualityChk.AzimuthChk_fl, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);

   result = ((dc_input_object.sym_detection_fr_radar.dets_input->azimuth > ptr_SensCalib->sensor[senstype].fov_min_az_rad[1]) &&
             (dc_input_object.sym_detection_fr_radar.dets_input->azimuth < ptr_SensCalib->sensor[senstype].fov_max_az_rad[1]))
                ? true
                : false;

   CHECK_AND_BUFFER(DataQualityChk.AzimuthChk_fr, dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex, result);

   result = ((dc_input_object.sym_detection_rl_radar.dets_input->azimuth > ptr_SensCalib->sensor[senstype].fov_min_az_rad[2]) &&
             (dc_input_object.sym_detection_rl_radar.dets_input->azimuth < ptr_SensCalib->sensor[senstype].fov_max_az_rad[2]))
                ? true
                : false;

   CHECK_AND_BUFFER(DataQualityChk.AzimuthChk_rl, dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex, result);

   result = ((dc_input_object.sym_detection_rr_radar.dets_input->azimuth > ptr_SensCalib->sensor[senstype].fov_min_az_rad[3]) &&
             (dc_input_object.sym_detection_rr_radar.dets_input->azimuth < ptr_SensCalib->sensor[senstype].fov_max_az_rad[3]))
                ? true
                : false;

   CHECK_AND_BUFFER(DataQualityChk.AzimuthChk_rr, dc_input_object.sym_detection_rr_radar.dets_info.ScanIndex, result);

   float32_T TimeDiff_fl = float32_T(dc_input_object.sym_detection_fl_radar.dets_info.timestamp - PrevIP.sym_detection_fl_radar.dets_info.timestamp);
   float32_T TimeDiff_fr = float32_T(dc_input_object.sym_detection_fr_radar.dets_info.timestamp - PrevIP.sym_detection_fr_radar.dets_info.timestamp);
   float32_T TimeDiff_rl = float32_T(dc_input_object.sym_detection_rl_radar.dets_info.timestamp - PrevIP.sym_detection_rl_radar.dets_info.timestamp);
   float32_T TimeDiff_rr = float32_T(dc_input_object.sym_detection_rr_radar.dets_info.timestamp - PrevIP.sym_detection_rr_radar.dets_info.timestamp);

   // Overrun Check
   result = (TimeDiff_fl > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.OverrunChk_fl, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);
   result = (TimeDiff_fr > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.OverrunChk_fr, dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex, result);
   result = (TimeDiff_rl > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.OverrunChk_rl, dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex, result);
   result = (TimeDiff_rr > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.OverrunChk_rr, dc_input_object.sym_detection_rr_radar.dets_info.ScanIndex, result);

   TimeDiff_fl = (TimeDiff_fl < 0) ? (TimeDiff_fl + 1000) : TimeDiff_fl;
   TimeDiff_fr = (TimeDiff_fr < 0) ? (TimeDiff_fr + 1000) : TimeDiff_fr;
   TimeDiff_rl = (TimeDiff_rl < 0) ? (TimeDiff_rl + 1000) : TimeDiff_rl;
   TimeDiff_rr = (TimeDiff_rr < 0) ? (TimeDiff_rr + 1000) : TimeDiff_rr;

   // Periodic Check
   result = (TimeDiff_fl > 40 && TimeDiff_fl < 80) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.PeriodChk_fl, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);
   result = (TimeDiff_fr > 40 && TimeDiff_fr < 80) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.PeriodChk_fr, dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex, result);
   result = (TimeDiff_rl > 40 && TimeDiff_rl < 80) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.PeriodChk_rl, dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex, result);
   result = (TimeDiff_rr > 40 && TimeDiff_rr < 80) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.PeriodChk_rr, dc_input_object.sym_detection_rr_radar.dets_info.ScanIndex, result);

   // Diagnol Firing Check
   float32_T DiagTimefr_rl = dc_input_object.sym_detection_rl_radar.dets_info.timestamp - dc_input_object.sym_detection_fr_radar.dets_info.timestamp;
   float32_T DiagTimefr_rr = dc_input_object.sym_detection_rr_radar.dets_info.timestamp - dc_input_object.sym_detection_fr_radar.dets_info.timestamp;
   float32_T DiagTimefr_fl = dc_input_object.sym_detection_fl_radar.dets_info.timestamp - dc_input_object.sym_detection_fr_radar.dets_info.timestamp;

   DiagTimefr_rl = (DiagTimefr_rl < 0) ? (DiagTimefr_rl + 1000) : DiagTimefr_rl;
   DiagTimefr_rr = (DiagTimefr_rr < 0) ? (DiagTimefr_rr + 1000) : DiagTimefr_rr;
   DiagTimefr_fl = (DiagTimefr_fl < 0) ? (DiagTimefr_fl + 1000) : DiagTimefr_fl;

   bool DiagChkfrrl = ((DiagTimefr_rl > 7) && (DiagTimefr_rl < 10)) ? true : false;
   bool DiagChkfrrr = ((DiagTimefr_rr > 22) && (DiagTimefr_rr < 28)) ? true : false;
   bool DiagChkfrfl = ((DiagTimefr_fl > 30) && (DiagTimefr_rr < 35)) ? true : false;

   result = (DiagChkfrrl && DiagChkfrrr && DiagChkfrfl) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.DiagFiringChk, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);
   /*
   //Overall Time check
   result = (DiagChkfrfl < 80) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.OverallTimeChk,dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex,result);*/

   // Mounting Position Check

   result = ((dc_input_object.Mounting_Values_FL.vcs_lat_position < 0) &&
             (dc_input_object.Mounting_Values_FL.vcs_lon_position < 0))
                ? true
                : false;
   CHECK_AND_BUFFER(DataQualityChk.MntPosChk_fl, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);

   result = ((dc_input_object.Mounting_Values_FR.vcs_lat_position > 0) &&
             (dc_input_object.Mounting_Values_FR.vcs_lon_position < 0))
                ? true
                : false;
   CHECK_AND_BUFFER(DataQualityChk.MntPosChk_fr, dc_input_object.sym_detection_fr_radar.dets_info.ScanIndex, result);

   result = ((dc_input_object.Mounting_Values_RL.vcs_lon_position < 0) &&
             (dc_input_object.Mounting_Values_RL.vcs_lat_position < 0))
                ? true
                : false;
   CHECK_AND_BUFFER(DataQualityChk.MntPosChk_rl, dc_input_object.sym_detection_rl_radar.dets_info.ScanIndex, result);

   result = ((dc_input_object.Mounting_Values_RR.vcs_lon_position < 0) &&
             (dc_input_object.Mounting_Values_RR.vcs_lat_position > 0))
                ? true
                : false;
   CHECK_AND_BUFFER(DataQualityChk.MntPosChk_rr, dc_input_object.sym_detection_rr_radar.dets_info.ScanIndex, result);

   // Vehicle Info Check
   result = (dc_input_object.vehicle_inputs.abs_speed >= 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VehInfoSpd, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);

   result = (dc_input_object.vehicle_inputs.rear_axle_position < 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VehInfoRearAxle, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);

   result = (dc_input_object.vehicle_inputs.host_vehicle_length > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VehInfoLength, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);

   result = (dc_input_object.vehicle_inputs.host_vehicle_width > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VehInfoWidth, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);

   result = (dc_input_object.vehicle_inputs.host_vehicle_height > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VehInfoHeight, dc_input_object.sym_detection_fl_radar.dets_info.ScanIndex, result);

#elif MRR_DC

   int fc_ScanIdDiff = dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex - PrevIP.sym_detection_fc_radar.dets_info.ScanIndex;
   CHECK_AND_BUFFER(DataQualityChk.ScanIdChk_fc, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, (fc_ScanIdDiff == 1) ? true : false);
   CHECK_AND_BUFFER(DataQualityChk.NoStaleDataChk_fc, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, (fc_ScanIdDiff == 0) ? false : true);
   result = GetLookIdCheck(dc_input_object.sym_detection_fc_radar.dets_info.LookType, dc_input_object.sym_detection_fc_radar.dets_info.LookID);
   CHECK_AND_BUFFER(DataQualityChk.LookIdChk_fc, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, result);
   result = (dc_input_object.sym_detection_fc_radar.dets_info.Count > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.ZeroDetectChk_fc, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, result);
   int expected_fc = seq[(seqIdx_fc + 1) % 4];
   seqIdx_fc       = (seqIdx_fc + 1) % 4;
   result          = (dc_input_object.sym_detection_fc_radar.dets_info.LookType == expected_fc) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.LookTypeChk_fc, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, result);
   float32_T TimeDiff_fc = float32_T(dc_input_object.sym_detection_fc_radar.dets_info.timestamp - PrevIP.sym_detection_fc_radar.dets_info.timestamp);
   result                = (TimeDiff_fc > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.OverrunChk_fc, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, result);
   TimeDiff_fc = (TimeDiff_fc < 0) ? (TimeDiff_fc + 1000) : TimeDiff_fc;
   result      = (TimeDiff_fc > 40 && TimeDiff_fc < 80) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.PeriodChk_fc, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, result);
   result = ((abs(dc_input_object.Mounting_Values_FC.vcs_lon_position) < 2) &&
             (abs(dc_input_object.Mounting_Values_FC.vcs_lat_position) < 2))
                ? true
                : false;
   CHECK_AND_BUFFER(DataQualityChk.MntPosChk_fc, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, result);

   result = (dc_input_object.sym_detection_fc_radar.dets_input->range_rate < (ptr_SensCalib->sensor[senstype].min_aliaised_range_rate[0])) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VuaChk_fc, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, result);

   result = ((dc_input_object.sym_detection_fc_radar.dets_input->elevation > ptr_SensCalib->sensor[senstype].fov_min_el_rad[0]) &&
             (dc_input_object.sym_detection_fc_radar.dets_input->elevation < ptr_SensCalib->sensor[senstype].fov_max_el_rad[0]))
                ? true
                : false;

   CHECK_AND_BUFFER(DataQualityChk.ElevChk_fc, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, result);

   result = ((dc_input_object.sym_detection_fc_radar.dets_input->azimuth > ptr_SensCalib->sensor[senstype].fov_min_az_rad[0]) &&
             (dc_input_object.sym_detection_fc_radar.dets_input->azimuth < ptr_SensCalib->sensor[senstype].fov_max_az_rad[0]))
                ? true
                : false;

   CHECK_AND_BUFFER(DataQualityChk.AzimuthChk_fc, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, result);

   // Vehicle Info Check
   result = (dc_input_object.vehicle_inputs.abs_speed >= 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VehInfoSpd, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, result);

   result = (dc_input_object.vehicle_inputs.rear_axle_position < 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VehInfoRearAxle, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, result);

   result = (dc_input_object.vehicle_inputs.host_vehicle_length > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VehInfoLength, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, result);

   result = (dc_input_object.vehicle_inputs.host_vehicle_width > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VehInfoWidth, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, result);

   result = (dc_input_object.vehicle_inputs.host_vehicle_height > 0) ? true : false;
   CHECK_AND_BUFFER(DataQualityChk.VehInfoHeight, dc_input_object.sym_detection_fc_radar.dets_info.ScanIndex, result);
#endif

   {
      f360_variant_A::F360_Core_Info_T *core = GetF360CoreInfo();
      bool elapsed_zero                      = false;
      if (core != nullptr && core->cnt_loops > 1 && core->elapsed_time_s == 0.0) {
         elapsed_zero = true;
      }
      uint16_t scanId = GetScanIndex();
      CHECK_AND_BUFFER(DataQualityChk.ZeroElapsedChk, scanId, !elapsed_zero);
   }

   PrevIP = dc_input_object;
   return;
}

bool GetLookIdCheck(int LookType, int LookId) {
   bool CheckLukId = false;

   if (LookType < 2 && LookId == 2) {
      CheckLukId = true;
   } else if (LookType > 1 && LookType < 4 && LookId == 1) {
      CheckLukId = true;
   } else {
      CheckLukId = false;
   }
   return CheckLukId;
}

void ResetIdxBuffer() {

   isFirstCall = true;
   seqIdx_fl   = -1;
   seqIdx_fr   = -1;
   seqIdx_rl   = -1;
   seqIdx_rr   = -1;
   seqIdx_fc   = -1;
}
// calculate checksum
unsigned16_T CalcSum16_Z2(const unsigned8_T *Ptr, unsigned Len) {
   unsigned16_T Sum = 0u;
   while (Len--) {
      const unsigned8_T Byte = *Ptr++;
      Sum += Byte;
   }
   return Sum;
}

DC_INPUT_DATA_T *GetDCInputdata() {
   return &dc_input_object;
}
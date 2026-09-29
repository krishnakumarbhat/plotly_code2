#define RESIM_LIB_INC
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <direct.h>
#endif
#ifdef __GNUC__
#include <sys/stat.h>
#endif
#include <iomanip>
#include <numeric>
#include <sstream>
#include <string.h>
#include <time.h>
#include <vector>

#include "pugiconfig.hpp"
#include "pugixml.hpp"

#include "Event_Logger.h"

#include "Tracker_VariantA_Wrapper.h"
#include "ced_iface.h"
#include "cta_iface.h"
#include "dc_app_version.h"
#include "esa_iface.h"
#include "fbk_iface.h"
#include "lcda_iface.h"
#include "ltb_iface.h"
#include "pt_iface.h"
#include "recw_iface.h"
#include "scw_iface.h"
#include "sil_input.h"
#include "ta_iface.h"
#include "DGPS_Decoder.h"

static FILE *SIL_XML_In   = NULL;
static FILE *SIL_XML_Out  = NULL;
static FILE *OPP_DETS_In  = NULL;
static FILE *OPP_DETS_Out = NULL;
FILE *SIL_XML_out         = NULL;
FILE *SIL_SOMIP_XML       = NULL;
FILE *SIL_SOMIP_INPUT_XML = NULL;
FILE *SIL_DIAG_XML        = NULL;

extern signed32_T CORE0_time;
extern signed32_T CORE1_time;
extern signed32_T CORE3_time;
extern signed32_T Tracker_Timing_info;
extern signed32_T RECW_Timing_info;
extern signed32_T CTA_Timing_info;
extern signed32_T LCDA_Timing_info;
extern signed32_T TA_Timing_info;
extern signed32_T SFE_Timing_info;
extern signed32_T SCW_Timing_info;
extern signed32_T CED_Timing_info;
extern unsigned16_T Tracker_index;

static unsigned8_T flag_dets = 0;
static unsigned8_T flag_cdc  = 0;
char pathname_XML[255]       = {0};
char *token                  = NULL;
char *token_1                = NULL;
char *token_2                = NULL;
// char* token_3 = NULL;

static uint8_T obj_properties_count = 0, recw_out_properties_count = 0, cta_out_properties_count[Max_CTA_Warn_Alert_Status] = {0};
DC_Statistic_T Statistic_ptr                                             = {0};
PFGS_Object_Properties_T PFGS_Obj_Properties_SI                          = {0};
RECW_Output_Properties_T RECW_Out_Properties_SI                          = {0};
CTA_Output_Properties_T CTA_Out_Properties_SI[Max_CTA_Warn_Alert_Status] = {0};
pugi::xml_node *staticreportXML;
FF_alerts_warns_statistics_T FF_alerts_warns_statistics;
FF_alerts_warns_statistics_T *FF_alerts_warns_statistics_ptr = Get_FF_alerts_warns_statistics_ptr();

extern std::vector<signed32_T> vecCORE0_time;
extern std::vector<signed32_T> vecCORE1_time;
extern std::vector<signed32_T> vecVRU_time;
extern std::vector<signed32_T> vecCORE3_time;
std::vector<float32_T> vecVeh_Speed;
std::vector<float32_T> vecYaw_rate;
int log_count = 0;
extern "C"
{
   u32p0_T Get_ND_UTC(void);
}

extern "C" void Extract_PFGS_Object_Properties_For_Statistic(void);
extern "C" void Extract_RECW_Output_Properties_For_Statistic(void);
extern "C" void Extract_CTA_Output_Properties_For_Statistic(void);
extern "C" unsigned32_T Get_LaneMarking_Msgs_PLPTS_Failure(void);
extern "C" unsigned32_T Get_LaneMarkingNext_Msg_PLPTS_Failure(void);

extern Radar_ECU_CORE0_T *Get_Previous_Cycle_CORE0_Ptr(void);
extern Fbk_Output_T *Feature_Function_Get_Output_ptr();
extern "C" Radar_ECU_CORE1_T *Get_Radar_CORE1_Ptr(void);
/** Number of detections per sensor*/
#define NUMBER_OF_DETECTIONS (200)
/** Number of fused objects over the entire field of view*/
#define NUMBER_OF_OBJECTS (50)

std::vector<uint16_T> UniqTrack;

extern "C" Cta_Output_T *Cta_Get_Output_Ptr(void);
extern "C" Ta_Output_T *Ta_Get_Output_Ptr(void);
extern "C" Scw_Output_T *Scw_Get_Output_Ptr(void);
extern "C" Recw_Output_T *Recw_Get_Output_Ptr(void);
extern "C" Ltb_Output_T *Ltb_Get_Output_Ptr(void);
extern "C" Lcda_Output_T *Lcda_Get_Output_Ptr(void);
extern "C" Esa_Output_T *Esa_Get_Output_Ptr(void);
extern "C" Ced_Output_T *Ced_Get_Output_Ptr(void);
extern "C" Pt_Output_T *Pt_Get_Output_Ptr(void);

std::string vecToStr(const std::vector<uint16_t> &vec) {
   std::ostringstream oss;
   for (size_t i = 0; i < vec.size(); ++i) {
      oss << vec[i];
      if (i != vec.size() - 1)
         oss << " ";
   }
   return oss.str();
}

int appendIfNotPresent(uint16_T value) {
   auto it = std::find(UniqTrack.begin(), UniqTrack.end(), value);

   if (it == UniqTrack.end()) {
      UniqTrack.push_back(value);
      return true; // newly added
   } else {

      return false; // already present
   }
}

void AppendSideNodes(pugi::xml_node &parent, const std::string &tagName,
                     const std::vector<uint16_t> &fl,
                     const std::vector<uint16_t> &fr,
                     const std::vector<uint16_t> &rl,
                     const std::vector<uint16_t> &rr) {
   pugi::xml_node checkNode = parent.append_child(tagName.c_str());

   checkNode.append_child("FL").append_attribute("ScanId") = vecToStr(fl).c_str();
   checkNode.append_child("FR").append_attribute("ScanId") = vecToStr(fr).c_str();
   checkNode.append_child("RL").append_attribute("ScanId") = vecToStr(rl).c_str();
   checkNode.append_child("RR").append_attribute("ScanId") = vecToStr(rr).c_str();
}

void AppendSideNodesFC(pugi::xml_node &parent, const std::string &tagName,
                       const std::vector<uint16_t> &fc) {
   pugi::xml_node checkNode                                = parent.append_child(tagName.c_str());
   checkNode.append_child("FC").append_attribute("ScanId") = vecToStr(fc).c_str();
}

void Open_XML_file(FILE *Filename) {
   SIL_XML_out = Filename;
}

void Write_statistic_Data(void *pLogFolder, bool endFile) {
   staticreportXML    = (pugi::xml_node *)pLogFolder;
   signed32_T maxTime = 0;
   pugi::xml_node DC_info;
   if (endFile == false) {
#ifdef SRR_DC
      DC_info = staticreportXML->append_child("SRR_DC_Statistic_Info");
#elif MRR_DC
      DC_info = staticreportXML->append_child("MRR_DC_Statistic_Info");
#endif
   } else if (endFile == true) {
      if (staticreportXML->child("DC_OverAll_Statistic"))
         staticreportXML->remove_child("DC_OverAll_Statistic");

      DC_info = staticreportXML->append_child("DC_OverAll_Statistic");
   }
   if (endFile == false) {
      log_count++;

      pugi::xml_node RESIM_SW_Version = DC_info.append_child("RESIM_SW_Version");

      RESIM_SW_Version.append_attribute("Major_Version") = APPLICATION_MAJOR_VERSION;
      RESIM_SW_Version.append_attribute("Minor_Version") = APPLICATION_MINOR_VERSION;
      RESIM_SW_Version.append_attribute("Patch_Version") = APPLICATION_PATCH_VERSION;
   }

   Load_Vehicle_Dyn_into_Statistic();
   pugi::xml_node DataQuality = DC_info.append_child("DC_Input_DataQuality_Check");

#ifdef SRR_DC
   AppendSideNodes(DataQuality, "ScanIndex_Check",
                   DataQualityChk.ScanIdChk_fl,
                   DataQualityChk.ScanIdChk_fr,
                   DataQualityChk.ScanIdChk_rl,
                   DataQualityChk.ScanIdChk_rr);

   AppendSideNodes(DataQuality, "LookId_Check",
                   DataQualityChk.LookIdChk_fl,
                   DataQualityChk.LookIdChk_fr,
                   DataQualityChk.LookIdChk_rl,
                   DataQualityChk.LookIdChk_rr);

   AppendSideNodes(DataQuality, "LookType_Check",
                   DataQualityChk.LookTypeChk_fl,
                   DataQualityChk.LookTypeChk_fr,
                   DataQualityChk.LookTypeChk_rl,
                   DataQualityChk.LookTypeChk_rr);

   AppendSideNodes(DataQuality, "StaleData_Check",
                   DataQualityChk.NoStaleDataChk_fl,
                   DataQualityChk.NoStaleDataChk_fr,
                   DataQualityChk.NoStaleDataChk_rl,
                   DataQualityChk.NoStaleDataChk_rr);

   AppendSideNodes(DataQuality, "FOV_Check",
                   DataQualityChk.FOVChk_fl,
                   DataQualityChk.FOVChk_fr,
                   DataQualityChk.FOVChk_rl,
                   DataQualityChk.FOVChk_rr);

   AppendSideNodes(DataQuality, "vua_Check",
                   DataQualityChk.VuaChk_fl,
                   DataQualityChk.VuaChk_fr,
                   DataQualityChk.VuaChk_rl,
                   DataQualityChk.VuaChk_rr);

   AppendSideNodes(DataQuality, "ZeroDetect_Check",
                   DataQualityChk.ZeroDetectChk_fl,
                   DataQualityChk.ZeroDetectChk_fr,
                   DataQualityChk.ZeroDetectChk_rl,
                   DataQualityChk.ZeroDetectChk_rr);

   AppendSideNodes(DataQuality, "Period_Check",
                   DataQualityChk.PeriodChk_fl,
                   DataQualityChk.PeriodChk_fr,
                   DataQualityChk.PeriodChk_rl,
                   DataQualityChk.PeriodChk_rr);

   AppendSideNodes(DataQuality, "Overrun_Check",
                   DataQualityChk.OverrunChk_fl,
                   DataQualityChk.OverrunChk_fr,
                   DataQualityChk.OverrunChk_rl,
                   DataQualityChk.OverrunChk_rr);

   AppendSideNodes(DataQuality, "Elevation_Check",
                   DataQualityChk.ElevChk_fl,
                   DataQualityChk.ElevChk_fr,
                   DataQualityChk.ElevChk_rl,
                   DataQualityChk.ElevChk_rr);

   AppendSideNodes(DataQuality, "Azimuth_Check",
                   DataQualityChk.AzimuthChk_fl,
                   DataQualityChk.AzimuthChk_fr,
                   DataQualityChk.AzimuthChk_rl,
                   DataQualityChk.AzimuthChk_rr);

   AppendSideNodes(DataQuality, "MountPos_Check",
                   DataQualityChk.MntPosChk_fl,
                   DataQualityChk.MntPosChk_fr,
                   DataQualityChk.MntPosChk_rl,
                   DataQualityChk.MntPosChk_rr);

   pugi::xml_node checkNode                                         = DataQuality.append_child("VehInfo_Check");
   checkNode.append_child("Speed").append_attribute("ScanId")       = vecToStr(DataQualityChk.VehInfoSpd).c_str();
   checkNode.append_child("RearAxlePos").append_attribute("ScanId") = vecToStr(DataQualityChk.VehInfoRearAxle).c_str();
   checkNode.append_child("Width").append_attribute("ScanId")       = vecToStr(DataQualityChk.VehInfoWidth).c_str();
   checkNode.append_child("Length").append_attribute("ScanId")      = vecToStr(DataQualityChk.VehInfoLength).c_str();
   checkNode.append_child("Height").append_attribute("ScanId")      = vecToStr(DataQualityChk.VehInfoHeight).c_str();

   pugi::xml_node DiagFiringChk             = DataQuality.append_child("DiagonalFiring_Check");
   DiagFiringChk.append_attribute("ScanId") = vecToStr(DataQualityChk.DiagFiringChk).c_str();

   pugi::xml_node OverallRunTimeChk             = DataQuality.append_child("OverallRunTime_Check");
   OverallRunTimeChk.append_attribute("ScanId") = vecToStr(DataQualityChk.OverallTimeChk).c_str();

#elif MRR_DC

   AppendSideNodesFC(DataQuality, "ScanIndex_Check",
                     DataQualityChk.ScanIdChk_fc);
   AppendSideNodesFC(DataQuality, "LookId_Check",
                     DataQualityChk.LookIdChk_fc);
   AppendSideNodesFC(DataQuality, "LookType_Check",
                     DataQualityChk.LookTypeChk_fc);
   AppendSideNodesFC(DataQuality, "StaleData_Check",
                     DataQualityChk.NoStaleDataChk_fc);
   AppendSideNodesFC(DataQuality, "FOV_Check",
                     DataQualityChk.FOVChk_fc);
   AppendSideNodesFC(DataQuality, "vua_Check",
                     DataQualityChk.VuaChk_fc);
   AppendSideNodesFC(DataQuality, "ZeroDetect_Check",
                     DataQualityChk.ZeroDetectChk_fc);
   AppendSideNodesFC(DataQuality, "Period_Check",
                     DataQualityChk.PeriodChk_fc);
   AppendSideNodesFC(DataQuality, "Overrun_Check",
                     DataQualityChk.OverrunChk_fc);
   AppendSideNodesFC(DataQuality, "Elevation_Check",
                     DataQualityChk.ElevChk_fc);
   AppendSideNodesFC(DataQuality, "Azimuth_Check",
                     DataQualityChk.AzimuthChk_fc);
   AppendSideNodesFC(DataQuality, "MountPos_Check",
                     DataQualityChk.MntPosChk_fc);

   pugi::xml_node checkNode                                         = DataQuality.append_child("VehInfo_Check");
   checkNode.append_child("Speed").append_attribute("ScanId")       = vecToStr(DataQualityChk.VehInfoSpd).c_str();
   checkNode.append_child("RearAxlePos").append_attribute("ScanId") = vecToStr(DataQualityChk.VehInfoRearAxle).c_str();
   checkNode.append_child("Width").append_attribute("ScanId")       = vecToStr(DataQualityChk.VehInfoWidth).c_str();
   checkNode.append_child("Length").append_attribute("ScanId")      = vecToStr(DataQualityChk.VehInfoLength).c_str();
   checkNode.append_child("Height").append_attribute("ScanId")      = vecToStr(DataQualityChk.VehInfoHeight).c_str();
#endif

   /* zero elapsed time scans collected during Run_Dia_Statistic - common for SRR and MRR */
   pugi::xml_node zeroNode             = DataQuality.append_child("ZeroElapsedtime_Check");
   zeroNode.append_attribute("ScanId") = vecToStr(DataQualityChk.ZeroElapsedChk).c_str();

   pugi::xml_node Vehicle_Dynamic_info    = DC_info.append_child("Vehicle_Dynamics");
   pugi::xml_node Vehicle_Dyn_vehicle_log = Vehicle_Dynamic_info.append_child("Vehicle_Log");
   // sprintf(vehdist_header, "%f m, %f km", Statistic_ptr.Vehicle_Dynamic_Data.TotalDistLog_m, Statistic_ptr.Vehicle_Dynamic_Data.TotalDistLog_km);

   std::string vehdist_header = std::to_string(Statistic_ptr.Vehicle_Dynamic_Data.TotalDistLog_m) + " m, " + std::to_string(Statistic_ptr.Vehicle_Dynamic_Data.TotalDistLog_km) + " km";
   if (endFile == false) {
      Vehicle_Dyn_vehicle_log.append_attribute("ReverseGear_Count") = Statistic_ptr.Vehicle_Dynamic_Data.reverseGearcnt;

      std::string maxVeh_speed_header = std::to_string(Statistic_ptr.Vehicle_Dynamic_Data.max_vehspeed_m) + " m/s, " + std::to_string(Statistic_ptr.Vehicle_Dynamic_Data.max_vehspeed_m * 3.6) + " km/hr";
      std::string minVeh_speed_header = std::to_string(Statistic_ptr.Vehicle_Dynamic_Data.min_vehspeed_m) + " m/s, " + std::to_string(Statistic_ptr.Vehicle_Dynamic_Data.min_vehspeed_m * 3.6) + " km/hr";

      Vehicle_Dyn_vehicle_log.append_attribute("Max_VehSpeed") = maxVeh_speed_header.c_str();
      Vehicle_Dyn_vehicle_log.append_attribute("Min_VehSpeed") = minVeh_speed_header.c_str();
      Vehicle_Dyn_vehicle_log.append_attribute("Max_YawRate")  = Statistic_ptr.Vehicle_Dynamic_Data.max_yawrate;
      Vehicle_Dyn_vehicle_log.append_attribute("Min_YawRate")  = Statistic_ptr.Vehicle_Dynamic_Data.min_yawrate;
      Vehicle_Dyn_vehicle_log.append_attribute("Avg_YawRate")  = Statistic_ptr.Vehicle_Dynamic_Data.avg_yawrate;
   }
   if (endFile == true) {
      // sprintf(avgVeh_speed_header, "%f m, %f km", (Statistic_ptr.Vehicle_Dynamic_Data.avgSpeedLog_m / log_count), (Statistic_ptr.Vehicle_Dynamic_Data.avgSpeedLog_m / log_count) * 3.6);

      std::string avgVeh_speed_header = std::to_string(Statistic_ptr.Vehicle_Dynamic_Data.avgSpeedLog_m) + " m/s, " + std::to_string(Statistic_ptr.Vehicle_Dynamic_Data.avgSpeedLog_m * 3.6) + " km/hr";
   }
   Vehicle_Dyn_vehicle_log.append_attribute("Dist_Travelled") = vehdist_header.c_str();
   std::string Speed_Range_in_hr_min_sec;
   ConvertToHr_sec_msee(Statistic_ptr.Vehicle_Dynamic_Data.inputcycle_count, &Speed_Range_in_hr_min_sec);
   Vehicle_Dyn_vehicle_log.append_attribute("Time_Duration") = Speed_Range_in_hr_min_sec.c_str();

   pugi::xml_node VSE_Count_Info                                      = DC_info.append_child("VSE_Diagnostic_Info");
   pugi::xml_node vsecountInfo                                        = VSE_Count_Info.append_child("Resim_Data");
   vsecountInfo.append_attribute("Veh_speed_over_ground_qf_NA_count") = Statistic_ptr.Vse_Output_Count.filt_veh_speed_over_ground_qf_count;
   vsecountInfo.append_attribute("Comp_yaw_rate_qf_NA_count")         = Statistic_ptr.Vse_Output_Count.comp_yaw_rate_qf_count;
   vsecountInfo.append_attribute("Raw_lat_accel_qf_NA_count")         = Statistic_ptr.Vse_Output_Count.raw_lat_accel_qf_count;
   vsecountInfo.append_attribute("Raw_long_accel_qf_NA_count")        = Statistic_ptr.Vse_Output_Count.raw_long_accel_qf_count;

   pugi::xml_node Tracker_info                            = DC_info.append_child("TRACKS_COUNT_INFORMATION");
   pugi::xml_node resim_Info                              = Tracker_info.append_child("Resim_Data");
   resim_Info.append_attribute("Total_Track_Count")       = Statistic_ptr.Tracker_Output_Data.totalTrackCount;
   resim_Info.append_attribute("Car_Count")               = Statistic_ptr.Tracker_Output_Data.Car_count;
   resim_Info.append_attribute("Truck_Count")             = Statistic_ptr.Tracker_Output_Data.Truck_count;
   resim_Info.append_attribute("TwoWheelTrack_count")     = Statistic_ptr.Tracker_Output_Data.TwoWheelTrack_count;
   resim_Info.append_attribute("Pedestrian_object_count") = Statistic_ptr.Tracker_Output_Data.pedestrainTrack_count;
   resim_Info.append_attribute("unknownTrack_count")      = Statistic_ptr.Tracker_Output_Data.unknownTrack_count;

// Vehicle_Dyn_vehicle_log.append_attribute("Avg_VehSpeed") = avgVeh_speed_header;
#if SRR_DC
   pugi::xml_node Feature_info = DC_info.append_child("Feature_Function_Info");

   /*CTA FF Info*/
   pugi::xml_node CTA_info                              = Feature_info.append_child("CTA");
   pugi::xml_node CTA_RESIM_Info                        = CTA_info.append_child("Resim_Info");
   pugi::xml_node CTA_RESIM_Data                        = CTA_RESIM_Info.append_child("Resim_Data");
   CTA_RESIM_Data.append_attribute("CTA_L_Warn_Count")  = Statistic_ptr.Features_Output_Data.CTA_left_warn_count;
   CTA_RESIM_Data.append_attribute("CTA_R_Warn_Count")  = Statistic_ptr.Features_Output_Data.CTA_right_warn_count;
   CTA_RESIM_Data.append_attribute("CTA_L_Alert_Count") = Statistic_ptr.Features_Output_Data.CTA_left_alert_count;
   CTA_RESIM_Data.append_attribute("CTA_R_Alert_Count") = Statistic_ptr.Features_Output_Data.CTA_right_alert_count;

   if (endFile == false) { /*Statistic Report writes for CTA Left Warn Sequence properties each split file*/
      unsigned16_T sequence_count = 0;
      if (Statistic_ptr.Features_Output_Data.CTA_left_warn_count < 1) {
         sequence_count = 1;
      } else {
         sequence_count = Statistic_ptr.Features_Output_Data.CTA_left_warn_count;
      }
      for (unsigned16_T i = 0; i < sequence_count; i++) {
         if (i < MAX_CTA_OUTPUT_PROPERTIES_COUNT) {
            pugi::xml_node CTA_Left_Warn_Properties_node             = CTA_RESIM_Info.append_child("RESIM_CTA_Left_Warn_Sequence_Properties_Data");
            CTA_Left_Warn_Properties_node.append_attribute("Number") = i + 1;

            pugi::xml_node CTA_Left_Warn_Core_Output                               = CTA_Left_Warn_Properties_node.append_child("Core_Output_Data");
            CTA_Left_Warn_Core_Output.append_attribute("Ego_Speed")                = Statistic_ptr.CTA_Output_Properties[CTA_Left_Warn][i].Ego_Speed;
            CTA_Left_Warn_Core_Output.append_attribute("CTA_Target_L_ID")          = Statistic_ptr.CTA_Output_Properties[CTA_Left_Warn][i].cta_id_left;
            CTA_Left_Warn_Core_Output.append_attribute("CTA_Target_R_ID")          = Statistic_ptr.CTA_Output_Properties[CTA_Left_Warn][i].cta_id_right;
            CTA_Left_Warn_Core_Output.append_attribute("CTA_TTC")                  = Statistic_ptr.CTA_Output_Properties[CTA_Left_Warn][i].most_critical_object_ttc;
            CTA_Left_Warn_Core_Output.append_attribute("CTA_L_Intersection_Point") = Statistic_ptr.CTA_Output_Properties[CTA_Left_Warn][i].Intersectionpoint_x_left;
            CTA_Left_Warn_Core_Output.append_attribute("CTA_R_Intersection_Point") = Statistic_ptr.CTA_Output_Properties[CTA_Left_Warn][i].Intersectionpoint_x_right;
            CTA_Left_Warn_Core_Output.append_attribute("CTB_Brake_Request")        = Statistic_ptr.CTA_Output_Properties[CTA_Left_Warn][i].RCTB_brake_request;
         } else {
            /*Sequence count reached to MAX Limit*/
            break;
         }
      }

      /*Statistic Report writes for CTA Right Warn Sequence properties each split file*/
      sequence_count = 0;
      if (Statistic_ptr.Features_Output_Data.CTA_right_warn_count < 1) {
         sequence_count = 1;
      } else {
         sequence_count = Statistic_ptr.Features_Output_Data.CTA_right_warn_count;
      }
      for (unsigned16_T i = 0; i < sequence_count; i++) {
         if (i < MAX_CTA_OUTPUT_PROPERTIES_COUNT) {
            pugi::xml_node CTA_Right_Warn_Properties_node             = CTA_RESIM_Info.append_child("RESIM_CTA_Right_Warn_Sequence_Properties_Data");
            CTA_Right_Warn_Properties_node.append_attribute("Number") = i + 1;

            pugi::xml_node CTA_Right_Warn_Core_Output                               = CTA_Right_Warn_Properties_node.append_child("Core_Output_Data");
            CTA_Right_Warn_Core_Output.append_attribute("Ego_Speed")                = Statistic_ptr.CTA_Output_Properties[CTA_Right_Warn][i].Ego_Speed;
            CTA_Right_Warn_Core_Output.append_attribute("CTA_Target_L_ID")          = Statistic_ptr.CTA_Output_Properties[CTA_Right_Warn][i].cta_id_left;
            CTA_Right_Warn_Core_Output.append_attribute("CTA_Target_R_ID")          = Statistic_ptr.CTA_Output_Properties[CTA_Right_Warn][i].cta_id_right;
            CTA_Right_Warn_Core_Output.append_attribute("CTA_TTC")                  = Statistic_ptr.CTA_Output_Properties[CTA_Right_Warn][i].most_critical_object_ttc;
            CTA_Right_Warn_Core_Output.append_attribute("CTA_L_Intersection_Point") = Statistic_ptr.CTA_Output_Properties[CTA_Right_Warn][i].Intersectionpoint_x_left;
            CTA_Right_Warn_Core_Output.append_attribute("CTA_R_Intersection_Point") = Statistic_ptr.CTA_Output_Properties[CTA_Right_Warn][i].Intersectionpoint_x_right;
            CTA_Right_Warn_Core_Output.append_attribute("CTB_Brake_Request")        = Statistic_ptr.CTA_Output_Properties[CTA_Right_Warn][i].RCTB_brake_request;
         } else {
            /*Sequence count reached to MAX Limit*/
            break;
         }
      }

      /*Statistic Report writes for CTA Left Alert Sequence properties each split file*/
      sequence_count = 0;
      if (Statistic_ptr.Features_Output_Data.CTA_left_alert_count < 1) {
         sequence_count = 1;
      } else {
         sequence_count = Statistic_ptr.Features_Output_Data.CTA_left_alert_count;
      }
      for (unsigned16_T i = 0; i < sequence_count; i++) {
         if (i < MAX_CTA_OUTPUT_PROPERTIES_COUNT) {
            pugi::xml_node CTA_Left_Alert_Properties_node             = CTA_RESIM_Info.append_child("RESIM_CTA_Left_Alert_Sequence_Properties_Data");
            CTA_Left_Alert_Properties_node.append_attribute("Number") = i + 1;

            pugi::xml_node CTA_Left_Alert_Core_Output                               = CTA_Left_Alert_Properties_node.append_child("Core_Output_Data");
            CTA_Left_Alert_Core_Output.append_attribute("Ego_Speed")                = Statistic_ptr.CTA_Output_Properties[CTA_Left_Alert][i].Ego_Speed;
            CTA_Left_Alert_Core_Output.append_attribute("CTA_Target_L_ID")          = Statistic_ptr.CTA_Output_Properties[CTA_Left_Alert][i].cta_id_left;
            CTA_Left_Alert_Core_Output.append_attribute("CTA_Target_R_ID")          = Statistic_ptr.CTA_Output_Properties[CTA_Left_Alert][i].cta_id_right;
            CTA_Left_Alert_Core_Output.append_attribute("CTA_TTC")                  = Statistic_ptr.CTA_Output_Properties[CTA_Left_Alert][i].most_critical_object_ttc;
            CTA_Left_Alert_Core_Output.append_attribute("CTA_L_Intersection_Point") = Statistic_ptr.CTA_Output_Properties[CTA_Left_Alert][i].Intersectionpoint_x_left;
            CTA_Left_Alert_Core_Output.append_attribute("CTA_R_Intersection_Point") = Statistic_ptr.CTA_Output_Properties[CTA_Left_Alert][i].Intersectionpoint_x_right;
            CTA_Left_Alert_Core_Output.append_attribute("CTB_Brake_Request")        = Statistic_ptr.CTA_Output_Properties[CTA_Left_Alert][i].RCTB_brake_request;
         } else {
            /*Sequence count reached to MAX Limit*/
            break;
         }
      }

      /*Statistic Report writes for CTA Right Alert Sequence properties each split file*/
      sequence_count = 0;
      if (Statistic_ptr.Features_Output_Data.CTA_right_alert_count < 1) {
         sequence_count = 1;
      } else {
         sequence_count = Statistic_ptr.Features_Output_Data.CTA_right_alert_count;
      }
      for (unsigned16_T i = 0; i < sequence_count; i++) {
         if (i < MAX_CTA_OUTPUT_PROPERTIES_COUNT) {
            pugi::xml_node CTA_Right_Alert_Properties_node             = CTA_RESIM_Info.append_child("RESIM_CTA_Right_Alert_Sequence_Properties_Data");
            CTA_Right_Alert_Properties_node.append_attribute("Number") = i + 1;

            pugi::xml_node CTA_Right_Alert_Core_Output                               = CTA_Right_Alert_Properties_node.append_child("Core_Output_Data");
            CTA_Right_Alert_Core_Output.append_attribute("Ego_Speed")                = Statistic_ptr.CTA_Output_Properties[CTA_Right_Alert][i].Ego_Speed;
            CTA_Right_Alert_Core_Output.append_attribute("CTA_Target_L_ID")          = Statistic_ptr.CTA_Output_Properties[CTA_Right_Alert][i].cta_id_left;
            CTA_Right_Alert_Core_Output.append_attribute("CTA_Target_R_ID")          = Statistic_ptr.CTA_Output_Properties[CTA_Right_Alert][i].cta_id_right;
            CTA_Right_Alert_Core_Output.append_attribute("CTA_TTC")                  = Statistic_ptr.CTA_Output_Properties[CTA_Right_Alert][i].most_critical_object_ttc;
            CTA_Right_Alert_Core_Output.append_attribute("CTA_L_Intersection_Point") = Statistic_ptr.CTA_Output_Properties[CTA_Right_Alert][i].Intersectionpoint_x_left;
            CTA_Right_Alert_Core_Output.append_attribute("CTA_R_Intersection_Point") = Statistic_ptr.CTA_Output_Properties[CTA_Right_Alert][i].Intersectionpoint_x_right;
            CTA_Right_Alert_Core_Output.append_attribute("CTB_Brake_Request")        = Statistic_ptr.CTA_Output_Properties[CTA_Right_Alert][i].RCTB_brake_request;
         } else {
            /*Sequence count reached to MAX Limit*/
            break;
         }
      }
   }

   /* CED FF Info*/
   pugi::xml_node CED_Info                                     = Feature_info.append_child("CED");
   pugi::xml_node CED_RESIM_Info                               = CED_Info.append_child("Resim_Data");
   CED_RESIM_Info.append_attribute("CED_L_Level1_Alert_Count") = Statistic_ptr.Features_Output_Data.CED_left_alert_level1_count;
   CED_RESIM_Info.append_attribute("CED_L_Level2_Alert_Count") = Statistic_ptr.Features_Output_Data.CED_left_alert_level2_count;

   CED_RESIM_Info.append_attribute("CED_FL_Level1_Alert_Count") = Statistic_ptr.Features_Output_Data.CED_FL_alert_level1_count;
   CED_RESIM_Info.append_attribute("CED_FL_Level2_Alert_Count") = Statistic_ptr.Features_Output_Data.CED_FL_alert_level2_count;

   CED_RESIM_Info.append_attribute("CED_FR_Level1_Alert_Count") = Statistic_ptr.Features_Output_Data.CED_FR_alert_level1_count;
   CED_RESIM_Info.append_attribute("CED_FR_Level2_Alert_Count") = Statistic_ptr.Features_Output_Data.CED_FR_alert_level2_count;

   CED_RESIM_Info.append_attribute("CED_R_Level1_Alert_Count") = Statistic_ptr.Features_Output_Data.CED_right_alert_level1_count;
   CED_RESIM_Info.append_attribute("CED_R_Level2_Alert_Count") = Statistic_ptr.Features_Output_Data.CED_right_alert_level2_count;

   CED_RESIM_Info.append_attribute("CED_RL_Level1_Alert_Count") = Statistic_ptr.Features_Output_Data.CED_RL_alert_level1_count;
   CED_RESIM_Info.append_attribute("CED_RL_Level2_Alert_Count") = Statistic_ptr.Features_Output_Data.CED_RL_alert_level2_count;

   CED_RESIM_Info.append_attribute("CED_RR_Level1_Alert_Count") = Statistic_ptr.Features_Output_Data.CED_RR_alert_level1_count;
   CED_RESIM_Info.append_attribute("CED_RR_Level2_Alert_Count") = Statistic_ptr.Features_Output_Data.CED_RR_alert_level2_count;

   pugi::xml_node ESA_Info                                      = Feature_info.append_child("ESA");
   pugi::xml_node ESA_resim_info                                = ESA_Info.append_child("Resim_Data");
   ESA_resim_info.append_attribute("ESA_left_alert_count")      = FF_alerts_warns_statistics_ptr->ESA_left_alert_count;
   ESA_resim_info.append_attribute("ESA_right_alert_count")     = FF_alerts_warns_statistics_ptr->ESA_right_alert_count;
   ESA_resim_info.append_attribute("ESA_left_id")               = FF_alerts_warns_statistics_ptr->ESA_left_id;
   ESA_resim_info.append_attribute("ESA_left_index")            = FF_alerts_warns_statistics_ptr->ESA_left_index;
   ESA_resim_info.append_attribute("ESA_left_alert")            = FF_alerts_warns_statistics_ptr->ESA_left_alert;
   ESA_resim_info.append_attribute("ESA_left_ttc_s")            = FF_alerts_warns_statistics_ptr->ESA_left_ttc_s;
   ESA_resim_info.append_attribute("ESA_left_ttp_s")            = FF_alerts_warns_statistics_ptr->ESA_left_ttp_s;
   ESA_resim_info.append_attribute("ESA_left_width_m")          = FF_alerts_warns_statistics_ptr->ESA_left_width_m;
   ESA_resim_info.append_attribute("ESA_left_length_m")         = FF_alerts_warns_statistics_ptr->ESA_left_length_m;
   ESA_resim_info.append_attribute("ESA_left_long_pos_m")       = FF_alerts_warns_statistics_ptr->ESA_left_long_pos_m;
   ESA_resim_info.append_attribute("ESA_left_lat_pos_m")        = FF_alerts_warns_statistics_ptr->ESA_left_lat_pos_m;
   ESA_resim_info.append_attribute("ESA_left_long_speed_mps")   = FF_alerts_warns_statistics_ptr->ESA_left_long_speed_mps;
   ESA_resim_info.append_attribute("ESA_left_lat_speed_mps")    = FF_alerts_warns_statistics_ptr->ESA_left_lat_speed_mps;
   ESA_resim_info.append_attribute("ESA_left_long_distance_m")  = FF_alerts_warns_statistics_ptr->ESA_left_long_distance_m;
   ESA_resim_info.append_attribute("ESA_left_existence_prob")   = FF_alerts_warns_statistics_ptr->ESA_left_existence_prob;
   ESA_resim_info.append_attribute("ESA_right_id")              = FF_alerts_warns_statistics_ptr->ESA_right_id;
   ESA_resim_info.append_attribute("ESA_right_index")           = FF_alerts_warns_statistics_ptr->ESA_right_index;
   ESA_resim_info.append_attribute("ESA_right_alert")           = FF_alerts_warns_statistics_ptr->ESA_right_alert;
   ESA_resim_info.append_attribute("ESA_right_ttc_s")           = FF_alerts_warns_statistics_ptr->ESA_right_ttc_s;
   ESA_resim_info.append_attribute("ESA_right_ttp_s")           = FF_alerts_warns_statistics_ptr->ESA_right_ttp_s;
   ESA_resim_info.append_attribute("ESA_right_width_m")         = FF_alerts_warns_statistics_ptr->ESA_right_width_m;
   ESA_resim_info.append_attribute("ESA_right_length_m")        = FF_alerts_warns_statistics_ptr->ESA_right_length_m;
   ESA_resim_info.append_attribute("ESA_right_long_pos_m")      = FF_alerts_warns_statistics_ptr->ESA_right_long_pos_m;
   ESA_resim_info.append_attribute("ESA_right_lat_pos_m")       = FF_alerts_warns_statistics_ptr->ESA_right_lat_pos_m;
   ESA_resim_info.append_attribute("ESA_right_long_speed_mps")  = FF_alerts_warns_statistics_ptr->ESA_right_long_speed_mps;
   ESA_resim_info.append_attribute("ESA_right_lat_speed_mps")   = FF_alerts_warns_statistics_ptr->ESA_right_lat_speed_mps;
   ESA_resim_info.append_attribute("ESA_right_long_distance_m") = FF_alerts_warns_statistics_ptr->ESA_right_long_distance_m;
   ESA_resim_info.append_attribute("ESA_right_existence_prob")  = FF_alerts_warns_statistics_ptr->ESA_right_existence_prob;

   pugi::xml_node LTB_Info                                         = Feature_info.append_child("LTB");
   pugi::xml_node LTB_RESIM_Info                                   = LTB_Info.append_child("Resim_Data");
   LTB_RESIM_Info.append_attribute("LTB_Left_alert_count")         = FF_alerts_warns_statistics_ptr->LTB_Left_alert_count;
   LTB_RESIM_Info.append_attribute("LTB_Right_alert_count")        = FF_alerts_warns_statistics_ptr->LTB_Right_alert_count;
   LTB_RESIM_Info.append_attribute("LTB_Left_alert_level1_count")  = FF_alerts_warns_statistics_ptr->LTB_Left_alert_level1_count;
   LTB_RESIM_Info.append_attribute("LTB_Left_alert_level2_count")  = FF_alerts_warns_statistics_ptr->LTB_Left_alert_level2_count;
   LTB_RESIM_Info.append_attribute("LTB_Right_alert_level1_count") = FF_alerts_warns_statistics_ptr->LTB_Right_alert_level1_count;
   LTB_RESIM_Info.append_attribute("LTB_Right_alert_level2_count") = FF_alerts_warns_statistics_ptr->LTB_Right_alert_level2_count;

   pugi::xml_node lcda_Info                                               = Feature_info.append_child("LCDA");
   pugi::xml_node lcda_bsw_Info                                           = lcda_Info.append_child("BSW");
   pugi::xml_node lcda_bsw_RESIM_info                                     = lcda_bsw_Info.append_child("Resim_Data");
   lcda_bsw_RESIM_info.append_attribute("LCDA_BSW_L_Alert_Level_1_Count") = Statistic_ptr.Features_Output_Data.lcda_bsw_Left_alert_Level1_count;
   lcda_bsw_RESIM_info.append_attribute("LCDA_BSW_L_Alert_Level_2_Count") = Statistic_ptr.Features_Output_Data.lcda_bsw_Left_alert_Level2_count;

   lcda_bsw_RESIM_info.append_attribute("LCDA_BSW_R_Alert_Level_1_Count") = Statistic_ptr.Features_Output_Data.lcda_bsw_Right_alert_Level1_count;
   lcda_bsw_RESIM_info.append_attribute("LCDA_BSW_R_Alert_Level_2_Count") = Statistic_ptr.Features_Output_Data.lcda_bsw_Right_alert_Level2_count;

   pugi::xml_node lcda_cvw_Info                                           = lcda_Info.append_child("CVW");
   pugi::xml_node lcda_cvw_RESIM_info                                     = lcda_cvw_Info.append_child("Resim_Data");
   lcda_cvw_RESIM_info.append_attribute("LCDA_CVW_L_Alert_Level_1_Count") = Statistic_ptr.Features_Output_Data.lcda_cvw_Left_alert_Level1_count;
   lcda_cvw_RESIM_info.append_attribute("LCDA_CVW_L_Alert_Level_2_Count") = Statistic_ptr.Features_Output_Data.lcda_cvw_Left_alert_Level2_count;

   lcda_cvw_RESIM_info.append_attribute("LCDA_CVW_R_Alert_Level_1_Count") = Statistic_ptr.Features_Output_Data.lcda_cvw_Right_alert_Level1_count;
   lcda_cvw_RESIM_info.append_attribute("LCDA_CVW_R_Alert_Level_2_Count") = Statistic_ptr.Features_Output_Data.lcda_cvw_Right_alert_Level2_count;

   pugi::xml_node lcda_slc_Info                                           = lcda_Info.append_child("SLC");
   pugi::xml_node lcda_slc_RESIM_info                                     = lcda_slc_Info.append_child("Resim_Data");
   lcda_slc_RESIM_info.append_attribute("LCDA_SLC_L_Alert_Level_1_Count") = Statistic_ptr.Features_Output_Data.lcda_slc_Left_alert_Level1_count;
   lcda_slc_RESIM_info.append_attribute("LCDA_SLC_L_Alert_Level_2_Count") = Statistic_ptr.Features_Output_Data.lcda_slc_Left_alert_Level2_count;

   lcda_slc_RESIM_info.append_attribute("LCDA_SLC_R_Alert_Level_1_Count") = Statistic_ptr.Features_Output_Data.lcda_slc_Right_alert_Level1_count;
   lcda_slc_RESIM_info.append_attribute("LCDA_SLC_R_Alert_Level_2_Count") = Statistic_ptr.Features_Output_Data.lcda_slc_Right_alert_Level2_count;

   pugi::xml_node RECW_info                            = Feature_info.append_child("RECW");
   pugi::xml_node RECW_Data                            = RECW_info.append_child("Resim_Data");
   RECW_Data.append_attribute("RECW_Alert_Count")      = Statistic_ptr.Features_Output_Data.RECW_Alert_count;
   RECW_Data.append_attribute("RECW_Acute_Warn_Count") = Statistic_ptr.Features_Output_Data.RECW_Acute_warn_count;

   if (endFile == false) {
      unsigned16_T sequence_count = 0;
      if (Statistic_ptr.Features_Output_Data.RECW_Alert_count < 2) {
         sequence_count = 2;
      } else {
         sequence_count = Statistic_ptr.Features_Output_Data.RECW_Alert_count;
      }
      for (unsigned16_T i = 0; i < sequence_count; i++) {
         if (i < MAX_RECW_OUTPUT_PROPERTIES_COUNT) {
            pugi::xml_node RECW_Alert_Properties_node             = RECW_info.append_child("RESIM_RECW_Alert_Sequence_Properties_Data");
            RECW_Alert_Properties_node.append_attribute("Number") = i + 1;

            pugi::xml_node RECW_Core_Output                                 = RECW_Alert_Properties_node.append_child("Core_Output_Data");
            RECW_Core_Output.append_attribute("Ego_Speed")                  = Statistic_ptr.RECW_Output_Properties[i].Ego_speed;
            RECW_Core_Output.append_attribute("RECW_Crash_Probability")     = Statistic_ptr.RECW_Output_Properties[i].recw_crash_probability;
            RECW_Core_Output.append_attribute("RECW_TTC")                   = Statistic_ptr.RECW_Output_Properties[i].recw_ttc;
            RECW_Core_Output.append_attribute("RECW_TTC_Warning_Threshold") = Statistic_ptr.RECW_Output_Properties[i].recw_ttc_warning_threshold;
            RECW_Core_Output.append_attribute("RECW_Obj_Speed")             = Statistic_ptr.RECW_Output_Properties[i].recw_obj_speed;
            RECW_Core_Output.append_attribute("RECW_Obj_Long_Pos")          = Statistic_ptr.RECW_Output_Properties[i].recw_obj_long_pos;
            RECW_Core_Output.append_attribute("RECW_Obj_Lat_Pos")           = Statistic_ptr.RECW_Output_Properties[i].recw_obj_lat_pos;
            RECW_Core_Output.append_attribute("RECW_Obj_Heading")           = Statistic_ptr.RECW_Output_Properties[i].recw_obj_heading;
            RECW_Core_Output.append_attribute("RECW_Overlap")               = Statistic_ptr.RECW_Output_Properties[i].recw_overlap;
         } else {
            /*Sequence count reached to MAX Limit*/
            break;
         }
      }
   }

   pugi::xml_node SCW_Info                                   = Feature_info.append_child("SCW");
   pugi::xml_node SCW_RESIM_Info                             = SCW_Info.append_child("Resim_Data");
   SCW_RESIM_Info.append_attribute("SCW_L_Dyn_Alert_Count")  = Statistic_ptr.Features_Output_Data.scw_left_dyn_alert_count;
   SCW_RESIM_Info.append_attribute("SCW_L_Guad_Alert_Count") = Statistic_ptr.Features_Output_Data.scw_left_Gaud_alert_count;
   SCW_RESIM_Info.append_attribute("SCW_R_Dyn_Alert_Count")  = Statistic_ptr.Features_Output_Data.scw_right_dyn_alert_count;
   SCW_RESIM_Info.append_attribute("SCW_R_Guad_Alert_Count") = Statistic_ptr.Features_Output_Data.scw_right_Gaud_alert_count;

#endif

   /* DGPS Diagnostic Information */
   pugi::xml_node DGPS_info       = DC_info.append_child("DGPS_Diagnostic_Info");
   pugi::xml_node DGPS_Resim_Data = DGPS_info.append_child("Timestamp_Data");
#ifdef SRR_DC
   DGPS_Resim_Data.append_attribute("Zero_Timestamp_Count") = GetZeroTimestampCountSRR();
#elif MRR_DC
   DGPS_Resim_Data.append_attribute("Zero_Timestamp_Count") = GetZeroTimestampCountMRR();
#endif
}

/*****************************************************************************
 *. Name: Update_Vehicle_Dynamic
 *.    Count the Vehicel speed, Yawrate & Reverse gear of Vehicle info
 *.
 *. Parameters: Vehicle_Dynamic_ptr
 *.
 *. Return Value: None
 *.
 *. Shared Variables: None.
 *.
 *. Design Information:
 *.    (xxx is version no.)
 *.    (xxx is version no.)
 *.
 *. SCR Information:
 *.
 ******************************************************************************/

void Update_Vehicle_Dynamic(Vehicle_Dynamic_Data_T *Vehicle_Dynamic_ptr) {
   f360_variant_A::F360_Host_Raw_T *ptr_VehicleInfo = GetHostRawInfo();
   unsigned8_T f_reverse_gear                       = ptr_VehicleInfo->reverse_gear;
   switch (f_reverse_gear) {
   case 0:
      Vehicle_Dynamic_ptr->noreverseGearcnt++;
      break;
   case 1:
      Vehicle_Dynamic_ptr->reverseGearcnt++;
      break;
   default:
      Vehicle_Dynamic_ptr->invalid_gear++;
      break;
   }
   Vehicle_Dynamic_ptr->inputcycle_count++;
   if (ptr_VehicleInfo->raw_speed != 0) {
      vecVeh_Speed.push_back(ptr_VehicleInfo->raw_speed);
   }
   if (ptr_VehicleInfo->raw_yaw_rate_rad != 0) {
      vecYaw_rate.push_back(ptr_VehicleInfo->raw_yaw_rate_rad);
   }
}

/*****************************************************************************
 *. Name: Update_Vehicle_Speed
 *.    Count the Vehicel speed count based on the range interms of Kpmh
 *.
 *. Parameters: vehicle_speed
 *.
 *. Return Value: None
 *.
 *. Shared Variables: None.
 *.
 *. Design Information:
 *.    (xxx is version no.)
 *.    (xxx is version no.)
 *.
 *. SCR Information:
 *.
 ******************************************************************************/
void Update_Vehicle_Speed(Vehicle_Speed_Data_T *vehicle_speed) {
   f360_variant_A::F360_Host_Raw_T *ptr_VehicleInfo = GetHostRawInfo();
   float32_T Max_Vehicle_Speed_kmph                 = 0;
   /*converting the Vehicle speed form mts to kmph*/
   Max_Vehicle_Speed_kmph = ptr_VehicleInfo->raw_speed * 3.6f;
   if (Max_Vehicle_Speed_kmph == 0) {
      vehicle_speed->Zero_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 0) && (Max_Vehicle_Speed_kmph <= 3)) {
      vehicle_speed->Zero_to_Three_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 3) && (Max_Vehicle_Speed_kmph <= 5)) {
      vehicle_speed->Three_to_Five_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 5) && (Max_Vehicle_Speed_kmph <= 10)) {
      vehicle_speed->Five_to_Ten_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 10) && (Max_Vehicle_Speed_kmph <= 15)) {
      vehicle_speed->Ten_to_Fifteen_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 15) && (Max_Vehicle_Speed_kmph <= 20)) {
      vehicle_speed->Fifteen_to_Twety_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 20) && (Max_Vehicle_Speed_kmph <= 25)) {
      vehicle_speed->Twety_to_TwetyFive_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 25) && (Max_Vehicle_Speed_kmph <= 30)) {
      vehicle_speed->TwetyFive_to_thirty_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 30) && (Max_Vehicle_Speed_kmph <= 35)) {
      vehicle_speed->Thirty_to_ThirtyFive_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 35) && (Max_Vehicle_Speed_kmph <= 50)) {
      vehicle_speed->ThirtyFive_to_Fifty_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 50) && (Max_Vehicle_Speed_kmph <= 70)) {
      vehicle_speed->Fifty_to_Seventy_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 70) && (Max_Vehicle_Speed_kmph <= 85)) {
      vehicle_speed->Seventy_to_EightyFive_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 85) && (Max_Vehicle_Speed_kmph <= 100)) {
      vehicle_speed->EightyFive_to_OneHundred_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 100) && (Max_Vehicle_Speed_kmph <= 150)) {
      vehicle_speed->OneHundred_to_OneHundredFifty_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 150) && (Max_Vehicle_Speed_kmph <= 200)) {
      vehicle_speed->OneHundredFifty_to_TwoHundred_kph_speed_range_count++;
   } else if ((Max_Vehicle_Speed_kmph > 200)) {
      vehicle_speed->Greater_Than_TwoHundred_kph_speed_range_count++;
   } else {
      /*Do nothing*/
   }
}
void Update_ZeroElapsedTime() {
   f360_variant_A::F360_Core_Info_T *core = GetF360CoreInfo();
   if (core == nullptr)
      return;
   if (core->cnt_loops > 1 && core->elapsed_time_s == (0.0)) {
      uint16_t scanId = GetScanIndex();
      if (std::find(DataQualityChk.ZeroElapsedChk.begin(), DataQualityChk.ZeroElapsedChk.end(), scanId) == DataQualityChk.ZeroElapsedChk.end()) {
         DataQualityChk.ZeroElapsedChk.push_back(scanId);
      }
   }
}

/*****************************************************************************
 *. Name: Run_Dia_Statistic
 *.    This API will call the respective the Feature & Tracker (input & output) counts API's
 *.
 *. Parameters: Statistic_Call
 *.
 *. Return Value: None
 *.
 *. Shared Variables: None.
 *.
 *. Design Information:
 *.    (xxx is version no.)
 *.    (xxx is version no.)
 *.
 *. SCR Information:
 *.
 ******************************************************************************/
void Run_Dia_Statistic() {
   Update_ZeroElapsedTime();
   Update_Vehicle_Dynamic(&Statistic_ptr.Vehicle_Dynamic_Data);
   Update_Vehicle_Speed(&Statistic_ptr.Vehicle_Speed);
   Vse_Output(&Statistic_ptr.Vse_Output_Count);
   Statistic_Tracker(&Statistic_ptr.Tracker_Output_Data);
   Update_Feature_Statistic_Data(&Statistic_ptr.Features_Output_Data);
}

void Load_Vehicle_Dyn_into_Statistic() {
   Float32_T Total_veh_speed = 0, Total_yaw_rate = 0;
   if (vecVeh_Speed.size() != 0) {
      Total_veh_speed                                    = std::accumulate(vecVeh_Speed.cbegin(), vecVeh_Speed.cend(), Float32_T(0.0f));
      Statistic_ptr.Vehicle_Dynamic_Data.avgSpeedLog_m   = Total_veh_speed / vecVeh_Speed.size();
      Statistic_ptr.Vehicle_Dynamic_Data.TotalDistLog_m  = (((float32_T)Statistic_ptr.Vehicle_Dynamic_Data.avgSpeedLog_m * 50 * Statistic_ptr.Vehicle_Dynamic_Data.inputcycle_count) / 1000);
      Statistic_ptr.Vehicle_Dynamic_Data.TotalDistLog_km = Statistic_ptr.Vehicle_Dynamic_Data.TotalDistLog_m * 0.001f;
      Statistic_ptr.Vehicle_Dynamic_Data.max_vehspeed_m  = *std::max_element(vecVeh_Speed.begin(), vecVeh_Speed.end());
      Statistic_ptr.Vehicle_Dynamic_Data.min_vehspeed_m  = *std::min_element(vecVeh_Speed.begin(), vecVeh_Speed.end());
   }
   if (vecYaw_rate.size() != 0) {
      Statistic_ptr.Vehicle_Dynamic_Data.max_yawrate = *std::max_element(vecYaw_rate.begin(), vecYaw_rate.end());
      Statistic_ptr.Vehicle_Dynamic_Data.min_yawrate = *std::min_element(vecYaw_rate.begin(), vecYaw_rate.end());
      Total_yaw_rate                                 = std::accumulate(vecYaw_rate.cbegin(), vecYaw_rate.cend(), Float32_T(0.0f));
      Statistic_ptr.Vehicle_Dynamic_Data.avg_yawrate = Total_yaw_rate / vecYaw_rate.size();
   }
   vecVeh_Speed.clear();
   vecYaw_rate.clear();
}

void Update_Feature_Statistics() {
   Esa_Output_T *esa_output_ptr = Esa_Get_Output_Ptr();
   Ltb_Output_T *ltb_output_ptr = Ltb_Get_Output_Ptr();
   if (esa_output_ptr->f_esa_alert[0] == 1)
      FF_alerts_warns_statistics_ptr->ESA_left_alert_count++;
   if (esa_output_ptr->f_esa_alert[1] == 1)
      FF_alerts_warns_statistics_ptr->ESA_right_alert_count++;
   if (esa_output_ptr->f_esa_alert[1] == 1 || esa_output_ptr->f_esa_alert[0] == 1) {
      FF_alerts_warns_statistics_ptr->ESA_left_id               = esa_output_ptr->esa_object[0].id;
      FF_alerts_warns_statistics_ptr->ESA_left_index            = esa_output_ptr->esa_object[0].index;
      FF_alerts_warns_statistics_ptr->ESA_left_alert            = esa_output_ptr->f_esa_alert[0];
      FF_alerts_warns_statistics_ptr->ESA_left_ttc_s            = esa_output_ptr->esa_object[0].ttc_s;
      FF_alerts_warns_statistics_ptr->ESA_left_ttp_s            = esa_output_ptr->esa_object[0].ttp_s;
      FF_alerts_warns_statistics_ptr->ESA_left_width_m          = esa_output_ptr->esa_object[0].width_m;
      FF_alerts_warns_statistics_ptr->ESA_left_length_m         = esa_output_ptr->esa_object[0].length_m;
      FF_alerts_warns_statistics_ptr->ESA_left_long_pos_m       = esa_output_ptr->esa_object[0].long_pos_m;
      FF_alerts_warns_statistics_ptr->ESA_left_lat_pos_m        = esa_output_ptr->esa_object[0].lat_pos_m;
      FF_alerts_warns_statistics_ptr->ESA_left_long_speed_mps   = esa_output_ptr->esa_object[0].long_speed_mps;
      FF_alerts_warns_statistics_ptr->ESA_left_lat_speed_mps    = esa_output_ptr->esa_object[0].lat_speed_mps;
      FF_alerts_warns_statistics_ptr->ESA_left_long_distance_m  = esa_output_ptr->esa_object[0].long_distance_m;
      FF_alerts_warns_statistics_ptr->ESA_left_existence_prob   = esa_output_ptr->esa_object[0].existence_prob;
      FF_alerts_warns_statistics_ptr->ESA_right_id              = esa_output_ptr->esa_object[1].id;
      FF_alerts_warns_statistics_ptr->ESA_right_index           = esa_output_ptr->esa_object[1].index;
      FF_alerts_warns_statistics_ptr->ESA_right_alert           = esa_output_ptr->f_esa_alert[1];
      FF_alerts_warns_statistics_ptr->ESA_right_ttc_s           = esa_output_ptr->esa_object[1].ttc_s;
      FF_alerts_warns_statistics_ptr->ESA_right_ttp_s           = esa_output_ptr->esa_object[1].ttp_s;
      FF_alerts_warns_statistics_ptr->ESA_right_width_m         = esa_output_ptr->esa_object[1].width_m;
      FF_alerts_warns_statistics_ptr->ESA_right_length_m        = esa_output_ptr->esa_object[1].length_m;
      FF_alerts_warns_statistics_ptr->ESA_right_long_pos_m      = esa_output_ptr->esa_object[1].long_pos_m;
      FF_alerts_warns_statistics_ptr->ESA_right_lat_pos_m       = esa_output_ptr->esa_object[1].lat_pos_m;
      FF_alerts_warns_statistics_ptr->ESA_right_long_speed_mps  = esa_output_ptr->esa_object[1].long_speed_mps;
      FF_alerts_warns_statistics_ptr->ESA_right_lat_speed_mps   = esa_output_ptr->esa_object[1].lat_speed_mps;
      FF_alerts_warns_statistics_ptr->ESA_right_long_distance_m = esa_output_ptr->esa_object[1].long_distance_m;
      FF_alerts_warns_statistics_ptr->ESA_right_existence_prob  = esa_output_ptr->esa_object[1].existence_prob;
   }
   if (ltb_output_ptr->ltb_alert_level[0] != 0)
      FF_alerts_warns_statistics_ptr->LTB_Left_alert_count++;
   if (ltb_output_ptr->ltb_alert_level[0] == 1)
      FF_alerts_warns_statistics_ptr->LTB_Left_alert_level1_count++;
   if (ltb_output_ptr->ltb_alert_level[0] == 2)
      FF_alerts_warns_statistics_ptr->LTB_Left_alert_level2_count++;
   if (ltb_output_ptr->ltb_alert_level[1] != 0)
      FF_alerts_warns_statistics_ptr->LTB_Right_alert_count++;
   if (ltb_output_ptr->ltb_alert_level[1] == 1)
      FF_alerts_warns_statistics_ptr->LTB_Right_alert_level1_count++;
   if (ltb_output_ptr->ltb_alert_level[1] == 2)
      FF_alerts_warns_statistics_ptr->LTB_Right_alert_level2_count++;
}

/*****************************************************************************
 *. Name: ConvertToHr_sec_msee
 *.    convert the cycle duration into hour,minutes, sec & milli sec
 *.
 *. Parameters: Speed_count,Speed_Range_in_hr_min_sec
 *.
 *. Return Value: None
 *.
 *. Shared Variables: None.
 *.
 *. Design Information:
 *.    (xxx is version no.)
 *.    (xxx is version no.)
 *.
 *. SCR Information:
 *.
 ******************************************************************************/
void ConvertToHr_sec_msee(unsigned32_T Speed_count, std::string *Speed_Range_in_hr_min_sec) {
   float64_T time_msTosec;
   unsigned16_T totalTime_hour;
   unsigned16_T totalTime_min;
   float32_T totalTime_sec;
   time_msTosec = (float64_T)((float64_T)(Speed_count * 50) / (float64_T)1000.0f);

   totalTime_hour = static_cast<unsigned16_T>(time_msTosec / 3600.0);
   totalTime_min  = static_cast<unsigned16_T>((time_msTosec - (static_cast<float64_T>(totalTime_hour) * 3600.0)) / 60.0);
   totalTime_sec  = static_cast<float32_T>(time_msTosec - (static_cast<float64_T>(totalTime_hour) * 3600.0) - (static_cast<float64_T>(totalTime_min) * 60.0));

   // sprintf(Speed_Range_in_hr_min_sec, "%dhr:%dmin:%fsec", totalTime_hour, totalTime_min, totalTime_sec);
   *Speed_Range_in_hr_min_sec = std::to_string(totalTime_hour) + "hr:" + std::to_string(totalTime_min) + "min:" + std::to_string(totalTime_sec) + "sec";
}

void Vse_Output(DC_Statistic_VSE_Output_T *vsePtr) {
   VSE_OUT *vse_Data = GetVSEOutput();
   if (vse_Data->VeVSE_VehFiltSpdOverGroundQF != 3) {
      vsePtr->filt_veh_speed_over_ground_qf_count++;
   }
   if (vse_Data->VeVSE_CompYawRateQF != 3) {
      vsePtr->comp_yaw_rate_qf_count++;
   }
   if (vse_Data->VeVSE_RawLatAccelQF != 3) {
      vsePtr->raw_lat_accel_qf_count++;
   }
   if (vse_Data->VeVSE_RawLongAccelQF != 3) {
      vsePtr->raw_long_accel_qf_count++;
   }
}

void Statistic_Tracker(DC_Statistic_Tracker_T *trackerPtr) {
   f360_variant_A::F360_Object_Log_Output_T *ptr_obj_log = GetF360TrackerObject();
   uint32_T obj_count                                    = ptr_obj_log->f360header.num_elements;

   for (uint32_T loopcounter = 0; loopcounter < obj_count; loopcounter++) {
      if (appendIfNotPresent(ptr_obj_log->object[loopcounter].reducedID)) {
         trackerPtr->totalTrackCount++;
         unsigned8_T object_class = ptr_obj_log->object[loopcounter].object_class;
         switch (object_class) {
         case 0:
            trackerPtr->unknownTrack_count++;
            break;
         case 1:
            trackerPtr->Car_count++;
            break;
         case 2:
            trackerPtr->TwoWheelTrack_count++;
            break;
         case 3:
            trackerPtr->Truck_count++;
            break;
         case 4:
            trackerPtr->pedestrainTrack_count++;
            break;
         default:
            trackerPtr->Invalid_count++;
            break;
         }
      }
   }
}

/*****************************************************************************
 *. Name: Update_Feature_Statistic_Data
 *.    This function will increment the feature Alerts/Warn count of input & output cycles
 *.
 *. Parameters: Features_Count
 *.
 *. Return Value: None
 *.
 *. Shared Variables: None.
 *.
 *. Design Information:
 *.    (xxx is version no.)
 *.    (xxx is version no.)
 *.
 *. SCR Information:
 *.
 ******************************************************************************/
void Update_Feature_Statistic_Data(DC_FF_alerts_warns_T *Features_Count) {
   float64_T Speed_Range_Kmph                       = 0;
   f360_variant_A::F360_Host_Raw_T *ptr_VehicleInfo = GetHostRawInfo();
   Speed_Range_Kmph                                 = ptr_VehicleInfo->raw_speed * 3.6;
   UINT8 new_fta_warning_si_therashold              = 40;
   UINT8 new_fta_braking_si_therashold              = 15;
   // static unsigned16_T no_fta_warning_si_input = 40;
   static unsigned16_T no_fta_warning_si_output = 40;
   // static unsigned16_T no_fta_braking_si_input = 0, no_fta_braking_Range_SI_input = 0;
   static unsigned16_T no_fta_braking_si_output = 0, no_fta_braking_Range_SI_output = 0;
   // static UINT8 is_stage1_braking_input = 0, braking_below_2_input = 0, braking_below_4_input = 0, braking_below_6_input = 0, braking_below_10_input = 0, braking_above_10_input = 0;
   static UINT8 is_stage1_braking_output = 0, braking_below_2_output = 0, braking_below_4_output = 0, braking_below_6_output = 0, braking_below_10_output = 0, braking_above_10_output = 0;
   // static UINT8 is_stage2_braking_input = 0;
   static UINT8 is_stage2_braking_output = 0;
   static UINT8 RECW_Acute_output_flag;
   static UINT8 RECW_Alert_output_flag;
   // static UINT8 f_cta_alert_left_input_flag = 0, f_cta_alert_right_input_flag = 0, f_cta_warn_left_input_flag = 0, f_cta_warn_right_input_flag = 0, RCTB_qualifier_input_flag = 0;
   static UINT8 f_cta_alert_left_output_flag = 0, f_cta_alert_right_output_flag = 0, f_cta_warn_left_output_flag = 0, f_cta_warn_right_output_flag = 0, RCTB_qualifier_output_flag = 0;

   /* Get CED alerts counts */
   Ced_Output_T *ptr_ced_output = Ced_Get_Output_Ptr();
   if (ptr_ced_output->f_ced_enable) {
      /*CED Left Alerts level Count*/
      if (ptr_ced_output->ced_alert[FBK_SIDE_LEFT] == 1) {
         Features_Count->CED_left_alert_level1_count++;
      }
      if (ptr_ced_output->ced_alert[FBK_SIDE_LEFT] == 2) {
         Features_Count->CED_left_alert_level2_count++;
      }
      /*CED FL Alerts Level Count*/
      if (((ptr_ced_output->ced_object[FBK_SIDE_LEFT].direction == 2) || (ptr_ced_output->ced_object[FBK_SIDE_RIGHT].direction == 2)) && (ptr_ced_output->ced_alert[FBK_SIDE_LEFT] == 1)) {
         Features_Count->CED_FL_alert_level1_count++;
      }
      if (((ptr_ced_output->ced_object[FBK_SIDE_LEFT].direction == 2) || (ptr_ced_output->ced_object[FBK_SIDE_RIGHT].direction == 2)) && (ptr_ced_output->ced_alert[FBK_SIDE_LEFT] == 2)) {
         Features_Count->CED_FL_alert_level2_count++;
      }
      /*CED FR Alerts Level Count*/
      if (((ptr_ced_output->ced_object[FBK_SIDE_LEFT].direction == 2) || (ptr_ced_output->ced_object[FBK_SIDE_RIGHT].direction == 2)) && (ptr_ced_output->ced_alert[FBK_SIDE_RIGHT] == 1)) {
         Features_Count->CED_FR_alert_level1_count++;
      }

      if (((ptr_ced_output->ced_object[FBK_SIDE_LEFT].direction == 2) || (ptr_ced_output->ced_object[FBK_SIDE_RIGHT].direction == 2)) && (ptr_ced_output->ced_alert[FBK_SIDE_RIGHT] == 2)) {
         Features_Count->CED_FR_alert_level2_count++;
      }
      /*CED Right Alerts level Count*/
      if (ptr_ced_output->ced_alert[FBK_SIDE_RIGHT] == 1) {
         Features_Count->CED_right_alert_level1_count++;
      }
      if (ptr_ced_output->ced_alert[FBK_SIDE_RIGHT] == 2) {
         Features_Count->CED_right_alert_level2_count++;
      }
      /*CED RR Alerts Level Count*/
      if (((ptr_ced_output->ced_object[FBK_SIDE_LEFT].direction == 1) || (ptr_ced_output->ced_object[FBK_SIDE_RIGHT].direction == 1)) && (ptr_ced_output->ced_alert[FBK_SIDE_RIGHT] == 1)) {
         Features_Count->CED_RR_alert_level1_count++;
      }

      if (((ptr_ced_output->ced_object[FBK_SIDE_LEFT].direction == 1) || (ptr_ced_output->ced_object[FBK_SIDE_RIGHT].direction == 1)) && (ptr_ced_output->ced_alert[FBK_SIDE_RIGHT] == 2)) {
         Features_Count->CED_RR_alert_level2_count++;
      }
      /*CED RL Alerts Level Count*/

      if (((ptr_ced_output->ced_object[FBK_SIDE_LEFT].direction == 1) || (ptr_ced_output->ced_object[FBK_SIDE_RIGHT].direction == 1)) && (ptr_ced_output->ced_alert[FBK_SIDE_LEFT] == 1)) {
         Features_Count->CED_RL_alert_level1_count++;
      }
      if (((ptr_ced_output->ced_object[FBK_SIDE_LEFT].direction == 1) || (ptr_ced_output->ced_object[FBK_SIDE_RIGHT].direction == 1)) && (ptr_ced_output->ced_alert[FBK_SIDE_LEFT] == 2)) {
         Features_Count->CED_RL_alert_level2_count++;
      }
   }
   /*Get CTA alerts counts */
   Cta_Output_T *p_cta_output = Cta_Get_Output_Ptr();
   if (p_cta_output->f_cta_enabled) {
      if (p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_LEFT].alert_level >= 1 || p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_RIGHT].alert_level >= 1) {
         if (Check_Alert_Status_changes_from_zero_to_one(p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_LEFT].alert_level, &f_cta_alert_left_output_flag)) {
            Features_Count->CTA_left_alert_count++;
            Store_CTA_Output_properties_to_Statistic_Buff(CTA_Left_Alert);
         }
         if (Check_Alert_Status_changes_from_zero_to_one(p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_RIGHT].alert_level, &f_cta_alert_right_output_flag)) {
            Features_Count->CTA_right_alert_count++;
            Store_CTA_Output_properties_to_Statistic_Buff(CTA_Right_Alert);
         }
      }

      if (p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_LEFT].alert_level >= 1 || p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_RIGHT].alert_level >= 1) {

         if (Check_Alert_Status_changes_from_zero_to_one(p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_LEFT].alert_level, &f_cta_warn_left_output_flag)) {
            Features_Count->CTA_left_warn_count++;
            Store_CTA_Output_properties_to_Statistic_Buff(CTA_Left_Warn);
         }
         if (Check_Alert_Status_changes_from_zero_to_one(p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_RIGHT].alert_level, &f_cta_warn_right_output_flag)) {
            Features_Count->CTA_right_warn_count++;
            Store_CTA_Output_properties_to_Statistic_Buff(CTA_Right_Warn);
         }
      }
   }
   /*Get LCDA alerts count*/
   Lcda_Output_T *ptr_lcda_output = Lcda_Get_Output_Ptr();

   if (ptr_lcda_output->f_bsw_enabled) {
      if (ptr_lcda_output->bsw_alert[0] == 1) {
         Features_Count->lcda_bsw_Left_alert_Level1_count++;
      }
      if (ptr_lcda_output->bsw_alert[0] == 2) {
         Features_Count->lcda_bsw_Left_alert_Level2_count++;
      }
      if (ptr_lcda_output->bsw_alert[1] == 1) {
         Features_Count->lcda_bsw_Right_alert_Level1_count++;
      }
      if (ptr_lcda_output->bsw_alert[1] == 2) {
         Features_Count->lcda_bsw_Right_alert_Level2_count++;
      }
   }

   if (ptr_lcda_output->f_cvw_enabled) {
      if (ptr_lcda_output->cvw_alert[0] == 1) {
         Features_Count->lcda_cvw_Left_alert_Level1_count++;
      }
      if (ptr_lcda_output->cvw_alert[0] == 2) {
         Features_Count->lcda_cvw_Left_alert_Level2_count++;
      }
      if (ptr_lcda_output->cvw_alert[1] == 1) {
         Features_Count->lcda_cvw_Right_alert_Level1_count++;
      }
      if (ptr_lcda_output->cvw_alert[1] == 2) {
         Features_Count->lcda_cvw_Right_alert_Level2_count++;
      }
   }

   if (ptr_lcda_output->f_slc_enabled) {
      if (ptr_lcda_output->slc_alert[0] == 1) {
         Features_Count->lcda_slc_Left_alert_Level1_count++;
      }
      if (ptr_lcda_output->slc_alert[0] == 2) {
         Features_Count->lcda_slc_Left_alert_Level2_count++;
      }
      if (ptr_lcda_output->slc_alert[1] == 1) {
         Features_Count->lcda_slc_Right_alert_Level1_count++;
      }
      if (ptr_lcda_output->slc_alert[1] == 2) {
         Features_Count->lcda_slc_Right_alert_Level2_count++;
      }
   }

   /*Get SCW alerts count*/
   Scw_Output_T *ptr_scw_output = Scw_Get_Output_Ptr();
   if (ptr_scw_output->f_scw_enabled) {
      if (ptr_scw_output->f_scw_dyn_enabled || ptr_scw_output->f_scw_guardrail_enabled) {
         if ((ptr_scw_output->scw_object[FBK_SIDE_RIGHT].type == 1) && (ptr_scw_output->scw_object[FBK_SIDE_RIGHT].alert_level > 0)) {
            Features_Count->scw_right_dyn_alert_count++;
         }

         if ((ptr_scw_output->scw_object[FBK_SIDE_RIGHT].type == 2) && (ptr_scw_output->scw_object[FBK_SIDE_RIGHT].alert_level > 0)) {
            Features_Count->scw_right_Gaud_alert_count++;
         }
         if ((ptr_scw_output->scw_object[FBK_SIDE_LEFT].type == 1) && (ptr_scw_output->scw_object[FBK_SIDE_LEFT].alert_level > 0)) {
            Features_Count->scw_left_dyn_alert_count++;
         }
         if ((ptr_scw_output->scw_object[FBK_SIDE_LEFT].type == 2) && (ptr_scw_output->scw_object[FBK_SIDE_LEFT].alert_level > 0)) {
            Features_Count->scw_left_Gaud_alert_count++;
         }
      }
   }
   /*Get TA alert count*/
   Ta_Output_T *ptr_ta_output = Ta_Get_Output_Ptr();
   if (ptr_ta_output->f_ta_enable) {
      if (((ptr_ta_output->ta_alert_level[0] == 1) || (ptr_ta_output->ta_alert_level[1] == 1)) && (Speed_Range_Kmph > 0 && Speed_Range_Kmph <= 35)) {
         Features_Count->PFGS_alert_level1_count++;
      } else if (((ptr_ta_output->ta_alert_level[0] == 2) || (ptr_ta_output->ta_alert_level[1] == 2)) && (Speed_Range_Kmph > 0 && Speed_Range_Kmph <= 35)) {
         Features_Count->PFGS_alert_level2_count++;
      } else if (((ptr_ta_output->ta_alert_level[0] == 3) || (ptr_ta_output->ta_alert_level[1] == 3)) && (Speed_Range_Kmph > 0 && Speed_Range_Kmph <= 35)) {
         Features_Count->PFGS_alert_level3_count++;
      } else if (((ptr_ta_output->ta_alert_level[0] == 4) || (ptr_ta_output->ta_alert_level[1] == 4)) && (Speed_Range_Kmph > 0 && Speed_Range_Kmph <= 35)) {
         Features_Count->PFGS_alert_level4_count++;
      } else {
      }
   }

   /*Get RECW alert count*/
   Recw_Output_T *ptr_recw_output = Recw_Get_Output_Ptr();
   if (Check_Alert_Status_changes_from_zero_to_one(ptr_recw_output->recw_alert_level, &RECW_Alert_output_flag)) {
      Features_Count->RECW_Alert_count++;
   }
}

uint8_T Check_Alert_Status_changes_from_zero_to_one(unsigned8_T Alert_signal, UINT8 *Alert_Status_flag) {
   UINT8 ret_val = 0;
   if (*Alert_Status_flag == 0 && Alert_signal == 1) {
      *Alert_Status_flag = Alert_signal;
      ret_val            = 1;
   } else {
      *Alert_Status_flag = Alert_signal;
   }
   return ret_val;
}

void Store_CTA_Output_properties_to_Statistic_Buff(CTA_Warn_Alert_Brake_T CTA_status) {
   /*Check the do we have any Limit to store the Output properties*/
   if (cta_out_properties_count[CTA_status] < MAX_CTA_OUTPUT_PROPERTIES_COUNT) {
      memcpy(&Statistic_ptr.CTA_Output_Properties[CTA_status][cta_out_properties_count[CTA_status]], &CTA_Out_Properties_SI[CTA_status], sizeof(CTA_Output_Properties_T));
      cta_out_properties_count[CTA_status]++;
   } else {
      /*Limit exceeded to store the Output properties for this Log*/
   }
}

FF_alerts_warns_statistics_T *Get_FF_alerts_warns_statistics_ptr() {
   return (&FF_alerts_warns_statistics);
}

void Reset_Statstic_data() {
   memset(&Statistic_ptr, 0, sizeof(DC_Statistic_T));
   DataQualityChk.clear();
   UniqTrack.clear();
}

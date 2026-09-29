#include "sil_output.h"
#include "Tracker_VariantA_Wrapper.h"
#include "stream_handler.h"
#include "sil_input.h"
#include "dc_read_config.h"
#include "sfl_wrapper.h"
#include "dc_config.h"
#include "DGPS_Decoder.h"

void PopulateRECUStreamData(f360_variant_A::F360_Object_Log_Output_T *obj, DC_INPUT_DATA_T *sil_input_buffer, f360_variant_A::F360_Detection_Log_Output_T *log_dets, rspp_variant_A::F360_Radar_Sensor_T *sens, f360_variant_A::F360_Core_Info_T *core_info);
void CopyTrackerOutToROTSAEStream(f360_variant_A::F360_Object_Log_Output_T *obj);
void CopyTrackerOutToROTISOStream(ROT_Object_List_Info_T *rot_obj, f360_variant_A::F360_Host_Calib_T *host_calib, Run_Mode_T run_mode);
void CopyTrackerOutToROTDetStream(rspp_variant_A::F360_Radar_Sensor_T *sens, f360_variant_A::F360_Detection_Log_Output_T *log_dets, f360_variant_A::F360_Host_Calib_T *host_calibs, Run_Mode_T run_mode);
void CopyFFOutToSFLStream(Cta_Output_T *cta_output, Ta_Output_T *ta_output, Scw_Output_T *scw_output, Recw_Output_T *recw_output, Lcda_Output_T *lcda_output, Ced_Output_T *ced_output, Pt_Output_T *pt_output);
void CopyHostDataToVSEStream(f360_variant_A::F360_Host_T *host_info, f360_variant_A::F360_Host_Raw_T *host_raw);
void CopyTracksToDspaceInputStream(DC_INPUT_DATA_T *sil_input_buffer);
void copySGDataToSGStream();
void CopyDGPSData(DGPS_Data_T *DGPSData);

SIL_DC_Output_Data_T *p_Output_Str_Ptr = nullptr;
DC_INPUT_DATA_T *dc_input              = GetDCInputdata();
static Run_Mode_T Run_Mode;

void InitMUDPStreams(Run_Mode_T run_mode) {
   Run_Mode = run_mode;
   if (getConfigParameters().UDP_Stream_Choice == "RECU") {
      RECUUDPLogInit();
   } else {
      TriggerStreamHandlerInit();
   }
}

void TransmitMUDPStreams() {

   if (getConfigParameters().UDP_Stream_Choice == "RECU") {
      PopulateRECUStreamData(GetF360TrackerObject(), dc_input, GetF360DetObject(), GetF360SensorInfo(), GetF360CoreInfo());
      CopyFFOutputToUDPBuffer(GetF360TrackerObject());
      RECUUDPLogSendFrames();
   } else {
      CopyHostDataToVSEStream(GetHostInfo(), GetHostRawInfo());
      CopyTracksToDspaceInputStream(dc_input);
      CopyTrackerOutToROTSAEStream(GetF360TrackerObject());
      CopyTrackerOutToROTISOStream(GetF360TrackerROTObject(), GetF360HostCalib(), Run_Mode);
      CopyTrackerOutToROTDetStream(GetF360SensorInfo(), GetF360DetObject(), GetF360HostCalib(), Run_Mode);
      CopyFFOutToSFLStream(Cta_Get_Output_Ptr(), Ta_Get_Output_Ptr(), Scw_Get_Output_Ptr(), Recw_Get_Output_Ptr(), Lcda_Get_Output_Ptr(), Ced_Get_Output_Ptr(), Pt_Get_Output_Ptr());
      CopyDGPSData(GetDGPSDataPtr());
#ifdef MRR_DC
      copySGDataToSGStream();
#endif
      TriggerStreamHandlerRun();
   }
}

SIL_DC_Output_Data_T *SetOutputDataPtr(SIL_DC_Output_Data_T *output_ptr) {
   p_Output_Str_Ptr = output_ptr;
   return p_Output_Str_Ptr;
}

void SetRecuOutputHeaderdata() {
   p_Output_Str_Ptr->Output_Data_Header.sym_Size    = sizeof(SIL_DC_Output_Data_T);
   p_Output_Str_Ptr->Output_Data_Header.sym_version = DC_OUTPUT_DATA_STRUCTURE_VERSION;
}

SIL_DC_Output_Data_T *GetOutputDataPtr() {
   return p_Output_Str_Ptr;
}
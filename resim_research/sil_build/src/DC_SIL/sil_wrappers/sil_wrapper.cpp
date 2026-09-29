#include <iostream>
#include <string>
#include <filesystem>
#include "sil_wrapper.h"
#include "sil_input.h"
#include "sil_output.h"
#include "dc_app_version.h"
#include "dc_read_config.h"
#include "Tracker_VariantA_Wrapper.h"
#include "stream_handler.h"
#include "olp_wrapper.h"
#include "sfl_wrapper.h"
#include "HDF_Trace.h"
#include "recu_stream_log.h"
#include "debug_file_write.h"
#include "Event_Logger.h"
#include "DGPS_Decoder.h"

namespace fs = std::filesystem;
/* Max file path */
#define MAX_FILE_PATH (4200)

static SIL_DC_Config_T *g_resim  = NULL;
static DC_Version_Info_T Version = {0};

/* Exposed API's common to FW and DC */
RECU_SIL_API sil_status_e RECU_SiL_Init(void *init_param) {
   auto status          = E_SIL_INIT_ERR;
   g_resim              = (SIL_DC_Config_T *)init_param;
   std::string xml_path = g_resim->pEmb_Cfg_Filepath;
   ReadConfigFromXML(xml_path);
   int cust                  = getConfigParameters().cust;
   int senstype              = getConfigParameters().senstype;
   std::string XTRKConfig    = getConfigParameters().XTRK_Files;
   DC_INPUT_DATA_T *dc_input = GetDCInputdata();
   std::string DGPSDecode    = getConfigParameters().DGPS_Decode_Status;
   if (DGPSDecode == "ENABLE") {
      initDgpsLibrary();
   }
   InitTracker(cust, senstype, dc_input, XTRKConfig);
   InitOLP();
   InitMUDPStreams(g_resim->Run_Mode);
   InitFeatureFunction();

   InitDebugFiles(Func_Call_LogFile_T::HDF_FILE_LOG);
   InitDebugFiles(Func_Call_LogFile_T::BIN_FILE_LOG);
   status = E_SIL_RUN_OK;
   return status;
}

RECU_SIL_API sil_status_e RECU_SiL_Execute(const void *input_rec, const void *output_rec) {
   SIL_DC_Output_Data_T *Output_Str_Ptr = (SIL_DC_Output_Data_T *)output_rec;
   auto status                          = E_SIL_RUN_ERR;
   std::string DGPSDecode               = getConfigParameters().DGPS_Decode_Status;
   SetOutputDataPtr(Output_Str_Ptr);
   SetRecuOutputHeaderdata();
   if (ProcessInputData((SIL_DC_Input_Data_T *)input_rec, g_resim->Run_Mode)) {
      if (DGPSDecode == "ENABLE") {
         if (!updateRunConfig()) {
            status = E_SIL_RUN_OK;
            return status;
         }
      }
      std::cout << "\r" << STD_DC_STRING_INFO << ": Scan Index = " << GetScanIndex();
      RunTracker();
      RunOLP(GetF360TrackerObject(), GetF360TrackerROTObject(), GetVSEOutput(), GetHostInfo());
      RunFeatureFunction();
      TransmitMUDPStreams();
      WriteDebugFiles(Func_Call_LogFile_T::STATISTIC_FILE_LOG);
      WriteDebugFiles(Func_Call_LogFile_T::HDF_FILE_LOG);
      WriteDebugFiles(Func_Call_LogFile_T::BIN_FILE_LOG);
   }
   status = E_SIL_RUN_OK;
   return status;
}

RECU_SIL_API sil_status_e RECU_SiL_Reset() {
   auto status = E_SIL_RUN_ERR;
   status      = E_SIL_RUN_OK;
   TrackerReset();
   clearOutputFile();
   return status;
}

RECU_SIL_API sil_status_e RECU_SiL_GetVersion(DC_Version_Info_T **ppVersion) {
   auto status              = E_SIL_RUN_OK;
   Version.Release_Revision = ((uint8_t)APPLICATION_MAJOR_VERSION);
   Version.Promote_Revision = ((uint8_t)APPLICATION_MINOR_VERSION);
   Version.Field_Revision   = ((uint8_t)APPLICATION_PATCH_VERSION);
   *ppVersion               = &Version;
   return status;
}

RECU_SIL_API sil_status_e RECU_SiL_SetFolderPath(const char *pLogFolder) {
   auto status = E_SIL_RUN_OK;

   fs::path filepath    = pLogFolder;
   std::string filename = filepath.filename().string();
#ifdef SRR_DC
   std::string Nfilename = "SRR_" + filename;
#elif MRR_DC
   std::string Nfilename = "MRR_" + filename;
#endif
   fs::path nFilePath      = filepath.parent_path() / Nfilename;
   std::string npLogFolder = nFilePath.string();
   pLogFolder              = npLogFolder.c_str();
   SetPathDebugFiles(Func_Call_LogFile_T::HDF_FILE_LOG, pLogFolder);
   SetPathDebugFiles(Func_Call_LogFile_T::BIN_FILE_LOG, pLogFolder);
   SetPathDebugFiles(Func_Call_LogFile_T::DGPS_FILE_LOG, pLogFolder);
   SetXTRKFiles(pLogFolder);

   return status;
}

RECU_SIL_API sil_status_e RECU_SiL_Exit(void *pLogFolder, bool endFile) {

   auto status = E_SIL_RUN_ERR;
   status      = E_SIL_RUN_OK;
   if (endFile == false) {
      ResetDebugFiles(Func_Call_LogFile_T::HDF_FILE_LOG);
   }

   if (pLogFolder != NULL && endFile == false) {
      ResetDebugFiles(Func_Call_LogFile_T::HDF_FILE_LOG);
      ResetDebugFiles(Func_Call_LogFile_T::STATISTIC_FILE_LOG, pLogFolder, endFile);
   }

   return status;
}

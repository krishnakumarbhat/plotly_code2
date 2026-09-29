#ifndef SIL_INPUT_H
#define SIL_INPUT_H

// Headers
#include "RR_ADAS_SYMBOL_Enum.h"
#include "Tracker_VariantA_Wrapper.h"
#include "dc_input_data.h"
#include "dc_read_config.h"
#include "recu_stream_log.h"
#include "sil_ecu_input_ext.h"
#include <iostream>
#include <string>
#include <vector>

enum ErrorCode {
   ErrHeaderSize,
   ErrHeaderChecksum,
   ErrHeaderVersion
};

typedef struct DC_ValidChecks_Tag {
   std::vector<uint16_t> ScanIdChk_fl;
   std::vector<uint16_t> ScanIdChk_fr;
   std::vector<uint16_t> ScanIdChk_rl;
   std::vector<uint16_t> ScanIdChk_rr;
   std::vector<uint16_t> ScanIdChk_fc;

   std::vector<uint16_t> LookIdChk_fl;
   std::vector<uint16_t> LookIdChk_fr;
   std::vector<uint16_t> LookIdChk_rl;
   std::vector<uint16_t> LookIdChk_rr;
   std::vector<uint16_t> LookIdChk_fc;

   std::vector<uint16_t> LookTypeChk_fl;
   std::vector<uint16_t> LookTypeChk_fr;
   std::vector<uint16_t> LookTypeChk_rl;
   std::vector<uint16_t> LookTypeChk_rr;
   std::vector<uint16_t> LookTypeChk_fc;

   std::vector<uint16_t> NoStaleDataChk_fl;
   std::vector<uint16_t> NoStaleDataChk_fr;
   std::vector<uint16_t> NoStaleDataChk_rl;
   std::vector<uint16_t> NoStaleDataChk_rr;
   std::vector<uint16_t> NoStaleDataChk_fc;

   std::vector<uint16_t> FOVChk_fl;
   std::vector<uint16_t> FOVChk_fr;
   std::vector<uint16_t> FOVChk_rl;
   std::vector<uint16_t> FOVChk_rr;
   std::vector<uint16_t> FOVChk_fc;

   std::vector<uint16_t> VuaChk_fl;
   std::vector<uint16_t> VuaChk_fr;
   std::vector<uint16_t> VuaChk_rl;
   std::vector<uint16_t> VuaChk_rr;
   std::vector<uint16_t> VuaChk_fc;

   std::vector<uint16_t> ZeroDetectChk_fl;
   std::vector<uint16_t> ZeroDetectChk_fr;
   std::vector<uint16_t> ZeroDetectChk_rl;
   std::vector<uint16_t> ZeroDetectChk_rr;
   std::vector<uint16_t> ZeroDetectChk_fc;
   std::vector<uint16_t> ZeroElapsedChk;

   std::vector<uint16_t> PeriodChk_fl;
   std::vector<uint16_t> PeriodChk_fr;
   std::vector<uint16_t> PeriodChk_rl;
   std::vector<uint16_t> PeriodChk_rr;
   std::vector<uint16_t> PeriodChk_fc;

   std::vector<uint16_t> OverrunChk_fl;
   std::vector<uint16_t> OverrunChk_fr;
   std::vector<uint16_t> OverrunChk_rl;
   std::vector<uint16_t> OverrunChk_rr;
   std::vector<uint16_t> OverrunChk_fc;

   std::vector<uint16_t> DiagFiringChk;
   std::vector<uint16_t> OverallTimeChk;

   std::vector<uint16_t> ElevChk_fl;
   std::vector<uint16_t> ElevChk_fr;
   std::vector<uint16_t> ElevChk_rl;
   std::vector<uint16_t> ElevChk_rr;
   std::vector<uint16_t> ElevChk_fc;

   std::vector<uint16_t> AzimuthChk_fl;
   std::vector<uint16_t> AzimuthChk_fr;
   std::vector<uint16_t> AzimuthChk_rl;
   std::vector<uint16_t> AzimuthChk_rr;
   std::vector<uint16_t> AzimuthChk_fc;

   std::vector<uint16_t> MntPosChk_fl;
   std::vector<uint16_t> MntPosChk_fr;
   std::vector<uint16_t> MntPosChk_rl;
   std::vector<uint16_t> MntPosChk_rr;
   std::vector<uint16_t> MntPosChk_fc;

   std::vector<uint16_t> VehInfoSpd;
   std::vector<uint16_t> VehInfoRearAxle;
   std::vector<uint16_t> VehInfoLength;
   std::vector<uint16_t> VehInfoWidth;
   std::vector<uint16_t> VehInfoHeight;

   void clear() {
      ScanIdChk_fl.clear();
      ScanIdChk_fr.clear();
      ScanIdChk_rl.clear();
      ScanIdChk_rr.clear();
      ScanIdChk_fc.clear();
      LookIdChk_fl.clear();
      LookIdChk_fr.clear();
      LookIdChk_rl.clear();
      LookIdChk_rr.clear();
      LookIdChk_fc.clear();
      LookTypeChk_fl.clear();
      LookTypeChk_fr.clear();
      LookTypeChk_rl.clear();
      LookTypeChk_rr.clear();
      LookTypeChk_fc.clear();
      NoStaleDataChk_fl.clear();
      NoStaleDataChk_fr.clear();
      NoStaleDataChk_rl.clear();
      NoStaleDataChk_rr.clear();
      NoStaleDataChk_fc.clear();
      FOVChk_fl.clear();
      FOVChk_fr.clear();
      FOVChk_rl.clear();
      FOVChk_rr.clear();
      FOVChk_fc.clear();
      VuaChk_fl.clear();
      VuaChk_fr.clear();
      VuaChk_rl.clear();
      VuaChk_rr.clear();
      VuaChk_fc.clear();
      ZeroDetectChk_fl.clear();
      ZeroDetectChk_fr.clear();
      ZeroDetectChk_rl.clear();
      ZeroDetectChk_rr.clear();
      ZeroDetectChk_fc.clear();
      ZeroElapsedChk.clear();
      PeriodChk_fl.clear();
      PeriodChk_fr.clear();
      PeriodChk_rl.clear();
      PeriodChk_rr.clear();
      PeriodChk_fc.clear();
      OverrunChk_fl.clear();
      OverrunChk_fr.clear();
      OverrunChk_rl.clear();
      OverrunChk_rr.clear();
      OverrunChk_fc.clear();
      DiagFiringChk.clear();
      OverallTimeChk.clear();
      ElevChk_fl.clear();
      ElevChk_fr.clear();
      ElevChk_rl.clear();
      ElevChk_rr.clear();
      ElevChk_fc.clear();
      AzimuthChk_fl.clear();
      AzimuthChk_fr.clear();
      AzimuthChk_rl.clear();
      AzimuthChk_rr.clear();
      AzimuthChk_fc.clear();
      MntPosChk_fl.clear();
      MntPosChk_fr.clear();
      MntPosChk_rl.clear();
      MntPosChk_rr.clear();
      MntPosChk_fc.clear();
      VehInfoSpd.clear();
      VehInfoRearAxle.clear();
      VehInfoLength.clear();
      VehInfoWidth.clear();
      VehInfoHeight.clear();
   }

} DC_ValidChecks_T;

extern DC_ValidChecks_T DataQualityChk;

void ResetIdxBuffer(void);
void ValidateScanIP();
bool GetLookIdCheck(int LookType, int LookId);
// void GetCalibValue(int Looktype , F360_SIL_Sensor_Calib_T* CalibSensorData ,float* rangeD , float* rangeRateD);

boolean_T ProcessInputData(SIL_DC_Input_Data_T *Input_Data_ptr, Run_Mode_T run_mode);
unsigned16_T CalcSum16_Z2(const unsigned8_T *Ptr, unsigned Len);
DC_INPUT_DATA_T *GetDCInputdata();

#endif

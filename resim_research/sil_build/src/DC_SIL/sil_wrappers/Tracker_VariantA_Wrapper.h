#ifndef _VARIANTA_WRAPPER_H_
#define _VARIANTA_WRAPPER_H_

#include <iostream>
#include <string>
#include "sil_ecu_input_ext.h"
#include "f360_log_types.h"
#include "rspp_radar_sensor.h"
#include "f360_core_info.h"
#include "f360_rot_object_log.h"
#include "VSE_Master_Model_L2_types.h"
#include "f360_host.h"
#include "f360_host_calib.h"
#include "f360_functional_safety_faults_log.h"
#include "rspp_detection_list.h"
#include "f360_host_raw.h"
#include "dc_config.h"
#include "sg_output.h"

f360_variant_A::F360_Object_Log_Output_T *GetF360TrackerObject();
f360_variant_A::F360_Detection_Log_Output_T *GetF360DetObject();
rspp_variant_A::F360_Radar_Sensor_T *GetF360SensorInfo();
f360_variant_A::F360_Core_Info_T *GetF360CoreInfo();
ROT_Object_List_Info_T *GetF360TrackerROTObject();
f360_variant_A::F360_Host_Calib_T *GetF360HostCalib();
VSE_OUT *GetVSEOutput();
f360_variant_A::F360_Host_T *GetHostInfo();
f360_variant_A::F360_Sensor_Calib_Log_Output_T *GetSensCalibPtr();
rspp_variant_A::RSPP_Detection_List_T *GetDetectionList();
f360_variant_A::F360_Host_Raw_T *GetHostRawInfo();
sg::SG_Output_T *GetSGOutputPtr();
Functional_Safety_Faults_Log_T *GetFunctionalSafetyFaultsLogPtr();

extern void InitTracker(int customer, int sensortype, DC_INPUT_DATA_T *sil_input_buffer, std::string XTRKConfig);
extern void RunTracker();
extern void SetXTRKFiles(const char *OpFilepath);
extern void TrackerReset();
extern double GetTrackerTimestamp();
static void setTrackerTimestamp(double);

#endif
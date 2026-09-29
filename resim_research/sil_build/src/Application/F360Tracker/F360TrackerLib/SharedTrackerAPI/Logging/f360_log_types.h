#ifndef F360_LOG_TYPES_VARIANT_A_H
#define F360_LOG_TYPES_VARIANT_A_H
/*===================================================================================*\
* FILE:  f360_log_types.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
*/

#include "../core/f360_constants.h"
#include "../Types/T360_Types.h"
#include "f360_log_data.h"

#include "f360_object_log.h"
#include "f360_rot_object_log.h"
#include "f360_detection_log.h"
#include "f360_sensor_calib_log.h"

#include "f360_internal_cluster.h"
#include "f360_internal_object.h"
#include "f360_internal_detection_history.h"
#include "f360_internal_reflection_buffer.h"
#include "f360_internal_underdrivability.h"
#include "f360_internal_cwd.h"

namespace f360_variant_A
{
   typedef struct F360_Object_Log_Output_Tag
   {
      F360_Log_Header_T f360header;
      F360_Object_Log_T object[NUMBER_OF_OBJECT_TRACKS];
   } F360_Object_Log_Output_T;

   typedef struct ROT_Object_Log_Output_Tag
   {
      F360_Log_Header_T f360header;
      ROT_Object_Output_T common_object[NUMBER_OF_OBJECT_TRACKS];
   } ROT_Object_Log_Output_T;

   typedef struct F360_Detection_Log_Output_Tag
   {
      F360_Log_Header_T f360header;
      F360_Detection_Log_T detection[MAX_NUMBER_OF_DETECTIONS];
   } F360_Detection_Log_Output_T;

   typedef struct F360_Sensor_Calib_Log_Output_Tag
   {
      F360_Log_Header_T f360header;
      F360_Sensor_Calib_Log_T sensor[MAX_NUMBER_OF_SENSORS];
   } F360_Sensor_Calib_Log_Output_T;

   typedef struct F360_Internal_Cluster_Log_Output_Tag
   {
      F360_Log_Header_T f360header;
      F360_Internal_Cluster_T cluster[NUMBER_OF_CLUSTERS];
   } F360_Internal_Cluster_Log_Output_T;

   typedef struct F360_Internal_Object_Log_Output_Tag
   {
      F360_Log_Header_T f360header;
      F360_Internal_Object_T object[NUMBER_OF_OBJECT_TRACKS];
   } F360_Internal_Object_Log_Output_T;

   typedef struct F360_Internal_Detection_History_Log_Output_Tag
   {
      F360_Log_Header_T f360header;
      F360_Internal_Detection_Hist_T det_hist[MAX_NUMBER_OF_HISTORIC_DETECTIONS];
   } F360_Internal_Detection_History_Log_Output_T;

   typedef struct F360_Internal_Reflection_Buffer_Log_Output_Tag
   {
      F360_Log_Header_T f360header;
      F360_Internal_Reflection_Buffer_T reflection_buffer[MAX_NUMBER_OF_SENSORS];
   } F360_Internal_Reflection_Buffer_Log_Output_T;

   typedef struct F360_Internal_CWD_Log_Output_Tag
   {
      F360_Log_Header_T f360header;
      F360_Internal_CWD_T cwd[MAX_NUMBER_OF_SENSORS];
   } F360_Internal_CWD_Log_Output_T;
}

#endif 

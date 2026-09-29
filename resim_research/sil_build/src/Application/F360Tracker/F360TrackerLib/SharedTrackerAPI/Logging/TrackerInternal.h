#ifndef F360_TRACKER_INTERNAL_T_VARIANT_A_H
#define F360_TRACKER_INTERNAL_T_VARIANT_A_H
/*=================================================================================*\
 * FILE: TrackerInternal.h
 *====================================================================================
 * Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 */

#include "../Types/f360_reuse.h"
#include "f360_log_types.h"

#if defined(__TASKING__)
// TBD
#elif defined(__DCC__) && defined(__DCC_LLVM__)
#pragma pack(4)
#elif defined(__DCC__)
#pragma pack(4,4)
#elif defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(push, save_pack, 4)
#else
#endif

namespace f360_variant_A
{
   typedef struct Internal_VCS_Position_Tag
   {
      float longitudinal;
      float lateral;
      float height;
   } Internal_VCS_Position_T;

   typedef struct Tracker_Internal_Tag
   {
      F360_Internal_Detection_Hist_T det_hist[MAX_NUMBER_OF_HISTORIC_DETECTIONS];
      F360_Internal_Cluster_T clusters[NUMBER_OF_CLUSTERS];
      F360_Internal_Object_T obj[NUMBER_OF_OBJECT_TRACKS];
      F360_Internal_CWD_T cwd[MAX_NUMBER_OF_SENSORS];
      F360_Internal_Reflection_Buffer_T reflection_buffer[MAX_NUMBER_OF_SENSORS];
      F360_Internal_Underdrivability_T ud_zone[NUMBER_OF_OVERHEAD_ZONES];
      float up_host_travel_distance;
      uint16_t up_circular_buffer_idx;
      uint8_t unused[6];
   } Tracker_Internal_T;
}

#if defined(__TASKING__)
// TBD
#elif defined(__DCC__) && defined(__DCC_LLVM__)
#pragma pack()
#elif defined(__DCC__)
#pragma pack(0)
#elif defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(pop, save_pack)
#else
#endif

#endif

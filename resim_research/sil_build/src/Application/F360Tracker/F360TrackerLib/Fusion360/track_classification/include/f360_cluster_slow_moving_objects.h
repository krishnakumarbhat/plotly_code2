/*===================================================================================*\
* FILE: f360_cluster_slow_moving_objects.h
*====================================================================================
* Copyright 2025 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This is the function prototype for the slow objects clustering.
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*
*
* DEVIATIONS FROM STANDARDS:
*
*
\*==========================================================================================*/

#ifndef CLUSTER_SLOW_MOVING_OBJECTS_H
#define CLUSTER_SLOW_MOVING_OBJECTS_H

#include "f360_reuse.h"
#include "f360_host.h"
#include "f360_tracker_info.h"
#include "f360_object_track.h"

namespace f360_variant_A
{
   void Cluster_Slow_Moving_Objects(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_T& host,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);

   void Count_Number_Of_Members_In_Clusters_And_Find_Cluster_Length(
      const F360_Tracker_Info_T& tracker_info,
      const uint32_t num_clusters,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);
}

#endif

/*===========================================================================*\
* FILE: f360_longi_stat_cluster.h
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360_Longi_Stat_Cluster_T structure declaration
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_LONGI_STAT_CLUSTER_H
#define F360_LONGI_STAT_CLUSTER_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_object_track.h"

namespace f360_variant_A
{
   typedef struct F360_Longi_Stat_Cluster_Tag
   {
      F360_Object_Track_T* first_object;
      F360_Object_Track_T* last_object;
      float32_t lat_mean;
      float32_t height_mean;
      uint16_t nr_objects;
      uint8_t padding[3];
   } F360_Longi_Stat_Cluster_T;

   float32_t Get_Cluster_Min_Long_Pos(const F360_Longi_Stat_Cluster_T& cluster);

   float32_t Get_Cluster_Max_Long_Pos(const F360_Longi_Stat_Cluster_T& cluster);
}


#endif

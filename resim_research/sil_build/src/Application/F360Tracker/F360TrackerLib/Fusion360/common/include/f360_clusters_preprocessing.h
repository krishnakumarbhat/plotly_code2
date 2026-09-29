/*===========================================================================*\
* FILE: f360_clusters_preprocessing.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declaration of Clusters_Preprocessing,
* Correct_Cluster_VCS_Props_Based_On_Host_Delta_Motion
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef F360_CLUSTERS_PREPROCESSING_H
#define F360_CLUSTERS_PREPROCESSING_H

#include "f360_calibrations.h"
#include "f360_tracker_info.h"
#include "f360_detection_hist.h"
#include "f360_host_props.h"
namespace f360_variant_A
{

   void Clusters_Preprocessing(
      const F360_Host_Props_T &host_props,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      F360_Detection_Hist_T& det_hist,
      F360_Tracker_Info_T &tracker_info
   );

   void Correct_Cluster_VCS_Props_Based_On_Host_Delta_Motion(
      const F360_Host_Props_T &host_props,
      F360_Cluster_T &cluster
      );
}
#endif

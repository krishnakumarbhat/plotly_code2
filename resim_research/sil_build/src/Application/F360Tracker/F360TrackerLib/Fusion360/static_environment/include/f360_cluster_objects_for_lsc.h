/*===========================================================================*\
* FILE: f360_cluster_objects_for_lsc.h
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Cluster_Objects_For_LSC() and sub-functions.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_CLUSTER_OBJECTS_FOR_LSC_H
#define F360_CLUSTER_OBJECTS_FOR_LSC_H

#include "f360_object_track.h"
#include "f360_longi_stat_cluster.h"
#include "f360_calibrations.h"
#include "f360_tracker_info.h"
#include "f360_constants.h"
#include "f360_host.h"

namespace f360_variant_A
{
   void Cluster_Objects_For_LSC(
      const F360_Calibrations_T& calibs,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_T& host,
      uint16_t& nr_next_ids_of_interest,
      uint16_t(&next_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
      uint16_t& nr_valid_clusters,
      F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS]
   );

   void Init_New_Iteration(
      const uint16_t nr_next_ids_of_interest,
      const uint16_t(&next_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
      uint16_t& nr_skipped_ids_of_interest,
      uint16_t& nr_clustered_ids,
      uint16_t& nr_ids_of_interest,
      uint16_t(&ids_of_interest)[NUMBER_OF_OBJECT_TRACKS]
   );

   void Cluster_Longi_Stat_Objects(
      const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
      const uint16_t nr_ids_of_interest,
      const uint16_t(&ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Calibrations_T& calibs,
      const F360_Host_T& host,
      uint16_t& current_idx,
      uint16_t& nr_skipped_ids_of_interest,
      uint16_t(&skipped_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
      uint16_t& nr_clustered_ids,
      uint16_t(&clustered_ids)[NUMBER_OF_OBJECT_TRACKS]
   );

   void Handle_LSC_Cluster(
      const uint16_t nr_clustered_ids,
      const uint16_t(&clustered_ids)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      uint16_t& nr_valid_clusters,
      F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS]
   );

   bool Arrange_Remaining_Objects(
      const uint16_t nr_ids_of_interest,
      const uint16_t(&ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
      const uint16_t current_idx,
      const uint16_t nr_skipped_ids_of_interest,
      const uint16_t(&skipped_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Calibrations_T& calibs,
      uint16_t& nr_next_ids_of_interest,
      uint16_t(&next_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS]
   );
}


#endif

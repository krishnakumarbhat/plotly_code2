/*===========================================================================*\
* FILE: f360_update_longi_stat_curves.h
*============================================================================
* Copyright (C) 2020 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Update_Longi_Stat_Curves() declaration and related subfunctions
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_UPDATE_LONGI_STAT_CURVES_H
#define F360_UPDATE_LONGI_STAT_CURVES_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_timing_info.h"
#include "f360_longi_stat_curve.h"
#include "f360_calibrations.h"
#include "f360_longi_stat_cluster.h"
#include "f360_host.h"
#include "f360_host_props.h"
#include "f360_static_env_poly_types.h"
#include "f360_globals.h"
#include "rspp_detection_list.h"


namespace f360_variant_A
{
   void Update_Longi_Stat_Curves(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calibs,
      const F360_Host_T& host,
      const F360_Host_Props_T& host_props,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Globals_T& globals,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
      F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES],
      Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      F360_TRKR_TIMING_INFO_T& timing_info
   );

   bool Arrange_First_Iteration(
      const F360_Tracker_Info_T &tracker_info,
      const F360_Calibrations_T& calibs,
      uint16_t &nr_next_ids_of_interest,
      uint16_t(&next_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS]
   );

   void Fit_Second_Order_Polynomials_To_Clusters(
      const uint16_t nr_valid_clusters,
      const F360_Longi_Stat_Cluster_T(&valid_clusters)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES],
      F360_Longi_Stat_Curve_T (&all_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
   );

   void Sanity_Check_And_Populate_LSC_Output(
      const F360_Calibrations_T& calibs,
      const uint16_t nr_downselected_clusters,
      const F360_Longi_Stat_Curve_T(&new_longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES],
      F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
   );

   void Remember_Old_LSC(
       const F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES],
       const F360_Host_Props_T& host_props,
       F360_Longi_Stat_Curve_T(&old_longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
   );
   void Extend_Old_LSC(
       const F360_Longi_Stat_Curve_T(&old_longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES],
       F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
   );
   void Extend_LSC_With_Detections(
      const F360_Calibrations_T& calibs,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
   );
}


#endif

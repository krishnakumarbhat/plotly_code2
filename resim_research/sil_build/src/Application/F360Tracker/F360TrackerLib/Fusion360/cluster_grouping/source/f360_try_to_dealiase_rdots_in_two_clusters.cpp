/*===================================================================================*\
* FILE: f360_try_to_dealiase_rdots_in_two_clusters.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*   ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*   ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#include "f360_math_func.h"
#include "f360_try_to_dealiase_rdots_in_two_clusters.h"
#include "f360_try_to_dealiase_rdots_in_two_clusters_support_functions.h"
#include "f360_is_two_look_type_ok_combine.h"
#include "f360_get_unique_rdot_interval_ids.h"
#include "f360_calibrations.h"
namespace f360_variant_A
{
   /*===========================================================================*\
     * FUNCTION: Try_To_Dealiase_Rdots_In_Two_Clusters
     *===========================================================================
     * RETURN VALUE:
     * bool f_success
     *
     * PARAMETERS:
     *
     * const F360_Cluster_T& cluster_older,
     * const F360_Cluster_T& cluster_newer,
     * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
     * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
     * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
     * const F360_Detection_Hist_T& detections_hist,
     * const F360_Tracker_Info_T& tracker_info,
     * const F360_Globals_T& globals,
     * const F360_Calibrations_T &calib,
     * float32_t& rngrate_interval_width_older,
     * float32_t& rngrate_interval_width_newer,
     * float32_t& interval_older,
     * float32_t& interval_newer)
     * 
     * DEVIATIONS FROM STANDARDS:
     * None.
     *
     * --------------------------------------------------------------------------
     * ABSTRACT:
     * --------------------------------------------------------------------------
     * Top level function for choosing proper subfunction for clusters' dealiasing
     *
     * PRECONDITIONS:
     * None
     *
     * POSTCONDITIONS:
     * None
     *
   \*===========================================================================*/
   bool Try_To_Dealiase_Rdots_In_Two_Clusters(
      const F360_Cluster_T& cluster_older,
      const F360_Cluster_T& cluster_newer,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T& detections_hist,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Globals_T& globals,
      const F360_Calibrations_T &calib,
      float32_t& rngrate_interval_width_older,
      float32_t& rngrate_interval_width_newer,
      float32_t& interval_older,
      float32_t& interval_newer)
   {
      bool f_success = false;

      rngrate_interval_width_older = 0.0F;
      rngrate_interval_width_newer = 0.0F;
      interval_older = 0.0F;
      interval_newer = 0.0F;

      if ((!cluster_newer.f_dealiased) && (!cluster_older.f_dealiased))
      {
         Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased(raw_detection_list, det_props, detections_hist, sensors, tracker_info, globals, calib, cluster_older, cluster_newer, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer, f_success);
      }
      else if (cluster_newer.f_dealiased && (!cluster_older.f_dealiased))
      {
         Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased(raw_detection_list, det_props, detections_hist, sensors, tracker_info, globals, calib, cluster_newer, cluster_older, rngrate_interval_width_older, interval_older, f_success);
      }
      else if (!cluster_newer.f_dealiased)
      {
         Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased(raw_detection_list, det_props, detections_hist, sensors, tracker_info, globals, calib, cluster_older, cluster_newer, rngrate_interval_width_newer, interval_newer, f_success);
      }
      else
      {
         Try_To_Dealiase_Rdots_When_Both_Clusters_Are_Dealiased(calib, cluster_older, cluster_newer, f_success);
      }
      return f_success;
   }
}

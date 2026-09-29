/*===================================================================================*\
* FILE: f360_post_update_track_adjustments.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of function for post update track adjustments
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_post_update_track_adjustments.h"
#include "f360_object_track_management.h"
#include "f360_adjust_fltr_type_dependent_params.h"
#include "f360_update_object_track_properties.h"
#include "f360_cancel_new_updated_trk_overlapping_confirmed_trks.h"
#include "f360_adjust_overlapping_confirmed_tracks.h"
#include "f360_sorted_clusters_mgmt.h"
#include "f360_sorted_tracks_mgmt.h"
#include "f360_get_wall_time.h"
#include "f360_detection_drop_countermeasure.h"
namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Post_Update_Track_Adjustments()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calibrations
   * const F360_Globals_T & globals
   * const rspp_variant_A::RSPP_Detection_List_T & raw_detect_list
   * const F360_Host_T & host
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * F360_Tracker_Info_T & tracker_info
   * F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   * F360_TRKR_TIMING_INFO_T & timing_info)
   * 
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function updates some track properties after the Kalman update has been
   * performed, including for example size estimation and overlap logic.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Post_Update_Track_Adjustments(
      const F360_Calibrations_T & calibrations,
      const F360_Globals_T & globals,
      const rspp_variant_A::RSPP_Detection_List_T & raw_detect_list,
      const F360_Host_T & host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Tracker_Info_T & tracker_info,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      const float32_t start_time = get_wall_time();

      Obj_Trk_Status_Book_Keeping(det_props, tracker_info, calibrations, object_tracks, timing_info);

      Adjust_Fltr_Type_Dependent_Params(tracker_info, object_tracks, calibrations, timing_info);

      Update_Object_Track_Properties(calibrations, globals, raw_detect_list, host, sensors, tracker_info, det_props, object_tracks, timing_info);

      Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks(calibrations, object_tracks, tracker_info, det_props, timing_info);

      Adjust_Overlapping_Confirmed_Tracks(calibrations, tracker_info, object_tracks, timing_info);

      Sorted_Clusters_Update_List(tracker_info);

      Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host.speed, object_tracks);

      timing_info.post_update_track_adjustments = get_wall_time() - start_time;
   }
}

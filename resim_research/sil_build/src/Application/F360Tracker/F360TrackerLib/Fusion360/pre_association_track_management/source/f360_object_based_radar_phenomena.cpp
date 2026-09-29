/*===================================================================================*\
* FILE: f360_object_based_radar_phenomena.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains implementations of functions related to object based radar 
*    phenomena detection.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_object_based_radar_phenomena.h"
#include "f360_object_based_radar_phenomena_internals.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Check_Dets_Against_Radar_Phenomena()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Tracker_Info_T &tracker_info,
   *  const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   *  const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   *  const F360_Calibrations_T &calibs,
   *  const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
   *  F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * Evaluates detections against known radar phenomena like angle jump and
   * multibounce base on objects near to the host.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Check_Dets_Against_Radar_Phenomena(
      const F360_Tracker_Info_T &tracker_info,
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T &calibs,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const F360_Object_Track_T &reference_track = object_tracks[tracker_info.active_obj_ids[i] - 1];

         if (Can_Object_Be_A_Reference(reference_track, calibs.rp_max_object_lateral_distance, calibs.rp_max_abs_pointing_disagreement, calibs.rp_min_confidence_level))
         {
            Check_Dets_Wrt_Reference(reference_track, sensors, calibs, raw_detection_list, detection_props);
         }
      }
   }
}

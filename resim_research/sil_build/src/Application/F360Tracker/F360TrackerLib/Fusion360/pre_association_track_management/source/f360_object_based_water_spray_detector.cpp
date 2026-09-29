/*===================================================================================*\
* FILE:  f360_object_based_water_spray_detector.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*--------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains functions for flagging detections that stems from water spray from objects
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_object_based_water_spray_detector.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_check_if_point_is_inside_box.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_get_track_bbox_in_vcs.h"
#include "f360_object_based_water_spray_detector_internals.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Object_Water_Spray_Detector()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Tracker_Info_T & tracker_info
   *  const F360_Calibrations_T & calibs
   *  const F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   *  const rspp_variant_A::RSPP_Detection_List_T & raw_det_list
   *  const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]
   *  F360_Detection_Props_T (&dets_props)[MAX_NUMBER_OF_DETECTIONS]
   *
   * EXTERNAL REFERENCES:
   * For more details, see DFD-390.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function flags detection that appear to stem from water spray from objects
   * and marks them as not ok to use.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Detect_Water_Spray_From_Objects(
      const F360_Tracker_Info_T & tracker_info,
      const F360_Calibrations_T & calibs,
      const F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const rspp_variant_A::RSPP_Detection_List_T & raw_det_list,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T (&dets_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      // Loop over all objects and find valid objects
      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;
         const F360_Object_Track_T &ref_object = object_tracks[obj_idx];

         if (Is_Object_Valid_For_Water_Spray(ref_object, calibs))
         {
            // Object is valid to search for water spray detections, find box in TCS where to search for detections
            float32_t ws_box[2][2] = {};
            Derive_Box_For_Water_Spray_Dets(ref_object, calibs, ws_box);

            // Convert corners of water spray box to VCS and get min and max longitudinal and lateral positions of the box
            const BboxCorners ws_box_vcs = Get_TCS_Box_Corners_In_VCS(ref_object, ws_box);
            float32_t min_max_ws_box_vcs[2][2] = {};
            Get_Min_And_Max_Of_VCS_Box(ws_box_vcs, min_max_ws_box_vcs);

            // Get detections of interest, i.e. from VCS box that covers the water spray box
            uint32_t num_dets_of_interest;
            int32_t det_idx_of_interest[MAX_NUMBER_OF_DETECTIONS] = {};
            Get_Det_Indexes_In_Vcs_Zone(
               min_max_ws_box_vcs[0][0],
               min_max_ws_box_vcs[0][1],
               min_max_ws_box_vcs[1][0],
               min_max_ws_box_vcs[1][1],
               raw_det_list,
               dets_props,
               det_idx_of_interest,
               num_dets_of_interest);

            // Loop over interesting detections
            for (uint32_t k = 0U; k < num_dets_of_interest; k++)
            {
               const int32_t det_idx = det_idx_of_interest[k];
               F360_Detection_Props_T &ref_det_prop = dets_props[det_idx];
               const rspp_variant_A::RSPP_Detection_T& ref_det_raw = raw_det_list.detections[det_idx];

               if (Is_Det_Valid_For_Water_Spray(ref_det_raw, ref_det_prop, calibs, ref_object.average_rcs, sensors))
               {
                  float32_t det_x_tcs;
                  float32_t det_y_tcs;
                  Convert_VCS_Posn_To_TCS_Posn(
                     ref_det_prop.vcs_position.x,
                     ref_det_prop.vcs_position.y,
                     ref_object.bbox.Get_Center().x,
                     ref_object.bbox.Get_Center().y,
                     ref_object.bbox.Get_Orientation(),
                     det_x_tcs,
                     det_y_tcs);

                  if (Check_If_Point_Is_Inside_Box_In_Same_CS(det_x_tcs, det_y_tcs, ws_box) &&
                     Does_RR_Fit_Water_Spray_Hypothesis(
                         ref_object.vcs_velocity, 
                         ref_det_raw.processed.sin_vcs_az,
                         ref_det_raw.processed.cos_vcs_az,
                         ref_det_prop.range_rate_compensated,
                         calibs.k_ows_range_rate_min_factor,
                         calibs.k_ows_range_rate_max_factor))
                  {
                     ref_det_prop.f_water_spray = true;
                  }
               }
            }
         }
      }
   }
}

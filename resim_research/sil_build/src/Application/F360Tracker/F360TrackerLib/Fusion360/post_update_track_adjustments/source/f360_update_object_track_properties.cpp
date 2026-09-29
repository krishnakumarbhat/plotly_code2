/*===================================================================================*\
* FILE:  f360_update_object_track_properties.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of Update_Object_Track_Properties() function
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_update_object_track_properties.h"
#include "f360_reuse.h"
#include "f360_math_func.h"
#include "f360_get_wall_time.h"
#include "f360_calc_obj_size.h"
#include "f360_calc_obj_processed_size.h"
#include "f360_calc_obj_height.h"
#include "f360_update_object_average_rcs.h"
#include "f360_update_object_maximum_rcs.h"
#include "f360_calc_heading_from_pos_diff.h"
#include "f360_update_object_reference_point.h"
#include "f360_sorted_tracks_mgmt.h"
#include "f360_identify_objects_that_are_immediatly_behind_another_object.h"
#include "f360_shrinking_supporting_functions.h"
#include "f360_calculate_obstacle_prob.h"



namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Update_Object_Track_Properties()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calib
   * const F360_Globals_T & globals
   * const F360_Tracker_Info_T & tracker_info
   * const rspp_variant_A::RSPP_Detection_List_T & raw_detect_list
   * const F360_Host_T & host
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   * F360_TRKR_TIMING_INFO_T & timing_info
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
   * This function updates object track properties such as objects dimensions, 
   * pointing and heading angle, total number of reduced detections, reference
   * point, linear moving, object height, and status and average RCS value.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Object_Track_Properties(
      const F360_Calibrations_T & calib,
      const F360_Globals_T & globals,
      const rspp_variant_A::RSPP_Detection_List_T & raw_detect_list,
      const F360_Host_T & host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T & timing_info)
   {      
      const float32_t start_time = get_wall_time();

      bool f_object_is_behind_another_object_array[NUMBER_OF_OBJECT_TRACKS];
      std::fill(cmn::begin(f_object_is_behind_another_object_array), cmn::end(f_object_is_behind_another_object_array), false);

      Identify_Objects_That_Are_Immediatly_Behind_Another_Object(calib.k_far_away_object_dist_sq_thr, tracker_info, object_tracks, f_object_is_behind_another_object_array);

      const float32_t CIPV_long_pos = Get_CIPV_Long_Pos(tracker_info, host, calib);

      for (int32_t num_obj_active = 0; num_obj_active < tracker_info.num_active_objs; num_obj_active++)
      {
         const int32_t obj_trk_idx = tracker_info.active_obj_ids[num_obj_active] - 1;
         F360_Object_Track_T& obj = object_tracks[obj_trk_idx];
   
         Calc_Heading_From_Pos_Diff(obj, calib);

         // Update total number of reduced dets
         obj.total_reduced_dets = obj.total_reduced_dets + obj.num_rr_inlier_dets;

         Update_Object_Average_Rcs(raw_detect_list.detections, calib, obj);

         Update_Object_Maximum_Rcs(raw_detect_list.detections, obj);

         Calc_Obj_Size(det_props, raw_detect_list, calib, sensors, globals, tracker_info, CIPV_long_pos, obj);
         Calc_Obj_Processed_Size(calib, obj);

         // Calculate current object height
         Calc_Obj_Height(raw_detect_list.detections, det_props, host, obj);

         Calculate_Properties_For_Obstacle_Prob(raw_detect_list, sensors, calib, obj);
         
         Calculate_And_Filter_Obstacle_Prob(host, obj);

         constexpr bool f_update_obj_states = true;
         constexpr bool f_update_obj_pos_only = false;
         const bool f_current_object_behind_another_object = f_object_is_behind_another_object_array[obj.id-1];
         Update_Object_Reference_Point(
            host.dist_rear_axle_to_vcs_m,
            f_update_obj_states,
            f_update_obj_pos_only,
            f_current_object_behind_another_object,
            calib,
            sensors,
            globals,
            obj);
      }

      Sorted_Tracks_Re_Sort(tracker_info);

      timing_info.obj_trk_properties = get_wall_time() - start_time;
   }
}

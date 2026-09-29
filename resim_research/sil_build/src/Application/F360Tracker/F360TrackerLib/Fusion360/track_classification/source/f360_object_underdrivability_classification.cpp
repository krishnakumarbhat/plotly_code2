/*===================================================================================*\
 * FILE:  f360_object_underdrivability_classification.cpp
 *====================================================================================
 * Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose."
 *-----------------------------------------------------------------------------------------
 * DESCRIPTION:
 * This file contains the implementation of Object_Underdriviability_Classification()
 * 
 * Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 **/

#include "f360_object_underdrivability_classification.h"
#include "f360_assign_underdrivability_status_to_tracks_ocg.h"
#include "f360_assign_underdrivability_status_to_tracks_sg.h"
#include "f360_classification_underdrivability_moving.h"
#include "f360_get_wall_time.h"
#include "f360_convert_object_prev_vcs_to_current_vcs.h"
#include "f360_host_props.h"

namespace f360_variant_A
{
   static float32_t Init_Object_Underdrivability_Classification_Timing_Info(F360_TRKR_TIMING_INFO_T& timing_info);

   static void Assign_Underdriveability_Status_To_All_Moving_Objects(
      const F360_Calibrations_T& calib, 
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_T &host,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T& timing_info);

   static void Assign_Underdriveability_Status_To_All_Nonmoving_Objects_OCG(
      const F360_Tracker_Info_T& tracker_info,
      const ocg::OCG_Outputs_T* const p_occupancy_grid,
      const F360_Host_T& host,
      const F360_Calibrations_T& calib,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T& timing_info);

   static void Assign_Underdriveability_Status_To_All_Nonmoving_Objects_SG(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_Props_T& host_props,
      const sg::SG_Output_T* const p_sg_output,
      const float32_t& dist_rear_axle_to_vcs_m,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);

   /*===========================================================================*\
   * FUNCTION: Object_Underdrivability_Classification()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T& tracker_info
   * const OCG_Outputs_T* const p_occupancy_grid
   * const F360_Host_T& host
   * const rspp_variant_A::RSPP_Detection_List_T& dets_raw
   * const F360_Calibrations_T & calib
   * F360_Object_Track_T& object
   * F360_TRKR_TIMING_INFO_T& timing_info
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
   * Main function for determination of underdrivability status for objects
   *
   * PRECONDITIONS:
   * None
   * 
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/

   void Object_Underdrivability_Classification(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_Props_T& host_props,
      const ocg::OCG_Outputs_T* const p_occupancy_grid,
      const sg::SG_Output_T* const p_sg_output,
      const F360_Host_T& host,
      const F360_Calibrations_T& calib,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      
      const float32_t start_time = Init_Object_Underdrivability_Classification_Timing_Info(timing_info);

      Assign_Underdriveability_Status_To_All_Moving_Objects(calib, tracker_info, host, object_tracks, timing_info);

      Assign_Underdriveability_Status_To_All_Nonmoving_Objects_OCG(tracker_info, p_occupancy_grid, host, calib, object_tracks, timing_info);

      Assign_Underdriveability_Status_To_All_Nonmoving_Objects_SG(tracker_info, host_props, p_sg_output, host.dist_rear_axle_to_vcs_m, object_tracks);

      timing_info.object_underdrivability_classification = get_wall_time() - start_time;
   }

   /*===========================================================================*\
   * FUNCTION: Assign_Underdriveability_Status_To_All_Moving_Objects()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calib,
   * const F360_Tracker_Info_T& tracker_info,
   * const F360_Host_T &host,
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * F360_TRKR_TIMING_INFO_T& timing_info
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Main function for determination of underdrivability status for moving objects
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static void Assign_Underdriveability_Status_To_All_Moving_Objects(
      const F360_Calibrations_T& calib,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_T &host,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      for (int32_t obj_idx = 0; obj_idx < tracker_info.num_active_objs; obj_idx++)
      {
         F360_Object_Track_T& curr_obj = object_tracks[tracker_info.active_obj_ids[obj_idx] - 1];

         if (curr_obj.f_moving)
         {
            Assign_Underdrivability_Status_To_Moving_Object(calib, host, curr_obj, timing_info);
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Assign_Underdriveability_Status_To_All_Nonmoving_Objects_OCG()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Tracker_Info_T& tracker_info,
   *  const ocg::OCG_Outputs_T* const p_occupancy_grid,
   *  const F360_Host_T& host,
   *  const F360_Calibrations_T& calib,
   *  F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   *  F360_TRKR_TIMING_INFO_T& timing_info
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Main function for determination of underdrivability status for stopped 
   * objects using occupancy grid
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static void Assign_Underdriveability_Status_To_All_Nonmoving_Objects_OCG(
      const F360_Tracker_Info_T& tracker_info,
      const ocg::OCG_Outputs_T* const p_occupancy_grid,
      const F360_Host_T& host,
      const F360_Calibrations_T& calib,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      F360_OCG_INTERNAL_T ocg_internal = {};
      const bool f_ocg_is_available = Preprocess_OCG(host, p_occupancy_grid, static_cast<float32_t>(tracker_info.time_us), ocg_internal);

      if (f_ocg_is_available)
      {
         for (int32_t obj_idx = 0; obj_idx < tracker_info.num_active_objs; obj_idx++)
         {
            F360_Object_Track_T& curr_obj = object_tracks[tracker_info.active_obj_ids[obj_idx] - 1];

            if (!curr_obj.f_moving)
            {
               // Occupancy grid is available, map ocg underdrivability status to that of object underdrivability status
               Assign_Underdrivability_Status_To_Stationary_Object_OCG(calib, *p_occupancy_grid, ocg_internal, host, curr_obj, timing_info);
            }
         }
      }
      else
      {
         for (int32_t obj_idx = 0; obj_idx < tracker_info.num_active_objs; obj_idx++)
         {
            F360_Object_Track_T& curr_obj = object_tracks[tracker_info.active_obj_ids[obj_idx] - 1];

            if (!curr_obj.f_moving)
            {
               curr_obj.underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
               curr_obj.probability_underdrivable_ocg = 0.0F;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Assign_Underdriveability_Status_To_All_Nonmoving_Objects_SG()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Tracker_Info_T& tracker_info,
   *  const sg::SG_Output_T* const p_sg_output,
   *  const float32_t& dist_rear_axle_to_vcs_m,
   *  F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Main function for determination of underdrivability status for stopped
   * objects using stationary geometries
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static void Assign_Underdriveability_Status_To_All_Nonmoving_Objects_SG(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_Props_T& host_props,
      const sg::SG_Output_T* const p_sg_output,
      const float32_t& dist_rear_axle_to_vcs_m,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      const bool f_sg_is_available = (NULL != p_sg_output);
      if (f_sg_is_available)
      {
         Assign_Underdrivability_Status_To_Stationary_Object_SG(tracker_info, host_props, *p_sg_output, dist_rear_axle_to_vcs_m, object_tracks);
      }
      else
      {
         for (int32_t obj_idx = 0; obj_idx < tracker_info.num_active_objs; obj_idx++)
         {
            F360_Object_Track_T& curr_obj = object_tracks[tracker_info.active_obj_ids[obj_idx] - 1];

            if (!curr_obj.f_moving)
            {
               curr_obj.drivable_status_sg = sg::SG_Drivability_Class_T::UNCLASSIFIED;
               curr_obj.drivable_confidence_sg = 100U;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Init_Object_Underdrivability_Classification_Timing_Info
   *===========================================================================
   * RETURN VALUE:
   * float32_t start_time
   *
   * PARAMETERS:
   * F360_TRKR_TIMING_INFO_T* const timing_info
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
   * Initialize timing variables and start timers
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static float32_t Init_Object_Underdrivability_Classification_Timing_Info(F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();
      timing_info.assign_underdrivability_status_to_tracks_ocg = 0.0F; // Initialize to zero. This timer will be iteratively updated for each object inside of Assign_Underdrivability_Status_To_Tracks_OCG()
      timing_info.determine_underdrivability_for_movable = 0.0F; // Initialize to zero. This timer will be iteratively updated for each object inside of Determine_Underdrivability_For_Movable()

      return start_time;
   }

}

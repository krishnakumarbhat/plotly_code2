/*===================================================================================*\
* FILE: f360_measurement_update_tracks.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains function definition of Measurement_Update_Tracks()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

/******************************
* Includes
*******************************/

#include "f360_measurement_update_tracks.h"
#include "f360_msmt_update_obj_trks_cca_moveable.h"
#include "f360_msmt_update_obj_trks_cca_non_moveable.h"
#include "f360_msmt_update_obj_trks_ctca.h"
#include "f360_msmt_update_object_timestamp.h"
#include "f360_msmt_update_support_functions_common.h"
#include "f360_pseudo_estimations.h"
#include "f360_sorted_tracks_mgmt.h"
#include "f360_get_wall_time.h"
#include "f360_norm_heading_angle.h"


namespace f360_variant_A
{
   /*===========================================================================*\
   * Internal Function Prototypes
   \*===========================================================================*/

   static float32_t Init_Msmt_Update_Timing_Info(F360_TRKR_TIMING_INFO_T & timing_info);

   /*===========================================================================*\
   * Global Function Definitions
   \*===========================================================================*/

   /*===========================================================================*\
   * FUNCTION: Measurement_Update_Tracks
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calibrations
   * const F360_Host_T & host
   * const F360_Tracker_Info_T & tracker_info
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const F360_Globals_T& globals
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
   * Top level function for measurement update of object
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Measurement_Update_Tracks(
      const F360_Calibrations_T & calibrations,
      const F360_Host_T & host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      const float32_t start_time = Init_Msmt_Update_Timing_Info(timing_info);

      for (int32_t index = 0; index < tracker_info.num_active_objs; index++)
      {
         F360_Object_Track_T &obj = object_tracks[tracker_info.active_obj_ids[index] - 1];

         Pseudo_Estimations(calibrations, host, det_props, sensors, globals, obj, timing_info);

         uint32_t selected_dets_num;
         uint32_t selected_dets_idx[MAX_DETS_IN_OBJ_TRK]{};
         uint32_t selected_dets_score[MSMT_UPDATE_MAX_NUM_OF_MSMT]{};
         std::fill(cmn::begin(selected_dets_score), cmn::end(selected_dets_score), 1U);
         Select_Dets_For_RR_Update(raw_detection_list.detections, calibrations.k_min_num_selected_dets_per_sensor_for_binning, det_props, obj, selected_dets_idx, selected_dets_num, selected_dets_score);

         switch (obj.trk_fltr_type)
         {
            case F360_TRACKER_TRKFLTR_CCA:
            {
               if (obj.movable_prob < 0.5F)
               {
                  Msmt_Update_Obj_Trks_CCA_Non_Moveable(host, det_props, raw_detection_list, calibrations, selected_dets_idx, selected_dets_num, obj, timing_info);
               }
               else
               {
                  Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calibrations, selected_dets_idx, selected_dets_num, obj, timing_info);
               }
               break;
            }
            case F360_TRACKER_TRKFLTR_CTCA:
            {
               Msmt_Update_ObjTrks_CTCA(det_props, raw_detection_list, calibrations, selected_dets_idx, selected_dets_num, selected_dets_score, obj, timing_info);
               break;
            }
            default:
            {
               break;
            }
         }
         Msmt_Update_Object_Timestamp(tracker_info.elapsed_time_s, sensors, raw_detection_list, det_props, obj);
      }

      Sorted_Tracks_Re_Sort(tracker_info);

      timing_info.measurement_update_tracks = get_wall_time() - start_time;
   }

   /*===========================================================================*\
   * FUNCTION: Init_Msmt_Update_Timing_Info
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
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static float32_t Init_Msmt_Update_Timing_Info(F360_TRKR_TIMING_INFO_T & timing_info)
   {
      const float32_t start_time = get_wall_time();
      timing_info.msmt_update_obj_trks_ctca = 0.0F;
      timing_info.msmt_update_obj_trks_cca = 0.0F;
      timing_info.msmt_update_obj_trks_cca_moveable = 0.0F;
      timing_info.msmt_update_obj_trks_cca_non_moveable = 0.0F;
      timing_info.pseudo_estimations = 0.0F;

      return start_time;
   }
}

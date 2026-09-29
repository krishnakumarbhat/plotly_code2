/*===========================================================================*\
* FILE: f360_detect_stationary_bounce.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains functions definitions: Detect_Stationary_Bounce_Detections() and Mark_Stationary_Bounce_Detections_For_Track().
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_detect_stationary_bounce.h"
#include "f360_math.h"
#include "f360_object_track.h"
#include "f360_norm_heading_angle.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"

namespace f360_variant_A
{
   /*===========================================================================*\
    * Internal Function Prototypes
   \*===========================================================================*/

   static bool Detection_To_Consider(
      const float32_t az_left_border,
      const float32_t az_right_border,
      const float32_t az_vcs_det);

   static void Calculate_Azimuth_Borders(
      const F360_Object_Track_T& curr_trk,
      const float32_t object_front_long_posn,
      const float32_t azimuth_border_ext,
      float32_t& right_border,
      float32_t& left_border);

   /*===========================================================================*\
   * FUNCTION: Detect_Stationary_Bounce_Detections()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T &calib
   * const F360_Host_T &host
   * const F360_Tracker_Info_T &tracker_info
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
   * F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS])
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
   * When host speed is above given threshold.
   * Function is looking for objects which may cause specific multipath, called
   * stationary bounce.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Detect_Stationary_Bounce_Detections(
      const F360_Calibrations_T& calib,
      const F360_Host_T& host,
      const F360_Tracker_Info_T& tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      if (host.speed > calib.k_stat_bounce_min_host_speed)
      {
         const F360_Object_Track_T* curr_trk = tracker_info.vcslong_sorted_start;
         for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
         {
            /* Since we want to look for only for detections,
            which may have big negative range rate we will consider
            only object behind host*/
            if (curr_trk->vcs_position.x > calib.k_stat_bounce_max_trk_long_posn)
            {
               break;
            }
            // Check whether object id in lateral position bounds
            if ((curr_trk->f_moving) &&
               (std::abs(curr_trk->vcs_position.y) < calib.k_stat_bounce_max_trk_lat_dist) &&
               (std::abs(curr_trk->vcs_heading.Value()) < calib.k_stat_bounce_max_trk_heading))       // Not sure how much it should be
            {
               const int32_t first_sort_idx = Get_First_Relevant_Long_Sorted_Det_Idx(calib.k_stat_bounce_min_det_long_posn, raw_detections);
               if (first_sort_idx >= 0)
               {
                  // Here, try to mark detections as stationary bounce
                  Mark_Stationary_Bounce_Detections_For_Track(calib, host, *curr_trk, raw_detections.number_of_valid_detections, first_sort_idx, raw_detections, det_props);
               }
            }
            // take next object from list
            curr_trk = tracker_info.vcslong_sorted_next_track[curr_trk->id - 1];
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Mark_Stationary_Bounce_Detections_For_Track()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T &calib
   * const F360_Host_T &host
   * const F360_Object_Track_T &curr_trk
   * const uint32_t num_det_vld
   * const int32_t first_sort_idx
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detections
   * F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * Based on given object, host and detections parameters, function checks whether
   * detection should be flagged as stationary bounce.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Mark_Stationary_Bounce_Detections_For_Track(
      const F360_Calibrations_T& calib,
      const F360_Host_T& host,
      const F360_Object_Track_T& curr_trk,
      const uint32_t num_det_valid,
      const int32_t first_sort_idx,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      const float32_t host_point_vcs_long_offset = -0.6F * host.dist_rear_axle_to_vcs_m;
      const float32_t object_front_long_posn = curr_trk.bbox.Get_Center().x - host_point_vcs_long_offset - 0.5F * curr_trk.bbox.Get_Length();

      // Calculate azimuth boundaries based on objects position and dimensions
      float32_t az_right_border;
      float32_t az_left_border;
      Calculate_Azimuth_Borders(curr_trk, object_front_long_posn, calib.k_stat_bounce_azimuth_border_ext, az_right_border, az_left_border);

      // Calculate ghost rel vel, host speed is used cause host longitudinal long velocity is missing
      int32_t det_idx = first_sort_idx;
      bool f_continue = true;
      for (uint32_t i = 0U; i < num_det_valid; i++)
      {
         f_continue = f_continue && (det_idx > F360_INVALID_ID);
         if (!f_continue)
         {
            break;
         }
         F360_Detection_Props_T& det_prop = det_props[det_idx];
         const rspp_variant_A::RSPP_Detection_T& raw_det = raw_detections.detections[det_idx];

         if (det_prop.vcs_position.x > object_front_long_posn)
         {
            f_continue = false;
         }
         else if ((!det_prop.f_stationary_bounce) && (Detection_To_Consider(az_left_border, az_right_border, raw_det.processed.vcs_az)))
         {
            const float32_t mirror_rel_vel_x = curr_trk.vcs_velocity.longitudinal - host.speed;
            const float32_t ghost_rel_vel = host.speed + (2.0F * mirror_rel_vel_x);

            if (std::abs(det_prop.range_rate_dealiased - raw_det.processed.cos_vcs_az * ghost_rel_vel) < calib.k_stat_bounce_range_rate_diff_thr)
            {
               det_prop.f_stationary_bounce = true;
               det_prop.f_ok_to_use = false;
            }
         }
         else
         {
            // Do nothing here, detection not relevant
         }
         det_idx = raw_det.processed.next_sorted_idx;
      }
   }

   /*===========================================================================*\
    * Internal Function Definitions
   \*===========================================================================*/

   /*===========================================================================*\
   * FUNCTION: Detection_To_Consider()
   * ===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const float32_t az_left_border
   * const float32_t az_right_border
   * const float32_t az_vcs_det
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
   * Function checks whether az_vcs_det lay between (right,left) zone.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool Detection_To_Consider(
      const float32_t az_left_border,
      const float32_t az_right_border,
      const float32_t az_vcs_det)
   {
      const float32_t az_det = Normalize_Heading_Angle(az_vcs_det, F360_PI);
      return ((az_left_border > az_det) && (az_right_border < az_det));
   }

   /*===========================================================================*\
   * FUNCTION: Calculate_Azimuth_Borders()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T &curr_trk
   * const float32_t object_front_long_posn
   * const float32_t azimuth_border_ext
   * float32_t &right_border
   * float32_t &left_border
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
   * Function calculate values of left and right azimuth border.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Calculate_Azimuth_Borders(
      const F360_Object_Track_T& curr_trk,
      const float32_t object_front_long_posn,
      const float32_t azimuth_border_ext,
      float32_t& right_border,
      float32_t& left_border)
   {
      right_border = F360_Atan2f((curr_trk.bbox.Get_Center().y + curr_trk.bbox.Get_Width() * 0.5F), object_front_long_posn);
      right_border = Normalize_Heading_Angle(right_border, F360_PI) - azimuth_border_ext;

      left_border = F360_Atan2f((curr_trk.bbox.Get_Center().y - curr_trk.bbox.Get_Width() * 0.5F), object_front_long_posn);
      left_border = Normalize_Heading_Angle(left_border, F360_PI) + azimuth_border_ext;
   }
}

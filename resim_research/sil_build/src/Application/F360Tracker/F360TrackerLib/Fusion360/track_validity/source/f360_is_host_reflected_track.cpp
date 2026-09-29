/*===================================================================================*\
* FILE: f360_is_host_reflected_track.cpp
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definiton of Is_Host_Reflected_Track() function
* 
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_is_host_reflected_track.h"
#include "f360_is_host_reflected_track_helpers.h"
#include "f360_math.h"
#include "f360_point.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Is_Host_Reflected_Track()
   *===========================================================================
   * RETURN VALUE:
   * bool f_is_host_mirror_track
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T& tracker_info
   * const F360_Tracker_Info_T& tracker_info
   * const F360_Host_T& host - host information
   * const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS] - estimated barriers
   * const F360_Calibrations_T& calib - tracker calibrations
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS] - table of references to sensors
   * int32_t obj_idx - index of analyzed object
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS] - table of references to objects
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
   * This function analyses whether object is a reflection of host.
   * Two types of reflections are consdidered:
   *  - no sep method - designed to detect host mirrors when reflector surface does not create SEP
   *  - sep method - designed to detect host mirror behind adjacent reflective barrier
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Is_Host_Reflected_Track(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_T& host,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T& calib,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const int32_t obj_idx,
      const F360_Trailer_Estimator_Output_T& trailer_data,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   )
   {
      bool f_is_host_mirror_track = false;
      bool f_is_suspected_host_mirror_track_by_SEP_method = false;
      bool f_is_host_mirror_non_SEP_method = false;

      float32_t sep_mirror_rot_angle = 0.0F;

      if ((std::abs(host.speed) > 1.5F) && Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method(host, object_tracks[obj_idx]))
      {
         f_is_host_mirror_non_SEP_method = Is_Ghost_Reflected_Host_Mirror_Without_SEP(object_tracks[obj_idx], tracker_info, object_tracks, host);
      }
      else
      {
         f_is_host_mirror_non_SEP_method = false; // Added to avoid MISRA violation
      }
      if ((!f_is_host_mirror_non_SEP_method) && (std::abs(host.speed) > 0.2F) && (Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method(host, trailer_data, object_tracks[obj_idx], calib)))
      {
         for (uint8_t sep_idx = 0U; ((sep_idx < F360_NUM_OF_STATIC_ENV_POLYS) && (!f_is_suspected_host_mirror_track_by_SEP_method)); sep_idx++)
         {
            if (Is_SEP_Valid_For_Host_Mirror_Ghost(sep[sep_idx], calib))
            {
               // SEP angle w.r.t host calculation if SEP is a straight line or under a curvature threshold to handle ghosts created by angled SEP w.r.t host
               float32_t sep_angle_from_x_axis = 0.0F;
               constexpr float32_t k_sep_p2_coeff_mirror_thres = 0.01F;
               if (std::abs(sep[sep_idx].p2) > k_sep_p2_coeff_mirror_thres)
               {
                  sep_angle_from_x_axis = 0.0F;
               }
               else
               {
                  sep_angle_from_x_axis = F360_Atan2f(sep[sep_idx].p1, 1.0F);
               }
               sep_mirror_rot_angle = F360_PI - 2.0F * (F360_PI_2 - sep_angle_from_x_axis);

               for (uint32_t sens_idx = 0U; ((sens_idx < MAX_NUMBER_OF_SENSORS) && (!f_is_suspected_host_mirror_track_by_SEP_method)); sens_idx++)
               {
                  const float32_t sensor_long_pos = sensors[sens_idx].constant.mounting_position.vcs_position.longitudinal;
                  const float32_t sensor_lat_pos = sensors[sens_idx].constant.mounting_position.vcs_position.lateral;
                  if ((sensors[sens_idx].variable.is_valid) && (Is_Sensor_Adjacent_To_SEP(sep[sep_idx], sensor_long_pos, sensor_lat_pos, calib)))
                  {
                     // check if reflected sensor point is in object's Extended Bbox 
                     const float32_t sep_lat_pos_sensor = sep[sep_idx].SEP_Lateral_Pos_At(sensor_long_pos);
                     const Point sensor_mirror_tcs_pos = Calc_Predicted_Reflected_Track_TCS_Position(object_tracks[obj_idx], sensor_long_pos, sensor_lat_pos, sep_lat_pos_sensor, sep_mirror_rot_angle);
                     if (Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox(object_tracks[obj_idx], sensor_mirror_tcs_pos, calib, host, trailer_data))
                     {
                        f_is_suspected_host_mirror_track_by_SEP_method = true;
                     }
                     else
                     {
                        //do nothing
                     }
                  }
                  else
                  {
                     //do nothing
                  }
               }
            }
         }
      }
      else {/*do nothing*/ }

      if (f_is_suspected_host_mirror_track_by_SEP_method)
      {
         float32_t max_heading;
         float32_t max_speed_diff;
         Determine_Heading_And_Speed_Threshold(host.speed, sep_mirror_rot_angle, calib, max_heading, max_speed_diff);
         if ((std::abs(object_tracks[obj_idx].vcs_heading.Value()) < max_heading) && (std::abs(host.speed - object_tracks[obj_idx].speed) < max_speed_diff))
         {
            object_tracks[obj_idx].mirror_prob = 1.0F;
            // reset counter for straight driving
            object_tracks[obj_idx].cntHostTurnForMirrorProb = 0;
            f_is_host_mirror_track = true;
         }
         else if (object_tracks[obj_idx].time_since_initialization < 0.8F)
         {
            object_tracks[obj_idx].mirror_prob = 0.9F;
            f_is_host_mirror_track = true;
         }
         else {/*f_is_host_mirror_track is already set to false */ }
      }
      if (f_is_host_mirror_non_SEP_method)
      {
         object_tracks[obj_idx].mirror_prob = 1.0F;
         f_is_host_mirror_track = true;
      }
      else {/*do nothing*/ }
      return f_is_host_mirror_track;
   }
}

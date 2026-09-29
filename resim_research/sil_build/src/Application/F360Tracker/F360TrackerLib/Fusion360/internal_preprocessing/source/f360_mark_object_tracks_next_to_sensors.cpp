/*===========================================================================*\
* FILE: f360_mark_object_tracks_next_to_sensors.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*    Contains function definition of Mark_Object_Tracks_Next_To_Sensors()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_mark_object_tracks_next_to_sensors.h"
#include "f360_math.h"
#include "f360_constants.h"
#include "f360_sensor_type.h"
#include "f360_get_wall_time.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Mark_Object_Tracks_Next_To_Sensors()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T &calib
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * const F360_Tracker_Info_T& tracker_info,
   * F360_Radar_Sensor_Props_T (&sensor_props)[MAX_NUMBER_OF_SENSORS]
   * F360_TRKR_TIMING_INFO_T &tracker_time_info
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
   * This function marks object tracks that are next to sensors.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Mark_Object_Tracks_Next_To_Sensors(
      const F360_Calibrations_T &calib,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Tracker_Info_T& tracker_info,
      F360_Radar_Sensor_Props_T (&sensor_props)[MAX_NUMBER_OF_SENSORS],
      F360_TRKR_TIMING_INFO_T &tracker_time_info)
   {
      const float32_t start_time = get_wall_time();

      for (int32_t sensor_index = 0; sensor_index < static_cast<int32_t> (MAX_NUMBER_OF_SRR_SENSORS); sensor_index++)
      {
         const bool f_corner_sensor = ((sensors[sensor_index].constant.mounting_location == F360_MOUNTING_LOCATION_LEFT_FORWARD) ||
            (sensors[sensor_index].constant.mounting_location == F360_MOUNTING_LOCATION_RIGHT_FORWARD) ||
            (sensors[sensor_index].constant.mounting_location == F360_MOUNTING_LOCATION_LEFT_REAR) ||
            (sensors[sensor_index].constant.mounting_location == F360_MOUNTING_LOCATION_RIGHT_REAR));

         if (f_corner_sensor)
         {
            // Precompute thresholds
            // From this lateral offset the side of an object is probably visible
            // The idea is: If the reflective vehicle is further to the front or rear it will produce a range rate that makes it distinguishable from a guardrail

            float32_t min_lat = sensors[sensor_index].constant.mounting_position.vcs_position.lateral;
            float32_t max_lat;
            if (min_lat < 0.0F)
            {
               max_lat = -calib.k_vp_vehicle_next_to_ego_max_lat_dist - min_lat;
            }
            else
            {
               max_lat = calib.k_vp_vehicle_next_to_ego_max_lat_dist + min_lat;
            }

            float32_t min_long = sensors[sensor_index].constant.mounting_position.vcs_position.longitudinal
               - calib.k_vp_vehicle_next_to_ego_max_long_dist;
            float32_t max_long = sensors[sensor_index].constant.mounting_position.vcs_position.longitudinal
               + calib.k_vp_vehicle_next_to_ego_max_long_dist;

            // Swap min <-> max if needed
            float32_t helper;
            if (min_lat > max_lat)
            {
               helper = min_lat;
               min_lat = max_lat;
               max_lat = helper;
            }

            if (min_long > max_long)
            {
               helper = min_long;
               min_long = max_long;
               max_long = helper;
            }

            float32_t object_track_min_dist = 1.0E4F;
            int32_t closest_object_track_index = -1;

            // Check if there is a vehicle in the zone
            for (uint32_t object_track_index = 0U; object_track_index < static_cast<uint32_t> (tracker_info.variant.num_tracks); object_track_index++)
            {

               if ((object_tracks[object_track_index].status > F360_OBJECT_STATUS_INVALID) &&
                  (object_tracks[object_track_index].f_moving) &&
                  (std::abs(object_tracks[object_track_index].vcs_heading.Value()) < calib.k_vp_vehicle_next_to_ego_max_abs_heading))
               {

                  const Point object_track_center_pos = object_tracks[object_track_index].bbox.Get_Center();

                  const float32_t half_length = object_tracks[object_track_index].bbox.Get_Length() * 0.5F;
                  const float32_t object_track_rear_long_pos = object_track_center_pos.x - half_length;
                  const float32_t object_track_front_long_pos = object_track_center_pos.x + half_length;

                  // Calculate the 'distance to metal'
                  const float32_t object_track_side_lat_pos = Calc_Object_Track_Side_Pos(
                     sensors[sensor_index].constant.mounting_position.vcs_position.lateral,
                     object_track_center_pos.y,
                     object_tracks[object_track_index].bbox.Get_Width());

                  const bool f_track_front_within_long_interval = (min_long < object_track_front_long_pos) && (object_track_front_long_pos < max_long);
                  const bool f_track_rear_within_long_interval = (min_long < object_track_rear_long_pos) && (object_track_rear_long_pos < max_long);
                  const bool f_track_occupies_entire_long_interval = (object_track_rear_long_pos < min_long) && (max_long < object_track_front_long_pos);
                  const bool f_track_side_within_lat_interval = (min_lat < object_track_side_lat_pos) && (object_track_side_lat_pos < max_lat);
                  const bool f_track_close_to_sensor = (f_track_side_within_lat_interval) &&
                     (f_track_occupies_entire_long_interval || f_track_front_within_long_interval || f_track_rear_within_long_interval);

                  // Object itself is to the side, front is in the front, rear is in the rear)
                  if (f_track_close_to_sensor)
                  {
                     // May be safer to use cartesian vector dist to ego center?
                     // TODO: Think about objects in ego lane
                     const float32_t ego_center_long = calib.host_vehicle_length * 0.5F;
                     const float32_t ego_center_lat = 0.0F;

                     const float32_t obj_dist = (object_track_center_pos.y - ego_center_lat) * (object_track_center_pos.y - ego_center_lat) +
                        (object_track_center_pos.x - ego_center_long) * (object_track_center_pos.x - ego_center_long);

                     if (obj_dist < object_track_min_dist)
                     {
                        // Save the index of the closest object (to overwrite lat vel/pos)
                        object_track_min_dist = obj_dist;
                        closest_object_track_index = static_cast<int32_t>(object_track_index);
                     }
                  }
               }
            }

            if (closest_object_track_index >= 0)
            {
               // Valid Object Track in sensors zone found
               sensor_props[sensor_index].f_object_track_next_to_sensor = true;
               sensor_props[sensor_index].next_to_sensor_object_track_id = object_tracks[closest_object_track_index].id;

               const float32_t long_offset = calib.k_vp_vehicle_next_to_ego_long_pos_offset;

               const Point object_track_center_pos = object_tracks[closest_object_track_index].bbox.Get_Center();

               const float32_t half_length = object_tracks[closest_object_track_index].bbox.Get_Length() * 0.5F;
               const float32_t half_width = object_tracks[closest_object_track_index].bbox.Get_Width() * 0.5F;

               // Since we only use Object Tracks with a heading ~ 0 we do not need to take heading into acoount here
               sensor_props[sensor_index].next_to_sensor_object_track_min_long_pos = object_track_center_pos.x - half_length - long_offset;
               sensor_props[sensor_index].next_to_sensor_object_track_max_long_pos = object_track_center_pos.x + half_length + long_offset;
               sensor_props[sensor_index].next_to_sensor_object_track_min_lat_pos = object_track_center_pos.y - half_width;
               sensor_props[sensor_index].next_to_sensor_object_track_max_lat_pos = object_track_center_pos.y + half_width;
            }
            else
            {
               // No valid Object Track in sensor zone found
               sensor_props[sensor_index].f_object_track_next_to_sensor = false;
               sensor_props[sensor_index].next_to_sensor_object_track_id = (-1);

               // Set min and max position to zero
               sensor_props[sensor_index].next_to_sensor_object_track_min_long_pos = 0.0F;
               sensor_props[sensor_index].next_to_sensor_object_track_max_long_pos = 0.0F;
               sensor_props[sensor_index].next_to_sensor_object_track_min_lat_pos = 0.0F;
               sensor_props[sensor_index].next_to_sensor_object_track_max_lat_pos = 0.0F;
            }
         }
      }

      tracker_time_info.mark_object_tracks_next_to_sensors = get_wall_time() - start_time;

   }

   /*===========================================================================*\
   * FUNCTION: Calc_Object_Track_Side_Pos()
   *===========================================================================
   * RETURN VALUE:
   * float32_t object_track_side_pos - track side lateral position
   *
   * PARAMETERS:
   * const float32_t vcs_sensor_lat_pos,
   * const float32_t object_track_center_lat_pos,
   * const float32_t object_track_width
   *
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
   * This function calculates position of the closer side of the object track
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Calc_Object_Track_Side_Pos(
      const float32_t vcs_sensor_lat_pos,
      const float32_t object_track_center_lat_pos,
      const float32_t object_track_width)
   {
      float32_t object_track_side_pos = object_track_center_lat_pos;
      if (0.0F <= vcs_sensor_lat_pos)
      {
         // Sensor on the right side
         object_track_side_pos -= object_track_width * 0.5F;
      }
      else
      {
         // Sensor on the left side
         object_track_side_pos += object_track_width * 0.5F;
      }
      return object_track_side_pos;
   }
}

/*===================================================================================*\
* FILE:  f360_visibility_info.cpp
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential – Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations of common functions for calculation of visibility information.
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/


#include "f360_visibility_info.h"
#include "f360_reference_point_support_functions.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Can_Object_Be_Detected_By_Sensors()
   *===========================================================================
   * RETURN VALUE:
   * bool - a flag telling that object can be detected by any sensor or not
   *
   * PARAMETERS:
   * F360_Object_Track_T & obj - reference to an object track
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS] - reference to array of sensors
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
   * This function determines whether an object track can be detected by any sensor
   * by checking if object centroid is located in the current (med/long range look)
   * FOV of any sensor.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Can_Object_Be_Detected_By_Sensors(
      const F360_Object_Track_T & obj,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS])
   {
      bool f_object_can_be_detected = false;
      for (uint32_t sidx = 0U; sidx < MAX_NUMBER_OF_SENSORS; sidx++)
      {
         if (sensors[sidx].variable.is_valid)
         {
            f_object_can_be_detected = Is_VCS_Point_Within_Current_FOV_Of_Sensor(
               obj.vcs_position,
               sensors[sidx],
               sensors[sidx].variable.look_id);

            if (f_object_can_be_detected)
            {
               break;
            }
         }
      }
      return f_object_can_be_detected;
   }

   /*===========================================================================*\
   * FUNCTION: Is_VCS_Point_Within_Current_FOV_Of_Sensor()
   *===========================================================================
   * RETURN VALUE:
   * bool - a flag telling that a point is located within current FOV of the sensor
   *
   * PARAMETERS:
   * const Point & point_pos - reference to VCS position of tested point
   * const F360_Radar_Sensor_T &sensor - reference to sensor
   * const F360_Det_Look_ID_T look_id - look_id of the sensor
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
   * This function determines whether VCS point is located in the currentan FOV
   * of any sensor (med/long range look)
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_VCS_Point_Within_Current_FOV_Of_Sensor(
      const Point & point_pos,
      const F360_Radar_Sensor_T &sensor,
      const F360_Det_Look_ID_T look_id)
   {
      const float32_t rel_posn_lon = point_pos.x - sensor.constant.mounting_position.vcs_position.longitudinal;
      const float32_t rel_posn_lat = point_pos.y - sensor.constant.mounting_position.vcs_position.lateral;

      bool f_point_within_current_FOV = Is_Rel_Point_Below_Given_Range_Limit(
         rel_posn_lon, rel_posn_lat, sensor.constant.range_limits[look_id]);

      if (f_point_within_current_FOV)
      {
         const F360_Det_Range_Type_T range_type = Get_Range_Type(look_id);
         f_point_within_current_FOV = Is_Rel_Point_Within_FOV_Azim(
            rel_posn_lon, rel_posn_lat, sensor, range_type);
      }

      return f_point_within_current_FOV;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Rel_Point_Within_FOV_Azim()
   *===========================================================================
   * RETURN VALUE:
   * bool - a flag telling that a point is located within azimuth interval for given range type of sensor
   *
   * PARAMETERS:
   * const float32_t rel_posn_lon - relative longitudinal position of the point wrt sensor in VCS
   * const float32_t rel_posn_lat - relative lateral position of the point wrt sensor in VCS
   * const F360_Det_Range_Type_T range_type - current range type of the sensor
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
   * This function determines whether point given by relative position wrt sensor
   * is located in the azimuth interval limited by minimum and maximum aziumth angle
   * of the current FOV of the sensor.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Rel_Point_Within_FOV_Azim(
      const float32_t rel_posn_lon,
      const float32_t rel_posn_lat,
      const F360_Radar_Sensor_T& sensor,
      const F360_Det_Range_Type_T range_type
   )
   {
      bool inside_left;
      bool inside_right;
      if (F360_DET_RANGE_TYPE_LONG == range_type)
      {
         inside_left = (rel_posn_lon * sensor.refined.left_fov_normal[F360_DET_LOOK_ID_0]) + (rel_posn_lat * sensor.refined.left_fov_normal[F360_DET_LOOK_ID_1]) > 0.0F;
         inside_right = (rel_posn_lon * sensor.refined.right_fov_normal[F360_DET_LOOK_ID_0]) + (rel_posn_lat * sensor.refined.right_fov_normal[F360_DET_LOOK_ID_1]) > 0.0F;
      }
      else
      {
         inside_left = (rel_posn_lon * sensor.refined.left_fov_normal[F360_DET_LOOK_ID_2]) + (rel_posn_lat * sensor.refined.left_fov_normal[F360_DET_LOOK_ID_3]) > 0.0F;
         inside_right = (rel_posn_lon * sensor.refined.right_fov_normal[F360_DET_LOOK_ID_2]) + (rel_posn_lat * sensor.refined.right_fov_normal[F360_DET_LOOK_ID_3]) > 0.0F;
      }
      const bool f_inside_left_and_right = (inside_left && inside_right);
      return f_inside_left_and_right;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Rel_Point_Below_Given_Range_Limit()
   *===========================================================================
   * RETURN VALUE:
   * bool - a flag telling that a point is located below given range limit from the sensor
   *
   * PARAMETERS:
   * const float32_t rel_posn_lon - relative longitudinal position of the point wrt sensor in VCS
   * const float32_t rel_posn_lat - relative lateral position of the point wrt sensor in VCS
   * const float32_t range_limit - maximum range
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
   * This function determines whether point given by relative position wrt sensor
   * is located witnin maximum range provided as input argument.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Rel_Point_Below_Given_Range_Limit(
      const float32_t rel_posn_lon,
      const float32_t rel_posn_lat,
      const float32_t range_limit)
   {
      const float32_t range_sq = (rel_posn_lon * rel_posn_lon) + (rel_posn_lat * rel_posn_lat);
      return (range_sq < (range_limit * range_limit));
   }

   /*===========================================================================*\
   * FUNCTION: Is_Object_In_Side_Blindzone()
   *===========================================================================
   * RETURN VALUE:
   * bool is_in_blindzone
   *
   * PARAMETERS:
   * F360_Object_Track_T& object_track
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Globals_T& globals
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function checks if an object is located in a side blindzone where
   * sensors cannot detect it properly. Only performs check if 4-corner sensors
   * are available and object is positioned between front and rear sensors.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Object_In_Side_Blindzone(
      const F360_Object_Track_T& object_track,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals)
   {
      bool is_in_blindzone = false;

      // Only check if 4-corner sensors are available and object is positioned between sensors
      if (globals.f_four_corner_sensors_available)
      {
         const float32_t obj_x = object_track.vcs_position.x;
         const bool is_obj_between_sensors = (obj_x >= globals.four_corner_sensor_config_min_longitudinal_position) && (obj_x <= globals.four_corner_sensor_config_max_longitudinal_position);

         if (is_obj_between_sensors)
         {
            // Check visibility in all valid sensors
            // It is intentionally set to true by default as we check for the visibility when 4 corner radars are present
            bool f_object_in_FOV = true;
            for (uint32_t current_sensor_idx = 0U; current_sensor_idx < MAX_NUMBER_OF_SENSORS; current_sensor_idx++)
            {
               const F360_Radar_Sensor_T& current_sensor_calib = sensors[current_sensor_idx];
               if (current_sensor_calib.variable.is_valid)
               {
                  float32_t left_fov_normal_vector[2];
                  float32_t right_fov_normal_vector[2];
                  if (F360_DET_RANGE_TYPE_LONG == Get_Range_Type(current_sensor_calib.variable.look_id))
                  {
                     left_fov_normal_vector[0] = current_sensor_calib.refined.left_fov_normal[0U];
                     left_fov_normal_vector[1] = current_sensor_calib.refined.left_fov_normal[1U];

                     right_fov_normal_vector[0] = current_sensor_calib.refined.right_fov_normal[0U];
                     right_fov_normal_vector[1] = current_sensor_calib.refined.right_fov_normal[1U];
                  }
                  else
                  {
                     left_fov_normal_vector[0] = current_sensor_calib.refined.left_fov_normal[2U];
                     left_fov_normal_vector[1] = current_sensor_calib.refined.left_fov_normal[3U];

                     right_fov_normal_vector[0] = current_sensor_calib.refined.right_fov_normal[2U];
                     right_fov_normal_vector[1] = current_sensor_calib.refined.right_fov_normal[3U];
                  }
                  f_object_in_FOV = Is_Point_Inside_FOV(object_track.vcs_position, current_sensor_calib, left_fov_normal_vector, right_fov_normal_vector);
                  if (f_object_in_FOV)
                  {
                     break;
                  }
               }
            }
            is_in_blindzone = !f_object_in_FOV;
         }
      }
      return is_in_blindzone;
   }
}

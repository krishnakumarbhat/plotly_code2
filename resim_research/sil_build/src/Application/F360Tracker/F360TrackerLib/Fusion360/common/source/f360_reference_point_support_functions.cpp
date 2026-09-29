/*===================================================================================*\
* FILE: f360_reference_point_support_functions.cpp
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition support functions related to handling of 
*   object reference point.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_reference_point_support_functions.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Is_Ref_Point_In_Sensors_FOV()
   *===========================================================================
   * RETURN VALUE:
   * bool f_point_visible
   *
   * PARAMETERS:
   * const F360_Reference_Point_T & ref_point,
   * const F360_Object_Track_T& object_track,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const F360_Globals_T& globals,
   * const F360_Calibrations_T& calibrations
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function checks if a given point is visible w.r.t to all sensor FOVs
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Ref_Point_In_Sensors_FOV(
      const F360_Reference_Point_T& ref_point,
      const F360_Object_Track_T& object_track,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals)
   {
      bool f_point_visible = false;

      // If it is not the sole center front radar configuration, visibility is always true
      if (!globals.f_single_front_center_radar_only)
      {
         f_point_visible = true;
      }
      else
      {
         const Point ref_point_pos_tcs = Get_Reference_Point_Pos_In_TCS(ref_point, object_track.bbox.Get_Length(), object_track.bbox.Get_Width());

         Point ref_point_pos_vcs;
         Convert_TCS_Posn_To_VCS_Posn(
            ref_point_pos_tcs.x,
            ref_point_pos_tcs.y,
            object_track.bbox.Get_Center().x,
            object_track.bbox.Get_Center().y,
            object_track.bbox.Get_Orientation(),
            ref_point_pos_vcs.x,
            ref_point_pos_vcs.y);


         // Check if the point is visible
         for (uint32_t current_sensor_idx = 0U; current_sensor_idx < MAX_NUMBER_OF_SENSORS; current_sensor_idx++)
         {
            const F360_Radar_Sensor_T& current_sensor_calib = sensors[current_sensor_idx];

            if (current_sensor_calib.variable.is_valid)
            {
               f_point_visible = Is_Point_Inside_FOV(ref_point_pos_vcs, current_sensor_calib, globals.rotated_left_fov_normal[current_sensor_idx], globals.rotated_right_fov_normal[current_sensor_idx]);

               if (f_point_visible)
               {
                  break;
               }
            }
         }
      }

      return f_point_visible;
   }


   /*===========================================================================*\
    * FUNCTION: Is_Point_Inside_FOV()
    *===========================================================================
    * RETURN VALUE:
    * bool f_inside_fov
    *
    * PARAMETERS:
    * const Point& point_vcs
    * const F360_Radar_Sensor_T& sensor
    * const float32_t(&left_fov_normal)[2]
    * const float32_t(&right_fov_normal)[2]
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * This function checks if a given point is within the specified FOV
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   bool Is_Point_Inside_FOV(
      const Point& point_vcs,
      const F360_Radar_Sensor_T& sensor,
      const float32_t(&left_fov_normal)[2],
      const float32_t(&right_fov_normal)[2])
   {
      const float32_t rel_ref_point_posn_lon = point_vcs.x - sensor.constant.mounting_position.vcs_position.longitudinal;
      const float32_t rel_ref_point_posn_lat = point_vcs.y - sensor.constant.mounting_position.vcs_position.lateral;

      // Check if the point is inside the specified FOV
      const bool inside_left = (rel_ref_point_posn_lon * left_fov_normal[0]) + (rel_ref_point_posn_lat * left_fov_normal[1]) > 0.0F;
      const bool inside_right = (rel_ref_point_posn_lon * right_fov_normal[0]) + (rel_ref_point_posn_lat * right_fov_normal[1]) > 0.0F;

      const bool f_inside_fov = (inside_left && inside_right);

      return f_inside_fov;
   }

   /*===========================================================================*\
    * FUNCTION: Get_Reference_Point_Pos_In_TCS()
    *===========================================================================
    * RETURN VALUE:
    * Point ref_pos_tcs
    *
    * PARAMETERS:
    * const F360_Reference_Point_T ref_point
    * const float32_t bbox_length
    * const float32_t bbox_width
    *
    * EXTERNAL REFERENCES:
    * None
    *
    * DEVIATIONS FROM STANDARDS:
    * None
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    *--------------------------------------------------------------------------
    * Function returns the TCS position of the specified reference point
    *
    * PRECONDITIONS :
    * None
    *
    * POSTCONDITIONS :
    * None
    *
    \*===========================================================================*/
   Point Get_Reference_Point_Pos_In_TCS(
      const F360_Reference_Point_T ref_point,
      const float32_t bbox_length,
      const float32_t bbox_width)
   {
      Point ref_pos_tcs = {};
      switch (ref_point)
      {
      case F360_REFERENCE_POINT_FRONT_LEFT:
      {
         ref_pos_tcs.x = 0.5F * bbox_length;
         ref_pos_tcs.y = -0.5F * bbox_width;
         break;
      }
      case F360_REFERENCE_POINT_FRONT:
      {
         ref_pos_tcs.x = 0.5F * bbox_length;
         ref_pos_tcs.y = 0.0F;
         break;
      }
      case F360_REFERENCE_POINT_FRONT_RIGHT:
      {
         ref_pos_tcs.x = 0.5F * bbox_length;
         ref_pos_tcs.y = 0.5F * bbox_width;
         break;
      }
      case F360_REFERENCE_POINT_RIGHT:
      {
         ref_pos_tcs.x = 0.0F;
         ref_pos_tcs.y = 0.5F * bbox_width;
         break;
      }
      case F360_REFERENCE_POINT_REAR_RIGHT:
      {
         ref_pos_tcs.x = -0.5F * bbox_length;
         ref_pos_tcs.y = 0.5F * bbox_width;
         break;
      }
      case F360_REFERENCE_POINT_REAR:
      {
         ref_pos_tcs.x = -0.5F * bbox_length;
         ref_pos_tcs.y = 0.0F;
         break;
      }
      case F360_REFERENCE_POINT_REAR_LEFT:
      {
         ref_pos_tcs.x = -0.5F * bbox_length;
         ref_pos_tcs.y = -0.5F * bbox_width;
         break;
      }
      case F360_REFERENCE_POINT_LEFT:
      {
         ref_pos_tcs.x = 0.0F;
         ref_pos_tcs.y = -0.5F * bbox_width;
         break;
      }
      default:
      {
         // Center
         ref_pos_tcs.x = 0.0F;
         ref_pos_tcs.y = 0.0F;
         break;
      }
      }
      return ref_pos_tcs;
   }


   /*===========================================================================*\
   * FUNCTION: Get_Distances_From_Ref_Point_To_Obj_Sides()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T& obj - Object structure
   * float32_t& len1
   * float32_t& len2
   * float32_t& wid1
   * float32_t& wid2
   *
   * EXTERNAL REFERENCES:
   * None
   *
   * DEVIATIONS FROM STANDARDS:
   * None
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   *--------------------------------------------------------------------------
   * Function assigns len1, len2, wid1 and wid2 based on the reference point location
   *
   * PRECONDITIONS :
   * None
   *
   * POSTCONDITIONS :
   * None
   *
   \*===========================================================================*/
   void Get_Distances_From_Ref_Point_To_Obj_Sides(
      const F360_Object_Track_T& obj,
      float32_t& len1,
      float32_t& len2,
      float32_t& wid1,
      float32_t& wid2)
   {
      const float32_t length = obj.length_processed;
      const float32_t width = obj.width_processed;

      switch (obj.reference_point)
      {
      case F360_REFERENCE_POINT_FRONT_LEFT:
      {
         len1 = length;
         len2 = 0.0F;
         wid1 = 0.0F;
         wid2 = width;
         break;
      }
      case F360_REFERENCE_POINT_FRONT:
      {
         len1 = length;
         len2 = 0.0F;
         wid1 = 0.5F * width;
         wid2 = wid1;
         break;
      }
      case F360_REFERENCE_POINT_FRONT_RIGHT:
      {
         len1 = length;
         len2 = 0.0F;
         wid1 = width;
         wid2 = 0.0F;
         break;
      }
      case F360_REFERENCE_POINT_RIGHT:
      {
         len1 = 0.5F * length;
         len2 = len1;
         wid1 = width;
         wid2 = 0.0F;
         break;
      }
      case F360_REFERENCE_POINT_REAR_RIGHT:
      {
         len1 = 0.0F;
         len2 = length;
         wid1 = width;
         wid2 = 0.0F;
         break;
      }
      case F360_REFERENCE_POINT_REAR:
      {
         len1 = 0.0F;
         len2 = length;
         wid1 = 0.5F * width;
         wid2 = wid1;
         break;
      }
      case F360_REFERENCE_POINT_REAR_LEFT:
      {
         len1 = 0.0F;
         len2 = length;
         wid1 = 0.0F;
         wid2 = width;
         break;
      }
      case F360_REFERENCE_POINT_LEFT:
      {
         len1 = 0.5F * length;
         len2 = len1;
         wid1 = 0.0F;
         wid2 = width;
         break;
      }
      default:
      {
         // Center
         len1 = 0.5F * length;
         len2 = len1;
         wid1 = 0.5F * width;
         wid2 = wid1;
         break;
      }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T& obj - Object structure
   * float32_t (&tcs_vec_from_center_rear_to_ref_pnt)[2]
   *
   * EXTERNAL REFERENCES:
   * None
   *
   * DEVIATIONS FROM STANDARDS:
   * None
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   *--------------------------------------------------------------------------
   * Function computes the vector from the REAR reference point to the current
   * chosen object reference point in TCS
   *
   * PRECONDITIONS :
   * None
   *
   * POSTCONDITIONS :
   * None
   *
   \*===========================================================================*/
   void Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point(
      const F360_Object_Track_T& obj,
      float32_t(&tcs_vec_from_center_rear_to_ref_pnt)[2])
   {
      const float32_t length = obj.bbox.Get_Length();
      const float32_t width = obj.bbox.Get_Width();

      switch (obj.reference_point)
      {
      case F360_REFERENCE_POINT_FRONT_LEFT:
      {
         tcs_vec_from_center_rear_to_ref_pnt[0] = length;
         tcs_vec_from_center_rear_to_ref_pnt[1] = -0.5F * width;
         break;
      }
      case F360_REFERENCE_POINT_FRONT:
      {
         tcs_vec_from_center_rear_to_ref_pnt[0] = length;
         tcs_vec_from_center_rear_to_ref_pnt[1] = 0.0F;
         break;
      }
      case F360_REFERENCE_POINT_FRONT_RIGHT:
      {
         tcs_vec_from_center_rear_to_ref_pnt[0] = length;
         tcs_vec_from_center_rear_to_ref_pnt[1] = 0.5F * width;
         break;
      }
      case F360_REFERENCE_POINT_RIGHT:
      {
         tcs_vec_from_center_rear_to_ref_pnt[0] = 0.5F * length;
         tcs_vec_from_center_rear_to_ref_pnt[1] = 0.5F * width;
         break;
      }
      case F360_REFERENCE_POINT_REAR_RIGHT:
      {
         tcs_vec_from_center_rear_to_ref_pnt[0] = 0.0F;
         tcs_vec_from_center_rear_to_ref_pnt[1] = 0.5F * width;
         break;
      }
      case F360_REFERENCE_POINT_REAR:
      {
         tcs_vec_from_center_rear_to_ref_pnt[0] = 0.0F;
         tcs_vec_from_center_rear_to_ref_pnt[1] = 0.0F;
         break;
      }
      case F360_REFERENCE_POINT_REAR_LEFT:
      {
         tcs_vec_from_center_rear_to_ref_pnt[0] = 0.0F;
         tcs_vec_from_center_rear_to_ref_pnt[1] = -0.5F * width;
         break;
      }
      case F360_REFERENCE_POINT_LEFT:
      {
         tcs_vec_from_center_rear_to_ref_pnt[0] = 0.5F * length;
         tcs_vec_from_center_rear_to_ref_pnt[1] = -0.5F * width;
         break;
      }
      default:
      {
         // Center
         tcs_vec_from_center_rear_to_ref_pnt[0] = 0.5F * length;
         tcs_vec_from_center_rear_to_ref_pnt[1] = 0.0F;
         break;
      }
      }
   }
}

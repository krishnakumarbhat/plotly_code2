/*===========================================================================*\
* FILE: f360_determine_dets_on_edge_of_fov.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains Mark_Detection_On_Edge_Of_FOV() and Is_Detection_In_FOV_Limit function definitions.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#include "f360_determine_dets_on_edge_of_fov.h"
#include "f360_constants.h"
#include "f360_reuse.h"
#include "f360_math_func.h"
#include "f360_norm_heading_angle.h"
#include <algorithm>

namespace f360_variant_A
{
   //Helper function to evaluate if sensor is on field of view
   static bool Is_Sensor_FOV_Edge(const float32_t (&left_fov_normal_sensor)[2],
                                     const float32_t (&right_fov_normal_sensor)[2],
                                     const float32_t (&rel_posn)[2])
   {
      const float32_t result1 = (rel_posn[0] * left_fov_normal_sensor[0]) + (rel_posn[1] * left_fov_normal_sensor[1]);
      const float32_t result2 = (rel_posn[0] * right_fov_normal_sensor[0]) + (rel_posn[1] * right_fov_normal_sensor[1]);
      const bool flag = (result1 > 0.0F) && (result2 > 0.0F);
      return(flag);
   }

   /*===========================================================================*\
   * FUNCTION: Mark_Detection_On_Edge_Of_FOV()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *   const rspp_variant_A::RSPP_Detection_T &detection,
   *   const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
   *   F360_Detection_Props_T &detection_prop
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
   * This function marks if detection is on edge of FOV.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Mark_Detection_On_Edge_Of_FOV(
      const rspp_variant_A::RSPP_Detection_T &detection,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T &detection_prop)
   {
      const int32_t analyzed_sensor_idx = detection.raw.sensor_id - 1;
      const float32_t detection_az = Normalize_Heading_Angle(detection.processed.vcs_az - sensors[analyzed_sensor_idx].constant.mounting_position.vcs_boresight_azimuth_angle, 0.0F);
      const bool f_detection_is_in_fov_limit = Is_Detection_In_FOV_Limit(sensors, analyzed_sensor_idx, detection_az);

      if (f_detection_is_in_fov_limit)
      {
         detection_prop.f_FOV_edge = false;
      }
      else
      {
         // second, check if the detection is in the interrior of other sensor's FOV. If this detection is not in
      // the interior of other sensor's FOV, we need to use this detection anyway.

         bool not_found_edge_detection_mr = true;
         for (int32_t sensor_idx = 0; (not_found_edge_detection_mr) && (sensor_idx < static_cast<int32_t> (MAX_NUMBER_OF_SENSORS)); sensor_idx++)
         {
            if ((analyzed_sensor_idx == sensor_idx) || (!(sensors[sensor_idx].variable.is_valid)))
            {
               continue;
            }

            float32_t rel_posn[2] = {
               detection_prop.vcs_position.x - sensors[sensor_idx].constant.mounting_position.vcs_position.longitudinal,
               detection_prop.vcs_position.y - sensors[sensor_idx].constant.mounting_position.vcs_position.lateral
            };

            struct {
               float32_t value;
               float32_t limit;
            } range;

            range.value = F360_Get_Hypotenuse(rel_posn[0], rel_posn[1]);
            const RefinedProps_T& sens_proc = sensors[sensor_idx].refined;
            const float32_t left_fov_normal_lr[2] = { sens_proc.left_fov_normal[F360_DET_LOOK_ID_0], sens_proc.left_fov_normal[F360_DET_LOOK_ID_1] };
            const float32_t right_fov_normal_lr[2] = { sens_proc.right_fov_normal[F360_DET_LOOK_ID_0], sens_proc.right_fov_normal[F360_DET_LOOK_ID_1] };
            const float32_t left_fov_normal_mr[2] = { sens_proc.left_fov_normal[F360_DET_LOOK_ID_2], sens_proc.left_fov_normal[F360_DET_LOOK_ID_3] };
            const float32_t right_fov_normal_mr[2] = { sens_proc.right_fov_normal[F360_DET_LOOK_ID_2], sens_proc.right_fov_normal[F360_DET_LOOK_ID_3] };

            // LR
            if (Is_Sensor_FOV_Edge(left_fov_normal_lr, right_fov_normal_lr, rel_posn))
            {
               range.limit = std::min(sensors[sensor_idx].constant.range_limits[F360_DET_LOOK_ID_0], sensors[sensor_idx].constant.range_limits[F360_DET_LOOK_ID_1]);
               if (range.value <= range.limit)
               {
                  detection_prop.f_FOV_edge = true;
                  break;
               }
            }

            // MR

            if (Is_Sensor_FOV_Edge(left_fov_normal_mr, right_fov_normal_mr, rel_posn))
            {
               range.limit = std::min(sensors[sensor_idx].constant.range_limits[F360_DET_LOOK_ID_2], sensors[sensor_idx].constant.range_limits[F360_DET_LOOK_ID_3]);
               if (range.value <= range.limit)
               {
                  detection_prop.f_FOV_edge = true;
                  not_found_edge_detection_mr = false;
               }
            }
         }
      }


   }

   /*===========================================================================*\
   * FUNCTION: Is_Detection_In_FOV_Limit()
   *===========================================================================
   * RETURN VALUE:
   * bool - flag indicating if detection is in field of view limits
   *
   * PARAMETERS:
   *   const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
   *   const int32_t sensor_idx,
   *   float32_t detection_az
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
   * This function checks if detection is in FOV limits.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Detection_In_FOV_Limit(
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const int32_t sensor_idx,
      const float32_t detection_az)
   {
      float32_t min_fov_limit;
      float32_t max_fov_limit;

      if (F360_DET_RANGE_TYPE_MEDIUM == Get_Range_Type(sensors[sensor_idx].variable.look_id))
      {
         min_fov_limit = sensors[sensor_idx].refined.interior_fov[F360_DET_LOOK_ID_2];
         max_fov_limit = sensors[sensor_idx].refined.interior_fov[F360_DET_LOOK_ID_3];
      }

      else
      {
         min_fov_limit = sensors[sensor_idx].refined.interior_fov[F360_DET_LOOK_ID_0];
         max_fov_limit = sensors[sensor_idx].refined.interior_fov[F360_DET_LOOK_ID_1];
      }

      return (detection_az >= min_fov_limit) && (detection_az <= max_fov_limit);
   }
}



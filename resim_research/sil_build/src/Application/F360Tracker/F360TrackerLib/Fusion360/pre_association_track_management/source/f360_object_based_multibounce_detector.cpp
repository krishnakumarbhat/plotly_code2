/*===================================================================================*\
* FILE: f360_object_based_multibounce_detector.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains implementations of functions related to object based multibounce 
*    detector.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_object_based_multibounce_detector.h"
#include "f360_object_based_multibounce_detector_internals.h"
#include <limits>


namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Check_Dets_Against_Multibounces()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Object_Track_T &obj_track,
   *  const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
   *  const bool(&relevant_sensors)[MAX_NUMBER_OF_SENSORS],
   *  const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   *  const F360_Calibrations_T &calibs,
   *  F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * Checks detections against multibounce phenomena. If detection meets conditions then
   * it is marked as f_double_bounce and is not ok to use
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Check_Dets_Against_Multibounces(
      const F360_Object_Track_T &obj_track,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      const bool(&relevant_sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T &calibs,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      const BoundingBox area_of_correct_detections = Create_Area_Of_Correct_Detections(obj_track, calibs.mb_restricted_area_width);

      for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
      {
         F360_Detection_Props_T &det_prop = detection_props[det_idx];
         const rspp_variant_A::RSPP_Detection_T &det_raw = raw_detection_list.detections[det_idx];      
         const int32_t sensor_idx = det_raw.raw.sensor_id - 1;

         if (relevant_sensors[sensor_idx] && 
            (!det_prop.f_double_bounce) &&
            (det_raw.raw.range < calibs.mb_max_det_range) &&
            (!area_of_correct_detections.Contains(det_prop.vcs_position)))
         {   
            const F360_VCS_Velocity_T sen_velocity_vcs = sensors[sensor_idx].variable.vcs_velocity;
            const Point sen_pos_vcs{ sensors[sensor_idx].constant.mounting_position.vcs_position.longitudinal, sensors[sensor_idx].constant.mounting_position.vcs_position.lateral };

            if (Is_Det_Object_Based_Multibounce(sen_velocity_vcs, sen_pos_vcs, det_prop, det_raw, obj_track.vcs_velocity, area_of_correct_detections, calibs))
            {
               det_prop.f_double_bounce = true;
               det_prop.f_ok_to_use = false;
            }
         }
      }
   }
}

/*===================================================================================*\
* FILE: f360_object_based_angle_jump_detector.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains implementations of functions related to object based angle 
*    jump detector.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_object_based_angle_jump_detector.h"
#include "f360_object_based_angle_jump_detector_internals.h"
#include "f360_math.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Check_Dets_Against_Angle_Jumps()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Object_Track_T &obj_track,
   *  const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
   *  const Sensor_Angle_Ambiguity_T(&valid_sensors)[MAX_NUMBER_OF_SENSORS],
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
   * Main function of object based angle jump detector. Angle jumps are found
   * based on object parameters (i.e. expected range rate). Only object that
   * are close enough to the host are taken into account.
   *
   * PRECONDITIONS:
   * Object should be in "time updated" state
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Check_Dets_Against_Angle_Jumps(
      const F360_Object_Track_T &obj_track,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      const Sensor_Angle_Ambiguity_T(&valid_sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T &calibs,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   )
   {
      Det_Restrictions_T det_restrictions = Calc_Det_Restrictions_Without_Max_Range(obj_track);

      for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
      {
         const rspp_variant_A::RSPP_Detection_T &det_raw = raw_detection_list.detections[det_idx];

         const int32_t sensor_idx = det_raw.raw.sensor_id - 1;
         if (valid_sensors[sensor_idx].f_sensor_relevant && (!detection_props[det_idx].f_object_based_angle_jump))
         {
            Calc_Max_And_Min_Range(sensors[sensor_idx], det_restrictions);
            F360_Detection_Props_T &det_prop = detection_props[det_idx];

            if (Is_Det_Suspected(det_raw.raw.range, det_prop.vcs_position, det_restrictions))
            {
               det_prop.f_object_based_angle_jump = Is_Det_Object_Based_Angle_Jump(sensors[sensor_idx], det_prop, det_raw, obj_track.vcs_velocity, det_restrictions, calibs, valid_sensors[sensor_idx].angle_ambiguity_candidates_rad);

               if (det_prop.f_object_based_angle_jump)
               {
                  det_prop.f_ok_to_use = false;
               }
            }
         }
      }
   }
}

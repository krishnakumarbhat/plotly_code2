/*===================================================================================*\
* FILE:  f360_calc_predicted_range_rate.cpp
*====================================================================================
*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This function calculates the predicted measured range rate if the detection
* stems from an object with a certain state vector.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#include "f360_calc_predicted_range_rate.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h" 

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Calc_Predicted_Range_Rate()
   *===========================================================================
   * RETURN VALUE:
   * float32_t predicted_range_rate
   *
   * PARAMETERS:
   * const F360_Detection_Props_T & det_p - Detection props structure
   * const rspp_variant_A::RSPP_Detection_T & det -   Detection raw structure
   * const F360_Object_Track_T & obj - Object structure
   * const F360_Radar_Sensor_T & sens - Detections parent sensor structure
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
   * Calculates the radial velocity component of an object towards the sensor based on
   * the objects estimated states
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   float32_t Calc_Predicted_Range_Rate(
      const F360_Detection_Props_T & det_p,
      const rspp_variant_A::RSPP_Detection_T & det,
      const F360_Object_Track_T & obj,
      const F360_Radar_Sensor_T & sens)
   {
      float32_t det_xvel_pred;
      float32_t det_yvel_pred;
      if (F360_TRACKER_TRKFLTR_CCA == obj.trk_fltr_type)
      {
         // Only linear velocity in x and y direction for CCA model
         det_xvel_pred = obj.vcs_velocity.longitudinal;
         det_yvel_pred = obj.vcs_velocity.lateral;
      }
      else
      {
         // CTCA model have both linear and non-linear components due to estimated curvature
         float32_t vcs_x_center_rear = 0.0F;
         float32_t vcs_y_center_rear = 0.0F;
         const Point rear_center_tcs_pos(-0.5F * obj.bbox.Get_Length(), 0.0F);
         Convert_TCS_Posn_To_VCS_Posn(rear_center_tcs_pos.x, rear_center_tcs_pos.y, obj.bbox.Get_Center().x, obj.bbox.Get_Center().y, obj.bbox.Get_Orientation(), vcs_x_center_rear, vcs_y_center_rear);

         det_xvel_pred = obj.vcs_velocity.longitudinal - obj.curvature * obj.speed * (det_p.vcs_position.y - vcs_y_center_rear);
         det_yvel_pred = obj.vcs_velocity.lateral + obj.curvature * obj.speed * (det_p.vcs_position.x - vcs_x_center_rear);
      }

      const float32_t relative_velocity_x = det_xvel_pred - sens.variable.vcs_velocity.longitudinal;
      const float32_t relative_velocity_y = det_yvel_pred - sens.variable.vcs_velocity.lateral;

      const float32_t predicted_range_rate = relative_velocity_x * det.processed.cos_vcs_az + relative_velocity_y * det.processed.sin_vcs_az;

      return predicted_range_rate;
   }
}


/*===================================================================================*\
* FILE:  f360_object_based_water_spray_detector_internals.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*--------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains internals functions for flagging detections that stems from 
* water spray from objects
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_object_based_water_spray_detector_internals.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_check_if_point_is_inside_box.h"


namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Is_Object_Valid_For_Water_Spray()
   *===========================================================================
   * RETURN VALUE:
   * bool - True if object is valid for water spray detector
   *
   * PARAMETERS:
   *  const F360_Object_Track_T & obj
   *  const F360_Calibrations_T & calibs
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
   * This function evaluates if an object is subject to be checked for water spray detections
   * behind it. Returns true if we should search for water spray detections for this object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Is_Object_Valid_For_Water_Spray(
      const F360_Object_Track_T & obj,
      const F360_Calibrations_T & calibs)
   {

      float32_t box[2][2] = {};
      box[0][0] = calibs.k_ows_min_long_pos;
      box[0][1] = calibs.k_ows_max_long_pos;
      box[1][0] = -calibs.k_ows_max_lat_pos;
      box[1][1] = calibs.k_ows_max_lat_pos;

      return ((obj.speed > calibs.k_ws_min_speed) &&
              Check_If_Point_Is_Inside_Box_In_Same_CS(obj.vcs_position.x, obj.vcs_position.y, box));
   }

   /*===========================================================================*\
   * FUNCTION: Derive_Box_For_Water_Spray_Dets()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Object_Track_T & obj
   *  const F360_Calibrations_T & calibs
   *  float32_t(&box)[2][2]
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
   * This function derives a box behind the object in TCS where we should search
   * for water spray detections. The size of the box is dependant on object speed.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Derive_Box_For_Water_Spray_Dets(
      const F360_Object_Track_T & obj,
      const F360_Calibrations_T & calibs,
      float32_t (&box)[2][2])
   {
      const float32_t half_length = 0.5F * obj.bbox.Get_Length();
      const float32_t half_width = 0.5F * obj.bbox.Get_Width();

      const float32_t ws_zone_long_extension = std::min(calibs.k_ows_zone_lng_ext_threshold, obj.speed * calibs.k_ows_para_box_obj_speed_factor);
      const float32_t half_width_ws_zone_extension = std::min(calibs.k_ows_zone_halfwidth_ext_threshold, obj.speed * calibs.k_ows_ortho_box_obj_speed_factor);
      box[0][0] = -(half_length + ws_zone_long_extension);
      box[0][1] = -half_length;
      box[1][0] = -(half_width + half_width_ws_zone_extension);
      box[1][1] = (half_width + half_width_ws_zone_extension);
   }

   /*===========================================================================*\
   * FUNCTION: Is_Det_Valid_For_Water_Spray()
   *===========================================================================
   * RETURN VALUE:
   * bool f_is_in_fp_area - True if detection is valid for water spray
   *
   * PARAMETERS:
   *  const rspp_variant_A::RSPP_Detection_T & det
   *  const F360_Detection_Props_T & det_p
   *  const F360_Calibrations_T & calibs
   *  const float32_t ref_obj_rcs
   *  const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
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
   * This function evaluates if a detection can be water spray based on the
   * detection properties. Returns true if the detection properties fulfills
   * the conditions as a suspicious water spray detection.
   * 
   * If vehicle is equipped with SRR6p sensors modified check based on RCS value
   * is done.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Is_Det_Valid_For_Water_Spray(
      const rspp_variant_A::RSPP_Detection_T & det,
      const F360_Detection_Props_T & det_p,
      const F360_Calibrations_T & calibs,
      const float32_t ref_obj_rcs,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS])
   {
      const int32_t sensor_id = det.raw.sensor_id;
      const F360_Sensor_Type_T sensor_type = sensors[sensor_id - 1].constant.sensor_type;

      bool f_is_in_fp_area = false;
      const bool f_srr6_plus_det = ((F360_SENSOR_TYPE_SRR6_PLUS_RADAR == sensor_type)
                            || (F360_SENSOR_TYPE_SRR6_PLUS_PLT_RADAR == sensor_type)
                            || (F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR == sensor_type)
                            || (F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XETH_RADAR == sensor_type)
                            || (F360_SENSOR_TYPE_SRR6_PLUS_PLT_1GB_EHT_RADAR == sensor_type));
      const bool f_srr7_plus_det = ((F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR == sensor_type)
                            || (F360_SENSOR_TYPE_SRR7_PLUS_RADAR == sensor_type)
                            || (F360_SENSOR_TYPE_SRR7_PLUS_V2_PLT_RADAR == sensor_type));
      const bool f_flr4_det = ((F360_SENSOR_TYPE_FLR4_PLT_RADAR == sensor_type)
                            || (F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR == sensor_type)
                            || (F360_SENSOR_TYPE_FLR4_RADAR == sensor_type)
                            || (F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR == sensor_type)
                            || (F360_SENSOR_TYPE_FLR4_PLUS_RADAR == sensor_type));
      const bool f_flr7_det = ((F360_SENSOR_TYPE_FLR7_PLT_RADAR == sensor_type)
                            || (F360_SENSOR_TYPE_FLR7_RADAR == sensor_type)
                            || (F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR == sensor_type));

      if ((f_srr6_plus_det)
      || (f_srr7_plus_det)
      || (f_flr4_det)
      || (f_flr7_det))
      {
        float32_t rcs_buffer = 10.0F;
        if (f_srr7_plus_det)
        {
            rcs_buffer = 7.25F;
        }

         f_is_in_fp_area = ((det_p.f_ok_to_use) && (det.raw.rcs < (ref_obj_rcs - rcs_buffer)));
      }
      else
      {
         f_is_in_fp_area = ((det_p.f_ok_to_use) && (rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH != det.raw.confid_azimuth)
                            && (det.raw.rcs < calibs.k_ws_max_det_rcs));
      }


      return f_is_in_fp_area;
   }


   /*===========================================================================*\
    * FUNCTION: Does_RR_Fit_Water_Spray_Hypothesis()
    *===========================================================================
    * RETURN VALUE:
    * bool - True if range rate fits
    *
    * PARAMETERS:
    *  const F360_VCS_Velocity_T &reference_velocity_otg - [m/s, m/s]
    *  const float32_t det_azimuth_sin                  - [-]
    *  const float32_t det_azimuth_cos                  - [-]
    *  const float32_t det_range_rate_otg               - [m/s]
    *  const float32_t rr_factor_min                    - [-]
    *  const float32_t rr_factor_max                    - [-]
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
    * Checks if range rate fits to water spray hypothesis. It assumes that water spray detection's
    * range rate is somewhere between <factor_min, factor_max> of expected range rate (calculated
    * from reference object)
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    \*===========================================================================*/
   bool Does_RR_Fit_Water_Spray_Hypothesis(
      const F360_VCS_Velocity_T &reference_velocity_otg,
      const float32_t det_azimuth_sin,
      const float32_t det_azimuth_cos,
      const float32_t det_range_rate_otg,
      const float32_t rr_factor_min,
      const float32_t rr_factor_max
   )
   {
      const float32_t expected_range_rate_otg = reference_velocity_otg.longitudinal * det_azimuth_cos + reference_velocity_otg.lateral * det_azimuth_sin;

      float32_t min_rr;
      float32_t max_rr;
      if (0.0F < expected_range_rate_otg)
      {
         min_rr = expected_range_rate_otg * rr_factor_min;
         max_rr = expected_range_rate_otg * rr_factor_max;
      }
      else
      {
         min_rr = expected_range_rate_otg * rr_factor_max;
         max_rr = expected_range_rate_otg * rr_factor_min;
      }

      return ((min_rr < det_range_rate_otg) && (det_range_rate_otg < max_rr));
   }
}

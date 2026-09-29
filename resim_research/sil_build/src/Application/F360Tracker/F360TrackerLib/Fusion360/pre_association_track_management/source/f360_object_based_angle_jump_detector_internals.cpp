/*===================================================================================*\
* FILE: f360_object_based_angle_jump_detector_internals.cpp
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

#include "f360_object_based_angle_jump_detector_internals.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_range_rates.h"
#include "f360_intersections.h"
#include "f360_angle.h"
#include "f360_try_to_dealiase_range_rate.h" 


namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Is_Det_Object_Based_Angle_Jump()
   *===========================================================================
   * RETURN VALUE:
   * bool - angle jump finding result.
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T& sensor,
   * const F360_Detection_Props_T &det_prop,
   * const rspp_variant_A::RSPP_Detection_T &det_raw,
   * const F360_VCS_Velocity_T &obj_velocity_vcs,,
   * const Det_Restrictions_T &det_restrictions,
   * const F360_Calibrations_T& calibs,
   * const float32_t (&angle_ambiguity_cands)[MAX_NUM_ANGLE_JUMPS]
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
   * Check if detection is a angle jump. It is done by checking if detection on
   * a new azimuth (after jump/correction) fits an object's velocity
   * profile given a compensated range rate diff threshold.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Det_Object_Based_Angle_Jump(
      const F360_Radar_Sensor_T& sensor,
      const F360_Detection_Props_T &det_prop,
      const rspp_variant_A::RSPP_Detection_T &det_raw,
      const F360_VCS_Velocity_T &obj_velocity_vcs,
      const Det_Restrictions_T &det_restrictions,
      const F360_Calibrations_T& calibs,
      const float32_t (&angle_ambiguity_cands)[MAX_NUM_ANGLE_JUMPS]
   )
   {
      bool f_result = false;

      const F360_VCS_Velocity_T sensor_velocity = sensor.variable.vcs_velocity;
      const Point sensor_mount_pos_vcs = Point(sensor.constant.mounting_position.vcs_position.longitudinal, sensor.constant.mounting_position.vcs_position.lateral);

      const float32_t border_lat_pos = 0.0F < det_prop.vcs_position.y ? det_restrictions.min_lat : det_restrictions.max_lat;
      const bool f_close_border_line = ((border_lat_pos - calibs.obj_aj_border_half_width) < det_prop.vcs_position.y) && (det_prop.vcs_position.y < (border_lat_pos + calibs.obj_aj_border_half_width));

      if (!f_close_border_line)
      {
         float32_t jumped_az_vcs_vals[MAX_NUM_ANGLE_JUMPS] = {};
         Calc_New_VCS_Aligned_Sensor_Azim(det_raw.raw.azimuth, angle_ambiguity_cands, sensor.constant.polarity, sensor.variable.vacs_boresight_az_estimated, jumped_az_vcs_vals);
         for (int8_t i=0; i < MAX_NUM_ANGLE_JUMPS; i++)
         {
            if ((std::abs(angle_ambiguity_cands[i]) > F360_PI) || f_result)
            {
               break;
            }
            const Angle jumped_az_vcs{ jumped_az_vcs_vals[i]};
            if (Is_Det_On_New_Azimuth_Valid(det_raw.raw.range, jumped_az_vcs, sensor_mount_pos_vcs, det_restrictions, calibs.obj_aj_max_double_range_hypothesis))
            {
               const float32_t azimuth_sin = jumped_az_vcs.Sin();
               const float32_t azimuth_cos = jumped_az_vcs.Cos();
               const float32_t rr_comp = Calculate_Det_Range_Rate_Comp(azimuth_cos, azimuth_sin, det_raw.raw.range_rate, sensor_velocity.lateral, sensor_velocity.longitudinal);

               const float32_t suspect_rr_diff = Calc_Range_Rate_Difference_From_Expectations(obj_velocity_vcs, azimuth_cos, azimuth_sin, rr_comp);

               constexpr float32_t thres_factor = 0.1F;
               float32_t thres_cap = 1.5F;

               if ((det_raw.raw.confid_azimuth < rspp_variant_A::RSPP_CONF_AZIMUTH_MIDLOW))
               {
                   thres_cap = 0.6F;
               }
               const float32_t rr_comp_thres_mov_hypo = std::min(std::abs(rr_comp) * thres_factor, thres_cap);

               float32_t dealiased_rr_comp_diff = 0.0F;
               float32_t rr_interval = 0.0F;
               // dealiase against 0 compensated range rate diff
               const bool f_suspected_angle_jump = Try_To_Dealiase_Range_Rate(
                  suspect_rr_diff,
                  0.0F,
                  rr_comp_thres_mov_hypo,
                  sensor.constant.v_wrapping[sensor.variable.look_id],
                  0.0F,
                  dealiased_rr_comp_diff,
                  rr_interval);

               if (f_suspected_angle_jump)
               {
                  f_result = true;
               }
            }
         }
      }

      return f_result;
   }


   /*===========================================================================*\
   * FUNCTION: Is_Det_Suspected()
   *===========================================================================
   * RETURN VALUE:
   * bool - detection suspection.
   *
   * PARAMETERS:
   * const float32_t det_range,
   * const Point &det_pos_vcs,
   * const Det_Restrictions_T &det_restrictions
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
   * Determine if detection should be checked as a potential angle jump
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Det_Suspected(
      const float32_t det_range,
      const Point &det_pos_vcs,
      const Det_Restrictions_T &det_restrictions
   )
   {
      bool f_suspected;

      if ((det_restrictions.min_range < det_range) && (det_range < det_restrictions.max_range))
      {
         f_suspected = !Is_Det_Inside_Restricted_AJ_Bbox(det_pos_vcs, det_restrictions);
      }
      else
      {
         f_suspected = false;
      }

      return f_suspected;
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Max_And_Min_Range()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Radar_Sensor_T& sensor,
   *  Det_Restrictions_T& det_restriction
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
   * Calculate maximum and minimum detection range for further validation in order to
   * filter out detections outside ROI per sensor.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Calc_Max_And_Min_Range(
      const F360_Radar_Sensor_T& sensor,
      Det_Restrictions_T& det_restriction)
   {
      constexpr float32_t range_cap = 120.0F;

      const F360_VCS_Position_T sensor_pos = sensor.constant.mounting_position.vcs_position;

      const Point sensor_pos_point = Point{sensor_pos.longitudinal, sensor_pos.lateral};

      const float32_t dist2corner1 = Point::get_distance_between_points({det_restriction.max_long, det_restriction.max_lat}, sensor_pos_point);
      const float32_t dist2corner2 = Point::get_distance_between_points({det_restriction.min_long, det_restriction.min_lat}, sensor_pos_point);
      const float32_t dist2corner3 = Point::get_distance_between_points({det_restriction.min_long, det_restriction.max_lat}, sensor_pos_point);
      const float32_t dist2corner4 = Point::get_distance_between_points({det_restriction.max_long, det_restriction.min_lat}, sensor_pos_point);
      constexpr uint32_t NUM_OF_CORNERS = 4U;
      const float32_t dist_array[NUM_OF_CORNERS] = {dist2corner1, dist2corner2, dist2corner3, dist2corner4};

      const float32_t raw_min_range = F360_Min_Element(dist_array, NUM_OF_CORNERS);
      const float32_t raw_max_range = F360_Max_Element(dist_array, NUM_OF_CORNERS);

      det_restriction.max_range = std::min(raw_max_range, range_cap);
      det_restriction.min_range = std::min(raw_min_range, range_cap);
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Det_Restrictions_Without_Max_Range()
   *===========================================================================
   * RETURN VALUE:
   * Det_Restrictions_T - structure with det resctrictions (max range is not filled)
   *
   * PARAMETERS:
   * const F360_Object_Track_T &obj_track
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
   * Calculate detection restrictions based on an object position with respect
   * to the host. Only max_range is not filled (filled with 0) due to it is
   * calculated for each detection separately based on sensor information.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Det_Restrictions_T Calc_Det_Restrictions_Without_Max_Range(
      const F360_Object_Track_T &obj_track
   )
   {
      Det_Restrictions_T det_restrictions = {};
      
      const float32_t close_side_margin = obj_track.bbox.Get_Width() * 0.8F;
      const float32_t far_side_margin = obj_track.bbox.Get_Width() * 0.7F;
      det_restrictions.min_lat = 0.0F < obj_track.bbox.Get_Center().y ? obj_track.bbox.Get_Center().y - close_side_margin : obj_track.bbox.Get_Center().y - far_side_margin;
      det_restrictions.max_lat = 0.0F < obj_track.bbox.Get_Center().y ? obj_track.bbox.Get_Center().y + far_side_margin: obj_track.bbox.Get_Center().y + close_side_margin;
      det_restrictions.min_long = obj_track.bbox.Get_Center().x - obj_track.bbox.Get_Length() * 0.5F;
      det_restrictions.max_long = obj_track.bbox.Get_Center().x + obj_track.bbox.Get_Length() * 0.5F;

      return det_restrictions;
   }


   /*===========================================================================*\
   * FUNCTION: Calc_New_VCS_Aligned_Sensor_Azim()
   *===========================================================================
   * RETURN VALUE:
   *
   * PARAMETERS:
   * const float32_t raw_az,
   * const float32_t (&angle_ambiguity_cands)[MAX_NUM_ANGLE_JUMPS],
   * const int32_t sensor_polarity,
   * const float32_t sensor_boresight_az
   * const float32_t (&jumped_az_vcs_vals)[MAX_NUM_ANGLE_JUMPS]
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
   * Calculate detection aziumth after jump correction for all valid angle jump candidates
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Calc_New_VCS_Aligned_Sensor_Azim(
      const float32_t raw_az,
      const float32_t (&angle_ambiguity_cands)[MAX_NUM_ANGLE_JUMPS],
      const int32_t sensor_polarity,
      const float32_t sensor_boresight_az,
      float32_t (&jumped_az_vcs_vals)[MAX_NUM_ANGLE_JUMPS]
   )
   {
      for (int8_t i=0; i < MAX_NUM_ANGLE_JUMPS; i++)
      {
         if (std::abs(angle_ambiguity_cands[i]) < F360_PI)
         {
            jumped_az_vcs_vals[i] = sensor_boresight_az + (raw_az + angle_ambiguity_cands[i]) * static_cast<float32_t>(sensor_polarity);
         }
         else
         {
            break;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Det_On_New_Azimuth_Valid()
   *===========================================================================
   * RETURN VALUE:
   * bool - detection validity.
   *
   * PARAMETERS:
   *  const float32_t det_range,
   *  const Angle &jumped_az_vcs,
   *  const Point &sensor_mount_pos_vcs,
   *  const Det_Restrictions_T det_restrictions
   *  const float32_t max_doube_range_hypothesis
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
   * Check if detection, after jump correction, is valid (i.e. meet some
   * specific conditions).
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Det_On_New_Azimuth_Valid(
      const float32_t det_range,
      const Angle &jumped_az_vcs,
      const Point &sensor_mount_pos_vcs,
      const Det_Restrictions_T &det_restrictions,
      const float32_t max_doube_range_hypothesis
   )
   {
      bool f_result = false;
      const float32_t long_component = det_range * jumped_az_vcs.Cos();
      const float32_t lat_component = det_range * jumped_az_vcs.Sin();

      Point new_pos;
      new_pos.x = sensor_mount_pos_vcs.x + long_component;
      new_pos.y = sensor_mount_pos_vcs.y + lat_component;
      const bool f_inside_simplified_bbox = Is_Det_Inside_Restricted_AJ_Bbox(new_pos, det_restrictions);

      if (f_inside_simplified_bbox)
      {
         f_result = true;
      }
      else if(det_range < max_doube_range_hypothesis)
      {
         Point new_double_pos;
         new_double_pos.x = sensor_mount_pos_vcs.x + 0.5F * long_component;
         new_double_pos.y = sensor_mount_pos_vcs.y + 0.5F * lat_component;
         f_result = Is_Det_Inside_Restricted_AJ_Bbox(new_double_pos, det_restrictions);
      }
      else
      {
         // Do nothing
      }

      return f_result;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Inside_Restricted_Bbox()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const Point &det_pos_vcs,
   * const Det_Restrictions_T &det_restrictions
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
   * Check if detection in inside restricted bounding box
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Det_Inside_Restricted_AJ_Bbox(
      const Point &det_pos_vcs,
      const Det_Restrictions_T &det_restrictions
   )
   {
      return ((det_restrictions.min_lat < det_pos_vcs.y)
         && (det_pos_vcs.y < det_restrictions.max_lat)
         && (det_restrictions.min_long < det_pos_vcs.x)
         && (det_pos_vcs.x < det_restrictions.max_long));
   }
}


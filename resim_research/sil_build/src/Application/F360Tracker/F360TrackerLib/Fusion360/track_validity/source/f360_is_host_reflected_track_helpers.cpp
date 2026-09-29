/*===================================================================================*\
* FILE: f360_is_host_reflected_track_helpers.cpp
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of supporting functions used in Is_Host_Reflected_Track().
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_is_host_reflected_track_helpers.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_check_if_point_is_inside_box.h"
#include "f360_math.h"
#include "f360_norm_heading_angle.h"
#include "f360_math_func.h"
#include "f360_bounding_box.h"
#include <algorithm>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox()
   *===========================================================================
   * RETURN VALUE:
   * bool f_is_in_bbox - Flag indicating whether predicted ghost mirror track is
   *                             placed in extended bounding box of analysed object.
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object       - Analysed object
   * const Point& sensor_mirror_tcs_pos      - Predicted ghost position in TCS of a sensor
   * const F360_Calibrations_T& calib        - Tracker calibrations
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
   * This function check whether predicted ghost position is placed in extended
   * bounding box of analysed object. Predicted ghost position is rotated to
   * match rotated host mirror if coming from an inclined SEP.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox(
      const F360_Object_Track_T& object,
      const Point& sensor_mirror_tcs_pos,
      const F360_Calibrations_T& calib,
      const F360_Host_T& host,
      const F360_Trailer_Estimator_Output_T& trailer_data)
   {

      float32_t box[2][2]{};

      float32_t half_length{};
      float32_t half_width{};
      half_length = 0.5F * object.bbox.Get_Length();
      half_width = 0.5F * object.bbox.Get_Width();

      if (host.f_trailer_presence_hardware)
      {
         const float32_t total_trailer_length = trailer_data.trailer_length[0] + trailer_data.trailer_length[1];
         const float32_t total_trailer_width = std::fmaxf(trailer_data.trailer_width[0], trailer_data.trailer_width[1]);
         half_width = total_trailer_width/2.0F;

         box[0][0] = -(half_length + calib.k_host_refl_bbox_long_ext);
         box[0][1] = (half_length + total_trailer_length + calib.k_host_refl_bbox_long_ext);
         box[1][0] = -(half_width + calib.k_host_refl_bbox_lat_ext);
         box[1][1] = (half_width + calib.k_host_refl_bbox_lat_ext);
      }
      else
      {
         box[0][0] = -(half_length + calib.k_host_refl_bbox_long_ext);
         box[0][1] = (half_length + calib.k_host_refl_bbox_long_ext);
         box[1][0] = -(half_width + calib.k_host_refl_bbox_lat_ext);
         box[1][1] = (half_width + calib.k_host_refl_bbox_lat_ext);

      }

      //Rotating Predicted Host Mirror Point since box calculated above assumes mirror object moves parallel to host and does not consider orientation
      Point rotated_sensor_mirror_tcs_pos;
      F360_Rotate_2D_Vector(sensor_mirror_tcs_pos.x, sensor_mirror_tcs_pos.y, object.bbox.Get_Orientation().Cos(), -object.bbox.Get_Orientation().Sin(), rotated_sensor_mirror_tcs_pos.x, rotated_sensor_mirror_tcs_pos.y);

      const bool f_is_in_bbox = Check_If_Point_Is_Inside_Box_In_Same_CS(rotated_sensor_mirror_tcs_pos.x, rotated_sensor_mirror_tcs_pos.y, box);

      return f_is_in_bbox;
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Predicted_Reflected_Track_TCS_Position()
   *===========================================================================
   * RETURN VALUE:
   * Point ghost_tcs_pos - Predicted host reflected track position in TCS of analysed track
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object   - Analysed object
   * const float32_t sensor_long_pos,    - Lateral position of SEP [m]
   * const float32_t sensor_lat_pos      - Assumed half of host length used to determine mirror track position [m]
   * const float32_t sep_lat_pos_sensor  - Lateral position of sensor's projection onto SEP [m]
   * const float32_t rot_angle		 - Rotation angle of the adjacent reflection point derived from SEP angle [rad]
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
   * This function calculates position of possible object reflection from host in analysed
   * track coordinates system.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   Point Calc_Predicted_Reflected_Track_TCS_Position(
      const F360_Object_Track_T& object,
      const float32_t sensor_long_pos,
      const float32_t sensor_lat_pos,
      const float32_t sep_lat_pos_sensor,
      const float32_t rot_angle)
   {
      Point ghost_tcs_pos = {};

     /* Before converting object reflection point to TCS, it needs to be rotated by appropriate angle (rot_angle) based on SEP angle */

     /* -First, reflection point from SEP at point directly adjacent to sensor is found (i.e. with no X offset from sensor) and stored in SEP coordinate system/SEP_cs with origin at SEP intersection point adjacent to sensor
        -If no SEP angle present, this is the location of possible host mirror and further calculations do not affect this host mirror point.
        -But if SEP angle present, this point needs to be rotated by rot_angle with origin of rotation at SEP_cs origin
        -After rotation, point is converted from SEP_cs to VCS and then to TCS */

     // Find reflection point from SEP adjacent to sensor in SEP_cs
      const float32_t unrotated_reflection_sep_cs_x = 0.0F;
      const float32_t unrotated_reflection_sep_cs_y = sep_lat_pos_sensor - sensor_lat_pos;

      float32_t rotated_reflection_sep_cs_x = 0.0F;
      float32_t rotated_reflection_sep_cs_y = 0.0F;

      Angle rotation_angle{};
      (void)rotation_angle.Value(rot_angle);

     // Rotate reflection point by rot_angle in SEP_cs with origin of rotation at SEP_cs origin (if ROT angle is 0 then output is same as input)
      F360_Rotate_2D_Vector(unrotated_reflection_sep_cs_x, unrotated_reflection_sep_cs_y, rotation_angle.Cos(), rotation_angle.Sin(), rotated_reflection_sep_cs_x, rotated_reflection_sep_cs_y);

     // Convert reflection point from SEP_cs to VCS
      const float32_t rotated_reflection_vcs_x = rotated_reflection_sep_cs_x + (sensor_long_pos);
      const float32_t rotated_reflection_vcs_y = rotated_reflection_sep_cs_y + (sep_lat_pos_sensor);

     // Convert reflection point from VCS To TCS
      Convert_VCS_Posn_To_TCS_Posn(
         rotated_reflection_vcs_x,
         rotated_reflection_vcs_y,
         object.bbox.Get_Center().x,
         object.bbox.Get_Center().y,
         object.bbox.Get_Orientation(),
         ghost_tcs_pos.x,
         ghost_tcs_pos.y);

      return ghost_tcs_pos;
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Heading_And_Speed_Threshold()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const float32_t host_speed - speed of host vehicle
   * const F360_Calibrations_T& calib - tracker calibrations
   * float32_t& max_heading - determined maximum heading, output value
   * float32_t& max_speed_diff - determined maximum speed diff, output value
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
   * This function picks maximum heading and calculates maximum speed difference
   * between host and analysed object to classify whether it is suspected of being
   * host mirror track.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Determine_Heading_And_Speed_Threshold(
      const float32_t host_speed,
      const float32_t rot_angle,
      const F360_Calibrations_T& calib,
      float32_t& max_heading,
      float32_t& max_speed_diff)
   {
      if (std::abs(host_speed) <= calib.k_host_refl_lowspeed_host_speed_th)
      {
         max_speed_diff = calib.k_host_refl_lowspeed_speed_diff_th;
         max_heading = std::abs(rot_angle) + calib.k_host_refl_lowspeed_heading_th;

      }
      else
      {
         max_heading = std::abs(rot_angle) + calib.k_host_refl_highspeed_heading_th;
         max_speed_diff = calib.k_host_refl_highspeed_min_speed_diff_th + (calib.k_host_refl_highspeed_speed_diff_ramp_coef * std::abs(host_speed));
         max_speed_diff = std::min(calib.k_host_refl_highspeed_max_speed_diff_th, max_speed_diff);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method()
   *===========================================================================
   * RETURN VALUE:
   * bool !f_is_not_suspected - Flag indicating whether analysed object is
   *                                    suspected of being host mirror track
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object - analysed object
   * const F360_Calibrations_T& calib - tracker calibrations
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
   * This function analyses whether object is suspected of being mirror track.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method(
      const F360_Host_T& host,
      const F360_Trailer_Estimator_Output_T& trailer_data,
      const F360_Object_Track_T& object,
      const F360_Calibrations_T& calib)
   {
      const bool f_is_host_dim_set = (host.vehicle_length > 0.0F) && (host.vehicle_width > 0.0F);
      const float32_t default_host_length = 5.0F;
      const float32_t host_length = f_is_host_dim_set ? host.vehicle_length : default_host_length;

      const float32_t host_and_trailer_length = host_length + trailer_data.trailer_length[0] + trailer_data.trailer_length[1];
      const float32_t lower_long_pos_threshold_for_host_reflection = host.f_trailer_presence_hardware ? -host_and_trailer_length : calib.k_host_refl_min_obj_long_pos;

      const bool f_suspected = (object.vcs_position.x > lower_long_pos_threshold_for_host_reflection)
         && (calib.k_host_refl_max_obj_long_pos > object.vcs_position.x)
         && object.f_moving;

      return f_suspected;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method()
   *===========================================================================
   * RETURN VALUE:
   * bool f_sep_valid
   *
   * PARAMETERS:
   * const F360_Host_T& host
   * const F360_Object_Track_T& object
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function determines whether object can be suspected to be host mirror.
   * Object must be getting closer to host, be closer than 55m to host and have
   * f_moving flag set.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method(const F360_Host_T& host, const F360_Object_Track_T& object)
   {
      bool f_suspected = false;
      const float32_t abs_object_vcs_y_pos = std::abs(object.vcs_position.y);
      const float32_t abs_object_vcs_heading = std::abs(object.vcs_heading.Value());
      const bool f_oncoming_offset_target = (abs_object_vcs_y_pos < 20.0F) &&
         (abs_object_vcs_y_pos > 3.0F) &&
         (abs_object_vcs_heading > F360_DEG2RAD(165.0F)); // If oncoming object is in front with some lateral offset and has very similar but opposite heading to host, then it is unlikely to be host mirror candidate

      if ((object.f_moving) && (!f_oncoming_offset_target))
      {
         const Point host_center(-host.dist_rear_axle_to_vcs_m * 0.6F, 0.0F);

         const Vector_T obj_velocity = { object.vcs_velocity };
         const Vector_T view_vector = { host_center, object.bbox.Get_Center() };

         const float32_t obj_proj_velocity = obj_velocity.Calc_Signed_Magnitude_Projected_On(view_vector);
         //check if both objects are approaching the mirror surface with range rate below set threshold
         const Point obj_center = object.bbox.Get_Center();
         const bool f_is_whithin_55m = ((std::abs(obj_center.x) < 55.0F) && (std::abs(obj_center.y) < 55.0F));

         // Negative value of the signed magnitude implies that object gets closer to the host
         const bool f_obj_approaches_host = obj_proj_velocity < 0.0F ? true : false;

         f_suspected = f_obj_approaches_host && f_is_whithin_55m;
      }
      else {/*do nothing*/ }
      return f_suspected;
   }

   /*===========================================================================*\
   * FUNCTION: Is_SEP_Valid_For_Host_Mirror_Ghost()
   *===========================================================================
   * RETURN VALUE:
   * bool f_sep_valid
   *
   * PARAMETERS:
   *  const Static_Env_Poly_T& sep,
   *  const F360_Calibrations_T& calib
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
   * This function evaluates if an SEP is valid to be used for countermeasure
   * Is_Host_Mirror_Ghost(). The SEP is valid if the SEP itself is valid,
   * the SEP interval is at least partly valid adjacent to host and does not
   * cross path with host.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Is_SEP_Valid_For_Host_Mirror_Ghost(
      const Static_Env_Poly_T& sep,
      const F360_Calibrations_T& calib)
   {
      const float32_t host_min_vcs_x = -2.0F * calib.k_host_refl_half_host_length;

      bool f_sep_valid = false;
      // Note that SEP is considered valid if it spans whole host length or if it begins and/or ends next to host
      if ((sep.status != F360_STATIC_ENV_POLY_STATUS_INVALID) &&
         (sep.upper_limit > host_min_vcs_x) && (sep.lower_limit < 0.0F))
      {
         const float32_t host_center_vcs_x = -calib.k_host_refl_half_host_length;
         const float32_t sep_lat_pos_host_center = sep.SEP_Lateral_Pos_At(host_center_vcs_x);

         const float32_t sep_long_pos_extreme_val = (-sep.p1) / (2.0F * sep.p2);
         const float32_t sep_lat_pos_extreme_val = sep.SEP_Lateral_Pos_At(sep_long_pos_extreme_val);

         // flag indicating that lateral position at host center and extreme value of LCS are different signs
         const bool pos_host_center_lat_pos_extreme_diff_sign = (sep_lat_pos_host_center * sep_lat_pos_extreme_val) < 0.0F;

         // SEP not valid if crossed path of the host
         if ((!pos_host_center_lat_pos_extreme_diff_sign) ||
            (std::abs(sep_long_pos_extreme_val) > calib.k_host_refl_filtering_distance))
         {
            f_sep_valid = true;
         }
         else
         {
            f_sep_valid = false;
         }
      }
      else
      {
         f_sep_valid = false;
      }

      return f_sep_valid;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Sensor_Valid_For_Host_Mirror_Reflection()
   *===========================================================================
   * RETURN VALUE:
   * bool f_sep_valid
   *
   * PARAMETERS:
   *  const Static_Env_Poly_T& sep,
   *  const float32_t& sensor_long_pos,
   *  const float32_t& sensor_lat_pos,
   *  const F360_Calibrations_T& calib
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
   * This function evaluates if sensor is on the same side as SEP and adjacent to the SEP.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Is_Sensor_Adjacent_To_SEP(
      const Static_Env_Poly_T& sep,
      const float32_t& sensor_long_pos,
      const float32_t& sensor_lat_pos,
      const F360_Calibrations_T& calib)
   {
      const float32_t sep_lat_pos_host_center = sep.SEP_Lateral_Pos_At(-calib.k_host_refl_half_host_length);

      // check if sensor is on the side of SEP
      const float32_t k_min_latpos = 0.5F;
      const bool sep_on_side = ((sep_lat_pos_host_center > 0.0F) && (sensor_lat_pos > k_min_latpos)) ||
         ((sep_lat_pos_host_center < 0.0F) && (sensor_lat_pos < -k_min_latpos));

      //check if sensor is adjacent to SEP
      const bool sep_adjacent = (sensor_long_pos > sep.lower_limit) && (sensor_long_pos < sep.upper_limit);
      const bool f_sensor_adjacent = (sep_on_side) && (sep_adjacent);

      return f_sensor_adjacent;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Ghost_Reflected_Host_Mirror_Without_SEP()
   *===========================================================================
   * RETURN VALUE:
   * bool f_reflective_guardrail_track
   *
   * PARAMETERS:
   * const F360_Object_Track_T& ghost_candidate,
   * const F360_Tracker_Info_T& tracker_info,
   * const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * const F360_Host_T& host
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function determines if a ghost candidate is a host reflection from a guardrail
   * when no SEP is created. The following steps show an overview of the logic:
   *
   * - A ghost candidate is checked against the host
   * - If their speeds are similar
   * - Create a line between the ghost candidate and the host and find the midpoint of this line - This is predicted reflection point
   * - Create a line at the predicted reflection point which is perpendicular to the line joining the host and ghost candidate - This is predicted guardrail line
   * - Confirm that ghost candidate pointing and host pointing are similar and in opposite direction to predicted guardrail line
   * - Search for stationary objects around predicted reflection point which are within a certain distance to the predicted guardrail line
   * - If more than 2 such objects detected, reflecting object hypothesis is true
   * - Mark ghost candidate as host mirror
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      const F360_Object_Track_T& ghost_candidate,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Host_T& host)

   {
      // host speed is checked to make sure it is not zero or very close to zero to prevent division by zero in speed comparison (Should never be zero due to earlier check on host speed before calling this function but added for safety)
      float32_t host_speed = host.speed;
      const float32_t abs_host_curvature = std::abs(host.curvature_rear);
      if (std::abs(host_speed) <= 0.1F)
      {
         host_speed = 0.1F; // prevent division by zero in speed comparison
      }

      bool f_reflective_guardrail_track = false;

      /* 
      Speed Diff Threshold Calculation
      - "speed_diff_threshold" is the maximum % speed difference between host and ghost candidate that is allowed for host mirror reflection check to proceed
      - F360_Linear_Equation_With_Saturation is used to interpolate speed_diff_threshold between 0.1 and 0.25 for abs_host_curvature in [0, 0.04] and saturated to 0.25 max.
      - i.e. for low curvature (straight driving) speed diff threshold is 0.1 (more strict) and for high curvature (tight turns) speed diff threshold is 0.25 (less strict) 
      */
      const float32_t speed_diff_threshold = F360_Linear_Equation_With_Saturation(
         abs_host_curvature,
         0.0F,           // min curvature [1/m]
         0.04F,          // max curvature [1/m]
         0.1F,           // speed diff threshold at min curvature [-] i.e. 10%
         0.25F           // speed diff threshold at max curvature [-] i.e. 25% (output is saturated to this value for curvature > max curvature)
      );
      
      if ((std::abs((host_speed - ghost_candidate.speed) / (host_speed)) < speed_diff_threshold)) // host speed is similar to ghost candidate speed
      {
         // Set up reference points for host and ghost candidate
         const Point host_center(-host.dist_rear_axle_to_vcs_m * 0.6F, 0.0F);
         const Point ghost_candidate_center(ghost_candidate.bbox.Get_Center().x, ghost_candidate.bbox.Get_Center().y);

         // Calculate the line from host to ghost candidate and its midpoint (which is also the suspected reflection point)
         const Line host_to_ghost_candidate_line(host_center, ghost_candidate_center);
         const Point host_to_ghost_candidate_midpoint = host_center.Calculate_Point_Fraction_Of_The_Way_To_Given_Point(ghost_candidate_center, 0.5F);

         // Calculate the perpendicular line to the host-ghost candidate line at its midpoint which is the suspected guardrail line and its angle
         const Line susp_guardrail_line = host_to_ghost_candidate_line.Get_Line_Perpendicular_At_Point(host_to_ghost_candidate_midpoint);
         const float32_t susp_guardrail_angle = F360_Atan2f(-susp_guardrail_line.Get_a(), susp_guardrail_line.Get_b());

         // Check that the pointing angle of ghost candidate and host to the suspected guardrail line is similar and in opposite direction to suspected guardrail before checking for stationary objects along the guardrail line
         const float32_t reflection_angle_diff_tolerance = abs_host_curvature < 0.04F ? F360_DEG2RAD(15.0F) : F360_DEG2RAD(25.0F); // if host is driving with high curvature (>= 0.04 1/m) then use larger angle tolerance of 25 degrees to account for pointing errors in tracking else use 15 degrees
         const float32_t expected_guardrail_angle = ghost_candidate.bbox.Get_Orientation().Value() * 0.5F; // Average pointing angle of ghost candidate and host is expected guardrail angle but host pointing angle is always 0 in vehicle coordinate system

         if ((std::abs(Normalize_Heading_Angle((susp_guardrail_angle - expected_guardrail_angle), 0.0F)) < reflection_angle_diff_tolerance) ||
            (std::abs(Normalize_Heading_Angle(((susp_guardrail_angle + F360_PI) - expected_guardrail_angle), 0.0F)) < reflection_angle_diff_tolerance))
         {
            int32_t nr_matching_guardrail_obj = 0;
            const float32_t min_dist_threshold = 3.0F; // This is the minimum distance that needs to be satisfied between the two guardrail objects either side and closest to the line joining host and ghost candidate. Ensures true reflective surface exists in between host and ghost and it is not two separate stationary objects.
            float32_t min_pos_dist = min_dist_threshold;
            float32_t min_neg_dist = -min_dist_threshold;
            bool f_found_pos = false;
            bool f_found_neg = false;
            // Loop over all active objects to find stationary objects close to the predicted reflection point and along the suspected guardrail line
            for (int32_t obj_idx = 0; obj_idx < tracker_info.num_active_objs; obj_idx++)
            {
               const int32_t guardrail_candidate_idx = tracker_info.active_obj_ids[obj_idx] - 1;
               const F360_Object_Track_T& guardrail_candidate = object_tracks[guardrail_candidate_idx];
               const float32_t guardrail_candidate_center_x = guardrail_candidate.bbox.Get_Center().x;
               const float32_t guardrail_candidate_center_y = guardrail_candidate.bbox.Get_Center().y;
               const float32_t predicted_guardrail_center_x = host_to_ghost_candidate_midpoint.x;
               const float32_t predicted_guardrail_center_y = host_to_ghost_candidate_midpoint.y;
               const float32_t guardrail_candidate_dist_to_susp_guardrail = susp_guardrail_line.Signed_Distance_To(guardrail_candidate.bbox.Get_Center());
               const float32_t guardrail_candidate_search_distance = 7.0F; // Max X and Y distance from predicted reflection point to search for guardrail objects

               const bool f_object_within_pred_zone = ((std::abs(guardrail_candidate_center_x - predicted_guardrail_center_x) < guardrail_candidate_search_distance) &&
                  ((std::abs(guardrail_candidate_center_y - predicted_guardrail_center_y) < guardrail_candidate_search_distance)) &&
                  (std::abs(guardrail_candidate_dist_to_susp_guardrail) < 2.0F));

               if ((guardrail_candidate.id != ghost_candidate.id) && // guardrail candidate is not ghost candidate
                  (guardrail_candidate.movable_prob < 0.5F) && // guardrail candidate is stationary and likely non moveable
                  (guardrail_candidate.status > F360_OBJECT_STATUS_NEW_UPDATED) && // guardrail candidate is not new object
                  (guardrail_candidate.otg_height < 5.0F) && // guardrail candidate is not underdrivable
                  (f_object_within_pred_zone)) // guardrail candidate is within predicted reflection zone
               {
                  const Point guardrail_point(guardrail_candidate_center_x, guardrail_candidate_center_y);
                  const float32_t dist = host_to_ghost_candidate_line.Signed_Distance_To(guardrail_point);
                  if ((dist > 0.0F) && (dist < min_pos_dist)) // This condition loops over all matching guardrail objects to find the closest object on positive side of the line joining host and ghost candidate (if it exists)
                  {
                     min_pos_dist = dist;
                     f_found_pos = true;
                  }
                  else if ((dist < 0.0F) && (dist > min_neg_dist)) // This condition loops over all matching guardrail objects to find the closest object on negative side of the line joining host and ghost candidate (if it exists)
                  {
                     min_neg_dist = dist;
                     f_found_neg = true;
                  }
                  else
                  {
                     // do nothing
                  }
                  nr_matching_guardrail_obj++;
                  if ((nr_matching_guardrail_obj > 4) && (f_found_pos) && (f_found_neg) && ((min_pos_dist - min_neg_dist) < min_dist_threshold))
                  {
                     break; // no need to continue searching if more than 4 matching guardrail objects which fulfill conditions are found
                  }
               }
            }
            // If more than 4 matching guardrail objects found then check if they are two objects on either side of the line joining host and ghost and they are close enough
            if (nr_matching_guardrail_obj > 4)
            {
               if ((f_found_pos) && (f_found_neg) && ((min_pos_dist - min_neg_dist) < min_dist_threshold))
               {
                  f_reflective_guardrail_track = true;
               }
            }
         }
      }
      return f_reflective_guardrail_track;
   }
}

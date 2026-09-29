/*===================================================================================*\
* FILE: f360_is_reflective_guardrail_track.cpp
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definiton of Is_Reflective_Guardrail_Track() and
* related sub function
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_is_reflective_guardrail_track.h"
#include "f360_math_func.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_check_if_point_is_inside_box.h"
#include "f360_norm_heading_angle.h"
#include <algorithm>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Is_Reflective_Guardrail_Track()
   *===========================================================================
   * RETURN VALUE:
   * True if guardrail is reflective; false otherwise
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T &tracker_info - tracker info struct
   * const int32_t ghost_candidate_idx - index of object track which is considered to be a ghost created by reflection of a real object
   * const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS]
   * const F360_Calibrations_T &cal - tracker calibrations
   * const F360_Host_T& host - Host info struct
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS] - object tracks array
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function determines if a ghost candidate is a reflection guardrail track
   *
   \*===========================================================================*/
   bool Is_Reflective_Guardrail_Track(
      const F360_Tracker_Info_T& tracker_info,
      const int32_t ghost_candidate_idx,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T& cal,
      const F360_Host_T& host,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   )
   {
      /* Check if this is a target behind guardrail can be a ghost created by reflection real source object from the guardrail.
      * We know host position and reflected object track candidate position. If we know as guardrail parameters as well from created SEP
      * we calcualte the position of source of reflection on the opposite side of the guardrail.
      * Then we iterate over object tracks to find the object which position matches roughly the source position
      * and other parameters matches approximately the parameters of reflected object candidate.
      * Is a matchin object track is found than function returns flag f_reflective_guardrail_track = true
      * what means that the reflected object track candidate is actually a ghost being only reflection of matching source object.
      * This is done in Is_Ghost_Reflected_By_SEP() function.
      */

      /* Consequently, if a SEP is not created from a guardrail or reflecting surface due to not fulfilling all criteria, a reflected
      * ghost is still checked for using a candidate ghost object, a matching stable source object and host properties. If potential reflecting
      * pair is found, then geometric checks are used to predict a reflecting surface and if stationary objects are found in this area,
      * candidate is considered to be a ghost. This is done in Is_Ghost_Reflected_Without_SEP() function.*/

      bool f_reflective_guardrail_track = false;
      const F360_Object_Track_T& ghost_candidate = object_tracks[ghost_candidate_idx];
      if (Is_Ghost_Obj_Valid_For_Reflection_Check(ghost_candidate, cal))
      {
         const float32_t min_mirror_prob_no_sep_check = 0.6F; // Minimum mirror probability for ghost candidate to be checked as potential ghost without SEP i.e. only check previously mirrored or newly init objects as ghost candidate
         for (int32_t array_idx = 0; array_idx < tracker_info.num_active_objs; array_idx++)
         {
            const int32_t source_candidate_idx = tracker_info.active_obj_ids[array_idx] - 1;
            const F360_Object_Track_T& source_candidate = object_tracks[source_candidate_idx];
            if (Is_Source_Obj_Valid_For_Reflection_Check(ghost_candidate, source_candidate, cal))
            {
               if ((ghost_candidate.behind_sep_id != F360_INVALID_UNSIGNED_ID) &&// ghost candidate is behind SEP and source candidate is not behind SEP (condition for checking reflected ghost from sep)
                   (source_candidate.behind_sep_id == F360_INVALID_UNSIGNED_ID))
               {
                  const uint8_t sep_idx = ghost_candidate.behind_sep_id - 1U;
                  const Point ghost_candidate_center_vcs = ghost_candidate.bbox.Get_Center();
                  f_reflective_guardrail_track = Is_Ghost_Reflected_By_SEP(sep[sep_idx], ghost_candidate, ghost_candidate_center_vcs, source_candidate, cal);
               }
               if ((!f_reflective_guardrail_track) && (ghost_candidate.mirror_prob > min_mirror_prob_no_sep_check)) // ghost candidate has a minimum mirror prob for continuing reflected ghost from no sep check
               {
                  f_reflective_guardrail_track = Is_Ghost_Reflected_Without_SEP(ghost_candidate, source_candidate, tracker_info, object_tracks, host);
               }
               if (f_reflective_guardrail_track)
               {
                  break; // exit for loop if ghost is confirmed
               }
            }
         }
      }
      if (f_reflective_guardrail_track)
      {
         object_tracks[ghost_candidate_idx].mirror_prob = 1.0F;
         //reset counter for straight driving
         object_tracks[ghost_candidate_idx].cntHostTurnForMirrorProb = 0;
      }
      return f_reflective_guardrail_track;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Ghost_Obj_Valid_For_Reflection_Check()
   *===========================================================================
   * RETURN VALUE:
   * bool f_obj_valid
   *
   * PARAMETERS:
   * const F360_Object_Track_T & ghost_candidate - reference to object to be checked
   * const F360_Calibrations_T &cal - tracker calibrations
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
   * This function determines if a ghost candidate is valid for further checks
   * as a potential ghost due to guardrail reflections
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Ghost_Obj_Valid_For_Reflection_Check(
      const F360_Object_Track_T& ghost_candidate,
      const F360_Calibrations_T& calib
   )
   {
      const bool f_filter_type_CTCA_Fast_CCA = (F360_TRACKER_TRKFLTR_CTCA == ghost_candidate.trk_fltr_type) || (std::abs(ghost_candidate.speed) > calib.fast_moving_thresh); //source candidate is CTCA or fast moving CCA
      const bool f_obj_valid = (f_filter_type_CTCA_Fast_CCA && ghost_candidate.f_moving);
      return f_obj_valid;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Source_Obj_Valid_For_Reflection_Check()
   *===========================================================================
   * RETURN VALUE:
   * bool f_obj_valid
   *
   * PARAMETERS:
   * const F360_Object_Track_T & ghost_candidate - reference to object to be checked
   * const F360_Calibrations_T &cal - tracker calibrations
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
   * This function determines if the source candidate for reflected track check
   * is valid for use in reflection check
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Source_Obj_Valid_For_Reflection_Check(
      const F360_Object_Track_T& ghost_candidate,
      const F360_Object_Track_T& source_candidate,
      const F360_Calibrations_T& calib
   )
   {
      bool f_obj_valid = false;
      if ((source_candidate.id != ghost_candidate.id) && // ghost and source candidate are different objects
         ((F360_TRACKER_TRKFLTR_CTCA == source_candidate.trk_fltr_type) || (std::abs(source_candidate.speed) > calib.fast_moving_thresh)) && // source candidate is CTCA or fast moving CCA
         (F360_INVALID_UNSIGNED_ID == source_candidate.on_sep_id) && // source candidate is not on SEP
         (source_candidate.f_moving) && // source candidate is moving
         (source_candidate.status > F360_OBJECT_STATUS_NEW_UPDATED) && // source candidate is not new or new coasted
         (source_candidate.reduced_id > 0)) // source candidate has a valid reduced ID
      {
         f_obj_valid = true;
      }

      return f_obj_valid;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Ghost_Reflected_By_SEP()
   *===========================================================================
   * RETURN VALUE:
   * bool f_reflective_guardrail_track
   *
   * PARAMETERS:
   * const Static_Env_Poly_T& sep,
   * const F360_Object_Track_T &ghost_candidate - object track considered to be candidate of a ghost reflected from source
   * const Point &ghost_cand_pos - ghost candidate position in VCS
   * const F360_Tracker_Info_T &tracker_info - tracker info struct
   * const F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS] - object tracks array
   * const F360_Calibrations_T &cal - tracker calibrations
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
   * This function determines if a ghost candidate is a reflection of SEP
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Ghost_Reflected_By_SEP(
      const Static_Env_Poly_T& sep,
      const F360_Object_Track_T& ghost_candidate,
      const Point& ghost_cand_pos,
      const F360_Object_Track_T& source_candidate,
      const F360_Calibrations_T& cal)
   {
      bool f_reflective_guardrail_track = false;

      // Check that point of reflection is not too far from host
      if ((ghost_candidate.sep_intersection_point.x >= cal.k_tv_refl_gr_trk_min_sep_lon_pos) && 
          (ghost_candidate.sep_intersection_point.x <= cal.k_tv_refl_gr_trk_max_sep_lon_pos))
      {
         const Point center_of_symmetry = Calculate_Center_Of_Symmetry_SEP(ghost_cand_pos, ghost_candidate.sep_intersection_point, sep);

         // Calculate hypothetical point that can be the source of reflection for ghost candidate
         Point hypothetic_source_pos_vcs;
         hypothetic_source_pos_vcs.x = (2.0F * center_of_symmetry.x) - ghost_cand_pos.x;
         hypothetic_source_pos_vcs.y = (2.0F * center_of_symmetry.y) - ghost_cand_pos.y;

         const float32_t tangent_slope = (2.0F * sep.p2 * ghost_candidate.sep_intersection_point.x) + sep.p1;
         const float32_t sep_tangent_angle = F360_Atanf(tangent_slope);

         if (Is_Source_Candidate_Similar_To_Ghost_Candidate(source_candidate, ghost_candidate, hypothetic_source_pos_vcs, sep_tangent_angle, cal))
         {
            f_reflective_guardrail_track = true;
         }
      }
      return f_reflective_guardrail_track;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Ghost_Reflected_Without_SEP()
   *===========================================================================
   * RETURN VALUE:
   * bool f_reflective_guardrail_track
   *
   * PARAMETERS:
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
   * This function determines if a ghost candidate is a reflection from a guardrail
   * when no SEP is created. The following steps show an overview of the logic:
   * 
   * - A ghost candidate is checked against a source candidate
   * - If their speeds are similar
   * - Create a line in between the two objects which is perpendicular to the line joining the two - This is predicted guardrail line
   * - Create a line between the ghost candidate and the host and find its intersection with predicted guardrail line - This is predicted reflection point
   * - Confirm that ghost candidate heading and source candidate heading are similar and in opposite direction to predicted guardrail line
   * - Search for stationary objects around predicted reflection point which are within a certain distance to the predicted guardrail line
   * - If more than 2 such objects detected, reflecting object hypothesis is true
   * - Mark ghost candidate as mirrored ghost

   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Ghost_Reflected_Without_SEP(
      const F360_Object_Track_T& ghost_candidate,
      const F360_Object_Track_T& source_candidate,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Host_T& host)

   {
      // source candidate speed is checked to make sure it is not zero or very close to zero to prevent division by zero in speed comparison
      float32_t source_candidate_speed = source_candidate.speed;
      const float32_t abs_host_curvature = std::abs(host.curvature_rear);
      
      const float32_t high_host_curvature_limit = 0.04F; // [1/m] Limit of host curvature above which host is considered to have high curvature. Speed and Angle check conditions below are slightly relaxed above this limit
      const bool f_high_curvature_host = (abs_host_curvature > high_host_curvature_limit);
      const bool f_ghost_candidate_prev_flagged_mirror = (ghost_candidate.mirror_prob > 0.9F);
      const bool f_relaxed_conditions = (f_high_curvature_host && f_ghost_candidate_prev_flagged_mirror); // Speed check and angle check conditions are relaxed if ghost candidate was already flagged as ghost previously and host is taking a sharp curve since tracking can be less accurate in these scenarios

      const float32_t ghost_to_source_rel_speed_diff_lim = f_relaxed_conditions ? 0.15F : 0.1F; // [-] Speed difference allowed between ghost and source candidates.

      if((source_candidate_speed <= 0.1F) && (source_candidate_speed >= -0.1F))
      {
         source_candidate_speed = 0.1F; // prevent division by zero in speed comparison
      }
      bool f_reflective_guardrail_track = false;
      if ((std::abs((source_candidate_speed - ghost_candidate.speed) / (source_candidate_speed)) < ghost_to_source_rel_speed_diff_lim) && // source candidate speed is similar to ghost candidate speed
         (std::abs(source_candidate.vcs_position.y) < std::abs(ghost_candidate.vcs_position.y))) // source candidate is closer to host laterally than ghost candidate (true for guardrail reflections)
      {
         // Set up reference points for host, source and ghost candidate
         const Point host_center(-host.dist_rear_axle_to_vcs_m * 0.6F, 0.0F);
         const Point source_candidate_refpt(source_candidate.predicted_vcs_position.x, source_candidate.predicted_vcs_position.y);
         const Point ghost_candidate_refpt(ghost_candidate.predicted_vcs_position.x, ghost_candidate.predicted_vcs_position.y);

         // Calculate the line from host to ghost candidate
         const Line host_to_ghost_candidate_line(host_center, ghost_candidate_refpt);

         // Calculate the line from source candidate to ghost candidate and its midpoint
         const Line source_to_ghost_candidate_line(source_candidate_refpt, ghost_candidate_refpt);
         const Point source_to_ghost_candidate_midpoint = source_candidate_refpt.Calculate_Point_Fraction_Of_The_Way_To_Given_Point(ghost_candidate_refpt, 0.5F);

         // Calculate the perpendicular line to the source-ghost candidate line at its midpoint which is the suspected guardrail line and its angle
         const Line susp_guardrail_line = source_to_ghost_candidate_line.Get_Line_Perpendicular_At_Point(source_to_ghost_candidate_midpoint);
         const float32_t susp_guardrail_angle = F360_Atan2f(-susp_guardrail_line.Get_a(), susp_guardrail_line.Get_b());

         // Check that the angle of ghost candidate and source candidate to the suspected guardrail line is similar and in opposite direction to suspected guardrail before checking for stationary objects along the guardrail line
         const float32_t reflection_angle_diff_tolerance = f_relaxed_conditions ? F360_DEG2RAD(25.0F) : F360_DEG2RAD(15.0F); // [-] Angle Diff allowed between expected and suspected guardrail
         const float32_t expected_guardrail_angle = (ghost_candidate.vcs_heading.Value() + source_candidate.vcs_heading.Value()) * 0.5F; // if ghost and source candidates are actually mirror pairs then the guardrail angle should be halfway between their headings

         if ((std::abs(Normalize_Heading_Angle((susp_guardrail_angle - expected_guardrail_angle), 0.0F)) < reflection_angle_diff_tolerance) || 
            (std::abs(Normalize_Heading_Angle(((susp_guardrail_angle + F360_PI) - expected_guardrail_angle), 0.0F)) < reflection_angle_diff_tolerance))
         {
            // Calculate the intersection point of the host-ghost candidate line and the suspected guardrail line which is the predicted reflection point on the guardrail
            const Point host_to_ghost_candidate_guardrail_intersection = host_to_ghost_candidate_line.Find_Intersection(susp_guardrail_line).coordinates;
            int32_t nr_matching_guardrail_obj = 0;
            // Loop over all active objects to find stationary CCA objects close to the predicted reflection point and along the suspected guardrail line
            for (int32_t obj_idx = 0; obj_idx < tracker_info.num_active_objs; obj_idx++)
            {
               const int32_t guardrail_candidate_idx = tracker_info.active_obj_ids[obj_idx] - 1;
               const F360_Object_Track_T& guardrail_candidate = object_tracks[guardrail_candidate_idx];
               const float32_t guardrail_candidate_center_x = guardrail_candidate.bbox.Get_Center().x;
               const float32_t guardrail_candidate_center_y = guardrail_candidate.bbox.Get_Center().y;
               const float32_t predicted_guardrail_center_x = host_to_ghost_candidate_guardrail_intersection.x;
               const float32_t predicted_guardrail_center_y = host_to_ghost_candidate_guardrail_intersection.y;
               const float32_t guardrail_candidate_dist_to_susp_guardrail = susp_guardrail_line.Signed_Distance_To(guardrail_candidate.bbox.Get_Center());
               const bool f_object_within_pred_zone = ((std::abs(guardrail_candidate_center_x - predicted_guardrail_center_x) < 5.0F) &&
                  ((std::abs(guardrail_candidate_center_y - predicted_guardrail_center_y) < 5.0F)) && (std::abs(guardrail_candidate_dist_to_susp_guardrail) < 2.0F));

               if ((guardrail_candidate.id != ghost_candidate.id) && // guardrail candidate is not ghost candidate
                  (guardrail_candidate.id != source_candidate.id) && // guardrail candidate is not source candidate
                  (F360_TRACKER_TRKFLTR_CCA == guardrail_candidate.trk_fltr_type) && // guardrail candidate is CCA
                  (guardrail_candidate.movable_prob < 0.5F) && // guardrail candidate is stationary and likely non moveable
                  (guardrail_candidate.status > F360_OBJECT_STATUS_NEW_UPDATED) && // guardrail candidate is not new object
                  (guardrail_candidate.otg_height < 5.0F) && // guardrail candidate is not underdrivable
                  (f_object_within_pred_zone)) // guardrail candidate is within predicted reflection zone which is defined as within 5m longitudinally and laterally of predicted reflection point AND within 2m of the suspected guardrail line
               {
                  nr_matching_guardrail_obj++;
               }
               if (nr_matching_guardrail_obj > 2)
               {
                  f_reflective_guardrail_track = true; // if at least 3 stationary objects found along suspected guardrail line close to predicted reflection point then consider ghost candidate to be a reflection ghost
                  break;
               }
            }
         }
      }
      return f_reflective_guardrail_track;
   }

   /*===========================================================================*\
   * FUNCTION: Calculate_Center_Of_Symmetry_SEP()
   *===========================================================================
   * RETURN VALUE:
   * Point center_of_symmetry
   *
   * PARAMETERS:
   * const Point &ghost_cand_pos,
   * const Point &point_of_reflection,
   * const Static_Env_Poly_T& sep
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
   * This function calculates the center of symmetry. The tangent line to the SEP is calculated at the
   * point of reflection. The center of symmetry is found along this tangent, at the point from where the
   * normal of the tangent line crosses the ghost position.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Point Calculate_Center_Of_Symmetry_SEP(
      const Point& ghost_cand_pos,
      const Point& point_of_reflection,
      const Static_Env_Poly_T& sep)
   {
      const float32_t p2 = sep.p2;
      const float32_t p1 = sep.p1;
      const float32_t p0 = sep.p0;

      const float32_t por_x_pow_2 = point_of_reflection.x * point_of_reflection.x;
      const float32_t por_x_pow_3 = por_x_pow_2 * point_of_reflection.x;
      const float32_t tangent_slope = 2.0F * p2 * point_of_reflection.x + p1;
      const float32_t center_of_symmetry_x_numerator = tangent_slope * (ghost_cand_pos.y - p0) + 2.0F * p2 * p2 * por_x_pow_3 + p2 * p1 * por_x_pow_2 + ghost_cand_pos.x;
      const float32_t center_of_symmetry_x_denominator = tangent_slope * tangent_slope + 1.0F;

      Point center_of_symmetry = {};
      center_of_symmetry.x = center_of_symmetry_x_numerator / center_of_symmetry_x_denominator;

      if (std::abs(tangent_slope) < F360_EPSILON)
      {
         // SEP is very close to straight longitudinally, i.e. tangent slope very close to 0.
         // Thus, center of symmetry can be approximated on the tangent line.
         center_of_symmetry.y = point_of_reflection.y;
      }
      else
      {
         center_of_symmetry.y = ghost_cand_pos.y + (ghost_cand_pos.x - center_of_symmetry.x) / tangent_slope;
      }

      return center_of_symmetry;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Heading_Consistent_With_SEP_Reflection()
   *===========================================================================
   * RETURN VALUE:
   * bool - true if the heading constraint for mirror reflection is satisfied
   *
   * PARAMETERS:
   * const float32_t source_heading_vcs - heading of source object (radians, VCS)
   * const float32_t ghost_heading_vcs  - heading of ghost object (radians, VCS)
   * const float32_t tangent_angle_vcs  - tangent angle of SEP parabola at reflection point (radians, VCS)
   * const float32_t tolerance          - maximum allowed angular deviation (radians)
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Checks whether the source and ghost headings are consistent with a mirror
   * reflection about the SEP tangent line at the reflection point.
   *
   * Both headings are transformed into the local tangent frame by subtracting
   * the tangent angle, then normalized to [-pi, pi]. For a true mirror
   * reflection the ghost heading in the local frame should be the negation of
   * the source heading in the local frame. The function returns true when
   * |theta_ghost_local + theta_source_local| <= tolerance.
   *
   \*===========================================================================*/
   bool Is_Heading_Consistent_With_SEP_Reflection(
      const float32_t source_heading_vcs,
      const float32_t ghost_heading_vcs,
      const float32_t tangent_angle_vcs,
      const float32_t tolerance)
   {
      // Transform headings into the local tangent frame
      const float32_t source_heading_local = Normalize_Heading_Angle(source_heading_vcs - tangent_angle_vcs, 0.0F);
      const float32_t ghost_heading_local  = Normalize_Heading_Angle(ghost_heading_vcs  - tangent_angle_vcs, 0.0F);

      // For a mirror reflection: ghost_heading_local should equal -source_heading_local
      const float32_t heading_sum = Normalize_Heading_Angle(ghost_heading_local + source_heading_local, 0.0F);

      return (std::abs(heading_sum) <= tolerance);
   }

   /*===========================================================================*\
   * FUNCTION: Is_Source_Candidate_Similar_To_Ghost_Candidate()
   *===========================================================================
   * RETURN VALUE:
   * bool f_candidates_similar
   *
   * PARAMETERS:
   * const F360_Object_Track_T &source_candidate, object track considered to be candidate of a source of reflection
   * const F360_Object_Track_T &ghost_candidate, object track considered to be candidate of a ghost reflected from source
   * const Point &hypothetic_source_pos, hypothetic position of source candidate center
   * const F360_Calibrations_T &cal, tracker calibrations
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
   * This function evaluates the similarity between ghost and source candidates
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Source_Candidate_Similar_To_Ghost_Candidate(
      const F360_Object_Track_T& source_candidate,
      const F360_Object_Track_T& ghost_candidate,
      const Point& hypothetic_source_pos,
      const float32_t sep_tangent_angle,
      const F360_Calibrations_T& cal)
   {
      const Point source_candidate_pos_vcs = source_candidate.bbox.Get_Center();

      float32_t min_spd = std::min(std::abs(source_candidate.speed), std::abs(ghost_candidate.speed));
      min_spd = std::abs(min_spd) < F360_EPSILON ? (F360_EPSILON * static_cast<float32_t>(F360_Sign(min_spd))) : min_spd;
      const float32_t diff_vel_rel = std::abs(source_candidate.speed - ghost_candidate.speed) / min_spd;

      const float32_t source_heading = Normalize_Heading_Angle(source_candidate.vcs_heading.Value(), 0.0F);
      const float32_t expected_ghost_heading = (ghost_candidate.f_oncoming) ? -source_heading : source_heading;

      const bool relative_velocity_diff = (diff_vel_rel < cal.k_tv_refl_gr_trk_max_diff_rel_vel);
      const bool pos_y_diff = (std::abs(source_candidate_pos_vcs.y - hypothetic_source_pos.y) < cal.k_tv_refl_gr_trk_max_diff_y);
      const bool source_similar_to_ghost_heading = (std::abs(Normalize_Heading_Angle(ghost_candidate.vcs_heading.Value() - expected_ghost_heading, 0.0F)) < cal.k_tv_refl_gr_trk_max_diff_heading);

      const bool f_sep_reflection_heading_consistent = Is_Heading_Consistent_With_SEP_Reflection(source_candidate.vcs_heading.Value(),
                                                                                                 ghost_candidate.vcs_heading.Value(),
                                                                                                 sep_tangent_angle,
                                                                                                 cal.k_tv_refl_gr_trk_max_diff_heading);
      
      bool f_candidates_similar = (relative_velocity_diff) && (pos_y_diff) && (source_similar_to_ghost_heading) && (f_sep_reflection_heading_consistent);

      if (f_candidates_similar) {

         float32_t max_ghost_diff_x_adapt;
         if (std::abs(source_candidate.vcs_heading.Value()) < cal.k_tv_refl_gr_trk_straight_mov_head_th)
         {
            max_ghost_diff_x_adapt = std::max(cal.k_tv_refl_gr_trk_max_diff_x, 0.5F * source_candidate.bbox.Get_Length());
         }
         else
         {
            max_ghost_diff_x_adapt = cal.k_tv_refl_gr_trk_max_diff_x;
         }

         const float32_t diff_x = std::abs(source_candidate_pos_vcs.x - hypothetic_source_pos.x);

         f_candidates_similar = ((diff_x < max_ghost_diff_x_adapt) ||
            Is_Hypot_Source_Pos_In_Source_Candidate_BBox(hypothetic_source_pos, source_candidate, ghost_candidate, cal));
      }

      return f_candidates_similar;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Hypot_Source_Pos_In_Source_Candidate_BBox()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const Point &hypothetic_source_pos - Hypothetic position of source candidate center
   * const F360_Object_Track_T &source_candidate - Object track considered to be candidate of a source of reflection
   * const F360_Object_Track_T &ghost_candidate - Object track considered to be candidate of a ghost reflected from source
   * const F360_Calibrations_T &cal - Tracker calibrations
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
   * This function checks whether the hypothetic source point is located within
   * source candidate bounding box with some margin.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Hypot_Source_Pos_In_Source_Candidate_BBox(
      const Point& hypothetic_source_pos,
      const F360_Object_Track_T& source_candidate,
      const F360_Object_Track_T& ghost_candidate,
      const F360_Calibrations_T& cal)
   {
      float32_t x_in_source_candidate_tcs;
      float32_t y_in_source_candidate_tcs;
      Convert_VCS_Posn_To_TCS_Posn(
         hypothetic_source_pos.x,
         hypothetic_source_pos.y,
         source_candidate.bbox.Get_Center().x,
         source_candidate.bbox.Get_Center().y,
         source_candidate.bbox.Get_Orientation(),
         x_in_source_candidate_tcs,
         y_in_source_candidate_tcs);

      const float32_t bbox_long_margin = std::max(cal.k_tv_refl_gr_trk_min_bbox_lat_margin, ghost_candidate.bbox.Get_Length());
      const float32_t half_length = 0.5F * source_candidate.bbox.Get_Length();
      const float32_t half_width = 0.5F * source_candidate.bbox.Get_Width();
      float32_t box[2][2];
      box[0][0] = -(half_length + bbox_long_margin);
      box[0][1] = half_length + bbox_long_margin;
      box[1][0] = -(half_width + cal.k_tv_refl_gr_trk_bbox_lat_margin);
      box[1][1] = half_width + cal.k_tv_refl_gr_trk_bbox_lat_margin;

      return Check_If_Point_Is_Inside_Box_In_Same_CS(x_in_source_candidate_tcs, y_in_source_candidate_tcs, box);
   }
}

/*===================================================================================*\
* FILE:  f360_multipath_detector.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of Multipath_Detector class methods.
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/


#include "f360_multipath_detector.h"
#include "f360_reflector_object.h"
#include "f360_math_func.h"
#include "f360_constants.h"


namespace f360_variant_A
{
   static float32_t Calc_Searching_Range(
      const Point &radar_pos,
      const Point &reflector_pos,
      const Point &mp_candidate_pos
   );

   /*=========================================================================
   * Method         Is_Multipath
   *
   * Description    Check if in provided mp_candidate can be a multipath.
   *
   * Parameters
   *  const Point &radar_pos - radar (wave source) position
   *  F360_Object_Track_T& mp_object_candidate - object which is mp_candidate
   *  const float32_t mp_candidate_range_rate - mp_candidate range rate
   *  const F360_Tracker_Info_T& tracker_info
   *
   * Returns        bool - true if mp_candidate is potential multipath.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   bool Multipath_Detector::Is_Multipath(
      const Point &radar_pos,
      const F360_Object_Track_T& mp_object_candidate,
      const float32_t mp_candidate_range_rate,
      const F360_Tracker_Info_T& tracker_info)
   {
      bool f_multipath;
      const Point mp_candidate_pos = { mp_object_candidate.vcs_position.x,  mp_object_candidate.vcs_position.y };
      const std::pair<bool, Point> reflection_point = refl_selector.Get_Reflection_Point(radar_pos, mp_candidate_pos, m_objects);
      const bool &f_reflection_found = reflection_point.first;
      if (f_reflection_found)
      {
         const float32_t searching_range = Calc_Searching_Range(radar_pos, reflection_point.second, mp_candidate_pos);
         f_multipath = Find_Corresponding_Source(radar_pos, reflection_point.second, searching_range, mp_object_candidate, mp_candidate_range_rate, tracker_info);
      }
      else
      {
         f_multipath = false;
      }


      return f_multipath;
   }

   /*=========================================================================
   * Method         Get_Source_Reflector
   *
   * Description    Returns found source reflector.
   *
   * Parameters     None.
   *
   * Returns        Object_Reflector* - pointer to found reflector..
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   const Object_Reflector* Multipath_Detector::Get_Source_Reflector()
   {
      Object_Reflector*reflector;

      if (obj_source.Is_Valid())
      {
         reflector = &obj_source;
      }
      else
      {
         reflector = nullptr;
      }

      return reflector;
   }

   /*=========================================================================
   * Method         Find_Next_Potential_Source
   *
   * Description    Substep of Find_Corresponding_Source method. It evaluets if 
   *                reflector can be the source reflector.
   *
   * Parameters
   * const int32_t mp_object_candidate_id
   * const Point &reflection_point
   * const float32_t remaining_dist
   * const F360_Tracker_Info_T& tracker_info
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void Multipath_Detector::Find_Next_Potential_Source(
      const int32_t mp_object_candidate_id,
      const Point &reflection_point,
      const float32_t remaining_dist,
      const F360_Tracker_Info_T& tracker_info)
   {
      obj_source = {};

      for (int32_t k = last_obj_src_idx + 1; k < static_cast<int32_t>(tracker_info.variant.num_tracks); k++)
      {
         const F360_Object_Track_T &source_obj = m_objects[k];
         
         if ((F360_Object_Status_T::F360_OBJECT_STATUS_INVALID != source_obj.status)
            && (F360_Object_Status_T::F360_OBJECT_STATUS_COASTED != source_obj.status)
            && (source_obj.f_moving)
            && (source_obj.id != mp_object_candidate_id))
         {
            const Object_Reflector reflector { source_obj };
            if (Verify_Distance_Hypothesis(reflector, reflection_point, remaining_dist))
            {
               last_obj_src_idx = k;
               obj_source = { m_objects[k] };
               break;
            }
         }
      }
   }

   /*=========================================================================
   * Method         Find_Corresponding_Source
   *
   * Description    For given reflection point, it looks for source reflector.
   *
   * Parameters
   *  const Point &radar_pos
   *  const Point &reflection_point
   *  const float32_t searching_range
   *  const Point &mp_candidate_pos,
   *  const float32_t mp_candidate_range_rate
   *  const F360_Tracker_Info_T& tracker_info
   *
   * Returns        bool - true if source is found.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   bool Multipath_Detector::Find_Corresponding_Source(
      const Point &radar_pos,
      const Point &reflection_point,
      const float32_t searching_range,
      const F360_Object_Track_T& mp_object_candidate,
      const float32_t mp_candidate_range_rate,
      const F360_Tracker_Info_T& tracker_info)
   {
      bool f_source_found = false;
      last_obj_src_idx = -1;

      while (!f_source_found)
      {
         Find_Next_Potential_Source(mp_object_candidate.id, reflection_point, searching_range, tracker_info);
         const Object_Reflector* const potential_src_reflector = Get_Source_Reflector();

         if (nullptr != potential_src_reflector)
         {
            if (!potential_src_reflector->Is_Inside(mp_object_candidate.vcs_position))
            {
               // Verify distance ratio is acceptable OR reflection angle is valid
               const Point source_pos = potential_src_reflector->Get_Object_Center_VCS_Position();
               const float32_t max_reflection_angle= 2.618F;  // 150 degrees in radians

               // As long as there is one of the three conditions met that supports the multipath hypothesis, the candidate can be considered as multipath and proceed to range rate verification. Otherwise, if both metrics fail, the candidate will be rejected as multipath.
               // The 3 metrics that allow the multipath: Distance to host, ratio between the distance from radar to reflection point and the distance from reflection point to source, and reflection angle.
               const bool f_distance_or_angle_valid = Distances_ok_for_Multipath(radar_pos, reflection_point, source_pos) ||
                                                      Reflection_Angle_ok_for_Multipath(radar_pos, reflection_point, source_pos, max_reflection_angle);

               if (f_distance_or_angle_valid)
               {
                  // Only proceed to range rate if either the distance or angle hypothesis is valid. Otherwise always return false
                  f_source_found = Verify_Range_Rate_Hypothesis(
                     *potential_src_reflector,
                     reflection_point,
                     searching_range,
                     mp_candidate_range_rate);
               }
            }
         }
         else
         {
            break;
         }
      }

      return f_source_found;
   }

   /*=========================================================================
   * Method         Verify_Range_Rate_Hypothesis
   *
   * Description    Verifies range rate hypothesis of true mutipath model.
   *
   * Parameters
   *  const Object_Reflector& reflector
   *  const Point &reflection_point
   *  const float32_t searching_range
   *  const float32_t mp_candidate_range_rate
   *
   * Returns        bool - true if hypothesis is confirmed.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   bool Multipath_Detector::Verify_Range_Rate_Hypothesis(
      const Object_Reflector& reflector,
      const Point &reflection_point,
      const float32_t searching_range,
      const float32_t mp_candidate_range_rate
   )
   {
      const Range_Rate_Interval_T rr_scope = reflector.Compute_Range_Rate_Interval(reflection_point, searching_range);
      const float32_t tolerance_rr = 0.3F;

      bool f_passed_verification;
      if ((mp_candidate_range_rate > (rr_scope.rr_min - tolerance_rr)) && (mp_candidate_range_rate < (rr_scope.rr_max + tolerance_rr)))
      {
         f_passed_verification = true;
      }
      else
      {
         f_passed_verification = false;
      }

      return f_passed_verification;
   }

   /*=========================================================================
   * Method         Verify_Distance_Hypothesis
   *
   * Description    Verifes range hypothesis of true mutipath model.
   *
   * Parameters
   *  Object_Reflector &source_reflector
   *  const Point &reflection_point
   *  const float32_t remaining_dist
   *
   * Returns        bool - true if hypothesis is confirmed.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   bool Multipath_Detector::Verify_Distance_Hypothesis(
      const Object_Reflector&source_reflector,
      const Point &reflection_point,
      const float32_t remaining_dist
   )
   {
      bool f_passed_verification;

      if (source_reflector.Is_On_Radius(reflection_point, remaining_dist))
      {
         f_passed_verification = true;
      }
      else
      {
         f_passed_verification = false;
      }

      return f_passed_verification;
   }

   /*=========================================================================
   * Method         Distances_ok_for_Multipath
   *
   * Description    Verifies either the source or the reflector is far enough from the host.
   *                Also checks the ratio between the distance from radar to reflection point and
   *                the distance from reflection point to source is acceptable for multipath.
   *                If either of the two conditions is satisfied, multipath hypothesis will not
   *                be rejected at this step.
   *
   * Parameters
   *  const Point &radar_pos - radar sensor position
   *  const Point &reflection_point - estimated reflection point on reflector
   *  const Point &source_obj_pos - true source object position
   *
   * Returns        bool - true if either the objects are far enough from the host, or the
   *                distance ratio is acceptable for multipath.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  If either reflector or object is far enough, multipath hypothesis can still be valid.
   *
   * Note           Uses squared distances to avoid square root computation.
   *                Equivalent to checking: distance(radar, reflection) >= distance(reflection, source).
   *========================================================================*/
   bool Distances_ok_for_Multipath(
      const Point &radar_pos,
      const Point &reflection_point,
      const Point &source_obj_pos
   )
   {
      // Calculate squared distance from radar to reflection point
      const float32_t dx_radar_refl = radar_pos.x - reflection_point.x;
      const float32_t dy_radar_refl = radar_pos.y - reflection_point.y;
      const float32_t radar_to_refl_dist_sq = dx_radar_refl * dx_radar_refl + dy_radar_refl * dy_radar_refl;

      // Calculate squared distance from reflection point to source object
      const float32_t dx_refl_source = source_obj_pos.x - reflection_point.x;
      const float32_t dy_refl_source = source_obj_pos.y - reflection_point.y;
      const float32_t refl_to_source_dist_sq = dx_refl_source * dx_refl_source + dy_refl_source * dy_refl_source;

      // Check if length of path of the radar wave reflection is long enough for typical multipath. Use sum of squares as proxy for path length to avoid square root computation.
      // If the reflection path is longer than 50m, we would still consider multipath possible even if the ratio is significant.
      // If the reflection path is NOT longer than 50m, then we would proceed to check if the source-to-reflector path is larger than reflector-to-radar path.
      // If any of the 2 statements is true, we would assume multi-path is possible and return true.
      return (radar_to_refl_dist_sq + refl_to_source_dist_sq > 2500.0F) || (radar_to_refl_dist_sq >= refl_to_source_dist_sq);
   }

   /*=========================================================================
   * Method         Reflection_Angle_ok_for_Multipath
   *
   * Description    Verifies reflection angle hypothesis based on specular
   *                reflection geometry. Checks if the angle between incident
   *                and reflected rays is below a maximum threshold.
   *
   * Parameters
   *  const Point &radar_pos - radar sensor position
   *  const Point &reflection_point - estimated reflection point on reflector
   *  const Point &source_obj_pos - true source object position
   *  const float32_t max_angle - maximum acceptable angle in radians
   *
   * Returns        bool - true if reflection angle is physically plausible.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           This check only enforces a maximum allowed angle to
   *                reject near-retroreflection cases. Assumes radar at origin.
   *========================================================================*/
   bool Reflection_Angle_ok_for_Multipath(
      const Point &radar_pos,
      const Point &reflection_point,
      const Point &source_obj_pos,
      const float32_t max_angle
   )
   {
      // Calculate azimuth angle of incident ray (reflection point to radar)
      const float32_t incident_ray_x = radar_pos.x - reflection_point.x;
      const float32_t incident_ray_y = radar_pos.y - reflection_point.y;
      const float32_t incident_azimuth = F360_Atan2f(incident_ray_y, incident_ray_x);

      // Calculate azimuth angle of reflected ray (reflection point to source object)
      const float32_t reflected_ray_x = source_obj_pos.x - reflection_point.x;
      const float32_t reflected_ray_y = source_obj_pos.y - reflection_point.y;
      const float32_t reflected_azimuth = F360_Atan2f(reflected_ray_y, reflected_ray_x);

      // Calculate absolute angle difference between the two rays
      float32_t angle_diff = std::abs(reflected_azimuth - incident_azimuth);

      // Normalize to [0, pi] range (since we want the smaller angle between rays)
      if (angle_diff > F360_PI)
      {
         angle_diff = F360_2PI - angle_diff;
      }

      // Check if angle_diff is within acceptable range
      // Maximum angle (near-retroreflection threshold): max_angle (e.g., 150 degrees in radians)
      // If true, we would assume multi-path is possible.
      return (angle_diff < max_angle);
   }

   /*===========================================================================*\
    * FUNCTION: Calc_Searching_Range()
    *===========================================================================
    * RETURN VALUE:
    * float32_t - remaining distance.
    *
    * PARAMETERS:
    *  const Point &radar_pos,
    *  const Point &reflector_pos,
    *  const Point &mp_candidate_pos
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
    * Calculate remaining distance from reflector to multipath candidate.
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
 \*===========================================================================*/
   static float32_t Calc_Searching_Range(
      const Point &radar_pos,
      const Point &reflector_pos,
      const Point &mp_candidate_pos
   )
   {
      const float32_t mp_candidate_to_radar_dist = F360_Get_Hypotenuse(radar_pos.y - mp_candidate_pos.y, radar_pos.x - mp_candidate_pos.x);
      const float32_t radar_to_refl_dist = F360_Get_Hypotenuse(radar_pos.y - reflector_pos.y, radar_pos.x - reflector_pos.x);
      return mp_candidate_to_radar_dist - radar_to_refl_dist;
   }

}


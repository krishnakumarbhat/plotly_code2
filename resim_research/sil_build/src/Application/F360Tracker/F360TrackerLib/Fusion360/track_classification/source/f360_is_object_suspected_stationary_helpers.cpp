/*===================================================================================*\
* FILE:  f360_check_if_object_is_suspected_stationary_helpers.cpp
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential - Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains implementation helpers functions used in Is_Object_Suspected_Stationary() function.
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#include "f360_is_object_suspected_stationary_helpers.h"
#include "f360_determine_cross_moving_obj.h"
#include "f360_math_func.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_trk_fltr_ctca_states.h"
#include "f360_statistical_tests.h"
#include "f360_vector.h"
#include "f360_bounding_box.h"
#include "f360_occlusion.h"
#include <algorithm>

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Is_Object_Stationary_By_Num_Dets
   *===========================================================================
   * RETURN VALUE:
   * bool f_stationary_suspected - flag indicating whether object is suspected of being stationary
   *
   * PARAMETERS:
   * F360_Object_Track_T& object - reference to analysed object
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * const float32_t min_moving_dets_percentage
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
   * Function checks whether number of moving detections associated to object indicates that object is stationary.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Is_Object_Stationary_By_Num_Dets(
      const F360_Object_Track_T& object,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const float32_t min_moving_dets_percentage)
   {
      bool f_stationary_suspected;
      if (0U < object.ndets)
      {
         int32_t cnt_moving_assoc_dets = 0;
         for (uint32_t idx = 0U; idx < object.ndets; idx++)
         {
            const uint32_t det_idx = object.detids[idx] - 1U;
            if (det_props[det_idx].motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING)
            {
               cnt_moving_assoc_dets++;
            }
         }

         const int32_t min_moving_num = static_cast<int32_t>(F360_Ceilf(static_cast<float32_t>(object.ndets) * min_moving_dets_percentage));
         if (cnt_moving_assoc_dets < min_moving_num)
         {
            f_stationary_suspected = true;
         }
         else
         {
            f_stationary_suspected = false;
         }
      }
      else
      {
         f_stationary_suspected = true;
      }
      return f_stationary_suspected;
   }

   /*===========================================================================*\
   * FUNCTION: Get_Object_Vel_Cov
   *===========================================================================
   * RETURN VALUE:
   * float32_t(&vel_cov)[2][2] - object velocity covariance matrix
   *
   * PARAMETERS:
   * float32_t(&vel_cov)[2][2] - object velocity covariance matrix, filled with data
   * const float32_t(&statecov)[STATE_DIMENSION][STATE_DIMENSION]) - object state covariance matrix
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
   * Function extracts object velocity covariance matrix from state covariance matrix.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Get_Object_Vel_Cov(
      float32_t(&vel_cov)[2][2],
      const float32_t(&statecov)[STATE_DIMENSION][STATE_DIMENSION])
   {
      vel_cov[0][0] = statecov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX];
      vel_cov[0][1] = statecov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY];
      vel_cov[1][0] = statecov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX];
      vel_cov[1][1] = statecov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY];
   }

   /*===========================================================================*\
   * FUNCTION: Calc_NEES_Value_For_Vel_Diff
   *===========================================================================
   * RETURN VALUE:
   * float32_t nees_value - calculated NEES cost function value.
   *
   * PARAMETERS:
   * const F360_VCS_Velocity_T& vcs_vel - object vcs velocity
   * const float32_t(&vel_cov)[2][2]) - object velocity covariance matrix
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
   * Function calculates NEES cost function value for velocity difference from assumed ref.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   float32_t Calc_NEES_Value_For_Vel_Diff(
      const F360_VCS_Velocity_T& vcs_vel,
      const float32_t(&vel_cov)[2][2])
   {
      float32_t nees_value;
      const float32_t determinant = F360_2d_Matrix_Determinant(vel_cov);

      if (F360_EPSILON < determinant)
      {
         const float32_t ref_vel_lat = 0.0F;
         const float32_t ref_vel_lon = 0.0F;

         F360_VCS_Velocity_T vel_diff;
         vel_diff.longitudinal = ref_vel_lon - vcs_vel.longitudinal;
         vel_diff.lateral = ref_vel_lat - vcs_vel.lateral;

         nees_value = ((vel_diff.longitudinal * vel_diff.longitudinal * vel_cov[0][0]) -
            (2.0F * vel_diff.longitudinal * vel_diff.lateral * vel_cov[1][0]) +
            (vel_diff.lateral * vel_diff.lateral * vel_cov[1][1])) / determinant;
      }
      else
      {
         const float32_t max_nees_value = 1.0F / F360_EPSILON;
         nees_value = max_nees_value;
      }
      return nees_value;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Object_Stationary_By_Vel_NEES
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * None.
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
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Is_Object_Stationary_By_Vel_NEES(
      F360_Object_Track_T& object,
      const F360_Calibrations_T& calib)
   {
      float32_t vel_cov[2][2]{};
      Get_Object_Vel_Cov(vel_cov, object.errcov);
      const float32_t nees_val = Calc_NEES_Value_For_Vel_Diff(object.vcs_velocity, vel_cov);
      const float32_t p_value = F360_Chi_Square_Test_Two_Degree_P_Value(nees_val, calib.k_object_motion_nees_max_test_variable);
      const float32_t p_value_th = Calc_P_Value_Threshold(object.vcs_position, calib.k_object_motion_nees_min_p_value_th, calib.k_object_motion_nees_range_scaling_factor);
      const bool f_stationary_suspected = p_value_th < p_value;

      return f_stationary_suspected;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Object_Stationary_By_Vel_Sigma
   *===========================================================================
   * RETURN VALUE:
   * bool f_stationary_suspected - flag indicating whether object is suspected of being stationary
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object - reference to analyzed object
   * const F360_Calibrations_T& calib  - reference to calibrations
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
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Is_Object_Stationary_By_Vel_Sigma(
      const F360_Object_Track_T& object,
      const F360_Calibrations_T& calib)
   {
      const float32_t abs_speed = std::abs(object.speed);
      const float32_t speed_std = F360_Sqrtf(std::abs(object.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S]));

      const bool f_stationary_suspected = Check_Stationary_Hypothesis(abs_speed,
         speed_std,
         calib.k_object_motion_min_speed,
         calib.k_object_motion_sigma_ctca_th);

      return f_stationary_suspected;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Object_Parallel_Moving
   *===========================================================================
   * RETURN VALUE:
   * bool f_parallel_moving - flag indicating whether object is moving parallel to host
   *
   * PARAMETERS:
   * const F360_Object_Track_T& obj - reference to analysed object
   * const F360_Host_T& host - reference to host properties
   * const F360_Calibrations_T& calib - reference to calibrations
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
   * Function verifies whether object is moving parallel to host at aproximately same speed.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Is_Object_Parallel_Moving(
      const F360_Object_Track_T& obj,
      const F360_Host_T& host,
      const F360_Calibrations_T& calib)
   {
       const bool f_heading_ok = (std::abs(obj.vcs_heading.Value()) < calib.k_object_motion_parallel_moving_heading_th);
       const bool f_motion_speed_ok = (calib.k_object_motion_min_speed < host.speed);
       const bool f_speed_diff_ok = (std::abs(obj.speed - host.speed) < calib.k_object_motion_parallel_moving_speed_diff_th);
       const bool f_pos_y_ok = (std::abs(obj.vcs_position.y) < calib.k_object_motion_parallel_moving_lat_posn_th);
       const bool f_pos_x_ok = (std::abs(obj.vcs_position.x) < calib.k_object_motion_parallel_moving_lon_posn_th);

      const bool f_parallel_moving = 
         f_heading_ok &&
         f_motion_speed_ok &&
         f_speed_diff_ok &&
         f_pos_y_ok &&
         f_pos_x_ok;

      return f_parallel_moving;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Object_Cross_Radial_Moving
   *===========================================================================
   * RETURN VALUE:
   * bool f_cross_radial_moving - flag indicating whether object is moving cross moving to host
   *
   * PARAMETERS:
   * const F360_Object_Track_T& obj - reference to analysed object
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
   * Function verifies object cross radial movement around host.
   * It checks if the angle between the object velocity vector and the radial direction is large enough
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Is_Object_Cross_Radial_Moving(
       const F360_Object_Track_T& object)
   {
       const float32_t k_tan_min_vel_ang_diff_to_radial_direction = 2.0F;
       const float32_t lower_speed_th_to_consider_as_cross_radial = 2.0F;

       const float32_t obj_vcs_azimuth = F360_Atan2f(object.vcs_position.y, object.vcs_position.x);
       const float32_t obj_azimuth_cosf = F360_Cosf(obj_vcs_azimuth);
       const float32_t obj_azimuth_sinf = F360_Sinf(obj_vcs_azimuth);

       // Object Radial Velocity 
       const float32_t obj_radial_vel = (object.vcs_velocity.longitudinal * obj_azimuth_cosf) + (object.vcs_velocity.lateral * obj_azimuth_sinf);
       // Object Tangential Velocity
       const float32_t obj_tangential_vel = (-object.vcs_velocity.longitudinal * obj_azimuth_sinf) + (object.vcs_velocity.lateral * obj_azimuth_cosf);

       const bool speed_lower_than_threshold = (object.speed > lower_speed_th_to_consider_as_cross_radial);
       const bool tangential_vel_greater_than_radial_vel = ((k_tan_min_vel_ang_diff_to_radial_direction * std::abs(obj_radial_vel)) < std::abs(obj_tangential_vel));
       
       const bool f_cross_radial_moving = ((speed_lower_than_threshold) && (tangential_vel_greater_than_radial_vel));

       return f_cross_radial_moving;
   }

   /*===========================================================================*\
   * FUNCTION: Calc_P_Value_Threshold
   *===========================================================================
   * RETURN VALUE:
   * float32_t p_value_th - threshold used to reject stationary hypothesis
   *
   * PARAMETERS:
   * const Point& obj_vcs_pos - object vcs position
   * const float32_t base_min_p_value - base p_value threshold
   * const float32_t range_scaling_factor - scaling factor used to change threshold
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
   * Function calculates p_value threhsold to classify object as moving. It changes
   * depending on object vcs position.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   float32_t Calc_P_Value_Threshold(
      const Point& obj_vcs_pos,
      const float32_t base_min_p_value,
      const float32_t range_scaling_factor)
   {
      const float32_t range = F360_Get_Hypotenuse(obj_vcs_pos.y, obj_vcs_pos.x);

      const float32_t range_factor = 1.0F + range_scaling_factor * range * range * range;

      const float32_t p_value_th = base_min_p_value / range_factor;

      return p_value_th;

   }

   /*===========================================================================*\
   * FUNCTION: Calc_Moving_Speed_Threshold
   *===========================================================================
   * RETURN VALUE:
   * float32_t moving_speed_threshold - speed threshold for object to be considered moving
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list - Raw detection list
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS] - sensors array 
   * const F360_Calibrations_T& calib - calibration parameters
   * F360_Object_Track_T& object - Object to calculate moving speed threshold for
   * F360_Occlusion_Data_T& occlusion_data - occlusion data
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function calculates moving speed threshold for objects in occlusion zone close
   * to host.
   *
   \*===========================================================================*/
   float32_t Calc_Moving_Speed_Threshold(
      const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calib,
      const F360_Object_Track_T& object,
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS])
   {
      float32_t moving_speed_threshold = calib.k_object_motion_min_speed;
      if ((std::abs(object.speed) > moving_speed_threshold)
            && (object.movable_prob < 0.5F)
            && (F360_TRACKER_TRKFLTR_CCA == object.trk_fltr_type))
      {
         const Point rear_left_corner_occlusion_zone = Point(calib.k_occlusion_zone_long_left_rear_stationary_check, calib.k_occlusion_zone_lat_left_rear_stationary_check);
         const Point front_right_corner_occlusion_zone = Point(calib.k_occlusion_zone_long_right_front_stationary_check, calib.k_occlusion_zone_lat_right_front_stationary_check);
         const BoundingBox occlusion_zone(rear_left_corner_occlusion_zone, front_right_corner_occlusion_zone);

         if (occlusion_zone.Contains(object.bbox.Get_Center()))
         {
            const F360_Occlusion_Status_T occlusion_status = Get_Point_Occlusion_Status(sensors, occlusion_data, object.bbox.Get_Center().x, object.bbox.Get_Center().y);
            
            if (OCCLUSION_STATUS_OCCLUDED == occlusion_status)
            {
               const float32_t cross_moving_speed_thr_inc = calib.k_object_motion_occluded_speed_scale_factor
                                                             * Calc_Cross_Moving_Weight(raw_detect_list, sensors, calib, object);
               moving_speed_threshold += cross_moving_speed_thr_inc;
            }
         }
      }
      return moving_speed_threshold;
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Cross_Moving_Weight
   *===========================================================================
   * RETURN VALUE:
   * float32_t weight_cross_moving - Weight between 0 and 1 for how much the object 
   * is moving orthogonally to the sensors that see the object.
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list - Raw detection list
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS] - sensors array
   * const F360_Calibrations_T& calib - calibration parameters
   * F360_Object_Track_T& object - Object to calculate cross moving weight for
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function calculates weight_cross_moving which is used for determine if object
   * is stationary. The weight depends how much the object is moving cross radial to
   * the sensors.
   *
   \*===========================================================================*/
   float32_t Calc_Cross_Moving_Weight(
      const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calib,
      const F360_Object_Track_T& object)
   {
      float32_t weight_cross_moving = 0.0F;
      if ((object.ndets > 0U) && Is_Object_Cross_Moving(object, calib))
      {
         uint32_t num_dets_per_sensor[MAX_NUMBER_OF_SENSORS]{};

         for (uint32_t idx = 0U; idx < object.ndets; idx++)
         {
            const uint32_t det_idx = object.detids[idx] - 1U;
            const int32_t sensor_idx = raw_detect_list.detections[det_idx].raw.sensor_id - 1;
            num_dets_per_sensor[sensor_idx]++;
         }

         for (uint8_t sensor_index = 0U; sensor_index < MAX_NUMBER_OF_SENSORS; sensor_index++) {
            if (num_dets_per_sensor[sensor_index] > 0U)
            {
               const Vector_T object_vcs_velocity_vector = Vector_T(object.vcs_velocity.longitudinal, object.vcs_velocity.lateral);
               const Point mount_location_point = Point(sensors[sensor_index].constant.mounting_position.vcs_position.longitudinal, sensors[sensor_index].constant.mounting_position.vcs_position.lateral);
               const Vector_T vector_from_sensor_2_obj = Vector_T(mount_location_point, object.vcs_position);
               const float32_t tmp_weight_cross_moving = 1.0F - std::abs(object_vcs_velocity_vector.Cos_Angle_Between(vector_from_sensor_2_obj));
               weight_cross_moving = std::max(weight_cross_moving, tmp_weight_cross_moving);
            }
         }
         
      }
      return weight_cross_moving;
   }
}

/** \file
* This file contains unit tests for content of f360_is_object_suspected_stationary_helpers.cpp file
*/

#include "f360_is_object_suspected_stationary_helpers.h"
#include "f360_trk_fltr_ctca_states.h"
#include <CppUTest/TestHarness.h>
#include "f360_math_func.h"
#include "f360_trk_fltr_ccv_states.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_occlusion.h"

using namespace f360_variant_A;

/** \defgroup  is_object_stationary_by_num_dets
*  @{
*/

/** \brief
* Test group of Is_Object_Stationary_By_Num_Dets() function. Tests verify whether
* function properly counts moving detections and decides whether object is suspected
* of being stationary.
*/
TEST_GROUP(is_object_stationary_by_num_dets)
{
   F360_Object_Track_T object;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Calibrations_T calib;

   /** \setup
   * Initialize tracker calibrations.
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
* Purpose of this test is to verify whether when object has no new dets it is marked as stationary suspected.
* \req
* NA.
*/
TEST(is_object_stationary_by_num_dets, Is_Object_Stationary_By_Num_Dets__no_new_dets)
{
   /** \precond
   * Set ndets to 0
   */
   object.ndets = 0;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Stationary_By_Num_Dets(object, det_props, calib.k_object_motion_min_moving_dets_percentage_th);

   /** \result
   * Check if result is true
   */
   CHECK_TRUE(result);
}

/** \purpose
* Purpose of this test is to verify whether when object has moving dets but their share is
* less than threshold function returns true
* \req
* NA.
*/
TEST(is_object_stationary_by_num_dets, Is_Object_Stationary_By_Num_Dets__moving_dets_percentage_below_threshold)
{
   /** \precond
   * Set ndets to 4
   * Set number of moving dets to be below threshold from calibrations
   */
   object.ndets = 4;
   const float32_t det_percentage_threshold = 0.5F;
   det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   det_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   det_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   object.detids[0] = 1;
   object.detids[1] = 2;
   object.detids[2] = 3;
   object.detids[3] = 4;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Stationary_By_Num_Dets(object, det_props, det_percentage_threshold);

   /** \result
   * Check if result is true
   */
   CHECK_TRUE(result);
}

/** \purpose
* Purpose of this test is to verify whether when object has moving dets and their percentage is
* above threshold function returns false
* \req
* NA.
*/
TEST(is_object_stationary_by_num_dets, Is_Object_Stationary_By_Num_Dets__moving_dets_percentage_above_threshold)
{
   /** \precond
   * Set ndets to 4
   * Set number of moving dets to be below threshold from calibrations
   */
   object.ndets = 4;
   const float32_t det_percentage_threshold = 0.5F;
   det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   det_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   det_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   object.detids[0] = 1;
   object.detids[1] = 2;
   object.detids[2] = 3;
   object.detids[3] = 4;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Stationary_By_Num_Dets(object, det_props, det_percentage_threshold);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}
/** @}*/


/** \defgroup  is_object_stationary_by_vel_sigma
*  @{
*/

/** \brief
* Test group of Is_Object_Stationary_By_Vel_Sigma() function. Tests verify whether
* function properly distinguishes whether CTCA objects are moving by their
* speed/speed_std ratio.
*/
TEST_GROUP(is_object_stationary_by_vel_sigma)
{
   F360_Object_Track_T object;
   F360_Calibrations_T calib;

   /** \setup
   * Initialize tracker calibrations.
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
* Purpose of this test is to verify whether when sigma ratio and speed are above threshold, object is not marked as stationary suspected.
* \req
* NA.
*/
TEST(is_object_stationary_by_vel_sigma, Is_Object_Stationary_By_Vel_Sigma__CTCA_sigma_above_threshold_speed_above_threshold)
{
   /** \precond
   * Set object speed to value above threshold
   * set object speed_std to meet:
   * speed/speed_std > sigma_th
   */
   object.speed = calib.k_object_motion_min_speed + 0.1F;
   float32_t speed_std = object.speed / (calib.k_object_motion_sigma_ctca_th + 1);
   object.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = speed_std * speed_std;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Stationary_By_Vel_Sigma(object, calib);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}

/** \purpose
* Purpose of this test is to verify whether when sigma ratio is above threshold but object speed is below threshold
* object is marked as stationary suspected.
* \req
* NA.
*/
TEST(is_object_stationary_by_vel_sigma, Is_Object_Stationary_By_Vel_Sigma__CTCA_sigma_above_threshold_speed_below_threshold)
{
   /** \precond
   * Set object speed to value below threshold
   * set object speed_std to meet:
   * speed/speed_std > sigma_th
   */
   object.speed = calib.k_object_motion_min_speed - 0.1F;
   float32_t speed_std = object.speed / (calib.k_object_motion_sigma_ctca_th + 1);
   object.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = speed_std * speed_std;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Stationary_By_Vel_Sigma(object, calib);

   /** \result
   * Check if result is true
   */
   CHECK_TRUE(result);
}

/** \purpose
* Purpose of this test is to verify whether sigma ratio is below threshold and object speed is below threshold
* object is marked as stationary suspected.
* \req
* NA.
*/
TEST(is_object_stationary_by_vel_sigma, Is_Object_Stationary_By_Vel_Sigma__CTCA_sigma_below_threshold_speed_below_threshold)
{
   /** \precond
   * Set object speed to value below threshold
   * set object speed_std to meet:
   * speed/speed_std < sigma_th
   */
   object.speed = calib.k_object_motion_min_speed - 0.1F;
   float32_t speed_std = object.speed / (calib.k_object_motion_sigma_ctca_th - 1);
   object.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = speed_std * speed_std;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Stationary_By_Vel_Sigma(object, calib);

   /** \result
   * Check if result is true
   */
   CHECK_TRUE(result);
}

/** \purpose
* Purpose of this test is to verify whether when sigma ratio is below threshold and object speed is above threshold
* object is marked as stationary suspected.
* \req
* NA.
*/
TEST(is_object_stationary_by_vel_sigma, Is_Object_Stationary_By_Vel_Sigma__CTCA_sigma_below_threshold_speed_above_threshold)
{
   /** \precond
   * Set object speed to value above threshold
   * set object speed_std to meet:
   * speed/speed_std < sigma_th
   */
   object.speed = calib.k_object_motion_min_speed + 0.1F;
   float32_t speed_std = object.speed / (calib.k_object_motion_sigma_ctca_th - 1);
   object.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = speed_std * speed_std;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Stationary_By_Vel_Sigma(object, calib);

   /** \result
   * Check if result is true
   */
   CHECK_TRUE(result);
}
/** @}*/


/** \defgroup  is_object_parallel_moving
*  @{
*/

/** \brief
* Test group of Is_Object_Parallel_Moving() function. Tests verify whether
* function properly marks objects that are moving in same direction as host, with
* similar speed and this speed is above threshold.
*/
TEST_GROUP(is_object_parallel_moving)
{
   F360_Object_Track_T object;
   F360_Host_T host;
   F360_Calibrations_T calib;

   /** \setup
   * Initialize tracker calibrations.
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
* Purpose of this test is to verify whether function returns true when all conditions are met
* \req
* NA.
*/
TEST(is_object_parallel_moving, Is_Object_Parallel_Moving__all_conditions_met)
{
   /** \precond
   * Set object parameters to meet all conditions
   * Set host speed to value above threshold
   */
   object.vcs_position.y = calib.k_object_motion_parallel_moving_lat_posn_th - 0.1F;
   object.vcs_position.x = calib.k_object_motion_parallel_moving_lon_posn_th - 0.1F;
   object.vcs_heading = Angle{ calib.k_object_motion_parallel_moving_heading_th - 0.1F };
   object.speed = 2.0F * calib.k_object_motion_min_speed;
   host.speed = 2.0F * calib.k_object_motion_min_speed;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Parallel_Moving(object, host, calib);

   /** \result
   * Check if result is true
   */
   CHECK_TRUE(result);
}

/** \purpose
* Purpose of this test is to verify whether function returns false when object heading is above threshold
* \req
* NA.
*/
TEST(is_object_parallel_moving, Is_Object_Parallel_Moving__heading_above_threshold)
{
   /** \precond
   * Set object parameters to meet all conditions
   * Set object heading to value above threshold
   * Set host speed to value above threshold
   */
   object.vcs_position.y = calib.k_object_motion_parallel_moving_lat_posn_th - 0.1F;
   object.vcs_position.x = calib.k_object_motion_parallel_moving_lon_posn_th - 0.1F;
   object.vcs_heading = Angle{ calib.k_object_motion_parallel_moving_heading_th + 0.1F };
   object.speed = 2.0F * calib.k_object_motion_min_speed;
   host.speed = 2.0F * calib.k_object_motion_min_speed;


   /** \action
   * Call tested function
   */
   bool result = Is_Object_Parallel_Moving(object, host, calib);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}

/** \purpose
* Purpose of this test is to verify whether function returns false when object speed is below threshold
* \req
* NA.
*/
TEST(is_object_parallel_moving, Is_Object_Parallel_Moving__speed_below_threshold)
{
   /** \precond
   * Set object parameters to meet all conditions
   * Set object speed to value below threshold
   * Set host speed to value above threshold
   */
   object.vcs_position.y = calib.k_object_motion_parallel_moving_lat_posn_th - 0.1F;
   object.vcs_position.x = calib.k_object_motion_parallel_moving_lon_posn_th - 0.1F;
   object.vcs_heading = Angle{ calib.k_object_motion_parallel_moving_heading_th - 0.1F };
   object.speed = 0.5F * calib.k_object_motion_min_speed;
   host.speed = 2.0F * calib.k_object_motion_min_speed;


   /** \action
   * Call tested function
   */
   bool result = Is_Object_Parallel_Moving(object, host, calib);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}

/** \purpose
* Purpose of this test is to verify whether function returns false when object and host speed difference is above threshold
* \req
* NA.
*/
TEST(is_object_parallel_moving, Is_Object_Parallel_Moving__speed_difference_above_threshold)
{
   /** \precond
   * Set object parameters to meet all conditions
   * Set object speed to value above threshold
   * Set host speed to be higher than object_sped + max_speed_diff
   */
   object.vcs_position.y = calib.k_object_motion_parallel_moving_lat_posn_th - 0.1F;
   object.vcs_position.x = calib.k_object_motion_parallel_moving_lon_posn_th - 0.1F;
   object.vcs_heading = Angle{ calib.k_object_motion_parallel_moving_heading_th - 0.1F };
   object.speed = 2.0F * calib.k_object_motion_min_speed;
   host.speed = object.speed + calib.k_object_motion_parallel_moving_speed_diff_th + 1.0F;


   /** \action
   * Call tested function
   */
   bool result = Is_Object_Parallel_Moving(object, host, calib);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}

/** \purpose
* Purpose of this test is to verify whether function returns false when object lateral position is beyond area of interest
* \req
* NA.
*/
TEST(is_object_parallel_moving, Is_Object_Parallel_Moving__object_lateral_position_above_threshold)
{
   /** \precond
   * Set object parameters to meet all conditions
   * Set object lateral position to value above threshold
   */
   object.vcs_position.y = calib.k_object_motion_parallel_moving_lat_posn_th + 0.1F;
   object.vcs_position.x = calib.k_object_motion_parallel_moving_lon_posn_th - 0.1F;
   object.vcs_heading = Angle{ calib.k_object_motion_parallel_moving_heading_th - 0.01F };
   object.speed = 2.0F * calib.k_object_motion_min_speed;
   host.speed = object.speed;


   /** \action
   * Call tested function
   */
   bool result = Is_Object_Parallel_Moving(object, host, calib);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}

/** \purpose
* Purpose of this test is to verify whether function returns false when object longitudinal position is beyond area of interest
* \req
* NA.
*/
TEST(is_object_parallel_moving, Is_Object_Parallel_Moving__object_longitudinal_position_above_threshold)
{
   /** \precond
   * Set object parameters to meet all conditions
   * Set object longitudinal position to value above threshold
   */
   object.vcs_position.y = calib.k_object_motion_parallel_moving_lat_posn_th - 0.1F;
   object.vcs_position.x = calib.k_object_motion_parallel_moving_lon_posn_th + 0.1F;
   object.vcs_heading = Angle{ calib.k_object_motion_parallel_moving_heading_th - 0.01F };
   object.speed = 2.0F * calib.k_object_motion_min_speed;
   host.speed = object.speed;


   /** \action
   * Call tested function
   */
   bool result = Is_Object_Parallel_Moving(object, host, calib);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}

/** \purpose
* Purpose of this test is to verify whether function returns false when object meets conditios but host is moving too slow
* \req
* NA.
*/
TEST(is_object_parallel_moving, Is_Object_Parallel_Moving__host_too_slow)
{
   /** \precond
   * Set object parameters to meet all conditions
   * Set object speed to value above threshold
   * Set host speed to be higher than object_sped + max_speed_diff
   */
   object.vcs_position.y = calib.k_object_motion_parallel_moving_lat_posn_th - 0.1F;
   object.vcs_position.x = calib.k_object_motion_parallel_moving_lon_posn_th - 0.1F;
   object.vcs_heading = Angle{ calib.k_object_motion_parallel_moving_heading_th - 0.1F };
   object.speed = 2.0F * calib.k_object_motion_min_speed;
   host.speed = calib.k_object_motion_min_speed - 0.01F;


   /** \action
   * Call tested function
   */
   bool result = Is_Object_Parallel_Moving(object, host, calib);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}
/** @}*/


/** \defgroup  is_object_stationary_by_vel_nees
*  @{
*/

/** \brief
* Test group of Is_Object_Stationary_By_Vel_NEES() function. Tests verify whether
* function properly marks CCA objects motion status.
*/
TEST_GROUP(is_object_stationary_by_vel_nees)
{
   F360_Object_Track_T object;
   F360_Calibrations_T calib;

   /** \setup
   * Initialize tracker calibrations.
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
* Purpose of this test is to verify whether object speed covariance is very small and object lateral
* and longitudinal velocities are high - function returns false.
* \req
* NA.
*/
TEST(is_object_stationary_by_vel_nees, Is_Object_Stationary_By_Vel_NEES__small_cov_high_velocities)
{
   /** \precond
   * Set object velocity covariance to zeros
   * Set opbject lateral and longitudinal velocity to values above zero
   */
   object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = 0.0F;
   object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = 0.0F;
   object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = 0.0F;
   object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = 0.0F;

   object.vcs_velocity.lateral = 10.0F;
   object.vcs_velocity.longitudinal = 10.0F;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Stationary_By_Vel_NEES(object, calib);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}

/** \purpose
* Purpose of this test is to verify whether object speed covariance is high and object lateral
* and longitudinal velocities are high - function returns true.
* \req
* NA.
*/
TEST(is_object_stationary_by_vel_nees, Is_Object_Stationary_By_Vel_NEES__high_cov_high_velocities)
{
   /** \precond
   * Set object velocity covariance to zeros
   * Set opbject lateral and longitudinal velocity to values above zero
   */
   object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = 100.0F;
   object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = 0.0F;
   object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = 0.0F;
   object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = 100.0F;

   object.vcs_velocity.lateral = 10.0F;
   object.vcs_velocity.longitudinal = 10.0F;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Stationary_By_Vel_NEES(object, calib);

   /** \result
   * Check if result is true
   */
   CHECK_TRUE(result);
}
/** @}*/


/** \defgroup  calc_p_value_threshold
*  @{
*/

/** \brief
* Test group of calc_p_value_threshold() function. Tests verify
* whether threshold is properly lowered.
*/
TEST_GROUP(calc_p_value_threshold)
{
   Point obj1_vcs_pos;
   Point obj2_vcs_pos;
   float32_t base_min_p_vale;
   float32_t range_scaling_factor;

   /** \setup
   * Initialize tracker calibrations.
   */
   TEST_SETUP()
   {
      base_min_p_vale = 0.2F;
      range_scaling_factor = 0.1F;

      obj1_vcs_pos = {};
      obj2_vcs_pos = {};
   }
};

/** \purpose
* Purpose of this test is to verify whether threshold for closer objects is greater than for further objects.
* \req
* NA.
*/
TEST(calc_p_value_threshold, calc_p_value_threshold__Threshold_Is_Greater_For_Closer_Objects)
{
   /** \precond
   * Set first object vcs position to 5.0, 0.0
   * Set second object vcs position to 20.0, 0.0
   */
   obj1_vcs_pos.y = 5.0F;
   obj1_vcs_pos.x = 0.0F;

   obj2_vcs_pos.y = 20.0F;
   obj2_vcs_pos.x = 0.0F;

   /** \action
   * Call tested function on both positions
   */
   const float32_t res1 = Calc_P_Value_Threshold(obj1_vcs_pos, base_min_p_vale, range_scaling_factor);
   const float32_t res2 = Calc_P_Value_Threshold(obj2_vcs_pos, base_min_p_vale, range_scaling_factor);

   /** \result
   * Check whether res1 is greater than res2
   */
   CHECK_TRUE(res2 < res1);
}

/** \purpose
* Purpose of this test is to verify whether threshold is not changed for objects at range equal to zero
* \req
* NA.
*/
TEST(calc_p_value_threshold, calc_p_value_threshold__Threshold_Does_Not_Change_For_Objects_At_Zero)
{
   /** \precond
   * Set first object vcs position to 0.0, 0.0
   */
   obj1_vcs_pos.y = 0.0F;
   obj1_vcs_pos.x = 0.0F;

   /** \action
   * Call tested function on both positions
   */
   const float32_t res = Calc_P_Value_Threshold(obj1_vcs_pos, base_min_p_vale, range_scaling_factor);

   /** \result
   * Check whether threshold did not change
   */
   DOUBLES_EQUAL(base_min_p_vale, res, F360_EPSILON);
}
/** @}*/

/** \defgroup  is_object_cross_radial_moving
*  @{
*/

/** \brief
* Test group of Is_Object_Cross_Radial_Moving() function. Tests verify whether
* function properly marks objects motion status.
*/
TEST_GROUP(is_object_cross_radial_moving)
{
   F360_Object_Track_T object;
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Calibrations_T calib;

   /** \setup
   * Initialize tracker calibrations.
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      object.vcs_position.y = 10.0F;
      object.vcs_position.x = 10.0F;
      object.speed = 2.1F;
      object.vcs_velocity.lateral = 1.0F;
      object.vcs_velocity.longitudinal = 2.0F;

   }
};

/** \purpose
* Purpose of this test is to verify that objets with speed below 2m/s should not be flaged as cross radially moving.
   radial velocity is negative tangential velocity is positive with radial smaller than tangential
* \req
* NA.
*/
TEST(is_object_cross_radial_moving, Is_Object_Cross_Radial_Moving_Obj_speed_equal_threshold_radial_velocity_negative_tangential_velocity_positive_with_radial_smaller_than_tangential)
{
   /** \precond
    * radial velocity = -0.70
    * tangential velocity = 2.12
   */
  object.vcs_velocity.lateral = 1.0F;
  object.vcs_velocity.longitudinal = -2.0F;
  object.speed = 2.0F;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Cross_Radial_Moving(object);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}


/** \purpose
* Purpose of this test is to verify that object is flagged as moving if object speed is greater than 2m/s 
* and tangential velocity is negative and radial velocity is positive with radial smaller than tangential.
* \req
* NA.
*/
TEST(is_object_cross_radial_moving, Is_Object_Cross_Radial_Moving_Obj_speed_greater_than_threshold_both_tangential_and_radial_velocity_are_negative_radial_greater_than_tangential)
{
   /** \precond
    * radial velocity = 0.70
    * tangential velocity = -2.12
   */
   object.vcs_velocity.lateral = -1.0F;
   object.vcs_velocity.longitudinal = 2.0F;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Cross_Radial_Moving(object);

   /** \result
   * Check if result is true
   */
   CHECK_TRUE(result);
}

/** \purpose
* Purpose of this test is to verify that object is flagged as moving if object speed is greater than 2m/s 
* and radial velocity is negative tangential velocity positive with radial smaller than tangential.
* \req
* NA.
*/
TEST(is_object_cross_radial_moving, Is_Object_Cross_Radial_Moving_Obj_speed_greater_than_threshold_tangential_and_radial_is_negative_tangential_positive_radial_smaller_than_tangential)
{
   /** \precond
    * radial velocity = -0.70
    * tangential velocity = 2.12
   */
   object.vcs_velocity.lateral = 1.0F;
   object.vcs_velocity.longitudinal = -2.0F;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Cross_Radial_Moving(object);

   /** \result
   * Check if result is true
   */
   CHECK_TRUE(result);
}

/** \purpose
* Purpose of this test is to verify that object is flagged as moving if object speed is greater than 2m/s 
* and radial velocity is positive tangential velocity negative with radial smaller than tangential.
* \req
* NA.
*/
TEST(is_object_cross_radial_moving, Is_Object_Cross_Radial_Moving_Obj_speed_greater_than_threshold_and_radial_is_positive_tangential_negative_radial_smaller_than_tangential)
{
   /** \precond
    * radial velocity = 0.70
    * tangential velocity = -2.12
   */
   object.vcs_velocity.lateral = -1.0F;
   object.vcs_velocity.longitudinal = 2.0F;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Cross_Radial_Moving(object);

   /** \result
   * Check if result is true
   */
   CHECK_TRUE(result);
}

/** \purpose
* Purpose of this test is to verify that object is not flagged as moving if object speed is greater than 2m/s 
* and tangential velocity is negative and radial velocity is positive with radial greater than tangential.
* \req
* NA.
*/
TEST(is_object_cross_radial_moving, Is_Object_Cross_Radial_Moving_Obj_speed_greater_than_threshold_tangential_velocity_is_negative_and_radial_velocity_is_positive_with_radial_greater_than_tangential)
{
   /** \precond
    * radial velocity = 4.24
    * tangential velocity = -2.82
   */
   object.vcs_velocity.lateral = 1.0F;
   object.vcs_velocity.longitudinal = 5.0F;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Cross_Radial_Moving(object);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}

/** \purpose
* Purpose of this test is to verify that object is not flagged as moving if object speed is greater than 2m/s 
* and tangential velocity is positive radial velocity is negative with radial greater than tangential.
* \req
* NA.
*/
TEST(is_object_cross_radial_moving, Is_Object_Cross_Radial_Moving_Obj_speed_greater_than_threshold_tangential_velocity_is_positive_radial_velocity_is_negative_with_radial_greater_than_tangential)
{
   /** \precond
    * radial velocity = -4.24
    * tangential velocity = 2.82
   */
   object.vcs_velocity.lateral = -1.0F;
   object.vcs_velocity.longitudinal = -5.0F;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Cross_Radial_Moving(object);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}

/** \purpose
* Purpose of this test is to verify that object is flagged as moving if object speed is greater than 2m/s 
* and radial velocity is negative and tangential velocity is negative with radial smaller than tangential.
* \req
* NA.
*/
TEST(is_object_cross_radial_moving, Is_Object_Cross_Radial_Moving_Obj_speed_greater_than_threshold_radial_velocity_is_negative_and_tangential_velocity_is_negative_with_radial_smaller_than_tangential)
{
   /** \precond
    * radial velocity = -2.12
    * tangential velocity = -4.94
   */
   object.vcs_velocity.lateral = -5.0F;
   object.vcs_velocity.longitudinal = 2.0F;
   /** \action
   * Call tested function
   */
   bool result = Is_Object_Cross_Radial_Moving(object);

   /** \result
   * Check if result is true
   */
   CHECK_TRUE(result);
}

/** \purpose
* Purpose of this test is to verify that object is flagged as moving if object speed is greater than 2m/s 
* and radial velocity is negative tangential velocity is positive with radial smaller than tangential.
* \req
* NA.
*/
TEST(is_object_cross_radial_moving, Is_Object_Cross_Radial_Moving_Obj_speed_greater_than_threshold_and_radial_is_negative_tangential_positive_radial_smaller_than_tangential)
{
   /** \precond
    * radial velocity = -2.12
    * tangential velocity = 4.94
   */
   object.vcs_velocity.lateral = 2.0F;
   object.vcs_velocity.longitudinal = -5.0F;

   /** \action
   * Call tested function
   */
   bool result = Is_Object_Cross_Radial_Moving(object);

   /** \result
   * Check if result is true
   */
   CHECK_TRUE(result);
}

/** \defgroup  Calc_Cross_Moving_Weight
*  @{
*/
/** \brief
* Test group of Calc_Cross_Moving_Weight() function. Test verifies if the
* cross moving weight is calculated correct.
* 
*/
TEST_GROUP(Calc_Cross_Moving_Weight)
{
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] {};
   F360_Calibrations_T calib {};
   F360_Object_Track_T object {};
   const float32_t test_pass_threshold = 1e-4F;

   /** \setup
   * Initialize tracker calibrations.
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
* Check that the cross moving weight is 0 when object vcs velocity vector is too
* parallel moving to host direction 
* \req
* NA.
*/
TEST(Calc_Cross_Moving_Weight, check_non_moving_stationary_object)
{
   /** \precond
   * Set object velocity vector such that it points < +/-10 deg
   * Set object position such that azimuth from host is < +/-45 deg
   * expexted data = 0.0
   */

   object.vcs_velocity.lateral = 0.1F;
   object.vcs_velocity.longitudinal = 1.0F;
   object.vcs_position.y = 0.1F;
   object.vcs_position.x = 1.0F;
   object.ndets = 1U;
   float32_t exp_res = 0.0F;


   /** \action
   * Call tested function
   */
   const float32_t res = Calc_Cross_Moving_Weight(raw_detect_list, sensors, calib, object);

   /** \result
   * Check whether res is 0.0F
   */
   DOUBLES_EQUAL(exp_res, res, F360_EPSILON);
}

/** \purpose
* Check that the return value is 0.0F if the number of dets for the object is zero
* \req
* NA.
*/
TEST(Calc_Cross_Moving_Weight, check_no_associated_dets)
{
   /** \precond
   * Set object velocity vector such that it points > +/-10 deg
   * Set object position such that azimuth from host is < +/-45 deg
   * number of dts for object  = 0
   * expexted data = 0.0
   */

   object.vcs_velocity.lateral = 2.0F;
   object.vcs_velocity.longitudinal = 1.0F;
   object.vcs_position.y = 0.1F;
   object.vcs_position.x = 1.0F;
   object.ndets = 0U;
   float32_t exp_res = 0.0F;


   /** \action
   * Call tested function
   */
   const float32_t res = Calc_Cross_Moving_Weight(raw_detect_list, sensors, calib, object);

   /** \result
   * Check whether res is 0.0F
   */
   DOUBLES_EQUAL(exp_res, res, F360_EPSILON);
}

/** \purpose
* Check that the cross_moving_weight is calculated correctly if one sensor
* has detections associated to object and object velocity vector is > 10 deg.
* \req
* NA.
*/
TEST(Calc_Cross_Moving_Weight, check_valid_velocity_one_detection)
{
   /** \precond
   * Set object velocity vector such that it points > +/-10 deg
   * Set object position such that azimuth from host is < +/-45 deg
   * number of dts for object  = 1
   * expexted data = 0.5527
   */

   object.vcs_velocity.lateral = 2.0F;
   object.vcs_velocity.longitudinal = 1.0F;
   object.vcs_position.y = 0.1F;
   object.vcs_position.x = 1.0F;
   object.ndets = 1;
   object.detids[0U] = 1U;
   raw_detect_list.detections[0U].raw.sensor_id = 1;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.0F;
   sensors[0].constant.mounting_position.vcs_position.lateral = 0.1F;
   float32_t exp_res = 0.5527F;


   /** \action
   * Call tested function
   */
   const float32_t res = Calc_Cross_Moving_Weight(raw_detect_list, sensors, calib, object);

   /** \result
   * Check whether res is 0.5527F
   */
   DOUBLES_EQUAL(exp_res, res, test_pass_threshold);
}

/** \purpose
* Check that the cross moving weight is calculated correct when object has detections from 2
* sensors and the velocity vector of object is > 10 deg
*
* \req
* NA.
*/
TEST(Calc_Cross_Moving_Weight, check_valid_velocity_two_sensor_detect_object)
{
   /** \precond
   * Set object velocity vector such that it points > +/-10 deg
   * Set object position such that azimuth from host is < +/-45 deg
   * number of dts for object  = 2
   * 2 sensor with detections - second sensor gives larger weight
   * expexted data = 0.6440F
   */

   object.vcs_velocity.lateral = -2.0F;
   object.vcs_velocity.longitudinal = 1.0F;
   object.vcs_position.y = 0.1F;
   object.vcs_position.x = 1.0F;
   object.ndets = 2;
   object.detids[0U] = 1U;
   object.detids[1U] = 2U;
   raw_detect_list.detections[0U].raw.sensor_id = 1;
   raw_detect_list.detections[1U].raw.sensor_id = 2;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.0F;
   sensors[0].constant.mounting_position.vcs_position.lateral = 0.1F;
   sensors[1].constant.mounting_position.vcs_position.longitudinal = 0.0F;
   sensors[2].constant.mounting_position.vcs_position.lateral = 0.0F;
   float32_t exp_res = 0.6440F;


   /** \action
   * Call tested function
   */
   const float32_t res = Calc_Cross_Moving_Weight(raw_detect_list, sensors, calib, object);

   /** \result
   * Check whether res is 0.6440F
   */
   DOUBLES_EQUAL(exp_res, res, test_pass_threshold);
}
/** @}*/

/** \defgroup  Calc_Moving_Speed_Threshold
*  @{
*/

/** \brief
* Test group of Calc_Moving_Speed_Threshold function. Tests checks
* that the speed threshold is calculated correctly.
* 
*/
TEST_GROUP(Calc_Moving_Speed_Threshold)
{
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] {};
   F360_Calibrations_T calib {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] {};
   F360_Object_Track_T object {};
   F360_TRKR_TIMING_INFO_T timing_info {};
   F360_Tracker_Info_T tracker_info {};
   F360_Occlusion_Data_T occlusion_data[MAX_NUMBER_OF_SENSORS]{};
   const float32_t test_pass_threshold = 1e-4F;

   /** \setup
   * Initialize tracker calibrations.
   */
   TEST_SETUP()
   {
      Set_Left_Rear_Sensor(sensors[0], sensors[0]);
      Set_Left_Front_Sensor(sensors[1], sensors[1]);
      Set_Right_Rear_Sensor(sensors[2], sensors[2]);
      Set_Right_Front_Sensor(sensors[3], sensors[3]);
      Initialize_Tracker_Calibrations(calib);
      (void)memset(occlusion_data, 0, sizeof(occlusion_data));
   }

   void Set_Base_Object_Parameters(
      F360_Object_Track_T& object, bool occluding)
   {
      if (occluding)
      {
         object.Set_Bbox_Orientation(Angle{F360_PI / 4.0F});
         object.vcs_heading = Angle{F360_PI / 4.0F};

         object.bbox.Set_Length(20.0F);
      }
      else
      {
         object.Set_Bbox_Orientation(Angle{0.0F});
         object.vcs_heading = Angle{0.0F};

         object.bbox.Set_Length(2.0F);
      }
      
      object.bbox.Set_Width(2.0F);

      object.movable_prob = 1.0F;
      object.status = F360_OBJECT_STATUS_UPDATED;
      
   }

   void Set_Left_Rear_Sensor(
      F360_Radar_Sensor_T& sensors,
      F360_Radar_Sensor_T& sensor)
   {
      Set_Common_Sensor_Parameters(sensors, sensor);
      sensors.constant.mounting_position.vcs_position.lateral = -0.75F;
      sensors.constant.mounting_position.vcs_position.longitudinal = -5.0F;
      sensors.constant.mounting_position.vcs_boresight_azimuth_angle = -2.35F;
   }

   void Set_Left_Front_Sensor(
      F360_Radar_Sensor_T& sensors,
      F360_Radar_Sensor_T& sensor)
   {
      Set_Common_Sensor_Parameters(sensors, sensor);
      sensors.constant.mounting_position.vcs_position.lateral = -0.75F;
      sensors.constant.mounting_position.vcs_position.longitudinal = -0.2F;
      sensors.constant.mounting_position.vcs_boresight_azimuth_angle = -0.78F;
   }

   void Set_Right_Rear_Sensor(
      F360_Radar_Sensor_T& sensors,
      F360_Radar_Sensor_T& sensor)
   {
      Set_Common_Sensor_Parameters(sensors, sensor);
      sensors.constant.mounting_position.vcs_position.lateral = 0.75F;
      sensors.constant.mounting_position.vcs_position.longitudinal = -5.0F;
      sensors.constant.mounting_position.vcs_boresight_azimuth_angle = 2.35F;
   }

   void Set_Right_Front_Sensor(
      F360_Radar_Sensor_T& sensors,
      F360_Radar_Sensor_T& sensor)
   {
      Set_Common_Sensor_Parameters(sensors, sensor);
      sensors.constant.mounting_position.vcs_position.lateral = 0.75F;
      sensors.constant.mounting_position.vcs_position.longitudinal = -0.2F;
      sensors.constant.mounting_position.vcs_boresight_azimuth_angle = 0.78F;
   }

   void Set_Common_Sensor_Parameters(
      F360_Radar_Sensor_T& sensors,
      F360_Radar_Sensor_T& sensor)
   {
      sensors.variable.is_valid = true;
      sensors.constant.sensor_type = F360_SENSOR_TYPE_SRR5_RADAR;
      sensor.variable.look_id = F360_DET_LOOK_ID_0;
      sensors.constant.fov_min_az_rad[sensor.variable.look_id] = -1.5F;
      sensors.constant.fov_max_az_rad[sensor.variable.look_id] = 1.5F;
      sensors.constant.range_limits[sensor.variable.look_id] = 50.0F;
   }

   void Set_Occluding_Objects_Parameters(bool is_object_occluding)
   {
      Set_Base_Object_Parameters(object_tracks[0], is_object_occluding);
      object_tracks[0].reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
      Point center = {-5.0F, 5.0F};
      object_tracks[0].bbox.Set_Center(center);
      object_tracks[0].vcs_position = object_tracks[0].bbox.Get_Corners().Rear_Right();
      object_tracks[0].confidenceLevel = 1.0F; 

      Set_Base_Object_Parameters(object_tracks[1], is_object_occluding);
      object_tracks[1].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object_tracks[1].Set_Bbox_Orientation(Angle{-F360_PI / 4.0F});
      object_tracks[1].vcs_heading = Angle{-F360_PI / 4.0F};
      center = {5.0F, 5.0F};
      object_tracks[1].bbox.Set_Center(center);
      object_tracks[1].vcs_position = object_tracks[1].bbox.Get_Corners().Rear_Left();
      object_tracks[1].confidenceLevel = 1.0F;

      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
   }
};

/** \purpose  
 * Purpose of this test is to verify that moving speed threshold is
 * not changed if the speed is lower than object motion min speed.
 * \req
 * NA.
 */
TEST(Calc_Moving_Speed_Threshold, Calc_Moving_Speed_Threshold_zero_speed_on_object)
{
   /** \precond
    * All basic input data was set in TEST_SETUP for sensors
    * Create occlusion object
    * object speed to 0
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   
   object.speed = 0.0;
   const float32_t exp_data = calib.k_object_motion_min_speed;

   /** \action
    * Determine occlusion status of point outside of FOV
    */
   const float32_t res = Calc_Moving_Speed_Threshold(raw_detect_list, sensors, calib, object, occlusion_data);
   /** \result
    * Check whether res == calib.k_object_motion_min_speed
    */
   DOUBLES_EQUAL(exp_data, res, test_pass_threshold);
}

/** \purpose  
 * Purpose of this test is to verify that moving_speed_threshold is not recalculated
 * when object is outside of occlusion zone.
 * \req
 * NA.
 */
TEST(Calc_Moving_Speed_Threshold, Calc_Moving_Speed_Threshold_outside_occlusion_zone)
{
   /** \precond
    * All basic input data was set in TEST_SETUP for occlusion
    * Create occlusion object
    * Set up object position to be outside of occlusion_zone
    */

   // Set up objects
   const bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   object.speed = calib.k_object_motion_min_speed + 0.2;
   object.vcs_position = Point(40.0F, 40.0F);
   const float32_t exp_data = calib.k_object_motion_min_speed;
   /** \action
    * * Determine threshold value
    */
   const float32_t res = Calc_Moving_Speed_Threshold(raw_detect_list, sensors, calib, object, occlusion_data);
   /** \result
    * Check whether res == calib.k_object_motion_min_speed
    */
   DOUBLES_EQUAL(exp_data, res, test_pass_threshold);
}

/** \purpose  
 * Purpose of this test is to verify that moving_speed_threshold is increased
 * when object centroid is inside of occlusion_zone and object is occluded
 * \req
 * NA.
 */
TEST(Calc_Moving_Speed_Threshold, Calc_Moving_Speed_Threshold_object_occluded)
{
   /** \precond
    * All basic input data was set in TEST_SETUP for occlusion
    * Create occlusion object
    * Set up object position to be inside of occlusion_zone
    * Set up occluding object
    */

   // Set up objects
   const bool is_occluding = true;
   Set_Occluding_Objects_Parameters(is_occluding);

   object.speed = calib.k_object_motion_min_speed + 0.2F;
   object.vcs_position = Point(8.0F, 7.0F);
   object.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = object.vcs_position;
   object.bbox.Set_Center(center);
   object.vcs_velocity.longitudinal = 7.0F;
   object.vcs_velocity.lateral = -8.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object.movable_prob = 0.0F;
   object.ndets = 1;
   object.detids[0U] = 1U;
   raw_detect_list.detections[0U].raw.sensor_id = 4;
   
   // set up occlusion
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   // Expected data
   const float32_t exp_res = 2.8312F;
   /** \action
    * * Determine threshold value
    */
   const float32_t res = Calc_Moving_Speed_Threshold(raw_detect_list, sensors, calib, object, occlusion_data);
   /** \result
    * Check whether res == exp_res
    */
   DOUBLES_EQUAL(exp_res, res, test_pass_threshold);
}

/** \purpose  
 * Purpose of this test is to verify that moving_speed_threshold is not recalculated
 * when object centroid is inside of occlusion_zone and object is not occluded.
 * \req
 * NA.
 */
TEST(Calc_Moving_Speed_Threshold, Calc_Moving_Speed_Threshold_object_not_occluded)
{
   /** \precond
    * All basic input data was set in TEST_SETUP for occlusion
    * Create occlusion object
    * Set up object position to be inside of occlusion_zone
    * Set up objects not occluding 
    */

   // Set up objects
   const bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);

   // Set up object properties 
   object.speed = calib.k_object_motion_min_speed + 0.2F;
   object.vcs_position = Point(8.0F, 1.0F);
   object.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = Point(8.0F, 1.0F);
   object.bbox.Set_Center(center);
   object.vcs_velocity.longitudinal = 1.0F;
   object.vcs_velocity.lateral = 10.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object.movable_prob = 0.0F;
   object.ndets = 1;
   object.detids[0U] = 1U;
   raw_detect_list.detections[0U].raw.sensor_id = 1;
   // set up occlusion
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   // Expected data
   const float32_t exp_res = 0.5F;
   /** \action
    * Determine threshold value
    */
   const float32_t res = Calc_Moving_Speed_Threshold(raw_detect_list, sensors, calib, object, occlusion_data);
   /** \result
    * Check whether res == exp_res
    */
   DOUBLES_EQUAL(exp_res, res, test_pass_threshold);
}
/** @}*/

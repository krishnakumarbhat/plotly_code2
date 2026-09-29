/** \file
 * This file contains unit tests for content of f360_object_motion_classification_helpers.cpp file
 */

#include "f360_object_motion_classification_helpers.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  update_object_properties
*  @{
*/

/** \brief
* Test group of Update_Object_Properties() function.
* Tests verify whether object motion properties are properly updated.
*/
TEST_GROUP(update_object_properties)
{
   F360_Object_Track_T object;
   F360_Globals_T globals;

   /** \setup
   * Initialize globals structure
   */
   TEST_SETUP()
   {
      globals.oncoming_speed_thresh = 10.0F;
      globals.obj_vehicular_spd_thresh = 5.0F;
   }
};

/** \purpose
* Purpose of this test is to verify whether object is marked as oncoming if meets all conditions
* \req
* NA.
*/
TEST(update_object_properties, update_object_properties__check_if_is_marked_as_oncoming_if_meets_all_conditions)
{
   /** \precond
   * Set object parameters to meet all conditions to be marked as oncoming
   */
   object.f_moving = true;
   object.vcs_velocity.longitudinal = globals.oncoming_speed_thresh - 10.0F;
   object.on_sep_id = F360_INVALID_UNSIGNED_ID;
   object.f_oncoming = false;

   /** \action
   * Call tested funciton
   */
   Update_Object_Properties(object, globals);

   /** \result
   * Check whether object f_oncoming flag was set
   */
   CHECK_TRUE(object.f_oncoming);
}

/** \purpose
* Purpose of this test is to verify whether object is not marked as oncoming if its longitudinal vcs velocity is above threshold
* \req
* NA.
*/
TEST(update_object_properties, update_object_properties__check_if_is_not_marked_as_oncoming_if_vcs_velocity_is_above_threshold)
{
   /** \precond
   * Set object parameters to meet all conditions to be marked as oncoming
   * Set vcs velocity to be above threshold
   */
   object.f_moving = true;
   object.vcs_velocity.longitudinal = globals.oncoming_speed_thresh + 10.0F;
   object.on_sep_id = F360_INVALID_UNSIGNED_ID;
   object.f_oncoming = true;

   /** \action
   * Call tested funciton
   */
   Update_Object_Properties(object, globals);

   /** \result
   * Check whether object f_oncoming flag was reset
   */
   CHECK_FALSE(object.f_oncoming);
}

/** \purpose
* Purpose of this test is to verify whether object is not marked as oncoming if it is not moving
* \req
* NA.
*/
TEST(update_object_properties, update_object_properties__check_if_is_not_marked_as_oncoming_if_it_is_not_moving)
{
   /** \precond
   * Set object parameters to meet all conditions to be marked as oncoming
   * Set f_moving flag as false
   */
   object.f_moving = false;
   object.vcs_velocity.longitudinal = globals.oncoming_speed_thresh - 10.0F;
   object.on_sep_id = F360_INVALID_UNSIGNED_ID;
   object.f_oncoming = true;

   /** \action
   * Call tested funciton
   */
   Update_Object_Properties(object, globals);

   /** \result
   * Check whether object f_oncoming flag was reset
   */
   CHECK_FALSE(object.f_oncoming);
}

/** \purpose
* Purpose of this test is to verify whether object is not marked as oncoming if its on guardrail
* \req
* NA.
*/
TEST(update_object_properties, update_object_properties__check_if_is_not_marked_as_oncoming_if_it_is_on_guardrail)
{
   /** \precond
   * Set object parameters to meet all conditions to be marked as oncoming
   * Set on sep id to 1
   */
   object.f_moving = true;
   object.vcs_velocity.longitudinal = globals.oncoming_speed_thresh - 10.0F;
   object.on_sep_id = 1;
   object.f_oncoming = true;

   /** \action
   * Call tested funciton
   */
   Update_Object_Properties(object, globals);

   /** \result
   * Check whether object f_oncoming flag was reset
   */
   CHECK_FALSE(object.f_oncoming);
}

/** \purpose
* Purpose of this test is to verify whether object is marked as vehicular track if meets all conditions
* \req
* NA.
*/
TEST(update_object_properties, update_object_properties__check_if_is_marked_as_vehicular_if_meets_all_conditions)
{
   /** \precond
   * Set object parameters to meet all conditions to be marked as vehicular
   */
   object.f_moving = true;
   object.f_vehicular_trk = false;
   object.speed = globals.obj_vehicular_spd_thresh + 10.0F;

   /** \action
   * Call tested funciton
   */
   Update_Object_Properties(object, globals);

   /** \result
   * Check whether object f_oncoming flag was reset
   */
   CHECK_TRUE(object.f_vehicular_trk);
}

/** \purpose
* Purpose of this test is to verify whether object is not marked as vehicular track if it is not moving
* \req
* NA.
*/
TEST(update_object_properties, update_object_properties__check_if_not_marked_as_vehicular_if_not_moving)
{
   /** \precond
   * Set object parameters to meet all conditions to be marked as vehicular
   * Set its moving flag to false
   */
   object.f_moving = false;
   object.f_vehicular_trk = false;
   object.speed = globals.obj_vehicular_spd_thresh + 10.0F;

   /** \action
   * Call tested funciton
   */
   Update_Object_Properties(object, globals);

   /** \result
   * Check whether object f_oncoming flag was reset
   */
   CHECK_FALSE(object.f_vehicular_trk);
}

/** \purpose
* Purpose of this test is to verify whether object is not marked as vehicular track if its speed is below threshold
* \req
* NA.
*/
TEST(update_object_properties, update_object_properties__check_if_not_marked_as_vehicular_if_speed_below_threshold)
{
   /** \precond
   * Set object parameters to meet all conditions to be marked as vehicular
   * Set its speed below threshold
   */
   object.f_moving = false;
   object.f_vehicular_trk = false;
   object.speed = 0.0F;

   /** \action
   * Call tested funciton
   */
   Update_Object_Properties(object, globals);

   /** \result
   * Check whether object f_oncoming flag was reset
   */
   CHECK_FALSE(object.f_vehicular_trk);
}

/** \purpose
* Purpose of this test is to verify whether object vehicular flag is not changed if flag is already set
* \req
* NA.
*/
TEST(update_object_properties, update_object_properties__check_if_vehicuar_flag_is_not_changed)
{
   /** \precond
   * Set object parameters to meet all conditions to be marked as vehicular
   * Set its speed below threshold
   */
   object.f_moving = false;
   object.f_vehicular_trk = true;
   object.speed = globals.obj_vehicular_spd_thresh - 10.0F;

   /** \action
   * Call tested funciton
   */
   Update_Object_Properties(object, globals);

   /** \result
   * Check whether object f_oncoming flag was reset
   */
   CHECK_TRUE(object.f_vehicular_trk);
}
/** @}*/

/** \defgroup  get_min_num_consec_moving
*  @{
*/

/** \brief
* Test group of Get_Min_Num_Consec_Moving() function.
* Tests verify whether function properly assigns minimal scans count for specific objects types.
*/
TEST_GROUP(get_min_num_consec_moving)
{
   F360_Object_Track_T object;
   float32_t host_yaw_rate_rad;
   F360_Calibrations_T calib;

   /** \setup
   * Initialize globals structure
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
* Purpose of this test is to verify whether when movable flag is set, function returns number equal to specific calib value.
* \req
* NA.
*/
TEST(get_min_num_consec_moving, get_min_num_consec_moving__movable_object)
{
   /** \precond
   * Set object flag as movable
   */
   object.movable_prob = 1.0F;

   /** \action
   * Call tested funciton
   */
   int32_t min_num_consec_moving = Get_Min_Num_Consec_Moving(object, host_yaw_rate_rad, calib);

   /** \result
   * Check whether returned value is equal to calib.k_object_motion_min_moving_cnt_movable
   */
   CHECK_EQUAL(calib.k_object_motion_min_consec_moving_cnt_movable_th, min_num_consec_moving);
}

/** \purpose
* Purpose of this test is to verify whether when movable flag is not set, and host yaw rate is below threshold
* function returns value equal to calib.k_object_motion_min_moving_cnt
* \req
* NA.
*/
TEST(get_min_num_consec_moving, get_min_num_consec_moving__not_movable_object_small_yaw_rate)
{
   /** \precond
   * Set object flag as movable
   * Set host yaw rate to 0
   */
   object.movable_prob = 0.0F;
   host_yaw_rate_rad = 0.0F;

   /** \action
   * Call tested funciton
   */
   int32_t min_num_consec_moving = Get_Min_Num_Consec_Moving(object, host_yaw_rate_rad, calib);

   /** \result
   * Check whether returned value is equal to calib.k_object_motion_min_moving_cnt
   */
   CHECK_EQUAL(calib.k_object_motion_min_consec_moving_cnt_th, min_num_consec_moving);
}

/** \purpose
* Purpose of this test is to verify whether when movable flag is not set, and host yaw rate is above threshold
* function returns value equal to calib.k_object_motion_min_moving_cnt_high_yaw
* \req
* NA.
*/
TEST(get_min_num_consec_moving, get_min_num_consec_moving__not_movable_object_high_yaw_rate)
{
   /** \precond
   * Set object flag as movable
   * Set host yaw rate to value above threshold
   */
   object.movable_prob = 0.0F;
   host_yaw_rate_rad = calib.k_object_motion_min_host_yaw_rate_th + 0.1F;

   /** \action
   * Call tested funciton
   */
   int32_t min_num_consec_moving = Get_Min_Num_Consec_Moving(object, host_yaw_rate_rad, calib);

   /** \result
   * Check whether returned value is equal to calib.k_object_motion_min_moving_cnt_high_yaw
   */
   CHECK_EQUAL(calib.k_object_motion_min_consec_moving_cnt_high_yaw_th, min_num_consec_moving);
}
/** @}*/

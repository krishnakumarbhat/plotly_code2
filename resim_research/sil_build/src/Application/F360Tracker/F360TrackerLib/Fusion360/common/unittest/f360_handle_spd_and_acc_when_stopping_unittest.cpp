/** \file
 * This file contains unit tests for content of f360_handle_spd_and_acc_when_stopping.cpp file
 */

#include "f360_handle_spd_and_acc_when_stopping.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_handle_spd_and_acc_when_stopping_ctca
 *  @{
 */

/** \brief
 * This test group is checking that the function Handle_Spd_And_Acc_When_Stopping_CTCA() is working as intended.
 * The purpose of the function is to set object speed and acceleration to 0 if an object is braking hard 
 * (acceleration is largely negative) and the sign of the object speed has changed from positive to negative.
 */
TEST_GROUP(f360_handle_spd_and_acc_when_stopping_ctca)
{	
   // Declare common variables used within all tests in this test group.
   float32_t previous_speed;
   F360_Calibrations_T calibs;
   F360_Object_Track_T obj;
   
   const float32_t test_pass_th = F360_EPSILON;

   
   /** \setup
    * Set up a test scenario where an object is braking hard and speed changes sign of speed has changed from positive to negative. I.e:
    *    - initialize calibrations to default tracker calibration values.
    *    - set object tangential acceleration to be just below the default tracker calibration value k_acc_threshold_for_breaking.
    *    - set previous speed to be just above 0 m/s
    *    - set object speed to be just below 0 m/s
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      previous_speed = F360_EPSILON;
      obj.speed = -F360_EPSILON;
      obj.tang_accel = -calibs.k_abs_acc_threshold_for_breaking - F360_EPSILON;

   }
};

/** \purpose  
 * Test so that the object speed and acceleration is set to 0 when an object is braking hard 
 * (acceleration is largely negative) and the sign of the object speed has changed from positive to negative.
 * \req
 * NA
 */
TEST(f360_handle_spd_and_acc_when_stopping_ctca, Test_Spd_And_Acc_Zero_When_Acc_Large_And_Speed_Changes_Sign_To_Neg)
{
   /** \precond
    * Default data from test setup can be used. I.e
    *    - calibrations correspond to the default tracker calibration values.
    *    - object tangential acceleration is just below the default tracker calibration value k_acc_threshold_for_breaking.
    *    - previous speed is just above 0 m/s
    *    - object speed is just below 0 m/s
    */
	
   /** \action
    * Call the function Handle_Spd_And_Acc_When_Stopping_CTCA()
    */
   Handle_Spd_And_Acc_When_Stopping_CTCA(previous_speed, calibs, obj);

   /** \result
    * Check so that
    * object speed is 0
    * object tangential acceleration is 0
    */
   DOUBLES_EQUAL_TEXT(0.0F, obj.speed, test_pass_th, "Speed is non-zero");
   DOUBLES_EQUAL_TEXT(0.0F, obj.tang_accel, test_pass_th, "Tangential acceleration is non-zero");
}
/** @}*/

/** \purpose  
 * Test so that the object speed and acceleration is not changed when an object is braking hard 
 * (acceleration is largely negative) and the sign of the object speed has changed from negative to positive.
 * \req
 * NA
 */
TEST(f360_handle_spd_and_acc_when_stopping_ctca, Test_Spd_And_Acc_Unghanged_When_Acc_Large_And_Speed_Changes_Sign_To_Pos)
{
   /** \precond
    * Default data from test setup that are used are:
    *    - calibrations correspond to the default tracker calibration values.
    *    - object tangential acceleration is just below the default tracker calibration value k__acc_threshold_for_breaking.
    * Also, set:
    *    - previous speed to just below 0 m/s
    *    - object speed to just above 0 m/s
    */
   previous_speed = -F360_EPSILON;
   obj.speed = F360_EPSILON;
	
   /** \action
    * Extract the object speed and tangential acceleration before the call to the function Handle_Spd_And_Acc_When_Stopping_CTCA().
    * Then call the function Handle_Spd_And_Acc_When_Stopping_CTCA()
    */
   const float32_t speed_before = obj.speed;
   const float32_t tang_acc_before = obj.tang_accel;
   Handle_Spd_And_Acc_When_Stopping_CTCA(previous_speed, calibs, obj);

   /** \result
    * Check so that
    *    - object speed is unchanged by the call to Handle_Spd_And_Acc_When_Stopping_CTCA()
    *    - object tangential acceleration is unchanged by the call to Handle_Spd_And_Acc_When_Stopping_CTCA()
    */
   DOUBLES_EQUAL_TEXT(speed_before, obj.speed, test_pass_th, "Speed has been modified");
   DOUBLES_EQUAL_TEXT(tang_acc_before, obj.tang_accel, test_pass_th, "Tangential acceleration has been modified");
}

/** \purpose  
 * Test so that the object speed and acceleration is not changed when an object is braking hard 
 * (acceleration is largely negative) and the sign of the object speed is positive and has not changed.
 * \req
 * NA
 */
TEST(f360_handle_spd_and_acc_when_stopping_ctca, Test_Spd_And_Acc_Unghanged_When_Acc_Large_And_Speed_Is_Neg_And_Sign_Not_Changed)
{
   /** \precond
    * Default data from test setup that are used are:
    *    - calibrations correspond to the default tracker calibration values.
    *    - object tangential acceleration is just below the default tracker calibration value k_acc_threshold_for_breaking.
    * Also, set:
    *    - previous speed to just below 0 m/s
    *    - object speed to just below 0 m/s
    */
   previous_speed = -F360_EPSILON;
   obj.speed = -F360_EPSILON;
	
   /** \action
    * Extract the object speed and tangential acceleration before the call to the function Handle_Spd_And_Acc_When_Stopping_CTCA().
    * Then call the function Handle_Spd_And_Acc_When_Stopping_CTCA()
    */
   const float32_t speed_before = obj.speed;
   const float32_t tang_acc_before = obj.tang_accel;
   Handle_Spd_And_Acc_When_Stopping_CTCA(previous_speed, calibs, obj);

   /** \result
    * Check so that
    *    - object speed is unchanged by the call to Handle_Spd_And_Acc_When_Stopping_CTCA()
    *    - object tangential acceleration is unchanged by the call to Handle_Spd_And_Acc_When_Stopping_CTCA()
    */
   DOUBLES_EQUAL_TEXT(speed_before, obj.speed, test_pass_th, "Speed has been modified");
   DOUBLES_EQUAL_TEXT(tang_acc_before, obj.tang_accel, test_pass_th, "Tangential acceleration has been modified");
}

/** \purpose  
 * Test so that the object speed and acceleration is not changed when an object is braking hard 
 * (acceleration is largely negative) and the sign of the object speed is negative and has not changed.
 * \req
 * NA
 */
TEST(f360_handle_spd_and_acc_when_stopping_ctca, Test_Spd_And_Acc_Unghanged_When_Acc_Large_And_Speed_Is_Pos_And_Sign_Not_Changed)
{
   /** \precond
    * Default data from test setup that are used are:
    *    - calibrations correspond to the default tracker calibration values.
    *    - object tangential acceleration is just below the default tracker calibration value k_acc_threshold_for_breaking.
    * Also, set:
    *    - previous speed to just above 0 m/s
    *    - object speed to just above 0 m/s
    */
   previous_speed = F360_EPSILON;
   obj.speed = F360_EPSILON;
	
   /** \action
    * Extract the object speed and tangential acceleration before the call to the function Handle_Spd_And_Acc_When_Stopping_CTCA().
    * Then call the function Handle_Spd_And_Acc_When_Stopping_CTCA()
    */
   const float32_t speed_before = obj.speed;
   const float32_t tang_acc_before = obj.tang_accel;
   Handle_Spd_And_Acc_When_Stopping_CTCA(previous_speed, calibs, obj);

   /** \result
    * Check so that
    *    - object speed is unchanged by the call to Handle_Spd_And_Acc_When_Stopping_CTCA()
    *    - object tangential acceleration is unchanged by the call to Handle_Spd_And_Acc_When_Stopping_CTCA()
    */
   DOUBLES_EQUAL_TEXT(speed_before, obj.speed, test_pass_th, "Speed has been modified");
   DOUBLES_EQUAL_TEXT(tang_acc_before, obj.tang_accel, test_pass_th, "Tangential acceleration has been modified");
}

/** \purpose  
 * Test so that the object speed and acceleration is not changed when an object is braking soft 
 * (acceleration is insignificantly negative) and the sign of the object speed has changed from positive to negative.
 * \req
 * NA
 */
TEST(f360_handle_spd_and_acc_when_stopping_ctca, Test_Spd_And_Acc_Unghanged_When_Acc_Small_And_Speed_Changes_Sign_To_Neg)
{
   /** \precond
    * Default data from test setup that are used are:
    *    - calibrations correspond to the default tracker calibration values.
    *    - previous speed is just above 0 m/s
    *    - object speed to just below 0 m/s
    * Also, set:
    *    - object tangential acceleration is just above the default tracker calibration value k_acc_threshold_for_breaking.
    */
   obj.tang_accel = -calibs.k_abs_acc_threshold_for_breaking + F360_EPSILON;
	
   /** \action
    * Extract the object speed and tangential acceleration before the call to the function Handle_Spd_And_Acc_When_Stopping_CTCA().
    * Then call the function Handle_Spd_And_Acc_When_Stopping_CTCA()
    */
   const float32_t speed_before = obj.speed;
   const float32_t tang_acc_before = obj.tang_accel;
   Handle_Spd_And_Acc_When_Stopping_CTCA(previous_speed, calibs, obj);

   /** \result
    * Check so that
    *    - object speed is unchanged by the call to Handle_Spd_And_Acc_When_Stopping_CTCA()
    *    - object tangential acceleration is unchanged by the call to Handle_Spd_And_Acc_When_Stopping_CTCA()
    */
   DOUBLES_EQUAL_TEXT(speed_before, obj.speed, test_pass_th, "Speed has been modified");
   DOUBLES_EQUAL_TEXT(tang_acc_before, obj.tang_accel, test_pass_th, "Tangential acceleration has been modified");
}

/** \defgroup  f360_handle_spd_and_acc_when_stopping_cca
 *  @{
 */

/** \brief
 * This test group is checking that the function Handle_Spd_And_Acc_When_Stopping_CCA() is working as intended.
 * The purpose of the function is to set object speed and acceleration to 0 if an object is braking hard 
 * (acceleration is largely negative) and the sign of the object speed has changed from positive to negative.
 */

TEST_GROUP(f360_handle_spd_and_acc_when_stopping_cca)
{	
   // Declare common variables used within all tests in this test group.
   F360_VCS_Velocity_T previous_vel;
   F360_Calibrations_T calibs;
   F360_Object_Track_T obj;
   
   const float32_t test_pass_th = F360_EPSILON;

   
   /** \setup
    * Set up a test scenario where an object with speed has changed from positive to negative. I.e:
    *    - initialize calibrations to default tracker calibration values.
    *    - set object tangential acceleration to be just below the default tracker calibration value k_acc_threshold_for_breaking.
    *    - set previous speed to be just above 0 m/s
    *    - set object speed to be just below 0 m/s
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      previous_vel.lateral = F360_EPSILON;
      previous_vel.longitudinal = F360_EPSILON;
      obj.speed = -F360_EPSILON;
      obj.tang_accel = -calibs.k_abs_acc_threshold_for_breaking - F360_EPSILON;

   }
};


/** \purpose  
 * Test so that the object speed and acceleration is not set to 0 when current velocity projection is negative.
 * \req
 * NA
 */
TEST(f360_handle_spd_and_acc_when_stopping_cca, Test_Spd_And_Acc_Not_Change_When_Current_Velocity_Projectioin_Negative)
{
   /** \precond
    * Default data from test setup can be used. I.e
    *    - calibrations correspond to the default tracker calibration values.
    *    - object tangential acceleration is just below the default tracker calibration value k_acc_threshold_for_breaking.
    */
   obj.vcs_accel.longitudinal = 0.5F + F360_EPSILON;
   obj.vcs_accel.lateral = 0.0F;
   obj.vcs_velocity.longitudinal = -calibs.k_abs_speed_threshold_for_stopping + F360_EPSILON;
   obj.speed = -calibs.k_abs_speed_threshold_for_stopping + F360_EPSILON;

   previous_vel.lateral = F360_EPSILON;
   previous_vel.longitudinal = -F360_EPSILON;

   float32_t exp_tang_accel = obj.tang_accel;
   float32_t exp_speed = obj.speed;

   /** \action
    * Call the function Handle_Spd_And_Acc_When_Stopping_CCA()
    */
   Handle_Spd_And_Acc_When_Stopping_CCA(previous_vel, calibs, obj);

   /** \result
    * Check so that
    * object speed is exp_speed
    * object tangential acceleration is exp_tang_accel
    */
   DOUBLES_EQUAL_TEXT(exp_speed, obj.speed, test_pass_th, "Speed is updated");
   DOUBLES_EQUAL_TEXT(exp_tang_accel, obj.tang_accel, test_pass_th, "Tangential acceleration is updated");
}
/** \purpose  
 * Test so that the object speed and acceleration is set to 0 when an object is braking hard 
 * (acceleration is largely negative) and the sign of the object speed has changed from positive to negative.
 * \req
 * NA
 */
TEST(f360_handle_spd_and_acc_when_stopping_cca, Test_Spd_And_Acc_Zero_When_Condition_Satisfied)
{
   /** \precond
    * Default data from test setup can be used. I.e
    *    - calibrations correspond to the default tracker calibration values.
    *    - object tangential acceleration is just below the default tracker calibration value k_acc_threshold_for_breaking.
    */
   obj.vcs_accel.longitudinal = 0.5F + F360_EPSILON;
   obj.vcs_accel.lateral = 0.0F;
   obj.vcs_velocity.longitudinal = calibs.k_abs_speed_threshold_for_stopping - F360_EPSILON;
   obj.speed = calibs.k_abs_speed_threshold_for_stopping - F360_EPSILON;

   previous_vel.lateral = F360_EPSILON;
   previous_vel.longitudinal = -F360_EPSILON;

   /** \action
    * Call the function Handle_Spd_And_Acc_When_Stopping_CCA()
    */
   Handle_Spd_And_Acc_When_Stopping_CCA(previous_vel, calibs, obj);

   /** \result
    * Check so that
    * object speed is 0
    * object tangential acceleration is 0
    */
   DOUBLES_EQUAL_TEXT(0, obj.speed, test_pass_th, "Speed is updated");
   DOUBLES_EQUAL_TEXT(0, obj.tang_accel, test_pass_th, "Tangential acceleration is updated");
}

/** \purpose  
 * Test so that the object speed and acceleration is not set to 0 when previous velocity projection is positive.
 * \req
 * NA
 */
TEST(f360_handle_spd_and_acc_when_stopping_cca, Test_Spd_And_Acc_Unchanged_When_Prev_Velocity_Projection_Positive)
{
   /** \precond
    * Default data from test setup can be used. I.e
    *    - calibrations correspond to the default tracker calibration values.
    *    - object tangential acceleration is just below the default tracker calibration value k_acc_threshold_for_breaking.
    */
   obj.vcs_accel.longitudinal = 0.5F + F360_EPSILON;
   obj.vcs_accel.lateral = 0.0F;
   obj.vcs_velocity.longitudinal = calibs.k_abs_speed_threshold_for_stopping - F360_EPSILON;
   obj.speed = calibs.k_abs_speed_threshold_for_stopping - F360_EPSILON;

   previous_vel.lateral = F360_EPSILON;
   previous_vel.longitudinal = F360_EPSILON;

   float32_t exp_tang_accel = obj.tang_accel;
   float32_t exp_speed = obj.speed;
   /** \action
    * Call the function Handle_Spd_And_Acc_When_Stopping_CTCA()
    */
   Handle_Spd_And_Acc_When_Stopping_CCA(previous_vel, calibs, obj);

   /** \result
    * Check so that
    * object speed is exp_speed
    * object tangential acceleration is exp_tang_accel
    */
   DOUBLES_EQUAL_TEXT(exp_speed, obj.speed, test_pass_th, "Speed is updated");
   DOUBLES_EQUAL_TEXT(exp_tang_accel, obj.tang_accel, test_pass_th, "Tangential acceleration is updated");
}
/** @}*/

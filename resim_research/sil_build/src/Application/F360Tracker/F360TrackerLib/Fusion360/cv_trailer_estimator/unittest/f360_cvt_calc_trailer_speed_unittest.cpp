/** \file
 * This file contains unit tests for content of f360_cvt_calc_trailer_speed.cpp file
 */

#include "f360_cvt_calc_trailer_speed.h"
#include "f360_math_func.h"
#include <CppUTest/TestHarness.h>
#include <vector>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_cvt_calc_trailer_speed
 *  @{
 */

/** \brief
 * Test group declares the variables and struct used for the tests
 */
TEST_GROUP(f360_cvt_calc_trailer_speed_unittest)
{

   float32_t trailer_angle;
   float32_t cos_trailer_angle;
   float32_t sin_trailer_angle;
   float32_t host_rear_axle_to_joint;
   float32_t host_speed;
   float32_t sideslip;
   float32_t cos_sideslip;
   float32_t sin_sideslip;
   float32_t dist_origin_to_host_rear_axle;
   float32_t joint_lon_vel;
   float32_t epsilon_float;

   // The struct to save all parameters for one test
   struct trailer_speed_ut_case_T
   {
      float32_t trailer_angle;   // Angle of the trailer, following Right hand rule, positive means trailer at rear left
      float32_t sideslip;    // Side slip angle of the host, Following Right Hand rule, positive means host turning right
      float32_t host_speed;  // Speed of the host
      float32_t host_rear_axle_to_joint;     // the longitudinal distance from the host's rear axle to the joint
      float32_t dist_origin_to_host_rear_axle;  // longitudinal distance from the vcs origin to the rear axle of the host
      float32_t reference_joint_lon_vel;  // Reference joint velocity for the assertion
      std::string test_description;  // Descriptor of the test
   };

   /** \setup
    * Initialize all input arguments
    * Set eps to 1e-3
    */
   TEST_SETUP()
   {

      trailer_angle = {};
      cos_trailer_angle = {};
      sin_trailer_angle = {};
      host_rear_axle_to_joint = {};
      host_speed = {};
      sideslip = {};
      cos_sideslip = {};
      sin_sideslip = {};
      dist_origin_to_host_rear_axle = {};
      epsilon_float = 1E-3F;
   }
};

/** \purpose
 * Compute the trailer speed in different setups of host speed directions, host sideslip directions and trailer directions.
 * * \req
 * NA
 *  */
TEST(f360_cvt_calc_trailer_speed_unittest, F360_CV_Trailer_Speeds_Correct)
{
   /** \precond
    * All intermediate variables initialized in test setup
    */

   // Define all test cases
   
   std::vector<trailer_speed_ut_case_T> all_test_cases = {
      //Trailer Angle,         Sideslip Angle,       host speed,   a1 (host_rear_axle_to_joint),   b1 (dist_origin_to_host_rear_axle),     reference_joint_lon_vel,         test_description
       {F360_DEG2RAD(0.0F),    F360_DEG2RAD(0.0F),    2.0F,          4.0F,                          8.0F,                                   2.0F,                           "Test1: Going Forward, 0 degree sideslip, 0 degree trailer angle"},
       {F360_DEG2RAD(90.0F),   F360_DEG2RAD(0.0F),    2.0F,          4.0F,                          8.0F,                                   0.0F,                           "Test2: Going Forward, 0 degree sideslip, 90 degrees trailer angle to left"},
       {F360_DEG2RAD(45.0F),   F360_DEG2RAD(30.0F),   2.0F,          4.0F,                          8.0F,                                   0.8712F,                        "Test3: Going Forward, 30 degrees sideslip to right, 45 degrees trailer angle to left"},
       {F360_DEG2RAD(-45.0F),  F360_DEG2RAD(30.0F),   2.0F,          4.0F,                          8.0F,                                   1.5783F,                        "Test4: Going Forward, 30 degrees sideslip to right, 45 degrees trailer angle to right"},
       {F360_DEG2RAD(-45.0F),  F360_DEG2RAD(-30.0F),  2.0F,          4.0F,                          8.0F,                                   0.8712F,                        "Test5: Going Forward, 30 degrees sideslip to left, 45 degrees trailer angle to right"},
       {F360_DEG2RAD(45.0F),   F360_DEG2RAD(-30.0F),  2.0F,          4.0F,                          8.0F,                                   1.5783F,                        "Test6: Going Forward, 30 degrees sideslip to left, 45 degrees trailer angle to left"},
       {F360_DEG2RAD(0.0F),    F360_DEG2RAD(0.0F),   -2.0F,          4.0F,                          8.0F,                                   -2.0F,                          "Test7: Reversing, 0 degree sideslip, 0 degree trailer angle"},
       {F360_DEG2RAD(90.0F),   F360_DEG2RAD(0.0F),   -2.0F,          4.0F,                          8.0F,                                   -0.0F,                          "Test8: Reversing, 0 degree sideslip, 90 degrees trailer angle to left"},
       {F360_DEG2RAD(45.0F),   F360_DEG2RAD(30.0F),  -2.0F,          4.0F,                          8.0F,                                   -0.8712F,                       "Test9: Reversing, 30 degrees sideslip to right, 45 degrees trailer angle to left"},
       {F360_DEG2RAD(-45.0F),  F360_DEG2RAD(30.0F),  -2.0F,          4.0F,                          8.0F,                                   -1.5783F,                       "Test10: Reversing, 30 degrees sideslip to right, 45 degrees trailer angle to right"},
       {F360_DEG2RAD(-45.0F),  F360_DEG2RAD(-30.0F), -2.0F,          4.0F,                          8.0F,                                   -0.8712F,                       "Test11: Reversing, 30 degrees sideslip to left, 45 degrees trailer angle to right"},
       {F360_DEG2RAD(45.0F),   F360_DEG2RAD(-30.0F), -2.0F,          4.0F,                          8.0F,                                   -1.5783F,                       "Test12: Reversing, 30 degrees sideslip to left, 45 degrees trailer angle to left"},
   };

   // Loop over all test cases
   for (const trailer_speed_ut_case_T &test_case_i : all_test_cases)
   {
      // Arrange all test parameters for the current test case
      joint_lon_vel = {};
      
      trailer_angle = test_case_i.trailer_angle;
      cos_trailer_angle = F360_Cosf(trailer_angle);
      sin_trailer_angle = F360_Sinf(trailer_angle);
      
      sideslip = test_case_i.sideslip;
      cos_sideslip = F360_Cosf(sideslip);
      sin_sideslip = F360_Sinf(sideslip);
      
      host_speed = test_case_i.host_speed;
      host_rear_axle_to_joint = test_case_i.host_rear_axle_to_joint;
      
      dist_origin_to_host_rear_axle = test_case_i.dist_origin_to_host_rear_axle;

      const float32_t reference_joint_lon_vel = test_case_i.reference_joint_lon_vel;
      
      /** \action
       * call calculate_trailer_longitudinal_velocity() to compute the trailer velocity for the current test case
       */

      calculate_trailer_longitudinal_velocity(cos_trailer_angle, sin_trailer_angle, host_rear_axle_to_joint, host_speed, cos_sideslip, sin_sideslip, dist_origin_to_host_rear_axle, joint_lon_vel);

      /** \result
       * Check that the computed trailer velocity is correct for the current test case
       */
      DOUBLES_EQUAL_TEXT(reference_joint_lon_vel, joint_lon_vel, epsilon_float, test_case_i.test_description.c_str());
   }
}


/** @}*/

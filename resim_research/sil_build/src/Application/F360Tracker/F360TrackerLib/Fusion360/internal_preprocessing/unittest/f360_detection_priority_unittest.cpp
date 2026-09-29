/** \file
 * This file contains unit tests for content of f360_detection_priority.cpp file
 */

#include "f360_detection_priority.h"
#include <CppUTest/TestHarness.h>
#include <iostream>
// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_detection_priority
 *  @{
 */

/** \brief
 * Test the behavior of the detection priority function. Ensure consistent behavior regardless of range-rate aliasing.
 */
TEST_GROUP(f360_detection_priority)
{	
   // Declare common variables used within all tests in this test group.
   const float host_speed_stationary = 0.0F;
   const float host_speed_fwd = 5.0F;
   const float host_speed_fwd_fast = 25.0F;
   const float min_aliased_rdot = -10.0F;
   const float vun = 30.0F;
   const float curvi_lat_pos = 0.0F;
};

/** \purpose  
 * Given a detection from a stationary target in front of the host vehicle, check that the detection is given the expected priority.
 * \req
 * NA
 */
TEST(f360_detection_priority, Stationary_Detection_In_Front)
{
   /** \precond
    * For each test, set up detection and host properties
    */
	
   
   /** \action
    * Call Detection_Priority() for scenarios with different host speed; stationary, slow moving, and fast moving.
    */
   float range = 10.0F;
   float azimuth = 0.0F;
   float range_rate = 0.0F;
   float priority_stationary_host = Detection_Priority(range, azimuth, range_rate, host_speed_stationary, min_aliased_rdot, vun, curvi_lat_pos);
   
   range = 10.0F;
   azimuth = 0.0F;
   range_rate = -host_speed_fwd;
   float priority_moving_host = Detection_Priority(range, azimuth, range_rate, host_speed_fwd, min_aliased_rdot, vun, curvi_lat_pos);

   range = 10.0F;
   azimuth = 0.0F;
   range_rate = -host_speed_fwd_fast + vun;
   float priority_fast_moving_host = Detection_Priority(range, azimuth, range_rate, host_speed_fwd_fast, min_aliased_rdot, vun, curvi_lat_pos);

   range = 150.0F;
   azimuth = 0.0F;
   range_rate = 0.0F;
   float priority_stationary_host_far = Detection_Priority(range, azimuth, range_rate, host_speed_stationary, min_aliased_rdot, vun, curvi_lat_pos);

   /** \result
    * Check that priority is 1, ie. max priority from position and min priority from motion.
    */	
   DOUBLES_EQUAL(1.0F, priority_stationary_host, 0.001F);
   DOUBLES_EQUAL(1.0F, priority_moving_host, 0.001F);
   DOUBLES_EQUAL(1.0F, priority_fast_moving_host, 0.001F);
   DOUBLES_EQUAL(1.0F, priority_stationary_host_far, 0.001F);
}

/** \purpose  
 * Given a detection from a stationary target in behind the host vehicle, check that the detection is given the expected priority.
 * \req
 * NA
 */
TEST(f360_detection_priority, Stationary_Detection_In_Rear)
{
   /** \precond
    * For each test, set up detection and host properties
    */
	   
   /** \action
    * Call Detection_Priority() for scenarios with different host speed; stationary and fast moving.
    */
   float range = 50.0F;
   float azimuth = 3.14F;
   float range_rate = 0.0F;
   float priority_stationary_host = Detection_Priority(range, azimuth, range_rate, host_speed_stationary, min_aliased_rdot, vun, curvi_lat_pos);
   
   range = 50.0F;
   azimuth = 3.14F;
   range_rate = host_speed_fwd_fast - vun;
   float priority_fast_moving_host = Detection_Priority(range, azimuth, range_rate, host_speed_fwd_fast, min_aliased_rdot, vun, curvi_lat_pos);
   
   /** \result
    * Check that priority is approximately 0.75, for both cases. Check that the priority is very similar.
    */	
   DOUBLES_EQUAL(0.8F, priority_stationary_host, 0.05F);
   DOUBLES_EQUAL(0.8F, priority_fast_moving_host, 0.05F);
   DOUBLES_EQUAL(priority_stationary_host, priority_fast_moving_host, 0.0001F);
}


/** \purpose  
 * Given a detection from a moving target in front of the host vehicle, check that the detection is given the expected priority.
 * \req
 * NA
 */
TEST(f360_detection_priority, Moving_Detection_In_Front)
{
   /** \precond
    * For each test, set up detection and host properties
    */
	   
   /** \action
    * Call Detection_Priority() for scenarios with different host speed; stationary and fast moving.
    */
   float range = 50.0F;
   float azimuth = 0.0F;
   float range_rate = 5.0F;
   float priority_moving_target = Detection_Priority(range, azimuth, range_rate, host_speed_stationary, min_aliased_rdot, vun, curvi_lat_pos);
   
   range = 50.0F;
   azimuth = 0.0F;
   range_rate = 10.0F;
   float priority_fast_moving_target = Detection_Priority(range, azimuth, range_rate, host_speed_fwd_fast, min_aliased_rdot, vun, curvi_lat_pos);
   
   /** \result
    * Check that priority is as high as possible, for both cases.
    */	
   DOUBLES_EQUAL(2.0F, priority_moving_target, 0.001F);
   DOUBLES_EQUAL(2.0F, priority_fast_moving_target, 0.001F);
}

/** \purpose
 * Given a detection from a moving target far in front of the turning host vehicle, check that the detection is given the expected priority.
 * \req
 * NA
 */
TEST(f360_detection_priority, Moving_Detection_High_Curvi_Lat)
{
   /** \precond
    * For each test, set up detection and host properties
    * High curvi lateral position
    */

    /** \action
     * Call Detection_Priority() for scenarios with different host speed; stationary and fast moving.
     */
   float curvi_lat_pos = 5.0F;

   float range = 200.0F;
   float azimuth = 3.14F;
   float range_rate = 5.0F;
   float priority_moving_target = Detection_Priority(range, azimuth, range_rate, host_speed_stationary, min_aliased_rdot, vun, curvi_lat_pos);

   range = 200.0F;
   azimuth = 3.14F;
   range_rate = 10.0F;
   float priority_fast_moving_target = Detection_Priority(range, azimuth, range_rate, host_speed_fwd_fast, min_aliased_rdot, vun, curvi_lat_pos);

   /** \result
    * Check that priority is as high as possible, for both cases.
    */

   DOUBLES_EQUAL(1.0F, priority_moving_target, 0.001F);
   DOUBLES_EQUAL(1.0F, priority_fast_moving_target, 0.001F);
}

/** \purpose
 * Given a detection from a moving target far in front of the turning host vehicle, check that the detection is given the expected priority.
 * \req
 * NA
 */
TEST(f360_detection_priority, Moving_Detection_Low_Curvi_Lat)
{
   /** \precond
    * For each test, set up detection and host properties
    * Low curvi lateral position
    */

    /** \action
     * Call Detection_Priority() for scenarios with different host speed; stationary and fast moving.
     */
   float curvi_lat_pos = 0.0F;

   float range = 200.0F;
   float azimuth = 3.14F;
   float range_rate = 5.0F;
   float priority_moving_target = Detection_Priority(range, azimuth, range_rate, host_speed_stationary, min_aliased_rdot, vun, curvi_lat_pos);

   range = 200.0F;
   azimuth = 3.14F;
   range_rate = 10.0F;
   float priority_fast_moving_target = Detection_Priority(range, azimuth, range_rate, host_speed_fwd_fast, min_aliased_rdot, vun, curvi_lat_pos);

   /** \result
    * Check that priority is as high as possible, for both cases.
    */

   DOUBLES_EQUAL(1.8F, priority_moving_target, 0.001F);
   DOUBLES_EQUAL(1.8F, priority_fast_moving_target, 0.001F);
}
/** @}*/

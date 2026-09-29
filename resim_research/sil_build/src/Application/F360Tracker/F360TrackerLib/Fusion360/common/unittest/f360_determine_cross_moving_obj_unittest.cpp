/** \file
 * This file contains unit tests for content of f360_determine_cross_moving_obj.cpp file
 */

#include "f360_determine_cross_moving_obj.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_determine_cross_moving_obj
 *  @{
 */

/** \brief
 * Tests all criteria used to determine if the object is moving across the host path.
 */
TEST_GROUP(f360_determine_cross_moving_obj)
{	
   F360_Object_Track_T object;
   F360_Calibrations_T calib;
   
   /** \setup
    * Creates a simple track object with parameters under test.
    */

   TEST_SETUP()
   {
      object.speed = 2;
      object.vcs_velocity.lateral = -2;
      object.vcs_velocity.longitudinal = 0;
      object.vcs_position.y = 2;
      object.vcs_position.x = 50;
      Initialize_Tracker_Calibrations(calib);
   }

};

/** \purpose  
 * Tests if output is True when object under test satisfies the cross moving 
 * test criteria.
 * \req
 * NA.
 */
TEST(f360_determine_cross_moving_obj, f360_determine_cross_moving_obj__cross_moving)
{
   bool output = Is_Object_Cross_Moving(object, calib);

   CHECK_TRUE(output);
}

/** \purpose  
 * Tests if output is True when object under test satisfies the cross moving behind host
 * test criteria.
 * \req
 * NA.
 */
TEST(f360_determine_cross_moving_obj, f360_determine_cross_moving_obj__cross_moving_behind_host)
{
   object.vcs_position.x = -50;

   bool output = Is_Object_Cross_Moving(object, calib);

   CHECK_TRUE(output);
}

/** \purpose
 * Tests if output is False when object under test is moving nearly parallel to the host.
 * \req
 * NA.
 */
TEST(f360_determine_cross_moving_obj, f360_determine_cross_moving_obj__parallel)
{
   object.vcs_velocity.lateral = 0.01F;
   object.vcs_velocity.longitudinal = 2;

   bool output = Is_Object_Cross_Moving(object, calib);
   
   CHECK_FALSE(output);
}

/** \purpose
 * Tests if output is False when object under test is at a high VCS azimuth angle.
 * \req
 * NA.
 */
TEST(f360_determine_cross_moving_obj, f360_determine_cross_moving_obj__high_vcs_azimuth)
{
   /** \precond
    * Lateral and longitudinal position of the object give a high azimuth angle.
    */
   object.vcs_position.y = 30;
   object.vcs_position.x = 15;

   bool output = Is_Object_Cross_Moving(object, calib);

   CHECK_FALSE(output);
}
/** @}*/

/** \file
 * This file contains unit tests for content of f360_verify_object_size.cpp file
 */

#include "f360_verify_object_size.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_verify_object_size
 *  @{
 */

/** \brief
 *  Test group for testing Verify_Object_Size function.
 */
TEST_GROUP(f360_verify_object_size)
{
   // Initialize common variables used within all tests in f360_verify_object_size test group.
   F360_Object_Track_T object_track_to_keep = {};
   F360_Object_Track_T object_track_to_kill = {};
   F360_Calibrations_T calibs;
   F360_Dimensions_T dimensions = {};
   F360_Tracker_Info_T tracker_info = {};
   
   /** \setup
    * Set up two objects with
    *   - speeds above 5 m/s
    *   - time since split = -1 (i.e. have not split recently)
    * Set initial merged object dimensions to
    *   - width = 2 m
    *   - length = 15 m
    */
   TEST_SETUP()
   {
      // Initialize calibrations.
      Initialize_Tracker_Calibrations(calibs);

      object_track_to_keep.speed = 5.1F;
      object_track_to_keep.time_since_split = -1.0F;

      object_track_to_kill.speed = 5.2F;
      object_track_to_kill.time_since_split = -1.0F;

      dimensions.length = 15.0F;
      dimensions.width = 2.0F;
   }
};

   /** \purpose
    *  Test that when keep object is slow moving (speed below shrinking threshold) but shrink prevention due to
    *  variant K inside either zone is active, Verify_Object_Size returns true for dimensions that would otherwise
    *  exceed slow-moving limits (since width/length checks are skipped in that branch).
    */
   TEST(f360_verify_object_size, Verify_Object_Size_Slow_Moving_Variant_K_In_Zone_Prevents_Shrink_Returns_True)
   {
      /** \precond
       *  Use object_track_to_keep speed below shrinking threshold and place object inside zone1 bounds.
       *  Set variant type to K to activate shrink prevention. Keep merged dimensions above slow-moving thresholds
       *  so they would fail if shrink prevention not active. object_track_to_keep.x within (-40,25); y within |y|<22.
       */
      object_track_to_keep.speed = 2.0F; // below calibs.k_object_shrinking_speed_threshold (5 m/s)
      object_track_to_keep.vcs_position.x = 0.0F; // inside zone1 longitudinal bounds
      object_track_to_keep.vcs_position.y = 0.0F; // inside zone1 lateral bounds
      tracker_info.variant.type = F360_VARIANT_TYPE_K;

      // Dimensions chosen to exceed potential slow-moving max length/width thresholds so test demonstrates bypass.
      dimensions.length = calibs.k_max_length_for_slow_moving_objects + 1.0F;
      dimensions.width = calibs.k_movable_max_target_width + 0.5F;

      /** \action
       *  Call Verify_Object_Size().
       */
      bool f_test_passed = Verify_Object_Size(object_track_to_keep, object_track_to_kill, calibs, tracker_info, dimensions);

      /** \result
       *  Function shall return true (variant K in zone prevents shrink-based dimension limiting).
       */
      CHECK_TRUE(f_test_passed);
   }

/** \purpose
 *  Test that when both objects are fast moving (speed above 5 m/s) and have not been split recently, they
 *  are allowed to merge when the merged objects dimensions are below the thresholds for fast moving objects.
 */
TEST(f360_verify_object_size, Verify_Object_Size_Both_Obj_Fast_Moving_Dim_Below_Thresh)
{
   /** \precond
    * Test scenario is set up in the TEST_GROUP.
    */
  
   /** \action
    * call Verify_Object_Size().
    */
   bool f_test_passed = Verify_Object_Size(object_track_to_keep, object_track_to_kill, calibs, tracker_info, dimensions);
   
   /** \result
    * Function shall return true.
    */
   CHECK_TRUE(f_test_passed);
}

/** \purpose
 *  Test that when both objects are slow moving (speed below 5 m/s) they
 *  are not allowed to merge when the merged objects dimensions are abolw the thresholds for slow moving objects.
 */
TEST(f360_verify_object_size, Verify_Object_Size_Both_Obj_Slow_Moving_Dim_Above_Thresh)
{
   /** \precond
    * Test scenario is set up in the TEST_GROUP.
    * Set speeds of both objects below slow moving threshold (5 m/s)
    */
   object_track_to_keep.speed = 2.7F;
   object_track_to_kill.speed = 2.9F;
  
   /** \action
    * call Verify_Object_Size().
    */
   bool f_test_passed = Verify_Object_Size(object_track_to_keep, object_track_to_kill, calibs, tracker_info, dimensions);
   
   /** \result
    * Function shall return false.
    */
   CHECK_FALSE(f_test_passed);
}

/** \purpose
 *  Test that when only the keep object is slow moving, they are not allowed to merge when
 *  the merged object dimensions are above the thresholds for fast moving objects.
 */
TEST(f360_verify_object_size, Verify_Object_Size_Keep_Obj_Slow_Dim_Below_Thresh)
{
   /** \precond
    * Test scenario is set up in the TEST_GROUP.
    * Set speeds of the keep object below slow moving threshold (5 m/s)
    */
   object_track_to_keep.speed = 2.7F;
  
   /** \action
    * call Verify_Object_Size().
    */
   bool f_test_passed = Verify_Object_Size(object_track_to_keep, object_track_to_kill, calibs, tracker_info, dimensions);
   
   /** \result
    * Function shall return false.
    */
   CHECK_FALSE(f_test_passed);
}

/** \purpose
 *  Test that when only the kill object is slow moving, they are allowed to merge when
 *  the merged object dimensions are below the thresholds for fast moving objects.
 */
TEST(f360_verify_object_size, Verify_Object_Size_Kill_Obj_Slow_Dim_Below_Thresh)
{
   /** \precond
    * Test scenario is set up in the TEST_GROUP.
    * Set speed of the kill object below slow moving threshold (5 m/s)
    */
   object_track_to_kill.speed = 2.7F;
  
   /** \action
    * call Verify_Object_Size().
    */
   bool f_test_passed = Verify_Object_Size(object_track_to_keep, object_track_to_kill, calibs, tracker_info, dimensions);
   
   /** \result
    * Function shall return true.
    */
   CHECK_TRUE(f_test_passed);
}

/** \purpose
 *  Test that when both objects are fast moving, they are not allowed to merge if the total merged
 *  object length is above the length threshold for fast moving objects.
 */
TEST(f360_verify_object_size, Verify_Object_Size_Both_Obj_Fast_Moving_Len_Above_Thresh)
{
   /** \precond
    * Test scenario is set up in the TEST_GROUP.
    * Set merged object length above the allowed threshold
    */
   dimensions.length = calibs.k_fast_movable_max_target_length + 3.0F + 0.001F;
  
   /** \action
    * call Verify_Object_Size().
    */
   bool f_test_passed = Verify_Object_Size(object_track_to_keep, object_track_to_kill, calibs, tracker_info, dimensions);
   
   /** \result
    * Function shall return false.
    */
   CHECK_FALSE(f_test_passed);
}

/** \purpose
 *  Test that when both objects are fast moving, they are not allowed to merge if the total merged
 *  object width is above the width threshold.
 */
TEST(f360_verify_object_size, Verify_Object_Size_Both_Obj_Fast_Moving_Width_Above_Thresh)
{
   /** \precond
    * Test scenario is set up in the TEST_GROUP.
    * Set merged object width above the allowed threshold
    */
   dimensions.width = 3.91F;
  
   /** \action
    * call Verify_Object_Size().
    */
   bool f_test_passed = Verify_Object_Size(object_track_to_keep, object_track_to_kill, calibs, tracker_info, dimensions);
   
   /** \result
    * Function shall return false.
    */
   CHECK_FALSE(f_test_passed);
}

/** \purpose
 *  Test that when both objects are fast moving, they are allowed to merge if the total merged
 *  object width is above the width threshold and both objects have been split recently.
 */
TEST(f360_verify_object_size, Verify_Object_Size_Both_Obj_Fast_Moving_Recently_Split_Width_Above_Thresh)
{
   /** \precond
    * Test scenario is set up in the TEST_GROUP.
    * Set merged object width above the allowed threshold
    * Set both object's time since split to a positive value, indicating that they have both been involved in a split recently.
    */
   object_track_to_keep.time_since_split = 1.0F;
   object_track_to_kill.time_since_split = 0.5F;
   dimensions.width = 3.91F;
  
   /** \action
    * call Verify_Object_Size().
    */
   bool f_test_passed = Verify_Object_Size(object_track_to_keep, object_track_to_kill, calibs, tracker_info, dimensions);
   
   /** \result
    * Function shall return true.
    */
   CHECK_TRUE(f_test_passed);
}

/** \purpose
 *  Test that when both objects are fast moving, they are not allowed to merge if the total merged
 *  object width is above the width threshold and only the keep object has been split recently.
 */
TEST(f360_verify_object_size, Verify_Object_Size_Both_Obj_Fast_Moving_Keep_Recently_Split_Width_Above_Thresh)
{
   /** \precond
    * Test scenario is set up in the TEST_GROUP.
    * Set merged object width above the allowed threshold
    * Set both object's time since split to a positive value, indicating that they have both been involved in a split recently.
    */
   object_track_to_keep.time_since_split = 1.0F;
   dimensions.width = 3.91F;
  
   /** \action
    * call Verify_Object_Size().
    */
   bool f_test_passed = Verify_Object_Size(object_track_to_keep, object_track_to_kill, calibs, tracker_info, dimensions);
   
   /** \result
    * Function shall return false.
    */
   CHECK_FALSE(f_test_passed);
}

/** \purpose
 *  Test that when both objects are fast moving, they are not allowed to merge if the total merged
 *  object width is above the width threshold and only the kill object has been split recently.
 */
TEST(f360_verify_object_size, Verify_Object_Size_Both_Obj_Fast_Moving_Kill_Recently_Split_Width_Above_Thresh)
{
   /** \precond
    * Test scenario is set up in the TEST_GROUP.
    * Set merged object width above the allowed threshold
    * Set both object's time since split to a positive value, indicating that they have both been involved in a split recently.
    */
   object_track_to_kill.time_since_split = 1.0F;
   dimensions.width = 3.91F;
  
   /** \action
    * call Verify_Object_Size().
    */
   bool f_test_passed = Verify_Object_Size(object_track_to_keep, object_track_to_kill, calibs, tracker_info, dimensions);
   
   /** \result
    * Function shall return false.
    */
   CHECK_FALSE(f_test_passed);
}

/** \purpose
 *  Test checks whether function returns false when both objects are slow moving (below 5m/s)
 *  and object width is above threshold for that object type.
 */
TEST(f360_verify_object_size, Verify_Object_Size_Max_Object_Width_Is_Above_Thr_And_Slow_Moving_Objects)
{
   /** \precond
    * Setup estimated dimension values and set object speeds to below 5m/s.
    */
   dimensions.width = calibs.k_movable_max_target_width + 0.1F;
   dimensions.length = calibs.k_slow_movable_max_target_length - 0.1F;
   object_track_to_keep.speed= 4.9F;
   object_track_to_kill.speed = 4.8F;

   /** \action
    * call Verify_Object_Size().
    */
   bool f_test_passed = Verify_Object_Size(object_track_to_keep, object_track_to_kill, calibs, tracker_info, dimensions);
   
   /** \result
    * Function shall return false.
    */
   CHECK_FALSE(f_test_passed);
}

/** \purpose
 *  Test checks whether function returns false when both objects are slow moving, 
 *  but estimated length is above threshold.
 */
TEST(f360_verify_object_size, Verify_Object_Size_Max_Object_Length_Above_Threshold_For_Slow_Moving_Objects)
{
   /** \precond
    *  Setup estimated dimension values and set both object soeeds to below 5m/s
    */
   dimensions.width = calibs.k_movable_max_target_width - 0.1F;
   dimensions.length = 6.6F;
   object_track_to_keep.speed = 4.8F;
   object_track_to_kill.speed = 4.9F;
   
   /** \action
    * call Verify_Object_Size().
    */
   bool f_test_passed = Verify_Object_Size(object_track_to_keep, object_track_to_kill, calibs, tracker_info, dimensions);

   /** \result
    * Function shall return false.
    */
   CHECK_FALSE(f_test_passed);
}
/** @}*/

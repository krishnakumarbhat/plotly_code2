/** \file
 * This file contains unit tests for content of f360_detection_drop_countermeasure.cpp file
 */

#include "f360_detection_drop_countermeasure.h"
#include <CppUTest/TestHarness.h>
#include "f360_constants.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_detection_drop_countermeasure
 *  @{
 */

/** \brief
 * Tests for Flag_Object_Suspectable_For_Detection_Drop_Variant_K function which sets the detection drop flag
 * on objects based on variant type, motion status, speed, position zone, status, and heading.
 */
TEST_GROUP(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests)
{
   // Declare common variables used within all tests in this test group.
   F360_Tracker_Info_T tracker_info;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   float32_t host_speed;

   /** \setup
    * Set up default scenario with variant K, one active object with conditions that would set the flag.
    */
   TEST_SETUP()
   {
      // Initialize arrays to zero
      memset(&tracker_info, 0, sizeof(tracker_info));
      memset(object_tracks, 0, sizeof(object_tracks));

      // Set up tracker info with variant K
      tracker_info.variant.type = F360_Tracker_Variant_T::F360_VARIANT_TYPE_K;
      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1; // Point to object_tracks[0] (ID is 1-indexed)

      // Set up host speed below threshold
      host_speed = 5.0F; // Below 8.33 m/s (30 km/h) threshold

      // Set up a default object that meets all conditions for detection drop
      object_tracks[0].f_moving = true;
      object_tracks[0].speed = 5.0F; // Below 8.33 m/s threshold
      object_tracks[0].vcs_position.x = 10.0F; // Within +/-20m
      object_tracks[0].vcs_position.y = 5.0F; // Within +/-10m
      object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
      object_tracks[0].vcs_heading.Value(F360_DEG2RAD(5.0F)); // Within +/-20 deg or outside +/-160 deg
      object_tracks[0].bbox.Set_Length(2.0F); // Below 2.5m threshold
      object_tracks[0].bbox.Set_Width(1.0F); // Below 1.5m threshold
      object_tracks[0].f_suspectable_for_det_drop = false;
   }

   /** \teardown
    * No specific teardown needed for these tests.
    */
   TEST_TEARDOWN()
   {
   }

   // Helper function to create an object with all conditions met
   void Setup_Object_With_All_Conditions_Met(int32_t idx)
   {
      object_tracks[idx].f_moving = true;
      object_tracks[idx].speed = 5.0F;
      object_tracks[idx].vcs_position.x = 10.0F;
      object_tracks[idx].vcs_position.y = 5.0F;
      object_tracks[idx].status = F360_OBJECT_STATUS_UPDATED;
      object_tracks[idx].vcs_heading.Value(F360_DEG2RAD(10.0F));
      object_tracks[idx].bbox.Set_Length(2.0F);
      object_tracks[idx].bbox.Set_Width(1.0F);
      object_tracks[idx].f_suspectable_for_det_drop = false;
   }
};

/** \purpose
 * Verify that the flag is set to true when all conditions are met for variant K.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, All_Conditions_Met_Flag_Set_True)
{
   /** \precond
    * Default setup has all conditions met.
    */

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to true.
    */
   CHECK_TRUE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to false when object is not moving.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Not_Moving_Flag_Set_False)
{
   /** \precond
    * Set f_moving to false.
    */
   object_tracks[0].f_moving = false;

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to false.
    */
   CHECK_FALSE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to false when speed is at the boundary (8.33 m/s).
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Speed_At_Boundary_Flag_Set_False)
{
   /** \precond
    * Set speed to exactly 8.33 m/s.
    */
   object_tracks[0].speed = 8.33F;

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to false.
    */
   CHECK_FALSE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to false when speed is above threshold.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Speed_Above_Threshold_Flag_Set_False)
{
   /** \precond
    * Set speed above 8.33 m/s.
    */
   object_tracks[0].speed = 10.0F;

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to false.
    */
   CHECK_FALSE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to true when speed is negative (reverse direction) and below threshold magnitude.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Negative_Speed_Below_Threshold_Flag_Set_True)
{
   /** \precond
    * Set speed to negative value with magnitude below threshold.
    */
   object_tracks[0].speed = -5.0F;

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to true (abs value is used).
    */
   CHECK_TRUE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to false when object status is neither COASTED nor UPDATED.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Invalid_Status_Flag_Set_False)
{
   /** \precond
    * Set status to something other than COASTED or UPDATED.
    */
   object_tracks[0].status = F360_OBJECT_STATUS_NEW;

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to false.
    */
   CHECK_FALSE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to true when status is COASTED.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Coasted_Status_Flag_Set_True)
{
   /** \precond
    * Set status to COASTED.
    */
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to true.
    */
   CHECK_TRUE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to false when heading is at the 20 deg boundary.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Heading_At_20_Deg_Boundary_Flag_Set_False)
{
   /** \precond
    * Set heading to exactly 20 deg.
    */
   object_tracks[0].vcs_heading.Value(F360_DEG2RAD(20.0F));

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to false.
    */
   CHECK_FALSE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to false when heading is between 20 deg and 160 deg.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Heading_Between_20_And_160_Deg_Flag_Set_False)
{
   /** \precond
    * Set heading to 90 deg (between 20 deg and 160 deg).
    */
   object_tracks[0].vcs_heading.Value(F360_DEG2RAD(90.0F));

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to false.
    */
   CHECK_FALSE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to false when heading is at the 160 deg boundary.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Heading_At_160_Deg_Boundary_Flag_Set_False)
{
   /** \precond
    * Set heading to exactly 160 deg.
    */
   object_tracks[0].vcs_heading.Value(F360_DEG2RAD(160.0F));

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to false.
    */
   CHECK_FALSE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to true when heading is greater than 160 deg.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Heading_Above_160_Deg_Flag_Set_True)
{
   /** \precond
    * Set heading to 170 deg (greater than 160 deg).
    */
   object_tracks[0].vcs_heading.Value(F360_DEG2RAD(170.0F));

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to true.
    */
   CHECK_TRUE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to true when heading is negative and within -20 deg.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Negative_Heading_Within_20_Deg_Flag_Set_True)
{
   /** \precond
    * Set heading to -15 deg (abs value < 20 deg).
    */
   object_tracks[0].vcs_heading.Value(F360_DEG2RAD(-15.0F));

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to true.
    */
   CHECK_TRUE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to true when heading is negative and beyond -160 deg.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Negative_Heading_Beyond_160_Deg_Flag_Set_True)
{
   /** \precond
    * Set heading to -170 deg (abs value > 160 deg).
    */
   object_tracks[0].vcs_heading.Value(F360_DEG2RAD(-170.0F));

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to true.
    */
   CHECK_TRUE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the function handles multiple active objects correctly.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Multiple_Objects_Mixed_Conditions)
{
   /** \precond
    * Set up three active objects with different conditions.
    */
   tracker_info.num_active_objs = 3;
   tracker_info.active_obj_ids[0] = 1; // Point to object_tracks[0]
   tracker_info.active_obj_ids[1] = 2; // Point to object_tracks[1]
   tracker_info.active_obj_ids[2] = 3; // Point to object_tracks[2]

   // Object 0: All conditions met
   Setup_Object_With_All_Conditions_Met(0);

   // Object 1: Speed too high
   Setup_Object_With_All_Conditions_Met(1);
   object_tracks[1].speed = 10.0F;

   // Object 2: Not moving
   Setup_Object_With_All_Conditions_Met(2);
   object_tracks[2].f_moving = false;

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that only object 0 has flag set to true.
    */
   CHECK_TRUE(object_tracks[0].f_suspectable_for_det_drop);
   CHECK_FALSE(object_tracks[1].f_suspectable_for_det_drop);
   CHECK_FALSE(object_tracks[2].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag resets to false when conditions are not met (flag was previously true).
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Flag_Reset_When_Conditions_Not_Met)
{
   /** \precond
    * Pre-set flag to true, then make object not moving.
    */
   object_tracks[0].f_suspectable_for_det_drop = true;
   object_tracks[0].f_moving = false;

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is now false.
    */
   CHECK_FALSE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to false when host speed is at the boundary (8.33 m/s).
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Host_Speed_At_Boundary_Flag_Set_False)
{
   /** \precond
    * Set host speed to exactly 8.33 m/s.
    */
   host_speed = 8.33F;

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to false.
    */
   CHECK_FALSE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to false when host speed is above threshold.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Host_Speed_Above_Threshold_Flag_Set_False)
{
   /** \precond
    * Set host speed above 8.33 m/s.
    */
   host_speed = 10.0F;

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to false.
    */
   CHECK_FALSE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to false when object length is at the boundary (2.5m).
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Object_Length_At_Boundary_Flag_Set_False)
{
   /** \precond
    * Set object length to exactly 2.5m.
    */
   object_tracks[0].bbox.Set_Length(2.5F);

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to false.
    */
   CHECK_FALSE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to false when object length is above threshold.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Object_Length_Above_Threshold_Flag_Set_False)
{
   /** \precond
    * Set object length above 2.5m.
    */
   object_tracks[0].bbox.Set_Length(3.0F);

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to false.
    */
   CHECK_FALSE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to false when object width is at the boundary (1.5m).
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Object_Width_At_Boundary_Flag_Set_False)
{
   /** \precond
    * Set object width to exactly 1.5m.
    */
   object_tracks[0].bbox.Set_Width(1.5F);

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to false.
    */
   CHECK_FALSE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \purpose
 * Verify that the flag is set to false when object width is above threshold.
 * \req
 * NA
 */
TEST(Flag_Object_Suspectable_For_Detection_Drop_Variant_K_Tests, Object_Width_Above_Threshold_Flag_Set_False)
{
   /** \precond
    * Set object width above 1.5m.
    */
   object_tracks[0].bbox.Set_Width(2.0F);

   /** \action
    * Call Flag_Object_Suspectable_For_Detection_Drop_Variant_K().
    */
   Flag_Object_Suspectable_For_Detection_Drop_Variant_K(tracker_info, host_speed, object_tracks);

   /** \result
    * Check that f_suspectable_for_det_drop is set to false.
    */
   CHECK_FALSE(object_tracks[0].f_suspectable_for_det_drop);
}

/** \brief
 * Tests for Is_Object_Within_Detection_Drop_Zone function which checks if an object is within
 * the detection drop zone based on position and behind_sep_id.
 */
TEST_GROUP(Is_Object_Within_Detection_Drop_Zone_Tests)
{
   F360_Object_Track_T obj_trk;

   /** \setup
    * Set up default scenario with object in zone.
    */
   TEST_SETUP()
   {
      // Initialize object to zero
      memset(&obj_trk, 0, sizeof(obj_trk));

      // Set up default object position within the zone
      obj_trk.vcs_position.x = 10.0F; // Within +/-20m
      obj_trk.vcs_position.y = 5.0F; // Within +/-10m
      obj_trk.behind_sep_id = F360_INVALID_UNSIGNED_ID;
   }

   /** \teardown
    * No specific teardown needed.
    */
   TEST_TEARDOWN()
   {
   }
};

/** \purpose
 * Verify that object is in zone when position is within boundaries.
 * \req
 * NA
 */
TEST(Is_Object_Within_Detection_Drop_Zone_Tests, Object_In_Zone)
{
   /** \precond
    * Object at (10, 5), within boundaries.
    */

   /** \action
    * Call Is_Object_Within_Detection_Drop_Zone().
    */
   bool result = Is_Object_Within_Detection_Drop_Zone(obj_trk);

   /** \result
    * Check that result is true.
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Verify that object is not in zone when x position is outside boundary (positive).
 * \req
 * NA
 */
TEST(Is_Object_Within_Detection_Drop_Zone_Tests, Object_Outside_X_Positive_Boundary)
{
   /** \precond
    * Set x position to 20.1m (outside +/-20m).
    */
   obj_trk.vcs_position.x = 20.1F;

   /** \action
    * Call Is_Object_Within_Detection_Drop_Zone().
    */
   bool result = Is_Object_Within_Detection_Drop_Zone(obj_trk);

   /** \result
    * Check that result is false.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that object is not in zone when x position is outside boundary (negative).
 * \req
 * NA
 */
TEST(Is_Object_Within_Detection_Drop_Zone_Tests, Object_Outside_X_Negative_Boundary)
{
   /** \precond
    * Set x position to -20.1m (outside +/-20m).
    */
   obj_trk.vcs_position.x = -20.1F;

   /** \action
    * Call Is_Object_Within_Detection_Drop_Zone().
    */
   bool result = Is_Object_Within_Detection_Drop_Zone(obj_trk);

   /** \result
    * Check that result is false.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that object is in zone when x position is at the boundary (19.9m).
 * \req
 * NA
 */
TEST(Is_Object_Within_Detection_Drop_Zone_Tests, Object_At_X_Positive_Boundary)
{
   /** \precond
    * Set x position to 19.9m (just inside boundary).
    */
   obj_trk.vcs_position.x = 19.9F;

   /** \action
    * Call Is_Object_Within_Detection_Drop_Zone().
    */
   bool result = Is_Object_Within_Detection_Drop_Zone(obj_trk);

   /** \result
    * Check that result is true.
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Verify that object is not in zone when y position is outside boundary (positive).
 * \req
 * NA
 */
TEST(Is_Object_Within_Detection_Drop_Zone_Tests, Object_Outside_Y_Positive_Boundary)
{
   /** \precond
    * Set y position to 10.1m (outside +/-10m).
    */
   obj_trk.vcs_position.y = 10.1F;

   /** \action
    * Call Is_Object_Within_Detection_Drop_Zone().
    */
   bool result = Is_Object_Within_Detection_Drop_Zone(obj_trk);

   /** \result
    * Check that result is false.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that object is not in zone when y position is outside boundary (negative).
 * \req
 * NA
 */
TEST(Is_Object_Within_Detection_Drop_Zone_Tests, Object_Outside_Y_Negative_Boundary)
{
   /** \precond
    * Set y position to -10.1m (outside +/-10m).
    */
   obj_trk.vcs_position.y = -10.1F;

   /** \action
    * Call Is_Object_Within_Detection_Drop_Zone().
    */
   bool result = Is_Object_Within_Detection_Drop_Zone(obj_trk);

   /** \result
    * Check that result is false.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that object is in zone when y position is at the boundary (9.9m).
 * \req
 * NA
 */
TEST(Is_Object_Within_Detection_Drop_Zone_Tests, Object_At_Y_Positive_Boundary)
{
   /** \precond
    * Set y position to 9.9m (just inside boundary).
    */
   obj_trk.vcs_position.y = 9.9F;

   /** \action
    * Call Is_Object_Within_Detection_Drop_Zone().
    */
   bool result = Is_Object_Within_Detection_Drop_Zone(obj_trk);

   /** \result
    * Check that result is true.
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Verify that object at exactly zero lateral position is handled correctly.
 * \req
 * NA
 */
TEST(Is_Object_Within_Detection_Drop_Zone_Tests, Object_At_Zero_Lateral_Position)
{
   /** \precond
    * Object at y=0.0m (on centerline).
    */
   obj_trk.vcs_position.y = 0.0F;

   /** \action
    * Call Is_Object_Within_Detection_Drop_Zone().
    */
   bool result = Is_Object_Within_Detection_Drop_Zone(obj_trk);

   /** \result
    * Check that result is true (within boundaries).
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Verify that object at extreme positions within boundaries is handled correctly.
 * \req
 * NA
 */
TEST(Is_Object_Within_Detection_Drop_Zone_Tests, Object_At_Extreme_Positions_Within_Boundaries)
{
   /** \precond
    * Object at x=19.9m, y=9.9m (near boundaries but inside).
    */
   obj_trk.vcs_position.x = 19.9F;
   obj_trk.vcs_position.y = 9.9F;

   /** \action
    * Call Is_Object_Within_Detection_Drop_Zone().
    */
   bool result = Is_Object_Within_Detection_Drop_Zone(obj_trk);

   /** \result
    * Check that result is true.
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Verify that object at extreme positions outside boundaries is handled correctly.
 * \req
 * NA
 */
TEST(Is_Object_Within_Detection_Drop_Zone_Tests, Object_At_Extreme_Positions_Outside_Boundaries)
{
   /** \precond
    * Object at x=20.1m, y=9.9m (x outside boundary).
    */
   obj_trk.vcs_position.x = 20.1F;
   obj_trk.vcs_position.y = 9.9F;

   /** \action
    * Call Is_Object_Within_Detection_Drop_Zone().
    */
   bool result = Is_Object_Within_Detection_Drop_Zone(obj_trk);

   /** \result
    * Check that result is false.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that object with behind_sep_id != INVALID is not considered.
 * \req
 * NA
 */
TEST(Is_Object_Within_Detection_Drop_Zone_Tests, Object_With_behind_sep_id_set)
{
   /** \precond
    * Object behind_sep_id != 0.
    */
   obj_trk.behind_sep_id = 1; // Set behind_sep_id to a valid value

   /** \action
    * Call Is_Object_Within_Detection_Drop_Zone().
    */
   bool result = Is_Object_Within_Detection_Drop_Zone(obj_trk);

   /** \result
    * Check that result is false.
    */
   CHECK_FALSE(result);
}

/** @}*/

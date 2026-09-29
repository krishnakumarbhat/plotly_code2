/** \file
 * This file contains unit tests for content of f360_update_extended_bbox_offsets_for_object_in_dead_zone.cpp file
 */

#include "f360_update_extended_bbox_offsets_for_object_in_dead_zone.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_extend_assoc_gates_of_object_in_dead_zone
 *  @{
 */

 /** \brief
  * Test group of Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone. Tests verify whether object associaiton gates are
  * properly increased when object is in dead zone and meets all conditions.
  */
TEST_GROUP(f360_update_extended_bbox_offsets_for_object_in_dead_zone)
{
   F360_Calibrations_T calib{};
   float32_t host_speed{};
   Dead_Zone_T dead_zone{};
   F360_Object_Track_T object{};
   float32_t long_buffer1{};
   float32_t long_buffer2{};

   /** \setup
    * Initialize tracker calibrations
    * Set up initial host speed
    * Set up dead zone parameters
    * Set up object length
    * Set up object parameters to meet all conditions
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      host_speed = 15.0F;

      dead_zone.basic.lower = -5.0F;
      dead_zone.basic.upper = 0.0F;

      dead_zone.extended.lower = dead_zone.basic.lower - calib.k_dead_zone_long_limit_extension;
      dead_zone.extended.upper = dead_zone.basic.upper + calib.k_dead_zone_long_limit_extension;

      object.bbox.Set_Length(2.0F);

      object.dead_zone_status = F360_Dead_Zone_Status_T::INSIDE;
      object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

      object.on_sep_id = F360_INVALID_UNSIGNED_ID;
      object.behind_sep_id = F360_INVALID_UNSIGNED_ID;

      object.vcs_position.x = -2.5F;
      object.vcs_position.y = -2.0F;
      object.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = {-2.5F,-2.0F};
      object.bbox.Set_Center(center);
      object.speed = host_speed;
   }
};

/** \purpose
 * Purpose of this test is to verify whether when object meets all conditions, is inside dead zone its association gates are properly updated.
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Of_Object_In_Dead_Zone__All_Conditions_Met_Assoc_Gates_Are_Updated)
{
   /** \precond
    * All is set in TEST_SETUP()
    */

    /** \action
     * Call tested function
     */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
    * Check whether long_buffer1 and long_buffer2 were increased to 2.5F both
    */
   DOUBLES_EQUAL(2.5F, long_buffer1, F360_EPSILON);
   DOUBLES_EQUAL(2.5F, long_buffer2, F360_EPSILON);
}


/** \purpose
 * Purpose of this test is to verify whether when object meets all conditions, is inside dead zone its association gates are not updated for a slow moving CCA object
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Of_Object_In_Dead_Zone__All_Conditions_Met_Assoc_Gates_Are_Updated_Slow_Moving_CCA)
{
   /** \precond
    * Set object filter to CCA and speed to just below calib.fast_moving_thres. 
    * Adjust host speed to be equal to object speed
    */
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object.speed = calib.fast_moving_thresh - 1e-3F;
   host_speed = object.speed; 

    /** \action
     * Call tested function
     */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
    * Check whether long_buffer1 and long_buffer2 are 0
    */
   DOUBLES_EQUAL(0.0F, long_buffer1, F360_EPSILON);
   DOUBLES_EQUAL(0.0F, long_buffer2, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether when object meets all conditions, is inside dead zone its association gates are properly updated for a fast moving CCA object
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Of_Object_In_Dead_Zone__All_Conditions_Met_Assoc_Gates_Are_Updated_Fast_Moving_CCA)
{
   /** \precond
    * Set object filter to CCA and speed to just above calib.fast_moving_thres. 
    * Adjust host speed to be equal to object speed
    */
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object.speed = calib.fast_moving_thresh + 1e-3F;
   host_speed = object.speed; 

    /** \action
     * Call tested function
     */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
    * Check whether long_buffer1 and long_buffer2 were increased to 2.5F both
    */
   DOUBLES_EQUAL(0.5001667F, long_buffer1, F360_EPSILON);
   DOUBLES_EQUAL(0.5001667F, long_buffer2, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether when object meets all conditions, is in front of host its association gates are properly updated.
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Of_Object_In_Dead_Zone__All_Conditions_Met_Is_In_Front_Of_Host_Assoc_Gates_Are_Updated)
{
   /** \precond
    * All is set in TEST_SETUP()
    * Set up object vcs longitudinal positon to be in front of host
    * Set up object DEAD_ZONE_STATUS to be qual to F360_Dead_Zone_Status_T::IN_FRONT;
    */
   Point center = {0.8F,0.0F};
   object.bbox.Set_Center(center);
   object.dead_zone_status = F360_Dead_Zone_Status_T::IN_FRONT;

   /** \action
    * Call tested function
    */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
    * Check whether long_buffer1 was incresed to 5.8F
    * Check whether long_buffer2 was increased to 2.2F
    */
   DOUBLES_EQUAL(5.8F, long_buffer1, F360_EPSILON);
   DOUBLES_EQUAL(2.2F, long_buffer2, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether when object meets all conditions, is entering front of host its association gates are properly updated.
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Of_Object_In_Dead_Zone__All_Conditions_Met_Is_Entering_Front_Of_Host_Assoc_Gates_Are_Updated)
{
   /** \precond
    * All is set in TEST_SETUP()
    * Set up object vcs longitudinal positon to be in front of host
    * Set up object DEAD_ZONE_STATUS to be qual to F360_Dead_Zone_Status_T::ENTERING_FRONT;
    */
   Point center = {1.8F,0.0F};
   object.bbox.Set_Center(center);
   object.dead_zone_status = F360_Dead_Zone_Status_T::ENTERING_FRONT;

   /** \action
    * Call tested function
    */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
    * Check whether long_buffer1 was incresed to 1.2F
    * Check whether long_buffer2 was increased to 1.8F
    */
   DOUBLES_EQUAL(1.8F, long_buffer1, F360_EPSILON);
   DOUBLES_EQUAL(1.2F, long_buffer2, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether when object meets all conditions, is in rear of host its association gates are properly updated.
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Of_Object_In_Dead_Zone__All_Conditions_Met_Is_In_Rear_Of_Host_Assoc_Gates_Are_Updated)
{
   /** \precond
    * All is set in TEST_SETUP()
    * Set up object vcs longitudinal positon to be in rear of host
    * Set up object DEAD_ZONE_STATUS to be qual to F360_Dead_Zone_Status_T::IN_REAR;
    */
   Point center = {-5.8F,0.0F};
   object.bbox.Set_Center(center);
   object.dead_zone_status = F360_Dead_Zone_Status_T::IN_REAR;

   /** \action
    * Call tested function
    */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
    * Check whether long_buffer1 was incresed to 2.2F
    * Check whether long_buffer2 was increased to 5.8F
    */
   DOUBLES_EQUAL(2.2F, long_buffer1, 1e-4);
   DOUBLES_EQUAL(5.8F, long_buffer2, 1e-4);
}

/** \purpose
 * Purpose of this test is to verify whether when object meets all conditions, is entering rear of host its association gates are properly updated.
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Of_Object_In_Dead_Zone__All_Conditions_Met_Is_Entering_Rear_Of_Host_Assoc_Gates_Are_Updated)
{
   /** \precond
    * All is set in TEST_SETUP()
    * Set up object vcs longitudinal positon to be in rear of host
    * Set up object DEAD_ZONE_STATUS to be qual to F360_Dead_Zone_Status_T::ENTERING_REAR;
    */
   Point center = {-6.8F,0.0F};
   object.bbox.Set_Center(center);
   object.dead_zone_status = F360_Dead_Zone_Status_T::ENTERING_REAR;

   /** \action
    * Call tested function
    */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
    * Check whether long_buffer1 was incresed to 1.2F
    * Check whether long_buffer2 was increased to 1.8F
    */
   DOUBLES_EQUAL(1.2F, long_buffer1, 1e-4);
   DOUBLES_EQUAL(1.8F, long_buffer2, 1e-4);
}

/** \purpose
 * Purpose of this test is to verify whether when object meets all conditions but its dead zone status is obsolete, its association gates are not modified
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Of_Object_In_Dead_Zone__All_Conditions_Met_Dead_Zone_Status_Obsolete_Assoc_Gates_Not_Modified)
{
   /** \precond
    * All is set in TEST_SETUP()
    * Set up object vcs longitudinal positon to be in rear of host
    * Set up object DEAD_ZONE_STATUS to be qual to F360_Dead_Zone_Status_T::LEAVING_REAR;
    */
   Point center = {-6.8F,0.0F};
   object.bbox.Set_Center(center);
   object.dead_zone_status = F360_Dead_Zone_Status_T::LEAVING_REAR;

   /** \action
    * Call tested function
    */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
    * Check whether long_buffer1 was not modified (is equal to 0.0F)
    * Check whether long_buffer2 was not modified (is equal to 0.0F)
    */
   DOUBLES_EQUAL(0.0F, long_buffer1, 1e-4);
   DOUBLES_EQUAL(0.0F, long_buffer2, 1e-4);
}

/** \purpose
 * Purpose of this test is to verify whether object association gates are not modified when its relative speed difference is too big
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Of_Object_In_Dead_Zone__Rel_Speed_Diff_Too_Big_Not_Updated)
{
   /** \precond
    * All is set in TEST_SETUP()
    * Set object speed to be equal to 2.0F * host_speed
    */
   object.speed = 2.0F * host_speed;

   /** \action
    * Call tested function
    */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
    * Check whether long_buffer1 and long_buffer2 were not modified (are equal to 0.0F)
    */
   DOUBLES_EQUAL(0.0F, long_buffer1, F360_EPSILON);
   DOUBLES_EQUAL(0.0F, long_buffer2, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether object association gates are not modified when its dead zone status is OUTSIDE
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Of_Object_In_Dead_Zone__Dead_Zone_Status_Outside)
{
   /** \precond
    * All is set in TEST_SETUP()
    * Set object DEAD ZONE status as OUTSIDE
    */
   object.dead_zone_status = F360_Dead_Zone_Status_T::OUTSIDE;

   /** \action
    * Call tested function
    */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
    * Check whether long_buffer1 and long_buffer2 were not modified (are equal to 0.0F)
    */
   DOUBLES_EQUAL(0.0F, long_buffer1, F360_EPSILON);
   DOUBLES_EQUAL(0.0F, long_buffer2, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether object association gates are not modified when its dead zone status is UNDEFINED
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Of_Object_In_Dead_Zone__Dead_Zone_Status_Undefined)
{
   /** \precond
    * All is set in TEST_SETUP()
    * Set object DEAD ZONE status as UNDEFINED
    */
   object.dead_zone_status = F360_Dead_Zone_Status_T::UNDEFINED;

   /** \action
    * Call tested function
    */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
    * Check whether long_buffer1 and long_buffer2 were not modified (are equal to 0.0F)
    */
   DOUBLES_EQUAL(0.0F, long_buffer1, F360_EPSILON);
   DOUBLES_EQUAL(0.0F, long_buffer2, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether object association gates are not modified when its on guardrail
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Of_Object_In_Dead_Zone__Track_On_Guardrail)
{
   /** \precond
    * All is set in TEST_SETUP()
    * Set object to be on some guardrail
    */
   object.on_sep_id = 1;

   /** \action
    * Call tested function
    */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
    * Check whether long_buffer1 and long_buffer2 were not modified (are equal to 0.0F)
    */
   DOUBLES_EQUAL(0.0F, long_buffer1, F360_EPSILON);
   DOUBLES_EQUAL(0.0F, long_buffer2, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether object association gates are not modified when its behind guardrail
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Of_Object_In_Dead_Zone__Track_Behind_Guardrail)
{
   /** \precond
    * All is set in TEST_SETUP()
    * Set object to be behind some guardrail
    */
   object.behind_sep_id = 2;

   /** \action
    * Call tested function
    */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
    * Check whether long_buffer1 and long_buffer2 were not modified (are equal to 0.0F)
    */
   DOUBLES_EQUAL(0.0F, long_buffer1, F360_EPSILON);
   DOUBLES_EQUAL(0.0F, long_buffer2, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify that saturation is applied when object is in ENTERING_FRONT dead zone status
 * and offsets are clamped to 4.0F when buffers already exceed the saturation limit.
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Saturate_When_Entering_Front_High_Speed)
{
   /** \precond
    * Set object to ENTERING_FRONT
    * Host speed high (15.0F) but saturation is triggered via pre-set buffers, not speed growth
    * Initialize buffers above saturation limit (10.0F) to force clamping
    */
   object.dead_zone_status = F360_Dead_Zone_Status_T::ENTERING_FRONT;
   long_buffer1 = 10.0F; // Larger than saturation limit (4.0F)
   long_buffer2 = 10.0F; // Larger than saturation limit (4.0F)

   /** \action
    * Call tested function
    */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
      * Check that offsets are capped to 4.0F in ENTERING_FRONT status
    */
   CHECK(long_buffer1 <= 4.0F);
   CHECK(long_buffer2 <= 4.0F);
}

/** \purpose
 * Purpose of this test is to verify saturation is not applied for any other status than ENTERING_FRONT.
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Not_Saturate_When_Status_Other_Than_Entering_Front)
{
   /** \precond
    * Set high host speed to potentially generate large offset values
    * Define array of statuses and their corresponding object positions
    * Initialize buffers to high value (10.0F) to detect if saturation caps them at 4.0F
    */
   host_speed = 15.0F;

   // Array of dead zone statuses to test
   F360_Dead_Zone_Status_T statuses[] = {
      F360_Dead_Zone_Status_T::INSIDE,
      F360_Dead_Zone_Status_T::IN_REAR,
      F360_Dead_Zone_Status_T::ENTERING_REAR,
      F360_Dead_Zone_Status_T::IN_FRONT
   };

   // Corresponding positions for each status
   float32_t positions[][2] = {
      {-2.5F, 0.0F},    // INSIDE
      {-5.8F, 0.0F},    // IN_REAR
      {-6.8F, 0.0F},    // ENTERING_REAR
      {0.8F, 0.0F},     // IN_FRONT
   };

   const uint32_t num_statuses = sizeof(statuses) / sizeof(statuses[0]);

   // Arrays to store output buffer values for each status
   float32_t buffers1[4];
   float32_t buffers2[4];

   /** \action
    * Loop through each dead zone status, initialize buffers high, and collect results
    */
   for (uint32_t i = 0U; i < num_statuses; ++i)
   {
      object.dead_zone_status = statuses[i];
      buffers1[i] = 10.0F;  // Initialize high to detect saturation capping at 4.0F
      buffers2[i] = 10.0F;  // Initialize high to detect saturation capping at 4.0F
      Point center = { positions[i][0], positions[i][1] };
      object.bbox.Set_Center(center);

      Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, buffers1[i], buffers2[i]);
   }

   /** \result
    * Verify that with any status other than ENTERING_FRONT saturation was not applied to buffers.
    */
   CHECK_TRUE_TEXT(buffers1[0] > 4.0F && buffers2[0] > 4.0F, "Buffer was saturated (and shouldn't be) for INSIDE status");
   CHECK_TRUE_TEXT(buffers1[1] > 4.0F && buffers2[1] > 4.0F, "Buffer was saturated (and shouldn't be) for IN_REAR status");
   CHECK_TRUE_TEXT(buffers1[2] > 4.0F && buffers2[2] > 4.0F, "Buffer was saturated (and shouldn't be) for ENTERING_REAR status");
   CHECK_TRUE_TEXT(buffers1[3] > 4.0F && buffers2[3] > 4.0F, "Buffer was saturated (and shouldn't be) for IN_FRONT status");
}

/** \purpose
 * Purpose of this test is to verify that saturation is applied when object is in ENTERING_FRONT dead zone status.
 * Buffers start below 4.0F, function calculates new values that would exceed 4.0F, and they are then capped to 4.0F.
 * \req
 * NA.
 */
TEST(f360_update_extended_bbox_offsets_for_object_in_dead_zone, Extend_Assoc_Gates_Saturate_When_Entering_Non_Saturated_Before)
{
   /** \precond
    * Set object to ENTERING_FRONT
    * Host speed high (15.0F) to generate large offset values
    * Set object position such that it is entering front
    * Initialize buffers below saturation limit (3.0F) before function call
    */
   object.dead_zone_status = F360_Dead_Zone_Status_T::ENTERING_FRONT;
   long_buffer1 = 3.0F; // Below saturation limit (4.0F)
   long_buffer2 = 3.0F; // Below saturation limit (4.0F)

   /** \action
    * Call tested function
    */
   Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calib, host_speed, dead_zone, object, long_buffer1, long_buffer2);

   /** \result
      * Check that offsets are capped to 4.0F in ENTERING_FRONT status
      * Buffers should not exceed 4.0F due to saturation limit
    */
   CHECK(long_buffer1 <= 4.0F);
   CHECK(long_buffer2 <= 4.0F);
}

/** @}*/
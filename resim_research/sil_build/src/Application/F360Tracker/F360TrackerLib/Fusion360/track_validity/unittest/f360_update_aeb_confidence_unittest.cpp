/** \file
 * This file contains unit tests for content of f360_update_aeb_confidence.cpp file
 */

#include "f360_update_aeb_confidence.h"
#include <CppUTest/TestHarness.h>
#include "f360_calibrations.h"
#include "f360_constants.h"
#include "f360_occlusion_types.h"

using namespace f360_variant_A;

/** \defgroup  f360_update_aeb_confidence
 *  @{
 */

/** \brief
 * Tests for Update_AEB_Confidence() function.
 */
TEST_GROUP(f360_update_aeb_confidence)
{
   F360_Host_T host = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};

   /** \setup
    * Set up one active object at index 0 with all conditions met for AEB_CONF_HIGH.
    * Host drives straight at 30 m/s, no sideslip, no yaw rate.
    */
   TEST_SETUP()
   {
      host.vcs_speed = 30.0F;
      host.vcs_sideslip = 0.0F;
      host.yaw_rate_rad = 0.0F;

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;

      F360_Object_Track_T& obj = object_tracks[0];
      obj.id = 1;
      obj.time_since_initialization = 3.0F;
      obj.exist_prob = 1.0F;
      obj.f_moving = true;
      obj.status = F360_OBJECT_STATUS_UPDATED;
      obj.underdrivable_status_ocg = ocg::OCG_Underdrivable_Status_T::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
      obj.drivable_status_sg = sg::SG_Drivability_Class_T::NONDRIVABLE;
      obj.vcs_heading.Value(0.0F);
      obj.confidenceLevel = 0.99F;
      obj.speed = 5.0F;
      obj.occlusion_status = F360_Occlusion_Status_T::OCCLUSION_STATUS_VISIBLE;
      obj.vcs_velocity.longitudinal = -10.0F;
      obj.vcs_velocity.lateral = 0.0F;
      obj.heading_rate = 0.0F;
      obj.vcs_position.Set_Position(50.0F, 0.0F);
      obj.bbox.Set_Length(4.5F);
      obj.bbox.Set_Width(1.8F);
      obj.bbox.Set_Orientation(Angle(0.0F));
      obj.reference_point = F360_REFERENCE_POINT_FRONT;
      obj.aeb_confidence = AEB_CONF_INVALID;
      obj.time_since_track_updated = 0.05F;
   }
};

/** \purpose
 * Verify that when all HIGH conditions are met, aeb_confidence increases
 * by one step from INVALID toward HIGH.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__All_High_Conditions_Met_Steps_Up_From_Invalid)
{
   /** \precond
    * Default setup: all HIGH conditions met, aeb_confidence starts at INVALID.
    */

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * aeb_confidence should increase by one step from INVALID (0) to LOW (1).
    */
   CHECK_EQUAL(AEB_CONF_LOW, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that repeated calls with HIGH conditions gradually ramp up
 * aeb_confidence from INVALID to HIGH through each intermediate level.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Ramp_Up_To_High_Over_Multiple_Calls)
{
   /** \precond
    * Default setup: all HIGH conditions met, aeb_confidence starts at INVALID.
    */

   /** \action
    * Call Update_AEB_Confidence four times.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);
   CHECK_EQUAL(AEB_CONF_LOW, object_tracks[0].aeb_confidence);

   Update_AEB_Confidence(host, tracker_info, object_tracks);
   CHECK_EQUAL(AEB_CONF_MEDIUM, object_tracks[0].aeb_confidence);

   Update_AEB_Confidence(host, tracker_info, object_tracks);
   CHECK_EQUAL(AEB_CONF_MEDIUM_HIGH, object_tracks[0].aeb_confidence);

   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * After four calls, aeb_confidence should reach HIGH.
    */
   CHECK_EQUAL(AEB_CONF_HIGH, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that aeb_confidence stays at HIGH when instant confidence equals
 * current confidence.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Stays_At_High_When_Already_High)
{
   /** \precond
    * aeb_confidence already at HIGH.
    */
   object_tracks[0].aeb_confidence = AEB_CONF_HIGH;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * aeb_confidence should remain HIGH.
    */
   CHECK_EQUAL(AEB_CONF_HIGH, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that aeb_confidence drops immediately to LOW when conditions
 * are no longer met (instant = LOW) while current is HIGH.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Drops_Immediately_To_Low)
{
   /** \precond
    * aeb_confidence at HIGH. Then violate a condition: set f_moving = false.
    */
   object_tracks[0].aeb_confidence = AEB_CONF_HIGH;
   object_tracks[0].f_moving = false;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * aeb_confidence should drop immediately to LOW (not step-wise down).
    */
   CHECK_EQUAL(AEB_CONF_LOW, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when time_since_initialization is below the HIGH threshold (2.0s)
 * but above the MEDIUM_HIGH threshold (1.5s), instant confidence is MEDIUM_HIGH.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Medium_High_When_Time_Since_Init_Below_High_Threshold)
{
   /** \precond
    * time_since_initialization = 1.8s (below 2.0 for HIGH, above 1.5 for MEDIUM_HIGH).
    * exist_prob = 0.96 (below 0.99 for HIGH, above 0.95 for MEDIUM_HIGH).
    */
   object_tracks[0].time_since_initialization = 1.8F;
   object_tracks[0].exist_prob = 0.96F;
   object_tracks[0].aeb_confidence = AEB_CONF_MEDIUM;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * Instant confidence is MEDIUM_HIGH. Current is MEDIUM, so it steps up to MEDIUM_HIGH.
    */
   CHECK_EQUAL(AEB_CONF_MEDIUM_HIGH, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when time_since_initialization is below the MEDIUM_HIGH threshold (1.5s)
 * but above the MEDIUM threshold (1.0s), instant confidence is MEDIUM.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Medium_When_Time_Since_Init_Below_Medium_High_Threshold)
{
   /** \precond
    * time_since_initialization = 1.2s (below 1.5 for MEDIUM_HIGH, above 1.0 for MEDIUM).
    * exist_prob = 0.93 (below 0.95 for MEDIUM_HIGH, above 0.92 for MEDIUM).
    * confidenceLevel = 0.93 (below 0.97 for MEDIUM_HIGH, above 0.92 for MEDIUM).
    * time_since_track_updated = 0.05 (below 0.11 for MEDIUM).
    */
   object_tracks[0].time_since_initialization = 1.2F;
   object_tracks[0].exist_prob = 0.93F;
   object_tracks[0].confidenceLevel = 0.93F;
   object_tracks[0].time_since_track_updated = 0.05F;
   object_tracks[0].aeb_confidence = AEB_CONF_LOW;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * Instant confidence is MEDIUM. Current is LOW, so it steps up to MEDIUM.
    */
   CHECK_EQUAL(AEB_CONF_MEDIUM, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when the object is not moving, instant confidence is LOW
 * regardless of other conditions being met.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Low_When_Object_Not_Moving)
{
   /** \precond
    * All conditions met except f_moving = false.
    */
   object_tracks[0].f_moving = false;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * Instant confidence is LOW. Starting from INVALID, steps up to LOW.
    */
   CHECK_EQUAL(AEB_CONF_LOW, object_tracks[0].aeb_confidence);

   /** \action
    * Call again - already at LOW, instant is LOW, should stay at LOW.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * aeb_confidence remains LOW.
    */
   CHECK_EQUAL(AEB_CONF_LOW, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when object status is not UPDATED, MEDIUM_HIGH and HIGH
 * confidence are not reached, so aeb_confidence cannot step up beyond MEDIUM
 * even when all other MEDIUM_HIGH conditions are met.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Not_Above_Medium_When_Status_Not_Updated)
{
   /** \precond
    * time_since_initialization = 1.8 (satisfies MEDIUM_HIGH time threshold).
    * exist_prob = 0.96 (satisfies MEDIUM_HIGH exist_prob threshold).
    * All other MEDIUM_HIGH conditions met.
    * Status = COASTED (MEDIUM_HIGH and HIGH require status == UPDATED; MEDIUM does not).
    * aeb_confidence starts at MEDIUM so a step-up to MEDIUM_HIGH would be visible.
    */
   object_tracks[0].time_since_initialization = 1.8F;
   object_tracks[0].exist_prob = 0.96F;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].aeb_confidence = AEB_CONF_MEDIUM;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * MEDIUM_HIGH fails because status != UPDATED. MEDIUM passes (no status check).
    * instant confidence is MEDIUM; current is MEDIUM, so aeb_confidence stays at MEDIUM.
    */
   CHECK_EQUAL(AEB_CONF_MEDIUM, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when object heading exceeds the HIGH/MEDIUM_HIGH threshold (15 deg)
 * but is within the MEDIUM threshold (25 deg), instant confidence is MEDIUM.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Medium_When_Heading_Between_15_And_25_Deg)
{
   /** \precond
    * Heading at 20 degrees (exceeds 15 deg limit for HIGH/MEDIUM_HIGH,
    * within 25 deg limit for MEDIUM).
    * time_since_track_updated < 0.11 for MEDIUM confidence level.
    */
   object_tracks[0].vcs_heading.Value(F360_DEG2RAD(20.0F));
   object_tracks[0].time_since_track_updated = 0.05F;
   object_tracks[0].aeb_confidence = AEB_CONF_LOW;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * Instant confidence is MEDIUM (heading within 25 deg). Steps up from LOW to MEDIUM.
    */
   CHECK_EQUAL(AEB_CONF_MEDIUM, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when object heading exceeds the MEDIUM threshold (25 deg),
 * instant confidence is LOW.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Low_When_Heading_Exceeds_25_Deg)
{
   /** \precond
    * Heading at 30 degrees (exceeds 25 deg limit for MEDIUM).
    */
   object_tracks[0].vcs_heading.Value(F360_DEG2RAD(30.0F));
   object_tracks[0].aeb_confidence = AEB_CONF_MEDIUM;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * Instant confidence is LOW. Drops immediately from MEDIUM to LOW.
    */
   CHECK_EQUAL(AEB_CONF_LOW, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when the object is occluded (not VISIBLE), instant confidence is LOW.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Low_When_Occluded)
{
   /** \precond
    * Occlusion status set to OCCLUDED.
    */
   object_tracks[0].occlusion_status = F360_Occlusion_Status_T::OCCLUSION_STATUS_OCCLUDED;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * aeb_confidence steps up from INVALID to LOW (instant is LOW due to occlusion).
    */
   CHECK_EQUAL(AEB_CONF_LOW, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when iso_relative_x_vel exceeds 19.4 m/s, instant confidence is LOW.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Low_When_Iso_Relative_X_Vel_Exceeds_Threshold)
{
   /** \precond
    * Set object velocity such that iso_relative_x_vel > 19.4.
    * Object moves away from host: vcs_velocity.longitudinal = 50 (forward, same as host).
    * host speed = 30, so iso_relative_x_vel ~ 50 - 30 = 20 > 19.4.
    */
   object_tracks[0].vcs_velocity.longitudinal = 50.0F;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * aeb_confidence steps up from INVALID to LOW (instant is LOW because iso_relative_x_vel > 19.4).
    */
   CHECK_EQUAL(AEB_CONF_LOW, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when underdrivable_status_ocg is not CAN_NOT_PASS_UNDER
 * and drivable_status_sg is not NONDRIVABLE, instant confidence is LOW.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Low_When_Neither_Underdrivable_Nor_Nondrivable)
{
   /** \precond
    * Set both drivability conditions to non-blocking values.
    */
   object_tracks[0].underdrivable_status_ocg = ocg::OCG_Underdrivable_Status_T::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   object_tracks[0].drivable_status_sg = sg::SG_Drivability_Class_T::UNCLASSIFIED;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * aeb_confidence steps up from INVALID to LOW.
    */
   CHECK_EQUAL(AEB_CONF_LOW, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when only underdrivable_status_ocg is CAN_NOT_PASS_UNDER
 * (and drivable_status_sg is not NONDRIVABLE), the drivability OR condition
 * is still satisfied and HIGH confidence can be reached.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__High_With_Only_OCG_Underdrivable)
{
   /** \precond
    * OCG is underdrivable, SG is drivable.
    */
   object_tracks[0].underdrivable_status_ocg = ocg::OCG_Underdrivable_Status_T::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   object_tracks[0].drivable_status_sg = sg::SG_Drivability_Class_T::UNCLASSIFIED;
   object_tracks[0].aeb_confidence = AEB_CONF_MEDIUM_HIGH;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * aeb_confidence steps up from MEDIUM_HIGH to HIGH since OCG alone satisfies the OR.
    */
   CHECK_EQUAL(AEB_CONF_HIGH, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when only drivable_status_sg is NONDRIVABLE
 * (and underdrivable_status_ocg is not CAN_NOT_PASS_UNDER), the drivability OR condition
 * is still satisfied and HIGH confidence can be reached.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__High_With_Only_SG_Nondrivable)
{
   /** \precond
    * OCG is NOT underdrivable, SG is nondrivable.
    */
   object_tracks[0].underdrivable_status_ocg = ocg::OCG_Underdrivable_Status_T::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   object_tracks[0].drivable_status_sg = sg::SG_Drivability_Class_T::NONDRIVABLE;
   object_tracks[0].aeb_confidence = AEB_CONF_MEDIUM_HIGH;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * aeb_confidence steps up from MEDIUM_HIGH to HIGH since SG alone satisfies the OR.
    */
   CHECK_EQUAL(AEB_CONF_HIGH, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when confidenceLevel is below 0.92 (MEDIUM threshold),
 * instant confidence is LOW.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Low_When_Confidence_Level_Below_Medium_Threshold)
{
   /** \precond
    * confidenceLevel = 0.90, below 0.92 MEDIUM threshold.
    */
   object_tracks[0].confidenceLevel = 0.90F;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * aeb_confidence steps up from INVALID to LOW.
    */
   CHECK_EQUAL(AEB_CONF_LOW, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when speed is below 3.0 m/s, instant confidence is LOW.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Low_When_Speed_Below_Threshold)
{
   /** \precond
    * Object speed = 2.5 m/s (below 3.0 threshold).
    */
   object_tracks[0].speed = 2.5F;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * aeb_confidence steps up from INVALID to LOW.
    */
   CHECK_EQUAL(AEB_CONF_LOW, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when time_since_track_updated exceeds 0.11s, MEDIUM confidence
 * is not reached even if other MEDIUM conditions are met, resulting in LOW.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Low_When_Time_Since_Updated_Exceeds_Medium_Threshold)
{
   /** \precond
    * Set conditions to only match MEDIUM level, but time_since_track_updated > 0.11.
    * time_since_initialization = 1.2 (only matches MEDIUM, not HIGH or MEDIUM_HIGH).
    * exist_prob = 0.93 (only matches MEDIUM).
    * confidenceLevel = 0.93 (only matches MEDIUM).
    * time_since_track_updated = 0.12 (exceeds 0.11 threshold for MEDIUM).
    */
   object_tracks[0].time_since_initialization = 1.2F;
   object_tracks[0].exist_prob = 0.93F;
   object_tracks[0].confidenceLevel = 0.93F;
   object_tracks[0].time_since_track_updated = 0.12F;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * aeb_confidence steps up from INVALID to LOW (MEDIUM confidence not reached).
    */
   CHECK_EQUAL(AEB_CONF_LOW, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that no active objects results in no changes to any object's aeb_confidence.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__No_Active_Objects)
{
   /** \precond
    * No active objects.
    */
   tracker_info.num_active_objs = 0;
   object_tracks[0].aeb_confidence = AEB_CONF_MEDIUM;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * aeb_confidence remains unchanged.
    */
   CHECK_EQUAL(AEB_CONF_MEDIUM, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when exist_prob is below HIGH threshold but above MEDIUM_HIGH threshold,
 * and time_since_initialization is above MEDIUM_HIGH threshold but below HIGH threshold,
 * instant confidence is MEDIUM_HIGH (not HIGH).
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Medium_High_Boundary_Exist_Prob)
{
   /** \precond
    * exist_prob = 0.96 (above 0.95 for MEDIUM_HIGH, below 0.99 for HIGH).
    * time_since_initialization = 1.6 (above 1.5, below 2.0).
    */
   object_tracks[0].exist_prob = 0.96F;
   object_tracks[0].time_since_initialization = 1.6F;
   object_tracks[0].aeb_confidence = AEB_CONF_MEDIUM;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * aeb_confidence steps up from MEDIUM to MEDIUM_HIGH.
    */
   CHECK_EQUAL(AEB_CONF_MEDIUM_HIGH, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when exist_prob alone is below the HIGH threshold (0.99) but all other
 * HIGH conditions including time_since_initialization are met.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Medium_High_When_Only_Exist_Prob_Below_High)
{
   /** \precond
    * Default setup with time_since_initialization = 3.0 (passes HIGH time threshold).
    * exist_prob = 0.96 (below 0.99 for HIGH, above 0.95 for MEDIUM_HIGH).
    * All other HIGH conditions are met.
    */
   object_tracks[0].exist_prob = 0.96F;
   object_tracks[0].aeb_confidence = AEB_CONF_MEDIUM;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * HIGH is not reached because exist_prob <= 0.99.
    * MEDIUM_HIGH conditions are met. Steps up from MEDIUM to MEDIUM_HIGH.
    */
   CHECK_EQUAL(AEB_CONF_MEDIUM_HIGH, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when exist_prob is below the MEDIUM_HIGH threshold (0.95), the
 * instant confidence falls to MEDIUM.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Medium_When_Exist_Prob_Below_Medium_High)
{
   /** \precond
    * time_since_initialization = 1.8 (fails HIGH, passes MEDIUM_HIGH time threshold).
    * exist_prob = 0.94 (below 0.95, fails MEDIUM_HIGH exist_prob check; above 0.92, passes MEDIUM).
    * confidenceLevel = 0.99 (default, passes MEDIUM_HIGH and MEDIUM).
    */
   object_tracks[0].time_since_initialization = 1.8F;
   object_tracks[0].exist_prob = 0.94F;
   object_tracks[0].aeb_confidence = AEB_CONF_LOW;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * Steps up from LOW to MEDIUM.
    */
   CHECK_EQUAL(AEB_CONF_MEDIUM, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that MEDIUM_HIGH confidence is reached when drivability is satisfied
 * only by SG nondrivable (OCG is drivable).
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Medium_High_With_SG_Only_Drivability)
{
   /** \precond
    * time_since_initialization = 1.8 (fails HIGH, passes MEDIUM_HIGH).
    * exist_prob = 0.96 (fails HIGH, passes MEDIUM_HIGH).
    * OCG = CAN_PASS_UNDER.
    * SG = NONDRIVABLE.
    */
   object_tracks[0].time_since_initialization = 1.8F;
   object_tracks[0].exist_prob = 0.96F;
   object_tracks[0].underdrivable_status_ocg = ocg::OCG_Underdrivable_Status_T::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   object_tracks[0].drivable_status_sg = sg::SG_Drivability_Class_T::NONDRIVABLE;
   object_tracks[0].aeb_confidence = AEB_CONF_MEDIUM;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * HIGH fails. MEDIUM_HIGH passes via SG-only drivability. Steps up from MEDIUM to MEDIUM_HIGH.
    */
   CHECK_EQUAL(AEB_CONF_MEDIUM_HIGH, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that when only time_since_initialization is below the MEDIUM threshold (1.0s),
 * the other conditions are met for MEDIUM, instant confidence is LOW.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Low_When_Time_Since_Init_Below_Medium_Threshold)
{
   /** \precond
    * time_since_initialization = 0.8 (below 1.0 for MEDIUM).
    * All other conditions are met for MEDIUM.
    */
   object_tracks[0].time_since_initialization = 0.8F;
   object_tracks[0].exist_prob = 0.93F;
   object_tracks[0].confidenceLevel = 0.93F;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * HIGH fails (time < 2.0). MEDIUM_HIGH fails (time < 1.5).
    * MEDIUM fails (time < 1.0). Instant is LOW. Steps up from INVALID to LOW.
    */
   CHECK_EQUAL(AEB_CONF_LOW, object_tracks[0].aeb_confidence);
}

/** \purpose
 * Verify that MEDIUM confidence is reached when drivability is satisfied
 * only by SG nondrivable (OCG is drivable).
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence, Update_AEB_Confidence__Medium_With_SG_Only_Drivability)
{
   /** \precond
    * time_since_initialization = 1.2 (fails HIGH and MEDIUM_HIGH, passes MEDIUM).
    * exist_prob = 0.93 (fails HIGH and MEDIUM_HIGH, passes MEDIUM).
    * confidenceLevel = 0.93 (fails HIGH and MEDIUM_HIGH, passes MEDIUM).
    * OCG = CAN_PASS_UNDER.
    * SG = NONDRIVABLE.
    */
   object_tracks[0].time_since_initialization = 1.2F;
   object_tracks[0].exist_prob = 0.93F;
   object_tracks[0].confidenceLevel = 0.93F;
   object_tracks[0].underdrivable_status_ocg = ocg::OCG_Underdrivable_Status_T::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   object_tracks[0].drivable_status_sg = sg::SG_Drivability_Class_T::NONDRIVABLE;
   object_tracks[0].aeb_confidence = AEB_CONF_LOW;

   /** \action
    * Call Update_AEB_Confidence.
    */
   Update_AEB_Confidence(host, tracker_info, object_tracks);

   /** \result
    * HIGH fails. MEDIUM_HIGH fails. MEDIUM passes via SG-only drivability.
    * Steps up from LOW to MEDIUM.
    */
   CHECK_EQUAL(AEB_CONF_MEDIUM, object_tracks[0].aeb_confidence);
}

/** @}*/

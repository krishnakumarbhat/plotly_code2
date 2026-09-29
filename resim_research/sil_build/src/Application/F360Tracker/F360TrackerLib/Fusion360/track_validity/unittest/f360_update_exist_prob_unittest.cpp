/** \file
   This unit-test file contains tests for Update_Existence_Probability() function.
*/

#include "f360_update_exist_prob.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;

/** \defgroup  f360_update_exist_prob
 *  @{
 */

/** \brief
* Test group of Update_Existenece_Probability function. Tests verify wheter
* existence probability is updated only for selected tracks and whether it is updated
* properly.
**/
TEST_GROUP(f360_update_exist_prob)
{
   /** \setup
   * Set up common variables
   * Set up sensor parameters
   * Set object vcs longitudinal position
   **/

   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calibrations = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_TRKR_TIMING_INFO_T timing_info;
   rspp_variant_A::RSPP_Detection_T detections[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS]{};
   F360_Object_Track_T& object = object_tracks[0];
   F360_Host_T host;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      sensors[0].variable.is_valid = true;
      sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[0].constant.range_limits[F360_DET_LOOK_ID_0] = 50.0F;
      sensors[0].refined.left_fov_normal[F360_DET_LOOK_ID_0] = 0.5F;
      sensors[0].refined.right_fov_normal[F360_DET_LOOK_ID_0] = 0.5F;

      object.vcs_position.x = 10.0F;

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;

      host.speed = 0.0F;
      host.vcs_sideslip = 0.0F;
   }

 };

/**
*\purpose  Purpose of this test is to verify whether object existence probability is updated if object is seen by sensor.
*\req    NA.
*/
TEST(f360_update_exist_prob, Update_Existence_Probability__ep_changed_if_object_is_seen_by_sensor)
{
   /** \precond
   * Set object position to be visible by sensor
   * Set object paramaeters to meet conditions to be updated as object with new information.
   **/
   const float32_t starting_ep = 0.433F;
   object.vcs_position.y = 0.0F;
   object.exist_prob = starting_ep;
   object.status = F360_OBJECT_STATUS_UPDATED;

   /** \action
   * Call Update_Existence_Probability()
   **/
   Update_Existence_Probability(tracker_info, calibrations, host, detections, object_tracks, timing_info);

   /** \result
   * Check whether object existence probability was modified
   **/
   CHECK_TRUE(starting_ep != object.exist_prob);
}

/**
*\purpose  Purpose of this test is to verify whether updated object existence probability is updated with new information
*\req    NA.
*/
TEST(f360_update_exist_prob, Update_Existence_Probability__updated_object_ep_updated_with_new_information)
{
   /** \precond
   * Set object position to be visible by sensor
   * Set object starting EP
   * Set object status to UPDATED
   **/
   const float32_t starting_ep = 0.433F;
   object.vcs_position.y = 0.0F;
   object.exist_prob = starting_ep;
   object.status = F360_OBJECT_STATUS_UPDATED;
   object.raw_confidence_level = 0.6F;

   /** \action
   * Call Update_Existence_Probability()
   **/
   Update_Existence_Probability(tracker_info, calibrations, host, detections, object_tracks, timing_info);

   /** \result
   * Check whether object existence probability was properly modified
   **/
   DOUBLES_EQUAL(0.901609838F, object.exist_prob, F360_EPSILON)
}

/**
*\purpose  Purpose of this test is to verify whether new updated object existence probability remains unchanged
*\req    NA.
*/
TEST(f360_update_exist_prob, Update_Existence_Probability__new_updated_object_ep_remains_unchanged)
{
   /** \precond
   * Set object position to be visible by sensor
   * Set object starting EP
   * Set object status to NEW UPDATED
   **/
   const float32_t starting_ep = 0.433F;
   object.vcs_position.y = 0.0F;
   object.exist_prob = starting_ep;
   object.status = F360_OBJECT_STATUS_NEW_UPDATED;
   object.raw_confidence_level = 0.6F;

   /** \action
   * Call Update_Existence_Probability()
   **/
   Update_Existence_Probability(tracker_info, calibrations, host, detections, object_tracks, timing_info);

   /** \result
   * Check whether object existence probability was properly modified
   **/
   DOUBLES_EQUAL(starting_ep, object.exist_prob, 1e-4F)
}

/**
*\purpose  Purpose of this test is to verify whether coasted object existence probability is updated with no new information.
*\req    NA.
*/
TEST(f360_update_exist_prob, Update_Existence_Probability__coasted_object_ep_updated_with_no_new_information)
{
   /** \precond
   * Set object position to be visible by sensor
   * Set object starting EP
   * Set object status to COASTED
   **/
   const float32_t starting_ep = 0.433F;
   object.vcs_position.y = 0.0F;
   object.exist_prob = starting_ep;
   object.status = F360_OBJECT_STATUS_COASTED;
   object.raw_confidence_level = 0.6F;

   /** \action
   * Call Update_Existence_Probability()
   **/
   Update_Existence_Probability(tracker_info, calibrations, host, detections, object_tracks, timing_info);

   /** \result
   * Check whether object existence probability was properly modified
   **/
   DOUBLES_EQUAL(0.4431523F, object.exist_prob, F360_EPSILON)
}

/**
*\purpose  Purpose of this test is to verify whether new object track EP is not modified.
*\req    NA.
*/
TEST(f360_update_exist_prob, Update_Existence_Probability__new_object_ep_is_not_modified)
{
   /** \precond
   * Set object position to be visible by sensor
   * Set object starting EP
   * Set object status to NEW
   **/
   const float32_t starting_ep = 0.433F;
   object.vcs_position.y = 0.0F;
   object.exist_prob = starting_ep;
   object.status = F360_OBJECT_STATUS_NEW;
   object.raw_confidence_level = 0.6F;

   /** \action
   * Call Update_Existence_Probability()
   **/
   Update_Existence_Probability(tracker_info, calibrations, host, detections, object_tracks, timing_info);

   /** \result
   * Check whether object existence probability was not modified
   **/
   DOUBLES_EQUAL(starting_ep, object.exist_prob, F360_EPSILON)
}

/**
*\purpose  Purpose of this test is to verify whether object with low RCS will have low EP.
*\req    NA.
*/
TEST(f360_update_exist_prob, Update_Existence_Probability__rain_target_saturation)
{
   /** \precond
   * Set object properties to match a target created by rain detections
   **/
   object.vcs_position.y = 0.0F;
   object.vcs_position.x = 1.0F;
   object.exist_prob = 1.0F;
   object.status = F360_OBJECT_STATUS_UPDATED;
   object.raw_confidence_level = 1.0F;
   object.average_rcs = -17.0F;
   object.maximum_rcs = -11.0F;
   object.time_since_initialization =  1.0F;

   /** \action
   * Call Update_Existence_Probability()
   **/
   Update_Existence_Probability(tracker_info, calibrations, host, detections, object_tracks, timing_info);

   /** \result
   * Check whether object existence probability is low due to rain target properties
   **/
   DOUBLES_EQUAL(0.65F, object.exist_prob, F360_EPSILON)
}

/**
*\purpose  Purpose of this test is to verify whether object with low average RCS but higher maximum RCS will have high EP.
*\req    NA.
*/
TEST(f360_update_exist_prob, Update_Existence_Probability__low_avg_high_max_RCS)
{
   /** \precond
   * Set object properties to match a low RCS pedestrian
   **/
   object.vcs_position.y = 0.0F;
   object.vcs_position.x = 1.0F;
   object.exist_prob = 1.0F;
   object.status = F360_OBJECT_STATUS_UPDATED;
   object.raw_confidence_level = 1.0F;
   object.average_rcs = -17.0F;
   object.maximum_rcs = -5.0F;
   object.time_since_initialization =  5.0F;

   /** \action
   * Call Update_Existence_Probability()
   **/
   Update_Existence_Probability(tracker_info, calibrations, host, detections, object_tracks, timing_info);

   /** \result
   * Check whether object existence probability was not modified
   **/
   DOUBLES_EQUAL(1.0F, object.exist_prob, F360_EPSILON)
}

/**
*\purpose  Purpose of this test is to verify whether in variant D EP is low if object has high percentage of IDM detections.
*\req    NA.
*/
TEST(f360_update_exist_prob, Update_Existence_Probability__IDM_decrease_variant_D)
{
   /** \precond
   * Set object with high percentage of IDM detections.
   * Set tracker variant to D.
   **/
   object.ndets = 5U;
   object.detids[0] = 1U; // IDM
   object.detids[1] = 2U; // IDM
   object.detids[2] = 3U; // non-IDM
   object.detids[3] = 4U; // IDM
   object.detids[4] = 5U; // IDM

   object.exist_prob = 1.0F;

   detections[0].raw.f_idm_det = true;
   detections[1].raw.f_idm_det = true;
   detections[2].raw.f_idm_det = false;
   detections[3].raw.f_idm_det = true;
   detections[4].raw.f_idm_det = true;

   tracker_info.variant.type = F360_VARIANT_TYPE_D;


   /** \action
   * Call Update_Existence_Probability()
   **/
   Update_Existence_Probability(tracker_info, calibrations, host, detections, object_tracks, timing_info);

   /** \result
   * Check whether object existence probability is low due to high percentage of IDM detections
   **/
   DOUBLES_EQUAL(0.65F, object.exist_prob, F360_EPSILON)
}
/** @}*/

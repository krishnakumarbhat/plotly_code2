/** \file
   This file contains qualification tests for Track Validity module.
*/

#include "f360_update_exist_prob.h"
#include "f360_update_object_confidence_levels.h"
#include "f360_calc_trk_ttc.h"
#include "rspp_detection.h"
#include "rspp_detection_list.h" // for rspp_variant_A::RSPP_Detection_List_T

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
TEST_GROUP(f360_update_exist_prob_qualtest)
{
   /** \setup
   * Set up common variables
   * Set up sensor parameters
   * Set object vcs longitudinal position
   **/

   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calibrations = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   rspp_variant_A::RSPP_Detection_T detections[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_TRKR_TIMING_INFO_T timing_info;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
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
*\purpose  Purpose of this test is to verify whether minimum existence probability is equal to minimum thereshold.
*\req    CPR-3777, CPR-3776
*/
TEST(f360_update_exist_prob_qualtest, Update_Existence_Probability__check_whether_minimum_EP_value_is_equal_to_min_threshold)
{
   /** \precond
   *Define needed data
   **/
   object.status = F360_OBJECT_STATUS_UPDATED;
   object.exist_prob = -1.0F;
   float32_t expected_EP = calibrations.k_ep_min_allowed_exist_prob;

   /** \action
   * Call tested function
   **/
   Update_Existence_Probability(tracker_info, calibrations, host, detections, object_tracks, timing_info);

   /** \result
   * Expected EP should be equald to k_exist_prob_min_allowed_exist_prob defined in calibrations.
   **/
   DOUBLES_EQUAL(expected_EP, object.exist_prob, F360_EPSILON);
}


/**
*\purpose  Purpose of this test is to verify whether maximum existence probability is equal 1.
*\req    CPR-3777, CPR-3776
*/
TEST(f360_update_exist_prob_qualtest, Update_Existence_Probability__check_whether_maximum_EP_value_is_1)
{
   /** \precond
   *Setting needed data
   **/
   object.status = F360_OBJECT_STATUS_UPDATED;
   object.exist_prob = 5.0F;
   object.raw_confidence_level = 1.0F;

   float32_t expected_EP = 1.0F;

   /** \action
   * Call tested function
   **/
   Update_Existence_Probability(tracker_info, calibrations, host, detections, object_tracks, timing_info);

   /** \result
   * exist_prob should not exceed 1.0
   **/
   DOUBLES_EQUAL(expected_EP, object.exist_prob, F360_EPSILON);
}

/**
*\purpose  Purpose of this test is to verify whether existence probability depend on object's state variance.
*\req    CPR-3778, CPR-3776
*/
TEST(f360_update_exist_prob_qualtest, Update_Existence_Probability__check_whether_EP_depend_onobject_state_variance)
{
   /** \step{1}
   *Calculate EP for object track
   **/

   /** \precond
   *Setting needed data
   **/
   object.errcov[0][0] = 0.1F;
   object.errcov[0][1] = 0.1F;
   object.status = F360_OBJECT_STATUS_UPDATED;
   object.raw_confidence_level = 1.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   /** \action
   *Call function
   **/
   Update_Existence_Probability(tracker_info, calibrations, host, detections, object_tracks, timing_info);
   const float32_t step_1_EP = object.exist_prob;

   /** \step{2}
   *Calculate EP for object track from step(1) after changing state covariance
   **/

   /** \precond
   *Setting needed data
   **/
   object.errcov[0][0] = 0.2F;

   /** \action
   * Call tested function
   **/
   Update_Existence_Probability(tracker_info, calibrations, host, detections, object_tracks, timing_info);
   const float32_t step_2_EP = object.exist_prob;

   /** \result
   * Existence probability should change after changing state covariance between two steps.
   **/
   CHECK_TRUE(step_1_EP != step_2_EP);
}
/** @}*/

/** \defgroup  f360_conf_track_confidence_level
*  @{
*/

/** \brief
*  This test group contains tests for Conf_Track_Confidence_Level() function regarding reqiurements from SRD document.
**/
TEST_GROUP(f360_conf_track_confidence_level_qualtest)
{
   /** \setup
   * Creating required data structures and initializing calibrations.
   **/
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calibrations = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);
   }
};

/**
*\purpose  Purpose of this test is to verify whether the tracker calculates confidence level for updated object tracks.
*\req    CPR-3772
*/
TEST(f360_conf_track_confidence_level_qualtest, Conf_Track_Confidence_Level__if_confidence_is_calculated_for_updated_obj_tracks)
{
   /** \precond
   *Setting needed data
   **/
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].time_since_track_updated = calibrations.k_hyst_time_for_coasted_objects - 0.001F;
   tracker_info.elapsed_time_s = 0.001F;
   object_tracks[0].num_rr_inlier_dets = 2;

   float32_t initial_confLevel = object_tracks[0].confidenceLevel;
   const int32_t obj_track_id = 0;

   /** \action
   *Call function
   **/
   Update_Object_Confidence_Levels(tracker_info, calibrations, object_tracks[obj_track_id]);

   /** \result
   * Initial confidence level different from confidence after run Conf_Track_Confidence_Level().
   **/
   CHECK(initial_confLevel != object_tracks[0].confidenceLevel);
}

/**
*\purpose  Purpose of this test is to verify whether the confidence level is in range 0 to 1 for object with status F360_OBJECT_STATUS_UPDATED,
*          when confidence initial value is 0.0
*\req    CPR-3773
*/
TEST(f360_conf_track_confidence_level_qualtest, Conf_Track_Confidence_Level__is_confidence_level_is_equal_or_higher_than_0_for_UPDATED)
{
   /** \precond
   *Setting needed data
   **/
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].time_since_track_updated = calibrations.k_hyst_time_for_coasted_objects - 0.001F;
   object_tracks[0].confidenceLevel = 0.0F;
   tracker_info.elapsed_time_s = 0.001F;
   object_tracks[0].num_rr_inlier_dets = 2;

   const int32_t obj_track_id = 0;

   /** \action
   *Call function
   **/
   Update_Object_Confidence_Levels(tracker_info, calibrations, object_tracks[obj_track_id]);

   /** \result
   * Confidence level >= 0 and <= 1.
   **/
   CHECK_TRUE(0.0F <= object_tracks[0].confidenceLevel);
   CHECK_TRUE(1.0F >= object_tracks[0].confidenceLevel);
}

/**
*\purpose  Purpose of this test is to verify whether the confidence level is in range 0 to 1 for object with status F360_OBJECT_STATUS_UPDATED,
*          when confidence initial value is 1.0
*\req    CPR-3773
*/
TEST(f360_conf_track_confidence_level_qualtest, Conf_Track_Confidence_Level__is_confidence_level_is_equal_or_lower_than_1_for_UPDATED)
{
   /** \precond
   *Setting needed data
   **/
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].time_since_track_updated = calibrations.k_hyst_time_for_coasted_objects - 0.001F;
   object_tracks[0].confidenceLevel = 1.0F;
   tracker_info.elapsed_time_s = 0.001F;
   object_tracks[0].num_rr_inlier_dets = 2;

   const int32_t obj_track_id = 0;

   /** \action
   *Call function
   **/
   Update_Object_Confidence_Levels(tracker_info, calibrations, object_tracks[obj_track_id]);

   /** \result
   * Confidence level should be  >= 0 and <= 1.
   **/
   CHECK_TRUE(0.0F <= object_tracks[0].confidenceLevel);
   CHECK_TRUE(1.0F >= object_tracks[0].confidenceLevel);
}

/**
*\purpose  Purpose of this test is to verify whether the confidence level is in range 0 to 1 for object with status F360_OBJECT_STATUS_COASTED,
*          when confidence initial value is 0.0
*\req    CPR-3773
*/
TEST(f360_conf_track_confidence_level_qualtest, Conf_Track_Confidence_Level__is_confidence_level_is_equal_or_higher_than_0_for_COASTED)
{
   /** \precond
   *Setting needed data
   **/
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].time_since_track_updated = calibrations.k_hyst_time_for_coasted_objects - 0.001F;
   object_tracks[0].confidenceLevel = 0.0F;
   tracker_info.elapsed_time_s = 0.001F;
   object_tracks[0].num_rr_inlier_dets = 2;

   const int32_t obj_track_id = 0;

   /** \action
   *Call function
   **/
   Update_Object_Confidence_Levels(tracker_info, calibrations, object_tracks[obj_track_id]);

   /** \result
   * Confidence level should be  >= 0 and <= 1.
   **/
   CHECK_TRUE(0.0F <= object_tracks[0].confidenceLevel);
   CHECK_TRUE(1.0F >= object_tracks[0].confidenceLevel);
}

/**
*\purpose  Purpose of this test is to verify whether the confidence level is in range 0 to 1 for object with status F360_OBJECT_STATUS_COASTED,
*          when confidence initial value is 1.0
*\req    CPR-3773
*/
TEST(f360_conf_track_confidence_level_qualtest, Conf_Track_Confidence_Level__is_confidence_level_is_equal_or_lower_than_1_for_COASTED)
{
   /** \precond
   *Setting needed data
   **/
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].time_since_track_updated = calibrations.k_hyst_time_for_coasted_objects - 0.001F;
   object_tracks[0].confidenceLevel = 1.0F;
   tracker_info.elapsed_time_s = 0.001F;
   object_tracks[0].num_rr_inlier_dets = 2;

   const int32_t obj_track_id = 0;

   /** \action
   *Call function
   **/
   Update_Object_Confidence_Levels(tracker_info, calibrations, object_tracks[obj_track_id]);

   /** \result
   * Confidence level should be  >= 0 and <= 1.
   **/
   CHECK_TRUE(0.0F <= object_tracks[0].confidenceLevel);
   CHECK_TRUE(1.0F >= object_tracks[0].confidenceLevel);
}

/**
*\purpose  Purpose of this test is to verify whether the confidence level for overlaping object is reduced by multiplying it by calibration factor.
*\req    CPR-3774
*/
TEST(f360_conf_track_confidence_level_qualtest, Conf_Track_Confidence_Level__is_confidence_overlaping_object_is_reduced_by_factor)
{
   /** \precond
   *Setting needed data
   **/
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[0].confidenceLevel = 1.0F;
   object_tracks[0].f_overlapping_with_object = true;
   const int32_t obj_track_id = 0;

   /** \action
   *Call function
   **/
   Update_Object_Confidence_Levels(tracker_info, calibrations, object_tracks[obj_track_id]);

   /** \result
   * Confidence level should be equal the calibration factor for overlaping objects. Initial confidence level is 1.0 so after the multiplying it
   * by the reduction factor for overlaping objects(which is lower than 1.0) they should be equal the reduction factor and lower than initial confidence level.
   **/
   DOUBLES_EQUAL(calibrations.k_conf_overlapping_reduction_factor, object_tracks[0].confidenceLevel, F360_EPSILON);
}

/**
*\purpose  Purpose of this test is to verify whether the confidence level depend on object track status and number of associated detections.
*\req    CPR-3775
*/
TEST(f360_conf_track_confidence_level_qualtest, Conf_Track_Confidence_Level__is_confidence_level_depend_on_object_status)
{
   /** \precond
   *Setting needed data
   **/
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].confidenceLevel = 1.0F;
   object_tracks[0].num_rr_inlier_dets = 5;
   object_tracks[0].time_since_track_updated = calibrations.k_hyst_time_for_coasted_objects - 0.001F;
   const int32_t obj_track_1_id = 0;

   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].confidenceLevel = 1.0F;
   object_tracks[1].num_rr_inlier_dets = 5;
   object_tracks[1].time_since_track_updated = calibrations.k_hyst_time_for_coasted_objects - 0.001F;
   const int32_t obj_track_2_id = 1;

   tracker_info.elapsed_time_s = 0.02F;

   /** \action
   *Call function
   **/
   Update_Object_Confidence_Levels(tracker_info, calibrations, object_tracks[obj_track_1_id]);
   Update_Object_Confidence_Levels(tracker_info, calibrations, object_tracks[obj_track_2_id]);

   /** \result
   * Confidence level should be calculated different way for objects with different statuses.
   **/
   CHECK_TRUE(object_tracks[0].confidenceLevel != object_tracks[1].confidenceLevel);
}

/**
*\purpose  Purpose of this test is to verify whether the confidence level depend on number of detections updating the object track.
*\req    CPR-3775
*/
TEST(f360_conf_track_confidence_level_qualtest, Conf_Track_Confidence_Level__is_confidence_level_depend_on_number_of_detections_updating_object)
{
   /** \precond
   *Setting needed data
   **/
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].confidenceLevel = 1.0F;
   object_tracks[0].num_rr_inlier_dets = 10;
   object_tracks[0].time_since_track_updated = calibrations.k_hyst_time_for_coasted_objects - 0.001F;
   const int32_t obj_track_1_id = 0;

   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].confidenceLevel = 1.0F;
   object_tracks[0].num_rr_inlier_dets = 1;
   object_tracks[1].time_since_track_updated = calibrations.k_hyst_time_for_coasted_objects - 0.001F;
   const int32_t obj_track_2_id = 1;

   tracker_info.elapsed_time_s = 0.02F;

   /** \action
   *Call function
   **/
   Update_Object_Confidence_Levels(tracker_info, calibrations, object_tracks[obj_track_1_id]);
   Update_Object_Confidence_Levels(tracker_info, calibrations, object_tracks[obj_track_2_id]);

   /** \result
   * Confidence level should depend on number of detections associated to object.
   **/
   CHECK_TRUE(object_tracks[0].confidenceLevel > object_tracks[1].confidenceLevel);
}

/**
*\purpose  Purpose of this test is to verify whether the confidence level depend on time since the object track was updated.
*\req    CPR-3775
*/
TEST(f360_conf_track_confidence_level_qualtest, Conf_Track_Confidence_Level__is_confidence_level_depend_on_time_since_the_object_was_updated)
{
   /** \precond
   *Setting needed data
   **/
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].confidenceLevel = 1.0F;
   object_tracks[0].num_rr_inlier_dets = 10;
   object_tracks[0].time_since_track_updated = calibrations.k_hyst_time_for_coasted_objects - 0.001F;
   const int32_t obj_track_1_id = 0;

   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].confidenceLevel = 1.0F;
   object_tracks[0].num_rr_inlier_dets = 10;
   object_tracks[1].time_since_track_updated = calibrations.k_hyst_time_for_coasted_objects + 0.001F;
   const int32_t obj_track_2_id = 1;

   tracker_info.elapsed_time_s = 0.02F;

   /** \action
   *Call function
   **/
   Update_Object_Confidence_Levels(tracker_info, calibrations, object_tracks[obj_track_1_id]);
   Update_Object_Confidence_Levels(tracker_info, calibrations, object_tracks[obj_track_2_id]);

   /** \result
   * Because of different time_since_track_updated for tested objects the confidenceLevel should differ.
   **/
   CHECK_TRUE(object_tracks[0].confidenceLevel != object_tracks[1].confidenceLevel);
}

/** @}*/

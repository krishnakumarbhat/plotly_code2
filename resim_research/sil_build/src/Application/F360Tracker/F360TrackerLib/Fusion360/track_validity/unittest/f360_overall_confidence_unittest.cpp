/** \file
 * This file contains unit tests for content of f360_overall_confidence.cpp file
 */

#include "f360_calibrations.h"
#include "f360_overall_confidence.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;


/** \defgroup  f360_overall_confidence
 *  @{
 */

/** \brief
 * Tests for overall confidence and underlying confidence signals attributing to smoothness checks on individual track states.
 */
TEST_GROUP(f360_overall_confidence)
{	
   /** \setup
    * Set some thresholds for the state machine.
    */
   F360_Calibrations_T calib = {};
   rspp_variant_A::RSPP_Detection_List_T detection_list{};

   const float thresh_easy = 1.0F;
   const float thresh_medium = 0.5F;
   const float thresh_hard = 0.2F;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
 * Check that high overall confidence is assigned to stable tracks with high underlying state confidence.
 * \req NA
 */
TEST(f360_overall_confidence, Assign_High_Overall_Conf)
{
   /** \precond
    * Define object states and tracker info.
    */
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};

   object_tracks[0].id = 1;
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 1.0F;
   object_tracks[0].speed = 0.0F;
   object_tracks[0].tang_accel = 0.0F;
   object_tracks[0].vcs_heading = Angle{ 0.0F };

   object_tracks[0].predicted_vcs_position.x = 10.0F;
   object_tracks[0].predicted_vcs_position.y = 1.0F;
   object_tracks[0].predicted_speed = 0.0F;

   object_tracks[0].conf_longitudinal_position = CONF9_HIGH;
   object_tracks[0].conf_lateral_position = CONF9_HIGH;
   object_tracks[0].conf_speed = CONF9_HIGH;
   object_tracks[0].ndets = 1;
   object_tracks[0].detids[0] = 1;
   detection_list.detections[0].raw.rcs = 20.0F;

   object_tracks[1].id = 2;
   object_tracks[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].vcs_position.x = 10.0F;
   object_tracks[1].vcs_position.y = -1.0F;
   object_tracks[1].vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].vcs_velocity.lateral = 0.0F;

   object_tracks[1].predicted_vcs_position.x = 10.0F;
   object_tracks[1].predicted_vcs_position.y = -1.0F;
   object_tracks[1].predicted_vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].predicted_vcs_velocity.lateral = 0.0F;

   object_tracks[1].conf_longitudinal_position = CONF9_HIGH;
   object_tracks[1].conf_lateral_position = CONF9_HIGH;
   object_tracks[1].conf_speed = CONF9_HIGH;

   
   object_tracks[1].ndets = 1;
   object_tracks[1].detids[0] = 2;
   detection_list.detections[1].raw.rcs = 20.0F;

   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.num_active_objs = 2;

   /** \action
    * Call the Overall_Confidence function.
    */
   Overall_Confidence(object_tracks, detection_list, tracker_info, calib);

   /** \result
    * Check that the confidence level is high.
    */
   CHECK_EQUAL(CONF3_HIGH, object_tracks[0].conf_overall);
   CHECK_EQUAL(CONF3_HIGH, object_tracks[1].conf_overall);
}

/** \purpose
 * Check that medium overall confidence is assigned to tracks that have coasted for too long.
 * \req NA
 */
TEST(f360_overall_confidence, Assign_Med_Overall_Conf)
{
   /** \precond
    * Define object states and tracker info.
    */
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};

   object_tracks[0].id = 1;
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].time_since_stage_start = 0.15F;
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 1.0F;
   object_tracks[0].speed = 0.0F;
   object_tracks[0].tang_accel = 0.0F;
   object_tracks[0].vcs_heading = Angle{ 0.0F };

   object_tracks[0].predicted_vcs_position.x = 10.0F;
   object_tracks[0].predicted_vcs_position.y = 1.0F;
   object_tracks[0].predicted_speed = 0.0F;

   object_tracks[0].conf_longitudinal_position = CONF9_HIGH;
   object_tracks[0].conf_lateral_position = CONF9_HIGH;
   object_tracks[0].conf_speed = CONF9_HIGH;

   object_tracks[0].ndets = 1;
   object_tracks[0].detids[0] = 1;
   detection_list.detections[0].raw.rcs = 20.0F;

   object_tracks[1].id = 2;
   object_tracks[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].time_since_stage_start = 0.15F;
   object_tracks[1].vcs_position.x = 10.0F;
   object_tracks[1].vcs_position.y = -1.0F;
   object_tracks[1].vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].vcs_velocity.lateral = 0.0F;

   object_tracks[1].predicted_vcs_position.x = 10.0F;
   object_tracks[1].predicted_vcs_position.y = -1.0F;
   object_tracks[1].predicted_vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].predicted_vcs_velocity.lateral = 0.0F;

   object_tracks[1].conf_longitudinal_position = CONF9_HIGH;
   object_tracks[1].conf_lateral_position = CONF9_HIGH;
   object_tracks[1].conf_speed = CONF9_HIGH;

   object_tracks[1].ndets = 1;
   object_tracks[1].detids[0] = 2;
   detection_list.detections[1].raw.rcs = 20.0F;

   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.num_active_objs = 2;

   /** \action
    * Call the Overall_Confidence function.
    */
   Overall_Confidence(object_tracks, detection_list, tracker_info, calib);

   /** \result
    * Check that the confidence level is medium.
    */
   CHECK_EQUAL(CONF3_MED, object_tracks[0].conf_overall);
   CHECK_EQUAL(CONF3_MED, object_tracks[1].conf_overall);
}

/** \purpose
 * Check that low overall confidence is assigned to unstable tracks.
 * \req NA
 */
TEST(f360_overall_confidence, Assign_Low_Overall_Conf)
{
   /** \precond
    * Define object states and tracker info.
    */
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};

   object_tracks[0].id = 1;
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].time_since_stage_start = 0.15F;
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 1.0F;
   object_tracks[0].speed = 0.0F;
   object_tracks[0].tang_accel = 0.0F;
   object_tracks[0].vcs_heading = Angle{ 0.0F };

   object_tracks[0].predicted_vcs_position.x = 10.0F;
   object_tracks[0].predicted_vcs_position.y = 1.0F;
   object_tracks[0].predicted_speed = 0.0F;

   object_tracks[0].conf_longitudinal_position = CONF9_LOW4;
   object_tracks[0].conf_lateral_position = CONF9_LOW4;
   object_tracks[0].conf_speed = CONF9_LOW4;

   object_tracks[0].ndets = 1;
   object_tracks[0].detids[0] = 1;
   detection_list.detections[1].raw.rcs = 20.0F;

   object_tracks[1].id = 2;
   object_tracks[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].time_since_stage_start = 0.15F;
   object_tracks[1].vcs_position.x = 10.0F;
   object_tracks[1].vcs_position.y = -1.0F;
   object_tracks[1].vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].vcs_velocity.lateral = 0.0F;

   object_tracks[1].predicted_vcs_position.x = 10.0F;
   object_tracks[1].predicted_vcs_position.y = -1.0F;
   object_tracks[1].predicted_vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].predicted_vcs_velocity.lateral = 0.0F;

   object_tracks[1].conf_longitudinal_position = CONF9_LOW4;
   object_tracks[1].conf_lateral_position = CONF9_LOW4;
   object_tracks[1].conf_speed = CONF9_LOW4;

   object_tracks[1].ndets = 1;
   object_tracks[1].detids[0] = 2;
   detection_list.detections[0].raw.rcs = 20.0F;

   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.num_active_objs = 2;

   /** \action
    * Call the Overall_Confidence function.
    */
   Overall_Confidence(object_tracks, detection_list, tracker_info, calib);

   /** \result
    * Check that the confidence level is medium.
    */
   CHECK_EQUAL(CONF3_LOW, object_tracks[0].conf_overall);
   CHECK_EQUAL(CONF3_LOW, object_tracks[1].conf_overall);
}

/** \defgroup  f360_confidence_blocking
 *  @{
 */

 /** \brief
  * It tests functionality of Confidence_Blocking().
  */
TEST_GROUP(f360_overall_confidence_blocking)
{
   /** \setup
    * Declare required structs and initialize calibs
    */
   F360_Calibrations_T calib{};
   rspp_variant_A::RSPP_Detection_List_T detection_list{};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS]{};
   F360_Tracker_Info_T tracker_info{};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
 * Check that overall confidence can be blocked when a single detection with low RCS is associated and overall
 * confidence is already none.
 * \req NA
 */
TEST(f360_overall_confidence_blocking, Check_Counter_Behavior)
{
   /** \step{1}
    * Run the overall confidence function with a new track
    */
   /** \precond
    * Define object states and tracker info.
    */
   object_tracks[0].id = 1;
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[0].vcs_position.x = 5.0F;
   object_tracks[0].vcs_position.y = 1.0F;
   object_tracks[0].speed = 0.0F;
   object_tracks[0].tang_accel = 0.0F;
   object_tracks[0].vcs_heading.Value(0.0F);

   object_tracks[0].predicted_vcs_position.x = 5.0F;
   object_tracks[0].predicted_vcs_position.y = 1.0F;
   object_tracks[0].predicted_speed = 0.0F;

   object_tracks[0].conf_overall = CONF3_NONE;
   object_tracks[0].ndets = 1;
   object_tracks[0].detids[0] = 1;
   object_tracks[0].low_rcs_dets_cnt = 2;
   detection_list.detections[0].raw.rcs = -20.0F;
   detection_list.detections[0].raw.range = 5.0F;

   tracker_info.active_obj_ids[0] = 1;
   tracker_info.num_active_objs = 1;

   /** \action
    * Call the Overall_Confidence function.
    */
   Overall_Confidence(object_tracks, detection_list, tracker_info, calib);

   /** \result
    * Check that the confidence level is none. 
    * Check that the confidence blocker count is unchanged since the information has already been processed by the clusters. 
    */
   CHECK_EQUAL(CONF3_NONE, object_tracks[0].conf_overall);
   CHECK_EQUAL(2, object_tracks[0].low_rcs_dets_cnt);

   /** \step{2}
    * Run the overall confidence function with an updated track
    */
   /** \precond
    * Set status to updated
    */
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;

   /** \action
    * Call the Overall_Confidence function.
    */
   Overall_Confidence(object_tracks, detection_list, tracker_info, calib);

   /** \result
    * Check that the confidence level is none. 
    * Check that the confidence blocker count is increased. 
    */
   CHECK_EQUAL(CONF3_NONE, object_tracks[0].conf_overall);
   CHECK_EQUAL(3, object_tracks[0].low_rcs_dets_cnt);
   
   /** \step{3}
    * Run the overall confidence function with a coasted track
    */
   /** \precond
    * Set status to coasted
    */
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;

   /** \action
    * Call the Overall_Confidence function.
    */
   Overall_Confidence(object_tracks, detection_list, tracker_info, calib);

   /** \result
    * Check that the confidence level is none. 
    * Check that the confidence blocker count is decreased by 2. 
    */
   CHECK_EQUAL(CONF3_NONE, object_tracks[0].conf_overall);
   CHECK_EQUAL(3, object_tracks[0].low_rcs_dets_cnt);

   /** \step{4}
    * Run the overall confidence function with an updated track with high rcs
    */
   /** \precond
    * Set status to updated and give the detection high rcs
    */
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   detection_list.detections[0].raw.rcs = 10.0F;

   /** \action
    * Call the Overall_Confidence function.
    */
   Overall_Confidence(object_tracks, detection_list, tracker_info, calib);

   /** \result
    * Check that the confidence level is none. 
    * Check that the confidence blocker count is decreased by 2. 
    */
   CHECK_EQUAL(CONF3_NONE, object_tracks[0].conf_overall);
   CHECK_EQUAL(1, object_tracks[0].low_rcs_dets_cnt);
   
   /** \step{5}
    * Run the overall confidence function and check that the track reaches high confidence
    */
   /** \precond
    * None
    */
   /** \action
    * Call the Overall_Confidence function 10 times.
    */
   for(int i = 0; i < 10; i++)
   {
      Overall_Confidence(object_tracks, detection_list, tracker_info, calib);
   }

   /** \result
    * Check that the confidence level is high. 
    * Check that the confidence blocker count is zero. 
    */
   CHECK_EQUAL(CONF3_HIGH, object_tracks[0].conf_overall);
   CHECK_EQUAL(0, object_tracks[0].low_rcs_dets_cnt);
}

/** @}*/

/** \file
   This file contains unit tests that verifies the content of f360_determine_association_hypothesis.cpp
*/

#include <math.h>
#include <float.h>
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <algorithm>
#include "f360_iterator.h"
#include "f360_determine_association_hypothesis.h"
#include "f360_calibrations.h"
#include "f360_detection_to_track_association.h"
#include "f360_detection_association_support_functions.h"

using namespace f360_variant_A;

/** \defgroup  f360_Test_Determine_Association_Hypothesis
*  @{
*/
/** \brief
*  Set up an object and place detections around it with various properties and check
*  that expected detections are associated to expected parameters. 
*  More details can be found in DFD-175
**/
TEST_GROUP(f360_Test_Determine_Association_Hypothesis)
{
   
   F360_Calibrations_T calib;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];
   uint32_t det_idx_array[MAX_NUMBER_OF_DETECTIONS];
   uint32_t num_dets_in_array;
   float32_t host_speed;
   rspp_variant_A::RSPP_Detection_T dets[MAX_NUMBER_OF_DETECTIONS];
   F360_Detection_Props_T det_p[MAX_NUMBER_OF_DETECTIONS];
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   float32_t det_score_array[MAX_NUMBER_OF_DETECTIONS];
   float32_t det_rdot_comp_array[MAX_NUMBER_OF_DETECTIONS];
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};

   /** \setup
   * Setup common data for all tests
   **/
   TEST_SETUP()
   {
      /** \precond
      * Initialize detection score array
      */
      std::fill(cmn::begin(det_score_array), cmn::end(det_score_array), INFTY);

      // Setup host speed first, then rest of parameters will follow
      host_speed = 2.0F;

      // Calibrations
      Initialize_Tracker_Calibrations(calib);

      // Sensor calibs
      sensors[0].constant.id = 1;
      sensors[0].constant.v_wrapping[0] = 25.0F;
      sensors[0].constant.r_wrapping[0] = 0.0F;
      
      // Sensor
      sensors[0].variable.vcs_velocity.longitudinal = host_speed;

      // Prepare array of detection indexes passed to association
      num_dets_in_array = 10;
      for (uint32_t i = 0U; i < num_dets_in_array; i++)
      {
         det_idx_array[i] = i;
      }
      
      // Set up two objects both along vcs_long axis with 3 meters apart.
      // With the dimensions of unit vectors this creates an overlapping zone of extended bounding boxes
      float32_t first_obj_long_pos = 10.0F;
      float32_t delta_dist = 2.5F;
      float32_t long_pos[2] = { first_obj_long_pos, first_obj_long_pos + delta_dist };
      float32_t num_obj = 2;
      for (uint32_t i = 0U; i < num_obj; i++)
      {
         objects[i].id = i + 1;
         objects[i].vcs_position.x = long_pos[i];
         objects[i].vcs_position.y = 0.0F;
         objects[i].vcs_heading = Angle{ 0.0F };
         objects[i].long_buffer_zone_len1 = 1.0F;
         objects[i].long_buffer_zone_len2 = 1.0F;
         objects[i].lat_buffer_zone_wid1 = 1.0F;
         objects[i].lat_buffer_zone_wid2 = 1.0F;
         objects[i].speed = 10.0F;
         objects[i].movable_prob = 1.0F;
         objects[i].vcs_velocity.longitudinal = 10.0F;
         objects[i].vcs_velocity.lateral = 0.0F;
         objects[i].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
         objects[i].Set_Bbox_Orientation(Angle{ F360_DEG2RAD(0.0F) });
         objects[i].Update_Bbox_Size(2.0F, 2.0F);
      }
      

      // Prepare detections and initialize them so that they are expected to associate to the first object
      for (uint32_t i = 0U; i < num_dets_in_array; i++)
      {
         // Give all detections the same sensor id
         dets[i].raw.sensor_id = 1;

         // Give all detections the same range rate. We can do this if all detections have their vcs_azimuth close to zero.
         // In this zone the radial component of the relative velocity between sensor and target is aligned in long direction.
         // The errors this approximation introduces is small in comparison to expected range rates of an object in this zone. 
         dets[i].raw.range_rate = objects[0].vcs_velocity.longitudinal - host_speed;

         // Now we define all test detections so we expect them to associate. Then tweak detections one by one when designing tests.
         det_p[i].vcs_position.x = 10.0F;
         dets[i].processed.vcs_position_x = det_p[i].vcs_position.x;
         dets[i].processed.cos_vcs_az = 1.0F;
         det_p[i].f_ok_to_use = true;

         // Clearing associated object id of detection
         det_p[0].object_track_id = 0;
      }
   }
};


/**
*\purpose  Verify that all detections are associated as expected for a moveable object in scenario C described in document in DFD-175.
*\req    NA
*/
TEST(f360_Test_Determine_Association_Hypothesis, Test_Determine_Association_Hypothesis_C_moveable_obj)
{
   /** \precond
   * Set up detections properties accordingly
   */
   objects[1].vcs_position.x = objects[0].vcs_position.x;
   objects[1].speed = 9.5F;
   objects[1].vcs_velocity.longitudinal = objects[1].speed;
   objects[1].Update_Bbox_Center();
   num_dets_in_array = 2U;

   det_p[0].vcs_position.x = objects[0].vcs_position.x;
   det_p[0].vcs_position.y = objects[0].vcs_position.y;

   /** \action
   * Call function Determine_Association_Hypothesis twice for both test objects
   **/
   for (uint32_t i = 0U; i < 2U; i++)
   {
      Determine_Association_Hypothesis(
         calib,
         sensors,
         host_speed,
         det_idx_array,
         num_dets_in_array,
         i,
         dets,
         sep,
         det_p,
         objects,
         det_score_array,
         det_rdot_comp_array);
   }

   /** \result
   * Check that correct parameters are returned
   **/
   CHECK_EQUAL_TEXT(0, det_p[0].object_track_id, "Detection is not unassociated as expected")
   CHECK_FALSE_TEXT(det_p[0].f_ok_to_use, "Detection not flagged as not ok to use")
}

/**
*\purpose  Verify that all detections are associated as expected for a moveable obj in scenario D described in document in DFD-175
*\req    NA
*/
TEST(f360_Test_Determine_Association_Hypothesis, Test_Determine_Association_Hypothesis_D_moveable_obj)
{
   /** \precond
   * Set up detections and objects properties accordingly
   */
   objects[0].vcs_position.x = objects[1].vcs_position.x;
   objects[0].speed = 9.5F;
   objects[0].vcs_velocity.longitudinal = objects[0].speed;
   objects[0].Update_Bbox_Center();

   num_dets_in_array = 2U;

   det_p[0].vcs_position.x = objects[0].vcs_position.x;
   det_p[0].vcs_position.y = objects[0].vcs_position.y;
   dets[0].processed.vcs_position_x = det_p[0].vcs_position.x;
   dets[0].processed.vcs_position_y = det_p[0].vcs_position.y;

   /** \action
   * Call function Determine_Association_Hypothesis twice for both test objects
   **/
   for (uint32_t i = 0U; i < 2U; i++)
   {
      Determine_Association_Hypothesis(
         calib,
         sensors,
         host_speed,
         det_idx_array,
         num_dets_in_array,
         i,
         dets,
         sep,
         det_p,
         objects,
         det_score_array,
         det_rdot_comp_array);
   }

   /** \result
   * Check that correct parameters are returned
   **/
   CHECK_EQUAL_TEXT(0, det_p[0].object_track_id, "Detection is not unassociated as expected")
   CHECK_FALSE_TEXT(det_p[0].f_ok_to_use, "Detection not flagged as not ok to use")
}

/**
*\purpose  Verify that all detections are associated as expected for a non-moveable objectin scenario C described in document in DFD-175
*\req    NA
*/
TEST(f360_Test_Determine_Association_Hypothesis, Test_Determine_Association_Hypothesis_C_non_moveable_obj)
{
   /** \precond
   * Set up detections properties accordingly
   */
   objects[0].movable_prob = 0.0F;
   objects[1].vcs_position.x = objects[0].vcs_position.x;
   objects[1].speed = 9.5F;
   objects[1].movable_prob = 0.0F;
   objects[1].vcs_velocity.longitudinal = objects[1].speed;
   objects[1].Update_Bbox_Center();
   num_dets_in_array = 2U;

   det_p[0].vcs_position.x = objects[0].vcs_position.x;
   det_p[0].vcs_position.y = objects[0].vcs_position.y;

   /** \action
   * Call function Determine_Association_Hypothesis twice for both test objects
   **/
   for (uint32_t i = 0U; i < 2U; i++)
   {
      Determine_Association_Hypothesis(
         calib,
         sensors,
         host_speed,
         det_idx_array,
         num_dets_in_array,
         i,
         dets,
         sep,
         det_p,
         objects,
         det_score_array,
         det_rdot_comp_array);
   }

   /** \result
   * Check that correct parameters are returned
   **/
   CHECK_EQUAL_TEXT(0, det_p[0].object_track_id, "Detection is not unassociated as expected")
   CHECK_FALSE_TEXT(det_p[0].f_ok_to_use, "Detection not flagged as not ok to use")
}

/**
*\purpose  Verify that all detections are associated as expected for a non-moveable object in scenario D described in document in DFD-175
*\req    NA
*/
TEST(f360_Test_Determine_Association_Hypothesis, Test_Determine_Association_Hypothesis_D_non_moveable_obj)
{
   /** \precond
   * Set up detections and objects properties accordingly
   */
   objects[1].movable_prob = 0.0F;
   objects[0].vcs_position.x = objects[1].vcs_position.x;
   objects[0].speed = 9.5F;
   objects[0].movable_prob = 0.0F;
   objects[0].vcs_velocity.longitudinal = objects[0].speed;
   objects[0].Update_Bbox_Center();

   num_dets_in_array = 2U;

   det_p[0].vcs_position.x = objects[0].vcs_position.x;
   det_p[0].vcs_position.y = objects[0].vcs_position.y;
   dets[0].processed.vcs_position_x = det_p[0].vcs_position.x;
   dets[0].processed.vcs_position_y = det_p[0].vcs_position.y;

   /** \action
   * Call function Determine_Association_Hypothesis twice for both test objects
   **/
   for (uint32_t i = 0U; i < 2U; i++)
   {
      Determine_Association_Hypothesis(
         calib,
         sensors,
         host_speed,
         det_idx_array,
         num_dets_in_array,
         i,
         dets,
         sep,
         det_p,
         objects,
         det_score_array,
         det_rdot_comp_array);
   }

   /** \result
   * Check that correct parameters are returned
   **/
   CHECK_EQUAL_TEXT(0, det_p[0].object_track_id, "Detection is not unassociated as expected")
   CHECK_FALSE_TEXT(det_p[0].f_ok_to_use, "Detection not flagged as not ok to use")
}

/**
*\purpose  Verify that a detections is allowed to associate when its predicted compensated range rates is not too close to the range rate interval width
*\req    NA
*/
TEST(f360_Test_Determine_Association_Hypothesis, Test_Determine_Association_Hypothesis_Assoc_Not_Blocked_By_Stat_Hypothesis)
{
   /** \precond
   * Objects and detections have been set up in the TEST_GROUP
   * In this test, consider only one detection and one object
   */
   num_dets_in_array = 1U;
   const uint32_t exp_assoc_obj_id = 1U;


   /** \action
   * Call function Determine_Association_Hypothesis once for the first object
   **/
   Determine_Association_Hypothesis(
      calib,
      sensors,
      host_speed,
      det_idx_array,
      num_dets_in_array,
      exp_assoc_obj_id - 1U,
      dets,
      sep,
      det_p,
      objects,
      det_score_array,
      det_rdot_comp_array);

   /** \result
   * Check that the detections is associated to the object
   **/
   CHECK_EQUAL_TEXT(exp_assoc_obj_id, det_p[0].object_track_id, "Detection is not associated as expected")
   CHECK_TRUE_TEXT(det_p[0].f_ok_to_use, "Detection incorrectly flagged as not ok to use")
}

/**
*\purpose  Verify that a detections is not allowed to associate when its predicted compensated range rates is close enough to the range rate interval width
*          and the stationary hypothesis is better than the moving.             
*\req    NA
*/
TEST(f360_Test_Determine_Association_Hypothesis, Test_Determine_Association_Hypothesis_Assoc_Blocked_By_Stat_Hypothesis)
{
   /** \precond
   * Objects and detections have been set up in the TEST_GROUP
   * In this test, consider only one detection and one object
   * Change the sensor's range rate interval width to be close to the speed of the objects
   */
   sensors[0].constant.v_wrapping[0] = 9.9F;
   num_dets_in_array = 1U;
   const int32_t obj_idx = 0;


   /** \action
   * Call function Determine_Association_Hypothesis once for the first object
   **/
   Determine_Association_Hypothesis(
      calib,
      sensors,
      host_speed,
      det_idx_array,
      num_dets_in_array,
      obj_idx,
      dets,
      sep,
      det_p,
      objects,
      det_score_array,
      det_rdot_comp_array);

   /** \result
   * Check that the detection is not associated to the object
   **/
   CHECK_EQUAL_TEXT(F360_INVALID_UNSIGNED_ID, det_p[0].object_track_id, "Detection is unexpectedly associated")
   CHECK_TRUE_TEXT(det_p[0].f_ok_to_use, "Detection incorrectly flagged as not ok to use")
}

/**
*\purpose  Verify that detection range and vcs position are updated correctly when such a detection is dealiased with -1 range rate interval
*          on a SFW radar
*\req    NA
*/
TEST(f360_Test_Determine_Association_Hypothesis, Test_Determine_Association_Hypothesis_SFW_Range_Offset_Negative_Interval)
{
   /** \precond
   * Objects and detections have been set up in the TEST_GROUP
   * In this test, consider only one detection and one object
   * Set the sensor's range rate interval width to be close to the speed of the objects
   * Set the r wrapping to be non-zero which implicates a detection from SFW waveform
   * Configure the range rate dealiasing interval exactly equals -1
   */
   sensors[0].constant.v_wrapping[0] = 9.9F;
   sensors[0].constant.r_wrapping[0] = 0.3F;
   objects[0].vcs_velocity.longitudinal = 0.0F;
   num_dets_in_array = 1U;
   const int32_t obj_idx = 0;

   const float32_t exp_dealiased_range = -0.3F; 
   const float32_t exp_vcs_pos_longitudinal = 9.7F; 


   /** \action
   * Call function Determine_Association_Hypothesis once for the first object
   **/
   Determine_Association_Hypothesis(
      calib,
      sensors,
      host_speed,
      det_idx_array,
      num_dets_in_array,
      obj_idx,
      dets,
      sep,
      det_p,
      objects,
      det_score_array,
      det_rdot_comp_array);

   /** \result
   * Check that detection's dealiased range and vcs posiition are updated correctly
   **/
   CHECK_EQUAL_TEXT(exp_dealiased_range, det_p[0].range_dealiased, "dealiased range is not correct")
   CHECK_EQUAL_TEXT(exp_vcs_pos_longitudinal, det_p[0].vcs_position.x, "detection vcs longitudinal position is not correctly updated")
}

/**
*\purpose  Verify that detection range and vcs position are updated correctly when such a detection is dealiased with +1 range rate interval
*          on a SFW radar
*\req    NA
*/
TEST(f360_Test_Determine_Association_Hypothesis, Test_Determine_Association_Hypothesis_SFW_Range_Positive_Interval)
{
   /** \precond
   * Objects and detections have been set up in the TEST_GROUP
   * In this test, consider only one detection and one object
   * Set the sensor's range rate interval width to be close to the speed of the objects
   * Set the r wrapping to be non-zero which implicates a detection from SFW waveform
   * Configure the range rate dealiasing interval exactly equals +1
   */
   sensors[0].constant.v_wrapping[0] = 9.9F;
   sensors[0].constant.r_wrapping[0] = 0.3F;
   objects[0].vcs_velocity.longitudinal = 20.9F;
   dets[0].raw.range_rate = 9.0F;
   num_dets_in_array = 1U;
   const int32_t obj_idx = 0;

   const float32_t exp_dealiased_range = 0.3F; 
   const float32_t exp_vcs_pos_longitudinal = 10.3F; 


   /** \action
   * Call function Determine_Association_Hypothesis once for the first object
   **/
   Determine_Association_Hypothesis(
      calib,
      sensors,
      host_speed,
      det_idx_array,
      num_dets_in_array,
      obj_idx,
      dets,
      sep,
      det_p,
      objects,
      det_score_array,
      det_rdot_comp_array);

   /** \result
   * Check that detection's dealiased range and vcs posiition are updated correctly
   **/
   CHECK_EQUAL_TEXT(exp_dealiased_range, det_p[0].range_dealiased, "dealiased range is not correct")
   CHECK_EQUAL_TEXT(exp_vcs_pos_longitudinal, det_p[0].vcs_position.x, "detection vcs longitudinal position is not correctly updated")
}

/**
*\purpose  Verify that detection range and vcs position are not updated due to that current association score doesn't exceed
*          that from another object
*\req    NA
*/
TEST(f360_Test_Determine_Association_Hypothesis, Test_Determine_Association_Hypothesis_SFW_No_Range_Offset_Update)
{
   /** \precond
   * Objects and detections have been set up in the TEST_GROUP
   * In this test, consider only one detection and one object
   * Set the sensor's range rate interval width to be close to the speed of the objects
   * Set the r wrapping to be non-zero which implicates a detection from SFW waveform
   * Configure the range rate dealiasing interval exactly equals +1
   */
   sensors[0].constant.v_wrapping[0] = 9.9F;
   sensors[0].constant.r_wrapping[0] = 0.3F;
   objects[0].vcs_velocity.longitudinal = 20.9F;
   dets[0].raw.range_rate = 9.0F;
   num_dets_in_array = 1U;
   det_score_array[0] = 0.001F;  // association score of the previous associated obj
   const int32_t obj_idx = 0;

   const float32_t exp_dealiased_range = 0.0F; 
   const float32_t exp_vcs_pos_longitudinal = 10.0F; 


   /** \action
   * Call function Determine_Association_Hypothesis once for the first object
   **/
   Determine_Association_Hypothesis(
      calib,
      sensors,
      host_speed,
      det_idx_array,
      num_dets_in_array,
      obj_idx,
      dets,
      sep,
      det_p,
      objects,
      det_score_array,
      det_rdot_comp_array);

   /** \result
   * Check that detection's dealiased range and vcs posiition are unchanged
   **/
   CHECK_EQUAL_TEXT(exp_dealiased_range, det_p[0].range_dealiased, "The dealiased range is not expected")
   CHECK_EQUAL_TEXT(exp_vcs_pos_longitudinal, det_p[0].vcs_position.x, "The detection vcs longitudinal position is not expected")
}

/**
*\purpose  Verify that a detections is not allowed to associate when its predicted range rate differs too much from the de-aliased range rate             
*\req    NA
*/
TEST(f360_Test_Determine_Association_Hypothesis, Test_Determine_Association_Hypothesis_Det_Not_Inside_RR_Gate)
{
   /** \precond
   * Objects and detections have been set up in the TEST_GROUP
   * In this test, consider only one detection and one object
   * Set detection's range rate to 3 m/s
   */
   num_dets_in_array = 1U;
   dets[0U].raw.range_rate = 3.0F;
   const int32_t obj_idx = 0;


   /** \action
   * Call function Determine_Association_Hypothesis once for the first object
   **/
   Determine_Association_Hypothesis(
      calib,
      sensors,
      host_speed,
      det_idx_array,
      num_dets_in_array,
      obj_idx,
      dets,
      sep,
      det_p,
      objects,
      det_score_array,
      det_rdot_comp_array);

   /** \result
   * Check that the detection is not associated to the object
   **/
   CHECK_EQUAL_TEXT(F360_INVALID_UNSIGNED_ID, det_p[0].object_track_id, "Detection is unexpectedly associated")
   CHECK_TRUE_TEXT(det_p[0].f_ok_to_use, "Detection incorrectly flagged as not ok to use")
}


/**
*\purpose  Verify that when two objects can associate to a detection, the detections range is only unfolded once for the object that it's finally associated to
*\req    NA
*/
TEST(f360_Test_Determine_Association_Hypothesis, Test_Determine_Association_Hypothesis_Multiple_Unfold_Range)
{
   /** \precond
   * Objects are moving at the same speeds.
   * Both objects fullfill the conditions for association, but object 1 is closer to detection 0. This excites association switching.
   */
   
   sensors[0].constant.v_wrapping[0] = 9.9F;
   sensors[0].constant.r_wrapping[0] = 0.3F;
   objects[0].vcs_velocity.longitudinal = 20.9F;
   objects[1].vcs_velocity.longitudinal = 20.9F;
   dets[0].raw.range_rate = 9.0F;
   num_dets_in_array = 2U;

   // det_p[0] closer to object[1]
   det_p[0].vcs_position.x = 11.5F;
   det_p[0].vcs_position.y = objects[0].vcs_position.y;
   dets[0].processed.vcs_position_x = det_p[0].vcs_position.x;
   dets[0].processed.vcs_position_y = det_p[0].vcs_position.y;
   
   const float32_t exp_dealiased_range = 0.3F; 
   const float32_t exp_det_p0_vcs_position_x = 11.8F;
   const uint32_t exp_assoc_obj_id = 2U;
   
   /** \action
   * Call function Determine_Association_Hypothesis twice for both test objects
   **/
   for (uint32_t i = 0U; i < 2U; i++)
   {
      Determine_Association_Hypothesis(
         calib,
         sensors,
         host_speed,
         det_idx_array,
         num_dets_in_array,
         i,
         dets,
         sep,
         det_p,
         objects,
         det_score_array,
         det_rdot_comp_array);
   }

   /** \result
   * Check that correct parameters are returned
   **/
   CHECK_EQUAL_TEXT(exp_dealiased_range, det_p[0].range_dealiased, "The dealiased range is not expected")
   CHECK_EQUAL_TEXT(exp_det_p0_vcs_position_x, det_p[0].vcs_position.x, "Detection is not dealiased correctly")
   CHECK_EQUAL_TEXT(exp_assoc_obj_id, det_p[0].object_track_id, "Detection is not associated as expected")
   

}

/** @}*/

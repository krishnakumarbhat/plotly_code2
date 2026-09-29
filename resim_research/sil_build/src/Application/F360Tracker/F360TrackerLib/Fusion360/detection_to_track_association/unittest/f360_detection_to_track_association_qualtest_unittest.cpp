/** \file
 * This file contains unit tests to test the requirements connected to f360_detection_to_track_association.cpp file
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
#include "f360_bounding_box.h"

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_Test_Determine_Association_Hypothesis_qualtest
*  @{
*/
/** \brief
*  Set up an object and place detections around it with various properties and check
*  that expected detections are associated to expected parameters.
*  More details can be found in DFD-175
**/
TEST_GROUP(f360_Test_Determine_Association_Hypothesis_qualtest)
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
      Point center;
      for (uint32_t i = 0U; i < num_obj; i++)
      {
         objects[i].id = i + 1;
         objects[i].vcs_position.x = long_pos[i];
         objects[i].vcs_position.y = 0.0F;
         objects[i].vcs_heading = Angle{ 0.0F };
         objects[i].movable_prob = 1.0F;
         objects[i].Set_Bbox_Orientation(Angle{ 0.0F });
         objects[i].Update_Bbox_Size(2.0F, 2.0F);
         objects[i].long_buffer_zone_len1 = 1.0F;
         objects[i].long_buffer_zone_len2 = 1.0F;
         objects[i].lat_buffer_zone_wid1 = 1.0F;
         objects[i].lat_buffer_zone_wid2 = 1.0F;
         objects[i].speed = 10.0F;
         objects[i].vcs_velocity.longitudinal = 10.0F;
         objects[i].vcs_velocity.lateral = 0.0F;
         objects[i].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
         center = objects[i].vcs_position;
         objects[i].bbox.Set_Center(center);
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
         dets[i].processed.cos_vcs_az = 1.0F;
         det_p[i].f_ok_to_use = true;
      }
   }
};

/**
*\purpose  Verify that all detections are associated to the first object in the object list
*\req    CPR-3729, CPR-3850, CPR-3852
*/
TEST(f360_Test_Determine_Association_Hypothesis_qualtest, Test_Determine_Association_Hypothesis_A)
{

   /** \action
   * Call Determine_Association_Hypothesis
   **/
   Determine_Association_Hypothesis(
      calib,
      sensors,
      host_speed,
      det_idx_array,
      num_dets_in_array,
      0,
      dets,
      sep,
      det_p,
      objects,
      det_score_array,
      det_rdot_comp_array);

   /** \result
   * Check that correct parameters are returned
   **/
   for (uint32_t i = 0U; i < num_dets_in_array; i++)
   {
      CHECK_EQUAL_TEXT(1, det_p[i].object_track_id, "Detection not associated with object id 1 as expected")
   }
}

/**
*\purpose  Verify that all detections are associated as expected in scenario B described in document in DFD-175
*\req    CPR-3729, CPR-3850
*/
TEST(f360_Test_Determine_Association_Hypothesis_qualtest, Test_Determine_Association_Hypothesis_B)
{
   /** \precond
   * Set up detections properties accordingly
   */
   objects[0].long_buffer_zone_len2 = 1.5F;
   objects[1].long_buffer_zone_len1 = 1.5F;
   objects[1].lat_buffer_zone_wid1 = 1.5F;

   det_p[0].vcs_position.x = objects[0].vcs_position.x - 1.5F;
   det_p[0].vcs_position.y = objects[0].vcs_position.y + 1.5F;
   det_p[1].vcs_position.x = objects[1].vcs_position.x;
   det_p[2].vcs_position.x = objects[1].vcs_position.x;
   det_p[2].vcs_position.y = objects[1].vcs_position.y - 1.5F;
   det_p[3].vcs_position.x = objects[0].vcs_position.x;
   det_p[4].vcs_position.x = objects[0].vcs_position.x;
   dets[4].raw.range_rate = 0.0F;
   det_p[5].vcs_position.x = objects[1].vcs_position.x;
   det_p[5].f_ok_to_use = false;
   det_p[6].vcs_position.x = 0.5F * (objects[1].vcs_position.x + objects[0].vcs_position.x);
   det_p[7].vcs_position.x = objects[1].vcs_position.x - 0.5F;
   det_p[8].vcs_position.x = objects[0].vcs_position.x + 0.5F;
   det_p[9].vcs_position.x = 0.5F * (objects[1].vcs_position.x + objects[0].vcs_position.x);
   det_p[9].vcs_position.y = objects[1].vcs_position.y - 1.5F;

   // Determine_Association_Hypothesis uses raw dets to compute vcs_position
   for (uint32_t i = 0U; i < num_dets_in_array; i++)
      {
         dets[i].processed.vcs_position_x = det_p[i].vcs_position.x;
         dets[i].processed.vcs_position_y = det_p[i].vcs_position.y;
      }

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
   CHECK_EQUAL_TEXT(1, det_p[0].object_track_id, "Detection not associated to object id 1 as expected")
   CHECK_EQUAL_TEXT(2, det_p[1].object_track_id, "Detection not associated to object id 2 as expected")
   CHECK_EQUAL_TEXT(2, det_p[2].object_track_id, "Detection not associated to object id 2 as expected")
   CHECK_EQUAL_TEXT(1, det_p[3].object_track_id, "Detection not associated to object id 1 as expected")
   CHECK_EQUAL_TEXT(0, det_p[4].object_track_id, "Detection is not unassociated as expected")
   CHECK_EQUAL_TEXT(0, det_p[5].object_track_id, "Detection is not unassociated as expected")
   CHECK_EQUAL_TEXT(1, det_p[6].object_track_id, "Detection not associated to object id 1 as expected")
   CHECK_EQUAL_TEXT(2, det_p[7].object_track_id, "Detection not associated to object id 2 as expected")
   CHECK_EQUAL_TEXT(1, det_p[8].object_track_id, "Detection not associated to object id 1 as expected")
   CHECK_EQUAL_TEXT(2, det_p[9].object_track_id, "Detection not associated to object id 2 as expected")
}

/** @}*/

/** \defgroup  f360_Test_Is_Det_Allowed_To_Associate_qualtest
*  @{
*/
/** \brief
*  Included tests related to calculation of detection is allowed to associate
*  based flags and properties.
**/
TEST_GROUP(f360_Test_Is_Det_Allowed_To_Associate_qualtest)
{

   F360_Detection_Props_T det_prop = {};
   rspp_variant_A::RSPP_Detection_T det_raw{};
   F360_Object_Track_T obj_track = {};
   F360_Calibrations_T calib = {};
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};

   /** \setup
   * Setup test so that the highest complexity branch passes.
   * Then tweak these parameters to reach full branch coverage in
   * each individual test
   **/
   TEST_SETUP()
   {
      // Detection data
      det_raw.processed.f_ok_to_use = true;
      det_raw.raw.f_bistatic = false;
      det_prop.f_ok_to_use = true;
      det_prop.on_sep_id = F360_INVALID_UNSIGNED_ID;
      det_prop.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      det_prop.vcs_position.x = 6.0F;
      det_prop.vcs_position.y = 8.0F;

      // Object data
      obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj_track.f_moving = true;
      obj_track.mirror_prob = 0.0F;
      obj_track.vcs_heading = Angle{ 0.0F };
      obj_track.vcs_position.x = 9.0F;
      obj_track.vcs_position.y = 8.0F;
      obj_track.reference_point = F360_REFERENCE_POINT_CENTER;
      obj_track.bbox.Set_Center(obj_track.vcs_position);
      obj_track.bbox.Set_Orientation(0.0F);
      obj_track.bbox.Set_Length(2.0F);
      obj_track.bbox.Set_Width(1.0F);

      // Init calibrations as defaults
      Initialize_Tracker_Calibrations(calib);
   }
};

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false due to detection being flagged as not ok to use.
*\req    CPR-3731
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate_qualtest, Test_Is_Det_Allowed_To_Associate_Det_Not_Ok_To_Use)
{
   /** \precond
   * Set detections as not ok to use
   **/
   det_prop.f_ok_to_use = false;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false due to detection being flagged as
*          on some SEP and the object is a moving CTCA track in front of the SEP
*\req    CPR-3730
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate_qualtest, Test_Is_Det_Allowed_To_Associate_Det_On_SEP_Obj_Moving_CTCA_In_Front)
{
   /** \precond
   * Set detections to be on some SEP
   * Set up an SEP association box, as a straight line y = p0 = 8
   * Set object to moving and its position to be in front of the SEP (y position < 8)
   **/
   det_prop.on_sep_id = 4;

   sep[det_prop.on_sep_id - 1U].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[det_prop.on_sep_id - 1U].lower_limit = 5.0F;
   sep[det_prop.on_sep_id - 1U].upper_limit = 11.0F;
   sep[det_prop.on_sep_id - 1U].p2 = 0.0F;
   sep[det_prop.on_sep_id - 1U].p1 = 0.0F;
   sep[det_prop.on_sep_id - 1U].p0 = 8.0F;

   obj_track.vcs_position.y = 6.0F;

   /** \action
   * Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false due to detection being flagged as
*          on some SEP and the object is a moving CTCA track behind of the SEP
*\req    CPR-3730
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate_qualtest, Test_Is_Det_Allowed_To_Associate_Det_On_SEP_Obj_Moving_CTCA_Behind)
{
   /** \precond
   * Set detections to be on some SEP
   * Set up an SEP association box, as a straight line y = p0 = 8
   * Set object to moving and its position to be behind the SEP (y position > 8)
   **/
   det_prop.on_sep_id = 4;

   sep[det_prop.on_sep_id - 1U].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[det_prop.on_sep_id - 1U].lower_limit = 5.0F;
   sep[det_prop.on_sep_id - 1U].upper_limit = 11.0F;
   sep[det_prop.on_sep_id - 1U].p2 = 0.0F;
   sep[det_prop.on_sep_id - 1U].p1 = 0.0F;
   sep[det_prop.on_sep_id - 1U].p0 = 8.0F;

   obj_track.vcs_position.y = 10.0F;

   /** \action
   * Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false due to detection being flagged as
*          on some SEP and the object is a moving CTCA track on the SEP
*\req    CPR-3730
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate_qualtest, Test_Is_Det_Allowed_To_Associate_Det_On_SEP_Obj_Moving_CTCA_On)
{
   /** \precond
   * Set detections to be on some SEP
   * Set up an SEP association box, as a straight line y = p0 = 8
   * Set object to moving and its position to be on the SEP (y position == 8)
   **/
   det_prop.on_sep_id = 4;

   sep[det_prop.on_sep_id - 1U].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[det_prop.on_sep_id - 1U].lower_limit = 5.0F;
   sep[det_prop.on_sep_id - 1U].upper_limit = 11.0F;
   sep[det_prop.on_sep_id - 1U].p2 = 0.0F;
   sep[det_prop.on_sep_id - 1U].p1 = 0.0F;
   sep[det_prop.on_sep_id - 1U].p0 = 8.0F;

   obj_track.vcs_position.y = 8.0F;

   /** \action
   * Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false due to detection being flagged as
*          on some SEP and the object is a moving CCA track in front of the SEP
*\req    CPR-3730
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate_qualtest, Test_Is_Det_Allowed_To_Associate_Det_On_SEP_Obj_Moving_CCA_In_Front)
{
   /** \precond
   * Set detections to be on some SEP
   * Set up an SEP association box, as a straight line y = p0 = 8
   * Set object to ba a moving CCA object with high speed (>3m/s) and with position in front of the SEP (y position < 8)
   **/
   det_prop.on_sep_id = 4;

   sep[det_prop.on_sep_id - 1U].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[det_prop.on_sep_id - 1U].lower_limit = 5.0F;
   sep[det_prop.on_sep_id - 1U].upper_limit = 11.0F;
   sep[det_prop.on_sep_id - 1U].p2 = 0.0F;
   sep[det_prop.on_sep_id - 1U].p1 = 0.0F;
   sep[det_prop.on_sep_id - 1U].p0 = 8.0F;

   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj_track.speed = 3.01F;
   obj_track.vcs_position.y = 6.0F;

   /** \action
   * Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false due to detection being flagged as
*          on some SEP and the object is a moving CCA track behind of the SEP
*\req    CPR-3730
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate_qualtest, Test_Is_Det_Allowed_To_Associate_Det_On_SEP_Obj_Moving_CCA_Behind)
{
   /** \precond
   * Set detections to be on some SEP
   * Set up an SEP association box, as a straight line y = p0 = 8
   * Set object to be a moving CCA object with high speed (>3m/s) and with position behind the SEP (y position > 8)
   **/
   det_prop.on_sep_id = 4;

   sep[det_prop.on_sep_id - 1U].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[det_prop.on_sep_id - 1U].lower_limit = 5.0F;
   sep[det_prop.on_sep_id - 1U].upper_limit = 11.0F;
   sep[det_prop.on_sep_id - 1U].p2 = 0.0F;
   sep[det_prop.on_sep_id - 1U].p1 = 0.0F;
   sep[det_prop.on_sep_id - 1U].p0 = 8.0F;

   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj_track.speed = 3.01F;
   obj_track.vcs_position.y = 10.0F;

   /** \action
   * Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false due to detection being flagged as
*          on some SEP and the object is a moving CCA track on the SEP
*\req    CPR-3730
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate_qualtest, Test_Is_Det_Allowed_To_Associate_Det_On_SEP_Obj_Moving_CCA_On)
{
   /** \precond
   * Set detections to be on some SEP
   * Set up an SEP association box, as a straight line y = p0 = 8
   * Set object to be a moving CCA object with high speed (> 3m/s) and with with position to be on the SEP (y position == 8)
   **/
   det_prop.on_sep_id = 4;

   sep[det_prop.on_sep_id - 1U].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[det_prop.on_sep_id - 1U].lower_limit = 5.0F;
   sep[det_prop.on_sep_id - 1U].upper_limit = 11.0F;
   sep[det_prop.on_sep_id - 1U].p2 = 0.0F;
   sep[det_prop.on_sep_id - 1U].p1 = 0.0F;
   sep[det_prop.on_sep_id - 1U].p0 = 8.0F;

   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj_track.speed = 3.01F;
   obj_track.vcs_position.y = 8.0F;

   /** \action
   * Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false due to object having a high mirror probability
*          and detection motion status is ambiguous.
*\req    CPR-3733
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate_qualtest, Test_Is_Det_Allowed_To_Associate_Mirror_Obj_Ambiguous_Detection)
{
   /** \precond
   * Set object mirror probability high
   * Set detections motion status to ambiguous
   **/
   obj_track.mirror_prob = 0.9F;
   det_prop.motion_status =rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false due to object and detection being on opposite sides of a SEP.
*          Detection is behind SEP and object is not.
*\req    CPR-3732
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate_qualtest, Test_Is_Det_Allowed_To_Associate_Detection_Behind_SEP_Obj_Moving)
{
   /** \precond
   * Object set to moving in TEST_SETUP
   * Set up a SEP as a straight line y = 8 and set object position in front of it and detection position behind it
   **/
   det_prop.vcs_position.y = 9.0F;
   
   sep[3].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[3].lower_limit = 5.0F;
   sep[3].upper_limit = 11.0F;
   sep[3].p2 = 0.0F;
   sep[3].p1 = 0.0F;
   sep[3].p0 = 8.0F;

   obj_track.vcs_position.y = 7.0F;
   Point center = obj_track.vcs_position;
   obj_track.bbox.Set_Center(center);

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  This test will test that Is_Det_Allowed_To_Associate() returns false due to moving CTCA object and detection being on opposite sides of a SEP.
*          The object is behind SEP while the detections is not.
*\req    CPR-3732
*/
TEST(f360_Test_Is_Det_Allowed_To_Associate_qualtest, Test_Is_Det_Allowed_To_Associate_Object_Behind_SEP_Obj_Moving_CTCA)
{
   /** \precond
   * Object set to moving in TEST_SETUP
   * Set up a SEP as a straight line y = 8 and set object position behind it and detection position in front of it
   **/
   sep[3].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[3].lower_limit = 5.0F;
   sep[3].upper_limit = 11.0F;
   sep[3].p2 = 0.0F;
   sep[3].p1 = 0.0F;
   sep[3].p0 = 8.0F;

   obj_track.vcs_position.y = 9.0F;
   Point center = obj_track.vcs_position;
   obj_track.bbox.Set_Center(center);

   det_prop.vcs_position.y = 8.0F;

   /** \action
   *Call Is_Det_Allowed_To_Associate
   **/
   bool f_allowed_to_associate = Is_Det_Allowed_To_Associate(
      det_prop,
      det_raw,
      obj_track,
      calib,
      sep);

   /** \result
   * Check that detection flag f_allowed_to_associate is false
   **/
   CHECK_FALSE_TEXT(f_allowed_to_associate, "Detection should not be ok for association")
}

/**
*\purpose  Verify that for the detection associated to object which is not downselected, the Object ID is assigned.
*\req    CPR-3852
*/
TEST(f360_Test_Determine_Association_Hypothesis_qualtest, Test_Determine_Association_Hypothesis_C)
{
   /** \precond
   * Set up non downselected object
   */
   objects[0].num_rr_inlier_dets = 0U;
   
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
   * Check that object ID is assigned to the detection associated to object
   **/
   CHECK_EQUAL_TEXT(1U, det_p[0].object_track_id, "Associated Detection is assigned with object ID as expected")
   CHECK_EQUAL_TEXT(1U, det_p[1].object_track_id, "Associated Detection is assigned with object ID as expected")
}

/**
*\purpose  Verify that for the detection associated to object which is downselected, the Object ID is assigned.
*\req    CPR-3852
*/
TEST(f360_Test_Determine_Association_Hypothesis_qualtest, Test_Determine_Association_Hypothesis_D)
{
   /** \precond
   * Set up downselected object
   */
   objects[0].num_rr_inlier_dets = 1U;
   
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
   * Check that object ID is assigned to the detection associated to object
   **/
   CHECK_EQUAL_TEXT(1U, det_p[0].object_track_id, "Associated Detection is assigned with object ID as expected")
   CHECK_EQUAL_TEXT(1U, det_p[1].object_track_id, "Associated Detection is assigned with object ID as expected")
}
/** @}*/


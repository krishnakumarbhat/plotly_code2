/** \file
 *  This file contains unit tests that verifies the content of f360_dets_inside_bbox.cpp
 */

#include "f360_dets_inside_bbox.h"
#include <CppUTest/TestHarness.h>
#include "math.h"
#include "f360_math.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h"
#include "f360_trk_fltr_ctca_states.h"
#include "f360_trk_fltr_ccv_states.h"
#include "f360_calibrations.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_get_track_bbox_in_vcs.h"
#include "f360_internal_preprocessing.h"
#include <algorithm>

using namespace f360_variant_A;

static void Initialize_Detection_Sensor_Ids(rspp_variant_A::RSPP_Detection_List_T& raw_detect_list)
{
   for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
   {
      raw_detect_list.detections[det_idx].raw.sensor_id = 1U;
   }
}

/** \defgroup  f360_dets_inside_bbox_calc_simple_ext_bbox_and_find_dets_inside
 *  @{
 */

/** \brief
 *  Testing of a function that
 *    1) calculates an extended bounding
 *       box for non-moveable objects based on their speed and distance form host
 *    2) based on detections distnce to object center mark detections as
 *       potential candidates to be associated to track
 **/
TEST_GROUP(f360_dets_inside_bbox_calc_simple_ext_bbox_and_find_dets_inside)
{
   /** \setup
    * Initialize common variables used within all tests for this test group
    **/
   F360_Object_Track_T object_track = {};
   uint32_t num_dets;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   uint32_t dets_in_extbox[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t num_dets_in_extbox;
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};
   F360_Calibrations_T calibs = {};
   float32_t host_dist_to_rear_axle;

   /** \setup
    * Set position and dimension of object. Position of object is set to be more than
    * calibs.k_obj_dist_for_min_assoc_gate_radius_non_moveable away from host to 
    * guarantee we get the largest possible association gates. Object should have reference
    * point CENTER meaning that object center and object vcs_poition should be the same.
    * Distance to hoast rear axle is set to 2.5m.
    * Clear all detections position (i.e. set to [0, 0]). Note: This means that default detection position is outside of the object bbox.
    *
    **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      object_track.vcs_position.x = 60.0F;
      object_track.vcs_position.y = 10.0F;
      object_track.reference_point = F360_REFERENCE_POINT_CENTER;
      object_track.bbox.Set_Center(object_track.vcs_position);
      object_track.bbox.Set_Orientation(Angle{ 0.0F }); // This value are not really relevat but we set it anyways to get a full definition of the object bbbox
      object_track.bbox.Set_Length(calibs.k_nonmoveable_target_diameter);
      object_track.bbox.Set_Width(calibs.k_nonmoveable_target_diameter);

      host_dist_to_rear_axle = 2.5F;

      Initialize_Detection_Sensor_Ids(raw_detect_list);

      for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
      {
         raw_detect_list.detections[det_idx].processed.vcs_position_x = 0.0F;
         raw_detect_list.detections[det_idx].processed.vcs_position_y = 0.0F;
      }
   }

};

/** \purpose
 * Test that function that marks detections as inside gate and marks them as candidates to be associated to object
 * works as expected when number of detections on object exceeds the maximum number of detections that an object can
 * associate to.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_simple_ext_bbox_and_find_dets_inside, calculate_simple_ext_bbox_and_find_dets_inside_all_dets_inside_bbox)
{
   /** \precond
    *  Use set up from test group except for:
    *  Set number of detections to 2 * MAX_DETS_IN_OBJ_TRK
    *  Create num_dets detections on object center
    *  Set expected number of detections in extended bounding box to MAX_DETS_IN_OBJ_TRK.

    * 
    **/
   num_dets = 2U * MAX_DETS_IN_OBJ_TRK;
   raw_detect_list.number_of_valid_detections = num_dets;

   for (uint32_t det_idx = 0U; det_idx < num_dets; det_idx++)
   {
      raw_detect_list.detections[det_idx].processed.vcs_position_x = object_track.vcs_position.x;
      raw_detect_list.detections[det_idx].processed.vcs_position_y = object_track.vcs_position.y;
   }
   
   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_props);

   uint32_t expected_num_dets_in_extbox = num_dets;

   /** \action
    *  Call Calculate_Simple_Ext_Bbox_And_Find_Dets_Inside().
    **/
   Calculate_Simple_Ext_Bbox_And_Find_Dets_Inside(calibs, raw_detect_list, host_dist_to_rear_axle, object_track, det_props, dets_in_extbox, num_dets_in_extbox);

   /** \result
    * Check that number of dets in extended bounding box matches the expected data.
    * Check that all detections are flagged as inside gate and marked as candidates to be associated.
    **/
   CHECK_TRUE_TEXT(expected_num_dets_in_extbox == num_dets_in_extbox, "The number of dets in extbbox is not equal to the expected number of used detections")

   for (uint32_t det_idx = 0U; det_idx < num_dets; det_idx++)
   {
      CHECK_TRUE(det_props[det_idx].f_inside_gate);
      bool f_det_in_extbox = false;
      for (uint32_t det_idx2 = 0U; det_idx2 < num_dets; det_idx2++)
      {
         if (dets_in_extbox[det_idx2] == det_idx)
         {
            f_det_in_extbox = true;
            break;
         }
      }
      CHECK_TRUE(f_det_in_extbox);
   }
}


/** \purpose
 * Test that function that marks detections as inside gate and marks them as candidates to be associated to object
 * works as expected when some detections are far from object and some are close.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_simple_ext_bbox_and_find_dets_inside, calculate_simple_ext_bbox_and_find_dets_inside_dets_inside_and_outside)
{
   /** \precond
    *  An object has been created in test group
    *  Set number of detections to 2 * MAX_DETS_IN_OBJ_TRK
    *  Create 3 detections in object center.
    *  Create one detection just inside object association gate
    *  Create one detection just outside of object association gate
    *  Let remeing detections be far away from object (this is setup in test group already)
    *  Set expected number of detections in extended bounding box to 4
    **/
   num_dets = 2U * MAX_DETS_IN_OBJ_TRK;
   raw_detect_list.number_of_valid_detections = num_dets;

   uint32_t det_1 = 5U;
   uint32_t det_2 = 20U;
   uint32_t det_3 = 50U;
   uint32_t det_4 = 79U;
   uint32_t det_5 = 13U;

   // Set three detection on object center
   raw_detect_list.detections[det_1].processed.vcs_position_x = object_track.vcs_position.x;
   raw_detect_list.detections[det_1].processed.vcs_position_y = object_track.vcs_position.y;

   raw_detect_list.detections[det_2].processed.vcs_position_x = object_track.vcs_position.x;
   raw_detect_list.detections[det_2].processed.vcs_position_y = object_track.vcs_position.y;

   raw_detect_list.detections[det_3].processed.vcs_position_x = object_track.vcs_position.x;
   raw_detect_list.detections[det_3].processed.vcs_position_y = object_track.vcs_position.y;

   
   // Set one detection just inside the association gate
   raw_detect_list.detections[det_4].processed.vcs_position_x = object_track.vcs_position.x + (calibs.k_nonmoveable_target_diameter * 0.5F) + calibs.k_max_assoc_gate_extension_non_moveable - 1e-3;
   raw_detect_list.detections[det_4].processed.vcs_position_y = object_track.vcs_position.y;

   // Set one detection just outside the association gate
   raw_detect_list.detections[det_5].processed.vcs_position_x = object_track.vcs_position.x + (calibs.k_nonmoveable_target_diameter * 0.5F) + calibs.k_max_assoc_gate_extension_non_moveable + 1e-3;
   raw_detect_list.detections[det_5].processed.vcs_position_y = object_track.vcs_position.y;


   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_props);
   uint32_t expected_num_dets_in_extbox = 4U;

   /** \action
    *  Call Calculate_Simple_Ext_Bbox_And_Find_Dets_Inside().
    **/
   Calculate_Simple_Ext_Bbox_And_Find_Dets_Inside(calibs, raw_detect_list, host_dist_to_rear_axle, object_track, det_props, dets_in_extbox, num_dets_in_extbox);

   /** \result
    *  Check that the 4 detections are flagged as inside gate and marked as candidates to be associated.
    **/
   CHECK_TRUE_TEXT(expected_num_dets_in_extbox == num_dets_in_extbox, "The number of dets in extbbox is not equal to the expected number of used detections")
   CHECK_TRUE(det_props[det_1].f_inside_gate)
   CHECK_TRUE(det_props[det_2].f_inside_gate)
   CHECK_TRUE(det_props[det_3].f_inside_gate)
   CHECK_TRUE(det_props[det_4].f_inside_gate)
   CHECK_TRUE(dets_in_extbox[0] == det_1)
   CHECK_TRUE(dets_in_extbox[1] == det_2)
   CHECK_TRUE(dets_in_extbox[2] == det_3)
   CHECK_TRUE(dets_in_extbox[3] == det_4)
}
/** @}*/


/** @}*/

/** \defgroup  f360_dets_inside_bbox_calculate_simple_ext_bbox
 *  @{
 */

/** \brief
 *  Testing of a function that calculates an extended bounding
 *  box (circular shape) for non-moveable objects based on their
 *  distance from host center and their speed
 **/
TEST_GROUP(f360_dets_inside_bbox_calculate_simple_ext_bbox)
{
   /** \setup
    * Initialize common variables used within all tests for this test group
    **/
   F360_Calibrations_T calibs = {};
   F360_Object_Track_T object_track = {};
   float32_t host_dist_to_rear_axle;
   const float32_t TEST_PASS_TH = 1e-6F;
   
   /** \setup
    * Initialize calibration structure to default values
    * Initialze host_dist_to_rear_axle to 2.5m.
    * Create a non-moveable object with
    *    - Position: [-host_dist_to_rear_axle*0.6F, 0] m (i.e. on host center)
    *    - Speed: 0.0m/s
    **/
   TEST_SETUP()
   {
       Initialize_Tracker_Calibrations(calibs);

       host_dist_to_rear_axle = 2.5F;

       object_track.vcs_position.x = -host_dist_to_rear_axle * 0.6F;
       object_track.vcs_position.y = 0.0F;
       object_track.speed = 0.0F;
   }
};

/** \purpose
 * Test that function that calculates a simple version of an extended bounding box for
 * objects works as intended when object is placed on host center and object speed is 
 * 0m/s (i.e. when the contribution to the bbox from both speed and position parts are minimum possible).
 * This tests that bbox is saturated such that is never smaller than a minumum
 * possible value when both speed and position parts are minimum possible.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calculate_simple_ext_bbox, calculate_simple_ext_bbox_low_saturation)
{
   /** \precond
    *  Setup from test group can be used
    **/

   /** \action
    *  Call Calculate_Simple_Ext_Bbox_Buffer_Zones().
    **/
   Calculate_Simple_Ext_Bbox_Buffer_Zones(calibs, host_dist_to_rear_axle, object_track);

   /** \result
    *  Check that long and lat buffer zones corresponds to expected data.
    **/

   DOUBLES_EQUAL_TEXT(calibs.k_min_assoc_gate_extension_non_moveable, object_track.long_buffer_zone_len1, TEST_PASS_TH, "Longitudinal buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(calibs.k_min_assoc_gate_extension_non_moveable, object_track.long_buffer_zone_len2, TEST_PASS_TH, "Longitudinal buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(calibs.k_min_assoc_gate_extension_non_moveable, object_track.lat_buffer_zone_wid1, TEST_PASS_TH, "Lateral buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(calibs.k_min_assoc_gate_extension_non_moveable, object_track.lat_buffer_zone_wid2, TEST_PASS_TH, "Lateral buffer zone does not match expected data.")
}


/** \purpose
 * Test that function that calculates a simple version of an extended bounding box for
 * objects works as intended when object is placed on host center and object speed is 
 * 1m/s (i.e. when the contribution to the bbox position part is minimum possible but from speed part
 * is non-zero). This tests that the speed contribution part is as expected (since 
 * contribution from position part is expected to be calibs.k_min_assoc_gate_extension_non_moveable).
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calculate_simple_ext_bbox, calculate_simple_ext_bbox_small_speed_contribution)
{
   /** \precond
    *  Setup from test group can be used except for change object speed to 1m/s
    **/
   object_track.speed = 1.0F;

   /** \action
    *  Call Calculate_Simple_Ext_Bbox_Buffer_Zones().
    **/
   Calculate_Simple_Ext_Bbox_Buffer_Zones(calibs, host_dist_to_rear_axle, object_track);

   /** \result
    *  Check that long and lat buffer zones corresponds to expected data.
    **/
   const float32_t expected_buffer_zone = calibs.k_min_assoc_gate_extension_non_moveable + 0.5F;
   DOUBLES_EQUAL_TEXT(expected_buffer_zone, object_track.long_buffer_zone_len1, TEST_PASS_TH, "Longitudinal buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_buffer_zone, object_track.long_buffer_zone_len2, TEST_PASS_TH, "Longitudinal buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_buffer_zone, object_track.lat_buffer_zone_wid1, TEST_PASS_TH, "Lateral buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_buffer_zone, object_track.lat_buffer_zone_wid2, TEST_PASS_TH, "Lateral buffer zone does not match expected data.")
}


/** \purpose
 * Test that function that calculates a simple version of an extended bounding box for
 * objects works as intended when object is placed on host center and object speed is 
 * just above 3.3m/s (i.e. when the contribution to the bbox position part is minimum possible
 * but from speed part is non-zero). This tests that the bbox is saturated such that is never 
 * larger than a minumum possible value when all of the contribution comes from the speed part
 *  (since contribution from position part is expected to be calibs.k_min_assoc_gate_extension_non_moveable).
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calculate_simple_ext_bbox, calculate_simple_ext_bbox_large_speed_contribution)
{
   /** \precond
    *  Setup from test group can be used except for change object speed to just above 3.3m/s
    **/
   object_track.speed = 3.3F + 1e-3F;

   /** \action
    *  Call Calculate_Simple_Ext_Bbox_Buffer_Zones().
    **/
   Calculate_Simple_Ext_Bbox_Buffer_Zones(calibs, host_dist_to_rear_axle, object_track);

   /** \result
    *  Check that long and lat buffer zones corresponds to expected data.
    **/
   DOUBLES_EQUAL_TEXT(calibs.k_max_assoc_gate_extension_non_moveable, object_track.long_buffer_zone_len1, TEST_PASS_TH, "Longitudinal buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(calibs.k_max_assoc_gate_extension_non_moveable, object_track.long_buffer_zone_len2, TEST_PASS_TH, "Longitudinal buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(calibs.k_max_assoc_gate_extension_non_moveable, object_track.lat_buffer_zone_wid1, TEST_PASS_TH, "Lateral buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(calibs.k_max_assoc_gate_extension_non_moveable, object_track.lat_buffer_zone_wid2, TEST_PASS_TH, "Lateral buffer zone does not match expected data.")
}


/** \purpose
 * Test that function that calculates a simple version of an extended bounding box for
 * objects works as intended when object is close to host and object speed is 
 * 0m/s (i.e. when the contribution to the bbox from speed part is 0 but from 
 * position part is larger than minimum possible). This tests that the position 
 * contribution part is as expected (since contribution from speed part is expected to be 0).
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calculate_simple_ext_bbox, calculate_simple_ext_bbox_close_to_host)
{
   /** \precond
    *  Setup from test group can be used except for change the object position to [-5, 6]m
    **/
   object_track.vcs_position.x = -5.0F;
   object_track.vcs_position.y = 6.0F;


   /** \action
    *  Call Calculate_Simple_Ext_Bbox_Buffer_Zones().
    **/
   Calculate_Simple_Ext_Bbox_Buffer_Zones(calibs, host_dist_to_rear_axle, object_track);

   /** \result
    *  Check that long and lat buffer zones corresponds to expected data.
    **/
   const float32_t expected_buffer = 0.3292253F;
   DOUBLES_EQUAL_TEXT(expected_buffer, object_track.long_buffer_zone_len1, TEST_PASS_TH, "Longitudinal buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_buffer, object_track.long_buffer_zone_len2, TEST_PASS_TH, "Longitudinal buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_buffer, object_track.lat_buffer_zone_wid1, TEST_PASS_TH, "Lateral buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_buffer, object_track.lat_buffer_zone_wid2, TEST_PASS_TH, "Lateral buffer zone does not match expected data.")
}

/** \purpose
 * Test that function that calculates a simple version of an extended bounding box for
 * objects works as intended when object is far from host and object speed is 
 * 0m/s (i.e. when the contribution to the bbox from speed part is 0 but from 
 * position part is larger than minimum possible). This tests that the position contribution
 *  part is as expected (since contribution from speed part is expected to be 0).
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calculate_simple_ext_bbox, calculate_simple_ext_bbox_far_from_host)
{
   /** \precond
    *  Setup from test group can be used except for change the object position to [45, -23]m
    **/
   object_track.vcs_position.x = 45.0F;
   object_track.vcs_position.y = -23.0F;


   /** \action
    *  Call Calculate_Simple_Ext_Bbox_Buffer_Zones().
    **/
   Calculate_Simple_Ext_Bbox_Buffer_Zones(calibs, host_dist_to_rear_axle, object_track);

   /** \result
    *  Check that long and lat buffer zones corresponds to expected data.
    **/
   DOUBLES_EQUAL_TEXT(calibs.k_max_assoc_gate_extension_non_moveable, object_track.long_buffer_zone_len1, TEST_PASS_TH, "Longitudinal buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(calibs.k_max_assoc_gate_extension_non_moveable, object_track.long_buffer_zone_len2, TEST_PASS_TH, "Longitudinal buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(calibs.k_max_assoc_gate_extension_non_moveable, object_track.lat_buffer_zone_wid1, TEST_PASS_TH, "Lateral buffer zone does not match expected data.")
   DOUBLES_EQUAL_TEXT(calibs.k_max_assoc_gate_extension_non_moveable, object_track.lat_buffer_zone_wid2, TEST_PASS_TH, "Lateral buffer zone does not match expected data.")
}


/** \defgroup  f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth
 *  @{
 */

/** \brief
 *  Testing of a function that calculates longitudinal and lateral buffer zones
 *  depending on distance to object and difference between object vcs pointing
 *  angle and azimuth angle to object.
 **/
TEST_GROUP(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth)
{
   /** \setup
    * Initialize common variables used within all tests for this test group
    **/
   F360_Object_Track_T object_track = {};
   F360_Calibrations_T calibs = {};
   float32_t dist_rear_axle_to_vcs_m = 2.0F;
   float32_t TEST_PASS_TH = 0.01F;

   /** \setup
    * Place an object with 0 vcs pointing angle in VCS
    * Set bbox length to 5m
    * Set bbox width to 2m
    **/
   TEST_SETUP()
   {
      object_track.bbox.Set_Length(5.0F);
      object_track.bbox.Set_Width(2.0F);
      object_track.reference_point = F360_REFERENCE_POINT_CENTER;
      object_track.Set_Bbox_Orientation(Angle{ 0.0F });
      Initialize_Tracker_Calibrations(calibs);
   }

};

/** \purpose
 * Test that function that calculates longitudinal and lateral buffer zones depending on distance to object
 * and difference between object vcs pointing angle and azimuth angle to object works as intended for an
 * object that is slightly more than 100 m away, straight in front of host with vcs pointing angle of zero.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_dependant_on_distance_and_azimuth_diff_object_100p1m_front_object_zero_pointing)
{
   /** \precond
    * Object vcs pointing angle has been set to 0 in test group.
    * Set object longitudinal vcs position from host center to 100.1m.
    * Set object lateral vcs position to 0m.
    * Set initial longitudinal buffer zone to 1m.
    * Set initial lateral buffer zone to 1m.
    * Set expected longitudinal buffer zone to 2m.
    * Set expected lateral buffer zone to 3.79m.
    **/
   object_track.vcs_position.x = 100.1F - dist_rear_axle_to_vcs_m *0.5F;
   object_track.vcs_position.y = 0.0F;
   object_track.bbox.Set_Length(1.0F);
   object_track.bbox.Set_Width(1.0F);
   Point center = {100.1F - dist_rear_axle_to_vcs_m *0.5F, 0.0F};
   object_track.bbox.Set_Center(center);

   float32_t long_buffer = 1.0F;
   float32_t lat_buffer = 1.0F;
   float32_t expected_long_buffer = 2.0F;
   float32_t expected_lat_buffer = 3.79F;
   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Check that buffer zones match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer, long_buffer, TEST_PASS_TH, "Longitudinal buffer zone did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "Lateral buffer zone did not match expected data.")
}


/** \purpose
 * Test that function that calculates longitudinal and lateral buffer zones depending on distance to object
 * and difference between object vcs pointing angle and azimuth angle to object works as intended for an
 * object that is slightly less than 100 m away, straight in front of host with vcs pointing angle of zero.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_dependant_on_distance_and_azimuth_diff_object_99p9m_front_object_zero_pointing)
{
   /** \precond
    * Object vcs pointing angle has been set to 0 in test group.
    * Set object longitudinal vcs position from host center to 99.9m.
    * Set object lateral vcs position to 0m.
    * Set initial longitudinal buffer zone to 1m.
    * Set initial lateral buffer zone to 1m.
    * Set expected longitudinal buffer zone to 1.999m.
    * Set expected lateral buffer zone to 4.33m.
    **/
   object_track.vcs_position.x = 99.9F - dist_rear_axle_to_vcs_m *0.5F;
   object_track.vcs_position.y = 0.0F;
   Point center = {99.9F - dist_rear_axle_to_vcs_m *0.5F, 0.0F};
   object_track.bbox.Set_Center(center);

   float32_t long_buffer = 1.0F;
   float32_t lat_buffer = 1.0F;
   float32_t expected_long_buffer = 1.999F;
   float32_t expected_lat_buffer = 4.33F;
   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Check that buffer zones match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer, long_buffer, TEST_PASS_TH, "Longitudinal buffer zone did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "Lateral buffer zone did not match expected data.")
}


/** \purpose
 * Test that function that calculates longitudinal and lateral buffer zones depending on distance to object
 * and difference between object vcs pointing angle and azimuth angle to object works as intended for an
 * object at 95m (between 90 m and 100 m) away, straight in front of host with vcs pointing angle of zero.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_dependant_on_distance_and_azimuth_diff_object_95m_front_object_zero_pointing)
{
   /** \precond
    * Object vcs pointing angle has been set to 0 in test group.
    * Set object longitudinal vcs position from host center to 95m.
    * Set object lateral vcs position to 0m.
    * Set initial longitudinal buffer zone to 1m.
    * Set initial lateral buffer zone to 1m.
    * Set expected longitudinal buffer zone to 1.95m.
    * Set expected lateral buffer zone to 4.33m.
    **/
   object_track.vcs_position.x = 95.0F - dist_rear_axle_to_vcs_m *0.5F;
   object_track.vcs_position.y = 0.0F;
   Point center = {95.0F - dist_rear_axle_to_vcs_m *0.5F, 0.0F};
   object_track.bbox.Set_Center(center);
   
   float32_t long_buffer = 1.0F;
   float32_t lat_buffer = 1.0F;
   float32_t expected_long_buffer = 1.95F;
   float32_t expected_lat_buffer = 4.33F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Check that buffer zones match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer, long_buffer, TEST_PASS_TH, "Longitudinal buffer zone did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "Lateral buffer zone did not match expected data.")
}


/** \purpose
 * Test that function that calculates longitudinal and lateral buffer zones depending on distance to object
 * and difference between object vcs pointing angle and azimuth angle to object works as intended for an
 * object that is slightly more than 90m away straight in front of host with vcs pointing angle of zero.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_dependant_on_distance_and_azimuth_diff_object_90p1m_front_object_zero_pointing)
{
   /** \precond
    * Object vcs pointing angle has been set to 0 in test group.
    * Set object longitudinal vcs position from host center to 90.1m.
    * Set object lateral vcs position to 0m.
    * Set initial longitudinal buffer zone to 1m.
    * Set initial lateral buffer zone to 1m.
    * Set expected longitudinal buffer zone to 1.901m.
    * Set expected lateral buffer zone to 4.33m.
    **/
   object_track.vcs_position.x = 90.1F - dist_rear_axle_to_vcs_m *0.5F;
   object_track.vcs_position.y = 0.0F;
   Point center = {90.1F - dist_rear_axle_to_vcs_m *0.5F, 0.0F};
   object_track.bbox.Set_Center(center);
   
   float32_t long_buffer = 1.0F;
   float32_t lat_buffer = 1.0F;
   float32_t expected_long_buffer = 1.901F;
   float32_t expected_lat_buffer = 4.33F;
   
   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Check that buffer zones match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer, long_buffer, TEST_PASS_TH, "Longitudinal buffer zone did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "Lateral buffer zone did not match expected data.")
}


/** \purpose
 * Test that function that calculates longitudinal and lateral buffer zones depending on distance to object
 * and difference between object vcs pointing angle and azimuth angle to object works as intended for an
 * object that is slightly less than 90m away straight in front of host with vcs pointing angle of zero.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_dependant_on_distance_and_azimuth_diff_object_89p9m_front_object_zero_pointing)
{
   /** \precond
    * Object vcs pointing angle has been set to 0 in test group.
    * Set object longitudinal vcs position from host center to 89.9m.
    * Set object lateral vcs position to 0m.
    * Set initial longitudinal buffer zone to 1m.
    * Set initial lateral buffer zone to 1m.
    * Set expected longitudinal buffer zone to 1.899m.
    * Set expected lateral buffer zone to 4.33m.
    **/
   object_track.vcs_position.x = 89.9F - dist_rear_axle_to_vcs_m *0.5F;
   object_track.vcs_position.y = 0.0F;
   Point center = {89.9F - dist_rear_axle_to_vcs_m *0.5F, 0.0F};
   object_track.bbox.Set_Center(center);

   float32_t long_buffer = 1.0F;
   float32_t lat_buffer = 1.0F;
   float32_t expected_long_buffer = 1.899F;
   float32_t expected_lat_buffer = 4.33F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Check that buffer zones match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer, long_buffer, TEST_PASS_TH, "Longitudinal buffer zone did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "Lateral buffer zone did not match expected data.")
}

/** \purpose
 * Test that function that calculates longitudinal and lateral buffer zones depending on distance to object
 * and difference between object vcs pointing angle and azimuth angle to object works as intended for an
 * object at 45m  (between 0 m and 90 m) away, straight in front of host with vcs pointing angle of zero.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_dependant_on_distance_and_azimuth_diff_object_45m_front_object_zero_pointing)
{
   /** \precond
    * Object vcs pointing angle has been set to 0 in test group.
    * Set object longitudinal vcs position from host center to 45m .
    * Set object lateral vcs position to 0m.
    * Set initial longitudinal buffer zone to 1m.
    * Set initial lateral buffer zone to 1m.
    * Set expected longitudinal buffer zone to 1.45m.
    * Set expected lateral buffer zone to 3.5m.
    **/
   object_track.vcs_position.x = 45.0F - dist_rear_axle_to_vcs_m *0.5F;
   object_track.vcs_position.y = 0.0F;
   Point center = {45.0F - dist_rear_axle_to_vcs_m *0.5F, 0.0F};
   object_track.bbox.Set_Center(center);

   float32_t long_buffer = 1.0F;
   float32_t lat_buffer = 1.0F;
   float32_t expected_long_buffer = 1.45F;
   float32_t expected_lat_buffer = 3.5F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Check that buffer zones match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer, long_buffer, TEST_PASS_TH, "Longitudinal buffer zone did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "Lateral buffer zone did not match expected data.")
}


/** \purpose
 * Test that function that calculates longitudinal and lateral buffer zones depending on distance to object
 * and difference between object vcs pointing angle and azimuth angle to object works as intended for an
 * object slightly more than 0m away straight in front of host with vcs pointing angle of zero.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_dependant_on_distance_and_azimuth_diff_object_0p1m_front_object_zero_pointing)
{
   /** \precond
    * Object vcs pointing angle has been set to 0 in test group.
    * Set object longitudinal vcs position from host center to 0.1m
    * Set object lateral vcs position to 0m.
    * Set initial longitudinal buffer zone to 1m.
    * Set initial lateral buffer zone to 1m.
    * Set expected longitudinal buffer zone to 1.001m.
    * Set expected lateral buffer zone to 1.005m.
    **/
   object_track.vcs_position.x = 0.1F - dist_rear_axle_to_vcs_m *0.5F;
   object_track.vcs_position.y = 0.0F;
   Point center = {0.1F - dist_rear_axle_to_vcs_m *0.5F, 0.0F};
   object_track.bbox.Set_Center(center);

   float32_t long_buffer = 1.0F;
   float32_t lat_buffer = 1.0F;
   float32_t expected_long_buffer = 1.001F;
   float32_t expected_lat_buffer = 1.005555555555556F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Check that buffer zones match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer, long_buffer, TEST_PASS_TH, "Longitudinal buffer zone did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "Lateral buffer zone did not match expected data.")
}


/** \purpose
 * Test that function that calculates longitudinal and lateral buffer zones depending on distance to object
 * and difference between object vcs pointing angle and azimuth angle to object works as intended for an
 * object at 0m away straight in front of host with vcs pointing angle of zero.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_dependant_on_distance_and_azimuth_diff_object_0m_front_object_zero_pointing)
{
   /** \precond
    * Object vcs pointing angle has been set to 0 in test group.
    * Set object longitudinal vcs position from host center to 0.0m
    * Set object lateral vcs position to 0m.
    * Set initial longitudinal buffer zone to 0.4m.
    * Set initial lateral buffer zone to 0.4m.
    * Set expected longitudinal buffer zone to 0.4m.
    * Set expected lateral buffer zone to 0.4m.
    **/
   object_track.vcs_position.x = 0.0F - dist_rear_axle_to_vcs_m *0.5F;
   object_track.vcs_position.y = 0.0F;
   Point center = {0.0F - dist_rear_axle_to_vcs_m *0.5F, 0.0F};
   object_track.bbox.Set_Center(center);

   float32_t long_buffer = 0.4F;
   float32_t lat_buffer = 0.4F;
   float32_t expected_long_buffer = 0.4F;
   float32_t expected_lat_buffer = 0.4F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Check that buffer zones match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer, long_buffer, TEST_PASS_TH, "Longitudinal buffer zone did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "Lateral buffer zone did not match expected data.")
}


/** \purpose
 * Test that function that calculates longitudinal and lateral buffer zones depending on distance to object
 * and difference between object vcs pointing angle and azimuth angle to object works as intended for a distant
 * object straight left of host with vcs pointing angle of zero.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_dependant_on_distance_and_azimuth_diff_object_far_left_zero_pointing)
{
   /** \precond
    * Object vcs pointing angle has been set to 0 in test group.
    * Set object longitudinal vcs position such that object centroid is straight left of approximated host center.
    * Set object lateral vcs position from host center to -150m.
    * Set initial longitudinal buffer zone to 0m.
    * Set initial lateral buffer zone to 0m.
    * Set expected longitudinal buffer zone to 3.33m.
    * Set expected lateral buffer zone to 1m.
    **/
   object_track.vcs_position.x = 0.0F - 0.5*dist_rear_axle_to_vcs_m;
   object_track.vcs_position.y = -150.0F;
   Point center{0.0F - 0.5F*dist_rear_axle_to_vcs_m, -150.0F};
   object_track.bbox.Set_Center(center);

   float32_t long_buffer = 0.0F;
   float32_t lat_buffer = 0.0F;
   float32_t expected_long_buffer = 3.33F;
   float32_t expected_lat_buffer = 1.0F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Check that buffer zones match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer, long_buffer, TEST_PASS_TH, "Longitudinal buffer zone did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "Lateral buffer zone did not match expected data.")
}


/** \purpose
 * Test that function that calculates longitudinal and lateral buffer zones depending on distance to object
 * and difference between object vcs pointing angle and azimuth angle to object works as intended for a distant
 * object straight right of host with vcs pointing angle of 90 degrees
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_dependant_on_distance_and_azimuth_diff_object_far_right_90_deg_pointing)
{
   /** \precond
    * Set object vcs pointing angle to 90 degrees
    * Set object longitudinal vcs position such that object centroid is straight right of approximated host center.
    * Set object lateral vcs position from host center to -150m.
    * Set initial longitudinal buffer zone to 0m.
    * Set initial lateral buffer zone to 0m.
    * Set expected longitudinal buffer zone to 1m.
    * Set expected lateral buffer zone to 3.33m.
    **/
   object_track.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90) });
   object_track.vcs_position.x = 0.0F - 0.5F*dist_rear_axle_to_vcs_m;
   object_track.vcs_position.y = 150.0F;
   Point center{0.0F - 0.5F*dist_rear_axle_to_vcs_m, -150.0F};
   object_track.bbox.Set_Center(center);

   float32_t long_buffer = 0.0F;
   float32_t lat_buffer = 0.0F;
   float32_t expected_long_buffer = 1.0F;
   float32_t expected_lat_buffer = 3.33F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Check that buffer zones match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer, long_buffer, TEST_PASS_TH, "Longitudinal buffer zone did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "Lateral buffer zone did not match expected data.")
}

/** \purpose
 * Test that function that calculates longitudinal and lateral buffer zones depending on distance to object
 * and difference between object vcs pointing angle and azimuth angle to object works as intended for a
 * object behind and to the right of host with vcs pointing angle of 45 degrees.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_dependant_on_distance_and_azimuth_diff_object_rear_right_45_deg_pointing)
{
   /** \precond
    * Set object vcs pointing angle to 45 degrees
    * Set object longitudinal vcs position such that object centroid 5m behind approximated host center.
    * Set object lateral vcs position to 5m.
    * Set initial longitudinal buffer zone to 0m.
    * Set initial lateral buffer zone to 0m.
    * Set expected longitudinal buffer zone to 0.392m.
    * Set expected lateral buffer zone to 0.070m.
    **/
   object_track.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(45) });
   object_track.vcs_position.x = -5.0F - 0.5*dist_rear_axle_to_vcs_m;
   object_track.vcs_position.y = 5.0F;
   Point center{-5.0F - 0.5F*dist_rear_axle_to_vcs_m, 5.0F};
   object_track.bbox.Set_Center(center);

   float32_t long_buffer = 0.0F;
   float32_t lat_buffer = 0.0F;
   float32_t expected_long_buffer = 0.392837100659193F;
   float32_t expected_lat_buffer = 0.070710678118655F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Check that buffer zones match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer, long_buffer, TEST_PASS_TH, "Longitudinal buffer zone did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "Lateral buffer zone did not match expected data.")
}

/** \purpose
 * Test that tightened azimuth buffer is applied for slow moving objects that meet
 * the criteria (speed 2-7 m/s, distance > 20m) with zero heading rate.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_with_tightened_azimuth_for_slow_moving_object)
{
   /** \precond
    * Set object properties to meet tightened gate criteria:
    * - movable_prob > 0.5
    * - speed between 2-7 m/s
    * - position in front of host at 25m (> 20m threshold)
    * - lateral position within limits
    * - vcs_heading within limits
    * - bbox orientation within limits
    * - time_since_started_move > 0.1s
    * - low tangential acceleration
    * - low range rate error mean
    * - heading_rate = 0
    **/
   object_track.movable_prob = 0.6F;
   object_track.speed = 5.0F;
   object_track.vcs_position.x = 25.0F - dist_rear_axle_to_vcs_m * 0.5F;
   object_track.vcs_position.y = 0.0F;
   Point center = {25.0F - dist_rear_axle_to_vcs_m * 0.5F, 0.0F};
   object_track.bbox.Set_Center(center);
   object_track.vcs_heading = Angle(F360_DEG2RAD(30.0F));
   object_track.Set_Bbox_Orientation(Angle(F360_DEG2RAD(0.0F)));
   object_track.time_since_started_move = 0.2F;
   object_track.tang_accel = 0.5F;
   object_track.filtered_hist_assoc_det_rr_err_mean = 0.5F;
   object_track.heading_rate = 0.0F;

   float32_t long_buffer = 1.0F;
   float32_t lat_buffer = 1.0F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Check that azimuth buffer is tightened. At 25m distance, the tightened buffer
    * should be approximately 0.8 m (interpolated between 0.666 at 20m and 2.6 at 100m).
    * The original azimuth buffer at 25m would be higher.
    * Range buffer component should remain as normal.
    **/
   // Tightened azimuth buffer at 25m: 0.666 + (25-20)/(100-20) * (2.6-0.666) = 0.787
   // Expected lat_buffer = 1.0 + range_component + tightened_azimuth_component
   CHECK_TEXT(lat_buffer < 3.0F, "Lateral buffer should be tightened for slow moving object");
   CHECK_TEXT(lat_buffer > 1.0F, "Lateral buffer should still be increased from initial value");
}

/** \purpose
 * Test that tightened azimuth buffer increases with heading rate for slow moving objects.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_with_tightened_azimuth_and_yaw_increase)
{
   /** \precond
    * Set object properties to meet tightened gate criteria with non-zero heading rate.
    * Place object at far distance (80m) where tightened buffer + yaw increase will be
    * less than normal buffer, so the increase is not capped by std::min.
    **/
   object_track.movable_prob = 0.6F;
   object_track.speed = 5.0F;
   object_track.vcs_position.x = 80.0F - dist_rear_axle_to_vcs_m * 0.5F;
   object_track.vcs_position.y = 0.0F;
   Point center = {80.0F - dist_rear_axle_to_vcs_m * 0.5F, 0.0F};
   object_track.bbox.Set_Center(center);
   object_track.vcs_heading = Angle(F360_DEG2RAD(0.0F));
   object_track.Set_Bbox_Orientation(Angle(F360_DEG2RAD(0.0F)));
   object_track.time_since_started_move = 0.2F;
   object_track.tang_accel = 0.5F;
   object_track.filtered_hist_assoc_det_rr_err_mean = 0.5F;
   object_track.bbox.Set_Length(5.0F);
   object_track.bbox.Set_Width(2.0F);

   // Get baseline with zero heading rate
   object_track.heading_rate = 0.0F;
   float32_t long_buffer_baseline = 1.0F;
   float32_t lat_buffer_baseline = 1.0F;
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer_baseline, lat_buffer_baseline);

   // Test with max heading rate
   object_track.heading_rate = 0.5F;
   float32_t long_buffer_yaw = 1.0F;
   float32_t lat_buffer_yaw = 1.0F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer_yaw, lat_buffer_yaw);

   /** \result
    * The yaw increase of 0.8m is added to tightened azimuth buffer, then the result is
    * capped by std::min with normal azimuth buffer. At 80m distance, the tightened buffer
    * plus yaw increase should still be less than the normal buffer, allowing the increase
    * to be applied. The increase contributes to lateral buffer after rotation.
    * Verify the buffer increases significantly.
    **/
   float32_t lat_buffer_increase = lat_buffer_yaw - lat_buffer_baseline;
   CHECK_TEXT(lat_buffer_increase >= 0.5F, "Lateral buffer should increase significantly with yaw rate at far distance");
   CHECK_TEXT(lat_buffer_yaw > lat_buffer_baseline, "Buffer with yaw should be larger than baseline");
}

/** \purpose
 * Test that tightened azimuth buffer handles negative heading rate correctly.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_with_tightened_azimuth_negative_yaw)
{
   /** \precond
    * Set object properties with negative heading rate.
    **/
   object_track.movable_prob = 0.6F;
   object_track.speed = 5.0F;
   object_track.vcs_position.x = 25.0F - dist_rear_axle_to_vcs_m * 0.5F;
   object_track.vcs_position.y = 0.0F;
   Point center = {25.0F - dist_rear_axle_to_vcs_m * 0.5F, 0.0F};
   object_track.bbox.Set_Center(center);
   object_track.vcs_heading = Angle(F360_DEG2RAD(30.0F));
   object_track.Set_Bbox_Orientation(Angle(F360_DEG2RAD(0.0F)));
   object_track.time_since_started_move = 0.2F;
   object_track.tang_accel = 0.5F;
   object_track.filtered_hist_assoc_det_rr_err_mean = 0.5F;

   // Test with positive heading rate
   object_track.heading_rate = 0.25F;
   float32_t long_buffer_pos = 1.0F;
   float32_t lat_buffer_pos = 1.0F;
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer_pos, lat_buffer_pos);

   // Test with negative heading rate
   object_track.heading_rate = -0.25F;
   float32_t long_buffer_neg = 1.0F;
   float32_t lat_buffer_neg = 1.0F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer_neg, lat_buffer_neg);

   /** \result
    * Both should give same result since absolute value is used.
    **/
   DOUBLES_EQUAL_TEXT(lat_buffer_pos, lat_buffer_neg, TEST_PASS_TH, "Positive and negative yaw should give same buffer");
   DOUBLES_EQUAL_TEXT(long_buffer_pos, long_buffer_neg, TEST_PASS_TH, "Positive and negative yaw should give same buffer");
}

/** \purpose
 * Test that tightened azimuth buffer is NOT applied when object is too close (< 20m).
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_no_tightening_when_object_too_close)
{
   /** \precond
    * Set object properties to meet tightened gate criteria but place at 19m (< 20m threshold).
    **/
   object_track.movable_prob = 0.6F;
   object_track.speed = 5.0F;
   object_track.vcs_position.x = 19.0F - dist_rear_axle_to_vcs_m * 0.5F;
   object_track.vcs_position.y = 0.0F;
   Point center = {19.0F - dist_rear_axle_to_vcs_m * 0.5F, 0.0F};
   object_track.bbox.Set_Center(center);
   object_track.vcs_heading = Angle(F360_DEG2RAD(30.0F));
   object_track.Set_Bbox_Orientation(Angle(F360_DEG2RAD(0.0F)));
   object_track.time_since_started_move = 0.2F;
   object_track.tang_accel = 0.5F;
   object_track.filtered_hist_assoc_det_rr_err_mean = 0.5F;
   object_track.heading_rate = 0.0F;

   float32_t long_buffer = 1.0F;
   float32_t lat_buffer = 1.0F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Normal azimuth buffer should be used, not tightened.
    * The normal buffer at this distance should be higher than the tightened one would be.
    **/
   CHECK_TEXT(lat_buffer > 1.5F, "Should use normal buffer when object too close for tightening");
}

/** \purpose
 * Test that tightened azimuth buffer is NOT applied when object speed is too high (>= 7.0 m/s).
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_no_tightening_when_speed_too_high)
{
   /** \precond
    * Set object at proper distance but speed at upper limit.
    **/
   object_track.movable_prob = 0.6F;
   object_track.speed = 7.0F;
   object_track.vcs_position.x = 25.0F - dist_rear_axle_to_vcs_m * 0.5F;
   object_track.vcs_position.y = 0.0F;
   Point center = {25.0F - dist_rear_axle_to_vcs_m * 0.5F, 0.0F};
   object_track.bbox.Set_Center(center);
   object_track.vcs_heading = Angle(F360_DEG2RAD(30.0F));
   object_track.Set_Bbox_Orientation(Angle(F360_DEG2RAD(0.0F)));
   object_track.time_since_started_move = 0.2F;
   object_track.tang_accel = 0.5F;
   object_track.filtered_hist_assoc_det_rr_err_mean = 0.5F;
   object_track.heading_rate = 0.0F;

   float32_t long_buffer = 1.0F;
   float32_t lat_buffer = 1.0F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Normal buffer should be used, not tightened.
    **/
   CHECK_TEXT(lat_buffer > 1.5F, "Should use normal buffer when speed too high for tightening");
}

/** \purpose
 * Test that tightened azimuth buffer is NOT applied when object speed is too low (<= 2.0 m/s).
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_no_tightening_when_speed_too_low)
{
   /** \precond
    * Set object at proper distance but speed at lower limit.
    **/
   object_track.movable_prob = 0.6F;
   object_track.speed = 2.0F;
   object_track.vcs_position.x = 25.0F - dist_rear_axle_to_vcs_m * 0.5F;
   object_track.vcs_position.y = 0.0F;
   Point center = {25.0F - dist_rear_axle_to_vcs_m * 0.5F, 0.0F};
   object_track.bbox.Set_Center(center);
   object_track.vcs_heading = Angle(F360_DEG2RAD(30.0F));
   object_track.Set_Bbox_Orientation(Angle(F360_DEG2RAD(0.0F)));
   object_track.time_since_started_move = 0.2F;
   object_track.tang_accel = 0.5F;
   object_track.filtered_hist_assoc_det_rr_err_mean = 0.5F;
   object_track.heading_rate = 0.0F;

   float32_t long_buffer = 1.0F;
   float32_t lat_buffer = 1.0F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * Normal buffer should be used, not tightened.
    **/
   CHECK_TEXT(lat_buffer > 1.5F, "Should use normal buffer when speed too low for tightening");
}

/** \purpose
 * Test that tightened azimuth buffer interpolates correctly at far distance (100m).
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_buffer_zones_distance_and_azimuth, calculate_buffer_zones_tightened_azimuth_at_max_distance)
{
   /** \precond
    * Set object properties to meet tightened gate criteria at 100m distance.
    **/
   object_track.movable_prob = 0.6F;
   object_track.speed = 5.0F;
   object_track.vcs_position.x = 100.0F - dist_rear_axle_to_vcs_m * 0.5F;
   object_track.vcs_position.y = 0.0F;
   Point center = {100.0F - dist_rear_axle_to_vcs_m * 0.5F, 0.0F};
   object_track.bbox.Set_Center(center);
   object_track.vcs_heading = Angle(F360_DEG2RAD(30.0F));
   object_track.Set_Bbox_Orientation(Angle(F360_DEG2RAD(0.0F)));
   object_track.time_since_started_move = 0.2F;
   object_track.tang_accel = 0.5F;
   object_track.filtered_hist_assoc_det_rr_err_mean = 0.5F;
   object_track.heading_rate = 0.0F;

   float32_t long_buffer = 1.0F;
   float32_t lat_buffer = 1.0F;

   /** \action
    *  Call Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff().
    **/
   Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibs, object_track, dist_rear_axle_to_vcs_m, long_buffer, lat_buffer);

   /** \result
    * At 100m, the tightened azimuth buffer should be 2.6m.
    * The buffer should still be tightened compared to normal gates.
    **/
   CHECK_TEXT(lat_buffer < 5.0F, "Even at max distance, tightened buffer should be less than normal");
   CHECK_TEXT(lat_buffer > 1.0F, "Buffer should still be increased from baseline");
}
/** @}*/

/** \defgroup f360_dets_inside_bbox_calc_ext_bbox_buffer_zones
 *  @{
 */

/** \brief
 *  Testing of a function that calculates buffer zones for an object's extended bounding box.
 *  The extended bounding box is determined by range to object, difference between object vcs pointing
 *  and azimuth from host to object, the object's position uncertainty and speed.
 **/
TEST_GROUP(f360_dets_inside_bbox_calc_ext_bbox_buffer_zones)
{
   /** \setup
    * Initialize common variables used within all tests for this test group
    **/
   F360_Object_Track_T object_track = {};
   F360_Calibrations_T calibs = {};
   float32_t host_speed = 0.0F;
   Dead_Zone_T dead_zone{};
   const float32_t dist_rear_axle_to_vcs_m = 2.0F;

   const float32_t TEST_PASS_TH = 0.01F;

   /** \setup
    * Set object speed to below calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer
    * Set object longitudinal vcs position 10m from approximated host center.
    * Set object lateral vcs position to 0.
    * Set object vcs pointing to 0.
    * Set bbox length to 5m.
    * Set bbox width to 2m.
    **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      object_track.speed = calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer - 0.1F;
      object_track.vcs_position.x = 10.0F - (0.5 * dist_rear_axle_to_vcs_m);
      object_track.vcs_position.y = 0.0F;
      object_track.Set_Bbox_Orientation(Angle{ 0.0F });
      object_track.bbox.Set_Length(5.0F);
      object_track.bbox.Set_Width(2.0F);
      object_track.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = object_track.vcs_position;
      object_track.bbox.Set_Center(center);
   }
};

/** \purpose
 * Test that function that calculates buffer zones for an object's extended bounding box works as intended for
 * a CTCA object that is located 10m in front of host and is moving with a speed of below calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer.
 * \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_ext_bbox_buffer_zones, calculate_ext_bbox_buffer_zones_CTCA_obj)
{
   /** \precond
    * Set object filter type to CTCA
    * Set expect longitudinal and lateral buffer zones.
    **/
   object_track. trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   float32_t expected_long_buffer = 0.5F;
   float32_t expected_lat_buffer = 0.955555555555556F;

   /** \action
    *  Call Calculate_Ext_Bbox_Buffer_Zones().
    **/
   Calculate_Ext_Bbox_Buffer_Zones(calibs, dist_rear_axle_to_vcs_m, host_speed, dead_zone, object_track);

   /** \result
    * Check that longitudinal and lateral buffer zones match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer, object_track.long_buffer_zone_len1, TEST_PASS_TH, "Object track longitudinal buffer zone does not match expected data")
   DOUBLES_EQUAL_TEXT(expected_long_buffer, object_track.long_buffer_zone_len2, TEST_PASS_TH, "Object track longitudinal buffer zone does not match expected data")
   DOUBLES_EQUAL_TEXT(expected_lat_buffer, object_track.lat_buffer_zone_wid1, TEST_PASS_TH, "Object track lateral buffer zone does not match expected data")
   DOUBLES_EQUAL_TEXT(expected_lat_buffer, object_track.lat_buffer_zone_wid2, TEST_PASS_TH, "Object track lateral buffer zone does not match expected data")
}
/** @}*/

/** \defgroup  f360_dets_inside_bbox_calc_ext_bbox_and_find_dets_inside
 *  @{
 */

/** \brief
 *  Testing of a function that calculates an extended bounding
 *  box for objects based on their length and width and compares radial distance
 *  from object center to detections to mark detections as
 *  potential candiates to be associated to track.
 **/
TEST_GROUP(f360_dets_inside_bbox_calc_ext_bbox_and_find_dets_inside)
{
   /** \setup
    * Initialize common variables used within all tests for this test group
    **/
   const float32_t dist_rear_axle_to_vcs_m = 2.0F;
   uint32_t num_dets;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Object_Track_T object_track = {};
   uint32_t dets_in_extbox[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t num_dets_in_extbox;
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};
   F360_Calibrations_T calibs = {};

   float32_t host_speed = 0.0F;
   Dead_Zone_T dead_zone{};
   /** \setup
    * Set object filter type to CTCA
    * Set object speed to below calibs.k_max_time_extending_moveable_ccv_assoc_gates
    * Set object longitudinal vcs position 10m from approximated host center.
    * Set object lateral vcs position to 0.
    * Set object vcs pointing to 0.
    * Set object dimensions to (2,2,2,2) for len1, len2, wid1 and wid2
    *
    **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs); 

      Initialize_Detection_Sensor_Ids(raw_detect_list);

      object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      object_track.speed = 5.0;
      object_track.vcs_position.x = 10.0F - (0.5 * dist_rear_axle_to_vcs_m);
      object_track.vcs_position.y = 0.0F;

      object_track.Set_Bbox_Orientation(Angle{ 0.0F });
      object_track.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = object_track.vcs_position;
      object_track.bbox.Set_Center(center);
      object_track.bbox.Set_Length(4.0F);
      object_track.bbox.Set_Width(4.0F);
   }

   // Helper function to add detections on object corners
   void Add_Detection_On_Front_Right_Corner_Of_Object(uint32_t det_idx)
   {
      Convert_TCS_Posn_To_VCS_Posn(object_track.bbox.Get_Length() * 0.5F,
                                   object_track.bbox.Get_Width() * 0.5F,
                                   object_track.bbox.Get_Center().x,
                                   object_track.bbox.Get_Center().y,
                                   object_track.bbox.Get_Orientation(),
                                   raw_detect_list.detections[det_idx].processed.vcs_position_x,
                                   raw_detect_list.detections[det_idx].processed.vcs_position_y);
   }

   void Add_Detection_On_Front_Right_Corner_Of_Object_With_Offset(uint32_t det_idx, float32_t offset_x, float32_t offset_y)
   {
      Convert_TCS_Posn_To_VCS_Posn(object_track.bbox.Get_Length() * 0.5F + offset_x,
         object_track.bbox.Get_Width() * 0.5F + offset_y,
         object_track.bbox.Get_Center().x,
         object_track.bbox.Get_Center().y,
         object_track.bbox.Get_Orientation(),
         raw_detect_list.detections[det_idx].processed.vcs_position_x,
         raw_detect_list.detections[det_idx].processed.vcs_position_y);
   }

   void Add_Detection_On_Front_Left_Corner_Of_Object(uint32_t det_idx)
   {
      Convert_TCS_Posn_To_VCS_Posn(object_track.bbox.Get_Length() * 0.5F,
                                   -object_track.bbox.Get_Width() * 0.5F,
                                   object_track.bbox.Get_Center().x,
                                   object_track.bbox.Get_Center().y,
                                   object_track.bbox.Get_Orientation(),
                                   raw_detect_list.detections[det_idx].processed.vcs_position_x,
                                   raw_detect_list.detections[det_idx].processed.vcs_position_y);
   }

   void Add_Detection_On_Rear_Right_Corner_Of_Object(uint32_t det_idx)
   {
      Convert_TCS_Posn_To_VCS_Posn(-object_track.bbox.Get_Length() * 0.5F,
                                   object_track.bbox.Get_Width() * 0.5F,
                                   object_track.bbox.Get_Center().x,
                                   object_track.bbox.Get_Center().y,
                                   object_track.bbox.Get_Orientation(),
                                   raw_detect_list.detections[det_idx].processed.vcs_position_x,
                                   raw_detect_list.detections[det_idx].processed.vcs_position_y);
   }

   void Add_Detection_On_Rear_Left_Corner_Of_Object(uint32_t det_idx)
   {
      Convert_TCS_Posn_To_VCS_Posn(-object_track.bbox.Get_Length() * 0.5F,
                                   -object_track.bbox.Get_Width() * 0.5F,
                                   object_track.bbox.Get_Center().x,
                                   object_track.bbox.Get_Center().y,
                                   object_track.bbox.Get_Orientation(),
                                   raw_detect_list.detections[det_idx].processed.vcs_position_x,
                                   raw_detect_list.detections[det_idx].processed.vcs_position_y);
   }
};

/** \purpose
 * Test that function that calculates an extended bounding box for an object and marks detections that are
 * inside the extended bounding box as inside gate and potential candidates for association works as
 * expected when there are more detections inside bounding box than that can be associated to object.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_ext_bbox_and_find_dets_inside, calculate_ext_bbox_and_find_dets_inside_all_dets_inside_bbox)
{
   /** \precond
    *  An object for which the longitudinal and lateral buffer zones will be 0.5m and 0.955555555555556 respectively has been set up in test group.
    *  Object track pointing and vcs pointing have been set to 0 in test group.
    *  Set number of detections to 2 * MAX_DETS_IN_OBJ_TRK
    *  Place num_dets detections on the object's corners
    *  Set expected number of dets in extended bounding box to MAX_DETS_IN_OBJ_TRK
    **/
   num_dets = 2U * MAX_DETS_IN_OBJ_TRK;
   raw_detect_list.number_of_valid_detections = num_dets;
   for (uint32_t det_idx = 0U; det_idx < num_dets; det_idx = det_idx + 4U)
   {
      Add_Detection_On_Front_Right_Corner_Of_Object(det_idx);
      Add_Detection_On_Front_Left_Corner_Of_Object(det_idx + 1);
      Add_Detection_On_Rear_Right_Corner_Of_Object(det_idx + 2);
      Add_Detection_On_Rear_Left_Corner_Of_Object(det_idx + 3);
   }
   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_props);

   uint32_t expected_num_dets_in_extbox = num_dets;

   /** \action
    *  Call Calculate_Ext_Bbox_And_Find_Dets_Inside().
    **/
   Calculate_Ext_Bbox_And_Find_Dets_Inside(calibs, dist_rear_axle_to_vcs_m, host_speed, raw_detect_list, dead_zone, det_props, object_track, dets_in_extbox, num_dets_in_extbox);

   /** \result
    * Check that number of dets in extended bounding box matches the expected data
    * Check that all detections are flagged as inside gate and marked as candidates to be associated.
    **/
   CHECK_TRUE_TEXT(expected_num_dets_in_extbox == num_dets_in_extbox, "The number of dets in extbbox is not equal to the expected number of used detections")

   for (uint32_t det_idx_extbox = 0U; det_idx_extbox < num_dets; det_idx_extbox++)
   {
      CHECK_TRUE(det_props[det_idx_extbox].f_inside_gate);

      const uint32_t* const begin = &dets_in_extbox[0];
      const uint32_t* const end = &dets_in_extbox[num_dets_in_extbox];
      const uint32_t* const result = std::find(begin, end, det_idx_extbox);
      const bool f_found = (result != end);
      CHECK_TRUE_TEXT(f_found, "Detection is not found in extbbox list");
   }
}

/** \purpose
 * Test that function that calculates an extended bounding box for an object and marks detections that are
 * inside the extended bounding box and marked as water spray as inside gate and potential candidates for association works as
 * expected when there are more detections inside bounding box than that can be associated to object.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_ext_bbox_and_find_dets_inside, calculate_ext_bbox_and_find_dets_inside_waterspray_in_zone)
{
   /** \precond
    *  An object for which the longitudinal and lateral buffer zones will be 0.5m and 0.955555555555556 respectively has been set up in test group.
    *  Object track pointing and vcs pointing have been set to 0 in test group.
    *  Set number of detections to 2 * MAX_DETS_IN_OBJ_TRK
    *  Place num_dets detections on the object's corners
    *  Set expected number of dets in extended bounding box to MAX_DETS_IN_OBJ_TRK
    *  Set last detection to be at the edge of limits
    **/
   num_dets = 2U * MAX_DETS_IN_OBJ_TRK;
   raw_detect_list.number_of_valid_detections = num_dets;
   for (uint32_t det_idx = 0U; det_idx < num_dets; det_idx = det_idx + 4U)
   {
      Add_Detection_On_Front_Right_Corner_Of_Object(det_idx);
      Add_Detection_On_Front_Left_Corner_Of_Object(det_idx + 1);
      Add_Detection_On_Rear_Right_Corner_Of_Object(det_idx + 2);
      Add_Detection_On_Rear_Left_Corner_Of_Object(det_idx + 3);

      det_props[det_idx].f_water_spray = true;
      det_props[det_idx + 1].f_water_spray = true;
      det_props[det_idx + 2].f_water_spray = true;
      det_props[det_idx + 3].f_water_spray = true;
   }
   Add_Detection_On_Front_Right_Corner_Of_Object_With_Offset(num_dets - 1,
      0.5F*calibs.k_ws_bbox_len_extension_factor - 0.0001F,
      0.955555555555556F*calibs.k_ws_bbox_wid_extension_factor - 0.0001F);

   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_props);

   uint32_t expected_num_dets_in_extbox = num_dets;

   /** \action
    *  Call Calculate_Ext_Bbox_And_Find_Dets_Inside().
    **/
   Calculate_Ext_Bbox_And_Find_Dets_Inside(calibs, dist_rear_axle_to_vcs_m, host_speed, raw_detect_list, dead_zone, det_props, object_track, dets_in_extbox, num_dets_in_extbox);

   /** \result
    * Check that number of dets in extended bounding box matches the expected data
    * Check that all detections are flagged as inside gate and marked as candidates to be associated.
    **/
   CHECK_TRUE_TEXT(expected_num_dets_in_extbox == num_dets_in_extbox, "The number of dets in extbbox is not equal to the expected number of used detections")

   for (uint32_t det_idx_extbox = 0U; det_idx_extbox < num_dets; det_idx_extbox++)
   {
      CHECK_TRUE(det_props[det_idx_extbox].f_inside_gate);

      const uint32_t* const begin = &dets_in_extbox[0];
      const uint32_t* const end = &dets_in_extbox[num_dets_in_extbox];
      const uint32_t* const result = std::find(begin, end, det_idx_extbox);
      const bool f_found = (result != end);
      CHECK_TRUE_TEXT(f_found, "Detection is not found in extbbox list");
   }
}

/** \purpose
 * Test that function that calculates an extended bounding box for an object and marks detections that are
 * inside the extended bounding box and marked as water spray as inside gate and potential candidates for association works as
 * expected when there are more detections inside bounding box than that can be associated to object. The last detection should not be
 * inside extended bounding box
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_ext_bbox_and_find_dets_inside, calculate_ext_bbox_and_find_dets_inside_waterspray_out_of_zone_longitudinally)
{
   /** \precond
    *  An object for which the longitudinal and lateral buffer zones will be 0.5m and 0.955555555555556 respectively has been set up in test group.
    *  Object track pointing and vcs pointing have been set to 0 in test group.
    *  Set number of detections to 2 * MAX_DETS_IN_OBJ_TRK
    *  Place num_dets detections on the object's corners
    *  Set expected number of dets in extended bounding box to MAX_DETS_IN_OBJ_TRK
    *  Set last detection to be longittudinally out of zone
    **/
   num_dets = 2U * MAX_DETS_IN_OBJ_TRK;
   raw_detect_list.number_of_valid_detections = num_dets;

   for (uint32_t det_idx = 0U; det_idx < num_dets; det_idx = det_idx + 4U)
   {
      Add_Detection_On_Front_Right_Corner_Of_Object(det_idx);
      Add_Detection_On_Front_Left_Corner_Of_Object(det_idx + 1);
      Add_Detection_On_Rear_Right_Corner_Of_Object(det_idx + 2);
      Add_Detection_On_Rear_Left_Corner_Of_Object(det_idx + 3);

      det_props[det_idx].f_water_spray = true;
      det_props[det_idx + 1].f_water_spray = true;
      det_props[det_idx + 2].f_water_spray = true;
      det_props[det_idx + 3].f_water_spray = true;
   }

   const uint32_t det_idx_can_not_associate = num_dets - 1;
   Add_Detection_On_Front_Right_Corner_Of_Object_With_Offset(det_idx_can_not_associate,
      0.5F*calibs.k_ws_bbox_len_extension_factor + 0.0001F,
      0.955555555555556F*calibs.k_ws_bbox_wid_extension_factor - 0.0001F);

   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_props);

   const uint32_t expected_num_dets_in_extbox = num_dets - 1;

   /** \action
    *  Call Calculate_Ext_Bbox_And_Find_Dets_Inside().
    **/
   Calculate_Ext_Bbox_And_Find_Dets_Inside(calibs, dist_rear_axle_to_vcs_m, host_speed, raw_detect_list, dead_zone, det_props, object_track, dets_in_extbox, num_dets_in_extbox);

   /** \result
    * Check that number of dets in extended bounding box matches the expected data
    * Check that all detections are flagged as inside gate and marked as candidates to be associated.
    **/
   CHECK_TRUE_TEXT(expected_num_dets_in_extbox == num_dets_in_extbox, "The number of dets in extbbox is not equal to the expected number of used detections")
   CHECK_FALSE(det_props[num_dets - 1].f_inside_gate);

   for (uint32_t det_idx_extbox = 0U; det_idx_extbox < det_idx_can_not_associate; det_idx_extbox++)
   {
      CHECK_TRUE(det_props[det_idx_extbox].f_inside_gate);

      const uint32_t* const begin = &dets_in_extbox[0];
      const uint32_t* const end = &dets_in_extbox[num_dets_in_extbox];
      const uint32_t* const result = std::find(begin, end, det_idx_extbox);
      const bool f_found = (result != end);
      CHECK_TRUE_TEXT(f_found, "Detection is not found in extbbox list");
   }

   const uint32_t* const begin = &dets_in_extbox[0];
   const uint32_t* const end = &dets_in_extbox[num_dets_in_extbox];
   const uint32_t* const result = std::find(begin, end, det_idx_can_not_associate);
   const bool f_found = (result != end);
   CHECK_FALSE_TEXT(f_found, "Detection is found be in extbbox list and it shouldn't be there");
}

/** \purpose
 * Test that function that calculates an extended bounding box for an object and marks detections that are
 * inside the extended bounding box and marked as water spray as inside gate and potential candidates for association works as
 * expected when there are more detections inside bounding box than that can be associated to object. The last detection should not be
 * inside extended bounding box
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_ext_bbox_and_find_dets_inside, calculate_ext_bbox_and_find_dets_inside_waterspray_out_of_zone_laterally)
{
   /** \precond
    *  An object for which the longitudinal and lateral buffer zones will be 0.5m and 0.955555555555556 respectively has been set up in test group.
    *  Object track pointing and vcs pointing have been set to 0 in test group.
    *  Set number of detections to 2 * MAX_DETS_IN_OBJ_TRK
    *  Place num_dets detections on the object's corners
    *  Set expected number of dets in extended bounding box to MAX_DETS_IN_OBJ_TRK
    *  Set last detection to be laterally out of zone
    **/
   num_dets = 2U * MAX_DETS_IN_OBJ_TRK;
   raw_detect_list.number_of_valid_detections = num_dets;
   for (uint32_t det_idx = 0U; det_idx < num_dets; det_idx = det_idx + 4U)
   {
      Add_Detection_On_Front_Right_Corner_Of_Object(det_idx);
      Add_Detection_On_Front_Left_Corner_Of_Object(det_idx + 1);
      Add_Detection_On_Rear_Right_Corner_Of_Object(det_idx + 2);
      Add_Detection_On_Rear_Left_Corner_Of_Object(det_idx + 3);

      det_props[det_idx].f_water_spray = true;
      det_props[det_idx + 1].f_water_spray = true;
      det_props[det_idx + 2].f_water_spray = true;
      det_props[det_idx + 3].f_water_spray = true;
   }

   const uint32_t det_idx_can_not_associate = num_dets - 1;

   Add_Detection_On_Front_Right_Corner_Of_Object_With_Offset(det_idx_can_not_associate,
      0.5F*calibs.k_ws_bbox_len_extension_factor - 0.0001F,
      0.955555555555556F*calibs.k_ws_bbox_wid_extension_factor + 0.0001F);

   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_props);

   const uint32_t expected_num_dets_in_extbox = num_dets - 1;

   /** \action
    *  Call Calculate_Ext_Bbox_And_Find_Dets_Inside().
    **/
   Calculate_Ext_Bbox_And_Find_Dets_Inside(calibs, dist_rear_axle_to_vcs_m, host_speed, raw_detect_list, dead_zone, det_props, object_track, dets_in_extbox, num_dets_in_extbox);

   /** \result
    * Check that number of dets in extended bounding box matches the expected data
    * Check that all detections are flagged as inside gate and marked as candidates to be associated.
    **/
   CHECK_TRUE_TEXT(expected_num_dets_in_extbox == num_dets_in_extbox, "The number of dets in extbbox is not equal to the expected number of used detections")

   CHECK_FALSE(det_props[num_dets - 1].f_inside_gate);

   for (uint32_t det_idx_extbox = 0U; det_idx_extbox < det_idx_can_not_associate; det_idx_extbox++)
   {
      CHECK_TRUE(det_props[det_idx_extbox].f_inside_gate);

      const uint32_t* const begin = &dets_in_extbox[0];
      const uint32_t* const end = &dets_in_extbox[num_dets_in_extbox];
      const uint32_t* const result = std::find(begin, end, det_idx_extbox);
      const bool f_found = (result != end);
      CHECK_TRUE_TEXT(f_found, "Detection is not found in extbbox list");
   }

   const uint32_t* const begin = &dets_in_extbox[0];
   const uint32_t* const end = &dets_in_extbox[num_dets_in_extbox];
   const uint32_t* const result = std::find(begin, end, det_idx_can_not_associate);
   const bool f_found = (result != end);
   CHECK_FALSE_TEXT(f_found, "Detection is found be in extbbox list and it shouldn't be there");
}

/** \purpose
 * Test that function that calculates an extended bounding box for an object and marks detections that are
 * inside the extended bounding box as inside gate and potential candidates for association works as
 * expected when object is behind host.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_ext_bbox_and_find_dets_inside, calculate_ext_bbox_and_find_dets_inside_all_dets_inside_bbox_obj_behind_host)
{
   /** \precond
    *  Object track pointing and vcs pointing have been set to 0 in test group.
    *  Set object position long/lat position to -10/-5m
    *  Set number of detections to 8
    *  Place 4 detection inside object extended bounding box
    *  Set expected number of dets in extended bounding box to 4
    **/
   object_track.vcs_position.x = -10.0F;
   object_track.vcs_position.y = -5.0F;
   object_track.Update_Bbox_Center();

   num_dets = 8U;
   raw_detect_list.number_of_valid_detections = num_dets;
   uint32_t det_1 = 1U;
   uint32_t det_2 = 2U;
   uint32_t det_3 = 3U;
   uint32_t det_4 = 4U;

   Add_Detection_On_Front_Right_Corner_Of_Object(det_4);
   raw_detect_list.detections[det_4].processed.vcs_position_x += 0.1F;
   Add_Detection_On_Front_Left_Corner_Of_Object(det_3);
   Add_Detection_On_Rear_Right_Corner_Of_Object(det_2);
   Add_Detection_On_Rear_Left_Corner_Of_Object(det_1);
   raw_detect_list.detections[det_1].processed.vcs_position_x += 0.1F;

   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_props);

   uint32_t expected_num_dets_in_extbox = 4U;

   /** \action
    *  Call Calculate_Ext_Bbox_And_Find_Dets_Inside().
    **/
   Calculate_Ext_Bbox_And_Find_Dets_Inside(calibs, dist_rear_axle_to_vcs_m, host_speed, raw_detect_list, dead_zone, det_props, object_track, dets_in_extbox, num_dets_in_extbox);

   /** \result
    * Check that number of dets in extended bounding box is 4
    * Check that correct detections are flagged as inside gate and marked as candidates to be associated.
    * Check that remaining detections were not marked as inside gate as object can't associate to more detections.
    **/
   CHECK_TRUE_TEXT(expected_num_dets_in_extbox == num_dets_in_extbox, "The number of dets in extbbox is not equal to the expected number of used detections")

   CHECK_TRUE(det_props[det_1].f_inside_gate);
   CHECK_TRUE(det_props[det_2].f_inside_gate);
   CHECK_TRUE(det_props[det_3].f_inside_gate);
   CHECK_TRUE(det_props[det_4].f_inside_gate);

   // Detections are sorted longitudinally.
   CHECK_TRUE(dets_in_extbox[0] == det_2);
   CHECK_TRUE(dets_in_extbox[1] == det_1);
   CHECK_TRUE(dets_in_extbox[2] == det_3);
   CHECK_TRUE(dets_in_extbox[3] == det_4);

   for (uint32_t det_idx = num_dets; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
   {
      CHECK_FALSE(det_props[det_idx].f_inside_gate);
   }
}

/** \purpose
 * Test that function that calculates an extended bounding box for an object and marks detections that are
 * inside the extended bounding box as inside gate and potential candidates for association works as
 * expected when some detections are far from object and some are close.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_ext_bbox_and_find_dets_inside, calculate_ext_bbox_and_find_dets_inside_dets_inside_and_outside_box)
{
   /** \precond
    *  An object for which the longitudinal and lateral buffer zones will be 0.5m and 0.955555555555556 respectively has been set up in test group.
    *  Set number of detections to 2 * MAX_DETS_IN_OBJ_TRK
    *  Create 4 detections close to object corners
    *  Set expected number of detections in extended bounding box to 4
    **/
   num_dets = 2U * MAX_DETS_IN_OBJ_TRK;
   raw_detect_list.number_of_valid_detections = num_dets;
   uint32_t det_1 = 5U;
   uint32_t det_2 = 20U;
   uint32_t det_3 = 50U;
   uint32_t det_4 = 79U;

   Add_Detection_On_Front_Right_Corner_Of_Object(det_1);
   raw_detect_list.detections[det_1].processed.vcs_position_x += 0.49F; // Move detection slightly outside solid bounding box (but still inside extended bounding box)
   Add_Detection_On_Front_Left_Corner_Of_Object(det_2);
   raw_detect_list.detections[det_2].processed.vcs_position_y -= 0.90F; // Move detection slightly inside solid bounding box
   Add_Detection_On_Rear_Right_Corner_Of_Object(det_3);
   raw_detect_list.detections[det_3].processed.vcs_position_y -= 0.3F; // Move detection slightly inside solid box
   Add_Detection_On_Rear_Left_Corner_Of_Object(det_4);
   raw_detect_list.detections[det_4].processed.vcs_position_x += 0.5F; // Move detection slightly inside solid box

   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_props);

   uint32_t expected_num_dets_in_extbox = 4U;
   /** \action
    *  Call Calculate_Ext_Bbox_And_Find_Dets_Inside().
    **/
   Calculate_Ext_Bbox_And_Find_Dets_Inside(calibs, dist_rear_axle_to_vcs_m, host_speed, raw_detect_list, dead_zone, det_props, object_track, dets_in_extbox, num_dets_in_extbox);

   /** \result
    *  Check that the 4 detections are flagged as inside gate and marked as candidates to be associated.
    **/
   CHECK_TRUE_TEXT(expected_num_dets_in_extbox == num_dets_in_extbox, "The number of dets in extbbox is not equal to the expected number of used detections")
   CHECK_TRUE(det_props[det_1].f_inside_gate)
   CHECK_TRUE(det_props[det_2].f_inside_gate)
   CHECK_TRUE(det_props[det_3].f_inside_gate)
   CHECK_TRUE(det_props[det_4].f_inside_gate)
   CHECK_TRUE(dets_in_extbox[0] == det_3)
   CHECK_TRUE(dets_in_extbox[1] == det_4)
   CHECK_TRUE(dets_in_extbox[2] == det_2)
   CHECK_TRUE(dets_in_extbox[3] == det_1)
}

/** \purpose
 * Test that function that calculates an extended bounding box for an object and marks detections that are
 * inside the extended bounding box as inside gate and potential candidates for association works as
 * expected when some detections are far from object and some are close and object is rotated 180 degrees
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_calc_ext_bbox_and_find_dets_inside, calculate_ext_bbox_and_find_dets_inside_dets_inside_and_outside_box_rotated_obj)
{
   /** \precond
    * An object for which the longitudinal and lateral buffer zones will be 0.5m and 0.955555555555556 respectively has been set up in test group.
    * Set object vcs pointing to 180 degrees (does not change long and lat assoc buffers)
    * Modify object len1/len2/wid1/wid2 to correspond to this rotation.
    * Set number of detections to 2 * MAX_DETS_IN_OBJ_TRK
    * Create 2 detections just inside extended bounding box
    * Create 2 detections just outside extended bounding box
    * Set expected number of detections in extended bounding box to 2
    **/
   object_track.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(180.0F) });

   num_dets = 2U * MAX_DETS_IN_OBJ_TRK;
   raw_detect_list.number_of_valid_detections = num_dets;

   uint32_t det_1 = 5U;
   uint32_t det_2 = 20U;
   uint32_t det_3 = 50U;
   uint32_t det_4 = 79U;

   // Two detections barely inside extended bounding box
   Add_Detection_On_Front_Right_Corner_Of_Object(det_1);
   raw_detect_list.detections[det_1].processed.vcs_position_x -= 0.49F; // Move detection such that it is barely inside extended box
   Add_Detection_On_Front_Left_Corner_Of_Object(det_2);
   raw_detect_list.detections[det_2].processed.vcs_position_y += 0.95F; // Move detection such that it is barely inside extended box

   // Two detections barely outside extended bounding box
   Add_Detection_On_Rear_Right_Corner_Of_Object(det_3);
   raw_detect_list.detections[det_3].processed.vcs_position_y -= 0.96F; // Move detection such that it is barely outside extended box
   Add_Detection_On_Rear_Left_Corner_Of_Object(det_4);
   raw_detect_list.detections[det_4].processed.vcs_position_x += 0.51F; // Move detection such that it is barely outside extended box

   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_props);

   uint32_t expected_num_dets_in_extbox = 2U;

   /** \action
    *  Call Calculate_Ext_Bbox_And_Find_Dets_Inside().
    **/
   Calculate_Ext_Bbox_And_Find_Dets_Inside(calibs, dist_rear_axle_to_vcs_m, host_speed, raw_detect_list, dead_zone, det_props, object_track, dets_in_extbox, num_dets_in_extbox);

   /** \result
    *  Check that the 2 detections inside extended bounding box are flagged as inside gate and marked as candidates to be associated.
    **/
   CHECK_TRUE_TEXT(expected_num_dets_in_extbox == num_dets_in_extbox, "The number of dets in extbbox is not equal to the expected number of used detections")
   CHECK_TRUE(det_props[det_1].f_inside_gate)
   CHECK_TRUE(det_props[det_2].f_inside_gate)
   CHECK_TRUE((dets_in_extbox[0] == det_1) || (dets_in_extbox[1] == det_1))
   CHECK_TRUE((dets_in_extbox[0] == det_2) || (dets_in_extbox[1] == det_2))

   CHECK_FALSE(det_props[det_3].f_inside_gate)
   CHECK_FALSE(det_props[det_4].f_inside_gate)
   CHECK_FALSE((dets_in_extbox[0] == det_3) || (dets_in_extbox[1] == det_3) || (dets_in_extbox[2] == det_3) || (dets_in_extbox[3] == det_3))
   CHECK_FALSE((dets_in_extbox[0] == det_4) || (dets_in_extbox[1] == det_4) || (dets_in_extbox[2] == det_4) || (dets_in_extbox[3] == det_4))
}
/** @}*/

/** \defgroup  f360_find_dets_inside_bbox_find_dets_in_object_vicinity
*  @{
*/

/** \brief
*  Testing of a function that finds detections in the vicinity of the object.
**/
TEST_GROUP(f360_find_dets_inside_bbox_find_dets_in_object_vicinity)
{
   /** \setup
   * Initialize common variables used within all tests for this test group
   **/
   F360_Object_Track_T object_track = {};
   uint32_t tot_num_dets;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   int32_t det_idx_of_interest[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t num_dets_of_interest;
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{}; // For sorting detections in vcs long order

   uint32_t exp_num_dets_of_interest;
   int32_t exp_det_idx_of_interest[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
   * Set position and dimension of object
   * Set object vcs pointing to 0 and cos/sin accordingly
   * Set object extended buffer zones to 1m
   * Set number of detections to 7
   * Create detections and set up expected det idxs to be returned
   **/
   TEST_SETUP()
   {
      object_track.Set_Bbox_Orientation(Angle{ 0.0F });
      object_track.vcs_position.x = 10.0F;
      object_track.vcs_position.y = 10.0F;

      object_track.Set_Bbox_Orientation(Angle{ 0.0F });
      object_track.Update_Bbox_Size(2.0F, 1.0F);

      object_track.long_buffer_zone_len1 = 1.0F;
      object_track.long_buffer_zone_len2 = 1.0F;
      object_track.lat_buffer_zone_wid1 = 1.0F;
      object_track.lat_buffer_zone_wid2 = 1.0F;

      tot_num_dets = 7U;

      // Set detection data
      raw_detect_list.detections[0].processed.vcs_position_x = 12.5F;
      raw_detect_list.detections[0].processed.vcs_position_y = 10.0F;
      raw_detect_list.detections[1].processed.vcs_position_x = 8.5F;
      raw_detect_list.detections[1].processed.vcs_position_y = 11.3F;
      raw_detect_list.detections[2].processed.vcs_position_x = 11.7F;
      raw_detect_list.detections[2].processed.vcs_position_y = 8.6F;
      raw_detect_list.detections[3].processed.vcs_position_x = 9.0F;
      raw_detect_list.detections[3].processed.vcs_position_y = 11.4F;
      raw_detect_list.detections[4].processed.vcs_position_x = 7.5F;
      raw_detect_list.detections[4].processed.vcs_position_y = 8.6F;
      raw_detect_list.detections[5].processed.vcs_position_x = -10.0F;
      raw_detect_list.detections[5].processed.vcs_position_y = -11.0F;
      raw_detect_list.detections[6].processed.vcs_position_x = 11.0F;
      raw_detect_list.detections[6].processed.vcs_position_y = 11.0F;

      Initialize_Detection_Sensor_Ids(raw_detect_list);
      raw_detect_list.number_of_valid_detections = tot_num_dets;
      Sort_Detections_Vcs_Long(raw_detect_list);
      Copy_Detections_Info(raw_detect_list, sensors, det_props);

      exp_num_dets_of_interest = 4U;
      exp_det_idx_of_interest[0U] = 1;
      exp_det_idx_of_interest[1U] = 3;
      exp_det_idx_of_interest[2U] = 6;
      exp_det_idx_of_interest[3U] = 2;
   }
};

TEST(f360_find_dets_inside_bbox_find_dets_in_object_vicinity, find_dets_in_object_vicinity)
{
   /** \precond
   * An object and 10 detections have been set up in test group. Detections have been set up such that 4 are inside extended bounding box and 6 are not.
   * Expected data has been set up in test group.
   * 
   **/

   /** \action
   *  Call Find_Dets_In_Object_Vicinity().
   **/
   Find_Dets_In_Object_Vicinity(object_track, raw_detect_list, det_props, num_dets_of_interest, det_idx_of_interest);

   /** \result
   * Check that number of detections of interest matches expected data.
   * Check that correct detections were marked as interesting
   **/
   CHECK_EQUAL_TEXT(exp_num_dets_of_interest, num_dets_of_interest, "The number of interesting detections was not correct!");
   CHECK_EQUAL_TEXT(exp_det_idx_of_interest[0], det_idx_of_interest[0], "The first detection index for interesting detections was not correct");
   CHECK_EQUAL_TEXT(exp_det_idx_of_interest[1], det_idx_of_interest[1], "The second detection index for interesting detections was not correct");
   CHECK_EQUAL_TEXT(exp_det_idx_of_interest[2], det_idx_of_interest[2], "The third detection index for interesting detections was not correct");
   CHECK_EQUAL_TEXT(exp_det_idx_of_interest[3], det_idx_of_interest[3], "The fourth detection index for interesting detections was not correct");
}

/** @}*/


/** \defgroup f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction
 *  @{
 */

/** \brief
 *  Testing of the function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() which extends the assoictaion
 * buffer zone for the object para side (front or rear) which is least visible from host.
 **/
TEST_GROUP(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction)
{
   /** \setup
    * Initialize common variables used within all tests for this test group
    **/
   F360_Object_Track_T object_track = {};
   F360_Calibrations_T calibs = {};
   float32_t dist_from_rear_axle_to_vcs = 2.0F;
   float32_t long_buffer1 = 1.0F;
   float32_t long_buffer2 = 1.0F;
   
   const float32_t TEST_PASS_TH = 1e-6F;


   /** \setup
   * Initialize tracker calibrations
   * Set object longitudinal vcs position 50m.
   * Set object lateral vcs position to 0.
   * Set object vcs pointing to 0 degrres.
   * Set object speed to be between calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      
      object_track.vcs_position.x = 50.0F;
      object_track.vcs_position.y = 0.0F;
      object_track.Set_Bbox_Orientation(Angle{ 0.0F });
      object_track.speed = 0.5F * (calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer + calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase);
   }
};


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does not extend the bounding box in any direction when the object
 * is in front of host with 0 pointing angle and when object speed is too small.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_front_0pnt_too_small_speed)
{
   /** \precond
    * Default object from test group is already placed in front of host and has a pointing of 0 degrees.
    * Change object speed to a low value
    **/
   object_track.speed = calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer - 0.1F;
   
   const float32_t expected_long_buffer1 = long_buffer1;
   const float32_t expected_long_buffer2 = long_buffer2;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones and time_since_ccv_moveable match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.");
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.");
}


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does extend the bounding box correctly in the object front 
 * direction when the object is in front of host with 0 pointing angle and when object speed is just above
 * calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_front_0pnt_speed_above_min_calib)
{
   /** \precond
    * Default object from test group is already placed in front of host and has a pointing of 0 degrees.
    * Change object speed to a low value
    **/
   object_track.speed = calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer + 0.1F;
   
   const float32_t expected_long_buffer1 = 1.0F;
   const float32_t expected_long_buffer2 = 1.338461F;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones and time_since_ccv_moveable match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does extend the bounding box correctly in the object front 
 * direction when the object is in front of host with 0 pointing angle and when object speed is in between
 * calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_front_0pnt_speed_between_min_and_max_calib)
{
   /** \precond
    * Default object from test group is already placed in front of host, has a pointing of 0 degrees and a speed in bewteen 
    * calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
    **/
   
   const float32_t expected_long_buffer1 = 1.0F;
   const float32_t expected_long_buffer2 = 1.65F;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones and time_since_ccv_moveable match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does extend the bounding box correctly in the object front 
 * direction when the object is in front of host with 0 pointing angle and when object speed is just below
 * calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_front_0pnt_speed_below_max_calib)
{
   /** \precond
    * Default object from test group is already placed in front of host, has a pointing of 0 degrees.
    * Change its speed to just below calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
    **/
   object_track.speed = calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase - 0.1F;
   
   const float32_t expected_long_buffer1 = 1.0F;
   const float32_t expected_long_buffer2 = 1.961539F;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones and time_since_ccv_moveable match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does extend the bounding box correctly in the object front 
 * direction when the object is in front of host with 0 pointing angle and when object speed is just above 
 * calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_front_0pnt_speed_above_max_calib)
{
   /** \precond
    * Default object from test group is already placed in front of host, has a pointing of 0 degrees.
    * Change its speed to just above calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
    **/
   object_track.speed = calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase + 0.1F;
   
   const float32_t expected_long_buffer1 = 1.0F;
   const float32_t expected_long_buffer2 = 2.1F;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones and time_since_ccv_moveable match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does extend the bounding box correctly in the object front 
 * direction when the object is in front of host with 45 deg pointing angle and when object speed is in between
 * calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_front_45pnt_speed_between_min_and_max_calib)
{
   /** \precond
    * Default object from test group is already placed in front of host and has a speed in bewteen calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer
    * and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase. Change its pointing to 45 degrees
    **/
   object_track.Set_Bbox_Orientation(Angle{ 0.785398163397448F });
   const float32_t expected_long_buffer1 = 1.0F;
   const float32_t expected_long_buffer2 = 1.45962F;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones and time_since_ccv_moveable match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}

/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does extend the bounding box correctly in the object front 
 * direction when the object is in front of host with -45 deg pointing angle and when object speed is in between
 * calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_front_neg45pnt_speed_between_min_and_max_calib)
{
   /** \precond
    * Default object from test group is already placed in front of host and has a speed in bewteen calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer
    * and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase. Change its pointing to -45 degrees
    **/
   object_track.Set_Bbox_Orientation(Angle{ -0.785398163397448F });
   const float32_t expected_long_buffer1 = 1.0F;
   const float32_t expected_long_buffer2 = 1.45962F;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones and time_since_ccv_moveable match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does not extend the bounding box when the object is in front of host
 * with 90 deg pointing angle (side towards host) and when object speed is in between calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer
 * and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_front_90pnt_speed_between_min_and_max_calib)
{
   /** \precond
    * Default object from test group is already placed in front of host and has a speed in bewteen calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer
    * and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase. Change its pointing to 90 degrees
    **/
   object_track.Set_Bbox_Orientation(Angle{ 1.570796326794897F });
   const float32_t expected_long_buffer1 = 1.0F;
   const float32_t expected_long_buffer2 = 1.0F;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones and time_since_ccv_moveable match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does not extend the bounding box when the object is in front of host
 * with -90 deg pointing angle (side towards host) and when object speed is in between calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer
 * and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_front_neg90pnt_speed_between_min_and_max_calib)
{
   /** \precond
    * Default object from test group is already placed in front of host and has a speed in bewteen calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer
    * and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase. Change its pointing to 90 degrees
    **/
   object_track.Set_Bbox_Orientation(Angle{ -1.570796326794897F });
   const float32_t expected_long_buffer1 = 1.0F;
   const float32_t expected_long_buffer2 = 1.0F;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones and time_since_ccv_moveable match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does extend the bounding box correctly in the object rear 
 * direction when the object is in front of host with 135 deg pointing angle and when object speed is in between
 * calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_front_135pnt_speed_between_min_and_max_calib)
{
   /** \precond
    * Default object from test group is already placed in front of host and has a speed in bewteen calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer
    * and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase. Change its pointing to 135 degrees
    **/
   object_track.Set_Bbox_Orientation(Angle{ 2.356194490192345F });
   const float32_t expected_long_buffer1 = 1.45962F;
   const float32_t expected_long_buffer2 = 1.0F;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones and time_since_ccv_moveable match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does extend the bounding box correctly in the object rear 
 * direction when the object is in front of host with -135 deg pointing angle and when object speed is in between
 * calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_front_neg135pnt_speed_between_min_and_max_calib)
{
   /** \precond
    * Default object from test group is already placed in front of host and has a speed in bewteen calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer
    * and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase. Change its pointing to 135 degrees
    **/
   object_track.Set_Bbox_Orientation(Angle{ -2.356194490192345F });
   const float32_t expected_long_buffer1 = 1.45962F;
   const float32_t expected_long_buffer2 = 1.0F;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones and time_since_ccv_moveable match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does extend the bounding box correctly in the object rear 
 * direction when the object is in front of host with 180 deg pointing angle and when object speed is in between
 * calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_front_180pnt_speed_between_min_and_max_calib)
{
   /** \precond
    * Default object from test group is already placed in front of host and has a speed in bewteen calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer
    * and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase. Change its pointing to 180 degrees
    **/
   object_track.Set_Bbox_Orientation(Angle{ 3.141592653589793F });
   const float32_t expected_long_buffer1 = 1.65F;
   const float32_t expected_long_buffer2 = 1.0F;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones and time_since_ccv_moveable match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does extend the bounding box correctly in the object rear 
 * direction when the object is to the right of host with -90 deg pointing angle and when object speed is in between
 * calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_right_neg90pnt_speed_between_min_and_max_calib)
{
   /** \precond
    * Default object from test group has a speed in bewteen calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer
    * and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase. Change its pointing to -90 degrees and 
    * its position to be directly to the right of host center.
    **/
   object_track.vcs_position.x = 0.0F - 0.5F*dist_from_rear_axle_to_vcs;
   object_track.vcs_position.y = 50.0F;
   object_track.Set_Bbox_Orientation(Angle{ -1.570796326794897F });
   const float32_t expected_long_buffer1 = 1.65F;
   const float32_t expected_long_buffer2 = 1.0F;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones and time_since_ccv_moveable match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does extend the bounding box correctly in the object front 
 * direction when the object is to the left of host with -90 deg pointing angle and when object speed is in between
 * calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_left_neg90pnt_speed_between_min_and_max_calib)
{
   /** \precond
    * Default object from test group has a speed in bewteen calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer
    * and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase. Change its pointing to -90 degrees and 
    * its position to be directly to the right of host center.
    **/
   object_track.vcs_position.x = 0.0F - 0.5F*dist_from_rear_axle_to_vcs;
   object_track.vcs_position.y = -50.0F;
   object_track.Set_Bbox_Orientation(Angle{ -1.570796326794897F });
   const float32_t expected_long_buffer1 = 1.0F;
   const float32_t expected_long_buffer2 = 1.65F;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does extend the bounding box correctly in the object front 
 * direction when the object is rear of host with 135 deg pointing angle and when object speed is in between
 * calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase.
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_rear_135pnt_speed_between_min_and_max_calib)
{
   /** \precond
    * Default object from test group has a speed in bewteen calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer
    * and calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase. Change its pointing to 135 degrees and 
    * its position to be directly to the right of host center.
    **/
   object_track.vcs_position.x = -50.0F;
   object_track.vcs_position.y = 0.0F;
   object_track.Set_Bbox_Orientation(Angle{ 2.356194490192345F });
   const float32_t expected_long_buffer1 = 1.0F;
   const float32_t expected_long_buffer2 = 1.459619F;
 
   
   /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
   Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


   /** \result
    * Check that buffer zones  match expected data.
    **/
   DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
   DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}


/** @}*/
/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does extend the bounding box correctly in front direction
 * when the speed is too high and we reach up to the maximum saturation
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_Max_Saturation)
{
    /** \precond
     * Default object from test group is already placed in front of host. Speed needs to be modified to be more than the maximum threshold for
     * saturation
     **/
    object_track.speed = calibs.k_second_max_speed_for_saturating_occluded_long_assoc_buffer_increase + 0.1F;
    const float32_t expected_long_buffer1 = 1.0F;
    const float32_t expected_long_buffer2 = 3.1F;


    /** \action
     *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
     **/
    Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


    /** \result
     * Check that buffer zones match expected data.
     **/
    DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
    DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}


/** \purpose
 * Test that function Increase_Buffer_Zone_In_Occluded_Parallel_Direction() does extend the bounding box correctly in front direction
 * when the speed is between k_max_speed_for_saturating_occluded_long_assoc_buffer_increase and k_second_min_speed_for_increasing_occluded_long_assoc_buffer
 *  \req     NA
 **/
TEST(f360_dets_inside_bbox_Increase_Buffer_Zone_In_Occluded_Parallel_Direction, Increase_Buffer_Zone_In_Occluded_Parallel_Direction_Saturation_Between_Breakpoints)
{
    /** \precond
    * Default object from test group is already placed in front of host. Speed needs to be modified to be more than the maximum threshold for
    * saturation
    **/
    object_track.speed = calibs.k_second_min_speed_for_increasing_occluded_long_assoc_buffer + 0.1F;
    const float32_t expected_long_buffer1 = 1.0F;
    const float32_t expected_long_buffer2 = 2.107143F;


    /** \action
    *  Call Increase_Buffer_Zone_In_Occluded_Parallel_Direction().
    **/
    Increase_Buffer_Zone_In_Occluded_Parallel_Direction(object_track, dist_from_rear_axle_to_vcs, calibs, long_buffer1, long_buffer2);


    /** \result
    * Check that buffer zones match expected data.
    **/
    DOUBLES_EQUAL_TEXT(expected_long_buffer1, long_buffer1, TEST_PASS_TH, "Longitudinal buffer zone 1 did not match expected data.")
        DOUBLES_EQUAL_TEXT(expected_long_buffer2, long_buffer2, TEST_PASS_TH, "Longitudinal buffer zone 2 did not match expected data.")
}
/** @}*/

/** @}*/

/** \defgroup  f360_decrease_association_gates_width_for_slow_objects
 *  @{
 */

 /** \brief
  *  Testing of a function that reduces the width of the association gates
  *  of an object. That happens when the object moves less than specific speed
  *  has heading less than a specific angle by their absolute value and are within
  *  a longitudinal distance from the host 
  **/
TEST_GROUP(f360_decrease_association_gates_width_for_slow_objects)
{
    /** \setup
     * Initialize common variables used within all tests for this test group
     **/
    const float32_t TEST_PASS_TH = 1e-6F;
    float32_t object_width = 1.0F;
    float32_t object_heading = 0.17F;
    float32_t object_vsc_long_pos = 10.0F;
    float32_t object_speed = 1.5F;
    float32_t lat_buffer = 2.0F;
    F360_Calibrations_T calibs = {};
    /** \setup
     * Set up calibrations
     **/
    TEST_SETUP()
    {
        Initialize_Tracker_Calibrations(calibs);
    }
};

/** \purpose
 * Test that the width of the association gates of an object are decreased if all
 * the conditions are filled
 **/
TEST(f360_decrease_association_gates_width_for_slow_objects, decrease_association_gates_width_all_conditions_true)
{
    /** \precond
     * Set expected values for the 2 buffers 
     **/
    float32_t expected_lat_buffer = 1.0F;

    /** \action
     *  Call Decrease_Association_Gates_Width_For_Slow_Objects().
     **/
    Decrease_Association_Gates_Width_For_Slow_Objects(calibs, object_width, object_vsc_long_pos, object_speed, object_heading, lat_buffer);

    /** \result
     * Check that the buffer for the width of the association gates has the correct value
     **/
    DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "The buffer did not match the expected data.")
}

/** \purpose
 * Test that the width of the association gates of an object are not decreased 
 * since the width of the object is smaller than the threshold
 **/
TEST(f360_decrease_association_gates_width_for_slow_objects, not_decrease_association_gates_width_object_has_smaller_width)
{
    /** \precond
     * Set expected values for the 2 buffers
     * Set the width of the object to be less than the threshold
     **/
    float32_t expected_lat_buffer = 2.0F;
    object_width = calibs.k_nonmoveable_target_diameter - 0.1;


    /** \action
     *  Call Decrease_Association_Gates_Width_For_Slow_Objects().
     **/
    Decrease_Association_Gates_Width_For_Slow_Objects(calibs, object_width, object_vsc_long_pos, object_speed, object_heading, lat_buffer);

    /** \result
     * Check that the buffer for the width of the association gates has the correct value
     **/
    DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "The buffer was modified while it shouldnt.")
}

/** \purpose
 * Test that the width of the association gates of an object are not decreased
 * since the heading of the object is bigger than the threshold in radius and
 * it belongs to the first quadrat
 **/
TEST(f360_decrease_association_gates_width_for_slow_objects, not_decrease_association_gates_width_object_has_bigger_heading_first_quadrat)
{
    /** \precond
     * Set expected values for the 2 buffers
     * Set the heading of the object to be more than the threshold and belong the the first quadrat
     **/
    float32_t expected_lat_buffer = 2.0F;
    object_heading = 1.0F;


    /** \action
     *  Call Decrease_Association_Gates_Width_For_Slow_Objects().
     **/
    Decrease_Association_Gates_Width_For_Slow_Objects(calibs, object_width, object_vsc_long_pos, object_speed, object_heading, lat_buffer);

    /** \result
     * Check that the buffer for the width of the association gates has the correct value
     **/
    DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "The buffer was modified while it shouldnt.")
}

/** \purpose
 * Test that the width of the association gates of an object are not decreased
 * since the heading of the object is bigger than the threshold in radius and
 * it belongs to the second quadrat
 **/
TEST(f360_decrease_association_gates_width_for_slow_objects, not_decrease_association_gates_width_object_has_bigger_heading_second_quadrat)
{
    /** \precond
     * Set expected values for the 2 buffers
     * Set the heading of the object to be more than the threshold and belong the the second quadrat
     **/
    float32_t expected_lat_buffer = 2.0F;
    object_heading = -1.0F;


    /** \action
     *  Call Decrease_Association_Gates_Width_For_Slow_Objects().
     **/
    Decrease_Association_Gates_Width_For_Slow_Objects(calibs, object_width, object_vsc_long_pos, object_speed, object_heading, lat_buffer);

    /** \result
     * Check that the buffer for the width of the association gates has the correct value
     **/
    DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "The buffer was modified while it shouldnt.")
}

/** \purpose
 * Test that the width of the association gates of an object are not decreased
 * since the heading of the object is bigger than the threshold in radius and
 * it belongs to the third quadrat
 **/
TEST(f360_decrease_association_gates_width_for_slow_objects, not_decrease_association_gates_width_object_has_bigger_heading_third_quadrat)
{
    /** \precond
     * Set expected values for the 2 buffers
     * Set the heading of the object to be more than the threshold and belong the the third quadrat
     **/
    float32_t expected_lat_buffer = 2.0F;
    object_heading = -2.8F;


    /** \action
     *  Call Decrease_Association_Gates_Width_For_Slow_Objects().
     **/
    Decrease_Association_Gates_Width_For_Slow_Objects(calibs, object_width, object_vsc_long_pos, object_speed, object_heading, lat_buffer);

    /** \result
     * Check that the buffer for the width of the association gates has the correct value
     **/
    DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "The buffer was modified while it shouldnt.")
}

/** \purpose
 * Test that the width of the association gates of an object are not decreased
 * since the heading of the object is bigger than the threshold in radius and
 * it belongs to the fourth quadrat
 **/
TEST(f360_decrease_association_gates_width_for_slow_objects, not_decrease_association_gates_width_object_has_bigger_heading_fourth_quadrat)
{
    /** \precond
     * Set expected values for the 2 buffers
     * Set the heading of the object to be more than the threshold and belong the the fourth quadrat
     **/
    float32_t expected_lat_buffer = 2.0F;
    object_heading = 2.8F;


    /** \action
     *  Call Decrease_Association_Gates_Width_For_Slow_Objects().
     **/
    Decrease_Association_Gates_Width_For_Slow_Objects(calibs, object_width, object_vsc_long_pos, object_speed, object_heading, lat_buffer);

    /** \result
     * Check that the buffer for the width of the association gates has the correct value
     **/
    DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "The buffer was modified while it shouldnt.")
}

/** \purpose
 * Test that the width of the association gates of an object are not decreased
 * since the longitudinal position of the object is bigger than the thresho
 **/
TEST(f360_decrease_association_gates_width_for_slow_objects, not_decrease_association_gates_width_object_has_bigger_long_pos)
{
    /** \precond
     * Set expected values for the 2 buffers
     * Set the heading of the object to be more than the threshold
     **/
    float32_t expected_lat_buffer = 2.0F;
    object_vsc_long_pos = 110.0F;


    /** \action
     *  Call Decrease_Association_Gates_Width_For_Slow_Objects().
     **/
    Decrease_Association_Gates_Width_For_Slow_Objects(calibs, object_width, object_vsc_long_pos, object_speed, object_heading, lat_buffer);

    /** \result
     * Check that the buffer for the width of the association gates has the correct value
     **/
    DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "The buffer was modified while it shouldnt.")
}

/** \purpose
 * Test that the width of the association gates of an object are not decreased
 * since the speed of the object is bigger than the threshold
 **/
TEST(f360_decrease_association_gates_width_for_slow_objects, not_decrease_association_gates_width_object_has_bigger_speed)
{
    /** \precond
     * Set expected values for the 2 buffers
     * Set the speed of the object to be more than the threshold
     **/
    float32_t expected_lat_buffer = 2.0F;
    object_speed = 4.0F;


    /** \action
     *  Call Decrease_Association_Gates_Width_For_Slow_Objects().
     **/
    Decrease_Association_Gates_Width_For_Slow_Objects(calibs, object_width, object_vsc_long_pos, object_speed, object_heading, lat_buffer);

    /** \result
     * Check that the buffer for the width of the association gates has the correct value
     **/
    DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "The buffer was modified while it shouldnt.")
}

/** \purpose
 * Test that the width of the association gates of an object are not decreased
 * since the lateral buffer of the object is smaller than the threshold
 **/
TEST(f360_decrease_association_gates_width_for_slow_objects, not_decrease_association_gates_width_object_has_smaller_lateral_buffer)
{
    /** \precond
     * Set expected values for the 2 buffers
     * Set the lateral buffer of the object to be less than the threshold
     **/
    float32_t expected_lat_buffer = 0.4F;
    lat_buffer = 0.4F;


    /** \action
     *  Call Decrease_Association_Gates_Width_For_Slow_Objects().
     **/
    Decrease_Association_Gates_Width_For_Slow_Objects(calibs, object_width, object_vsc_long_pos, object_speed, object_heading, lat_buffer);

    /** \result
     * Check that the buffer for the width of the association gates has the correct value
     **/
    DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "The buffer was modified while it shouldnt.")
}

/** \purpose
 * Test that the width of the association gates of an object are decreased
 * up to the minimum saturation point
 **/
TEST(f360_decrease_association_gates_width_for_slow_objects, _decrease_association_gates_width_up_to_saturation)
{
    /** \precond
     * Set expected values for the 2 buffers
     * Set the speed of the object to be very low so the buffer reduction to be very big
     **/
    float32_t expected_lat_buffer = 0.7F;
    object_speed = 0.1F;


    /** \action
     *  Call Decrease_Association_Gates_Width_For_Slow_Objects().
     **/
    Decrease_Association_Gates_Width_For_Slow_Objects(calibs, object_width, object_vsc_long_pos, object_speed, object_heading, lat_buffer);

    /** \result
     * Check that the buffers for the width of the association gates have the correct value
     **/
    DOUBLES_EQUAL_TEXT(expected_lat_buffer, lat_buffer, TEST_PASS_TH, "The buffer was modified while it shouldnt.")
}

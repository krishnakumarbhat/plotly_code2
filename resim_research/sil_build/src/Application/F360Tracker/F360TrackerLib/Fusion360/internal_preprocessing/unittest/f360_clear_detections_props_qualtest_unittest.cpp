/** \file
 * This file contains qualification tests for content of f360_clear_detections_props.cpp file
 */

#include "f360_clear_detections_props.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_clear_detections_props
 *  @{
 */

/** \brief
 * This test group is used to test the funcitonality of f360_clear_detections_props
 */
TEST_GROUP(f360_clear_detections_props)
{
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   const float32_t test_threshold = 0.000001F;

   /** \setup
    * Set up two detections at different indices in the array with random non-default values.
    */
   TEST_SETUP()
   {
      const uint32_t num_dets = 2U;
      uint32_t indices[2U];
      indices[0] = 0U;
      indices[1] = 10U;

      for (uint32_t i = 0U; i < num_dets; i++)
      {
         uint32_t idx = indices[i];
         det_props[idx].vcs_position.x = static_cast<float32_t>(i) + 1.0F;
         det_props[idx].vcs_position.y = static_cast<float32_t>(i) + 1.0F;
         det_props[idx].range_rate_dealiased = static_cast<float32_t>(i) + 1.0F;
         det_props[idx].range_dealiased = static_cast<float32_t>(i) + 1.0F;
         det_props[idx].range_rate_compensated = static_cast<float32_t>(i) + 1.0F;
         det_props[idx].range_rate_predicted = static_cast<float32_t>(i) + 1.0F;
         det_props[idx].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
         det_props[idx].cluster_id = 1 + static_cast<uint16_t>(i);
         det_props[idx].object_track_id = 1 + i;
         det_props[idx].f_dealiased = true;
         det_props[idx].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_NEARBY;
         det_props[idx].f_double_bounce = true;
         det_props[idx].f_close_target = true;
         det_props[idx].f_FOV_edge = true;
         det_props[idx].f_rr_inlier = true;
         det_props[idx].f_used_in_rr_msmt_update = true;
         det_props[idx].f_inside_gate = true;
         det_props[idx].f_ok_to_use = true;
         det_props[idx].f_det_pair = true;
         det_props[idx].f_potential_angle_jump = true;
         det_props[idx].f_object_based_angle_jump = true;
         det_props[idx].f_water_spray = true;
         det_props[idx].f_stationary_bounce= true;
         det_props[idx].f_azimuth_rdot_outlier = true;
         det_props[idx].f_trailer_related_det = true;
         det_props[idx].f_has_close_det_with_similar_rr = true;
         det_props[idx].behind_sep_id = 1 + static_cast<uint8_t>(i);
         det_props[idx].on_sep_id = 1 + static_cast<uint8_t>(i);
      }
   }
};

/** \purpose
 * Test checks that all the detections in the list only contain default values after Clear_Detections_Props is called.
 * \req
 * CPR-3842
 */
TEST(f360_clear_detections_props, Clear_Detections_Props)
{
   /** \precond
    * Two detections with non-default values have been added to the detection props list.
    */

   /** \action
    * Call Clear_Detections_Props
    */
   Clear_Detections_Props(det_props);

   /** \result
    * Check that all fields are reset to default values for all detections in the detection props list.
    */
   for (uint32_t i = 0U; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      DOUBLES_EQUAL(0.0F, det_props[i].vcs_position.x, test_threshold);
      DOUBLES_EQUAL(0.0F, det_props[i].vcs_position.y, test_threshold);
      DOUBLES_EQUAL(0.0F, det_props[i].range_rate_dealiased, test_threshold);
      DOUBLES_EQUAL(0.0F, det_props[i].range_dealiased, test_threshold);
      DOUBLES_EQUAL(0.0F, det_props[i].range_rate_compensated, test_threshold);
      DOUBLES_EQUAL(0.0F, det_props[i].range_rate_predicted, test_threshold);
      CHECK_EQUAL(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_INVALID, det_props[i].motion_status);
      CHECK_EQUAL(0, det_props[i].cluster_id);
      CHECK_EQUAL(0, det_props[i].object_track_id);
      CHECK_FALSE(det_props[i].f_dealiased);
      CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, det_props[i].wheel_spin_type);
      CHECK_FALSE(det_props[i].f_double_bounce);
      CHECK_FALSE(det_props[i].f_close_target);
      CHECK_FALSE(det_props[i].f_FOV_edge);
      CHECK_FALSE(det_props[i].f_rr_inlier);
      CHECK_FALSE(det_props[i].f_used_in_rr_msmt_update);
      CHECK_FALSE(det_props[i].f_inside_gate);
      CHECK_TRUE(det_props[i].f_ok_to_use);
      CHECK_FALSE(det_props[i].f_det_pair);
      CHECK_FALSE(det_props[i].f_potential_angle_jump);
      CHECK_FALSE(det_props[i].f_object_based_angle_jump);
      CHECK_FALSE(det_props[i].f_water_spray);
      CHECK_FALSE(det_props[i].f_stationary_bounce);
      CHECK_FALSE(det_props[i].f_azimuth_rdot_outlier);
      CHECK_FALSE(det_props[i].f_trailer_related_det);
      CHECK_FALSE(det_props[i].f_has_close_det_with_similar_rr);
      CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, det_props[i].behind_sep_id);
      CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, det_props[i].on_sep_id);

   }
}
/** @}*/

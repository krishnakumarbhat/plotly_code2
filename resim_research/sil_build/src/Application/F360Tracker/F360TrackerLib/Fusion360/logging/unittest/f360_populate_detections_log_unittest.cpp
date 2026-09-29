/** \file
 * This file contains unit tests for content of f360_populate_detections_log.cpp file
 */

#include "f360_populate_detections_log.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_populate_detections_log
 *  @{
 */

/** \brief
 * The test suite for Populate_Detections_Log functions.
 * 
 */
TEST_GROUP(f360_populate_detections_log)
{  
   // Declare common variables used within all tests in this test group.
   rspp_variant_A::RSPP_Detection_List_T detection_list;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Detection_Log_Output_T detection_log;
   /** \setup
    * Set up the default input variables values for the whole group test.
    */
   TEST_SETUP()
   {
      // Initialize input data
      detection_list.number_of_valid_detections = 1U; // initialize one valid detection from the detection list
      detection_list.detections[0].raw.sensor_id = 1; // set the detection source sensor id 1
      detection_list.detections[0].raw.det_id = 2; // set the detection id 2
      det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING; // set the detection motion status as moving
      det_props[0].vcs_position.x = 5.0F;
      det_props[0].vcs_position.y = -1.0F;
      det_props[0].range_rate_dealiased = 5.0F;
      det_props[0].range_rate_compensated = 10.0F;
      det_props[0].object_track_id = 10;
      det_props[0].cluster_id = 5;
      det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
      det_props[0].f_dealiased = true;
      det_props[0].f_double_bounce = false;
      det_props[0].f_FOV_edge = false;
      det_props[0].f_rr_inlier = true;
      det_props[0].f_used_in_rr_msmt_update = true;
      det_props[0].f_close_target = false;
      det_props[0].f_inside_gate = true;
      det_props[0].f_ok_to_use = true;
      det_props[0].on_sep_id = 0;
   }
};

/** \purpose  
 * Test that function Populate_Detections_Log will return as expected.
 * \req NA
 */
TEST(f360_populate_detections_log, Test_Populate_Detections_Log)
{
   /** \precond
    * Using the test group predefine input in this case
    */
   
   /** \action
    * call Populate_Detections_Log() with test group default setup values.
    */
   Populate_Detections_Log(&detection_log, detection_list, det_props);
   

   /** \result
    * check that the detection_log output match expected data.
    */
   CHECK_TRUE(detection_log.detection[0].sensorID == 1U);
   CHECK_TRUE(detection_log.detection[0].raw_det_id == 2);
   CHECK_EQUAL(detection_log.detection[0].vcs_x,5.0F);
   CHECK_EQUAL(detection_log.detection[0].vcs_y, -1.0F);
   CHECK_EQUAL(detection_log.detection[0].rngrate_dealiased, 5.0F);
   CHECK_EQUAL(detection_log.detection[0].rngrate_comp, 10.0F);
   CHECK_TRUE(detection_log.detection[0].objTrkID == 10U);
   CHECK_TRUE(detection_log.detection[0].clusterID == 5U);
   CHECK_TRUE(detection_log.detection[0].motion_status == 1);
   CHECK_TRUE(detection_log.detection[0].wheel_spin == 2U);
   CHECK_TRUE(detection_log.detection[0].f_dealiased == 1U);
   CHECK_TRUE(detection_log.detection[0].f_double_bounce == 0U);
   CHECK_TRUE(detection_log.detection[0].f_FOV_edge == 0U);
   CHECK_TRUE(detection_log.detection[0].f_rr_inlier == 1U);
   CHECK_TRUE(detection_log.detection[0].f_used_in_rr_msmt_update == 1U);
   CHECK_TRUE(detection_log.detection[0].f_close_target == 0U);
   CHECK_TRUE(detection_log.detection[0].f_inside_gate == 1U);
   CHECK_TRUE(detection_log.detection[0].f_ok_to_use == 1U);
   CHECK_TRUE(detection_log.detection[0].f_on_guardrail == 0U);

   /** \action
    * change the detection props and call Populate_Detections_Log() again 
    * to check the output for branch coverage purpose.
    */
   det_props[0].f_dealiased = false;
   det_props[0].f_double_bounce = true;
   det_props[0].f_FOV_edge = true;
   det_props[0].f_rr_inlier = false;
   det_props[0].f_used_in_rr_msmt_update = false;
   det_props[0].f_close_target = true;
   det_props[0].f_inside_gate = false;
   det_props[0].f_ok_to_use = false;
   det_props[0].on_sep_id = 1;
   Populate_Detections_Log(&detection_log, detection_list, det_props);
   CHECK_TRUE(detection_log.detection[0].f_dealiased == 0U);
   CHECK_TRUE(detection_log.detection[0].f_double_bounce == 1U);
   CHECK_TRUE(detection_log.detection[0].f_FOV_edge == 1U);
   CHECK_TRUE(detection_log.detection[0].f_rr_inlier == 0U);
   CHECK_TRUE(detection_log.detection[0].f_used_in_rr_msmt_update == 0U);
   CHECK_TRUE(detection_log.detection[0].f_close_target == 1U);
   CHECK_TRUE(detection_log.detection[0].f_inside_gate == 0U);
   CHECK_TRUE(detection_log.detection[0].f_ok_to_use == 0U);
   CHECK_TRUE(detection_log.detection[0].f_on_guardrail == 1U);

}
/** @}*/

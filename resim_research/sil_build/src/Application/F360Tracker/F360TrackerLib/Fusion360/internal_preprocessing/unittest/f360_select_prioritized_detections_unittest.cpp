/** \file
 * This file contains unit tests for content of f360_select_prioritized_detections.cpp file
 */

#include "f360_select_prioritized_detections.h"
#include "f360_constants.h"
#include <CppUTest/TestHarness.h>
#include <time.h>
#include <stdlib.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_select_prioritized_detections
 *  @{
 */

/** \brief
 * Check that when there are more than the specified number of detections, detections become disqualified.
 */
TEST_GROUP(f360_select_prioritized_detections)
{
   // Declare common variables used within all tests in this test group.
   rspp_variant_A::RSPP_Detection_List_T det_list;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];
   F360_Det_Look_ID_T look_id = F360_DET_LOOK_ID_0;
   F360_Host_T host = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS];
};

/** \purpose
 * Check that detections get prioritized and a number of them get disqualified.
 * \req
 * NA
 */
TEST(f360_select_prioritized_detections, Number_Of_Prioritized_Detections)
{
   /** \precond
    * Set up sensor calibs and populate detection list with more detections than maximum downselected.
    */
   host.speed = 0.0F;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
   sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = 0.0F;
   sensors[0].constant.v_wrapping[0] = 30.0F;
   sensors[0].constant.polarity = 1.0F;
   sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   sensors[1].constant.mounting_position.vcs_boresight_azimuth_angle = -30.0F;
   sensors[1].constant.v_wrapping[0] = 30.0F;
   sensors[1].constant.polarity = 1.0F;
   sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
   sensors[2].constant.mounting_position.vcs_boresight_azimuth_angle = -120.0F;
   sensors[2].constant.v_wrapping[0] = 30.0F;
   sensors[2].constant.polarity = 1.0F;
   int32_t n_dets = 0;

   for (int32_t i = 0; i < 150; i++)
   {
      det_list.detections[n_dets].raw.azimuth = (static_cast<float>(rand() % 900) * 0.1F - 45.0F) * F360_PI / 180.0F;
      det_list.detections[n_dets].raw.range_rate = static_cast<float>(rand() % 300) * 0.1F - 10.0F;
      det_list.detections[n_dets].raw.range = static_cast<float>(rand() % 1500) * 0.1F;
      det_list.detections[n_dets].raw.sensor_id = 1;
      detection_props[n_dets].f_ok_to_use = true;
      n_dets++;
   }

   for (int32_t i = 0; i < 64; i++)
   {
      det_list.detections[n_dets].raw.azimuth = (static_cast<float>(rand() % 900) * 0.1F - 45.0F) * F360_PI / 180.0F;
      det_list.detections[n_dets].raw.range_rate = static_cast<float>(rand() % 300) * 0.1F - 10.0F;
      det_list.detections[n_dets].raw.range = static_cast<float>(rand() % 700) * 0.1F;
      det_list.detections[n_dets].raw.sensor_id = 2;
      detection_props[n_dets].f_ok_to_use = true;
      n_dets++;
   }

   for (int32_t i = 0; i < 64; i++)
   {
      det_list.detections[n_dets].raw.azimuth = (static_cast<float>(rand() % 900) * 0.1F - 45.0F) * F360_PI / 180.0F;
      det_list.detections[n_dets].raw.range_rate = static_cast<float>(rand() % 300) * 0.1F - 10.0F;
      det_list.detections[n_dets].raw.range = static_cast<float>(rand() % 700) * 0.1F;
      det_list.detections[n_dets].raw.sensor_id = 3;
      detection_props[n_dets].f_ok_to_use = true;
      n_dets++;
   }
   det_list.number_of_valid_detections = n_dets;

   /** \action
    * Call Select_Prioritized_Detections()
    */
   Select_Prioritized_Detections(det_list, sensors, host, detection_props);

   /** \result
    * Check that there are the expected number of detections.
    */
   int32_t cnt_CF = 0;
   int32_t cnt_LF = 0;
   int32_t cnt_LR = 0;

   for (int32_t i = 0; i < n_dets; i++)
   {
      if (det_list.detections[i].raw.sensor_id == 1 && detection_props[i].f_ok_to_use)
      {
         cnt_CF++;
      }
      else if (det_list.detections[i].raw.sensor_id == 2 && detection_props[i].f_ok_to_use)
      {
         cnt_LF++;
      }
      else if (det_list.detections[i].raw.sensor_id == 3 && detection_props[i].f_ok_to_use)
      {
         cnt_LR++;
      }
   }
   CHECK_EQUAL(0, cnt_CF);
   CHECK_EQUAL(64, cnt_LF);
   CHECK_EQUAL(64, cnt_LR);
}
/** @}*/

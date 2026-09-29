/** \file
   This file contains test for functions in f360_mark_water_spray_detections.
*/

#include "f360_mark_host_water_spray_detections.h"
#include <CppUTest/TestHarness.h>
#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_math.h"
#include "f360_internal_preprocessing.h"
#include "f360_look_type.h"

//module declarations
using namespace f360_variant_A;

/** \defgroup  f360_mark_water_spray_detections
 *  @{
 */
/** \brief
*  This test group contains tests for Mark_Water_Spray_Detections
*  function.
**/
TEST_GROUP(f360_mark_water_spray_detections)
{
   rspp_variant_A::RSPP_Detection_List_T raw_det_list{};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Host_T host = {};
   F360_Calibrations_T calibs = {};


   /** \setup
    * Default setup will change f_water_spray for all detections to true after function call
    **/
   TEST_SETUP()
   {

      // Initialize calibrations
      Initialize_Tracker_Calibrations(calibs);

      // Initialize host
      host.dist_rear_axle_to_vcs_m = 3.7F;
      host.speed = 10.0F;

      // Initialize detections
      raw_det_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;
       for (uint32_t det_idx = 0U; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
      {
         rspp_variant_A::RSPP_Detection_T& det = raw_det_list.detections[det_idx];

         det.processed.f_ok_to_use = true;
      }
      raw_det_list.number_of_valid_detections = 6U;

      raw_det_list.detections[0].raw.rcs = calibs.k_ws_max_det_rcs - 1.0F;
      raw_det_list.detections[0].raw.range_rate = 1.0F;
      det_props[0].f_water_spray = false;
      det_props[0].f_ok_to_use = true;
      raw_det_list.detections[0].processed.vcs_position_x = -host.dist_rear_axle_to_vcs_m;
      raw_det_list.detections[0].processed.vcs_position_y = -0.5F;

      raw_det_list.detections[1].raw.rcs = calibs.k_ws_max_det_rcs - 1.0F;
      raw_det_list.detections[1].raw.range_rate = 1.0F;
      det_props[1].f_water_spray = false;
      det_props[1].f_ok_to_use = true;
      raw_det_list.detections[1].processed.vcs_position_x = -host.dist_rear_axle_to_vcs_m;
      raw_det_list.detections[1].processed.vcs_position_y = 0.5F;

      raw_det_list.detections[2].raw.rcs = calibs.k_ws_max_det_rcs - 1.0F;
      raw_det_list.detections[2].raw.range_rate = 1.0F;
      det_props[2].f_water_spray = false;
      det_props[2].f_ok_to_use = true;
      raw_det_list.detections[2].processed.vcs_position_x = -host.dist_rear_axle_to_vcs_m;
      raw_det_list.detections[2].processed.vcs_position_y = -0.5F;

      raw_det_list.detections[3].raw.rcs = calibs.k_ws_max_det_rcs - 1.0F;
      raw_det_list.detections[3].raw.range_rate = 1.0F;
      det_props[3].f_water_spray = false;
      det_props[3].f_ok_to_use = true;
      raw_det_list.detections[3].processed.vcs_position_x = -host.dist_rear_axle_to_vcs_m;
      raw_det_list.detections[3].processed.vcs_position_y = 0.5F;

      raw_det_list.detections[4].raw.rcs = calibs.k_ws_max_det_rcs - 1.0F;
      raw_det_list.detections[4].raw.range_rate = 1.0F;
      det_props[4].f_water_spray = false;
      det_props[4].f_ok_to_use = true;
      raw_det_list.detections[4].processed.vcs_position_x = -host.dist_rear_axle_to_vcs_m;
      raw_det_list.detections[4].processed.vcs_position_y = -0.5F;

      raw_det_list.detections[5].raw.rcs = calibs.k_ws_max_det_rcs - 1.0F;
      raw_det_list.detections[5].raw.range_rate = 1.0F;
      det_props[5].f_water_spray = false;
      det_props[5].f_ok_to_use = true;
      raw_det_list.detections[5].processed.vcs_position_x = -host.dist_rear_axle_to_vcs_m;
      raw_det_list.detections[5].processed.vcs_position_y = 0.5F;


      // Initialize detection sorted info (Done in Clear_Detection() function call in production code)
      for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
      {
         raw_det_list.detections[det_idx].processed.prev_sorted_idx = F360_INVALID_ID;
         raw_det_list.detections[det_idx].processed.next_sorted_idx = F360_INVALID_ID;
      }
      Sort_Detections_Vcs_Long(raw_det_list);
      Copy_Detections_Info(raw_det_list, sensors, det_props);
   }
};

/** \purpose
* Verify that detections have been flagged as water spray correctly with default preconditions
* \req CPR-3861
*/
TEST(f360_mark_water_spray_detections, default_test)
{
   /** \precond
   * Use default settings
   **/

   /** \action
   * Execute the function
   **/
   Mark_Host_Water_Spray_Detections(
      raw_det_list,
      host,
      calibs,
      det_props);

   /** \result
   * Check that f_water_spray is set to true for all detections
   **/
   CHECK_TRUE_TEXT(det_props[0].f_water_spray, "f_water_spray does not indicate water spray")
   CHECK_TRUE_TEXT(det_props[1].f_water_spray, "f_water_spray does not indicate water spray")
   CHECK_TRUE_TEXT(det_props[2].f_water_spray, "f_water_spray does not indicate water spray")
   CHECK_TRUE_TEXT(det_props[3].f_water_spray, "f_water_spray does not indicate water spray")
   CHECK_TRUE_TEXT(det_props[4].f_water_spray, "f_water_spray does not indicate water spray")
   CHECK_TRUE_TEXT(det_props[5].f_water_spray, "f_water_spray does not indicate water spray")
}

/** \purpose
* Test that detections that are outside host water spray zone are correctly flagged as not being water spray detections
* \req CPR-3861
*/
TEST(f360_mark_water_spray_detections, detections_outside_vcs_zone)
{
   /** \precond
   * - Detection 0 is placed in front of host
   * - Detection 1 is placed to the to the left of host water spray zone
   * - Detection 2 is placed to the to the right of host water spray zone
   * - Detection 3 is placed behind of host water spray zone
   **/
   raw_det_list.detections[0].processed.vcs_position_x = 5.0F;
   raw_det_list.detections[0].processed.vcs_position_y = 0.0F;

   raw_det_list.detections[1].processed.vcs_position_x = -host.dist_rear_axle_to_vcs_m;
   raw_det_list.detections[1].processed.vcs_position_y = -4.0F;

   raw_det_list.detections[2].processed.vcs_position_x = -host.dist_rear_axle_to_vcs_m;
   raw_det_list.detections[2].processed.vcs_position_y = 4.0F;

   raw_det_list.detections[3].processed.vcs_position_x = 10.0F;
   raw_det_list.detections[3].processed.vcs_position_y = 0.0F;

   Sort_Detections_Vcs_Long(raw_det_list);
   Copy_Detections_Info(raw_det_list, sensors, det_props);

   /** \action
   * Execute the function
   **/
   Mark_Host_Water_Spray_Detections(
      raw_det_list,
      host,
      calibs,
      det_props);

   /** \result
   * Check that f_water_spray is set to false for the first 4 detections
   **/
   CHECK_FALSE_TEXT(det_props[0].f_water_spray, "f_water_spray falsely indicates water spray")
   CHECK_FALSE_TEXT(det_props[1].f_water_spray, "f_water_spray falsely indicates water spray")
   CHECK_FALSE_TEXT(det_props[2].f_water_spray, "f_water_spray falsely indicates water spray")
   CHECK_FALSE_TEXT(det_props[3].f_water_spray, "f_water_spray falsely indicates water spray")
   CHECK_TRUE_TEXT(det_props[4].f_water_spray, "f_water_spray does not indicate water spray")
   CHECK_TRUE_TEXT(det_props[5].f_water_spray, "f_water_spray does not indicate water spray")
}

/** \purpose
* Verify that detections have been correctly flagged as not water spray detections when host speed is below limit
*\req    CPR-3861
*/
TEST(f360_mark_water_spray_detections, low_host_speed)
{
   /** \precond
   Set host speed below speed limit
   **/
   host.speed = 4.0F;

   /** \action
   * Execute the function
   **/
   Mark_Host_Water_Spray_Detections(
      raw_det_list,
      host,
      calibs,
      det_props);

   /** \result
   * Check that f_water_spray is false for all detections
   **/
   CHECK_FALSE_TEXT(det_props[0].f_water_spray, "f_water_spray falsely indicates water spray")
   CHECK_FALSE_TEXT(det_props[1].f_water_spray, "f_water_spray falsely indicates water spray")
   CHECK_FALSE_TEXT(det_props[2].f_water_spray, "f_water_spray falsely indicates water spray")
   CHECK_FALSE_TEXT(det_props[3].f_water_spray, "f_water_spray falsely indicates water spray")
   CHECK_FALSE_TEXT(det_props[4].f_water_spray, "f_water_spray falsely indicates water spray")
   CHECK_FALSE_TEXT(det_props[5].f_water_spray, "f_water_spray falsely indicates water spray")
}

/**
*\purpose  Test behavior of host water spray detector when f_ok_to_use is set to false for one detection
*\req    CPR-3861
*/
TEST(f360_mark_water_spray_detections, f_ok_to_use_false)
{
   /** \precond
   Set f_ok_to_use to false for det 0
   **/
   det_props[0].f_ok_to_use = false;

   /** \action
   * Execute the function
   **/
   Mark_Host_Water_Spray_Detections(
      raw_det_list,
      host,
      calibs,
      det_props);

   /** \result
   * Check that f_water_spray is set to false only for det 0
   **/
   CHECK_FALSE_TEXT(det_props[0].f_water_spray, "f_water_spray indicates water spray")
   CHECK_TRUE_TEXT(det_props[1].f_water_spray, "f_water_spray does not indicate water spray")
   CHECK_TRUE_TEXT(det_props[2].f_water_spray, "f_water_spray does not indicate water spray")
   CHECK_TRUE_TEXT(det_props[3].f_water_spray, "f_water_spray does not indicate water spray")
   CHECK_TRUE_TEXT(det_props[4].f_water_spray, "f_water_spray does not indicate water spray")
   CHECK_TRUE_TEXT(det_props[5].f_water_spray, "f_water_spray does not indicate water spray")

}

/**
*\purpose  Test behaviour of host water spray detector for detection with high rcs
*\req    CPR-3861
*/
TEST(f360_mark_water_spray_detections, det_high_rcs)
{
   /** \precond
   Set rcs above limit for det 1
   **/
   raw_det_list.detections[1].raw.rcs = -24.0F;

   /** \action
   * Execute the function
   **/
   Mark_Host_Water_Spray_Detections(
      raw_det_list,
      host,
      calibs,
      det_props);

   /** \result
   * Check that f_water_spray is set to false only for det 1
   **/
   CHECK_TRUE_TEXT(det_props[0].f_water_spray, "f_water_spray does not indicate water spray")
   CHECK_FALSE_TEXT(det_props[1].f_water_spray, "f_water_spray indicates water spray")
   CHECK_TRUE_TEXT(det_props[2].f_water_spray, "f_water_spray does not indicate water spray")
   CHECK_TRUE_TEXT(det_props[3].f_water_spray, "f_water_spray does not indicate water spray")
   CHECK_TRUE_TEXT(det_props[4].f_water_spray, "f_water_spray does not indicate water spray")
   CHECK_TRUE_TEXT(det_props[5].f_water_spray, "f_water_spray does not indicate water spray")

}

/**
*\purpose  Test behaviour of host water spray detector with det range rate outside of limits
*\req    CPR-3861
*/
TEST(f360_mark_water_spray_detections, det_range_rate)
{
   /** \precond
   Set range rate below limit for det 2 and above limit for det 3
   **/
   raw_det_list.detections[2].raw.range_rate = 0.0F;
   raw_det_list.detections[3].raw.range_rate = host.speed + 1.0F;

   /** \action
   * Execute the function
   **/
   Mark_Host_Water_Spray_Detections(
      raw_det_list,
      host,
      calibs,
      det_props);

   /** \result
   * Check that f_water_spray is set to false only for det 2 and det 3
   **/
   CHECK_TRUE_TEXT(det_props[0].f_water_spray, "f_water_spray low det range rate water spray")
   CHECK_TRUE_TEXT(det_props[1].f_water_spray, "f_water_spray low det range rate water spray")
   CHECK_FALSE_TEXT(det_props[2].f_water_spray, "f_water_spray indicates water spray")
   CHECK_FALSE_TEXT(det_props[3].f_water_spray, "f_water_spray low det range rate water spray")
   CHECK_TRUE_TEXT(det_props[4].f_water_spray, "f_water_spray low det range rate water spray")
   CHECK_TRUE_TEXT(det_props[5].f_water_spray, "f_water_spray low det range rate water spray")

}
/** @}*/

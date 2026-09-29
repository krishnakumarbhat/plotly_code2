/** \file
 * This file contains unit tests for content of f360_handle_host_vehicle_clutter.cpp file
 */

#include "f360_handle_host_vehicle_clutter.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_handle_host_vehicle_clutter
 *  @{
 */

 /** \brief
  * Test group of Handle_Host_Vehicle_Clutter function. Tests verify whether detections are marked as not okay to use
  * if they are placed inside HVC TP area.
  */
TEST_GROUP(f360_handle_host_vehicle_clutter)
{
   F360_Detection_Props_T det_p[MAX_NUMBER_OF_DETECTIONS];
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];
   F360_Calibrations_T calib;
   rspp_variant_A::RSPP_Detection_T &det_raw = raw_detection_list.detections[0];
   F360_Detection_Props_T &det_prop = det_p[0];

   /** \setup
    * Initialize tracker calibrations
    * Set selected detection parameters
    * Set sensors parameters
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      raw_detection_list.number_of_valid_detections = 1;
      raw_detection_list.detections[0].raw.f_host_veh_clutter = true;

      sensors[0].constant.mounting_position.vcs_position.lateral = -2.0F;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = -2.0F;

      sensors[1].constant.mounting_position.vcs_position.lateral = -1.0F;
      sensors[1].constant.mounting_position.vcs_position.longitudinal = 2.0F;

      sensors[2].constant.mounting_position.vcs_position.lateral = 2.0F;
      sensors[2].constant.mounting_position.vcs_position.longitudinal = -1.0F;

      sensors[3].constant.mounting_position.vcs_position.lateral = 3.0F;
      sensors[3].constant.mounting_position.vcs_position.longitudinal = -1.5F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether detections inside HVC TP area are marked as not okay to use
 * \req
 * NA.
 */
TEST(f360_handle_host_vehicle_clutter, handle_host_vehicle_clutter__dets_marked_as_not_okay_to_use)
{
   /** \precond
   * Set detection position to be placed inside HVC TP area
    */
   det_prop.vcs_position.y = 0.0F;
   det_prop.vcs_position.x = 0.0F;
   det_prop.f_ok_to_use = true;
   det_prop.behind_sep_id = F360_INVALID_UNSIGNED_ID;
   det_prop.on_sep_id = F360_INVALID_UNSIGNED_ID;

   /** \action
    * Call tested function
    */
   Handle_Host_Vehicle_Clutter(det_raw, sensors, calib, det_prop);

   /** \result
   * Check whether detection f_ok_to_use flag was set to false
    */
   CHECK_FALSE(det_prop.f_ok_to_use)
}

/** \purpose
* Purpose of this test is to verify whether detections outside HVC TP zone, not on and not behind guardrail do not have their f_ok_to_use flag changed
* \req
* NA.
*/
TEST(f360_handle_host_vehicle_clutter, handle_host_vehicle_clutter__dets_marked_as_okay_to_use)
{
   /** \precond
   * Set detection position to be placed outside HVC TP area
   * Set detection f_ok_to_use flag as true
   */
   det_prop.vcs_position.y = -50.0F;
   det_prop.vcs_position.x = 50.0F;
   det_prop.f_ok_to_use = true;
   det_prop.behind_sep_id = F360_INVALID_UNSIGNED_ID;
   det_prop.on_sep_id = F360_INVALID_UNSIGNED_ID;

   /** \action
   * Call tested function
   */
   Handle_Host_Vehicle_Clutter(det_raw, sensors, calib, det_prop);

   /** \result
   * Check whether detection f_ok_to_use flag was not changed
   */
   CHECK_TRUE(det_prop.f_ok_to_use)
}

/** \purpose
* Purpose of this test is to verify whether detections that are behind guardrail and HVC are marked as not okay to use
* \req
* NA.
*/
TEST(f360_handle_host_vehicle_clutter, handle_host_vehicle_clutter__hvc_and_behind_guardrail_dets_marked_as_not_okay_to_use)
{
   /** \precond
   * Set detection position to be placed inside HVC TP area
   */
   det_prop.vcs_position.y = 50.0F;
   det_prop.vcs_position.x = 0.0F;
   det_prop.f_ok_to_use = true;
   det_prop.behind_sep_id = 1;
   det_prop.on_sep_id = F360_INVALID_UNSIGNED_ID;

   /** \action
   * Call tested function
   */
   Handle_Host_Vehicle_Clutter(det_raw, sensors, calib, det_prop);

   /** \result
   * Check whether detection f_ok_to_use flag was set to false
   */
   CHECK_FALSE(det_prop.f_ok_to_use)
}

/** \purpose
* Purpose of this test is to verify whether detections that are on guardrail and HVC are marked as not okay to use
* \req
* NA.
*/
TEST(f360_handle_host_vehicle_clutter, handle_host_vehicle_clutter__hvc_and_on_guardrail_dets_marked_as_not_okay_to_use)
{
   /** \precond
   * Set detection position to be placed inside HVC TP area
   */
   det_prop.vcs_position.y = 50.0F;
   det_prop.vcs_position.x = 0.0F;
   det_prop.f_ok_to_use = true;
   det_prop.behind_sep_id = F360_INVALID_UNSIGNED_ID;
   det_prop.on_sep_id = 1;

   /** \action
   * Call tested function
   */
   Handle_Host_Vehicle_Clutter(det_raw, sensors, calib, det_prop);

   /** \result
   * Check whether detection f_ok_to_use flag was set to false
   */
   CHECK_FALSE(det_prop.f_ok_to_use)
}
/** @}*/

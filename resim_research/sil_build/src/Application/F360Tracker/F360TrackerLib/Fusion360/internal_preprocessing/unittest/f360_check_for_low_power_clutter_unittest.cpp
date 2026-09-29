/** \file
 * This file contains unit tests for content of f360_check_for_low_power_clutter.cpp file
 */

#include "f360_check_for_low_power_clutter.h"
#include "f360_math.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"
#include <CppUTest/TestHarness.h>
#include "f360_set_variant.h"

using namespace f360_variant_A;

static void Add_Detection(
   const int16_t idx,
   const float32_t vcs_pos_x,
   const float32_t vcs_pos_y,
   const int32_t sensor_id,
   const int8_t confid_az,
   rspp_variant_A::RSPP_Detection_List_T (&raw_detection_list),
   F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   );

/** \defgroup  f360_check_for_low_power_clutter
 *  @{
 */

/** \brief
 * Test general functionality of f360_check_for_low_power_clutter
 */
TEST_GROUP(f360_check_for_low_power_clutter)
{
   F360_Host_T host;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS]{};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Tracker_Info_T tracker_info{};
   const float32_t min_vcs_longpos_threshold = 8.0F;
   const float32_t max_vcs_latpos_threshold = 1.0F;
   const float32_t next_det_longpos_threshold = 50.0F;
   const float32_t test_threshold = 0.0001F;
};

/** \purpose
 * Verify that the main call of Check_For_Low_Power_Clutter will reduce severity value and will not update clutter flag given that:
   * host is stationary
   * there is are no matching, valid detection
 * \req NA.
 */
TEST(f360_check_for_low_power_clutter, f360_check_for_low_power_clutter_main_call_no_dets_host_stationary)
{
   /** \precond
   * host is stationary
   * f_low_power_clutter is set to arbitary value
   * low_power_clutter_severity has non-zero value
   * at least one valid sensor
   **/
   host.speed = 0.0F;
   tracker_info.f_low_power_clutter = true;
   sensors[0].variable.is_valid = true;
   sensor_props[0].low_power_clutter_severity = 0.5F;

   /** \action
   * Call Check_For_Low_Power_Clutter
   */
   Check_For_Low_Power_Clutter(host, sensors, raw_detection_list, sensor_props, detection_props, tracker_info);

   /** \result
    * Severity value reduced properly
    * Confirm the flag state did not change
    */
   DOUBLES_EQUAL(0.4975F, tracker_info.low_power_clutter_severity, test_threshold);
   CHECK_TRUE(tracker_info.f_low_power_clutter)
}

/** \purpose
 * Verify that the main call of Check_For_Low_Power_Clutter will reduce severity value and will not update clutter flag given that:
   * host speed is negative
   * there is are no matching, valid detection
 * \req NA.
 */
TEST(f360_check_for_low_power_clutter, f360_check_for_low_power_clutter_main_call_no_dets_host_neg_speed)
{
   /** \precond
   * host speed is negative
   * f_low_power_clutter is set to arbitary value
   * low_power_clutter_severity has non-zero value
   * at least one valid sensor
   **/
   host.speed = -1.0F;
   tracker_info.f_low_power_clutter = true;
   sensors[0].variable.is_valid = true;
   sensor_props[0].low_power_clutter_severity = 0.5F;

   /** \action
   * Call Check_For_Low_Power_Clutter
   */
   Check_For_Low_Power_Clutter(host, sensors, raw_detection_list, sensor_props, detection_props, tracker_info);

   /** \result
    * Severity value reduced properly
    * Confirm the flag state did not change
    */
   DOUBLES_EQUAL(0.4975F, tracker_info.low_power_clutter_severity, test_threshold);
   CHECK_TRUE(tracker_info.f_low_power_clutter)
}

/** \purpose
 * Verify that the main call of Check_For_Low_Power_Clutter will reduce severity value and will not update clutter flag given that:
   * host is moving
   * there is one matching, valid detection
   * detection properties are set in a way the low_power_clutter flag will not be updated
 * \req NA.
 */
TEST(f360_check_for_low_power_clutter, f360_check_for_low_power_clutter_main_call_no_dets_host_moving_one_det)
{
   /** \precond
   * host is moving
   * f_low_power_clutter is set to arbitary value
   * low_power_clutter_severity has non-zero value
   * at least one valid SRR6-type sensor
   * set one detection within ROI with lowest az confidence
   **/
   host.speed = 1.0F;
   tracker_info.f_low_power_clutter = true;
   sensors[0].variable.is_valid = true;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR;
   sensor_props[0].low_power_clutter_severity = 0.5F;
   raw_detection_list.vcslong_sorted_ref_det_idx[1] = 0;
   raw_detection_list.detections[0].processed.next_sorted_idx = -1;
   Add_Detection(0, min_vcs_longpos_threshold + 0.1F, max_vcs_latpos_threshold - 0.1F, 1, 3, raw_detection_list, detection_props);

   /** \action
   * Call Check_For_Low_Power_Clutter
   */
   Check_For_Low_Power_Clutter(host, sensors, raw_detection_list, sensor_props, detection_props, tracker_info);

   /** \result
    * Severity value reduced properly
    * Confirm the flag state did not change
    */
   DOUBLES_EQUAL(0.4975F, tracker_info.low_power_clutter_severity, test_threshold);
   CHECK_TRUE(tracker_info.f_low_power_clutter);
}


/** \purpose
 * Verify that the main call of Check_For_Low_Power_Clutter will reduce severity value and will not update clutter flag given that:
   * host is moving
   * there is one matching, valid detection
   * detection properties are set in a way the low_power_clutter flag will not be updated
 * \req NA.
 */
TEST(f360_check_for_low_power_clutter, f360_check_for_low_power_clutter_main_call_no_dets_host_moving_one_det_snsrtype1)
{
   /** \precond
   * host is moving
   * f_low_power_clutter is set to arbitary value
   * low_power_clutter_severity has non-zero value
   * one valid F360_SENSOR_TYPE_FLR4_PLT_RADAR sensor
   * set one detection within ROI with lowest az confidence
   **/
   host.speed = 1.0F;
   tracker_info.f_low_power_clutter = true;
   sensors[0].variable.is_valid = true;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_PLT_RADAR;
   sensor_props[0].low_power_clutter_severity = 0.5F;
   raw_detection_list.vcslong_sorted_ref_det_idx[1] = 0;
   raw_detection_list.detections[0].processed.next_sorted_idx = -1;
   Add_Detection(0, min_vcs_longpos_threshold + 0.1F, max_vcs_latpos_threshold - 0.1F, 1, 3, raw_detection_list, detection_props);

   /** \action
   * Call Check_For_Low_Power_Clutter
   */
   Check_For_Low_Power_Clutter(host, sensors, raw_detection_list, sensor_props, detection_props, tracker_info);

   /** \result
    * Severity value reduced properly
    * Confirm the flag state did not change
    */
   DOUBLES_EQUAL(0.4975F, tracker_info.low_power_clutter_severity, test_threshold);
   CHECK_TRUE(tracker_info.f_low_power_clutter);
}

/** \purpose
 * Verify that the main call of Check_For_Low_Power_Clutter will reduce severity value and will not update clutter flag given that:
   * host is moving
   * there is one matching, valid detection
   * detection properties are set in a way the low_power_clutter flag will not be updated
 * \req NA.
 */
TEST(f360_check_for_low_power_clutter, f360_check_for_low_power_clutter_main_call_no_dets_host_moving_one_det_snsrtype2)
{
   /** \precond
   * host is moving
   * f_low_power_clutter is set to arbitary value
   * low_power_clutter_severity has non-zero value
   * one valid F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR sensor
   * set one detection within ROI with lowest az confidence
   **/
   host.speed = 1.0F;
   tracker_info.f_low_power_clutter = true;
   sensors[0].variable.is_valid = true;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR;
   sensor_props[0].low_power_clutter_severity = 0.5F;
   raw_detection_list.vcslong_sorted_ref_det_idx[1] = 0;
   raw_detection_list.detections[0].processed.next_sorted_idx = -1;
   Add_Detection(0, min_vcs_longpos_threshold + 0.1F, max_vcs_latpos_threshold - 0.1F, 1, 3, raw_detection_list, detection_props);

   /** \action
   * Call Check_For_Low_Power_Clutter
   */
   Check_For_Low_Power_Clutter(host, sensors, raw_detection_list, sensor_props, detection_props, tracker_info);

   /** \result
    * Severity value reduced properly
    * Confirm the flag state did not change
    */
   DOUBLES_EQUAL(0.4975F, tracker_info.low_power_clutter_severity, test_threshold);
   CHECK_TRUE(tracker_info.f_low_power_clutter);
}

/** \purpose
 * Verify that the main call of Check_For_Low_Power_Clutter will reduce severity value and will not update clutter flag given that:
   * host is moving
   * there is one matching, valid detection
   * detection properties are set in a way the low_power_clutter flag will not be updated
 * \req NA.
 */
TEST(f360_check_for_low_power_clutter, f360_check_for_low_power_clutter_main_call_no_dets_host_moving_one_det_snsrtype3)
{
   /** \precond
   * host is moving
   * f_low_power_clutter is set to arbitary value
   * low_power_clutter_severity has non-zero value
   * one valid F360_SENSOR_TYPE_SRR6_PLUS_PLT_1GB_EHT_RADAR sensor
   * set one detection within ROI with lowest az confidence
   **/
   host.speed = 1.0F;
   tracker_info.f_low_power_clutter = true;
   sensors[0].variable.is_valid = true;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR6_PLUS_PLT_1GB_EHT_RADAR;
   sensor_props[0].low_power_clutter_severity = 0.5F;
   raw_detection_list.vcslong_sorted_ref_det_idx[1] = 0;
   raw_detection_list.detections[0].processed.next_sorted_idx = -1;
   Add_Detection(0, min_vcs_longpos_threshold + 0.1F, max_vcs_latpos_threshold - 0.1F, 1, 3, raw_detection_list, detection_props);

   /** \action
   * Call Check_For_Low_Power_Clutter
   */
   Check_For_Low_Power_Clutter(host, sensors, raw_detection_list, sensor_props, detection_props, tracker_info);

   /** \result
    * Severity value reduced properly
    * Confirm the flag state did not change
    */
   DOUBLES_EQUAL(0.4975F, tracker_info.low_power_clutter_severity, test_threshold);
   CHECK_TRUE(tracker_info.f_low_power_clutter);
}

/** \purpose
 * Verify that the main call of Check_For_Low_Power_Clutter will reduce severity value and will not update clutter flag given that:
   * host is moving
   * there is one matching, valid detection
   * detection properties are set in a way the low_power_clutter flag will not be updated
 * \req NA.
 */
TEST(f360_check_for_low_power_clutter, f360_check_for_low_power_clutter_main_call_no_dets_host_moving_one_det_snsrtype4)
{
   /** \precond
   * host is moving
   * f_low_power_clutter is set to arbitary value
   * low_power_clutter_severity has non-zero value
   * one valid F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XETH_RADAR sensor
   * set one detection within ROI with lowest az confidence
   **/
   host.speed = 1.0F;
   tracker_info.f_low_power_clutter = true;
   sensors[0].variable.is_valid = true;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XETH_RADAR;
   sensor_props[0].low_power_clutter_severity = 0.5F;
   raw_detection_list.vcslong_sorted_ref_det_idx[1] = 0;
   raw_detection_list.detections[0].processed.next_sorted_idx = -1;
   Add_Detection(0, min_vcs_longpos_threshold + 0.1F, max_vcs_latpos_threshold - 0.1F, 1, 3, raw_detection_list, detection_props);

   /** \action
   * Call Check_For_Low_Power_Clutter
   */
   Check_For_Low_Power_Clutter(host, sensors, raw_detection_list, sensor_props, detection_props, tracker_info);

   /** \result
    * Severity value reduced properly
    * Confirm the flag state did not change
    */
   DOUBLES_EQUAL(0.4975F, tracker_info.low_power_clutter_severity, test_threshold);
   CHECK_TRUE(tracker_info.f_low_power_clutter);
}

/** \purpose
 * Verify that the main call of Check_For_Low_Power_Clutter will reduce severity value and will not update clutter flag given that:
   * host is moving
   * there is one matching, valid detection
   * detection properties are set in a way the low_power_clutter flag will not be updated
 * \req NA.
 */
TEST(f360_check_for_low_power_clutter, f360_check_for_low_power_clutter_main_call_no_dets_host_moving_one_det_snsrtype5)
{
   /** \precond
   * host is moving
   * f_low_power_clutter is set to arbitary value
   * low_power_clutter_severity has non-zero value
   * one valid F360_SENSOR_TYPE_SRR6_PLUS_PLT_RADAR sensor
   * set one detection within ROI with lowest az confidence
   **/
   host.speed = 1.0F;
   tracker_info.f_low_power_clutter = true;
   sensors[0].variable.is_valid = true;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR6_PLUS_PLT_RADAR;
   sensor_props[0].low_power_clutter_severity = 0.5F;
   raw_detection_list.vcslong_sorted_ref_det_idx[1] = 0;
   raw_detection_list.detections[0].processed.next_sorted_idx = -1;
   Add_Detection(0, min_vcs_longpos_threshold + 0.1F, max_vcs_latpos_threshold - 0.1F, 1, 3, raw_detection_list, detection_props);

   /** \action
   * Call Check_For_Low_Power_Clutter
   */
   Check_For_Low_Power_Clutter(host, sensors, raw_detection_list, sensor_props, detection_props, tracker_info);

   /** \result
    * Severity value reduced properly
    * Confirm the flag state did not change
    */
   DOUBLES_EQUAL(0.4975F, tracker_info.low_power_clutter_severity, test_threshold);
   CHECK_TRUE(tracker_info.f_low_power_clutter);
}

/** \purpose
 * Verify that the main call of Check_For_Low_Power_Clutter will reduce severity value and will not update clutter flag given that:
   * host is moving
   * there is one matching, valid detection with lateral position higher that latpos threshold
   * detection properties are set in a way the low_power_clutter flag will not be updated
 * \req NA.
 */
TEST(f360_check_for_low_power_clutter, f360_check_for_low_power_clutter_main_call_no_dets_host_moving_one_det_not_in_latpos)
{
   /** \precond
   * host is moving
   * f_low_power_clutter is set to arbitary value
   * low_power_clutter_severity has non-zero value
   * at least one valid SRR6-type sensor
   * set one detection within ROI with lowest az confidence but not in the lateral position threshold
   **/
   host.speed = 1.0F;
   tracker_info.f_low_power_clutter = true;
   sensors[0].variable.is_valid = true;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR;
   sensor_props[0].low_power_clutter_severity = 0.5F;
   raw_detection_list.vcslong_sorted_ref_det_idx[1] = 0;
   raw_detection_list.detections[0].processed.next_sorted_idx = -1;
   Add_Detection(0, min_vcs_longpos_threshold + 0.1F, max_vcs_latpos_threshold + 0.1F, 1, 3, raw_detection_list, detection_props);

   /** \action
   * Call Check_For_Low_Power_Clutter
   */
   Check_For_Low_Power_Clutter(host, sensors, raw_detection_list, sensor_props, detection_props, tracker_info);

   /** \result
    * Severity value reduced properly
    * Confirm the flag state did not change
    */
   DOUBLES_EQUAL(0.4975F, tracker_info.low_power_clutter_severity, test_threshold);
   CHECK_TRUE(tracker_info.f_low_power_clutter);
}

/** \purpose
 * Verify that the main call of Check_For_Low_Power_Clutter will reduce severity value and will not update clutter flag given that:
   * host is moving
   * there is one matching, valid detection
   * there is one valid detection outside longitudinal zone, so the loop will break
   * detection properties are set in a way the low_power_clutter flag will not be updated
 * \req NA.
 */
TEST(f360_check_for_low_power_clutter, f360_check_for_low_power_clutter_main_call_no_dets_host_moving_two_dets)
{
   /** \precond
   * host is moving
   * f_low_power_clutter is set to arbitary value
   * low_power_clutter_severity has non-zero value
   * at least one valid SRR6-type sensor
   * set two detections
      * one within ROI with lowest az confidence
      * one not to be considered
   **/
   host.speed = 1.0F;
   tracker_info.f_low_power_clutter = true;
   sensors[0].variable.is_valid = true;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR;
   sensor_props[0].low_power_clutter_severity = 0.5F;
   raw_detection_list.vcslong_sorted_ref_det_idx[1] = 0;
   raw_detection_list.vcslong_sorted_ref_det_idx[2] = 1;
   raw_detection_list.detections[0].processed.next_sorted_idx = 1;
   raw_detection_list.detections[1].processed.next_sorted_idx = -1;
   Add_Detection(0, min_vcs_longpos_threshold + 0.1F, max_vcs_latpos_threshold - 0.1F, 1, 3, raw_detection_list, detection_props);
   Add_Detection(1, next_det_longpos_threshold + 0.1F, max_vcs_latpos_threshold - 0.1F, 1, 3, raw_detection_list, detection_props);

   /** \action
   * Call Check_For_Low_Power_Clutter
   */
   Check_For_Low_Power_Clutter(host, sensors, raw_detection_list, sensor_props, detection_props, tracker_info);

   /** \result
    * Severity value reduced properly
    * Check f_low_power_clutter state did not change
    */
   DOUBLES_EQUAL(0.4975F, tracker_info.low_power_clutter_severity, test_threshold);
   CHECK_TRUE(tracker_info.f_low_power_clutter);
}

/** \purpose
 * Verify that the main call of Check_For_Low_Power_Clutter will properly increase severity value and will update clutter flag given that:
   * host is moving
   * there are 10 matching, valid detection
   * detection properties are set in a way the low_power_clutter flag will be updated
 * \req NA.
 */
TEST(f360_check_for_low_power_clutter, f360_check_for_low_power_clutter_main_call_severity_check_true)
{
   /** \precond
   * host is moving
   * f_low_power_clutter is set to false
   * low_power_clutter_severity has upper edge value
   * at least one valid SRR6-type sensor
   * multiple valid detections indicating rain clutter
   **/
   host.speed = 1.0F;
   tracker_info.f_low_power_clutter = false;
   sensors[0].variable.is_valid = true;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR;
   sensor_props[0].low_power_clutter_severity = 0.65F;
   for (uint8_t i = 0U; i < 10U; i++)
   {
      Add_Detection(i, min_vcs_longpos_threshold + 0.1F*i, max_vcs_latpos_threshold - 0.1F*i, 1, 3, raw_detection_list, detection_props);
      raw_detection_list.vcslong_sorted_ref_det_idx[i+1] = i;
      raw_detection_list.detections[i].processed.next_sorted_idx = i+1;
   }

   /** \action
   * Call Check_For_Low_Power_Clutter
   */
   Check_For_Low_Power_Clutter(host, sensors, raw_detection_list, sensor_props, detection_props, tracker_info);

   /** \result
    * Severity value increased properly
    * Check f_low_power_clutter state did change to true
    */
   DOUBLES_EQUAL(0.6675F, tracker_info.low_power_clutter_severity, test_threshold);
   CHECK_TRUE(tracker_info.f_low_power_clutter);
}

/** \purpose
 * Verify that the main call of Check_For_Low_Power_Clutter will also uses radar rain level for calculation
 * of final severity value
 * \req NA.
 */
TEST(f360_check_for_low_power_clutter, f360_check_for_low_power_clutter_gen7)
{
   /** \precond
   * host is moving
   * f_low_power_clutter is set to arbitary value
   * low_power_clutter_severity is set to 1
   * tracler_info.rain_level_filtered is set to 0.2F (some low value)
   * at least one valid gen7 sensor and one detection coming from it
   **/
   host.speed = 0.1F;
   tracker_info.f_low_power_clutter = false;
   sensors[0].variable.is_valid = true;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_RADAR;
   sensor_props[0].low_power_clutter_severity = 1.0F;
   tracker_info.rain_level_filtered = 0.2F;

   raw_detection_list.number_of_valid_detections = 1U;
   raw_detection_list.detections[0].raw.sensor_id = 1;
   detection_props[0].vcs_position.x = min_vcs_longpos_threshold + 0.1F;
   detection_props[0].vcs_position.y = 0.0F;


   /** \action
   * Call Check_For_Low_Power_Clutter
   */
   Check_For_Low_Power_Clutter(host, sensors, raw_detection_list, sensor_props, detection_props, tracker_info);

   /** \result
    * Confirm the flag state did activate
    */
   CHECK_FALSE(tracker_info.f_low_power_clutter)
}

/** @}*/
/** \defgroup  f360_is_det_in_roi
 *  @{
 */

/** \brief
 * Test Is_Det_In_ROI() functions from f360_check_for_low_power_clutter
 */
TEST_GROUP(f360_is_det_in_roi)
{
   const float32_t min_vcs_longpos_threshold = 8.0F;
   const float32_t max_vcs_longpos_threshold = 50.0F;
   const float32_t max_vcs_latpos_threshold = 3.0F;
   F360_Detection_Props_T detection_prop;
   bool f_check_abs_longpos = false;
};

/** \purpose
 * Verify that detection is considered properly to be in the zone
 * \req NA.
 */
TEST(f360_is_det_in_roi, det_in_roi)
{
   /** \precond
   * detection vcs_position.x inside of roi
   * detection vcs_position.y inside of roi
   **/

   detection_prop.vcs_position.x = min_vcs_longpos_threshold + 0.01F;
   detection_prop.vcs_position.y = max_vcs_latpos_threshold - 0.01F;

   /** \action
    * Call Is_Det_In_ROI
    */
   bool f_det_in_roi = Is_Det_In_ROI(min_vcs_longpos_threshold, max_vcs_longpos_threshold, detection_prop, f_check_abs_longpos);

   /** \result
    * Check that position is correctly flagged as in ROI
    */
   CHECK_TRUE(f_det_in_roi)
}

/** \purpose
 * Verify that detection is considered properly to be in the zone when its lateral position is above the constant threshold, 
 * but its longpos is at the edge of the threshold so condition for variable lateral position is checked.
 * \req NA.
 */
TEST(f360_is_det_in_roi, det_in_roi_significant_longpos)
{
   /** \precond
   * detection vcs_position.x at the edge of longpos threshold
   * detection vcs_position.y above const latpos threshold
   **/

   detection_prop.vcs_position.x = max_vcs_longpos_threshold - 0.01F;
   detection_prop.vcs_position.y = max_vcs_latpos_threshold + 0.1F;

   /** \action
    * Call Is_Det_In_ROI
    */
   bool f_det_in_roi = Is_Det_In_ROI(min_vcs_longpos_threshold, max_vcs_longpos_threshold, detection_prop, f_check_abs_longpos);

   /** \result
    * Check that position is correctly flagged as in ROI
    */
   CHECK_TRUE(f_det_in_roi)
}

/** \purpose
 * Verify that detection is considered properly to not be in the zone, if its longpos is above set thresholds and
 * its lateral position is within the boundaries.
 * \req NA.
 */
TEST(f360_is_det_in_roi, det_not_in_longzone)
{
   /** \precond
   * detection vcs_position.x beyond longzone
   * detection vcs_position.y in latzone
   **/

   detection_prop.vcs_position.x = max_vcs_longpos_threshold + 0.01F;
   detection_prop.vcs_position.y = max_vcs_latpos_threshold - 0.1F;

   /** \action
    * Call Is_Det_In_ROI
    */
   bool f_det_in_roi = Is_Det_In_ROI(min_vcs_longpos_threshold, max_vcs_longpos_threshold, detection_prop, f_check_abs_longpos);

   /** \result
    * Check that position is correctly flagged as not in ROI
    */
   CHECK_FALSE(f_det_in_roi)
}

/** \purpose
 * Verify that detection is considered properly to not be in the zone, if it has its lateral position above set thresholds,
 * and its longitudinal position within boundaries.
 * \req NA.
 */
TEST(f360_is_det_in_roi, det_not_in_latzone)
{
   /** \precond
   * detection vcs_position.x in longzone
   * detection vcs_position.y not in latzone
   **/

   detection_prop.vcs_position.x = max_vcs_longpos_threshold - 0.01F;
   detection_prop.vcs_position.y = 100.0F;

   /** \action
    * Call Is_Det_In_ROI
    */
   bool f_det_in_roi = Is_Det_In_ROI(min_vcs_longpos_threshold, max_vcs_longpos_threshold, detection_prop, f_check_abs_longpos);

   /** \result
    * Check that position is correctly flagged as not in ROI
    */
   CHECK_FALSE(f_det_in_roi)
}

/** \purpose
 * Verify that detection is considered properly to be in the zone, if it position has negative value but flag for checking
 * absolute position is set.
 * \req NA.
 */
TEST(f360_is_det_in_roi, check_abs_det_in_zone)
{
   /** \precond
   * detection absolute vcs_position.x in longzone
   **/
   f_check_abs_longpos = true;
   detection_prop.vcs_position.x = - max_vcs_longpos_threshold + 0.01F;

   /** \action
    * Call Is_Det_In_ROI
    */
   bool f_det_in_roi = Is_Det_In_ROI(min_vcs_longpos_threshold, max_vcs_longpos_threshold, detection_prop, f_check_abs_longpos);

   /** \result
    * Check that position is correctly flagged as in ROI
    */
   CHECK_TRUE(f_det_in_roi)
}

/** \purpose
 * Verify that detection is considered properly to not be in the zone, if it's absolute x position is above set thresholds.
 * \req NA.
 */
TEST(f360_is_det_in_roi, check_abs_det_not_in_zone_low_longpos)
{
   /** \precond
   * detection absolute vcs_position.x higher than threshold
   **/
   f_check_abs_longpos = true;
   detection_prop.vcs_position.x = - max_vcs_longpos_threshold - 0.01F;

   /** \action
    * Call Is_Det_In_ROI
    */
   bool f_det_in_roi = Is_Det_In_ROI(min_vcs_longpos_threshold, max_vcs_longpos_threshold, detection_prop, f_check_abs_longpos);

   /** \result
    * Check that position is correctly flagged as not in ROI
    */
   CHECK_FALSE(f_det_in_roi)
}

/** \purpose
 * Verify that detection is considered properly to not be in the zone, if it's absolute x position is below set thresholds.
 * \req NA.
 */
TEST(f360_is_det_in_roi, check_abs_det_not_in_zone_big_longpos)
{
   /** \precond
   * detection absolute vcs_position.x lower than threshold
   **/
   f_check_abs_longpos = true;
   detection_prop.vcs_position.x = - min_vcs_longpos_threshold + 0.01F;

   /** \action
    * Call Is_Det_In_ROI
    */
   bool f_det_in_roi = Is_Det_In_ROI(min_vcs_longpos_threshold, max_vcs_longpos_threshold, detection_prop, f_check_abs_longpos);

   /** \result
    * Check that position is correctly flagged as not in ROI
    */
   CHECK_FALSE(f_det_in_roi)
}

/** @}*/
/** \defgroup  f360_detection_props_match_hypothesis
 *  @{
 */

/** \brief
 * Test Detection_Props_Match_Hypothesis() function from f360_check_for_low_power_clutter
 */
TEST_GROUP(f360_detection_props_match_hypothesis)
{
   const float32_t detection_rcs_threshold = -20.0F;
   const int8_t detection_az_confid_higher = 2;
   const int8_t detection_az_confid_lower = 3;
   rspp_variant_A::RSPP_Detection_T detection;
   F360_Detection_Props_T detection_prop;
   low_power_clutter_thresholds thresholds{};

   /** \setup
    * Initialize thresholds' values
    */
   TEST_SETUP()
   {
      thresholds.max_rcs = detection_rcs_threshold;
      thresholds.max_snr = 10.0F;
      thresholds.detection_az_confid = detection_az_confid_lower;
   }
};

/** \purpose
 * Verify that detection is considered when its azimuth confidence is not the lowest, but its rcs value matches the hypothesis.
 * \req NA.
 */
TEST(f360_detection_props_match_hypothesis, det_matches_hypothesis_rcs)
{
   /** \precond
   * sensor type is gen6 sensor
   * detection rcs below threshold
   * detection has low azimuth confidence
   **/
   detection.raw.rcs = detection_rcs_threshold - 0.01F;
   detection.raw.confid_azimuth = detection_az_confid_higher;
   thresholds.f_gen6_sensor = true;

   /** \action
    * Call Detection_Props_Match_Hypothesis
    */
   bool f_det_matches_hypothesis = Detection_Props_Match_Hypothesis(detection, detection_prop, thresholds);

   /** \result
    * Confirm detection matches hypothesis
    */
   CHECK_TRUE(f_det_matches_hypothesis)
}

/** \purpose
 * Verify that detection is considered when its azimuth confidence is the lowest and its rcs does not have to be checked.
 * \req NA.
 */
TEST(f360_detection_props_match_hypothesis, det_matches_hypothesis_az_confid)
{
   /** \precond
   * sensor type is gen6 sensor
   * detection rcs above threshold
   * detection has the lowest azimuth confidence
   **/
   thresholds.f_gen6_sensor = true;
   detection.raw.rcs = detection_rcs_threshold + 0.01F;
   detection.raw.confid_azimuth = detection_az_confid_lower;

   /** \action
    * Call Detection_Props_Match_Hypothesis
    */
   bool f_det_matches_hypothesis = Detection_Props_Match_Hypothesis(detection, detection_prop, thresholds);

   /** \result
    * Confirm detection matches hypothesis
    */
   CHECK_TRUE(f_det_matches_hypothesis)
}

/** \purpose
 * Verify that detection is not considered if sensor type is not gen6 radar
 * \req NA.
 */
TEST(f360_detection_props_match_hypothesis, det_does_not_match_hypothesis_sensor_type)
{
   /** \precond
   * sensor type is not gen6 sensor
   * detection rcs above threshold
   * detection has the lowest azimuth confidence
   **/
   thresholds.f_gen6_sensor = false;
   detection.raw.rcs = detection_rcs_threshold - 0.01F;
   detection.raw.confid_azimuth = detection_az_confid_lower;

   /** \action
    * Call Detection_Props_Match_Hypothesis
    */
   bool f_det_matches_hypothesis = Detection_Props_Match_Hypothesis(detection, detection_prop, thresholds);

   /** \result
    * Confirm detection matches hypothesis
    */
   CHECK_FALSE(f_det_matches_hypothesis)
}

/** \purpose
 * Verify that detection is not considered if its az confidence is good
 * \req NA.
 */
TEST(f360_detection_props_match_hypothesis, det_does_not_match_hypothesis_high_az_confid)
{
   /** \precond
   * sensor type is gen6 sensor
   * detection rcs above threshold
   * detection has the highest azimuth confidence
   **/
   thresholds.f_gen6_sensor = true;
   detection.raw.rcs = detection_rcs_threshold - 0.01F;
   detection.raw.confid_azimuth = 0;

   /** \action
    * Call Detection_Props_Match_Hypothesis
    */
   bool f_det_matches_hypothesis = Detection_Props_Match_Hypothesis(detection, detection_prop, thresholds);

   /** \result
    * Confirm detection matches hypothesis
    */
   CHECK_FALSE(f_det_matches_hypothesis)
}


/** @}*/
/** \defgroup  f360_detection_props_match_hypothesis_gen7
 *  @{
 */

/** \brief
 * Test Detection_Props_Match_Hypothesis() function from f360_check_for_low_power_clutter for gen7 sensors
 */
TEST_GROUP(f360_detection_props_match_hypothesis_gen7)
{
   const float32_t detection_rcs_threshold = -10.0F;
   const float32_t detection_snr_threshold = 10.0F;
   const int8_t detection_az_confid__threshold = 3;
   rspp_variant_A::RSPP_Detection_T detection;
   F360_Detection_Props_T detection_prop;
   low_power_clutter_thresholds thresholds{};

   /** \setup
    * Initialize thresholds' values
    * Initialize detection properties so that it will barely match the hypothesis
    */
   TEST_SETUP()
   {
      thresholds.max_rcs = detection_rcs_threshold;
      thresholds.max_snr = detection_snr_threshold;
      thresholds.detection_az_confid = detection_az_confid__threshold;
      thresholds.f_gen6_sensor = false;
      thresholds.f_flr7_sensor = true;

      detection.raw.rcs = detection_rcs_threshold - 0.1F;
      detection.raw.snr = detection_snr_threshold - 0.1F;
      detection.raw.confid_azimuth = detection_az_confid__threshold;
      detection_prop.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   }
};
/** \purpose
 * Verify that detection matches hypothesis for Gen7 SRR sensor when properties are within limits.
 * \req NA.
 */
TEST(f360_detection_props_match_hypothesis_gen7, det_does_match_hypothesis)
{
   /** \precond
   * sensor type is SRR
   **/
   thresholds.f_flr7_sensor = false;
   thresholds.f_srr7plus_sensor = true;

   /** \action
    * Call Detection_Props_Match_Hypothesis
    */
   bool f_det_matches_hypothesis = Detection_Props_Match_Hypothesis(detection, detection_prop, thresholds);

   /** \result
    * Confirm detection matches hypothesis
    */
   CHECK_TRUE(f_det_matches_hypothesis)
}

/** \purpose
 * Verify that detection does not match hypothesis if sensor is not Gen7.
 * \req NA.
 */
TEST(f360_detection_props_match_hypothesis_gen7, does_NOT_match_hypothesis_unknown_gen)
{
   /** \precond
   * sensor type is not gen7 sensor
   **/
   thresholds.f_flr7_sensor = false;

   /** \action
    * Call Detection_Props_Match_Hypothesis
    */
   bool f_det_matches_hypothesis = Detection_Props_Match_Hypothesis(detection, detection_prop, thresholds);

   /** \result
    * Confirm detection NOT matches hypothesis
    */
   CHECK_FALSE(f_det_matches_hypothesis)
}

/** \purpose
 * Verify that detection does not match hypothesis for Gen7 non-SRR sensor when RCS is above threshold.
 * \req NA.
 */
TEST(f360_detection_props_match_hypothesis_gen7, det_does_NOT_match_hypothesis_bad_RCS)
{
   /** \precond
   * sensor type is gen7 sensor (non-SRR)
   * detection rcs above threshold
   **/
   detection.raw.rcs = -9.9F;

   /** \action
    * Call Detection_Props_Match_Hypothesis
    */
   bool f_det_matches_hypothesis = Detection_Props_Match_Hypothesis(detection, detection_prop, thresholds);

   /** \result
    * Confirm detection NOT matches hypothesis
    */
   CHECK_FALSE(f_det_matches_hypothesis)
}

/** \purpose
 * Verify that detection does not match hypothesis for Gen7 non-SRR sensor when SNR is above threshold.
 * \req NA.
 */
TEST(f360_detection_props_match_hypothesis_gen7, det_does_NOT_match_hypothesis_bad_SNR)
{
   /** \precond
   * detection snr above threshold
   **/
   detection.raw.snr = 10.1F;

   /** \action
    * Call Detection_Props_Match_Hypothesis
    */
   bool f_det_matches_hypothesis = Detection_Props_Match_Hypothesis(detection, detection_prop, thresholds);

   /** \result
    * Confirm detection NOT matches hypothesis
    */
   CHECK_FALSE(f_det_matches_hypothesis)
}

/** \purpose
 * Verify that detection match hypothesis for Gen7 SRR sensor when RCS is above threshold.
 * \req NA.
 */
TEST(f360_detection_props_match_hypothesis_gen7, det_does_match_hypothesis_SRR_bad_RCS)
{
   /** \precond
   * sensor type is SRR
   * detection rcs above threshold
   **/
   thresholds.f_flr7_sensor = false;
   thresholds.f_srr7plus_sensor = true;

   detection.raw.rcs = -9.9F;

   /** \action
    * Call Detection_Props_Match_Hypothesis
    */
   bool f_det_matches_hypothesis = Detection_Props_Match_Hypothesis(detection, detection_prop, thresholds);

   /** \result
    * Confirm detection matches hypothesis
    */
   CHECK_TRUE(f_det_matches_hypothesis)
}

/** \purpose
 * Verify that detection does not match hypothesis for Gen7 SRR sensor when RCS and confid azimuth are above threshold
 * \req NA.
 */
TEST(f360_detection_props_match_hypothesis_gen7, det_does_NOT_match_hypothesis_SRR_bad_RCS_az_confid)
{
   /** \precond
   * sensor type is SRR
   * detection rcs above threshold
   * detection confid azimuth above threshold
   **/
   thresholds.f_flr7_sensor = false;
   thresholds.f_srr7plus_sensor = true;

   detection.raw.rcs = -9.9F;
   detection.raw.confid_azimuth = 2;

   /** \action
    * Call Detection_Props_Match_Hypothesis
    */
   bool f_det_matches_hypothesis = Detection_Props_Match_Hypothesis(detection, detection_prop, thresholds);

   /** \result
    * Confirm detection NOT matches hypothesis
    */
   CHECK_FALSE(f_det_matches_hypothesis)
}

/** \purpose
 * Verify that detection does not match hypothesis for Gen7 SRR sensor when RCS and SNR are above threshold
 * \req NA.
 */
TEST(f360_detection_props_match_hypothesis_gen7, det_does_NOT_match_hypothesis_SRR_bad_RCS_SNR)
{
   /** \precond
   * sensor type is SRR
   * detection rcs above threshold
   * detection snr above threshold
   **/
   thresholds.f_flr7_sensor = false;
   thresholds.f_srr7plus_sensor = true;

   detection.raw.rcs = -9.9F;
   detection.raw.snr = 10.1F;

   /** \action
    * Call Detection_Props_Match_Hypothesis
    */
   bool f_det_matches_hypothesis = Detection_Props_Match_Hypothesis(detection, detection_prop, thresholds);

   /** \result
    * Confirm detection NOT matches hypothesis
    */
   CHECK_FALSE(f_det_matches_hypothesis)
}

/** \purpose
 * Verify that detection does not match hypothesis for Gen7 sensor when detection is not moving.
 * \req NA.
 */
TEST(f360_detection_props_match_hypothesis_gen7, det_does_NOT_match_hypothesis_not_moving)
{
   /** \precond
   * detection motion status ambiguous
   **/
   detection_prop.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   /** \action
    * Call Detection_Props_Match_Hypothesis
    */
   bool f_det_matches_hypothesis = Detection_Props_Match_Hypothesis(detection, detection_prop, thresholds);

   /** \result
    * Confirm detection NOT matches hypothesis
    */
   CHECK_FALSE(f_det_matches_hypothesis)
}

/** @}*/
/** \defgroup  f360_update_max_severity_value
 *  @{
 */

/** \brief
 * Test Update_Max_Severity_Value() function from f360_check_for_low_power_clutter
 */
TEST_GROUP(f360_update_max_severity_value)
{
   uint32_t num_matching_dets[MAX_NUMBER_OF_SENSORS];
   uint32_t num_matching_inner_dets[MAX_NUMBER_OF_SENSORS];
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS]{};
   const uint32_t k_severity_level_low = 4U;
   const uint32_t k_severity_level_mid = 6U;
   const uint32_t k_severity_level_high = 8U;
   float32_t ref = 0.0F;
   const float32_t test_threshold = 0.0001F;
};

/** \purpose
 * Verify that severity value is not updated when sensors are not valid
 * \req NA.
 */
TEST(f360_update_max_severity_value, value_not_updated_sensor_not_valid)
{
   /** \precond
   * all sensors not valid
   **/
   for (uint32_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      sensors[i].variable.is_valid = false;
   }

   /** \action
    * Call Update_Max_Severity_Value
    */
   const float32_t max_severity = Update_Max_Severity_Value(num_matching_dets, num_matching_inner_dets, sensors, sensor_props);

   /** \result
    * Confirm detection matches hypothesis
    */
   CHECK_EQUAL(0.0F, max_severity)
}

/** \purpose
 * Verify that proper ref value is used to calculate max_severtiy when severity_level is high and low_power_clutter_severity value is above 1.0F. 
 * Expected behaviour is that max_severity will decrease.
 * \req NA.
 */
TEST(f360_update_max_severity_value, value_updated_sensor_severity_high_alpha_high)
{
   /** \precond
   * all sensors not valid
   **/
   sensors[0].variable.is_valid = true;
   sensor_props[0].low_power_clutter_severity = 1.0F + 0.01F;
   num_matching_dets[0]  = k_severity_level_high;
   num_matching_inner_dets[0] = 2U;

   /** \action
    * Call Update_Max_Severity_Value
    */
   const float32_t max_severity = Update_Max_Severity_Value(num_matching_dets, num_matching_inner_dets, sensors, sensor_props);

   /** \result
    * Confirm detection matches hypothesis
    */
   DOUBLES_EQUAL(1.00995F, max_severity, 0.0001F)
}

/** \purpose
 * Verify that proper ref value is used to calculate max_severtiy when severity_level is high and low_power_clutter_severity value is above 1.0F. 
 * There are no matching inner detections
 * Expected behaviour is that max_severity will decrease.
 * \req NA.
 */
TEST(f360_update_max_severity_value, value_updated_sensor_severity_high_alpha_high_no_matching_inner_dets)
{
   /** \precond
   * one sensor is valid
   * power clutter severity above 1.0F
   * matching dets are set to severity level high
   * no matching inner dets
   **/
   sensors[0].variable.is_valid = true;
   sensor_props[0].low_power_clutter_severity = 1.0F + 0.01F;
   num_matching_dets[0]  = k_severity_level_high;
   num_matching_inner_dets[0] = 0U;

   /** \action
    * Call Update_Max_Severity_Value
    */
   const float32_t max_severity = Update_Max_Severity_Value(num_matching_dets, num_matching_inner_dets, sensors, sensor_props);

   /** \result
    * Confirm detection matches hypothesis
    */
   DOUBLES_EQUAL(1.0049F, max_severity, test_threshold)
}

/** \purpose
 * Verify that proper ref value is used to calculate max_severity when severity_level is medium and low_power_clutter_severity value is above 1.0F.
 * We expect that max_severity will decrease to calculated value.
 * \req NA.
 */
TEST(f360_update_max_severity_value, value_updated_sensor_severity_mid_alpha_high)
{
   /** \precond
   * all sensors not valid
   **/
   sensors[0].variable.is_valid = true;
   sensor_props[0].low_power_clutter_severity = 1.0F + 0.01F;
   num_matching_dets[0]  = k_severity_level_mid;
   num_matching_inner_dets[0] = 1U;

   /** \action
    * Call Update_Max_Severity_Value
    */
   const float32_t max_severity = Update_Max_Severity_Value(num_matching_dets, num_matching_inner_dets, sensors, sensor_props);

   /** \result
    * Confirm detection matches hypothesis
    */
   DOUBLES_EQUAL(1.0089F, max_severity, test_threshold)
}

/** \purpose
 * Verify that proper ref value is used to calculate max_severity when severity_level is low and low_power_clutter_severity value is above 1.0F.
 * We expect that max_severity will decrease.
 * \req NA.
 */
TEST(f360_update_max_severity_value, value_updated_sensor_severity_low_alpha_high)
{
   /** \precond
   * all sensors not valid
   **/
   sensors[0].variable.is_valid = true;
   sensor_props[0].low_power_clutter_severity = 1.0F + 0.01F;
   num_matching_dets[0]  = k_severity_level_low;
   num_matching_inner_dets[0] = 1U;

   /** \action
    * Call Update_Max_Severity_Value
    */
   const float32_t max_severity = Update_Max_Severity_Value(num_matching_dets, num_matching_inner_dets, sensors, sensor_props);

   /** \result
    * Confirm detection matches hypothesis
    */
   DOUBLES_EQUAL(1.0079F, max_severity, test_threshold)
}

/** \purpose
 * Verify that proper ref value is used to calculate max_severity when severity_level is below thresholds and low_power_clutter_severity value is above 1.0F.
 * We expect that max_severity will decrease.
 * \req NA.
 */
TEST(f360_update_max_severity_value, value_updated_sensor_severity_lowest_alpha_high)
{
   /** \precond
   * all sensors not valid
   **/
   sensors[0].variable.is_valid = true;
   sensor_props[0].low_power_clutter_severity = 1.0F + 0.01F;
   num_matching_dets[0]  = k_severity_level_low - 1U;
   num_matching_inner_dets[0] = 1U;

   /** \action
    * Call Update_Max_Severity_Value
    */
   const float32_t max_severity = Update_Max_Severity_Value(num_matching_dets, num_matching_inner_dets, sensors, sensor_props);

   /** \result
    * Confirm detection matches hypothesis
    */
   DOUBLES_EQUAL(1.0049F, max_severity, test_threshold)
}

/** \purpose
 * Verify that proper ref value is used to calculate max_severity when severity_level is high and low_power_clutter_severity value is below 1.0F.
 * We expect that max_severity will decrease.
 * \req NA.
 */
TEST(f360_update_max_severity_value, value_updated_sensor_severity_high_alpha_low)
{
   /** \precond
   * all sensors not valid
   **/
   sensors[0].variable.is_valid = true;
   sensor_props[0].low_power_clutter_severity = 1.0F - 0.01F;
   num_matching_dets[0]  = k_severity_level_high;
   num_matching_inner_dets[0] = 2U;

   /** \action
    * Call Update_Max_Severity_Value
    */
   const float32_t max_severity = Update_Max_Severity_Value(num_matching_dets, num_matching_inner_dets, sensors, sensor_props);

   /** \result
    * Confirm detection matches hypothesis
    */
   DOUBLES_EQUAL(0.9905F, max_severity, test_threshold)
}

/** \purpose
 * Verify that proper ref value is used to calculate max_severity for different FLR7 sensors when severity_level is high and low_power_clutter_severity value is below 1.0F.
 * We expect that max_severity will increase.
 * \req NA.
 */
TEST(f360_update_max_severity_value, value_updated_sensor_severity_high_FLR7)
{
   /** \precond
   * Set one sensor valid with FLR7 type
   **/
  
   sensors[0].variable.is_valid = true;
   num_matching_dets[0] = k_severity_level_high + 4U;
   num_matching_inner_dets[0] = 4U;


   uint32_t num_valid_types = 3U;
   F360_Sensor_Type_T sensor_types[MAX_NUMBER_OF_SENSORS] = {
         F360_SENSOR_TYPE_FLR7_RADAR,
         F360_SENSOR_TYPE_FLR7_PLT_RADAR,
         F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR};
         
   for (uint32_t i = 0U; i < num_valid_types; i++)
   {
      sensors[0].constant.sensor_type = sensor_types[i];
      sensor_props[0].low_power_clutter_severity = 1.0F - 0.01F;

      /** \action
       * Call Update_Max_Severity_Value
       */
      const float32_t max_severity = Update_Max_Severity_Value(num_matching_dets, num_matching_inner_dets, sensors, sensor_props);

      /** \result
       * Confirm detection matches hypothesis
       */
      DOUBLES_EQUAL(0.9905F, max_severity, test_threshold)
   }
}

/** @}*/
/** \defgroup  f360_check_radar_rain_flag
 *  @{
 */

/** \brief
 * Test Check_Radar_Rain_Flag() function from f360_check_radar_rain_flag
 */
TEST_GROUP(f360_check_radar_rain_flag)
{
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS]{};
   F360_Tracker_Info_T tracker_info{};

   /** \setup
    * Make sure all the sensors are not valid
    * tracker_info.rain_level_filtered set to 0.5F
    */
   TEST_SETUP()
   {
      for (uint32_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         sensors[i].variable.is_valid = false;
         sensor_props[i].rain_level_filtered = 1.0F;
      }
   }
};

/** \purpose
 * Verify that functions works correctly if there are 0 valid sensors.
 * \req NA.
 */
TEST(f360_check_radar_rain_flag, rain_level_not_updated_sensors_not_valid)
{
   /** \precond
   * all sensors not valid
   **/

   /** \action
    * Call Check_Radar_Rain_Flag
    */
   Check_Radar_Rain_Flag(sensors, sensor_props, tracker_info);

   /** \result
    * Confirm rain level is updated correctly
    */
   DOUBLES_EQUAL(0.0F, tracker_info.rain_level_filtered, 0.0001F)
}

/** \purpose
 * Verify that rain_level_filtered is calculated correctly when there are 2 valid sensors with rain level severity 1 and 2.
 * \req NA.
 */
TEST(f360_check_radar_rain_flag, rain_level_updated_two_valid_sensors)
{
   /** \precond
   * 2 sensors valid
   * first sensor rain level 1
   * second sensor rain level 2
   **/
  sensors[0].variable.is_valid = true;
  sensors[0].variable.overall_rain_level = 1U;
  sensors[1].variable.is_valid = true;
  sensors[1].variable.overall_rain_level = 2U;

   /** \action
    * Call Check_Radar_Rain_Flag
    */
   Check_Radar_Rain_Flag(sensors, sensor_props, tracker_info);

   /** \result
    * Confirm rain level is updated correctly
    */
   DOUBLES_EQUAL(0.5125F, tracker_info.rain_level_filtered, 0.0001F)
}

/** @}*/
/** \defgroup  f360_set_sensor_dependent_thresholds
 *  @{
 */

/** \brief
 * Test Set_Sensor_Dependent_Thresholds() function from f360_set_sensor_dependent_thresholds
 */
TEST_GROUP(f360_set_sensor_dependent_thresholds)
{
   low_power_clutter_thresholds thresholds{};
};

/** \purpose
 * Verify that thresholds are updated correctly for gen6 sensors
 * \req NA.
 */
TEST(f360_set_sensor_dependent_thresholds, sensor_unknow)
{
   /** \precond
   * all sensors not valid
   **/
   uint8_t sensor_type = F360_SENSOR_TYPE_UNKNOWN ;
   /** \action
    * Call Set_Sensor_Dependent_Thresholds
    */
   Set_Sensor_Dependent_Thresholds(sensor_type, thresholds);

   /** \result
    * Confirm thresholds are updated correctly
    */
   CHECK_FALSE(thresholds.f_gen6_sensor);
   CHECK_FALSE(thresholds.f_flr7_sensor);
   CHECK_FALSE(thresholds.f_srr7plus_sensor);

}

/** \purpose
 * Verify that thresholds are updated correctly for gen6 sensors
 * \req NA.
 */
TEST(f360_set_sensor_dependent_thresholds, gen6_sensor_type)
{
   /** \precond
   * all sensors not valid
   **/
  uint32_t num_valid_types = 7U;
  uint8_t sensor_types[MAX_NUMBER_OF_SENSORS] = {F360_SENSOR_TYPE_SRR6_PLUS_RADAR,
   F360_SENSOR_TYPE_SRR6_PLUS_PLT_RADAR,
   F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR,
   F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XETH_RADAR,
   F360_SENSOR_TYPE_SRR6_PLUS_PLT_1GB_EHT_RADAR,
   F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR,
   F360_SENSOR_TYPE_FLR4_PLT_RADAR};
   for (uint32_t i = 0U; i < num_valid_types; i++)
   {
      /** \action
       * Call Set_Sensor_Dependent_Thresholds
       */
      Set_Sensor_Dependent_Thresholds(sensor_types[i], thresholds);

      /** \result
       * Confirm thresholds are updated correctly
       */
      CHECK_TRUE(thresholds.f_gen6_sensor);
      CHECK_FALSE(thresholds.f_flr7_sensor);
      CHECK_FALSE(thresholds.f_srr7plus_sensor);
      DOUBLES_EQUAL(-20.0F, thresholds.max_rcs, 0.0001F);
      DOUBLES_EQUAL(13.0F, thresholds.max_snr, 0.0001F);
   }
}

/** \purpose
 * Verify that thresholds are updated correctly for gen7 sensors
 * \req NA.
 */
TEST(f360_set_sensor_dependent_thresholds, gen7_sensor_type)
{
   /** \precond
   * all sensors not valid
   **/
   uint32_t num_valid_types = 6U;
   uint8_t sensor_types[MAX_NUMBER_OF_SENSORS] = {F360_SENSOR_TYPE_SRR7_PLUS_RADAR,
         F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR,
         F360_SENSOR_TYPE_SRR7_PLUS_V2_PLT_RADAR,
         F360_SENSOR_TYPE_FLR7_RADAR,
         F360_SENSOR_TYPE_FLR7_PLT_RADAR,
         F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR};
   for (uint32_t i = 0U; i < num_valid_types; i++)
   {
      /** \action
      * Call Set_Sensor_Dependent_Thresholds
      */
      Set_Sensor_Dependent_Thresholds(sensor_types[i], thresholds);

      if(i > 2U)
      {
         CHECK_TRUE(thresholds.f_flr7_sensor);
         CHECK_FALSE(thresholds.f_srr7plus_sensor);
      }
      else
      {
         CHECK_TRUE(thresholds.f_srr7plus_sensor);
         CHECK_FALSE(thresholds.f_flr7_sensor);

      }
      /** \result
       * Confirm thresholds are updated correctly
       */
      CHECK_FALSE(thresholds.f_gen6_sensor);
      DOUBLES_EQUAL(-10.0F, thresholds.max_rcs, 0.0001F);
      DOUBLES_EQUAL(10.0F, thresholds.max_snr, 0.0001F);
   }
}

static void Add_Detection(
   const int16_t idx,
   const float32_t vcs_pos_x,
   const float32_t vcs_pos_y,
   const int32_t sensor_id,
   const int8_t confid_az,
   rspp_variant_A::RSPP_Detection_List_T (&raw_detection_list),
   F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   )
{
   raw_detection_list.number_of_valid_detections += 1U;
   raw_detection_list.detections[idx].processed.vcs_position_x = vcs_pos_x;
   raw_detection_list.detections[idx].processed.vcs_position_y = vcs_pos_y;
   raw_detection_list.detections[idx].raw.sensor_id = sensor_id;
   raw_detection_list.detections[idx].raw.confid_azimuth = confid_az;

   detection_props[idx].vcs_position.x = raw_detection_list.detections[idx].processed.vcs_position_x;
   detection_props[idx].vcs_position.y = raw_detection_list.detections[idx].processed.vcs_position_y;
}

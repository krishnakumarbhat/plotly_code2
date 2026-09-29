/** \file
 * This file contains unit tests for content of f360_object_based_water_spray_detector_internals.cpp file
 */

#include "f360_object_based_water_spray_detector_internals.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_object_based_water_spray_detector_object_validity
 *  @{
 */

 /** \brief
  * Test group contains variables and test cases related to if an object should be searched for water spray detections or not.
  * Initialize variables so the object is valid then tweak these for each test case to reach full coverage.
  */
TEST_GROUP(f360_object_based_water_spray_detector_object_validity)
{

   F360_Object_Track_T obj = {};
   F360_Calibrations_T calib = {};

   /** \setup
    * Setup an object so that it is a valid object to check for water spray detections
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      obj.speed = calib.k_ws_min_speed + 1.0F;
      obj.vcs_position.x = 0.0F;
      obj.vcs_position.y = 0.0F;
   }
};

/** \purpose
 * Verify that object is valid when it's within VCS zone and have speed over threshold
 * \req
 * NA
 */
TEST(f360_object_based_water_spray_detector_object_validity, Is_Object_Valid_For_Water_Spray_Valid_Object)
{

   /** \action
    * Call function
    */
   bool f_valid_obj = Is_Object_Valid_For_Water_Spray(obj, calib);

   /** \result
    * Expect the object to be valid for water spray check
    */
   CHECK_TRUE_TEXT(f_valid_obj, "Object not valid for water spray check as expected");
}

/** \purpose
* Verify that object is not valid when it's within VCS zone but have speed below threshold
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_object_validity, Is_Object_Valid_For_Water_Spray_Speed_Too_Low)
{
   /** \precond
   * Set object speed below threshold
   */
   obj.speed = calib.k_ws_min_speed - 1.0F;

   /** \action
   * Call function
   */
   bool f_valid_obj = Is_Object_Valid_For_Water_Spray(obj, calib);

   /** \result
   * Expect the object to not be valid for water spray check
   */
   CHECK_FALSE_TEXT(f_valid_obj, "Object inaccurately valid for water spray check");
}

/** \purpose
* Verify that object is not valid when speed is greater than threshold but object is outside VCS zone
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_object_validity, Is_Object_Valid_For_Water_Spray_Outside_Zone)
{
   /** \precond
   * Set object position outside zone
   */
   obj.vcs_position.x = calib.k_ows_max_long_pos + 1.0F;

   /** \action
   * Call function
   */
   bool f_valid_obj = Is_Object_Valid_For_Water_Spray(obj, calib);

   /** \result
   * Expect the object to not be valid for water spray check
   */
   CHECK_FALSE_TEXT(f_valid_obj, "Object inaccurately valid for water spray check");
}

/** \purpose
* Verify that object is not valid when both speed is below threshold and object is outside VCS zone
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_object_validity, Is_Object_Valid_For_Water_Spray_Speed_Too_Low_Outside_Zone)
{
   /** \precond
   * Set object speed below threshold
   * Set object position outside zone
   */
   obj.speed = calib.k_ws_min_speed - 1.0F;
   obj.vcs_position.y = calib.k_ows_max_lat_pos + 1.0F;

   /** \action
   * Call function
   */
   bool f_valid_obj = Is_Object_Valid_For_Water_Spray(obj, calib);

   /** \result
   * Expect the object to not be valid for water spray check
   */
   CHECK_FALSE_TEXT(f_valid_obj, "Object inaccurately valid for water spray check");
}

/** @}*/


/** \defgroup  f360_object_based_water_spray_detector_detection_validity
*  @{
*/

/** \brief
* Test group contains variables and test cases related to if a detection can be a water spray detection or not based on it's properties
* Initialize variables so the detection is valid and then tweak these for each test case to reach full coverage.
*/
TEST_GROUP(f360_object_based_water_spray_detector_detection_validity)
{

   rspp_variant_A::RSPP_Detection_T det = {};
   F360_Detection_Props_T det_p = {};
   F360_Calibrations_T calib = {};
   float32_t ref_obj_rcs = 0.0F;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};


   /** \setup
   * Setup a detection so that it is a valid detection that can be a water spray detection
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      det_p.f_ok_to_use = true;
      det.raw.sensor_id = 1;
      det.raw.confid_azimuth = 1;
      det.raw.rcs = calib.k_ws_max_det_rcs - 1.0F;
   }
};

/** \purpose
* Verify that detection is valid for potential water spray when all conditions are fulfilled
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid)
{

   /** \action
   * Call function
   */
   bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

   /** \result
   * Expect the detection to be valid for water spray
   */
   CHECK_TRUE_TEXT(f_valid_det, "Detection not valid for water spray as expected");
}

/** \purpose
* Verify that detection is not valid for potential water spray due to it is flagged as not ok to use
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_Det_Not_Ok_To_Use)
{
   /** \precond
   * Set detection as not ok to use
   */
   det_p.f_ok_to_use = false;

   /** \action
   * Call function
   */
   bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

   /** \result
   * Expect the detection to not be valid for water spray check
   */
   CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is not valid for potential water spray due to its azimuth confidence is high
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_Best_Az_Conf)
{
   /** \precond
   * Set detection azimuth confidence to best level
   */
   det.raw.confid_azimuth = 0;

   /** \action
   * Call function
   */
   bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

   /** \result
   * Expect the detection to not be valid for water spray check
   */
   CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is not valid for potential water spray due to its rcs is too high
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_Too_High)
{
   /** \precond
   * Set detection rcs greater than threshold
   */
   det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;

   /** \action
   * Call function
   */
   bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

   /** \result
   * Expect the detection to not be valid for water spray check
   */
   CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is valid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_TYPE_SRR6_PLUS_RADAR and object's average RCS is high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid_rcs_High_F360_SENSOR_TYPE_SRR6_PLUS_RADAR)
{
   /** \precond
   * Set detection rcs greater than normal threshold
   * Detection is changed to come from an F360_SENSOR_TYPE_SRR6_PLUS_RADAR radar
   * Object's average RCS is high
   */
   det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
   det.raw.sensor_id = 1;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR6_PLUS_RADAR;
   ref_obj_rcs = 5.0F;

   /** \action
   * Call function
   */
   bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

   /** \result
   * Expect the detection to not be valid for water spray check
   */
   CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is valid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_TYPE_SRR6_PLUS_PLT_RADAR and object's average RCS is high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid_rcs_High_F360_SENSOR_TYPE_SRR6_PLUS_PLT_RADAR)
{
   /** \precond
   * Set detection rcs greater than normal threshold
   * Detection is changed to come from an F360_SENSOR_TYPE_SRR6_PLUS_PLT_RADAR
   * Object's average RCS is high
   */
   det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
   det.raw.sensor_id = 1;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR6_PLUS_PLT_RADAR;
   ref_obj_rcs = 5.0F;

   /** \action
   * Call function
   */
   bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

   /** \result
   * Expect the detection to not be valid for water spray check
   */
   CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is valid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR and object's average RCS is high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid_rcs_High_F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR)
{
   /** \precond
   * Set detection rcs greater than normal threshold
   * Detection is changed to come from an F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR
   * Object's average RCS is high
   */
   det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
   det.raw.sensor_id = 1;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR;
   ref_obj_rcs = 5.0F;

   /** \action
   * Call function
   */
   bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

   /** \result
   * Expect the detection to not be valid for water spray check
   */
   CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is valid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XETH_RADAR and object's average RCS is high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid_rcs_High_F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XETH_RADAR)
{
   /** \precond
   * Set detection rcs greater than normal threshold
   * Detection is changed to come from an F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XETH_RADAR
   * Object's average RCS is high
   */
   det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
   det.raw.sensor_id = 1;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XETH_RADAR;
   ref_obj_rcs = 5.0F;

   /** \action
   * Call function
   */
   bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

   /** \result
   * Expect the detection to not be valid for water spray check
   */
   CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is valid for potential water spray due to its rcs is high but the sensor type is SRR6_PLUS_PLT_1GB_EHT_RADAR and object's average RCS is high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid_rcs_High_SRR6_PLUS_PLT_1GB_EHT_RADAR)
{
   /** \precond
   * Set detection rcs greater than normal threshold
   * Detection is changed to come from an SRR6_PLUS_PLT_1GB_EHT_RADAR
   * Object's average RCS is high
   */
   det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
   det.raw.sensor_id = 1;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR6_PLUS_PLT_1GB_EHT_RADAR;
   ref_obj_rcs = 5.0F;

   /** \action
   * Call function
   */
   bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

   /** \result
   * Expect the detection to not be valid for water spray check
   */
   CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is valid for potential water spray due to its rcs is high but the sensor type is FLR4 and object's average RCS is high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid_rcs_High_RCS_FLR4)
{
   /** \precond
   * Set detection rcs greater than normal threshold
   * Detection is changed to come from an F360_SENSOR_TYPE_FLR4_PLT_RADAR
   * Object's average RCS is high
   */
   det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
   det.raw.sensor_id = 1;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_PLT_RADAR;
   ref_obj_rcs = 5.0F;

   /** \action
   * Call function
   */
   bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

   /** \result
   * Expect the detection to not be valid for water spray check
   */
   CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is FLR4 but object's average RCS is not high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_Low_RCS_FLR4)
{
   /** \precond
   * Set detection rcs greater than normal threshold
   * Detection is changed to come from an FLR4 radar
   * Object's average RCS is low
   */
   det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
   det.raw.sensor_id = 1;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_PLT_RADAR;
   ref_obj_rcs = -15.0F;

   /** \action
   * Call function
   */
   bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

   /** \result
   * Expect the detection to not be valid for water spray check
   */
   CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is FLR4 and object's average RCS is high enough
* and the detection is flagged as not ok to use.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_High_RCS_FLR4_NOK_To_Use)
{
   /** \precond
   * Set detection rcs greater than normal threshold
   * Set detection as not ok to use
   * Detection is changed to come from an FLR4 radar
   * Object's average RCS is high
   */
   det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
   det.raw.sensor_id = 1;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_PLT_RADAR;
   ref_obj_rcs = 5.0F;
   det_p.f_ok_to_use = false;

   /** \action
   * Call function
   */
   bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

   /** \result
   * Expect the detection to not be valid for water spray check
   */
   CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is valid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR and object's average RCS is high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid_rcs_High_F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Detection is changed to come from a F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR;
    ref_obj_rcs = 5.0F;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR but object's average RCS is not high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_Low_RCS_F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Detection is changed to come from an F360_SENSOR_FLR4_PLT_STANDALONE_RADAR radar
    * Object's average RCS is low
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR;
    ref_obj_rcs = -15.0F;

    /** \action
       * Call function
       */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR and object's average RCS is high enough
* and the detection is flagged as not ok to use.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_High_RCS_F360_SENSOR_FLR4_PLT_STANDALONE_RADAR_NOK_To_Use)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Set detection as not ok to use
    * Detection is changed to come from an F360_SENSOR_FLR4_PLT_STANDALONE_RADAR radar
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR;
    ref_obj_rcs = 5.0F;
    det_p.f_ok_to_use = false;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is valid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_FLR4_RADAR and object's average RCS is high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid_rcs_High_RCS_F360_SENSOR_TYPE_FLR4_RADAR)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Detection is changed to come from an F360_SENSOR_TYPE_FLR4_RADAR
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_RADAR;
    ref_obj_rcs = 5.0F;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_FLR4_RADAR but object's average RCS is not high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_Low_RCS_F360_SENSOR_TYPE_FLR4_RADAR)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Detection is changed to come from an F360_SENSOR_TYPE_FLR4_RADAR radar
    * Object's average RCS is low
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_RADAR;
    ref_obj_rcs = -15.0F;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_FLR4_RADAR and object's average RCS is high enough
* and the detection is flagged as not ok to use.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_High_RCS_F360_SENSOR_TYPE_FLR4_RADAR_NOK_To_Use)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Set detection as not ok to use
    * Detection is changed to come from an FLR4 radar
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_RADAR;
    ref_obj_rcs = 5.0F;
    det_p.f_ok_to_use = false;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is valid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_SRR7_PLUS_PLT_RADAR and object's average RCS is high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid_rcs_High_F360_SENSOR_SRR7_PLUS_PLT_RADAR)
{
    /** \precond
    * Set detection rcs high for a waterspray detection
    * Detection is changed to come from an F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR;
    ref_obj_rcs = 5.0F;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to be valid for water spray check
    */
    CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_SRR7_PLUS_PLT_RADAR but object's average RCS is not high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_Low_F360_SENSOR_SRR7_PLUS_PLT_RADAR)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Detection is changed to come from an F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR radar
    * Object's average RCS is low
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR;
    ref_obj_rcs = -20.0F;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_SRR7_PLUS_PLT_RADAR and object's average RCS is high enough
* and the detection is flagged as not ok to use.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_High_F360_SENSOR_SRR7_PLUS_PLT_RADAR_NOK_To_Use)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Set detection as not ok to use
    * Detection is changed to come from an F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR radar
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR;
    ref_obj_rcs = 5.0F;
    det_p.f_ok_to_use = false;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is valid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_SRR7_PLUS_RADAR and object's average RCS is high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid_rcs_High_F360_SENSOR_SRR7_PLUS_RADAR)
{
    /** \precond
    * Set detection rcs high for a waterspray detection
    * Detection is changed to come from an F360_SENSOR_TYPE_SRR7_PLUS_RADAR
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
    ref_obj_rcs = 5.0F;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to be valid for water spray check
    */
    CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_SRR7_PLUS_RADAR but object's average RCS is not high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_Low_F360_SENSOR_SRR7_PLUS_RADAR)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Detection is changed to come from an F360_SENSOR_SRR7_PLUS_RADAR radar
    * Object's average RCS is low
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
    ref_obj_rcs = -20.0F;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_SRR7_PLUS_RADAR and object's average RCS is high enough
* and the detection is flagged as not ok to use.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_High_360_SENSOR_SRR7_PLUS_RADAR_NOK_To_Use)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Set detection as not ok to use
    * Detection is changed to come from an F360_SENSOR_TYPE_SRR7_PLUS_RADAR radar
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
    ref_obj_rcs = 5.0F;
    det_p.f_ok_to_use = false;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is valid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_FLR7_PLT_RADAR and object's average RCS is high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid_rcs_High_F360_SENSOR_FLR7_PLT_RADAR)
{
    /** \precond
    * Set detection rcs high for a waterspray detection
    * Detection is changed to come from an F360_SENSOR_TYPE_FLR7_PLT_RADAR
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_PLT_RADAR;
    ref_obj_rcs = 5.0F;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_FLR7_PLT_RADAR but object's average RCS is not high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_Low_F360_SENSOR_FLR7_PLT_RADAR)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Detection is changed to come from an F360_SENSOR_TYPE_FLR7_PLT_RADAR radar
    * Object's average RCS is low
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_PLT_RADAR;
    ref_obj_rcs = -20.0F;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_FLR7_PLT_RADAR and object's average RCS is high enough
* and the detection is flagged as not ok to use.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_High_F360_SENSOR_FLR7_PLT_RADAR_NOK_To_Use)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Set detection as not ok to use
    * Detection is changed to come from an F360_SENSOR_TYPE_FLR7_PLT_RADAR radar
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_PLT_RADAR;
    ref_obj_rcs = 5.0F;
    det_p.f_ok_to_use = false;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is valid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_TYPE_FLR7_RADAR and object's average RCS is high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid_rcs_High_F360_SENSOR_FLR7_RADAR)
{
    /** \precond
    * Set detection rcs high for a waterspray detection
    * Detection is changed to come from an F360_SENSOR_TYPE_FLR7_RADAR
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_RADAR;
    ref_obj_rcs = 5.0F;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to be valid for water spray check
    */
    CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_FLR7_RADAR but object's average RCS is not high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_Low_F360_SENSOR_TYPE_FLR7_RADAR)
    {
      /** \precond
       * Set detection rcs greater than normal threshold
       * Detection is changed to come from an F360_SENSOR_TYPE_FLR7_RADAR radar
       * Object's average RCS is low
       */
      det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
      det.raw.sensor_id = 1;
      sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_RADAR;
      ref_obj_rcs = -20.0F;

      /** \action
       * Call function
       */
      bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

      /** \result
       * Expect the detection to not be valid for water spray check
       */
      CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection coming from sensor FLR7V2 is valid for potential water spray 
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_F360_SENSOR_TYPE_FLR7V2)
    {
      /** \precond
       * Set detection rcs greater than normal threshold
       * Detection is changed to come from an F360_SENSOR_TYPE_FLR7V2 radar
       */
      det.raw.rcs = calib.k_ws_max_det_rcs - 1.0F;
      det.raw.sensor_id = 1;
      sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR;

      /** \action
       * Call function
       */
      bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

      /** \result
       * Expect the detection to be valid for water spray check
       */
      CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection coming from sensor SRR7V2 is valid for potential water spray 
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_F360_SENSOR_TYPE_SRR7V2)
    {
      /** \precond
       * Set detection rcs greater than normal threshold
       * Detection is changed to come from an F360_SENSOR_TYPE_SRR7V2 radar
       */
      det.raw.rcs = calib.k_ws_max_det_rcs - 1.0F;
      det.raw.sensor_id = 1;
      sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_V2_PLT_RADAR ;

      /** \action
       * Call function
       */
      bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

      /** \result
       * Expect the detection to be valid for water spray check
       */
      CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_FLR7_RADAR and object's average RCS is high enough
* and the detection is flagged as not ok to use.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_High_F360_SENSOR_FLR7_RADAR_NOK_To_Use)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Set detection as not ok to use
    * Detection is changed to come from an F360_SENSOR_TYPE_FLR7_RADAR radar
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_RADAR;
    ref_obj_rcs = 5.0F;
    det_p.f_ok_to_use = false;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is valid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR and object's average RCS is high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid_rcs_High_F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR)
{
    /** \precond
    * Set detection rcs high for a waterspray detection
    * Detection is changed to come from an F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR;
    ref_obj_rcs = 5.0F;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to be valid for water spray check
    */
    CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR but object's average RCS is not high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_Low_F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Detection is changed to come from an F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR radar
    * Object's average RCS is low
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR;
    ref_obj_rcs = -20.0F;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR and object's average RCS is high enough
* and the detection is flagged as not ok to use.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_High_F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR_NOK_To_Use)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Set detection as not ok to use
    * Detection is changed to come from an F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR radar
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR;
    ref_obj_rcs = 5.0F;
    det_p.f_ok_to_use = false;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is valid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_TYPE_FLR4_PLUS_RADAR and object's average RCS is high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Valid_rcs_High_F360_SENSOR_TYPE_FLR4_PLUS_RADAR)
{
    /** \precond
    * Set detection rcs high for a waterspray detection
    * Detection is changed to come from an F360_SENSOR_TYPE_FLR4_PLUS_RADAR
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_PLUS_RADAR;
    ref_obj_rcs = 5.0F;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to be valid for water spray check
    */
    CHECK_TRUE_TEXT(f_valid_det, "Detection inaccurately invalid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_TYPE_FLR4_PLUS_RADAR but object's average RCS is not high enough.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_Low_F360_SENSOR_TYPE_FLR4_PLUS_RADAR)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Detection is changed to come from an F360_SENSOR_TYPE_FLR4_PLUS_RADAR radar
    * Object's average RCS is low
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_PLUS_RADAR;
    ref_obj_rcs = -20.0F;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** \purpose
* Verify that detection is invalid for potential water spray due to its rcs is high but the sensor type is F360_SENSOR_TYPE_FLR4_PLUS_RADAR and object's average RCS is high enough
* and the detection is flagged as not ok to use.
* \req
* NA
*/
TEST(f360_object_based_water_spray_detector_detection_validity, Is_Det_Valid_For_Water_Spray_Det_Not_Valid_rcs_High_F360_SENSOR_TYPE_FLR4_PLUS_RADAR_NOK_To_Use)
{
    /** \precond
    * Set detection rcs greater than normal threshold
    * Set detection as not ok to use
    * Detection is changed to come from an F360_SENSOR_TYPE_FLR4_PLUS_RADAR radar
    * Object's average RCS is high
    */
    det.raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
    det.raw.sensor_id = 1;
    sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR4_PLUS_RADAR;
    ref_obj_rcs = 5.0F;
    det_p.f_ok_to_use = false;

    /** \action
    * Call function
    */
    bool f_valid_det = Is_Det_Valid_For_Water_Spray(det, det_p, calib, ref_obj_rcs, sensors);

    /** \result
    * Expect the detection to not be valid for water spray check
    */
    CHECK_FALSE_TEXT(f_valid_det, "Detection inaccurately valid for water spray check");
}

/** @}*/
/** \defgroup  f360_Does_RR_Fit_Water_Spray_Hypothesis__functionality
 *  @{
 */

 /** \brief
  * Test group for verifiing behavior of Does_Range_Rate_Fit_To_Velocity() function.
  */
TEST_GROUP(f360_Does_RR_Fit_Water_Spray_Hypothesis__functionality)
{
   const float tolerance = 0.00001F;
   F360_Calibrations_T calib{};
   const F360_VCS_Velocity_T velocity_otg{ 5.0F, 15.0F };
   const float32_t azimuth_sin = F360_Sinf(F360_DEG2RAD(26.565F));
   const float32_t azimuth_cos = F360_Cosf(F360_DEG2RAD(26.565F));
   const float32_t range_rate_otg = 11.18034F;

   /** \setup
    * Setup an object so that it is a valid object to check for water spray detections
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
 * Check if function returns true when provided range rate is just below upper limit
 * \req
 * NA
 */
TEST(f360_Does_RR_Fit_Water_Spray_Hypothesis__functionality, below_upper_limit)
{
   /** \precond
   * Same as setup plus set scaled range rate below upper threshold
   */   
   const float32_t scaled_range_rate_otg = range_rate_otg * calib.k_ows_range_rate_max_factor - tolerance;

   /** \action
    * Call function Does_Range_Rate_Fit_To_Velocity()
    */
   const bool f_fit = Does_RR_Fit_Water_Spray_Hypothesis(velocity_otg, azimuth_sin, azimuth_cos, scaled_range_rate_otg, calib.k_ows_range_rate_min_factor, calib.k_ows_range_rate_max_factor);

   /** \result
    * Expect true
    */
   CHECK_TRUE(f_fit);
}

/** \purpose
 * Check if function returns false when provided range rate is just above upper limit
 * \req
 * NA
 */
TEST(f360_Does_RR_Fit_Water_Spray_Hypothesis__functionality, above_upper_limit)
{
   /** \precond
   * Same as setup plus set scaled range rate above upper threshold
   */
   const float32_t scaled_range_rate_otg = range_rate_otg * calib.k_ows_range_rate_max_factor + tolerance;

   /** \action
    * Call function Does_Range_Rate_Fit_To_Velocity()
    */
   const bool f_fit = Does_RR_Fit_Water_Spray_Hypothesis(velocity_otg, azimuth_sin, azimuth_cos, scaled_range_rate_otg, calib.k_ows_range_rate_min_factor, calib.k_ows_range_rate_max_factor);

   /** \result
    * Expect false
    */
   CHECK_FALSE(f_fit);
}

/** \purpose
 * Check if function returns true when provided range rate is just above lower limit
 * \req
 * NA
 */
TEST(f360_Does_RR_Fit_Water_Spray_Hypothesis__functionality, above_lower_limit)
{
   /** \precond
   * Same as setup plus set scaled range rate above lower threshold
   */
   const float32_t scaled_range_rate_otg = range_rate_otg * calib.k_ows_range_rate_min_factor + tolerance;

   /** \action
    * Call function Does_Range_Rate_Fit_To_Velocity()
    */
   const bool f_fit = Does_RR_Fit_Water_Spray_Hypothesis(velocity_otg, azimuth_sin, azimuth_cos, scaled_range_rate_otg, calib.k_ows_range_rate_min_factor, calib.k_ows_range_rate_max_factor);

   /** \result
    * Expect true
    */
   CHECK_TRUE(f_fit);
}

/** \purpose
 * Check if function returns false when provided range rate is just below lower limit
 * \req
 * NA
 */
TEST(f360_Does_RR_Fit_Water_Spray_Hypothesis__functionality, below_lower_limit)
{
   /** \precond
   * Same as setup plus set scaled range rate below lower threshold
   */
   const float32_t scaled_range_rate_otg = range_rate_otg * calib.k_ows_range_rate_min_factor - tolerance;

   /** \action
    * Call function Does_Range_Rate_Fit_To_Velocity()
    */
   const bool f_fit = Does_RR_Fit_Water_Spray_Hypothesis(velocity_otg, azimuth_sin, azimuth_cos, scaled_range_rate_otg, calib.k_ows_range_rate_min_factor, calib.k_ows_range_rate_max_factor);

   /** \result
    * Expect false
    */
   CHECK_FALSE(f_fit);
}

/** \purpose
 * Check if function returns true when provided range rate is negative and within limits
 * \req
 * NA
 */
TEST(f360_Does_RR_Fit_Water_Spray_Hypothesis__functionality, negative_range_rate_within_limits)
{
   /** \precond
   * Same as setup plus set scaled range rate to negative value within limits
   */
   const float32_t scaled_range_rate_otg = -range_rate_otg * calib.k_ows_range_rate_min_factor - tolerance;
   const F360_VCS_Velocity_T neg_velocity_otg{ -velocity_otg.longitudinal, -velocity_otg.lateral };

   /** \action
    * Call function Does_Range_Rate_Fit_To_Velocity()
    */
   const bool f_fit = Does_RR_Fit_Water_Spray_Hypothesis(neg_velocity_otg, azimuth_sin, azimuth_cos, scaled_range_rate_otg, calib.k_ows_range_rate_min_factor, calib.k_ows_range_rate_max_factor);

   /** \result
    * Expect true
    */
   CHECK_TRUE(f_fit);
}

/** \purpose
 * Check if function returns false when provided range rate is negative and out of limits
 * \req
 * NA 
 */
TEST(f360_Does_RR_Fit_Water_Spray_Hypothesis__functionality, negative_range_rate_out_of_limits)
{
   /** \precond
   * Same as setup plus set scaled range rate to negative value out of limits
   */
   const float32_t scaled_range_rate_otg = -range_rate_otg * calib.k_ows_range_rate_min_factor + tolerance;
   const F360_VCS_Velocity_T neg_velocity_otg{ -velocity_otg.longitudinal, -velocity_otg.lateral };

   /** \action
    * Call function Does_Range_Rate_Fit_To_Velocity()
    */
   const bool f_fit = Does_RR_Fit_Water_Spray_Hypothesis(neg_velocity_otg, azimuth_sin, azimuth_cos, scaled_range_rate_otg, calib.k_ows_range_rate_min_factor, calib.k_ows_range_rate_max_factor);

   /** \result
    * Expect false
    */
   CHECK_FALSE(f_fit);
}
/** @}*/

/** \file
 * This file contains unit tests for the content of the f360_msmt_update_object_timestamp.cpp file
 */

#include "f360_msmt_update_object_timestamp.h"

#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_msmt_update_object_timestamp
 *  @{
 */

/** \brief
 * Test group for Msmt_Update_Object_Timestamp().
 * Tests in this test group verify that object time since measurement is updated correctly.
 **/
TEST_GROUP(f360_msmt_update_object_timestamp)
{
   /** \setup
    * Set up default test input data
    **/
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = { };
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Object_Track_T object_track = { };

   const float32_t elapsed_time = 0.05F;
   const float32_t test_pass_threshold = 1e-8F;

   TEST_SETUP()
   {
      // Initialize object track
      object_track.time_since_measurement = 0.01F;
      object_track.ndets = 2;
      object_track.detids[0] = 1U;
      object_track.detids[1] = 2U;

      // Initialize two detections
      det_props[0].f_rr_inlier = true;
      det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      raw_detection_list.detections[0].raw.sensor_id = 1;
      sensors[0].refined.time_since_measurement_s = 0.02F;

      det_props[1].f_rr_inlier = true;
      det_props[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      raw_detection_list.detections[1].raw.sensor_id = 2;
      sensors[1].refined.time_since_measurement_s = 0.04F;
   }
};

/** \purpose  This test checks that the function Msmt_Update_Object_Timestamp() updates the
 *   object time_since_measurement when valid detections are associated with object
 *\req NA
 */
TEST(f360_msmt_update_object_timestamp, Test_With_Dets)
{

   /** \precond
    * Use the default setup from the test group with one object and two valid detections.
    **/

   /** \action
    * Call the function Msmt_Update_Object_Timestamp()
    **/
   Msmt_Update_Object_Timestamp(elapsed_time, sensors, raw_detection_list, det_props, object_track);

   /** \result
    * Check that the function output matches the expected output. Expected output is that the
    * time_since_measurement of the object is the same as time_since_measurement of the newest detection.
    **/
   DOUBLES_EQUAL(sensors[0].refined.time_since_measurement_s, object_track.time_since_measurement, test_pass_threshold);
}

/** \purpose  This test checks that the function Msmt_Update_Object_Timestamp() updates the
 *   time_since_measurement of the object correctly when the object has no detections.
 *\req NA
 */
TEST(f360_msmt_update_object_timestamp, Test_No_Dets)
{

   /** \precond
    * Set the number of detections to zero for the object.
    **/
   object_track.ndets = 0;

   const float32_t exp_time_since_measurement = object_track.time_since_measurement + elapsed_time;

   /** \action
    * Call the function Msmt_Update_Object_Timestamp()
    **/
   Msmt_Update_Object_Timestamp(elapsed_time, sensors, raw_detection_list, det_props, object_track);

   /** \result
    * Check that the function output matches the expected output. Expected output is that the
    * elapsed time is added to time_since_measurement of the object.
    **/
   DOUBLES_EQUAL(exp_time_since_measurement, object_track.time_since_measurement, test_pass_threshold);
}

/** \purpose  This test checks that the function Msmt_Update_Object_Timestamp_Newest_Det() updates the
 *   time_since_measurement of the object correctly when the object has two valid detections.
 *\req NA
 */
TEST(f360_msmt_update_object_timestamp, Msmt_Update_Object_Timestamp_Newest_Det_Test_With_Dets)
{
   /** \precond
    * Use the default setup from the test group with one object and two valid detections.
    **/

   /** \action
    * Call the function Msmt_Update_Object_Timestamp_Newest_Det()
    **/
   Msmt_Update_Object_Timestamp_Newest_Det(elapsed_time, sensors, raw_detection_list, det_props, object_track);

   /** \result
    * Check that the function output matches the expected output. Expected output is that the
    * time_since_measurement of the object is the same as time_since_measurement of the newest detection.
    **/
   DOUBLES_EQUAL(sensors[0].refined.time_since_measurement_s, object_track.time_since_measurement, test_pass_threshold);
}

/** \purpose  This test checks that the function Msmt_Update_Object_Timestamp_Newest_Det() updates the
 *   time_since_measurement of the object correctly when the object has no valid detections.
 *\req NA
 */
TEST(f360_msmt_update_object_timestamp, Msmt_Update_Object_Timestamp_Newest_Det_Test_No_Valid_Dets)
{
   /** \precond
    * For the first detection set f_rr_inlier to false and for the second detection set
    * wheel_spin_type to F360_DETECTION_WHEELSPIN_TYPE_OBJECT to make both detections invalid.
    **/
   det_props[0].f_rr_inlier = false;
   det_props[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;

   const float32_t exp_time_since_measurement = object_track.time_since_measurement + elapsed_time;

   /** \action
    * Call the function Msmt_Update_Object_Timestamp()
    **/
   Msmt_Update_Object_Timestamp_Newest_Det(elapsed_time, sensors, raw_detection_list, det_props, object_track);

   /** \result
    * Check that the function output matches the expected output. Expected output is that the
    * elapsed time is added to time_since_measurement of the object.
    **/
   DOUBLES_EQUAL(exp_time_since_measurement, object_track.time_since_measurement, test_pass_threshold);
}

/** \purpose  This test checks that the function Find_Newest_Det_Timestamp() uses the
 * time_since_measurement from the newest detections when setting timestamp_newest.
 *\req NA
 */
TEST(f360_msmt_update_object_timestamp, Find_Newest_Det_Timestamp_With_Dets)
{
   /** \precond
    *  Use the default setup from the test group with two valid detections.
    **/
   float32_t timestamp_newest;
   bool f_valid_det_found;

   /** \action
    * Call the function Find_Newest_Det_Timestamp()
    **/
   Find_Newest_Det_Timestamp(object_track, sensors, raw_detection_list, det_props, timestamp_newest, f_valid_det_found);

   /** \result
    * Check that the function output matches the expected output. Expected output is that the
    * time_since_measurement of the newest detection is used for timestamp_newest and that
    * f_valid_det_found is true.
    **/
   DOUBLES_EQUAL(sensors[0].refined.time_since_measurement_s, timestamp_newest, test_pass_threshold);
   CHECK_TRUE(f_valid_det_found)
}

/** \purpose  This test checks that the function Find_Newest_Det_Timestamp() uses the one
 * valid detection for setting timestamp_newest.
 *\req NA
 */
TEST(f360_msmt_update_object_timestamp, Find_Newest_Det_Timestamp_Test_One_Valid_Det)
{
   /** \precond
    * For the first detection set f_rr_inlier to false.
    **/
   det_props[0].f_rr_inlier = false;

   float32_t timestamp_newest;
   bool f_valid_det_found;

   /** \action
    * Call the function Find_Newest_Det_Timestamp()
    **/
   Find_Newest_Det_Timestamp(object_track, sensors, raw_detection_list, det_props, timestamp_newest, f_valid_det_found);

   /** \result
    * Check that the function output matches the expected output. Expected output is that the
    * time_since_measurement from the second detection is used for timestamp_newest and that
    * f_valid_det_found is true.
    **/
   DOUBLES_EQUAL(sensors[1].refined.time_since_measurement_s, timestamp_newest, test_pass_threshold);
   CHECK_TRUE(f_valid_det_found)
}

/** \purpose  This test checks that the function Find_Newest_Det_Timestamp() handles the case
 * with no valid detections correctly.
 *\req NA
 */
TEST(f360_msmt_update_object_timestamp, Find_Newest_Det_Timestamp_Test_No_Valid_Dets)
{
   /** \precond
    * For the first detection set f_rr_inlier to false and for the second detection set
    * wheel_spin_type to F360_DETECTION_WHEELSPIN_TYPE_OBJECT to make both detections invalid.
    **/
   det_props[0].f_rr_inlier = false;
   det_props[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   float32_t timestamp_newest;
   bool f_valid_det_found;

   /** \action
    * Call the function Find_Newest_Det_Timestamp()
    **/
   Find_Newest_Det_Timestamp(object_track, sensors, raw_detection_list, det_props, timestamp_newest, f_valid_det_found);

   /** \result
    * Check that the function output matches the expected output. Expected output is that the
    * newest timestamp is equal to INFTY and that f_valid_det_found is false.
    **/
   const float32_t exp_timestamp_newest = INFTY;
   DOUBLES_EQUAL(exp_timestamp_newest, timestamp_newest, test_pass_threshold);
   CHECK_FALSE(f_valid_det_found)
}

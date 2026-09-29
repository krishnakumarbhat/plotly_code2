/** \file
 * This file contains unit tests for content of f360_object_based_angle_jump_detector.cpp file
 */

#include "f360_object_based_angle_jump_detector.h"
#include <CppUTest/TestHarness.h>

#include "f360_math.h"

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_Check_Dets_Against_Angle_Jumps
 *  @{
 */

/** \brief
* Test group for testing Check_Dets_Against_Radar_Phenomena(). Moved from angle jump detector.
*/
TEST_GROUP(f360_Check_Dets_Against_Angle_Jumps)
{
   const float tolerance = 0.00001F;
   F360_Object_Track_T relevant_object{};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   bool valid_sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Calibrations_T calibs{};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS]{};

   F360_Detection_Props_T &relevant_detection_prop = detection_props[0];
   rspp_variant_A::RSPP_Detection_T &relevant_detection_raw = raw_detection_list.detections[0];
   F360_Radar_Sensor_T &rear_right_sensor_cal = sensors[0];
   F360_Radar_Sensor_T &rear_right_sensor = sensors[0];
   Sensor_Angle_Ambiguity_T sensors_angle_amb[MAX_NUMBER_OF_SENSORS];

   /** \setup
   * Configure scenario that all conditions are met for marking detection as angle jump
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      // object
      relevant_object.vcs_position = Point(-2.0F, 6.0F);
      relevant_object.vcs_velocity = { 16.0F, 0.0F };
      relevant_object.Update_Bbox_Size(6.0F, 2.0F);

      // sensor
      valid_sensors[0] = true;
      rear_right_sensor_cal.constant.mounting_position.vcs_position = { -4.0F, 0.4F, 0.0F };
      rear_right_sensor.variable.vcs_velocity = { 15.0F, 0.0F };

      // detection
      raw_detection_list.number_of_valid_detections = 1;
      relevant_detection_raw.raw.sensor_id = 1;
      relevant_detection_raw.raw.range = 5.0F;
      relevant_detection_raw.raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_MIDHIGH;
      relevant_detection_raw.raw.range_rate = 0.0F;
      relevant_detection_prop.vcs_position = Point(-8.33, 2.9F);
      relevant_detection_raw.processed.vcs_az = F360_DEG2RAD(150.0F);
      relevant_detection_raw.processed.cos_vcs_az = F360_Cosf(relevant_detection_raw.processed.vcs_az);
      relevant_detection_raw.processed.sin_vcs_az = F360_Sinf(relevant_detection_raw.processed.vcs_az);
      relevant_detection_prop.range_rate_compensated = 13.8564F;

      sensors_angle_amb[0] = {true, {F360_DEG2RAD(60.F), INFTY, INFTY, INFTY, INFTY, INFTY}};
   }
};

/** \purpose
* Check if detection is NOT marked as angle jump due to no valid sensor
* \req
* NA
*/
TEST(f360_Check_Dets_Against_Angle_Jumps, detection_not_marked_due_to_no_valid_sensor)
{
   /** \precond
    * Same as setup with below changes:
    * no valid sensor
    */
   valid_sensors[0] = false;

    /** \action
     * Call Check_Dets_Against_Angle_Jumps()
     */
   Check_Dets_Against_Angle_Jumps(relevant_object, raw_detection_list, sensors_angle_amb, sensors, calibs, detection_props);
   /** \result
    * Detection NOT marked as angle jump
    */
   CHECK_FALSE(relevant_detection_prop.f_object_based_angle_jump);
}

/** \purpose
* Check if detection is NOT marked as angle jump due to being already object based angle jump
* \req
* NA
*/
TEST(f360_Check_Dets_Against_Angle_Jumps, detection_not_marked_due_to_being_already_object_based_angle_jump)
{
   /** \precond
    * Same as setup with below changes:
    * mark detection as angle jump
    * set detection to ok to use
    */
   relevant_detection_prop.f_ok_to_use = true;
   relevant_detection_prop.f_object_based_angle_jump=true;

   /** \action
    * Call Check_Dets_Against_Angle_Jumps()
    */
   Check_Dets_Against_Angle_Jumps(relevant_object, raw_detection_list, sensors_angle_amb, sensors, calibs, detection_props);

   /** \result
    * Detection marked as not ok to use. This is the only way to check it
    */
   CHECK_TRUE(relevant_detection_prop.f_ok_to_use);
}

/** \purpose
* Check if detection is NOT marked as angle jump due to is not suspected
* \req
* NA
*/
TEST(f360_Check_Dets_Against_Angle_Jumps, detection_not_marked_due_to_is_not_suspected)
{
   /** \precond
    * Same as setup with below changes:
    * set confidence azimuth to HIGH
    */
   relevant_detection_raw.raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;

   /** \action
    * Call Check_Dets_Against_Angle_Jumps()
    */
   Check_Dets_Against_Angle_Jumps(relevant_object, raw_detection_list, sensors_angle_amb, sensors, calibs, detection_props);

   /** \result
    * Detection NOT marked as angle jump
    */
   CHECK_FALSE(relevant_detection_prop.f_object_based_angle_jump);
}

/** \purpose
* Check if detection is NOT marked as angle jump due to not pass basic hypothesis (close to object border)
* \req
* NA
*/
TEST(f360_Check_Dets_Against_Angle_Jumps, detection_not_marked_due_to_not_pass_basic_hypothesis__object_border)
{
   /** \precond
    * Same as setup with below changes:
    * set range to 
    */
   relevant_detection_prop.vcs_position.y = relevant_object.vcs_position.y - 0.5F * relevant_object.bbox.Get_Width() + 0.0001F;

   /** \action
    * Call Check_Dets_Against_Angle_Jumps()
    */
   Check_Dets_Against_Angle_Jumps(relevant_object, raw_detection_list, sensors_angle_amb, sensors, calibs, detection_props);

   /** \result
    * Detection NOT marked as angle jump
    */
   CHECK_FALSE(relevant_detection_prop.f_object_based_angle_jump);
}

/** \purpose
* Check calibrations correctness
* \req
* NA
*/
TEST(f360_Check_Dets_Against_Angle_Jumps, calibrations_correctness)
{
   /** \result
    * Calibraions set as expected
    */
   DOUBLES_EQUAL(0.25F, calibs.obj_aj_border_half_width, tolerance);
   DOUBLES_EQUAL(10.0F, calibs.obj_aj_max_double_range_hypothesis, tolerance);
}
/** @}*/
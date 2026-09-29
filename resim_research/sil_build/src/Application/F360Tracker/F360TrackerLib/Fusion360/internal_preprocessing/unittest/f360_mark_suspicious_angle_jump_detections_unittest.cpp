/** \file
 * This file contains unit tests for content of f360_mark_suspicious_angle_jump_detections.cpp file
 */

#include "f360_mark_suspicious_angle_jump_detections.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_mark_suspicious_angle_jump_detection
 *  @{
 */

/** \brief
 * The purpose of this test group is to test mark_suspicious_angle_jump_detection().
 * mark_suspicious_angle_jump_detection() is used for flagging angle jump and then prevent detections from being used.
 */
TEST_GROUP(f360_mark_suspicious_angle_jump_detections)
{
   rspp_variant_A::RSPP_Detection_T det_raw{};
   F360_Detection_Props_T det_prop{};
   F360_Radar_Sensor_T sensor;
   F360_Host_T host;
   
   /** \setup
    * Set default setup to step into the function:
    * host.speed > 10;
    * abs(range_rate_compensated) > 0.7;
    * f_ok_to_use set to true; 
    * sensor_type set to FLR, currently only FLR7 can be used;
    * raw.snr < 25.0;
    * raw.range > 10.0;
    * abs(raw.azimuth) > 15 degrees;
    * sensor.constat.polarity can be 1 or -1, for this case polarity is set to 1; 
    * sensor.constant.v_wrapping set the value equal to sensor's look_id = 2; 
    * General setup should be able to pass all if statements and step into the function.
    * Then, in subsequent test cases, the properties are more refined.
    */
   TEST_SETUP()
   {
      host.speed = 30.0F;
      sensor.constant.sensor_type = F360_SENSOR_TYPE_FLR7_RADAR;
      sensor.constant.polarity = 1.0F;
      sensor.constant.v_wrapping[0] = 28.1F;
      sensor.constant.v_wrapping[1] = 28.1F;
      sensor.variable.vcs_velocity.lateral = 0.02F;
      sensor.variable.vcs_velocity.longitudinal = 20.0F;
      sensor.variable.vacs_boresight_az_estimated = -0.00057435036F;
      sensor.variable.is_valid = true;
      sensor.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
      det_prop.f_ok_to_use = true;
      det_prop.range_rate_compensated = -2.0F;
      det_prop.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      det_prop.vcs_position.y = 15.0F;
      det_raw.raw.azimuth = -0.47F;
      det_raw.raw.range_rate = -20.0F;
      det_raw.raw.snr = 10.0F;
      det_raw.raw.range = 108.0F;
      det_raw.processed.vcs_az = det_raw.raw.azimuth + sensor.variable.vacs_boresight_az_estimated;
   }
};

/** \purpose  
 * Purpose of this test is to verify that a detection that fits the angle jump hypothesis and has higher range is correctly flagged as ambiguous angle detection.
 */
TEST(f360_mark_suspicious_angle_jump_detections, check_is_detection_correctly_marked_1)
{
   /** \precond
    * This uses the test setup detection.
    */

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Stationary_Angle_Jump_Detection(sensor, host, det_raw, det_prop);

   /** \result
    * Check if the detection is correctly marked as f_angle_amb.
    */
   CHECK_TRUE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that a detection that fits the angle jump hypothesis and has higher range is correctly flagged as ambiguous angle detection for srr7 sensor.
 */
TEST(f360_mark_suspicious_angle_jump_detections, check_is_detection_correctly_marked_srr)
{
   /** \precond
    * This uses the test setup detection.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
   sensor.variable.vacs_boresight_az_estimated = 1.16F;
   det_raw.raw.azimuth = 0.23F;
   host.speed = 10.5F;
   det_raw.raw.range = 22.5F;
   det_raw.raw.range_rate = -10.0F;
   det_prop.range_rate_compensated = -8.0F;
   det_raw.processed.vcs_az = det_raw.raw.azimuth + sensor.variable.vacs_boresight_az_estimated;
   sensor.constant.v_wrapping[0] = 36.37F;
   sensor.constant.v_wrapping[1] = 31.96F;
   sensor.constant.v_wrapping[2] = 35.24F;
   sensor.constant.v_wrapping[3] = 30.71F;
   sensor.variable.vcs_velocity.lateral = 0.002F;
   sensor.variable.vcs_velocity.longitudinal = 10.4F;

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Stationary_Angle_Jump_Detection(sensor, host, det_raw, det_prop);

   /** \result
    * Check if the detection is correctly marked as f_angle_amb.
    */
   CHECK_TRUE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that if the higher sensor velocity for the measured range rate don't satisfy the angle jump hypothesis, the detection is not flagged as amb angle dets even if all other pre-conditions are fulfilled.
 */
TEST(f360_mark_suspicious_angle_jump_detections, check_is_detection_correctly_marked_3)
{
   /** \precond
    * Set the sensor velocity longitudinal to the higher value than in the test setup.
    */
   sensor.variable.vcs_velocity.longitudinal = 21.0F;

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Stationary_Angle_Jump_Detection(sensor, host, det_raw, det_prop);

   /** \result
    * Check if the detection is not marked as f_angle_amb.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Check that the function does flag correctly a detection as an ambiguous angle detection if there is another FLR radar type.
 */
TEST(f360_mark_suspicious_angle_jump_detections, check_for_different_radar_type)
{
   /** \precond
    * Set the sensor type to FLR7 PLT.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_FLR7_PLT_RADAR;
   
   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Stationary_Angle_Jump_Detection(sensor, host, det_raw, det_prop);

   /** \result
    * Check whether detection f_angle_amb flag was set to true.
    */
   CHECK_TRUE(det_prop.f_angle_amb)
}

/** \purpose  
 * Check that the function does not flag a detection as an ambiguous angle detection if it's marked as not ok to use.
 */
TEST(f360_mark_suspicious_angle_jump_detections, check_for_not_ok_to_use)
{
   /** \precond
    * Set the detection flag ok to use to false.
    */
   det_prop.f_ok_to_use = false;
   
   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Stationary_Angle_Jump_Detection(sensor, host, det_raw, det_prop);

   /** \result
    * Check whether detection f_angle_amb flag stays set as false.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Check that the function does not flag a detection as an ambiguous angle detection if the host speed is too low.
 */
TEST(f360_mark_suspicious_angle_jump_detections, check_for_low_host_speed)
{
   /** \precond
    * Set the host speed value lower than 10.0F.
    */
   host.speed = 3.0F;
   
   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Stationary_Angle_Jump_Detection(sensor, host, det_raw, det_prop);

   /** \result
    * Check whether detection f_angle_amb flag stays set as false.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Check that the function does not flag a detection as an ambiguous angle detection if the detection is stationary.
 */
TEST(f360_mark_suspicious_angle_jump_detections, check_another_moving_status)
{
   /** \precond
    * Set the detection motion status to stationary.
    */
   det_prop.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   
   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Stationary_Angle_Jump_Detection(sensor, host, det_raw, det_prop);

   /** \result
    * Check whether detection f_angle_amb flag stays set as false.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Check that the function does not flag a detection as an ambiguous angle detection if range of detection is too low.
 */
TEST(f360_mark_suspicious_angle_jump_detections, check_for_low_range)
{
   /** \precond
    * Set the detection range value lower than 10.0F.
    */
   det_raw.raw.range = 3.0F;
   
   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Stationary_Angle_Jump_Detection(sensor, host, det_raw, det_prop);

   /** \result
    * Check whether detection f_angle_amb flag stays set as false.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Check that the function does not flag a detection as an ambiguous angle detection if range rate compensated is too low.
 */
TEST(f360_mark_suspicious_angle_jump_detections, check_for_low_range_rate_compensated)
{
   /** \precond
    * Set the detection range rate compensated lower than 0.7F.
    */
   det_prop.range_rate_compensated = 0.6F;
   
   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Stationary_Angle_Jump_Detection(sensor, host, det_raw, det_prop);

   /** \result
    * Check whether detection f_angle_amb flag stays set as false.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Check that the function does not flag a detection as an ambiguous angle detection if snr is too high. 
 */
TEST(f360_mark_suspicious_angle_jump_detections, check_for_high_snr)
{
   /** \precond
    * Set the detection snr value higher than 25.0F.
    */
   det_raw.raw.snr = 26.0F;
   
   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Stationary_Angle_Jump_Detection(sensor, host, det_raw, det_prop);

   /** \result
    * Check whether detection f_angle_amb flag stays set as false.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Check that the function does not flag a detection as an ambiguous angle detection if azimuth is too low. 
 */
TEST(f360_mark_suspicious_angle_jump_detections, check_for_low_azimuth)
{
   /** \precond
    * Set the detection azimuth value lower than 0.2F.
    */
   det_raw.raw.azimuth = 0.1F;
   
   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Stationary_Angle_Jump_Detection(sensor, host, det_raw, det_prop);

   /** \result
    * Check whether detection f_angle_amb flag stays set as false.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose
 * Check that the function does not flag a detection as an ambiguous angle detection when when 
 * detection comes from outside of +-15 deg azimuth cone and other conditions are default.
 */
TEST(f360_mark_suspicious_angle_jump_detections, check_vcs_az_false_other_conditions_true)
{
   /** \precond
    * Set conditions to make:
    * - f_vcs_az_cond = false (abs(rspp_det.processed.vcs_az) <= 15 degrees)
    */

   det_raw.processed.vcs_az = 0.1F;  // Less than 15 degrees (0.2618 rad)

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Stationary_Angle_Jump_Detection(sensor, host, det_raw, det_prop);

   /** \result
    * Check whether detection f_angle_amb flag stays set as false
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** \purpose  
 * Purpose of this test is to verify that a with matching angles but no matching range rate
 * is not flagged as angle jump.
 */
TEST(f360_mark_suspicious_angle_jump_detections, check_is_detection_no_rr_match_srr)
{
   /** \precond
    * This uses the test setup detection.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
   sensor.variable.vacs_boresight_az_estimated = 1.16F;
   det_raw.raw.azimuth = 0.23F;
   host.speed = 10.5F;
   det_raw.raw.range = 22.5F;
   det_raw.raw.range_rate = -5.0F;
   det_prop.range_rate_compensated = 5.5F;
   det_raw.processed.vcs_az = det_raw.raw.azimuth + sensor.variable.vacs_boresight_az_estimated;
   sensor.constant.v_wrapping[0] = 36.37F;
   sensor.constant.v_wrapping[1] = 31.96F;
   sensor.constant.v_wrapping[2] = 35.24F;
   sensor.constant.v_wrapping[3] = 30.71F;
   sensor.variable.vcs_velocity.lateral = 0.002F;
   sensor.variable.vcs_velocity.longitudinal = 10.4F;

   /** \action
    * Call tested function.
    */
   Mark_Suspicious_Stationary_Angle_Jump_Detection(sensor, host, det_raw, det_prop);

   /** \result
    * Check if the detection is correctly marked as f_angle_amb.
    */
   CHECK_FALSE(det_prop.f_angle_amb)
}

/** @}*/

/** \defgroup  f360_determine_precond_for_angle_jumps
 *  @{
 */

/** \brief
 * The purpose of this test group is to test Determine_Precond_For_Angle_Jumps().
 * Determine_Precond_For_Angle_Jumps() is used to figure out what setup should be used to test angle jump hypothesis for specific detection.
 */
TEST_GROUP(f360_determine_precond_for_angle_jumps)
{
   F360_Radar_Sensor_T sensor;
   Sensor_Stationary_Angle_Ambiguity_T sensor_angle_amb;
   rspp_variant_A::RSPP_Detection_T det_raw{};
   F360_Detection_Props_T det_prop{};

   /** \setup
    * sensor is valid and is SRR7 plus
    */
   TEST_SETUP()
   {
      sensor.variable.is_valid = true;
      sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
      sensor_angle_amb = {};
      sensor.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
      sensor.constant.polarity = 1.0F;
      det_raw.raw.azimuth = 0.0F;
      det_prop.vcs_position.y = 15.0F;
   }
};

/** \purpose  
 * Purpose of this test is to verify that we get correct angle jump candidates when the used sensor is SRR7 plus.
 */
TEST(f360_determine_precond_for_angle_jumps, sensor_srr7_plus)
{
   /** \action
    * Call tested function.
    */
   const bool f_is_stationary_angle_jump_context = false;
   Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_stationary_angle_jump_context, sensor_angle_amb);

   /** \result
    * Check if all the potenatial angle jumps candidates are correct
    */
   CHECK_TRUE(sensor_angle_amb.f_sensor_relevant)
   CHECK_EQUAL(8U, sensor_angle_amb.nr_of_candidates)
   DOUBLES_EQUAL(F360_DEG2RAD(-90.0F), sensor_angle_amb.angle_ambiguity_candidates_rad[0], 0.0001F)
   DOUBLES_EQUAL(F360_DEG2RAD(-48.6F), sensor_angle_amb.angle_ambiguity_candidates_rad[1], 0.0001F)
   DOUBLES_EQUAL(F360_DEG2RAD(-30.0F), sensor_angle_amb.angle_ambiguity_candidates_rad[2], 0.0001F)
   DOUBLES_EQUAL(F360_DEG2RAD(-14.5F), sensor_angle_amb.angle_ambiguity_candidates_rad[3], 0.0001F)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[4], 0.0001F)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[5], 0.0001F)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[6], 0.0001F)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[7], 0.0001F)
}

/** \purpose
 * Purpose of this test is to verify that SRR7p Angle Jump Calculation is triggered even when Sensor is a Front Left mounted sensor
 */
TEST(f360_determine_precond_for_angle_jumps, sensor_srr7_plus_FLmounting)
{
    /** \precond
    * Set the sensor mounting location to Left Forward.
    */
    sensor.constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
    /** \action
     * Call tested function.
     */
    const bool f_is_stationary_angle_jump_context = false;
    Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_stationary_angle_jump_context, sensor_angle_amb);

    /** \result
     * Check if 8 angle jump candidates are output inferring that SRR7p angle jump algorithm was executed
     */
    CHECK_TRUE(sensor_angle_amb.f_sensor_relevant)
    CHECK_EQUAL(8U, sensor_angle_amb.nr_of_candidates)
}

/** \purpose
 * Purpose of this test is to verify that SRR7p Angle Jump Calculation is NOT triggered when sensor mounted is not on the front left or front right mounting location
 */
TEST(f360_determine_precond_for_angle_jumps, sensor_srr7_plus_RLmounting)
{
    /** \precond
    * Set the sensor mounting location to Left Rear.
    */
    sensor.constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
    /** \action
     * Call tested function.
     */
    const bool f_is_stationary_angle_jump_context = false;
    Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_stationary_angle_jump_context, sensor_angle_amb);

    /** \result
     * Check if f_sensor_relevant is correctly flagged as false and there are no angle jump candidates
     */
    CHECK_FALSE(sensor_angle_amb.f_sensor_relevant)
    CHECK_EQUAL(0U, sensor_angle_amb.nr_of_candidates)
}

/** \purpose  
 * Purpose of this test is to verify that we get correct angle jump candidates when the used sensor is FLR7.
 */
TEST(f360_determine_precond_for_angle_jumps, sensor_flr7)
{
   /** \precond
    * Set the sensor type to FLR7.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_FLR7_RADAR;
   sensor.constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
   /** \action
    * Call tested function.
    */
   const bool f_is_stationary_angle_jump_context = false;
   Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_stationary_angle_jump_context, sensor_angle_amb);

   /** \result
    * Check if all the potenatial angle jumps candidates are correct
    */
   CHECK_TRUE(sensor_angle_amb.f_sensor_relevant)
   CHECK_EQUAL(2U, sensor_angle_amb.nr_of_candidates)
   DOUBLES_EQUAL(F360_DEG2RAD(-19.4712F), sensor_angle_amb.angle_ambiguity_candidates_rad[0], 0.0001F)
   DOUBLES_EQUAL(F360_DEG2RAD(19.4712F), sensor_angle_amb.angle_ambiguity_candidates_rad[1], 0.0001F)
}

/** \purpose  
 * Purpose of this test is to verify what happens when the sensor type is unknown
 */
TEST(f360_determine_precond_for_angle_jumps, sensor_type_unknown)
{
   /** \precond
    * Set the sensor type to unknown.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_UNKNOWN;

   /** \action
    * Call tested function.
    */
   const bool f_is_stationary_angle_jump_context = false;
   Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_stationary_angle_jump_context, sensor_angle_amb);

   /** \result
    * Check if f_sensor_relevant is correctly flagged as false and there are no angle jump candidates
    */
   CHECK_FALSE(sensor_angle_amb.f_sensor_relevant)
   CHECK_EQUAL(0U, sensor_angle_amb.nr_of_candidates)
}

/** \purpose
 * Purpose of this test is to verify that FLR7 radars are disabled in moving angle jump context.
 */
TEST(f360_determine_precond_for_angle_jumps, moving_context_flr7_disabled)
{
   /** \precond
    * Set the sensor type to FLR7 and make it valid.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_FLR7_RADAR;
   sensor.variable.is_valid = true;

   /** \action
    * Call tested function with moving angle jump context (true).
    */
   const bool f_is_moving_angle_jump_context = true;
   Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_moving_angle_jump_context, sensor_angle_amb);

   /** \result
    * Check if f_sensor_relevant is correctly flagged as false for FLR7 in moving context
    */
   CHECK_FALSE(sensor_angle_amb.f_sensor_relevant)
   CHECK_EQUAL(0U, sensor_angle_amb.nr_of_candidates)
}

/** \purpose
 * Purpose of this test is to verify that SRR7+ radars with RIGHT_FORWARD mounting are enabled in moving angle jump context.
 */
TEST(f360_determine_precond_for_angle_jumps, moving_context_srr7_plus_enabled_right_forward)
{
   /** \precond
    * Set the sensor type to SRR7+ with right forward mounting and make it valid.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
   sensor.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
   sensor.variable.is_valid = true;
   det_raw.raw.azimuth = 0.0F; // Set to center for predictable LUT behavior

   /** \action
    * Call tested function with moving angle jump context (true).
    */
   const bool f_is_moving_angle_jump_context = true;
   Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_moving_angle_jump_context, sensor_angle_amb);

   /** \result
    * Check if f_sensor_relevant is correctly flagged as true for SRR7+ with RIGHT_FORWARD mounting in moving context
    */
   CHECK_TRUE(sensor_angle_amb.f_sensor_relevant)
   CHECK_TRUE(sensor_angle_amb.nr_of_candidates > 0U)
}

/** \purpose
 * Purpose of this test is to verify that SRR7+ radars with LEFT_FORWARD mounting are enabled in moving angle jump context.
 */
TEST(f360_determine_precond_for_angle_jumps, moving_context_srr7_plus_enabled_left_forward)
{
   /** \precond
    * Set the sensor type to SRR7+ with left forward mounting and make it valid.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
   sensor.constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   sensor.variable.is_valid = true;
   det_raw.raw.azimuth = 0.0F;

   /** \action
    * Call tested function with moving angle jump context (true).
    */
   const bool f_is_moving_angle_jump_context = true;
   Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_moving_angle_jump_context, sensor_angle_amb);

   /** \result
    * Check if f_sensor_relevant is correctly flagged as true for SRR7+ with LEFT_FORWARD mounting in moving context
    */
   CHECK_TRUE(sensor_angle_amb.f_sensor_relevant)
   CHECK_TRUE(sensor_angle_amb.nr_of_candidates > 0U)
}

/** \purpose
 * Purpose of this test is to verify that SRR7+ radars with LEFT_REAR mounting are enabled in moving angle jump context.
 */
TEST(f360_determine_precond_for_angle_jumps, moving_context_srr7_plus_enabled_left_rear)
{
   /** \precond
    * Set the sensor type to SRR7+ with left rear mounting and make it valid.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
   sensor.constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
   sensor.variable.is_valid = true;
   det_raw.raw.azimuth = 0.0F;

   /** \action
    * Call tested function with moving angle jump context (true).
    */
   const bool f_is_moving_angle_jump_context = true;
   Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_moving_angle_jump_context, sensor_angle_amb);

   /** \result
    * Check if f_sensor_relevant is correctly flagged as true for SRR7+ with LEFT_REAR mounting in moving context
    */
   CHECK_TRUE(sensor_angle_amb.f_sensor_relevant)
   CHECK_TRUE(sensor_angle_amb.nr_of_candidates > 0U)
}

/** \purpose
 * Purpose of this test is to verify that SRR7+ radars with RIGHT_REAR mounting are enabled in moving angle jump context.
 */
TEST(f360_determine_precond_for_angle_jumps, moving_context_srr7_plus_enabled_right_rear)
{
   /** \precond
    * Set the sensor type to SRR7+ with right rear mounting and make it valid.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
   sensor.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
   sensor.variable.is_valid = true;
   det_raw.raw.azimuth = 0.0F;

   /** \action
    * Call tested function with moving angle jump context (true).
    */
   const bool f_is_moving_angle_jump_context = true;
   Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_moving_angle_jump_context, sensor_angle_amb);

   /** \result
    * Check if f_sensor_relevant is correctly flagged as true for SRR7+ with RIGHT_REAR mounting in moving context
    */
   CHECK_TRUE(sensor_angle_amb.f_sensor_relevant)
   CHECK_TRUE(sensor_angle_amb.nr_of_candidates > 0U)
}

/** \purpose
 * Purpose of this test is to verify that SRR7+ radars with CENTER_FORWARD mounting are disabled in moving angle jump context.
 */
TEST(f360_determine_precond_for_angle_jumps, moving_context_srr7_plus_disabled_center_forward)
{
   /** \precond
    * Set the sensor type to SRR7+ with center forward mounting and make it valid.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
   sensor.constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
   sensor.variable.is_valid = true;
   det_raw.raw.azimuth = 0.0F;

   /** \action
    * Call tested function with moving angle jump context (true).
    */
   const bool f_is_moving_angle_jump_context = true;
   Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_moving_angle_jump_context, sensor_angle_amb);

   /** \result
    * Check if f_sensor_relevant is correctly flagged as false for SRR7+ with non-forward mounting in moving context
    */
   CHECK_FALSE(sensor_angle_amb.f_sensor_relevant)
   CHECK_EQUAL(0U, sensor_angle_amb.nr_of_candidates)
}

/** \purpose
 * Purpose of this test is to verify that FLR7 PLT radars are disabled in moving angle jump context.
 */
TEST(f360_determine_precond_for_angle_jumps, moving_context_flr7_plt_disabled)
{
   /** \precond
    * Set the sensor type to FLR7 PLT and make it valid.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_FLR7_PLT_RADAR;
   sensor.variable.is_valid = true;

   /** \action
    * Call tested function with moving angle jump context (true).
    */
   const bool f_is_moving_angle_jump_context = true;
   Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_moving_angle_jump_context, sensor_angle_amb);

   /** \result
    * Check if f_sensor_relevant is correctly flagged as false for FLR7 PLT in moving context
    */
   CHECK_FALSE(sensor_angle_amb.f_sensor_relevant)
   CHECK_EQUAL(0U, sensor_angle_amb.nr_of_candidates)
}

/** \purpose
 * Purpose of this test is to verify that unknown sensor types are disabled in moving angle jump context.
 */
TEST(f360_determine_precond_for_angle_jumps, moving_context_unknown_sensor_disabled)
{
   /** \precond
    * Set the sensor type to unknown and make it valid.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_UNKNOWN;
   sensor.variable.is_valid = true;

   /** \action
    * Call tested function with moving angle jump context (true).
    */
   const bool f_is_moving_angle_jump_context = true;
   Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_moving_angle_jump_context, sensor_angle_amb);

   /** \result
    * Check if f_sensor_relevant is correctly flagged as false for unknown sensor in moving context
    */
   CHECK_FALSE(sensor_angle_amb.f_sensor_relevant)
   CHECK_EQUAL(0U, sensor_angle_amb.nr_of_candidates)
}

/** \purpose
 * Purpose of this test is to verify that invalid sensors are handled correctly in moving angle jump context.
 */
TEST(f360_determine_precond_for_angle_jumps, moving_context_invalid_sensor)
{
   /** \precond
    * Set the sensor type to SRR7+ with right forward mounting but make it invalid.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
   sensor.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
   sensor.variable.is_valid = false;

   /** \action
    * Call tested function with moving angle jump context (true).
    */
   const bool f_is_moving_angle_jump_context = true;
   Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_moving_angle_jump_context, sensor_angle_amb);

   /** \result
    * Check if f_sensor_relevant is correctly flagged as false for invalid sensor in moving context
    */
   CHECK_FALSE(sensor_angle_amb.f_sensor_relevant)
   CHECK_EQUAL(0U, sensor_angle_amb.nr_of_candidates)
}

/** \purpose
 * Purpose of this test is to verify that sensor gen7v2 FLR types are enabled in not moving angle jump context.
 */
TEST(f360_determine_precond_for_angle_jumps, moving_context_gen7v2_flr_enabled)
{
   /** \precond
    * Set the sensor type to gen7v2 FLR and make it valid.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR;
   sensor.variable.is_valid = true;

   /** \action
    * Call tested function with moving angle jump context (false).
    */
   const bool f_is_moving_angle_jump_context = false;
   Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_moving_angle_jump_context, sensor_angle_amb);

   /** \result
    * Check if f_sensor_relevant is correctly flagged as true for gen7v2 FLR sensor in not moving context
    */
   CHECK_TRUE(sensor_angle_amb.f_sensor_relevant)
   CHECK_EQUAL(2U, sensor_angle_amb.nr_of_candidates)
}

/** \purpose
 * Purpose of this test is to verify that SRR gen7v2 sensors are handled correctly in moving angle jump context.
 */
TEST(f360_determine_precond_for_angle_jumps, moving_context_gen7v2_srr_enabled)
{
   /** \precond
    * Set the sensor type to SRR7+v2 with right forward mounting.
    */
   sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_V2_PLT_RADAR;
   sensor.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
   sensor.variable.is_valid = true;

   /** \action
    * Call tested function with moving angle jump context (true).
    */
   const bool f_is_moving_angle_jump_context = true;
   Determine_Precond_For_Angle_Jumps(sensor, det_raw, det_prop, f_is_moving_angle_jump_context, sensor_angle_amb);

   /** \result
    * Check if f_sensor_relevant is correctly flagged as true for valid sensor in moving context
    */
   CHECK_TRUE(sensor_angle_amb.f_sensor_relevant)
   CHECK_EQUAL(8U, sensor_angle_amb.nr_of_candidates)
}

/** @}*/

/** \defgroup  f360_get_srr7p_amb_angles
 *  @{
 */

 /** \brief
  * The purpose of this test group is to test Get_SRR7p_Amb_Angles().
  * Get_SRR7p_Amb_Angles() is used to calculate the angle jumps that need to be checked in SRR7p 
  * for a given measured det azimuth (since angle for angle jump depends on measured det aziumuth for SRR7p.
  */
TEST_GROUP(f360_get_srr7p_amb_angles)
{
    F360_Radar_Sensor_T sensor;
    Sensor_Stationary_Angle_Ambiguity_T sensor_angle_amb;
    rspp_variant_A::RSPP_Detection_T det_raw{};
    F360_Detection_Props_T det_prop{};

    /** \setup
     * set up azimuth of detection between 0 and 5 degrees but closer to 0 degrees.
     */
    TEST_SETUP()
    {
        sensor_angle_amb = {};
        sensor.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
        sensor.constant.polarity = 1.0F;
        det_raw.raw.azimuth = F360_DEG2RAD(2.0F);
        det_prop.vcs_position.y = 15.0F;
    }
};

/** \purpose
 * Purpose of this test is to verify that we get correct angle jump candidates for SRR7 plus given a certain raw det azimuth
 * which is in between two look up table rows but closer to the lower value so that it should select the lower value of table.
 */
TEST(f360_get_srr7p_amb_angles, srr7_plus_amb_angle_check_floor)
{
    /** \action
     * Call tested function.
     */
    Get_SRR7p_Amb_Angles(sensor, det_raw, det_prop, sensor_angle_amb);

    /** \result
     * Check if all the potenatial angle jumps candidates are correct
     */
    CHECK_EQUAL(8U, sensor_angle_amb.nr_of_candidates)
    DOUBLES_EQUAL(F360_DEG2RAD(-90.0F), sensor_angle_amb.angle_ambiguity_candidates_rad[0], 0.0001F)
    DOUBLES_EQUAL(F360_DEG2RAD(-48.6F), sensor_angle_amb.angle_ambiguity_candidates_rad[1], 0.0001F)
    DOUBLES_EQUAL(F360_DEG2RAD(-30.0F), sensor_angle_amb.angle_ambiguity_candidates_rad[2], 0.0001F)
    DOUBLES_EQUAL(F360_DEG2RAD(-14.5F), sensor_angle_amb.angle_ambiguity_candidates_rad[3], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[4], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[5], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[6], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[7], 0.0001F)
}

/** \purpose
 * Purpose of this test is to verify that we get correct angle jump candidates for SRR7 plus given a certain raw det azimuth
 * which is in between two look up table rows but closer to the upper value so that it should select the upper value of table.
 */
TEST(f360_get_srr7p_amb_angles, srr7_plus_amb_angle_check_roof)
{
    /** \precond
    * Set azimuth between 0 and 5 degrees but closer to 5 degrees
    */
    det_raw.raw.azimuth = F360_DEG2RAD(3.0F);
    /** \action
     * Call tested function.
     */
    Get_SRR7p_Amb_Angles(sensor, det_raw, det_prop, sensor_angle_amb);

    /** \result
     * Check if all the potenatial angle jumps candidates are correct
     */
    CHECK_EQUAL(8U, sensor_angle_amb.nr_of_candidates)
    DOUBLES_EQUAL(F360_DEG2RAD(-70.9F), sensor_angle_amb.angle_ambiguity_candidates_rad[0], 0.0001F)
    DOUBLES_EQUAL(F360_DEG2RAD(-46.5F), sensor_angle_amb.angle_ambiguity_candidates_rad[1], 0.0001F)
    DOUBLES_EQUAL(F360_DEG2RAD(-29.4F), sensor_angle_amb.angle_ambiguity_candidates_rad[2], 0.0001F)
    DOUBLES_EQUAL(F360_DEG2RAD(-14.4F), sensor_angle_amb.angle_ambiguity_candidates_rad[3], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[4], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[5], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[6], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[7], 0.0001F)
}

/** \purpose
 * Purpose of this test is to verify that the angle jump candidate selected from the lookup table is correct
 * when the object is on the left of host and the polarity of the sensor stays at 1.
 */
TEST(f360_get_srr7p_amb_angles, srr7_plus_amb_angle_check_left_of_host)
{
    /** \precond
    * Set det position to left of host
    */
    det_prop.vcs_position.y = -15.0F;

    /** \action
     * Call tested function.
     */
    Get_SRR7p_Amb_Angles(sensor, det_raw, det_prop, sensor_angle_amb);

    /** \result
     * Check if all the potenatial angle jumps candidates are correct
     */
    CHECK_EQUAL(8U, sensor_angle_amb.nr_of_candidates)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[0], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[1], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[2], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[3], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[4], 0.0001F)
    DOUBLES_EQUAL(F360_DEG2RAD(14.5F), sensor_angle_amb.angle_ambiguity_candidates_rad[5], 0.0001F)
    DOUBLES_EQUAL(F360_DEG2RAD(30.0F), sensor_angle_amb.angle_ambiguity_candidates_rad[6], 0.0001F)
    DOUBLES_EQUAL(F360_DEG2RAD(48.6F), sensor_angle_amb.angle_ambiguity_candidates_rad[7], 0.0001F)
}

/** \purpose
 * Purpose of this test is to verify that the angle jump candidate selected from the lookup table is correct
 * when the object is on the right of host and the polarity of the sensor is -1.
 */
TEST(f360_get_srr7p_amb_angles, srr7_plus_amb_angle_check_neg_polarity)
{
    /** \precond
    * Set det position to left of host
    */
    sensor.constant.polarity = -1.0F;

    /** \action
     * Call tested function.
     */
    Get_SRR7p_Amb_Angles(sensor, det_raw, det_prop, sensor_angle_amb);

    /** \result
     * Check if all the potenatial angle jumps candidates are correct
     */
    CHECK_EQUAL(8U, sensor_angle_amb.nr_of_candidates)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[0], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[1], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[2], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[3], 0.0001F)
    DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[4], 0.0001F)
    DOUBLES_EQUAL(F360_DEG2RAD(14.5F), sensor_angle_amb.angle_ambiguity_candidates_rad[5], 0.0001F)
    DOUBLES_EQUAL(F360_DEG2RAD(30.0F), sensor_angle_amb.angle_ambiguity_candidates_rad[6], 0.0001F)
    DOUBLES_EQUAL(F360_DEG2RAD(48.6F), sensor_angle_amb.angle_ambiguity_candidates_rad[7], 0.0001F)
}

/** \purpose
 * Purpose of this test is to verify that Get_SRR7p_Amb_Angles selects positive (counterclockwise) angle
 * candidates for a rear sensor when the detection is on the positive-y side of the host.
 * For a forward sensor with positive y the clockwise (negative) shifts are selected.
 * For a rear sensor the direction is inverted: positive y yields counterclockwise (positive) shifts.
 */
TEST(f360_get_srr7p_amb_angles, srr7_plus_amb_angle_check_rear_sensor_positive_y)
{
   /** \precond
    * Change mounting to RIGHT_REAR. det_prop.vcs_position.y remains +15.0 from TEST_SETUP.
    * For a forward sensor + positive y this would give clockwise (negative) shifts.
    * For a rear sensor it gives counterclockwise (positive) shifts.
    */
   sensor.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;

   /** \action
    * Call tested function.
    */
   Get_SRR7p_Amb_Angles(sensor, det_raw, det_prop, sensor_angle_amb);

   /** \result
    * LUT row 15 (raw az ~2 deg) positive entries: 14.5, 30.0, 48.6 deg.
    * Entries 0-4 become INFTY (negative or zero values excluded by min=0, max=135).
    */
   CHECK_EQUAL(8U, sensor_angle_amb.nr_of_candidates)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[0], 0.0001F)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[1], 0.0001F)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[2], 0.0001F)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[3], 0.0001F)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[4], 0.0001F)
   DOUBLES_EQUAL(F360_DEG2RAD(14.5F), sensor_angle_amb.angle_ambiguity_candidates_rad[5], 0.0001F)
   DOUBLES_EQUAL(F360_DEG2RAD(30.0F), sensor_angle_amb.angle_ambiguity_candidates_rad[6], 0.0001F)
   DOUBLES_EQUAL(F360_DEG2RAD(48.6F), sensor_angle_amb.angle_ambiguity_candidates_rad[7], 0.0001F)
}

/** \purpose
 * Purpose of this test is to verify that Get_SRR7p_Amb_Angles selects negative (clockwise) angle
 * candidates for a rear sensor when the detection is on the negative-y side of the host.
 * For a forward sensor with negative y the counterclockwise (positive) shifts are selected.
 * For a rear sensor the direction is inverted: negative y yields clockwise (negative) shifts.
 */
TEST(f360_get_srr7p_amb_angles, srr7_plus_amb_angle_check_rear_sensor_negative_y)
{
   /** \precond
    * Change mounting to RIGHT_REAR and move detection to negative y (left side of host).
    * For a forward sensor + negative y this would give counterclockwise (positive) shifts.
    * For a rear sensor it gives clockwise (negative) shifts.
    */
   sensor.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
   det_prop.vcs_position.y = -15.0F;

   /** \action
    * Call tested function.
    */
   Get_SRR7p_Amb_Angles(sensor, det_raw, det_prop, sensor_angle_amb);

   /** \result
    * LUT row 15 (raw az ~2 deg) negative entries: -90.0, -48.6, -30.0, -14.5 deg.
    * Entries 4-7 become INFTY (positive or zero values excluded by min=-135, max=0).
    */
   CHECK_EQUAL(8U, sensor_angle_amb.nr_of_candidates)
   DOUBLES_EQUAL(F360_DEG2RAD(-90.0F), sensor_angle_amb.angle_ambiguity_candidates_rad[0], 0.0001F)
   DOUBLES_EQUAL(F360_DEG2RAD(-48.6F), sensor_angle_amb.angle_ambiguity_candidates_rad[1], 0.0001F)
   DOUBLES_EQUAL(F360_DEG2RAD(-30.0F), sensor_angle_amb.angle_ambiguity_candidates_rad[2], 0.0001F)
   DOUBLES_EQUAL(F360_DEG2RAD(-14.5F), sensor_angle_amb.angle_ambiguity_candidates_rad[3], 0.0001F)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[4], 0.0001F)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[5], 0.0001F)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[6], 0.0001F)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[7], 0.0001F)
}

/** \defgroup  f360_get_flr7_amb_angles
 *  @{
 */

 /** \brief
  * The purpose of this test group is to test Get_FLR7_Amb_Angles().
  * Get_FLR7_Amb_Angles() is used to calculate the angle jumps that need to be checked in FLR7
  * for a given measured detection azimuth (since angle for angle jump depends on measured det azimuth for FLR7).
  */
TEST_GROUP(f360_get_flr7_amb_angles)
{
   Sensor_Stationary_Angle_Ambiguity_T sensor_angle_amb;
   rspp_variant_A::RSPP_Detection_T det_raw{};

   /** \setup
    * Set up default detection azimuth to 0 degrees.
    */
   TEST_SETUP()
   {
      det_raw.raw.azimuth = F360_DEG2RAD(0.0F);
      sensor_angle_amb = {};
   }
};

/** \purpose
 * Purpose of this test is to verify that we get correct angle jump candidates for FLR7 given a center azimuth (0 deg).
 */
TEST(f360_get_flr7_amb_angles, flr7_amb_angle_center_azimuth)
{
   /** \action
    * Call tested function.
    */
   Get_FLR7_Amb_Angles(det_raw, sensor_angle_amb);

   /** \result
    * Check if both angle jump candidates are correct.
    */
   CHECK_EQUAL(2U, sensor_angle_amb.nr_of_candidates)
   DOUBLES_EQUAL(F360_DEG2RAD(-19.4712F), sensor_angle_amb.angle_ambiguity_candidates_rad[0], 0.0001F)
   DOUBLES_EQUAL(F360_DEG2RAD(19.4712F), sensor_angle_amb.angle_ambiguity_candidates_rad[1], 0.0001F)
}

/** \purpose
 * Purpose of this test is to verify that the LUT index saturates at lower bound for highly negative azimuth.
 */
TEST(f360_get_flr7_amb_angles, flr7_amb_angle_min_azimuth)
{
   /** \precond
    * Set azimuth to -50 degrees (below LUT range).
    */
   det_raw.raw.azimuth = F360_DEG2RAD(-50.0F);

   /** \action
    * Call tested function.
    */
   Get_FLR7_Amb_Angles(det_raw, sensor_angle_amb);

   /** \result
    * Check if both angle jump candidates are correct (from LUT index 0). Smaller angle jump should be set to INFTY in this case as it is out of FOV.
    */
   CHECK_EQUAL(2U, sensor_angle_amb.nr_of_candidates)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[0], 0.0001F)
   DOUBLES_EQUAL(F360_DEG2RAD(21.9737F), sensor_angle_amb.angle_ambiguity_candidates_rad[1], 0.0001F)
}

/** \purpose
 * Purpose of this test is to verify that the LUT index saturates at upper bound for highly positive azimuth.
 */
TEST(f360_get_flr7_amb_angles, flr7_amb_angle_max_azimuth)
{
   /** \precond
    * Set azimuth to +50 degrees (above LUT range).
    */
   det_raw.raw.azimuth = F360_DEG2RAD(50.0F);

   /** \action
    * Call tested function.
    */
   Get_FLR7_Amb_Angles(det_raw, sensor_angle_amb);

   /** \result
    * Check if both angle jump candidates are correct (from LUT index 8). Bigger angle jump should be set to INFTY in this case as it is out of FOV.
    */
   CHECK_EQUAL(2U, sensor_angle_amb.nr_of_candidates)
   DOUBLES_EQUAL(F360_DEG2RAD(-21.9737F), sensor_angle_amb.angle_ambiguity_candidates_rad[0], 0.0001F)
   DOUBLES_EQUAL(INFTY, sensor_angle_amb.angle_ambiguity_candidates_rad[1], 0.0001F)
}
/** @}*/

/** \defgroup f360_is_rear_corner_sensor
 *  @{
 */

/** \brief
 * The purpose of this test group is to test Is_Rear_Corner_Sensor().
 * Is_Rear_Corner_Sensor() returns true when the sensor has a rear corner mounting location
 * (LEFT_REAR or RIGHT_REAR) and false for all other mounting locations.
 */
TEST_GROUP(f360_is_rear_corner_sensor)
{
   F360_Radar_Sensor_T sensor;

   TEST_SETUP()
   {
      sensor = {};
   }
};

/** \purpose
 * Purpose of this test is to verify that Is_Rear_Corner_Sensor returns true for LEFT_REAR mounting.
 */
TEST(f360_is_rear_corner_sensor, returns_true_for_left_rear)
{
   /** \precond
    * Set mounting location to LEFT_REAR.
    */
   sensor.constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;

   /** \result
    * Check that Is_Rear_Corner_Sensor returns true.
    */
   CHECK_TRUE(Is_Rear_Corner_Sensor(sensor))
}

/** \purpose
 * Purpose of this test is to verify that Is_Rear_Corner_Sensor returns true for RIGHT_REAR mounting.
 */
TEST(f360_is_rear_corner_sensor, returns_true_for_right_rear)
{
   /** \precond
    * Set mounting location to RIGHT_REAR.
    */
   sensor.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;

   /** \result
    * Check that Is_Rear_Corner_Sensor returns true.
    */
   CHECK_TRUE(Is_Rear_Corner_Sensor(sensor))
}

/** \purpose
 * Purpose of this test is to verify that Is_Rear_Corner_Sensor returns false for LEFT_FORWARD mounting.
 */
TEST(f360_is_rear_corner_sensor, returns_false_for_left_forward)
{
   /** \precond
    * Set mounting location to LEFT_FORWARD.
    */
   sensor.constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;

   /** \result
    * Check that Is_Rear_Corner_Sensor returns false.
    */
   CHECK_FALSE(Is_Rear_Corner_Sensor(sensor))
}

/** \purpose
 * Purpose of this test is to verify that Is_Rear_Corner_Sensor returns false for RIGHT_FORWARD mounting.
 */
TEST(f360_is_rear_corner_sensor, returns_false_for_right_forward)
{
   /** \precond
    * Set mounting location to RIGHT_FORWARD.
    */
   sensor.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;

   /** \result
    * Check that Is_Rear_Corner_Sensor returns false.
    */
   CHECK_FALSE(Is_Rear_Corner_Sensor(sensor))
}

/** @}*/

/** \file
 * This file contains unit tests for content of f360_object_based_angle_jump_detector_internals.cpp file
 */

#include "f360_object_based_angle_jump_detector_internals.h"
#include "f360_math.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;
//using namespace f360_variant_A_variant_A::aj_detector; removed temporarly - DFU-511




/** \defgroup  f360_object_based_angle_jump_detector_internals__Is_Det_Suspected
 *  @{
 */
 /** \brief
  * Test group for testing Is_Det_Suspected()
  */
TEST_GROUP(f360_object_based_angle_jump_detector_internals__Is_Det_Suspected)
{
   Det_Restrictions_T det_restrictions {};
   const float32_t floating_threshold = 0.0001F;

   /** \setup
   * Set detection restrictions structure
   **/
   TEST_SETUP()
   {
      det_restrictions.min_lat = 2.0F;
      det_restrictions.max_lat = 7.0F;
      det_restrictions.max_long = 10.0F;
      det_restrictions.min_long = -10.0F;
      det_restrictions.max_range = 5.0F;
   }
};

/** \purpose
 * Is_Det_Suspected should return true if all conditions are met (case 1)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Suspected, detection_meets_all_condtions_to_be_suspected_case_1)
{
   /** \precond
    * Range below threshold
    * detection is outside of restricted bounding box
    */
   float32_t det_range = det_restrictions.max_range - floating_threshold;

   Point det_pos;
   det_pos.y = det_restrictions.max_lat + floating_threshold;
   det_pos.x = det_restrictions.min_long + floating_threshold;

   /** \action
    * Call Is_Det_Suspected()
    */

   bool result = Is_Det_Suspected(det_range, det_pos, det_restrictions);

   /** \result
    * Check if true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Is_Det_Suspected should return true if all conditions are met (case 2)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Suspected, detection_meets_all_condtions_to_be_suspected_case_2)
{
   /** \precond
    * Range below threshold
    * detection is outside of restricted bounding box
    */
   float32_t det_range = det_restrictions.max_range - floating_threshold;

   Point det_pos;
   det_pos.y = det_restrictions.min_lat - floating_threshold;
   det_pos.x = det_restrictions.min_long + floating_threshold;

   /** \action
    * Call Is_Det_Suspected()
    */

   bool result = Is_Det_Suspected(det_range, det_pos, det_restrictions);

   /** \result
    * Check if true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Is_Det_Suspected should return true if all conditions are met (case 3)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Suspected, detection_meets_all_condtions_to_be_suspected_case_3)
{
   /** \precond
    * Range below threshold
    * detection is outside of restricted bounding box
    */
   float32_t det_range = det_restrictions.max_range - floating_threshold;

   Point det_pos;
   det_pos.y = det_restrictions.max_lat - floating_threshold;
   det_pos.x = det_restrictions.min_long - floating_threshold;

   /** \action
    * Call Is_Det_Suspected()
    */

   bool result = Is_Det_Suspected(det_range, det_pos, det_restrictions);

   /** \result
    * Check if true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Is_Det_Suspected should return true if all conditions are met (case 4)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Suspected, detection_meets_all_condtions_to_be_suspected_case_4)
{
   /** \precond
    * Range below threshold
    * detection is outside of restricted bounding box
    */
   float32_t det_range = det_restrictions.max_range - floating_threshold;

   Point det_pos;
   det_pos.y = det_restrictions.max_lat - floating_threshold;
   det_pos.x = det_restrictions.max_long + floating_threshold;

   /** \action
    * Call Is_Det_Suspected()
    */

   bool result = Is_Det_Suspected(det_range, det_pos, det_restrictions);

   /** \result
    * Check if true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Is_Det_Suspected should return false if range is too high
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Suspected, not_suspected_due_to_high_range)
{
   /** \precond
    * Range above threshold
    * detection is outside of restricted bounding box
    */
   float32_t det_range = det_restrictions.max_range + floating_threshold;

   Point det_pos;
   det_pos.y = det_restrictions.max_lat + floating_threshold;
   det_pos.x = det_restrictions.min_long + floating_threshold;

   /** \action
    * Call Is_Det_Suspected()
    */

   bool result = Is_Det_Suspected(det_range, det_pos, det_restrictions);

   /** \result
    * Check if false
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Is_Det_Suspected should return false if range is too low
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Suspected, not_suspected_due_to_low_range)
{
   /** \precond
    * Range below threshold
    * detection is outside of restricted bounding box
    */
   float32_t det_range = det_restrictions.min_range - floating_threshold;

   Point det_pos;
   det_pos.y = det_restrictions.max_lat + floating_threshold;
   det_pos.x = det_restrictions.min_long + floating_threshold;

   /** \action
    * Call Is_Det_Suspected()
    */

   bool result = Is_Det_Suspected(det_range, det_pos, det_restrictions);

   /** \result
    * Check if false
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Is_Det_Suspected should return false if detection is inside bounding box
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Suspected, not_suspected_due_to_being_inside_of_bbox)
{
   /** \precond
    * Range within threshold
    * detection is inside restricted bounding box
    */
   float32_t det_range = det_restrictions.max_range - floating_threshold;

   Point det_pos;
   det_pos.y = det_restrictions.max_lat - floating_threshold;
   det_pos.x = det_restrictions.min_long + floating_threshold;

   /** \action
    * Call Is_Det_Suspected()
    */
   bool result = Is_Det_Suspected(det_range, det_pos, det_restrictions);

   /** \result
    * Check if false
    */
   CHECK_FALSE(result);
}
/** @}*/


/** \defgroup  f360_object_based_angle_jump_detector_internals__Calc_Det_Restrictions_Without_Max_Range
 *  @{
 */
 /** \brief
  * Test group for testing Calc_Det_Restrictions_Without_Max_Range()
  */
TEST_GROUP(f360_object_based_angle_jump_detector_internals__Calc_Det_Restrictions_Without_Max_Range)
{
   const float32_t floating_threshold = 0.0001F;

   F360_Object_Track_T object_track {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};

   /** \setup
   * Setup calibrations
   * Set object width
   **/
   TEST_SETUP()
   {
      object_track.bbox.Set_Width(3.0F);
   }
};

/** \purpose
 * Calc_Det_Restrictions_Without_Max_Range should correctly calculate restriction from object on the right side (without saturation)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Calc_Det_Restrictions_Without_Max_Range, calculation_correctness_right_object_no_saturation)
{
   /** \precond
    * Object on the right
    */
   object_track.vcs_position = {-3.0F, 2.0F};
   Point center = {-3.0F, 2.0F};
   object_track.reference_point = F360_REFERENCE_POINT_CENTER;
   object_track.bbox.Set_Center(center);
   object_track.bbox.Set_Length(6.0F);

   /** \action
    * Call Calc_Det_Restrictions_Without_Max_Range()
    */
   Det_Restrictions_T det_restrictions = Calc_Det_Restrictions_Without_Max_Range(object_track);

   /** \result
    * 
    */
   DOUBLES_EQUAL(0.0F, det_restrictions.max_range, floating_threshold);
   DOUBLES_EQUAL(0.0F, det_restrictions.min_range, floating_threshold);
   DOUBLES_EQUAL(-0.4F, det_restrictions.min_lat, floating_threshold);
   DOUBLES_EQUAL(4.1F, det_restrictions.max_lat, floating_threshold);
   DOUBLES_EQUAL(-6.0F, det_restrictions.min_long, floating_threshold);
   DOUBLES_EQUAL(0.0F, det_restrictions.max_long, floating_threshold);
}

/** \purpose
 * Calc_Det_Restrictions_Without_Max_Range should correctly calculate restriction from object on the right side (with saturation)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Calc_Det_Restrictions_Without_Max_Range, calculation_correctness_right_object_with_saturation)
{
   /** \precond
    * Object on the right
    */
   object_track.vcs_position = {-3.0F, 2.0F};
   object_track.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = {-3.0F, 2.0F};
   object_track.bbox.Set_Center(center);
   object_track.bbox.Set_Length(12.0F);

   /** \action
    * Call Calc_Det_Restrictions_Without_Max_Range()
    */
   Det_Restrictions_T det_restrictions = Calc_Det_Restrictions_Without_Max_Range(object_track);

   /** \result
    *
    */
   DOUBLES_EQUAL(det_restrictions.max_range, 0.0F, floating_threshold);
   DOUBLES_EQUAL(det_restrictions.min_range, 0.0F, floating_threshold);
   DOUBLES_EQUAL(det_restrictions.min_lat, -0.4F, floating_threshold);
   DOUBLES_EQUAL(det_restrictions.max_lat, 4.1F, floating_threshold);
   DOUBLES_EQUAL(det_restrictions.min_long, -9.0F, floating_threshold);
   DOUBLES_EQUAL(det_restrictions.max_long, 3.0F, floating_threshold);
}

/** \purpose
 * Calc_Det_Restrictions_Without_Max_Range should correctly calculate restriction from object on the left side (without saturation)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Calc_Det_Restrictions_Without_Max_Range, calculation_correctness_left_object_no_saturation)
{
   /** \precond
    * Object on the right
    */
   object_track.vcs_position = {-3.0F, -2.0F};
   object_track.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = {-3.0F, -2.0F};
   object_track.bbox.Set_Center(center);
   object_track.bbox.Set_Length(6.0F);

   /** \action
    * Call Calc_Det_Restrictions_Without_Max_Range()
    */
   Det_Restrictions_T det_restrictions = Calc_Det_Restrictions_Without_Max_Range(object_track);

   /** \result
    *
    */
   DOUBLES_EQUAL(0.0F, det_restrictions.max_range, floating_threshold);
   DOUBLES_EQUAL(0.0F, det_restrictions.min_range, floating_threshold);
   DOUBLES_EQUAL(-4.1F, det_restrictions.min_lat, floating_threshold);
   DOUBLES_EQUAL(0.4F, det_restrictions.max_lat, floating_threshold);
   DOUBLES_EQUAL(-6.0F, det_restrictions.min_long, floating_threshold);
   DOUBLES_EQUAL(0.0F, det_restrictions.max_long, floating_threshold);
}

/** \purpose
 * Calc_Det_Restrictions_Without_Max_Range should correctly calculate restriction from object on the left side (with saturation)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Calc_Det_Restrictions_Without_Max_Range, calculation_correctness_left_object_with_saturation)
{
   /** \precond
    * Object on the right
    */
   object_track.vcs_position = {-3.0F, -2.0F};
   object_track.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = {-3.0F, -2.0F};
   object_track.bbox.Set_Center(center);
   object_track.bbox.Set_Length(12.0F);

   /** \action
    * Call Calc_Det_Restrictions_Without_Max_Range()
    */
   Det_Restrictions_T det_restrictions = Calc_Det_Restrictions_Without_Max_Range(object_track);

   /** \result
    *
    */
   DOUBLES_EQUAL(0.0F, det_restrictions.max_range, floating_threshold);
   DOUBLES_EQUAL(0.0F, det_restrictions.min_range, floating_threshold);
   DOUBLES_EQUAL(-4.1F, det_restrictions.min_lat, floating_threshold);
   DOUBLES_EQUAL(0.4F, det_restrictions.max_lat, floating_threshold);
   DOUBLES_EQUAL(-9.0F, det_restrictions.min_long, floating_threshold);
   DOUBLES_EQUAL(3.0F, det_restrictions.max_long, floating_threshold);
}
/** @}*/


/** \defgroup  f360_object_based_angle_jump_detector_internals__Calc_Max_And_Min_Range
 *  @{
 */
 /** \brief
  * Test group for testing Calc_Max_And_Min_Range()
  */
TEST_GROUP(f360_object_based_angle_jump_detector_internals__Calc_Max_And_Min_Range)
{
   const float32_t floating_threshold = 0.0001F;

   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];
   bool valid_sensors[MAX_NUMBER_OF_SENSORS];

   Det_Restrictions_T det_restrictions{};

   /** \setup
   * Set sensors calibrations. Only first is valid.
   **/
   TEST_SETUP()
   {
      for (int idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
      {
         sensors[idx].variable.is_valid = false;
         valid_sensors[idx] = false;
      }

      // Front
      sensors[0].constant.mounting_position.vcs_position.lateral = 0.1F;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.5F;
      sensors[0].variable.is_valid = true;
      valid_sensors[0] = true;
   }
};

/** \purpose
 * Calc_Max_And_Min_Range should correctly calculate max & min range per sensor for an object on the right
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Calc_Max_And_Min_Range, calculation_correctness_object_on_the_right)
{
   /** \precond
    * Object on the front right represented by det_restriction
    */
   det_restrictions.min_lat = 1.0F;
   det_restrictions.max_lat = 4.0F;
   det_restrictions.min_long = 1.0F;
   det_restrictions.max_long = 7.0F;

   /** \action
    * Call Calc_Max_And_Min_Range()
    */
   
   Calc_Max_And_Min_Range(sensors[0], det_restrictions);

   DOUBLES_EQUAL(7.5802F, det_restrictions.max_range, floating_threshold);
   DOUBLES_EQUAL(1.0296F, det_restrictions.min_range, floating_threshold);
}

/** \purpose
 * Calc_Max_And_Min_Range should correctly calculate max & min range per sensor for an object on the left
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Calc_Max_And_Min_Range, calculation_correctness_object_on_the_left)
{
   /** \precond
    * Object on the front left represented by det_restriction
    */
   det_restrictions.min_lat = -1.0F;
   det_restrictions.max_lat = -4.0F;
   det_restrictions.min_long = 1.0F;
   det_restrictions.max_long = 7.0F;

      /** \action
    * Call Calc_Max_And_Min_Range()
    */
   
   Calc_Max_And_Min_Range(sensors[0], det_restrictions);

   DOUBLES_EQUAL(7.685F, det_restrictions.max_range, floating_threshold);
   DOUBLES_EQUAL(1.2083F, det_restrictions.min_range, floating_threshold);
}
/** @}*/


/** \defgroup  f360_object_based_angle_jump_detector_internals__Is_Det_Object_Based_Angle_Jump
 *  @{
 */
 /** \brief
  * Test group for testing Is_Det_Object_Based_Angle_Jump()
  */
TEST_GROUP(f360_object_based_angle_jump_detector_internals__Is_Det_Object_Based_Angle_Jump)
{
   const float32_t floating_threshold = 0.0001F;

   F360_VCS_Velocity_T sensor_velocity {};
   Point sensor_mount_pos_vcs {};
   F360_VCS_Velocity_T obj_velocity_vcs {};
   F360_Calibrations_T calibs {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   Sensor_Angle_Ambiguity_T sensors_angle_amb[MAX_NUMBER_OF_SENSORS];
   F360_Detection_Props_T det_prop {};
   rspp_variant_A::RSPP_Detection_T det_raw {};
   Det_Restrictions_T det_restrictions {};

   /** \setup
   * Set angle jump value to 60 deg - SRR5 case
   * Reference object overtakes host
   * Sensor is mounted front right
   * Set detection property mimicing a real angle jump scenario
   **/
   TEST_SETUP()
   {
      // object overtakes host
      sensor_velocity.lateral = 0.5F;
      sensor_velocity.longitudinal = 5.0F;
      obj_velocity_vcs.lateral = 0.0F;
      obj_velocity_vcs.longitudinal = 10.0F;

      //Front right
      sensor_mount_pos_vcs.y = 0.9F;
      sensor_mount_pos_vcs.x = -0.25F;

      calibs.obj_aj_border_half_width = 0.25F;

      // set detection filtering conditions
      det_restrictions.max_range = 10.0F;
      det_restrictions.min_lat = 1.0F;
      det_restrictions.max_lat = 4.0F;
      det_restrictions.min_long = -6.0F;
      det_restrictions.max_long = 2.5F;

      // set detection properties
      det_raw.raw.range = 4.2F;
      det_raw.raw.range_rate = 5.0F;
      det_raw.raw.sensor_id = 1;
      det_raw.raw.azimuth = F360_DEG2RAD(0.0F);

      sensors[0].constant.polarity = 1;
      sensors[0].variable.vacs_boresight_az_estimated = F360_DEG2RAD(0.0F);

      sensors_angle_amb[0] = {true, {F360_DEG2RAD(60.F), INFTY, INFTY, INFTY, INFTY, INFTY}};
   }
};

/** \purpose
 * Is_Det_Object_Based_Angle_Jump should return false if detection is close to object's border (right side)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Object_Based_Angle_Jump, false_if_det_close_to_border_right_side)
{
   /** \precond
    * Default setup that makes target detection a TRUE angle jump
    * Change detection location close to the border of the object to the right adjacent lane
    */
   det_prop.vcs_position.y = 1.0F - calibs.obj_aj_border_half_width + floating_threshold;

   /** \action
    * Call Is_Det_Object_Based_Angle_Jump()
    */
   bool f_result = Is_Det_Object_Based_Angle_Jump(sensors[0], det_prop, det_raw, obj_velocity_vcs, det_restrictions, calibs, sensors_angle_amb[0].angle_ambiguity_candidates_rad);

   /** \result
    * False
    */
   CHECK_FALSE(f_result);
}

/** \purpose
 * Is_Det_Object_Based_Angle_Jump should return false if detection is close to object's border (left side)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Object_Based_Angle_Jump, false_if_det_close_to_border_left_side)
{
   /** \precond
    * Default setup that makes target detection a TRUE angle jump
    * Change detection location close to the border of the object to the left adjacent lane
    */
   det_restrictions.min_lat = -4.0F;
   det_restrictions.max_lat = -1.0F;
   det_prop.vcs_position.y = -1.0F + calibs.obj_aj_border_half_width - floating_threshold;

   /** \action
    * Call Is_Det_Object_Based_Angle_Jump()
    */
   bool f_result = Is_Det_Object_Based_Angle_Jump(sensors[0], det_prop, det_raw, obj_velocity_vcs, det_restrictions, calibs, sensors_angle_amb[0].angle_ambiguity_candidates_rad);

   /** \result
    * False
    */
   CHECK_FALSE(f_result);
}

/** \purpose
 * Is_Det_Object_Based_Angle_Jump should return false if detection is not valid on new azimuth position (after jump correction)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Object_Based_Angle_Jump, false_if_det_not_valid_on_new_y_pos)
{
   /** \precond
    * Default setup that makes target detection a TRUE angle jump
    * Change detection's raw azimuth that it does not falls into the reference bounding box after jump correction
    */
   det_raw.raw.azimuth = F360_DEG2RAD(40.0F);

   /** \action
    * Call Is_Det_Object_Based_Angle_Jump()
    */
   bool f_result = Is_Det_Object_Based_Angle_Jump(sensors[0], det_prop, det_raw, obj_velocity_vcs, det_restrictions, calibs, sensors_angle_amb[0].angle_ambiguity_candidates_rad);

   /** \result
    * False
    */
   CHECK_FALSE(f_result);
}

/** \purpose
 * Is_Det_Object_Based_Angle_Jump should return true if suspect range rate difference is lower than the threshold
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Object_Based_Angle_Jump, true_if_suspect_diff_lower_than_threshold)
{
   /** \precond
    * Default setup that makes target detection a TRUE angle jump
    */

   /** \action
    * Call Is_Det_Object_Based_Angle_Jump()
    */
   bool f_result = Is_Det_Object_Based_Angle_Jump(sensors[0], det_prop, det_raw, obj_velocity_vcs, det_restrictions, calibs, sensors_angle_amb[0].angle_ambiguity_candidates_rad);

   /** \result
    * True
    */
   CHECK_TRUE(f_result);
}

/** \purpose
 * Is_Det_Object_Based_Angle_Jump should return false if suspect range rate difference is higher than range rate difference
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Object_Based_Angle_Jump, true_if_suspect_diff_higher_than_origin)
{
   /** \precond
    * Default setup that makes target detection a TRUE angle jump
    * Change raw range rate that makes the angle jump range rate hypothesis not hold.
    */
   det_raw.raw.range_rate = 2.0F;

   /** \action
    * Call Is_Det_Object_Based_Angle_Jump()
    */
   bool f_result = Is_Det_Object_Based_Angle_Jump(sensors[0], det_prop, det_raw, obj_velocity_vcs, det_restrictions, calibs, sensors_angle_amb[0].angle_ambiguity_candidates_rad);

   /** \result
    * False
    */
   CHECK_FALSE(f_result);
}

/** \purpose
 * Check if Is_Det_Object_Based_Angle_Jump returns false given an angle jump candidate larger than PI.
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Object_Based_Angle_Jump, false_if_angle_jump_cand_invalid)
{
   /** \precond
    * Default setup that makes target detection a TRUE angle jump
    * Change an angle ambiguity candidate larger than PI which is invalid
    */
   sensors_angle_amb[0] = {true, {F360_DEG2RAD(361.F), INFTY, INFTY, INFTY, INFTY, INFTY}};

   /** \action
    * Call Is_Det_Object_Based_Angle_Jump()
    */
   bool f_result = Is_Det_Object_Based_Angle_Jump(sensors[0], det_prop, det_raw, obj_velocity_vcs, det_restrictions, calibs, sensors_angle_amb[0].angle_ambiguity_candidates_rad);

   /** \result
    * False
    */
   CHECK_FALSE(f_result);
}

/** \purpose
 * Check if Is_Det_Object_Based_Angle_Jump returns false after going through all 6 angle ambiguity candidates
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Object_Based_Angle_Jump, test_against_all_angle_amb_candidates)
{
   /** \precond
    * Default setup that makes target detection a TRUE angle jump given 60 deg angle jump
    * Fill 6 valid angle ambiguity candidates but with 0 deg angle shift
    */
   sensors_angle_amb[0] = {true, {F360_DEG2RAD(0.0F), F360_DEG2RAD(0.0F), F360_DEG2RAD(0.0F), F360_DEG2RAD(0.0F), F360_DEG2RAD(0.0F), F360_DEG2RAD(0.0F)}};

   /** \action
    * Call Is_Det_Object_Based_Angle_Jump()
    */
   bool f_result = Is_Det_Object_Based_Angle_Jump(sensors[0], det_prop, det_raw, obj_velocity_vcs, det_restrictions, calibs, sensors_angle_amb[0].angle_ambiguity_candidates_rad);

   /** \result
    * False
    */
   CHECK_FALSE(f_result);
}

/** \purpose
 * Check if Is_Det_Object_Based_Angle_Jump returns false for a detection with high azimuth confidence (equal to 0) due to the lower range rate threshold
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Object_Based_Angle_Jump, test_AJ_is_set_correctly_based_on_high_azimuth_confidence)
{
    /** \precond
     * Default setup that makes target detection a TRUE angle jump
     * Set the azimuth confidence equal to RSPP_CONF_AZIMUTH_HIGH
     * Set the raw range rate of the detection to 12
     * set the longtitudinal velocity to 6
     * set the v_wrapping of the first sensor to 10
     */
    det_raw.raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;
    det_raw.raw.range_rate = 12.0F;
    obj_velocity_vcs.longitudinal = 6.0F;
    sensors[0].constant.v_wrapping[sensors[0].variable.look_id] = 10.0F;

    /** \action
     * Call Is_Det_Object_Based_Angle_Jump()
     */
    bool f_result = Is_Det_Object_Based_Angle_Jump(sensors[0], det_prop, det_raw, obj_velocity_vcs, det_restrictions, calibs, sensors_angle_amb[0].angle_ambiguity_candidates_rad);

    /** \result
     * False
     */
    CHECK_FALSE(f_result);
}

/** \purpose
 * Check if Is_Det_Object_Based_Angle_Jump returns false for a detection with mid high azimuth confidence (equal to 1) due to the lower range rate threshold
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Object_Based_Angle_Jump, test_AJ_is_set_correctly_based_on_mid_high_azimuth_confidence)
{
    /** \precond
     * Default setup that makes target detection a TRUE angle jump
     * Set the azimuth confidence equal to RSPP_CONF_AZIMUTH_LOW
     * Set the raw range rate of the detection to 12
     * set the longtitudinal velocity to 6
     * set the v_wrapping of the first sensor to 10
     */
    det_raw.raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_MIDHIGH;
    det_raw.raw.range_rate = 12.0F;
    obj_velocity_vcs.longitudinal = 6.0F;
    sensors[0].constant.v_wrapping[sensors[0].variable.look_id] = 10.0;

    /** \action
     * Call Is_Det_Object_Based_Angle_Jump()
     */
    bool f_result = Is_Det_Object_Based_Angle_Jump(sensors[0], det_prop, det_raw, obj_velocity_vcs, det_restrictions, calibs, sensors_angle_amb[0].angle_ambiguity_candidates_rad);

    /** \result
     * False
     */
    CHECK_FALSE(f_result);
}

/** \purpose
 * Check if Is_Det_Object_Based_Angle_Jump returns true for a detection with mid low azimuth confidence (equal to 2) due to a higher range rate threshold
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Object_Based_Angle_Jump, test_AJ_is_set_correctly_based_on_mid_low_azimuth_confidence)
{
    /** \precond
     * Default setup that makes target detection a TRUE angle jump
     * Set the azimuth confidence equal to RSPP_CONF_AZIMUTH_LOW
     * Set the raw range rate of the detection to 12
     * set the longtitudinal velocity to 6
     * set the v_wrapping of the first sensor to 10
     */
    det_raw.raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_MIDLOW;
    det_raw.raw.range_rate = 12.0F;
    obj_velocity_vcs.longitudinal = 6.0F;
    sensors[0].constant.v_wrapping[sensors[0].variable.look_id] = 10.0;

    /** \action
     * Call Is_Det_Object_Based_Angle_Jump()
     */
    bool f_result = Is_Det_Object_Based_Angle_Jump(sensors[0], det_prop, det_raw, obj_velocity_vcs, det_restrictions, calibs, sensors_angle_amb[0].angle_ambiguity_candidates_rad);

    /** \result
     * False
     */
    CHECK_TRUE(f_result);
}

/** \purpose
 * Check if Is_Det_Object_Based_Angle_Jump returns true for a detection with low azimuth confidence (equal to 3) due to a higher range rate threshold
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Object_Based_Angle_Jump, test_AJ_is_set_correctly_based_on_low_azimuth_confidence)
{
    /** \precond
     * Default setup that makes target detection a TRUE angle jump
     * Set the azimuth confidence equal to RSPP_CONF_AZIMUTH_LOW
     * Set the raw range rate of the detection to 12
     * set the longtitudinal velocity to 6
     * set the v_wrapping of the first sensor to 10
     */
    det_raw.raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;
    det_raw.raw.range_rate = 12.0F;
    obj_velocity_vcs.longitudinal = 6.0F;
    sensors[0].constant.v_wrapping[sensors[0].variable.look_id] = 10.0;

    /** \action
     * Call Is_Det_Object_Based_Angle_Jump()
     */
    bool f_result = Is_Det_Object_Based_Angle_Jump(sensors[0], det_prop, det_raw, obj_velocity_vcs, det_restrictions, calibs, sensors_angle_amb[0].angle_ambiguity_candidates_rad);

    /** \result
     * False
     */
    CHECK_TRUE(f_result);
}
/** @}*/


/** \defgroup  f360_object_based_angle_jump_detector_internals__Calc_New_VCS_Aligned_Sensor_Azim
 *  @{
 */
 /** \brief
  * Test group for testing Calc_New_VCS_Aligned_Sensor_Azim()
  */
TEST_GROUP(f360_object_based_angle_jump_detector_internals__Calc_New_VCS_Aligned_Sensor_Azim)
{
   const float32_t floating_threshold = 0.0001F;

   float32_t jump_value;

   Sensor_Angle_Ambiguity_T sensors_angle_amb[MAX_NUMBER_OF_SENSORS];
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   float32_t jumped_az_vcs_vals[MAX_NUM_ANGLE_JUMPS] = {};

   /** \setup
   * Set jump value to 60 deg
   **/
   TEST_SETUP()
   {
      jump_value = F360_DEG2RAD(60.0F);
      sensors_angle_amb[0] = {true, {F360_DEG2RAD(60.0F), INFTY, INFTY, INFTY, INFTY, INFTY}};

      sensors[0].constant.polarity = 1.0F;
      sensors[0].variable.vacs_boresight_az_estimated = 0.0F;
   }
};

/** \purpose
 * Calc_New_VCS_Aligned_Sensor_Azim should return correct value if origin azimuth is 0
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Calc_New_VCS_Aligned_Sensor_Azim, calculation_correctness_zero_azimuth)
{
   float32_t prev_az_vcs = F360_DEG2RAD(0.0F);

   /** \action
    * Call Calc_New_VCS_Aligned_Sensor_Azim()
    */
   Calc_New_VCS_Aligned_Sensor_Azim(prev_az_vcs, sensors_angle_amb[0].angle_ambiguity_candidates_rad, sensors[0].constant.polarity, sensors[0].variable.vacs_boresight_az_estimated, jumped_az_vcs_vals);

   DOUBLES_EQUAL(prev_az_vcs + jump_value, jumped_az_vcs_vals[0], floating_threshold);
}

/** \purpose
 * Calc_New_VCS_Aligned_Sensor_Azim should return correct value if origin azimuth is betwen 0 and 90
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Calc_New_VCS_Aligned_Sensor_Azim, calculation_correctness_between_0_and_90)
{
   float32_t prev_az_vcs = F360_DEG2RAD(90.0F - floating_threshold);

   /** \action
    * Call Calc_New_VCS_Aligned_Sensor_Azim()
    */
   Calc_New_VCS_Aligned_Sensor_Azim(prev_az_vcs, sensors_angle_amb[0].angle_ambiguity_candidates_rad, sensors[0].constant.polarity, sensors[0].variable.vacs_boresight_az_estimated, jumped_az_vcs_vals);

   DOUBLES_EQUAL(prev_az_vcs + jump_value, jumped_az_vcs_vals[0], floating_threshold);
}

/** \purpose
 * Calc_New_VCS_Aligned_Sensor_Azim should return correct value if original azimuth is betwen -90 and 0
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Calc_New_VCS_Aligned_Sensor_Azim, calculation_correctness_between_neg_90_and_0)
{
   float32_t prev_az_vcs = F360_DEG2RAD(-90.0F + floating_threshold);

   /** \action
    * Call Calc_New_VCS_Aligned_Sensor_Azim()
    */
   Calc_New_VCS_Aligned_Sensor_Azim(prev_az_vcs, sensors_angle_amb[0].angle_ambiguity_candidates_rad, sensors[0].constant.polarity, sensors[0].variable.vacs_boresight_az_estimated, jumped_az_vcs_vals);

   DOUBLES_EQUAL(prev_az_vcs + jump_value, jumped_az_vcs_vals[0], floating_threshold);
}

/** \purpose
 * Calc_New_VCS_Aligned_Sensor_Azim should return correct value if origin azimuth is 90
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Calc_New_VCS_Aligned_Sensor_Azim, calculation_correctness_90)
{
   float32_t prev_az_vcs = F360_DEG2RAD(90.0F);

   /** \action
    * Call Calc_New_VCS_Aligned_Sensor_Azim()
    */
   Calc_New_VCS_Aligned_Sensor_Azim(prev_az_vcs, sensors_angle_amb[0].angle_ambiguity_candidates_rad, sensors[0].constant.polarity, sensors[0].variable.vacs_boresight_az_estimated, jumped_az_vcs_vals);

   DOUBLES_EQUAL(prev_az_vcs + jump_value, jumped_az_vcs_vals[0], floating_threshold);
}

/** \purpose
 * Calc_New_VCS_Aligned_Sensor_Azim should return correct value if origin azimuth is betwen 90 and 180
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Calc_New_VCS_Aligned_Sensor_Azim, calculation_correctness_between_90_and_180)
{
   float32_t prev_az_vcs = F360_DEG2RAD(120.0F);

   /** \action
    * Call Calc_New_VCS_Aligned_Sensor_Azim()
    */
   Calc_New_VCS_Aligned_Sensor_Azim(prev_az_vcs, sensors_angle_amb[0].angle_ambiguity_candidates_rad, sensors[0].constant.polarity, sensors[0].variable.vacs_boresight_az_estimated, jumped_az_vcs_vals);

   DOUBLES_EQUAL(prev_az_vcs + jump_value, jumped_az_vcs_vals[0], floating_threshold);
}

/** \purpose
 * Calc_New_VCS_Aligned_Sensor_Azim should return correct value if origin azimuth is betwen -180 and -90
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Calc_New_VCS_Aligned_Sensor_Azim, calculation_correctness_between_neg_90_and_neg_180)
{
   float32_t prev_az_vcs = F360_DEG2RAD(-120.0F);

   /** \action
    * Call Calc_New_VCS_Aligned_Sensor_Azim()
    */
   Calc_New_VCS_Aligned_Sensor_Azim(prev_az_vcs, sensors_angle_amb[0].angle_ambiguity_candidates_rad, sensors[0].constant.polarity, sensors[0].variable.vacs_boresight_az_estimated, jumped_az_vcs_vals);

   DOUBLES_EQUAL(prev_az_vcs + jump_value, jumped_az_vcs_vals[0], floating_threshold);
}

/** \purpose
 * Calc_New_VCS_Aligned_Sensor_Azim should return correct value if origin azimuth is -180
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Calc_New_VCS_Aligned_Sensor_Azim, calculation_correctness_between_neg_180)
{
   float32_t prev_az_vcs = F360_DEG2RAD(-180.0F);

   /** \action
    * Call Calc_New_VCS_Aligned_Sensor_Azim()
    */
   Calc_New_VCS_Aligned_Sensor_Azim(prev_az_vcs, sensors_angle_amb[0].angle_ambiguity_candidates_rad, sensors[0].constant.polarity, sensors[0].variable.vacs_boresight_az_estimated, jumped_az_vcs_vals);

   DOUBLES_EQUAL(prev_az_vcs + jump_value, jumped_az_vcs_vals[0], floating_threshold);
}
/** @}*/

/** \defgroup  f360_object_based_angle_jump_detector_internals__Is_Det_Inside_Restricted_AJ_Bbox
 *  @{
 */
 /** \brief
  * Test group for testing Is_Det_Inside_Restricted_AJ_Bbox()
  */
TEST_GROUP(f360_object_based_angle_jump_detector_internals__Is_Det_Inside_Restricted_AJ_Bbox)
{
   const float32_t floating_threshold = 0.0001F;

   Det_Restrictions_T det_restrictions {};

   /** \setup
   * Set bounding box
   **/
   TEST_SETUP()
   {
      det_restrictions.min_lat = 1.1F;
      det_restrictions.max_lat = 4.0F;
      det_restrictions.min_long = -3.2F;
      det_restrictions.max_long = 5.1F;
   }
};

/** \purpose
 * Calc_Range_Rate_Difference_From_Expectations should return true if detection is inside
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Inside_Restricted_AJ_Bbox, true_if_detection_inside)
{
   Point det_pos_vcs;
   det_pos_vcs.y = det_restrictions.min_lat + floating_threshold;
   det_pos_vcs.x = det_restrictions.max_long - floating_threshold;

   /** \action
    * Call Is_Det_Inside_Restricted_AJ_Bbox()
    */
   bool f_result = Is_Det_Inside_Restricted_AJ_Bbox(det_pos_vcs, det_restrictions);

   /** \result
    * True
    */
   CHECK_TRUE(f_result);
}

/** \purpose
 * Calc_Range_Rate_Difference_From_Expectations should return false if is outside (case 1)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Inside_Restricted_AJ_Bbox, false_if_detection_is_outsie_case_1)
{
   Point det_pos_vcs;
   det_pos_vcs.y = det_restrictions.min_lat - floating_threshold;
   det_pos_vcs.x = det_restrictions.max_long - floating_threshold;

   /** \action
    * Call Is_Det_Inside_Restricted_AJ_Bbox()
    */
   bool f_result = Is_Det_Inside_Restricted_AJ_Bbox(det_pos_vcs, det_restrictions);

   /** \result
    * False
    */
   CHECK_FALSE(f_result);
}

/** \purpose
 * Calc_Range_Rate_Difference_From_Expectations should return false if is outside (case 2)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Inside_Restricted_AJ_Bbox, false_if_detection_is_outsie_case_2)
{
   Point det_pos_vcs;
   det_pos_vcs.y = det_restrictions.max_lat + floating_threshold;
   det_pos_vcs.x = det_restrictions.max_long - floating_threshold;

   /** \action
    * Call Is_Det_Inside_Restricted_AJ_Bbox()
    */
   bool f_result = Is_Det_Inside_Restricted_AJ_Bbox(det_pos_vcs, det_restrictions);

   /** \result
    * False
    */
   CHECK_FALSE(f_result);
}

/** \purpose
 * Calc_Range_Rate_Difference_From_Expectations should return false if is outside (case 3)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Inside_Restricted_AJ_Bbox, false_if_detection_is_outsie_case_3)
{
   Point det_pos_vcs;
   det_pos_vcs.y = det_restrictions.min_lat + floating_threshold;
   det_pos_vcs.x = det_restrictions.max_long + floating_threshold;

   /** \action
    * Call Is_Det_Inside_Restricted_AJ_Bbox()
    */
   bool f_result = Is_Det_Inside_Restricted_AJ_Bbox(det_pos_vcs, det_restrictions);

   /** \result
    * False
    */
   CHECK_FALSE(f_result);
}

/** \purpose
 * Calc_Range_Rate_Difference_From_Expectations should return false if is outside (case 4)
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_Inside_Restricted_AJ_Bbox, false_if_detection_is_outsie_case_4)
{
   Point det_pos_vcs;
   det_pos_vcs.y = det_restrictions.min_lat + floating_threshold;
   det_pos_vcs.x = det_restrictions.min_long - floating_threshold;

   /** \action
    * Call Is_Det_Inside_Restricted_AJ_Bbox()
    */
   bool f_result = Is_Det_Inside_Restricted_AJ_Bbox(det_pos_vcs, det_restrictions);

   /** \result
    * False
    */
   CHECK_FALSE(f_result);
}
/** @}*/


/** \defgroup  f360_object_based_angle_jump_detector_internals__Is_Det_On_New_Azimuth_Valid
 *  @{
 */
 /** \brief
  * Test group for testing Is_Det_On_New_Azimuth_Valid()
  */
TEST_GROUP(f360_object_based_angle_jump_detector_internals__Is_Det_On_New_Azimuth_Valid)
{
   const float32_t floating_threshold = 0.0001F;

   Det_Restrictions_T det_restrictions {};
   Point sensor_mount_pos_vcs {};
   F360_Calibrations_T calibrations {};

   /** \setup
   * Set bounding box
   * Set sensor mount pos
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      det_restrictions.min_lat = 1.1F;
      det_restrictions.max_lat = 4.0F;
      det_restrictions.min_long = -8.2F;
      det_restrictions.max_long = 2.1F;

      //Rear right
      sensor_mount_pos_vcs.y = 0.9F;
      sensor_mount_pos_vcs.x = -3.5F;
   }
};

/** \purpose
 * Is_Det_On_New_Azimuth_Valid should return true if all conditions are met
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_On_New_Azimuth_Valid, true_if_all_conditions_met)
{
   const float32_t det_range = 3.0F;
   const Angle new_azimuth_vcs{ F360_DEG2RAD(90.0F) };

   /** \action
    * Call Is_Det_On_New_Azimuth_Valid()
    */
   bool f_result = Is_Det_On_New_Azimuth_Valid(det_range, new_azimuth_vcs, sensor_mount_pos_vcs, det_restrictions, calibrations.obj_aj_max_double_range_hypothesis);

   /** \result
    * True
    */
   CHECK_TRUE(f_result);
}

/** \purpose
 * Is_Det_On_New_Azimuth_Valid should return true if detection is on double distance
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_On_New_Azimuth_Valid, true_if_det_double_distance)
{
   const float32_t det_range = 6.0F;
   const Angle new_azimuth_vcs{ F360_DEG2RAD(90.0F) };

   /** \action
    * Call Is_Det_On_New_Azimuth_Valid()
    */
   bool f_result = Is_Det_On_New_Azimuth_Valid(det_range, new_azimuth_vcs, sensor_mount_pos_vcs, det_restrictions, calibrations.obj_aj_max_double_range_hypothesis);

   /** \result
    * True
    */
   CHECK_TRUE(f_result);
}

/** \purpose
 * Is_Det_On_New_Azimuth_Valid should return false if detection is too far away
 * \req
 *  NA.
 */
TEST(f360_object_based_angle_jump_detector_internals__Is_Det_On_New_Azimuth_Valid, false_det_too_far_away)
{
   const float32_t det_range = 15.0F;
   const Angle new_azimuth_vcs{ F360_DEG2RAD(90.0F) };

   /** \action
    * Call Is_Det_On_New_Azimuth_Valid()
    */
   bool f_result = Is_Det_On_New_Azimuth_Valid(det_range, new_azimuth_vcs, sensor_mount_pos_vcs, det_restrictions, calibrations.obj_aj_max_double_range_hypothesis);

   /** \result
    * False
    */
   CHECK_FALSE(f_result);
}
/** @}*/

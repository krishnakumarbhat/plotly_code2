/** \file
 * This file contains unit tests for content of f360_object_based_radar_phenomena_internals.cpp file
 */

#include "f360_object_based_radar_phenomena_internals.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_object_based_radar_phenomena_internals__Can_Object_Be_A_Reference
 *  @{
 */
 /** \brief
  * Test group for testing Can_Object_Be_A_Reference()
  */
TEST_GROUP(f360_object_based_radar_phenomena_internals__Can_Object_Be_A_Reference)
{
   const float32_t floating_threshold = 0.0001F;
   F360_Object_Track_T object_track;

   F360_Calibrations_T calibs;

   /** \setup
    * Set object status to F360_OBJECT_STATUS_UPDATED
    * Set object to be moving
    * Set object's pointing below threshold
    * Set object's lateral postion below threshold
    * Set calibs
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      object_track.status = F360_OBJECT_STATUS_UPDATED;
      object_track.f_moving = true;
      object_track.confidenceLevel = calibs.rp_min_confidence_level + floating_threshold;
      Point center = {0.0F, calibs.rp_max_object_lateral_distance - floating_threshold};
      object_track.bbox.Set_Center(center);
      object_track.bbox.Set_Orientation(Angle{ calibs.rp_max_abs_pointing_disagreement - floating_threshold });
      object_track.bbox.Set_Length(5.0F);
      object_track.bbox.Set_Width(2.0F);
      object_track.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object_track.vcs_position = object_track.bbox.Get_Corners().Rear_Left();
   }
};

/** \purpose
 * Can_Object_Be_A_Reference should return true if object meets all condtions, case 1
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Can_Object_Be_A_Reference, object_meets_all_conditions_to_be_valid_case_1)
{
   /** \precond
    * Same as setup
    */

   /** \action
    * Call Can_Object_Be_A_Reference()
    */
   bool result = Can_Object_Be_A_Reference(object_track, calibs.rp_max_object_lateral_distance, calibs.rp_max_abs_pointing_disagreement, calibs.rp_min_confidence_level);

   /** \result
    * Check if true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Can_Object_Be_A_Reference should return false if object meets all condtions except for mirror probability greater than 0
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Can_Object_Be_A_Reference, object_mirror_probability_greater_than_0)
{
   /** \precond
    * Same as setup except for the mirror probability greater than 0
    */
   object_track.mirror_prob = 0.1F;

   /** \action
    * Call Can_Object_Be_A_Reference()
    */
   bool result = Can_Object_Be_A_Reference(object_track, calibs.rp_max_object_lateral_distance, calibs.rp_max_abs_pointing_disagreement, calibs.rp_min_confidence_level);

   /** \result
    * Check if false
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Can_Object_Be_A_Reference should return true if object meets all condtions, case 2
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Can_Object_Be_A_Reference, object_meets_all_conditions_to_be_valid_case_2)
{
   /** \precond
   * Same as setup + below changes:
   * Set object's lateral postion within threshold (just above negative limit)
    */
   Point center = {object_track.bbox.Get_Center().x, -calibs.rp_max_object_lateral_distance + floating_threshold};
   object_track.bbox.Set_Center(center);
   object_track.reference_point = F360_REFERENCE_POINT_RIGHT;
   object_track.vcs_position = {-0.17355F, -7.0151F};

   /** \action
    * Call Can_Object_Be_A_Reference()
    */
   bool result = Can_Object_Be_A_Reference(object_track, calibs.rp_max_object_lateral_distance, calibs.rp_max_abs_pointing_disagreement, calibs.rp_min_confidence_level);

   /** \result
    * Check if true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Can_Object_Be_A_Reference should return true if object meets all condtions, case 3
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Can_Object_Be_A_Reference, object_meets_all_conditions_to_be_valid_case_3)
{
   /** \precond
   * Same as setup + below changes:
   * Set object's pointing within threshold (just above negative limit)
    */
   object_track.Set_Bbox_Orientation(Angle{ -calibs.rp_max_abs_pointing_disagreement + floating_threshold });

   /** \action
    * Call Can_Object_Be_A_Reference()
    */
   bool result = Can_Object_Be_A_Reference(object_track, calibs.rp_max_object_lateral_distance, calibs.rp_max_abs_pointing_disagreement, calibs.rp_min_confidence_level);

   /** \result
    * Check if true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Can_Object_Be_A_Reference should return true if object meets all condtions, case 4
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Can_Object_Be_A_Reference, object_meets_all_conditions_to_be_valid_case_4)
{
   /** \precond
   * Same as setup + below changes:
   * Set object's laterla position within threshold (just above negative limit)
   * Set object's pointing within threshold (just above negative limit)
    */
   object_track.Set_Bbox_Orientation(Angle{ -calibs.rp_max_abs_pointing_disagreement + floating_threshold });
   Point center = {object_track.bbox.Get_Center().x, -calibs.rp_max_object_lateral_distance + floating_threshold};
   object_track.bbox.Set_Center(center);
   object_track.reference_point = F360_REFERENCE_POINT_RIGHT;
   object_track.vcs_position = {0.17355F, -7.0151F};

   /** \action
    * Call Can_Object_Be_A_Reference()
    */
   bool result = Can_Object_Be_A_Reference(object_track, calibs.rp_max_object_lateral_distance, calibs.rp_max_abs_pointing_disagreement, calibs.rp_min_confidence_level);

   /** \result
    * Check if true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Can_Object_Be_A_Reference should return false if object is invalid
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Can_Object_Be_A_Reference, object_is_not_valid_due_to_being_invalid)
{
   /** \precond
    * Same as setup + below changes:
    * Set object status to INVALID
    */
   object_track.status = F360_OBJECT_STATUS_INVALID;

   /** \action
    * Call Can_Object_Be_A_Reference()
    */
   bool result = Can_Object_Be_A_Reference(object_track, calibs.rp_max_object_lateral_distance, calibs.rp_max_abs_pointing_disagreement, calibs.rp_min_confidence_level);

   /** \result
    * Check if false
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Can_Object_Be_A_Reference should return false if object not moving
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Can_Object_Be_A_Reference, object_is_not_valid_due_to_not_being_moving)
{
   /** \precond
   * Same as setup + below changes:
    * Set object to be stationary
    */
   object_track.f_moving = false;

   /** \action
    * Call Can_Object_Be_A_Reference()
    */
   bool result = Can_Object_Be_A_Reference(object_track, calibs.rp_max_object_lateral_distance, calibs.rp_max_abs_pointing_disagreement, calibs.rp_min_confidence_level);

   /** \result
    * Check if false
    */
   CHECK_FALSE(result);
}

/** \purpose
   * Can_Object_Be_A_Reference should return false if object's pointing is too high (positive)
   * \req
   *  NA.
   */
TEST(f360_object_based_radar_phenomena_internals__Can_Object_Be_A_Reference, object_is_not_valid_due_too_high_pointing_positive)
{
   /** \precond
   * Same as setup + below changes:
   * Set object's pointing above threshold
   */
   object_track.Set_Bbox_Orientation(Angle{ calibs.rp_max_abs_pointing_disagreement + floating_threshold });

   /** \action
   * Call Can_Object_Be_A_Reference()
   */
   bool result = Can_Object_Be_A_Reference(object_track, calibs.rp_max_object_lateral_distance, calibs.rp_max_abs_pointing_disagreement, calibs.rp_min_confidence_level);

   /** \result
   * Check if false
   */
   CHECK_FALSE(result);
}


/** \purpose
   * Can_Object_Be_A_Reference should return false if object's pointing is too high (negative)
   * \req
   *  NA.
   */
TEST(f360_object_based_radar_phenomena_internals__Can_Object_Be_A_Reference, object_is_not_valid_due_too_high_pointing_negative)
{
   /** \precond
   * Same as setup + below changes:
   * Set object's pointing above threshold
   */
   object_track.Set_Bbox_Orientation(Angle{ -calibs.rp_max_abs_pointing_disagreement - floating_threshold });

   /** \action
   * Call Can_Object_Be_A_Reference()
   */
   bool result = Can_Object_Be_A_Reference(object_track, calibs.rp_max_object_lateral_distance, calibs.rp_max_abs_pointing_disagreement, calibs.rp_min_confidence_level);

   /** \result
   * Check if false
   */
   CHECK_FALSE(result);
}

/** \purpose
   * Can_Object_Be_A_Reference should return false if object's lateral postion is too high (positive)
   * \req
   *  NA.
   */
TEST(f360_object_based_radar_phenomena_internals__Can_Object_Be_A_Reference, object_is_not_valid_due_too_high_lateral_position_positive)
{
   /** \precond
   * Same as setup + below changes:
   * Set object's lateral postion above threshold
   */
   Point center = {object_track.bbox.Get_Center().x, calibs.rp_max_object_lateral_distance + floating_threshold};
   object_track.bbox.Set_Center(center);
   object_track.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   object_track.vcs_position = object_track.bbox.Get_Corners().Rear_Left();

   /** \action
   * Call Can_Object_Be_A_Reference()
   */
   bool result = Can_Object_Be_A_Reference(object_track, calibs.rp_max_object_lateral_distance, calibs.rp_max_abs_pointing_disagreement, calibs.rp_min_confidence_level);

   /** \result
   * Check if false
   */
   CHECK_FALSE(result);
}

/** \purpose
   * Can_Object_Be_A_Reference should return false if object's lateral postion is too high (negative)
   * \req
   *  NA.
   */
TEST(f360_object_based_radar_phenomena_internals__Can_Object_Be_A_Reference, object_is_not_valid_due_too_high_lateral_position_negative)
{
   /** \precond
   * Same as setup + below changes:
   * Set object's lateral postion above threshold
   */
   Point center = {object_track.bbox.Get_Center().x, -(calibs.rp_max_object_lateral_distance + floating_threshold)};
   object_track.bbox.Set_Center(center);
   object_track.reference_point = F360_REFERENCE_POINT_RIGHT;
   object_track.vcs_position = {-0.17355F, -7.0153F};

   /** \action
   * Call Can_Object_Be_A_Reference()
   */
   bool result = Can_Object_Be_A_Reference(object_track, calibs.rp_max_object_lateral_distance, calibs.rp_max_abs_pointing_disagreement, calibs.rp_min_confidence_level);

   /** \result
   * Check if false
   */
   CHECK_FALSE(result);
}

/** \purpose
   * Can_Object_Be_A_Reference should return false if object's confidence level is too low
   * \req
   *  NA.
   */
TEST(f360_object_based_radar_phenomena_internals__Can_Object_Be_A_Reference, object_is_not_valid_due_too_low_confidence_level)
{
   /** \precond
   * Same as setup + below changes:
   * Set object's confidence level equal to threshold
   */
   object_track.confidenceLevel = calibs.rp_min_confidence_level - 0.1F;

   /** \action
   * Call Can_Object_Be_A_Reference()
   */
   bool result = Can_Object_Be_A_Reference(object_track, calibs.rp_max_object_lateral_distance, calibs.rp_max_abs_pointing_disagreement, calibs.rp_min_confidence_level);

   /** \result
   * Check if false
   */
   CHECK_FALSE(result);
}
/** @}*/

/** \defgroup  f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce
 *  @{
 */
 /** \brief
  * Test group for testing Determine_Precond_For_Angle_Jump_And_MultiBounce()
  */
TEST_GROUP(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce)
{
   const float32_t floating_threshold = 0.0001F;

   F360_Object_Track_T object_track{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};

   Sensor_Angle_Ambiguity_T sensors_angle_amb[MAX_NUMBER_OF_SENSORS];
   bool relevant_sensors_for_multi_bounce[MAX_NUMBER_OF_SENSORS] = {};
   const float32_t longitudinal_margin = 0.0F;

   /** \setup
   * 
   **/
   TEST_SETUP()
   {
      sensors[0].variable.is_valid = true;
      sensors[0].constant.mounting_position.vcs_position = { 0.0F, 1.0F, 0.0F };
      sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
      sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_RADAR;
      object_track.reference_point = F360_REFERENCE_POINT_CENTER;
      object_track.vcs_position = { -5.0F, 3.0F };
      object_track.bbox.Set_Center(object_track.vcs_position);
      object_track.bbox.Set_Orientation(Angle(0.0F));
      object_track.bbox.Set_Length(10.0F + floating_threshold);
      object_track.bbox.Set_Width(2.0F);
   }
};

/** \purpose
 * Determine_Precond_For_Angle_Jump_And_MultiBounce should mark one of the sensors as relevant given that all conditions are met and sensor is of type FLR7
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce, all_conditions_Met_FLR7)
{
   /** \precond
    * Same as setup
    */

   /** \action
    * Call Determine_Precond_For_Angle_Jump_And_MultiBounce()
    */
   Determine_Precond_For_Angle_Jump_And_MultiBounce(sensors[0], 0, object_track.bbox, longitudinal_margin, relevant_sensors_for_multi_bounce, sensors_angle_amb);

   /** \result
    * Check if only first sensor is relevant for both angle jump and multibounce
    */
   for (int i = 1; i < MAX_NUMBER_OF_SENSORS;i++)
   {
      CHECK_FALSE(relevant_sensors_for_multi_bounce[i]);
      CHECK_FALSE(sensors_angle_amb[i].f_sensor_relevant);
   }
   CHECK_TRUE(relevant_sensors_for_multi_bounce[0]);
   CHECK_TRUE(sensors_angle_amb[0].f_sensor_relevant);
}

/** \purpose
 * Determine_Precond_For_Angle_Jump_And_MultiBounce should mark one of the sensors as relevant given that all conditions are met and sensor is of type FLR7.
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce, all_conditions_Met_FLR7_PLT)
{
   /** \precond
    * Same as setup
    * Set sensor type as F360_SENSOR_TYPE_FLR7_PLT_RADAR
    */
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_PLT_RADAR;

   /** \action
    * Call Determine_Precond_For_Angle_Jump_And_MultiBounce()
    */
   Determine_Precond_For_Angle_Jump_And_MultiBounce(sensors[0], 0, object_track.bbox, longitudinal_margin, relevant_sensors_for_multi_bounce, sensors_angle_amb);

   /** \result
    * Check if only first sensor is relevant for both angle jump and multibounce
    */
   for (int i = 1; i < MAX_NUMBER_OF_SENSORS;i++)
   {
      CHECK_FALSE(relevant_sensors_for_multi_bounce[i]);
      CHECK_FALSE(sensors_angle_amb[i].f_sensor_relevant);
   }
   CHECK_TRUE(relevant_sensors_for_multi_bounce[0]);
   CHECK_TRUE(sensors_angle_amb[0].f_sensor_relevant);
}

/** \purpose
 * Determine_Precond_For_Angle_Jump_And_MultiBounce should mark one of the sensors as relevant given that all conditions are met and sensor is of type FLR7v2.
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce, all_conditions_Met_FLR7v2)
{
   /** \precond
    * Same as setup
    * Set sensor type as F360_SENSOR_TYPE_FLR7v2_RADAR
    */
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR;

   /** \action
    * Call Determine_Precond_For_Angle_Jump_And_MultiBounce()
    */
   Determine_Precond_For_Angle_Jump_And_MultiBounce(sensors[0], 0, object_track.bbox, longitudinal_margin, relevant_sensors_for_multi_bounce, sensors_angle_amb);

   /** \result
    * Check if only first sensor is relevant for both angle jump and multibounce
    */
   for (int i = 1; i < MAX_NUMBER_OF_SENSORS;i++)
   {
      CHECK_FALSE(relevant_sensors_for_multi_bounce[i]);
      CHECK_FALSE(sensors_angle_amb[i].f_sensor_relevant);
   }
   CHECK_TRUE(relevant_sensors_for_multi_bounce[0]);
   CHECK_TRUE(sensors_angle_amb[0].f_sensor_relevant);
}

/** \purpose
 * Determine_Precond_For_Angle_Jump_And_MultiBounce should mark one of the sensors as relevant given that sensor is mounted left side 1.
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce, all_conditions_Met_and_left_side1_sensor)
{
   /** \precond
    * Set location enum to F360_MOUNTING_LOCATION_LEFT_SIDE1
    */
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_SIDE1;

    /** \action
     * Call Determine_Precond_For_Angle_Jump_And_MultiBounce()
     */
   Determine_Precond_For_Angle_Jump_And_MultiBounce(sensors[0], 0, object_track.bbox, longitudinal_margin, relevant_sensors_for_multi_bounce, sensors_angle_amb);
   /** \result
    * Check if only first sensor is relevant
    */
   for (int i = 1; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      CHECK_FALSE(relevant_sensors_for_multi_bounce[i]);
      CHECK_FALSE(sensors_angle_amb[i].f_sensor_relevant);
   }
   CHECK_TRUE(relevant_sensors_for_multi_bounce[0]);
   CHECK_TRUE(sensors_angle_amb[0].f_sensor_relevant);
}

/** \purpose
 * Determine_Precond_For_Angle_Jump_And_MultiBounce should mark one of the sensors as relevant given that sensor is mounted rear left
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce, all_conditions_Met_and_rear_left_sensor)
{
   /** \precond
    * Set location enum to F360_MOUNTING_LOCATION_LEFT_REAR 
    */
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;

   /** \action
    * Call Determine_Precond_For_Angle_Jump_And_MultiBounce()
    */
   Determine_Precond_For_Angle_Jump_And_MultiBounce(sensors[0], 0, object_track.bbox, longitudinal_margin, relevant_sensors_for_multi_bounce, sensors_angle_amb);

   /** \result
    * Check if only first sensor is relevant
    */
   for (int i = 1; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      CHECK_FALSE(relevant_sensors_for_multi_bounce[i]);
      CHECK_FALSE(sensors_angle_amb[i].f_sensor_relevant);
   }
   CHECK_TRUE(relevant_sensors_for_multi_bounce[0]);
   CHECK_TRUE(sensors_angle_amb[0].f_sensor_relevant);
}

/** \purpose
 * Determine_Precond_For_Angle_Jump_And_MultiBounce should mark one of the sensors as relevant given that sensor is mounted left side 2.
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce, all_conditions_Met_and_left_side2_sensor)
{
   /** \precond
    * Set location enum to F360_MOUNTING_LOCATION_LEFT_SIDE2
    */
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_SIDE2;

   /** \action
    * Call Determine_Precond_For_Angle_Jump_And_MultiBounce()
    */
   Determine_Precond_For_Angle_Jump_And_MultiBounce(sensors[0], 0, object_track.bbox, longitudinal_margin, relevant_sensors_for_multi_bounce, sensors_angle_amb);

   /** \result
    * Check if only first sensor is relevant
    */
   for (int i = 1; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      CHECK_FALSE(relevant_sensors_for_multi_bounce[i]);
      CHECK_FALSE(sensors_angle_amb[i].f_sensor_relevant);
   }
   CHECK_TRUE(relevant_sensors_for_multi_bounce[0]);
   CHECK_TRUE(sensors_angle_amb[0].f_sensor_relevant);
}

/** \purpose
 * Determine_Precond_For_Angle_Jump_And_MultiBounce should mark one of the sensors as relevant given that sensor is mounted right forward.
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce, all_conditions_Met_and_right_forward_sensor)
{
   /** \precond
    * Set location enum to F360_MOUNTING_LOCATION_RIGHT_FORWARD 
    */
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;

   /** \action
    * Call Determine_Precond_For_Angle_Jump_And_MultiBounce()
    */
   Determine_Precond_For_Angle_Jump_And_MultiBounce(sensors[0], 0, object_track.bbox, longitudinal_margin, relevant_sensors_for_multi_bounce, sensors_angle_amb);

   /** \result
    * Check if only first sensor is relevant
    */
   for (int i = 1; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      CHECK_FALSE(relevant_sensors_for_multi_bounce[i]);
      CHECK_FALSE(sensors_angle_amb[i].f_sensor_relevant);
   }
   CHECK_TRUE(relevant_sensors_for_multi_bounce[0]);
   CHECK_TRUE(sensors_angle_amb[0].f_sensor_relevant);
}

/** \purpose
 * Determine_Precond_For_Angle_Jump_And_MultiBounce should mark one of the sensors as relevant given that sensor is mounted rear side 1.
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce, all_conditions_Met_and_right_side1_sensor)
{
   /** \precond
    * Set location enum to F360_MOUNTING_LOCATION_RIGHT_SIDE1 
    */
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_SIDE1;

   /** \action
    * Call Determine_Precond_For_Angle_Jump_And_MultiBounce()
    */
   Determine_Precond_For_Angle_Jump_And_MultiBounce(sensors[0], 0, object_track.bbox, longitudinal_margin, relevant_sensors_for_multi_bounce, sensors_angle_amb);

   /** \result
    * Check if only first sensor is relevant
    */
   for (int i = 1; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      CHECK_FALSE(relevant_sensors_for_multi_bounce[i]);
      CHECK_FALSE(sensors_angle_amb[i].f_sensor_relevant);
   }
   CHECK_TRUE(relevant_sensors_for_multi_bounce[0]);
   CHECK_TRUE(sensors_angle_amb[0].f_sensor_relevant);
}

/** \purpose
 * Determine_Precond_For_Angle_Jump_And_MultiBounce should mark one of the sensors as relevant given that sensor is mounted rear side 2.
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce, all_conditions_Met_and_right_side2_sensor)
{
   /** \precond
    * Set location enum to F360_MOUNTING_LOCATION_RIGHT_SIDE2 
    */
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_SIDE2;

   /** \action
    * Call Determine_Precond_For_Angle_Jump_And_MultiBounce()
    */
   Determine_Precond_For_Angle_Jump_And_MultiBounce(sensors[0], 0, object_track.bbox, longitudinal_margin, relevant_sensors_for_multi_bounce, sensors_angle_amb);

   /** \result
    * Check if only first sensor is relevant
    */
   for (int i = 1; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      CHECK_FALSE(relevant_sensors_for_multi_bounce[i]);
      CHECK_FALSE(sensors_angle_amb[i].f_sensor_relevant);
   }
   CHECK_TRUE(relevant_sensors_for_multi_bounce[0]);
   CHECK_TRUE(sensors_angle_amb[0].f_sensor_relevant);
}

/** \purpose
 * Determine_Precond_For_Angle_Jump_And_MultiBounce should mark one of the sensors as relevant given that sensor is mounted rear right.
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce, all_conditions_Met_and_rear_right_sensor)
{
   /** \precond
    * Set location enum to F360_MOUNTING_LOCATION_RIGHT_REAR 
    */
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;

   /** \action
    * Call Determine_Precond_For_Angle_Jump_And_MultiBounce()
    */
   Determine_Precond_For_Angle_Jump_And_MultiBounce(sensors[0], 0, object_track.bbox, longitudinal_margin, relevant_sensors_for_multi_bounce, sensors_angle_amb);

   /** \result
    * Check if only first sensor is relevant
    */
   for (int i = 1; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      CHECK_FALSE(relevant_sensors_for_multi_bounce[i]);
      CHECK_FALSE(sensors_angle_amb[i].f_sensor_relevant);
   }
   CHECK_TRUE(relevant_sensors_for_multi_bounce[0]);
   CHECK_TRUE(sensors_angle_amb[0].f_sensor_relevant);
}

/** \purpose
 * Determine_Precond_For_Angle_Jump_And_MultiBounce should not mark any sensors as relevant for multi bounce due to sensor being forward
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce, not_marked_due_to_sensor_being_forward)
{
   /** \precond
    * Set location enum to F360_MOUNTING_LOCATION_CENTER_FORWARD
    */
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;

   /** \action
    * Call Determine_Precond_For_Angle_Jump_And_MultiBounce()
    */
   Determine_Precond_For_Angle_Jump_And_MultiBounce(sensors[0], 0, object_track.bbox, longitudinal_margin, relevant_sensors_for_multi_bounce, sensors_angle_amb);

   /** \result
    * Check if there is no relevant sensor
    */
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      CHECK_FALSE(relevant_sensors_for_multi_bounce[i]);
   }
}

/** \purpose
 * Determine_Precond_For_Angle_Jump_And_MultiBounce should not mark any sensors as relevant for multi bounce due to that object is too far away from front bumper
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce, not_marked_due_to_object_is_to_far_away__front_bumper)
{
   /** \precond
    * Set object longitudinal position to -5.0F
    */
   Point center = {-5.0F - floating_threshold, object_track.bbox.Get_Center().y};
   object_track.bbox.Set_Center(center);


   /** \action
    * Call Determine_Precond_For_Angle_Jump_And_MultiBounce()
    */
   Determine_Precond_For_Angle_Jump_And_MultiBounce(sensors[0], 0, object_track.bbox, longitudinal_margin, relevant_sensors_for_multi_bounce, sensors_angle_amb);

   /** \result
    * Check if there is no relevant sensor
    */
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      CHECK_FALSE(relevant_sensors_for_multi_bounce[i]);
   }
}

/** \purpose
 * Determine_Precond_For_Angle_Jump_And_MultiBounce should not mark any sensors as relevant for multi bounce due to that object is too far away from rear bumper
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce, not_marked_due_to_object_is_to_far_away__rear_bumper)
{
   /** \precond
    * Set object longitudinal position to 5.0F
    */
   Point center = {5.0F + floating_threshold, object_track.bbox.Get_Center().y};
   object_track.bbox.Set_Center(center);

   /** \action
    * Call Determine_Precond_For_Angle_Jump_And_MultiBounce()
    */
   Determine_Precond_For_Angle_Jump_And_MultiBounce(sensors[0], 0, object_track.bbox, longitudinal_margin, relevant_sensors_for_multi_bounce, sensors_angle_amb);

   /** \result
    * Check if there is no relevant sensor
    */
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      CHECK_FALSE(relevant_sensors_for_multi_bounce[i]);
   }
}


/** \purpose
 * Determine_Precond_For_Angle_Jump_And_MultiBounce should not mark any sensors as relevant for multi bounce
 * because object is on opposite side wrt. sensor
 * \req
 *  NA.
 */
TEST(f360_object_based_radar_phenomena_internals__Determine_Precond_For_Angle_Jump_And_MultiBounce, not_marked_due_to_object_is_on_different_side_than_sensor)
{
   /** \precond
    * Set object lateral position to negative value
    */
   Point center = {object_track.bbox.Get_Center().x, -object_track.bbox.Get_Center().y};
   object_track.bbox.Set_Center(center);

   /** \action
    * Call Determine_Precond_For_Angle_Jump_And_MultiBounce()
    */
   Determine_Precond_For_Angle_Jump_And_MultiBounce(sensors[0], 0, object_track.bbox, longitudinal_margin, relevant_sensors_for_multi_bounce, sensors_angle_amb);

   /** \result
    * Check if there is no relevant sensor
    */
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      CHECK_FALSE(relevant_sensors_for_multi_bounce[i]);
   }
}
/** @}*/

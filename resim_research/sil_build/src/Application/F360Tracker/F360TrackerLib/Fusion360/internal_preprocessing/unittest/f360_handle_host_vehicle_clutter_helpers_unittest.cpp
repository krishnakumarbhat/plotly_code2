/** \file
 * This file contains unit tests for content of f360_handle_host_vehicle_clutter_helpers.cpp file
 */

#include "f360_handle_host_vehicle_clutter_helpers.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  is_det_in_hvc_tp_area
 *  @{
 */

 /** \brief
  * Test group of Is_Det_In_HVC_TP_Area. Tests verify whether function properly distinguishes whether detection is
  * or is not placed inside TP area.
  */
TEST_GROUP(is_det_in_hvc_tp_area)
{
   Point vcs_pos;
   Pos_Limits_T hvc_tp_area;

   /** \setup
    * Set area parameters
    */
   TEST_SETUP()
   {
      hvc_tp_area.max_lat_pos = 2.0F;
      hvc_tp_area.min_lat_pos = -2.0F;

      hvc_tp_area.min_long_pos = -2.0F;
      hvc_tp_area.max_long_pos = 2.0F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether function returns true if detection is placed inside area.
 * \req
 * NA.
 */
TEST(is_det_in_hvc_tp_area, is_det_in_hvc_tp_area__det_is_in_zone)
{
   /** \precond
    * Set detection position to be placed inside zone.
    */
   vcs_pos.y = 0.0F;
   vcs_pos.x = 0.0F;

   /** \action
    * Call tested function
    */
   const bool f_is_inside = Is_Det_In_HVC_TP_Area(vcs_pos, hvc_tp_area);

   /** \result
    * Check whether returned value is true.
    */
   CHECK_TRUE(f_is_inside);
}

/** \purpose
* Purpose of this test is to verify whether function returns false if detection lateral position is too low.
* \req
* NA.
*/
TEST(is_det_in_hvc_tp_area, is_det_in_hvc_tp_area__lat_pos_too_low)
{
   /** \precond
   * Set detection position to have too low lateral position
   */
   vcs_pos.y = hvc_tp_area.min_lat_pos - 0.1F;
   vcs_pos.x = 0.0F;

   /** \action
   * Call tested function
   */
   const bool f_is_inside = Is_Det_In_HVC_TP_Area(vcs_pos, hvc_tp_area);

   /** \result
   * Check whether returned value is false.
   */
   CHECK_FALSE(f_is_inside);
}

/** \purpose
* Purpose of this test is to verify whether function returns false if detection lateral position is too high.
* \req
* NA.
*/
TEST(is_det_in_hvc_tp_area, is_det_in_hvc_tp_area__lat_pos_too_high)
{
   /** \precond
   * Set detection position to have too high lateral position
   */
   vcs_pos.y = hvc_tp_area.max_lat_pos + 0.1F;
   vcs_pos.x = 0.0F;

   /** \action
   * Call tested function
   */
   const bool f_is_inside = Is_Det_In_HVC_TP_Area(vcs_pos, hvc_tp_area);

   /** \result
   * Check whether returned value is false.
   */
   CHECK_FALSE(f_is_inside);
}

/** \purpose
* Purpose of this test is to verify whether function returns false if detection longitudinal position is too high.
* \req
* NA.
*/
TEST(is_det_in_hvc_tp_area, is_det_in_hvc_tp_area__long_pos_too_high)
{
   /** \precond
   * Set detection position to have too high longitudinal position
   */
   vcs_pos.y = 0.0F;
   vcs_pos.x = hvc_tp_area.max_long_pos + 0.1F;

   /** \action
   * Call tested function
   */
   const bool f_is_inside = Is_Det_In_HVC_TP_Area(vcs_pos, hvc_tp_area);

   /** \result
   * Check whether returned value is false.
   */
   CHECK_FALSE(f_is_inside);
}

/** \purpose
* Purpose of this test is to verify whether function returns false if detection longitudinal position is too low.
* \req
* NA.
*/
TEST(is_det_in_hvc_tp_area, is_det_in_hvc_tp_area__long_pos_too_low)
{
   /** \precond
   * Set detection position to have too low longitudinal position
   */
   vcs_pos.y = 0.0F;
   vcs_pos.x = hvc_tp_area.min_long_pos - 0.1F;

   /** \action
   * Call tested function
   */
   const bool f_is_inside = Is_Det_In_HVC_TP_Area(vcs_pos, hvc_tp_area);

   /** \result
   * Check whether returned value is false.
   */
   CHECK_FALSE(f_is_inside);
}
/** @}*/


/** \defgroup  get_hvc_tp_area
 *  @{
 */

 /** \brief
  * Test group of Get_HVC_TP_Area. Tests verify whether function properly extracts minimal and maximal positions of sensors.
  */
TEST_GROUP(get_hvc_tp_area)
{
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   float32_t max_range_flagging_hvc_dets{};
   float32_t zone_width = 0.5F;

   /** \setup
   * Set four sensors positions
   */
   TEST_SETUP()
   {
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
* Purpose of this test is to verify whether there are no valid sensors function
* returns zone defined by calibration value.
* \req
* NA.
*/
TEST(get_hvc_tp_area, get_hvc_tp_area__no_valid_sensors)
{
   /** \precond
   * Set sensors is_valid flag to false
   */
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      sensors[i].variable.is_valid = false;
   }

   /** \action
   * Call tested function
   */
   const Pos_Limits_T zone = Get_HVC_TP_Area(sensors, zone_width);

   /** \result
   * Check whether returned values are equal to default width.
   */
   CHECK_EQUAL(-zone_width, zone.min_lat_pos);
   CHECK_EQUAL(zone_width, zone.max_lat_pos);
   CHECK_EQUAL(-zone_width, zone.min_long_pos);
   CHECK_EQUAL(zone_width, zone.max_long_pos);
}

/** \purpose
* Purpose of this test is to verify whether there are valid sensors,
* function picks proper values.
* \req
* NA.
*/
TEST(get_hvc_tp_area, get_hvc_tp_area__valid_values_are_picked)
{
   /** \precond
   * Set 4 sensors is_valid flag to true
   */
   for (int i = 0; i < 4; i++)
   {
      sensors[i].variable.is_valid = true;
   }

   /** \action
   * Call tested function
   */
   const Pos_Limits_T zone = Get_HVC_TP_Area(sensors, zone_width);

   /** \result
   * Check whether returned values are equal to default width.
   */
   DOUBLES_EQUAL(-2.5F, zone.min_lat_pos, F360_EPSILON);
   DOUBLES_EQUAL(3.5F, zone.max_lat_pos, F360_EPSILON);
   DOUBLES_EQUAL(-2.5F, zone.min_long_pos, F360_EPSILON);
   DOUBLES_EQUAL(2.5F, zone.max_long_pos, F360_EPSILON);
}
/** @}*/

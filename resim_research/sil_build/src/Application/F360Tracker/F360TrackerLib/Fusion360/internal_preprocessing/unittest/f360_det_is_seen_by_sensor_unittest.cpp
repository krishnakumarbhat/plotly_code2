/** \file
 * This file contains unit tests for content of f360_det_is_seen_by_sensor.cpp file
 */

#include "f360_det_is_seen_by_sensor.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_det_is_seen_by_sensor
 *  @{
 */

/** \brief
 * Purpose of this test is to verify whether function properly determines whether object is seen by sensor.
 */
TEST_GROUP(f360_det_is_seen_by_sensor)
{
   Point det_pos{};
   F360_Radar_Sensor_Props_T radar_sensor_props{};
   float32_t lat_th_ext = 0.3F;

   TEST_SETUP()
   {
      radar_sensor_props.next_to_sensor_object_track_min_lat_pos = 4.0F;
      radar_sensor_props.next_to_sensor_object_track_max_lat_pos = 4.0F;
      radar_sensor_props.f_object_track_next_to_sensor = true;
   }
};

/** \purpose  
 * Purpose of this test is to verify whether funciton returns true when there is no object track next to sensor.
 * \req
 * NA.
 */
TEST(f360_det_is_seen_by_sensor, Det_Is_Seen_By_Sensor__No_Object_Near_Sensor)
{
   /** \precond
    * Reset flag f_object_track_next_to_sensor
    * Place detection to make it visible by sensor
    */
   radar_sensor_props.f_object_track_next_to_sensor = false;
   det_pos.y = 2.0F;

   /** \action
    * Call tested function
    */
   const bool f_det_is_seen = Is_Det_Seen_By_Sensor(det_pos, radar_sensor_props, lat_th_ext);

   /** \result
    * Check whether funciton returned true
    */	
   CHECK_TRUE(f_det_is_seen);
}

/** \purpose
* Purpose of this test is to verify whether funciton returns false when there is object track next to sensor
* and it ocludes detection
* \req
* NA.
*/
TEST(f360_det_is_seen_by_sensor, Det_Is_Seen_By_Sensor__Object_Near_Sensor_Det_Occluded)
{
   /** \precond
   * Place detection to make it occluded by track
   */
   det_pos.y = 10.0F;

   /** \action
   * Call tested function
   */
   const bool f_det_is_seen = Is_Det_Seen_By_Sensor(det_pos, radar_sensor_props, lat_th_ext);

   /** \result
   * Check whether funciton returned false
   */
   CHECK_FALSE(f_det_is_seen);
}

/** \purpose
* Purpose of this test is to verify whether funciton returns true when there is object track next to sensor
* and it does not ocludes detection
* \req
* NA.
*/
TEST(f360_det_is_seen_by_sensor, Det_Is_Seen_By_Sensor__Object_Near_Sensor_Det_Not_Occluded)
{
   /** \precond
   * Place detection to make it not occluded by track
   */
   det_pos.y = 2.0F;

   /** \action
   * Call tested function
   */
   const bool f_det_is_seen = Is_Det_Seen_By_Sensor(det_pos, radar_sensor_props, lat_th_ext);

   /** \result
   * Check whether funciton returned false
   */
   CHECK_TRUE(f_det_is_seen);
}
/** @}*/

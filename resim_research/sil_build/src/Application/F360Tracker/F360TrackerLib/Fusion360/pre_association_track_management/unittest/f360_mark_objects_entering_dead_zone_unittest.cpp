/** \file
 * This file contains unit tests for content of f360_mark_objects_entering_dead_zone.cpp file
 */

#include "f360_mark_objects_entering_dead_zone.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_mark_objects_entering_dead_zone
 *  @{
 */

 /** \brief
  * Test group of Mark_Objects_Enterning_Dead_Zone function. Tests verify whether objects
  * dead zone status is updated when needed
  */
TEST_GROUP(f360_mark_objects_entering_dead_zone)
{
   F360_Host_T host{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Calibrations_T calib{};
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS]{};
   F360_Tracker_Info_T tracker_info{};
   F360_Object_Track_T& object = objects[0];

   /** \setup
    * Set up longitudinal position of two objects to make dead zone be in range <-5.0F, 0.5F>
    * Initialize tracker calibrations
    * Set up parameters of one object
    */
   TEST_SETUP()
   {
      sensors[0].variable.is_valid = true;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.5F;

      sensors[1].variable.is_valid = true;
      sensors[1].constant.mounting_position.vcs_position.longitudinal = -5.0F;

      Initialize_Tracker_Calibrations(calib);

      object.vcs_position.y = 2.0F;
      object.vcs_position.x = -2.5F;
      object.status = F360_OBJECT_STATUS_UPDATED;

      host.speed = calib.k_dead_zone_min_host_speed + 1.0F;


      tracker_info.active_obj_ids[0] = 1;
      tracker_info.num_active_objs = 1;

   }
};

/** \purpose
 * Purpose of this test is to verify whether when host speed is above threshold object dead zone status is updated
 * \req
 * NA.
 */
TEST(f360_mark_objects_entering_dead_zone, Mark_Objects_Enterning_Dead_Zone__Host_Speed_Above_Threshold)
{
   /** \precond
    * all is set in TEST_SETUP()
    */

    /** \action
     * Call tested function
     */
   Mark_Objects_Enterning_Dead_Zone(host, sensors, calib, tracker_info, objects);

   /** \result
    * Check whether object dead zone status was updated to F360_Dead_Zone_Status_T::INSIDE
    */
   CHECK_EQUAL(F360_Dead_Zone_Status_T::INSIDE, object.dead_zone_status);
}

/** \purpose
 * Purpose of this test is to verify whether when host speed is below threshold object dead zone status is set to undefined
 * \req
 * NA.
 */
TEST(f360_mark_objects_entering_dead_zone, Mark_Objects_Enterning_Dead_Zone__Host_Speed_Below_Threshold)
{
   /** \precond
    * all is set in TEST_SETUP()
    * Set host speed to be below threshold
    * Set up object status as F360_Dead_Zone_Status_T::INSIDE
    */
   host.speed = 0.5F * calib.k_dead_zone_min_host_speed;
   object.dead_zone_status = F360_Dead_Zone_Status_T::INSIDE;

    /** \action
     * Call tested function
     */
   Mark_Objects_Enterning_Dead_Zone(host, sensors, calib, tracker_info, objects);

   /** \result
    * Check whether object dead zone status was updated to F360_Dead_Zone_Status_T::UNDEFINED
    */
   CHECK_EQUAL(F360_Dead_Zone_Status_T::UNDEFINED, object.dead_zone_status);
}
/** @}*/

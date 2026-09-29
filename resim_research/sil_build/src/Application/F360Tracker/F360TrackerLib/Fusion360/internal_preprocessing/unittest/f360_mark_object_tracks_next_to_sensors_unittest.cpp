/** \file
 * This file contains unit tests for content of f360_mark_object_tracks_next_to_sensors.cpp file
 */

#include "f360_mark_object_tracks_next_to_sensors.h"
#include <CppUTest/TestHarness.h>
#include "f360_constants.h"
#include "f360_sensor_type.h"
#include "f360_set_variant.h"
using namespace f360_variant_A;

/** \defgroup  f360_mark_object_tracks_next_to_sensors
 *  @{
 */

/** \brief
 * This test group aims to test the functionality of f360_mark_object_tracks_next_to_sensors
 */
TEST_GROUP(f360_mark_object_tracks_next_to_sensors)
{
   // Declare common variables used within all tests in this test group.
   F360_Calibrations_T calibrations = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS]= {};
   F360_Tracker_Info_T tracker_info = {};
   F360_TRKR_TIMING_INFO_T  timing_info = {};
   int sensor_index;
   int object_track_index;
   const float float_tolerance = 1e-6F;

   /** \setup
    * setup calibrations, sensor calibrations and an object's properties.
    * reset sensor properties which is the tests output
    */
   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);
      sensor_index=0;
      object_track_index=0;
      // Set calibration parameters
      calibrations.host_vehicle_length = 4.0f;
      calibrations.k_vp_vehicle_next_to_ego_max_lat_dist = 5.0f;
      calibrations.k_vp_vehicle_next_to_ego_max_lat_dist = 5.0f;
      calibrations.k_vp_vehicle_next_to_ego_max_long_dist = 5.0f;
      calibrations.k_vp_vehicle_next_to_ego_max_abs_heading = 0.785398185f;
      calibrations.k_vp_vehicle_next_to_ego_long_pos_offset = 0.5f;
      // Set sensor calibration parameters
      sensors[sensor_index].constant.mounting_position.vcs_position.lateral = -0.839999974f;
      sensors[sensor_index].constant.mounting_position.vcs_position.longitudinal = -0.275000006f;
      sensors[sensor_index].constant.mounting_location =F360_MOUNTING_LOCATION_LEFT_FORWARD;
      // Reset the output
      sensor_props[sensor_index].f_object_track_next_to_sensor  = false ;
      sensor_props[sensor_index].next_to_sensor_object_track_min_long_pos  = 0.0f ;
      sensor_props[sensor_index].next_to_sensor_object_track_max_long_pos  = 0.0f ;
      sensor_props[sensor_index].next_to_sensor_object_track_min_lat_pos  = 0.0f ;
      sensor_props[sensor_index].next_to_sensor_object_track_max_lat_pos  = 0.0f ;
      // Set object track properties
      object_tracks[object_track_index].vcs_position.y = -4.47537279f;
      object_tracks[object_track_index].vcs_position.x = -0.822061718f;
      object_tracks[object_track_index].status = F360_OBJECT_STATUS_NEW_UPDATED;
      object_tracks[object_track_index].f_moving = true;
      object_tracks[object_track_index].vcs_heading = Angle{ -0.0620114803F };
      object_tracks[object_track_index].Set_Bbox_Orientation(Angle{ -0.0620114803F });
      object_tracks[object_track_index].Update_Bbox_Size(6.0F, 1.271207452F);
   }
   void check_object_track_next_to_sensor(bool expected_f_object_track_next_to_sensor,
                                          int expected_next_to_sensor_object_track_id,
                                          float expected_next_to_sensor_object_track_min_long_pos,
                                          float expected_next_to_sensor_object_track_max_long_pos,
                                          float expected_next_to_sensor_object_track_min_lat_pos,
                                          float expected_next_to_sensor_object_track_max_lat_pos)
   {
      CHECK( expected_f_object_track_next_to_sensor == sensor_props[sensor_index].f_object_track_next_to_sensor )
      CHECK_EQUAL( expected_next_to_sensor_object_track_id, sensor_props[sensor_index].next_to_sensor_object_track_id )
      DOUBLES_EQUAL(expected_next_to_sensor_object_track_min_long_pos ,sensor_props[sensor_index].next_to_sensor_object_track_min_long_pos, float_tolerance)
      DOUBLES_EQUAL(expected_next_to_sensor_object_track_max_long_pos ,sensor_props[sensor_index].next_to_sensor_object_track_max_long_pos, float_tolerance)
      DOUBLES_EQUAL(expected_next_to_sensor_object_track_min_lat_pos  ,sensor_props[sensor_index].next_to_sensor_object_track_min_lat_pos , float_tolerance)
      DOUBLES_EQUAL(expected_next_to_sensor_object_track_max_lat_pos  ,sensor_props[sensor_index].next_to_sensor_object_track_max_lat_pos , float_tolerance)      
   }
};

/** \purpose
 * Verify that the valid object track in the left forward sensor's zone is being marked successfully
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, marking_object_being_on_left_forward_corner_sensor_zone)
{
   /** \precond
    * Set object track properties from test's setup
    */

   // Calculate expected overwritten lat/long position
   const float32_t long_offset = calibrations.k_vp_vehicle_next_to_ego_long_pos_offset;
   const Point object_track_center_pos = object_tracks[object_track_index].bbox.Get_Center();

   bool expected_f_object_track_next_to_sensor = true;
   int expected_next_to_sensor_object_track_id= object_track_index;
   float expected_next_to_sensor_object_track_min_long_pos = object_track_center_pos.x - (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) - long_offset;
   float expected_next_to_sensor_object_track_max_long_pos = object_track_center_pos.x + (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) + long_offset;
   float expected_next_to_sensor_object_track_min_lat_pos  = object_track_center_pos.y - (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);
   float expected_next_to_sensor_object_track_max_lat_pos  = object_track_center_pos.y + (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track is being marked successfully
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}
/** \purpose
 * Verify that the valid object track in the left rear sensor's zone is being marked successfully
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, marking_object_being_on_left_rear_corner_sensor_zone)
{
   /** \precond
    * Set object track properties from test's setup
    * Change sensor's mounting_location to left rear
    */
   sensors[sensor_index].constant.mounting_location =F360_MOUNTING_LOCATION_LEFT_REAR;

   // Calculate expected overwritten lat/long position
   const float32_t long_offset = calibrations.k_vp_vehicle_next_to_ego_long_pos_offset;
   const Point object_track_center_pos = object_tracks[object_track_index].bbox.Get_Center();

   bool expected_f_object_track_next_to_sensor = true;
   int expected_next_to_sensor_object_track_id= object_track_index;
   float expected_next_to_sensor_object_track_min_long_pos = object_track_center_pos.x - (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) - long_offset;
   float expected_next_to_sensor_object_track_max_long_pos = object_track_center_pos.x + (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) + long_offset;
   float expected_next_to_sensor_object_track_min_lat_pos  = object_track_center_pos.y - (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);
   float expected_next_to_sensor_object_track_max_lat_pos  = object_track_center_pos.y + (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track is being marked successfully
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}
/** \purpose
 * Verify that the valid object track in the right forward sensor's zone is being marked successfully
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, marking_object_being_on_right_forward_corner_sensor_zone)
{
   /** \precond
    * Set object track properties from test's setup
    * Change sensor's mounting_location to right forward
    */
   sensors[sensor_index].constant.mounting_location =F360_MOUNTING_LOCATION_RIGHT_FORWARD;

   // Calculate expected overwritten lat/long position
   const float32_t long_offset = calibrations.k_vp_vehicle_next_to_ego_long_pos_offset;
   const Point object_track_center_pos = object_tracks[object_track_index].bbox.Get_Center();

   bool expected_f_object_track_next_to_sensor = true;
   int expected_next_to_sensor_object_track_id= object_track_index;
   float expected_next_to_sensor_object_track_min_long_pos = object_track_center_pos.x - (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) - long_offset;
   float expected_next_to_sensor_object_track_max_long_pos = object_track_center_pos.x + (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) + long_offset;
   float expected_next_to_sensor_object_track_min_lat_pos  = object_track_center_pos.y - (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);
   float expected_next_to_sensor_object_track_max_lat_pos  = object_track_center_pos.y + (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track is being marked successfully
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}
/** \purpose  
 * Verify that the valid object track in the right rear sensor's zone is being marked successfully
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, marking_object_being_on_right_rear_corner_sensor_zone)
{
   /** \precond
    * Set object track properties from test's setup
    * Change sensor's mounting_location to right rear
    */
   sensors[sensor_index].constant.mounting_location =F360_MOUNTING_LOCATION_RIGHT_REAR;

   // Calculate expected overwritten lat/long position
   const float32_t long_offset = calibrations.k_vp_vehicle_next_to_ego_long_pos_offset;
   const Point object_track_center_pos = object_tracks[object_track_index].bbox.Get_Center();

   bool expected_f_object_track_next_to_sensor = true;
   int expected_next_to_sensor_object_track_id= object_track_index;
   float expected_next_to_sensor_object_track_min_long_pos = object_track_center_pos.x - (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) - long_offset;
   float expected_next_to_sensor_object_track_max_long_pos = object_track_center_pos.x + (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) + long_offset;
   float expected_next_to_sensor_object_track_min_lat_pos  = object_track_center_pos.y - (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);
   float expected_next_to_sensor_object_track_max_lat_pos  = object_track_center_pos.y + (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track is being marked successfully
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}
/** \purpose
 * Verify that the object track isn't marked if its not in a corner sensor's zone
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, object_being_on_center_sensor__zone_isnt_marked)
{
   /** \precond
    * Set object track properties from test's setup
    * Change sensor's mounting_location to center forward
    */
   sensors[sensor_index].constant.mounting_location =F360_MOUNTING_LOCATION_CENTER_FORWARD;

   // Calculate expected overwritten lat/long position
   bool expected_f_object_track_next_to_sensor = false;
   int expected_next_to_sensor_object_track_id= 0;
   float expected_next_to_sensor_object_track_min_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_max_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_min_lat_pos  = 0.0f;
   float expected_next_to_sensor_object_track_max_lat_pos  = 0.0f;

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track isn't being marked and has default zero value
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}

/** \purpose
 * Verify that the valid object track in the left forward sensor's zone is being marked successfully,when a swap is needed between the minimum longitudinal distance and the
 * maximum longitudinal (min > max) given negative k_vp_vehicle_next_to_ego_max_long_dist calibration
 * Note: the only way to have a min_long distance > max_long distance is when we have a negative k_vp_vehicle_next_to_ego_max_long_dist calibration value
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, swap_minimum_and_maximum_long_distance)
{
   /** \precond
    * Set object track properties from test's setup
    * Change calibrations k_vp_vehicle_next_to_ego_max_long_dist to -5
    */
   calibrations.k_vp_vehicle_next_to_ego_max_long_dist = -5.0f;

      // Calculate expected overwritten lat/long position
   const float32_t long_offset = calibrations.k_vp_vehicle_next_to_ego_long_pos_offset;
   const Point object_track_center_pos = object_tracks[object_track_index].bbox.Get_Center();

   bool expected_f_object_track_next_to_sensor = true;
   int expected_next_to_sensor_object_track_id= object_track_index;
   float expected_next_to_sensor_object_track_min_long_pos = object_track_center_pos.x - (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) - long_offset;
   float expected_next_to_sensor_object_track_max_long_pos = object_track_center_pos.x + (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) + long_offset;
   float expected_next_to_sensor_object_track_min_lat_pos  = object_track_center_pos.y - (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);
   float expected_next_to_sensor_object_track_max_lat_pos  = object_track_center_pos.y + (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track is being marked successfully
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}

/** \purpose
 * Verify that the object track in the left forward sensor's zone isn't marked if its invalid
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, invalid_object_isnt_marked)
{
   /** \precond
    * Set object track properties from test's setup
    * Change object track status to invalid
    */
   object_tracks[object_track_index].status = F360_OBJECT_STATUS_INVALID;

   // Calculate expected overwritten lat/long position
   bool expected_f_object_track_next_to_sensor = false;
   int expected_next_to_sensor_object_track_id= -1;
   float expected_next_to_sensor_object_track_min_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_max_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_min_lat_pos  = 0.0f;
   float expected_next_to_sensor_object_track_max_lat_pos  = 0.0f;

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track isn't marked
    */   
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}

/** \purpose
 * Verify that the object track in the left forward sensor's zone isn't marked if its stationary
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, stationary_object_isnt_marked)
{
   /** \precond
    * Set object track properties from test's setup
    * Change object track f_moving to false
    */
   sensors[sensor_index].constant.mounting_location =F360_MOUNTING_LOCATION_RIGHT_FORWARD;
   object_tracks[object_track_index].f_moving = false;

   // Calculate expected overwritten lat/long position
   bool expected_f_object_track_next_to_sensor = false;
   int expected_next_to_sensor_object_track_id= -1;
   float expected_next_to_sensor_object_track_min_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_max_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_min_lat_pos  = 0.0f;
   float expected_next_to_sensor_object_track_max_lat_pos  = 0.0f;

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track isn't marked
    */   
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}
/** \purpose
 * Verify that the valid object track in the left forward sensor's zone isn't marked when its vcs heading is greater than k_vp_vehicle_next_to_ego_max_abs_heading calibration value
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, high_vcs_heading_object_isnt_marked)
{
   /** \precond
    * Set object track properties from test's setup
    * Change object tracks vcs_heading to a value greater than calibration.k_vp_vehicle_next_to_ego_max_abs_heading
    */
   sensors[sensor_index].constant.mounting_location =F360_MOUNTING_LOCATION_RIGHT_FORWARD;
   object_tracks[object_track_index].vcs_heading = Angle{ 1.5F };

   // Calculate expected overwritten lat/long position 
   bool expected_f_object_track_next_to_sensor = false;
   int expected_next_to_sensor_object_track_id= -1;
   float expected_next_to_sensor_object_track_min_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_max_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_min_lat_pos  = 0.0f;
   float expected_next_to_sensor_object_track_max_lat_pos  = 0.0f;

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track isn't marked
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}

/** \purpose
 * Verify that the object track in the left forward sensor's zone isn't marked when its not  within the acceptable front longitudinal interval, lower than the minimum range
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, object_outside_the_minimum_accpetable_front_longitudinal_interval_range_isnt_marked)
{
   /** \precond
    * Set object track properties from test's setup
    * Change object_tracks longitudinal vcs position
    */
   object_tracks[object_track_index].vcs_position.x = -10.0f;
   object_tracks[object_track_index].Update_Bbox_Center();

   // Calculate expected overwritten lat/long position 
   bool expected_f_object_track_next_to_sensor = false;
   int expected_next_to_sensor_object_track_id= -1;
   float expected_next_to_sensor_object_track_min_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_max_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_min_lat_pos  = 0.0f;
   float expected_next_to_sensor_object_track_max_lat_pos  = 0.0f;

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track isn't  marked
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}
/** \purpose
 * Verify that the object track in the left forward sensor's zone isn't marked when its not  within the acceptable front longitudinal interval,higher than the maximum range
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, object_outside_the_accpetable_maximum_front_longitudinal_interval_range_isnt_marked)
{
   /** \precond
    * Set object track properties from test's setup
    * Change object tracks longitudinal vcs position
    */
   object_tracks[object_track_index].vcs_position.x = 8.0f;
   object_tracks[object_track_index].Update_Bbox_Center();

   // Calculate expected overwritten lat/long position
   bool expected_f_object_track_next_to_sensor = false;
   int expected_next_to_sensor_object_track_id= -1;
   float expected_next_to_sensor_object_track_min_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_max_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_min_lat_pos  = 0.0f;
   float expected_next_to_sensor_object_track_max_lat_pos  = 0.0f;

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track isn't marked
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}
/** \purpose  
 * Verify that the object track in the left forward sensor's zone isn't marked when its not  within the acceptable rear longitudinal interval, lower than the minimum range
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, object_outside_the_accpetable_minimum_rear_longitudinal_interval_range_isnt_marked)
{
   /** \precond
    * Set object track properties from test's setup
    * Change object_track longitudinal vcs position
    */
   object_tracks[object_track_index].vcs_position.x = -10.0f;
   object_tracks[object_track_index].Update_Bbox_Center();

   // Calculate expected overwritten lat/long position
   bool expected_f_object_track_next_to_sensor = false;
   int expected_next_to_sensor_object_track_id= -1;
   float expected_next_to_sensor_object_track_min_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_max_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_min_lat_pos  = 0.0f;
   float expected_next_to_sensor_object_track_max_lat_pos  = 0.0f;

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track isn't  marked
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}

/** \purpose
 * Verify that the object track in the left forward sensor's zone isn't marked when its not  within the acceptable rear longitudinal interval, higher than the maximum range
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, object_outside_the_accpetable_maximum_rear_longitudinal_interval_range_isnt_marked)
{
   /** \precond
    * Set object track properties from test's setup
    * Change object tracks longitudinal vcs position
    */
   object_tracks[object_track_index].vcs_position.x = 10.0f;
   object_tracks[object_track_index].Update_Bbox_Center();

   // Calculate expected overwritten lat/long position
   bool expected_f_object_track_next_to_sensor = false;
   int expected_next_to_sensor_object_track_id= -1;
   float expected_next_to_sensor_object_track_min_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_max_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_min_lat_pos  = 0.0f;
   float expected_next_to_sensor_object_track_max_lat_pos  = 0.0f;

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track isn't  marked
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}
/** \purpose
 * Verify that the object track in the left forward sensor's zone isn't marked when its not  within the acceptable lateral interval
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, object_outside_the_acceptable_lateral_interval_range_isnt_marked)
{
   /** \precond
    * Set object track properties from test's setup
    * Change object tracks longitudinal vcs position
    */
   object_tracks[object_track_index].vcs_position.y = -6.0f;
   object_tracks[object_track_index].Update_Bbox_Center();

   // Calculate expected overwritten lat/long position
   bool expected_f_object_track_next_to_sensor = false;
   int expected_next_to_sensor_object_track_id= -1;
   float expected_next_to_sensor_object_track_min_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_max_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_min_lat_pos  = 0.0f;
   float expected_next_to_sensor_object_track_max_lat_pos  = 0.0f;

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track isn't  marked
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}

/** \purpose
 * Verify that the object track in the left forward sensor's zone is marked successfully when the object track occupies the entire longitudinal interval
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, marking_object_which_occupies_entire_longitudinal_interval)
{
   /** \precond
    * Change sensor calibration longitudinal vcs position
    * Set object track properties
    */
   sensors[0].constant.mounting_location =F360_MOUNTING_LOCATION_RIGHT_REAR;
   sensors[sensor_index].constant.mounting_position.vcs_position.longitudinal = -4.870f;
   sensors[sensor_index].constant.mounting_position.vcs_position.lateral = -0.870f;
   object_tracks[object_track_index].vcs_position.x = -10.248f;
   object_tracks[object_track_index].vcs_position.y = -5.034f;
   object_tracks[object_track_index].Set_Bbox_Orientation(Angle{ -0.06F });
   object_tracks[object_track_index].Update_Bbox_Size(22.374F, 2.500F);

   // Calculate expected overwritten lat/long position
   const float32_t long_offset = calibrations.k_vp_vehicle_next_to_ego_long_pos_offset;
   const Point object_track_center_pos = object_tracks[object_track_index].bbox.Get_Center();

   bool expected_f_object_track_next_to_sensor = true;
   int expected_next_to_sensor_object_track_id= object_track_index;
   float expected_next_to_sensor_object_track_min_long_pos = object_track_center_pos.x - (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) - long_offset;
   float expected_next_to_sensor_object_track_max_long_pos = object_track_center_pos.x + (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) + long_offset;
   float expected_next_to_sensor_object_track_min_lat_pos  = object_track_center_pos.y - (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);
   float expected_next_to_sensor_object_track_max_lat_pos  = object_track_center_pos.y + (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track is marked successfully
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}
/** \purpose
 * Verify that the valid object track in the left forward sensor's zone is being marked successfully when the object track is within front longitudinal and lateral interval
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, marking_object_being_within_acceptable_front_longitudinal_and_lateral_interval)
{
   /** \precond
    * Set object track properties from test's setup
    */
   object_tracks[object_track_index].vcs_position.x = -2.5f;
   object_tracks[object_track_index].Update_Bbox_Center();

   // Calculate expected overwritten lat/long position
   const float32_t long_offset = calibrations.k_vp_vehicle_next_to_ego_long_pos_offset;
   const Point object_track_center_pos = object_tracks[object_track_index].bbox.Get_Center();

   bool expected_f_object_track_next_to_sensor = true;
   int expected_next_to_sensor_object_track_id= object_track_index;
   float expected_next_to_sensor_object_track_min_long_pos = object_track_center_pos.x - (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) - long_offset;
   float expected_next_to_sensor_object_track_max_long_pos = object_track_center_pos.x + (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) + long_offset;
   float expected_next_to_sensor_object_track_min_lat_pos  = object_track_center_pos.y - (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);
   float expected_next_to_sensor_object_track_max_lat_pos  = object_track_center_pos.y + (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);
  
   /** \result
    * The object track is being marked successfully
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}
/** \purpose  
 * Verify that the valid object track in the left forward sensor's zone is being marked successfully when the object track is within rear longitudinal interval and lateral interval
 * \req
 * NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, marking_object_being_within_acceptable_rear_longitudinal_and_lateral_interval)
{
   /** \precond
    * Set object track properties from test's setup
    */
   object_tracks[object_track_index].vcs_position.x = 2.5f;
   object_tracks[object_track_index].Update_Bbox_Center();

   // Calculate expected overwritten lat/long position
   const float32_t long_offset = calibrations.k_vp_vehicle_next_to_ego_long_pos_offset;
   const Point object_track_center_pos = object_tracks[object_track_index].bbox.Get_Center();

   bool expected_f_object_track_next_to_sensor = true;
   int expected_next_to_sensor_object_track_id= object_track_index;
   float expected_next_to_sensor_object_track_min_long_pos = object_track_center_pos.x - (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) - long_offset;
   float expected_next_to_sensor_object_track_max_long_pos = object_track_center_pos.x + (object_tracks[object_track_index].bbox.Get_Length() * 0.5F) + long_offset;
   float expected_next_to_sensor_object_track_min_lat_pos  = object_track_center_pos.y - (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);
   float expected_next_to_sensor_object_track_max_lat_pos  = object_track_center_pos.y + (object_tracks[object_track_index].bbox.Get_Width() * 0.5F);

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track is being marked successfully
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}
/** \purpose
 * Verify that the object track in the left forward sensor's zone isn't marked when the object distance is greater than object_track minimum distance
 * \req
 *  NA.
 */
TEST(f360_mark_object_tracks_next_to_sensors, object_with_a_distance_greater_than_object_track_minimum_distance_isnt_marked)
{
   /** \precond
    * Change object_track vcs position longitudinal and lateral.
    * Change calibrations and sensor calibrations.
    */
   //to have an obj_dist greater than object_track_min_dist which is 10000.0f and to reach the final code where we update the output we must change calibrations.
   object_tracks[object_track_index].vcs_position.x = 85.0f;
   object_tracks[object_track_index].vcs_position.y = 61.0f;
   object_tracks[object_track_index].Update_Bbox_Center();

   calibrations.k_vp_vehicle_next_to_ego_max_lat_dist = 20.00000000;
   sensors[sensor_index].constant.mounting_position.vcs_position.lateral = 50.0;
   sensors[sensor_index].constant.mounting_position.vcs_position.longitudinal = 85.0;
   
   // Calculate expected overwritten lat/long position 
   bool expected_f_object_track_next_to_sensor = false;
   int expected_next_to_sensor_object_track_id= -1;
   float expected_next_to_sensor_object_track_min_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_max_long_pos = 0.0f;
   float expected_next_to_sensor_object_track_min_lat_pos  = 0.0f;
   float expected_next_to_sensor_object_track_max_lat_pos  = 0.0f;

   /** \action
    * Call Mark_Object_Tracks_Next_To_Sensors function.
    */
   Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

   /** \result
    * The object track isn't marked
    */
   check_object_track_next_to_sensor(expected_f_object_track_next_to_sensor,expected_next_to_sensor_object_track_id,
                                     expected_next_to_sensor_object_track_min_long_pos, expected_next_to_sensor_object_track_max_long_pos,
                                     expected_next_to_sensor_object_track_min_lat_pos ,expected_next_to_sensor_object_track_max_lat_pos);
}
/** @}*/

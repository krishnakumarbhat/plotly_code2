/** \file
 * This file contains unit tests for content of f360_handle_bistatic.cpp file
 */

#include "f360_handle_bistatic.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_handle_bistatic
 *  @{
 */

/** \brief
 * Purpose of this test group is to verify whether bistatic detections are properly marked as not okay to use
 */
TEST_GROUP(f360_handle_bistatic)
{
   const float tolerance = 0.000001F;
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Calibrations_T calib{};
   F360_Detection_Props_T dets_props[MAX_NUMBER_OF_DETECTIONS]{};
   rspp_variant_A::RSPP_Detection_T& det_raw = raw_detection_list.detections[0];
   F360_Detection_Props_T& det_props = dets_props[0];
   
   /** \setup
    * Setup one detection parameters
    * Initialize tracker calibrations
    * Set sensor occlusion info
    */
   TEST_SETUP()
   {
      raw_detection_list.number_of_valid_detections = 1;
      det_raw.raw.f_bistatic = true;
      det_props.f_ok_to_use = true;

      det_raw.raw.sensor_id = 1;

      Initialize_Tracker_Calibrations(calib);

      sensor_props[0].f_object_track_next_to_sensor = true;
      sensor_props[0].next_to_sensor_object_track_max_lat_pos = 2.0F;
      sensor_props[0].next_to_sensor_object_track_min_lat_pos = 2.0F;
   }
};

/** \purpose  
 * Purpose of this test is to verify whether bistatic detections do not have their f_ok_to_use flag changed.
 * \req
 * NA.
 */
TEST(f360_handle_bistatic, Handle_Bistatic__No_Bistatic_Do_Not_Have_Their_Flag_Changed)
{
   /** \precond
    * Set detection f_bistatic flag to false
    */
   det_raw.raw.f_bistatic = false;
	
   /** \action
    * Call tested function
    */
   Handle_Bistatic_Dets(det_raw, calib, sensor_props[0], det_props);

   /** \result
    * Check whether f_ok_to_use flag was not reset
    */	
   CHECK_TRUE(det_props.f_ok_to_use);
}

/** \purpose
* Purpose of this test is to verify whether bistatic detections that is on guardrail is marked as not okay to use
* \req
* NA.
*/
TEST(f360_handle_bistatic, Handle_Bistatic__Bistatic_On_Guardrail_Marked_As_Not_Okay_To_Use)
{
   /** \precond
   * Set detection on sep id to 1
   */
   det_props.on_sep_id = 1;

   /** \action
   * Call tested function
   */
   Handle_Bistatic_Dets(det_raw, calib, sensor_props[0], det_props);

   /** \result
   * Check whether f_ok_to_use flag was set to false
   */
   CHECK_FALSE(det_props.f_ok_to_use);
}

/** \purpose
* Purpose of this test is to verify whether bistatic detections that is behind guardrail is marked as not okay to use
* \req
* NA.
*/
TEST(f360_handle_bistatic, Handle_Bistatic__Bistatic_Behind_Guardrail_Marked_As_Not_Okay_To_Use)
{
   /** \precond
   * Set detection behind sep id to 1 
   */
   det_props.behind_sep_id = 1;

   /** \action
   * Call tested function
   */
   Handle_Bistatic_Dets(det_raw, calib, sensor_props[0], det_props);

   /** \result
   * Check whether f_ok_to_use flag was set to false
   */
   CHECK_FALSE(det_props.f_ok_to_use);
}

/** \purpose
* Purpose of this test is to verify whether bistatic detections that is not in bistatic FP area
* is marked as not okay to use
* \req
* NA.
*/
TEST(f360_handle_bistatic, Handle_Bistatic__Bistatic_Outside_FP_Area)
{
   /** \precond
   * Set detection to be placed outside FP area
   */
   det_props.vcs_position.x = 100.0F;
   det_props.vcs_position.y = 100.0F;

   /** \action
   * Call tested function
   */
   Handle_Bistatic_Dets(det_raw, calib, sensor_props[0], det_props);

   /** \result
   * Check whether f_ok_to_use flag was set to false
   */
   CHECK_FALSE(det_props.f_ok_to_use);
}

/** \purpose
* Purpose of this test is to verify whether bistatic detections that is in bistatic FP area
* but is occluded is marked as not okay to use
* \req
* NA.
*/
TEST(f360_handle_bistatic, Handle_Bistatic__Bistatic_In_FP_Area_Occluded)
{
   /** \precond
   * Set detection to be placed outside FP area
   */
   det_props.vcs_position.x = 0.0F;
   det_props.vcs_position.y = 4.0F;

   /** \action
   * Call tested function
   */
   Handle_Bistatic_Dets(det_raw, calib, sensor_props[0], det_props);

   /** \result
   * Check whether f_ok_to_use flag was set to false
   */
   CHECK_FALSE(det_props.f_ok_to_use);
}

/** \purpose
* Purpose of this test is to verify whether bistatic detections that is in bistatic FP area
* and is not occluded does not have its f_ok_to_use flag reset
* \req
* NA.
*/
TEST(f360_handle_bistatic, Handle_Bistatic__Bistatic_In_FP_Area_Not_Occluded)
{
   /** \precond
   * Set detection to be placed inside FP area
   * Set detection lateral position to be not occluded
   */
   det_props.vcs_position.x = 0.0F;
   det_props.vcs_position.y = 1.0F;

   /** \action
   * Call tested function
   */
   Handle_Bistatic_Dets(det_raw, calib, sensor_props[0], det_props);

   /** \result
   * Check whether f_ok_to_use flag was not reset
   */
   CHECK_TRUE(det_props.f_ok_to_use);
}

/** \purpose
* Test calibs setup
* \req
* NA.
*/
TEST(f360_handle_bistatic, Calibs_Seetup)
{
   /** \result
   * Calibs set as expected
   */   
   DOUBLES_EQUAL(-5.0F, calib.k_bistatic_cond_assoc_area_min_lat, tolerance);
   DOUBLES_EQUAL(5.0F, calib.k_bistatic_cond_assoc_area_max_lat, tolerance);
   DOUBLES_EQUAL(-12.0F, calib.k_bistatic_cond_assoc_area_min_lon, tolerance);
   DOUBLES_EQUAL(30.0F, calib.k_bistatic_cond_assoc_area_max_lon, tolerance);
   DOUBLES_EQUAL(0.7F, calib.k_bistatic_lat_th_extension, tolerance);
}
/** @}*/

/** \file
 * This file contains unit tests for content of f360_detect_stationary_bounce.cpp file
 */

#include "f360_detect_stationary_bounce.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_detect_stationary_bounce
 *  @{
 */

/** \brief
 * test group contains test cases for Detect_Stationary_Bounce_Detections() function.
 */
TEST_GROUP(f360_detect_stationary_bounce)
{	
   // Initialize data needed for all test cases in this test group.
   F360_Calibrations_T calib;
   F360_Host_T host = {};
   F360_Tracker_Info_T  tracker_info = {};
   F360_Detection_Props_T detections[MAX_NUMBER_OF_DETECTIONS] = {};
   bool f_expected_flag[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t num_det_vld = 0U;
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};
   /** \setup
    * Initialize tracker calibration
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose   Checks if any detection has not been flagged as stationary bounce when there is no active object in FOV.
 *
 * \req  NA
 */
TEST(f360_detect_stationary_bounce, Test_Detect_Stationary_Bounce_Lack_Of_Objects_In_FOV)
{
   /** \precond
    *  There in no acitve object in FOV.
    */
   tracker_info.vcslong_sorted_start = NULL; 
   tracker_info.num_active_objs = 0;

   /** \action
    * call Detect_Stationary_Bounce_Detections().
    */
   Detect_Stationary_Bounce_Detections(calib, host, tracker_info, raw_detect_list, detections);

   /** \result
    * check if all detections data has stationary bounce flag sets for false.
    */	
   bool f_correct_output = true;

   for (int32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      f_correct_output = f_correct_output && (f_expected_flag[i] == detections[i].f_stationary_bounce);
   }

   CHECK_TRUE(f_correct_output);
}

/** \purpose   Checks if any detection has not been flagged as stationary bounce
*              when host speed is too low for stationary bounce algorith activation.
*
* \req  NA
*/
TEST(f360_detect_stationary_bounce, Test_Detect_Stationary_Bounce_When_Host_Speed_Is_Below_Theshold)
{
   /** \precond
    *  Guardrail is active.
    *  Host speed is smaller than minimum host speed for alogrithm activation.
    */
   host.speed = calib.k_stat_bounce_min_host_speed - 1.0F;

   /** \action
    * call Detect_Stationary_Bounce_Detections().
    */
   Detect_Stationary_Bounce_Detections(calib, host, tracker_info, raw_detect_list, detections);

   /** \result
    * check if all detection data has stationary bounce flag sets for false.
    */
   bool f_correct_output = true;

   for (int32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      f_correct_output = f_correct_output && (f_expected_flag[i] == detections[i].f_stationary_bounce);
   }

   CHECK_TRUE(f_correct_output);
}

/** \purpose   Checks if any detection has not been flagged as stationary bounce 
*              when active object has bigger longitudnal position  than maximum
*              object longitudinal position for looking for stationary bounce.
*              
* \req  NA
*/
TEST(f360_detect_stationary_bounce, Test_Detect_Stationary_Bounce_Object_With_Too_High_Long_Posn)
{
   /** \precond
    *  Guardrail is active.
    *  Host speed is greater than minimum host speed for alogrithm activation.
    *  Active object longitudinal position is greater than maximum object longitudinal position 
    *  for stationary bounce activation.
    */
   F360_Object_Track_T trk_object = {};
   trk_object.id = 1;
   trk_object.f_moving = true;
   trk_object.vcs_position.x = calib.k_stat_bounce_max_trk_long_posn + 1.0F;
   tracker_info.vcslong_sorted_start = &trk_object;
   tracker_info.num_active_objs = 1;

   host.speed = calib.k_stat_bounce_min_host_speed + 1.0F;

   /** \action
    * call Detect_Stationary_Bounce_Detections().
    */
   Detect_Stationary_Bounce_Detections(calib, host, tracker_info, raw_detect_list, detections);

   /** \result
    * check if all detection data has stationary bounce flag sets for false.
    */
   bool f_correct_output = true;

   for (int32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      f_correct_output = f_correct_output && (f_expected_flag[i] == detections[i].f_stationary_bounce);
   }

   CHECK_TRUE(f_correct_output);
}


/** \purpose   Checks if any detection has not been flagged as stationary bounce
*              when active object has bigger longitudnal position  than maximum
*              object longitudinal position for looking for stationary bounce.
*
* \req  NA
*/
TEST(f360_detect_stationary_bounce, Test_Detect_Stationary_Bounce_Object_For_Non_Moving_Object)
{
   /** \precond
    *  Guardrail is active.
    *  Host speed is greater than minimum host speed for alogrithm activation.
    *  Active object longitudinal position is greater than min object longitudinal position
    *  for stationary bounce activation.
    *  Object is stationary.
    */
   F360_Object_Track_T trk_object = {};
   trk_object.id = 1;
   trk_object.f_moving = false;
   trk_object.vcs_position.x = calib.k_stat_bounce_max_trk_long_posn - 1.0F;
   tracker_info.vcslong_sorted_start = &trk_object;
   tracker_info.num_active_objs = 1;

   host.speed = calib.k_stat_bounce_min_host_speed + 1.0F;

   /** \action
    * call Detect_Stationary_Bounce_Detections().
    */
   Detect_Stationary_Bounce_Detections(calib, host, tracker_info, raw_detect_list, detections);

   /** \result
    * check if all detection data has stationary bounce flag sets for false.
    */
   bool f_correct_output = true;

   for (int32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      f_correct_output = f_correct_output && (f_expected_flag[i] == detections[i].f_stationary_bounce);
   }

   CHECK_TRUE(f_correct_output);
}


/** \purpose   Checks if any detection has not been flagged as stationary bounce
*              when active object has bigger lonateral position than maximum
*              object lateral position for looking for stationary bounce.
*
* \req  NA
*/
TEST(f360_detect_stationary_bounce, Test_Detect_Stationary_Bounce_Object_With_Too_Big_Lat_Posn)
{
   /** \precond
    *  Guardrail is active.
    *  Host speed is greater than minimum host speed for alogrithm activation.
    *  Active object abs lateral position is greater than maximum object lateral position 
    *  for stationary bounce activation.
    */
   F360_Object_Track_T trk_object = {};
   trk_object.id = 1;
   trk_object.f_moving = true;
   trk_object.vcs_position.x = calib.k_stat_bounce_max_trk_long_posn - 1.0F;
   trk_object.vcs_position.y = calib.k_stat_bounce_max_trk_lat_dist + 1.0F;
   trk_object.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = trk_object.vcs_position;
   trk_object.bbox.Set_Center(center);
   trk_object.vcs_heading = Angle{ calib.k_stat_bounce_max_trk_heading - 0.2F };
   tracker_info.vcslong_sorted_next_track[trk_object.id - 1] = NULL;
   tracker_info.vcslong_sorted_start = &trk_object;
   tracker_info.num_active_objs = 1;

   host.speed = calib.k_stat_bounce_min_host_speed + 1.0F;

   /** \action
    * call Detect_Stationary_Bounce_Detections().
    */
   Detect_Stationary_Bounce_Detections(calib, host, tracker_info, raw_detect_list, detections);

   /** \result
    * check if all detection data has stationary bounce flag sets for false.
    */
   bool f_correct_output = true;

   for (int32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      f_correct_output = f_correct_output && (f_expected_flag[i] == detections[i].f_stationary_bounce);
   }

   CHECK_TRUE(f_correct_output);
}

/** \purpose   Checks if any detection has not been flagged as stationary bounce
*              when active object has bigger heading  than maximum
*              object heading for looking for stationary bounce.
*
* \req  NA
*/
TEST(f360_detect_stationary_bounce, Test_Detect_Stationary_Bounce_Object_With_Too_High_Heading)
{
   /** \precond
    *  Guardrail is active.
    *  Host speed is greater than minimum host speed for alogrithm activation.
    *  Active object  abs heading is greater than maximum object heading 
    *  for stationary bounce activation.
    */
   F360_Object_Track_T trk_object = {};
   trk_object.id = 1;
   trk_object.f_moving = true;
   trk_object.vcs_position.x = calib.k_stat_bounce_max_trk_long_posn - 1.0F;
   trk_object.vcs_position.y = calib.k_stat_bounce_max_trk_lat_dist - 1.0F;
   trk_object.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = trk_object.vcs_position;
   trk_object.bbox.Set_Center(center);
   trk_object.vcs_heading = Angle{ calib.k_stat_bounce_max_trk_heading + 0.2F};
   tracker_info.vcslong_sorted_start = &trk_object;
   tracker_info.num_active_objs = 1;
   tracker_info.vcslong_sorted_next_track[trk_object.id - 1] = NULL;

   host.speed = calib.k_stat_bounce_min_host_speed + 1.0F;

   /** \action
    * call Detect_Stationary_Bounce_Detections().
    */
   Detect_Stationary_Bounce_Detections(calib, host, tracker_info, raw_detect_list, detections);

   /** \result
    * check if all detection data has stationary bounce flag sets for false.
    */
   bool f_correct_output = true;

   for (int32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      f_correct_output = f_correct_output && (f_expected_flag[i] == detections[i].f_stationary_bounce);
   }

   CHECK_TRUE(f_correct_output);
}

/** \purpose   Checks if detection is flagged as stationary bounce
*              when all conditions are meet.
*
* \req  NA
*/
TEST(f360_detect_stationary_bounce, Test_Detect_Stationary_Bounce_Has_Proper_Param)
{
   /** \precond
    *  Guardrail is active.
    *  Host speed is greater than minimum host speed for alogrithm activation.
    *  Active object  abs heading is smaller than maximum object heading
    *  for stationary bounce activation.
    */
   F360_Object_Track_T trk_object = {};
   trk_object.id = 1;
   trk_object.f_moving = true;
   trk_object.vcs_position.x = calib.k_stat_bounce_max_trk_long_posn - 1.0F;
   trk_object.vcs_position.y = 0.0F;
   trk_object.vcs_heading = Angle{ 0.0F };
   trk_object.vcs_velocity.longitudinal = 15.0F;
   trk_object.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = trk_object.vcs_position;
   trk_object.bbox.Set_Center(center);
   trk_object.bbox.Set_Width(2.0F);

   tracker_info.vcslong_sorted_start = &trk_object;
   tracker_info.num_active_objs = 1;
   tracker_info.vcslong_sorted_next_track[trk_object.id - 1] = NULL;

   host.speed = calib.k_stat_bounce_min_host_speed + 1.0F;

   num_det_vld = 1U;
   raw_detect_list.number_of_valid_detections = num_det_vld;
   raw_detect_list.detections[0].processed.cos_vcs_az = -1.0F;
   raw_detect_list.detections[0].processed.vcs_az = F360_PI;
   detections[0].vcs_position.x = trk_object.vcs_position.x - 20.0F;
   raw_detect_list.detections[0].processed.next_sorted_idx = F360_INVALID_ID;
   detections[0].range_rate_dealiased = -19.0F;

   f_expected_flag[0] = true;

   /** \action
    * call Detect_Stationary_Bounce_Detections().
    */
   Detect_Stationary_Bounce_Detections(calib, host, tracker_info, raw_detect_list, detections);

   /** \result
    * check if first detection data has stationary bounce flag set for true.
    */
   bool f_correct_output = true;

   for (int32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      f_correct_output = f_correct_output && (f_expected_flag[i] == detections[i].f_stationary_bounce);
   }

   CHECK_TRUE(f_correct_output);
}
/** @}*/

/** \defgroup  f360_mark_stationary_bounce_detections
 *  @{
 */

 /** \brief test group contains test cases for    Mark_Stationary_Bounce_Detections_For_Track() function.
  *   
  */
TEST_GROUP(f360_mark_stationary_bounce_detections)
{
   // Initialize data needed for all test cases in this test group.
   F360_Calibrations_T calib;
   F360_Host_T host;
   F360_Object_Track_T curr_trk;
   F360_Detection_Props_T detections[MAX_NUMBER_OF_DETECTIONS];
   rspp_variant_A::RSPP_Detection_List_T raw_detections{};
   bool f_expected_flag[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t num_det_vld;
   int32_t first_sort_idx;
   /** \setup
    * Initialize tracker calibration and object properties.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      curr_trk.vcs_position.x = -40.0F;
      curr_trk.vcs_position.y = 0.0F;
      curr_trk.vcs_velocity.longitudinal = 15.0F;
      curr_trk.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = curr_trk.vcs_position;
      curr_trk.bbox.Set_Center(center);
      curr_trk.bbox.Set_Width(2.0F);

      host.speed = 12.0F;
      first_sort_idx = 0;
   }
};

/** \purpose   Checks if detections has been flagged properly.
 *
 * \req  NA
 */
TEST(f360_mark_stationary_bounce_detections, Test_Mark_Stationary_Bounce_Detections)
{
   /** \precond
    *  Fist detection shall meet all requierment to be flagged as stationary bounced.
    *  Second detection range rate doesn't meet range rate condition.
    *  Third detection doesn't meet azimuth constrains.
    *  Fourth detection  is closer to host than respected object.
    *  Fifth detection doesn't meet azimuth constrains.
    */

   num_det_vld = 4U;
   raw_detections.detections[0].processed.cos_vcs_az = -1.0F;
   raw_detections.detections[0].processed.vcs_az = F360_PI;
   detections[0].vcs_position.x = curr_trk.vcs_position.x - 20.0F;
   raw_detections.detections[0].processed.next_sorted_idx = 1;
   detections[0].range_rate_dealiased = -18.0F;

   raw_detections.detections[1].processed.cos_vcs_az = -1.0F;
   raw_detections.detections[1].processed.vcs_az = F360_PI;
   detections[1].vcs_position.x = curr_trk.vcs_position.x - 20.0F;
   raw_detections.detections[1].processed.next_sorted_idx = 2;
   detections[1].range_rate_dealiased = -18.0F + 2.0F*calib.k_stat_bounce_range_rate_diff_thr;

   raw_detections.detections[2].processed.cos_vcs_az = -1.0F;
   raw_detections.detections[2].processed.vcs_az = F360_PI_2;
   detections[2].vcs_position.x = curr_trk.vcs_position.x - 4.0F;
   raw_detections.detections[2].processed.next_sorted_idx = 3;
   detections[2].range_rate_dealiased = -18.0F;

   raw_detections.detections[3].processed.cos_vcs_az = -1.0F;
   raw_detections.detections[3].processed.vcs_az = F360_PI;
   detections[3].vcs_position.x = curr_trk.vcs_position.x + 4.0F;
   raw_detections.detections[3].processed.next_sorted_idx = 4;
   detections[3].range_rate_dealiased = -18.0F;

   raw_detections.detections[4].processed.cos_vcs_az = -1.0F;
   raw_detections.detections[4].processed.vcs_az = F360_PI_2;
   detections[4].vcs_position.x = curr_trk.vcs_position.x - 4.0F;
   raw_detections.detections[4].processed.next_sorted_idx = F360_INVALID_ID;
   detections[4].range_rate_dealiased = -18.0F;

   f_expected_flag[0] = true;
   /** \action
    * call Mark_Stationary_Bounce_Detections_For_Track().
    */
   Mark_Stationary_Bounce_Detections_For_Track(calib, host, curr_trk, num_det_vld, first_sort_idx, raw_detections, detections);

   /** \result
    * Only first detection should be flagged as stationary bounce.
    */
   bool f_correct_output = true;

   for (int32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      f_correct_output = f_correct_output && (f_expected_flag[i] == detections[i].f_stationary_bounce);
   }

   CHECK_TRUE(f_correct_output);

}

/** \purpose   Checks if detections has been flagged properly.
 *
 * \req  NA
 */
TEST(f360_mark_stationary_bounce_detections, Test_Mark_Stationary_Bounce_Detection_Were_Flagger_Before)
{
   /** \precond
    * Fist detection shall meet all requierment to be flagged as stationary bounced.
    * Second detection wase already flagged as stationary bouce so flag value shall reamin unchanged.
    */

   num_det_vld = 2U;
   raw_detections.detections[0].processed.cos_vcs_az = -1.0F;
   raw_detections.detections[0].processed.vcs_az = F360_PI;
   detections[0].vcs_position.x = curr_trk.vcs_position.x - 20.0F;
   raw_detections.detections[0].processed.next_sorted_idx = 1;
   detections[0].range_rate_dealiased = -18.0F;

   raw_detections.detections[1].processed.cos_vcs_az = -1.0F;
   raw_detections.detections[1].processed.vcs_az = F360_PI;
   detections[1].vcs_position.x = curr_trk.vcs_position.x - 20.0F;
   raw_detections.detections[1].processed.next_sorted_idx = F360_INVALID_ID;
   detections[1].range_rate_dealiased = -18.0F + 2.0F*calib.k_stat_bounce_range_rate_diff_thr;
   detections[1].f_stationary_bounce = true;

   f_expected_flag[0] = true;
   f_expected_flag[1] = true;

   /** \action
    * call Mark_Stationary_Bounce_Detections_For_Track().
    */
   Mark_Stationary_Bounce_Detections_For_Track(calib, host, curr_trk, num_det_vld, first_sort_idx, raw_detections, detections);

   /** \result
    * Checking if two detections has stationary bounce flag sets for true.
    */
   bool f_correct_output = true;

   for (int32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      f_correct_output = f_correct_output && (f_expected_flag[i] == detections[i].f_stationary_bounce);
   }

   CHECK_TRUE(f_correct_output);
}

/** \purpose   Checks if detections has been flagged properly.
 *
 * \req  NA
 */
TEST(f360_mark_stationary_bounce_detections, Test_Mark_Stationary_Bounce_Detection_Are_Set_Correctly)
{
   /** \precond
    * Fist detection should not meet azimuth requierment to be flagged as stationary bounced.
    * Second detection was already flagged as stationary bouce so flag value shall remain unchanged.
    * Third detecion should be marked as stationary bounce.
    */

   num_det_vld = 3U;
   raw_detections.detections[0].processed.cos_vcs_az = -1.0F;
   raw_detections.detections[0].processed.vcs_az = -F360_PI_2;
   detections[0].vcs_position.x = curr_trk.vcs_position.x - 4.0F;
   raw_detections.detections[0].processed.next_sorted_idx = 1;
   detections[0].range_rate_dealiased = -18.0F;

   raw_detections.detections[1].processed.cos_vcs_az = -1.0F;
   raw_detections.detections[1].processed.vcs_az = F360_PI;
   detections[1].vcs_position.x = curr_trk.vcs_position.x - 20.0F;
   raw_detections.detections[1].processed.next_sorted_idx = 2;
   detections[1].range_rate_dealiased = -18.0F + 2.0F*calib.k_stat_bounce_range_rate_diff_thr;
   detections[1].f_stationary_bounce = true;

   raw_detections.detections[2].processed.cos_vcs_az = -1.0F;
   raw_detections.detections[2].processed.vcs_az = F360_PI;
   detections[2].vcs_position.x = curr_trk.vcs_position.x - 20.0F;
   raw_detections.detections[2].processed.next_sorted_idx = F360_INVALID_ID;
   detections[2].range_rate_dealiased = -18.0F;;

   f_expected_flag[0] = false;
   f_expected_flag[1] = true;
   f_expected_flag[2] = true;
   /** \action
    * call Mark_Stationary_Bounce_Detections_For_Track().
    */
   Mark_Stationary_Bounce_Detections_For_Track(calib, host, curr_trk, num_det_vld, first_sort_idx, raw_detections, detections);

   /** \result
    * Checking if detections has correctly stationary bounce flag sets.
    */
   bool f_correct_output = true;

   for (int32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      f_correct_output = f_correct_output && (f_expected_flag[i] == detections[i].f_stationary_bounce);
   }

   CHECK_TRUE(f_correct_output);
}

/** \purpose   Checks if detections has been flagged properly.
 *
 * \req  NA
 */
TEST(f360_mark_stationary_bounce_detections, Test_Mark_Stationary_Bounce_Detection_Only_One_Meet_Distance_Thr)
{
   /** \precond
    * Fist detection should meet allrequierment to be flagged as stationary bounced.
    * Second detection doesn't meet distance condition. 
    * Third detecion should not be marked as stationary bounce.
    */

   num_det_vld = 3U;
   raw_detections.detections[0].processed.cos_vcs_az = -1.0F;
   raw_detections.detections[0].processed.vcs_az = -F360_PI;
   detections[0].vcs_position.x = curr_trk.vcs_position.x - 4.0F;
   raw_detections.detections[0].processed.next_sorted_idx = 1;
   detections[0].range_rate_dealiased = -18.0F;

   raw_detections.detections[1].processed.cos_vcs_az = -1.0F;
   raw_detections.detections[1].processed.vcs_az = -F360_PI;
   detections[1].vcs_position.x = curr_trk.vcs_position.x + 4.0F;
   raw_detections.detections[1].processed.next_sorted_idx = 2;
   detections[1].range_rate_dealiased = -18.0F;

   raw_detections.detections[2].processed.cos_vcs_az = -1.0F;
   raw_detections.detections[2].processed.vcs_az = F360_PI_2;
   detections[2].vcs_position.x = curr_trk.vcs_position.x + 50.0F;
   raw_detections.detections[2].processed.next_sorted_idx = F360_INVALID_ID;
   detections[2].range_rate_dealiased = -18.0F;;

   f_expected_flag[0] = true;
   f_expected_flag[1] = false;
   f_expected_flag[2] = false;

   /** \action
    * call Mark_Stationary_Bounce_Detections_For_Track().
    */
   Mark_Stationary_Bounce_Detections_For_Track(calib, host, curr_trk, num_det_vld, first_sort_idx, raw_detections, detections);

   /** \result
    * Checking if detections has correctly stationary bounce flag sets.
    */
   bool f_correct_output = true;

   for (int32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      f_correct_output = f_correct_output && (f_expected_flag[i] == detections[i].f_stationary_bounce);
   }

   CHECK_TRUE(f_correct_output);
}

/** @}*/

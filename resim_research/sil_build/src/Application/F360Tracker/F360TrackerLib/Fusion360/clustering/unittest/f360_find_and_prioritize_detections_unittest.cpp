/** \file
 * This file contains unit tests for content of f360_find_and_prioritize_detections.cpp file
 */

#include "f360_find_and_prioritize_detections.h"
#include <CppUTest/TestHarness.h>

#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_clustering_data_generator.h"

using namespace f360_variant_A;

/** \defgroup  f360_find_and_prioritize_detections
*  @{
*/

/** \brief
*  Includes tests that will test the behavior of function find_and_prioritize_detections.
**/
TEST_GROUP(f360_find_and_prioritize_detections)
{
   /** \setup
   * Setting up arguments for find_and_prioritize_detections function and assigning them with basic values
   **/
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Calibrations_T calib = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Host_T host = {};
   int16_t valid_det_count;
   int16_t valid_det_sorted_idxs[MAX_NUMBER_OF_DETECTIONS];

   bool valid_dets[MAX_NUMBER_OF_DETECTIONS] = {};
   bool f_cluster_moving;
   const int8_t dets_in_zone = 10;

   // Needed for support functions

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      host.vcs_speed = 5.0F;
      f_cluster_moving = true;

      // Reset output structures
      valid_det_count = 0;
      for (int i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
      {
         valid_dets[i] = false;
         valid_det_sorted_idxs[i] = 0U;
         raw_detection_list.detections[i].raw.sensor_id = 1U;
      }
      
   }
};

/**
*\purpose
* Check function behaviour of Find_And_Prioritize_Detections()
*\req
* NA
*/
TEST(f360_find_and_prioritize_detections, main_function_check_priority_zones_sorting)
{
   /** \precond
   Preparing valid detection data for each priority zone
   **/

   // assigning moving detections third zone
   float32_t det_vcs_pos_lat = 20.0F;
   float32_t det_vcs_pos_long = 0.0F;
   for (int16_t i = 0; i < 10; i++)
   {
      det_vcs_pos_long += 0.1F;
      det_vcs_pos_lat += 0.1F;

      Add_Simple_Det_Data(raw_detection_list, raw_detection_list.detections[i], i, det_vcs_pos_long, det_vcs_pos_lat, 
         0.0F, true, rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, detection_props);
      Add_Det_Data_For_Validity(detection_props[i], raw_detection_list.detections[i], 
         false, false, false, false, true, false, false, 0, F360_DETECTION_WHEELSPIN_TYPE_INVALID);
   }
   // assigning moving detections first zone
   det_vcs_pos_lat = 0.0F;
   det_vcs_pos_long = 0.0F;
   for (int16_t i = 10; i < 20; i++)
   {
      det_vcs_pos_long += 0.1F;

      Add_Simple_Det_Data(raw_detection_list, raw_detection_list.detections[i], i, det_vcs_pos_long, det_vcs_pos_lat, 
         0.0F, true, rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, detection_props);
      Add_Det_Data_For_Validity(detection_props[i], raw_detection_list.detections[i], 
         false, false, false, false, true, false, false, 0, F360_DETECTION_WHEELSPIN_TYPE_INVALID);
   }

   // assigning moving detections fourth zone
   det_vcs_pos_lat = 100.0F;
   det_vcs_pos_long = 100.0F;
   for (int16_t i = 20; i < 30; i++)
   {
      det_vcs_pos_long += 0.1F;
      det_vcs_pos_lat += 0.1F;

      Add_Simple_Det_Data(raw_detection_list, raw_detection_list.detections[i], i, det_vcs_pos_long, det_vcs_pos_lat, 
         0.0F, true, rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, detection_props);
      Add_Det_Data_For_Validity(detection_props[i], raw_detection_list.detections[i], 
         false, false, false, false, true, false, false, 0, F360_DETECTION_WHEELSPIN_TYPE_INVALID);
   }

   // assigning moving detections second zone
   det_vcs_pos_lat = 0.0F;
   det_vcs_pos_long = 0.0F;
   for (int16_t i = 30; i < 40; i++)
   {
      det_vcs_pos_long -= 0.1F;

      Add_Simple_Det_Data(raw_detection_list, raw_detection_list.detections[i], i, det_vcs_pos_long, det_vcs_pos_lat, 
         0.0F, true, rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, detection_props);
      Add_Det_Data_For_Validity(detection_props[i], raw_detection_list.detections[i], 
         false, false, false, false, true, false, false, 0, F360_DETECTION_WHEELSPIN_TYPE_INVALID);
   }

   // assigning moving detections fifth zone
   det_vcs_pos_lat = -100.0F;
   det_vcs_pos_long = -100.0F;
   for (int16_t i = 40; i < 50; i++)
   {
      det_vcs_pos_long -= 0.1F;
      det_vcs_pos_lat -= 0.1F;

      Add_Simple_Det_Data(raw_detection_list, raw_detection_list.detections[i], i, det_vcs_pos_long, det_vcs_pos_lat, 
         0.0F, true, rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, detection_props);
      Add_Det_Data_For_Validity(detection_props[i], raw_detection_list.detections[i], 
         false, false, false, false, true, false, false, 0, F360_DETECTION_WHEELSPIN_TYPE_INVALID);
   }
   
   Sort_Detections_Vcs_Long(raw_detection_list);
   
   /** \action
   * Calling Find_And_Prioritize_Detections
   **/
   // Find_And_Prioritize_Detections(raw_detection_list, sensors, calib, host, clustering_config, detection_props, valid_det_count, valid_det_sorted_idxs, valid_dets);
   Find_And_Prioritize_Detections(raw_detection_list, sensors, host, detection_props, f_cluster_moving, valid_det_count, valid_det_sorted_idxs, valid_dets);
   
   /** \result
   * Checking if detections are properly sorted by priority zones
   **/

   // first zone
   for (int8_t i = 0; i < 10; i++)
   {
      CHECK_EQUAL(10+i, valid_det_sorted_idxs[i]);
   }

   // second zone
   for (int8_t i = 0; i < 10; i++)
   {
      CHECK_EQUAL(30+i, valid_det_sorted_idxs[dets_in_zone + i]);
   }

   // third zone
   for (int8_t i = 0; i < 10; i++)
   {
      CHECK_EQUAL(i, valid_det_sorted_idxs[2 * dets_in_zone + i]);
   }

   // fourth zone
   for (int8_t i = 0; i < 10; i++)
   {
      CHECK_EQUAL(20+i, valid_det_sorted_idxs[3 * dets_in_zone + i]);
   }

   // fifth zone
   for (int8_t i = 0; i < 10; i++)
   {
      CHECK_EQUAL(40+i, valid_det_sorted_idxs[4 * dets_in_zone + i]);
   }
}

/** @}*/

/** \defgroup  Find_Forward_And_Backward_Starting_Det_Indexes
*  @{
*/

/** \brief
*  Includes tests that will test the behavior of function Find_Forward_And_Backward_Starting_Det_Indexes.
**/
TEST_GROUP(Find_Forward_And_Backward_Starting_Det_Indexes)
{
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
   * Set up 50 detections, in ascending position starting at 1 vcs longpos
   * Then, setup 50 more detections, in descending order starting at -1.0F;
   **/
   TEST_SETUP()
   {
      raw_detection_list.number_of_valid_detections = 100;
      raw_detection_list.vcslong_det_idx_min = 101;

      // Traversing forwards
      // First detection with longpos >= 0
      float det_xpos = 1.0F;
      detection_props[1].vcs_position.x = det_xpos;
      raw_detection_list.detections[1].processed.prev_sorted_idx = 52;
      raw_detection_list.detections[1].processed.next_sorted_idx = 2;

      for (uint16_t i = 2U; i < 51; i++)
      {  
         det_xpos += 1.0F;
         detection_props[i].vcs_position.x = det_xpos;
         raw_detection_list.detections[i].processed.prev_sorted_idx = i-1;
         raw_detection_list.detections[i].processed.next_sorted_idx = i+1;
      }
      // Top detection
      det_xpos += 1.0F;
      detection_props[51].vcs_position.x = det_xpos;
      raw_detection_list.detections[51].processed.prev_sorted_idx = 50;
      raw_detection_list.detections[51].processed.next_sorted_idx = F360_INVALID_ID;
      
      // Traversing backwards
      // First detection with longpos < 0
      det_xpos = -1.0F;
      detection_props[52].vcs_position.x = det_xpos;
      raw_detection_list.detections[52].processed.prev_sorted_idx = 50;
      raw_detection_list.detections[52].processed.next_sorted_idx = 1;
      for (uint16_t i = 53; i < 101; i++)
      {  
         det_xpos -= 1.0F;
         detection_props[i].vcs_position.x = det_xpos;
         raw_detection_list.detections[i].processed.prev_sorted_idx = i+1;
         raw_detection_list.detections[i].processed.next_sorted_idx = i-1;
      }
      // Bottom detection
      det_xpos -= 1.0F;
      detection_props[101].vcs_position.x = det_xpos;
      raw_detection_list.detections[101].processed.prev_sorted_idx = F360_INVALID_ID;
      raw_detection_list.detections[101].processed.next_sorted_idx = 100;
   }
};


/**
*\purpose
* Check function behaviour of Find_Forward_And_Backward_Starting_Det_Indexes() when there are several detections 
* both in front and behind of vcs longpos 0
*\req
* NA
*/
TEST(Find_Forward_And_Backward_Starting_Det_Indexes, Find_With_Dets_Both_Front_And_Behind_Many_Detections)
{
   /** \precond
   Done in test setup
   **/
   
   /** \action
   * Calling Find_Forward_And_Backward_Starting_Det_Indexes
   **/
   const Traverse_Starting_Det_Indexes det_starting_indexes = Find_Forward_And_Backward_Starting_Det_Indexes(raw_detection_list, detection_props);

   /** \result
   * Check if the correct detection indexes are returned
   * forward_det_idx should be 1 as it is the first detection with vcs xpos >= 0
   * backward_det_idx should be 52 as it is the first valid detection with vcs xpos < 0
   **/
   CHECK_EQUAL(1, det_starting_indexes.forward_det_idx);
   CHECK_EQUAL(52, det_starting_indexes.backward_det_idx);
}

/**
*\purpose
* Check function behaviour of Find_Forward_And_Backward_Starting_Det_Indexes() when there are several 
* detections but only in front of vcs longpos 0
*\req
* NA
*/
TEST(Find_Forward_And_Backward_Starting_Det_Indexes, Find_With_Dets_Only_Front_Many_Detections)
{
   /** \precond
   Set number of valid detections to 25
   Set vcs_long_det_idx_min to 1
   Set raw_detection_list.detections[1].processed.prev_sorted_idx to F360_INVALID_ID
   Set raw_detection_list.detections[25].processed.prev_sorted_idx to F360_INVALID_ID
   **/
   raw_detection_list.number_of_valid_detections = 25;
   raw_detection_list.vcslong_det_idx_min = 1;

   raw_detection_list.detections[1].processed.prev_sorted_idx = F360_INVALID_ID;
   raw_detection_list.detections[25].processed.next_sorted_idx = F360_INVALID_ID;

   /** \action
   * Calling Find_Forward_And_Backward_Starting_Det_Indexes
   **/
   const Traverse_Starting_Det_Indexes det_starting_indexes = Find_Forward_And_Backward_Starting_Det_Indexes(raw_detection_list, detection_props);

   /** \result
   * Check if the correct detection indexes are returned
   * forward_det_idx should be 1 the first detection with vcs xpos >= 0
   * backward_det_idx should be invalid as there are no detections with vcs xpos < 0
   **/
   CHECK_EQUAL(1, det_starting_indexes.forward_det_idx);
   CHECK_EQUAL(F360_INVALID_ID, det_starting_indexes.backward_det_idx);
}

/**
*\purpose
* Check function behaviour of Find_Forward_And_Backward_Starting_Det_Indexes() when there are several detections 
* but only behind of vcs longpos 0
*\req
* NA
*/
TEST(Find_Forward_And_Backward_Starting_Det_Indexes, Find_With_Dets_Only_Rear_Many_Detections)
{
   /** \precond
   Set raw_detection_list.number_of_valid_detections to 25
   Set raw_detection_list.vcslong_det_idx_min to 101
   Set raw_detection_list.detections[77].processed.next_sorted_idx to F360_INVALID_ID
   **/
   raw_detection_list.number_of_valid_detections = 25;
   raw_detection_list.vcslong_det_idx_min = 101;

   raw_detection_list.detections[77].processed.next_sorted_idx = F360_INVALID_ID;

   /** \action
   * Calling Find_Forward_And_Backward_Starting_Det_Indexes
   **/
   const Traverse_Starting_Det_Indexes det_starting_indexes = Find_Forward_And_Backward_Starting_Det_Indexes(raw_detection_list, detection_props);

   /** \result
   * Check if the correct detection indexes are returned
   * forward_det_idx should be invalid as there are no detections with vcs xpos >= 0
   * backward_det_idx should be 77 as it is the detection with the largest vcs xpos but still < 0
   **/
   CHECK_EQUAL(F360_INVALID_ID, det_starting_indexes.forward_det_idx);
   CHECK_EQUAL(77, det_starting_indexes.backward_det_idx);
}


/**
*\purpose
* Check function behaviour of Find_Forward_And_Backward_Starting_Det_Indexes() when there are 2 
* detections 1 in front and one behind of vcs longpos 0
*\req
* NA
*/
TEST(Find_Forward_And_Backward_Starting_Det_Indexes, Find_With_Dets_Front_And_Back_2_Detections)
{
   /** \precond
   Set raw_detection_list.number_of_valid_detections to 2
   Set raw_detection_list.vcslong_det_idx_min to 1
   Set raw_detection_list.detections[52].processed.prev_sorted_idx to F360_INVALID_ID
   Set raw_detection_list.detections[1].processed.next_sorted_idx to F360_INVALID_ID
   **/
   raw_detection_list.number_of_valid_detections = 2;
   raw_detection_list.vcslong_det_idx_min = 52;

   raw_detection_list.detections[52].processed.prev_sorted_idx = F360_INVALID_ID;
   raw_detection_list.detections[1].processed.next_sorted_idx = F360_INVALID_ID;

   /** \action
   * Calling Find_Forward_And_Backward_Starting_Det_Indexes
   **/
   const Traverse_Starting_Det_Indexes det_starting_indexes = Find_Forward_And_Backward_Starting_Det_Indexes(raw_detection_list, detection_props);

   /** \result
   * Check if the correct detection indexes are returned
   * forward_det_idx should be 1 the first detection with vcs xpos >= 0
   * backward_det_idx should be 52 as it is the detection with the largest vcs xpos but still < 0
   **/
   CHECK_EQUAL(1, det_starting_indexes.forward_det_idx);
   CHECK_EQUAL(52, det_starting_indexes.backward_det_idx);
}

/**
*\purpose
* Check function behaviour of Find_Forward_And_Backward_Starting_Det_Indexes() when there are 2 
* detections only but in front of vcs longpos 0
*\req
* NA
*/
TEST(Find_Forward_And_Backward_Starting_Det_Indexes, Find_With_Dets_Only_Front_2_Detections)
{
   /** \precond
   Set raw_detection_list.number_of_valid_detections to 2
   Set raw_detection_list.vcslong_det_idx_min to 1
   Set raw_detection_list.detections[1].processed.prev_sorted_idx to F360_INVALID_ID
   Set raw_detection_list.detections[2].processed.next_sorted_idx to F360_INVALID_ID
   **/
   raw_detection_list.number_of_valid_detections = 2;
   raw_detection_list.vcslong_det_idx_min = 1;

   raw_detection_list.detections[1].processed.prev_sorted_idx = F360_INVALID_ID;
   raw_detection_list.detections[2].processed.next_sorted_idx = F360_INVALID_ID;

   /** \action
   * Calling Find_Forward_And_Backward_Starting_Det_Indexes
   **/
   const Traverse_Starting_Det_Indexes det_starting_indexes = Find_Forward_And_Backward_Starting_Det_Indexes(raw_detection_list, detection_props);

   /** \result
   * Check if the correct detection indexes are returned
   * forward_det_idx should be 1 the first detection with vcs xpos >= 0
   * backward_det_idx should be invalid as there are no detections with vcs xpos < 0
   **/
   CHECK_EQUAL(1, det_starting_indexes.forward_det_idx);
   CHECK_EQUAL(F360_INVALID_ID, det_starting_indexes.backward_det_idx);
}

/**
*\purpose
* Check function behaviour of Find_Forward_And_Backward_Starting_Det_Indexes() when there are 2 detections but only behind of vcs longpos 0
*\req
* NA
*/
TEST(Find_Forward_And_Backward_Starting_Det_Indexes, Find_With_Dets_Only_Rear_2_Detections)
{
   /** \precond
   Set raw_detection_list.number_of_valid_detections to 2
   Set raw_detection_list.vcslong_det_idx_min to 101
   Set raw_detection_list.detections[101].processed.prev_sorted_idx to F360_INVALID_ID
   Set raw_detection_list.detections[100].processed.next_sorted_idx to F360_INVALID_ID
   **/
   raw_detection_list.number_of_valid_detections = 2;
   raw_detection_list.vcslong_det_idx_min = 101;

   raw_detection_list.detections[101].processed.prev_sorted_idx = F360_INVALID_ID;
   raw_detection_list.detections[100].processed.next_sorted_idx = F360_INVALID_ID;

   /** \action
   * Calling Find_Forward_And_Backward_Starting_Det_Indexes
   **/
   const Traverse_Starting_Det_Indexes det_starting_indexes = Find_Forward_And_Backward_Starting_Det_Indexes(raw_detection_list, detection_props);

   /** \result
   * Check if the correct detection indexes are returned
   * forward_det_idx should be invalid as there are no detections with vcs xpos >= 0
   * backward_det_idx should be 100 as it is the detection with the largest vcs xpos but still < 0
   **/
   CHECK_EQUAL(F360_INVALID_ID, det_starting_indexes.forward_det_idx);
   CHECK_EQUAL(100, det_starting_indexes.backward_det_idx);
}

/**
*\purpose
* Check function behaviour of Find_Forward_And_Backward_Starting_Det_Indexes() when there is
* only 1 detection and it is in front of vcs longpos 0
*\req
* NA
*/
TEST(Find_Forward_And_Backward_Starting_Det_Indexes, Find_With_Dets_Only_Front_1_Detection)
{
   /** \precond
   Set raw_detection_list.number_of_valid_detections to 1
   Setraw_detection_list.vcslong_det_idx_min to 1
   Set raw_detection_list.detections[1].processed.prev_sorted_idx to F360_INVALID_ID
   Set raw_detection_list.detections[1].processed.next_sorted_idx to F360_INVALID_ID
   **/
   raw_detection_list.number_of_valid_detections = 1;
   raw_detection_list.vcslong_det_idx_min = 1;

   raw_detection_list.detections[1].processed.prev_sorted_idx = F360_INVALID_ID;
   raw_detection_list.detections[1].processed.next_sorted_idx = F360_INVALID_ID;

   /** \action
   * Calling Find_Forward_And_Backward_Starting_Det_Indexes
   **/
   const Traverse_Starting_Det_Indexes det_starting_indexes = Find_Forward_And_Backward_Starting_Det_Indexes(raw_detection_list, detection_props);

   /** \result
   * Check if the correct detection indexes are returned
   * forward_det_idx should be 1 the first detection with vcs xpos >= 0
   * backward_det_idx should be invalid as there are no detections with vcs xpos < 0
   **/
   CHECK_EQUAL(1, det_starting_indexes.forward_det_idx);
   CHECK_EQUAL(F360_INVALID_ID, det_starting_indexes.backward_det_idx);
}

/**
*\purpose
* Check function behaviour of Find_Forward_And_Backward_Starting_Det_Indexes() when there is
* only 1 detection and it is in behind of vcs longpos 0
*\req
* NA
*/
TEST(Find_Forward_And_Backward_Starting_Det_Indexes, Find_With_Dets_Only_Rear_1_Detection)
{
   /** \precond
   Set 
   **/
   raw_detection_list.number_of_valid_detections = 1;
   raw_detection_list.vcslong_det_idx_min = 101;

   raw_detection_list.detections[101].processed.prev_sorted_idx = F360_INVALID_ID;
   raw_detection_list.detections[101].processed.next_sorted_idx = F360_INVALID_ID;

   /** \action
   * Calling Find_Forward_And_Backward_Starting_Det_Indexes
   **/
   const Traverse_Starting_Det_Indexes det_starting_indexes = Find_Forward_And_Backward_Starting_Det_Indexes(raw_detection_list, detection_props);

   /** \result
   * Check if the correct detection indexes are returned
   * forward_det_idx should be invalid as there are no detections with vcs xpos >= 0
   * backward_det_idx should be 101 as it is the detection with the largest vcs xpos but still < 0
   **/
   CHECK_EQUAL(F360_INVALID_ID, det_starting_indexes.forward_det_idx);
   CHECK_EQUAL(101, det_starting_indexes.backward_det_idx);
}

/** @}*/

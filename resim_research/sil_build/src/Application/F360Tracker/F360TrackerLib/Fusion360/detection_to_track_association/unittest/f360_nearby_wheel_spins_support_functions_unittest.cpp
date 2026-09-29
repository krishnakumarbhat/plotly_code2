/** \file
 * This file contains unit tests for content of f360_near_by_wheel_spins_support_functions.cpp file
 */

#include "f360_nearby_wheel_spins_support_functions.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_internal_preprocessing.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_near_by_wheel_spins_support_functions_cluster_verification
 *  @{
 */

/** \brief
 * It checks if correct values of cluster are returned by Find_Next_Wheel_Spin_Cluster()
 * for various situations.
 */
TEST_GROUP(f360_near_by_wheel_spins_support_functions_cluster_verification)
{
   F360_Calibrations_T calibrations;

   F360_Detection_Props_T det_properties[MAX_NUMBER_OF_DETECTIONS];
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   Wheel_Spin_Internals_T wheel_spins_internals;
   BoundingBox area_of_interest;
   int num_of_dets;

   /** \setup
    * Describe what is done in test setup. Remove test setup function and this tag if it is not used.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      area_of_interest = BoundingBox{ Point{calibrations.k_nbws_min_long_pos, calibrations.k_nbws_min_lat_pos}, Point{calibrations.k_nbws_max_long_pos, calibrations.k_nbws_max_lat_pos} };

      num_of_dets = 1;
      raw_detect_list.number_of_valid_detections = num_of_dets;
      det_properties[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
      raw_detect_list.detections[0].processed.vcs_position_x = 3.0F;
      raw_detect_list.detections[0].processed.vcs_position_y = 2.0F;
      for (int32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
      {
         raw_detect_list.detections[i].raw.sensor_id = 1U;
      }
   }
};

/** \purpose  
 * Function should return invalid cluster if there is no single det
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins_support_functions_cluster_verification, Find_Next_Wheel_Spin_Cluster__no_dets)
{
   /** \precond
    * Remove det and sort
    */
   num_of_dets = 0;
   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_properties);
	
   /** \action
    * Call Find_Next_Wheel_Spin_Cluster()
    */
   Wheel_Spin_Cluster_T out_cluster = Find_Next_Wheel_Spin_Cluster(calibrations, raw_detect_list, area_of_interest, det_properties, wheel_spins_internals);

   /** \result
    * 
    */	
   CHECK_FALSE_TEXT(out_cluster.f_valid, "Wheel spin cluster should be invalid");
}

/** \purpose
 * Check if cluster data is filled correctly (case 1: one wheel spin det)
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins_support_functions_cluster_verification, Find_Next_Wheel_Spin_Cluster__check_cluster_data_filing_correctnes_case_1)
{
   /** \precond
    * Sort det
    */
   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_properties);

   /** \actionIt is no
    * Call Find_Next_Wheel_Spin_Cluster()
    */
   Wheel_Spin_Cluster_T out_cluster = Find_Next_Wheel_Spin_Cluster(calibrations, raw_detect_list, area_of_interest, det_properties, wheel_spins_internals);

   /** \result
    * Valid cluster with proper parmeters
    */
   CHECK_TRUE_TEXT(out_cluster.f_valid, "Wheel spin cluster should be valid");
   CHECK_EQUAL_TEXT(0, out_cluster.min_long_det_idx, "Minimum det idx sholud be 0");
   CHECK_EQUAL_TEXT(0, out_cluster.max_long_det_idx, "Maximum det idx sholud be 0");
   DOUBLES_EQUAL_TEXT(raw_detect_list.detections[0].processed.vcs_position_y, out_cluster.min_lat_pos, F360_EPSILON, "Wrong cluster minimum lateral position");
   DOUBLES_EQUAL_TEXT(raw_detect_list.detections[0].processed.vcs_position_y, out_cluster.max_lat_pos, F360_EPSILON, "Wrong cluster maximum lateral position");
}

/** \purpose
 * Check if cluster data is filled correctly (case 2: three wheel spins dets)
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins_support_functions_cluster_verification, Find_Next_Wheel_Spin_Cluster__check_cluster_data_filing_correctnes_case_2)
{
   /** \precond
    * Add two dets and sort them
    */
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   raw_detect_list.detections[1].processed.vcs_position_x = 3.2F;
   raw_detect_list.detections[1].processed.vcs_position_y = 1.9F;
   num_of_dets++;

   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   raw_detect_list.detections[2].processed.vcs_position_x = 3.3F;
   raw_detect_list.detections[2].processed.vcs_position_y = 2.1F;
   num_of_dets++;
   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_properties);

   /** \action
    * Call Find_Next_Wheel_Spin_Cluster()
    */
   Wheel_Spin_Cluster_T out_cluster = Find_Next_Wheel_Spin_Cluster(calibrations, raw_detect_list, area_of_interest, det_properties, wheel_spins_internals);

   /** \result
    * Valid cluster with proper parmeters
    */
   CHECK_TRUE_TEXT(out_cluster.f_valid, "Wheel spin cluster should be valid");
   CHECK_EQUAL_TEXT(0, out_cluster.min_long_det_idx, "Minimum det idx sholud be 0");
   CHECK_EQUAL_TEXT(2, out_cluster.max_long_det_idx, "Maximum det idx sholud be 2");
   DOUBLES_EQUAL_TEXT(raw_detect_list.detections[1].processed.vcs_position_y, out_cluster.min_lat_pos, F360_EPSILON, "Wrong cluster minimum lateral position");
   DOUBLES_EQUAL_TEXT(raw_detect_list.detections[2].processed.vcs_position_y, out_cluster.max_lat_pos, F360_EPSILON, "Wrong cluster maximum lateral position");
}

/** \purpose
 * Check if cluster data is filled correctly (case 3: two wheel spins dets and one non wheel spin between)
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins_support_functions_cluster_verification, Find_Next_Wheel_Spin_Cluster__check_cluster_data_filing_correctnes_case_3)
{
   /** \precond
    * Sort dets
    */
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   raw_detect_list.detections[1].processed.vcs_position_x = 3.2F;
   raw_detect_list.detections[1].processed.vcs_position_y = 1.9F;
   num_of_dets++;

   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   raw_detect_list.detections[2].processed.vcs_position_x = 3.1F;
   raw_detect_list.detections[2].processed.vcs_position_y = 2.1F;
   num_of_dets++;
   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_properties);

   /** \action
    * Call Find_Next_Wheel_Spin_Cluster()
    */
   Wheel_Spin_Cluster_T out_cluster = Find_Next_Wheel_Spin_Cluster(calibrations, raw_detect_list, area_of_interest, det_properties, wheel_spins_internals);

   /** \result
    *
    */
   CHECK_TRUE_TEXT(out_cluster.f_valid, "Wheel spin cluster should be valid");
   CHECK_EQUAL_TEXT(0, out_cluster.min_long_det_idx, "Minimum det idx sholud be 0");
   CHECK_EQUAL_TEXT(1, out_cluster.max_long_det_idx, "Maximum det idx sholud be 1");
   DOUBLES_EQUAL_TEXT(raw_detect_list.detections[1].processed.vcs_position_y, out_cluster.min_lat_pos, F360_EPSILON, "Wrong cluster minimum lateral position");
   DOUBLES_EQUAL_TEXT(raw_detect_list.detections[0].processed.vcs_position_y, out_cluster.max_lat_pos, F360_EPSILON, "Wrong cluster maximum lateral position");
}
/** @}*/


/** \defgroup  f360_near_by_wheel_spins_support_functions_internal_data
 *  @{
 */

 /** \brief
  * It is used to check if internal data of clustering is properly 
  * handled.
  */
TEST_GROUP(f360_near_by_wheel_spins_support_functions_internal_data)
{
   F360_Calibrations_T calibrations;

   F360_Detection_Props_T det_properties[MAX_NUMBER_OF_DETECTIONS];
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   Wheel_Spin_Internals_T wheel_spins_internals;
   BoundingBox area_of_interest;
   int num_of_dets;

   /** \setup
    * Createa one detection, set are of interest and init calibrations and internals data
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      area_of_interest = BoundingBox{ Point{calibrations.k_nbws_min_long_pos, calibrations.k_nbws_min_lat_pos}, Point{calibrations.k_nbws_max_long_pos, calibrations.k_nbws_max_lat_pos} };

      num_of_dets = 1;
      raw_detect_list.number_of_valid_detections = num_of_dets;
      det_properties[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
      raw_detect_list.detections[0].processed.vcs_position_x = 3.0F;
      raw_detect_list.detections[0].processed.vcs_position_y = 2.0F;
      for (int32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
      {
         raw_detect_list.detections[i].raw.sensor_id = 1U;
      }
   }
};

/** \purpose
 * Check internal data corrrectnes if there is no dets
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins_support_functions_internal_data, Find_Next_Wheel_Spin_Cluster__no_dets)
{
   /** \precond
    * "Clear" detections and sort
    */
   num_of_dets = 0;
   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_properties);

   /** \action
    * Call Find_Next_Wheel_Spin_Cluster()
    */
   (void)Find_Next_Wheel_Spin_Cluster(calibrations, raw_detect_list, area_of_interest, det_properties, wheel_spins_internals);

   /** \result
    * Internal data should be filled with invalid values
    */
   CHECK_FALSE_TEXT(wheel_spins_internals.f_first_call, "First call should be set to false");
   CHECK_EQUAL_TEXT(F360_INVALID_ID, wheel_spins_internals.next_starting_det_idx, "Next starting idx should be -1");
   CHECK_EQUAL_TEXT(F360_INVALID_ID, wheel_spins_internals.prev_ws_det_idx, "Previous wheel spin det should be -1");
}

/** \purpose
 * Check internal data corrrectnes if there is only one wheel spin det
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins_support_functions_internal_data, Find_Next_Wheel_Spin_Cluster__one_ws_det)
{
   /** \precond
    * Sort dets
    */
   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_properties);

   /** \action
    * Call Find_Next_Wheel_Spin_Cluster()
    */
   (void)Find_Next_Wheel_Spin_Cluster(calibrations, raw_detect_list, area_of_interest, det_properties, wheel_spins_internals);

   /** \result
    * No next starting det is determined
    */
   CHECK_FALSE_TEXT(wheel_spins_internals.f_first_call, "First call should be set to false");
   CHECK_EQUAL_TEXT(F360_INVALID_ID, wheel_spins_internals.next_starting_det_idx, "Next starting idx should be -1");
   CHECK_EQUAL_TEXT(0, wheel_spins_internals.prev_ws_det_idx, "Previous wheel spin det should be 0");
}

/** \purpose
 * Check internal data corrrectnes if there is only one non-wheel spin det
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins_support_functions_internal_data, Find_Next_Wheel_Spin_Cluster__one_no_ws_det)
{
   /** \precond
    * Sort dets
    */
   det_properties[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;

   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_properties);

   /** \action
    * Call Find_Next_Wheel_Spin_Cluster()
    */
   (void)Find_Next_Wheel_Spin_Cluster(calibrations, raw_detect_list, area_of_interest, det_properties, wheel_spins_internals);

   /** \result
    * Internal data should be filled with invalid values
    */
   CHECK_FALSE_TEXT(wheel_spins_internals.f_first_call, "First call should be set to false");
   CHECK_EQUAL_TEXT(F360_INVALID_ID, wheel_spins_internals.next_starting_det_idx, "Next starting idx should be -1");
   CHECK_EQUAL_TEXT(F360_INVALID_ID, wheel_spins_internals.prev_ws_det_idx, "Previous wheel spin det should be -1");
}

/** \purpose
 * Check internal data corrrectnes if there are two clusters (one det each)
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins_support_functions_internal_data, Find_Next_Wheel_Spin_Cluster__two_clusters_one_loop)
{
   /** \precond
    * Add wheel spin det and sort them
    */
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   raw_detect_list.detections[1].processed.vcs_position_x = 6.0F;
   raw_detect_list.detections[1].processed.vcs_position_y = 2.0F;
   num_of_dets++;
   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_properties);

   /** \action
    * Call Find_Next_Wheel_Spin_Cluster()
    */
   (void)Find_Next_Wheel_Spin_Cluster(calibrations, raw_detect_list, area_of_interest, det_properties, wheel_spins_internals);

   /** \result
    * Properly filled internal data
    */
   CHECK_FALSE_TEXT(wheel_spins_internals.f_first_call, "First call should be set to false");
   CHECK_EQUAL_TEXT(1, wheel_spins_internals.next_starting_det_idx, "Next starting idx should be 1");
   CHECK_EQUAL_TEXT(0, wheel_spins_internals.prev_ws_det_idx, "Previous wheel spin det should be 0");
}

/** \purpose
 * Check internal data corrrectnes if there are one cluster (two dets) and one wheels spin det lateraly far away
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins_support_functions_internal_data, Find_Next_Wheel_Spin_Cluster__one_cluster_of_two_dets_and_one_ws_det_far_away)
{
   /** \precond
    * Add two wheel spin dets and sort them
    */
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   raw_detect_list.detections[1].processed.vcs_position_x = raw_detect_list.detections[0].processed.vcs_position_x + calibrations.k_nbws_long_asc_th - 0.0001;
   raw_detect_list.detections[1].processed.vcs_position_y = raw_detect_list.detections[0].processed.vcs_position_y;
   num_of_dets++;

   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   raw_detect_list.detections[2].processed.vcs_position_x = raw_detect_list.detections[0].processed.vcs_position_x + calibrations.k_nbws_long_asc_th - 2*0.0001;
   raw_detect_list.detections[2].processed.vcs_position_y = raw_detect_list.detections[0].processed.vcs_position_y + calibrations.k_nbws_lat_asc_th + 0.0001;
   num_of_dets++;
   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_properties);

   /** \action
    * Call Find_Next_Wheel_Spin_Cluster()
    */
   (void)Find_Next_Wheel_Spin_Cluster(calibrations, raw_detect_list, area_of_interest, det_properties, wheel_spins_internals);

   /** \result
    * Properly filled internal data
    */
   CHECK_FALSE_TEXT(wheel_spins_internals.f_first_call, "First call should be set to false");
   CHECK_EQUAL_TEXT(2, wheel_spins_internals.next_starting_det_idx, "Next starting idx should be 2");
   CHECK_EQUAL_TEXT(1, wheel_spins_internals.prev_ws_det_idx, "Previous wheel spin det should be 1");
}

/** \purpose
 * Check internal data corrrectnes if there are two clusters (one det each) after full loop
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins_support_functions_internal_data, Find_Next_Wheel_Spin_Cluster__two_clusters_full_loop)
{
   /** \precond
    * Add one wheel spin det and sort them
    */
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   raw_detect_list.detections[1].processed.vcs_position_x = 6.0F;
   raw_detect_list.detections[1].processed.vcs_position_y = 2.0F;
   num_of_dets++;
   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_properties);

   /** \action
    * Call Find_Next_Wheel_Spin_Cluster()
    */
   for(int i = 0; i < 10; i++)
   {
      Wheel_Spin_Cluster_T out_cluster = Find_Next_Wheel_Spin_Cluster(calibrations, raw_detect_list, area_of_interest, det_properties, wheel_spins_internals);

      if (!out_cluster.f_valid)
      {
         break;
      }
   }
   
   /** \result
    * No next starting det is determined
    */
   CHECK_FALSE_TEXT(wheel_spins_internals.f_first_call, "First call should be set to false");
   CHECK_EQUAL_TEXT(F360_INVALID_ID, wheel_spins_internals.next_starting_det_idx, "Next starting idx should be -1");
   CHECK_EQUAL_TEXT(1, wheel_spins_internals.prev_ws_det_idx, "Previous wheel spin det should be 1");
}

/** \purpose
 * Check if longitudinally too far det will be set as next new starting cluster point
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins_support_functions_internal_data, Find_Next_Wheel_Spin_Cluster___longitudinally_too_far_as_new_starting_point)
{
   /** \precond
    * Add two wheel spin dets and sort them
    */
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   raw_detect_list.detections[1].processed.vcs_position_x = raw_detect_list.detections[0].processed.vcs_position_x + calibrations.k_nbws_long_asc_th + 0.0001;
   raw_detect_list.detections[1].processed.vcs_position_y = raw_detect_list.detections[0].processed.vcs_position_y + calibrations.k_nbws_lat_asc_th + 0.0001;
   num_of_dets++;

   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   raw_detect_list.detections[2].processed.vcs_position_x = raw_detect_list.detections[0].processed.vcs_position_x + calibrations.k_nbws_long_asc_th - 0.0001;
   raw_detect_list.detections[2].processed.vcs_position_y = raw_detect_list.detections[0].processed.vcs_position_y + calibrations.k_nbws_lat_asc_th + 0.0001;
   num_of_dets++;
   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_properties);

   /** \action
    * Call Find_Next_Wheel_Spin_Cluster()
    */
   (void)Find_Next_Wheel_Spin_Cluster(calibrations, raw_detect_list, area_of_interest, det_properties, wheel_spins_internals);

   /** \result
    * Properly filled internal data
    */
   CHECK_FALSE_TEXT(wheel_spins_internals.f_first_call, "First call should be set to false");
   CHECK_EQUAL_TEXT(2, wheel_spins_internals.next_starting_det_idx, "Next starting idx should be 2");
   CHECK_EQUAL_TEXT(0, wheel_spins_internals.prev_ws_det_idx, "Previous wheel spin det should be 0");
}

/** \purpose
 * Check if laterally too far det will be set as next new starting cluster point
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins_support_functions_internal_data, Find_Next_Wheel_Spin_Cluster___laterally_too_far_as_new_starting_point)
{
   /** \precond
    * Add two wheel spin dets and sort them
    */
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   raw_detect_list.detections[1].processed.vcs_position_x = raw_detect_list.detections[0].processed.vcs_position_x + calibrations.k_nbws_long_asc_th - 2 * 0.0001;
   raw_detect_list.detections[1].processed.vcs_position_y = raw_detect_list.detections[0].processed.vcs_position_y + calibrations.k_nbws_lat_asc_th + 0.0001;
   num_of_dets++;

   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   raw_detect_list.detections[2].processed.vcs_position_x = calibrations.k_nbws_max_long_pos + 0.0001;
   raw_detect_list.detections[2].processed.vcs_position_y = raw_detect_list.detections[0].processed.vcs_position_y;
   num_of_dets++;
   raw_detect_list.detections[1].raw.sensor_id = 1U;
   raw_detect_list.detections[2].raw.sensor_id = 1U;
   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_properties);

   /** \action
    * Call Find_Next_Wheel_Spin_Cluster()
    */
   (void)Find_Next_Wheel_Spin_Cluster(calibrations, raw_detect_list, area_of_interest, det_properties, wheel_spins_internals);

   /** \result
    * Properly filled internal data
    */
   CHECK_FALSE_TEXT(wheel_spins_internals.f_first_call, "First call should be set to false");
   CHECK_EQUAL_TEXT(1, wheel_spins_internals.next_starting_det_idx, "Next starting idx should be 1");
   CHECK_EQUAL_TEXT(0, wheel_spins_internals.prev_ws_det_idx, "Previous wheel spin det should be 0");
}

/** \purpose
 * Check if laterally too far det will not be set as next new starting cluster point
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins_support_functions_internal_data, Find_Next_Wheel_Spin_Cluster___laterally_too_far_and_not_new_starting_point)
{
   /** \precond
    * Add two wheel spin dets and sort them
    */
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   raw_detect_list.detections[1].processed.vcs_position_x = raw_detect_list.detections[0].processed.vcs_position_x + calibrations.k_nbws_long_asc_th - 2 * 0.0001;
   raw_detect_list.detections[1].processed.vcs_position_y = raw_detect_list.detections[0].processed.vcs_position_y + calibrations.k_nbws_lat_asc_th + 0.0001;
   num_of_dets++;

   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   raw_detect_list.detections[2].processed.vcs_position_x = calibrations.k_nbws_max_long_pos + 0.0001;
   raw_detect_list.detections[2].processed.vcs_position_y = raw_detect_list.detections[0].processed.vcs_position_y;
   num_of_dets++;

   det_properties[3].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   raw_detect_list.detections[3].processed.vcs_position_x = raw_detect_list.detections[0].processed.vcs_position_x + calibrations.k_nbws_long_asc_th - 3 * 0.0001;
   raw_detect_list.detections[3].processed.vcs_position_y = raw_detect_list.detections[0].processed.vcs_position_y + calibrations.k_nbws_lat_asc_th + 0.0001;
   num_of_dets++;

   raw_detect_list.detections[1].raw.sensor_id = 1U;
   raw_detect_list.detections[2].raw.sensor_id = 1U;
   raw_detect_list.detections[3].raw.sensor_id = 1U;
   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);
   Copy_Detections_Info(raw_detect_list, sensors, det_properties);

   /** \action
    * Call Find_Next_Wheel_Spin_Cluster()
    */
   (void)Find_Next_Wheel_Spin_Cluster(calibrations, raw_detect_list, area_of_interest, det_properties, wheel_spins_internals);

   /** \result
    * Properly filled internal data
    */
   CHECK_FALSE_TEXT(wheel_spins_internals.f_first_call, "First call should be set to false");
   CHECK_EQUAL_TEXT(3, wheel_spins_internals.next_starting_det_idx, "Next starting idx should be 3");
   CHECK_EQUAL_TEXT(0, wheel_spins_internals.prev_ws_det_idx, "Previous wheel spin det should be 0");
}
/** @}*/

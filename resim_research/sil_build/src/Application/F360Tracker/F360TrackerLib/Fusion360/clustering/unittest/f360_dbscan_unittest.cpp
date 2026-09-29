/** \file
   Give a detailed description of what  this unit-test file contain.
*/

#include "f360_dbscan.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

using namespace f360_variant_A;

/** \defgroup  f360_dbscan
 *  @{
 */

/** \brief
*  Test group for dbscan for moving detections.
**/
TEST_GROUP(f360_dbscan_moving_dets)
{
   /** \setup
   * Set needed inputs to default values.
   **/      
   F360_Tracker_Info_T tracker_info = {};
   bool valid_dets[MAX_NUMBER_OF_DETECTIONS]{};
   uint32_t num_det_pts;
   F360_Detection_Props_T detections[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Local_Clusters_T output_data;
   rspp_variant_A::RSPP_Detection_List_T raw_detections{};
   int16_t detection_list[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   const bool f_cluster_moving = true;

   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);

      std::fill(std::begin(valid_dets), std::end(valid_dets), false);
      std::fill(std::begin(detections), std::end(detections), F360_Detection_Props_T{});
      num_det_pts = 0U;
      for (uint32_t det_index = 0U; det_index < MAX_NUMBER_OF_DETECTIONS; det_index++)
      {
         raw_detections.detections[det_index].processed.next_sorted_idx = F360_INVALID_ID;
         raw_detections.detections[det_index].processed.prev_sorted_idx = F360_INVALID_ID;
         raw_detections.detections[det_index].raw.sensor_id = 1;
      }

      output_data = {};
   }
};

/**
*\purpose  Check function behaviour for single detection as an input.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, single_detection)
{
   /** \precond
   Set single detection position values. 
   **/
   num_det_pts = 1;
   detections[0].vcs_position = { 0.0, 0.0 };
   valid_dets[0] = true;
   detection_list[0] = 0;

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if cluster was created and ensure number of detections index inside cluster
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 1U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 1U, "Number of detections inside cluster is not equal to expected value");  
}

/**
*\purpose  Check function behaviour for two close detections as an input.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, two_close_detections)
{
   /** \precond
   Set detections properties.
   **/
   num_det_pts = 2;
   raw_detections.number_of_valid_detections = num_det_pts;
   
   detections[0].vcs_position = { 0.0F, 0.0F };
   detections[1].vcs_position = { 1.0F, 0.0F };
   // Set next detection only to set next_sorted_idx for detection with index 1
   detections[2].vcs_position = { 1.2F, 0.0F };

   raw_detections.detections[0].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.next_sorted_idx = 2;
   raw_detections.detections[2].processed.prev_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 0;

   //prepare detection list
   for (uint32_t det_index = 0U; det_index < num_det_pts; det_index++)
   {
      detection_list[det_index] = det_index;
      valid_dets[det_index] = true;
   }

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if cluster was created and ensure number of detections index inside cluster
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 1U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 2U, "Number of detections inside cluster is not equal to expected value");
}

/**
*\purpose  Check function behaviour for two close detections as an input. Similar case to two_close_detections but with inversed indexing.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, two_close_detections_inverse_indexing)
{
   /** \precond
   Set detections properties.
   **/
   num_det_pts = 2;
   raw_detections.number_of_valid_detections = num_det_pts;

   detections[0].vcs_position = { 1.2F, 0.0F };
   detections[1].vcs_position = { 1.0F, 0.0F };
   // Set next detection only to set next_sorted_idx for detection with index 0
   detections[2].vcs_position = { 0.0F, 0.0F };

   raw_detections.detections[2].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.next_sorted_idx = 0;
   raw_detections.detections[0].processed.prev_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 2;

   //prepare detection list
   for (uint32_t det_index = 0U; det_index < num_det_pts; det_index++)
   {
      detection_list[det_index] = det_index;
      valid_dets[det_index] = true;
   }

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if cluster was created and ensure number of detections index inside cluster
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 1U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 2U, "Number of detections inside cluster is not equal to expected value");
}

/**
*\purpose  Check function behaviour for three detections as an input - two close and third is far.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, two_close_one_far_detections)
{
   /** \precond
   Set detections properties
   **/
   num_det_pts = 3;
   raw_detections.number_of_valid_detections = num_det_pts;

   detections[0].vcs_position = { 1.0F, 1.0F };
   detections[1].vcs_position = { 1.5F, 1.2F };
   detections[2].vcs_position = { 5.0F, 5.0F };

   raw_detections.detections[0].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.next_sorted_idx = 2;
   raw_detections.detections[2].processed.prev_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 0;

   //prepare detection list
   for (uint32_t det_index = 0U; det_index < num_det_pts; det_index++)
   {
      detection_list[det_index] = det_index;
      valid_dets[det_index] = true;
   }

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if clusters were created and ensure number of detections inside clusters
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 2U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 2U, "Number of detections inside cluster is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[1] == 1U, "Number of detections inside cluster is not equal to expected value");

}

/**
*\purpose  Check function behaviour for three detections as an input - all are close to each other.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, three_close_detections)
{
   /** \precond
   Set detection position values.
   **/
   num_det_pts = 3;
   raw_detections.number_of_valid_detections = num_det_pts;

   // Set associated detection state: first detection point (1.0, 1.0) WCS
   detections[0].vcs_position = { 1.0F, 1.0F };

   // Second detection point (1.5, 1.2) WCS
   detections[1].vcs_position = { 1.5F, 1.2F };

   // Third detection point(2.0, 2.0) WCS
   detections[2].vcs_position = { 2.0F, 2.0F };

   raw_detections.detections[0].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.next_sorted_idx = 2;
   raw_detections.detections[2].processed.prev_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 0;

   //prepare detection list
   for (uint32_t det_index = 0U; det_index < num_det_pts; det_index++)
   {
      detection_list[det_index] = det_index;
      valid_dets[det_index] = true;
   }

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if cluster was created and ensure number of detections inside cluster
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 1U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 3U, "Number of detections inside cluster is not equal to expected value");
}

/**
*\purpose  Check function behaviour for three detections as an input - all are close to each other, but second is not valid.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, three_close_detections_second_not_valid)
{
   /** \precond
   Set detection position values.
   **/
   num_det_pts = 3;
   raw_detections.number_of_valid_detections = num_det_pts;

   // Set associated detection state: first detection point (1.0, 1.0) WCS
   detections[0].vcs_position = { 1.0F, 1.0F };

   // Second detection point (1.5, 1.2) WCS
   detections[1].vcs_position = { 1.5F, 1.2F };

   // Third detection point(2.0, 2.0) WCS
   detections[2].vcs_position = { 2.0F, 2.0F };

   raw_detections.detections[0].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.next_sorted_idx = 2;
   raw_detections.detections[2].processed.prev_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 0;

   valid_dets[0] = true;
   valid_dets[1] = false;
   valid_dets[2] = true;

   detection_list[0] = 0;
   detection_list[1] = 2;
   
   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if cluster was created and ensure number of detections inside cluster
   **/
   CHECK_EQUAL_TEXT(1, output_data.num_clusters, "Unexpected number of clusters");
   CHECK_EQUAL_TEXT(2, output_data.num_dets_in_clusters[0], "Unexpected number of detections in 1st cluster");
}

/**
*\purpose  Check function behaviour for three detections as an input - all are close to each other but one have larger range rate.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, three_close_detections_but_one_large_rrate)
{
   /** \precond
   Set detection position values.
   **/
   num_det_pts = 3;
   raw_detections.number_of_valid_detections = num_det_pts;

   detections[0].vcs_position = { 1.0F, 1.0F };
   detections[1].vcs_position = { 1.5F, 1.2F };
   detections[2].vcs_position = { 2.0F, 2.0F };
   detections[2].range_rate_dealiased = 10.0F;

   raw_detections.detections[0].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.next_sorted_idx = 2;
   raw_detections.detections[2].processed.prev_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 0;

   //prepare detection list
   for (uint32_t det_index = 0U; det_index < num_det_pts; det_index++)
   {
      detection_list[det_index] = det_index;
      valid_dets[det_index] = true;
   }

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if clusters were created and ensure number of detections inside clusters
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 2U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 2U, "Number of detections inside cluster is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[1] == 1U, "Number of detections inside cluster is not equal to expected value");
}

/**
*\purpose  Check function behaviour for four detections as an input - all are located in single straight line.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, four_detections_in_same_line)
{
   /** \precond
   Set detection position values.
   **/

   num_det_pts = 4;
   raw_detections.number_of_valid_detections = num_det_pts;

   detections[0].vcs_position = { 1.0F, 0.0F };
   detections[1].vcs_position = { 2.0F, 0.0F };
   detections[2].vcs_position = { 2.5F, 0.0F };
   detections[3].vcs_position = { 3.5F, 0.0F };

   raw_detections.detections[0].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.next_sorted_idx = 2;
   raw_detections.detections[2].processed.next_sorted_idx = 3;
   raw_detections.detections[3].processed.prev_sorted_idx = 2;
   raw_detections.detections[2].processed.prev_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 0;

   //prepare detection list
   for (uint32_t det_index = 0U; det_index < num_det_pts; det_index++)
   {
      detection_list[det_index] = det_index;
      valid_dets[det_index] = true;
   }

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if cluster was created and number of detections inside cluster.
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 1U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 4U, "Number of detections inside cluster is not equal to expected value");
}

/**
*\purpose  Check function behaviour for four detections as an input - 1st and 3rd should be in same cluster and 2nd and 4th in other.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, four_detections_mixed_clusters)
{
   /** \precond
   Set detection position values.
   **/

   num_det_pts = 4;
   raw_detections.number_of_valid_detections = num_det_pts;

   detections[0].vcs_position = { 1.0F, 1.0F };
   detections[1].vcs_position = { 1.5F, 10.0F };
   detections[2].vcs_position = { 2.0F, 2.0F };
   detections[3].vcs_position = { 2.5F, 10.0F };

   raw_detections.detections[0].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.next_sorted_idx = 2;
   raw_detections.detections[2].processed.next_sorted_idx = 3;
   raw_detections.detections[3].processed.prev_sorted_idx = 2;
   raw_detections.detections[2].processed.prev_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 0;

   //prepare detection list
   for (uint32_t det_index = 0U; det_index < num_det_pts; det_index++)
   {
      detection_list[det_index] = det_index;
      valid_dets[det_index] = true;
   }

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if cluster was created and number of detections inside cluster.
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 2U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 2U, "Number of detections inside cluster is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[1] == 2U, "Number of detections inside cluster is not equal to expected value");
}

/**
*\purpose  Check function behaviour for four detections as an input - all are located in single straight line but two inner detections have similar range rates.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, four_detections_in_same_line_two_inner_similar_rrates)
{
   /** \precond
   Set detection position values.
   **/
   num_det_pts = 4;
   raw_detections.number_of_valid_detections = num_det_pts;

   detections[0].vcs_position = { 1.0F, 0.0F };
   detections[1].vcs_position = { 2.0F, 0.0F };
   detections[1].range_rate_dealiased = 10.0F;
   detections[2].vcs_position = { 2.5F, 0.0F };
   detections[2].range_rate_dealiased = 11.0F;
   detections[3].vcs_position = { 3.5F, 0.0F };

   raw_detections.detections[0].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.next_sorted_idx = 2;
   raw_detections.detections[2].processed.next_sorted_idx = 3;
   raw_detections.detections[3].processed.prev_sorted_idx = 2;
   raw_detections.detections[2].processed.prev_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 0;

   //prepare detection list
   for (uint32_t det_index = 0U; det_index < num_det_pts; det_index++)
   {
      detection_list[det_index] = det_index;
      valid_dets[det_index] = true;
   }

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if clusters were created and ensure number of detections inside clusters
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 3U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 1U, "Number of detections inside cluster is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[1] == 2U, "Number of detections inside cluster is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[2] == 1U, "Number of detections inside cluster is not equal to expected value");

}

/**
*\purpose  Check function behaviour for four detections as an input - all are located in single straight line, but all have diferent range rates.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, four_detections_in_same_line_four_different_rrates)
{
   /** \precond
   Set detection position values.
   **/
   num_det_pts = 4;
   detections[0].vcs_position = { 1.0F, 0.0F };
   detections[1].vcs_position = { 2.0F, 0.0F };
   detections[1].range_rate_dealiased = 3.0F;

   detections[2].vcs_position = { 2.5F, 0.0F };
   detections[2].range_rate_dealiased = 10.0F;

   detections[3].vcs_position = { 3.5F, 0.0F };
   detections[3].range_rate_dealiased = 15.0F;

   raw_detections.detections[0].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.next_sorted_idx = 2;
   raw_detections.detections[2].processed.next_sorted_idx = 3;
   raw_detections.detections[3].processed.prev_sorted_idx = 2;
   raw_detections.detections[2].processed.prev_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 0;

   //prepare detection list
   for (uint32_t det_index = 0U; det_index < num_det_pts; det_index++)
   {
      detection_list[det_index] = det_index;
      valid_dets[det_index] = true;
   }

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if cluster was created and ensure detection index inside cluster
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 4U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 1U, "Number of detections inside cluster is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[1] == 1U, "Number of detections inside cluster is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[2] == 1U, "Number of detections inside cluster is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[3] == 1U, "Number of detections inside cluster is not equal to expected value");

}

/**
*\purpose  Check function behaviour for six detections in hexagonal shape as an input.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, six_dets_hexagonal_shape)
{
   /** \precond
   Set detection position values.
   **/

   num_det_pts = 6;
   raw_detections.number_of_valid_detections = num_det_pts;
   const float32_t sqrt_3 = 1.7320508F;

   // Set associated detection state: first detection point (-1.0, 0.0) WCS
   detections[0].vcs_position = { -1.0F, 0.0F };
   detections[1].vcs_position = { -0.5F, -sqrt_3 };
   detections[2].vcs_position = { 0.5F, sqrt_3 };
   detections[3].vcs_position = { 1.0F, 0.0F };
   detections[4].vcs_position = { 0.5F, sqrt_3 };
   detections[5].vcs_position = { -0.5F, sqrt_3 };

   raw_detections.detections[0].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.next_sorted_idx = 5;
   raw_detections.detections[5].processed.next_sorted_idx = 2;
   raw_detections.detections[2].processed.next_sorted_idx = 4;
   raw_detections.detections[4].processed.next_sorted_idx = 3;

   raw_detections.detections[3].processed.prev_sorted_idx = 4;
   raw_detections.detections[4].processed.prev_sorted_idx = 2;
   raw_detections.detections[2].processed.prev_sorted_idx = 5;
   raw_detections.detections[5].processed.prev_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 0;

   //prepare detection list
   for (uint32_t det_index = 0U; det_index < num_det_pts; det_index++)
   {
      detection_list[det_index] = det_index;
      valid_dets[det_index] = true;
   }

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if cluster was created and ensure detection index inside cluster
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 1U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 6U, "Number of detections inside cluster is not equal to expected value");
}

/**
*\purpose  Check function behaviour for six detections in hexagonal shape + one outlier as an input.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, six_dets_hexagonal_shape_one_outlier)
{
   /** \precond
   Set detection position values.
   **/

   num_det_pts = 7;
   raw_detections.number_of_valid_detections = num_det_pts;
   const float32_t sqrt_3 = 1.7320508F;

   // Set associated detection state: first detection point (-1.0, 0.0) WCS
   detections[0].vcs_position = { -1.0F, 0.0F };
   detections[1].vcs_position = { -0.5F, -sqrt_3 };
   detections[2].vcs_position = { 0.5F, sqrt_3 };
   detections[3].vcs_position = { 1.0F, 0.0F };
   detections[4].vcs_position = { 0.5F, sqrt_3 };
   detections[5].vcs_position = { -0.5F, sqrt_3 };
   detections[6].vcs_position = { 10.0F, sqrt_3 };

   raw_detections.detections[0].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.next_sorted_idx = 5;
   raw_detections.detections[5].processed.next_sorted_idx = 2;
   raw_detections.detections[2].processed.next_sorted_idx = 4;
   raw_detections.detections[4].processed.next_sorted_idx = 3;
   raw_detections.detections[3].processed.next_sorted_idx = 6;

   raw_detections.detections[6].processed.prev_sorted_idx = 3;
   raw_detections.detections[3].processed.prev_sorted_idx = 4;
   raw_detections.detections[4].processed.prev_sorted_idx = 2;
   raw_detections.detections[2].processed.prev_sorted_idx = 5;
   raw_detections.detections[5].processed.prev_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 0;

   //prepare detection list
   for (uint32_t det_index = 0U; det_index < num_det_pts; det_index++)
   {
      detection_list[det_index] = det_index;
      valid_dets[det_index] = true;
   }

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if cluster was created and ensure detection index inside cluster
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 2U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 6U, "Number of detections inside cluster is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[1] == 1U, "Number of detections inside cluster is not equal to expected value");

}

/**
*\purpose  Check function behaviour for detections in spiral shape case. Visualization of spiral can be found: 
http://s03.delphiauto.net/04/RSE_2/active/ActiveSafetyAlgorithmGroup/F360Core/SysSWITnV/ENG06%20Software%20Detailed%20Design%20and%20Construct/UT_References/clustering_UT_spiral_case.PNG
*\req    NA
*/
TEST(f360_dbscan_moving_dets, spiral_shape)
{
   /** \precond
   Create 30 detections to be in spiral shape.
   **/
   num_det_pts = 30U;
   raw_detections.number_of_valid_detections = num_det_pts;
   
   detections[0].vcs_position = {0.00F, 0.00F };
   detections[1].vcs_position = {0.09F, 0.04F };
   detections[2].vcs_position = {0.13F, 0.15F };
   detections[3].vcs_position = {0.09F, 0.29F };
   detections[4].vcs_position = {-0.04F, 0.40F };
   detections[5].vcs_position = {-0.25F, 0.43F };
   detections[6].vcs_position = {-0.49F, 0.35F };
   detections[7].vcs_position = {-0.68F, 0.15F };
   detections[8].vcs_position = {-0.78F, -0.17F };
   detections[9].vcs_position = {-0.73F, -0.53F };
   detections[10].vcs_position = {-0.50F, -0.87F };
   detections[11].vcs_position = {-0.11F, -1.09F };
   detections[12].vcs_position = {0.37F, -1.14F };
   detections[13].vcs_position = {0.87F, -0.97F };
   detections[14].vcs_position = {1.28F, -0.57F };
   detections[15].vcs_position = {1.50F, -0.00F };
   detections[16].vcs_position = {1.46F, 0.65F };
   detections[17].vcs_position = {1.14F, 1.26F };
   detections[18].vcs_position = {0.56F, 1.71F };
   detections[19].vcs_position = {-0.20F, 1.89F };
   detections[20].vcs_position = {-1.00F, 1.73F };
   detections[21].vcs_position = {-1.70F, 1.23F };
   detections[22].vcs_position = {-2.15F, 0.46F };
   detections[23].vcs_position = {-2.25F, -0.48F };
   detections[24].vcs_position = {-1.94F, -1.41F };
   detections[25].vcs_position = {-1.25F, -2.17F };
   detections[26].vcs_position = {-0.27F, -2.59F };
   detections[27].vcs_position = {0.83F, -2.57F };
   detections[28].vcs_position = {1.87F, -2.08F };
   detections[29].vcs_position = {2.65F, -1.18F };

   raw_detections.detections[23].processed.next_sorted_idx = 22;
   raw_detections.detections[22].processed.next_sorted_idx = 24;
   raw_detections.detections[24].processed.next_sorted_idx = 21;
   raw_detections.detections[21].processed.next_sorted_idx = 25;
   raw_detections.detections[25].processed.next_sorted_idx = 20;
   raw_detections.detections[20].processed.next_sorted_idx = 8;
   raw_detections.detections[8].processed.next_sorted_idx = 9;
   raw_detections.detections[9].processed.next_sorted_idx = 7;
   raw_detections.detections[7].processed.next_sorted_idx = 10;
   raw_detections.detections[10].processed.next_sorted_idx = 6;
   raw_detections.detections[6].processed.next_sorted_idx = 26;
   raw_detections.detections[26].processed.next_sorted_idx = 5;
   raw_detections.detections[5].processed.next_sorted_idx = 19;
   raw_detections.detections[19].processed.next_sorted_idx = 11;
   raw_detections.detections[11].processed.next_sorted_idx = 4;
   raw_detections.detections[4].processed.next_sorted_idx = 0;
   raw_detections.detections[0].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.next_sorted_idx = 3;
   raw_detections.detections[3].processed.next_sorted_idx = 2;
   raw_detections.detections[2].processed.next_sorted_idx = 12;
   raw_detections.detections[12].processed.next_sorted_idx = 18;
   raw_detections.detections[18].processed.next_sorted_idx = 27;
   raw_detections.detections[27].processed.next_sorted_idx = 13;
   raw_detections.detections[13].processed.next_sorted_idx = 17;
   raw_detections.detections[17].processed.next_sorted_idx = 14;
   raw_detections.detections[14].processed.next_sorted_idx = 16;
   raw_detections.detections[16].processed.next_sorted_idx = 15;
   raw_detections.detections[15].processed.next_sorted_idx = 28;
   raw_detections.detections[28].processed.next_sorted_idx = 29;

   raw_detections.detections[29].processed.prev_sorted_idx = 28;
   raw_detections.detections[28].processed.prev_sorted_idx = 15;
   raw_detections.detections[15].processed.prev_sorted_idx = 16;
   raw_detections.detections[16].processed.prev_sorted_idx = 14;
   raw_detections.detections[14].processed.prev_sorted_idx = 17;
   raw_detections.detections[17].processed.prev_sorted_idx = 13;
   raw_detections.detections[13].processed.prev_sorted_idx = 27;
   raw_detections.detections[27].processed.prev_sorted_idx = 18;
   raw_detections.detections[18].processed.prev_sorted_idx = 12;
   raw_detections.detections[12].processed.prev_sorted_idx = 2;
   raw_detections.detections[2].processed.prev_sorted_idx = 3;
   raw_detections.detections[3].processed.prev_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 0;
   raw_detections.detections[0].processed.prev_sorted_idx = 4;
   raw_detections.detections[4].processed.prev_sorted_idx = 11;
   raw_detections.detections[11].processed.prev_sorted_idx = 19;
   raw_detections.detections[19].processed.prev_sorted_idx = 5;
   raw_detections.detections[5].processed.prev_sorted_idx = 26;
   raw_detections.detections[26].processed.prev_sorted_idx = 6;
   raw_detections.detections[6].processed.prev_sorted_idx = 10;
   raw_detections.detections[10].processed.prev_sorted_idx = 7;
   raw_detections.detections[7].processed.prev_sorted_idx = 9;
   raw_detections.detections[9].processed.prev_sorted_idx = 8;
   raw_detections.detections[8].processed.prev_sorted_idx = 20;
   raw_detections.detections[20].processed.prev_sorted_idx = 25;
   raw_detections.detections[25].processed.prev_sorted_idx = 21;
   raw_detections.detections[21].processed.prev_sorted_idx = 24;
   raw_detections.detections[24].processed.prev_sorted_idx = 22;
   raw_detections.detections[22].processed.prev_sorted_idx = 23;

   for (uint32_t index = 0U; index < num_det_pts; index++)
   {
      valid_dets[index] = true;
      detection_list[index] = index;
   }

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if cluster was created and ensure detection index inside cluster
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 1U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 30U, "Number of detections inside cluster is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[1] == 0U, "Number of detections inside cluster is not equal to expected value");

}


/**
*\purpose  Check function behaviour for detections in spiral shape when there are too many detections for a single cluster.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, spiral_shape_small_variant)
{
   /** \precond
   Create 30 detections to be in spiral shape.
   Update variant definition to allow a maximum of 20 detections in a cluster.
   **/
   num_det_pts = 30U;
   raw_detections.number_of_valid_detections = num_det_pts;
   
   detections[0].vcs_position = {0.00F, 0.00F };
   detections[1].vcs_position = {0.09F, 0.04F };
   detections[2].vcs_position = {0.13F, 0.15F };
   detections[3].vcs_position = {0.09F, 0.29F };
   detections[4].vcs_position = {-0.04F, 0.40F };
   detections[5].vcs_position = {-0.25F, 0.43F };
   detections[6].vcs_position = {-0.49F, 0.35F };
   detections[7].vcs_position = {-0.68F, 0.15F };
   detections[8].vcs_position = {-0.78F, -0.17F };
   detections[9].vcs_position = {-0.73F, -0.53F };
   detections[10].vcs_position = {-0.50F, -0.87F };
   detections[11].vcs_position = {-0.11F, -1.09F };
   detections[12].vcs_position = {0.37F, -1.14F };
   detections[13].vcs_position = {0.87F, -0.97F };
   detections[14].vcs_position = {1.28F, -0.57F };
   detections[15].vcs_position = {1.50F, -0.00F };
   detections[16].vcs_position = {1.46F, 0.65F };
   detections[17].vcs_position = {1.14F, 1.26F };
   detections[18].vcs_position = {0.56F, 1.71F };
   detections[19].vcs_position = {-0.20F, 1.89F };
   detections[20].vcs_position = {-1.00F, 1.73F };
   detections[21].vcs_position = {-1.70F, 1.23F };
   detections[22].vcs_position = {-2.15F, 0.46F };
   detections[23].vcs_position = {-2.25F, -0.48F };
   detections[24].vcs_position = {-1.94F, -1.41F };
   detections[25].vcs_position = {-1.25F, -2.17F };
   detections[26].vcs_position = {-0.27F, -2.59F };
   detections[27].vcs_position = {0.83F, -2.57F };
   detections[28].vcs_position = {1.87F, -2.08F };
   detections[29].vcs_position = {2.65F, -1.18F };

   raw_detections.detections[23].processed.next_sorted_idx = 22;
   raw_detections.detections[22].processed.next_sorted_idx = 24;
   raw_detections.detections[24].processed.next_sorted_idx = 21;
   raw_detections.detections[21].processed.next_sorted_idx = 25;
   raw_detections.detections[25].processed.next_sorted_idx = 20;
   raw_detections.detections[20].processed.next_sorted_idx = 8;
   raw_detections.detections[8].processed.next_sorted_idx = 9;
   raw_detections.detections[9].processed.next_sorted_idx = 7;
   raw_detections.detections[7].processed.next_sorted_idx = 10;
   raw_detections.detections[10].processed.next_sorted_idx = 6;
   raw_detections.detections[6].processed.next_sorted_idx = 26;
   raw_detections.detections[26].processed.next_sorted_idx = 5;
   raw_detections.detections[5].processed.next_sorted_idx = 19;
   raw_detections.detections[19].processed.next_sorted_idx = 11;
   raw_detections.detections[11].processed.next_sorted_idx = 4;
   raw_detections.detections[4].processed.next_sorted_idx = 0;
   raw_detections.detections[0].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.next_sorted_idx = 3;
   raw_detections.detections[3].processed.next_sorted_idx = 2;
   raw_detections.detections[2].processed.next_sorted_idx = 12;
   raw_detections.detections[12].processed.next_sorted_idx = 18;
   raw_detections.detections[18].processed.next_sorted_idx = 27;
   raw_detections.detections[27].processed.next_sorted_idx = 13;
   raw_detections.detections[13].processed.next_sorted_idx = 17;
   raw_detections.detections[17].processed.next_sorted_idx = 14;
   raw_detections.detections[14].processed.next_sorted_idx = 16;
   raw_detections.detections[16].processed.next_sorted_idx = 15;
   raw_detections.detections[15].processed.next_sorted_idx = 28;
   raw_detections.detections[28].processed.next_sorted_idx = 29;

   raw_detections.detections[29].processed.prev_sorted_idx = 28;
   raw_detections.detections[28].processed.prev_sorted_idx = 15;
   raw_detections.detections[15].processed.prev_sorted_idx = 16;
   raw_detections.detections[16].processed.prev_sorted_idx = 14;
   raw_detections.detections[14].processed.prev_sorted_idx = 17;
   raw_detections.detections[17].processed.prev_sorted_idx = 13;
   raw_detections.detections[13].processed.prev_sorted_idx = 27;
   raw_detections.detections[27].processed.prev_sorted_idx = 18;
   raw_detections.detections[18].processed.prev_sorted_idx = 12;
   raw_detections.detections[12].processed.prev_sorted_idx = 2;
   raw_detections.detections[2].processed.prev_sorted_idx = 3;
   raw_detections.detections[3].processed.prev_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 0;
   raw_detections.detections[0].processed.prev_sorted_idx = 4;
   raw_detections.detections[4].processed.prev_sorted_idx = 11;
   raw_detections.detections[11].processed.prev_sorted_idx = 19;
   raw_detections.detections[19].processed.prev_sorted_idx = 5;
   raw_detections.detections[5].processed.prev_sorted_idx = 26;
   raw_detections.detections[26].processed.prev_sorted_idx = 6;
   raw_detections.detections[6].processed.prev_sorted_idx = 10;
   raw_detections.detections[10].processed.prev_sorted_idx = 7;
   raw_detections.detections[7].processed.prev_sorted_idx = 9;
   raw_detections.detections[9].processed.prev_sorted_idx = 8;
   raw_detections.detections[8].processed.prev_sorted_idx = 20;
   raw_detections.detections[20].processed.prev_sorted_idx = 25;
   raw_detections.detections[25].processed.prev_sorted_idx = 21;
   raw_detections.detections[21].processed.prev_sorted_idx = 24;
   raw_detections.detections[24].processed.prev_sorted_idx = 22;
   raw_detections.detections[22].processed.prev_sorted_idx = 23;

   for (uint32_t index = 0U; index < num_det_pts; index++)
   {
      valid_dets[index] = true;
      detection_list[index] = index;
   }

   tracker_info.variant.num_dets_in_track = 20;

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check that two clusters were created with 20 and 10 associated detections respectively.
   **/
   CHECK_EQUAL(2, output_data.num_clusters);
   CHECK_EQUAL(20, output_data.num_dets_in_clusters[0]);
   CHECK_EQUAL(10, output_data.num_dets_in_clusters[1]);
}


/**
*\purpose  Check function behaviour for a line of detections with enough spacing that each detection have at most 2 other in range.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, line_shape_small_variant)
{
   /** \precond
   Create 30 detections in a line.
   Update variant definition to allow a maximum of 20 detections in a cluster.
   **/
   num_det_pts = 30U;
   raw_detections.number_of_valid_detections = num_det_pts;

   float32_t longpos = 0.0F;
   for (int32_t index = 0; index < static_cast<int32_t>(num_det_pts); index++)
   {
      detections[index].vcs_position = {longpos, 2.0F };
      longpos += 1.2F;

      raw_detections.detections[index].processed.prev_sorted_idx = index - 1;
      raw_detections.detections[index].processed.next_sorted_idx = index + 1;

      valid_dets[index] = true;
      detection_list[index] = index;
   }

   tracker_info.variant.num_dets_in_track = 20;

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check that two clusters were created with 20 and 10 associated detections respectively.
   **/
   CHECK_EQUAL(2, output_data.num_clusters);
   CHECK_EQUAL(20, output_data.num_dets_in_clusters[0]);
   CHECK_EQUAL(10, output_data.num_dets_in_clusters[1]);
}


/**
*\purpose  Check function behaviour if cluster number is saturated.
*\req    NA
*/
TEST(f360_dbscan_moving_dets, cluster_number_saturated)
{
   /** \precond
   Set number of detections to be greater than maximal allowed clusters number. Set detections position in way that
   each detection will create separate cluster.
   **/

   num_det_pts = MAX_TRACKER_POSN_CLUSTERS + 1;
   float32_t x_pos = 0.0F;
   for (uint32_t index = 0U; index < num_det_pts; index++)
   {
      detections[index].vcs_position = {x_pos, 0.0F };
      valid_dets[index] = true;
      x_pos += 2.01F;
      detection_list[index] = index;
   }

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check how many clusters was created
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == MAX_TRACKER_POSN_CLUSTERS, "Number of created clusters is not equal to expected value");
}

/** @}*/

/** \defgroup  f360_dbscan_interval_aware
 *  @{
 */

/** \brief
 * Test group for interval-aware range rate comparison logic in Cluster_Region_Query.
 * When clustering moving detections, same-sensor detections whose dealiased range rate
 * difference exceeds the direct gate can still be clustered if the wrapped (modulo v_wrapping)
 * difference falls within the gate. On a match the candidate detection's range rate and
 * position are compensated by one wrapping interval.
 */
TEST_GROUP(f360_dbscan_interval_aware)
{
   F360_Tracker_Info_T tracker_info = {};
   bool valid_dets[MAX_NUMBER_OF_DETECTIONS]{};
   uint32_t num_det_pts;
   F360_Detection_Props_T detections[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Local_Clusters_T output_data;
   rspp_variant_A::RSPP_Detection_List_T raw_detections{};
   int16_t detection_list[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   const bool f_cluster_moving = true;
   const float32_t tolerance = 1e-5F;

   /** \setup
    * Set needed inputs to default values. Sensor 1 is configured with realistic
    * v_wrapping = 25.0 m/s and r_wrapping = 0.4 m at look_id 0.
    */
   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);

      std::fill(std::begin(valid_dets), std::end(valid_dets), false);
      std::fill(std::begin(detections), std::end(detections), F360_Detection_Props_T{});
      num_det_pts = 0U;
      for (uint32_t det_index = 0U; det_index < MAX_NUMBER_OF_DETECTIONS; det_index++)
      {
         raw_detections.detections[det_index].processed.next_sorted_idx = F360_INVALID_ID;
         raw_detections.detections[det_index].processed.prev_sorted_idx = F360_INVALID_ID;
      }

      output_data = {};

      // Default sensor configuration
      sensors[0] = {};
      sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[0].constant.v_wrapping[0] = 25.0F;
      sensors[0].constant.r_wrapping[0] = 0.4F;
      sensors[1] = {};
      sensors[1].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[1].constant.v_wrapping[0] = 25.0F;
      sensors[1].constant.r_wrapping[0] = 0.4F;
   }

   void Setup_Two_Detection_Scenario(const float32_t rr0, const float32_t rr1,
                                     const int32_t sensor_id_0, const int32_t sensor_id_1)
   {
      num_det_pts = 2;
      raw_detections.number_of_valid_detections = num_det_pts;

      detections[0].vcs_position = { 10.0F, 0.0F };
      detections[0].range_rate_dealiased = rr0;
      detections[1].vcs_position = { 10.5F, 0.0F };
      detections[1].range_rate_dealiased = rr1;

      raw_detections.detections[0].raw.sensor_id = sensor_id_0;
      raw_detections.detections[1].raw.sensor_id = sensor_id_1;

      raw_detections.detections[0].processed.next_sorted_idx = 1;
      raw_detections.detections[1].processed.prev_sorted_idx = 0;

      for (uint32_t det_index = 0U; det_index < num_det_pts; det_index++)
      {
         detection_list[det_index] = static_cast<int16_t>(det_index);
         valid_dets[det_index] = true;
      }
   }
};

/**
*\purpose  Verify that two spatially close same-sensor detections whose range rate difference exceeds the 2.0 gate
*          are clustered together when the wrapped range rate difference falls within the gate, and that the candidate
*          detection's range rate and position are compensated by one wrapping interval.
*\req    NA
*/
TEST(f360_dbscan_interval_aware, Cluster_Region_Query__Wrapped_Range_Rate_Same_Sensor_Clusters)
{
   /** \precond
    * Two close detections from sensor 1 with v_wrapping = 25.0, r_wrapping = 0.4.
    * Detection 0 has range_rate_dealiased = 0.5, detection 1 has range_rate_dealiased = 25.0.
    * Both represent the same physical velocity: 25.0 is 0.5 shifted by close to one wrapping interval.
    * Raw difference = 24.5 (exceeds 2.0 gate), wrapped_diff = min(24.5, 25.0 - 24.5) = 0.5 < 2.0 => wrapping match.
    * sign = (0.5 > 25.0) ? 1.0 : -1.0 => -1.0 (shift candidate downward by one interval).
    */
   Setup_Two_Detection_Scenario(0.5F, 25.0F, 1, 1);

   // Set raw range and cos/sin for position compensation verification
   raw_detections.detections[1].raw.range = 10.5F;
   raw_detections.detections[1].processed.cos_vcs_az = 1.0F;
   raw_detections.detections[1].processed.sin_vcs_az = 0.0F;

   /** \action
    * Call DBscan function.
    */
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
    * Both detections should be in one cluster. Candidate detection 1 should have:
    * - range_rate_dealiased = 25.0 + (-1.0) * 25.0 = 0.0
    * - range_rate_compensated = 0.0 + (-1.0) * 25.0 = -25.0
    * - range_dealiased = 10.5 + (-0.4) = 10.1
    * - vcs_position.x = 10.5 + (-0.4) * 1.0 = 10.1
    * - vcs_position.y = 0.0 + (-0.4) * 0.0 = 0.0
    */
   CHECK_EQUAL_TEXT(1, output_data.num_clusters, "Expected single cluster via wrapping match");
   CHECK_EQUAL_TEXT(2, output_data.num_dets_in_clusters[0], "Expected both detections in the cluster");

   DOUBLES_EQUAL(0.0F, detections[1].range_rate_dealiased, tolerance);
   DOUBLES_EQUAL(-25.0F, detections[1].range_rate_compensated, tolerance);
   DOUBLES_EQUAL(10.1F, detections[1].range_dealiased, tolerance);
   DOUBLES_EQUAL(10.1F, detections[1].vcs_position.x, tolerance);
   DOUBLES_EQUAL(0.0F, detections[1].vcs_position.y, tolerance);
}

/**
*\purpose  Verify that two spatially close detections from different sensors with a large range rate difference
*          are placed in separate clusters because the interval-aware wrapping logic requires same-sensor detections.
*\req    NA
*/
TEST(f360_dbscan_interval_aware, Cluster_Region_Query__Different_Sensors_No_Wrapping)
{
   /** \precond
    * Two close detections from different sensors (sensor 1 and sensor 2) with range rate difference
    * of 24.5 (exceeds 2.0 gate). Even though wrapping would bring it below the gate for same sensor,
    * different sensors prevent the wrapping path from being taken.
    */
   Setup_Two_Detection_Scenario(0.5F, 25.0F, 1, 2);

   /** \action
    * Call DBscan function.
    */
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
    * Detections should be in separate clusters because wrapping is not applied across different sensors.
    */
   CHECK_EQUAL_TEXT(2, output_data.num_clusters, "Expected two separate clusters for different sensors");
   CHECK_EQUAL_TEXT(1, output_data.num_dets_in_clusters[0], "Expected one detection in first cluster");
   CHECK_EQUAL_TEXT(1, output_data.num_dets_in_clusters[1], "Expected one detection in second cluster");
}

/**
*\purpose  Verify that two spatially close same-sensor detections with wrapped range rate difference
*          still exceeding the gate remain in separate clusters.
*\req    NA
*/
TEST(f360_dbscan_interval_aware, Cluster_Region_Query__Same_Sensor_Wrapped_Diff_Still_Exceeds_Gate)
{
   /** \precond
    * Two close detections from sensor 1 with v_wrapping = 25.0.
    * Detection 0 has range_rate_dealiased = 0.0, detection 1 has range_rate_dealiased = 22.0.
    * Raw difference = 22.0, abs_wrapped_diff = 3.0 >= 2.0 => wrapping does not help.
    */
   Setup_Two_Detection_Scenario(0.0F, 22.0F, 1, 1);

   /** \action
    * Call DBscan function.
    */
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
    * Detections should be in separate clusters because even the wrapped difference (3.0) exceeds the 2.0 gate.
    */
   CHECK_EQUAL_TEXT(2, output_data.num_clusters, "Expected two separate clusters when wrapped diff still exceeds gate");
   CHECK_EQUAL_TEXT(1, output_data.num_dets_in_clusters[0], "Expected one detection in first cluster");
   CHECK_EQUAL_TEXT(1, output_data.num_dets_in_clusters[1], "Expected one detection in second cluster");
}

/**
*\purpose  Verify that range rate compensation shifts candidate toward base detection when base has higher
*          range rate than candidate (sign = +1), correctly adjusting range_rate_dealiased,
*          range_rate_compensated, range_dealiased, and vcs_position.
*\req    NA
*/
TEST(f360_dbscan_interval_aware, Cluster_Region_Query__Wrapped_Match_Compensation_Sign_Positive)
{
   /** \precond
    * Two close detections from sensor 1 with v_wrapping = 25.0, r_wrapping = 0.4.
    * Detection 0 has range_rate_dealiased = 25.0 (base), detection 1 has range_rate_dealiased = 0.5 (candidate).
    * Both represent the same physical velocity: 0.5 is 25.0 shifted down by close to one wrapping interval.
    * Abs diff = 24.5, wrapped_diff = min(24.5, 25.0 - 24.5) = 0.5 < 2.0 => wrapping match.
    * sign = (25.0 > 0.5) ? 1.0 : -1.0 => 1.0 (shift candidate upward by one interval).
    */
   Setup_Two_Detection_Scenario(25.0F, 0.5F, 1, 1);
   detections[1].range_rate_compensated = 0.5F;

   raw_detections.detections[1].raw.range = 20.0F;
   raw_detections.detections[1].processed.cos_vcs_az = 0.8F;
   raw_detections.detections[1].processed.sin_vcs_az = 0.6F;

   /** \action
    * Call DBscan function.
    */
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
    * Both detections should be in one cluster. Candidate detection 1 should have:
    * - range_rate_dealiased = 0.5 + 1.0 * 25.0 = 25.5
    * - range_rate_compensated = 0.5 + 1.0 * 25.0 = 25.5
    * - range_dealiased = 20.0 + 0.4 = 20.4
    * - vcs_position.x = 10.5 + 0.4 * 0.8 = 10.82
    * - vcs_position.y = 0.0 + 0.4 * 0.6 = 0.24
    */
   CHECK_EQUAL_TEXT(1, output_data.num_clusters, "Expected single cluster via wrapping match");
   CHECK_EQUAL_TEXT(2, output_data.num_dets_in_clusters[0], "Expected both detections in the cluster");

   DOUBLES_EQUAL(25.5F, detections[1].range_rate_dealiased, tolerance);
   DOUBLES_EQUAL(25.5F, detections[1].range_rate_compensated, tolerance);
   DOUBLES_EQUAL(20.4F, detections[1].range_dealiased, tolerance);
   DOUBLES_EQUAL(10.82F, detections[1].vcs_position.x, tolerance);
   DOUBLES_EQUAL(0.24F, detections[1].vcs_position.y, tolerance);
}

/** @}*/

/** \defgroup  f360_dbscan_stationary
 *  @{
 */

/** \brief
*  Test group for dbscan for stationary detections.
**/
TEST_GROUP(f360_dbscan_stationary_dets)
{
   /** \setup
   * Nothing to setup in this test group
   **/
   F360_Tracker_Info_T tracker_info = {};
   bool valid_dets[MAX_NUMBER_OF_DETECTIONS]{};
   uint32_t num_det_pts;
   F360_Detection_Props_T detections[MAX_NUMBER_OF_DETECTIONS]{};
   rspp_variant_A::RSPP_Detection_List_T raw_detections{};
   F360_Local_Clusters_T output_data;
   int16_t detection_list[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   const bool f_cluster_moving = false;

   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);

      std::fill(std::begin(valid_dets), std::end(valid_dets), false);
      std::fill(std::begin(detections), std::end(detections), F360_Detection_Props_T{});
      num_det_pts = 0U;
      for (uint32_t det_index = 0U; det_index < MAX_NUMBER_OF_DETECTIONS; det_index++)
      {
         raw_detections.detections[det_index].processed.next_sorted_idx = F360_INVALID_ID;
         raw_detections.detections[det_index].processed.prev_sorted_idx = F360_INVALID_ID;
         raw_detections.detections[det_index].raw.sensor_id = 1;
      }

      output_data = {};
   }
};

/**
*\purpose  Check function behaviour for single detection as an input.
*\req    NA
*/
TEST(f360_dbscan_stationary_dets, single_detection)
{
   /** \precond
   Set detection position values.
   **/
   num_det_pts = 1;
   detections[0].vcs_position = { 0.0, 0.0 };
   valid_dets[0] = true;
   detection_list[0] = 0;


   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if cluster was created and ensure detection index inside cluster
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 1U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 1U, "Number of detections inside cluster is not equal to expected value");
}

/**
*\purpose  Check function behaviour for two close detections as an input.
*\req    NA
*/
TEST(f360_dbscan_stationary_dets, two_close_detections)
{
   /** \precond
   Set detection position values.
   **/
   num_det_pts = 2;
   detections[0].vcs_position = { 0.0F, 0.0F };
   detections[1].vcs_position = { 1.0F, 0.0F };
   
   raw_detections.detections[0].processed.next_sorted_idx = 1;
   raw_detections.detections[1].processed.prev_sorted_idx = 0;

   valid_dets[0] = true;
   valid_dets[1] = true;
   detection_list[0] = 0;
   detection_list[1] = 1;
   raw_detections.number_of_valid_detections = 2;

   /** \action
   Call dbscan function.
   **/
   DBscan(tracker_info, raw_detections, sensors, valid_dets, f_cluster_moving, detection_list, num_det_pts, detections, output_data);

   /** \result
   Check if cluster was created and ensure detection index inside cluster
   **/
   CHECK_TRUE_TEXT(output_data.num_clusters == 1U, "Number of created clusters is not equal to expected value");
   CHECK_TRUE_TEXT(output_data.num_dets_in_clusters[0] == 2U, "Number of detections inside cluster is not equal to expected value");
}

/** @}*/

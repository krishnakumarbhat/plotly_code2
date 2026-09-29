/** \file
 * This file contains unit tests for content of f360_post_process_longi_stat_clusters.cpp file
 */

#include "f360_post_process_longi_stat_clusters.h"
#include <CppUTest/TestHarness.h>
#include <algorithm>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

// Support function used within multiple test groups to create a LSC cluster and set position of objects in the cluster
F360_Longi_Stat_Cluster_T Create_LSC_Cluster_And_Set_Obj_Position(
   const uint32_t(&obj_ids)[NUMBER_OF_OBJECT_TRACKS],
   const uint32_t num_objs_to_create,
   const float32_t first_obj_long_pos,
   const float32_t obj_long_pos_inc,
   const float32_t obj_lat_pos,
   F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS])
{

   const uint32_t first_obj_idx = obj_ids[0U] - 1U;
   objects[first_obj_idx].lsc_prev_in_cluster = NULL;

   objects[first_obj_idx].vcs_position.x = first_obj_long_pos;
   objects[first_obj_idx].vcs_position.y = obj_lat_pos;
   objects[first_obj_idx].reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = objects[first_obj_idx].vcs_position;
   objects[first_obj_idx].bbox.Set_Center(center);

   for (uint32_t i = 1U; i < num_objs_to_create; i++)
   {
      const uint32_t prev_obj_idx = obj_ids[i - 1U] - 1U;
      const uint32_t obj_idx = obj_ids[i] - 1U;

      objects[prev_obj_idx].lsc_next_in_cluster = &objects[obj_idx];
      objects[obj_idx].lsc_prev_in_cluster = &objects[prev_obj_idx];

      objects[obj_idx].vcs_position.x = objects[prev_obj_idx].vcs_position.x + obj_long_pos_inc;
      objects[obj_idx].vcs_position.y = obj_lat_pos;
      objects[obj_idx].reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = objects[obj_idx].vcs_position;
      objects[obj_idx].bbox.Set_Center(center);

   }

   const uint32_t last_obj_idx = obj_ids[num_objs_to_create - 1U] - 1U;
   objects[last_obj_idx].lsc_next_in_cluster = NULL;

   F360_Longi_Stat_Cluster_T cluster = {};
   cluster.first_object = &objects[first_obj_idx];
   cluster.last_object = &objects[last_obj_idx];
   cluster.nr_objects = num_objs_to_create;
   cluster.lat_mean = obj_lat_pos;

   return cluster;
}

/** \defgroup  f360_post_process_longi_stat_clusters_merge_cluster_pair
 *  @{
 */

/** \brief
 * Tests using this test group will verify the functionality of function Merge_Longi_Stat_Cluster_Pair used to merge two LSC clusters.
 */
TEST_GROUP(f360_post_process_longi_stat_clusters_merge_cluster_pair)
{	
   const float32_t test_pass_th = 1e-8F;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS] = {};
   uint16_t nr_clusters;
   F360_Longi_Stat_Cluster_T clusters[NR_LONGI_STAT_CLUSTERS] = {};
   uint16_t nr_next_clusters;
   F360_Longi_Stat_Cluster_T next_clusters[NR_LONGI_STAT_CLUSTERS] = {};
   
   /** \setup
    * Set up a default scenario where three clusters exists:
    * First cluster consists of 4 objects all at lateral vcs position 3m, longitudinal vcs position starts from 0 and increments with 1m per object
    * Second cluster consists of 4 objects all at lateral vcs position 4m, longitudinal vcs position starts from 6 and increments with 1m per object
    * Third cluster consists of 5 objects all at lateral vcs position 9m, longitudinal vcs position starts from 20 and increments with 1m per object
    */
   TEST_SETUP()
   {
      F360_Longi_Stat_Cluster_T cluster_1 = Create_LSC_Cluster_And_Set_Obj_Position({ 1U, 2U, 3U, 4U }, 4U, 0.0F, 1.0F, 3.0F, objects);
      F360_Longi_Stat_Cluster_T cluster_2 = Create_LSC_Cluster_And_Set_Obj_Position({ 5U, 6U, 7U, 8U }, 4U, 6.0F, 1.0F, 4.0F, objects);
      F360_Longi_Stat_Cluster_T cluster_3 = Create_LSC_Cluster_And_Set_Obj_Position({ 9U, 10U, 11U, 12U, 13U }, 5U, 20.0F, 1.0F, 9.0F, objects);

      clusters[0U] = cluster_1;
      clusters[1U] = cluster_2;
      clusters[2U] = cluster_3;
      nr_clusters = 3U;

      next_clusters[0U] = cluster_1;
      next_clusters[1U] = cluster_2;
      next_clusters[2U] = cluster_3;
      nr_next_clusters = 3U;
   }
};

/** \purpose  
 * Verify that function that merges two LSC clusters into one works as expected.
 * \req
 * NA.
 */
TEST(f360_post_process_longi_stat_clusters_merge_cluster_pair, Merge_Longi_Stat_Cluster_Pair)
{
   /** \precond
    * In test group setup:
    * - Three clusters have been created and added to the array of clusters and array of next clusters 
    *    - First cluster contains 4 objects with vcs lateral mean 3
    *    - Second cluster contains 4 objects with vcs lateral mean 4
    *    - Third cluster contains 5 objects with vcs lateral mean 9
    * - Number of clusters and number of next clusters have been set to 3.
    * Set primary cluster idx to 0
    * Set secondary cluster idx to 1
    */
   const uint16_t primary_cluster_idx = 0U;
   const uint16_t secondary_cluster_idx = 1U;
	
   /** \action
    * Call Merge_Longi_Stat_Cluster_Pair
    */
   Merge_Longi_Stat_Cluster_Pair(nr_clusters, clusters, primary_cluster_idx, secondary_cluster_idx, nr_next_clusters, next_clusters);

   /** \result
    * Check that number of next clusters has been decreased to 2
    * Check that number of objects in first cluster of next clusters array is 8
    * Check that the lateral mean of the merged cluster is 3.5
    * Check that last object in first cluster points to first object in second cluster
    * Check that first object in secondary cluster now points to last object in first cluster
    * Check that the third cluster was successfully moved to idx 1 in next clusters array by checking number of objects and lateral mean
    */	
   CHECK_EQUAL_TEXT(2U, nr_next_clusters, "Number of next clusters was not decreased to 2 as expected.");
   CHECK_EQUAL_TEXT(8U, next_clusters[0U].nr_objects, "Number of objects in merged cluster was not 8 as expected.");
   DOUBLES_EQUAL_TEXT(3.5F, next_clusters[0U].lat_mean, test_pass_th, "The lateral mean of the merged cluster was not 3.5 as expected.");
   POINTERS_EQUAL_TEXT(clusters[secondary_cluster_idx].first_object, clusters[primary_cluster_idx].last_object->lsc_next_in_cluster, "The last object in primary cluster does not point to the first object in the secondary cluster.");
   POINTERS_EQUAL_TEXT(clusters[primary_cluster_idx].last_object, clusters[secondary_cluster_idx].first_object->lsc_prev_in_cluster, "The first object secondary cluster does not point to the last object in the primary cluster.");

   CHECK_EQUAL_TEXT(5U, next_clusters[1U].nr_objects, "The third cluster, which contained 5 objects, was not moved to the the expected slot in next clusters array.");
   DOUBLES_EQUAL_TEXT(9.0F, next_clusters[1U].lat_mean, test_pass_th, "The third cluster, which had a lateral mean of 9, was not moved to the the expected slot in next clusters array.");
}

/** \purpose
 * Verify that function that merges two LSC clusters into one correctly sets the last object in the merged cluster to the one with the greatest longitudinal position from the original clusters.
 * \req
 * NA.
 */
IGNORE_TEST(f360_post_process_longi_stat_clusters_merge_cluster_pair, Merge_Longi_Stat_Cluster_Pair_Last_Object_Is_Set_Correctly)
{
   /** \precond
    * In test group setup:
    * - Three clusters have been created and added to the array of clusters and array of next clusters
    *    - First cluster contains 4 objects with vcs lateral mean 3
    *    - Second cluster contains 4 objects with vcs lateral mean 4
    *    - Third cluster contains 5 objects with vcs lateral mean 9, longitudinal position starts from 20m and increments with 1m per object (20, 21, 22, 23, 24) m.
    * Create a fourth cluster with 3 objects with vcs lateral mean 9.5, longitudinal position starts from 18m and increments with 0.5m per object (18, 18.5, 19) m
    * Set number of clusters to 4
    * Set primary cluster idx to 2
    * Set secondary cluster idx to 3
    */
   F360_Longi_Stat_Cluster_T cluster_4 = Create_LSC_Cluster_And_Set_Obj_Position({ 90U, 91U, 92U }, 3U, 18.0F, 0.5F, 9.5F, objects);
   clusters[3U] = cluster_4;
   next_clusters[3U] = cluster_4;

   nr_clusters = 4U;
   nr_next_clusters = 4U;

   const uint16_t primary_cluster_idx = 2U;
   const uint16_t secondary_cluster_idx = 3U;

   /** \action
    * Call Merge_Longi_Stat_Cluster_Pair
    */
   Merge_Longi_Stat_Cluster_Pair(nr_clusters, clusters, primary_cluster_idx, secondary_cluster_idx, nr_next_clusters, next_clusters);

   /** \result
    * Check that the number of objects was correctly updated for the merged cluster
    * Check that the last object in the merged cluster is set to the one with the greatest longitudinal position from the two original clusters.
    * Check that the last object in the cluster is the one with the greatest longitudinal position (i.e. that the last object in merged cluster corresponds to last object of primary cluster)
    */
   CHECK_EQUAL_TEXT(8U, next_clusters[primary_cluster_idx].nr_objects, "The number of objects in the merged cluster was not updated as expected.");
   POINTERS_EQUAL_TEXT(clusters[primary_cluster_idx].last_object, next_clusters[primary_cluster_idx].last_object, "The last object in th.");
   DOUBLES_EQUAL(24, next_clusters[primary_cluster_idx].last_object->vcs_position.x, test_pass_th);

}
/** @}*/

/** \defgroup  f360_post_process_longi_stat_clusters_try_to_merge_longi_stat_cluster
 *  @{
 */

/** \brief
 * Tests using this test group will verify the functionality of function Try_To_Merge_Longi_Stat_Clusters that loops through LSC clusters and tries to merge clusters with similar properties.
 */
TEST_GROUP(f360_post_process_longi_stat_clusters_try_to_merge_longi_stat_cluster)
{
   const float32_t test_pass_th = 1e-8F;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS] = {};
   uint16_t nr_clusters;
   F360_Longi_Stat_Cluster_T clusters[NR_LONGI_STAT_CLUSTERS] = {};
   uint16_t nr_next_clusters;
   F360_Longi_Stat_Cluster_T next_clusters[NR_LONGI_STAT_CLUSTERS] = {};
   F360_Calibrations_T calibs = {};
   uint16_t next_idx = 1U;
   F360_Tracker_Info_T tracker_info = {};

   /** \setup
    * Initialize tracker calibrations 
    * Set up a default scenario where three clusters exists:
    * - First cluster contains 4 objects with vcs lateral mean 3, longitudinal position starts from 0m and increments with 1m per object (0, 1, 2, 3) m
    * - Second cluster contains 4 objects with vcs lateral mean 4, longitudinal position starts from 5m and increments with 1m per object (5, 6, 7, 8) m
    * - Third cluster contains 5 objects with vcs lateral mean 9, longitudinal position starts from 20m and increments with 1m per object (20, 21, 22, 23, 24) m
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      F360_Longi_Stat_Cluster_T cluster_1 = Create_LSC_Cluster_And_Set_Obj_Position({ 1U, 2U, 3U, 4U }, 4U, 0.0F, 1.0F, 3.0F, objects);
      F360_Longi_Stat_Cluster_T cluster_2 = Create_LSC_Cluster_And_Set_Obj_Position({ 5U, 6U, 7U, 8U }, 4U, 5.0F, 1.0F, 4.0F, objects);
      F360_Longi_Stat_Cluster_T cluster_3 = Create_LSC_Cluster_And_Set_Obj_Position({ 9U, 10U, 11U, 12U, 13U }, 5U, 20.0F, 1.0F, 9.0F, objects);

      clusters[0U] = cluster_1;
      clusters[1U] = cluster_2;
      clusters[2U] = cluster_3;
      nr_clusters = 3U;

      next_clusters[0U] = cluster_1;
      next_clusters[1U] = cluster_2;
      next_clusters[2U] = cluster_3;
      nr_next_clusters = 3U;
   }
};

/** \purpose
 * Verify that function that loops through LSC clusters and merges clusters that are close to each other works as expected when the first two clusters are close both laterally and longitudinally.
 * \req
 * NA.
 */
TEST(f360_post_process_longi_stat_clusters_try_to_merge_longi_stat_cluster, Try_To_Merge_Longi_Stat_Clusters_First_Two_Clusters_Close)
{
   /** \precond
    * In test group setup:
    * - Tracker calibrations have been initialized
    * - Three clusters have been created and added to the array of clusters and array of next clusters
    *    - First cluster contains 4 objects with vcs lateral mean 3, longitudinal position starts from 0m and increments with 1m per object (0, 1, 2, 3) m
    *    - Second cluster contains 4 objects with vcs lateral mean 4, longitudinal position starts from 5m and increments with 1m per object (5, 6, 7, 8) m
    *    - Third cluster contains 5 objects with vcs lateral mean 9, longitudinal position starts from 20m and increments with 1m per object (20, 21, 22, 23, 24) m
    * - Number of clusters and number of next clusters have been set to 3.
    * - Index for which to cluster to start comparison with has been set to 1 (i.e. compare cluster 0-1, then 0-2 etc).
    */

   /** \action
    * Call Try_To_Merge_Longi_Stat_Clusters
    */
   const bool f_merged_occured = Try_To_Merge_Longi_Stat_Clusters(nr_clusters, clusters, calibs, tracker_info, next_idx, nr_next_clusters, next_clusters);

   /** \result
    * Check that a merge occured.
    * Check that number of next clusters has been decreased to 2
    * Check that number of objects in first cluster of next clusters array is 8
    * Check that the lateral mean of the merged cluster is 3.5
    * Check that last object in first cluster points to first object in second cluster
    * Check that first object in secondary cluster now points to last object in first cluster
    * Check that the third cluster was successfully moved to idx 1 in next clusters array by checking number of objects and lateral mean
    */
   CHECK_TRUE_TEXT(f_merged_occured, "Function reported that no merge occured when a merge should have occured.")
   CHECK_EQUAL_TEXT(2U, nr_next_clusters, "Number of next clusters was not decreased to 2 as expected.");
   CHECK_EQUAL_TEXT(8U, next_clusters[0].nr_objects, "Number of objects in merged cluster was not 8 as expected.");
   DOUBLES_EQUAL_TEXT(3.5F, next_clusters[0].lat_mean, test_pass_th, "The lateral mean of the merged cluster was not 3.5 as expected.");
   POINTERS_EQUAL_TEXT(clusters[1U].first_object, clusters[0U].last_object->lsc_next_in_cluster, "The last object in primary cluster does not point to the first object in the secondary cluster.");
   POINTERS_EQUAL_TEXT(clusters[0U].last_object, clusters[1U].first_object->lsc_prev_in_cluster, "The first object secondary cluster does not point to the last object in the primary cluster.");

   CHECK_EQUAL_TEXT(5U, next_clusters[1U].nr_objects, "The third cluster, which contained 5 objects, was not moved to the the expected slot in next clusters array.");
   DOUBLES_EQUAL_TEXT(9.0F, next_clusters[1U].lat_mean, test_pass_th, "The third cluster, which had a lateral mean of 9, was not moved to the the expected slot in next clusters array.");
}

/** \purpose
 * Verify that function that loops through LSC clusters and merges clusters that are close to each other works as expected when the first two clusters are close longitudinally but not laterally.
 * \req
 * NA.
 */
TEST(f360_post_process_longi_stat_clusters_try_to_merge_longi_stat_cluster, Try_To_Merge_Longi_Stat_Clusters_First_Two_Clusters_Close_Longitudinally_Not_Laterally)
{
   /** \precond
    * In test group setup:
    * - Tracker calibrations have been initialized
    * - Three clusters have been created and added to the array of clusters and array of next clusters
    *    - First cluster contains 4 objects with vcs lateral mean 3, longitudinal position starts from 0m and increments with 1m per object (0, 1, 2, 3) m
    *    - Second cluster contains 4 objects. Longitudinal position starts from 5m and increments with 1m per object (5, 6, 7, 8) m
    *    - Third cluster contains 5 objects with vcs lateral mean 9, longitudinal position starts from 20m and increments with 1m per object (20, 21, 22, 23, 24) m
    * - Number of clusters and number of next clusters have been set to 3
    * - Index for which to cluster to start comparison with has been set to 1 (i.e. compare cluster 0-1, then 0-2 etc)
    * Set lateral position of first object in second cluster to 6.2
    */

    clusters[1U].first_object->vcs_position.x = 6.2F;
    clusters[1U].first_object->reference_point = F360_REFERENCE_POINT_CENTER;
    Point center = clusters[1U].first_object->vcs_position;
    clusters[1U].first_object->bbox.Set_Center(center);

   /** \action
    * Call Try_To_Merge_Longi_Stat_Clusters
    */
   const bool f_merged_occured = Try_To_Merge_Longi_Stat_Clusters(nr_clusters, clusters, calibs, tracker_info, next_idx, nr_next_clusters, next_clusters);

   /** \result
    * Check that no merge occured 
    * Check that number of next clusters is still 3
    * Check that number of objects in the three clusters did not change
    */
   CHECK_FALSE_TEXT(f_merged_occured, "Function reported that a merge occured when no merge should have occured.");
   CHECK_EQUAL_TEXT(3U, nr_next_clusters, "Number of next clusters should have been 3 as no merge occured.");
   CHECK_EQUAL_TEXT(4U, next_clusters[0].nr_objects, "Number of objects in first cluster changed unexpectedly.");
   CHECK_EQUAL_TEXT(4U, next_clusters[1].nr_objects, "Number of objects in second cluster changed unexpectedly.");
   CHECK_EQUAL_TEXT(5U, next_clusters[2].nr_objects, "Number of objects in third cluster changed unexpectedly.");
}

/** \purpose
   * Verify that function that loops through LSC clusters and merges clusters that are close to each other works as expected when the first two clusters are close laterally but not longitudinally.
   * \req
   * NA.
   */
TEST(f360_post_process_longi_stat_clusters_try_to_merge_longi_stat_cluster, Try_To_Merge_Longi_Stat_Clusters_First_Two_Clusters_Close_Laterally_Not_Longitudinally)
{
   /** \precond
      * In test group setup:
      * - Tracker calibrations have been initialized
      * - Three clusters have been created and added to the array of clusters and array of next clusters
      *    - First cluster contains 4 objects with vcs lateral mean 3, longitudinal position starts from 0m and increments with 1m per object (0, 1, 2, 3) m
      *    - Second cluster contains 4 objects with vcs lateral mean 4, longitudinal position starts from 5m and increments with 1m per object (5, 6, 7, 8) m
      *    - Third cluster contains 5 objects with vcs lateral mean 9, longitudinal position starts from 20m and increments with 1m per object (20, 21, 22, 23, 24) m
      * - Number of clusters and number of next clusters have been set to 3
      * - Index for which to cluster to start comparison with has been set to 1 (i.e. compare cluster 0-1, then 0-2 etc)
      * Set longitudinal vcs position of last object in first cluster to 2.9
      * Set longitudinal vcs position of first object in second cluster to 6
      */
   clusters[0U].last_object->vcs_position.x = 2.9F;
   clusters[1U].first_object->vcs_position.x = 6.0F;
   clusters[0U].last_object->reference_point = F360_REFERENCE_POINT_CENTER;
   clusters[1U].first_object->reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = clusters[0U].last_object->vcs_position;
   clusters[0U].last_object->bbox.Set_Center(center);
   center = clusters[1U].first_object->vcs_position;
   clusters[1U].first_object->bbox.Set_Center(center);

   /** \action
    * Call Try_To_Merge_Longi_Stat_Clusters
    */
   const bool f_merged_occured = Try_To_Merge_Longi_Stat_Clusters(nr_clusters, clusters, calibs, tracker_info, next_idx, nr_next_clusters, next_clusters);

   /** \result
    * Check that no merge occured
    * Check that number of next clusters is still 3
    * Check that number of objects in the three clusters did not change
    */
   CHECK_FALSE_TEXT(f_merged_occured, "Function reported that a merge occured when no merge should have occured.");
   CHECK_EQUAL_TEXT(3U, nr_next_clusters, "Number of next clusters should have been 3 as no merge occured.");
   CHECK_EQUAL_TEXT(4U, next_clusters[0].nr_objects, "Number of objects in first cluster changed unexpectedly.");
   CHECK_EQUAL_TEXT(4U, next_clusters[1].nr_objects, "Number of objects in second cluster changed unexpectedly.");
   CHECK_EQUAL_TEXT(5U, next_clusters[2].nr_objects, "Number of objects in third cluster changed unexpectedly.");
}

/** \purpose
   * Verify that function that loops through LSC clusters and merges clusters that are close to each other works as expected when the first two clusters are close enough both laterally longitudinally to pass
   * initial gate but end points not close enough laterally to merge.
   * \req
   * NA.
   */
TEST(f360_post_process_longi_stat_clusters_try_to_merge_longi_stat_cluster, Try_To_Merge_Longi_Stat_Clusters_First_Two_Clusters_Close_Longitudinally_And_Mean_Close_Laterally_End_Points_Not)
{
   /** \precond
      * In test group setup:
      * - Tracker calibrations have been initialized
      * - Three clusters have been created and added to the array of clusters and array of next clusters
      *    - First cluster contains 4 objects with vcs lateral mean 3, longitudinal position starts from 0m and increments with 1m per object (0, 1, 2, 3) m
      *    - Second cluster contains 4 objects with vcs lateral mean 4, longitudinal position starts from 5m and increments with 1m per object (5, 6, 7, 8) m
      *    - Third cluster contains 5 objects with vcs lateral mean 9, longitudinal position starts from 20m and increments with 1m per object (20, 21, 22, 23, 24) m
      * - Number of clusters and number of next clusters have been set to 3
      * - Index for which to cluster to start comparison with has been set to 1 (i.e. compare cluster 0-1, then 0-2 etc)
      * Set lateral mean of second cluster to 5m
      * Set lateral vcs position to 5m for all objects in second cluster
      */
   clusters[1U].lat_mean = 5.0F;
   F360_Object_Track_T* p_obj = clusters[1U].first_object;
   p_obj->vcs_position.y = 5.0F;
   p_obj->reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = p_obj->vcs_position;
   p_obj->bbox.Set_Center(center);
   for (uint16_t i = 0U; i < (clusters[1U].nr_objects - 1U); i++)
   {
      p_obj = p_obj->lsc_next_in_cluster;
      p_obj->vcs_position.y = 5.0F;
      p_obj->reference_point = F360_REFERENCE_POINT_CENTER;
      center = p_obj->vcs_position;
      p_obj->bbox.Set_Center(center);
   }

   /** \action
    * Call Try_To_Merge_Longi_Stat_Clusters
    */
   const bool f_merged_occured = Try_To_Merge_Longi_Stat_Clusters(nr_clusters, clusters, calibs, tracker_info, next_idx, nr_next_clusters, next_clusters);

   /** \result
    * Check that no merge occured
    * Check that number of next clusters is still 3
    * Check that number of objects in the three clusters did not change
    */
   CHECK_FALSE_TEXT(f_merged_occured, "Function reported that a merge occured when no merge should have occured.");
   CHECK_EQUAL_TEXT(3U, nr_next_clusters, "Number of next clusters should have been 3 as no merge occured.");
   CHECK_EQUAL_TEXT(4U, next_clusters[0].nr_objects, "Number of objects in first cluster changed unexpectedly.");
   CHECK_EQUAL_TEXT(4U, next_clusters[1].nr_objects, "Number of objects in second cluster changed unexpectedly.");
   CHECK_EQUAL_TEXT(5U, next_clusters[2].nr_objects, "Number of objects in third cluster changed unexpectedly.");
}

/** \purpose
   * Verify that function that loops through LSC clusters and merges clusters that are close to each other works as expected when the second and third cluster are close both laterally and longitudinally.
   * \req
   * NA.
   */
TEST(f360_post_process_longi_stat_clusters_try_to_merge_longi_stat_cluster, Try_To_Merge_Longi_Stat_Clusters_Second_Third_Clusters_Close)
{
   /** \precond
    * In test group setup:
    * - Tracker calibrations have been initialized
    * - Two clusters have been created and added to the array of clusters and array of next clusters on index 0 and 2
    *    - Cluster with idx 0 contains 4 objects with vcs lateral mean 3, longitudinal position starts from 0m and increments with 1m per object (0, 1, 2, 3) m
    *    - Cluster with idx 2 contains 5 objects with vcs lateral mean 9, longitudinal position starts from 20m and increments with 1m per object (20, 21, 22, 23, 24) m
    * - Number of clusters and number of next clusters have been set to 3.
    * - Index for which to cluster to start comparison with has been set to 1 (i.e. compare cluster 0-1, then 0-2 etc).
    * Create a cluster of 10 objects that has lateral mean of 8 and longitudinal position from 14m and increments with 0.5m per object (14-18.5)m and place it on index 1 in array of clusters 
    */
   F360_Longi_Stat_Cluster_T cluster_2 = Create_LSC_Cluster_And_Set_Obj_Position({ 14U, 29U, 18U, 16U, 100U, 102U, 103U, 104U, 105U, 106U }, 10U, 14.0F, 0.5F, 8.0F, objects);
   clusters[1] = cluster_2;
   next_clusters[1] = cluster_2;

   /** \action
    * Call Try_To_Merge_Longi_Stat_Clusters
    */
   const bool f_merged_occured = Try_To_Merge_Longi_Stat_Clusters(nr_clusters, clusters, calibs, tracker_info, next_idx, nr_next_clusters, next_clusters);

   /** \result
    * Check that a merge occured.
    * Check that number of next clusters has been decreased to 2
    * Check that number of objects in merged cluster is 15
    * Check that the lateral mean of the merged cluster is 8.333333
    * Check that last object in second cluster points to first object in third cluster
    * Check that first object in third cluster now points to last object in second cluster
    */
   CHECK_TRUE_TEXT(f_merged_occured, "Function reported that no merge occured when a merge should have occured.");
   CHECK_EQUAL_TEXT(2U, nr_next_clusters, "Number of next clusters was not decreased to 2 as expected.");
   CHECK_EQUAL_TEXT(15U, next_clusters[1].nr_objects, "Number of objects in merged cluster was not 15 as expected.");
   DOUBLES_EQUAL_TEXT(8.333333F, next_clusters[1].lat_mean, test_pass_th, "The lateral mean of the merged cluster was not 8.33 as expected.");
   POINTERS_EQUAL_TEXT(clusters[2U].first_object, clusters[1U].last_object->lsc_next_in_cluster, "The last object in second cluster does not point to the first object in the third cluster.");
   POINTERS_EQUAL_TEXT(clusters[1U].last_object, clusters[2U].first_object->lsc_prev_in_cluster, "The first object in third cluster does not point to the last object in the second cluster.");
}

/** \purpose
   * Verify that function that loops through LSC clusters and merges clusters that are close to each other works as expected when the second and third cluster are:
   *     far away from the host (both are more than 50m away),
   *     close laterally,
   *     their gap in longitudinal distance (5m) is slightly bigger then the standard threshold for merging (3m)
   * \req
   * NA.
   */
  TEST(f360_post_process_longi_stat_clusters_try_to_merge_longi_stat_cluster, Try_To_Merge_Longi_Stat_Clusters_Second_Third_Clusters_Close_And_Far)
  {
     /** \precond
      * In test group setup:
      * - Tracker calibrations have been initialized
      * - Number of clusters and number of next clusters have been set to 3.
      * - Index for which to cluster to start comparison with has been set to 1 (i.e. compare cluster 0-1, then 0-2 etc).
      * Create a cluster of 10 objects that has lateral mean of 8 and longitudinal position from 50m and increments with 1m per object (50-59)m and place it on index 1 in array of clusters 
      * Create a cluster of 10 objects that has lateral mean of 9 and longitudinal position from 64m and increments with 1m per object (64-73)m and place it on index 2 in array of clusters 
      */
      F360_Longi_Stat_Cluster_T cluster_2 = Create_LSC_Cluster_And_Set_Obj_Position({ 14U, 29U, 18U, 16U, 100U, 102U, 103U, 104U, 105U, 106U }, 10U, 50.0F, 1.0F, 8.0F, objects);
      clusters[1] = cluster_2;
      next_clusters[1] = cluster_2;

      F360_Longi_Stat_Cluster_T cluster_3 = Create_LSC_Cluster_And_Set_Obj_Position({ 108U, 109U, 110U, 111U, 112U, 113U, 114U, 115U, 116U, 117U}, 10U, 64.0F, 1.0F, 9.0F, objects);
      clusters[2] = cluster_3;
  
     /** \action
      * Call Try_To_Merge_Longi_Stat_Clusters
      */
     const bool f_merged_occured = Try_To_Merge_Longi_Stat_Clusters(nr_clusters, clusters, calibs, tracker_info, next_idx, nr_next_clusters, next_clusters);
  
     /** \result
      * Check that a merge occured.
      * Check that number of next clusters has been decreased to 2
      * Check that number of objects in merged cluster is 20
      * Check that the lateral mean of the merged cluster is 8.5
      * Check that last object in second cluster points to first object in third cluster
      * Check that first object in third cluster now points to last object in second cluster
      */
     CHECK_TRUE_TEXT(f_merged_occured, "Function reported that no merge occured when a merge should have occured.");
     CHECK_EQUAL_TEXT(2U, nr_next_clusters, "Number of next clusters was not decreased to 2 as expected.");
     CHECK_EQUAL_TEXT(20U, next_clusters[1].nr_objects, "Number of objects in merged cluster was not 15 as expected.");
     DOUBLES_EQUAL_TEXT(8.5F, next_clusters[1].lat_mean, test_pass_th, "The lateral mean of the merged cluster was not 8.33 as expected.");
     POINTERS_EQUAL_TEXT(clusters[2U].first_object, clusters[1U].last_object->lsc_next_in_cluster, "The last object in second cluster does not point to the first object in the third cluster.");
     POINTERS_EQUAL_TEXT(clusters[1U].last_object, clusters[2U].first_object->lsc_prev_in_cluster, "The first object in third cluster does not point to the last object in the second cluster.");
  }


/** @}*/

/** \defgroup  f360_post_process_longi_stat_clusters_merge_longi_stat_cluster
 *  @{
 */

/** \brief
 * Tests using this test group will verify the functionality of function Merge_Longi_Stat_Clusters that loops through LSC clusters and merges clusters with similar properties.
 */
TEST_GROUP(f360_post_process_longi_stat_clusters_merge_longi_stat_cluster)
{
   const float32_t test_pass_th = 1e-8F;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS] = {};
   uint16_t nr_valid_clusters;
   F360_Longi_Stat_Cluster_T valid_clusters[NR_LONGI_STAT_CLUSTERS] = {};
   F360_Calibrations_T calibs = {};
   F360_Tracker_Info_T tracker_info = {};

   /** \setup
    * Initialize tracker calibrations
    * Set up a default scenario where five clusters exists and clusters to be merged is: (1, 3) and (2, 4)
    * - First cluster contains 6 objects with vcs lateral mean -10, longitudinal position starts from -5 and increments with 2m per object (-5, -3, -1, 1, 3) m
    * - Second cluster contains 4 objects with vcs lateral mean 3, longitudinal position starts from 0m and increments with 1m per object (0, 1, 2, 3) m
    * - Third cluster contains 20 objects with vcs lateral mean -9 m, longitudinal position starts from 2m and increments with 1m per object (2-21m)
    * - Fourth cluster contains 4 objects with vcs lateral mean 4, longitudinal position starts from 5m and increments with 1m per object (5, 6, 7, 8) m
    * - Fifth cluster contains 5 objects with vcs lateral mean 9, longitudinal position starts from 20m and increments with 1m per object (20, 21, 22, 23, 24) m
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      F360_Longi_Stat_Cluster_T cluster_1 = Create_LSC_Cluster_And_Set_Obj_Position({ 50U, 51U, 52U, 70U, 71U}, 5U, -5.0F, 2.0F, -10.0F, objects);
      F360_Longi_Stat_Cluster_T cluster_2 = Create_LSC_Cluster_And_Set_Obj_Position({ 1U, 2U, 3U, 4U }, 4U, 0.0F, 1.0F, 3.0F, objects);
      F360_Longi_Stat_Cluster_T cluster_3 = Create_LSC_Cluster_And_Set_Obj_Position(
         { 80U, 81U, 82U, 83U, 84U, 85U, 86U, 87U, 88U, 89U, 
           90U, 91U, 92U, 93U, 94U, 95U, 96U, 97U, 98U, 99U }, 20U, 2.0F, 1.0F, -9.0F, objects);
      F360_Longi_Stat_Cluster_T cluster_4 = Create_LSC_Cluster_And_Set_Obj_Position({ 5U, 6U, 7U, 8U }, 4U, 5.0F, 1.0F, 4.0F, objects);
      F360_Longi_Stat_Cluster_T cluster_5 = Create_LSC_Cluster_And_Set_Obj_Position({ 9U, 10U, 11U, 12U, 13U }, 5U, 20.0F, 1.0F, 9.0F, objects);

      valid_clusters[0U] = cluster_1;
      valid_clusters[1U] = cluster_2;
      valid_clusters[2U] = cluster_3;
      valid_clusters[3U] = cluster_4;
      valid_clusters[4U] = cluster_5;
      nr_valid_clusters = 5U;

   }
};

/** \purpose
 * Verify that function that merges LSC clusters that are close to each other works as expected when there are multiple clusters that should be merged on both sides of host.
 * \req
 * NA.
 */
TEST(f360_post_process_longi_stat_clusters_merge_longi_stat_cluster, Merge_Longi_Stat_Clusters)
{
   /** \precond
    * In test group setup:
    * - Tracker calibrations have been initialized   
    * 5 clusters have been created such that cluster (1,3) and (2,4) is to be merged with each other:
    * - First cluster contains 5 objects with vcs lateral mean -10, longitudinal position starts from -5 and increments with 2m per object (-5, -3, -1, 1, 3) m
    * - Second cluster contains 4 objects with vcs lateral mean 3, longitudinal position starts from 0m and increments with 1m per object (0, 1, 2, 3) m
    * - Third cluster contains 20 objects with vcs lateral mean -9 m, longitudinal position starts from 2m and increments with 1m per object (2-21m)
    * - Fourth cluster contains 4 objects with vcs lateral mean 4, longitudinal position starts from 5m and increments with 1m per object (5, 6, 7, 8) m
    * - Fifth cluster contains 5 objects with vcs lateral mean 9, longitudinal position starts from 20m and increments with 1m per object (20, 21, 22, 23, 24) m
    */

    /** \action
     * Call Merge_Longi_Stat_Clusters
     */
   Merge_Longi_Stat_Clusters(calibs, tracker_info, nr_valid_clusters, valid_clusters);

   /** \result
    * Check that number of valid clusters has been decreased to 3 (2 merges occured)
    * Check that number of objects in first cluster is 25 after merge with third cluster
    * Check that the lateral mean of the first cluster was updated correctly
    * Check that the number of objects in second cluster is 8 after merge with fourth cluster
    * Check that the lateral mean of the second cluster was updated correctly
    */
   CHECK_EQUAL_TEXT(3U, nr_valid_clusters, "The number of valid clusters did not decrease to 3 as expected.");
   
   CHECK_EQUAL_TEXT(25U, valid_clusters[0U].nr_objects, "The number of objects in the first cluster was not correctly updated.");
   DOUBLES_EQUAL_TEXT(-9.2F, valid_clusters[0U].lat_mean, test_pass_th, "The lateral mean of the first cluster was not updated correctly.");

   CHECK_EQUAL_TEXT(8U, valid_clusters[1U].nr_objects, "The number of objects in the first cluster war not correctly updated.");
   DOUBLES_EQUAL_TEXT(3.5F, valid_clusters[1U].lat_mean, test_pass_th, "The lateral mean of the second cluster was not updated correctly.");
}

/** \purpose
 * Verify that function that merges LSC clusters that are close to each other works as expected when there are only two valid clusters 
 * and they are close both laterally and longitudinally.
 * \req
 * NA.
 */
TEST(f360_post_process_longi_stat_clusters_merge_longi_stat_cluster, Merge_Longi_Stat_Clusters_Only_Two_Clusters_That_Should_Merge)
{
   /** \precond
    * In test group setup:
    * - Tracker calibrations have been initialized   
    * 3 clusters have been created such that cluster (1,3) is to be merged with each other:
    * - First cluster contains 5 objects with vcs lateral mean -10, longitudinal position starts from -5 and increments with 2m per object (-5, -3, -1, 1, 3) m
    * - Third cluster contains 20 objects with vcs lateral mean -9 m, longitudinal position starts from 2m and increments with 1m per object (2-21m)
    * Move the third cluster to second slot in array
    * Set number of valid clusters to 2.
    */
   valid_clusters[1U] = valid_clusters[2U];
   nr_valid_clusters = 2U;

   /** \action
    * Call Merge_Longi_Stat_Clusters
    */
   Merge_Longi_Stat_Clusters(calibs, tracker_info, nr_valid_clusters, valid_clusters);

   /** \result
    * Check that number of valid clusters has been decreased to 1
    * Check that number of objects in merged cluster is 25
    * Check that the lateral mean of the merged cluster is -9.2
    */
   CHECK_EQUAL_TEXT(1U, nr_valid_clusters, "Number of valid clusters was not decreased to 1 as expected.");
   CHECK_EQUAL_TEXT(25U, valid_clusters[0].nr_objects, "Number of objects in merged cluster was not 25 as expected.");
   DOUBLES_EQUAL_TEXT(-9.2F, valid_clusters[0U].lat_mean, test_pass_th, "The lateral mean of the merged cluster was not -9.2 as expected.");
}
/** @}*/

/** \defgroup  f360_post_process_longi_stat_clusters
 *  @{
 */

/** \brief
 * Tests using this test group will verify the functionality of function Post_Process_Longi_Stat_Clusters that does post processing of the found clusters. Post processing includes merging of clusters with 
 * similar properties.
 */
TEST_GROUP(f360_post_process_longi_stat_clusters)
{
   const float32_t test_pass_th = 1e-8F;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS] = {};
   uint16_t nr_valid_clusters;
   F360_Longi_Stat_Cluster_T valid_clusters[NR_LONGI_STAT_CLUSTERS] = {};
   F360_Calibrations_T calibs = {};
   F360_Tracker_Info_T tracker_info = {};
   float32_t host_curvature_rear;
   float32_t host_turn_radius;

   /** \setup
    * Initialize tracker calibrations
    * Set up a default scenario where five clusters exists and clusters to be merged is: (1, 3) and (2, 4)
    * - First cluster contains 6 objects with vcs lateral mean -10, longitudinal position starts from -5 and increments with 2m per object (-5, -3, -1, 1, 3) m
    * - Second cluster contains 4 objects with vcs lateral mean 3, longitudinal position starts from 0m and increments with 1m per object (0, 1, 2, 3) m
    * - Third cluster contains 20 objects with vcs lateral mean -9 m, longitudinal position starts from 2m and increments with 1m per object (2-21m)
    * - Fourth cluster contains 4 objects with vcs lateral mean 4, longitudinal position starts from 5m and increments with 1m per object (5, 6, 7, 8) m
    * - Fifth cluster contains 5 objects with vcs lateral mean 9, longitudinal position starts from 20m and increments with 1m per object (20, 21, 22, 23, 24) m
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      F360_Longi_Stat_Cluster_T cluster_1 = Create_LSC_Cluster_And_Set_Obj_Position({ 50U, 51U, 52U, 70U, 71U }, 5U, -5.0F, 2.0F, -10.0F, objects);
      F360_Longi_Stat_Cluster_T cluster_2 = Create_LSC_Cluster_And_Set_Obj_Position({ 1U, 2U, 3U, 4U }, 4U, 0.0F, 1.0F, 3.0F, objects);
      F360_Longi_Stat_Cluster_T cluster_3 = Create_LSC_Cluster_And_Set_Obj_Position(
         { 80U, 81U, 82U, 83U, 84U, 85U, 86U, 87U, 88U, 89U,
           90U, 91U, 92U, 93U, 94U, 95U, 96U, 97U, 98U, 99U }, 20U, 2.0F, 1.0F, -9.0F, objects);
      F360_Longi_Stat_Cluster_T cluster_4 = Create_LSC_Cluster_And_Set_Obj_Position({ 5U, 6U, 7U, 8U }, 4U, 5.0F, 1.0F, 4.0F, objects);
      F360_Longi_Stat_Cluster_T cluster_5 = Create_LSC_Cluster_And_Set_Obj_Position({ 9U, 10U, 11U, 12U, 13U }, 5U, 20.0F, 1.0F, 9.0F, objects);

      valid_clusters[0U] = cluster_1;
      valid_clusters[1U] = cluster_2;
      valid_clusters[2U] = cluster_3;
      valid_clusters[3U] = cluster_4;
      valid_clusters[4U] = cluster_5;
      nr_valid_clusters = 5U;

      host_curvature_rear = 0.0F;
      host_turn_radius = std::abs(host_curvature_rear) > F360_EPSILON ? 1.0F / host_curvature_rear : INFTY;
   }
};

/** \purpose
 * Verify that function that performs post processing of LSC clusters works as expected by verifying that clusters with similar properties are merged 
 * when there are multiple clusters that should be merged on both sides of host.
 * \req
 * NA.
 */
TEST(f360_post_process_longi_stat_clusters, Post_Process_Longi_Stat_Clusters)
{
   /** \precond
    * In test group setup:
    * - Tracker calibrations have been initialized
    * 5 clusters have been created such that cluster (1,3) and (2,4) is to be merged with each other:
    * - First cluster contains 5 objects with vcs lateral mean -10, longitudinal position starts from -5 and increments with 2m per object (-5, -3, -1, 1, 3) m
    * - Second cluster contains 4 objects with vcs lateral mean 3, longitudinal position starts from 0m and increments with 1m per object (0, 1, 2, 3) m
    * - Third cluster contains 20 objects with vcs lateral mean -9 m, longitudinal position starts from 2m and increments with 1m per object (2-21m)
    * - Fourth cluster contains 4 objects with vcs lateral mean 4, longitudinal position starts from 5m and increments with 1m per object (5, 6, 7, 8) m
    * - Fifth cluster contains 5 objects with vcs lateral mean 9, longitudinal position starts from 20m and increments with 1m per object (20, 21, 22, 23, 24) m
    */

    /** \action
     * Call Post_Process_Longi_Stat_Clusters
     */
   Post_Process_Longi_Stat_Clusters(calibs, tracker_info, host_turn_radius, nr_valid_clusters, valid_clusters);

   /** \result
    * Check that number of valid clusters has been decreased to 3 (2 merges occured)
    * Check that number of objects in first cluster is 25 after merge with third cluster
    * Check that the lateral mean of the first cluster was updated correctly
    * Check that the number of objects in second cluster is 8 after merge with fourth cluster
    * Check that the lateral mean of the second cluster was updated correctly
    */
   CHECK_EQUAL_TEXT(3U, nr_valid_clusters, "The number of valid clusters did not decrease to 3 as expected.");

   CHECK_EQUAL_TEXT(25U, valid_clusters[0U].nr_objects, "The number of objects in the first cluster was not correctly updated.");
   DOUBLES_EQUAL_TEXT(-9.2F, valid_clusters[0U].lat_mean, test_pass_th, "The lateral mean of the first cluster was not updated correctly.");

   CHECK_EQUAL_TEXT(8U, valid_clusters[1U].nr_objects, "The number of objects in the first cluster war not correctly updated.");
   DOUBLES_EQUAL_TEXT(3.5F, valid_clusters[1U].lat_mean, test_pass_th, "The lateral mean of the second cluster was not updated correctly.");
}

/** @}*/

/** \defgroup  f360_post_process_longi_stat_clusters_remove_overhead_clusters
 *  @{
 */

/** \brief
 * Tests using this test group will verify the functionality of function Remove_Overhead_Longi_Stat_Clusters
 */
TEST_GROUP(f360_post_process_longi_stat_clusters_remove_overhead_clusters)
{
   uint16_t nr_clusters;
   F360_Longi_Stat_Cluster_T clusters[NR_LONGI_STAT_CLUSTERS] = {};

   /** \setup
    * Set up a default scenario with two clusters
    */
   TEST_SETUP()
   {
      F360_Longi_Stat_Cluster_T cluster_1;
      F360_Longi_Stat_Cluster_T cluster_2;

      // Set height of the clusters to random values below the threshold (4 m)
      cluster_1.height_mean = 1.0F;
      cluster_2.height_mean = 2.0F;

      clusters[0U] = cluster_1;
      clusters[1U] = cluster_2;

      nr_clusters = 2U;
   }
};

/** \purpose  
 * Verify that both clusters are kept when their heights are below threshold
 * \req
 * NA.
 */
TEST(f360_post_process_longi_stat_clusters_remove_overhead_clusters, Remove_Overhead_Longi_Stat_Clusters_Both_Kept)
{
   /** \precond
    * In test group setup:
    * - Two clusters have been set up with heights below threshold
    * Set up exp data identical to input data before function call
    */
   const uint16_t exp_nr_clusters = nr_clusters;
   F360_Longi_Stat_Cluster_T exp_clusters[NR_LONGI_STAT_CLUSTERS] = {};
   for (uint16_t i = 0U; i < exp_nr_clusters; i++)
   {
      exp_clusters[i] = clusters[i];
   }

   /** \action
    * Call Remove_Overhead_Longi_Stat_Clusters
    */
   Remove_Overhead_Longi_Stat_Clusters(nr_clusters, clusters);

   /** \result
    * Check that both clusters are kept after the function call
    */	
   CHECK_EQUAL_TEXT(exp_nr_clusters, nr_clusters, "Number of cluster was not the same as before.");
   for (uint16_t i = 0U; i < exp_nr_clusters; i++)
   {
      DOUBLES_EQUAL_TEXT(exp_clusters[i].height_mean, clusters[i].height_mean, 0.0001F,"Mean heights of clusters was not the same as before.")
   } 
}

/** \purpose  
 * Verify that the first cluster is removed when its heights is above the threshold
 * \req
 * NA.
 */
TEST(f360_post_process_longi_stat_clusters_remove_overhead_clusters, Remove_Overhead_Longi_Stat_Clusters_First_Cluster_Removed)
{
   /** \precond
    * In test group setup:
    * - Two clusters have been set up with heights below threshold
    * Change height of the first cluster to be above the threshold
    * Set up exp data to reflect removal of the first cluster
    */
   const uint16_t exp_nr_clusters = nr_clusters - 1U;
   F360_Longi_Stat_Cluster_T exp_clusters[NR_LONGI_STAT_CLUSTERS] = {};
   exp_clusters[0U] = clusters[1U];
   clusters[0U].height_mean = 6.0F;

   /** \action
    * Call Remove_Overhead_Longi_Stat_Clusters
    */
   Remove_Overhead_Longi_Stat_Clusters(nr_clusters, clusters);

   /** \result
    * Check that both clusters are kept after the function call
    */
   CHECK_EQUAL_TEXT(exp_nr_clusters, nr_clusters, "Number of cluster was not the same as before.");
   for (uint16_t i = 0U; i < exp_nr_clusters; i++)
   {
      DOUBLES_EQUAL_TEXT(exp_clusters[i].height_mean, clusters[i].height_mean, 0.0001F,"Mean heights of clusters was not the same as before.")
   }
}

/** @}*/

/** \defgroup  f360_post_process_longi_stat_clusters_allow_LSCs_with_low_height_or_most_objects_not_in_path
 *  @{
 */

/** \brief
 * Tests using this test group will verify the functionality of function 
 * Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path.
 * This function filters out LSC clusters based on 
 * two criteria: the percentage of objects in the path of the host and the 
 * mean height of the cluster. The function iterates through each cluster 
 * and evaluates its objects to determine if they are in the path of the host 
 * using the `Check_If_Object_In_Path_Of_Host` function. For each cluster, 
 * the percentage of objects in the path is calculated. If this percentage 
 * is below 40% or if the mean height of the cluster is below 2.0 meters, the 
 * cluster is kept, otherwise the LSC cluster is removed.
 * similar properties.
 */
TEST_GROUP(f360_post_process_longi_stat_clusters_allow_LSCs_with_low_height_or_most_objects_not_in_path)
{
   const float32_t test_pass_th = 1e-8F;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS] = {};
   uint16_t nr_valid_clusters;
   F360_Longi_Stat_Cluster_T valid_clusters[NR_LONGI_STAT_CLUSTERS] = {};
   F360_Calibrations_T calibs = {};
   F360_Tracker_Info_T tracker_info = {};
   float32_t host_curvature_rear;
   float32_t host_turn_radius;

   /** \setup
    * Initialize tracker calibrations
    * - First cluster contains 6 objects with vcs lateral mean -10, longitudinal position starts from -5 and increments with 2m per object (-5, -3, -1, 1, 3) m
    * - Second cluster contains 4 objects with vcs lateral mean 3, longitudinal position starts from 0m and increments with 1m per object (0, 1, 2, 3) m
    * - Second cluster is placed in host path (y=0)
    * Host curvature is set to 0 (straight road)
    * The clusters are setup such that their mid longitudinal positions are within 60m
    * The cluster are setup such that their mean heights are below 2m
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      F360_Longi_Stat_Cluster_T cluster_1 = Create_LSC_Cluster_And_Set_Obj_Position({ 1U, 2U, 3U, 4U, 5U }, 5U, -5.0F, 2.0F, -10.0F, objects);
      F360_Longi_Stat_Cluster_T cluster_2 = Create_LSC_Cluster_And_Set_Obj_Position({ 6U, 7U, 8U, 9U }, 4U, 0.0F, 0.0F, 3.0F, objects);

      cluster_1.height_mean = 1.0F; // Low height, should be below 2m
      cluster_2.height_mean = 1.2F; // Low height, should be below 2m

      valid_clusters[0U] = cluster_1;
      valid_clusters[1U] = cluster_2;
      nr_valid_clusters = 2U;

      host_curvature_rear = 0.0F;
      host_turn_radius = std::abs(host_curvature_rear) > F360_EPSILON ? 1.0F / host_curvature_rear : INFTY;
   }
};

/** \purpose  
 * Verify that the LSC clusters, with most objects not in host path, are kept
 * \req
 * NA.
 */
TEST(f360_post_process_longi_stat_clusters_allow_LSCs_with_low_height_or_most_objects_not_in_path, Allow_LSC_Clusters_with_most_objects_not_in_path)
{
   /** \precond
    * 3 clusters have been set up
    * the cluster 1 & 2 are setup such that their mid longitudinal positions are within 60m and their heights are below threshold (2m)
    * the cluster 2 is placed in host path (y=0) and has high height
    * cluster 3 is setup to resemble a guradrail on the side of the road with height above 2m
    * the host rear curvature is set such that the host path would intersect with cluster 3
    * The intention is that cluster 3 should be kept as most of its objects are not in host path
    */
   host_curvature_rear = 0.03F;
   host_turn_radius = std::abs(host_curvature_rear) > F360_EPSILON ? 1.0F / host_curvature_rear : INFTY;
   F360_Longi_Stat_Cluster_T cluster_3 = Create_LSC_Cluster_And_Set_Obj_Position({10U, 11U, 12U, 13U, 14U, 15U, 16U, 17U, 18U}, 9U, 10.0F, 3.0F, 14.0F, objects);
   cluster_3.height_mean = 2.5F; // High height, should be above 2m
   valid_clusters[2U] = cluster_3;
   nr_valid_clusters = 3U;

   const uint16_t exp_nr_valid_clusters = 3U;

   /** \action
    * Call Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path(calibs, host_turn_radius, nr_valid_clusters, valid_clusters);
    */
   Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path(calibs, host_turn_radius, nr_valid_clusters, valid_clusters);

   /** \result
    * Check that all clusters are kept after the function call
    */
   CHECK_EQUAL_TEXT(exp_nr_valid_clusters, nr_valid_clusters,"Number of valid clusters is not as expected.");
}

/** \purpose  
 * Verify that the null pointer condition is handled correctly when checking if objects are in host path
 * \req
 * NA.
 */
TEST(f360_post_process_longi_stat_clusters_allow_LSCs_with_low_height_or_most_objects_not_in_path, Check_Null_ptr_condition_is_working_as_expected)
{
   /** \precond
    * 2 clusters have been set up
    * In the setup, both clusters are setup such that their mid longitudinal positions are within 60m and heights are below threshold (2m)
    * In this test, cluster 1 is modified such that its second object points to nullptr
    * The intention is that both clusters should be kept as the null pointer condition should be handled correctly
    */
   valid_clusters[0U].height_mean = 2.5F;
   objects[1U].lsc_next_in_cluster = nullptr;

   const uint16_t exp_nr_valid_clusters = 2U;

   /** \action
    * Call Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path(calibs, host_turn_radius, nr_valid_clusters, valid_clusters);
    */
   Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path(calibs, host_turn_radius, nr_valid_clusters, valid_clusters);

   /** \result
    * Check that all clusters are kept after the function call
    */
   CHECK_EQUAL_TEXT(exp_nr_valid_clusters, nr_valid_clusters, "Number of valid clusters is not as expected.");
}

/** \purpose  
 * Verify that the clusters with high height, with objects longitudinal position within threshold (60m) and in host path are removed
 * \req
 * NA.
 */
TEST(f360_post_process_longi_stat_clusters_allow_LSCs_with_low_height_or_most_objects_not_in_path, Dont_Allow_LSC_Clusters_with_high_height_in_path)
{
   /** \precond
    * 2 clusters have been set up
    * the cluster 1 and 2 is setup such that its mid longitudinal positions is within 60m
    * the cluster 1 is setup with low height (less than 2m)
    * the cluster 2 is placed in host path (y=0)
    * the cluster 2 is modified to have have high height (greater than 2m)
    * the intention is that cluster 2 should be removed as its height is above threshold and its objects are in host path
    */
   F360_Longi_Stat_Cluster_T cluster_2_new = Create_LSC_Cluster_And_Set_Obj_Position({ 6U, 7U, 8U, 9U }, 4U, 0.0F, 1.0F, 0.0F, objects);
   cluster_2_new.height_mean = 2.2F; // High height, should be above 2m
   valid_clusters[1U] = cluster_2_new;

   const uint16_t exp_nr_valid_clusters = 1U;

   /** \action
    * Call Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path(calibs, host_turn_radius, nr_valid_clusters, valid_clusters);
    */
   Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path(calibs, host_turn_radius, nr_valid_clusters, valid_clusters);

   /** \result
    * Check that all clusters are kept after the function call
    */
   CHECK_EQUAL_TEXT(exp_nr_valid_clusters, nr_valid_clusters, "Number of valid clusters is not as expected.");
}

/** \purpose  
 * Verify that the clusters with low height, with objects longitudinal position beyond threshold (60m) and in host path are removed
 * \req
 * NA.
 */
TEST(f360_post_process_longi_stat_clusters_allow_LSCs_with_low_height_or_most_objects_not_in_path, Dont_Allow_LSC_Clusters_with_low_height_but_in_path)
{
   /** \precond
    * 2 clusters have been set up with heights below threshold (2m)
    * the cluster 1 is setup such that its mid longitudinal position is within 60m
    * the cluster 2 is modified to have the object's longitudinal position beyond 60m
    * the cluster 2 is placed in host path (y=0)
    * Both cluster are setup with low height
    * The intention is that cluster 2 should be removed as its objects are in host path
    */
   F360_Longi_Stat_Cluster_T cluster_2_new = Create_LSC_Cluster_And_Set_Obj_Position({ 6U, 7U, 8U, 9U }, 4U, 61.0F, 1.0F, 0.0F, objects);
   cluster_2_new.height_mean = 1.2F; // Low height, should be below 2m
   valid_clusters[1U] = cluster_2_new;

   const uint16_t exp_nr_valid_clusters = 1U;

   /** \action
    * Call Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path(calibs, host_turn_radius, nr_valid_clusters, valid_clusters);
    */
   Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path(calibs, host_turn_radius, nr_valid_clusters, valid_clusters);

   /** \result
    * Check that all clusters are kept after the function call
    */
   CHECK_EQUAL_TEXT(exp_nr_valid_clusters, nr_valid_clusters, "Number of valid clusters is not as expected.");
}


/** \purpose  
 * Verify that the clusters with heights below threshold (2m) are kept, even when cluster's objects are in path of the host
 * \req
 * NA.
 */
TEST(f360_post_process_longi_stat_clusters_allow_LSCs_with_low_height_or_most_objects_not_in_path, Allow_LSC_clusters_with_low_height)
{
   /** \precond
    * In test group setup:
    * 2 clusters have been set up with heights below threshold (2m)
    * The clusters are setup such that their mid longitudinal positions are within 60m
    * The intention is that both clusters should be kept as their heights are below threshold and they are within longitudinal distance threshold
    */
   const uint16_t exp_nr_valid_clusters = 2U;

   /** \action
    * Call Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path(calibs, host_turn_radius, nr_valid_clusters, valid_clusters);
    */
   Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path(calibs, host_turn_radius, nr_valid_clusters, valid_clusters);

   /** \result
    * Check that all clusters are kept after the function call
    */
   CHECK_EQUAL_TEXT(exp_nr_valid_clusters, nr_valid_clusters, "Number of valid clusters is not as expected.");
}
/** @}*/

/** \defgroup  f360_post_process_longi_stat_clusters_check_if_Object_in_path_of_host
 *  @{
 */

 /** \brief
  * The test group for testing Check_If_Object_In_Path_Of_Host function
  */
TEST_GROUP(f360_post_process_longi_stat_clusters_check_if_Object_in_path_of_host)
{
   F360_Object_Track_T object;
   F360_Calibrations_T calibs;
   float host_turn_radius;

   /** \setup
    * Set host host turn raius to 100000 (approx 0 curvature)
    * Set object position to be 20m in front and 5m lateral of host
    * Set object to be stationary CCA 
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      host_turn_radius = 100000.0F;
      object.vcs_position.x = 20.0F;
      object.vcs_position.y = 5.0F;
   }

};
/** \purpose
 * Purpose is to verify that object which is not close to the host path is identified as not in host path
 * \req
 * NA
 */
TEST(f360_post_process_longi_stat_clusters_check_if_Object_in_path_of_host, Verify_If_Object_Is_Not_In_Path_When_Not_Close_To_Host_Path)
{
   /** \precond
    * The default case from the test setup
    */

   /** \action
    * Call function Check_If_Object_In_Path_Of_Host
    */
   const bool f_object_in_host_path = Check_If_Object_In_Path_Of_Host(calibs, host_turn_radius, object);

   /** \result
    * Verify
    */
   CHECK_FALSE_TEXT(f_object_in_host_path, "The object is incorrectly considered to be in host path");
}

/** \purpose
 * Purpose is to verify that object which is longitudinally close to host is 
 * considered not in host path when between 1 and 2 m away from host path
 * \req
 * NA
 */
TEST(f360_post_process_longi_stat_clusters_check_if_Object_in_path_of_host, Verify_If_Object_Is_Not_In_Path_When_Longituinally_Close)
{
   /** \precond
    * Set object position to be 1.5m lateral of host
    * Set object position to be 3m in front of host
    */
   object.vcs_position.x = 3.0F;
   object.vcs_position.y = 1.5F;
   
   /** \action
    * Call function Check_If_Object_In_Path_Of_Host
    */
   const bool f_object_in_host_path = Check_If_Object_In_Path_Of_Host(calibs, host_turn_radius, object);

   /** \result
    * Verify
    */
   CHECK_FALSE_TEXT(f_object_in_host_path, "The object is incorrectly considered to be in host path");
}

/** \purpose
 * Purpose is to verify that object which is not longitudinally close enough to host is
 * considered in path when between 1 and 2 m away from host path
 * \req
 * NA
 */
TEST(f360_post_process_longi_stat_clusters_check_if_Object_in_path_of_host, Verify_If_Object_Is_In_Path_When_Close_To_Path_And_Not_Longitudinally_Close)
{
   /** \precond
    * Set object position to be 1.5m lateral of host
    * Set object position to be 5.6m in front of host
    */
   object.vcs_position.x = 5.6F;
   object.vcs_position.y = 1.5F;

   /** \action
    * Call function Check_If_Object_In_Path_Of_Host
    */
   const bool f_object_in_host_path = Check_If_Object_In_Path_Of_Host(calibs, host_turn_radius, object);

   /** \result
    * Verify
    */
   CHECK_TRUE_TEXT(f_object_in_host_path, "The object is incorrectly considered to be not in host path");
}


/** \purpose
 * Purpose is to verify that object which is close to the host path is concidered in host path
 * \req
 * NA
 */
TEST(f360_post_process_longi_stat_clusters_check_if_Object_in_path_of_host, Verify_If_Object_Is_Not_In_Path_When_Laterally_Close_To_Host_Path)
{
   /** \precond
    * The object location is set to be located close to the host path
    */
   object.vcs_position.x = 20.0F;
   object.vcs_position.y = 1.0F;

   /** \action
    * Call function Check_If_Object_In_Path_Of_Host
    */
   const bool f_object_in_host_path = Check_If_Object_In_Path_Of_Host(calibs, host_turn_radius, object);

   /** \result
    * Verify
    */
   CHECK_TRUE_TEXT(f_object_in_host_path, "The object is incorrectly considered to be not in host path");
}

/** \purpose
 * Purpose is to verify that object which is not close to the host path while host has a non-zero curvature is concidered to be not in host path
 * \req
 * NA
 */
TEST(f360_post_process_longi_stat_clusters_check_if_Object_in_path_of_host, Verify_If_Object_Is_Not_In_Path_When_Not_Close_Path_With_Curved_Host_Path)
{
   /** \precond
    * set up the object postion and host curvature
    */
   host_turn_radius = 50.0F; // corresponding to 0.02 curvature
   object.vcs_position.x = 30.0F;
   object.vcs_position.y = 2.0F;

   /** \action
    * Call function Check_If_Object_In_Path_Of_Host
    */
   const bool f_object_in_host_path = Check_If_Object_In_Path_Of_Host(calibs, host_turn_radius, object);

   /** \result
    * Verify
    */
   CHECK_FALSE_TEXT(f_object_in_host_path, "The object is incorrectly considered to be in host path");
}
/** @}*/

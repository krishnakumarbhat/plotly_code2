/** \file
 * This file contains unit tests for content of f360_cluster_objects_for_lsc.cpp file
 */

#include "f360_cluster_objects_for_lsc.h"
#include <CppUTest/TestHarness.h>

#include "f360_math_func.h"
#include "f360_lsc_data_generator.h"
#include "f360_update_longi_stat_curves.h"
#include "f360_set_variant.h"

using namespace f360_variant_A;

/** \defgroup  f360_cluster_objects_for_lsc_Init_New_Iteration
 *  @{
 */

/** \brief
 * Test group for testing of function Init_New_Iteration() which prepares data for a new LSC
 * clustering iteration.
 */
TEST_GROUP(f360_cluster_objects_for_lsc_Init_New_Iteration)
{
   uint16_t nr_next_ids_of_interest;
   uint16_t next_ids_of_interest[NUMBER_OF_OBJECT_TRACKS];
   uint16_t nr_skipped_ids_of_interest;
   uint16_t nr_clustered_ids;
   uint16_t nr_ids_of_interest;
   uint16_t ids_of_interest[NUMBER_OF_OBJECT_TRACKS];
   
   /** \setup
    * Fill next_ids_of_interest array with 3 arbitrary ids.
    */
   TEST_SETUP()
   {
      nr_next_ids_of_interest = 3U;
      next_ids_of_interest[0] = 10U;
      next_ids_of_interest[1] = 20U;
      next_ids_of_interest[2] = 30U;
   }
};

/** \purpose  
 * Purpose is to verify functionality of Init_New_Iteration(). 
 * We expect this function to reset the number of skipped ids and clustered ids
 * from a previous iteration.
 * We also expect the function to fill ids_of_interest array with ids from next_ids_of_interest.
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Init_New_Iteration, Init_New_Iteration_Verify_Variables_Reset_And_Update)
{
   /** \action
    * Call function
    */
   Init_New_Iteration(nr_next_ids_of_interest, next_ids_of_interest, nr_skipped_ids_of_interest, nr_clustered_ids, nr_ids_of_interest, ids_of_interest);

   /** \result
    * Verify that variables have been reseted
    * Verify that ids of interest now matches ids of next_ids_of_interest
    */
   CHECK_EQUAL(0U, nr_skipped_ids_of_interest);
   CHECK_EQUAL(0U, nr_clustered_ids);

   CHECK_EQUAL(nr_next_ids_of_interest, nr_ids_of_interest);
   for (uint32_t i = 0U; i < nr_ids_of_interest; i++)
   {
      CHECK_EQUAL(next_ids_of_interest[i], ids_of_interest[i]);
   }
}
/** @}*/


/** \defgroup  f360_cluster_objects_for_lsc_Cluster_Longi_Stat_Objects
 *  @{
 */

 /** \brief
  * Test group for testing of function Cluster_Longi_Stat_Objects() which clusters objects for LSC. 
  * Tests in this test group also implicitly tests function Is_Object_Valid_For_LSC_Cluster().
  * Objects are created in longitudinal sorted order to match the expectation that objects have been 
  * sorted in longitudinal order before call to this function.
  */
TEST_GROUP(f360_cluster_objects_for_lsc_Cluster_Longi_Stat_Objects)
{
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   uint16_t nr_ids_of_interest;
   uint16_t ids_of_interest[NUMBER_OF_OBJECT_TRACKS];
   uint16_t current_idx;
   uint16_t nr_skipped_ids_of_interest;
   uint16_t skipped_ids_of_interest[NUMBER_OF_OBJECT_TRACKS];
   uint16_t nr_clustered_ids;
   uint16_t clustered_ids[NUMBER_OF_OBJECT_TRACKS];
   F360_Calibrations_T calibs;
   F360_Host_T host;

   /** \setup
    * Create 7 objects
    *    - First object is invalid due to CTCA filter type
    *    - Second object is valid, should start a cluster
    *    - Third object is valid, should be added to cluster
    *    - Fourth object is valid but too far in lateral direction, should be added to skipped ids array
    *    - Fifth object is invalid due to speed too high
    *    - Sixth object is valid but too far away in lateral direction, should be added to skipped ids array
    *    - Seventh object is too far away in longitudinal direction, we expect the function to break and return after reaching this object
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      host.speed = 3.0F;
      
      // Setup the 7 objects
      objects[0].id = 1;
      objects[0].vcs_position.x = 0.0F;
      objects[0].vcs_position.y = 0.0F;
      objects[0].reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = objects[0].vcs_position;
      objects[0].bbox.Set_Center(center);

      objects[1].id = 2;
      objects[1].vcs_position.x = 1.0F;
      objects[1].vcs_position.y = 0.0F;
      objects[1].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[1].vcs_position;
      objects[1].bbox.Set_Center(center);

      objects[2].id = 3;
      objects[2].vcs_position.x = 1.5F;
      objects[2].vcs_position.y = 0.0F;
      objects[2].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[2].vcs_position;
      objects[2].bbox.Set_Center(center);

      objects[3].id = 4;
      objects[3].vcs_position.x = 2.0F;
      objects[3].vcs_position.y = 10.0F;
      objects[3].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[3].vcs_position;
      objects[3].bbox.Set_Center(center);

      objects[4].id = 5;
      objects[4].vcs_position.x = 2.5F;
      objects[4].vcs_position.y = 0.0F;
      objects[4].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[4].vcs_position;
      objects[4].bbox.Set_Center(center);

      objects[5].id = 6;
      objects[5].vcs_position.x = 3.0F;
      objects[5].vcs_position.y = -10.0F;
      objects[5].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[5].vcs_position;
      objects[5].bbox.Set_Center(center);

      objects[6].id = 7;
      objects[6].vcs_position.x = 10.0F;
      objects[6].vcs_position.y = 0.0F;
      objects[6].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[6].vcs_position;
      objects[6].bbox.Set_Center(center);

      // Fill object ids to array of interesting ids
      nr_ids_of_interest = 7U;
      for (uint32_t i = 0U; i < nr_ids_of_interest; i++)
      {
         ids_of_interest[i] = i + 1U;
      }
   }
};

/** \purpose
 * Purpose is to verify functionality of Cluster_Longi_Stat_Objects() when the function reaches an object 
 * that is too far away in longitudinal direction and thus breaks. We verify that expected objects have 
 * been clustered, skipped or ignored and verify that the function have breaked on the expected index in
 * ids_of_interest array.
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Cluster_Longi_Stat_Objects, Cluster_Longi_Stat_Objects_Verify_Clustering_When_Reaching_Object_Too_Far_Away_In_Long_Dir)
{

   /** \action
    * Call function
    */
   Cluster_Longi_Stat_Objects(objects, nr_ids_of_interest, ids_of_interest, calibs, host, current_idx, nr_skipped_ids_of_interest, skipped_ids_of_interest, nr_clustered_ids, clustered_ids);

   /** \result
    * Verify that first, second, third and fifth object have been clustered
    * Verify that fourth and sixth object have been added to skipped_ids_of_interest
    * Verify that inner loop have breaked on seventh object (index 6)
    */
   CHECK_EQUAL(4U, nr_clustered_ids);
   CHECK_EQUAL(1U, clustered_ids[0]);
   CHECK_EQUAL(2U, clustered_ids[1]);
   CHECK_EQUAL(3U, clustered_ids[2]);
   CHECK_EQUAL(5U, clustered_ids[3]);

   CHECK_EQUAL(2U, nr_skipped_ids_of_interest);
   CHECK_EQUAL(4U, skipped_ids_of_interest[0]);
   CHECK_EQUAL(6U, skipped_ids_of_interest[1]);

   CHECK_EQUAL(6U, current_idx);
}

/** \purpose
 * Purpose is to verify functionality of Cluster_Longi_Stat_Objects() when the function does not reach an object
 * that is too far away in longitudinal direction and thus encounters the end of the object list. We verify that expected objects have
 * been clustered, skipped or ignored and verify that the function have handled all objects by checking the index in the ids_of_interest array.
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Cluster_Longi_Stat_Objects, Cluster_Longi_Stat_Objects_Verify_Clustering_When_Not_Reaching_An_Object_Too_Far_Away_In_Long_Dir)
{
   /** \precond
    * Modify longitudinal position of object seven to be within valid distance to object 3
    * which is the latest object that should have been added to the cluster.
    */
   objects[6].vcs_position.x = 3.5F;
   Point center = objects[6].vcs_position;
   objects[6].bbox.Set_Center(center);

   /** \action
    * Call function
    */
   Cluster_Longi_Stat_Objects(objects, nr_ids_of_interest, ids_of_interest, calibs,  host, current_idx, nr_skipped_ids_of_interest, skipped_ids_of_interest, nr_clustered_ids, clustered_ids);

   /** \result
    * Verify that second, third and seventh object have been clustered
    * Verify that fourth and sixth object have been added to skipped_ids_of_interest
    * Verify that we have reached the last index in the ids_of_interest array
    */
   CHECK_EQUAL(5U, nr_clustered_ids);
   CHECK_EQUAL(1U, clustered_ids[0]);
   CHECK_EQUAL(2U, clustered_ids[1]);
   CHECK_EQUAL(3U, clustered_ids[2]);
   CHECK_EQUAL(5U, clustered_ids[3]);
   CHECK_EQUAL(7U, clustered_ids[4]);

   CHECK_EQUAL(2U, nr_skipped_ids_of_interest);
   CHECK_EQUAL(4U, skipped_ids_of_interest[0]);
   CHECK_EQUAL(6U, skipped_ids_of_interest[1]);

   CHECK_EQUAL(7U, current_idx);
}

/** \purpose
 * Purpose is to verify functionality of Cluster_Longi_Stat_Objects() when the function does not reach an object
 * that is too far away in longitudinal direction according to the extended clustering gate
 * and thus encounters the end of the object list. We verify that expected objects have
 * been clustered, skipped or ignored and verify that the function have handled all objects by checking the index in the ids_of_interest array.
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Cluster_Longi_Stat_Objects, Cluster_Longi_Stat_Objects_Verify_Clustering_When_Dets_In_Extended_Long_Range)
{
   /** \precond
    * Make the host go faster than 80 km/h which will trigger clustering gate extension
    * 
    * No changes to the object's list since the last one was previously outside the gate
    * but due to highway speed will be included into a cluster
    */
   host.speed = 22.3F;

   /** \action
    * Call function
    */
   Cluster_Longi_Stat_Objects(objects, nr_ids_of_interest, ids_of_interest, calibs,  host, current_idx, nr_skipped_ids_of_interest, skipped_ids_of_interest, nr_clustered_ids, clustered_ids);

   /** \result
    * Verify that second, third and seventh object have been clustered
    * Verify that fourth and sixth object have been added to skipped_ids_of_interest
    * Verify that we have reached the last index in the ids_of_interest array
    */
   CHECK_EQUAL(5U, nr_clustered_ids);
   CHECK_EQUAL(1U, clustered_ids[0]);
   CHECK_EQUAL(2U, clustered_ids[1]);
   CHECK_EQUAL(3U, clustered_ids[2]);
   CHECK_EQUAL(5U, clustered_ids[3]);
   CHECK_EQUAL(7U, clustered_ids[4]);

   CHECK_EQUAL(2U, nr_skipped_ids_of_interest);
   CHECK_EQUAL(4U, skipped_ids_of_interest[0]);
   CHECK_EQUAL(6U, skipped_ids_of_interest[1]);

   CHECK_EQUAL(7U, current_idx);
}

/** \purpose
 * Purpose is to verify functionality of Cluster_Longi_Stat_Objects() when the function reaches an object 
 * that is too far away in longitudinal direction according to the extended clustering gate and thus breaks. 
 * We verify that expected objects have 
 * been clustered, skipped or ignored and verify that the function have breaked on the expected index in
 * ids_of_interest array.
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Cluster_Longi_Stat_Objects, Cluster_Longi_Stat_Objects_Verify_Clustering_When_Dets_Outside_Extended_Long_Range)
{

   /** \precond
    * Modify longitudinal position of object seven to be outside valid distance to object 3
    * which is the latest object that should have been added to the cluster.
    * 
    * Make the host go faster than 80 km/h which will trigger clustering gate extension
    */
   host.speed = 22.3F;
   objects[6].vcs_position.x = 15.0F;
   Point center = objects[6].vcs_position;
   objects[6].bbox.Set_Center(center);

   /** \action
    * Call function
    */
   Cluster_Longi_Stat_Objects(objects, nr_ids_of_interest, ids_of_interest, calibs, host, current_idx, nr_skipped_ids_of_interest, skipped_ids_of_interest, nr_clustered_ids, clustered_ids);

   /** \result
    * Verify that first, second, third and fifth object have been clustered
    * Verify that fourth and sixth object have been added to skipped_ids_of_interest
    * Verify that inner loop have breaked on seventh object (index 6)
    */
   CHECK_EQUAL(4U, nr_clustered_ids);
   CHECK_EQUAL(1U, clustered_ids[0]);
   CHECK_EQUAL(2U, clustered_ids[1]);
   CHECK_EQUAL(3U, clustered_ids[2]);
   CHECK_EQUAL(5U, clustered_ids[3]);

   CHECK_EQUAL(2U, nr_skipped_ids_of_interest);
   CHECK_EQUAL(4U, skipped_ids_of_interest[0]);
   CHECK_EQUAL(6U, skipped_ids_of_interest[1]);

   CHECK_EQUAL(6U, current_idx);
}


/** \purpose
 * Purpose is to verify functionality of Cluster_Longi_Stat_Objects() when the function doesn't reaches an object
 * that is too far in lateral direction according to the extended clustering gate
 * and thus encounters the end of the object list. We verify that expected objects have
 * been clustered, skipped or ignored and verify that the function have handled all objects by checking the index in the ids_of_interest array.
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Cluster_Longi_Stat_Objects, Cluster_Longi_Stat_Objects_Verify_Clustering_When_Dets_In_Extended_Lat_Range)
{
   /** \precond
    * Make the host go faster than 80 km/h which will trigger clustering gate extension
    * 
    * Two new objects that are outside extended lateral clustering gate
    */
   host.speed = 22.3F;

   objects[7].id = 8;
   objects[7].vcs_position.x = 10.5F;
   objects[7].vcs_position.y = -10.0F;
   objects[7].reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = objects[7].vcs_position;
   objects[7].bbox.Set_Center(center);

   objects[8].id = 9;
   objects[8].vcs_position.x = 11.0F;
   objects[8].vcs_position.y = 10.0F;
   objects[8].reference_point = F360_REFERENCE_POINT_CENTER;
   center = objects[8].vcs_position;
   objects[8].bbox.Set_Center(center);

   nr_ids_of_interest += 2U;
   for (uint32_t i = 7U; i < nr_ids_of_interest; i++)
   {
      ids_of_interest[i] = i + 1U;
   }

   /** \action
    * Call function
    */
   Cluster_Longi_Stat_Objects(objects, nr_ids_of_interest, ids_of_interest, calibs, host, current_idx, nr_skipped_ids_of_interest, skipped_ids_of_interest, nr_clustered_ids, clustered_ids);

   /** \result
    * Verify that second, third and seventh object have been clustered
    * Verify that fourth and sixth object have been added to skipped_ids_of_interest
    * Verify that we have reached the last index in the ids_of_interest array
    */
   CHECK_EQUAL(5U, nr_clustered_ids);
   CHECK_EQUAL(1U, clustered_ids[0]);
   CHECK_EQUAL(2U, clustered_ids[1]);
   CHECK_EQUAL(3U, clustered_ids[2]);
   CHECK_EQUAL(5U, clustered_ids[3]);
   CHECK_EQUAL(7U, clustered_ids[4]);

   CHECK_EQUAL(4U, nr_skipped_ids_of_interest);
   CHECK_EQUAL(4U, skipped_ids_of_interest[0]);
   CHECK_EQUAL(6U, skipped_ids_of_interest[1]);
   CHECK_EQUAL(8U, skipped_ids_of_interest[2]);
   CHECK_EQUAL(9U, skipped_ids_of_interest[3]);

   CHECK_EQUAL(9U, current_idx);
}

/** \purpose
 * Purpose is to verify functionality of Cluster_Longi_Stat_Objects() when the function does not reach an object
 * that is too far away in longitudinal direction according to the reduced clustering gate due to low host speed
 * and object's low longitudinal position.
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Cluster_Longi_Stat_Objects, Cluster_Longi_Stat_Objects_Verify_Clustering_When_Objs_In_Reduced_Range_Host_Low_Speed)
{
   /** \precond
    * Make the host go slower than 80 km/h which will potentially trigger smaller clustering gate
    * Set third object's position to be just inside reduced clustering gate (position difference = 3.74 m)
    * Set fifth object's position to be just outside reduced clustering gate (position difference = 3.76 m)
    */
   host.speed = 2.7F;
   objects[2].vcs_position.x = 4.74F;
   objects[2].bbox.Set_Center(objects[2].vcs_position);
   objects[4].vcs_position.x = 8.51F;
   objects[4].bbox.Set_Center(objects[4].vcs_position);

   /** \action
    * Call function
    */
   Cluster_Longi_Stat_Objects(objects, nr_ids_of_interest, ids_of_interest, calibs,  host, current_idx, nr_skipped_ids_of_interest, skipped_ids_of_interest, nr_clustered_ids, clustered_ids);

   /** \result
    * Verify that first, second and third objects have been clustered
    */
   CHECK_EQUAL(3U, nr_clustered_ids);
   CHECK_EQUAL(1U, clustered_ids[0]);
   CHECK_EQUAL(2U, clustered_ids[1]);
   CHECK_EQUAL(3U, clustered_ids[2]);
}

/** @}*/


/** \defgroup  f360_cluster_objects_for_lsc_Handle_LSC_Cluster
 *  @{
 */

 /** \brief
  * Test group for testing of function Handle_LSC_Cluster() which validates a LSC cluster
  * and derives some properties of the cluster if it is valid.
  * Objects are created in longitudinal sorted order to match the expectation that objects have been
  * sorted in longitudinal order before call to this function.
  */
TEST_GROUP(f360_cluster_objects_for_lsc_Handle_LSC_Cluster)
{
   uint16_t nr_clustered_ids;
   uint16_t clustered_ids[NUMBER_OF_OBJECT_TRACKS];
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   uint16_t nr_valid_clusters;
   F360_Longi_Stat_Cluster_T valid_clusters[NR_LONGI_STAT_CLUSTERS];
   F360_Calibrations_T calibs;

   float32_t expected_lat_mean;
   float32_t expected_height_mean;

   /** \setup
    * Create 3 objects and add them to the clustered_ids array.
    * Assign an arbitrary vcs position to the objects and calculate their mean lateral position
    * To verify that the function correctly appends a valid cluster to the array of all clusters
    * we mimic that there already exists one cluster.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      // Setup the 3 objects
      objects[0].id = 1;
      objects[0].vcs_position.x = 0.0F;
      objects[0].vcs_position.y = 0.0F;
      objects[0].otg_height = 11.0F;
      objects[0].reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = objects[0].vcs_position;
      objects[0].bbox.Set_Center(center);

      objects[1].id = 2;
      objects[1].vcs_position.x = 1.0F;
      objects[1].vcs_position.y = 0.5F;
      objects[1].otg_height = 4.0F;
      objects[1].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[1].vcs_position;
      objects[1].bbox.Set_Center(center);

      objects[2].id = 3;
      objects[2].vcs_position.x = 1.5F;
      objects[2].vcs_position.y = -0.2F;
      objects[2].otg_height = 7.0F;
      objects[2].reference_point = F360_REFERENCE_POINT_CENTER;
      center = objects[2].vcs_position;
      objects[2].bbox.Set_Center(center);

      // Find the lateral mean and mean height
      float32_t lat_pos_array[NUMBER_OF_OBJECT_TRACKS];
      float32_t height_array[NUMBER_OF_OBJECT_TRACKS];
      for (uint32_t i = 0U; i < 3U; i++)
      {
         uint32_t obj_idx = objects[i].id - 1U;
         lat_pos_array[i] = objects[obj_idx].vcs_position.y;
         height_array[i] = objects[obj_idx].otg_height;
      }
      expected_lat_mean = F360_Mean(lat_pos_array, 3U);
      expected_height_mean = F360_Mean(height_array, 3U);

      // Fill object ids to array of clustered ids
      nr_clustered_ids = 3U;
      for (uint32_t i = 0U; i < nr_clustered_ids; i++)
      {
         clustered_ids[i] = i + 1U;
      }

      // Mimic that one cluster already exists in the array of all clusters
      nr_valid_clusters = 1U;
   }
};

/** \purpose
 * Purpose is to verify that a cluster containing enough objects is deemed as valid and that
 * the cluster properties are calculated correctly from the clustered objects.
 * We also verify that the valid cluster have been appended correctly to the array
 * of all valid clusters.
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Handle_LSC_Cluster, Handle_LSC_Cluster_Valid_Cluster)
{
   /** \action
    * Call function
    */
   Handle_LSC_Cluster(nr_clustered_ids, clustered_ids, calibs, objects, nr_valid_clusters, valid_clusters);

   /** \result
    * Verify that cluster have been appended to the array of all valid clusters
    * Verify calculated properties of the cluster
    */
   CHECK_EQUAL(2U, nr_valid_clusters);
   CHECK_EQUAL(3U, valid_clusters[1].nr_objects); // Verify that there are 3 objects in the appended cluster
   CHECK_EQUAL(1U, valid_clusters[1].first_object->id); // Verify object ids of the appended cluster
   CHECK_EQUAL(2U, valid_clusters[1].first_object->lsc_next_in_cluster->id); // Verify object ids of the appended cluster
   CHECK_EQUAL(3U, valid_clusters[1].last_object->id); // Verify object ids of the appended cluster

   // Verify cluster properties
   DOUBLES_EQUAL(objects[0].vcs_position.x, valid_clusters[1].first_object->vcs_position.x, F360_EPSILON);
   DOUBLES_EQUAL(objects[2].vcs_position.x, valid_clusters[1].last_object->vcs_position.x, F360_EPSILON);
   DOUBLES_EQUAL(expected_lat_mean, valid_clusters[1].lat_mean, F360_EPSILON);
   DOUBLES_EQUAL(expected_height_mean, valid_clusters[1].height_mean, F360_EPSILON);
}

/** \purpose
 * Purpose is to verify that a clusters mean height is calculated correctly when one of the objects is
 * outside the relevant zone for mean height calculations (i.e. has a otg_height of 0).
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Handle_LSC_Cluster, Handle_LSC_Cluster_Obj_Zero_OTG_Height)
{
   /** \precond
    * Move one detection such that its curvi lat pos is outside the relevant zone
    * Re-calculate the expected mean height
    */
   objects[2U].otg_height = 0.0F;
   expected_height_mean = (objects[0U].otg_height + objects[1U].otg_height) / 2.0F; 
   /** \action
    * Call function
    */
   Handle_LSC_Cluster(nr_clustered_ids, clustered_ids, calibs, objects, nr_valid_clusters, valid_clusters);

   /** \result
    * Verify that the correct cluster mean height is calculated
    */

   DOUBLES_EQUAL(expected_height_mean, valid_clusters[1].height_mean, F360_EPSILON);
}

/** \purpose
 * Purpose is to verify that a cluster with only 2 objects is deemed as invalid and not appended to
 * the array of all valid clusters. No cluster properties should be calculated in this case.
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Handle_LSC_Cluster, Handle_LSC_Cluster_Invalid_Cluster)
{
   /** \precond
    * Modify the number of clustered ids to only 2, thus making it invalid
    */
   nr_clustered_ids = 2U;

   /** \action
    * Call function
    */
   Handle_LSC_Cluster(nr_clustered_ids, clustered_ids, calibs, objects, nr_valid_clusters, valid_clusters);

   /** \result
    * Verify that no cluster have been appended to the array of all valid clusters
    */
   CHECK_EQUAL(1U, nr_valid_clusters);
}
/** @}*/

/** \defgroup  f360_cluster_objects_for_lsc_Arrange_Remaining_Objects
 *  @{
 */

 /** \brief
  * Test group for testing of function Arrange_Remaining_Objects() which
  * arranges objects for a potential upcoming clustering iteration.
  */
TEST_GROUP(f360_cluster_objects_for_lsc_Arrange_Remaining_Objects)
{
   uint16_t nr_ids_of_interest;
   uint16_t ids_of_interest[NUMBER_OF_OBJECT_TRACKS];
   uint16_t current_idx;
   uint16_t nr_skipped_ids_of_interest;
   uint16_t skipped_ids_of_interest[NUMBER_OF_OBJECT_TRACKS];
   uint16_t nr_next_ids_of_interest;
   uint16_t next_ids_of_interest[NUMBER_OF_OBJECT_TRACKS];
   F360_Calibrations_T calibs;

   bool f_data_remaining;

   /** \setup
    * Assign 6 objects to nr_ids_of_interest which contains objects that have been used 
    * for clustering during this iteration.
    * Assign 3 of these objects to skipped_ids_of_interest array which means that they have been 
    * skipped during this iteration.
    * Set current_idx to 5 meaning that the clustering algorithm have breaked before reaching the
    * last object.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      nr_ids_of_interest = 6U;
      for (uint32_t i = 0U; i < nr_ids_of_interest; i++)
      {
         ids_of_interest[i] = i + 1U;
      }

      // Let object index 1 and 2 be skipped
      nr_skipped_ids_of_interest = 2U;
      skipped_ids_of_interest[0] = 2U;
      skipped_ids_of_interest[1] = 3U;

      current_idx = 5U;

   }
};

/** \purpose
 * Purpose is to verify that skipped and not yet reached objects are arranged correctly
 * for an upcoming new clustering iteration. In this test there are both skipped and not
 * yet reached objects and there are enough objects left to do a new clustering iteration.
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Arrange_Remaining_Objects, Arrange_Remaining_Objects_Skipped_And_Not_Reached_Objects)
{
   /** \action
    * Call function
    */
   f_data_remaining = Arrange_Remaining_Objects(nr_ids_of_interest, ids_of_interest, current_idx, nr_skipped_ids_of_interest, skipped_ids_of_interest, calibs, nr_next_ids_of_interest, next_ids_of_interest);

   /** \result
    * Verify function have returned true indicating that there are enough objects for another clustering iteration
    * Verify that correct number of next objects is returned
    * Verify that ids have been arranged in expected order
    */
   CHECK_TRUE(f_data_remaining);
   CHECK_EQUAL(3U, nr_next_ids_of_interest);
   CHECK_EQUAL(2U, next_ids_of_interest[0]); // First skipped id should be first in next_ids_of_interest array
   CHECK_EQUAL(3U, next_ids_of_interest[1]); // Second skipped id should be second in next_ids_of_interest array
   CHECK_EQUAL(6U, next_ids_of_interest[2]); // Last object that was not reached should be last in next_ids_of_interest array
}

/** \purpose
 * Purpose is to verify that skipped and not yet reached objects are arranged correctly
 * for an upcoming new clustering iteration. In this test there are only skipped objects and 
 * there are enough objects left to do a new clustering iteration.
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Arrange_Remaining_Objects, Arrange_Remaining_Objects_Only_Not_Reached_Objects)
{
   /** \precond
    * Modify the number of skipped ids
    * Modify current_idx to mimic that clustering algo have breaked with enough objects not yet reached
    */
   nr_skipped_ids_of_interest = 0U;
   current_idx = 2U;

   /** \action
    * Call function
    */
   f_data_remaining = Arrange_Remaining_Objects(nr_ids_of_interest, ids_of_interest, current_idx, nr_skipped_ids_of_interest, skipped_ids_of_interest, calibs, nr_next_ids_of_interest, next_ids_of_interest);

   /** \result
    * Verify function have returned true indicating that there are enough objects for another clustering iteration
    * Verify that correct number of next objects is returned
    * Verify that ids have been arranged in expected order
    */
   CHECK_TRUE(f_data_remaining);
   CHECK_EQUAL(4U, nr_next_ids_of_interest);
   CHECK_EQUAL(3U, next_ids_of_interest[0]); // Object reached that caused clustering algorithm to break
   CHECK_EQUAL(4U, next_ids_of_interest[1]); // First object not yet reached
   CHECK_EQUAL(5U, next_ids_of_interest[2]); // Second object not yet reached
   CHECK_EQUAL(6U, next_ids_of_interest[3]); // Last object not yet reached
}

/** \purpose
 * Purpose is to verify that skipped and not yet reached objects are arranged correctly
 * for an upcoming new clustering iteration. In this test there are only skipped objects and
 * there are enough objects left to do a new clustering iteration.
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Arrange_Remaining_Objects, Arrange_Remaining_Objects_Only_Skipped_Objects)
{
   /** \precond
    * Modify the number of skipped ids so there are enough objects for another clustering iteration
    * Add another id to skipped ids of interest with an arbitrary id.
    * Modify current_idx to mimic that clustering algo have processed all objects
    */
   nr_skipped_ids_of_interest = 3U;
   skipped_ids_of_interest[2] = 5U;
   current_idx = 6U;

   /** \action
    * Call function
    */
   f_data_remaining = Arrange_Remaining_Objects(nr_ids_of_interest, ids_of_interest, current_idx, nr_skipped_ids_of_interest, skipped_ids_of_interest, calibs, nr_next_ids_of_interest, next_ids_of_interest);

   /** \result
    * Verify function have returned true indicating that there are enough objects for another clustering iteration
    * Verify that correct number of next objects is returned
    * Verify that ids have been arranged in expected order
    */
   CHECK_TRUE(f_data_remaining);
   CHECK_EQUAL(3U, nr_next_ids_of_interest);
   CHECK_EQUAL(2U, next_ids_of_interest[0]); // First skipped id should be first in next_ids_of_interest array
   CHECK_EQUAL(3U, next_ids_of_interest[1]); // Second skipped id should be second in next_ids_of_interest array
   CHECK_EQUAL(5U, next_ids_of_interest[2]); // Third skipped id should be third in next_ids_of_interest array
}

/** \purpose
 * Purpose is to verify that skipped and not yet reached objects are arranged correctly
 * for an upcoming new clustering iteration. In this test there are only 2 skipped
 * objects and the clustering algorithm have processed all objects. This means that we 
 * expect funtion to return false as there are not enough objects to form a LSC.
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Arrange_Remaining_Objects, Arrange_Remaining_Objects_Not_Enough_Objects)
{
   /** \precond
    * Modify current_idx to mimic that clustering algo have processed all objects
    */
   current_idx = 6U;

   /** \action
    * Call function
    */
   f_data_remaining = Arrange_Remaining_Objects(nr_ids_of_interest, ids_of_interest, current_idx, nr_skipped_ids_of_interest, skipped_ids_of_interest, calibs, nr_next_ids_of_interest, next_ids_of_interest);

   /** \result
    * Verify function have returned false indicating that there are not enough objects for another clustering iteration
    * Verify that correct number of next objects is returned
    */
   CHECK_FALSE(f_data_remaining);
   CHECK_EQUAL(0U, nr_next_ids_of_interest);
}
/** @}*/

/** \defgroup  f360_cluster_objects_for_lsc_Cluster_Objects_For_LSC
 *  @{
 */

 /** \brief
  * Test group for testing of function Cluster_Objects_For_LSC() which
  * clusters objects through calls to sub functions. This function
  * performs the recursive clustering iterations and output is
  * an array of clusters (if any have been formed)
  */
TEST_GROUP(f360_cluster_objects_for_lsc_Cluster_Objects_For_LSC)
{
   F360_Tracker_Info_T tracker_info;
   F360_Host_T host;

   uint16_t nr_next_ids_of_interest;
   uint16_t next_ids_of_interest[NUMBER_OF_OBJECT_TRACKS];
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   uint16_t nr_valid_clusters;
   F360_Longi_Stat_Cluster_T valid_clusters[NR_LONGI_STAT_CLUSTERS];
   F360_Calibrations_T calibs;

   F360_LSC_Object_Group_Settings_T group_A;
   F360_LSC_Object_Group_Settings_T group_B;
   F360_LSC_Object_Group_Settings_T group_C;
   F360_LSC_Object_Group_Settings_T group_D;

   /** \setup
    * Add four groups of objects
    * Prepare data for first clustering iteration by calling Arrange_First_Iteration()
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Set_Tracker_Variant(tracker_info.variant);

      Initialize_Tracker_Info(tracker_info);

      group_A = Add_LSC_Group_A(tracker_info, objects);
      group_B = Add_LSC_Group_B(tracker_info, objects);
      group_C = Add_LSC_Group_C(tracker_info, objects);
      group_D = Add_LSC_Group_D(tracker_info, objects);
      host.curvature_rear = 0.0F;

      (void)Arrange_First_Iteration(
         tracker_info,
         calibs,
         nr_next_ids_of_interest,
         next_ids_of_interest
         );
   }
};

/** \purpose
 * Purpose is to verify that objects are clustered as expected on a high level.
 * 4 groups of objects have been created, A and B should form one cluster each while
 * group C and D shouldn't create any cluster since they do not contain enough objects.
 * \req
 * NA
 */
TEST(f360_cluster_objects_for_lsc_Cluster_Objects_For_LSC, Cluster_Objects_For_LSC_High_Level_2_Clusters_Expected)
{
   /** \action
    * Call function
    */
   Cluster_Objects_For_LSC(calibs, tracker_info, host, nr_next_ids_of_interest, next_ids_of_interest, objects, nr_valid_clusters, valid_clusters);

   /** \result
    * Verify group A and B have been clustered. Group A is expected to be the first cluster since
    * this group have its first object at smaller longitudinal position than group B.
    */
   CHECK_EQUAL(2U, nr_valid_clusters);

   CHECK_EQUAL(group_A.nr_objects, valid_clusters[0].nr_objects);
   F360_Object_Track_T* curr_obj = valid_clusters[0].first_object;
   for (uint32_t i = 0U; i < valid_clusters[0].nr_objects; i++)
   {
      CHECK_EQUAL(group_A.ids[i], curr_obj->id);
      curr_obj = curr_obj->lsc_next_in_cluster;
   }

   CHECK_EQUAL(group_B.nr_objects, valid_clusters[1].nr_objects);
   curr_obj = valid_clusters[1].first_object;
   for (uint32_t i = 0U; i < valid_clusters[1].nr_objects; i++)
   {
      CHECK_EQUAL(group_B.ids[i], curr_obj->id);
      curr_obj = curr_obj->lsc_next_in_cluster;
   }
}

/** @}*/

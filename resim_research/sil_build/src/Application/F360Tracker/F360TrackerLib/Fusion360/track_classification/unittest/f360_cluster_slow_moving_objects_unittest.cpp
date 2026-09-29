/** \file
 * This file contains unit tests for content of f360_cluster_slow_moving_objects.cpp file
 */

#include "f360_cluster_slow_moving_objects.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_cluster_slow_moving_objects
 *  @{
 */

/** \brief
 * This test group is used to set up test cases for clustering of slow moving objects through the function
 * Cluster_Slow_Moving_Objects and sub functions.
 */
TEST_GROUP(f360_cluster_slow_moving_objects)
{	
   F360_Host_T host = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};

   /** \setup
    * The default test case consists of 6 objects set up to the left of host,
    * all slow moving, some outside the relevant zone close to host and some inside it
    * that are close enough to form clusters. This default set up is modified in later 
    * tests to create more scenarios.
    */
   TEST_SETUP()
   {
      host.speed = 1.0F;

      tracker_info.num_active_objs = 6;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
      tracker_info.active_obj_ids[2] = 3;
      tracker_info.active_obj_ids[3] = 4;
      tracker_info.active_obj_ids[4] = 5;
      tracker_info.active_obj_ids[5] = 6;

      object_tracks[0].id = 1;
      object_tracks[0].vcs_position.x = -70.1F;
      object_tracks[0].vcs_position.y = -2.0F;
      object_tracks[0].speed = 0.3F;
      object_tracks[0].slow_moving_cluster_id = 100U;

      object_tracks[1].id = 2;
      object_tracks[1].vcs_position.x = -4.0F;
      object_tracks[1].vcs_position.y = -2.0F;
      object_tracks[1].speed = 0.5F;
      object_tracks[1].slow_moving_cluster_id = 100U;

      object_tracks[2].id = 3;
      object_tracks[2].vcs_position.x = -3.5F;
      object_tracks[2].vcs_position.y = -3.0F;
      object_tracks[2].speed = 0.25F;
      object_tracks[2].slow_moving_cluster_id = 100U;

      object_tracks[3].id = 4;
      object_tracks[3].vcs_position.x = -3.0F;
      object_tracks[3].vcs_position.y = -2.0F;
      object_tracks[3].speed = 0.99F;
      object_tracks[3].slow_moving_cluster_id = 100U;

      object_tracks[4].id = 5;
      object_tracks[4].vcs_position.x = -1.1F;
      object_tracks[4].vcs_position.y = -2.0F;
      object_tracks[4].speed = 0.7F;
      object_tracks[4].slow_moving_cluster_id = 100U;

      object_tracks[5].id = 6;
      object_tracks[5].vcs_position.x = 70.1F;
      object_tracks[5].vcs_position.y = -2.0F;
      object_tracks[5].speed = 0.21F;
      object_tracks[5].slow_moving_cluster_id = 100U;

      tracker_info.vcslong_sorted_start = &(object_tracks[0]);
      tracker_info.vcslong_sorted_prev_track[0] = NULL;
      tracker_info.vcslong_sorted_next_track[0] = &(object_tracks[1]);

      tracker_info.vcslong_sorted_prev_track[1] = &(object_tracks[0]);
      tracker_info.vcslong_sorted_next_track[1] = &(object_tracks[2]);

      tracker_info.vcslong_sorted_prev_track[2] = &(object_tracks[1]);
      tracker_info.vcslong_sorted_next_track[2] = &(object_tracks[3]);

      tracker_info.vcslong_sorted_prev_track[3] = &(object_tracks[2]);
      tracker_info.vcslong_sorted_next_track[3] = &(object_tracks[4]);

      tracker_info.vcslong_sorted_prev_track[4] = &(object_tracks[3]);
      tracker_info.vcslong_sorted_next_track[4] = &(object_tracks[5]);

      tracker_info.vcslong_sorted_prev_track[5] = &(object_tracks[4]);
      tracker_info.vcslong_sorted_next_track[5] = NULL;
   }
};

/** \purpose  
 * Verify that the first object, too far behind host, and the last object, too far ahead
 * of host are not clustered while the rest of the object, that are close enough to each other,
 * are all assigned the same cluster id.
 * \req
 * NA.
 */
TEST(f360_cluster_slow_moving_objects, Cluster_Slow_Moving_Objects_One_Cluster)
{
   /** \precond
    * From the TEST_GROUP a default scenario is set up such that
    * - object 1 is too far behind host to be considered for clustering
    * - object 6 is too far ahead of host to be considered for clustering
    * - objects 2-5 are close enough to host and each other to be able to cluster
    * - all object speeds are within slow moving thresholds
    * - host speed is below max threshold
    * - all objects are set up with cluster ID 100 by default, which should be re-assigned or reset
    */

   /** \action
    * Call Cluster_Slow_Moving_Objects().
    */
   Cluster_Slow_Moving_Objects(tracker_info, host, object_tracks);
   
   /** \result
    * Verify that the correct cluster IDs are assigned to all objects
    */
   CHECK_EQUAL_TEXT(0U, object_tracks[0].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(1U, object_tracks[1].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(1U, object_tracks[2].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(1U, object_tracks[3].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(1U, object_tracks[4].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(0U, object_tracks[5].slow_moving_cluster_id, "Incorrect cluster id");	
}

/** \purpose  
 * Verify that no clusters are formed when the objects in the middle that would connect the cluster
 * have a speed above the slow moving threshold, making them invalid for clustering.
 * \req
 * NA.
 */
TEST(f360_cluster_slow_moving_objects, Cluster_Slow_Moving_Objects_No_Clusters_Speed_Too_High)
{
   /** \precond
    * From the TEST_GROUP a default scenario is set up such that
    * - object 1 is too far behind host to be considered for clustering
    * - object 6 is too far ahead of host to be considered for clustering
    * - objects 2-5 are close enough to host and each other to be able to cluster
    * - speeds of objects 3 and 4 is set above the slow moving threshold
    * - host speed is below max threshold
    * - all objects are set up with cluster ID 100 by default, which should be re-assigned or reset
    */
   object_tracks[2].speed = 10.0F;
   object_tracks[3].speed = 10.0F;

   /** \action
    * Call Cluster_Slow_Moving_Objects().
    */
   Cluster_Slow_Moving_Objects(tracker_info, host, object_tracks);
   
   /** \result
    * Verify that no clusters are formed, i.e. all objects have cluster ID 0.
    */
   CHECK_EQUAL_TEXT(0U, object_tracks[0].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(0U, object_tracks[1].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(0U, object_tracks[2].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(0U, object_tracks[3].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(0U, object_tracks[4].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(0U, object_tracks[5].slow_moving_cluster_id, "Incorrect cluster id");	
}

/** \purpose  
 * Verify that no clusters are formed when host speed is above maximum threshold.
 * \req
 * NA.
 */
TEST(f360_cluster_slow_moving_objects, Cluster_Slow_Moving_Objects_No_Clusters_Host_Speed_Too_High)
{
   /** \precond
    * From the TEST_GROUP a default scenario is set up such that
    * - object 1 is too far behind host to be considered for clustering
    * - object 6 is too far ahead of host to be considered for clustering
    * - objects 2-5 are close enough to host and each other to be able to cluster
    * - host speed is above max threshold
    * - all objects are set up with cluster ID 100 by default, which should be re-assigned or reset
    */
   host.speed = 20.1F;

   /** \action
    * Call Cluster_Slow_Moving_Objects().
    */
   Cluster_Slow_Moving_Objects(tracker_info, host, object_tracks);
   
   /** \result
    * Verify that no clusters are formed, i.e. all objects have cluster ID 0.
    */
   CHECK_EQUAL_TEXT(0U, object_tracks[0].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(0U, object_tracks[1].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(0U, object_tracks[2].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(0U, object_tracks[3].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(0U, object_tracks[4].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(0U, object_tracks[5].slow_moving_cluster_id, "Incorrect cluster id");	
}

/** \purpose  
 * Verify that two different clusters are formed when all objects are to the left of host but
 * separated into two groups laterally.
 * \req
 * NA.
 */
TEST(f360_cluster_slow_moving_objects, Cluster_Slow_Moving_Objects_Two_Clusters_Left_Of_Host)
{
   /** \precond
    * From the TEST_GROUP a default scenario is set up such that
    * - object 6 is too far ahead of host to be considered for clustering
    * - objects 1-5 are close enough to host and each other to be able to cluster
    * - objects 1 and 3 are shifted to the left to be able to form their own cluster
    * - host speed is below max threshold
    * - all objects are set up with cluster ID 100 by default, which should be re-assigned or reset
    */
   object_tracks[0].vcs_position.x = -4.1F;
   object_tracks[0].vcs_position.y = -5.0F;

   object_tracks[2].vcs_position.y = -4.1F;

   object_tracks[4].vcs_position.x = -1.9F;
   /** \action
    * Call Cluster_Slow_Moving_Objects().
    */
   Cluster_Slow_Moving_Objects(tracker_info, host, object_tracks);
   
   /** \result
    * Verify the 
    */
   CHECK_EQUAL_TEXT(1U, object_tracks[0].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(2U, object_tracks[1].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(1U, object_tracks[2].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(2U, object_tracks[3].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(2U, object_tracks[4].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(0U, object_tracks[5].slow_moving_cluster_id, "Incorrect cluster id");	
}

/** \purpose  
 * Verify that two different clusters are formed, on on each side of host, when there are objects
 * close enough to each other on either side of host.
 * \req
 * NA.
 */
TEST(f360_cluster_slow_moving_objects, Cluster_Slow_Moving_Objects_Two_Clusters_Opposite_Sides_Of_Host)
{
   /** \precond
    * From the TEST_GROUP a default scenario is set up such that
    * - object 6 is too far ahead of host to be considered for clustering
    * - objects 1-5 are close enough to host and each other to be able to cluster
    * - objects 1 and 3 are shifted to the right side of host
    * - host speed is below max threshold
    * - all objects are set up with cluster ID 100 by default, which should be re-assigned or reset
    */
   object_tracks[0].vcs_position.x = -4.1F;
   object_tracks[0].vcs_position.y = 5.0F;

   object_tracks[2].vcs_position.y = 5.0F;

   object_tracks[4].vcs_position.x = -1.9F;
   /** \action
    * Call Cluster_Slow_Moving_Objects().
    */
   Cluster_Slow_Moving_Objects(tracker_info, host, object_tracks);
   
   /** \result
    * Verify the 
    */
   CHECK_EQUAL_TEXT(1U, object_tracks[0].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(2U, object_tracks[1].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(1U, object_tracks[2].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(2U, object_tracks[3].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(2U, object_tracks[4].slow_moving_cluster_id, "Incorrect cluster id");
   CHECK_EQUAL_TEXT(0U, object_tracks[5].slow_moving_cluster_id, "Incorrect cluster id");
}

/** \purpose  
 * Verify that no clusters are formed when there's only one object present.
 * \req
 * NA.
 */
TEST(f360_cluster_slow_moving_objects, Cluster_Slow_Moving_Objects_One_Object)
{
   /** \precond
    * - tracker info is reset
    * - set number of objects to 1
    * - arrange sorted list of objects by vcs position accordingly
    * - set object position close to host
    */
   tracker_info = {};
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.vcslong_sorted_start = &(object_tracks[0]);
   tracker_info.vcslong_sorted_prev_track[0] = NULL;
   tracker_info.vcslong_sorted_next_track[0] = NULL;

   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -2.0F;

   /** \action
    * Call Cluster_Slow_Moving_Objects().
    */
   Cluster_Slow_Moving_Objects(tracker_info, host, object_tracks);
   
   /** \result
    * Verify that the correct cluster IDs are assigned to all objects
    */
   CHECK_EQUAL_TEXT(0U, object_tracks[0].slow_moving_cluster_id, "Incorrect cluster id");
}

/** \purpose  
 * Verify that all objects are assigned the correct number of objects in their respecive
 * clusters when there's a single cluster containing all objects.
 * \req
 * NA.
 */
TEST(f360_cluster_slow_moving_objects, Count_Number_Of_Members_In_Clusters_1_Cluster)
{
   /** \precond
    * - Set the cluster ID of all objects to 5.
    */
   object_tracks[0].slow_moving_cluster_id = 5U;
   object_tracks[1].slow_moving_cluster_id = 5U;
   object_tracks[2].slow_moving_cluster_id = 5U;
   object_tracks[3].slow_moving_cluster_id = 5U;
   object_tracks[4].slow_moving_cluster_id = 5U;
   object_tracks[5].slow_moving_cluster_id = 5U;

   /** \action
    * Call Count_Number_Of_Members_In_Clusters_And_Find_Cluster_Length().
    */
   Count_Number_Of_Members_In_Clusters_And_Find_Cluster_Length(tracker_info, 5U, object_tracks);
   
   /** \result
    * Verify that the correct number of objects in cluster is assigned to all objects
    */
   CHECK_EQUAL_TEXT(6U, object_tracks[0].num_members_in_slow_moving_obj_cluster, "Incorrect number of objects in cluster");
   CHECK_EQUAL_TEXT(6U, object_tracks[1].num_members_in_slow_moving_obj_cluster, "Incorrect number of objects in cluster");
   CHECK_EQUAL_TEXT(6U, object_tracks[2].num_members_in_slow_moving_obj_cluster, "Incorrect number of objects in cluster");
   CHECK_EQUAL_TEXT(6U, object_tracks[3].num_members_in_slow_moving_obj_cluster, "Incorrect number of objects in cluster");
   CHECK_EQUAL_TEXT(6U, object_tracks[4].num_members_in_slow_moving_obj_cluster, "Incorrect number of objects in cluster");
   CHECK_EQUAL_TEXT(6U, object_tracks[5].num_members_in_slow_moving_obj_cluster, "Incorrect number of objects in cluster");
}

/** \purpose  
 * Verify that all objects are assigned the correct number of objects in their respecive
 * clusters when there are two clusters and one object not in any cluster.
 * \req
 * NA.
 */
TEST(f360_cluster_slow_moving_objects, Count_Number_Of_Members_In_Clusters_2_Clusters)
{
   /** \precond
    * - Set the cluster ID of 3 objects to 1.
    * - Set the cluster ID of 2 objects to 2.
    * - Set the cluster ID of 1 object to 0.
    */
   object_tracks[0].slow_moving_cluster_id = 1U;
   object_tracks[1].slow_moving_cluster_id = 2U;
   object_tracks[2].slow_moving_cluster_id = 1U;
   object_tracks[3].slow_moving_cluster_id = 2U;
   object_tracks[4].slow_moving_cluster_id = 1U;
   object_tracks[5].slow_moving_cluster_id = 0U;

   /** \action
    * Call Count_Number_Of_Members_In_Clusters_And_Find_Cluster_Length().
    */
   Count_Number_Of_Members_In_Clusters_And_Find_Cluster_Length(tracker_info, 2U, object_tracks);
   
   /** \result
    * Verify that the correct number of objects in cluster is assigned to all objects
    */
   CHECK_EQUAL_TEXT(3U, object_tracks[0].num_members_in_slow_moving_obj_cluster, "Incorrect number of objects in cluster");
   CHECK_EQUAL_TEXT(2U, object_tracks[1].num_members_in_slow_moving_obj_cluster, "Incorrect number of objects in cluster");
   CHECK_EQUAL_TEXT(3U, object_tracks[2].num_members_in_slow_moving_obj_cluster, "Incorrect number of objects in cluster");
   CHECK_EQUAL_TEXT(2U, object_tracks[3].num_members_in_slow_moving_obj_cluster, "Incorrect number of objects in cluster");
   CHECK_EQUAL_TEXT(3U, object_tracks[4].num_members_in_slow_moving_obj_cluster, "Incorrect number of objects in cluster");
   CHECK_EQUAL_TEXT(0U, object_tracks[5].num_members_in_slow_moving_obj_cluster, "Incorrect number of objects in cluster");
}
/** @}*/

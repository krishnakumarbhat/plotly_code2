/** \file
   Give a detailed description of what  this unit-test file contain.
*/

#include "f360_sorted_clusters_mgmt.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

//#include "headerfile_needed.h"

using namespace f360_variant_A;
//sneak in mocked functions
//Declaration of stubbed/mock functions

//Implementation of stubbed interfaces

/** \defgroup  f360_sorted_clusters_mgmt
 *  @{
 */

/** \brief
*  Group for testing function f360_sorted_clusters_mgmt
**/
TEST_GROUP(f360_sorted_clusters_mgmt)
{
   F360_Tracker_Info_T tracker_info = {};
   /** \setup
   * Nothing to setup in this test group
   **/
   TEST_SETUP()
   {
   }

   /** \teardown
   * Nothing to teardown in this test group
   **/
   TEST_TEARDOWN()
   {
      //mock.clear();
   }

};

/**
*\purpose  Test function will sorted the list and swap the second and third element,
           first and last element will keep unchanged.
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersReSortWith4ElementsNotSorted)
{
    /** \precond
    * Create clusters list with 4 element, not sorted clusters with repspect to longitudinal position.
    * But the 2nd and 3rd elment are swaped , then call function re sort to bring them back sorted.
    **/

    // put number of active objects to 4
    tracker_info.num_active_clusters = 4;

    // Initialize List with clusters with long position (3, 6, 5, 8) [m].
    F360_Cluster_T  clusters[4] = {};
    clusters[0].vcs_position_x = 3.0F;
    clusters[0].id = 1;
    clusters[1].vcs_position_x = 6.0F;
    clusters[1].id = 2;
    clusters[2].vcs_position_x = 5.0F;
    clusters[2].id = 3;
    clusters[3].vcs_position_x = 8.0F;
    clusters[3].id = 4;

    // Initialize vcslong_sorted_cluster_prev and vcslong_sorted_cluster_next arrays in tracker_info.
    tracker_info.vcslong_sorted_cluster_start = &clusters[0];
    tracker_info.vcslong_sorted_cluster_prev[0] = NULL;
    tracker_info.vcslong_sorted_cluster_next[0] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_prev[1] = &clusters[0];
    tracker_info.vcslong_sorted_cluster_next[1] = &clusters[2];
    tracker_info.vcslong_sorted_cluster_prev[2] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_next[2] = &clusters[3];
    tracker_info.vcslong_sorted_cluster_prev[3] = &clusters[2];
    tracker_info.vcslong_sorted_cluster_next[3] = NULL;

    /** \action
    * call Sorted_Clusters_Re_Sort()
    **/
    Sorted_Clusters_Re_Sort(tracker_info);

    /** \result
    * check the order of the elements in the vcslong_sorted_cluster_prev,
    * and vcslong_sorted_cluster_next.
    **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[1], &clusters[2]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[1], &clusters[3]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[2], &clusters[0]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[2], &clusters[1]);
}
/**
*\purpose  Test function will sort the element correctly, and assign the next of the second
*          element and prev of the first element to null.
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersReSort2Elements)
{
    /** \precond
    * Create clusters list with 2 element, unsorted clusters with respect to longitudinal position.
    **/

    // put number of active objects to 2
    tracker_info.num_active_clusters = 2;

    // Initialize List with clusters with long position (6, 5) [m].
    F360_Cluster_T  clusters[2] = {};
    clusters[0].vcs_position_x = 6.0F;
    clusters[0].id = 1;
    clusters[1].vcs_position_x = 5.0F;
    clusters[1].id = 2;

    // Initialize vcslong_sorted_cluster_prev and vcslong_sorted_cluster_next arrays in tracker_info.
    tracker_info.vcslong_sorted_cluster_start = &clusters[0];
    tracker_info.vcslong_sorted_cluster_prev[0] = NULL;
    tracker_info.vcslong_sorted_cluster_next[0] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_prev[1] = &clusters[0];
    tracker_info.vcslong_sorted_cluster_next[1] = NULL;

    /** \action
    * call Sorted_Clusters_Re_Sort()
    **/
    Sorted_Clusters_Re_Sort(tracker_info);

    /** \result
    * check the order of the elements in the vcslong_sorted_cluster_prev,
    * and vcslong_sorted_cluster_next.
    **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[0], &clusters[1]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[1], &clusters[0]);
}
/**
*\purpose  Test function will keep the sort as it is if the input is sorted but swapped.
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersReSort2ElementsSorted)
{
    /** \precond
    * Create clusters list with 2 element, sorted clusters with respect to longitudinal position . But
    * the are swapped in the order..
    **/

    // put number of active objects to 2
    tracker_info.num_active_clusters = 2;

    // Initialize List with clusters with long position (5, 6) [m].
    F360_Cluster_T  clusters[2] = {};
    clusters[0].vcs_position_x = 6.0F;
    clusters[0].id = 1;
    clusters[1].vcs_position_x = 5.0F;
    clusters[1].id = 2;

    // Initialize vcslong_sorted_cluster_prev and vcslong_sorted_cluster_next arrays in tracker_info.
    tracker_info.vcslong_sorted_cluster_start = &clusters[0];
    tracker_info.vcslong_sorted_cluster_prev[0] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_next[0] = NULL;
    tracker_info.vcslong_sorted_cluster_prev[1] = NULL;
    tracker_info.vcslong_sorted_cluster_next[1] = &clusters[0];

    /** \action
    * call Sorted_Tracks_Re_Sort()
    **/
    Sorted_Clusters_Re_Sort(tracker_info);

    /** \result
    * check the order of the elements in the vcslong_sorted_cluster_prev,
    * and vcslong_sorted_cluster_next.
    **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[0], &clusters[1]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[1], &clusters[0]);
}

/**
*\purpose  Test function With Empty list nothing should changed.
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersUpdateListEmptyList)
{
    /** \precond
    * Keep trackerinfo without initialization 
    **/

   /** \action
   * call Sorted_Clusters_Update_List()
   **/
   Sorted_Clusters_Update_List(tracker_info);
   
   /** \result
    * check the list is empty
    **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_start, NULL);
}

/**
*\purpose  Test function With not empty list.
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersUpdateListNotEmptyList)
{
    /** \precond
    * Keep tracker info without initialization
    **/
    F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
    clusters[0].vcs_position_x = 5.0F;
    clusters[0].id = 1;
    clusters[1].vcs_position_x = 6.0F;
    clusters[1].id = 2;
    clusters[2].vcs_position_x = 10.0F;
    clusters[2].id = 3;
    tracker_info.num_active_clusters = 3;

    tracker_info.vcslong_sorted_cluster_start = &clusters[0];
    tracker_info.vcslong_sorted_cluster_next[clusters[0].id - 1] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_next[clusters[1].id - 1] = &clusters[2];
    tracker_info.vcslong_sorted_cluster_prev[clusters[2].id - 1] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_prev[clusters[1].id - 1] = &clusters[0];

    /** \action
    * call Sorted_Clusters_Update_List()
    **/
    Sorted_Clusters_Update_List(tracker_info);

    /** \result
     * check start pointer points to fisrt element.
     **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_start, &clusters[0]);
}

/**
*\purpose  Test function for insert new cluser in empty cluster list.
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersInsert)
{
   /** \precond
   * Kepp tracker infor without empty i nitialization
   **/
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
   F360_Cluster_T new_cluster;
   int16_t initial_guess_id = 1;

   /** \action
   * call Sorted_Clusters_Insert()
   **/
   Sorted_Clusters_Insert(tracker_info, clusters, new_cluster, initial_guess_id);

   /** \result
    * check first element is the new cluster.
    **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_start, &new_cluster);
}

/**
*\purpose  Test function for insert new cluser in the beginning of list.
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersInsertInBeginning)
{
    /** \precond
    * Kepp tracker infor without empty i nitialization
    **/
    F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
    clusters[0].vcs_position_x = 10.0F;
    clusters[0].id = 1;

    tracker_info.vcslong_sorted_cluster_start = &clusters[0];

    F360_Cluster_T new_cluster;
    new_cluster.vcs_position_x = 5.0F;
    new_cluster.id = 2;
    int16_t initial_guess_id = 0;

    /** \action
    * call Sorted_Clusters_Insert()
    **/
    Sorted_Clusters_Insert(tracker_info, clusters, new_cluster, initial_guess_id);

    /** \result
     * check first element is the new cluster.
     **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_start, &new_cluster);
}

/**
*\purpose  Test function for insert new cluser in the end of list.
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersInsertInEnd)
{
    /** \precond
    * Kepp tracker infor without empty i nitialization
    **/
    F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
    clusters[0].vcs_position_x = 5.0F;
    clusters[0].id = 1;

    tracker_info.vcslong_sorted_cluster_start = &clusters[0];

    F360_Cluster_T new_cluster;
    new_cluster.vcs_position_x = 10.0F;
    new_cluster.id = 2;
    int16_t initial_guess_id = 0;

    /** \action
    * call Sorted_Clusters_Insert()
    **/
    Sorted_Clusters_Insert(tracker_info, clusters, new_cluster, initial_guess_id);

    /** \result
     * check the list is sorted.
     **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_start, &clusters[0]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[0], &new_cluster);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[1], &clusters[0]);
}

/**
*\purpose  Test function for insert new cluser in the middle of sorted list.
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersInsertInTheMiddleSortedList)
{
    /** \precond
    * Kepp tracker infor without empty i nitialization
    **/
    F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
    clusters[0].vcs_position_x = 5.0F;
    clusters[0].id = 1;
    clusters[1].vcs_position_x = 6.0F;
    clusters[1].id = 2;
    clusters[2].vcs_position_x = 10.0F;
    clusters[2].id = 3;

    tracker_info.vcslong_sorted_cluster_start = &clusters[0];
    tracker_info.vcslong_sorted_cluster_next[clusters[0].id - 1] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_next[clusters[1].id - 1] = &clusters[2];
    tracker_info.vcslong_sorted_cluster_prev[clusters[2].id - 1] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_prev[clusters[1].id - 1] = &clusters[0];

    F360_Cluster_T new_cluster;
    new_cluster.vcs_position_x = 7.0F;
    new_cluster.id = 4;
    int16_t initial_guess_id = 0;

    /** \action
    * call Sorted_Clusters_Insert()
    **/
    Sorted_Clusters_Insert(tracker_info, clusters, new_cluster, initial_guess_id);

    /** \result
     * check the list is sorted.
     **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_start, &clusters[0]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[clusters[0].id - 1], &clusters[1]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[clusters[1].id - 1], &new_cluster);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[new_cluster.id -1], &clusters[2]);

    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[clusters[2].id - 1], &new_cluster);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[new_cluster.id - 1], &clusters[1]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[clusters[1].id - 1], &clusters[0]);
}

/**
*\purpose  Test function for inserting new cluser with same x position
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersInsertEqualXPos)
{
    /** \precond
    * Initialize cluster and tracker_info with sorted list.
    * Initialize one new cluster with same x pos
    **/
    F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
    clusters[0].vcs_position_x = 5.0F;
    clusters[0].id = 1;
    clusters[1].vcs_position_x = 6.0F;
    clusters[1].id = 2;
    clusters[2].vcs_position_x = 10.0F;
    clusters[2].id = 3;

    tracker_info.vcslong_sorted_cluster_start = &clusters[0];
    tracker_info.vcslong_sorted_cluster_next[clusters[0].id - 1] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_next[clusters[1].id - 1] = &clusters[2];
    tracker_info.vcslong_sorted_cluster_prev[clusters[2].id - 1] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_prev[clusters[1].id - 1] = &clusters[0];

    F360_Cluster_T new_cluster;
    new_cluster.vcs_position_x = 6.0F;
    new_cluster.id = 4;
    int16_t initial_guess_id = 0;

    /** \action
    * call Sorted_Clusters_Insert()
    **/
    Sorted_Clusters_Insert(tracker_info, clusters, new_cluster, initial_guess_id);

    /** \result
     * check that the new cluster is inserted after the cluster with equal x pos.
     **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_start, &clusters[0]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[clusters[0].id - 1], &clusters[1]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[clusters[1].id - 1], &new_cluster);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[new_cluster.id - 1], &clusters[2]);

    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[clusters[2].id - 1], &new_cluster);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[new_cluster.id - 1], &clusters[1]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[clusters[1].id - 1], &clusters[0]);
}

/**
*\purpose  Test function for inserting new cluster with same x pos batch .
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersInsertEqualXPosBatch)
{
    /** \precond
    * initialize cluster and tracker_info with sorted list.
    * intialize 1 new cluster with same x pos.
    **/
    F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
    clusters[0].vcs_position_x = 5.0F;
    clusters[0].id = 1;
    clusters[1].vcs_position_x = 6.0F;
    clusters[1].id = 2;
    clusters[2].vcs_position_x = 10.0F;
    clusters[2].id = 3;
    clusters[3].vcs_position_x = 6.0F;
    clusters[3].id = 4;

    tracker_info.vcslong_sorted_cluster_start = &clusters[0];
    tracker_info.vcslong_sorted_cluster_next[clusters[0].id - 1] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_next[clusters[1].id - 1] = &clusters[2];
    tracker_info.vcslong_sorted_cluster_prev[clusters[2].id - 1] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_prev[clusters[1].id - 1] = &clusters[0];

    float32_t vcs_long[MAX_TRACKER_POSN_CLUSTERS] = { 6.0F};
    int16_t new_cluster_ids[MAX_TRACKER_POSN_CLUSTERS] = {4};
    uint16_t num_clusters = 1U;

    /** \action
    * call Sorted_Clusters_Insert_Batch()
    **/
    Sorted_Clusters_Insert_Batch(tracker_info, clusters, vcs_long, new_cluster_ids, num_clusters);

    /** \result
     * check that the new cluster is inserted after the cluster with equal x pos.
     **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_start, &clusters[0]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[clusters[0].id - 1], &clusters[1]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[clusters[1].id - 1], &clusters[3]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[clusters[3].id - 1], &clusters[2]);

    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[clusters[2].id - 1], &clusters[3]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[clusters[3].id - 1], &clusters[1]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[clusters[1].id - 1], &clusters[0]);
}

/**
*\purpose  Test function for insert new unsorted cluser batch .
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersInsertUnsortedBatch)
{
    /** \precond
    * initialize cluster and tracker_info with sorted list.
    * intialize 2 new cluster list with unsorted longitudinal value.
    **/
    F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
    clusters[0].vcs_position_x = 5.0F;
    clusters[0].id = 1;
    clusters[1].vcs_position_x = 6.0F;
    clusters[1].id = 2;
    clusters[2].vcs_position_x = 10.0F;
    clusters[2].id = 3;
    clusters[3].vcs_position_x = 15.0F;
    clusters[3].id = 4;

    tracker_info.vcslong_sorted_cluster_start = &clusters[0];
    tracker_info.vcslong_sorted_cluster_next[clusters[0].id - 1] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_prev[clusters[1].id - 1] = &clusters[0];

    float32_t vcs_long[MAX_TRACKER_POSN_CLUSTERS] = { 15, 10};
    int16_t new_cluster_ids[MAX_TRACKER_POSN_CLUSTERS] = {4, 3};
    uint16_t num_clusters = 2U;

    /** \action
    * call Sorted_Clusters_Insert_Batch()
    **/
    Sorted_Clusters_Insert_Batch(tracker_info, clusters, vcs_long, new_cluster_ids, num_clusters);

    /** \result
     * check the list is sorted.
     **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_start, &clusters[0]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[0], &clusters[1]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[1], &clusters[2]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[2], &clusters[3]);
}

/**
*\purpose  Test function for insert new unsorted cluser batch .
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersInsertSortedBatch)
{
    /** \precond
    * initialize cluster and tracker_info with sorted list.
    * intialize 2 new cluster list with unsorted longitudinal value.
    **/
    F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
    clusters[0].vcs_position_x = 5.0F;
    clusters[0].id = 1;
    clusters[1].vcs_position_x = 6.0F;
    clusters[1].id = 2;
    clusters[2].vcs_position_x = 10.0F;
    clusters[2].id = 3;
    clusters[3].vcs_position_x = 15.0F;
    clusters[3].id = 4;

    tracker_info.vcslong_sorted_cluster_start = &clusters[0];
    tracker_info.vcslong_sorted_cluster_next[clusters[0].id - 1] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_prev[clusters[1].id - 1] = &clusters[0];

    float32_t vcs_long[MAX_TRACKER_POSN_CLUSTERS] = { 10.0F, 15.0F };
    int16_t new_cluster_ids[MAX_TRACKER_POSN_CLUSTERS] = { 3, 4 };
    uint16_t num_clusters = 2U;

    /** \action
    * call Sorted_Clusters_Insert_Batch()
    **/
    Sorted_Clusters_Insert_Batch(tracker_info, clusters, vcs_long, new_cluster_ids, num_clusters);

    /** \result
     * check the list is sorted.
     **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_start, &clusters[0]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[0], &clusters[1]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[1], &clusters[2]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[2], &clusters[3]);
}

/**
*\purpose  Test function for insert new unsorted cluser batch .
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersInsertSortedBatchinEmptyList)
{
    /** \precond
    * initialize cluster and tracker_info with sorted list.
    * intialize 2 new cluster list with unsorted longitudinal value.
    **/
    F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
    clusters[0].vcs_position_x = 5.0F;
    clusters[0].id = 1;
    clusters[1].vcs_position_x = 6.0F;
    clusters[1].id = 2;

    float32_t vcs_long[MAX_TRACKER_POSN_CLUSTERS] = { 5.0F, 6.0F };
    int16_t new_cluster_ids[MAX_TRACKER_POSN_CLUSTERS] = { 1, 2 };
    uint16_t num_clusters = 2U;

    /** \action
    * call Sorted_Clusters_Insert_Batch()
    **/
    Sorted_Clusters_Insert_Batch(tracker_info, clusters, vcs_long, new_cluster_ids, num_clusters);

    /** \result
     * check the list is sorted.
     **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_start, &clusters[0]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[0], &clusters[1]);
}

/**
*\purpose  Test function for insert new unsorted cluser batch .
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersInsertSortedBatchinMiddleList)
{
    /** \precond
    * initialize cluster and tracker_info with sorted list.
    * intialize 1 new cluster list.
    **/
    F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
    clusters[0].vcs_position_x = 5.0F;
    clusters[0].id = 1;
    clusters[1].vcs_position_x = 13.0F;
    clusters[1].id = 2;
    clusters[2].vcs_position_x = 10.0F;
    clusters[2].id = 3;

    tracker_info.vcslong_sorted_cluster_start = &clusters[0];
    tracker_info.vcslong_sorted_cluster_next[clusters[0].id - 1] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_prev[clusters[1].id - 1] = &clusters[0];

    float32_t vcs_long[MAX_TRACKER_POSN_CLUSTERS] = { 10.0F };
    int16_t new_cluster_ids[MAX_TRACKER_POSN_CLUSTERS] = { 3 };
    uint16_t num_clusters = 1U;

    /** \action
    * call Sorted_Clusters_Insert_Batch()
    **/
    Sorted_Clusters_Insert_Batch(tracker_info, clusters, vcs_long, new_cluster_ids, num_clusters);

    /** \result
     * check the list is sorted.
     **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_start, &clusters[0]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[0], &clusters[2]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[2], &clusters[1]);
}


/**
*\purpose  Test function for removing cluster from sorted list.
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersRemoveFirstCluster)
{
    /** \precond
    * initialize cluster and tracker_info with sorted list.
    **/
    F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
    clusters[0].vcs_position_x = 5.0F;
    clusters[0].id = 1;
    clusters[1].vcs_position_x = 13.0F;
    clusters[1].id = 2;

    tracker_info.vcslong_sorted_cluster_start = &clusters[0];
    tracker_info.vcslong_sorted_cluster_next[clusters[0].id - 1] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_prev[clusters[1].id - 1] = &clusters[0];

    /** \action
    * call Sorted_Clusters_Remove()
    **/
    Sorted_Clusters_Remove(tracker_info, clusters[0]);

    /** \result
     * Check that the cluster was removed from the sorted list.
     **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_start, &clusters[1]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[1], NULL);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[1], NULL);
}

/**
*\purpose  Test function for removing cluster from sorted list.
*\req      NA
*/
TEST(f360_sorted_clusters_mgmt, TestSortedClustersRemoveLastCluster)
{
    /** \precond
    * initialize cluster and tracker_info with sorted list.
    **/
    F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
    clusters[0].vcs_position_x = 5.0F;
    clusters[0].id = 1;
    clusters[1].vcs_position_x = 13.0F;
    clusters[1].id = 2;

    tracker_info.vcslong_sorted_cluster_start = &clusters[0];
    tracker_info.vcslong_sorted_cluster_next[clusters[0].id - 1] = &clusters[1];
    tracker_info.vcslong_sorted_cluster_prev[clusters[1].id - 1] = &clusters[0];

    /** \action
    * call Sorted_Clusters_Remove()
    **/
    Sorted_Clusters_Remove(tracker_info, clusters[1]);

    /** \result
     * Check that the cluster was removed from the sorted list.
     **/
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_start, &clusters[0]);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_next[0], NULL);
    POINTERS_EQUAL(tracker_info.vcslong_sorted_cluster_prev[0], NULL);
}
/** @}*/

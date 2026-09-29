/** \file
   File contains test cases for Terminate_Clusters() function
*/

#include "f360_terminate_clusters.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

//#include "headerfile_needed.h"

//sneak in mocked functions
//Declaration of stubbed/mock functions

//Implementation of stubbed interfaces

/** \defgroup  f360_terminate_clusters
 *  @{
 */
using namespace f360_variant_A;
/** \brief
 *  Group of test for Terminate_Clusters function
 */

TEST_GROUP(f360_terminate_clusters)
{
   /** \setup
   * Seting up default tolerance, clusters, historical detections, tracker info
   */
   float32_t tolerance = 10e-6F;
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS];
   F360_Detection_Hist_T det_hist;
   F360_Tracker_Info_T tracker_info;

   TEST_SETUP()
   {
      for (int32_t cluester_idx = 0; cluester_idx < NUMBER_OF_CLUSTERS; cluester_idx++)
      {
         clusters[cluester_idx] = {};
      }
      det_hist = {};
      tracker_info = {};
      Set_Tracker_Variant(tracker_info.variant);
   }

   /** \teardown
    * Nothing to teardown in this test group
    */
   TEST_TEARDOWN()
   {
      //mock.clear();
   }

};

/**
*\purpose  Checkig if functions Terminate_Clusters clears clusters with f_to_be_killed flag set to true and remain these which have this flag set to false
*\req    NA
*/
TEST(f360_terminate_clusters, test_all_clusters_should_be_killed)
{
   /** \precond
    * Setting up some values for clusters and make proper clusters active in tracker info
    */
   tracker_info.num_active_clusters = 3;
   tracker_info.active_cluster_ids[0] = 1;
   tracker_info.active_cluster_ids[1] = 3;
   tracker_info.active_cluster_ids[2] = 5;

   clusters[0].f_to_be_killed = true;
   clusters[0].id = 1;
   clusters[0].vcs_position_x = 11.0F;
   clusters[0].vcs_position_y = 12.0F;

   clusters[2].f_to_be_killed = true;
   clusters[2].id = 3;
   clusters[2].vcs_position_x = 5.2F;
   clusters[2].vcs_position_y = 2.3F;

   clusters[4].f_to_be_killed = true;
   clusters[4].id = 5;
   clusters[4].vcs_position_x = 11.0F;
   clusters[4].vcs_position_y = 12.0F;

   /** \action
    * Calling Terminate_Clusters function
    */
   Terminate_Clusters(clusters, det_hist, tracker_info);

   /** \result
    * Checking if function Terminate_Clusters clears proper clusters
    */
   DOUBLES_EQUAL(0.0F, clusters[0].vcs_position_x, tolerance);
   DOUBLES_EQUAL(0.0F, clusters[0].vcs_position_y, tolerance);
   CHECK_FALSE(clusters[0].f_to_be_killed);
   CHECK_EQUAL(1, clusters[0].id);

   DOUBLES_EQUAL(0.0F, clusters[2].vcs_position_x, tolerance);
   DOUBLES_EQUAL(0.0F, clusters[2].vcs_position_y, tolerance);
   CHECK_FALSE(clusters[2].f_to_be_killed);
   CHECK_EQUAL(3, clusters[2].id); 

   DOUBLES_EQUAL(0.0F, clusters[4].vcs_position_x, tolerance);
   DOUBLES_EQUAL(0.0F, clusters[4].vcs_position_y, tolerance);
   CHECK_FALSE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(5, clusters[4].id);
}

/**
*\purpose  Checkig if functions Terminate_Clusters clears clusters with f_to_be_killed flag set to true and remain these which have this flag set to false
*\req    NA
*/
TEST(f360_terminate_clusters, test_all_clusters_should_not_be_killed)
{
   /** \precond
    * Setting up some values for clusters and make proper clusters active in tracker info
    */
   tracker_info.num_active_clusters = 3;
   tracker_info.active_cluster_ids[0] = 2;
   tracker_info.active_cluster_ids[1] = 4;
   tracker_info.active_cluster_ids[2] = 6;

   clusters[1].f_to_be_killed = false;
   clusters[1].id = 2;
   clusters[1].vcs_position_x = 11.0F;
   clusters[1].vcs_position_y = 12.0F;

   clusters[3].f_to_be_killed = false;
   clusters[3].id = 4;
   clusters[3].vcs_position_x = 11.0F;
   clusters[3].vcs_position_y = 12.0F;

   clusters[5].f_to_be_killed = false;
   clusters[5].id = 6;
   clusters[5].vcs_position_x = 5.5F;
   clusters[5].vcs_position_y = 4.4F;

   /** \action
    * Calling Terminate_Clusters function
    */
   Terminate_Clusters(clusters, det_hist, tracker_info);

   /** \result
    * Checking if function Terminate_Clusters clears proper clusters
    */
   DOUBLES_EQUAL(11.0F, clusters[1].vcs_position_x, tolerance);
   DOUBLES_EQUAL(12.0F, clusters[1].vcs_position_y, tolerance);
   CHECK_FALSE(clusters[1].f_to_be_killed);
   CHECK_EQUAL(2, clusters[1].id);

   DOUBLES_EQUAL(11.0F, clusters[3].vcs_position_x, tolerance);
   DOUBLES_EQUAL(12.0F, clusters[3].vcs_position_y, tolerance);
   CHECK_FALSE(clusters[3].f_to_be_killed);
   CHECK_EQUAL(4, clusters[3].id);

   DOUBLES_EQUAL(5.5F, clusters[5].vcs_position_x, tolerance);
   DOUBLES_EQUAL(4.4F, clusters[5].vcs_position_y, tolerance);
   CHECK_FALSE(clusters[5].f_to_be_killed);
   CHECK_EQUAL(6, clusters[5].id);
}

/**
*\purpose  Checkig if functions Terminate_Clusters clears clusters with f_to_be_killed flag set to true and remain these which have this flag set to false
*\req    NA
*/
TEST(f360_terminate_clusters, test_mixed_clusters_to_be_killed_and_not_to_be_killed)
{
   /** \precond
    * Setting up some values for clusters and make proper clusters active in tracker info
    */
   tracker_info.num_active_clusters = 5;
   tracker_info.active_cluster_ids[0] = 2;
   tracker_info.active_cluster_ids[1] = 4;
   tracker_info.active_cluster_ids[2] = 6;
   tracker_info.active_cluster_ids[3] = 7;
   tracker_info.active_cluster_ids[4] = 10;

   clusters[1].f_to_be_killed = false;
   clusters[1].id = 2;
   clusters[1].vcs_position_x = 11.4F;
   clusters[1].vcs_position_y = 12.4F;

   clusters[3].f_to_be_killed = true;
   clusters[3].id = 4;
   clusters[3].vcs_position_x = 11.0F;
   clusters[3].vcs_position_y = 12.0F;

   clusters[5].f_to_be_killed = false;
   clusters[5].id = 6;
   clusters[5].vcs_position_x = 5.5F;
   clusters[5].vcs_position_y = 4.4F;

   clusters[6].f_to_be_killed = true;
   clusters[6].id = 7;
   clusters[6].vcs_position_x = 23.3F;
   clusters[6].vcs_position_y = -45.8F;

   clusters[9].f_to_be_killed = true;
   clusters[9].id = 10;
   clusters[9].vcs_position_x = 9.9F;
   clusters[9].vcs_position_y = 9.9F;

   /** \action
    * Calling Terminate_Clusters function
    */
   Terminate_Clusters(clusters, det_hist, tracker_info);

   /** \result
    * Checking if function Terminate_Clusters clears proper clusters
    */
   DOUBLES_EQUAL(11.4F, clusters[1].vcs_position_x, tolerance);
   DOUBLES_EQUAL(12.4F, clusters[1].vcs_position_y, tolerance);
   CHECK_FALSE(clusters[1].f_to_be_killed);
   CHECK_EQUAL(2, clusters[1].id);

   DOUBLES_EQUAL(0.0F, clusters[3].vcs_position_x, tolerance);
   DOUBLES_EQUAL(0.0F, clusters[3].vcs_position_y, tolerance);
   CHECK_FALSE(clusters[3].f_to_be_killed);
   CHECK_EQUAL(4, clusters[3].id);

   DOUBLES_EQUAL(5.5F, clusters[5].vcs_position_x, tolerance);
   DOUBLES_EQUAL(4.4F, clusters[5].vcs_position_y, tolerance);
   CHECK_FALSE(clusters[5].f_to_be_killed);
   CHECK_EQUAL(6, clusters[5].id);

   DOUBLES_EQUAL(0.0F, clusters[6].vcs_position_x, tolerance);
   DOUBLES_EQUAL(0.0F, clusters[6].vcs_position_y, tolerance);
   CHECK_FALSE(clusters[5].f_to_be_killed);
   CHECK_EQUAL(7, clusters[6].id);
   
   DOUBLES_EQUAL(0.0F, clusters[9].vcs_position_x, tolerance);
   DOUBLES_EQUAL(0.0F, clusters[9].vcs_position_y, tolerance);
   CHECK_FALSE(clusters[9].f_to_be_killed);
   CHECK_EQUAL(10, clusters[9].id);
}
/** @}*/


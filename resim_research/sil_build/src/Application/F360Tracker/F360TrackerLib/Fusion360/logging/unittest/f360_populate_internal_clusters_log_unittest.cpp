/** \file
 * This file contains unit tests for content of f360_populate_internal_clusters_log.cpp file
 */

#include "f360_populate_internal_clusters_log.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_populate_internal_clusters_log
 *  @{
 */

/** \brief
 * Test suit for functions in f360_populate_internal_clusters_log.cpp
 * Verify those functions return the value as expected.
 */
TEST_GROUP(f360_populate_internal_clusters_log)
{	
   // Declare and initialize common variables used within all tests in this test group.
   F360_Internal_Cluster_T cluster_log[NUMBER_OF_CLUSTERS]{};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS]{};
   int16_t active_cluster_ids[NUMBER_OF_CLUSTERS]{};
   F360_Tracker_Info_T tracker_info{};
   int16_t num_active_clusters = 0;
   /** \setup
    * Set up the default values for the whole test group
    */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      num_active_clusters = 2;
      active_cluster_ids[0] = 5;
      active_cluster_ids[1] = 10;
      clusters[4].vcs_position_x = 10.0F;
      clusters[4].vcs_position_y = 2.0F;
      clusters[4].rep_vcs_az = 0.1974F;
      clusters[4].rep_rdotcomp = 10.0F;
      clusters[4].time_since_cluster_updated = 1.0F;
      clusters[4].time_since_measurement = 1.0F;
      clusters[4].id = 5;
      clusters[4].num_types_of_dets[0] = 2;
      clusters[4].num_types_of_dets[1] = 0;
      clusters[4].clutter_counter = 1;
      clusters[4].f_dealiased = true;
      clusters[4].f_to_be_killed = false;
      clusters[4].low_rcs_dets_cnt = 0;
      clusters[9].vcs_position_x = 10.0F;
      clusters[9].vcs_position_y = 2.0F;
      clusters[9].rep_vcs_az = 0.1974F;
      clusters[9].rep_rdotcomp = 10.0F;
      clusters[9].time_since_cluster_updated = 1.0F;
      clusters[9].time_since_measurement = 1.0F;
      clusters[9].id = 10;
      clusters[9].num_types_of_dets[0] = 2;
      clusters[9].num_types_of_dets[1] = 0;
      clusters[9].clutter_counter = 1;
      clusters[9].f_dealiased = true;
      clusters[9].f_to_be_killed = false;
      clusters[9].low_rcs_dets_cnt = 0;
   }
};

/** \purpose  
 * Verify Populate_Internal_Clusters_Log_Data returns the data as expected
 * \req NA
 */
TEST(f360_populate_internal_clusters_log, Test_Populate_Internal_Clusters_Log_Data)
{
   /** \precond
    * Use the test group default values
    */
	
   /** \action
    * Describe the action of the test. E.g. call Some_Function().
    */
   Populate_Internal_Clusters_Log_Data(cluster_log,clusters,num_active_clusters,active_cluster_ids);

   /** \result
    * check that the output match expected data.
    */
   CHECK_EQUAL_TEXT(2.0F, cluster_log[0].vcs_lat_posn, "Cluster log lateral position does not match the tolerent gate");
   CHECK_EQUAL_TEXT(10.0F, cluster_log[0].vcs_long_posn, "Cluster log longitudinal position does not match the tolerent gate");
   CHECK_EQUAL_TEXT(0.1974F, cluster_log[0].rep_vcs_az, "Cluster log vcs azimuth angle  does not match the tolerent gate");
   CHECK_EQUAL_TEXT(10.0F, cluster_log[0].rep_rdotcomp, "Cluster log range rate compensated does not match the tolerent gate");
   CHECK_EQUAL_TEXT(1.0F, cluster_log[0].time_since_cluster_updated,"Cluster log time since created position does not match the tolerent gate");
   CHECK_EQUAL_TEXT(1.0F, cluster_log[0].time_since_measurement, "Cluster log time since measurement does not match the tolerent gate");
   CHECK_EQUAL_TEXT(5, cluster_log[0].id,"Cluster log id does not match the tolerent gate");
   CHECK_EQUAL_TEXT(2, cluster_log[0].num_types_of_dets[0], "Cluster log number of moving detection does not match the tolerent gate");
   CHECK_EQUAL_TEXT(0, cluster_log[0].num_types_of_dets[1], "Cluster log number of stationary detection does not match the tolerent gate");
   CHECK_EQUAL_TEXT(1, cluster_log[0].clutter_counter, "Cluster clutter counter does not match the tolerent gate");
   CHECK_EQUAL_TEXT(1, cluster_log[0].f_dealiased, "Cluster log f_dealiased flag does not match the tolerent gate");
   CHECK_EQUAL_TEXT(0, cluster_log[0].f_to_be_killed, "Cluster log f_to_be_killed flag does not match the tolerent gate");
   CHECK_EQUAL_TEXT(0, cluster_log[0].low_rcs_dets_cnt, "Cluster log low rcs detection count does not match the tolerent gate");
   CHECK_EQUAL_TEXT(2.0F, cluster_log[1].vcs_lat_posn, "Cluster log lateral position does not match the tolerent gate");
   CHECK_EQUAL_TEXT(10.0F, cluster_log[1].vcs_long_posn, "Cluster log longitudinal position does not match the tolerent gate");
   CHECK_EQUAL_TEXT(0.1974F, cluster_log[1].rep_vcs_az, "Cluster log vcs azimuth angle  does not match the tolerent gate");
   CHECK_EQUAL_TEXT(10.0F, cluster_log[1].rep_rdotcomp, "Cluster log range rate compensated does not match the tolerent gate");
   CHECK_EQUAL_TEXT(1.0F, cluster_log[1].time_since_cluster_updated, "Cluster log time since created position does not match the tolerent gate");
   CHECK_EQUAL_TEXT(1.0F, cluster_log[1].time_since_measurement, "Cluster log time since measurement does not match the tolerent gate");
   CHECK_EQUAL_TEXT(10, cluster_log[1].id, "Cluster log id does not match the tolerent gate");
   CHECK_EQUAL_TEXT(2, cluster_log[1].num_types_of_dets[0], "Cluster log number of moving detection does not match the tolerent gate");
   CHECK_EQUAL_TEXT(0, cluster_log[1].num_types_of_dets[1], "Cluster log number of stationary detection does not match the tolerent gate");
   CHECK_EQUAL_TEXT(1, cluster_log[1].clutter_counter, "Cluster clutter counter does not match the tolerent gate");
   CHECK_EQUAL_TEXT(1, cluster_log[1].f_dealiased, "Cluster log f_dealiased flag does not match the tolerent gate");
   CHECK_EQUAL_TEXT(0, cluster_log[1].f_to_be_killed, "Cluster log f_to_be_killed flag does not match the tolerent gate");
   CHECK_EQUAL_TEXT(0, cluster_log[1].low_rcs_dets_cnt, "Cluster log low rcs detection count does not match the tolerent gate");

   clusters[9].f_to_be_killed = true;
   /** \action
    * Describe the action of the test. E.g. call Some_Function().
    */
   Populate_Internal_Clusters_Log_Data(cluster_log,clusters,num_active_clusters,active_cluster_ids);
   CHECK_EQUAL_TEXT(1U, cluster_log[1].f_to_be_killed, "Cluster log f_to_be_killed flag does not match the tolerent gate");

}

/** \purpose  
 * Verify Populate_Internal_Clusters_Data returns the data as expected
 * \req NA
 */
TEST(f360_populate_internal_clusters_log, Test_Populate_Internal_Clusters_Data)
{
   /** \precond
    * Add an extra cluster log to test when id is over the variant max allowed num_clusters.
    */
	tracker_info.variant.num_clusters = 450U;
   num_active_clusters = 3;
   active_cluster_ids[2] = 500;
   cluster_log[499].vcs_long_posn = 10.0F;
   cluster_log[499].vcs_lat_posn = 2.0F;
   cluster_log[499].rep_vcs_az = 0.1974F;
   cluster_log[499].rep_rdotcomp = 10.0F;
   cluster_log[499].time_since_cluster_updated = 1.0F;
   cluster_log[499].time_since_measurement = 1.0F;
   cluster_log[499].id = 500;
   cluster_log[499].num_types_of_dets[0] = 2;
   cluster_log[499].num_types_of_dets[1] = 0;
   cluster_log[499].clutter_counter = 1;
   cluster_log[499].f_dealiased = true;
   cluster_log[499].f_to_be_killed = false;
   cluster_log[499].low_rcs_dets_cnt = 0;

   /** \action
    * call Populate_Internal_Clusters_Log_Data() to generated cluster_log. Set one clusters[5] 
    * with time_since_created initial value -1.0F;
    */
   Populate_Internal_Clusters_Log_Data(cluster_log,clusters,num_active_clusters,active_cluster_ids);
   clusters[3].time_since_cluster_updated = -1.0F;
   cluster_log[4].time_since_cluster_updated = -1.0F;
   cluster_log[4].id = 15U;
   // Call Populate_Internal_Clusters_Data() to check if it return the values as expected
   Populate_Internal_Clusters_Data(clusters,tracker_info,cluster_log);

   /** \result
    * check that the output match expected data.
    */
   CHECK_EQUAL_TEXT(0, clusters[14].id, "Tracker info number of active clusters does not match expected")
   CHECK_EQUAL_TEXT(2, tracker_info.num_active_clusters, "Tracker info number of active clusters does not match expected")
   CHECK_EQUAL_TEXT(5, tracker_info.active_cluster_ids[0], "Tracker info active clusters id does not match expected")
   CHECK_EQUAL_TEXT(10, tracker_info.active_cluster_ids[1], "Tracker info active clusters id does not match expected")
   CHECK_EQUAL_TEXT(4, tracker_info.inactive_cluster_ids[0], "Tracker info active clusters id does not match expected")

}
/** @}*/

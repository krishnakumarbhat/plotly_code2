/** \file
   File contains tests for Coarse_Cluster_Gate function
*/

#include "f360_coarse_cluster_gate.h"
#include "f360_cluster_grouping_data_generator.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;
/** \defgroup  f360_coarse_cluster_gate
 *  @{
 */

/** \brief
*  Test group for Coarse_Cluster_Gate function
**/
TEST_GROUP(f360_coarse_cluster_gate)
{
   /** \setup
   * Setting up calubrations and clusters
   **/
   F360_Calibrations_T calib = {};
   F360_Cluster_T cluster_1 = {};
   F360_Cluster_T cluster_2 = {};

   bool f_success = false;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/**
*\purpose  Checking nominal case for Coarse_Cluster_Gate function
*\req    NA
*/
TEST(f360_coarse_cluster_gate, Test_Two_Close_Stat_Clusters)
{
   /** \precond
   * Filling up cluster with position and time since cluster update
   **/
   Fill_Cluster(0.0, 10.0, 0.0, 0.0, cluster_1);
   Fill_Cluster(0.0, 9.0, 0.0, 0.0, cluster_2);

   /** \action
   * Call Coarse_Cluster_Gate function
   **/
   f_success = Coarse_Cluster_Gate(calib, cluster_1, cluster_2);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  Checking nominal case for Coarse_Cluster_Gate function
*\req    NA
*/
TEST(f360_coarse_cluster_gate, Test_Two_Distant_Stat_Clusters)
{
   /** \precond
   * Filling up cluster with position and time since cluster update
   **/
   Fill_Cluster(0.0, 10.0, 0.0, 0.0, cluster_1);
   Fill_Cluster(0.0, 20.0, 0.0, 0.0, cluster_2);

   /** \action
   * Call Coarse_Cluster_Gate function
   **/
   f_success = Coarse_Cluster_Gate(calib, cluster_1, cluster_2);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  Checking nominal case for Coarse_Cluster_Gate function
*\req    NA
*/
TEST(f360_coarse_cluster_gate, Test_Two_Distant_Clusters_One_Moving_One_Stat_1)
{
   /** \precond
   * Filling up cluster with position and time since cluster update
   **/
   Fill_Cluster(0.0, 10.0, 10.0, 0.0, cluster_1);
   Fill_Cluster(0.0, 1.0, 0.0, 0.0, cluster_2);
   cluster_1.num_types_of_dets[0] = 1;

   /** \action
   * Call Coarse_Cluster_Gate function
   **/
   f_success = Coarse_Cluster_Gate(calib, cluster_1, cluster_2);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  Checking nominal case for Coarse_Cluster_Gate function
*\req    NA
*/
TEST(f360_coarse_cluster_gate, Test_Two_Distant_Clusters_One_Moving_One_Stat_2)
{
   /** \precond
   * Filling up cluster with position and time since cluster update
   **/
   Fill_Cluster(0.0, 10.0, 0.0, 0.0, cluster_1);
   Fill_Cluster(0.0, 1.0, 10.0, 0.0, cluster_2);
   cluster_2.num_types_of_dets[0] = 1;

   /** \action
   * Call Coarse_Cluster_Gate function
   **/
   f_success = Coarse_Cluster_Gate(calib, cluster_1, cluster_2);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  Checking nominal case for Coarse_Cluster_Gate function
*\req    NA
*/
TEST(f360_coarse_cluster_gate, Test_Two_Distant_Clusters_Both_Moving)
{
   /** \precond
   * Filling up cluster with position and time since cluster update
   **/
   Fill_Cluster(0.0, 12.0, 10.0, 0.0, cluster_1);
   Fill_Cluster(0.0, 2.0, 10.0, 0.0, cluster_2);
   cluster_1.num_types_of_dets[0] = 1;
   cluster_2.num_types_of_dets[0] = 1;

   /** \action
   * Call Coarse_Cluster_Gate function
   **/
   f_success = Coarse_Cluster_Gate(calib, cluster_1, cluster_2);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  Checking nominal case for Coarse_Cluster_Gate function
*\req    NA
*/
TEST(f360_coarse_cluster_gate, Test_Two_Clusters_Both_Moving_In_First_Gate_Dist)
{
   /** \precond
   * Filling up cluster with position and time since cluster update
   **/
   Fill_Cluster(0.0, 10.0, 10.0, 0.0, cluster_1);
   Fill_Cluster(0.0, 8.0, 10.0, 0.0, cluster_2);
   cluster_1.num_types_of_dets[0] = 1;
   cluster_2.num_types_of_dets[0] = 1;

   /** \action
   * Call Coarse_Cluster_Gate function
   **/
   f_success = Coarse_Cluster_Gate(calib, cluster_1, cluster_2);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  Checking if clusters within time gate are checked for closer gate
*\req    NA
*/
TEST(f360_coarse_cluster_gate, Test_Two_Clusters_Both_Moving_Outof_Second_Gate_Dist_Same_Time_Instance)
{
   /** \precond
   * Filling up cluster with position and time since cluster update
   **/
   Fill_Cluster(0.0, 12.0, 10.0, 0.0, cluster_1);
   Fill_Cluster(0.0, 2.0, 10.0, 0.0, cluster_2);
   cluster_1.num_types_of_dets[0] = 1;
   cluster_2.num_types_of_dets[0] = 1;

   /** \action
   * Call Coarse_Cluster_Gate function
   **/
   f_success = Coarse_Cluster_Gate(calib, cluster_1, cluster_2);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  Checking if clusters within time gate are checked for closer gate
*\req    NA
*/
TEST(f360_coarse_cluster_gate, Test_Two_Clusters_Both_Moving_Outof_Second_Gate_Dist_One_Time_Instance_Apart)
{
   /** \precond
   * Filling up cluster with position and time since cluster update
   **/
   Fill_Cluster(0.0, 12.0, 10.0, 0.0, cluster_1);
   Fill_Cluster(0.0, 2.0, 10.0, 0.0, cluster_2);
   cluster_1.num_types_of_dets[0] = 1;
   cluster_2.num_types_of_dets[0] = 1;
   cluster_2.time_since_measurement = 0.05;

   /** \action
   * Call Coarse_Cluster_Gate function
   **/
   f_success = Coarse_Cluster_Gate(calib, cluster_1, cluster_2);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  Checking if clusters within time gate are checked for closer gate
*\req    NA
*/
TEST(f360_coarse_cluster_gate, Test_Two_Clusters_Both_Moving_In_Second_Gate_Dist_Four_Time_Instance_Apart)
{
   /** \precond
   * Filling up cluster with position and time since cluster update
   **/
   Fill_Cluster(0.0, 12.0, 10.0, 0.0, cluster_1);
   Fill_Cluster(0.0, 2.0, 10.0, 0.0, cluster_2);
   cluster_1.num_types_of_dets[0] = 1;
   cluster_2.num_types_of_dets[0] = 1;
   cluster_2.time_since_measurement = 0.2;

   /** \action
   * Call Coarse_Cluster_Gate function
   **/
   f_success = Coarse_Cluster_Gate(calib, cluster_1, cluster_2);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  Checking clusters within time gate
*\req    NA
*/
TEST(f360_coarse_cluster_gate, Test_Two_Distant_Clusters_Both_Moving_Outof_Second_Dist_Gate)
{
   /** \precond
   * Filling up cluster with position and time since cluster update
   **/
   Fill_Cluster(0.0, 20.0, 10.0, 0.0, cluster_1);
   Fill_Cluster(0.0, 4.0, 10.0, 0.0, cluster_2);
   cluster_1.num_types_of_dets[0] = 1;
   cluster_2.num_types_of_dets[0] = 1;
   cluster_2.time_since_measurement = 0.2;

   /** \action
   * Call Coarse_Cluster_Gate function
   **/
   f_success = Coarse_Cluster_Gate(calib, cluster_1, cluster_2);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_FALSE(f_success);
}

/** @}*/

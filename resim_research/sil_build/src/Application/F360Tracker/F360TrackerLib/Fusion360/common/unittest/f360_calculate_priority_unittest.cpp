/** \file
   Give a detailed description of what does this unit-test file contain
*/

#include "f360_calculate_priority.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;
//#include "headerfile_needed.h"

//sneak in mocked functions
//Declaration of stubbed/mock functions

//Implementation of stubbed interfaces

/** \defgroup  f360_calculate_priority_for_cluster
 *  This function check such that Calculate_Priority_For_Cluster() is functioning as intended
 *  @{
 */

/** \brief
*  Add brief description of test group
**/
TEST_GROUP(f360_calculate_priority_for_cluster)
{

   F360_Host_T host{}; // Host properties
   F360_Cluster_T cluster{}; // Cluster properties
   const float32_t  cluster_expected_confidence = 1.0F; // Default confidence that is expected for all clusters

   /** \setup
   * Setup default test host properites - same for all testst. Only host vcs_speed and host curvature_rear
   *    are used by the function so only thees needs to be set up. Exact values are not that important. 
   * 
   * Setup default cluster position - same for all test. Exact values are not that important.
   *    Note: the cluster properties f_dealiased, rep_rdotcomp and num_types_of_dets[0] are also used by the
   *    function and needs to be setup individually for all tests
   **/
   TEST_SETUP()
   {
      // Default host properties
      host.vcs_speed = 20.0F;
      host.curvature_rear = 0.005F;

      // Default cluster position
      cluster.vcs_position_x = 30.0F;
      cluster.vcs_position_y = -0.5F;
   }
};

/**
*\purpose  Check such that the function is working as intended when the cluster is not dealiased and has no
*          associated moving detections. Expected behaviour is that priority should be computed based on zero
*          moving probability.
*\req    NA
*/
TEST(f360_calculate_priority_for_cluster, Test_Not_Dealised_No_Moving_Detections)
{

   /** \precond
   * Use default host and cluster settings from test group.
   * Set cluster
   *    - f_dealised flag to false
   *    - rep_rdot_comp to something large (larger than 0.8F)
   *    - num_types_of_dets[0] to 0
   * Set expected cluster moving probability for this test to 0.
   **/
   cluster.f_dealiased = false;
   cluster.rep_rdotcomp = 20.0F;
   cluster.num_types_of_dets[0] = 0;
   const float32_t cluster_expected_moving_prob = 0.0F;

   /** \action
   * Compute expected cluster priority by calling Calculate_Priority() with expected cluster confidence and moving probability.
   * Call Calculate_Priority_For_Cluster() to compute cluster priority
   **/
   const float32_t expected_cluster_prio = Calculate_Priority(host, cluster_expected_moving_prob, cluster_expected_confidence, cluster.vcs_position_x, cluster.vcs_position_y);
   const float32_t actual_cluster_prio = Calculate_Priority_For_Cluster(host, cluster);

   /** \result
   * Test that the cluster priority has the expected value
   **/
   DOUBLES_EQUAL(expected_cluster_prio, actual_cluster_prio, F360_EPSILON);
}

/**
*\purpose  Check such that the function is working as intended when the cluster is not dealiased and has
*          associated moving detections. Expected behaviour is that priority should be computed based on
*          highest moving probability.
*\req    NA
*/
TEST(f360_calculate_priority_for_cluster, Test_Not_Dealised_One_Moving_Detections)
{

   /** \precond
   * Use default host and cluster settings from test group.
   * Set cluster
   *    - f_dealised flag to false
   *    - rep_rdot_comp to something small (smaller than 0.8F)
   *    - num_types_of_dets[0] to 1
   * Set expected cluster moving probability for this test to 1.
   **/
   cluster.f_dealiased = false;
   cluster.rep_rdotcomp = 0.0F;
   cluster.num_types_of_dets[0] = 1;
   const float32_t cluster_expected_moving_prob = 1.0F;

   /** \action
   * Compute expected cluster priority by calling Calculate_Priority() with expected cluster confidence and moving probability.
   * Call Calculate_Priority_For_Cluster() to compute cluster priority
   **/
   const float32_t expected_cluster_prio = Calculate_Priority(host, cluster_expected_moving_prob, cluster_expected_confidence, cluster.vcs_position_x, cluster.vcs_position_y);
   const float32_t actual_cluster_prio = Calculate_Priority_For_Cluster(host, cluster);

   /** \result
   * Test that the cluster priority has the expected value
   **/
   DOUBLES_EQUAL(expected_cluster_prio, actual_cluster_prio, F360_EPSILON);
}

/**
*\purpose  Check such that the function is working as intended when the cluster is dealiased and has no
*          associated moving detections and small compensated range rate. Expected behaviour is that
*          priority should be computed based on zero moving probability.
*\req    NA
*/
TEST(f360_calculate_priority_for_cluster, Test_Dealised_No_Moving_Detections_Small_Rdotcomp)
{

   /** \precond
   * Use default host and cluster settings from test group.
   * Set cluster
   *    - f_dealised flag to true
   *    - rep_rdot_comp to something small (larger than 0.8F)
   *    - num_types_of_dets[0] to 0
   * Set expected cluster moving probability for this test to 0.
   **/
   cluster.f_dealiased = true;
   cluster.rep_rdotcomp = 0.8F - F360_EPSILON;
   cluster.num_types_of_dets[0] = 0;
   const float32_t cluster_expected_moving_prob = 0.0F;

   /** \action
   * Compute expected cluster priority by calling Calculate_Priority() with expected cluster confidence and moving probability.
   * Call Calculate_Priority_For_Cluster() to compute cluster priority
   **/
   const float32_t expected_cluster_prio = Calculate_Priority(host, cluster_expected_moving_prob, cluster_expected_confidence, cluster.vcs_position_x, cluster.vcs_position_y);
   const float32_t actual_cluster_prio = Calculate_Priority_For_Cluster(host, cluster);

   /** \result
   * Test that the cluster priority has the expected value
   **/
   DOUBLES_EQUAL(expected_cluster_prio, actual_cluster_prio, F360_EPSILON);
}

/**
*\purpose  Check such that the function is working as intended when the cluster is dealiased and has
*          associated moving detections but small compensated range rate. Expected behaviour is that
*          priority should be computed based on highest moving probability.
*\req    NA
*/
TEST(f360_calculate_priority_for_cluster, Test_Dealised_One_Moving_Detections_Small_Rdotcomp)
{

   /** \precond
   * Use default host and cluster settings from test group.
   * Set cluster
   *    - f_dealised flag to true
   *    - rep_rdot_comp to 0
   *    - num_types_of_dets[0] to 1
   * Set expected cluster moving probability for this test to 1.
   **/
   cluster.f_dealiased = true;
   cluster.rep_rdotcomp = 0.0F;
   cluster.num_types_of_dets[0] = 1;
   const float32_t cluster_expected_moving_prob = 1.0F;

   /** \action
   * Compute expected cluster priority by calling Calculate_Priority() with expected cluster confidence and moving probability.
   * Call Calculate_Priority_For_Cluster() to compute cluster priority
   **/
   const float32_t expected_cluster_prio = Calculate_Priority(host, cluster_expected_moving_prob, cluster_expected_confidence, cluster.vcs_position_x, cluster.vcs_position_y);
   const float32_t actual_cluster_prio = Calculate_Priority_For_Cluster(host, cluster);

   /** \result
   * Test that the cluster priority has the expected value
   **/
   DOUBLES_EQUAL(expected_cluster_prio, actual_cluster_prio, F360_EPSILON);
}

/**
*\purpose  Check such that the function is working as intended when the cluster is dealiased, has no
*          associated moving detections but large compensated range rate (abive 0.8). Expected
*          behaviour is that priority should be computed based on highest moving probability.
*\req    NA
*/
TEST(f360_calculate_priority_for_cluster, Test_Dealised_No_Moving_Detections_High_Rdotcomp)
{

   /** \precond
   * Use default host and cluster settings from test group.
   * Set cluster
   *    - f_dealised flag to true
   *    - rep_rdot_comp to something large (larger than 0.8F)
   *    - num_types_of_dets[0] to 0
   * Set expected cluster moving probability for this test to 1.
   **/
   cluster.f_dealiased = true;
   cluster.rep_rdotcomp = 0.8F + F360_EPSILON;
   cluster.num_types_of_dets[0] = 0;
   const float32_t cluster_expected_moving_prob = 1.0F;

   /** \action
   * Compute expected cluster priority by calling Calculate_Priority() with expected cluster confidence and moving probability.
   * Call Calculate_Priority_For_Cluster() to compute cluster priority
   **/
   const float32_t expected_cluster_prio = Calculate_Priority(host, cluster_expected_moving_prob, cluster_expected_confidence, cluster.vcs_position_x, cluster.vcs_position_y);
   const float32_t actual_cluster_prio = Calculate_Priority_For_Cluster(host, cluster);

   /** \result
   * Test that the cluster priority has the expected value
   **/
   DOUBLES_EQUAL(expected_cluster_prio, actual_cluster_prio, F360_EPSILON);
}

/**
*\purpose  Check such that the function is working as intended when the cluster is dealiased, has no
*          associated moving detections but large magnitude but negative compensated range rate (below  -0.8).
*          Expected behaviour is that priority should be computed based on highest moving probability.
*\req    NA
*/
TEST(f360_calculate_priority_for_cluster, Test_Dealised_No_Moving_Detections_High_Negative_Rdotcomp)
{

   /** \precond
   * Use default host and cluster settings from test group.
   * Set cluster
   *    - f_dealised flag to true
   *    - rep_rdot_comp to something largely negative (below -0.8F)
   *    - num_types_of_dets[0] to 0
   * Set expected cluster moving probability for this test to 1.
   **/
   cluster.f_dealiased = true;
   cluster.rep_rdotcomp = -0.8F - F360_EPSILON;
   cluster.num_types_of_dets[0] = 0;
   const float32_t cluster_expected_moving_prob = 1.0F;

   /** \action
   * Compute expected cluster priority by calling Calculate_Priority() with expected cluster confidence and moving probability.
   * Call Calculate_Priority_For_Cluster() to compute cluster priority
   **/
   const float32_t expected_cluster_prio = Calculate_Priority(host, cluster_expected_moving_prob, cluster_expected_confidence, cluster.vcs_position_x, cluster.vcs_position_y);
   const float32_t actual_cluster_prio = Calculate_Priority_For_Cluster(host, cluster);

   /** \result
   * Test that the cluster priority has the expected value
   **/
   DOUBLES_EQUAL(expected_cluster_prio, actual_cluster_prio, F360_EPSILON);
}

/** @}*/

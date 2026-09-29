/** \file
   This unit-test file contains test for verifying that the Clear_Cluster function
   clears the cluster struct.
*/

#include "f360_clear_cluster.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include <cstring>

using namespace f360_variant_A;
//sneak in mocked functions
//Declaration of stubbed/mock functions

//Implementation of stubbed interfaces

/** \defgroup  f360_clear_cluster
 *  @{
**/

/** \brief
*  The Clear_Cluster function will reset all the members of the given cluster to
*  the initial values.
*  Test that a given cluster is cleared.
**/
TEST_GROUP(f360_clear_cluster)
{
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
*\purpose Verify that data member of a given cluster is cleared.
*\req    NA
*/
TEST(f360_clear_cluster, TestCall2Function)
{
   /** \step{1}
    * Put data in a cluster and ensure that it is cleared after calling function
    **/

   /** \precond
   * Set up cluster and populate it's data members.
   **/
   F360_Cluster_T cluster;
   F360_Cluster_T zero_cluster;
   (void*)memset(&cluster, 0, sizeof(F360_Cluster_T));
   (void*)memset(&zero_cluster, 0, sizeof(F360_Cluster_T));
   zero_cluster.id = 1;
   zero_cluster.time_since_cluster_updated = -1.0F;
   zero_cluster.time_since_measurement = -1.0F;

   cluster.vcs_position_x = 25.6F;
   cluster.vcs_position_y = 10.1F;
   cluster.rep_vcs_az = 1.15F;
   cluster.cos_vcs_az = 0.2F;
   cluster.sin_vcs_az = 0.96F;
   cluster.rep_rdotcomp = 0.2F;
   cluster.id = 1;
   cluster.ndets = 2;
   cluster.detids[0] = 1;
   cluster.detids[1] = 2;
   cluster.num_old_dets = 2;
   cluster.old_det_idx[0] = 3;
   cluster.old_det_idx[1] = 4;
   cluster.num_types_of_dets[0] = 1;
   cluster.num_types_of_dets[1] = 1;
   cluster.f_to_be_killed = true;
   cluster.low_rcs_dets_cnt = 1U;
   cluster.f_dealiased = true;
   cluster.time_since_cluster_updated = 0.05F;


   /** \action
   * Call the Clear_Cluster function.
   **/
   Clear_Cluster(cluster);

   /** \result
   * Loop through all bytes of the cluster data structure and ensure that the data
   * corresponds to a cluster which has been memset to zero.
   **/
   char* ptr = (char*)&cluster;
   char* ptr2 = (char*)&zero_cluster;
   for (uint32_t i = 0; i < sizeof(F360_Cluster_T)/sizeof(char); i++)
   {
      CHECK_TRUE(*ptr2 == *ptr);
      ptr++;
      ptr2++;
   }

}

/** @}*/

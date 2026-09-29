/** \file
   File contains unit tests for Update_Cluster_State function.
*/

#include "f360_update_cluster_state.h"
#include "f360_math.h"

#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

inline bool Compare_Clusters(const F360_Cluster_T & expected_cluster, const F360_Cluster_T& cluster)
{
   const float32_t ERROR_THR = 0.0001F;
   bool f_are_equal = true;
   f_are_equal &= std::abs(expected_cluster.vcs_position_x - cluster.vcs_position_x) < ERROR_THR;
   f_are_equal &= std::abs(expected_cluster.vcs_position_y - cluster.vcs_position_y) < ERROR_THR;
   f_are_equal &= std::abs(expected_cluster.rep_vcs_az - cluster.rep_vcs_az) < ERROR_THR;
   f_are_equal &= std::abs(expected_cluster.cos_vcs_az - cluster.cos_vcs_az) < ERROR_THR;
   f_are_equal &= std::abs(expected_cluster.sin_vcs_az - cluster.sin_vcs_az) < ERROR_THR;
   f_are_equal &= std::abs(expected_cluster.ndets - cluster.ndets) < ERROR_THR;
   f_are_equal &= std::abs(expected_cluster.num_old_dets - cluster.num_old_dets) < ERROR_THR;
   f_are_equal &= std::abs(expected_cluster.f_dealiased -  cluster.f_dealiased) < ERROR_THR;

   return f_are_equal;
}

/** \defgroup  f360_update_cluster_state
 *  @{
 */

/** \brief
 *  Test group for Update_Cluster_State function
 **/
TEST_GROUP(f360_update_cluster_state)
{
   /** \setup
    * Setting up arguments for Update_Cluster_State function and assigning them with basic values
    **/   
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Detection_Hist_T  det_hist = {};
   F360_Cluster_T cluster = {};
   TEST_SETUP()
   { 
      for (uint32_t i = 0; i < MAX_DETS_IN_OBJ_TRK; i++)
      {
         cluster.detids[i] = i + 1;
      }
   }
};

/**
 *\purpose   Checking whether cluster f_dealiase flag is set for true when there is no detections 
 *           in cluster.
 *\req    NA
 */
TEST(f360_update_cluster_state, Test_Lack_Of_Detections_In_Cluster)
{
   /** \precond
    *   Set number of dets for 0. 
    **/
   cluster.ndets = 0;
   cluster.num_old_dets = 0;

   /** \action
    *    Call Update_Cluster_State
    **/
   Update_Cluster_State(det_props, raw_detection_list, det_hist, cluster);

   /** \result
    *   Checking if cluster has flag f_dealiased set for true.
    **/
   CHECK_TRUE(cluster.f_dealiased);
}

/**
 *\purpose  Checking whether cluster f_dealiase flag is set for false when there is one not dealiased detection 
 *          in cluster.
 *\req    NA
 */
TEST(f360_update_cluster_state, Test_Not_Dealiased_Detection)
{
   /** \precond
    *   Set number of dets to 1 and this detecion f_dealiased flag to false. 
    **/
   cluster.ndets = 1;
   cluster.num_old_dets = 0;
   det_props[0].f_dealiased = false;

   /** \action
    *    Call Update_Cluster_State
    **/
   Update_Cluster_State(det_props, raw_detection_list, det_hist, cluster);

   /** \result
    *  Checking if cluster has flag f_dealiased set for false.
    **/
   CHECK_FALSE(cluster.f_dealiased);
}

/**
 *\purpose  Checking whether cluster f_dealiase flag is set for false when there is one old detection 
 *          in cluster.
 *\req    NA
 */
TEST(f360_update_cluster_state, Test_Not_Dealiased_Historical_Detection)
{
   /** \precond
    *   Set number of old dets to 1 and this detecion f_dealiased flag to false. 
    **/
   cluster.ndets = 0;
   cluster.num_old_dets = 1;
   det_hist.det_data[0].f_dealiased = false;
   /** \action
    *    Call Update_Cluster_State
    **/
   Update_Cluster_State(det_props, raw_detection_list, det_hist, cluster);

   /** \result
    *   Checking if cluster has flag f_dealiased set for false.
    **/
   CHECK_FALSE(cluster.f_dealiased);
}

/**
 *\purpose  Checking whether cluster properties are equal to expected cluster properties. 
 *\req    NA
 */
TEST(f360_update_cluster_state, Test_Cluster_Only_With_Current_Dets)
{
   /** \precond
    *   Set number of dets to 3 and those detecions f_dealiased flag to true.
    *   Set those detections parameters
    **/
   cluster.rep_vcs_az = 1.0F;
   cluster.ndets = 3;

   det_props[0].vcs_position.x = 10.0F;
   det_props[0].vcs_position.y = -5.0F;
   det_props[0].range_rate_compensated = -11.7F;
   raw_detection_list.detections[0].processed.vcs_az = 0.98F;
   det_props[0].f_dealiased = true;

   det_props[1].vcs_position.x = 10.5F;
   det_props[1].vcs_position.y = -5.2F;
   det_props[1].range_rate_compensated = -12.3F;
   raw_detection_list.detections[1].processed.vcs_az = 1.01F;
   det_props[1].f_dealiased = true;

   det_props[2].vcs_position.x = 9.5F;
   det_props[2].vcs_position.y = -4.8F;
   det_props[2].range_rate_compensated = -12.0F;
   raw_detection_list.detections[2].processed.vcs_az = 1.01F;
   det_props[2].f_dealiased = true;

   F360_Cluster_T exp_cluster = {};
   exp_cluster.ndets = cluster.ndets;
   exp_cluster.rep_vcs_az = 1.0F;
   exp_cluster.cos_vcs_az = F360_Cosf(cluster.rep_vcs_az);
   exp_cluster.sin_vcs_az = F360_Sinf(cluster.rep_vcs_az);
   exp_cluster.vcs_position_x = 10.0F;
   exp_cluster.vcs_position_y = -5.0F;
   exp_cluster.rep_rdotcomp = -12.0F;
   exp_cluster.f_dealiased = true;

   /** \action
    *    Call Update_Cluster_State
    **/
   Update_Cluster_State(det_props, raw_detection_list, det_hist, cluster);

   /** \result
    *   Checking if cluster properties ar equal to expected. 
    **/
   bool f_equal_clusters = Compare_Clusters(exp_cluster, cluster);
   CHECK_TRUE(f_equal_clusters);
}

/**
 *\purpose Checking whether cluster properties are equal to expected cluster properties. 
 *\req    NA
 */
TEST(f360_update_cluster_state, Test_Cluster_Only_With_Current_Dets_And_One_Is_Not_Dealiased)
{
   /** \precond
    *   Set number of dets to 3 and one of those detecions f_dealiased flag to false.
    *   Set those detections parameters
    **/
   cluster.rep_vcs_az = 1.0F;
   cluster.ndets = 3;

   det_props[0].vcs_position.x = 10.0F;
   det_props[0].vcs_position.y = -5.0F;
   det_props[0].range_rate_compensated = -11.7F;
   raw_detection_list.detections[0].processed.vcs_az = 0.98F;
   det_props[0].f_dealiased = true;

   det_props[1].vcs_position.x = 10.5F;
   det_props[1].vcs_position.y = -5.2F;
   det_props[1].range_rate_compensated = -12.3F;
   raw_detection_list.detections[1].processed.vcs_az = 1.01F;
   det_props[1].f_dealiased = true;

   det_props[2].vcs_position.x = 9.5F;
   det_props[2].vcs_position.y = -4.8F;
   det_props[2].range_rate_compensated = -12.0F;
   raw_detection_list.detections[2].processed.vcs_az = 1.01F;
   det_props[2].f_dealiased = false;

   F360_Cluster_T exp_cluster = {};
   exp_cluster.ndets = cluster.ndets;
   exp_cluster.rep_vcs_az = 1.0F;
   exp_cluster.cos_vcs_az = F360_Cosf(cluster.rep_vcs_az);
   exp_cluster.sin_vcs_az = F360_Sinf(cluster.rep_vcs_az);
   exp_cluster.vcs_position_x = 10.0F;
   exp_cluster.vcs_position_y = -5.0F;
   exp_cluster.rep_rdotcomp = -12.0F;
   exp_cluster.f_dealiased = false;

   /** \action
    *    Call Update_Cluster_State
    **/
   Update_Cluster_State(det_props, raw_detection_list, det_hist, cluster);

   /** \result
    *   Checking if cluster properties ar equal to expected. 
    **/
   bool f_equal_clusters = Compare_Clusters(exp_cluster, cluster);
   CHECK_TRUE(f_equal_clusters);
}

/**
 *\purpose Checking whether cluster properties are equal to expected cluster properties. 
 *\req    NA
 */
TEST(f360_update_cluster_state, Test_Cluster_One_Hist_Det_Not_Dealiased)
{
   /** \precond
    *   Set number of dets to 3 and those detecions f_dealiased flag to true.
    *   Set those detections parameters.
    *   Set number of old dets to 1, and this detection f_dealiased flag to false.
    **/
   cluster.rep_vcs_az = 1.0F;
   cluster.ndets = 3;

   det_props[0].vcs_position.x = 10.0F;
   det_props[0].vcs_position.y = -5.0F;
   det_props[0].range_rate_compensated = -11.7F;
   raw_detection_list.detections[0].processed.vcs_az = 0.98F;
   det_props[0].f_dealiased = true;

   det_props[1].vcs_position.x = 10.5F;
   det_props[1].vcs_position.y = -5.2F;
   det_props[1].range_rate_compensated = -12.3F;
   raw_detection_list.detections[1].processed.vcs_az = 1.01F;
   det_props[1].f_dealiased = true;

   det_props[2].vcs_position.x = 9.5F;
   det_props[2].vcs_position.y = -4.8F;
   det_props[2].range_rate_compensated = -12.0F;
   raw_detection_list.detections[2].processed.vcs_az = 1.01F;
   det_props[2].f_dealiased = true;

   cluster.num_old_dets = 2;
   cluster.old_det_idx[0] = 0;
   cluster.old_det_idx[1] = 1;

   det_hist.det_data[0].f_dealiased = true;
   det_hist.det_data[1].f_dealiased = false;

   F360_Cluster_T exp_cluster = {};
   exp_cluster.ndets = cluster.ndets;
   exp_cluster.num_old_dets = cluster.num_old_dets;
   exp_cluster.rep_vcs_az = 1.0F;
   exp_cluster.cos_vcs_az = F360_Cosf(cluster.rep_vcs_az);
   exp_cluster.sin_vcs_az = F360_Sinf(cluster.rep_vcs_az);
   exp_cluster.vcs_position_x = 10.0F;
   exp_cluster.vcs_position_y = -5.0F;
   exp_cluster.rep_rdotcomp = -12.0F;
   exp_cluster.f_dealiased = false;

   /** \action
    *    Call Update_Cluster_State
    **/
   Update_Cluster_State(det_props, raw_detection_list, det_hist, cluster);

   /** \result
    *   Checking if cluster properties ar equal to expected. 
    **/
   bool f_equal_clusters = Compare_Clusters(exp_cluster, cluster);
   CHECK_TRUE(f_equal_clusters);
}

/** @}*/

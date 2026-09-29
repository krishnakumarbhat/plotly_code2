/** \file
 * This file contains unit tests for content of f360_estimate_velocity_by_cloud.cpp file
 */

#include "f360_estimate_velocity_by_cloud.h"
#include "f360_math_func.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_estimate_velocity_by_cloud
 *  @{
 */

/** \brief
 * Group for testing Estimate_Velocity_By_Cloud()- function calculating lateral and longitudinal velocities with the cloud algorithm using
 * IRLS (Iterative Reweighted Least Squares) as well as confidences of the estimation.
 */
TEST_GROUP(f360_estimate_velocity_by_cloud)
{
   // Declare common variables used within all tests in this test group.
   F360_Detection_Hist_T det_hist;
   rspp_variant_A::RSPP_Detection_List_T raw_detections;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Cluster_T cluster;
   float32_t longvel_by_cloud;
   float32_t latvel_by_cloud;
   float32_t xpos_cloud;
   float32_t ypos_cloud;
   float32_t xvel_cloud;
   float32_t yvel_cloud;
   float32_t ypos_min;
   float32_t ypos_max;

   /** \setup
    * Prepare positions and velocities as well as initial range of position variability throughought the detections of a moving cluster.
    */
   TEST_SETUP()
   {
      xpos_cloud = 10.0F;
      ypos_cloud = 0.0F;
      xvel_cloud = 10.0F;
      yvel_cloud = 0.0F;
      ypos_min = -1.5F;
      ypos_max = 1.5F;
   }

   // Define helper functions used in this test group here. E.g. Add_Detection_To_Detection_List().

   void add_detection_to_cluster(
      F360_Cluster_T& cluster,
      rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      float32_t det_y_pos,
      float32_t rr_comp)
   {
      int32_t det_idx = raw_detect_list.number_of_valid_detections;
      raw_detect_list.number_of_valid_detections++;
      cluster.detids[cluster.ndets] = det_idx + 1;
      cluster.ndets++;

      raw_detect_list.detections[det_idx].raw.sensor_id = 1;
      raw_detect_list.detections[det_idx].processed.vcs_position_x = cluster.vcs_position_x;
      raw_detect_list.detections[det_idx].processed.vcs_position_y = det_y_pos;
      raw_detect_list.detections[det_idx].processed.vcs_az = F360_Atan2f(
         raw_detect_list.detections[det_idx].processed.vcs_position_y,
         raw_detect_list.detections[det_idx].processed.vcs_position_x);
      raw_detect_list.detections[det_idx].processed.cos_vcs_az = F360_Cosf(raw_detect_list.detections[det_idx].processed.vcs_az);
      raw_detect_list.detections[det_idx].processed.sin_vcs_az = F360_Sinf(raw_detect_list.detections[det_idx].processed.vcs_az);
      raw_detect_list.detections[det_idx].processed.range_rate_compensated = rr_comp;
      raw_detect_list.detections[det_idx].raw.confid_azimuth = 0;
      raw_detect_list.detections[det_idx].raw.confid_elevation = 0;
      raw_detect_list.detections[det_idx].raw.f_super_res = false;

      det_props[det_idx].vcs_position.x = raw_detect_list.detections[det_idx].processed.vcs_position_x;
      det_props[det_idx].vcs_position.y = raw_detect_list.detections[det_idx].processed.vcs_position_y;
      det_props[det_idx].range_rate_compensated = raw_detect_list.detections[det_idx].processed.range_rate_compensated;
   }

   // creates moving cluster with given number of current and historical detections with varying detection properties to cover all of their possible options
   void setup_moving_cluster_for_cloud_nonperfect_dets(
      int32_t num_curr_dets,
      int32_t num_hist_dets,
      F360_Cluster_T (&clusters),
      rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Detection_Hist_T& det_hist,
      const float xpos, const float ypos, const float xvel, const float yvel)
   {
      clusters.id = 1;
      clusters.vcs_position_x = xpos;
      clusters.vcs_position_y = ypos;
      clusters.rep_vcs_az = F360_Atan2f(ypos, xpos);
      clusters.cos_vcs_az = F360_Cosf(clusters.rep_vcs_az);
      clusters.sin_vcs_az = F360_Sinf(clusters.rep_vcs_az);
      clusters.ndets = 0;
      clusters.num_old_dets = 0;
      clusters.rep_rdotcomp = clusters.cos_vcs_az * xvel + clusters.sin_vcs_az * yvel;
      clusters.f_dealiased = true;

      float32_t ypos_arr[MAX_NUMBER_OF_DETECTIONS]{};
      for (int32_t i = 0; i < num_hist_dets; i++)
      {
         ypos_arr[i] = F360_Linear_Equation(
            i,
            0,
            num_hist_dets,
            ypos_min,
            ypos_max);
      }

      for (int32_t i = 0; i < num_curr_dets; i++)
      {
         int32_t n = raw_detect_list.number_of_valid_detections;
         raw_detect_list.number_of_valid_detections++;
         clusters.detids[i] = n + 1;
         clusters.ndets++;
         raw_detect_list.detections[n].raw.sensor_id = 1;
         raw_detect_list.detections[n].processed.vcs_position_x = clusters.vcs_position_x;
         raw_detect_list.detections[n].processed.vcs_position_y = clusters.vcs_position_y + ypos_arr[i];
         raw_detect_list.detections[n].processed.vcs_az = F360_Atan2f(raw_detect_list.detections[n].processed.vcs_position_y, raw_detect_list.detections[n].processed.vcs_position_x);
         raw_detect_list.detections[n].processed.cos_vcs_az = F360_Cosf(raw_detect_list.detections[n].processed.vcs_az);
         raw_detect_list.detections[n].processed.sin_vcs_az = F360_Sinf(raw_detect_list.detections[n].processed.vcs_az);
         raw_detect_list.detections[n].processed.range_rate_compensated = raw_detect_list.detections[n].processed.cos_vcs_az * xvel + raw_detect_list.detections[n].processed.sin_vcs_az * yvel;
         raw_detect_list.detections[n].raw.confid_azimuth = (i % 4);
         raw_detect_list.detections[n].raw.confid_elevation = std::min(i, 3);
         raw_detect_list.detections[n].raw.elevation = F360_DEG2RAD(i*2);
         raw_detect_list.detections[n].raw.f_super_res= static_cast<bool>(i % 2);

         det_props[n].vcs_position.x = raw_detect_list.detections[n].processed.vcs_position_x;
         det_props[n].vcs_position.y = raw_detect_list.detections[n].processed.vcs_position_y;
         det_props[n].range_rate_compensated = raw_detect_list.detections[n].processed.range_rate_compensated;
         det_props[n].f_FOV_edge = static_cast<bool>((i + 1) % 2);
      }

      for (int32_t i = 0; i < num_hist_dets; i++)
      {
         float T = 0.05F;

         int32_t n = det_hist.n_occupied;
         clusters.num_old_dets++;
         det_hist.n_occupied++;
         clusters.old_det_idx[i] = n;
         det_hist.det_data[n].sensor_id = 1;
         det_hist.det_data[n].vcs_position_x = clusters.vcs_position_x - T * xvel;
         det_hist.det_data[n].vcs_position_y = clusters.vcs_position_y - T * yvel  + ypos_arr[i];
         det_hist.det_data[n].vcs_az = F360_Atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
         det_hist.det_data[n].rdot_comp = F360_Cosf(det_hist.det_data[n].vcs_az) * xvel + F360_Sinf(det_hist.det_data[n].vcs_az) * yvel;
         det_hist.det_data[n].time_since_meas = T;
         det_hist.det_data[n].az_conf = ((i + 1) % 4);
         det_hist.det_data[n].el_conf = std::min(i, 3);
         det_hist.det_data[n].f_super_res= static_cast<bool>(i % 2);
         det_hist.det_data[n].elevation = F360_DEG2RAD(i*2);
         det_hist.det_data[n].f_FOV_edge = static_cast<bool>(i % 2);
      }
   }

   void setup_moving_cluster_for_cloud_perfect_dets(
   int32_t num_curr_dets,
   int32_t num_hist_dets,
      F360_Cluster_T (&clusters),
      rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Detection_Hist_T& det_hist,
      const float xpos, const float ypos, const float xvel, const float yvel, const bool f_gap = false)
   {
      clusters.id = 1;
      clusters.vcs_position_x = xpos;
      clusters.vcs_position_y = ypos;
      clusters.rep_vcs_az = F360_Atan2f(ypos, xpos);
      clusters.cos_vcs_az = F360_Cosf(clusters.rep_vcs_az);
      clusters.sin_vcs_az = F360_Sinf(clusters.rep_vcs_az);
      clusters.ndets = 0;
      clusters.num_old_dets = 0;
      clusters.rep_rdotcomp = clusters.cos_vcs_az * xvel + clusters.sin_vcs_az * yvel;
      clusters.f_dealiased = true;

      float32_t ypos_arr[MAX_NUMBER_OF_DETECTIONS]{};
      if (!f_gap)
      {
         for (int32_t i = 0; i < num_hist_dets; i++)
         {
            ypos_arr[i] = F360_Linear_Equation(
               i,
               0,
               num_hist_dets,
               ypos_min,
               ypos_max);
         }
      }
      else
      {
         // create a gap in the middle of the ypos range
         int32_t half_num_hist_dets = num_hist_dets / 2;
         for (int32_t i = 0; i < half_num_hist_dets; i++)
         {
            ypos_arr[i] = F360_Linear_Equation(
               i,
               0,
               half_num_hist_dets,
               ypos_min,
               (ypos_min + ypos_max) / 2.0F - 1.0F);
         }
         for (int32_t i = half_num_hist_dets; i < num_hist_dets; i++)
         {
            ypos_arr[i] = F360_Linear_Equation(
               i - half_num_hist_dets,
               0,
               num_hist_dets - half_num_hist_dets,
               (ypos_min + ypos_max) / 2.0F + 1.0F,
               ypos_max);
         }
      }
      

      for (int32_t i = 0; i < num_curr_dets; i++)
      {
         int32_t n = raw_detect_list.number_of_valid_detections;
         raw_detect_list.number_of_valid_detections++;
         clusters.detids[i] = n + 1;
         clusters.ndets++;
         raw_detect_list.detections[n].raw.sensor_id = 1;
         raw_detect_list.detections[n].processed.vcs_position_x = clusters.vcs_position_x;
         raw_detect_list.detections[n].processed.vcs_position_y = clusters.vcs_position_y + ypos_arr[i];
         raw_detect_list.detections[n].processed.vcs_az = F360_Atan2f(raw_detect_list.detections[n].processed.vcs_position_y, raw_detect_list.detections[n].processed.vcs_position_x);
         raw_detect_list.detections[n].processed.cos_vcs_az = F360_Cosf(raw_detect_list.detections[n].processed.vcs_az);
         raw_detect_list.detections[n].processed.sin_vcs_az = F360_Sinf(raw_detect_list.detections[n].processed.vcs_az);
         raw_detect_list.detections[n].processed.range_rate_compensated = raw_detect_list.detections[n].processed.cos_vcs_az * xvel + raw_detect_list.detections[n].processed.sin_vcs_az * yvel;
         raw_detect_list.detections[n].raw.confid_azimuth = 0;
         raw_detect_list.detections[n].raw.confid_elevation = 0;
         raw_detect_list.detections[n].raw.f_super_res = false;

         det_props[n].vcs_position.x = raw_detect_list.detections[n].processed.vcs_position_x;
         det_props[n].vcs_position.y = raw_detect_list.detections[n].processed.vcs_position_y;
         det_props[n].range_rate_compensated = raw_detect_list.detections[n].processed.range_rate_compensated;
      }

      for (int32_t i = 0; i < num_hist_dets; i++)
      {
         float T = 0.05F;

         int32_t n = det_hist.n_occupied;
         clusters.num_old_dets++;
         det_hist.n_occupied++;
         clusters.old_det_idx[i] = n;
         det_hist.det_data[n].sensor_id = 1;
         det_hist.det_data[n].vcs_position_x = clusters.vcs_position_x - T * xvel;
         det_hist.det_data[n].vcs_position_y = clusters.vcs_position_y - T * yvel  + ypos_arr[i];
         det_hist.det_data[n].vcs_az = F360_Atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
         det_hist.det_data[n].rdot_comp = F360_Cosf(det_hist.det_data[n].vcs_az) * xvel + F360_Sinf(det_hist.det_data[n].vcs_az) * yvel;
         det_hist.det_data[n].time_since_meas = T;
         det_hist.det_data[n].f_super_res = false;
         det_hist.det_data[n].elevation = F360_DEG2RAD(3.0F);
         det_hist.det_data[n].az_conf = 0;
         det_hist.det_data[n].el_conf = 0;
      }
   }

};

/** \purpose
 * Tests whether lateral and longitudinal velocity of a moving cluster with good quality detections is
 * estimated correctly and with high confidence
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, HighConfidenceEstimation){
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * ypos_min = -1.5F
    * ypos_max = 1.5F
    *
    * moving cluster set up with 4 current and 6 historical detections
    * quality of all the detections is good (properties with high confidence, etc.)
    */

   setup_moving_cluster_for_cloud_perfect_dets(4, 6, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_HIGH
    * Expected longvel_by_cloud consistent with input xvel_cloud whithin small margin
    * Expected latvel_by_cloud consistent with input yvel_cloud whithin small margin
    */
   CHECK_EQUAL(result, CONF3_HIGH);
   DOUBLES_EQUAL(longvel_by_cloud, xvel_cloud, 0.0001F);
   DOUBLES_EQUAL(latvel_by_cloud, yvel_cloud, 0.0001F);
}

/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_NONE when the cluster does not meet the minimal requirement
 * on the amount of detections to be further processed by the cloud algorithm.
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, InsufficientDetections) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * ypos_min = -1.5F
    * ypos_max = 1.5F
    *
    * moving cluster set up with 2 current and 2 historical detections
    * quality of all the detections is good (properties with high confidence, etc.)
    */
   setup_moving_cluster_for_cloud_perfect_dets(2, 2, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);
   /** \result
    * Expected confidence CONF3_NONE
    */
   CHECK_EQUAL(result, CONF3_NONE);
}



/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_NONE when the detections associated to the cluster are not spreaded sufficiently
 * in terms of azimuth for the cluster to be further considered in the cloud algorithm.
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, InsufficientAzimuthSpread) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    *
    * test specific (low azimuth spread of the detections):
    * ypos_min = -0.1F
    * ypos_max = 0.1F;
    *
    * moving cluster set up with 4 current and 6 historical detections
    * quality of all the detections is good (properties with high confidence, etc.)
    */
   ypos_min = -0.1F;
   ypos_max = 0.1F;
   setup_moving_cluster_for_cloud_perfect_dets(4, 6, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_NONE
    */
   CHECK_EQUAL(result, CONF3_NONE);
}


/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_HIGH when the the detections are varying in quality (properties- wise), but
 * other than that no outliers, etc.
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, NonPerfectDetections) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * ypos_min = -1.5F
    * ypos_max = 1.5F
    *
    * moving cluster set up with 6 current and 6 historical detections
    * quality of all the detections is varying
    */
   setup_moving_cluster_for_cloud_nonperfect_dets(6, 6, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_HIGH
    * Expected longvel_by_cloud consistent with input xvel_cloud whithin small margin
    * Expected latvel_by_cloud consistent with input yvel_cloud whithin small margin
    */

   CHECK_EQUAL(result, CONF3_HIGH);
   DOUBLES_EQUAL(longvel_by_cloud, xvel_cloud, 0.0001F);
   DOUBLES_EQUAL(latvel_by_cloud, yvel_cloud, 0.0001F);
}

/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_NONE when there is more than twice as much position ambiguous detections
 * than moving detections.
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, Num_Types_Of_Dets_1) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * 
    *
    * test specific:
    * cluster.num_types_of_dets[1]=10 // ambiguous / staionary dets
    * cluster.num_types_of_dets[0]=4 // moving dets
    * ypos_min = -1.0F
    * ypos_max = 1.0F
    *
    * moving cluster set up with 7 current and 7 historical detections
    * quality of all the detections is varying
    */
   ypos_min = -1.0F;
   ypos_max = 1.0F;
   setup_moving_cluster_for_cloud_nonperfect_dets(7, 7, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);
   cluster.num_types_of_dets[1]=10;
   cluster.num_types_of_dets[0]=4;

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_NONE
    */
   CHECK_EQUAL(result, CONF3_NONE);
}

/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_HIGH when there is more than twice as much moving detections
 * than position ambiguous detections.
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, Num_Types_Of_Dets_2) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * ypos_min = -1.5F
    * ypos_max = 1.5F
    *
    * test specific:
    * cluster.num_types_of_dets[1]=4 // ambiguous / staionary dets
    * cluster.num_types_of_dets[0]=10 // moving dets
    *
    * moving cluster set up with 7 current and 7 historical detections
    * quality of all the detections is varying
    */
   setup_moving_cluster_for_cloud_nonperfect_dets(7, 7, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);
   cluster.num_types_of_dets[1]=4;
   cluster.num_types_of_dets[0]=10;

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_NONE
    */
   CHECK_EQUAL(result, CONF3_HIGH);
}

/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_MED when there is less than twice and more than once as much ambiguous detections
 * than moving detections.
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, Num_Types_Of_Dets_3) {
   /** \precond
    * Preconditions from group setup:
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * ypos_min = -1.5F
    * ypos_max = 1.5F
    *
    * test specific:
    * cluster.num_types_of_dets[1]=8 // ambiguous / staionary dets
    * cluster.num_types_of_dets[0]=5 // moving dets
    * xpos_cloud = 15.0F
    *
    * moving cluster set up with 7 current and 7 historical detections
    * quality of all the detections is varying
    */
   xpos_cloud = 15.0F;
   setup_moving_cluster_for_cloud_nonperfect_dets(7, 7, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);
   cluster.num_types_of_dets[1]=8;
   cluster.num_types_of_dets[0]=5;


   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_MED
    */
   CHECK_EQUAL(result, CONF3_MED);
}

/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_HIGH when there is less than twice and more than once as much moving detections
 * than position ambiguous detections.
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, Num_Types_Of_Dets_4) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * ypos_min = -1.5F
    * ypos_max = 1.5F
    *
    * test specific:
    * cluster.num_types_of_dets[1]=6 // ambiguous / staionary dets
    * cluster.num_types_of_dets[0]=10 // moving dets
    *
    * moving cluster set up with 6 current and 10 historical detections
    * quality of all the detections is varying
    */
   setup_moving_cluster_for_cloud_nonperfect_dets(6, 10, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);
   cluster.num_types_of_dets[1]=6;
   cluster.num_types_of_dets[0]=10;

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_MED
    */
   CHECK_EQUAL(result, CONF3_HIGH);
}

/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_NONE when there is too big difference between rdot_comp and
 * est_rdot_comp in the detections
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, Average_Weight_Below_Minimal_Threshold) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * ypos_min = -1.5F
    * ypos_max = 1.5F
    *
    * test specific:
    * cluster set up with low weighted detections (big dfferences between rdot_comp and est_rdot_comp)
    *
    * moving cluster set up with 4 current and 6 historical detections
    * quality of all the detections good (properties- wise)
    */

   setup_moving_cluster_for_cloud_perfect_dets(4, 6, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);
   for (int32_t i = 0; i < 6; i++)
   {
      det_hist.det_data[i].rdot_comp *= 2.0F;
   }

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_NONE
    */
   CHECK_EQUAL(result, CONF3_NONE);
}

/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_LOW when the average weight of all the detections is
 * below mid confidence threshold set up in Estimate_Velocity_By_Cloud(). (ex. rdot_comp is skewed- weighs detections lower)
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, Average_Weight_Mid_Conf_Threshold) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * ypos_min = -1.5F
    * ypos_max = 1.5F
    *
    * moving cluster set up with 4 current and 6 historical detections
    * quality of all the detections good (properties- wise)
    */
   setup_moving_cluster_for_cloud_perfect_dets(4, 6, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);
   for (int32_t i = 0; i < cluster.num_old_dets; i++)
   {
      det_hist.det_data[i].rdot_comp *= 1.02F;
   }

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_LOW
    */
   CHECK_EQUAL(result, CONF3_LOW);
}

/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_MED when the average weight of all the detections is
 * above the threshold for high confidence, but there is not enough detections for the cluster to be estimated as CONF3_HIGH
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, Average_Weight_High_Conf_Threshold_Fewer_Dets) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * ypos_min = -1.5F
    * ypos_max = 1.5F
    *
    * moving cluster set up with 5 current and 2 historical detections
    * quality of all the detections good (properties- wise)
    */
   setup_moving_cluster_for_cloud_perfect_dets(5, 2, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_MED
    */
   CHECK_EQUAL(result, CONF3_MED);
}

/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_LOW when there are many rdot outliers among the cluster detections
 * and the detections azimuth spread is low.
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, Az_Spread_Low_Fewer_Inliers) {
   /** \precond
    * Preconditions from group setup:
    * ypos_cloud = 0.0F
    *
    * test specific:
    * ypos_min = -2.55859F
    * ypos_max = 2.55859F
    * xpos_cloud = 100.0F
    * yvel_cloud = 1.2F
    * xvel_cloud = 1.2F
    *
    * moving cluster set up with 2 current and 6 historical detections
    * quality of all the detections good (properties- wise)
    * historical detections rdot_comp is skewed to make the detections rdot outliers
    */
   ypos_min = -2.55859F;
   ypos_max = 2.55859F;
   xpos_cloud = 100.0F;
   yvel_cloud = 1.2F;
   xvel_cloud = 1.2F;

   setup_moving_cluster_for_cloud_perfect_dets(2, 6, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);

   for (int32_t i = 0; i < cluster.num_old_dets; i++)
   {
      det_hist.det_data[i].rdot_comp *= 2.5F;
   }

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_LOW
    */
   CHECK_EQUAL(result, CONF3_LOW);
}

/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_HIGH when there is big number of detections,
 * but some of them are rdot outliers, and average weight of the detections is below high threshold.
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, Lots_Of_Inliers_Avg_Weight_Below_High_Threshold) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * yvel_cloud = 0.0F
    *
    * test specific:
    * xvel_cloud = 1.0F
    * ypos_min = 3.0F
    * ypos_max = 4.0F
    *
    * moving cluster set up with big amount of detections 10 current and 20 historical detections
    * quality of all the detections varying (properties- wise)
    * rdot_comp for half of the historical detections is skewed to make the detections rdot outliers
    */
   xvel_cloud = 1.0F;
   ypos_min = 3.0F;
   ypos_max = 4.0F;
   setup_moving_cluster_for_cloud_nonperfect_dets(10, 20, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);
   for (int32_t i = 0; i < 10; i++)
   {
      det_hist.det_data[i].rdot_comp *= 1.5F;
   }

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_HIGH
    */
   CHECK_EQUAL(result, CONF3_HIGH);
}

/** \purpose
 * Tests whether the confidence of estimation is reduced to CONF3_MED from HIGH due to high cross radial velocity 
 * calculated by cloud init (w.r.t. radial velocity) with insufficient azimuth spread
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, Cross_Rad_Vel_Est_Fast_Obj_Conf_Reduction) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    *
    * test specific:
    * xvel_cloud = 2.0F
    * yvel_cloud = 8.0F
    * ypos_min = -0.15F
    * ypos_max = 0.15F
    *
    * moving cluster set up with 10 current and 10 historical detections
    * quality of all the detections is good (properties with high confidence, etc.)
    */
   xvel_cloud = 2.0F;
   yvel_cloud = 8.0F;
   ypos_min = -0.15F;
   ypos_max = 0.15F;
   setup_moving_cluster_for_cloud_perfect_dets(10, 10, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_MED
    */
   CHECK_EQUAL(result, CONF3_MED);
}

/** \purpose
 * Tests whether the confidence of estimation is NOT reduced to CONF3_MED from HIGH due to high cross radial velocity 
 * calculated by cloud init (w.r.t. radial velocity) because of sufficient azimuth spread
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, Cross_Rad_Vel_Est_Fast_Obj_No_Conf_Reduction_Az) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * ypos_min = -1.5F
    * yposmin = 1.5F
    *
    * test specific:
    * xvel_cloud = 2.0F
    * yvel_cloud = 8.0F
    *
    * moving cluster set up with 10 current and 10 historical detections
    * quality of all the detections is good (properties with high confidence, etc.)
    */
   xvel_cloud = 2.0F;
   yvel_cloud = 8.0F;
   setup_moving_cluster_for_cloud_perfect_dets(10, 10, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_HIGH
    */
   CHECK_EQUAL(result, CONF3_HIGH);
}

/** \purpose
 * Tests whether the confidence of estimation is NOT reduced to CONF3_MED from HIGH due to calculated cross radial
 * velocity (w.r.t. radial velocity) being not high enough to trigger the reduction
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, Cross_Rad_Vel_Est_Fast_Obj_No_Conf_Reduction_Vel) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    *
    * test specific:
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * ypos_min = -0.25F
    * ypos_max = 0.25F
    *
    * moving cluster set up with 10 current and 10 historical detections
    * quality of all the detections is good (properties with high confidence, etc.)
    */
   xvel_cloud = 10.0F;
   yvel_cloud = 0.0F;
   ypos_min = -0.25F;
   ypos_max = 0.25F;
   setup_moving_cluster_for_cloud_perfect_dets(10, 10, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_HIGH
    */
   CHECK_EQUAL(result, CONF3_HIGH);
}

/** \purpose
 * Tests whether the confidence of estimation is reduced to CONF3_MED from HIGH due to very high cross radial velocity 
 * calculated by cloud init (w.r.t. radial velocity) using very low rdotcomp dets
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, Cross_Rad_Vel_Est_Slow_Obj_Conf_Reduction) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * ypos_min = -1.5F
    * yposmin = 1.5F
    *
    * test specific:
    * xvel_cloud = 0.0F
    * yvel_cloud = 2.0F
    *
    * moving cluster set up with 2 current and 10 historical detections
    * quality of all the detections is good (properties with high confidence, etc.)
    * All detections are set to have very low rdot comp
    */
   xvel_cloud = 0.0F;
   yvel_cloud = 2.0F;
   setup_moving_cluster_for_cloud_perfect_dets(2, 10, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);
   
   for (int32_t i = 0; i < 2; i++)
   {
      det_hist.det_data[i].rdot_comp = 0.01F;
   }

   for (int32_t i = 0; i < 10; i++)
   {
      det_props[i].range_rate_compensated = 0.01F;
      raw_detections.detections[i].processed.range_rate_compensated = 0.01F;
   }
   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_MED
    */
   CHECK_EQUAL(result, CONF3_MED);
}

/** \purpose
 * Tests whether the confidence of estimation is NOT reduced to CONF3_MED from HIGH due to calculated cross radial
 * velocity being not high enough (w.r.t. radial velocity) to trigger the reduction even with low rdotcomp detections
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, Cross_Rad_Vel_Est_Slow_Obj_No_Conf_Reduction_Vel) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * ypos_min = -1.5F
    * yposmin = 1.5F
    *
    * test specific:
    * xvel_cloud = 0.3F
    * yvel_cloud = 2.0F
    *
    * moving cluster set up with 2 current and 10 historical detections
    * quality of all the detections is good (properties with high confidence, etc.)
    * All detections are set to have very low rdot comp
    */
   xvel_cloud = 0.3F;
   yvel_cloud = 2.0F;
   setup_moving_cluster_for_cloud_perfect_dets(2, 10, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);
   
   for (int32_t i = 0; i < 2; i++)
   {
      det_hist.det_data[i].rdot_comp = 0.01F;
   }

   for (int32_t i = 0; i < 10; i++)
   {
      det_props[i].range_rate_compensated = 0.01F;
      raw_detections.detections[i].processed.range_rate_compensated = 0.01F;
   }
   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_HIGH
    */
   CHECK_EQUAL(result, CONF3_HIGH);
}

/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_NONE when the cluster is far from host and azimuth spread is below the higher threshold.
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, RangeCondition_AzimuthSpreadRequirement) {
   /** \precond
    * Cluster position set far from host (xpos_cloud = 200.0F, ypos_cloud = 0.0F)
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * ypos_min = -1.0F
    * ypos_max = 1.0F
    * moving cluster set up with 4 current and 6 historical detections
    * quality of all the detections is good (properties with high confidence, etc.)
    */
   xpos_cloud = 200.0F;
   ypos_cloud = 0.0F;
   xvel_cloud = 10.0F;
   yvel_cloud = 0.0F;
   ypos_min = -1.0F;
   ypos_max = 1.0F;
   setup_moving_cluster_for_cloud_perfect_dets(4, 6, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_NONE due to insufficient azimuth spread for far cluster
    */
   CHECK_EQUAL(result, CONF3_NONE);
}

/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_LOW when the total detection azimuth spread is high, but spread of inliers is not high.
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, WeakInlierSmallSpread){
   /** \precond
    * Preconditions from group setup:
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * 
    * 
    * Preconditions specific to this test:
    * xpos_cloud = 50.0F
    * ypos_min = -1.5F
    * ypos_max = 1.5F
    * moving cluster set up with 10 historic detections and one current outlier
    * quality of all the detections is good (properties with high confidence, etc.)
    * cluster.num_types_of_dets[1]=4 // ambiguous / staionary dets
    * cluster.num_types_of_dets[0]=7 // moving dets
    */
   xpos_cloud = 50.0F;
   ypos_min = -1.5F;
   ypos_max = 1.5F;
   setup_moving_cluster_for_cloud_perfect_dets(0, 10, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);
   add_detection_to_cluster(cluster, raw_detections, det_props, ypos_max + 1.0F, 15.0F);
   cluster.num_types_of_dets[1] = 4;
   cluster.num_types_of_dets[0] = 7;

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
       * Expected confidence CONF3_LOW small spread of inliers despite high total azimuth spread
    */
   CHECK_EQUAL(result, CONF3_LOW);
}

/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_MED (is decreased) when there is a big gap in the azimuths of detections.
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, WeakInlierGap){
   /** \precond
    * Preconditions from group setup:
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * ypos_min = -1.5F
    * ypos_max = 1.5F
    * 
    * 
    * Preconditions specific to this test:
    * xpos_cloud = 50.0F
    * moving cluster set up with 10 historic detections
    * quality of all the detections is good (properties with high confidence, etc.)
    * Detections fit perfectly to the object velocity, but there is a gap in azimuths of the detections
    */
   xpos_cloud = 50.0F;
   bool f_is_gap = true;
   setup_moving_cluster_for_cloud_perfect_dets(0, 15, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud, f_is_gap);

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
       * Expected confidence CONF3_MED
    */
   CHECK_EQUAL(result, CONF3_MED);
}

/** \purpose
 * Tests whether the confidence of estimation is set to CONF3_MED (is decreased) when cross-radial velocity component is significant, but azimuth spread is low
 *  - for that case, the azimuth threshold is increased.
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, CrossRadialLowSpread){
   /** \precond
    * Preconditions from group setup:
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * ypos_min = -1.5F
    * ypos_max = 1.5F
    * 
    * 
    * Preconditions specific to this test:
    * xpos_cloud = 50.0F
    * yvel_cloud = 12.0F
    * moving cluster set up with 10 historic detections and one current outlier
    * quality of all the detections is good (properties with high confidence, etc.)
    * Detections fit perfectly to the object velocity, but there is a gap in azimuths of the detections
    */
   xpos_cloud = 60.0F;
   yvel_cloud = 12.0F;
   setup_moving_cluster_for_cloud_perfect_dets(0, 10, cluster, raw_detections, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
       * Expected confidence CONF3_MED
    */
   CHECK_EQUAL(result, CONF3_MED);
}

/** \purpose
 * Tests whether the estimation result is CONF3_NONE when data is corrupted - cluster.vcs_az is not matched with detections azimuths,
 * which causes IRLS algorithm to fail (determinant < 0.01).
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_cloud, CorruptedData) {
   /** \precond
    * Preconditions from group setup:
    * xpos_cloud = 10.0F
    * ypos_cloud = 0.0F
    * xvel_cloud = 10.0F
    * yvel_cloud = 0.0F
    * ypos_min = -1.5F
    * ypos_max = 1.5F
    *
    * test specific:
    * cluster configured with 5 current detections having azimuths near -pi and +pi
    * azimuth spread is large, but cos/sin values are nearly collinear
    */
   raw_detections.number_of_valid_detections = 0;
   det_hist.n_occupied = 0;

   cluster.id = 1;
   cluster.vcs_position_x = xpos_cloud;
   cluster.vcs_position_y = ypos_cloud;
   cluster.rep_vcs_az = F360_Atan2f(ypos_cloud, xpos_cloud);
   cluster.cos_vcs_az = F360_Cosf(cluster.rep_vcs_az);
   cluster.sin_vcs_az = F360_Sinf(cluster.rep_vcs_az);
   cluster.ndets = 0;
   cluster.num_old_dets = 0;
   cluster.rep_rdotcomp = cluster.cos_vcs_az * xvel_cloud + cluster.sin_vcs_az * yvel_cloud;
   cluster.f_dealiased = true;
   cluster.num_types_of_dets[0] = 0;
   cluster.num_types_of_dets[1] = 0;

   const float32_t tiny_angle = 0.0001F;
   const float32_t azimuths[5] =
   {
      F360_PI - tiny_angle,
      -F360_PI + tiny_angle,
      F360_PI - tiny_angle,
      -F360_PI + tiny_angle,
      F360_PI - tiny_angle
   };

   for (int32_t i = 0; i < 5; i++)
   {
      const int32_t det_idx = raw_detections.number_of_valid_detections;
      raw_detections.number_of_valid_detections++;

      cluster.detids[cluster.ndets] = det_idx + 1;
      cluster.ndets++;

      raw_detections.detections[det_idx].raw.sensor_id = 1;
      raw_detections.detections[det_idx].raw.confid_azimuth = 0;
      raw_detections.detections[det_idx].raw.confid_elevation = 0;
      raw_detections.detections[det_idx].raw.f_super_res = false;
      raw_detections.detections[det_idx].raw.elevation = 0.0F;

      raw_detections.detections[det_idx].processed.vcs_az = azimuths[i];
      raw_detections.detections[det_idx].processed.cos_vcs_az = F360_Cosf(azimuths[i]);
      raw_detections.detections[det_idx].processed.sin_vcs_az = F360_Sinf(azimuths[i]);
      raw_detections.detections[det_idx].processed.vcs_position_x = xpos_cloud;
      raw_detections.detections[det_idx].processed.vcs_position_y = ypos_cloud;
      raw_detections.detections[det_idx].processed.range_rate_compensated = 0.0F;

      det_props[det_idx].vcs_position.x = raw_detections.detections[det_idx].processed.vcs_position_x;
      det_props[det_idx].vcs_position.y = raw_detections.detections[det_idx].processed.vcs_position_y;
      det_props[det_idx].range_rate_compensated = 0.0F;
      det_props[det_idx].f_FOV_edge = false;
   }

   longvel_by_cloud = 0.0F;
   latvel_by_cloud = 0.0F;

   /** \action
    * call Estimate_Velocity_By_Cloud()
    */
   CONF3_T result = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, longvel_by_cloud, latvel_by_cloud);

   /** \result
    * Expected confidence CONF3_NONE due to determinant below threshold
    */
   CHECK_EQUAL(result, CONF3_NONE);
}
/** @}*/

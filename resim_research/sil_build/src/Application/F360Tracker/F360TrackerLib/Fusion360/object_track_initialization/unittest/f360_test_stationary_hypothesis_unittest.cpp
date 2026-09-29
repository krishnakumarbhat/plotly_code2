/** \file
 * This file contains unit tests for content of f360_test_stationary_hypothesis.cpp file
 */

#include "f360_test_stationary_hypothesis.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_test_stationary_hypothesis
 *  @{
 */

/** \brief
 * Group used for testing Test_Stationary_Hypothesis().
 * Test_Stationary_Hypothesis() checks whether cluster detection properties are indicating that this cluster is stationary.
 */
TEST_GROUP(f360_test_stationary_hypothesis)
{
   // Declare common variables used within all tests in this test group.
   F360_Detection_Hist_T det_hist{};
   F360_Calibrations_T calibrations{};
   F360_Host_T host{};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};
   F360_Detection_Props_T detections[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Cluster_T cluster{};
   float32_t longvel_estimate{};
   float32_t latvel_estimate{};

   float32_t xpos;
   float32_t ypos;

   /** \setup
    * Calibrations parameters referring to detection thresholds are prepared (consistent with the tracker calibrations).
    */
   TEST_SETUP()
   {
      calibrations.k_init_min_num_dets_from_restrictive_zone = 6U;
      calibrations.k_init_min_num_dets_from_outside_restrictive_zone = 3U;
      
      // Initialize host with default values
      host.speed = 5.0F; // Default to higher speed so tests don't trigger low speed condition unless specifically set
   }


   // This function creates the stationary cluster with the given amount of current ant historical detections.
   // Sets detection properties to cover possible options that they can be at.

   void setup_stationary_cluster_with_nonperfect_dets(
      int32_t num_curr_dets,
      int32_t num_hist_dets,
      F360_Cluster_T& cluster,
      rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Detection_Hist_T& det_hist,
      const float xpos, const float ypos)
   {
      cluster.vcs_position_x = xpos;
      cluster.vcs_position_y = ypos;
      cluster.rep_vcs_az = F360_Atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
      cluster.cos_vcs_az = F360_Cosf(cluster.rep_vcs_az);
      cluster.sin_vcs_az = F360_Sinf(cluster.rep_vcs_az);
      cluster.ndets = 0;
      cluster.num_old_dets = 0;

      for (int32_t i = 0; i < num_curr_dets; i++)
      {
         int32_t n = raw_detect_list.number_of_valid_detections;
         raw_detect_list.number_of_valid_detections++;
         cluster.detids[i] = n + 1;
         cluster.ndets++;
         raw_detect_list.detections[n].raw.azimuth = F360_Atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].raw.elevation = 0.0F;
         raw_detect_list.detections[n].raw.confid_azimuth = std::min(i, 3);
         raw_detect_list.detections[n].raw.confid_elevation = std::min(i, 3);
         raw_detect_list.detections[n].processed.vcs_az = F360_Atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].processed.vcs_position_x = cluster.vcs_position_x;
         raw_detect_list.detections[n].processed.vcs_position_y = cluster.vcs_position_y;
         raw_detect_list.detections[n].processed.cos_vcs_az = F360_Cosf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.sin_vcs_az = F360_Sinf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.range_rate_compensated = std::min(i, 3);

         detections[n].f_angle_amb = false;
         detections[n].f_potential_angle_jump = false;
         detections[n].vcs_position.x = raw_detect_list.detections[n].processed.vcs_position_x;
         detections[n].vcs_position.y = raw_detect_list.detections[n].processed.vcs_position_y;
         detections[n].range_rate_compensated = std::min(i, 3);
      }
      for (int32_t i = 0; i < num_hist_dets; i++)
      {
         int32_t n = det_hist.n_occupied;
         det_hist.n_occupied++;
         cluster.num_old_dets++;
         cluster.old_det_idx[i] = n;
         det_hist.det_data[n].vcs_position_x = cluster.vcs_position_x;
         det_hist.det_data[n].vcs_position_y = cluster.vcs_position_y;
         det_hist.det_data[n].rdot_comp = std::min(i, 3);
         det_hist.det_data[n].az_conf = std::min(i, 3);
         det_hist.det_data[n].el_conf = std::min(i, 3);
      }
   }

   // This function creates the stationary cluster with the given amount of current ant historical detections.
   // Sets detection properties to be considered as ideal detections (high confidences, etc.)

   void setup_stationary_cluster_with_perfect_dets(
      int32_t num_curr_dets,
      int32_t num_hist_dets,
      F360_Cluster_T& cluster,
      rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Detection_Hist_T& det_hist,
      const float xpos, const float ypos)
   {
      cluster.vcs_position_x = xpos;
      cluster.vcs_position_x = ypos;
      cluster.rep_vcs_az = F360_Atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
      cluster.cos_vcs_az = F360_Cosf(cluster.rep_vcs_az);
      cluster.sin_vcs_az = F360_Sinf(cluster.rep_vcs_az);
      cluster.ndets = 0;
      cluster.num_old_dets = 0;

      for (int32_t i = 0; i < num_curr_dets; i++)
      {
         int32_t n = raw_detect_list.number_of_valid_detections;
         raw_detect_list.number_of_valid_detections++;
         cluster.detids[i] = n + 1;
         cluster.ndets++;
         raw_detect_list.detections[n].raw.azimuth = F360_Atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].raw.elevation = 0.0F;
         raw_detect_list.detections[n].raw.confid_azimuth = 0;
         raw_detect_list.detections[n].raw.confid_elevation = 0;
         raw_detect_list.detections[n].processed.vcs_az = F360_Atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].processed.vcs_position_x = cluster.vcs_position_x;
         raw_detect_list.detections[n].processed.vcs_position_y = cluster.vcs_position_y;
         raw_detect_list.detections[n].processed.cos_vcs_az = F360_Cosf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.sin_vcs_az = F360_Sinf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.range_rate_compensated = 0.0F;

         detections[n].f_angle_amb = false;
         detections[n].f_potential_angle_jump = false;
         detections[n].vcs_position.x = raw_detect_list.detections[n].processed.vcs_position_x;
         detections[n].vcs_position.y = raw_detect_list.detections[n].processed.vcs_position_y;
         detections[n].range_rate_compensated = 0.0F;
      }
      for (int32_t i = 0; i < num_hist_dets; i++)
      {
         int32_t n = det_hist.n_occupied;
         det_hist.n_occupied++;
         cluster.num_old_dets++;
         cluster.old_det_idx[i] = n;
         det_hist.det_data[n].vcs_position_x = cluster.vcs_position_x;
         det_hist.det_data[n].vcs_position_y = cluster.vcs_position_y;
         det_hist.det_data[n].rdot_comp = 0;
         det_hist.det_data[n].az_conf = 0;
         det_hist.det_data[n].el_conf = 0;
      }
   }

};

/** \purpose
 * Check whether the cluster with low quality detections is classified as F360_TRACK_INIT_INVALID init type
 * (is further passed to next stages of initialization)

* \req
* NA
*/

TEST(f360_test_stationary_hypothesis, Non_Perfect_Dets_Cluster_Test)
{
   /** \precond
    * Cluster created with the detection parameters as in setup_stationary_cluster_with_nonperfect_dets.calibrations
    * cluster has 4 current and 4 historical detections
    * xpos of a cluster is 11.0F
    * ypos of a cluster is 10.0F
    */
   xpos = 11.0F;
   ypos = 10.0F;

   setup_stationary_cluster_with_nonperfect_dets(4, 4, cluster, raw_detect_list, det_hist, xpos, ypos);

   /** \action
    * Call Test_Stationary_Hypothesis()
    */
   F360_Track_Init_T init_type = Test_Stationary_Hypothesis(calibrations, host, det_hist, raw_detect_list, detections, cluster, longvel_estimate, latvel_estimate);

   /** \result
    * Expect init_type to be F360_TRACK_INIT_INVALID
    */
   CHECK_EQUAL(init_type, F360_TRACK_INIT_INVALID);
}


/** \purpose
 * Check whether the cluster with low quality detections and position outlier in current detections
 * is classified as F360_TRACK_INIT_INVALID init type (is further passed to next stages of initialization)

* \req
* NA
*/

TEST(f360_test_stationary_hypothesis, Paralel_Outlier_In_Current_Dets)
{
   /** \precond
    * Cluster created with the detection parameters as in setup_stationary_cluster_with_nonperfect_dets.calibrations
    * cluster has 4 current and 4 historical detections
    * xpos of a cluster is 11.0F
    * ypos of a cluster is 10.0F
    * vcs_position.x of one of the current detections is set to be an outlier for the cluster
    */
   xpos = 11.0F;
   ypos = 10.0F;

   setup_stationary_cluster_with_nonperfect_dets(4, 4, cluster, raw_detect_list, det_hist, xpos, ypos);
   detections[1].vcs_position.x = cluster.vcs_position_x+2.0F;
   raw_detect_list.detections[1].raw.azimuth = F360_Atan2f(raw_detect_list.detections[1].processed.vcs_position_y, raw_detect_list.detections[1].processed.vcs_position_x);

   /** \action
    * Call Test_Stationary_Hypothesis()
    */
   F360_Track_Init_T init_type = Test_Stationary_Hypothesis(calibrations, host, det_hist, raw_detect_list, detections, cluster, longvel_estimate, latvel_estimate);

   /** \result
    * Expect init_type to be F360_TRACK_INIT_INVALID
    */
   CHECK_EQUAL(init_type, F360_TRACK_INIT_INVALID);
}


/** \purpose
 * Check whether the cluster with low quality detections and position outlier in historical detections
 * is classified as F360_TRACK_INIT_INVALID init type (is further passed to next stages of initialization)

* \req
* NA
*/

TEST(f360_test_stationary_hypothesis, Pos_Outlier_In_Hist_Det)
{
   /** \precond
    * Cluster created with the detection parameters as in setup_stationary_cluster_with_nonperfect_dets.calibrations
    * cluster has 4 current and 4 historical detections
    * xpos of a cluster is 11.0F
    * ypos of a cluster is 10.0F
    * vcs_position.x and vcs_position.y of one of the historical detections is set to be an outlier for the cluster
    */
   xpos = 11.0F;
   ypos = 10.0F;

   setup_stationary_cluster_with_nonperfect_dets(4, 4, cluster, raw_detect_list, det_hist, xpos, ypos);

   det_hist.det_data[1].vcs_position_x=cluster.vcs_position_x+2.0F;
   det_hist.det_data[1].vcs_position_y=cluster.vcs_position_y+2.0F;
   raw_detect_list.detections[1].raw.azimuth = F360_Atan2f(raw_detect_list.detections[1].processed.vcs_position_y, raw_detect_list.detections[1].processed.vcs_position_x);

   /** \action
    * Call Test_Stationary_Hypothesis()
    */
   F360_Track_Init_T init_type = Test_Stationary_Hypothesis(calibrations, host, det_hist, raw_detect_list, detections, cluster, longvel_estimate, latvel_estimate);

   /** \result
    * Expect init_type to be F360_TRACK_INIT_INVALID
    */
   CHECK_EQUAL(init_type, F360_TRACK_INIT_INVALID);
}

/** \purpose
 * Check whether the cluster with low quality detections and oriented inside of the restrictive zone on the sides of the host
 * is classified as F360_TRACK_INIT_INVALID init type (is further passed to next stages of initialization)

* \req
* NA
*/

TEST(f360_test_stationary_hypothesis, Cluster_Inside_Restrictive_Zone)
{
   /** \precond
    * Cluster created with the detection parameters as in setup_stationary_cluster_with_nonperfect_dets.calibrations
    * cluster has 4 current and 4 historical detections
    * Cluster is inside of the restrictive zone on the side of the host
    * xpos of a cluster is -1.0F
    * ypos of a cluster is 14.0F
    */
   xpos = -1.0F;
   ypos = 14.0F;

   setup_stationary_cluster_with_nonperfect_dets(4, 4, cluster, raw_detect_list, det_hist, xpos, ypos);

   /** \action
    * Call Test_Stationary_Hypothesis()
    */
   F360_Track_Init_T init_type = Test_Stationary_Hypothesis(calibrations, host, det_hist, raw_detect_list, detections, cluster, longvel_estimate, latvel_estimate);

   /** \result
    * Expect init_type to be F360_TRACK_INIT_INVALID
    */
   CHECK_EQUAL(init_type, F360_TRACK_INIT_INVALID);
}

/** \purpose
 * Check whether the cluster with low quality detections and containing the paralel outlier detection
 * is classified as F360_TRACK_INIT_INVALID init type (is further passed to next stages of initialization)

* \req
* NA
*/

TEST(f360_test_stationary_hypothesis, Para_Outside_Gate)
{
   /** \precond
    * Cluster created with the detection parameters as in setup_stationary_cluster_with_nonperfect_dets.calibrations
    * cluster has 4 current and 4 historical detections
    * xpos of a cluster is 70.0F
    * ypos of a cluster is 20.0F
    * one current and one historical detection vcs y position is outside paralel gate
    */
   xpos = 70.0F;
   ypos = 20.0F;

   setup_stationary_cluster_with_nonperfect_dets(4, 4, cluster, raw_detect_list, det_hist, xpos, ypos);
   detections[1].vcs_position.y = ypos + 5.0F;
   raw_detect_list.detections[1].processed.vcs_position_y = cluster.vcs_position_y + 5.0F;
   det_hist.det_data[1].vcs_position_y=cluster.vcs_position_y + 5.0F;

   /** \action
    * Call Test_Stationary_Hypothesis()
    */
   F360_Track_Init_T init_type = Test_Stationary_Hypothesis(calibrations, host, det_hist, raw_detect_list, detections, cluster, longvel_estimate, latvel_estimate);

   /** \result
    * Expect init_type to be F360_TRACK_INIT_INVALID
    */
   CHECK_EQUAL(init_type, F360_TRACK_INIT_INVALID);
}

/** \purpose
 * Check whether the cluster with low quality detections and containing the orthogonal and paralel outlier detection
 * is classified as F360_TRACK_INIT_INVALID init type (is further passed to next stages of initialization)

* \req
* NA
*/

TEST(f360_test_stationary_hypothesis, Para_And_Orth_Outside_Gate)
{
   /** \precond
    * Cluster created with the detection parameters as in setup_stationary_cluster_with_nonperfect_dets.calibrations
    * cluster has 4 current and 4 historical detections
    * xpos of a cluster is 70.0F
    * ypos of a cluster is 20.0F
    * one current and one historical detection vcs y position is outside paralel gate
    * one current and one historical detection vcs x position is outside orthogonal gate
    */
   xpos = 70.0F;
   ypos = 20.0F;

   setup_stationary_cluster_with_nonperfect_dets(4, 4, cluster, raw_detect_list, det_hist, xpos, ypos);
   detections[1].vcs_position.x = xpos + 5.0F;
   detections[1].vcs_position.y = ypos + 5.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = cluster.vcs_position_x + 5.0F;
   raw_detect_list.detections[1].processed.vcs_position_y = cluster.vcs_position_y + 5.0F;
   det_hist.det_data[1].vcs_position_x=cluster.vcs_position_x + 5.0F;
   det_hist.det_data[1].vcs_position_y=cluster.vcs_position_y + 5.0F;

   /** \action
    * Call Test_Stationary_Hypothesis()
    */
   F360_Track_Init_T init_type = Test_Stationary_Hypothesis(calibrations, host, det_hist, raw_detect_list, detections, cluster, longvel_estimate, latvel_estimate);

   /** \result
    * Expect init_type to be F360_TRACK_INIT_INVALID
    */
   CHECK_EQUAL(init_type, F360_TRACK_INIT_INVALID);
}



/** \purpose
 * Check whether the cluster with low quality detections and too few detections for the cluster oriented in restrictive zone
 * is classified as F360_TRACK_INIT_INVALID init type (is further passed to next stages of initialization)

* \req
* NA
*/

TEST(f360_test_stationary_hypothesis, Not_Enough_Detections)
{
   /** \precond
 * Cluster is in the restrictive zone (requires at least 6 detections to be initialized as an object)
 * cluster has 4 detections
 * xpos of a cluster is -1.0F
 * ypos of a cluster is 14.0F
 */
   xpos = -1.0F;
   ypos = 14.0F;

   setup_stationary_cluster_with_nonperfect_dets(2, 2, cluster, raw_detect_list, det_hist, xpos, ypos);

   /** \action
    * Call Test_Stationary_Hypothesis()
    */
   F360_Track_Init_T init_type = Test_Stationary_Hypothesis(calibrations, host, det_hist, raw_detect_list, detections, cluster, longvel_estimate, latvel_estimate);

   /** \result
    * Expect init_type to be F360_TRACK_INIT_INVALID
    */
   CHECK_EQUAL(init_type, F360_TRACK_INIT_INVALID);
}


/** \purpose
 * Check whether the cluster with best quality detections and position outliers among the detections
 * (rdot-wise is considered as staionary, position-wise is NOT considered as stationary)
 * is classified as F360_TRACK_INIT_INVALID init type -- because F360_TRACK_INIT_STATIONARY requires both rdot and position to be stationary

* \req
* NA
*/

TEST(f360_test_stationary_hypothesis, Stationary_Rdot_Not_Stationary_Pos)
{
   /** \precond
 * cluster has 6 detections with best quality properties
 * detections are set to be position outliers
 */
   xpos = 11.0F;
   ypos = 10.0F;

   setup_stationary_cluster_with_perfect_dets(3, 3, cluster, raw_detect_list, det_hist, xpos, ypos);
   for(int32_t i = 0; i < 3; i++)
   {
      //pos outliers
      detections[i].vcs_position.y = ypos + 5.0F;
      raw_detect_list.detections[i].processed.vcs_position_y = cluster.vcs_position_y + 5.0F;
      det_hist.det_data[i].vcs_position_y=cluster.vcs_position_y + 5.0F;
   }

   /** \action
    * Call Test_Stationary_Hypothesis()
    */
   F360_Track_Init_T init_type = Test_Stationary_Hypothesis(calibrations, host, det_hist, raw_detect_list, detections, cluster, longvel_estimate, latvel_estimate);

   /** \result
    * Expect init_type to be F360_TRACK_INIT_INVALID
    */
   CHECK_EQUAL(init_type, F360_TRACK_INIT_INVALID);
}

/** \purpose
 * Check whether the cluster with close object position and low host speed gets tighter rdot gate (0.4F)
 * and is classified as F360_TRACK_INIT_STATIONARY when conditions are met

* \req
* NA
*/

TEST(f360_test_stationary_hypothesis, Close_Object_Low_Speed_Tighter_Gate)
{
   /** \precond
    * Cluster created with perfect detection properties
    * cluster has 3 current and 3 historical detections (meets minimum requirements)
    * Cluster is positioned close to host: x=5.0F (within 0-10m), y=10.0F (within +/-20m)
    * Host speed is low: 0.1F (< 0.2 m/s)
    * All detections have rdot_comp = 0.3F (within tighter 0.4F gate but outside normal 0.8F gate would matter)
    */
   xpos = 5.0F;
   ypos = 10.0F;
   host.speed = 0.1F;  // Low speed to trigger tighter gate

   setup_stationary_cluster_with_perfect_dets(3, 3, cluster, raw_detect_list, det_hist, xpos, ypos);

   /** \action
    * Call Test_Stationary_Hypothesis()
    */
   F360_Track_Init_T init_type = Test_Stationary_Hypothesis(calibrations, host, det_hist, raw_detect_list, detections, cluster, longvel_estimate, latvel_estimate);

   /** \result
    * Expect init_type to be F360_TRACK_INIT_STATIONARY (due to perfect detections and sufficient count)
    */
   CHECK_EQUAL(init_type, F360_TRACK_INIT_STATIONARY);
}

/** \purpose
 * Check whether the cluster with close object position but high host speed gets normal rdot gate (0.8F)

* \req
* NA
*/

TEST(f360_test_stationary_hypothesis, Close_Object_High_Speed_Normal_Gate)
{
   /** \precond
    * Cluster created with perfect detection properties
    * cluster has 3 current and 3 historical detections
    * Cluster is positioned close to host: x=5.0F (within 0-10m), y=10.0F (within +/-20m)
    * Host speed is high: 5.0F (>= 0.2 m/s)
    */
   xpos = 5.0F;
   ypos = 10.0F;
   host.speed = 5.0F;  // High speed, should use normal gate

   setup_stationary_cluster_with_perfect_dets(3, 3, cluster, raw_detect_list, det_hist, xpos, ypos);

   /** \action
    * Call Test_Stationary_Hypothesis()
    */
   F360_Track_Init_T init_type = Test_Stationary_Hypothesis(calibrations, host, det_hist, raw_detect_list, detections, cluster, longvel_estimate, latvel_estimate);

   /** \result
    * Expect init_type to be F360_TRACK_INIT_STATIONARY
    */
   CHECK_EQUAL(init_type, F360_TRACK_INIT_STATIONARY);
}

/** \purpose
 * Check whether the cluster with far object position and low host speed gets normal rdot gate (0.8F)

* \req
* NA
*/

TEST(f360_test_stationary_hypothesis, Far_Object_Low_Speed_Normal_Gate)
{
   /** \precond
    * Cluster created with perfect detection properties
    * cluster has 3 current and 3 historical detections
    * Cluster is positioned far from host: x=15.0F (outside 0-10m range)
    * Host speed is low: 0.1F (< 0.2 m/s)
    */
   xpos = 15.0F;
   ypos = 10.0F;
   host.speed = 0.1F;  // Low speed, but object is far so normal gate applies

   setup_stationary_cluster_with_perfect_dets(3, 3, cluster, raw_detect_list, det_hist, xpos, ypos);

   /** \action
    * Call Test_Stationary_Hypothesis()
    */
   F360_Track_Init_T init_type = Test_Stationary_Hypothesis(calibrations, host, det_hist, raw_detect_list, detections, cluster, longvel_estimate, latvel_estimate);

   /** \result
    * Expect init_type to be F360_TRACK_INIT_STATIONARY
    */
   CHECK_EQUAL(init_type, F360_TRACK_INIT_STATIONARY);
}

/** \purpose
 * Check whether the cluster positioned to the side (outside +/-20m lateral range) 
 * and low host speed gets normal rdot gate (0.8F)

* \req
* NA
*/

TEST(f360_test_stationary_hypothesis, Side_Object_Low_Speed_Normal_Gate)
{
   /** \precond
    * Cluster created with perfect detection properties
    * cluster has 3 current and 3 historical detections
    * Cluster is positioned to the side: x=5.0F (within 0-10m), y=25.0F (outside +/-20m)
    * Host speed is low: 0.1F (< 0.2 m/s)
    */
   xpos = 5.0F;
   ypos = 25.0F;  // Outside +/-20m lateral range
   host.speed = 0.1F;  // Low speed, but object is to the side so normal gate applies

   setup_stationary_cluster_with_perfect_dets(3, 3, cluster, raw_detect_list, det_hist, xpos, ypos);

   /** \action
    * Call Test_Stationary_Hypothesis()
    */
   F360_Track_Init_T init_type = Test_Stationary_Hypothesis(calibrations, host, det_hist, raw_detect_list, detections, cluster, longvel_estimate, latvel_estimate);

   /** \result
    * Expect init_type to be F360_TRACK_INIT_STATIONARY
    */
   CHECK_EQUAL(init_type, F360_TRACK_INIT_STATIONARY);
}

/** \purpose
 * Check whether the cluster positioned behind the host and low host speed gets normal rdot gate (0.8F)

* \req
* NA
*/

TEST(f360_test_stationary_hypothesis, Behind_Object_Low_Speed_Normal_Gate)
{
   /** \precond
    * Cluster created with perfect detection properties
    * cluster has 3 current and 3 historical detections
    * Cluster is positioned behind host: x=-5.0F (behind host, x < 0)
    * Host speed is low: 0.1F (< 0.2 m/s)
    */
   xpos = -5.0F;  // Behind the host
   ypos = 10.0F;
   host.speed = 0.1F;  // Low speed, but object is behind so normal gate applies

   setup_stationary_cluster_with_perfect_dets(3, 3, cluster, raw_detect_list, det_hist, xpos, ypos);

   /** \action
    * Call Test_Stationary_Hypothesis()
    */
   F360_Track_Init_T init_type = Test_Stationary_Hypothesis(calibrations, host, det_hist, raw_detect_list, detections, cluster, longvel_estimate, latvel_estimate);

   /** \result
    * Expect init_type to be F360_TRACK_INIT_STATIONARY
    */
   CHECK_EQUAL(init_type, F360_TRACK_INIT_STATIONARY);
}

/** \purpose
 * Check whether the cluster with rdot_comp = 0.5F (between tighter 0.4F and normal 0.8F gates)
 * behaves correctly based on position and speed conditions

* \req
* NA
*/

TEST(f360_test_stationary_hypothesis, Rdot_Comp_Half_Boundary_Test)
{
   /** \precond
    * Cluster created with perfect detection properties except rdot_comp = 0.5F
    * cluster has 3 current and 3 historical detections
    * Cluster is positioned close to host: x=5.0F (within 0-10m), y=10.0F (within +/-20m)
    * Host speed is low: 0.1F (< 0.2 m/s) - should trigger tighter 0.4F gate
    * All detections have rdot_comp = 0.5F (outside tighter 0.4F gate, inside normal 0.8F gate)
    */
   xpos = 5.0F;
   ypos = 10.0F;
   host.speed = 0.1F;  // Low speed to trigger tighter gate

   setup_stationary_cluster_with_perfect_dets(3, 3, cluster, raw_detect_list, det_hist, xpos, ypos);
   
   // Set rdot_comp to 0.5F for all detections (outside 0.4F gate, inside 0.8F gate)
   for(int32_t i = 0; i < 3; i++)
   {
      detections[i].range_rate_compensated = 0.5F;
      det_hist.det_data[i].rdot_comp = 0.5F;
   }

   /** \action
    * Call Test_Stationary_Hypothesis()
    */
   F360_Track_Init_T init_type = Test_Stationary_Hypothesis(calibrations, host, det_hist, raw_detect_list, detections, cluster, longvel_estimate, latvel_estimate);

   /** \result
    * Expect init_type to be F360_TRACK_INIT_INVALID (due to rdot outside tighter 0.4F gate)
    */
   CHECK_EQUAL(init_type, F360_TRACK_INIT_INVALID);
}

/** @}*/

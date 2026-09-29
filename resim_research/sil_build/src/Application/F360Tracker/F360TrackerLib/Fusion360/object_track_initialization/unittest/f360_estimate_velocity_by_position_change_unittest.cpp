/** \file
 * This file contains unit tests for content of f360_estimate_velocity_by_position_change.cpp file
 */

#include "f360_estimate_velocity_by_position_change.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_estimate_velocity_by_position_change
 *  @{
 */

/** \brief
 * Testing the functionality of estimation of the velocity by position change- velocity estimates as well as confidence level.
 */
TEST_GROUP(f360_estimate_velocity_by_position_change)
{
   // Declare common variables used within all tests in this test group.

   F360_Detection_Hist_T det_hist;
   rspp_variant_A::RSPP_Detection_List_T raw_detections;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];
   F360_Cluster_T cluster;

   float32_t longvel_by_position;
   float32_t latvel_by_position;

   float32_t xpos;
   float32_t ypos;
   float32_t xvel;
   float32_t yvel;


   /** \setup
    * Setting up two sensors time_since_measurement
    * setting x, y positions of the input cluster
    * setting x, y velocities of the input cluster
    */
   TEST_SETUP()
   {
      sensors[0].refined.time_since_measurement_s = 0.00F;
      sensors[1].refined.time_since_measurement_s = 0.00F;

      xpos = 50.0F;
      ypos = 2.0F;
      xvel = -2.0F;
      yvel = 0.0F;
   }

   // Define helper functions used in this test group here. E.g. Add_Detection_To_Detection_List().

   /*  setting up the moving cluster:
   * - 22 detections- 20 historical, 2 current,
   * - no angle jumps, no angle ambiguity,
   * - high azimuth confidence
   * - high elevation confidence
   */

   void setup_moving_cluster(
      F360_Cluster_T& cluster,
      rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Detection_Hist_T& det_hist,
      const float xpos, const float ypos,
      const float xvel, const float yvel, const bool rdot_invalid)
   {
      cluster.vcs_position_x = xpos;
      cluster.vcs_position_y = ypos;
      cluster.rep_vcs_az = F360_Atan2f(ypos, xpos);
      cluster.cos_vcs_az = F360_Cosf(cluster.rep_vcs_az);
      cluster.sin_vcs_az = F360_Sinf(cluster.rep_vcs_az);
      cluster.ndets = 0;
      cluster.num_old_dets = 0;
      cluster.rep_rdotcomp = cluster.cos_vcs_az * xvel + cluster.sin_vcs_az * yvel;
      cluster.f_dealiased = true;

      for (int32_t i = 0; i < 2; i++)
      {
         int32_t n = raw_detect_list.number_of_valid_detections;
         raw_detect_list.number_of_valid_detections++;
         cluster.detids[i] = n + 1;
         cluster.ndets++;
         raw_detect_list.detections[n].raw.sensor_id = 1;
         raw_detect_list.detections[n].raw.azimuth = F360_Atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].raw.elevation = 0.0F;
         raw_detect_list.detections[n].raw.confid_azimuth = 0;
         raw_detect_list.detections[n].raw.confid_elevation = 0;
         raw_detect_list.detections[n].processed.vcs_az = F360_Atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].processed.vcs_position_x = cluster.vcs_position_x;
         raw_detect_list.detections[n].processed.vcs_position_y = cluster.vcs_position_y;
         raw_detect_list.detections[n].processed.cos_vcs_az = F360_Cosf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.sin_vcs_az = F360_Sinf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.range_rate_compensated = cluster.rep_rdotcomp;

         det_props[n].f_angle_amb = false;
         det_props[n].f_potential_angle_jump = false;
         det_props[n].vcs_position.x = raw_detect_list.detections[n].processed.vcs_position_x;
         det_props[n].vcs_position.y = raw_detect_list.detections[n].processed.vcs_position_y;
      }


      for (int32_t i = 0; i < 10; i++)
      {
         int32_t i1 = i*2;
         int32_t i2 = i*2+1;
         float32_t T = 0.05F;

         cluster.num_old_dets++;
         int32_t n = det_hist.n_occupied;
         cluster.old_det_idx[i1] = n;
         det_hist.n_occupied++;
         det_hist.det_data[n].sensor_id = 1;
         float32_t rdot_multiplier = rdot_invalid ? 2.5F: 1.0F;
         det_hist.det_data[n].vcs_position_x = cluster.vcs_position_x - T * (i + 1) * rdot_multiplier * xvel;
         det_hist.det_data[n].vcs_position_y = cluster.vcs_position_y - T * (i + 1) * rdot_multiplier * yvel;
         det_hist.det_data[n].vcs_az = F360_Atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
         det_hist.det_data[n].rdot_comp = F360_Cosf(det_hist.det_data[n].vcs_az) * xvel + F360_Sinf(det_hist.det_data[n].vcs_az) * yvel;
         det_hist.det_data[n].time_since_meas = (i + 1) * T;

         cluster.num_old_dets++;
         n = det_hist.n_occupied;
         cluster.old_det_idx[i2] = n;
         det_hist.n_occupied++;
         det_hist.det_data[n].sensor_id = 1;
         det_hist.det_data[n].vcs_position_x = cluster.vcs_position_x - T * (i + 1) * rdot_multiplier * xvel;
         det_hist.det_data[n].vcs_position_y = cluster.vcs_position_y - T * (i + 1) * rdot_multiplier * yvel;
         det_hist.det_data[n].vcs_az = F360_Atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
         det_hist.det_data[n].rdot_comp = F360_Cosf(det_hist.det_data[n].vcs_az) * xvel + F360_Sinf(det_hist.det_data[n].vcs_az) * yvel;
         det_hist.det_data[n].time_since_meas = (i + 1) * T;
      }
      cluster.num_types_of_dets[0] = cluster.num_old_dets + cluster.ndets;
   }
};

/** \purpose
 * Check whether the method returns CONF3_MED confidence when some of the old_dets positions are skewed.
 * (causing the weighted_inlier_ratio_long to be lowered)
 *
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_position_change, Weighted_Inlier_Ratio_Long_Below_Min_Inlier_Threshold_For_High_Conf)
{
   /** \precond
    * Preconditions from setup, additionally:
    * set up the cluster without range rate modifier
    * majority (16) of the old detections from the cluster have the vcx x position shifted
    * (azimuth and rdot_comp recalculated accordingly)
    */
   setup_moving_cluster(cluster, raw_detections, det_hist, xpos, ypos, xvel, yvel, false);

   // skew some detections to lower the weighted_inlier_ratio_long
   for (int32_t i = 0; i < cluster.num_old_dets-4; i++)
   {
      det_hist.det_data[i].vcs_position_x -= 1.8F;
      det_hist.det_data[i].vcs_az = F360_Atan2f(det_hist.det_data[i].vcs_position_y, det_hist.det_data[i].vcs_position_x);
      det_hist.det_data[i].rdot_comp = F360_Cosf(det_hist.det_data[i].vcs_az) * xvel + F360_Sinf(det_hist.det_data[i].vcs_az) * yvel;
   }

   /** \action
    * Call Estimate_Velocity_By_Position_Change().
    */
   CONF3_T result = Estimate_Velocity_By_Position_Change(sensors, det_hist, raw_detections, det_props, cluster, longvel_by_position, latvel_by_position);

   /** \result
    * CONF3_MED confidence
    */
   CHECK_TEXT(CONF3_MED == result, "Estimation confidence not MED as expected");
}


/** \purpose
 * Check whether the method returns correct estimation confidence and velocity estimates
 * when the detections are split between two sensors and meet the inlier ratio requirement for high confidence,
 * but don't satisfy it in terms of the amount of detections (not enough confirmed timesamps and not enough
 * detections from a single sensor)
 *
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_position_change, Detections_Split_Between_Two_Sensors_High_Conf_sum_confirmed_ts_Below_Threshold_num_confirmed_single_sensor_ts_Below_Threshold)
{
   /** \precond
    * Preconditions from setup, additionally:
    * set up the cluster without range rate modifier
    * truncate historical detections to 12
    * for every other current and historical detection set the sensor id to 2
    */
   setup_moving_cluster(cluster, raw_detections, det_hist, xpos, ypos, xvel, yvel, false);
   cluster.num_old_dets = 12;

   for (int32_t i = 0; i < cluster.ndets; i++)
   {
      raw_detections.detections[i].raw.sensor_id= 1 + static_cast<uint8_t>(i%2);
   }
   for (int32_t i = 0; i < cluster.num_old_dets; i++)
   {
      det_hist.det_data[i].sensor_id = 1 + static_cast<uint8_t>(i%2);
   }

   /** \action
    * Call Estimate_Velocity_By_Position_Change().
    */
   CONF3_T result = Estimate_Velocity_By_Position_Change(sensors, det_hist, raw_detections, det_props, cluster, longvel_by_position, latvel_by_position);

   /** \result
    * CONF3_MED confidence
    * velocity estimate consistent with input cluster speed
    */
   CHECK_TEXT(CONF3_MED == result, "Estimation confidence not MED as expected");
   DOUBLES_EQUAL(xvel, longvel_by_position, 0.001F);
   DOUBLES_EQUAL(yvel, latvel_by_position, 0.001F);
}


/** \purpose
 * Check whether the method returns correct estimation confidence and velocity estimates
 * when the detections are split between two sensors and meet the inlier ratio requirement for high confidence and
 * satisfy it in terms of the amount of detections (enough confirmed timestamps overall, but not enough comming
 * from a single sensor)
 *
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_position_change, Detections_Split_Between_Two_Sensors_High_Conf_sum_confirmed_ts_Above_Threshold_num_confirmed_single_sensor_ts_Below_Threshold)
{
   /** \precond
    * Preconditions from setup, additionally:
    * set up the cluster without range rate modifier
    * for every other current and historical detection set the sensor id to 2
    */
   setup_moving_cluster(cluster, raw_detections, det_hist, xpos, ypos, xvel, yvel, false);
   for (int32_t i = 0; i < cluster.ndets; i++)
   {
      raw_detections.detections[i].raw.sensor_id= 1 + static_cast<uint8_t>(i%2);
   }
   for (int32_t i = 0; i < cluster.num_old_dets; i++)
   {
      det_hist.det_data[i].sensor_id = 1 + static_cast<uint8_t>(i%2);
   }
   /** \action
    * Call Estimate_Velocity_By_Position_Change().
    */
   CONF3_T result = Estimate_Velocity_By_Position_Change(sensors, det_hist, raw_detections, det_props, cluster, longvel_by_position, latvel_by_position);

   /** \result
    * CONF3_MED confidence
    * velocity estimate consistent with input cluster speed
    */
   CHECK_TEXT(CONF3_HIGH == result, "Estimation confidence not MED as expected");
   DOUBLES_EQUAL(xvel, longvel_by_position, 0.001F);
   DOUBLES_EQUAL(yvel, latvel_by_position, 0.001F);
}



/** \purpose
 * Check whether the method returns correct estimation confidence and velocity estimates
 * when the detections are split between two sensors and meet the inlier ratio requirement for medium confidence
 * but doesn't satisfy it in terms of the amount of detections (not enough confirmed timestamps overall and not
 * enough comming from a single sensor)
 *
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_position_change, Detections_Split_Between_Two_Sensors_Med_Conf_sum_confirmed_ts_Below_Threshold_num_confirmed_single_sensor_ts_Below_Threshold)
{
   /** \precond
    * Preconditions from setup, additionally:
    * set up the cluster without range rate modifier
    * historical detections truncated to 12
    * for every other current and historical detection set the sensor id to 2
    * majority (16) of the old detections from the cluster have the vcx x position shifted
    * (azimuth and rdot_comp recalculated accordingly)
    */

   setup_moving_cluster(cluster, raw_detections, det_hist, xpos, ypos, xvel, yvel, false);
   cluster.num_old_dets = 12;

   for (int32_t i = 0; i < cluster.ndets; i++)
   {
      raw_detections.detections[i].raw.sensor_id= 1 + static_cast<uint8_t>(i%2);
   }
   for (int32_t i = 0; i < cluster.num_old_dets; i++)
   {
      det_hist.det_data[i].sensor_id = 1 + static_cast<uint8_t>(i%2);
   }


   // skew some detections to lower the weighted_inlier_ratio_long
   for (int32_t i = 0; i < cluster.num_old_dets-4; i++)
   {
      det_hist.det_data[i].vcs_position_x -= 1.8F;
      det_hist.det_data[i].vcs_az = F360_Atan2f(det_hist.det_data[i].vcs_position_y, det_hist.det_data[i].vcs_position_x);
      det_hist.det_data[i].rdot_comp = F360_Cosf(det_hist.det_data[i].vcs_az) * xvel + F360_Sinf(det_hist.det_data[i].vcs_az) * yvel;
   }


   /** \action
    * Call Estimate_Velocity_By_Position_Change().
    */
   CONF3_T result = Estimate_Velocity_By_Position_Change(sensors, det_hist, raw_detections, det_props, cluster, longvel_by_position, latvel_by_position);

   /** \result
    * CONF3_NONE confidence
    */
   CHECK_TEXT(CONF3_NONE == result, "Estimation confidence not NONE as expected");
}


/** \purpose
 * Check whether the method returns correct estimation confidence and velocity estimates
 * when the detections are split between two sensors and does not meet the inlier ratio requirement for medium confidence
 * and doesn't satisfy it in terms of the amount of detections (not enough confirmed timestamps overall and not
 * enough comming from a single sensor)
 *
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_position_change, Detections_Split_Between_Two_Sensors_sum_confirmed_ts_Below_Threshold_num_confirmed_single_sensor_ts_Below_Threshold)
{
   /** \precond
    * Preconditions from setup, additionally:
    * set up the cluster without range rate modifier
    * historical detections truncated to 11
    * for every other current and historical detection set the sensor id to 2
    */

   setup_moving_cluster(cluster, raw_detections, det_hist, xpos, ypos, xvel, yvel, false);
   cluster.num_old_dets = 11;

   for (int32_t i = 0; i < cluster.ndets; i++)
   {
      raw_detections.detections[i].raw.sensor_id= 1 + static_cast<uint8_t>(i%2);
   }
   for (int32_t i = 0; i < cluster.num_old_dets; i++)
   {
      det_hist.det_data[i].sensor_id = 1 + static_cast<uint8_t>(i%2);
   }

   /** \action
    * Call Estimate_Velocity_By_Position_Change().
    */
   CONF3_T result = Estimate_Velocity_By_Position_Change(sensors, det_hist, raw_detections, det_props, cluster, longvel_by_position, latvel_by_position);

   /** \result
    * CONF3_LOW confidence
    * velocity estimate consistent with input cluster speed
    */
   CHECK_TEXT(CONF3_LOW == result, "Estimation confidence not LOW as expected");
   DOUBLES_EQUAL(xvel, longvel_by_position, 0.001F);
   DOUBLES_EQUAL(yvel, latvel_by_position, 0.001F);
}


/** \purpose
 * Check whether the method returns correct estimation confidence when vcs x positions in some of the historical detections
 * are set to be longitudinal outliers.
 *
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_position_change, invalid_longitudinal_velocity_output)
{
   /** \precond
    * Preconditions from setup, additionally:
    * set up the cluster without range rate modifier
    * some of the detections vcs x positions are skewed to be considered as longitudinal outliers
    */

   setup_moving_cluster(cluster, raw_detections, det_hist, xpos, ypos, xvel, yvel, false);
   float32_t pos_modifier = 0.0F;
   int32_t n = 0;
   for (int32_t i = 0; i < 10; i++)
   {
      pos_modifier = i%3 == 0 ? 3.0F : 0.0F;
      det_hist.det_data[n].vcs_position_x -= pos_modifier;
      det_hist.det_data[n].vcs_az = F360_Atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
      det_hist.det_data[n].rdot_comp = F360_Cosf(det_hist.det_data[n].vcs_az) * xvel + F360_Sinf(det_hist.det_data[n].vcs_az) * yvel;
      n++;
      det_hist.det_data[n].vcs_position_x -= pos_modifier;
      det_hist.det_data[n].vcs_az = F360_Atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
      det_hist.det_data[n].rdot_comp = F360_Cosf(det_hist.det_data[n].vcs_az) * xvel + F360_Sinf(det_hist.det_data[n].vcs_az) * yvel;
   }

   /** \action
    * Call Estimate_Velocity_By_Position_Change().
    */
   CONF3_T result = Estimate_Velocity_By_Position_Change(sensors, det_hist, raw_detections, det_props, cluster, longvel_by_position, latvel_by_position);

   /** \result
    * CONF3_NONE confidence
    */
   CHECK_TEXT(CONF3_NONE == result, "Estimation confidence not NONE as expected");
}



/** \purpose
 * Check whether the method returns correct estimation confidence when vcs y positions in some of the historical detections
 * are set to be lateral outliers.
 *
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_position_change, invalid_lateral_velocity_output)
{
   /** \precond
    * Preconditions from setup, additionally:
    * set up the cluster without range rate modifier
    * some of the detections vcs y positions are skewed to be considered as lateral outliers
    */

   setup_moving_cluster(cluster, raw_detections, det_hist, xpos, ypos, xvel, yvel, false);
   float32_t pos_modifier = 0.0F;
   int32_t n = 0;
   for (int32_t i = 0; i < 10; i++)
   {
      pos_modifier = i%3 == 0 ? 3.0F : 0.0F;
      det_hist.det_data[n].vcs_position_y -= pos_modifier;
      det_hist.det_data[n].vcs_az = F360_Atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
      det_hist.det_data[n].rdot_comp = F360_Cosf(det_hist.det_data[n].vcs_az) * xvel + F360_Sinf(det_hist.det_data[n].vcs_az) * yvel;
      n++;
      det_hist.det_data[n].vcs_position_y -= pos_modifier;
      det_hist.det_data[n].vcs_az = F360_Atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
      det_hist.det_data[n].rdot_comp = F360_Cosf(det_hist.det_data[n].vcs_az) * xvel + F360_Sinf(det_hist.det_data[n].vcs_az) * yvel;
   }

   /** \action
    * Call Estimate_Velocity_By_Position_Change().
    */
   CONF3_T result = Estimate_Velocity_By_Position_Change(sensors, det_hist, raw_detections, det_props, cluster, longvel_by_position, latvel_by_position);

   /** \result
    * CONF3_NONE confidence
    */
   CHECK_TEXT(CONF3_MED == result, "Estimation confidence not NONE as expected");
}


/** \purpose
 * Check whether the method returns correct estimation confidence when range rate in the historical detections
 * is not consistent with the speed which the cluster should have.
 *
 * \req
 * NA
 */
TEST(f360_estimate_velocity_by_position_change, invalid_rdot)
{
   /** \precond
    * Preconditions from setup, additionally:
    * xvel set to -5.0F
    * set up the cluster with range rate modifier (vcs x and y positions are indicating as the cluster is moving faster,
    * but rdot_comp does not reflect that)
    */

   float32_t xvel = -5.0F;
   setup_moving_cluster(cluster, raw_detections, det_hist, xpos, ypos, xvel, yvel, true);

   /** \action
    * Call Estimate_Velocity_By_Position_Change().
    */
   CONF3_T result = Estimate_Velocity_By_Position_Change(sensors, det_hist, raw_detections, det_props, cluster, longvel_by_position, latvel_by_position);

   /** \result
    * CONF3_NONE confidence
    */
   CHECK_TEXT(CONF3_NONE == result, "Estimation confidence not NONE as expected");
}

/** @}*/

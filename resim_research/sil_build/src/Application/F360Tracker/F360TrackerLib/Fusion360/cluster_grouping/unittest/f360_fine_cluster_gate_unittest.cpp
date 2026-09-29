/** \file
   File contains tests for Fine_Cluster_Gate function
*/

#include "f360_fine_cluster_gate.h"
#include "f360_cluster_grouping_data_generator.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;

/** \defgroup  f360_fine_cluster_gate
 *  @{
 */

/** \brief
*  Test Group for Fine_Cluster_Gate function
**/
TEST_GROUP(f360_fine_cluster_gate)
{
   /** \setup
   * Setting up clusers and intervals
   **/
   F360_Calibrations_T calib = {};
   F360_Cluster_T cluster_1 = {};
   F360_Cluster_T cluster_2 = {};
   float32_t rdot_interval_1 = 0.0;
   float32_t rdot_interval_2 = 0.0;
   float32_t alias_interval_1 = 0.0;
   float32_t alias_interval_2 = 0.0;

   float32_t tolerance = 1e-6;

   bool f_success = false;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/**
*\purpose  Checking if gating flag is calculated correctly for two cluster with same rage rates 1
*\req    NA
*/
TEST(f360_fine_cluster_gate, Fine_Cluster_Gate__NominalSimplePossitiveCase)
{
   /** \precond
   * Filling up cluster with position, range rates and time since cluster update
   **/
   Fill_Cluster(0.0, 10.0, 10.0, 0.0, cluster_1);
   Fill_Cluster(0.0, 9.0, 10.0, 0.05, cluster_2);

   /** \action
   * Call Fine_Cluster_Gate function
   **/   
   f_success = Fine_Cluster_Gate(calib, cluster_1, cluster_2, rdot_interval_1, rdot_interval_2, alias_interval_1, alias_interval_2);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  Checking if gating flag is calculated correctly for two cluster with same rage rates 2
*\req    NA
*/
TEST(f360_fine_cluster_gate, Fine_Cluster_Gate__NominalPossitiveCase)
{
   /** \precond
   * Filling up cluster with position, range rates and time since cluster update
   **/
   Fill_Cluster(11.0, 11.0, 5.0, 0.0, cluster_1);
   Fill_Cluster(10.5, 10.5, 5.0, 0.05, cluster_2);

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Fine_Cluster_Gate(calib, cluster_1, cluster_2, rdot_interval_1, rdot_interval_2, alias_interval_1, alias_interval_2);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  Checking if gating flag is calculated correctly for two cluster with same rage rates 3
*\req    NA
*/
TEST(f360_fine_cluster_gate, Fine_Cluster_Gate__NominalPossitiveCase2)
{
   /** \precond
   * Filling up cluster with position, range rates and time since cluster update
   **/
   Fill_Cluster(11.0, 11.0, -5.0, 0.0, cluster_2);
   Fill_Cluster(10.5, 10.5, -5.0, 0.05, cluster_1);

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Fine_Cluster_Gate(calib, cluster_1, cluster_2, rdot_interval_1, rdot_interval_2, alias_interval_1, alias_interval_2);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  Checking if gating flag is calculated correctly for two cluster with same rage rates, low time difference and low but significant position difference
*\req    NA
*/
TEST(f360_fine_cluster_gate, Fine_Cluster_Gate__HighPosDiffWithHighRR)
{
   /** \precond
   * Filling up cluster with position, range rates and time since cluster update
   **/
   Fill_Cluster(10.0, 10.0, 40.0, 0.0, cluster_1);
   Fill_Cluster(8.0, 8.0, 40.0, 0.05, cluster_2);

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Fine_Cluster_Gate(calib, cluster_1, cluster_2, rdot_interval_1, rdot_interval_2, alias_interval_1, alias_interval_2);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  Checking if gating flag is calculated correctly for two cluster with same rage rates and high position difference and high time difference
*\req    NA
*/
TEST(f360_fine_cluster_gate, Fine_Cluster_Gate__HighTimeDiff)
{
   /** \precond
   * Filling up cluster with position, range rates and time since cluster update
   **/
   Fill_Cluster(15.0, 15.0, 30.0, 0.0, cluster_1);
   Fill_Cluster(5.0, 5.0, 30.0, 0.5, cluster_2);

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Fine_Cluster_Gate(calib, cluster_1, cluster_2, rdot_interval_1, rdot_interval_2, alias_interval_1, alias_interval_2);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  Checking if gating flag is set to false if two cluster have the same range rates, high time difference and high position difference.
In particular, check that extension factor induced by high time difference is saturated so that the flag will be false
*\req    NA
*/
TEST(f360_fine_cluster_gate, Fine_Cluster_Gate__HighTimeDiff_saturated)
{
   /** \precond
   * Filling up cluster with position, range rates and time since cluster update
   **/
   Fill_Cluster(50.0, -4.0, 0.0, 0.0, cluster_1);
   Fill_Cluster(50.0, 4.0, 0.0, 0.5, cluster_2);

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Fine_Cluster_Gate(calib, cluster_1, cluster_2, rdot_interval_1, rdot_interval_2, alias_interval_1, alias_interval_2);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  Checking if gating flag is calculated correctly for two cluster with same rage rates, high position difference and no time difference
*\req    NA
*/
TEST(f360_fine_cluster_gate, Fine_Cluster_Gate__ClustersTooFarAwayLowRR)
{
   /** \precond
   * Filling up cluster with position, range rates and time since cluster update
   **/
   Fill_Cluster(50.0, -15.0, 5.0, 0.0, cluster_1);
   Fill_Cluster(5.0, 5.0, 5.0, 0.0, cluster_2);

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Fine_Cluster_Gate(calib, cluster_1, cluster_2, rdot_interval_1, rdot_interval_2, alias_interval_1, alias_interval_2);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  Checking if gating flag is calculated correctly for two cluster with same high rage rates, low position difference and low time difference
*\req    NA
*/
TEST(f360_fine_cluster_gate, Fine_Cluster_Gate__CloseClustersHighRR)
{
   /** \precond
   * Filling up cluster with position, range rates and time since cluster update
   **/
   Fill_Cluster(5.2, 5.2, 40.0, 0.0, cluster_1);
   Fill_Cluster(5.0, 5.0, 40.0, 0.15, cluster_2);

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Fine_Cluster_Gate(calib, cluster_1, cluster_2, rdot_interval_1, rdot_interval_2, alias_interval_1, alias_interval_2);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  Checking if gating flag is calculated correctly for two cluster with same rage rates but with interval change, low position difference and low time difference
*\req    NA
*/
TEST(f360_fine_cluster_gate, Fine_Cluster_Gate__NoneZeroInterval)
{
   /** \precond
   * Filling up cluster with position, range rates and time since cluster update
   **/
   Fill_Cluster(6.0, 6.0, 30.0, 0.0, cluster_1);
   Fill_Cluster(5.0, 5.0, 0.0, 0.05, cluster_2);

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Fine_Cluster_Gate(calib, cluster_1, cluster_2, rdot_interval_1, 30.0, alias_interval_1, 1.0);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  Checking if gating flag is calculated correctly for two cluster at faraway distance which are faraway from each other in cross-radial direction
*\req    NA
*/
TEST(f360_fine_cluster_gate, Fine_Cluster_Gate__Far_Away_Cross_Radial_Clusters)
{
   /** \precond
   * Filling up cluster with position, range rates and time since cluster update
   **/
   Fill_Cluster(5.0, 100.0, 40.0, 0.0, cluster_1);
   Fill_Cluster(-5.0, 100.0, 40.0, 0.0, cluster_2);

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Fine_Cluster_Gate(calib, cluster_1, cluster_2, rdot_interval_1, rdot_interval_2, alias_interval_1, alias_interval_2);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/** @}*/

/** \defgroup  f360_calc_radial_gate
*  @{
*/

/** \brief
*  Test Group to check Calc_Radial_Gate function works as intended
**/
TEST_GROUP(f360_calc_radial_gate)
{
   /** \setup
   * Setting up clusters' parameters, calibrations and expected values
   **/
   F360_Calibrations_T calib = {};
   float32_t abs_mean_rdot_comp;
   float32_t range;
   float32_t tolerance;
   float32_t exp_radial_gate_slow;
   float32_t exp_radial_gate_fast;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      abs_mean_rdot_comp = 5.0F;
      range = 68.0F;
      tolerance = 1e-6;

      exp_radial_gate_slow = calib.k_radial_gate;
      exp_radial_gate_fast = 2.0F * calib.k_radial_gate;
   }
};

/**
*\purpose  Checking if radial gate is calculated correctly for fast clusters
*\req    NA
*/
TEST(f360_calc_radial_gate, Calc_Radial_Gate__FastClusters)
{
   /** \precond
   * Fast clusters at range below threshold
   **/

   /** \action
   * Call Calc_Radial_Gate function
   **/
   const float32_t radial_gate = Calc_Radial_Gate(calib, abs_mean_rdot_comp, range);

   /** \result
   * Checking if radial gate is calculated correctly - big for fast clusters
   **/
   DOUBLES_EQUAL(exp_radial_gate_fast, radial_gate, tolerance);
}

/**
*\purpose  Checking if radial gate is calculated correctly for slow clusters
*\req    NA
*/
TEST(f360_calc_radial_gate, Calc_Radial_Gate__SlowClusters)
{
   /** \precond
   * Slow clusters at range below threshold
   **/
   abs_mean_rdot_comp = 2.0F;

   /** \action
   * Call Calc_Radial_Gate function
   **/
   const float32_t radial_gate = Calc_Radial_Gate(calib, abs_mean_rdot_comp, range);

   /** \result
   * Checking if radial gate is calculated correctly - small for slow clusters
   **/
   DOUBLES_EQUAL(exp_radial_gate_slow, radial_gate, tolerance);
}

/**
*\purpose  Checking if radial gate is calculated correctly for fast clusters with range slightly above the threshold
*\req    NA
*/
TEST(f360_calc_radial_gate, Calc_Radial_Gate__FastClustersAboveThreshold)
{
   /** \precond
   * Fast clusters at range just above threshold
   **/
   range = 72.0F;

   /** \action
   * Call Calc_Radial_Gate function
   **/
   const float32_t radial_gate = Calc_Radial_Gate(calib, abs_mean_rdot_comp, range);

   /** \result
   * Checking if radial gate is calculated correctly - small for fast clusters above threshold
   **/
   DOUBLES_EQUAL(exp_radial_gate_fast + 0.05F, radial_gate, tolerance);
}

/**
*\purpose  Checking if radial gate is extended for faraway fast-moving clusters
*\req    NA
*/
TEST(f360_calc_radial_gate, Calc_Radial_Gate__FarawayFastClusters)
{
   /** \precond
   * Fast clusters at faraway range
   **/
   range = 100.0F;

   /** \action
   * Call Calc_Radial_Gate function
   **/
   const float32_t radial_gate = Calc_Radial_Gate(calib, abs_mean_rdot_comp, range);

   /** \result
   * Checking if radial gate is calculated correctly - extended for faraway fast clusters, with extension factor saturated to 0.75
   **/
   DOUBLES_EQUAL(exp_radial_gate_fast + 0.75F, radial_gate, tolerance);
}

/**
*\purpose  Checking if radial gate is not extended for faraway slow-moving clusters
*\req    NA
*/
TEST(f360_calc_radial_gate, Calc_Radial_Gate__FarawaySlowClusters)
{
   /** \precond
   * Slow clusters at faraway range
   **/
   range = 100.0F;
   abs_mean_rdot_comp = 2.0F;

   /** \action
   * Call Calc_Radial_Gate function
   **/
   const float32_t radial_gate = Calc_Radial_Gate(calib, abs_mean_rdot_comp, range);

   /** \result
   * Checking if radial gate is calculated correctly - not extended for faraway slow clusters
   **/
   DOUBLES_EQUAL(exp_radial_gate_slow, radial_gate, tolerance);
}

/** @}*/

/** \defgroup  f360_calc_cross_radial_gate
*  @{
*/

/** \brief
*  Test Group to check Calc_Cross_Radial_Gate function works as intended
**/
TEST_GROUP(f360_calc_cross_radial_gate)
{
   /** \setup
   * Setting up clusters' parameters and calibrations
   **/
   F360_Calibrations_T calib = {};
   float32_t abs_mean_rdot_comp;
   float32_t range;
   float32_t max_cross_radial_speed;
   float32_t delta_time;
   float32_t tolerance;
   float32_t exp_value;
   float32_t exp_size_long_range;


   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      abs_mean_rdot_comp = 10.0F;
      range = 10.0F;
      max_cross_radial_speed = calib.k_default_max_cross_radial_speed;
      delta_time = 0.05F;
      tolerance = 1e-6;
   }
};

/**
*\purpose  Checking if cross-radial gate is calculated correctly for short range clusters - close range logic is used
*\req    NA
*/
TEST(f360_calc_cross_radial_gate, Calc_Cross_Radial_Gate__ShortRangeClusters)
{
   /** \precond
   * Clusters at short range, setup as in test group
   **/

  exp_value = calib.k_az_scaling_factor_close_range * range + max_cross_radial_speed * delta_time;

  /** \action
   * Call Calc_Cross_Radial_Gate function
   **/
  const float32_t cross_radial_gate = Calc_Cross_Radial_Gate(calib, abs_mean_rdot_comp, range, max_cross_radial_speed, delta_time);

  /** \result
   * Checking if cross-radial gate is calculated correctly - short range logic
   * Expected behaviour: close range logic
   **/
   DOUBLES_EQUAL(exp_value, cross_radial_gate, tolerance);
}

/**
*\purpose  Checking if cross-radial gate is calculated correctly (saturated) for short range clusters with higher max_cross_radial_speed and time difference
*\req    NA
*/
TEST(f360_calc_cross_radial_gate, Calc_Cross_Radial_Gate__ShortRangeClustersHighCrossRadialSpeed)
{
   /** \precond
   * Clusters at short range, high max_cross_radial_speed and time difference
   **/

  max_cross_radial_speed = 40.0F;
  delta_time = 0.2F;

  exp_value = calib.k_az_scaling_factor_close_range * range + calib.k_cross_radial_movement_increased_saturation;
  
  /** \action
   * Call Calc_Cross_Radial_Gate function
   **/
  const float32_t cross_radial_gate = Calc_Cross_Radial_Gate(calib, abs_mean_rdot_comp, range, max_cross_radial_speed, delta_time);

  /** \result
   * Checking if cross-radial gate is calculated correctly - short range logic with saturated cross-radial motion extension should be used
   * Expected behaviour: close range logic with saturated cross-radial motion extension should be used
   **/
  DOUBLES_EQUAL(exp_value, cross_radial_gate, tolerance);
}

/**
*\purpose  Checking if cross-radial gate is calculated correctly for long range high-speed clusters - faraway range logic is used
*\req    NA
*/
TEST(f360_calc_cross_radial_gate, Calc_Cross_Radial_Gate__LongRangeHighSpeedClusters)
{
   /** \precond
   * Clusters at long range
   **/
  range = 50.0F;
  exp_value = calib.k_az_scaling_factor_long_range * range + max_cross_radial_speed * delta_time + calib.k_fast_moving_size_for_cross_radial_gate;

  /** \action
   * Call Calc_Cross_Radial_Gate function
   **/
  const float32_t cross_radial_gate = Calc_Cross_Radial_Gate(calib, abs_mean_rdot_comp, range, max_cross_radial_speed, delta_time);
  /** \result
   * Checking if cross-radial gate is calculated correctly - long range logic with maximum size should be used for long range high-speed clusters
   * Expected behaviour: long range logic with maximum size
   **/
   DOUBLES_EQUAL(exp_value, cross_radial_gate, tolerance);
}

/**
*\purpose  Checking if cross-radial gate is calculated correctly for long range low-speed clusters - faraway range logic with minimum size is used
*\req    NA
*/
TEST(f360_calc_cross_radial_gate, Calc_Cross_Radial_Gate__LongRangeLowSpeedClusters)
{
   /** \precond
   * Clusters at long range
   **/
  range = 50.0F;
  abs_mean_rdot_comp = 2.0F;
  exp_value = calib.k_az_scaling_factor_long_range * range + max_cross_radial_speed * delta_time + calib.k_slow_moving_size_for_cross_radial_gate;
  /** \action
   * Call Calc_Cross_Radial_Gate function
   **/
  const float32_t cross_radial_gate = Calc_Cross_Radial_Gate(calib, abs_mean_rdot_comp, range, max_cross_radial_speed, delta_time);
  /** \result
   * Checking if cross-radial gate is calculated correctly - long range logic with minimum size should be used for long range low-speed clusters
   * Expected behaviour: long range logic with minimum size
   **/
   DOUBLES_EQUAL(exp_value, cross_radial_gate, tolerance);
}

/**
*\purpose  Checking if cross-radial gate is calculated correctly for medium range clusters - close range logic is used if clusters are faster
*\req    NA
*/
TEST(f360_calc_cross_radial_gate, Calc_Cross_Radial_Gate__MediumRangeHighSpeedClusters)
{
   /** \precond
   * Clusters at medium range
   **/
  range = 25.0F;
  abs_mean_rdot_comp = 7.5F;
  exp_value = calib.k_az_scaling_factor_close_range * range + max_cross_radial_speed * delta_time;
  /** \action
   * Call Calc_Cross_Radial_Gate function
   **/
  const float32_t cross_radial_gate = Calc_Cross_Radial_Gate(calib, abs_mean_rdot_comp, range, max_cross_radial_speed, delta_time);
  /** \result
   * Checking if cross-radial gate is calculated correctly - close range logic should be used for medium range high-speed clusters
   * Expected behaviour: close range logic
   **/
   DOUBLES_EQUAL(exp_value, cross_radial_gate, tolerance);
}

/**
*\purpose  Checking if cross-radial gate is calculated correctly for medium range clusters - long range logic is used if cluster is slower
*\req    NA
*/
TEST(f360_calc_cross_radial_gate, Calc_Cross_Radial_Gate__MediumRangeLowSpeedClusters)
{
   /** \precond
   * Clusters at medium range
   **/
  range = 25.0F;
  abs_mean_rdot_comp = 4.5F;
  exp_size_long_range = F360_Linear_Equation_With_Saturation(abs_mean_rdot_comp, calib.k_slow_moving_speed_thr_for_cross_radial_gate, calib.k_fast_moving_speed_thr_for_cross_radial_gate, calib.k_slow_moving_size_for_cross_radial_gate, calib.k_fast_moving_size_for_cross_radial_gate);
  exp_value = calib.k_az_scaling_factor_long_range * range + max_cross_radial_speed * delta_time + exp_size_long_range;
  /** \action
   * Call Calc_Cross_Radial_Gate function
   **/
  const float32_t cross_radial_gate = Calc_Cross_Radial_Gate(calib, abs_mean_rdot_comp, range, max_cross_radial_speed, delta_time);
  /** \result
   * Checking if cross-radial gate is calculated correctly - long range logic should be used for medium range low-speed clusters
   * Expected behaviour: long range logic with size based on speed
   **/
   DOUBLES_EQUAL(exp_value, cross_radial_gate, tolerance);
}

/** @}*/

/** \defgroup  f360_calc_cross_radial_gate_short_vs_long
*  @{
*/

/** \brief
*  Test Group to check if the result of Calc_Cross_Radial_Gate function is the minimum of two short range and long range logic
**/
TEST_GROUP(f360_calc_cross_radial_gate_short_vs_long)
{
   /** \setup
   * Setting up clusters' parameters and calibrations
   **/
   F360_Calibrations_T calib = {};
   float32_t max_cross_radial_speed;
   float32_t delta_time;
   float32_t tolerance;
   float32_t exp_value;
   float32_t exp_size_long_range;

   float32_t range_array[5U]{};
   float32_t rdot_comp_array[5U]{};
   float32_t cross_radial_gate_array[5U * 5U]{};
   float32_t short_cross_radial_gate_array[5U * 5U]{};
   float32_t long_cross_radial_gate_array[5U * 5U]{};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      max_cross_radial_speed = calib.k_default_max_cross_radial_speed;
      delta_time = 0.05F;
      tolerance = 1e-3;
   }
};

/**
*\purpose  Checking if cross-radial gate is always the minimum of long range and close range logic
*\req    NA
*/
TEST(f360_calc_cross_radial_gate_short_vs_long, Calc_Cross_Radial_Gate__ShortVsLong)
{

   /** \precond
   * Setup various range and rdot values
   **/

   for (uint8_t i = 0U; i < 5U; i++)
   {
      range_array[i] = 10.0F * i;
      rdot_comp_array[i] = 2.0F * i;
   }

   /** \action
   * Call Calc_Cross_Radial_Gate, Calc_Cross_radial function
   **/
  for (uint8_t i = 0U; i < 5U; i++)
   {
      for (uint8_t j = 0U; j < 5U; j++)
      {
         const uint8_t idx = 5U * i + j;
         cross_radial_gate_array[idx] = Calc_Cross_Radial_Gate(calib, rdot_comp_array[j], range_array[i], max_cross_radial_speed, delta_time);
         short_cross_radial_gate_array[idx] = Calc_Cross_Radial_Gate_Short_Range(calib, range_array[i], max_cross_radial_speed, delta_time);
         long_cross_radial_gate_array[idx] = Calc_Cross_Radial_Gate_Long_Range(calib, rdot_comp_array[j], range_array[i], max_cross_radial_speed, delta_time);
      }
   }

   /** \result
   * Checking if cross-radial gate is calculated correctly - always minimum of those two
   **/
   for (uint8_t i = 0U; i < 25U; i++)
   {
      const float32_t cross_radial_gate = cross_radial_gate_array[i];
      const float32_t short_cross_radial_gate = short_cross_radial_gate_array[i];
      const float32_t long_cross_radial_gate = long_cross_radial_gate_array[i];

      CHECK_FALSE_TEXT(cross_radial_gate > short_cross_radial_gate + tolerance, "Cross radial gate is bigger than short range logic gate, which should not happen, as cross radial gate should be the minimum of short and long range logic");
      CHECK_FALSE_TEXT(cross_radial_gate > long_cross_radial_gate + tolerance, "Cross radial gate is bigger than long range logic gate, which should not happen, as cross radial gate should be the minimum of short and long range logic");
   }
}

/** @}*/

/** \defgroup  f360_close_range_beside_host_adjustment
*  @{
*/

/** \brief
*  Test Group to check Close_Range_Beside_Host_Adjustment function works as intended
**/
TEST_GROUP(f360_close_range_beside_host_adjustment)
{
   /** \setup
   * Setting up clusters' parameters and calibrations
   **/
   F360_Calibrations_T calib = {};
   float32_t range;
   float32_t trk_az_rep_sin;
   float32_t max_cross_radial_speed;
   float32_t radial_gate;
   float32_t tolerance;
   float32_t exp_radial_gate;
   float32_t exp_max_cross_radial_speed;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      range = 2.0F;
      trk_az_rep_sin = 1.0F;
      radial_gate = 2.0F * calib.k_radial_gate;
      max_cross_radial_speed = calib.k_default_max_cross_radial_speed;
      tolerance = 1e-6;
   }
};

/**
*\purpose  Checking if full close range beside host adjustment is applied correctly if object is in specified range-azimuth region
*\req    NA
*/
TEST(f360_close_range_beside_host_adjustment, Close_Range_Beside_Host_Adjustment_Full_Adjustment)
{
   /** \precond
   * Clusters in specified region, as in test group setup
   **/

  exp_max_cross_radial_speed = calib.k_increased_max_cross_radial_speed;
  exp_radial_gate = 2.0F;

  /** \action
   * Call Close_Range_Beside_Host_Adjustment function
   **/
  Close_Range_Beside_Host_Adjustment(calib, range, trk_az_rep_sin, radial_gate, max_cross_radial_speed);

  /** \result
   * Checking if max_cross_radial speed was increased to maximum value and if radial gate was decreased
   * Expected behaviour: max_cross_radial_speed increased, radial gate decreased
   **/
   DOUBLES_EQUAL(exp_max_cross_radial_speed, max_cross_radial_speed, tolerance);
   DOUBLES_EQUAL(exp_radial_gate, radial_gate, tolerance);
}

/**
*\purpose  Checking if close range beside host adjustment is applied correctly if object is in specified range-azimuth region, but radial gate is small so unchanged
*\req    NA
*/
TEST(f360_close_range_beside_host_adjustment, Close_Range_Beside_Host_Adjustment_No_Radial_Gate_Adjustment)
{
   /** \precond
   * Clusters in specified region, as in test group setup, radial gate small
   **/
  radial_gate = calib.k_radial_gate;
  exp_max_cross_radial_speed = calib.k_increased_max_cross_radial_speed;
  exp_radial_gate = radial_gate;

  /** \action
   * Call Close_Range_Beside_Host_Adjustment function
   * Expected behaviour: max_cross_radial_speed increased, radial gate unchanged
   **/
  Close_Range_Beside_Host_Adjustment(calib, range, trk_az_rep_sin, radial_gate, max_cross_radial_speed);

   /** \result
   * Checking if max_cross_radial speed was increased to maximum value and if radial gate was decreased
   * Expected behaviour: max_cross_radial_speed increased, radial gate unchanged
   **/
   DOUBLES_EQUAL(exp_max_cross_radial_speed, max_cross_radial_speed, tolerance);
   DOUBLES_EQUAL(exp_radial_gate, radial_gate, tolerance);
}

/**
*\purpose  Checking if close range beside host adjustment is not applied if object is outside specified region by range
*\req    NA
*/
TEST(f360_close_range_beside_host_adjustment, Close_Range_Beside_Host_Adjustment_No_Adjustment_Range)
{
   /** \precond
   * Clusters with range outside region
   **/
  range = 10.0F;
  exp_max_cross_radial_speed = max_cross_radial_speed;
  exp_radial_gate = radial_gate;

  /** \action
   * Call Close_Range_Beside_Host_Adjustment function
   **/
  Close_Range_Beside_Host_Adjustment(calib, range, trk_az_rep_sin, radial_gate, max_cross_radial_speed);

  /** \result
   * Checking if max_cross_radial speed and radial gate were unchanged
   * Expected behaviour: max_cross_radial_speed, radial gate unchanged
   **/
   DOUBLES_EQUAL(exp_max_cross_radial_speed, max_cross_radial_speed, tolerance);
   DOUBLES_EQUAL(exp_radial_gate, radial_gate, tolerance);
}

/**
*\purpose  Checking if close range beside host adjustment is not applied if object is outside specified region by azimuth
*\req    NA
*/
TEST(f360_close_range_beside_host_adjustment, Close_Range_Beside_Host_Adjustment_No_Adjustment_Azimuth)
{
   /** \precond
   * Clusters with azimuth outside region
   **/
  trk_az_rep_sin = 0.0F;
  exp_max_cross_radial_speed = max_cross_radial_speed;
  exp_radial_gate = radial_gate;

  /** \action
   * Call Close_Range_Beside_Host_Adjustment function
   **/
  Close_Range_Beside_Host_Adjustment(calib, range, trk_az_rep_sin, radial_gate, max_cross_radial_speed);

  /** \result
   * Checking if max_cross_radial speed and radial gate were unchanged
   * Expected behaviour: max_cross_radial_speed, radial gate unchanged
   **/
   DOUBLES_EQUAL(exp_max_cross_radial_speed, max_cross_radial_speed, tolerance);
   DOUBLES_EQUAL(exp_radial_gate, radial_gate, tolerance);
}

/**
*\purpose  Checking if close range beside host adjustment is applied in-between full-adjustment region and no-adjustment regions
*\req    NA
*/
TEST(f360_close_range_beside_host_adjustment, Close_Range_Beside_Host_Adjustment_Gradual_Adjustment)
{
   /** \precond
   * Clusters in between regions
   **/
  trk_az_rep_sin = 0.6F;
  range = 6.0F;
  exp_max_cross_radial_speed = 17.5F;
  exp_radial_gate = 2.75F;

  /** \action
   * Call Close_Range_Beside_Host_Adjustment function
   **/
  Close_Range_Beside_Host_Adjustment(calib, range, trk_az_rep_sin, radial_gate, max_cross_radial_speed);

  /** \result
   * Checking if max_cross_radial speed and radial gate were adjusted accordingly
   * Expected behaviour: max_cross_radial_speed, radial gate changed gradually
   **/
   DOUBLES_EQUAL(exp_max_cross_radial_speed, max_cross_radial_speed, tolerance);
   DOUBLES_EQUAL(exp_radial_gate, radial_gate, tolerance);
}

/** @}*/



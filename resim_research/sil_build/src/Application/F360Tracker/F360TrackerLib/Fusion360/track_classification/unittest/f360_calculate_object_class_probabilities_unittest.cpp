/** \file
 * This file contains unit tests for content of f360_calculate_object_class_probabilities.cpp file
 */

#include "f360_calculate_object_class_probabilities.h"
#include "f360_object_track.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup calcAprioriProbability
 *  @{
 */

/** \brief
 * Test Group of calcAprioriProbability() function. Tests verify whether apriori probabilities
 * are properly attenuated based on object attributes and normalized.
 */
TEST_GROUP(calcAprioriProbability)
{
   F360_Object_Track_T object; // local test object
   OBJ_CLASS_A_PRIORI_PROBABILITIES_T apriori;
   F360_Tracker_Info_T tracker_info; // tracker info for elapsed time

   /** \setup
    * Initialize object with zeros and neutral values for fields used by calcAprioriProbability.
    */
   TEST_SETUP()
   {
      object.speed = 0.0F;
      object.average_rcs = 5.0F;
      object.maximum_rcs = 6.0F;
      object.num_members_in_slow_moving_obj_cluster = 0U;
      tracker_info.elapsed_time_s = 0.05F; // Default 50ms cycle time
   }
};

/** \purpose
 * Purpose of this test is to verify baseline uniform probabilities and normalization when no attenuating conditions apply.
 * \req
 * NA.
 */
TEST(calcAprioriProbability, calcAprioriProbability__baseline_uniform)
{
   /** \precond
    * Object speed below all thresholds, RCS below threshold, cluster size = 0.
    */
   object.speed = 1.0F;

   /** \action
    * Call tested function.
    */
   calcAprioriProbability(apriori, object, tracker_info);

   /** \result
    * All classes should remain equal after normalization => each 0.2.
    */
   DOUBLES_EQUAL(0.2F, apriori.pedestrian, 1e-6F);
   DOUBLES_EQUAL(0.2F, apriori.bicycle, 1e-6F);
   DOUBLES_EQUAL(0.2F, apriori.motorcycle, 1e-6F);
   DOUBLES_EQUAL(0.2F, apriori.car, 1e-6F);
   DOUBLES_EQUAL(0.2F, apriori.truck, 1e-6F);
}

/** \purpose
 * Purpose of this test is to verify pedestrian apriori is zeroed when speed exceeds 4 m/s while others normalize accordingly.
 * \req
 * NA.
 */
TEST(calcAprioriProbability, calcAprioriProbability__speed_exceeds_ped_threshold)
{
   /** \precond
    * Object speed > 4.0, but <=8.0 so bicycle still allowed; RCS & cluster benign.
    */
   object.speed = 4.01F;

   /** \action */
   calcAprioriProbability(apriori, object, tracker_info);

   /** \result
    * pedestrian should be 0; remaining four equal then normalized.
    * Raw remaining each 0.2 => sum 0.8 -> normalized each 0.25.
    */
   DOUBLES_EQUAL(0.0F, apriori.pedestrian, 1e-6F);
   DOUBLES_EQUAL(0.25F, apriori.bicycle, 1e-6F);
   DOUBLES_EQUAL(0.25F, apriori.motorcycle, 1e-6F);
   DOUBLES_EQUAL(0.25F, apriori.car, 1e-6F);
   DOUBLES_EQUAL(0.25F, apriori.truck, 1e-6F);
}

/** \purpose
 * Purpose of this test is to verify pedestrian and bicycle apriori are zeroed when speed exceeds 8 m/s.
 * \req
 * NA.
 */
TEST(calcAprioriProbability, calcAprioriProbability__speed_exceeds_bicycle_threshold)
{
   /** \precond
    * Object speed > 8.0; RCS & cluster benign.
    */
   object.speed = 8.01F;

   /** \action */
   calcAprioriProbability(apriori, object, tracker_info);

   /** \result
    * pedestrian & bicycle zero; motorcycle/car/truck equal then normalized.
    * Raw remaining each 0.2 => sum 0.6 -> normalized each 0.3333333.
    */
   DOUBLES_EQUAL(0.0F, apriori.pedestrian, 1e-6F);
   DOUBLES_EQUAL(0.0F, apriori.bicycle, 1e-6F);
   DOUBLES_EQUAL(1.0F/3.0F, apriori.motorcycle, 1e-6F);
   DOUBLES_EQUAL(1.0F/3.0F, apriori.car, 1e-6F);
   DOUBLES_EQUAL(1.0F/3.0F, apriori.truck, 1e-6F);
}

/** \purpose
 * Purpose of this test is to verify pedestrian bicycle and motorcycle apriori are zeroed when RCS exceeds 12 dBm^2.
 * \req
 * NA.
 */
TEST(calcAprioriProbability, calcAprioriProbability__rcs_exceeds_threshold)
{
   /** \precond
    * Object max RCS > 12.0; speed benign.
    */
   object.speed = 1.0F;
   object.average_rcs = 13.0F; // average higher than 12 triggers zeroing
   object.maximum_rcs = 11.0F; // ensure max picks 13 via fmax

   /** \action */
   calcAprioriProbability(apriori, object, tracker_info);

   /** \result
    * Pedestrian, bicycle and motorcycle zero; others normalized equally (motorcycle, car, truck).
    * Raw remaining each 0.5 => sum 1.0 -> normalized each 1/2.
    */
   DOUBLES_EQUAL(0.0F, apriori.pedestrian, 1e-6F);
   DOUBLES_EQUAL(0.0F, apriori.bicycle, 1e-6F);
   DOUBLES_EQUAL(0.0F, apriori.motorcycle, 1e-6F);
   DOUBLES_EQUAL(1.0F/2.0F, apriori.car, 1e-6F);
   DOUBLES_EQUAL(1.0F/2.0F, apriori.truck, 1e-6F);
}

/** \purpose
 * Purpose of this test is to verify pedestrian apriori is zeroed for cluster size >2 and bicycle & motorcycle are preserved at size 3 (two wheeler factor still 1).
 * \req
 * NA.
 */
TEST(calcAprioriProbability, calcAprioriProbability__cluster_size_three)
{
   /** \precond
    * num_members_in_slow_moving_obj_cluster = 3 (>2 zeros pedestrian, <=3 keeps two wheelers)
    */
   object.speed = 1.0F;
   object.num_members_in_slow_moving_obj_cluster = 3U;

   /** \action */
   calcAprioriProbability(apriori, object, tracker_info);

   /** \result
    * pedestrian zero; bicycle & motorcycle remain 0.2; car & truck 0.2; normalize over sum 0.8 => each 0.25.
    */
   DOUBLES_EQUAL(0.0F, apriori.pedestrian, 1e-6F);
   DOUBLES_EQUAL(0.25F, apriori.bicycle, 1e-6F);
   DOUBLES_EQUAL(0.25F, apriori.motorcycle, 1e-6F);
   DOUBLES_EQUAL(0.25F, apriori.car, 1e-6F);
   DOUBLES_EQUAL(0.25F, apriori.truck, 1e-6F);
}

/** \purpose
 * Purpose of this test is to verify pedestrian & bicycle & motorcycle apriori are zeroed for cluster size >3.
 * \req
 * NA.
 */
TEST(calcAprioriProbability, calcAprioriProbability__cluster_size_four)
{
   /** \precond
    * num_members_in_slow_moving_obj_cluster = 4 (>2 zeros pedestrian, >3 zeros 2wheelers)
    */
   object.speed = 1.0F;
   object.num_members_in_slow_moving_obj_cluster = 4U;

   /** \action */
   calcAprioriProbability(apriori, object, tracker_info);

   /** \result
    * ped, bicycle, motorcycle zero; car & truck normalized -> each 0.5.
    */
   DOUBLES_EQUAL(0.0F, apriori.pedestrian, 1e-6F);
   DOUBLES_EQUAL(0.0F, apriori.bicycle, 1e-6F);
   DOUBLES_EQUAL(0.0F, apriori.motorcycle, 1e-6F);
   DOUBLES_EQUAL(0.5F, apriori.car, 1e-6F);
   DOUBLES_EQUAL(0.5F, apriori.truck, 1e-6F);
}

/** \purpose
 * Purpose of this test is to verify truck apriori reduction using linear equation with saturation around its transition points.
 * \req
 * NA.
 */
TEST(calcAprioriProbability, calcAprioriProbability__truck_scaling_at_bounds)
{
   /** \precond
    * Object speed above upper saturation threshold (speed > 35 m/s) so truck apriori should use minimum factor 0.001.
    * Choose single representative speed 36.0F. Other attributes benign.
    */
   object.speed = 35.01F;

   /** \action
    * Call tested function.
    */
   calcAprioriProbability(apriori, object, tracker_info);

   /** \result
    * Truck raw apriori: 0.2F * 0.001F = 0.0002F. Other raw classes remain 0.2F.
    * Sum raw = 0.8F + 0.0002F = 0.8002F. Normalized pedestrian/bicycle/motorcycle/car = 0.2 / 0.8002. Truck = 0.0002 / 0.8002.
    */
   const float32_t raw_truck = 0.2F * 0.001F;
   const float32_t raw_sum = (0.2F * 4.0F) + raw_truck; // 0.8002F
   const float32_t norm = 1.0F / raw_sum;
   const float32_t exp_truck = raw_truck * norm;

   DOUBLES_EQUAL(exp_truck, apriori.truck, 1e-3F);
}
/** @} */

/** \defgroup filteringAndNormalizationOfProbabilities
 *  @{
 */

/** \brief
 * Test Group of filteringAndNormalizationOfProbabilities() function. Tests verify normalization and fallback path behavior.
 */
TEST_GROUP(filteringAndNormalizationOfProbabilities)
{
   F360_Object_Track_T object; // local test object
   /** \setup
    * Initialize object probabilities to zero to force sum==0 branch in function under test.
    */
   TEST_SETUP()
   {
      object.probability_pedestrian = 0.0F;
      object.probability_bicycle = 0.0F;
      object.probability_motorcycle = 0.0F;
      object.probability_car = 0.0F;
      object.probability_truck = 0.0F;
      object.probability_undet = 0.4F; // choose a mid value to verify it remains unchanged
   }
};

/** \purpose
 * Purpose of this test is to verify that when all class probabilities and criteria_Bayes probabilities are zero,
 * the fallback path (sum==0) is used, sum set to 1.0F, and resulting class probabilities remain zero after normalization.
 * \req
 * NA.
 */
TEST(filteringAndNormalizationOfProbabilities, filteringAndNormalizationOfProbabilities__all_zero_inputs)
{
   /** \precond
    * All object probabilities set to 0.0F. probability_undet set to 0.4F. criteria_Bayes probabilities all 0.0F.
    */
   OBJECT_CLASS_PROBABILITY_T criteria_Bayes; // local zero-initialized struct
   criteria_Bayes.probability_pedestrian = 0.0F;
   criteria_Bayes.probability_bicycle = 0.0F;
   criteria_Bayes.probability_motorcycle = 0.0F;
   criteria_Bayes.probability_car = 0.0F;
   criteria_Bayes.probability_truck = 0.0F;
   criteria_Bayes.probability_unknown = 0.0F;

   /** \action
    * Call filteringAndNormalizationOfProbabilities().
    */
   filteringAndNormalizationOfProbabilities(object, criteria_Bayes);

   /** \result
    * All class probabilities should remain 0.0F (0 filtered + 0 innovation, then multiplied by normalization factor).
    * probability_undet should remain unchanged at 0.4F (function does not modify it).
    */
   DOUBLES_EQUAL(0.0F, object.probability_pedestrian, 1e-6F);
   DOUBLES_EQUAL(0.0F, object.probability_bicycle, 1e-6F);
   DOUBLES_EQUAL(0.0F, object.probability_motorcycle, 1e-6F);
   DOUBLES_EQUAL(0.0F, object.probability_car, 1e-6F);
   DOUBLES_EQUAL(0.0F, object.probability_truck, 1e-6F);
   DOUBLES_EQUAL(0.4F, object.probability_undet, 1e-6F);
}

/** @} */

/** \defgroup evaluateNormalDistribution
 *  @{
 */

/** \brief
 * Test Group of evaluateNormalDistribution() function. Tests verify pdf value computation for edge cases.
 */
TEST_GROUP(evaluateNormalDistribution)
{
   TEST_SETUP() { }
};

/** \purpose
 * Purpose of this test is to verify that when standard_deviation is below threshold it is replaced by 1.0F and
 * the returned pdf value equals 1/sqrt(2*pi) for value==mean==0.
 * \req
 * NA.
 */
TEST(evaluateNormalDistribution, evaluateNormalDistribution__stddev_below_threshold)
{
   /** \precond
    * value = 0, mean = 0, standard_deviation passed as 0 < threshold (1e-10).
    */
   const float32_t value = 0.0F;
   const float32_t mean = 0.0F;
   float32_t inv_stddev = 1e11F; // triggers branch

   /** \action
    * Call evaluateNormalDistribution().
    */
   const float32_t pdf = evaluateNormalDistribution(value, mean, inv_stddev);

   /** \result
    * Expected pdf = 1 / sqrt(2*pi) ~ 0.39894228.
    */
   DOUBLES_EQUAL(0.39894228F, pdf, 1e-6F);
}

/** @} */

/** \defgroup calcProbability_Criteria_Bayes
 *  @{
 */

/** \brief
 * Test Group of calcProbability_Criteria_Bayes() function. Tests verify Bayes probability computation paths.
 */
TEST_GROUP(calcProbability_Criteria_Bayes)
{
   F360_Object_Track_T object;
   F360_Calibrations_T calib; // only selected fields will be set
   /** \setup
    * Initialize object and calibration values used by pdf evaluation; set bbox length to arbitrary value.
    */
   TEST_SETUP()
   {
      object.bbox.Set_Length(2.0F); // length used in evaluateNormalDistribution
      // Set calibration means & std dev to nominal non-zero values
      calib.k_ad_oc_mean_length_pedestrian = 1.0F;
      calib.k_ad_oc_inv_standard_deviation_length_pedestrian = 1.0F / 0.5F;
      calib.k_ad_oc_mean_length_2wheel = 1.8F;
      calib.k_ad_oc_inv_standard_deviation_length_2wheel = 1.0F / 0.6F;
      calib.k_ad_oc_mean_length_car = 4.0F;
      calib.k_ad_oc_inv_standard_deviation_length_car = 1.0F / 0.8F;
      calib.k_ad_oc_mean_length_truck = 8.0F;
      calib.k_ad_oc_inv_standard_deviation_length_truck = 1.0F / 1.0F;
   }
};

/** \purpose
 * Purpose of this test is to verify that when all apriori probabilities are zero the sum of weighted pdfs is zero,
 * triggering the branch that sets sum to 1.0F, yielding zero posterior probabilities.
 * \req
 * NA.
 */
TEST(calcProbability_Criteria_Bayes, calcProbability_Criteria_Bayes__all_apriori_zero_triggers_sum_branch)
{
   /** \precond
    * Set all apriori probabilities to 0.0F.
    */
   OBJ_CLASS_A_PRIORI_PROBABILITIES_T apriori;
   apriori.pedestrian = 0.0F;
   apriori.bicycle = 0.0F;
   apriori.motorcycle = 0.0F;
   apriori.car = 0.0F;
   apriori.truck = 0.0F;

   OBJECT_CLASS_PROBABILITY_T criteria_Bayes;

   /** \action
    * Call calcProbability_Criteria_Bayes() function.
    */
   calcProbability_Criteria_Bayes(apriori, criteria_Bayes, object, calib);

   /** \result
    * All posterior probabilities should be 0.0F due to zero numerator terms; probability_unknown set to 0.0F.
    */
   DOUBLES_EQUAL(0.0F, criteria_Bayes.probability_pedestrian, 1e-6F);
   DOUBLES_EQUAL(0.0F, criteria_Bayes.probability_bicycle, 1e-6F);
   DOUBLES_EQUAL(0.0F, criteria_Bayes.probability_motorcycle, 1e-6F);
   DOUBLES_EQUAL(0.0F, criteria_Bayes.probability_car, 1e-6F);
   DOUBLES_EQUAL(0.0F, criteria_Bayes.probability_truck, 1e-6F);
   DOUBLES_EQUAL(0.0F, criteria_Bayes.probability_unknown, 1e-6F);
}

/** @} */

/** \defgroup calcUndetProb
 *  @{
 */

/** \brief
 * Test Group of calcUndetProb() function. Tests verify probability clamping logic.
 */
TEST_GROUP(calcUndetProb)
{
   F360_Object_Track_T object;
   F360_Calibrations_T calib;
   /** \setup
    * Initialize object with probability_undet above 1.0F and non-new status to exercise decrement then clamp (>1.0F) branch.
    */
   TEST_SETUP()
   {
      object.status = F360_OBJECT_STATUS_UPDATED; // non-new so decrement path taken
      object.probability_undet = 1.5F; // above upper bound
      calib.k_ad_oc_step_decrease_prob_unknown = 0.1F; // small decrement keeps value >1.0F
   }
};

/** \purpose
 * Purpose of this test is to verify that when probability_undet remains > 1.0F after decrement it is clamped to 1.0F (else-if branch).
 * \req
 * NA.
 */
TEST(calcUndetProb, calcUndetProb__clamp_above_one)
{
   /** \precond
    * probability_undet initialized to 1.5F (>1.0F) and status UPDATED; decrement of 0.1F leaves 1.4F (>1.0F).
    */

   /** \action
    * Call calcUndetProb().
    */
   calcUndetProb(object, calib);

   /** \result
    * probability_undet should be clamped to 1.0F.
    */
   DOUBLES_EQUAL(1.0F, object.probability_undet, 1e-6F);
}

/** @} */

/** \defgroup calcProbability
 *  @{
 */

/** \brief
 * Test Group of calcProbability() function. Tests verify branching for delaying classification vs filtering and setting object class.
 */
TEST_GROUP(calcProbability)
{
   F360_Object_Track_T object;
   F360_Calibrations_T calib;
   OBJ_CLASS_A_PRIORI_PROBABILITIES_T apriori;
   /** \setup
    * Initialize calibration parameters used in object class probability logic.
    */
   TEST_SETUP()
   {
      // Length distribution parameters (arbitrary plausible values)
      calib.k_ad_oc_mean_length_pedestrian = 1.0F;
      calib.k_ad_oc_inv_standard_deviation_length_pedestrian = 1.0F / 0.5F;
      calib.k_ad_oc_mean_length_2wheel = 1.8F;
      calib.k_ad_oc_inv_standard_deviation_length_2wheel = 1.0F / 0.6F;
      calib.k_ad_oc_mean_length_car = 4.0F;
      calib.k_ad_oc_inv_standard_deviation_length_car = 1.0F / 0.8F;
      calib.k_ad_oc_mean_length_truck = 8.0F;
      calib.k_ad_oc_inv_standard_deviation_length_truck = 1.0F / 1.0F;

      // Speed thresholds & probability decrease
      calib.k_ad_oc_max_stationary_speed = 1.0F;
      calib.k_ad_oc_prob_decrease = 0.5F; // used in Update_Probability_Undetermined

      // Initialize object baseline probabilities (will be manipulated per test)
      object.probability_pedestrian = 0.2F;
      object.probability_bicycle = 0.2F;
      object.probability_motorcycle = 0.2F;
      object.probability_car = 0.2F;
      object.probability_truck = 0.2F;
      object.probability_undet = 0.2F;
      object.bbox.Set_Length(2.0F);
      object.time_since_started_move = 0.0F;
      object.tang_accel = 0.0F;
      object.length_of_slow_moving_obj_cluster = 0.0F;
      object.num_members_in_slow_moving_obj_cluster = 0U;
      object.movable_prob = 1.0F;
      object.speed = 0.0F;
      object.object_class = F360_OBJ_CLASS_UNDETERMINED;
      object.status = F360_OBJECT_STATUS_UPDATED; // non-new so calcUndetProb will decrement (not used heavily in these tests)
   }
};

/** \purpose
 * Purpose of this test is to verify delay classification branch via VRU takeoff (f_likely_vehicle_takeoff true) causing Update_Probability_Undetermined and selecting UNDETERMINED.
 * \req
 * NA.
 */
TEST(calcProbability, calcProbability__delay_due_to_VRU_takeoff)
{
   /** \precond
    * Apriori sets pedestrian highest; time_since_started_move < 0.55 triggers f_likely_vehicle_takeoff; movable_prob high; speed high enough not stationary.
    */
   apriori.pedestrian = 0.6F; apriori.bicycle = 0.1F; apriori.motorcycle = 0.1F; apriori.car = 0.1F; apriori.truck = 0.1F;
   object.time_since_started_move = 0.3F; // <0.55F
   object.tang_accel = 0.0F; // not needed
   object.speed = 5.0F; // moving
   object.movable_prob = 1.0F; // prevents stationary path
   object.num_members_in_slow_moving_obj_cluster = 1U;
   object.object_class = F360_OBJ_CLASS_UNDETERMINED; // needed for takeoff condition

   /** \action */
   calcProbability(object, apriori, calib);

   /** \result
    * Update_Probability_Undetermined applied: each non-unknown decreased by 0.5 => 0.1; undet becomes 1 - (5*0.1)=0.5. Classification should be UNDETERMINED.
    */
   DOUBLES_EQUAL(0.5F, object.probability_undet, 1e-6F);
   CHECK_TRUE(object.object_class == F360_OBJ_CLASS_UNDETERMINED);
}

/** \purpose
 * Purpose of this test is to verify delay classification branch via moving-in-long-cluster (f_moving_in_long_cluster true) with pedestrian-like max class replaced by motorcycle (speed <8). Classification delayed.
 * \req
 * NA.
 */
TEST(calcProbability, calcProbability__delay_due_to_cluster_VRU)
{
   /** \precond
    * Apriori sets motorcycle highest; speed <8 so possible VRU; cluster length >2.5 and members>0; takeoff conditions false (time_since_started_move >=0.55 & tang_accel low) so delay comes from cluster.
    */
   apriori.pedestrian = 0.05F; apriori.bicycle = 0.05F; apriori.motorcycle = 0.7F; apriori.car = 0.1F; apriori.truck = 0.1F;
   object.speed = 5.0F; // <8 for motorcycle VRU condition
   object.time_since_started_move = 1.0F; // >=0.55
   object.tang_accel = 0.0F; // low
   object.num_members_in_slow_moving_obj_cluster = 2U;
   object.length_of_slow_moving_obj_cluster = 3.0F; // >2.5
   object.movable_prob = 1.0F;

   /** \action */
   calcProbability(object, apriori, calib);

   /** \result
    * Delay branch executed; probabilities decreased & undet increases. Undetermined should dominate.
    */
   CHECK_TRUE(object.object_class == F360_OBJ_CLASS_UNDETERMINED);
   CHECK_TRUE(object.probability_undet > object.probability_motorcycle);
}

/** \purpose
 * Purpose of this test is to verify stationary speed band triggers delay classification (object speed in stationary range and movable_prob <0.5) even when max class is non-VRU.
 * \req
 * NA.
 */
TEST(calcProbability, calcProbability__delay_due_to_stationary_speed_band)
{
   /** \precond
    * Apriori sets truck highest (non-VRU), speed in (min_ped_speed, max_stationary_speed), movable_prob <0.5 triggers stationary speed branch.
    */
   apriori.pedestrian = 0.05F; apriori.bicycle = 0.05F; apriori.motorcycle = 0.05F; apriori.car = 0.2F; apriori.truck = 0.65F;
   object.speed = 0.7F; // inside stationary band
   object.movable_prob = 0.4F; // <0.5
   object.time_since_started_move = 2.0F; // large
   object.tang_accel = 0.0F;

   /** \action */
   calcProbability(object, apriori, calib);

   /** \result
    * Delay branch executed; undetermined should increase and dominate classification.
    */
   CHECK_TRUE(object.object_class == F360_OBJ_CLASS_UNDETERMINED);
   CHECK_TRUE(object.probability_undet > 0.3F);
}

/** \purpose
 * Purpose of this test is to verify filtering path (no delay) when none of delay conditions hold (movable_prob >=0.5, speed outside stationary band, no VRU conditions, no cluster).
 * \req
 * NA.
 */
TEST(calcProbability, calcProbability__filtering_path_normalization)
{
   /** \precond
    * Apriori sets car highest. Speed high (>8) so motorcycle VRU clause false. movable_prob >=0.5. No cluster. time_since_started_move large.
    */
   apriori.pedestrian = 0.05F; apriori.bicycle = 0.05F; apriori.motorcycle = 0.4F; apriori.car = 0.35F; apriori.truck = 0.15F; // motorcycle likely highest but speed >8 makes f_possible_VRU false
   object.speed = 12.0F; // >8
   object.movable_prob = 0.8F; // >=0.5
   object.time_since_started_move = 5.0F;
   object.tang_accel = 0.0F;
   object.num_members_in_slow_moving_obj_cluster = 0U;
   object.length_of_slow_moving_obj_cluster = 0.0F;

   /** \action */
   calcProbability(object, apriori, calib);

   /** \result
    * Filtering path executed; object class should be one of non-undetermined (motorcycle expected). probability_undet unchanged (0.2F).
    */
   CHECK_TRUE(object.object_class == F360_OBJ_CLASS_MOTORCYCLE);
   DOUBLES_EQUAL(0.2F, object.probability_undet, 1e-5F);
}

/** \purpose
 * Purpose of this test is to verify classification remains UNDETERMINED when apriori probabilities are all zero (criteria_Bayes all zero) and filtering path chosen.
 * \req
 * NA.
 */
TEST(calcProbability, calcProbability__undetermined_due_to_zero_apriori)
{
   /** \precond
    * All apriori zero -> criteria_Bayes probabilities zero. Movable_prob high & speed high to avoid delay; initial probabilities set low except undet=0.9.
    */
   apriori.pedestrian = 0.0F; apriori.bicycle = 0.0F; apriori.motorcycle = 0.0F; apriori.car = 0.0F; apriori.truck = 0.0F;
   object.probability_pedestrian = 0.01F;
   object.probability_bicycle = 0.01F;
   object.probability_motorcycle = 0.01F;
   object.probability_car = 0.01F;
   object.probability_truck = 0.01F;
   object.probability_undet = 0.9F; // dominant
   object.speed = 15.0F;
   object.movable_prob = 0.9F;
   object.time_since_started_move = 10.0F;

   /** \action */
   calcProbability(object, apriori, calib);

   /** \result
    * Filtering path (since delay conditions false) retains dominance of undetermined -> classification UNDETERMINED.
    */
   CHECK_TRUE(object.object_class == F360_OBJ_CLASS_UNDETERMINED);
   CHECK_TRUE(object.probability_undet >= 0.9F);
}

/** \purpose
 * Purpose of this test is to verify delay classification branch via high tangential acceleration takeoff (second OR leg true) causing Update_Probability_Undetermined and selecting UNDETERMINED.
 * \req
 * NA.
 */
TEST(calcProbability, calcProbability__delay_due_to_high_accel_takeoff)
{
   // Precondition: pedestrian highest apriori; time_since_started_move >=0.55 so first OR leg false; tang_accel >0.5 makes second OR leg true; object currently UNDETERMINED.
   apriori.pedestrian = 0.5F; apriori.bicycle = 0.1F; apriori.motorcycle = 0.1F; apriori.car = 0.2F; apriori.truck = 0.1F;
   object.time_since_started_move = 0.80F; // >=0.55F ensures first OR leg false
   object.tang_accel = 0.6F; // >0.5F triggers high acceleration takeoff branch
   object.speed = 6.0F; // moving
   object.movable_prob = 1.0F; // avoid stationary path
   object.object_class = F360_OBJ_CLASS_UNDETERMINED; // required for f_likely_vehicle_takeoff

   /** \action */
   calcProbability(object, apriori, calib);

   /** \result
    * delay classification path chosen -> Update_Probability_Undetermined used raising undet probability over others
    */
   CHECK_TRUE(object.object_class == F360_OBJ_CLASS_UNDETERMINED);
   CHECK_TRUE(object.probability_undet > 0.3F);
}

/** \purpose
 * Purpose of this test is to verify filtering path when takeoff conditions are true but object_class is known (not UNDETERMINED).
 * \req
 * NA.
 */
TEST(calcProbability, calcProbability__filtering_when_takeoff_conditions_but_class_known)
{
   // Precondition: Early move indicators present but object_class already CAR so f_likely_vehicle_takeoff should be false.
   apriori.pedestrian = 0.45F; apriori.bicycle = 0.05F; apriori.motorcycle = 0.1F; apriori.car = 0.3F; apriori.truck = 0.1F; // pedestrian highest
   object.time_since_started_move = 0.2F; // <0.55F would trigger takeoff if class were UNDETERMINED
   object.tang_accel = 0.7F; // >0.5F also would trigger
   object.speed = 5.0F;
   object.movable_prob = 1.0F;
   object.object_class = F360_OBJ_CLASS_CAR; // not UNDETERMINED => takeoff flag blocked

   calcProbability(object, apriori, calib);

   // Result: filtering path executed -> undet probability unchanged (0.2F) and pedestrian becomes class
   DOUBLES_EQUAL(0.2F, object.probability_undet, 1e-5F);
   CHECK_TRUE(object.object_class == F360_OBJ_CLASS_MOTORCYCLE);
}

/** @}*/

/** \defgroup determineToFreezeVehicularClass
 *  @{
 */

/** \brief
 * Test Group of Determine_To_Freeze_Vehicular_Class() function. Tests verify whether
 * vehicular class freezing is correctly determined based on speed, RCS, and temporal evidence.
 */
TEST_GROUP(determineToFreezeVehicularClass)
{
   F360_Object_Track_T object; // local test object
   F360_Tracker_Info_T tracker_info; // tracker info for elapsed time

   /** \setup
    * Initialize object with neutral values for fields used by Determine_To_Freeze_Vehicular_Class.
    */
   TEST_SETUP()
   {
      object.speed = 0.0F;
      object.average_rcs = 5.0F;
      object.maximum_rcs = 6.0F;
      object.time_since_initialization = 1.0F; // Default > 0.5s for speed consideration
      object.time_since_obj_considered_veh_for_class_freeze = -1.0F; // Default inactive timer
      object.movable_prob = 0.8F; // Default high movable probability
      tracker_info.elapsed_time_s = 0.05F; // Default 50ms cycle time
   }
};

/** \purpose
 * Verify that vehicular class is NOT frozen when speed and RCS are below VRU thresholds.
 * \req
 * NA.
 */
TEST(determineToFreezeVehicularClass, no_freeze_low_speed_low_rcs)
{
   /** \precond
    * Object speed <= 8.0 m/s and RCS <= 12.0 dBm^2, timer inactive (default setup values).
    */
   object.speed = 5.0F; // Below 8.0 m/s threshold
   // average_rcs = 5.0F, maximum_rcs = 6.0F from setup (both below 12.0 dBm^2)
   // time_since_obj_considered_veh_for_class_freeze = -1.0F from setup (inactive)

   /** \action */
   bool result = Determine_To_Freeze_Vehicular_Class(tracker_info, object);

   /** \result
    * Should return false and timer should remain inactive.
    */
   CHECK_FALSE(result);
   DOUBLES_EQUAL(-1.0F, object.time_since_obj_considered_veh_for_class_freeze, 1e-6F);
}

/** \purpose
 * Verify that vehicular class IS frozen when speed exceeds VRU threshold (after 0.5s initialization).
 * \req
 * NA.
 */
TEST(determineToFreezeVehicularClass, freeze_high_speed_after_initialization)
{
   /** \precond
    * Object speed > 8.0 m/s and time_since_initialization > 0.5s (from setup).
    */
   object.speed = 12.0F; // Above 8.0 m/s threshold
   // time_since_initialization = 1.0F from setup (> 0.5s)
   // average_rcs = 5.0F, maximum_rcs = 6.0F from setup (below RCS threshold)

   /** \action */
   bool result = Determine_To_Freeze_Vehicular_Class(tracker_info, object);

   /** \result
    * Should return true and timer should be reset to 0.0.
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(0.0F, object.time_since_obj_considered_veh_for_class_freeze, 1e-6F);
}

/** \purpose
 * Verify that vehicular class is NOT frozen when speed exceeds threshold but initialization time < 0.5s.
 * \req
 * NA.
 */
TEST(determineToFreezeVehicularClass, no_freeze_high_speed_early_initialization)
{
   /** \precond
    * Object speed > 8.0 m/s but time_since_initialization <= 0.5s.
    */
   object.speed = 12.0F; // Above 8.0 m/s threshold
   object.time_since_initialization = 0.3F; // <= 0.5s
   // average_rcs = 5.0F, maximum_rcs = 6.0F from setup (below RCS threshold)
   // time_since_obj_considered_veh_for_class_freeze = -1.0F from setup (inactive)

   /** \action */
   bool result = Determine_To_Freeze_Vehicular_Class(tracker_info, object);

   /** \result
    * Should return false and timer should remain inactive.
    */
   CHECK_FALSE(result);
   DOUBLES_EQUAL(-1.0F, object.time_since_obj_considered_veh_for_class_freeze, 1e-6F);
}

/** \purpose
 * Verify that vehicular class IS frozen when average RCS exceeds VRU threshold.
 * \req
 * NA.
 */
TEST(determineToFreezeVehicularClass, freeze_high_average_rcs)
{
   /** \precond
    * Object average RCS > 12.0 dBm^2, speed below threshold.
    */
   object.speed = 5.0F; // Below speed threshold
   object.average_rcs = 15.0F; // Above 12.0 dBm^2 threshold
   // maximum_rcs = 6.0F from setup (below threshold, but average dominates)

   /** \action */
   bool result = Determine_To_Freeze_Vehicular_Class(tracker_info, object);

   /** \result
    * Should return true and timer should be reset to 0.0.
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(0.0F, object.time_since_obj_considered_veh_for_class_freeze, 1e-6F);
}

/** \purpose
 * Verify that vehicular class IS frozen when maximum RCS exceeds VRU threshold.
 * \req
 * NA.
 */
TEST(determineToFreezeVehicularClass, freeze_high_maximum_rcs)
{
   /** \precond
    * Object maximum RCS > 12.0 dBm^2, speed below threshold.
    */
   object.speed = 5.0F; // Below speed threshold
   // average_rcs = 5.0F from setup (below threshold)
   object.maximum_rcs = 18.0F; // Above 12.0 dBm^2 threshold

   /** \action */
   bool result = Determine_To_Freeze_Vehicular_Class(tracker_info, object);

   /** \result
    * Should return true and timer should be reset to 0.0.
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(0.0F, object.time_since_obj_considered_veh_for_class_freeze, 1e-6F);
}

/** \purpose
 * Verify that vehicular class continues to be frozen while timer is active and within max time.
 * \req
 * NA.
 */
TEST(determineToFreezeVehicularClass, continue_freeze_active_timer_within_limit)
{
   /** \precond
    * Timer is active (>= 0), below max time (30s), and movable_prob > 0.5 (from setup).
    */
   object.speed = 5.0F; // Below thresholds
   // average_rcs = 5.0F, maximum_rcs = 6.0F from setup (below thresholds)
   object.time_since_obj_considered_veh_for_class_freeze = 5.0F; // Active, < 30s
   // movable_prob = 0.8F from setup (> 0.5)
   tracker_info.elapsed_time_s = 0.1F; // 100ms cycle

   /** \action */
   bool result = Determine_To_Freeze_Vehicular_Class(tracker_info, object);

   /** \result
    * Should return true and timer should be incremented by elapsed time.
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(5.1F, object.time_since_obj_considered_veh_for_class_freeze, 1e-6F);
}

/** \purpose
 * Verify that vehicular class freeze expires when timer exceeds maximum allowed time.
 * \req
 * NA.
 */
TEST(determineToFreezeVehicularClass, expire_freeze_timer_exceeds_max)
{
   /** \precond
    * Timer exceeds max time (30s), no confirming speed/RCS.
    */
   object.speed = 5.0F; // Below thresholds
   // average_rcs = 5.0F, maximum_rcs = 6.0F from setup (below thresholds)
   object.time_since_obj_considered_veh_for_class_freeze = 35.0F; // > 30s max
   // movable_prob = 0.8F from setup (> 0.5)

   /** \action */
   bool result = Determine_To_Freeze_Vehicular_Class(tracker_info, object);

   /** \result
    * Should return false and timer should be invalidated (-1.0).
    */
   CHECK_FALSE(result);
   DOUBLES_EQUAL(-1.0F, object.time_since_obj_considered_veh_for_class_freeze, 1e-6F);
}

/** \purpose
 * Verify that vehicular class freeze expires when movable probability is too low.
 * \req
 * NA.
 */
TEST(determineToFreezeVehicularClass, expire_freeze_low_movable_prob)
{
   /** \precond
    * Timer is active but movable_prob <= 0.5.
    */
   object.speed = 5.0F; // Below thresholds
   // average_rcs = 5.0F, maximum_rcs = 6.0F from setup (below thresholds)
   object.time_since_obj_considered_veh_for_class_freeze = 5.0F; // Active, < 30s
   object.movable_prob = 0.3F; // <= 0.5

   /** \action */
   bool result = Determine_To_Freeze_Vehicular_Class(tracker_info, object);

   /** \result
    * Should return false and timer should be invalidated (-1.0).
    */
   CHECK_FALSE(result);
   DOUBLES_EQUAL(-1.0F, object.time_since_obj_considered_veh_for_class_freeze, 1e-6F);
}

/** \purpose
 * Verify timer behavior at exactly the 30s boundary.
 * \req
 * NA.
 */
TEST(determineToFreezeVehicularClass, freeze_timer_at_boundary)
{
   /** \precond
    * Timer is exactly at 30s limit.
    */
   object.speed = 5.0F; // Below thresholds
   // average_rcs = 5.0F, maximum_rcs = 6.0F from setup (below thresholds)
   object.time_since_obj_considered_veh_for_class_freeze = 30.0F; // Exactly at max
   // movable_prob = 0.8F from setup (> 0.5)

   /** \action */
   bool result = Determine_To_Freeze_Vehicular_Class(tracker_info, object);

   /** \result
    * Should return false and timer should be invalidated (-1.0).
    */
   CHECK_FALSE(result);
   DOUBLES_EQUAL(-1.0F, object.time_since_obj_considered_veh_for_class_freeze, 1e-6F);
}

/** \purpose
 * Verify function uses maximum of average_rcs and maximum_rcs for threshold comparison.
 * \req
 * NA.
 */
TEST(determineToFreezeVehicularClass, rcs_uses_maximum_of_both_values)
{
   /** \precond
    * average_rcs > threshold, maximum_rcs < threshold - should still trigger.
    */
   object.speed = 5.0F; // Below speed threshold
   object.average_rcs = 14.0F; // Above 12.0 threshold
   object.maximum_rcs = 8.0F; // Below 12.0 threshold

   /** \action */
   bool result = Determine_To_Freeze_Vehicular_Class(tracker_info, object);

   /** \result
    * Should trigger freeze because fmax(average_rcs, maximum_rcs) = 14.0 > 12.0.
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(0.0F, object.time_since_obj_considered_veh_for_class_freeze, 1e-6F);
}

/** @}*/


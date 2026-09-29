/** \file
 * This file contains unit tests for content of try_to_dealiase_range_rate.cpp file
 */

#include "f360_try_to_dealiase_range_rate.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_try_to_dealiase_range_rate
 *  @{
 */

/** \brief
 * Test group contains common variable declaration for all tests.
 * The sensor properties are defined in this test group and is
 * the same for all tests.
 * To be able to dealias range rates towards a prediction a threshold
 * is needed which is the same for all tests and choosen so that
 * it's easy to derive expected data for different test cases.
 */
TEST_GROUP(f360_try_to_dealiase_range_rate)
{	
   float32_t rdot;
   float32_t rdot_pred;
   float32_t threshold;
   float32_t rdot_interval_width;
   float32_t min_aliased_rdot;
   float32_t rdot_dealiased;
   float32_t interval;
   bool f_dealiased;

   float32_t exp_rdot_dealiased;
   float32_t exp_interval;
   bool exp_f_dealiased;

   float32_t test_pass_thres = 0.00001F;
   
   /** \setup
    * Setup arbitrary sensor settings
    */
   TEST_SETUP()
   {
      rdot_interval_width = 30.0F;
      min_aliased_rdot = -20.0F;
      threshold = 2.5F;
   }
   
};

/** \purpose  
 * Verify that range rate is dealiased correctly when measured range rate and predicted range rate is 
 * in the base interval.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate, Try_To_Dealiase_Range_Rate_Test_Base_Interval)
{
   /** \precond
    * Set range rate and predicted range rate to 0
    */
   rdot = 0.0F;
   rdot_pred = 0.0F;

   exp_rdot_dealiased = 0.0F;
   exp_interval = 0.0F;
   exp_f_dealiased = true;
	
   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate(rdot, rdot_pred, threshold, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * in the second lower interval and measured range rate matches prediction
 * and has been aliased.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate, Try_To_Dealiase_Range_Rate_Test_Second_Lower_Interval)
{
   /** \precond
    * Set predicted range rate in the second lower interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the second lower interval.
    */
   rdot = 0.0F;
   rdot_pred = -60.0F;

   exp_rdot_dealiased = -60.0F;
   exp_interval = -2.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate(rdot, rdot_pred, threshold, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * in the third upper interval and measured range rate matches prediction
 * and has been aliased.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate, Try_To_Dealiase_Range_Rate_Test_Third_Upper_Interval)
{
   /** \precond
    * Set predicted range rate in the third upper interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the third upper interval.
    */
   rdot = -10.0F;
   rdot_pred = 80.0F;
   exp_f_dealiased = true;

   exp_rdot_dealiased = 80.0F;
   exp_interval = 3.0F;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate(rdot, rdot_pred, threshold, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * in the second upper interval and close to the interval limit.
 * Measured range rate is in the first interval and close to the interval limit.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate, Try_To_Dealiase_Range_Rate_Test_Between_First_And_Second_Upper_Pred_In_Second_Upper)
{
   /** \precond
    * Set predicted range rate in the second upper interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the first upper interval.
    */
   rdot = 9.0F;
   rdot_pred = 41.0F;

   exp_rdot_dealiased = 39.0F;
   exp_interval = 1.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate(rdot, rdot_pred, threshold, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * in the first upper interval and close to the second interval.
 * Measured range rate is in the second interval and close to the interval limit.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate, Try_To_Dealiase_Range_Rate_Test_Between_First_And_Second_Upper_Pred_In_First_Upper)
{
   /** \precond
    * Set predicted range rate in the first upper interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the second upper interval.
    */
   rdot = 11.0F;
   rdot_pred = 69.0F;

   exp_rdot_dealiased = 71.0F;
   exp_interval = 2.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate(rdot, rdot_pred, threshold, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * in the fifth lower interval and close to the fourth interval.
 * Measured range rate is in the fourth lower interval and close to the interval limit.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate, Try_To_Dealiase_Range_Rate_Test_Between_Fourth_And_Fifth_Lower_Pred_In_Fifth_Lower)
{
   /** \precond
    * Set predicted range rate in the fifth lower interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the fourth lower interval.
    */
   rdot = -19.0F;
   rdot_pred = -141.0F;

   exp_rdot_dealiased = -139.0F;
   exp_interval = -4.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate(rdot, rdot_pred, threshold, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * in the fourth lower interval and close to the fifth interval.
 * Measured range rate is in the fifth lower interval and close to the interval limit.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate, Try_To_Dealiase_Range_Rate_Test_Between_Fourth_And_Fifth_Lower_Pred_In_Fourth_Lower)
{
   /** \precond
    * Set predicted range rate in the fifth lower interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the fourth lower interval.
    */
   rdot = 9.0F;
   rdot_pred = -139.0F;

   exp_rdot_dealiased = -141.0F;
   exp_interval = -5.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate(rdot, rdot_pred, threshold, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * in the second upper interval and close to the first upper interval.
 * Measured range rate is in the second upper interval and close to the first interval limit.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate, Try_To_Dealiase_Range_Rate_Test_Between_First_And_Second_Upper_Both_In_Second)
{
   /** \precond
    * Set predicted range rate in the second lower interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the second lower interval.
    */
   rdot = -19.0F;
   rdot_pred = 41.0F;

   exp_rdot_dealiased = 41.0F;
   exp_interval = 2.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate(rdot, rdot_pred, threshold, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * exactly on the interval limit between first and second lower interval.
 * Measured range rate is in the second lower interval and close to the first interval limit.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate, Try_To_Dealiase_Range_Rate_Test_Pred_Exactly_On_Limit)
{
   /** \precond
    * Set predicted range rate on the interval limit between first and second 
    * upper interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the second upper interval.
    */
   rdot = 9.0F;
   rdot_pred = -50.0F;

   exp_rdot_dealiased = -51.0F;
   exp_interval = -2.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate(rdot, rdot_pred, threshold, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * exactly on the interval limit between first and second lower interval.
 * Measured range rate exactly on the min aliased range rate of the sensor.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate, Try_To_Dealiase_Range_Rate_Test_Both_Pred_And_Meas_Exactly_On_Limit)
{
   /** \precond
    * Set predicted range rate on the interval limit between first and second
    * lower interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the first lower interval.
    */
   rdot = min_aliased_rdot;
   rdot_pred = -50.0F;

   exp_rdot_dealiased = -50.0F;
   exp_interval = -1.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate(rdot, rdot_pred, threshold, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is not dealiased when predicted range rate
 * doesn't match measured dealiased range rate.
 * Prediction in base interval and measured also in base interval but
 * not matching prediction.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate, Try_To_Dealiase_Range_Rate_Test_Pred_Not_Matching_Base)
{
   /** \precond
    * Set predicted range rate in the base interval.
    * Set measured range rate in the base interval but different
    * from prediction
    */
   rdot = 8.0F;
   rdot_pred = -15.0F;

   exp_f_dealiased = false;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate(rdot, rdot_pred, threshold, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
}

/** \purpose
 * Verify that range rate is not dealiased when predicted range rate
 * doesn't match measured dealiased range rate.
 * Prediction in first upper interval and measured in base interval and
 * does not match prediction.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate, Try_To_Dealiase_Range_Rate_Test_Pred_Not_Matching_First_Upper)
{
   /** \precond
    * Set predicted range rate in first upper interval.
    * Set measured range rate in the base interval but different
    * from prediction
    */
   rdot = 8.0F;
   rdot_pred = 15.0F;

   exp_f_dealiased = false;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate(rdot, rdot_pred, threshold, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
}

/** @}*/

/** \defgroup  f360_try_to_dealiase_range_rate_non_symmetric
 *  @{
 */

/** \brief
 * Test group contains common variable declaration for all tests 
 * of function Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval().
 * The sensor properties are defined in this test group and is
 * the same for all tests.
 * To be able to dealias range rates towards a prediction a threshold
 * is needed. This is set to the same for all tests and choosen so that
 * it's easy to derive expected data for different test cases.
 */
TEST_GROUP(f360_try_to_dealiase_range_rate_non_symmetric)
{	
   float32_t rdot;
   float32_t rdot_pred;
   float32_t threshold_lower;
   float32_t threshold_upper;
   float32_t rdot_interval_width;
   float32_t min_aliased_rdot;
   float32_t rdot_dealiased;
   float32_t interval;
   bool f_dealiased;

   float32_t exp_rdot_dealiased;
   float32_t exp_interval;
   bool exp_f_dealiased;

   float32_t test_pass_thres = 0.00001F;
   
   /** \setup
    * Setup arbitrary sensor settings
    */
   TEST_SETUP()
   {
      rdot_interval_width = 30.0F;
      min_aliased_rdot = -20.0F;
      threshold_lower = -3.0F;
      threshold_upper = 2.0F;
   }
   
};

/** \purpose  
 * Verify that range rate is dealiased correctly when measured range rate and predicted range rate is 
 * in the base interval.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate_non_symmetric, Try_To_Dealiase_Range_Rate_Test_Base_Interval)
{
   /** \precond
    * Set range rate and predicted range rate to 0
    */
   rdot = 0.0F;
   rdot_pred = 0.0F;

   exp_rdot_dealiased = 0.0F;
   exp_interval = 0.0F;
   exp_f_dealiased = true;
	
   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(rdot, rdot_pred, threshold_lower, threshold_upper, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * in the second lower interval and measured range rate matches prediction
 * and has been aliased.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate_non_symmetric, Try_To_Dealiase_Range_Rate_Test_Second_Lower_Interval)
{
   /** \precond
    * Set predicted range rate in the second lower interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the second lower interval.
    */
   rdot = 0.0F;
   rdot_pred = -60.0F;

   exp_rdot_dealiased = -60.0F;
   exp_interval = -2.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(rdot, rdot_pred, threshold_lower, threshold_upper, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * in the third upper interval and measured range rate matches prediction
 * and has been aliased.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate_non_symmetric, Try_To_Dealiase_Range_Rate_Test_Third_Upper_Interval)
{
   /** \precond
    * Set predicted range rate in the third upper interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the third upper interval.
    */
   rdot = -10.0F;
   rdot_pred = 80.0F;

   exp_rdot_dealiased = 80.0F;
   exp_interval = 3.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(rdot, rdot_pred, threshold_lower, threshold_upper, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * in the second upper interval and close to the interval limit.
 * Measured range rate is in the first interval and close to the interval limit.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate_non_symmetric, Try_To_Dealiase_Range_Rate_Test_Between_First_And_Second_Upper_Pred_In_Second_Upper)
{
   /** \precond
    * Set predicted range rate in the second upper interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the first upper interval.
    */
   rdot = 9.5F;
   rdot_pred = 41.0F;

   exp_rdot_dealiased = 39.5F;
   exp_interval = 1.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(rdot, rdot_pred, threshold_lower, threshold_upper, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * in the first upper interval and close to the second interval.
 * Measured range rate is in the second interval and close to the interval limit.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate_non_symmetric, Try_To_Dealiase_Range_Rate_Test_Between_First_And_Second_Upper_Pred_In_First_Upper)
{
   /** \precond
    * Set predicted range rate in the first upper interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the second upper interval.
    */
   rdot = -19.0F;
   rdot_pred = 39.5F;

   exp_rdot_dealiased = 41.0F;
   exp_interval = 2.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(rdot, rdot_pred, threshold_lower, threshold_upper, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * in the fifth lower interval and close to the fourth interval.
 * Measured range rate is in the fourth lower interval and close to the interval limit.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate_non_symmetric, Try_To_Dealiase_Range_Rate_Test_Between_Fourth_And_Fifth_Lower_Pred_In_Fifth_Lower)
{
   /** \precond
    * Set predicted range rate in the fifth lower interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the fourth lower interval.
    */
   rdot = -18.5F;
   rdot_pred = -141.0F;

   exp_rdot_dealiased = -138.5F;
   exp_interval = -4.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(rdot, rdot_pred, threshold_lower, threshold_upper, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * in the fourth lower interval and close to the fifth interval.
 * Measured range rate is in the fifth lower interval and close to the interval limit.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate_non_symmetric, Try_To_Dealiase_Range_Rate_Test_Between_Fourth_And_Fifth_Lower_Pred_In_Fourth_Lower)
{
   /** \precond
    * Set predicted range rate in the fifth lower interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the fourth lower interval.
    */
   rdot = 9.0F;
   rdot_pred = -139.5F;

   exp_rdot_dealiased = -141.0F;
   exp_interval = -5.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(rdot, rdot_pred, threshold_lower, threshold_upper, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * in the second upper interval and close to the first upper interval.
 * Measured range rate is in the second upper interval and close to the first interval limit.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate_non_symmetric, Try_To_Dealiase_Range_Rate_Test_Between_First_And_Second_Upper_Both_In_Second)
{
   /** \precond
    * Set predicted range rate in the second upper interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the second lower interval.
    */
   rdot = -19.0F;
   rdot_pred = 12.5F;

   exp_rdot_dealiased = 11.0F;
   exp_interval = 1.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(rdot, rdot_pred, threshold_lower, threshold_upper, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * exactly on the interval limit between first and second lower interval.
 * Measured range rate is in the second lower interval and close to the first interval limit.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate_non_symmetric, Try_To_Dealiase_Range_Rate_Test_Pred_Exactly_On_Limit)
{
   /** \precond
    * Set predicted range rate on the interval limit between first and second 
    * lower interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the second lower interval.
    */
   rdot = 8.5F;
   rdot_pred = -50.0F;

   exp_rdot_dealiased = -51.5F;
   exp_interval = -2.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(rdot, rdot_pred, threshold_lower, threshold_upper, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is dealiased correctly when predicted range rate is
 * exactly on the interval limit between first and second lower interval.
 * Measured range rate exactly on the min aliased range rate of the sensor.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate_non_symmetric, Try_To_Dealiase_Range_Rate_Test_Both_Pred_And_Meas_Exactly_On_Limit)
{
   /** \precond
    * Set predicted range rate on the interval limit between first and second
    * lower interval.
    * Set measured range rate to match measured aliased range rate
    * if the true range rate was in the first lower interval.
    */
   rdot = min_aliased_rdot;
   rdot_pred = -50.0F;

   exp_rdot_dealiased = -50.0F;
   exp_interval = -1.0F;
   exp_f_dealiased = true;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(rdot, rdot_pred, threshold_lower, threshold_upper, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
   DOUBLES_EQUAL(exp_rdot_dealiased, rdot_dealiased, test_pass_thres);
   DOUBLES_EQUAL(exp_interval, interval, test_pass_thres);
}

/** \purpose
 * Verify that range rate is not dealiased when predicted range rate
 * doesn't match measured dealiased range rate.
 * Prediction in base interval and measured also in base interval but
 * not matching prediction. Difference is set such that dealiasing is 
 * prevented by the lower range rate threshold.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate_non_symmetric, Try_To_Dealiase_Range_Rate_Test_Pred_Not_Matching_Base_Low_Th)
{
   /** \precond
    * Set predicted range rate in the base interval.
    * Set measured range rate in the base interval but different
    * from prediction
    */
   rdot = 6.0F;
   rdot_pred = 2.5F;

   exp_f_dealiased = false;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(rdot, rdot_pred, threshold_lower, threshold_upper, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
}

/** \purpose
 * Verify that range rate is not dealiased when predicted range rate
 * doesn't match measured dealiased range rate.
 * Prediction in base interval and measured also in base interval but
 * not matching prediction. Difference is set such that dealiasing is 
 * prevented by the upper range rate threshold.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate_non_symmetric, Try_To_Dealiase_Range_Rate_Test_Pred_Not_Matching_Base_Upper_Th)
{
   /** \precond
    * Set predicted range rate in the base interval.
    * Set measured range rate in the base interval but different
    * from prediction
    */
   rdot = 6.0F;
   rdot_pred = 8.5F;

   exp_f_dealiased = false;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(rdot, rdot_pred, threshold_lower, threshold_upper, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
}

/** \purpose
 * Verify that range rate is not dealiased when predicted range rate
 * doesn't match measured dealiased range rate.
 * Prediction in first upper interval and measured in base interval and
 * does not match prediction. Difference is set such that dealiasing is 
 * prevented by the lower range rate threshold.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate_non_symmetric, Try_To_Dealiase_Range_Rate_Test_Pred_Not_Matching_First_Upper_Low_Th)
{
   /** \precond
    * Set predicted range rate in first upper interval.
    * Set measured range rate in the base interval but different
    * from prediction
    */
   rdot = 6.0F;
   rdot_pred = 32.5F;

   exp_f_dealiased = false;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(rdot, rdot_pred, threshold_lower, threshold_upper, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
}

/** \purpose
 * Verify that range rate is not dealiased when predicted range rate
 * doesn't match measured dealiased range rate.
 * Prediction in first upper interval and measured in base interval and
 * does not match prediction. Difference is set such that dealiasing is 
 * prevented by the upper range rate threshold.
 * \req
 * NA
 */
TEST(f360_try_to_dealiase_range_rate_non_symmetric, Try_To_Dealiase_Range_Rate_Test_Pred_Not_Matching_First_Upper_Upper_Th)
{
   /** \precond
    * Set predicted range rate in first upper interval.
    * Set measured range rate in the base interval but different
    * from prediction
    */
   rdot = 6.0F;
   rdot_pred = 38.5F;

   exp_f_dealiased = false;

   /** \action
    * Call function
    */
   f_dealiased = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(rdot, rdot_pred, threshold_lower, threshold_upper, rdot_interval_width, min_aliased_rdot, rdot_dealiased, interval);

   /** \result
    * Compare against expected data
    */
   CHECK_EQUAL(exp_f_dealiased, f_dealiased);
}

/** @}*/

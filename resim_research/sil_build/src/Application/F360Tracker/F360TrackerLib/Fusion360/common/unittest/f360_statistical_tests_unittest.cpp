/** \file
 * This file contains unit tests for content of f360_statistical_tests.cpp file
 */

#include "f360_statistical_tests.h"
#include <CppUTest/TestHarness.h>
#include "f360_constants.h"

using namespace f360_variant_A;

/** \defgroup  f360_statistical_tests
 *  @{
 */

/** \brief
 * Test group of f360_statistical_tests.cpp file content. Tests verify whether statistical tests
 * are properly performed.
 */
TEST_GROUP(f360_statistical_tests)
{	

};

/** \purpose  
 * Purpose of this test is to verify whether F360_Chi_Square_Test_Two_Degree_P_Value properly calculates p_value
 * \req
 * NA.
 */
TEST(f360_statistical_tests, F360_Chi_Square_Test_Two_Degree_P_Value__p_value_properly_calculated)
{
   /** \precond
    * Set up test variable
    * Set up maximum test variable
    * Set up expected probability value
    */
   float32_t test_variable = 0.1F;
   float32_t maximum_test_variable = 10.0F;
   const float32_t expected_p_value = 0.951229453F;
	
   /** \action
    * Call tested function
    */
   const float32_t p_value = F360_Chi_Square_Test_Two_Degree_P_Value(test_variable, maximum_test_variable);
   /** \result
    * Check whether calculated probability value is equal to expected value.
    */	
   DOUBLES_EQUAL(expected_p_value, p_value, F360_EPSILON);

}

/** \purpose
 * Purpose of this test is to verify whether F360_Chi_Square_Test_Two_Degree_P_Value properly calculates p_value
 * when its above 1 and should be saturated.
 * \req
 * NA.
 */
TEST(f360_statistical_tests, F360_Chi_Square_Test_Two_Degree_P_Value__p_value_properly_saturated)
{
   /** \precond
    * Set up test variable such that initially calculated p_value is > 1
    * Set up maximum test variable > test variable
    * Set up expected probability value to 1
    */
   float32_t test_variable = -0.1F;
   float32_t maximum_test_variable = 10.0F;
   const float32_t expected_p_value = 1.0F;

   /** \action
    * Call tested function
    */
   const float32_t p_value = F360_Chi_Square_Test_Two_Degree_P_Value(test_variable, maximum_test_variable);
   /** \result
    * Check whether calculated probability value is equal to expected value.
    */
   DOUBLES_EQUAL(expected_p_value, p_value, F360_EPSILON);

}

/** \purpose
* Purpose of this test is to verify whether when test variable is greater than maximum test variable, minimum probability is returned
* \req
* NA.
*/
TEST(f360_statistical_tests, F360_Chi_Square_Test_Two_Degree_P_Value__test_variable_greater_than_max_test_variable)
{
   /** \precond
   * Set up test variable
   * Set up maximum test variable
   * Set up expected probability value
   */
   const float32_t test_variable = 100.0F;
   const float32_t maximum_test_variable = 10.0F;
   const float32_t expected_p_value = 0.0F;

   /** \action
   * Call tested function
   */
   const float32_t p_value = F360_Chi_Square_Test_Two_Degree_P_Value(test_variable, maximum_test_variable);
   /** \result
   * Check whether calculated probability value is equal to expected value.
   */
   DOUBLES_EQUAL(expected_p_value, p_value, F360_EPSILON);

}

/** \purpose
* Purpose of this test is to verify whether test properly rejects hypothesis under test
* when p_value is below given significance level alpha
* \req
* NA.
*/
TEST(f360_statistical_tests, F360_Try_To_Reject_Test__properly_rejects_hypothesis)
{
   /** \precond
   * Set up p_value
   * Set up alpha level
   */
   const float32_t p_value = 0.5F;
   const float32_t alpha_level = 0.65F;

   /** \action
   * Call tested function
   */
   const bool test_result  = F360_Try_To_Reject_Test(p_value, alpha_level);
   /** \result
   * Check whether test_result is true
   */
   CHECK_TRUE(test_result);
}

/** \purpose
* Purpose of this test is to verify whether test properly does not reject hypothesis 
* under test when p_value is greater or equal given significance level alpha
* \req
* NA.
*/
TEST(f360_statistical_tests, F360_Try_To_Reject_Test__properly_does_not_reject_hypothesis)
{
   /** \precond
   * Set up p_value
   * Set up alpha level
   */
   const float32_t p_value = 0.7F;
   const float32_t alpha_level = 0.65F;

   /** \action
   * Call tested function
   */
   const bool test_result = F360_Try_To_Reject_Test(p_value, alpha_level);
   /** \result
   * Check whether test_result is false
   */
   CHECK_FALSE(test_result);
}

/** \purpose
* Purpose of this test is to verify whether standard deviation value equal to zero is properly handled
* \req
* NA.
*/
TEST(f360_statistical_tests, Check_Stationary_Hypothesis__handles_std_equal_to_zero)
{
   /** \precond
   * Set up abs test val
   * Set up test value std
   * Set up test value threshold
   * Set up sigma threshold
   */
   const float32_t abs_test_val = 10.0F;
   const float32_t test_val_std = 0.0F;
   const float32_t test_val_th = 1.0F;
   const float32_t sigma_th = 3.0F;

   /** \action
   * Call tested function
   */
   const bool test_result = Check_Stationary_Hypothesis(abs_test_val, test_val_std, test_val_th, sigma_th);
   /** \result
   * Check whether test_result is false
   */
   CHECK_FALSE(test_result);
}

/** \purpose
* Purpose of this test is to verify whether when sigma is above threshold and test value is above threshold
* function returns false
* \req
* NA.
*/
TEST(f360_statistical_tests, Check_Stationary_Hypothesis__sigma_and_test_variable_above_thresholds)
{
   /** \precond
   * Set up abs test val
   * Set up test value std
   * Set up test value threshold
   * Set up sigma threshold
   */
   const float32_t abs_test_val = 10.0F;
   const float32_t test_val_std = 1.0F;
   const float32_t test_val_th = 1.0F;
   const float32_t sigma_th = 3.0F;

   /** \action
   * Call tested function
   */
   const bool test_result = Check_Stationary_Hypothesis(abs_test_val, test_val_std, test_val_th, sigma_th);
   /** \result
   * Check whether test_result is false
   */
   CHECK_FALSE(test_result);
}

/** \purpose
* Purpose of this test is to verify whether when sigma is above threshold and test variable is below threshold
* function returns true
* \req
* NA.
*/
TEST(f360_statistical_tests, Check_Stationary_Hypothesis__sigma_above_threshold_test_variable_below_threshold)
{
   /** \precond
   * Set up abs test val
   * Set up test value std
   * Set up test value threshold
   * Set up sigma threshold
   */
   const float32_t abs_test_val = 10.0F;
   const float32_t test_val_std = 1.0F;
   const float32_t test_val_th = 20.0F;
   const float32_t sigma_th = 3.0F;

   /** \action
   * Call tested function
   */
   const bool test_result = Check_Stationary_Hypothesis(abs_test_val, test_val_std, test_val_th, sigma_th);
   /** \result
   * Check whether test_result is true
   */
   CHECK_TRUE(test_result);
}

/** \purpose
* Purpose of this test is to verify whether when sigma is below threshold and test variable is above threshold
* function returns true
* \req
* NA.
*/
TEST(f360_statistical_tests, Check_Stationary_Hypothesis__sigma_below_threshold_test_variable_above_threshold)
{
   /** \precond
   * Set up abs test val
   * Set up test value std
   * Set up test value threshold
   * Set up sigma threshold
   */
   const float32_t abs_test_val = 10.0F;
   const float32_t test_val_std = 20.0F;
   const float32_t test_val_th = 1.0F;
   const float32_t sigma_th = 3.0F;

   /** \action
   * Call tested function
   */
   const bool test_result = Check_Stationary_Hypothesis(abs_test_val, test_val_std, test_val_th, sigma_th);

   /** \result
   * Check whether test_result is true
   */
   CHECK_TRUE(test_result);
}
/** @}*/

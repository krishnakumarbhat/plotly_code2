/*===================================================================================*\
* FILE:  f360_statistical_tests.h
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential – Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains implementation of statistical tests functions.
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#include "f360_statistical_tests.h"
#include "f360_math_func.h"
namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: F360_Chi_Square_Test_Two_Degree_P_Value()
   *===========================================================================
   * RETURN VALUE:
   * float32_t p_value - probability of test_variable passing test
   *
   * PARAMETERS:
   * const float32_t test_variable - test variable value
   * const float32_t max_test_variable - maximum value of test variable
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Calculate p-value of test variable with chi-square distribution with two
   * degree of freedom
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t F360_Chi_Square_Test_Two_Degree_P_Value(
      const float32_t test_variable,
      const float32_t max_test_variable)
   {
      const float32_t min_p_value = 0.0F;
      
      float32_t p_value;

      if (max_test_variable < test_variable)
      {
         p_value = min_p_value;
      }
      else
      {
         p_value = F360_Expf((-test_variable) * 0.5F);
         
         const float32_t max_p_value = 1.0F;
         p_value = F360_Saturate(p_value, min_p_value, max_p_value);
      }

      return p_value;
   }


   /*===========================================================================*\
   * FUNCTION: F360_Try_To_Reject_Test
   *===========================================================================
   * RETURN VALUE:
   * bool test result: true - rejected, false - not rejected
   *
   * PARAMETERS:
   * const float32_t p_value
   * const float32_t alpha_level
   * 
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Calculate p-value of test variable with chi-square distribution with two degree of freedom
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool F360_Try_To_Reject_Test(
      const float32_t p_value, 
      const float32_t alpha_level)
   {

      return (p_value < alpha_level);
   }

   /*===========================================================================*\
   * FUNCTION: Check_Stationary_Hypothesis()
   *===========================================================================
   * RETURN VALUE:
   * bool - flag indicating whether stationary test is passed
   *
   * PARAMETERS:
   * const float32_t abs_test_val - absolute value of test variable
   * const float32_t test_val_std - test variable standard deviation
   * const float32_t test_val_th - minimal value of test variable
   * const float32_t sigma_th - threshold of test_val/test_val_std ratio
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function checks stationary hypothesis
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Check_Stationary_Hypothesis(
      const float32_t abs_test_val,
      const float32_t test_val_std,
      const float32_t test_val_th,
      const float32_t sigma_th)
   {
      const float32_t test_val_std_tmp = test_val_std < F360_EPSILON ? F360_EPSILON : test_val_std;

      const float32_t sigma = abs_test_val / test_val_std_tmp;

      const bool f_stationary_suspected = ((sigma <= sigma_th) || (abs_test_val <= test_val_th));

      return f_stationary_suspected;
   }
}



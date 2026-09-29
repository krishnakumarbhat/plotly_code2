/*===================================================================================*\
* FILE:  f360_statistical_tests.h
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential – Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of statistical tests functions.
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#ifndef F360_STATISTICAL_TESTS_H
#define F360_STATISTICAL_TESTS_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   float32_t F360_Chi_Square_Test_Two_Degree_P_Value(
      const float32_t test_variable,
      const float32_t max_test_variable);

   bool Check_Stationary_Hypothesis(
      const float32_t abs_test_val,
      const float32_t test_val_std,
      const float32_t test_val_th,
      const float32_t sigma_th);

   bool F360_Try_To_Reject_Test(
      const float32_t p_value, 
      const float32_t alpha_level);
}
#endif



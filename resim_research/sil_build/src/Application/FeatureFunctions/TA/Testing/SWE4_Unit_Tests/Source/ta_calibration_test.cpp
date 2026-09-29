/**
 * @file ta_calibration_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for TA calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-45227}
 */

#include "ta_calibration_test.hpp"

extern "C"
{
#include "pa_obj_in.h"
#include "ta_core_calibration.c"
#include "ta_update_calibration.h"
}

#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * Test for calibration file. Run test for print function.
 * \uts{CSCSA-45229} \sdd{CSCSA-216784} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Core_Calibration_Test, Ta_Core_Cal_Print__check_that_no_fatal_failure_occures)
{
   /** \arrange Set up variable */
   FILE *p_testfile = stdout;

   /** \action n/a */

   /** \assert Expect no failures. */
   EXPECT_NO_FATAL_FAILURE(Ta_Core_Cal_Print(p_testfile, &ta_cal));
}

/**
 * Test for calibration file. Run test to check basic Ta_Core_Cal_Update_Defaults work.
 * \uts{CSCSA-65067} \sdd{CSCSA-216781} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Core_Calibration_Test, Ta_Core_Cal_Update_Defaults__is_reseting_k_rta_obj_age_min)
{
   /** \arrange n/a */

   /** \action n/a */
   const uint8_t orginal_value = ta_cal.k_rta_obj_age_min;
   ta_cal.k_rta_obj_age_min += 1;
   Ta_Core_Cal_Update_Defaults(&ta_cal);

   /** \assert Expect equal adress. */
   EXPECT_EQ(orginal_value, ta_cal.k_rta_obj_age_min);
}

/**
 * Test for calibration file. Run test to check basic Ta_Update_Core_Cal_By_Core work.
 * \uts{CSCSA-65068} \sdd{CSCSA-216782} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Core_Calibration_Test, Ta_Update_Core_Cal_By_Core__is_setting_k_rta_obj_age_min)
{
   /** \arrange n/a */

   /** \action n/a */
   const uint8_t orginal_value       = ta_cal.k_rta_obj_age_min;
   const uint8_t new_value           = orginal_value + 1;
   Ta_Core_Calibration_T calibration = ta_cal;

   calibration.k_rta_obj_age_min = new_value;

   /** \assert Expect correct values. */
   EXPECT_EQ(orginal_value, ta_cal.k_rta_obj_age_min);
   boolean_T result = Ta_Update_Core_Cal_By_Core(&ta_cal, &calibration);
   ASSERT_TRUE(result);
   EXPECT_EQ(new_value, ta_cal.k_rta_obj_age_min);
}

/**
 * Test for calibration file. Run test to check error handing in Ta_Update_Core_Cal_By_Core.
 * \uts{CSCSA-65069} \sdd{CSCSA-216782} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Core_Calibration_Test, Ta_Update_Core_Cal_By_Core__k_rta_obj_age_min_error_handing)
{
   /** \arrange n/a */

   /** \action n/a */
   const uint8_t orginal_value       = ta_cal.k_rta_obj_age_min;
   const uint8_t new_value           = TA_MAX_K_RTA_OBJ_AGE_MIN + 1;
   Ta_Core_Calibration_T calibration = ta_cal;

   calibration.k_rta_obj_age_min = new_value;

   /** \assert Expect correct values. */
   EXPECT_EQ(orginal_value, ta_cal.k_rta_obj_age_min);
   boolean_T result = Ta_Update_Core_Cal_By_Core(&ta_cal, &calibration);
   ASSERT_FALSE(result);
   EXPECT_EQ(orginal_value, ta_cal.k_rta_obj_age_min);
}

#endif /* CT_ACTIVATE_CAL_PRINT */

/**
 * @file ccta_calibration_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for CTA calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-277495}
 */

#include "cta_customer_calibration_test.hpp"

extern "C"
{
#include "cta_customer_calibration.c"
#include "cta_customer_calibration_check.h"
#include "cta_update_calibration.h"
#include "pa_obj_in.h"
}


#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * Test for calibration file. Run test for print function.
 * \uts{CSCSA-277496} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Customer_Calibration_Test, Cta_Customer_Calibration_Print__check_that_no_fatal_failure_occures)
{
   /** \arrange Set up variable */
   FILE *p_testfile = stdout;

   /** \action n/a */

   /** \assert Expect no failures. */
   EXPECT_NO_FATAL_FAILURE(Cta_Customer_Cal_Print(p_testfile, &cta_cal));
}

/**
 * Test for calibration file. Run test to check basic Cta_Update_Cal_Defaults work.
 * \uts{CSCSA-277497} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Customer_Calibration_Test, Cta_Customer_Calibration_Update_Defaults__is_reseting_k_cta_cycle_count_hold_true_warning)
{
   /** \arrange n/a */

   /** \action n/a */
   /** \assert Expect equal adress. */
   ASSERT_NO_FATAL_FAILURE(Cta_Customer_Cal_Update_Defaults(&cta_cal));
}


/**
 * Test for calibration file. Run test to check Cta_Customer_Calibration_In_Boundary basic work for uint8_t.
 * \uts{CSCSA-277498} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Customer_Calibration_Test, Cta_Are_Calibrations_In_Boundary)
{
   /** \arrange n/a */

   /** \action n/a */

   /** \assert Expect correct work of Cta_Customer_Calibration_In_Boundary for uint8_t fields. */

   EXPECT_TRUE(Cta_Customer_Cal_In_Boundary(&cta_cal));
}

#endif

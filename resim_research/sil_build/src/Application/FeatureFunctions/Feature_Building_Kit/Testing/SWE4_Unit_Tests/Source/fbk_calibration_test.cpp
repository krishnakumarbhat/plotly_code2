/**
 * @file fbk_calibration_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42220}
 */

#include "fbk_calibration_test.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <stdio.h>
#include <string.h>

extern "C"
{
#include "fbk_core_calibration.c"
#include "fbk_core_calibration_check.h"
#include "fbk_update_calibration.h"
#include "pa_reuse.h"
}


#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * Test for calibration file. Run test for print function.
 * \uts{CSCSA-42261} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Core_Calibration_Test, Fbk_Core_Cal_Print__check_that_no_fatal_failure_occures)
{
   /** \arrange Set up variable */
   FILE *p_testfile = stdout;

   /** \action n/a */

   /** \assert Expect no failures. */
   EXPECT_NO_FATAL_FAILURE(Fbk_Core_Cal_Print(p_testfile, &fbk_cals));
}


/**
 * Test for calibration file. Run test to check basic Fbk_Core_Cal_Update_Defaults work.
 * \uts{CSCSA-73101} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Core_Calibration_Test, Fbk_Core_Cal_Update_Defaults__is_reseting_k_fbk_host_trail_max_recording_speed)
{
   /** \arrange n/a */

   /** \action n/a */
   const float32_T orginal_value = fbk_cals.k_fbk_host_trail_max_recording_speed;
   fbk_cals.k_fbk_host_trail_max_recording_speed += 1;
   Fbk_Core_Cal_Update_Defaults(&fbk_cals);

   /** \assert Expect equal adress. */
   EXPECT_EQ(orginal_value, fbk_cals.k_fbk_host_trail_max_recording_speed);
}

/**
 * Test for calibration file. Run test to check basic Fbk_Update_Core_Cal_By_Core work.
 * \uts{CSCSA-73102} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Core_Calibration_Test, Fbk_Update_Core_Cal_By_Core__is_setting_k_fbk_host_trail_max_recording_speed)
{
   /** \arrange n/a */

   /** \action n/a */
   const float32_T orginal_value      = fbk_cals.k_fbk_host_trail_max_recording_speed;
   const float32_T new_value          = orginal_value + 1;
   Fbk_Core_Calibration_T calibration = fbk_cals;

   calibration.k_fbk_host_trail_max_recording_speed = new_value;

   /** \assert Expect correct values. */
   EXPECT_EQ(orginal_value, fbk_cals.k_fbk_host_trail_max_recording_speed);
   boolean_T result = Fbk_Update_Core_Cal_By_Core(&fbk_cals, &calibration);
   ASSERT_TRUE(result);
   EXPECT_EQ(new_value, fbk_cals.k_fbk_host_trail_max_recording_speed);
}

template <typename T> using Single_Case_T = std::tuple<const T Fbk_Core_Calibration_T::*, T, T>;

template <typename T> using All_Cases_T = std::vector<Single_Case_T<T>>;

template <typename T> void Fbk_Test_Fbk_Core_Cal_In_Boundary_For_Single_Member_Type(const All_Cases_T<T> &all_cases)
{
   Fbk_Core_Calibration_T calibration;
   Fbk_Core_Cal_Update_Defaults(&calibration);
   for (const Single_Case_T<T> &single_case : all_cases)
   {
      EXPECT_TRUE(Fbk_Core_Cal_In_Boundary(&calibration));
      const T Fbk_Core_Calibration_T::*member_ptr = std::get<0>(single_case);
      const T min_value                           = std::get<1>(single_case);
      const T max_value                           = std::get<2>(single_case);

      if (std::numeric_limits<T>::min() < min_value)
      {
         const_cast<T &>(calibration.*member_ptr) = min_value - 1;
         EXPECT_FALSE(Fbk_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = min_value;
      EXPECT_TRUE(Fbk_Core_Cal_In_Boundary(&calibration));

      if (std::numeric_limits<T>::max() > max_value)
      {
         const_cast<T &>(calibration.*member_ptr) = max_value + 1;
         EXPECT_FALSE(Fbk_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = max_value;
      EXPECT_TRUE(Fbk_Core_Cal_In_Boundary(&calibration));
   }
}


/**
 * Test for calibration file. Run test to check Fbk_Core_Cal_In_Boundary basic work for float32_T.
 * \uts{CSCSA-73103} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Core_Calibration_Test, Fbk_Core_Cal_In_Boundary__Float32_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<float32_T> all_cases = {
      {&Fbk_Core_Calibration_T::k_fbk_host_trail_max_recording_speed, FBK_MIN_K_FBK_HOST_TRAIL_MAX_RECORDING_SPEED,
       FBK_MAX_K_FBK_HOST_TRAIL_MAX_RECORDING_SPEED},
      {&Fbk_Core_Calibration_T::k_fbk_host_trail_dist_separation, FBK_MIN_K_FBK_HOST_TRAIL_DIST_SEPARATION,
       FBK_MAX_K_FBK_HOST_TRAIL_DIST_SEPARATION},
      {&Fbk_Core_Calibration_T::k_fbk_host_trail_heading_separation, FBK_MIN_K_FBK_HOST_TRAIL_HEADING_SEPARATION,
       FBK_MAX_K_FBK_HOST_TRAIL_HEADING_SEPARATION},
   };
   /** \assert Expect correct work of Fbk_Core_Cal_In_Boundary for float32_T fields. */
   Fbk_Test_Fbk_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}


#endif /* CT_ACTIVATE_CAL_PRINT */

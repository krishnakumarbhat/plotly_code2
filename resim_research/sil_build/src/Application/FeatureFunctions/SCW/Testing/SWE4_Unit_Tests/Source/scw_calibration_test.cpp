/**
 * @file scw_calibration_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for SCW calibration unit tests
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44546}
 */

#include "scw_calibration_test.hpp"

extern "C"
{
#include "pa_obj_in.h"
#include "scw_core_calibration.c"
#include "scw_core_calibration_check.h"
#include "scw_update_calibration.h"
}


#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * Test for calibration file. Run test for print function.
 * \uts{CSCSA-44548} \sdd{CSCSA-218603} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Core_Calibration_Test, Scw_Core_Cal_Print__check_that_no_fatal_failure_occures)
{
   /** \arrange Set up variable */
   FILE *p_testfile = stdout;

   /** \action n/a */

   /** \assert Expect no failures. */
   EXPECT_NO_FATAL_FAILURE(Scw_Core_Cal_Print(p_testfile, &scw_cals));
}


/**
 * Test for calibration file. Run test to check basic Scw_Core_Cal_Update_Defaults work.
 * \uts{CSCSA-101382} \sdd{CSCSA-218600} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Core_Calibration_Test, Scw_Core_Cal_Update_Defaults__is_reseting_k_scw_guardrail_cycles_in_zone_threshold)
{
   /** \arrange n/a */

   /** \action n/a */
   const uint8_t orginal_value = scw_cals.k_scw_guardrail_cycles_in_zone_threshold;
   scw_cals.k_scw_guardrail_cycles_in_zone_threshold += 1;
   Scw_Core_Cal_Update_Defaults(&scw_cals);

   /** \assert Expect equal adress. */
   EXPECT_EQ(orginal_value, scw_cals.k_scw_guardrail_cycles_in_zone_threshold);
}

/**
 * Test for calibration file. Run test to check basic Scw_Update_Core_Cal_By_Core work.
 * \uts{CSCSA-101383} \sdd{CSCSA-218601} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Core_Calibration_Test, Scw_Update_Core_Cal_By_Core__is_setting_k_scw_guardrail_cycles_in_zone_threshold)
{
   /** \arrange n/a */

   /** \action n/a */
   const uint8_t orginal_value        = scw_cals.k_scw_guardrail_cycles_in_zone_threshold;
   const uint8_t new_value            = orginal_value + 1;
   Scw_Core_Calibration_T calibration = scw_cals;

   calibration.k_scw_guardrail_cycles_in_zone_threshold = new_value;

   /** \assert Expect correct values. */
   EXPECT_EQ(orginal_value, scw_cals.k_scw_guardrail_cycles_in_zone_threshold);
   boolean_T result = Scw_Update_Core_Cal_By_Core(&scw_cals, &calibration);
   ASSERT_TRUE(result);
   EXPECT_EQ(new_value, scw_cals.k_scw_guardrail_cycles_in_zone_threshold);
}

/**
 * Test for calibration file. Run test to check basic Scw_Update_Core_Cal_By_Core work.
 * \uts{CSCSA-101384} \sdd{CSCSA-218601} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Core_Calibration_Test, Scw_Update_Core_Cal_By_Core__NULL_calibration_pointer)
{
   /** \arrange n/a */

   /** \action n/a */
   boolean_T result = Scw_Update_Core_Cal_By_Core(&scw_cals, NULL);

   /** \assert Expect function returns FALSE. */
   EXPECT_FALSE(result);
}

/**
 * Test for calibration file. Run test to check basic Scw_Update_Core_Cal_By_Core work.
 * \uts{CSCSA-101385} \sdd{CSCSA-218601} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Core_Calibration_Test, Scw_Update_Core_Cal_By_Core__calibration_beyond_boundary)
{
   /** \arrange n/a */
   scw_cals.k_scw_guardrail_cycles_in_zone_threshold = SCW_MAX_K_SCW_GUARDRAIL_CYCLES_IN_ZONE_THRESHOLD + 1u;

   /** \action n/a */
   boolean_T result = Scw_Update_Core_Cal_By_Core(&scw_cals, &scw_cals);

   /** \assert Expect function returns FALSE. */
   EXPECT_FALSE(result);
}

template <typename T> using Single_Case_T = std::tuple<const T Scw_Core_Calibration_T::*, T, T>;

template <typename T> using All_Cases_T = std::vector<Single_Case_T<T>>;

template <typename T> void Scw_Test_Scw_Core_Cal_In_Boundary_For_Single_Member_Type(const All_Cases_T<T> &all_cases)
{
   Scw_Core_Calibration_T calibration;
   Scw_Core_Cal_Update_Defaults(&calibration);
   for (const Single_Case_T<T> &single_case : all_cases)
   {
      EXPECT_TRUE(Scw_Core_Cal_In_Boundary(&calibration));
      const T Scw_Core_Calibration_T::*member_ptr = std::get<0>(single_case);
      const T min_value                           = std::get<1>(single_case);
      const T max_value                           = std::get<2>(single_case);

      if (std::numeric_limits<T>::min() < min_value)
      {
         const_cast<T &>(calibration.*member_ptr) = min_value - 1;
         EXPECT_FALSE(Scw_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = min_value;
      EXPECT_TRUE(Scw_Core_Cal_In_Boundary(&calibration));

      if (std::numeric_limits<T>::max() > max_value)
      {
         const_cast<T &>(calibration.*member_ptr) = max_value + 1;
         EXPECT_FALSE(Scw_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = max_value;
      EXPECT_TRUE(Scw_Core_Cal_In_Boundary(&calibration));
   }
}

/**
 * Test for calibration file. Run test to check Scw_Core_Cal_In_Boundary basic work for float32_T.
 * \uts{CSCSA-101386} \sdd{CSCSA-218590} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Core_Calibration_Test, Scw_Core_Cal_In_Boundary__Float32_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<float32_T> all_cases = {
      {&Scw_Core_Calibration_T::k_scw_min_host_speed, SCW_MIN_K_SCW_MIN_HOST_SPEED, SCW_MAX_K_SCW_MIN_HOST_SPEED},
      {&Scw_Core_Calibration_T::k_scw_min_host_speed_hys, SCW_MIN_K_SCW_MIN_HOST_SPEED_HYS, SCW_MAX_K_SCW_MIN_HOST_SPEED_HYS},
      {&Scw_Core_Calibration_T::k_scw_candidate_relative_vel_hys, SCW_MIN_K_SCW_CANDIDATE_RELATIVE_VEL_HYS,
       SCW_MAX_K_SCW_CANDIDATE_RELATIVE_VEL_HYS},
      {&Scw_Core_Calibration_T::k_scw_min_exist_prob_radar_guardrail, SCW_MIN_K_SCW_MIN_EXIST_PROB_RADAR_GUARDRAIL,
       SCW_MAX_K_SCW_MIN_EXIST_PROB_RADAR_GUARDRAIL},
   };
   /** \assert Expect correct work of Scw_Core_Cal_In_Boundary for float32_T fields. */
   Scw_Test_Scw_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}

/**
 * Test for calibration file. Run test to check Scw_Core_Cal_In_Boundary basic work for uint8_t.
 * \uts{CSCSA-101387} \sdd{CSCSA-218590} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Core_Calibration_Test, Scw_Core_Cal_In_Boundary__Uint8_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<uint8_t> all_cases = {
      {&Scw_Core_Calibration_T::k_scw_min_candidate_age, SCW_MIN_K_SCW_MIN_CANDIDATE_AGE, SCW_MAX_K_SCW_MIN_CANDIDATE_AGE},
      {&Scw_Core_Calibration_T::k_scw_candidate_mature_cycles_in_zone_threshold,
       SCW_MIN_K_SCW_CANDIDATE_MATURE_CYCLES_IN_ZONE_THRESHOLD, SCW_MAX_K_SCW_CANDIDATE_MATURE_CYCLES_IN_ZONE_THRESHOLD},
      {&Scw_Core_Calibration_T::k_scw_guardrail_cycles_in_zone_threshold, SCW_MIN_K_SCW_GUARDRAIL_CYCLES_IN_ZONE_THRESHOLD,
       SCW_MAX_K_SCW_GUARDRAIL_CYCLES_IN_ZONE_THRESHOLD},
   };
   /** \assert Expect correct work of Scw_Core_Cal_In_Boundary for float32_T fields. */
   Scw_Test_Scw_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}

#endif /* CT_ACTIVATE_CAL_PRINT */

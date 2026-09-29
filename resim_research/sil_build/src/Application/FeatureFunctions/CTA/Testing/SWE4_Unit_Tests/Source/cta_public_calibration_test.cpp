/**
 * @file ccta_calibration_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for CTA calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-277499}
 */

#include "cta_public_calibration_test.hpp"

extern "C"
{
#include "cta_public_calibration.c"
#include "cta_public_calibration_check.h"
#include "cta_update_calibration.h"
#include "pa_obj_in.h"
}


#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * Test for calibration file. Run test for print function.
 * \uts{CSCSA-277500} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Public_Calibration_Test, Cta_Public_Calibration_Print__check_that_no_fatal_failure_occures)
{
   /** \arrange Set up variable */
   FILE *p_testfile = stdout;

   /** \action n/a */

   /** \assert Expect no failures. */
   EXPECT_NO_FATAL_FAILURE(Cta_Public_Cal_Print(p_testfile, &cta_cal));
}

/**
 * Test for calibration file. Run test to check basic Cta_Update_Cal_Defaults work.
 * \uts{CSCSA-277501} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Public_Calibration_Test, Cta_Public_Calibration_Update_Defaults__is_reseting_k_cta_cycle_count_hold_true_warning)
{
   /** \arrange n/a */

   /** \action n/a */
   const uint8_t orginal_value = cta_cal.k_cta_cycle_count_hold_true_warning;
   cta_cal.k_cta_cycle_count_hold_true_warning += 1;
   Cta_Public_Cal_Update_Defaults(&cta_cal);

   /** \assert Expect equal adress. */
   EXPECT_EQ(orginal_value, cta_cal.k_cta_cycle_count_hold_true_warning);
}


template <typename T> using Single_Case_T = std::tuple<const T Cta_Public_Calibration_T::*, T, T>;

template <typename T> using All_Cases_T = std::vector<Single_Case_T<T>>;

template <typename T> void Cta_Test_Cta_Are_Calibrations_In_Boundary_For_Single_Member_Type(const All_Cases_T<T> &all_cases)
{
   Cta_Public_Calibration_T calibration;
   Cta_Public_Cal_Update_Defaults(&calibration);
   for (const Single_Case_T<T> &single_case : all_cases)
   {
      EXPECT_TRUE(Cta_Public_Cal_In_Boundary(&calibration));
      const T Cta_Public_Calibration_T::*member_ptr = std::get<0>(single_case);
      const T min_value                             = std::get<1>(single_case);
      const T max_value                             = std::get<2>(single_case);

      if (std::numeric_limits<T>::min() < min_value)
      {
         const_cast<T &>(calibration.*member_ptr) = min_value - 1;
         EXPECT_FALSE(Cta_Public_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = min_value;
      EXPECT_TRUE(Cta_Public_Cal_In_Boundary(&calibration));

      if (std::numeric_limits<T>::max() > max_value)
      {
         const_cast<T &>(calibration.*member_ptr) = max_value + 1;
         EXPECT_FALSE(Cta_Public_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = max_value;
      EXPECT_TRUE(Cta_Public_Cal_In_Boundary(&calibration));
   }
}


/**
 * Test for calibration file. Run test to check Cta_Public_Calibration_In_Boundary basic work for float32_T.
 * \uts{CSCSA-277502} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Public_Calibration_Test, Cta_Are_Calibrations_In_Boundary__float32_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<float32_T> all_cases = {
      {&Cta_Public_Calibration_T::k_cta_ego_abs_speed_max, CTA_MIN_K_CTA_EGO_ABS_SPEED_MAX, CTA_MAX_K_CTA_EGO_ABS_SPEED_MAX},
      {&Cta_Public_Calibration_T::k_cta_min_speed, CTA_MIN_K_CTA_MIN_SPEED, CTA_MAX_K_CTA_MIN_SPEED},
      {&Cta_Public_Calibration_T::k_cta_max_speed, CTA_MIN_K_CTA_MAX_SPEED, CTA_MAX_K_CTA_MAX_SPEED},
      {&Cta_Public_Calibration_T::k_cta_stop_alert_ttc, CTA_MIN_K_CTA_STOP_ALERT_TTC, CTA_MAX_K_CTA_STOP_ALERT_TTC},
      {&Cta_Public_Calibration_T::k_cta_min_lateral_approach_speed, CTA_MIN_K_CTA_MIN_LATERAL_APPROACH_SPEED,
       CTA_MAX_K_CTA_MIN_LATERAL_APPROACH_SPEED},
      {&Cta_Public_Calibration_T::k_cta_max_length_fov, CTA_MIN_K_CTA_MAX_LENGTH_FOV, CTA_MAX_K_CTA_MAX_LENGTH_FOV},
      {&Cta_Public_Calibration_T::k_cta_min_ttc_additional_mature_qualification,
       CTA_MIN_K_CTA_MIN_TTC_ADDITIONAL_MATURE_QUALIFICATION, CTA_MAX_K_CTA_MIN_TTC_ADDITIONAL_MATURE_QUALIFICATION},
      {&Cta_Public_Calibration_T::k_cta_max_heading_variance, CTA_MIN_K_CTA_MAX_HEADING_VARIANCE, CTA_MAX_K_CTA_MAX_HEADING_VARIANCE},
      {&Cta_Public_Calibration_T::k_cta_range_to_path_segment_ghost_qualif, CTA_MIN_K_CTA_RANGE_TO_PATH_SEGMENT_GHOST_QUALIF,
       CTA_MAX_K_CTA_RANGE_TO_PATH_SEGMENT_GHOST_QUALIF},
   };
   /** \assert Expect correct work of Cta_Public_Calibration_In_Boundary for float32_T fields. */
   Cta_Test_Cta_Are_Calibrations_In_Boundary_For_Single_Member_Type(all_cases);
}

/**
 * Test for calibration file. Run test to check Cta_Public_Calibration_In_Boundary basic work for uint8_t.
 * \uts{CSCSA-277503} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Public_Calibration_Test, Cta_Are_Calibrations_In_Boundary__uint8_t)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<uint8_t> all_cases = {
      {&Cta_Public_Calibration_T::k_cta_cycle_count_hold_true_warning, CTA_MIN_K_CTA_CYCLE_COUNT_HOLD_TRUE_WARNING,
       CTA_MAX_K_CTA_CYCLE_COUNT_HOLD_TRUE_WARNING},
      {&Cta_Public_Calibration_T::k_cta_min_object_age_check_valid, CTA_MIN_K_CTA_MIN_OBJECT_AGE_CHECK_VALID,
       CTA_MAX_K_CTA_MIN_OBJECT_AGE_CHECK_VALID},
      {&Cta_Public_Calibration_T::k_cta_cycle_count_suppress_true_warning, CTA_MIN_K_CTA_CYCLE_COUNT_SUPPRESS_TRUE_WARNING,
       CTA_MAX_K_CTA_CYCLE_COUNT_SUPPRESS_TRUE_WARNING},
      {&Cta_Public_Calibration_T::k_cta_min_object_age_thres, CTA_MIN_K_CTA_MIN_OBJECT_AGE_THRES, CTA_MAX_K_CTA_MIN_OBJECT_AGE_THRES},
      {&Cta_Public_Calibration_T::k_cta_cycles_coasted_to_ignore, CTA_MIN_K_CTA_CYCLES_COASTED_TO_IGNORE,
       CTA_MAX_K_CTA_CYCLES_COASTED_TO_IGNORE},
      {&Cta_Public_Calibration_T::k_cta_object_supress_counter, CTA_MIN_K_CTA_OBJECT_SUPRESS_COUNTER,
       CTA_MAX_K_CTA_OBJECT_SUPRESS_COUNTER},
      {&Cta_Public_Calibration_T::k_cta_min_mature_cycles_level_qualifiction, CTA_MIN_K_CTA_MIN_MATURE_CYCLES_LEVEL_QUALIFICTION,
       CTA_MAX_K_CTA_MIN_MATURE_CYCLES_LEVEL_QUALIFICTION},
      {&Cta_Public_Calibration_T::k_cta_additional_qualification_mature_cycles,
       CTA_MIN_K_CTA_ADDITIONAL_QUALIFICATION_MATURE_CYCLES, CTA_MAX_K_CTA_ADDITIONAL_QUALIFICATION_MATURE_CYCLES},
      {&Cta_Public_Calibration_T::k_cta_min_qual_age_obj_crossing_paths, CTA_MIN_K_CTA_MIN_QUAL_AGE_OBJ_CROSSING_PATHS,
       CTA_MAX_K_CTA_MIN_QUAL_AGE_OBJ_CROSSING_PATHS},
   };
   /** \assert Expect correct work of Cta_Public_Calibration_In_Boundary for uint8_t fields. */
   Cta_Test_Cta_Are_Calibrations_In_Boundary_For_Single_Member_Type(all_cases);
}

#endif

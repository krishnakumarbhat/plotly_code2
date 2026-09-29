/**
 * @file esa_calibration_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for ESA calibration unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-123871}
 */

#include "esa_calibration_test.hpp"

extern "C"
{
#include "esa_core_calibration.c"
#include "esa_core_calibration_check.h"
#include "esa_update_calibration.h"
#include "pa_reuse.h"
}

#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * Test for calibration file. Run test for print function.
 * \uts{CSCSA-123873} \sdd{CSCSA-216528} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Core_Calibration_Test, Esa_Core_Cal_Print__check_that_no_fatal_failure_occures)
{
   /** \arrange Set up variable */
   FILE *p_testfile = stdout;

   /** \action n/a */

   /** \assert Expect no failures. */
   EXPECT_NO_FATAL_FAILURE(Esa_Core_Cal_Print(p_testfile, &esa_cal));
}


/**
 * Test for calibration file. Run test to check basic Esa_Core_Cal_Update_Defaults work.
 * \uts{CSCSA-123875} \sdd{CSCSA-216529} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Core_Calibration_Test, Esa_Core_Cal_Update_Defaults__is_reseting_k_esa_alert_holding_cycles)
{
   /** \arrange n/a */
   const uint8_t orginal_value = esa_cal.k_esa_alert_holding_cycles;

   /** \action n/a */
   esa_cal.k_esa_alert_holding_cycles += 1;
   Esa_Core_Cal_Update_Defaults(&esa_cal);

   /** \assert Expect equal adress. */
   EXPECT_EQ(orginal_value, esa_cal.k_esa_alert_holding_cycles);
}


/**
 * Test for calibration file. Run test to check basic Esa_Update_Core_Cal_By_Core work.
 * \uts{CSCSA-123876} \sdd{CSCSA-216531} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Core_Calibration_Test, Esa_Update_Core_Cal_By_Core__is_setting_k_esa_alert_holding_cycles)
{
   /** \arrange n/a */
   const uint8_t orginal_value        = esa_cal.k_esa_alert_holding_cycles;
   const uint8_t new_value            = orginal_value + 1;
   Esa_Core_Calibration_T calibration = esa_cal;

   /** \action n/a */
   calibration.k_esa_alert_holding_cycles = new_value;

   /** \assert Expect correct values. */
   EXPECT_EQ(orginal_value, esa_cal.k_esa_alert_holding_cycles);
   boolean_T result = Esa_Update_Core_Cal_By_Core(&esa_cal, &calibration);
   EXPECT_TRUE(result);
   EXPECT_EQ(new_value, esa_cal.k_esa_alert_holding_cycles);
}

/**
 * Test for calibration file. Run test to check Esa_Update_Core_Cal_By_Core error handling.
 * \uts{CSCSA-123877} \sdd{CSCSA-216531} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Core_Calibration_Test, Esa_Update_Core_Cal_By_Core__error_handling)
{
   /** \arrange n/a */
   const uint8_t orginal_value = esa_cal.k_esa_alert_holding_cycles;
   const uint8_t new_value     = ESA_MAX_K_ESA_ALERT_HOLDING_CYCLES + 1;

   Esa_Core_Calibration_T calibration = esa_cal;

   /** \action n/a */
   calibration.k_esa_alert_holding_cycles = new_value;

   /** \assert Expect correct values. */
   EXPECT_FALSE(Esa_Update_Core_Cal_By_Core(&esa_cal, nullptr));
   EXPECT_EQ(orginal_value, esa_cal.k_esa_alert_holding_cycles);
   EXPECT_FALSE(Esa_Update_Core_Cal_By_Core(&esa_cal, &calibration));
   EXPECT_EQ(orginal_value, esa_cal.k_esa_alert_holding_cycles);
}


template <typename T> using Single_Case_T = std::tuple<const T Esa_Core_Calibration_T::*, T, T>;

template <typename T> using All_Cases_T = std::vector<Single_Case_T<T>>;

template <typename T> void Esa_Test_Esa_Core_Cal_In_Boundary_For_Single_Member_Type(const All_Cases_T<T> &all_cases)
{
   Esa_Core_Calibration_T calibration;
   Esa_Core_Cal_Update_Defaults(&calibration);
   for (const Single_Case_T<T> &single_case : all_cases)
   {
      EXPECT_TRUE(Esa_Core_Cal_In_Boundary(&calibration));
      const T Esa_Core_Calibration_T::*member_ptr = std::get<0>(single_case);
      const T min_value                           = std::get<1>(single_case);
      const T max_value                           = std::get<2>(single_case);

      if (std::numeric_limits<T>::min() < min_value)
      {
         const_cast<T &>(calibration.*member_ptr) = min_value - 1;
         EXPECT_FALSE(Esa_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = min_value;
      EXPECT_TRUE(Esa_Core_Cal_In_Boundary(&calibration));

      if (std::numeric_limits<T>::max() > max_value)
      {
         const_cast<T &>(calibration.*member_ptr) = max_value + 1;
         EXPECT_FALSE(Esa_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = max_value;
      EXPECT_TRUE(Esa_Core_Cal_In_Boundary(&calibration));
   }
}


/**
 * Test for calibration file. Run test to check Esa_Core_Cal_In_Boundary basic work for float32_T.
 * \uts{CSCSA-123878} \sdd{CSCSA-216546} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Core_Calibration_Test, Esa_Core_Cal_In_Boundary__float32_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<float32_T> all_cases = {
      {&Esa_Core_Calibration_T::k_esa_max_range, ESA_MIN_K_ESA_MAX_RANGE, ESA_MAX_K_ESA_MAX_RANGE},
      {&Esa_Core_Calibration_T::k_esa_min_lane_width, ESA_MIN_K_ESA_MIN_LANE_WIDTH, ESA_MAX_K_ESA_MIN_LANE_WIDTH},
      {&Esa_Core_Calibration_T::k_esa_max_lane_width, ESA_MIN_K_ESA_MAX_LANE_WIDTH, ESA_MAX_K_ESA_MAX_LANE_WIDTH},
      {&Esa_Core_Calibration_T::k_esa_min_exist_prob, ESA_MIN_K_ESA_MIN_EXIST_PROB, ESA_MAX_K_ESA_MIN_EXIST_PROB},
      {&Esa_Core_Calibration_T::k_esa_min_curve_radius, ESA_MIN_K_ESA_MIN_CURVE_RADIUS, ESA_MAX_K_ESA_MIN_CURVE_RADIUS},
      {&Esa_Core_Calibration_T::k_esa_min_curve_radius_hys, ESA_MIN_K_ESA_MIN_CURVE_RADIUS_HYS, ESA_MAX_K_ESA_MIN_CURVE_RADIUS_HYS},
      {&Esa_Core_Calibration_T::k_esa_max_curvi_heading_abs, ESA_MIN_K_ESA_MAX_CURVI_HEADING_ABS, ESA_MAX_K_ESA_MAX_CURVI_HEADING_ABS},
      {&Esa_Core_Calibration_T::k_esa_min_obj_curvi_long_vel_abs, ESA_MIN_K_ESA_MIN_OBJ_CURVI_LONG_VEL_ABS,
       ESA_MAX_K_ESA_MIN_OBJ_CURVI_LONG_VEL_ABS},
      {&Esa_Core_Calibration_T::k_esa_critical_longitudinal_ttc, ESA_MIN_K_ESA_CRITICAL_LONGITUDINAL_TTC,
       ESA_MAX_K_ESA_CRITICAL_LONGITUDINAL_TTC},
      {&Esa_Core_Calibration_T::k_esa_critical_longitudinal_ttc_hys, ESA_MIN_K_ESA_CRITICAL_LONGITUDINAL_TTC_HYS,
       ESA_MAX_K_ESA_CRITICAL_LONGITUDINAL_TTC_HYS},
      {&Esa_Core_Calibration_T::k_esa_obj_safe_deceleration_threshold, ESA_MIN_K_ESA_OBJ_SAFE_DECELERATION_THRESHOLD,
       ESA_MAX_K_ESA_OBJ_SAFE_DECELERATION_THRESHOLD},
      {&Esa_Core_Calibration_T::k_esa_obj_safe_deceleration_threshold_hys, ESA_MIN_K_ESA_OBJ_SAFE_DECELERATION_THRESHOLD_HYS,
       ESA_MAX_K_ESA_OBJ_SAFE_DECELERATION_THRESHOLD_HYS},
      {&Esa_Core_Calibration_T::k_esa_host_activation_speed_min, ESA_MIN_K_ESA_HOST_ACTIVATION_SPEED_MIN,
       ESA_MAX_K_ESA_HOST_ACTIVATION_SPEED_MIN},
      {&Esa_Core_Calibration_T::k_esa_host_activation_speed_min_hys, ESA_MIN_K_ESA_HOST_ACTIVATION_SPEED_MIN_HYS,
       ESA_MAX_K_ESA_HOST_ACTIVATION_SPEED_MIN_HYS},
      {&Esa_Core_Calibration_T::k_esa_host_activation_speed_max, ESA_MIN_K_ESA_HOST_ACTIVATION_SPEED_MAX,
       ESA_MAX_K_ESA_HOST_ACTIVATION_SPEED_MAX},
      {&Esa_Core_Calibration_T::k_esa_host_activation_speed_max_hys, ESA_MIN_K_ESA_HOST_ACTIVATION_SPEED_MAX_HYS,
       ESA_MAX_K_ESA_HOST_ACTIVATION_SPEED_MAX_HYS},
   };
   /** \assert Expect correct work of Esa_Core_Cal_In_Boundary for float32_T fields. */
   Esa_Test_Esa_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}


/**
 * Test for calibration file. Run test to check Esa_Core_Cal_In_Boundary basic work for uint8_t.
 * \uts{CSCSA-123879} \sdd{CSCSA-216546} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Core_Calibration_Test, Esa_Core_Cal_In_Boundary__uint8_t)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<uint8_t> all_cases = {
      {&Esa_Core_Calibration_T::k_esa_alert_holding_cycles, ESA_MIN_K_ESA_ALERT_HOLDING_CYCLES, ESA_MAX_K_ESA_ALERT_HOLDING_CYCLES},
      {&Esa_Core_Calibration_T::k_esa_min_mature_cycles, ESA_MIN_K_ESA_MIN_MATURE_CYCLES, ESA_MAX_K_ESA_MIN_MATURE_CYCLES},
      {&Esa_Core_Calibration_T::k_esa_min_track_age, ESA_MIN_K_ESA_MIN_TRACK_AGE, ESA_MAX_K_ESA_MIN_TRACK_AGE}};
   /** \assert Expect correct work of Esa_Core_Cal_In_Boundary for uint8_t fields. */
   Esa_Test_Esa_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}

#endif /* CT_ACTIVATE_CAL_PRINT */

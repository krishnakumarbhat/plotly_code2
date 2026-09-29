
/**
 * @file ta_output_boundary_check_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Tests the output range check of TA.
 *
 * Attention: This code is auto-generated - do not modify manually!
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */
#ifndef TA_OUTPUT_BOUNDARY_CHECK_TEST_HPP
#define TA_OUTPUT_BOUNDARY_CHECK_TEST_HPP


#include "gtest/gtest.h"           // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h" // IWYU pragma: keep
extern "C"
{
#include "pa_reuse.h"    // IWYU pragma: keep
#include "ta_output_t.h" // IWYU pragma: keep
}

extern "C"
{
#include "ta_output_boundary_check.c" // IWYU pragma: keep
}


/**
 * @brief Used for creation of test fixtures for Ta outputs range checks
 *
 * @return True when containing outputs are within their boundaries
 *
 * @SRD{n/a}
 * @SAD{n/a}
 * @SDD{n/a}
 * @verification{none}
 **/
class Ta_Output_Boundary_Check_Test : public ::testing::Test
{
 public:
   Ta_Output_T *p_output;

   Ta_Output_T output{};

   void SetUp() override
   {
      p_output = &output;
   }

   void TearDown() override
   {
   }

   void Ta_Set_Up_Middle_Of_The_Road_Output(void);

   template <typename T> void Ta_Set_Array_With_Default_Val(T array_in[], uint8_t array_size, T default_val)
   {
      for (uint8_t i = 0; i < array_size; i++)
      {
         array_in[i] = default_val;
      }
   }
};
void inline Ta_Output_Boundary_Check_Test::Ta_Set_Up_Middle_Of_The_Road_Output(void)
{
   p_output->fta_brake_deceleration_request =
      ((float32_T) (0.5f * (TA_FTA_BRAKE_DECELERATION_REQUEST_MIN_VAL + TA_FTA_BRAKE_DECELERATION_REQUEST_MAX_VAL)));
   p_output->fta_target_gap                = ((float32_T) (0.5f * (TA_FTA_TARGET_GAP_MIN_VAL + TA_FTA_TARGET_GAP_MAX_VAL)));
   p_output->fta_ttc                       = ((float32_T) (0.5f * (TA_FTA_TTC_MIN_VAL + TA_FTA_TTC_MAX_VAL)));
   p_output->fta_alert_level               = ((uint8_t) (0.5f * (TA_FTA_ALERT_LEVEL_MAX_VAL)));
   p_output->fta_symbol_request            = ((uint8_t) (0.5f * (TA_FTA_SYMBOL_REQUEST_MAX_VAL)));
   p_output->fta_brake_threshold_reduction = ((uint8_t) (0.5f * (TA_FTA_BRAKE_THRESHOLD_REDUCTION_MAX_VAL)));
   p_output->fta_brake_conditioning        = ((uint8_t) (0.5f * (TA_FTA_BRAKE_CONDITIONING_MAX_VAL)));
   p_output->fta_maneuver_direction        = ((uint8_t) (0.5f * (TA_FTA_MANEUVER_DIRECTION_MAX_VAL)));
   p_output->f_diagnostic_mode             = ((boolean_T) (0.5f * (TA_F_DIAGNOSTIC_MODE_MIN_VAL + TA_F_DIAGNOSTIC_MODE_MAX_VAL)));
   p_output->ta_current_deceleration_estimate =
      ((float32_T) (0.5f * (TA_CURRENT_DECELERATION_ESTIMATE_MIN_VAL + TA_CURRENT_DECELERATION_ESTIMATE_MAX_VAL)));
   p_output->f_fta_enable              = ((uint8_t) (0.5f * (TA_F_FTA_ENABLE_MAX_VAL)));
   p_output->f_rta_enable              = ((uint8_t) (0.5f * (TA_F_RTA_ENABLE_MAX_VAL)));
   p_output->f_rta_enable_turning_area = ((uint8_t) (0.5f * (TA_F_RTA_ENABLE_TURNING_AREA_MAX_VAL)));
   p_output->f_rta_enable_dynamic_area = ((uint8_t) (0.5f * (TA_F_RTA_ENABLE_DYNAMIC_AREA_MAX_VAL)));
   p_output->rta_dynamic_area_status   = ((uint8_t) (0.5f * (TA_RTA_DYNAMIC_AREA_STATUS_MAX_VAL)));
   p_output->rta_turning_area_status   = ((uint8_t) (0.5f * (TA_RTA_TURNING_AREA_STATUS_MAX_VAL)));
   p_output->rta_alert_left            = ((uint8_t) (0.5f * (TA_RTA_ALERT_LEFT_MAX_VAL)));
   p_output->rta_alert_right           = ((uint8_t) (0.5f * (TA_RTA_ALERT_RIGHT_MAX_VAL)));
}

#endif /*TA_OUTPUT_BOUNDARY_CHECK_TEST_HPP*/

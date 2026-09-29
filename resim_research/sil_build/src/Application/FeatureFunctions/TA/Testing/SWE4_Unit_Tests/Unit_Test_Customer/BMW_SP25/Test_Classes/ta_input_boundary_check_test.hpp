
/**
 * @file ta_input_boundary_check_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Tests the input range check of TA.
 *
 * Attention: This code is auto-generated - do not modify manually!
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */
#ifndef TA_INPUT_BOUNDARY_CHECK_TEST_HPP
#define TA_INPUT_BOUNDARY_CHECK_TEST_HPP


#include "gtest/gtest.h"           // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h" // IWYU pragma: keep
extern "C"
{
#include "pa_reuse.h"   // IWYU pragma: keep
#include "ta_input_t.h" // IWYU pragma: keep
}

extern "C"
{
#include "ta_input_boundary_check.c" // IWYU pragma: keep
}


/**
 * @brief Used for creation of test fixtures for Ta inputs range checks
 *
 * @return True when containing inputs are within their boundaries
 *
 * @SRD{n/a}
 * @SAD{n/a}
 * @SDD{n/a}
 * @verification{none}
 **/
class Ta_Input_Boundary_Check_Test : public ::testing::Test
{
 public:
   Ta_Input_T *p_input;

   Ta_Input_T input{};

   void SetUp() override
   {
      p_input = &input;
   }

   void TearDown() override
   {
   }

   void Ta_Set_Up_Middle_Of_The_Road_Input(void);

   template <typename T> void Ta_Set_Array_With_Default_Val(T array_in[], uint8_t array_size, T default_val)
   {
      for (uint8_t i = 0; i < array_size; i++)
      {
         array_in[i] = default_val;
      }
   }
};
void inline Ta_Input_Boundary_Check_Test::Ta_Set_Up_Middle_Of_The_Road_Input(void)
{
   p_input->fta_obj_offset_x_positive =
      ((float32_T) (0.5f * (TA_FTA_OBJ_OFFSET_X_POSITIVE_MIN_VAL + TA_FTA_OBJ_OFFSET_X_POSITIVE_MAX_VAL)));
   p_input->fta_obj_offset_x_negative =
      ((float32_T) (0.5f * (TA_FTA_OBJ_OFFSET_X_NEGATIVE_MIN_VAL + TA_FTA_OBJ_OFFSET_X_NEGATIVE_MAX_VAL)));
   p_input->fta_obj_offset_y_positive =
      ((float32_T) (0.5f * (TA_FTA_OBJ_OFFSET_Y_POSITIVE_MIN_VAL + TA_FTA_OBJ_OFFSET_Y_POSITIVE_MAX_VAL)));
   p_input->fta_obj_offset_y_negative =
      ((float32_T) (0.5f * (TA_FTA_OBJ_OFFSET_Y_NEGATIVE_MIN_VAL + TA_FTA_OBJ_OFFSET_Y_NEGATIVE_MAX_VAL)));
   p_input->f_rta_enable                 = ((uint8_t) (0.5f * (TA_F_RTA_ENABLE_MAX_VAL)));
   p_input->f_fta_enable                 = ((uint8_t) (0.5f * (TA_F_FTA_ENABLE_MAX_VAL)));
   p_input->fta_steering_angle_max_left  = ((uint8_t) (0.5f * (TA_FTA_STEERING_ANGLE_MAX_LEFT_MAX_VAL)));
   p_input->fta_steering_angle_max_right = ((uint8_t) (0.5f * (TA_FTA_STEERING_ANGLE_MAX_RIGHT_MAX_VAL)));
}

#endif /*TA_INPUT_BOUNDARY_CHECK_TEST_HPP*/

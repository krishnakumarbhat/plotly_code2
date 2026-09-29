/**
 * @file ta_bmw_diagnostic_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SRR5 TA diagnostic tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-45135}
 */

#include "ta_bmw_diagnostic_test.hpp"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "ta_bmw_diagnostic.c"
}


#ifndef NDEBUG
/**
 * Checks, if Ta_Get_Target_Shift_Offset_Long throws exception, when input pointer is NULL.
 * \uts{CSCSA-45136} \sdd{SF-8587} \testtype{positive}
 */
TEST_F(Ta_Bmw_Diagnostic_Test, Ta_Get_Target_Shift_Offset_Long__ta_input_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Get_Target_Shift_Offset_Long throws exception, when p_ta_input is NULL pointer. */
   EXPECT_DEATH({ Ta_Get_Target_Shift_Offset_Long(NULL); }, ".*p_ta_input.*");
}
#endif // !NDEBUG

/**
 * Checks, if address Ta_Get_Target_Shift_Offset_Long returns correct sum.
 * \uts{CSCSA-45137} \sdd{SF-8587} \testtype{positive}
 */
TEST_F(Ta_Bmw_Diagnostic_Test, Ta_Get_Target_Shift_Offset_Long__returns_sum_of_longitudinal_parameters)
{
   /** \arrange Declare variable of type float32_T. */
   float32_T ret_value;

   ta_input.fta_obj_offset_x_positive = 100.0f;
   ta_input.fta_obj_offset_x_negative = -50.0f;

   /** \action Calculate longitudinal target shift offset */
   ret_value = Ta_Get_Target_Shift_Offset_Long(&ta_input);

   /** \assert Check if value is sum of input parameters fta_obj_offset_x_positive and fta_obj_offset_x_negative. */
   EXPECT_FLOAT_EQ(ret_value, 0.01f * (ta_input.fta_obj_offset_x_positive + ta_input.fta_obj_offset_x_negative));
}

#ifndef NDEBUG
/**
 * Checks, if Ta_Get_Target_Shift_Offset_Lat throws exception, when input pointer is NULL.
 * \uts{CSCSA-45138} \sdd{SF-8588} \testtype{positive}
 */
TEST_F(Ta_Bmw_Diagnostic_Test, Ta_Get_Target_Shift_Offset_Lat__ta_input_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Get_Target_Shift_Offset_Lat throws exception, when p_ta_input is NULL pointer. */
   EXPECT_DEATH({ Ta_Get_Target_Shift_Offset_Lat(NULL); }, ".*p_ta_input.*");
}
#endif // !NDEBUG

/**
 * Checks, if address Ta_Get_Target_Shift_Offset_Lat returns correct sum.
 * \uts{CSCSA-45139} \sdd{SF-8588} \testtype{positive}
 */
TEST_F(Ta_Bmw_Diagnostic_Test, Ta_Get_Target_Shift_Offset_Lat__returns_sum_of_longitudinal_parameters)
{
   /** \arrange Declare variable of type float32_T. */
   float32_T ret_value;

   ta_input.fta_obj_offset_y_positive = 100.0f;
   ta_input.fta_obj_offset_y_negative = -50.0f;

   /** \action Calculate longitudinal target shift offset */
   ret_value = Ta_Get_Target_Shift_Offset_Lat(&ta_input);

   /** \assert Check if value is sum of input parameters fta_obj_offset_x_positive and fta_obj_offset_x_negative. */
   EXPECT_FLOAT_EQ(ret_value, -0.01f * (ta_input.fta_obj_offset_y_positive + ta_input.fta_obj_offset_y_negative));
}

#ifndef NDEBUG
/**
 * Checks, if Ta_Is_Diagnostic_Mode_Enabled throws exception, when ta input pointer is NULL.
 * \uts{CSCSA-45140} \sdd{SF-8589} \testtype{positive}
 */
TEST_F(Ta_Bmw_Diagnostic_Test, Ta_Is_Diagnostic_Mode_Enabled__ta_input_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Get_Target_Shift_Offset_Lat throws exception, when p_ta_input is NULL pointer. */
   EXPECT_DEATH({ Ta_Is_Diagnostic_Mode_Enabled(NULL, &ta_cals); }, ".*p_ta_input.*");
}

/**
 * Checks, if Ta_Is_Diagnostic_Mode_Enabled throws exception, when ta cal pointer is NULL.
 * \uts{CSCSA-45141} \sdd{SF-8589} \testtype{positive}
 */
TEST_F(Ta_Bmw_Diagnostic_Test, Ta_Is_Diagnostic_Mode_Enabled__ta_cal_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Get_Target_Shift_Offset_Lat throws exception, when p_ta_cals is NULL pointer. */
   EXPECT_DEATH({ Ta_Is_Diagnostic_Mode_Enabled(&ta_input, NULL); }, ".*p_ta_cals.*");
}
#endif // !NDEBUG

/**
 * Checks, if Ta_Is_Diagnostic_Mode_Enabled returns false, when input parameter k_f_ta_enable_debug_mode is 0.
 * \uts{CSCSA-45142} \sdd{SF-8589} \testtype{positive}
 */
TEST_F(Ta_Bmw_Diagnostic_Test, Ta_Is_Diagnostic_Mode_Enabled__returns_false_if_debug_mode_disabled)
{
   /** \arrange Declare variable of type boolean_T. */
   boolean_T ret_value;

   ta_cals.k_f_ta_enable_debug_mode   = 0u;
   ta_input.f_fta_enable              = 1u;
   ta_input.fta_obj_offset_x_positive = 100.0f;
   ta_input.fta_obj_offset_x_negative = 0.0f;
   ta_input.fta_obj_offset_y_positive = 0.0f;
   ta_input.fta_obj_offset_y_negative = 0.0f;

   /** \action Call Ta_Is_Diagnostic_Mode_Enabled */
   ret_value = Ta_Is_Diagnostic_Mode_Enabled(&ta_input, &ta_cals);

   /** \assert Check if diagnostic mode is disabled. */
   EXPECT_EQ(ret_value, FBK_FALSE);
}

/**
 * Checks, if Ta_Is_Diagnostic_Mode_Enabled returns false, when input parameter f_fta_enable is 0.
 * \uts{CSCSA-45143} \sdd{SF-8589} \testtype{positive}
 */
TEST_F(Ta_Bmw_Diagnostic_Test, Ta_Is_Diagnostic_Mode_Enabled__returns_false_if_fta_disabled)
{
   /** \arrange Declare variable of type boolean_T. */
   boolean_T ret_value;

   ta_cals.k_f_ta_enable_debug_mode   = 1u;
   ta_input.f_fta_enable              = 0u;
   ta_input.fta_obj_offset_x_positive = 100.0f;
   ta_input.fta_obj_offset_x_negative = 0.0f;
   ta_input.fta_obj_offset_y_positive = 0.0f;
   ta_input.fta_obj_offset_y_negative = 0.0f;

   /** \action Call Ta_Is_Diagnostic_Mode_Enabled */
   ret_value = Ta_Is_Diagnostic_Mode_Enabled(&ta_input, &ta_cals);

   /** \assert Check if diagnostic mode is disabled. */
   EXPECT_EQ(ret_value, FBK_FALSE);
}

/**
 * Checks, if Ta_Is_Diagnostic_Mode_Enabled returns false, when all offset parameters are 0.
 * \uts{CSCSA-45144} \sdd{SF-8589} \testtype{positive}
 */
TEST_F(Ta_Bmw_Diagnostic_Test, Ta_Is_Diagnostic_Mode_Enabled__returns_false_if_all_fta_obj_offset_are_0)
{
   /** \arrange Declare variable of type boolean_T. */
   boolean_T ret_value;

   ta_cals.k_f_ta_enable_debug_mode   = 1u;
   ta_input.f_fta_enable              = 1u;
   ta_input.fta_obj_offset_x_positive = 0.0f;
   ta_input.fta_obj_offset_x_negative = 0.0f;
   ta_input.fta_obj_offset_y_positive = 0.0f;
   ta_input.fta_obj_offset_y_negative = 0.0f;

   /** \action Call Ta_Is_Diagnostic_Mode_Enabled */
   ret_value = Ta_Is_Diagnostic_Mode_Enabled(&ta_input, &ta_cals);

   /** \assert Check if diagnostic mode is disabled. */
   EXPECT_EQ(ret_value, FBK_FALSE);
}

/**
 * Checks, if Ta_Is_Diagnostic_Mode_Enabled returns true, when input parameters f_fta_enable and k_f_ta_enable_debug_mode are 1 and
 * one of offset parameters is not 0. \uts{CSCSA-45145} \sdd{SF-8589} \testtype{positive}
 */
TEST_F(Ta_Bmw_Diagnostic_Test, Ta_Is_Diagnostic_Mode_Enabled__returns_true_if_fta_obj_offset_x_positive_not_0)
{
   /** \arrange Declare variable of type boolean_T. */
   boolean_T ret_value;

   ta_cals.k_f_ta_enable_debug_mode   = 1u;
   ta_input.f_fta_enable              = 1u;
   ta_input.fta_obj_offset_x_positive = 100.0f;
   ta_input.fta_obj_offset_x_negative = 0.0f;
   ta_input.fta_obj_offset_y_positive = 0.0f;
   ta_input.fta_obj_offset_y_negative = 0.0f;

   /** \action Call Ta_Is_Diagnostic_Mode_Enabled */
   ret_value = Ta_Is_Diagnostic_Mode_Enabled(&ta_input, &ta_cals);

   /** \assert Check if diagnostic mode is enabled. */
   EXPECT_EQ(ret_value, FBK_TRUE);
}

/**
 * Checks, if Ta_Is_Diagnostic_Mode_Enabled returns true, when input parameters f_fta_enable and k_f_ta_enable_debug_mode are 1 and
 * one of offset parameters is not 0. \uts{CSCSA-45146} \sdd{SF-8589} \testtype{positive}
 */
TEST_F(Ta_Bmw_Diagnostic_Test, Ta_Is_Diagnostic_Mode_Enabled__returns_true_if_fta_obj_offset_x_negative_not_0)
{
   /** \arrange Declare variable of type boolean_T. */
   boolean_T ret_value;

   ta_cals.k_f_ta_enable_debug_mode   = 1u;
   ta_input.f_fta_enable              = 1u;
   ta_input.fta_obj_offset_x_positive = 0.0f;
   ta_input.fta_obj_offset_x_negative = -100.0f;
   ta_input.fta_obj_offset_y_positive = 0.0f;
   ta_input.fta_obj_offset_y_negative = 0.0f;

   /** \action Call Ta_Is_Diagnostic_Mode_Enabled */
   ret_value = Ta_Is_Diagnostic_Mode_Enabled(&ta_input, &ta_cals);

   /** \assert Check if diagnostic mode is enabled. */
   EXPECT_EQ(ret_value, FBK_TRUE);
}

/**
 * Checks, if Ta_Is_Diagnostic_Mode_Enabled returns true, when input parameters f_fta_enable and k_f_ta_enable_debug_mode are 1 and
 * one of offset parameters is not 0. \uts{CSCSA-45147} \sdd{SF-8589} \testtype{positive}
 */
TEST_F(Ta_Bmw_Diagnostic_Test, Ta_Is_Diagnostic_Mode_Enabled__returns_true_if_fta_obj_offset_y_positive_not_0)
{
   /** \arrange Declare variable of type boolean_T. */
   boolean_T ret_value;

   ta_cals.k_f_ta_enable_debug_mode   = 1u;
   ta_input.f_fta_enable              = 1u;
   ta_input.fta_obj_offset_x_positive = 0.0f;
   ta_input.fta_obj_offset_x_negative = 0.0f;
   ta_input.fta_obj_offset_y_positive = 100.0f;
   ta_input.fta_obj_offset_y_negative = 0.0f;

   /** \action Call Ta_Is_Diagnostic_Mode_Enabled */
   ret_value = Ta_Is_Diagnostic_Mode_Enabled(&ta_input, &ta_cals);

   /** \assert Check if diagnostic mode is enabled. */
   EXPECT_EQ(ret_value, FBK_TRUE);
}

/**
 * Checks, if Ta_Is_Diagnostic_Mode_Enabled returns true, when input parameters f_fta_enable and k_f_ta_enable_debug_mode are 1 and
 * one of offset parameters is not 0. \uts{CSCSA-45148} \sdd{SF-8589} \testtype{positive}
 */
TEST_F(Ta_Bmw_Diagnostic_Test, Ta_Is_Diagnostic_Mode_Enabled__returns_true_if_fta_obj_offset_y_negative_not_0)
{
   /** \arrange Declare variable of type boolean_T. */
   boolean_T ret_value;

   ta_cals.k_f_ta_enable_debug_mode   = 1u;
   ta_input.f_fta_enable              = 1u;
   ta_input.fta_obj_offset_x_positive = 0.0f;
   ta_input.fta_obj_offset_x_negative = 0.0f;
   ta_input.fta_obj_offset_y_positive = 0.0f;
   ta_input.fta_obj_offset_y_negative = -100.0f;

   /** \action Call Ta_Is_Diagnostic_Mode_Enabled */
   ret_value = Ta_Is_Diagnostic_Mode_Enabled(&ta_input, &ta_cals);

   /** \assert Check if diagnostic mode is enabled. */
   EXPECT_EQ(ret_value, FBK_TRUE);
}

/**
 * @file ta_constants_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44802}
 */

#include "ta_constants_test.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ta_constants.h"
}

using ::testing::FloatNear;

/**
 * Check if predicted time step is correctly estimated.
 * \uts{CSCSA-44803} \sdd{SF-8678} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Constants_Test, Ta_Init_Prediction_Time_Step__Init_to_valid_times)
{
   /** \arrange Write invalid value in persistent memory */
   p_ta_persistent->ta_pred_step_dt = 100.0f;

   /** \action Initialize the predicted timestep */
   Ta_Init_Prediction_Time_Step(p_ta_persistent, &ta_cal);

   /** \assert Check if predicted time step was set as expected */
   EXPECT_FLOAT_EQ(p_ta_persistent->ta_pred_step_dt, ta_cal.k_ta_alert_lvl_2_ttc_threshold / ta_cal.k_ta_prediction_steps_max);
}

/**
 * Check if predicted time step is set to default value, when cal parameter k_ta_alert_lvl_2_ttc_threshold equals 0.
 * \uts{CSCSA-44805} \sdd{SF-8678} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Constants_Test, Ta_Init_Prediction_Time_Step__default_values_when_cal_paramerter_0)
{
   /** \arrange Write invalid value in persistent memory */
   p_ta_persistent->ta_pred_step_dt      = 100.0f;
   ta_cal.k_ta_alert_lvl_2_ttc_threshold = 0.0f;

   /** \action Initialize the predicted timestep */
   Ta_Init_Prediction_Time_Step(p_ta_persistent, &ta_cal);

   /** \assert Check if predicted time step was set as expected */
   EXPECT_FLOAT_EQ(p_ta_persistent->ta_pred_step_dt, FBK_ZERO_F);
}

/**
 * If the cal values are ill-defined, ensure that a division by zero is prevented. Instead the default value zero shall be used.
 * \uts{CSCSA-44804} \sdd{SF-8678} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Constants_Test, Ta_Init_Prediction_Time_Step__Init_to_valid_times_with_predictionsteps_equals_0)
{
   /** \arrange Create ill-valued cal values. Create required pointer holding the estimated time step and get it from persistent
    * memory. */
   ta_cal.k_ta_alert_lvl_2_ttc_threshold = 0u;
   ta_cal.k_ta_prediction_steps_max      = 0u;
   p_ta_persistent->ta_pred_step_dt      = 100.0f;

   /** \action Initialize the predicted timestep */
   Ta_Init_Prediction_Time_Step(p_ta_persistent, &ta_cal);

   /** \assert Check if predicted time step was set to zero */
   EXPECT_FLOAT_EQ(p_ta_persistent->ta_pred_step_dt, FBK_ZERO_F);
}

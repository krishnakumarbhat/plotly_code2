/**
 * @file ced_common_functions_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for ced.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41441}
 */

#include "ced_common_functions_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

/**
 * Tests whether the given input values are calculated to the correct time.
 * \uts{CSCSA-41442} \sdd{SF-3450} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Common_Functions_Test, Ced_Get_Time_To_Travel_Given_Distance__calculate_time_without_acceleration)
{
   /** \arrange set up distance, velocity and acceleration. */
   float32_T distance     = 10.0f;
   float32_T velocity     = 1.0f;
   float32_T acceleration = 0.0f;

   float32_T time_without_acc = distance / velocity;

   /** \action Calculate the time based on the input values. */
   float32_T time = Ced_Get_Time_To_Travel_Given_Distance(distance, velocity, acceleration);

   /** \assert Expect correctly calculated time. */
   EXPECT_FLOAT_EQ(time, time_without_acc);
}

/**
 * Tests whether the given input values are calculated to the correct time.
 * \uts{CSCSA-41443} \sdd{SF-3450} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Common_Functions_Test, Ced_Get_Time_To_Travel_Given_Distance__calculate_negative_time_obj_past_reference_position)
{
   /** \arrange set up distance, velocity and acceleration. */
   float32_T distance     = -10.0f;
   float32_T velocity     = 1.0f;
   float32_T acceleration = 0.0f;

   float32_T time_without_acc = distance / velocity;

   /** \action Calculate the time based on the input values. */
   float32_T time = Ced_Get_Time_To_Travel_Given_Distance(distance, velocity, acceleration);

   /** \assert Expect correctly calculated time. */
   EXPECT_FLOAT_EQ(time, time_without_acc);
}

/**
 * Tests whether the given input values are calculated to the correct time.
 * \uts{CSCSA-41444} \sdd{SF-3450} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Common_Functions_Test, Ced_Get_Time_To_Travel_Given_Distance__return_default_value_for_velocity_close_to_zero)
{
   /** \arrange set up distance, velocity and acceleration. */
   float32_T distance     = 10.0f;
   float32_T velocity     = EPSILON * EPSILON;
   float32_T acceleration = 0.0f;

   /** \action Calculate the time based on the input values. */
   float32_T time = Ced_Get_Time_To_Travel_Given_Distance(distance, velocity, acceleration);

   /** \assert Expect default time value. */
   EXPECT_FLOAT_EQ(time, CED_INVALID_TIME);
}

/**
 * Tests whether the given input values are calculated to the correct time.
 * \uts{CSCSA-41445} \sdd{SF-3450} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Common_Functions_Test, Ced_Get_Time_To_Travel_Given_Distance__return_default_value_for_negative_velocity)
{
   /** \arrange set up distance, velocity and acceleration. */
   float32_T distance     = 10.0f;
   float32_T velocity     = -1.0f;
   float32_T acceleration = 0.0f;

   /** \action Calculate the time based on the input values. */
   float32_T time = Ced_Get_Time_To_Travel_Given_Distance(distance, velocity, acceleration);

   /** \assert Expect default time value. */
   EXPECT_FLOAT_EQ(time, CED_INVALID_TIME);
}

/**
 * Tests whether the given input values are calculated to the correct time.
 * \uts{CSCSA-41446} \sdd{SF-3450} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Common_Functions_Test,
       Ced_Get_Time_To_Travel_Given_Distance__return_default_time_if_object_stopped_before_traveling_complete_distance)
{
   /** \arrange set up distance, velocity and acceleration. */
   float32_T distance     = 10.0f;
   float32_T velocity     = 1.0f;
   float32_T acceleration = -99.0f;

   /** \action Calculate the time based on the input values. */
   float32_T time = Ced_Get_Time_To_Travel_Given_Distance(distance, velocity, acceleration);

   /** \assert Expect default time value. */
   EXPECT_FLOAT_EQ(time, CED_INVALID_TIME);
}

/**
 * Tests whether the given input values are calculated to the correct time.
 * \uts{CSCSA-41447} \sdd{SF-3450} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Common_Functions_Test, Ced_Get_Time_To_Travel_Given_Distance__solve_quadratic_equation_for_obj_accelerating)
{
   /** \arrange set up distance, velocity and acceleration. */
   float32_T distance     = 10.0f;
   float32_T velocity     = 10.0f;
   float32_T acceleration = 1.0f;

   float32_T time_without_acc = distance / velocity;

   /** \action Calculate the time based on the input values. */
   float32_T time = Ced_Get_Time_To_Travel_Given_Distance(distance, velocity, acceleration);

   /** \assert Expect correctly calculated time. Check that correct root was selected. */
   EXPECT_LT(time, time_without_acc);
   EXPECT_GT(time, 0.9f * time_without_acc);
}

/**
 * Tests whether the given input values are calculated to the correct time.
 * \uts{CSCSA-41448} \sdd{SF-3450} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Common_Functions_Test, Ced_Get_Time_To_Travel_Given_Distance__solve_quadratic_equation_for_obj_decelerating)
{
   /** \arrange set up distance, velocity and acceleration. */
   float32_T distance     = 10.0f;
   float32_T velocity     = 10.0f;
   float32_T acceleration = -1.0f;

   float32_T time_without_acc = distance / velocity;

   /** \action Calculate the time based on the input values. */
   float32_T time = Ced_Get_Time_To_Travel_Given_Distance(distance, velocity, acceleration);

   /** \assert Expect correctly calculated time. Check that correct root was selected. */
   EXPECT_GT(time, time_without_acc);
   EXPECT_LT(time, 1.1f * time_without_acc);
}

/**
 * Tests whether the given input values are calculated to the correct time.
 * \uts{CSCSA-41449} \sdd{SF-3450} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Common_Functions_Test, Ced_Get_Time_To_Travel_Given_Distance__solve_quadratic_equation_exact_solution)
{
   /** \arrange set up distance, velocity and acceleration. */
   float32_T distance     = 10.0f;
   float32_T velocity     = 10.0f;
   float32_T acceleration = -5.0f;

   /** \action Calculate the time based on the input values. */
   float32_T time = Ced_Get_Time_To_Travel_Given_Distance(distance, velocity, acceleration);

   /** \assert Expect correctly calculated time. Roots are the same. */
   EXPECT_FLOAT_EQ(time, 2.0f);
}
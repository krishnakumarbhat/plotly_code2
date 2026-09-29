/**
 * @file pt_constants_selection.test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Source file for PT constants selection test.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "pt_constants_selection_test.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "pa_reuse.h"
#include "pt_constants.c"
#include "pt_constants.h"
#include <algorithm>
}

using ::testing::ContainerEq;
using ::testing::Not;


TEST_F(Pt_Constants_Selection_Test, Is_Obj_Invalid_For_Path_Tracking__updated_grid_gets_returned)
{
   // Arrange
   float32_T expected_array[PT_NUM_GRID_POINTS] = {-60.0f, -55.0f, -50.0f, -45.0f, -40.0f, -35.0f, -30.0f, -25.0f, -20.0f,
                                                   -15.0f, -10.0f, -5.0f,  0.0f,   5.0f,   10.0f,  15.0f,  20.0f,  25.0f,
                                                   30.0f,  35.0f,  40.0f,  45.0f,  50.0f,  55.0f,  60.0f};

   float32_T res_array[PT_NUM_GRID_POINTS];
   // Action
   Pt_Update_Grid_Array_Defaults(res_array);
   // Expect
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(expected_array[i], res_array[i]);
   }
}

TEST_F(Pt_Constants_Selection_Test, Is_Obj_Invalid_For_Path_Tracking__expected_array_unequal_to_returned_one)
{
   // Arrange
   float32_T expected_array[PT_NUM_GRID_POINTS] = {-62.0f, -57.0f, -51.0f, -48.0f, -41.0f, -32.0f, -30.0f, -21.0f, -20.0f,
                                                   -13.0f, -9.0f,  -1.0f,  0.5f,   3.0f,   12.0f,  14.0f,  23.0f,  29.0f,
                                                   32.0f,  38.0f,  43.0f,  46.0f,  52.0f,  57.0f,  63.0f};

   float32_T res_array[PT_NUM_GRID_POINTS];
   // Action
   Pt_Update_Grid_Array_Defaults(res_array);
   // Expect
   EXPECT_NE(expected_array, res_array);
}

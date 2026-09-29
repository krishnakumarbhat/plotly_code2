/**
 * @file pt_iface_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for pt_constants_test.cpp functions
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44117}
 */

#include "pt_constants_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pt_constants.c"
#include "pt_constants.h"
}


/**
 * Check whether path tracking grid array default setter. Verify that the correct default values are set.
 * \uts{CSCSA-44118} \sdd{SF-7591} \testtype{positive}
 */
TEST_F(Pt_Constants_Test, Pt_Update_Grid_Array_Defaults__check_correct_default_values)
{
   float32_T grid_pt_array[PT_NUM_GRID_POINTS];
   /** \arrange Set up array with non default values. */
   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      grid_pt_array[idx] = 0.0f;
   }
   /** \action call update routine. */
   Pt_Update_Grid_Array_Defaults(grid_pt_array);

   /** \assert Expect that the grid array is set to default. */
   EXPECT_FLOAT_EQ(grid_pt_array[0], -80.0f);
}

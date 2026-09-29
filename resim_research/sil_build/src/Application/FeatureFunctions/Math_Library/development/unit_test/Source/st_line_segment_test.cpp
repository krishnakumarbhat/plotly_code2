/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include <gtest/gtest.h>
#include "Basic_Vectors.hpp"
#include "st_vector_2d_helper.hpp"
#include "ml_line_segment.h"
#include "ml_line_segment_t.h"



/**
* Test the function to create a line segment
* \sdd{WI-13961}
*/
TEST(StLineSegmentTest, WI_14972_Create_Line_Segment_test)
{
   /** \arrange */

   /** \action call function under test */
   Line_Segment_T test_result = Create_Line_Segment_Fom_Points(&Basic_Vectors::vec_x_normal, &Basic_Vectors::vec_y_normal);

   /** \assert */
   EXPECT_VECTOR_2D_EQ(test_result.p0, Basic_Vectors::vec_x_normal);
   EXPECT_VECTOR_2D_EQ(test_result.p1, Basic_Vectors::vec_y_normal);
}

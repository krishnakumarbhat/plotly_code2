/*===================================================================*\
* Copyright 2019, Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential.
\*===================================================================*/

#ifndef FBK_REF_POINT_CALC_TEST_HPP
#define FBK_REF_POINT_CALC_TEST_HPP

#include "gtest/gtest.h" // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_ref_point.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
}

/**
 * Class used to create a fixture for field_of_interest_factory module test
 * With help of its inherited Setup function the fixture is
 * able to create the same initialization for all of its tests
 */
class Fbk_Ref_Point_Calc_Test : public ::testing::Test
{

 protected:
   float32_T host_length = 5.0f;
   Fbk_Ref_Point_T ref_point{};           /**<reference point*/
   Fbk_Object_Corners_T target_corners{}; /**<target corners*/
   Vector_2d_T target_vcs_pos{};          /**<targets vcs pos*/
   Vector_2d_T host_ref_point{};          /**<host ref points*/

   virtual void SetUp()
   {
      host_ref_point = Create_2d_Vector_Coordinates(-host_length, 0.0f);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* FBK_REF_POINT_CALC_TEST_HPP */

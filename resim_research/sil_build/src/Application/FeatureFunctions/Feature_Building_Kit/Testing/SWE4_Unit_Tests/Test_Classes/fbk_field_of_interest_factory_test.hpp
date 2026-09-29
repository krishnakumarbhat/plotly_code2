/*===================================================================*\
* Copyright 2019, Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential.
\*===================================================================*/

#ifndef FBK_FIELD_OF_INTEREST_FACTORY_TEST_HPP
#define FBK_FIELD_OF_INTEREST_FACTORY_TEST_HPP

#include "gtest/gtest.h" // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_field_of_interest.h"
#include "pa_reuse.h"
}

/**
 * Class used to create a fixture for field_of_interest_factory module test
 * With help of its inherited Setup function the fixture is
 * able to create the same initialization for all of its tests
 */
class Fbk_Field_Of_Interest_Factory_Test : public ::testing::Test
{

 protected:
   uint8_t field_of_interest_size;    /**<Size of field of interest*/
   Fbk_Field_Of_Interest_T res_foi{}; /**<result field of interest for each case*/


   virtual void SetUp()
   {
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* FBK_FIELD_OF_INTEREST_FACTORY_TEST_HPP */

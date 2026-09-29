#ifndef PT_PERSISTENT_HANDLER_TEST_HPP
#define PT_PERSISTENT_HANDLER_TEST_HPP

/**
 * @file pt_persistent_handler_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for PT persistent handler functions unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "pt_persistent_t.h"
}

class Pt_Persistent_Handler_Test : public ::testing::Test
{
 public:
   Pt_Persistent_T path_tracking_persistent{};
   /**
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};

#endif /* PT_PERSISTENT_HANDLER_TEST_HPP */

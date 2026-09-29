/**
 * @file main.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Main Source file for PT unit tests.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*============================================*\
* MACROS
\*===========================================================================*/

#include "gmock/gmock.h"
#include "gtest/gtest_pred_impl.h"

int main(int argc, char *argv[])
{
   ::testing::InitGoogleMock(&argc, argv);
   return RUN_ALL_TESTS();
}
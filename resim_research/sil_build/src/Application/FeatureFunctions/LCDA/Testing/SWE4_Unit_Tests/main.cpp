/**
 * @file main.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief LCDA unit test main file
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "gmock/gmock.h"
#include "gtest/gtest_pred_impl.h"

int main(int argc, char *argv[])
{
   ::testing::InitGoogleMock(&argc, argv);
   return RUN_ALL_TESTS();
}

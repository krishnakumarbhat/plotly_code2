/*===================================================================*\
* Copyright 2004, Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential.
*--------------------------------------------------------------------
*
* Description:
*
* Applicable Standards (in order of precedence: highest first):
*
* Deviations from Delco C Coding standards:
*
*
\*===================================================================*/
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
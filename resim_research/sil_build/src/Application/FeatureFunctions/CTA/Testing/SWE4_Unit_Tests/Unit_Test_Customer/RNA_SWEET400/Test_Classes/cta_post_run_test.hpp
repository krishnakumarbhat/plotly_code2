#ifndef CTA_POST_RUN_RNA_TEST
#define CTA_POST_RUN_RNA_TEST
/*===================================================================*\
* Copyright 2018, Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential.
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

#include "gtest/gtest_pred_impl.h" // IWYU pragma: keep
#include <gtest/gtest.h>           // IWYU pragma: keep

extern "C"
{
#include "cta_core_calibration.h"
#include "cta_core_output_t.h"
#include "cta_input_t.h"
#include "cta_instance.h"
#include "cta_output_t.h"
#include "fbk_macros.h"
#include "fbk_output.h"
#include "fbk_ref_point.h"
#include "fbk_vehicle_data_t.h"
#include "pa_context.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Cta_Post_Run_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Cta_Core_Calibration_T *p_cta_cals   = nullptr;
   Fbk_Object_Data_T *object_data       = nullptr;
   Fbk_Vehicle_Data_T *p_vehicle_data   = nullptr;
   Cta_Core_Output_T *p_cta_core_output = nullptr;
   Fbk_Object_Corners_T target_corners{};
   Pa_Data_T data{};
   Cta_Output_T cta_output;
   Cta_Input_T cta_input;
   Cta_Instance_T cta_instance{};
   Fbk_Output_T fbk_output{};

   void SetUp() override
   {
      p_cta_cals = &cta_instance.calibration;
      Cta_Core_Cal_Update_Defaults(p_cta_cals);
      fbk_output.p_pa_data              = &data;
      object_data                       = data.object_data;
      p_vehicle_data                    = &(data.vehicle_data);
      cta_instance.core_input.p_pa_data = &data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};

#endif
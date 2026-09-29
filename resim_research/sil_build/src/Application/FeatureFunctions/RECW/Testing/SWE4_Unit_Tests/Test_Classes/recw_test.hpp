#ifndef RECW_TEST_HPP
#define RECW_TEST_HPP

/**
 * @file recw_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is main RECW test header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_guardrail_data_t.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "recw.c"
#include "recw_core_calibration.h"
#include "recw_core_output_t.h"
#include "recw_instance.h"
#include "recw_persistent_t.h"
}

class Recw_Test : public ::testing::Test
{
 protected:
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Guardrail_Data_T *guardrail_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Recw_Instance_T recw_instance{};
   Recw_Persistent_T &recw_pers = recw_instance.persistent;

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      /* Init cals */
      Recw_Core_Cal_Update_Defaults(&recw_instance.calibration);
      recw_instance.calibration.k_recw_f_only_allow_consecutive_alert_levels = 0u;

      /* Initialize context data */
      guardrail_data = data.guardrail_data;
      p_vehicle_data = &(data.vehicle_data);

      /* Init core_input */
      recw_instance.core_input.f_enable_recw = FBK_TRUE;

      /* Init recw_core_output */
      Recw_Reset_Core_Output(&recw_instance.core_output);

      /* Init persistent data */
      Recw_Reset_Persistent(&recw_pers);

      /* Set some vehicle data and tracker data for all tests */
      recw_instance.core_input.p_pa_data = &data;
      p_vehicle_data->host_length        = 5.0f;
      p_vehicle_data->host_width         = 2.0f;
      data.time_diff_to_last_cycle       = 0.05f;
      object_data                        = data.object_data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }

 public:
};

#endif // RECW_TEST_HPP

#ifndef CED_IFACE_TEST_H
#define CED_IFACE_TEST_H

/**
 * @file ced_iface_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for ced_iface unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_core_calibration.h"
#include "ced_core_input_t.h"
#include "ced_iface.h"
#include "ced_input_t.h"
#include "ced_instance.h"
}


/**
 * Class used to create a fixture for Ced_Iface_Test.c
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ced_Iface_Test : public ::testing::Test
{
 protected:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Pa_Data_T data{};
   Ced_Instance_T ced_instance{};
   Ced_Input_T ced_input{};
   Ced_Output_T ced_output{};
   Fbk_Output_T fbk_output{};
   Pt_Output_T pt_output{};
   Fbk_Index_Id_Lookup_Table_T empty_lookup_table;

   virtual void SetUp()
   {
      Ced_Core_Cal_Update_Defaults(&(ced_instance.calibration));

      fbk_output.p_pa_data = &data;
      Fbk_Reset_Index_Id_Lookup_Table(&empty_lookup_table);
      fbk_output.p_index_id_lookup_table = &empty_lookup_table;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }

 public:
};

#endif /* CED_IFACE_TEST_H */

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "ml_bool.h"
#include "ml_runtime_parameters.h"
#include "ml_runtime_parameter_boolean_t.h"
#include "ml_runtime_parameter_error_t.h"
#include "ml_runtime_parameter_float32_t.h"
#include "ml_runtime_parameter_int16_t.h"
#include "ml_runtime_parameter_int32_t.h"
#include "ml_runtime_parameter_int8_t.h"
#include "ml_runtime_parameter_legal_state_t.h"
#include "ml_runtime_parameter_uint16_t.h"
#include "ml_runtime_parameter_uint32_t.h"
#include "ml_runtime_parameter_uint8_t.h"


class RuntimeParametersTestFixture : public ::testing::Test
{
protected:
   uint8_t expected_default_value = 0;
   uint8_t expected_set_value = 1;
   Runtime_Parameter_Uint8_T rt_value_set = {
         /* .value  = */ expected_set_value,
         /* .is_set = */ TRUE,
      };
   Runtime_Parameter_Uint8_T rt_value_unset = {
         /* .value  = */ expected_set_value,
         /* .is_set = */ FALSE,
      };
};

/**
 * This Unit test, checks the vector assignment
 * \sdd{WI-13601}
 * \sdd{WI-13607}
 */
TEST_F(RuntimeParametersTestFixture, WI_15117_generate_runtime_value_error_not_settable_Test)
{
   /** \arrange */
   Runtime_Parameter_Error_T error_setting_prohibited;

   uint8_t expecting_standard_value;

   Init_Runtime_Param_Error(&error_setting_prohibited);

   /** \action call function under test */
   expecting_standard_value = Generate_Runtime_Value_Uint8(&rt_value_set, RUNTIME_PARAMETER_PROHIBITED, expected_default_value, &error_setting_prohibited);

   /** \assert */
   EXPECT_TRUE(error_setting_prohibited.parameter_not_settable);
   EXPECT_FALSE(error_setting_prohibited.parameter_must_be_set);
   EXPECT_EQ(expecting_standard_value, expected_default_value);
}

/** Testing not to set a prohibited value
* \sdd{WI-13601}
* \sdd{WI-13607}
*/
TEST_F(RuntimeParametersTestFixture, WI_15154_generate_runtime_value_error_not_settable_Test2)
{
   /** \arrange */
   Runtime_Parameter_Error_T error_setting_prohibited;

   uint8_t expecting_standard_value;

   Init_Runtime_Param_Error(&error_setting_prohibited);

   /** \action call function under test */
   expecting_standard_value = Generate_Runtime_Value_Uint8(&rt_value_unset, RUNTIME_PARAMETER_PROHIBITED, expected_default_value, &error_setting_prohibited);

   /** \assert */
   EXPECT_FALSE(error_setting_prohibited.parameter_not_settable);
   EXPECT_FALSE(error_setting_prohibited.parameter_must_be_set);
   EXPECT_EQ(expecting_standard_value, expected_default_value);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13607}
*/
TEST_F(RuntimeParametersTestFixture, WI_15155_generate_runtime_value_error_prohibited_Test)
{
   /** \arrange */
   Runtime_Parameter_Error_T error_not_setting_mandatory;

   uint8_t expecting_standard_value;

   Init_Runtime_Param_Error(&error_not_setting_mandatory);

   /** \action call function under test */
   expecting_standard_value = Generate_Runtime_Value_Uint8(&rt_value_unset, RUNTIME_PARAMETER_MANDATORY, expected_default_value, &error_not_setting_mandatory);

   /** \assert */

   EXPECT_FALSE(error_not_setting_mandatory.parameter_not_settable);
   EXPECT_TRUE(error_not_setting_mandatory.parameter_must_be_set);
   EXPECT_EQ(expecting_standard_value, expected_default_value);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13607}
*/
TEST_F(RuntimeParametersTestFixture, WI_15156_generate_runtime_value_error_prohibited_Test2)
{
   /** \arrange */
   Runtime_Parameter_Error_T error_not_setting_mandatory;

   uint8_t expecting_standard_value;

   Init_Runtime_Param_Error(&error_not_setting_mandatory);

   /** \action call function under test */
   expecting_standard_value = Generate_Runtime_Value_Uint8(&rt_value_set, RUNTIME_PARAMETER_MANDATORY, expected_default_value, &error_not_setting_mandatory);

   /** \assert */

   EXPECT_FALSE(error_not_setting_mandatory.parameter_not_settable);
   EXPECT_FALSE(error_not_setting_mandatory.parameter_must_be_set);
   EXPECT_EQ(expecting_standard_value, expected_set_value);
}

/**
* \sdd{WI-13601}
*/
TEST_F(RuntimeParametersTestFixture, WI_15157_runtime_parameters_error_init_Test)
{
   /** \arrange */
   Runtime_Parameter_Error_T no_error_expected;

   /** \action call function under test */
   Init_Runtime_Param_Error(&no_error_expected);

   /** \assert */
   EXPECT_FALSE(no_error_expected.parameter_must_be_set);
   EXPECT_FALSE(no_error_expected.parameter_not_settable);
}

/* This tests tries to set a runtime value
* \sdd{WI-13601}
* \sdd{WI-13607}
*/
TEST_F(RuntimeParametersTestFixture, WI_15158_generate_runtime_value_setting_value_Test)
{
   /** \arrange */
   Runtime_Parameter_Error_T no_error_expected;

   uint8_t expecting_set_value;

   Init_Runtime_Param_Error(&no_error_expected);

   /** \action call function under test */
   expecting_set_value = Generate_Runtime_Value_Uint8(&rt_value_set, RUNTIME_PARAMETER_OPTIONAL, expected_default_value, &no_error_expected);

   /** \assert */
   EXPECT_EQ(expecting_set_value, expected_set_value);

   EXPECT_FALSE(no_error_expected.parameter_must_be_set);
   EXPECT_FALSE(no_error_expected.parameter_not_settable);
}

/* This test tries to NOT set a runtime value
* \sdd{WI-13601}
* \sdd{WI-13607}
*/
TEST_F(RuntimeParametersTestFixture, WI_15159_generate_runtime_value_setting_standard_Test)
{
   /** \arrange */
   Runtime_Parameter_Error_T no_error_expected;

   uint8_t expecting_standard_value;

   Init_Runtime_Param_Error(&no_error_expected);

   /** \action call function under test */
   expecting_standard_value = Generate_Runtime_Value_Uint8(&rt_value_unset, RUNTIME_PARAMETER_OPTIONAL, expected_default_value, &no_error_expected);

   /** \assert */
   EXPECT_EQ(expecting_standard_value, expected_default_value);

   EXPECT_FALSE(no_error_expected.parameter_must_be_set);
   EXPECT_FALSE(no_error_expected.parameter_not_settable);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13622}
*/
TEST_F(RuntimeParametersTestFixture, WI_15160_test_runtime_value_in_range_Test)
{
   uint8_t standard_value = 105;

   uint8_t min_value = 100;
   uint8_t max_value = 110;
   uint8_t ok_value  = standard_value;

   uint8_t ret_ok;

   Runtime_Parameter_Error_T no_error_expected;

   Init_Runtime_Param_Error(&no_error_expected);

   /** \action call function under test */
   ret_ok = Test_Runtime_Value_Uint8(ok_value, max_value, min_value, standard_value, &no_error_expected);

   /** \assert */
   EXPECT_EQ(ret_ok, ok_value);

   EXPECT_FALSE(no_error_expected.out_of_range);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13622}
*/
TEST_F(RuntimeParametersTestFixture, WI_15161_test_runtime_value_out_of_range_Test)
{
   uint8_t standard_value = 105;

   uint8_t min_value = 100;
   uint8_t max_value = 110;
   uint8_t nok_value = 99;

   uint8_t ret_nok;

   Runtime_Parameter_Error_T error;

   Init_Runtime_Param_Error(&error);

   /** \action call function under test */
   ret_nok = Test_Runtime_Value_Uint8(nok_value, max_value, min_value, standard_value, &error);

   /** \assert */
   EXPECT_EQ(ret_nok, standard_value);

   EXPECT_TRUE(error.out_of_range);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13607}
*/
TEST_F(RuntimeParametersTestFixture, WI_15162_Generate_Runtime_Value_Uint8_exists)
{
   /** \arrange */
   Runtime_Parameter_Error_T no_error_expected;

   uint8_t expecting_set_value;
   Runtime_Parameter_Uint8_T local_rt_value_is_set;

   local_rt_value_is_set.value  = 1;
   local_rt_value_is_set.is_set = TRUE;

   Init_Runtime_Param_Error(&no_error_expected);

   /** \action call function under test */
   expecting_set_value = Generate_Runtime_Value_Uint8(&local_rt_value_is_set, RUNTIME_PARAMETER_OPTIONAL, expected_default_value, &no_error_expected);

   /** \assert */
   EXPECT_EQ(expecting_set_value, expected_set_value);

   EXPECT_FALSE(no_error_expected.parameter_must_be_set);
   EXPECT_FALSE(no_error_expected.parameter_not_settable);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13614}
*/
TEST_F(RuntimeParametersTestFixture, WI_15163_Generate_Runtime_Value_Uint16_exists)
{
   /** \arrange */
   Runtime_Parameter_Error_T no_error_expected;

   uint16_t expecting_set_value;
   Runtime_Parameter_Uint16_T local_rt_value_is_set;

   local_rt_value_is_set.value  = 1;
   local_rt_value_is_set.is_set = TRUE;

   Init_Runtime_Param_Error(&no_error_expected);

   /** \action call function under test */
   expecting_set_value = Generate_Runtime_Value_Uint16(&local_rt_value_is_set, RUNTIME_PARAMETER_OPTIONAL, expected_default_value, &no_error_expected);

   /** \assert */
   EXPECT_EQ(expecting_set_value, expected_set_value);

   EXPECT_FALSE(no_error_expected.parameter_must_be_set);
   EXPECT_FALSE(no_error_expected.parameter_not_settable);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13613}
*/
TEST_F(RuntimeParametersTestFixture, WI_15164_Generate_Runtime_Value_Uint32_exists)
{
   /** \arrange */
   Runtime_Parameter_Error_T no_error_expected;

   uint32_t expecting_set_value;
   Runtime_Parameter_Uint32_T local_rt_value_is_set;

   local_rt_value_is_set.value  = 1;
   local_rt_value_is_set.is_set = TRUE;

   Init_Runtime_Param_Error(&no_error_expected);

   /** \action call function under test */
   expecting_set_value = Generate_Runtime_Value_Uint32(&local_rt_value_is_set, RUNTIME_PARAMETER_OPTIONAL, expected_default_value, &no_error_expected);

   /** \assert */
   EXPECT_EQ(expecting_set_value, expected_set_value);

   EXPECT_FALSE(no_error_expected.parameter_must_be_set);
   EXPECT_FALSE(no_error_expected.parameter_not_settable);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13612}
*/
TEST_F(RuntimeParametersTestFixture, WI_15165_Generate_Runtime_Value_Int8_exists)
{
   /** \arrange */
   Runtime_Parameter_Error_T no_error_expected;

   int8_t expecting_set_value;
   Runtime_Parameter_Int8_T local_rt_value_is_set;

   local_rt_value_is_set.value  = 1;
   local_rt_value_is_set.is_set = TRUE;

   Init_Runtime_Param_Error(&no_error_expected);

   /** \action call function under test */
   expecting_set_value = Generate_Runtime_Value_Int8(&local_rt_value_is_set, RUNTIME_PARAMETER_OPTIONAL, expected_default_value, &no_error_expected);

   /** \assert */
   EXPECT_EQ(expecting_set_value, expected_set_value);

   EXPECT_FALSE(no_error_expected.parameter_must_be_set);
   EXPECT_FALSE(no_error_expected.parameter_not_settable);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13611}
*/
TEST_F(RuntimeParametersTestFixture, WI_15166_Generate_Runtime_Value_Int16_exists)
{
   /** \arrange */
   Runtime_Parameter_Error_T no_error_expected;

   int16_t expecting_set_value;
   Runtime_Parameter_Int16_T local_rt_value_is_set;

   local_rt_value_is_set.value  = 1;
   local_rt_value_is_set.is_set = TRUE;

   Init_Runtime_Param_Error(&no_error_expected);

   /** \action call function under test */
   expecting_set_value = Generate_Runtime_Value_Int16(&local_rt_value_is_set, RUNTIME_PARAMETER_OPTIONAL, expected_default_value, &no_error_expected);

   /** \assert */
   EXPECT_EQ(expecting_set_value, expected_set_value);

   EXPECT_FALSE(no_error_expected.parameter_must_be_set);
   EXPECT_FALSE(no_error_expected.parameter_not_settable);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13610}
*/
TEST_F(RuntimeParametersTestFixture, WI_15167_Generate_Runtime_Value_Int32_exists)
{
   /** \arrange */
   Runtime_Parameter_Error_T no_error_expected;

   int32_t expecting_set_value;
   Runtime_Parameter_Int32_T local_rt_value_is_set;

   local_rt_value_is_set.value  = 1;
   local_rt_value_is_set.is_set = TRUE;

   Init_Runtime_Param_Error(&no_error_expected);

   /** \action call function under test */
   expecting_set_value = Generate_Runtime_Value_Int32(&local_rt_value_is_set, RUNTIME_PARAMETER_OPTIONAL, expected_default_value, &no_error_expected);

   /** \assert */
   EXPECT_EQ(expecting_set_value, expected_set_value);

   EXPECT_FALSE(no_error_expected.parameter_must_be_set);
   EXPECT_FALSE(no_error_expected.parameter_not_settable);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13608}
*/
TEST_F(RuntimeParametersTestFixture, WI_15168_Generate_Runtime_Value_Boolean_exists)
{
   /** \arrange */
   Runtime_Parameter_Error_T no_error_expected;

   int32_t expecting_set_value;
   Runtime_Parameter_Boolean_T local_rt_value_is_set;

   local_rt_value_is_set.value  = TRUE;
   local_rt_value_is_set.is_set = TRUE;

   Init_Runtime_Param_Error(&no_error_expected);

   /** \action call function under test */
   expecting_set_value = Generate_Runtime_Value_Boolean(&local_rt_value_is_set, RUNTIME_PARAMETER_OPTIONAL, expected_default_value, &no_error_expected);

   /** \assert */
   EXPECT_EQ(expecting_set_value, expected_set_value);

   EXPECT_FALSE(no_error_expected.parameter_must_be_set);
   EXPECT_FALSE(no_error_expected.parameter_not_settable);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13609}
*/
TEST_F(RuntimeParametersTestFixture, WI_15169_Generate_Runtime_Value_Float32_exists)
{
   /** \arrange */
   Runtime_Parameter_Error_T no_error_expected;

   float32_T expecting_set_value;
   Runtime_Parameter_Float32_T local_rt_value_is_set;

   local_rt_value_is_set.value  = 1.0f;
   local_rt_value_is_set.is_set = TRUE;

   Init_Runtime_Param_Error(&no_error_expected);

   /** \action call function under test */
   expecting_set_value = Generate_Runtime_Value_Float32(&local_rt_value_is_set, RUNTIME_PARAMETER_OPTIONAL, expected_default_value, &no_error_expected);

   /** \assert */
   EXPECT_EQ(expecting_set_value, expected_set_value);

   EXPECT_FALSE(no_error_expected.parameter_must_be_set);
   EXPECT_FALSE(no_error_expected.parameter_not_settable);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13622}
*/
TEST_F(RuntimeParametersTestFixture, WI_15170_Test_Runtime_Value_Uint8_exists)
{
   uint8_t standard_value = 105;

   uint8_t min_value = 100;
   uint8_t max_value = 110;
   uint8_t nok_value = 99;

   uint8_t ret_nok;

   Runtime_Parameter_Error_T error;

   Init_Runtime_Param_Error(&error);

   /** \action call function under test */
   ret_nok = Test_Runtime_Value_Uint8(nok_value, max_value, min_value, standard_value, &error);

   /** \assert */
   EXPECT_EQ(ret_nok, standard_value);

   EXPECT_TRUE(error.out_of_range);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13621}
*/
TEST_F(RuntimeParametersTestFixture, WI_15171_Test_Runtime_Value_Uint16_exists)
{
   uint16_t standard_value = 105;

   uint16_t min_value = 100;
   uint16_t max_value = 110;
   uint16_t nok_value = 99;

   uint16_t ret_nok;

   Runtime_Parameter_Error_T error;

   Init_Runtime_Param_Error(&error);

   /** \action call function under test */
   ret_nok = Test_Runtime_Value_Uint16(nok_value, max_value, min_value, standard_value, &error);

   /** \assert */
   EXPECT_EQ(ret_nok, standard_value);

   EXPECT_TRUE(error.out_of_range);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13620}
*/
TEST_F(RuntimeParametersTestFixture, WI_15172_Test_Runtime_Value_Uint32_exists)
{
   uint32_t standard_value = 105;

   uint32_t min_value = 100;
   uint32_t max_value = 110;
   uint32_t nok_value = 99;

   uint32_t ret_nok;

   Runtime_Parameter_Error_T error;

   Init_Runtime_Param_Error(&error);

   /** \action call function under test */
   ret_nok = Test_Runtime_Value_Uint32(nok_value, max_value, min_value, standard_value, &error);

   /** \assert */
   EXPECT_EQ(ret_nok, standard_value);

   EXPECT_TRUE(error.out_of_range);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13619}
*/
TEST_F(RuntimeParametersTestFixture, WI_15173_Test_Runtime_Value_Int8_exists)
{
   int8_t standard_value = 105;

   int8_t min_value = 100;
   int8_t max_value = 110;
   int8_t nok_value = 99;

   int8_t ret_nok;

   Runtime_Parameter_Error_T error;

   Init_Runtime_Param_Error(&error);

   /** \action call function under test */
   ret_nok = Test_Runtime_Value_Int8(nok_value, max_value, min_value, standard_value, &error);

   /** \assert */
   EXPECT_EQ(ret_nok, standard_value);

   EXPECT_TRUE(error.out_of_range);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13615}
*/
TEST_F(RuntimeParametersTestFixture, WI_15174_Test_Runtime_Value_Int16_exists)
{
   int16_t standard_value = 105;

   int16_t min_value = 100;
   int16_t max_value = 110;
   int16_t nok_value = 99;

   int16_t ret_nok;

   Runtime_Parameter_Error_T error;

   Init_Runtime_Param_Error(&error);

   /** \action call function under test */
   ret_nok = Test_Runtime_Value_Int16(nok_value, max_value, min_value, standard_value, &error);

   /** \assert */
   EXPECT_EQ(ret_nok, standard_value);

   EXPECT_TRUE(error.out_of_range);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13617}
*/
TEST_F(RuntimeParametersTestFixture, WI_15175_Test_Runtime_Value_Int32_exists)
{
   int32_t standard_value = 105;

   int32_t min_value = 100;
   int32_t max_value = 110;
   int32_t nok_value = 99;

   int32_t ret_nok;

   Runtime_Parameter_Error_T error;

   Init_Runtime_Param_Error(&error);

   /** \action call function under test */
   ret_nok = Test_Runtime_Value_Int32(nok_value, max_value, min_value, standard_value, &error);

   /** \assert */
   EXPECT_EQ(ret_nok, standard_value);

   EXPECT_TRUE(error.out_of_range);
}

/**
* \sdd{WI-13601}
* \sdd{WI-13618}
*/
TEST_F(RuntimeParametersTestFixture, WI_15176_Test_Runtime_Value_Float32_exists)
{
   float32_T standard_value = 105;

   float32_T min_value = 100;
   float32_T max_value = 110;
   float32_T nok_value = 99;

   float32_T ret_nok;

   Runtime_Parameter_Error_T error;

   Init_Runtime_Param_Error(&error);

   /** \action call function under test */
   ret_nok = Test_Runtime_Value_Float32(nok_value, max_value, min_value, standard_value, &error);

   /** \assert */
   EXPECT_EQ(ret_nok, standard_value);

   EXPECT_TRUE(error.out_of_range);
}

/**
* \sdd{WI-13624}
*/
TEST_F(RuntimeParametersTestFixture, WI_15177_set_runtime_value_boolean_T)
{
	Runtime_Parameter_Boolean_T ret_runtime_param;
	boolean_T input = TRUE;
   /** \action call function under test */
   ret_runtime_param = Set_Runtime_Parameter_Extern_Boolean(input);
   /** \assert */
   EXPECT_TRUE(ret_runtime_param.is_set);
    EXPECT_EQ(ret_runtime_param.value, input);
}

/**
* \sdd{WI-13616}
*/
TEST_F(RuntimeParametersTestFixture, WI_15178_test_Map_Cal_Settings_To_Runtime_Parameter_Legal_State_optional)
{
	Runtime_Parameter_Legal_State_T ret_legal_state_1;

	boolean_T must_be_set = FALSE;
	boolean_T prohibited_to_set = FALSE;
   /** \action call function under test */
    ret_legal_state_1 = Map_Cal_Settings_To_Runtime_Parameter_Legal_State(must_be_set,prohibited_to_set);
   /** \assert */
   EXPECT_EQ(ret_legal_state_1, RUNTIME_PARAMETER_OPTIONAL);


}

/**
* \sdd{WI-13616}
*/
TEST_F(RuntimeParametersTestFixture, WI_15179_test_Map_Cal_Settings_To_Runtime_Parameter_Legal_State_prohibited)
{

	Runtime_Parameter_Legal_State_T ret_legal_state_2;
	boolean_T must_be_set = FALSE;
	boolean_T prohibited_to_set = FALSE;
   /** \action call function under test */
    must_be_set = FALSE;
	prohibited_to_set = TRUE;
    ret_legal_state_2 = Map_Cal_Settings_To_Runtime_Parameter_Legal_State(must_be_set,prohibited_to_set);

   /** \assert */
   EXPECT_EQ(ret_legal_state_2, RUNTIME_PARAMETER_PROHIBITED);

}

/**
* \sdd{WI-13616}
*/
TEST_F(RuntimeParametersTestFixture, WI_15180_test_Map_Cal_Settings_To_Runtime_Parameter_Legal_State_prohibited2)
{

	Runtime_Parameter_Legal_State_T ret_legal_state_3;

	boolean_T must_be_set = FALSE;
	boolean_T prohibited_to_set = FALSE;
   /** \action call function under test */

    must_be_set = TRUE;
	prohibited_to_set = TRUE;
    ret_legal_state_3 = Map_Cal_Settings_To_Runtime_Parameter_Legal_State(must_be_set,prohibited_to_set);

   /** \assert */
   EXPECT_EQ(ret_legal_state_3, RUNTIME_PARAMETER_PROHIBITED);


}

/**
* \sdd{WI-13616}
*/
TEST_F(RuntimeParametersTestFixture, WI_15181_test_Map_Cal_Settings_To_Runtime_Parameter_Legal_State_mandatory)
{

	Runtime_Parameter_Legal_State_T ret_legal_state_4;
	boolean_T must_be_set = FALSE;
	boolean_T prohibited_to_set = FALSE;
   /** \action call function under test */

    must_be_set = TRUE;
	prohibited_to_set = FALSE;
    ret_legal_state_4 = Map_Cal_Settings_To_Runtime_Parameter_Legal_State(must_be_set,prohibited_to_set);

   /** \assert */

   EXPECT_EQ(ret_legal_state_4, RUNTIME_PARAMETER_MANDATORY);

}

/**
* \sdd{WI-13631}
*/
TEST_F(RuntimeParametersTestFixture, WI_15182_set_runtime_value_uint8_T)
{
	Runtime_Parameter_Uint8_T ret_runtime_param;
	uint8_t input = 2;
   /** \action call function under test */
   ret_runtime_param = Set_Runtime_Parameter_Extern_Uint8(input);
   /** \assert */
   EXPECT_TRUE(ret_runtime_param.is_set ==  TRUE &&
			ret_runtime_param.value == input);

}

/**
* \sdd{WI-13630}
*/
TEST_F(RuntimeParametersTestFixture, WI_15183_set_runtime_value_uint16_T)
{
   Runtime_Parameter_Uint16_T ret_runtime_param;
	uint16_t input = 5000;
   /** \action call function under test */
   ret_runtime_param = Set_Runtime_Parameter_Extern_Uint16(input);
   /** \assert */
   EXPECT_TRUE(ret_runtime_param.is_set ==  TRUE &&
			ret_runtime_param.value == input);

}

/**
* \sdd{WI-13629}
*/
TEST_F(RuntimeParametersTestFixture, WI_15184_set_runtime_value_uint32_T)
{
	Runtime_Parameter_Uint32_T ret_runtime_param;
	uint32_t input = 50000;
   /** \action call function under test */
   ret_runtime_param = Set_Runtime_Parameter_Extern_Uint32(input);
   /** \assert */
   EXPECT_TRUE(ret_runtime_param.is_set ==  TRUE &&
			ret_runtime_param.value == input);

}

/**
* \sdd{WI-13628}
*/
TEST_F(RuntimeParametersTestFixture, WI_15185_set_runtime_value_int8_T)
{
	Runtime_Parameter_Int8_T ret_runtime_param;
	int8_t input = 8;
   /** \action call function under test */
   ret_runtime_param = Set_Runtime_Parameter_Extern_Int8(input);
   /** \assert */
   EXPECT_TRUE(ret_runtime_param.is_set ==  TRUE &&
			ret_runtime_param.value == input);

}

/**
* \sdd{WI-13627}
*/
TEST_F(RuntimeParametersTestFixture, WI_15187_set_runtime_value_int16_T)
{
	Runtime_Parameter_Int16_T ret_runtime_param;
	int16_t input = 5000;
   /** \action call function under test */
   ret_runtime_param = Set_Runtime_Parameter_Extern_Int16(input);
   /** \assert */
   EXPECT_TRUE(ret_runtime_param.is_set ==  TRUE &&
			ret_runtime_param.value == input);

}

/**
* \sdd{WI-13626}
*/
TEST_F(RuntimeParametersTestFixture, WI_15188_set_runtime_value_int32_T)
{
	Runtime_Parameter_Int32_T ret_runtime_param;
	int32_t input = 5000;
   /** \action call function under test */
   ret_runtime_param = Set_Runtime_Parameter_Extern_Int32(input);
   /** \assert */
   EXPECT_TRUE(ret_runtime_param.is_set ==  TRUE &&
			ret_runtime_param.value == input);

}

/**
* \sdd{WI-13625}
*/
TEST_F(RuntimeParametersTestFixture, WI_15189_set_runtime_value_float2_T)
{
	Runtime_Parameter_Float32_T ret_runtime_param;
	float32_T input = 16.78f;
   /** \action call function under test */
   ret_runtime_param = Set_Runtime_Parameter_Extern_Float32(input);
   /** \assert */
   EXPECT_TRUE(ret_runtime_param.is_set ==  TRUE &&
			ret_runtime_param.value == input);

}

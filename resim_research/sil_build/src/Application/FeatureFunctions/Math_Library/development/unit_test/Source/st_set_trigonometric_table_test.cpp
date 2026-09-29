#include "gtest/gtest.h"
#include "ml_macros.h"
#include "ml_math.h"
#include "ml_set_trigonometric_table.h"
#include "ml_trigonometry.h"
#include "ml_serial_buffer_t.h"
#include "ml_serialization_error_t.h"
#include "ml_serial_buffer_t.h"


#define EXPECTED_ACCURACY_FAST_MATH (0.005f)

static uint32_t checksum_stream_le[] = { 0xfafbfcfd, 0xc362f617, 0x9c203371 };

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
static void Swap_Endianness(uint32_t *p_data)
{
   uint32_t data_enty = *p_data;
   char *p_byte_source = (char*)&data_enty;
   char *p_byte_target = (char*)p_data;
   p_byte_target[0] = p_byte_source[3];
   p_byte_target[1] = p_byte_source[2];
   p_byte_target[2] = p_byte_source[1];
   p_byte_target[3] = p_byte_source[0];
}
#endif

TEST(Serialize_Trig_Table_Checksum, result_as_expected)
{
   /** \arrange save the existing table content to be able to set it back after the test */
   Shared_Toolbox_Serial_Buffer_T saved_tables;
   char data_saved_tables[1000000];
   saved_tables.p_data = data_saved_tables;
   saved_tables.length = 1000000;
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(&saved_tables));

   uint32_t data[12];
   Shared_Toolbox_Serial_Buffer_T st_serialized_buffer_le_checksum;
   st_serialized_buffer_le_checksum.p_data = (char*)&data[0];
   st_serialized_buffer_le_checksum.length = 12;
   Compute_Trig_Tables();
   Shared_Toolbox_Serialization_Error_T ret = Serialize_Trig_Table_Checksum(&st_serialized_buffer_le_checksum);
   if (ret != SHARED_TOOLBOX_SRL_ERR_UNKNOWN)
   {
      /* otherwise the ST is configured to not have this function */
         EXPECT_EQ(data[1], 2663727851);
         EXPECT_EQ(data[2], 2615703301);
   }

   /** \arrange set the table content back after the test */
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Trig_Table(&saved_tables));
}

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
/* These tests do not make sense if the actual function implementation is not present */
TEST(Serialize_Trig_Table_Checksum, called_with_null_pointer)
{
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PTR, Serialize_Trig_Table_Checksum(NULL));
}


TEST(Serialize_Trig_Table_Checksum, not_enough_memory)
{
   uint32_t data[3];
   Shared_Toolbox_Serial_Buffer_T st_serialized_buffer_le_checksum;
   st_serialized_buffer_le_checksum.p_data = (char*)&data[0];
   st_serialized_buffer_le_checksum.length = 10;
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_NOMEM, Serialize_Trig_Table_Checksum(&st_serialized_buffer_le_checksum));
}
#endif

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
/**
* Try to set tables with predefined big endian stream
*/
TEST(SetTrigTableByChecksum, BE)
{
   /** \arrange swap endianness of predefined checksum stream */
   uint32_t checksum_data[3];
   for (size_t i = 0; i < 3; i++)
   {
      checksum_data[i] = checksum_stream_le[i];
      Swap_Endianness(&checksum_data[i]);
   }
   Shared_Toolbox_Serial_Buffer_T st_serialized_buffer_checksum_data_be;
   st_serialized_buffer_checksum_data_be.p_data = (char*)&checksum_data[0];
   st_serialized_buffer_checksum_data_be.length = 3 * sizeof(uint32_t);
   EXPECT_EQ(Set_Trig_Table_By_Checksum(&st_serialized_buffer_checksum_data_be), SHARED_TOOLBOX_SRL_SUCCESS);
}

/**
* Try to set tables with predefined little endian stream but faulty magic number
*/
TEST(SetTrigTableByChecksum, LE_wrong_magic)
{
   /** \arrange swap endianness of predefined checksum stream */
   uint32_t checksum_data[3];
   for (size_t i = 0; i < 3; i++)
   {
      checksum_data[i] = checksum_stream_le[i];
   }
   checksum_data[0] = 0x00;
   Shared_Toolbox_Serial_Buffer_T st_serialized_buffer_checksum_data_be;
   st_serialized_buffer_checksum_data_be.p_data = (char*)&checksum_data[0];
   st_serialized_buffer_checksum_data_be.length = 3 * sizeof(uint32_t);
   EXPECT_EQ(Set_Trig_Table_By_Checksum(&st_serialized_buffer_checksum_data_be), SHARED_TOOLBOX_SRL_ERR_PARSE);
}

/**
* Try to set tables with predefined little endian stream but faulty checksum 1
*/
TEST(SetTrigTableByChecksum, LE_wrong_cs1)
{
   /** \arrange swap endianness of predefined checksum stream */
   uint32_t checksum_data[3];
   for (size_t i = 0; i < 3; i++)
   {
      checksum_data[i] = checksum_stream_le[i];
   }
   checksum_data[1] = 0x00;
   Shared_Toolbox_Serial_Buffer_T st_serialized_buffer_checksum_data_be;
   st_serialized_buffer_checksum_data_be.p_data = (char*)&checksum_data[0];
   st_serialized_buffer_checksum_data_be.length = 3 * sizeof(uint32_t);
   EXPECT_EQ(Set_Trig_Table_By_Checksum(&st_serialized_buffer_checksum_data_be), SHARED_TOOLBOX_SRL_ERR_PARSE);
}

/**
* Try to set tables with predefined little endian stream but faulty checksum 2
*/
TEST(SetTrigTableByChecksum, LE_wrong_cs2)
{
   /** \arrange swap endianness of predefined checksum stream */
   uint32_t checksum_data[3];
   for (size_t i = 0; i < 3; i++)
   {
      checksum_data[i] = checksum_stream_le[i];
   }
   checksum_data[2] = 0x00;
   Shared_Toolbox_Serial_Buffer_T st_serialized_buffer_checksum_data_be;
   st_serialized_buffer_checksum_data_be.p_data = (char*)&checksum_data[0];
   st_serialized_buffer_checksum_data_be.length = 3 * sizeof(uint32_t);
   EXPECT_EQ(Set_Trig_Table_By_Checksum(&st_serialized_buffer_checksum_data_be), SHARED_TOOLBOX_SRL_ERR_PARSE);
}

/**
* FastMathFixture tests that run from -4*pi till 4*pi
*/
class SetTrigTableByChecksumParam8piFixture:
   public ::testing::TestWithParam<float>
{
protected:
   SetTrigTableByChecksumParam8piFixture()
   {
      Shared_Toolbox_Serial_Buffer_T st_serialized_buffer;
      st_serialized_buffer.p_data = (char*)&checksum_stream_le[0];
      st_serialized_buffer.length = 12;
      EXPECT_EQ(Set_Trig_Table_By_Checksum(&st_serialized_buffer), SHARED_TOOLBOX_SRL_SUCCESS) << " Setting by little endian checksum failed ";
   }
};

/**
* FastMathFixture tests that run from -4*pi till 4*pi
*/
INSTANTIATE_TEST_SUITE_P(ProvidedValues8pi,
   SetTrigTableByChecksumParam8piFixture,
   testing::Range(-(4.0f * PI), (4.0f * PI), 0.001f*PI) /* Input values to be tested  */
);

/**
* Comparing the result of the fast FUT with the matching math.h function for a range of values
*/
TEST_P(SetTrigTableByChecksumParam8piFixture, Fast_Cos_test)
{
   float32_T test_value = GetParam();
   float32_T fast_result = Fast_Cos(test_value);
   EXPECT_NEAR(fast_result, cosf(test_value), EXPECTED_ACCURACY_FAST_MATH);
}

/**
* Comparing the result of the fast FUT with the matching math.h function for a range of values
*/
TEST_P(SetTrigTableByChecksumParam8piFixture, Fast_Sin_test)
{
   float32_T test_value = GetParam();
   float32_T fast_result = Fast_Sin(test_value);
   EXPECT_NEAR(fast_result, sinf(test_value), EXPECTED_ACCURACY_FAST_MATH);
}

/**
* Try to set tables with predefined little endian stream
*/
TEST(SetTrigTableByChecksum, LE)
{
   Shared_Toolbox_Serial_Buffer_T st_serialized_buffer_le_checksum;
   st_serialized_buffer_le_checksum.p_data = (char*)&checksum_stream_le[0];
   st_serialized_buffer_le_checksum.length = 12;
   EXPECT_EQ(Set_Trig_Table_By_Checksum(&st_serialized_buffer_le_checksum), SHARED_TOOLBOX_SRL_SUCCESS);
}

/**
* Call FUT with null pointer
*/
TEST(SetTrigTableByChecksum, null)
{
   EXPECT_EQ(Set_Trig_Table_By_Checksum(NULL), SHARED_TOOLBOX_SRL_ERR_PTR);
}

/**
* Try to set tables with predefined little endian stream
*/
TEST(SetTrigTableByChecksum, buffer_too_small)
{
   Shared_Toolbox_Serial_Buffer_T st_serialized_buffer_le_checksum;
   st_serialized_buffer_le_checksum.p_data = (char*)&checksum_stream_le[0];
   st_serialized_buffer_le_checksum.length = 10;
   EXPECT_EQ(Set_Trig_Table_By_Checksum(&st_serialized_buffer_le_checksum), SHARED_TOOLBOX_SRL_ERR_NOMEM);
}
#else

/**
* The function is not supposed to be called (since it cannot operate in this mode)
*/
TEST(SetTrigTableByChecksum, Returning_error_in_unsupported_mode )
{
   Shared_Toolbox_Serial_Buffer_T st_serialized_buffer_le_checksum;
   st_serialized_buffer_le_checksum.p_data = (char*)&checksum_stream_le[0];
   st_serialized_buffer_le_checksum.length = 10;
   EXPECT_EQ(Set_Trig_Table_By_Checksum(&st_serialized_buffer_le_checksum), SHARED_TOOLBOX_SRL_ERR_UNKNOWN);
}
#endif

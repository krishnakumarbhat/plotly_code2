#ifndef FAST_MATH_SERIALIZATION_FIXTURE_HPP
#define FAST_MATH_SERIALIZATION_FIXTURE_HPP
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "ml_exp.h"
#include "ml_macros.h"
#include "ml_trigonometry.h"
#include "ml_serial_buffer_t.h"
#include "ml_serial_buffer_t.h"


/**
* Test fixture for fast math table serialization
*/
class FastMathSerializationFixture :
   public ::testing::Test
{
public:
   FastMathSerializationFixture()
   {
      /** \arrange Since these tests mess with the table content we need to restore the table content after the test did run.
      * Therefore the table is serialized and later deserialized again */
      saved_trig_tables.p_data = data_saved_trig_tables;
      saved_trig_tables.length = 1000000;
      EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(&saved_trig_tables));
      saved_exp_tables.p_data = data_saved_exp_tables;
      saved_exp_tables.length = 1000000;
      EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Exp_Table(&saved_exp_tables));

      /** \arrange Prepare two serialization buffers for the tests to use */
      buffer.p_data = data;
      buffer.length = 1000000;
      buffer2.p_data = data2;
      buffer2.length = 1000000;

      /** \arrange compute the tables */
      Compute_Exp_Table();
      Compute_Trig_Tables();
   }
   ~FastMathSerializationFixture() override
   {
      /** \arrange After the test did run reset the tables to its original content*/
      saved_trig_tables.p_data = data_saved_trig_tables;
      saved_trig_tables.length = 1000000;
      EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Trig_Table(&saved_trig_tables));
      saved_exp_tables.p_data = data_saved_exp_tables;
      saved_exp_tables.length = 1000000;
      EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Exp_Table(&saved_exp_tables));
   }
   /**
   * Swaps the endianness of the given buffer
   */
   static void Swap_Endianness(Shared_Toolbox_Serial_Buffer_T *p_buffer /**< Buffer to swap endianness in */)
   {
      for (size_t i = 0; i < p_buffer->length; i = i + 4)
      {
         Swap(p_buffer->p_data[i], p_buffer->p_data[i + 3], char);
         Swap(p_buffer->p_data[i + 1], p_buffer->p_data[i + 2], char);
      }
   }
protected:
   char data[1000000];
   char data2[1000000];
   Shared_Toolbox_Serial_Buffer_T buffer;
   Shared_Toolbox_Serial_Buffer_T buffer2;
   Shared_Toolbox_Serial_Buffer_T saved_trig_tables;
   char data_saved_trig_tables[1000000];

   Shared_Toolbox_Serial_Buffer_T saved_exp_tables;
   char data_saved_exp_tables[1000000];
};

#endif

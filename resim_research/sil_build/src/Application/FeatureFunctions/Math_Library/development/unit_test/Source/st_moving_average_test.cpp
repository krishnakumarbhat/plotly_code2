/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>

#include "reuse.h"
#include "ml_moving_average.h"
#include "ml_moving_average_filter_instance_t.h"

class TestMovingAverage : public ::testing::Test
{
public:
   static void Apply_Data_To_Filter(
      Moving_Average_Filter_Instance_T *p_filter_inst,
      float32_T                        *m_input_data_points_array,
      int32_t                           num_data_points)
   {
      for (int idx = 0; idx < num_data_points; idx++)
      {
         Moving_Average_Run(p_filter_inst, m_input_data_points_array[idx]);
      }
   }

   static void Apply_Data_To_Filter(
      Moving_Average_Filter_Instance_T *p_filter_inst,
      float32_T                         input_data_point,
      int32_t                           repetitions)
   {
      for (int idx = 0; idx < repetitions; idx++)
      {
         Moving_Average_Run(p_filter_inst, input_data_point);
      }
   }

protected:
   uint8_t m_interval_to_average = 5u;
   uint8_t m_num_m_input_data_points = 10u;

   float32_T m_input_data_points[10] = { 1.0f, 4.0f, 10.0f, 20.0f, 22.0f, 18.0f, 26.0f, 16.0f, 25.0f, 30.0f};
   float32_T m_data_buffer[5] = {0,0,0,0,0};
};


TEST_F(TestMovingAverage, demo)
{
   /**[Moving_Average]*/
#define DATA_BUFFER_SIZE    (5)
   /* prepare the filter */
   uint8_t window_size = DATA_BUFFER_SIZE;       /* window size to be used in buffer */
   Moving_Average_Filter_Instance_T filter_inst; /* Instance of the filter buffer */
   float data_buf[DATA_BUFFER_SIZE];             /* data buffer used for filtering */
   float filtered_value;                         /* to store the result */

   /* initialize the filter instance */
   filter_inst = Moving_Average_Init(window_size, data_buf);
   /* call the filter a bit more often than the buffer is long */
   for (int i = 0; i < DATA_BUFFER_SIZE + 3; i++)
   {
      /* Call the moving average filter a bunch of times */
      filtered_value = Moving_Average_Run(&filter_inst, (float)i);
   }
   /* Expect the result to be the average of the last 5 values (3, 4, 5, 6 and 7 sum up to 25) that have been send into the buffer */
   EXPECT_EQ(filtered_value, 25.0f / window_size);
   /**[Moving_Average]*/
}

/**
 * \sdd{WI-13805}
 */
TEST_F(TestMovingAverage, WI_15085_Moving_Average_Init__sets_ringbuffer_size_equal_to_m_interval_to_average)
{
   /** \arrange */
   Moving_Average_Filter_Instance_T filter_inst;

   /** \action call function under test */
   filter_inst = Moving_Average_Init(m_interval_to_average, m_data_buffer);

   /** \assert */
   ASSERT_EQ(filter_inst.ringbuffer_size, m_interval_to_average);
}

/**
 * \sdd{WI-13805}
 */
TEST_F(TestMovingAverage, WI_15086_Moving_Average_Init__sets_inverse_size_equal_to_inverse_of_m_interval_to_average)
{
   /** \arrange */
   Moving_Average_Filter_Instance_T filter_inst;

   /** \action call function under test */
   filter_inst = Moving_Average_Init(m_interval_to_average, m_data_buffer);

   /** \assert */
   ASSERT_EQ(filter_inst.inverse_size, 1.0f / m_interval_to_average);
}

/**
 * \sdd{WI-13805}
 */
TEST_F(TestMovingAverage, WI_15087_Moving_Average_Init__sets_averaged_cycles_to_zero)
{
   /** \arrange */
   Moving_Average_Filter_Instance_T filter_inst;

   /** \action call function under test */
   filter_inst = Moving_Average_Init(m_interval_to_average, m_data_buffer);

   /** \assert */
   ASSERT_EQ(filter_inst.averaged_cycles, 0);
}


/**
 * \sdd{WI-13805}
 */
TEST_F(TestMovingAverage, WI_15088_Moving_Average_Init__sets_first_empty_place_in_ringbuffer_to_zero)
{
   /** \arrange */
   Moving_Average_Filter_Instance_T filter_inst;

   /** \action call function under test */
   filter_inst = Moving_Average_Init(m_interval_to_average, m_data_buffer);

   /** \assert */
   ASSERT_EQ(filter_inst.first_empty_place_in_ringbuffer, 0);
}

/**
 * \sdd{WI-13805}
 */
TEST_F(TestMovingAverage, WI_15089_Moving_Average_Init__sets_ringbuffer_array_to_address_of_input_m_data_buffer)
{
   /** \arrange */
   Moving_Average_Filter_Instance_T filter_inst;

   /** \action call function under test */
   filter_inst = Moving_Average_Init(m_interval_to_average, m_data_buffer);

   /** \assert */
   ASSERT_EQ(filter_inst.ringbuffer_array, m_data_buffer);
}

/**
 * \sdd{WI-13805}
 */
TEST_F(TestMovingAverage, WI_15090_Moving_Average_Init__sets_values_in_ringbuffer_array_to_zero)
{
   /** \arrange */
   Moving_Average_Filter_Instance_T filter_inst;
   uint8_t idx;

   /** \action call function under test */
   filter_inst = Moving_Average_Init(m_interval_to_average, m_data_buffer);

   /** \assert */
   // Check if all the values in the data buffer are also reset to zero
   for (idx = 0; idx < m_interval_to_average; idx++)
   {
      ASSERT_FLOAT_EQ(m_data_buffer[idx], 0.0f);
   }
}


/**
 * Resets the moving average ring buffer entries to zero.
 * \sdd{WI-13803}
 */
TEST_F(TestMovingAverage, WI_15629_Moving_Average_Reset_Ringbuffer__sets_values_in_ringbuffer_array_to_zero)
{
   Moving_Average_Filter_Instance_T filter_inst;
   uint8_t idx;

   /** \arrange
    * set up a ring buffer filled with the input data points from test setup
    */
   filter_inst.ringbuffer_size  = m_num_m_input_data_points;
   filter_inst.ringbuffer_array = m_input_data_points;

   /** \action call function under test */
   Moving_Average_Reset_Ringbuffer(&filter_inst);

   /** \assert
    * all the values in the data buffer are reset to zero
    */
   for (idx = 0; idx < filter_inst.ringbuffer_size; idx++)
   {
      ASSERT_FLOAT_EQ(filter_inst.ringbuffer_array[idx], 0.0f);
   }
}


/**
 * Resets the moving average filter entries to zero.
 * \sdd{WI-13804}
 */
TEST_F(TestMovingAverage, WI_15630_Moving_Average_Reset__sets_values_in_filter_to_zero)
{
   Moving_Average_Filter_Instance_T filter_inst;
   uint8_t idx;

   /** \arrange
    * set up a ring buffer filled with the input data points from test setup
    */
   filter_inst.ringbuffer_size  = m_num_m_input_data_points;
   filter_inst.ringbuffer_array = m_input_data_points;

   /** \arrange
    * set the current ring buffer position and averaged cycles to non-zero values
    */
   filter_inst.averaged_cycles = 5;
   filter_inst.first_empty_place_in_ringbuffer = 1;

   /** \action call function under test */
   Moving_Average_Reset(&filter_inst);

   /** \assert
    * all the values in the data buffer are reset to zero
    */
   for (idx = 0; idx < filter_inst.ringbuffer_size; idx++)
   {
      ASSERT_FLOAT_EQ(filter_inst.ringbuffer_array[idx], 0.0f);
   }

   /** \assert
    * the current ring buffer position and averaged cycles are zero
    */
   ASSERT_EQ(filter_inst.averaged_cycles, 0);
   ASSERT_EQ(filter_inst.averaged_cycles, 0);
}


/**
 * \sdd{WI-13802}
 */
TEST_F(TestMovingAverage, WI_15091_Moving_Average_Run__averages_when_num_input_values_is_less_than_m_interval_to_average)
{
   /** \arrange */
   Moving_Average_Filter_Instance_T filter_inst;
   uint8_t   idx;
   float32_T filtered_value = 0.0f;

   filter_inst = Moving_Average_Init(m_interval_to_average, m_data_buffer);

   /** \action call function under test */
   for (idx = 0; idx < 3; idx++)
   {
      filtered_value = Moving_Average_Run(&filter_inst, m_input_data_points[idx]);
   }

   /** \assert */
   ASSERT_FLOAT_EQ(filtered_value, 5.0f);
}


/**
 * \sdd{WI-13802}
 */
TEST_F(TestMovingAverage, WI_15092_Moving_Average_Run__averages_when_num_input_values_greater_than_m_interval_to_average)
{
   /** \arrange */
   Moving_Average_Filter_Instance_T filter_inst;
   uint8_t   idx;
   float32_T filtered_value = 0.0f;

   filter_inst = Moving_Average_Init(m_interval_to_average, m_data_buffer);

   /** \action call function under test */
   for (idx = 0; idx < 8; idx++)
   {
      filtered_value = Moving_Average_Run(&filter_inst, m_input_data_points[idx]);
   }

   /** \assert */
   // Expected value is the average of the last values at indexes  3, 4, 5, 6, 7 in the input array
   ASSERT_FLOAT_EQ(filtered_value, 20.4f);
}

/**
 * \sdd{WI-13802}
 */
TEST_F(TestMovingAverage, WI_15093_Moving_Average_Run__averages_when_num_input_values_equals_m_interval_to_average)
{
   /** \arrange */
   Moving_Average_Filter_Instance_T filter_inst;
   uint8_t   idx;
   float32_T filtered_value = 0.0f;

   filter_inst = Moving_Average_Init(m_interval_to_average, m_data_buffer);

   /** \action call function under test */
   for (idx = 0; idx < m_interval_to_average; idx++)
   {
      filtered_value = Moving_Average_Run(&filter_inst, m_input_data_points[idx]);
   }

   /** \assert */
   // Expected value is the average of the first 5 values in the input array
   ASSERT_FLOAT_EQ(filtered_value, 11.4f);
}


/**
 * \sdd{WI-13802}
 */
TEST_F(TestMovingAverage, WI_15094_Moving_Average_Run__averages_correctly_when_more_than_one_filter_instance_is_present)
{
   /** \arrange */
   Moving_Average_Filter_Instance_T filter_inst_a;
   Moving_Average_Filter_Instance_T filter_inst_b;

   uint8_t idx;

   float32_T filtered_value_a = 0.0f;
   float32_T filtered_value_b = 0.0f;

   const uint8_t   m_interval_to_average_b  = 2;
   const float32_T m_input_data_points_b[4] = { 9.0f, 9.0f, 0.0f, 0.0f };
   float32_T       m_data_buffer_b[m_interval_to_average_b];

   // Filter A will use the data from the test fixture
   filter_inst_a = Moving_Average_Init(m_interval_to_average, m_data_buffer);

   filter_inst_b = Moving_Average_Init(m_interval_to_average_b, m_data_buffer_b);


   /** \action call function under test */
   // Run filter A
   for (idx = 0; idx < 8; idx++)
   {
      filtered_value_a = Moving_Average_Run(&filter_inst_a, m_input_data_points[idx]);
   }

   // Run filter B
   for (idx = 0; idx < 4; idx++)
   {
      filtered_value_b = Moving_Average_Run(&filter_inst_b, m_input_data_points_b[idx]);
   }

   /** \assert */
   // Expected value for filter A is the average of the values at indexes  3, 4, 5, 6, 7 in the input array
   ASSERT_FLOAT_EQ(filtered_value_a, 20.4f);

   // Expected value for filter B is the average of the last 2 values - zeros
   ASSERT_FLOAT_EQ(filtered_value_b, 0.0f);
}

/**
 * \sdd{WI-13802}
 */
TEST_F(TestMovingAverage, WI_15095_Moving_Average_Run__averages_when_num_input_values_equals_m_interval_to_average__test2)
{
   /** \arrange */
   Moving_Average_Filter_Instance_T filter_inst_b;

   uint8_t idx;

   float32_T filtered_value_b = 0.0f;

   const uint8_t   m_interval_to_average_b  = 2;
   const float32_T m_input_data_points_b[4] = { 9.0f, 9.0f, 0.0f, 0.0f };
   float32_T       m_data_buffer_b[m_interval_to_average_b];

   filter_inst_b = Moving_Average_Init(m_interval_to_average_b, m_data_buffer_b);


   /** \action call function under test */

   // Run filter B
   for (idx = 0; idx < 4; idx++)
   {
      filtered_value_b = Moving_Average_Run(&filter_inst_b, m_input_data_points_b[idx]);
   }

   /** \assert */
   // Expected value for filter B is the average of the last 2 values - zeros
   ASSERT_FLOAT_EQ(filtered_value_b, 0.0f);
}


/**
 * \sdd{WI-13802}
 */
TEST_F(TestMovingAverage, WI_15096_Moving_Average_Run__should_be_invariant_to_periodic_values_if_period_is_bigger_then_the_filter_size)
{
   /** \arrange */
   const int32_t   m_interval_to_average_start = 5;
   const int32_t   m_data_buffer_size          = 5;
   const float32_T probe_value = 1.0f;

   Moving_Average_Filter_Instance_T filter_inst;
   float32_T local_m_data_buffer[m_data_buffer_size];
   float32_T filtered_value_start;
   float32_T filtered_value_end;

   filter_inst = Moving_Average_Init(m_interval_to_average_start, local_m_data_buffer);

   /** \action call function under test */
   Apply_Data_To_Filter(&filter_inst, m_input_data_points, m_num_m_input_data_points);
   filtered_value_start = Moving_Average_Run(&filter_inst, probe_value);

   Apply_Data_To_Filter(&filter_inst, m_input_data_points, m_num_m_input_data_points);
   filtered_value_end = Moving_Average_Run(&filter_inst, probe_value);

   /** \assert */
   ASSERT_FLOAT_EQ(filtered_value_start, filtered_value_end);
}

/**
 * \sdd{WI-13802}
 */
TEST_F(TestMovingAverage, WI_15097_Moving_Average_Run__should_be_invariant_if_filter_size_is_reduced)
{
   /** \arrange */
   const int32_t   m_interval_to_average_start = 4;
   const int32_t   m_interval_to_average_end   = 2;
   const int32_t   m_data_buffer_size          = 20;
   const float32_T probe_value = 1.0f;

   Moving_Average_Filter_Instance_T filter_inst;
   float32_T local_m_data_buffer[m_data_buffer_size];
   float32_T filtered_value_end;

   filter_inst = Moving_Average_Init(m_interval_to_average_start, local_m_data_buffer);
   Apply_Data_To_Filter(&filter_inst, m_input_data_points, m_num_m_input_data_points);

   /** \action call function under test */
   filter_inst.ringbuffer_size = m_interval_to_average_end;
   filter_inst.inverse_size    = 1.0f / m_interval_to_average_end;
   Apply_Data_To_Filter(&filter_inst, probe_value, m_interval_to_average_end);
   filtered_value_end = Moving_Average_Run(&filter_inst, probe_value);

   /** \assert */
   ASSERT_FLOAT_EQ(probe_value, filtered_value_end);
}

/**
 * \sdd{WI-13802}
 */
TEST_F(TestMovingAverage, WI_15098_Moving_Average_Run__should_be_invariant_if_filter_size_is_increased_with_sufficient_input)
{
   /** \arrange */
   const int32_t   m_interval_to_average_start = 2;
   const int32_t   m_interval_to_average_end   = 4;
   const int32_t   m_data_buffer_size          = 20;
   const float32_T probe_value = 1.0f;

   Moving_Average_Filter_Instance_T filter_inst;
   float32_T local_m_data_buffer[m_data_buffer_size];
   float32_T filtered_value_end;

   filter_inst = Moving_Average_Init(m_interval_to_average_start, local_m_data_buffer);
   Apply_Data_To_Filter(&filter_inst, m_input_data_points, m_num_m_input_data_points);

   /** \action call function under test */
   filter_inst.ringbuffer_size = m_interval_to_average_end;
   filter_inst.inverse_size    = 1.0f / m_interval_to_average_end;
   Apply_Data_To_Filter(&filter_inst, probe_value, m_interval_to_average_end);
   filtered_value_end = Moving_Average_Run(&filter_inst, probe_value);

   /** \assert */
   ASSERT_FLOAT_EQ(probe_value, filtered_value_end);
}


/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"
#include <assert.h>
#include "ml_macros.h"
#include "ml_moving_average.h"

/**
 * The sum over the ring buffer is computed. Only filled elements are used.
 * \return the sum of all elements in the given ring buffer
 * \ingroup moving_average
 */
static float32_T Moving_Average_Sum_Over_Ringbuffer(const Moving_Average_Filter_Instance_T *const p_internal_filter_status /**< Ring buffer to be summed up */);

Moving_Average_Filter_Instance_T Moving_Average_Init(
   const uint8_t    interval_to_average,
   float32_T *const p_ringbuffer_array)
{
   Moving_Average_Filter_Instance_T ret_value;

   /** \throws Assert if ring buffer_array is a NULL pointer */
   assert(NULL != p_ringbuffer_array);
   /** \throws Assert if Interval_To_Average is zero */
   assert(0 < interval_to_average);

   ret_value.ringbuffer_array = p_ringbuffer_array;
   ret_value.ringbuffer_size  = interval_to_average;
   ret_value.inverse_size     = 1.0f / (float)interval_to_average;
   Moving_Average_Reset(&ret_value);

   return ret_value;
}


void Moving_Average_Reset(Moving_Average_Filter_Instance_T *const p_internal_filter_status)
{
   /** \throws Assert if p_internal_filter_status is a NULL pointer */
   assert(NULL != p_internal_filter_status);
   /** \throws Assert if ringbuffer_array is a NULL pointer */
   assert(NULL != p_internal_filter_status->ringbuffer_array);

   Moving_Average_Reset_Ringbuffer(p_internal_filter_status);
   p_internal_filter_status->first_empty_place_in_ringbuffer = 0u;
   p_internal_filter_status->averaged_cycles = 0u;
}


void Moving_Average_Reset_Ringbuffer(Moving_Average_Filter_Instance_T *const p_internal_filter_status)
{
   uint8_t idx;

   /** \throws Assert if p_internal_filter_status is a NULL pointer */
   assert(NULL != p_internal_filter_status);
   /** \throws Assert if ringbuffer_array is a NULL pointer */
   assert(NULL != p_internal_filter_status->ringbuffer_array);

   for (idx = 0; idx < p_internal_filter_status->ringbuffer_size; idx++)
   {
      p_internal_filter_status->ringbuffer_array[idx] = 0.f;
   }
}


float32_T Moving_Average_Run(
   Moving_Average_Filter_Instance_T *const p_internal_filter_status,
   const float32_T                         value)
{
   float32_T filtered_value = 0.0f;

   /** \throws Assert if p_internal_filter_status is a NULL pointer */
   assert(NULL != p_internal_filter_status);
   /** \throws Assert if ringbuffer_array is a NULL pointer */
   assert(NULL != p_internal_filter_status->ringbuffer_array);

   /* Save the new value in the ring buffer */
   p_internal_filter_status->ringbuffer_array[p_internal_filter_status->first_empty_place_in_ringbuffer] = value;

   /* Update the first empty place */
   p_internal_filter_status->first_empty_place_in_ringbuffer = (p_internal_filter_status->first_empty_place_in_ringbuffer + 1u) % p_internal_filter_status->ringbuffer_size;

   if (p_internal_filter_status->averaged_cycles < p_internal_filter_status->ringbuffer_size)
   {
      p_internal_filter_status->averaged_cycles++;
      filtered_value = Moving_Average_Sum_Over_Ringbuffer(p_internal_filter_status) / (float)p_internal_filter_status->averaged_cycles;
   }
   else
   {
      /* Saturate the averaged cycles to the size of the ring buffer */
      p_internal_filter_status->averaged_cycles = p_internal_filter_status->ringbuffer_size;

      filtered_value = Moving_Average_Sum_Over_Ringbuffer(p_internal_filter_status) * p_internal_filter_status->inverse_size;
   }

   return filtered_value;
}


static float32_T Moving_Average_Sum_Over_Ringbuffer(const Moving_Average_Filter_Instance_T *const p_internal_filter_status)
{
   uint8_t idx;
   float   ret_value;

   /** \throws Assert if p_internal_filter_status is a NULL pointer */
   assert(NULL != p_internal_filter_status);
   /** \throws Assert if ringbuffer_array is a NULL pointer */
   assert(NULL != p_internal_filter_status->ringbuffer_array);

   ret_value = 0;

   for (idx = 0; idx < p_internal_filter_status->ringbuffer_size; idx++)
   {
      ret_value += p_internal_filter_status->ringbuffer_array[idx];
   }

   return ret_value;
}


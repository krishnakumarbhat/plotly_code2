/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include "reuse.h"
#include <assert.h>
#include "ml_line.h"
#include "ml_line_parameter.h"
#include "ml_lookup_table_2d.h"
#include "ml_math_infinity_silent.h"

float32_T Get_Value_From_2d_Lookup_Table(
   const float32_T *const p_table_x,
   const float32_T *const p_table_y,
   const uint8_t          size,
   const float32_T        x)
{
   float32_T ret_value = AS_TOOLBOX_INFINITY;
   uint8_t   loop_idx;

   if (x < p_table_x[0])
   {
      ret_value = p_table_y[0];
   }
   else if (p_table_x[size - 1] < x)
   {
      ret_value = p_table_y[size - 1];
   }
   else
   {
      for (loop_idx = 1; loop_idx < size; loop_idx++)
      {
         assert(p_table_x[loop_idx - 1] <= p_table_x[loop_idx]);
         if ((p_table_x[loop_idx - 1] <= x) && (x <= p_table_x[loop_idx]))
         {
            ret_value = Get_Y_Value_From_Line_By_Coordinates(p_table_x[loop_idx - 1], p_table_y[loop_idx - 1], p_table_x[loop_idx], p_table_y[loop_idx], x);
            break;
         }
      }
   }
   assert(ret_value != AS_TOOLBOX_INFINITY);
   return ret_value;
}


float32_T Interpolate_To_Zero(
   const float32_T x,
   const float32_T x_threshold,
   const float32_T y_threshold)
{
   float32_T ret_value;

   if (x < x_threshold)      //TODO check whether >0 is necessary
   {
      if (x < 0)
      {
         ret_value = 0;
      }
      else
      {
         ret_value = y_threshold * (x / x_threshold);
      }
   }
   else
   {
      ret_value = y_threshold;
   }

   return ret_value;
}


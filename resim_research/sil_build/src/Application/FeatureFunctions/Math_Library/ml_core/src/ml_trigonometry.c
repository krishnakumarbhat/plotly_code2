/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"
#include "ml_fast_math_table_macros.h"
#include "ml_checksum.h"
#include "ml_interval.h"
#include "ml_macros.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include <math.h>
#include "ml_bool.h"


#include <assert.h>

/**
 * Returns the angle for given sides adjacent_left, adjacent_right and opposite
 *
 * \return    float angle
 * \ingroup trigonometric_functions_triangles
 */
static float Triangle_Angle_From_Sides(
   float adjacent_left,  /**< Side left of angle to calculate */
   float adjacent_right, /**< Side right of angle to calculate */
   float opposite        /**< Side opposite of angle to calculate */
);

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE != ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION
static const float32_T HALF_PI = (((float32_T)PI) * 0.5f); /** HALF_PI is only needed in this module and not needed if fast math library is not used */
#endif

/**
 * Threshold to check if there is a rounding error
 * \ingroup trigonometric_functions_triangles
 */
#define TRIANGLE_THRESHOLD_ROUNDING_ERROR (( float)1E-3)

static float Triangle_Angle_From_Sides(
   float adjacent_left,
   float adjacent_right,
   float opposite)
{
   float angle_ret = 0.0f;

   assert(adjacent_left > THRESHOLD_IS_ZERO);
   assert(adjacent_right > THRESHOLD_IS_ZERO);
   assert(opposite > THRESHOLD_IS_ZERO);
   if ((adjacent_left > THRESHOLD_IS_ZERO) && (adjacent_right > THRESHOLD_IS_ZERO) && (opposite > THRESHOLD_IS_ZERO))
   {
      if (((adjacent_left + adjacent_right) > opposite) &&
          ((adjacent_left + opposite) > adjacent_right) &&
          ((opposite + adjacent_right) > adjacent_left))
      {
         /*regular triangle*/
         float cosine_val = (((adjacent_left * adjacent_left) + (adjacent_right * adjacent_right)) - (opposite * opposite)) / (2.0f * adjacent_left * adjacent_right);
         cosine_val = Enforce_Range(cosine_val, -1.0f, 1.0f);
         angle_ret  = Fast_Acos(cosine_val);
      }
      else if (Abs((adjacent_left + adjacent_right) - opposite) < TRIANGLE_THRESHOLD_ROUNDING_ERROR)
      {
         /*degenerated triangle, angle in center*/
         angle_ret = PI;
      }
      else if ((Abs((adjacent_left + opposite) - adjacent_right) < TRIANGLE_THRESHOLD_ROUNDING_ERROR) ||
               (Abs((opposite + adjacent_right) - adjacent_left) < TRIANGLE_THRESHOLD_ROUNDING_ERROR))
      {
         /*degenerated triangle, angle at either end*/
         angle_ret = 0.0f;
      }
      else
      {
         /*not a triangle*/
         assert((adjacent_left + adjacent_right) >= opposite);
         assert((adjacent_left + opposite) >= adjacent_right);
         assert((opposite + adjacent_right) >= adjacent_left);
      }
   }
   else
   {
      /*not a triangle*/
      assert(FALSE);
   }
   return angle_ret;
}


float Triangle_Alpha_From_Abc(
   float a,
   float b,
   float c)
{
   assert(a > THRESHOLD_IS_ZERO);
   assert(b > THRESHOLD_IS_ZERO);
   assert(c > THRESHOLD_IS_ZERO);
   return Triangle_Angle_From_Sides(b, c, a);
}


float Triangle_Beta_From_Abc(
   float a,
   float b,
   float c)
{
   assert(a > THRESHOLD_IS_ZERO);
   assert(b > THRESHOLD_IS_ZERO);
   assert(c > THRESHOLD_IS_ZERO);
   return Triangle_Angle_From_Sides(c, a, b);
}


float Triangle_Gamma_From_Abc(
   float a,
   float b,
   float c)
{
   assert(a > THRESHOLD_IS_ZERO);
   assert(b > THRESHOLD_IS_ZERO);
   assert(c > THRESHOLD_IS_ZERO);
   return Triangle_Angle_From_Sides(a, b, c);
}


/* QAC complains about unused static functions */
#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
#include "ml_serialization.h"
#endif


#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE

static float CS_TABLE[CS_QUARTER_TABLE_SIZE];

static float ATAN_TABLE[ATAN_TABLE_SIZE];

void Compute_Trig_Tables(void)
{
   int   i_table;
   float table_x_value = 0.0f;
   float table_spacing = PI / (2.0f * (float32_T)(CS_QUARTER_TABLE_MAX_INDEX_FLOAT));
   for (i_table = 0; i_table < (CS_QUARTER_TABLE_SIZE-1); i_table++)
   {
      CS_TABLE[i_table] = (float)cosf(table_x_value);
      table_x_value += table_spacing;
   }
   /* On some machines the last value overshoots because the tiny errors sum up.
      Ensure that the last value in the table matches exactly by setting it to zero. */
   CS_TABLE[CS_QUARTER_TABLE_SIZE - 1] = 0.0f;
   table_x_value = 0.0f;
   table_spacing = 1.f / ATAN_INV_PREC;
   for (i_table = 0; i_table < ATAN_TABLE_SIZE; i_table++)
   {
      ATAN_TABLE[i_table] = atanf(table_x_value);
      table_x_value += table_spacing;
   }
}

Shared_Toolbox_Serialization_Error_T Serialize_Trig_Table(Shared_Toolbox_Serial_Buffer_T *p_buffer)
{
   Shared_Toolbox_Serialization_Error_T ret_val;
   size_t cos_table_byte_size = (CS_QUARTER_TABLE_SIZE * sizeof(float32_T));
   size_t atan_table_byte_size = (ATAN_TABLE_SIZE * sizeof(float32_T));
   /* header consists of the magic number and the size of each of the serialized tables */
   size_t header_byte_size = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);
   if (NULL == p_buffer)
   {
      ret_val = SHARED_TOOLBOX_SRL_ERR_PTR;
   }
   /* Size of data to serialize + header */
   else if (p_buffer->length < (cos_table_byte_size + atan_table_byte_size + header_byte_size))
   {
      ret_val = SHARED_TOOLBOX_SRL_ERR_NOMEM;
   }
   else
   {
      size_t current_byte_local = 0;
      uint32_t magic = 0xa0b0c0d0;
      current_byte_local += Serialize_Write_Uint32(&p_buffer->p_data[current_byte_local], magic);
      current_byte_local += Serialize_Write_Uint32(&p_buffer->p_data[current_byte_local], CS_QUARTER_TABLE_SIZE);
      current_byte_local += Serialize_Write_Uint32(&p_buffer->p_data[current_byte_local], ATAN_TABLE_SIZE);
      current_byte_local += Serialize_Write_Float_Array(&p_buffer->p_data[current_byte_local], CS_TABLE, CS_QUARTER_TABLE_SIZE);
      (void)Serialize_Write_Float_Array(&p_buffer->p_data[current_byte_local], ATAN_TABLE, ATAN_TABLE_SIZE);

      ret_val = SHARED_TOOLBOX_SRL_SUCCESS;
   }
   return ret_val;
}

Shared_Toolbox_Serialization_Error_T Serialize_Trig_Table_Checksum(Shared_Toolbox_Serial_Buffer_T *p_buffer)
{
   Shared_Toolbox_Serialization_Error_T ret_val;
   if (NULL == p_buffer)
   {
      ret_val = SHARED_TOOLBOX_SRL_ERR_PTR;
   }
   /* Size of data to serialize + magic bytes + length of tables */
   else if (p_buffer->length < (4 + 4 + 4))
   {
      ret_val = SHARED_TOOLBOX_SRL_ERR_NOMEM;
   }
   else
   {
      size_t current_byte_local = 0;
      uint32_t magic = 0xfafbfcfd;
      uint32_t checksum;
      current_byte_local += Serialize_Write_Uint32(&p_buffer->p_data[current_byte_local], magic);

      checksum = Calc_Checksum_U32(CS_TABLE, CS_QUARTER_TABLE_SIZE * sizeof(float));
      current_byte_local += Serialize_Write_Uint32(&p_buffer->p_data[current_byte_local], checksum);

      checksum = Calc_Checksum_U32(ATAN_TABLE, ATAN_TABLE_SIZE * sizeof(float));
      (void)Serialize_Write_Uint32(&p_buffer->p_data[current_byte_local], checksum);
      ret_val = SHARED_TOOLBOX_SRL_SUCCESS;
   }
   return ret_val;
}

Shared_Toolbox_Serialization_Error_T Deserialize_Trig_Table(Shared_Toolbox_Serial_Buffer_T *p_buffer)
{
   Shared_Toolbox_Serialization_Error_T ret_val;
   if (NULL == p_buffer)
   {
      ret_val = SHARED_TOOLBOX_SRL_ERR_PTR;
   }
   /* Size of data to serialize + magic bytes + length of tables */
   else if (p_buffer->length < ((CS_QUARTER_TABLE_SIZE * sizeof(float32_T)) + (ATAN_TABLE_SIZE * sizeof(float32_T))+ sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t)))
   {
      ret_val = SHARED_TOOLBOX_SRL_ERR_NOMEM;
   }
   else
   {
      size_t current_byte_local = 0;
      uint32_t magic_number = Serialize_Read_LE(&p_buffer->p_data[0]);
      current_byte_local += sizeof(uint32_t);
      if (0xa0b0c0d0 == magic_number)
      {
         size_t table_size_cos = Serialize_Read_LE(&p_buffer->p_data[current_byte_local]);
         current_byte_local += sizeof(uint32_t);
         if (table_size_cos != CS_QUARTER_TABLE_SIZE)
         {
            ret_val = SHARED_TOOLBOX_SRL_ERR_PARSE;
         }
         else
         {
            size_t table_size_atan = Serialize_Read_LE(&p_buffer->p_data[current_byte_local]);
            current_byte_local += sizeof(uint32_t);
            if (table_size_atan != ATAN_TABLE_SIZE)
            {
               ret_val = SHARED_TOOLBOX_SRL_ERR_PARSE;
            }
            else
            {
               current_byte_local += Deserialize_Float_Array_Le(&p_buffer->p_data[current_byte_local], CS_QUARTER_TABLE_SIZE, CS_TABLE);
               (void)Deserialize_Float_Array_Le(&p_buffer->p_data[current_byte_local], ATAN_TABLE_SIZE, ATAN_TABLE);
               ret_val = SHARED_TOOLBOX_SRL_SUCCESS;
            }
         }
      }
      else if (0xd0c0b0a0 == magic_number)
      {
         size_t table_size_cos = Serialize_Read_BE(&p_buffer->p_data[current_byte_local]);
         current_byte_local += sizeof(uint32_t);
         if (table_size_cos != CS_QUARTER_TABLE_SIZE)
         {
            ret_val = SHARED_TOOLBOX_SRL_ERR_PARSE;
         }
         else
         {
            size_t table_size_atan = Serialize_Read_BE(&p_buffer->p_data[current_byte_local]);
            current_byte_local += sizeof(uint32_t);
            if (table_size_atan != ATAN_TABLE_SIZE)
            {
               ret_val = SHARED_TOOLBOX_SRL_ERR_PARSE;
            }
            else
            {
               current_byte_local += Deserialize_Float_Array_Be(&p_buffer->p_data[current_byte_local], CS_QUARTER_TABLE_SIZE, CS_TABLE);
               (void)Deserialize_Float_Array_Be(&p_buffer->p_data[current_byte_local], ATAN_TABLE_SIZE, ATAN_TABLE);
               ret_val = SHARED_TOOLBOX_SRL_SUCCESS;
            }
         }
      }
      else
      {
         ret_val = SHARED_TOOLBOX_SRL_ERR_PARSE;
      }
   }
   return ret_val;
}


#endif

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE != ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION
float32_T Fast_Cos(float32_T x)
{
   uint32_t index;
   float32_T ret_val;
#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_TABLE
 /*PRQA S 0842 ++*/
#include "ml_cos_tbl_predef.h"
 /*PRQA S 0842 --*/
#endif

   /* assert that table is filled */
   assert(CS_TABLE[0] == 1.0f);

   if (Is_Nan(x))   /*PRQA S 3341 */ /* Checking for nan */
   {
      /* x = nan*/
      ret_val = x;
   }
   else
   {

      /* compute index into full CS table */
      /* as if there was a full table containing all values from 0 to 2*PI (full 360 degrees) */
      assert(0 == CS_FULL_TABLE_SIZE % 4);
      if (x < 0.f)
      {
         index = ((uint32_t)((-x * CS_INV_PREC) + .5f)) & CS_FULL_TABLE_MASK;
      }
      else
      {
         index = ((uint32_t)((x * CS_INV_PREC) + .5f)) & CS_FULL_TABLE_MASK;
      }

      /* get value from quarter CS table */
      /* as the cosine is symmetric, we only need one quarter of the full 360 degrees table to save all look up values */
      /* hence we compute the index into the quarter table and read out the corresponding value */
      if (index < CS_QUARTER_TABLE_SIZE)
      {
         ret_val = CS_TABLE[index];
      }
      else if (index <= CS_HALF_TABLE_MAX_INDEX)
      {
         assert((CS_HALF_TABLE_MAX_INDEX - index) < CS_QUARTER_TABLE_SIZE);
         ret_val = -CS_TABLE[CS_HALF_TABLE_MAX_INDEX - index];
      }
      else if (index <= (CS_HALF_TABLE_MAX_INDEX + CS_QUARTER_TABLE_MAX_INDEX))
      {
         assert((index - CS_HALF_TABLE_MAX_INDEX) < CS_QUARTER_TABLE_SIZE);
         ret_val = -CS_TABLE[index - CS_HALF_TABLE_MAX_INDEX];
      }
      else
      {
         assert((CS_FULL_TABLE_SIZE - index) < CS_QUARTER_TABLE_SIZE);
         ret_val = CS_TABLE[CS_FULL_TABLE_SIZE - index];
      }
   }
   return ret_val;
}


float32_T Fast_Sin(float32_T x)
{
   return Fast_Cos(x - HALF_PI);
}


float32_T Fast_Tan(float32_T x)
{
   float32_T cos_nonzero;

   cos_nonzero = Fast_Cos(x);
   cos_nonzero = Enforce_Nonzero(cos_nonzero, THRESHOLD_IS_ZERO);

   return Fast_Sin(x) / cos_nonzero;
}


float32_T Fast_Atan(float32_T x)
{
   float32_T ret_val;
#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_TABLE
 /*PRQA S 0842 ++*/
#include "ml_atan_tbl_predef.h"
 /*PRQA S 0842 --*/
#endif

   /* assert that table is filled */
   assert(ATAN_TABLE[1] > 0.0f);

   if (Is_Nan(x))  /*PRQA S 3341 */ /* Checking for nan */
   {
      /* x = nan*/
      ret_val = x;
   }
   else
   {
      int index;
      /* handle negative condition*/
      if (x < 0.f)
      {
         float32_T x2;
         x2 = -x;
         if (x2 <= 1.f)
         {
            /* magnitude less than 1, use table directly*/
            index = (int)(x2 * ATAN_INV_PREC);
            ret_val = -ATAN_TABLE[index];
         }
         else
         {
            /* magnitude greater than 1, use inverse into table*/
            index = (int)(ATAN_INV_PREC / x2);
            ret_val = ATAN_TABLE[index] - HALF_PI;
         }
      }
      else
      {
         if (x <= 1.f)
         {
            /* magnitude less than 1, use table directly*/
            index = (int)(x * ATAN_INV_PREC);
            ret_val = ATAN_TABLE[index];
         }
         else
         {
            /* magnitude greater than 1, use inverse into table*/
            index = (int)(ATAN_INV_PREC / x);
            ret_val = HALF_PI - ATAN_TABLE[index];
         }
      }
   }
   return ret_val;
}


float32_T Fast_Atan2(
   float32_T y,
   float32_T x)
{
   float32_T ret_val;
   if (Is_Nan(x))  /*PRQA S 3341 */ /* Checking for nan */
   {
      /* x is nan*/
      ret_val = x;
   }
   else if (Is_Nan(y))  /*PRQA S 3341 */ /* Checking for nan */
   {
      /* y is nan*/
      ret_val = y;
   }

   else if (x > 0.f)
   {
      ret_val = Fast_Atan(y / x);
   }
   else if (x < 0.f)
   {
      if (y >= 0.f)
      {
         /* ((x < 0.f) && (y >= 0.f)) */
         ret_val = Fast_Atan(y / x) + ((float32_T)PI);
      }
      else
      {
         /* ((x < 0.f) && (y < 0.f)) */
         ret_val = Fast_Atan(y / x) - ((float32_T)PI);
      }
   }
   else
   {
      /* (x == 0.f) */
      if (y > 0.f)
      {
         ret_val = HALF_PI;
      }
      else if (y < 0.f)
      {
         ret_val = -HALF_PI;
      }
      else
      {
         /* ((x == 0.f) && (y == 0.f)) is undefined; just return x value*/
         ret_val = x;
      }
   }
   return ret_val;
}



float32_T Fast_Asin(float32_T x)
{
   float32_T x_sq;
   float32_T ret_val;

   x_sq = x * x;

   if (x_sq <= 1.f)
   {
      ret_val = 2.f * Fast_Atan(x / (1.f + Fast_Sqrt(1.f - x_sq)));
   }
   else
   {
      /* undefined; just return x value*/
      ret_val = x;
   }
   return ret_val;
}


float32_T Fast_Acos(float32_T x)
{
   float32_T ret_val;
   if ((x >= -1.f) && (x <= 1.f))
   {
      ret_val = HALF_PI - Fast_Asin(x);
   }
   else
   {
      /* undefined; just return x value*/
      ret_val = x;
   }
   return ret_val;
}
#endif



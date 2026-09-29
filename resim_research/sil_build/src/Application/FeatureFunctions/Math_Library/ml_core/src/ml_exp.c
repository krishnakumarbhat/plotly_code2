/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "ml_fast_math_table_macros.h"

#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
#include "ml_serialization.h"
#endif

#include <assert.h>
#include <math.h>
#include "ml_exp.h"
#include "ml_macros.h"

#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
static float EXP_TABLE[EXP_TABLE_SIZE];

void Compute_Exp_Table(void)
{
   int   i_table;
   float table_spacing = 1.0f / EXP_INV_PREC;
   float table_x_value = LOWEST_DOMAIN_FLOAT;

   for (i_table = 0; i_table < EXP_TABLE_SIZE; i_table++)
   {
      EXP_TABLE[i_table] = expf(table_x_value);
      table_x_value += table_spacing;
   }
}


Shared_Toolbox_Serialization_Error_T Serialize_Exp_Table(Shared_Toolbox_Serial_Buffer_T *p_buffer)
{
   Shared_Toolbox_Serialization_Error_T ret_val;

   if (NULL == p_buffer)
   {
      ret_val = SHARED_TOOLBOX_SRL_ERR_PTR;
   }
   /* Size of data to serialize + magic bytes + length of table */
   else if (p_buffer->length < ((EXP_TABLE_SIZE * sizeof(float32_T)) + 4 + 4))
   {
      ret_val = SHARED_TOOLBOX_SRL_ERR_NOMEM;
   }
   else
   {
      size_t current_byte_local = 0;
      uint32_t magic = 0xa0b0c0d0;
      current_byte_local += Serialize_Write_Uint32(&p_buffer->p_data[current_byte_local], magic);
      current_byte_local += Serialize_Write_Uint32(&p_buffer->p_data[current_byte_local], EXP_TABLE_SIZE);
      (void)Serialize_Write_Float_Array(&p_buffer->p_data[current_byte_local], EXP_TABLE, EXP_TABLE_SIZE);
      ret_val = SHARED_TOOLBOX_SRL_SUCCESS;
   }
   return ret_val;
}

Shared_Toolbox_Serialization_Error_T Deserialize_Exp_Table(Shared_Toolbox_Serial_Buffer_T *p_buffer)
{
   Shared_Toolbox_Serialization_Error_T ret_val;
   if (NULL == p_buffer)
   {
      ret_val = SHARED_TOOLBOX_SRL_ERR_PTR;
   }
   /* Size of data to serialize + magic bytes + length of table */
   else if (p_buffer->length < ((EXP_TABLE_SIZE * sizeof(float32_T)) + 4 + 4))
   {
      ret_val = SHARED_TOOLBOX_SRL_ERR_NOMEM;
   }
   else
   {
      size_t current_byte_local = 0;
      uint32_t magic_number = Serialize_Read_LE(&p_buffer->p_data[current_byte_local]);
      current_byte_local += 4;
      if (0xa0b0c0d0 == magic_number)
      {
         size_t table_size = Serialize_Read_LE(&p_buffer->p_data[current_byte_local]);
         current_byte_local += 4;
         if (table_size != EXP_TABLE_SIZE)
         {
            ret_val = SHARED_TOOLBOX_SRL_ERR_PARSE;
         }
         else
         {
            (void)Deserialize_Float_Array_Le(&p_buffer->p_data[current_byte_local], EXP_TABLE_SIZE, EXP_TABLE);
            ret_val = SHARED_TOOLBOX_SRL_SUCCESS;
         }
      }
      else if (0xd0c0b0a0 == magic_number)
      {
         size_t table_size = Serialize_Read_BE(&p_buffer->p_data[current_byte_local]);
         current_byte_local += 4;
         if (table_size != EXP_TABLE_SIZE)
         {
            ret_val = SHARED_TOOLBOX_SRL_ERR_PARSE;
         }
         else
         {
            (void)Deserialize_Float_Array_Be(&p_buffer->p_data[current_byte_local], EXP_TABLE_SIZE, EXP_TABLE);
            ret_val =  SHARED_TOOLBOX_SRL_SUCCESS;
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

#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE != ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION
float32_T Fast_Exp(float32_T x)
{
#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_TABLE
 /*PRQA S 0842 ++*/
#include "ml_exp_tbl_predef.h"
 /*PRQA S 0842 --*/
#endif

   int32_t index;
   int32_t temp_var;

   assert(EXP_TABLE[0] > 0.f);

   if (Is_Nan(x))  /*PRQA S 3341 */ /* Checking for nan */
   {
      /* x = nan*/
      return x;
   }

   if (x <= LOWEST_DOMAIN_FLOAT)
   {
      /* falling back to expf() */
      return expf(x);
   }

   if (x >= HIGHEST_DOMAIN_FLOAT)
   {
      /* falling back to expf() */
      return expf(x);
   }

   /* compute index into EXP table*/
   temp_var = (int32_t)(x * EXP_INV_PREC);

   index = temp_var + MAG_LOWEST_DOMAIN;

   assert((index >= 0) && (index < EXP_TABLE_SIZE));
   return EXP_TABLE[index];
}

#endif

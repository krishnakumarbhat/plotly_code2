/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <math.h>
#include <assert.h>
#include "ml_bool.h"
#include "ml_macros.h"
#include "ml_math.h"
#include "ml_sieve.h"
#include "ml_math_infinity_silent.h"

/**
* Index where the most extreme value can be found in a sieve set.
* \ingroup Sieve
*/
#define SIEVE_SET_EXTREME_VALUE_INDEX (0)


void Init_Max_Sieve(Max_Sieve_T * const p_sieve)
{
   assert(NULL != p_sieve);

   p_sieve->known_max = -AS_TOOLBOX_INFINITY;
}


void Init_Min_Sieve(Min_Sieve_T * const p_sieve)
{
   assert(NULL != p_sieve);

   p_sieve->known_min = AS_TOOLBOX_INFINITY;
}

void Init_Min_Max_Sieve(Min_Max_Sieve_T * const p_sieve)
{
   assert(NULL != p_sieve);

   Init_Max_Sieve(&p_sieve->max_sieve);
   Init_Min_Sieve(&p_sieve->min_sieve);
}

float32_T Max_Sieve(
   Max_Sieve_T * const p_sieve,
   float32_T    value)
{
   float32_T my_value;
   assert(NULL != p_sieve);
   assert(value >= -AS_TOOLBOX_INFINITY);
   assert(value <= AS_TOOLBOX_INFINITY);

   my_value = value;

   if (p_sieve->known_max < my_value)
   {
      Swap(p_sieve->known_max, my_value, float32_T);
   }
   return my_value;
}


float32_T Min_Sieve(
   Min_Sieve_T * const p_sieve,
   float32_T    value)
{
   float32_T my_value;

   assert(NULL != p_sieve);
   assert(value >= -AS_TOOLBOX_INFINITY);
   assert(value <= AS_TOOLBOX_INFINITY);

   my_value = value;

   if (p_sieve->known_min > my_value)
   {
      Swap(p_sieve->known_min, my_value, float32_T);
   }

   return my_value;
}

float32_T Min_Max_Sieve(
   Min_Max_Sieve_T * const p_sieve,
   float32_T        value)
{
   float32_T my_value;

   assert(NULL != p_sieve);
   assert(value >= -AS_TOOLBOX_INFINITY);
   assert(value <= AS_TOOLBOX_INFINITY);

   my_value = value;

   if (my_value < AS_TOOLBOX_INFINITY)
   {
      my_value = Max_Sieve(&p_sieve->max_sieve, my_value);
   }
   if (my_value > -AS_TOOLBOX_INFINITY)
   {
      my_value = Min_Sieve(&p_sieve->min_sieve, my_value);
   }

   return my_value;
}


void Init_Max_Sieve_Set(
   Max_Sieve_T * const p_sieve,
   const uint8_t      sieve_set_size)
{
   uint8_t sieve_i;

   assert(NULL != p_sieve);

   for (sieve_i = 0; sieve_i < sieve_set_size; sieve_i++)
   {
      Init_Max_Sieve(&p_sieve[sieve_i]);
   }
}

void Init_Min_Sieve_Set(
   Min_Sieve_T *p_sieve,
   uint8_t      sieve_set_size)
{
   uint8_t sieve_i;

   assert(NULL != p_sieve);

   for (sieve_i = 0; sieve_i < sieve_set_size; sieve_i++)
   {
      Init_Min_Sieve(&p_sieve[sieve_i]);
   }
}

void Init_Min_Max_Sieve_Set(
   Min_Max_Sieve_T * const p_sieve,
   const uint8_t          sieve_set_size)
{
   uint8_t sieve_i;

   assert(NULL != p_sieve);

   for (sieve_i = 0; sieve_i < sieve_set_size; sieve_i++)
   {
      Init_Max_Sieve(&p_sieve[sieve_i].max_sieve);
      Init_Min_Sieve(&p_sieve[sieve_i].min_sieve);
   }
}


float32_T Get_Max_Sieve_Set_Content_At_Index(
   const Max_Sieve_T *p_sieve,
   uint8_t      index)
{
   float32_T value;

   assert(NULL != p_sieve);

   value = Get_Max_Sieve_Content(&p_sieve[index]);

   return value;
}

float32_T Get_Max_From_Max_Sieve_Set(const Max_Sieve_T *p_sieve)
{
   float32_T value;

   assert(NULL != p_sieve);

   value = Get_Max_Sieve_Set_Content_At_Index(p_sieve, SIEVE_SET_EXTREME_VALUE_INDEX);

   return value;
}

float32_T Get_Min_Sieve_Set_Content_At_Index(
   const Min_Sieve_T *p_sieve,
   uint8_t      index)
{
   float32_T value;

   assert(NULL != p_sieve);

   value = Get_Min_Sieve_Content(&p_sieve[index]);

   return value;
}

float32_T Get_Min_From_Min_Sieve_Set(
   const Min_Sieve_T *p_sieve)
{
   float32_T value;

   assert(NULL != p_sieve);

   value = Get_Min_Sieve_Set_Content_At_Index(p_sieve, SIEVE_SET_EXTREME_VALUE_INDEX);

   return value;
}


float32_T Max_Sieve_Set(
   Max_Sieve_T * const p_sieve,
   const uint8_t      sieve_set_size,
   float32_T    value)
{
   uint8_t sieve_i;
   float32_T my_value;

   assert(NULL != p_sieve);
   assert(value >= -AS_TOOLBOX_INFINITY);
   assert(value <= AS_TOOLBOX_INFINITY);

   my_value = value;

   for (sieve_i = 0; sieve_i < sieve_set_size; sieve_i++)
   {
      my_value = Max_Sieve(&p_sieve[sieve_i], my_value);
   }
   return my_value;
}


float32_T Get_Max_Sieve_Content(const Max_Sieve_T * const p_sieve)
{
   assert(NULL != p_sieve);

   return p_sieve->known_max;
}

float32_T Get_Min_Sieve_Content(const Min_Sieve_T *p_sieve)
{
   assert(NULL != p_sieve);

   return p_sieve->known_min;
}

float32_T Get_Min_Max_Sieve_Min_Content(
   const Min_Max_Sieve_T * const p_sieve,
   const uint8_t          index)
{
   assert(NULL != p_sieve);

   return Get_Min_Sieve_Content(&p_sieve[index].min_sieve);
}

float32_T Get_Min_Max_Sieve_Max_Content(
   const Min_Max_Sieve_T * const p_sieve,
   const uint8_t          index)
{
   assert(NULL != p_sieve);

   return Get_Max_Sieve_Content(&p_sieve[index].max_sieve);
}

float32_T Min_Sieve_Set(
   Min_Sieve_T *p_sieve,
   uint8_t      sieve_set_size,
   float32_T    value)
{
   uint8_t sieve_i;
   float32_T my_value;

   assert(NULL != p_sieve);
   assert(value >= -AS_TOOLBOX_INFINITY);
   assert(value <= AS_TOOLBOX_INFINITY);

   my_value = value;

   for (sieve_i = 0; sieve_i < sieve_set_size; sieve_i++)
   {
      my_value = Min_Sieve(&p_sieve[sieve_i], my_value);
   }
   return my_value;
}

float32_T Min_Max_Sieve_Set(
   Min_Max_Sieve_T * const p_sieve,
   const uint8_t          sieve_set_size,
   float32_T        value)
{
   uint8_t sieve_i;
   float32_T my_value;

   assert(NULL != p_sieve);
   assert(value >= -AS_TOOLBOX_INFINITY);
   assert(value <= AS_TOOLBOX_INFINITY);

   my_value = value;

   for (sieve_i = 0; sieve_i < sieve_set_size; sieve_i++)
   {
      my_value = Min_Max_Sieve(&p_sieve[sieve_i], my_value);
   }
   return my_value;
}

boolean_T Is_Sieved_Value_Valid(const float32_T value)
{
   boolean_T f_not_neg_infinity = FALSE;
   boolean_T f_not_pos_infinity = FALSE;

   if (value < AS_TOOLBOX_INFINITY)
   {
      f_not_pos_infinity = TRUE;
   }
   if (value > -AS_TOOLBOX_INFINITY)
   {
      f_not_neg_infinity = TRUE;
   }
   return Is_True(f_not_neg_infinity) && Is_True(f_not_pos_infinity);
}



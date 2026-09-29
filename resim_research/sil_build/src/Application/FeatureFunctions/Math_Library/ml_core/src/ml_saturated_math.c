/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <assert.h>
#include <limits.h>
#include "ml_macros.h"
#include "ml_saturated_math.h"


void Sat_Inc_Uint8(uint8_t *p_number)
{
   assert(p_number != NULL);

   if ((*p_number) < UINT8_MAX)
   {
      (*p_number)++;
   }
}

void Sat_Inc_Int8(int8_t *p_number)
{
   assert(p_number != NULL);

   if ((*p_number) < INT8_MAX)
   {
      (*p_number)++;
   }
}

void Sat_Inc_Uint16(uint16_t *p_number)
{
   assert(p_number != NULL);

   if ((*p_number) < UINT16_MAX)
   {
      (*p_number)++;
   }
}

void Sat_Inc_Int16(int16_t *p_number)
{
   assert(p_number != NULL);

   if ((*p_number) < INT16_MAX)
   {
      (*p_number)++;
   }
}

void Sat_Inc_Uint32(uint32_t *p_number)
{
   assert(p_number != NULL);

   if ((*p_number) < UINT32_MAX)
   {
      (*p_number)++;
   }
}

void Sat_Inc_Int32(int32_t *p_number)
{
   assert(p_number != NULL);

   if ((*p_number) < INT32_MAX)
   {
      (*p_number)++;
   }
}

void Sat_Dec_Uint8(uint8_t *p_number)
{
   assert(p_number != NULL);

   if ((*p_number) > 0)
   {
      (*p_number)--;
   }
}

void Sat_Dec_Int8(int8_t *p_number)
{
   assert(p_number != NULL);

   if ((*p_number) > INT8_MIN)
   {
      (*p_number)--;
   }
}

void Sat_Dec_Uint16(uint16_t *p_number)
{
   assert(p_number != NULL);

   if ((*p_number) > 0)
   {
      (*p_number)--;
   }
}

void Sat_Dec_Int16(int16_t *p_number)
{
   assert(p_number != NULL);

   if ((*p_number) > INT16_MIN)
   {
      (*p_number)--;
   }
}

void Sat_Dec_Uint32(uint32_t *p_number)
{
   assert(p_number != NULL);

   if ((*p_number) > 0)
   {
      (*p_number)--;
   }
}

void Sat_Dec_Int32(int32_t *p_number)
{
   assert(p_number != NULL);

   if ((*p_number) > INT32_MIN)
   {
      (*p_number)--;
   }
}

uint8_t Sat_Add_Uint8(
   uint8_t summand1,
   uint8_t summand2)
{
   uint16_t sum_u16;

   sum_u16 = (uint16_t)summand1 + (uint16_t)summand2;
   if ((sum_u16) > UINT8_MAX)
   {
      sum_u16 = UINT8_MAX;
   }
   return (uint8_t)sum_u16;
}

int8_t Sat_Add_Int8(
   int8_t summand1,
   int8_t summand2)
{
   int16_t sum16;

   sum16 = (int16_t)summand1 + (int16_t)summand2;
   if ((sum16) > INT8_MAX)
   {
      sum16 = INT8_MAX;
   }
   return (int8_t)sum16;
}

uint16_t Sat_Add_Uint16(
   uint16_t summand1,
   uint16_t summand2)
{
   uint32_t sum_u32;

   sum_u32 = (uint32_t)summand1 + (uint32_t)summand2;
   if ((sum_u32) > UINT16_MAX)
   {
      sum_u32 = UINT16_MAX;
   }
   return (uint16_t)sum_u32;
}

int16_t Sat_Add_Int16(
   int16_t summand1,
   int16_t summand2)
{
   int32_t sum32;

   sum32 = (int32_t)summand1 + (int32_t)summand2;
   if ((sum32) > INT16_MAX)
   {
      sum32 = INT16_MAX;
   }
   return (int16_t)sum32;
}

uint32_t Sat_Add_Uint32(
   uint32_t summand1,
   uint32_t summand2)
{
   uint32_t sum = summand1 + summand2;

   if (sum < summand1)
   {
      sum = UINT32_MAX;
   }
   return sum;
}

int32_t Sat_Add_Int32(
   int32_t summand1,
   int32_t summand2)
{
   int64_t sum64;

   sum64 = (int64_t)summand1 + (int64_t)summand2;
   if ((sum64) > INT32_MAX)
   {
      sum64 = INT32_MAX;
   }
   else if ((sum64) < INT32_MIN)
   {
      sum64 = INT32_MIN;
   }
   else
   {
      /* Do nothing */
   }
   return (int32_t)sum64;
}

uint8_t Sat_Sub_Uint8(
   uint8_t minuend,
   uint8_t subtrahend)
{
   int16_t difference_i16;

   difference_i16 = (int16_t)minuend - (int16_t)subtrahend;
   if ((difference_i16) < 0)
   {
      difference_i16 = 0;
   }
   return (uint8_t)difference_i16;
}

int8_t Sat_Sub_Int8(
   int8_t minuend,
   int8_t subtrahend)
{
   int16_t difference_i16;

   difference_i16 = (int16_t)minuend - (int16_t)subtrahend;
   if ((difference_i16) < INT8_MIN)
   {
      difference_i16 = INT8_MIN;
   }
   return (int8_t)difference_i16;
}

uint16_t Sat_Sub_Uint16(
   uint16_t minuend,
   uint16_t subtrahend)
{
   int32_t difference_i32;

   difference_i32 = (int32_t)minuend - (int32_t)subtrahend;
   if ((difference_i32) < 0)
   {
      difference_i32 = 0;
   }
   return (uint16_t)difference_i32;
}

int16_t Sat_Sub_Int16(
   int16_t minuend,
   int16_t subtrahend)
{
   int32_t difference_i32;

   difference_i32 = (int32_t)minuend - (int32_t)subtrahend;
   if ((difference_i32) < INT16_MIN)
   {
      difference_i32 = INT16_MIN;
   }
   return (int16_t)difference_i32;
}

uint32_t Sat_Sub_Uint32(
   uint32_t minuend,
   uint32_t subtrahend)
{
   int64_t difference_i64;

   difference_i64 = (int64_t)minuend - (int64_t)subtrahend;
   if ((difference_i64) < 0)
   {
      difference_i64 = 0;
   }
   return (uint32_t)difference_i64;
}

int32_t Sat_Sub_Int32(
   int32_t minuend,
   int32_t subtrahend)
{
   int64_t difference_i64;

   difference_i64 = (int64_t)minuend - (int64_t)subtrahend;
   if ((difference_i64) < INT32_MIN)
   {
      difference_i64 = INT32_MIN;
   }
   return (int32_t)difference_i64;
}


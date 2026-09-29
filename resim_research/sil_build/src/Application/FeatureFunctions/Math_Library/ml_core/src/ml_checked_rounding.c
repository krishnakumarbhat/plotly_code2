/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include <math.h>
#include <assert.h>
#include "ml_checked_rounding.h"

uint8_t Roundf_Checked_Uint8(const float32_T value)
{
   float32_T temp = (float32_T)Ml_Roundf(value);
   assert(temp <= ((float32_T)UINT8_MAX));
   assert(temp >= 0.f);
   return (uint8_t)temp;
}


int8_t Roundf_Checked_Int8(const float32_T value)
{
   float32_T temp = (float32_T)Ml_Roundf(value);
   assert(temp <= ((float32_T)INT8_MAX));
   assert(temp >= ((float32_T)INT8_MIN));
   return (int8_t)temp;
}


uint16_t Roundf_Checked_Uint16(const float32_T value)
{
   float32_T temp = (float32_T)Ml_Roundf(value);
   assert(temp <= ((float32_T)UINT16_MAX));
   assert(temp >= 0.f);
   return (uint16_t)temp;
}

int16_t Roundf_Checked_Int16(const float32_T value)
{
   float32_T temp = (float32_T)Ml_Roundf(value);
   assert(temp <= ((float32_T)INT16_MAX));
   assert(temp >= ((float32_T)INT16_MIN));
   return (int16_t)temp;
}


uint32_t Roundf_Checked_Uint32(const float32_T value)
{
   float32_T temp = (float32_T)Ml_Roundf(value);
   assert(temp <= ((float32_T)UINT32_MAX));
   assert(temp >= 0.f);
   return (uint32_t)temp;
}


int32_t Roundf_Checked_Int32(const float32_T value)
{
   float32_T temp = (float32_T)Ml_Roundf(value);
   assert(temp <= ((float32_T)INT32_MAX));
   assert(temp >= ((float32_T)INT32_MIN));
   return (int32_t)temp;
}

/*===========================================================================*\
* Copyright 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include <assert.h>

#include "reuse.h"
#include "ml_checksum.h"
#include "ml_macros.h"



uint8_t Calc_Checksum_U8(
   const void *data,
   size_t size)
{
   uint8_t checksum = 0x00; /* return value */
   size_t i;
   assert(size > 0);
   assert(NULL != data);
   for (i = 0; i < size; i++)
   {
      checksum += ((const char*)data)[i]; /* PRQA S 316 *//* Cast is intended to allow function to be called with any data type */
   }
   return checksum;
}

uint16_t Calc_Checksum_U16(
   const void *data,
   size_t size)
{
   uint16_t checksum = 0x00; /* return value */
   size_t i;
   assert(size > 0);
   assert(NULL != data);
   assert(size % sizeof(uint16_t) == 0);
   for (i = 0; i < (size/ sizeof(uint16_t)); i++)
   {
      checksum += ((const uint16_t*)data)[i]; /* PRQA S 316 *//* Cast is intended to allow function to be called with any data type */
   }
   return checksum;
}

uint32_t Calc_Checksum_U32(
   const void *data,
   size_t size)
{
   uint32_t checksum = 0x00; /* return value */
   size_t i;
   assert(size > 0);
   assert(NULL != data);
   assert(size % sizeof(uint32_t) == 0);
   for (i = 0; i < (size / sizeof(uint32_t)); i++)
   {
      checksum += ((const uint32_t*)data)[i]; /* PRQA S 316 *//* Cast is intended to allow function to be called with any data type */
   }
   return checksum;
}

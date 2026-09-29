/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#include "ml_bit_manipulation.h"

#include <stdio.h>

#include <assert.h>

/* PRQA S 4130 EOF */

/*===========================================================================*\
* Internal macro definitions
\*===========================================================================*/

#ifndef BIT_SIZE_OF_BITFIELD32_T
#define BIT_SIZE_OF_BITFIELD32_T (32)
#endif

/**
 * Returns an integer with only the lowest set bit set. eg: 110 => 010
 *
 * \ingroup ml_bit_manipulation
 *
 */
#define MASK_LOWEST_SET_BIT(n)    ((uint32_t)(n) & - (int32_t)(n)) /* PRQA S 3453 */

/*===========================================================================*\
* Global Function definitions
\*===========================================================================*/

int8_t Bitman_Get_Highest_Set_Bit_Index_BF32(bitfield32_t n)
{
   int32_t b = 0;
   bitfield32_t n_local = n; /* in order to not modify parameters */

   if (0 == n_local)
   {
      return -1;
   }

  /* PRQA S 3412 ++*/
#define step(x)/* PRQA S 842 */  /* Speed is prioritized higher than QAC here */     \
   if (n_local >= (((uint32_t)1) << (x))) \
   {                                      \
      b  += (x);                          \
      n_local >>= (x);                    \
   }/* PRQA S 3412 */

   step(16)
   step(8)
   step(4)
   step(2)
   step(1) // cppcheck-suppress unreadVariable
#undef step /* PRQA S 841 *//* PRQA S 842 */
   return (int8_t)b;
}


int8_t Bitman_Get_Lowest_Set_Bit_Index_BF32(bitfield32_t n)
{
   return Bitman_Get_Highest_Set_Bit_Index_BF32(MASK_LOWEST_SET_BIT(n));/* PRQA S 3760 */ /*implicit conversion is intended and safe*/
}


bitfield32_t Bitman_Unset_Bit_BF32(
   bitfield32_t n,
   uint8_t      index)
{
   bitfield32_t bit = ~(1 << index);/* PRQA S 3760 */ /*implicit conversion is intended and safe*/

   assert(index < BIT_SIZE_OF_BITFIELD32_T);
   return n & bit;
}

boolean_T Bitman_Get_Bit_BF32(
   bitfield32_t n,
   uint8_t      index)
{
   bitfield32_t bit = 1 << index;/* PRQA S 3760 */ /*implicit conversion is intended and safe*/

   assert(index < BIT_SIZE_OF_BITFIELD32_T);

   return (n & bit) > 0;
}


int8_t Bitman_Get_Nth_Lowest_Set_Bit_Index_BF32(
   uint32_t number,
   uint32_t n)
{
   uint32_t i;
   uint32_t number_local = number; /* in order to not modify parameters */
   for(i = 0; i < n; i++)
   {
      uint32_t temp = MASK_LOWEST_SET_BIT(number_local);/* PRQA S 3760 */ /*implicit conversion is intended and safe*/
      number_local ^= temp;
   }
   return Bitman_Get_Lowest_Set_Bit_Index_BF32(number_local);
}


bitfield32_t Bitman_Invert_Bit_BF32(
   bitfield32_t n,
   uint8_t      index)
{
   bitfield32_t bit = 1 << index;/* PRQA S 3760 */ /*implicit conversion is intended and safe*/

   assert(index < BIT_SIZE_OF_BITFIELD32_T);

   return n ^ bit;
}

void Bitman_Unset_Bit_Pointer_BF32(
   uint32_t *p_n,
   uint8_t index)
{
   assert(NULL != p_n);

   *p_n = Bitman_Unset_Bit_BF32(*p_n, index);
}

bitfield32_t Bitman_Set_Bit_BF32(
   bitfield32_t n,
   uint8_t index)
{
   bitfield32_t bit = 1 << index;/* PRQA S 3760 */ /*implicit conversion is intended and safe*/

   assert(index < BIT_SIZE_OF_BITFIELD32_T);

   return n | bit;
}

void Bitman_Set_Bit_Pointer_BF32(
   uint32_t *p_n,
   uint8_t index)
{

   assert(NULL != p_n);

   *p_n = Bitman_Set_Bit_BF32(*p_n, index);
}

uint8_t Bitman_Count_Sparse_Set_Bits_BF32(uint32_t n)
{
   uint8_t count = 0;
   uint32_t n_local = n; /* in order to not modify parameters */
   while (n_local > 0)
   {
      n_local &= (n_local - 1);
      count++;
   }
   return count;
}

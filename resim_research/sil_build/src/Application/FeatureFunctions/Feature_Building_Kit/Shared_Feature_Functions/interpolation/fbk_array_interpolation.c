/**
 * @file fbk_array_interpolation.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Source file with functions for array interpolation.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_array_interpolation.h"
#include "pa_reuse.h"
#include <assert.h>

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
uint8_t Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr(const uint8_t *p_array_in, const uint8_t length_of_array, const uint8_t val_to_check)
{
   uint8_t index_x;

   /* Asserts */
   assert(NULL != p_array_in);
   assert(p_array_in[0] <= val_to_check);
   assert(val_to_check <= p_array_in[length_of_array - 1]);
   assert(p_array_in[0] <= p_array_in[length_of_array - 1]);

   index_x = 1u;
   /**< Offset in condition needs to be provided or else an array overflow occures*/
   while ((index_x < length_of_array) && (p_array_in[index_x] < val_to_check))
   {
      index_x++;
   }
   return index_x;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
uint8_t Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr(const float32_T *p_array_in, const uint8_t length_of_array, const float32_T val_to_check)
{
   uint8_t index_x;

   /* Asserts */
   assert(NULL != p_array_in);
   assert(p_array_in[0] <= val_to_check);
   assert(val_to_check <= p_array_in[length_of_array - 1]);
   assert(p_array_in[0] <= p_array_in[length_of_array - 1]);

   index_x = 1u;
   /**< Offset in condition needs to be provided or else an array overflow occures*/
   while ((index_x < length_of_array) && (p_array_in[index_x] < val_to_check))
   {
      index_x++;
   }
   return index_x;
}

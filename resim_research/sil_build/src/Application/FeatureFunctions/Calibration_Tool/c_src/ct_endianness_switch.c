/**
 * @file ct_endianness_switch.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implementation of array reversing of SFL calibration tool.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "ct_endianness_switch.h"
#include <string.h>

/*===========================================================================*\
 * Function definitions
\*===========================================================================*/

/**
 * @brief Replaces the values in the provided two memory locations with the specified number of bytes.
 *
 * @return void
 */

static void Ct_Swap(uint8_t *p_array_addr, const size_t iter_start, const size_t iter_end, const size_t number_of_bytes)
{
   uint32_t temp_full = 0u;
   uint8_t *p_temp_full      = (uint8_t *) (&temp_full);
   /* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
   memcpy(p_temp_full, &p_array_addr[iter_start], number_of_bytes);
   /* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
   memcpy(&p_array_addr[iter_start], &p_array_addr[iter_end], number_of_bytes);
   /* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
   memcpy(&p_array_addr[iter_end], p_temp_full, number_of_bytes);
}

/**
 * @brief Reverse the array in the structure.
 *
 * @return void
 */

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Ct_Reverse_Array(uint8_t *p_array_addr, size_t array_size_in_bytes, const CT_DATATYPE_NUM_BYTE_T number_of_bytes)
{
   const uint8_t size_type    = (uint8_t) number_of_bytes;
   uint32_t forward_array_pos = 0u;
   uint32_t iter              = 0u;
   size_t backward_array_pos  = array_size_in_bytes - (size_t) number_of_bytes;
   /* When the variable size is odd, below code also work because we want the iteration to end before the array's middle is reached
    * or crossed. As a result, we halt one iteration before reaching the array's middle. */
   float32_T half_size = (float32_T) array_size_in_bytes * 0.5f;
   uint32_t iter_break = (uint32_t) half_size;

   for (iter = 0u; iter < iter_break; iter += size_type)
   {
      Ct_Swap(p_array_addr, forward_array_pos, backward_array_pos, (size_t) number_of_bytes);
      forward_array_pos += size_type;
      backward_array_pos -= size_type;
   }
}
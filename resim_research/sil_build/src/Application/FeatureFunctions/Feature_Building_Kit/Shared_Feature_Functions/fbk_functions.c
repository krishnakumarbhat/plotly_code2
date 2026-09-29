/**
 * @file fbk_functions.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Define FBK functions.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

#include "fbk_functions.h"
#include "fbk_macros.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Function-like Macros
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Swap_Float(float32_T *const x, float32_T *const y)
{
   float32_T temp = *x;
   *x             = *y;
   *y             = temp;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Swap_Uint8(uint8_t *const x, uint8_t *const y)
{
   uint8_t temp = *x;
   *x           = *y;
   *y           = temp;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
boolean_T Fbk_Is_Float_In_Given_Range(const float32_T value,
                                      const float32_T lower_threshold,
                                      const float32_T upper_threshold,
                                      const boolean_T f_check_extended_range,
                                      const float32_T lower_threshold_extension,
                                      const float32_T upper_threshold_extension)
{
   /* Return flag */
   boolean_T result = FBK_FALSE;

   if (Fbk_Is_True(f_check_extended_range))
   {
      /* Check the extended range for the given value */
      if ((value >= (lower_threshold + lower_threshold_extension)) && (value <= (upper_threshold + upper_threshold_extension)))
      {
         result = FBK_TRUE;
      }
   }
   else
   {
      /* Check the normal range for the given value */
      if ((value >= lower_threshold) && (value <= upper_threshold))
      {
         result = FBK_TRUE;
      }
   }

   return result;
}

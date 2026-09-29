/*===================================================================================*\
* FILE: f360_find_interval_in_array.h
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of template function Find_Interval_In_Array
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef F360_FIND_INTERVAL_IN_ARRAY_H
#define F360_FIND_INTERVAL_IN_ARRAY_H

#include "f360_reuse.h"
#include <cstddef>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Find_Interval_In_Array()
   *===========================================================================
   * RETURN VALUE:
   * uint32_t i - interval index
   *
   * PARAMETERS:
   * const float32_t const value - value to be searched for in the array
   * const float32_t (&array)[N] - array of sorted values
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function finds interval containing the value in a float array sorted in ascending order.
   * 0 is returned if the value is below the lowest value in array.
   * array_size is returned if the value is above the highest value in array.
   * result n means that the value is between array[n-1] and array[n].
   *
   * PRECONDITIONS:
   * Array values have to be sorted in ascending order.
   *
   \*===========================================================================*/
   template <std::size_t N>
   uint32_t Find_Interval_In_Array(
      const float32_t value,
      const float32_t (&array)[N])
   {
      uint32_t i;
      for (i = 0U; i < N; i++)
      {
         if (value < array[i])
         {
            break;
         }
      }
      return i;
   }
}
#endif

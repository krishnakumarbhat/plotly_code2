/*===================================================================================*\
* FILE: cmn_math_func.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function signature Matlab related build in functions
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*    ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef OCG_CMN_MATH_FUNC_H
#define OCG_CMN_MATH_FUNC_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 5045)
#endif

#include "ocg_reuse.h"


#include <numeric>
#include <algorithm>
#include <cassert>


namespace ocg
{
   namespace cmn
   {

      /*===========================================================================*\
      * FUNCTION: Mean()
      *===========================================================================
      * RETURN VALUE:
      * float : Mean of array 
      *
      * PARAMETERS:
      * const float (&input_arr)[N] - Input array
      * const uint32_t num_ele - Number of elements in the array
      *
      * --------------------------------------------------------------------------
      * ABSTRACT:
      * --------------------------------------------------------------------------
      * This function finds the mean of the passed array
      * If the number of elements are given to be 0, then the function returns a zero mean.
      *
      * PRECONDITIONS:
      * The number of elements to check must be less than or equal to the length of the input array.
      *
      \*===========================================================================*/
      template<std::size_t N>
      float Mean(
         const float (&input_arr)[N], 
         const uint32_t num_ele)
      {
         assert(num_ele <= N);

         float mean;
         if (num_ele > 0U)
         {
            const float sum = std::accumulate(&input_arr[0], &input_arr[num_ele], 0.0F);
            mean = (sum / static_cast<float>(num_ele));
         }
         else
         {
            // Safe state to avoid division with zero is to return 0 mean
            mean = 0.0F;
         }

         return mean;
      }

      float Saturate(
         const float input,
         const float min_value,
         const float max_value);

      float Linear_Equation(
         const float x,
         const float x1,
         const float x2,
         const float y1,
         const float y2);

      float Linear_Equation_With_Saturation(
         const float x,
         const float x1,
         const float x2,
         const float y1,
         const float y2);
   } // cmn
} // ocg

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // OCG_CMN_MATH_FUNC_H

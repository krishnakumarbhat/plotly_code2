/*===================================================================================*\
* FILE: cmn_math_func.cpp
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*------------------------------------------------------------------------------------
* %full_filespec: AIT-69%
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains  c version of matlab built-in functions.
*
* ABBREVIATIONS:
*   NONE
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/

/******************************
 * Includes
 *******************************/
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include "cmn_math_func.h"
#include "cmn_math_constants.h"

namespace ocg
{
   namespace cmn
   {
      /*===========================================================================*\
   * FUNCTION: Saturate()
   *===========================================================================
   * RETURN VALUE:
   * float max
   *
   * PARAMETERS:
   * const float input
   * const float min_value
   * const float max_value
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * OBSOLETE - to be replaced in DFU-898 by Clamp
   * Saturate value between Min and Max
   *
   \*===========================================================================*/
      float Saturate(
         const float input,
         const float min_value,
         const float max_value)
      {
         float output = std::max(input, min_value);
         output = std::min(output, max_value);
         return output;
      }

      /*===========================================================================*\
      * FUNCTION: Linear_Equation
      *===========================================================================
      * RETURN VALUE:
      * float
      *
      * PARAMETERS:
      *  const float x,
      *  const float x1,
      *  const float x2,
      *  const float y1,
      *  const float y2,
      *
      * --------------------------------------------------------------------------
      * ABSTRACT:
      * --------------------------------------------------------------------------
      * Linear function on two points
      *
      \*===========================================================================*/
      float Linear_Equation(
         const float x,
         const float x1,
         const float x2,
         const float y1,
         const float y2)
      {
         float y;

         const float denominator = x2 - x1;
         if (std::fabs(denominator) > ocg::cmn::OCG_MIN_DENOMINATOR)
         {
            // Linear equation for 2 points.
            y = y1 + (((y2 - y1) / denominator) * (x - x1));
         }
         else
         {
            // Signum function.
            if (((x < x1) && (x1 <= x2)) || ((x > x1) && (x1 > x2)))
            {
               y = y1;
            }
            else if (((x < x2) && (x2 < x1)) || ((x > x2) && (x2 >= x1)))
            {
               y = y2;
            }
            else
            {
               y = (y1 + y2) * 0.5F;
            }
         }

         return y;
      }

      /*===========================================================================*\
      * FUNCTION: Linear_Equation_With_Saturation()
      *===========================================================================
      * RETURN VALUE:
      * bool
      *
      * PARAMETERS:
      *  const float x,
      *  const float x1,
      *  const float x2,
      *  const float y1,
      *  const float y2,
      *
      * --------------------------------------------------------------------------
      * ABSTRACT:
      * --------------------------------------------------------------------------
      * Linear function on two points with saturation
      *
      \*===========================================================================*/
      float Linear_Equation_With_Saturation(
         const float x,
         const float x1,
         const float x2,
         const float y1,
         const float y2)
      {
         float y = Linear_Equation(x, x1, x2, y1, y2);

         const float y_min = std::min(y1, y2);
         const float y_max = std::max(y1, y2);

         y = Saturate(y, y_min, y_max);

         return y;
      }
   } // cmn
} // ocg

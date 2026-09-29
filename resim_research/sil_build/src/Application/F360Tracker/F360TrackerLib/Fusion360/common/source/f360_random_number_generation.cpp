/*===================================================================================*\
* FILE: f360_random_number_generation.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   Definitions of functions used for random number generation
*
* Applicable Standards (in order of precedence: highest first):
*    ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*    ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_random_number_generation.h"
namespace f360_variant_A
{
   static uint32_t xn = 1U;

   /*===========================================================================*\
   * FUNCTION: Init_Random_Number_Generator
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * uint32_t seed
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Set seed for random number generator
   *
   \*===========================================================================*/
   void Init_Random_Number_Generator(const uint32_t seed)
   {
      xn = seed;
   }

   /*===========================================================================*\
   * FUNCTION: Generate_Random_Number
   *===========================================================================
   * RETURN VALUE:
   * uint32_t Random number
   *
   * PARAMETERS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Generate random number in range of 0 to (2^31 - 1)
   *
   \*===========================================================================*/
   uint32_t Generate_Random_Number(void)
   {
      // a, b, c - parameters of random number generator
      // Selected values gives maximum possible period (2^31 - 1)
      const uint32_t a = 16807U;
      const uint32_t b = 0U;
      const uint32_t c = 2147483647U;

      xn = ((a * xn) + b) % c;
      return xn;
   }

   /*===========================================================================*\
   * FUNCTION: Generate_Random_Index
   *===========================================================================
   * RETURN VALUE:
   * uint32_t Random index
   *
   * PARAMETERS:
   * uint32_t max_index
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Generate random number in range of 0 to max_index
   *
   \*===========================================================================*/
   uint32_t Generate_Random_Index(const uint32_t max_index)
   {
      xn = Generate_Random_Number();
      const uint32_t index = xn % (max_index + 1U);
      return index;
   }
}

/*===================================================================================*\
* FILE: f360_random_number_generation.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   Declaration of functions used for random number generation
*
* Applicable Standards (in order of precedence: highest first):
*   ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*   ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_RANDOM_NUMBER_GENERATION_H
#define F360_RANDOM_NUMBER_GENERATION_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   void Init_Random_Number_Generator(const uint32_t seed);

   uint32_t Generate_Random_Number(void);

   uint32_t Generate_Random_Index(const uint32_t max_index);
}
#endif

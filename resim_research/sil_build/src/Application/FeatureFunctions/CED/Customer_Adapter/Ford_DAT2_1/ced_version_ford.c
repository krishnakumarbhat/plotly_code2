/**
 * @file ced_version_ford.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for the Ford_DAT2_1 versioning of CED.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_version_ford.h"
#include "pa_reuse.h"

/*===========================================================================*\
 * Global function definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Function is intended for external usage] */
uint16_t Ced_Get_Ford_Sw_Major_Version(void)
{
   const uint16_t ford_sw_major_version = 10u;

   return ford_sw_major_version;
}

/* coverity[misra_c_2012_rule_8_7_violation][Function is intended for external usage] */
uint16_t Ced_Get_Ford_Sw_Minor_Version(void)
{
   const uint16_t ford_sw_minor_version = 0u;

   return ford_sw_minor_version;
}

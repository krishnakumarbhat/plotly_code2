
/**
 * @file ta_output_boundary_check.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements the output range checks for TA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/**************************************************
 * Includes
 ***************************************************/

#include "ta_output_boundary_check.h"
#include "fbk_macros.h"
#include <assert.h>


/**************************************************
 * Global function definition
 ***************************************************/

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
boolean_T Ta_Are_Outputs_In_Boundary(const Ta_Output_T *p_output)
{
   assert(p_output != NULL);

   /* Not implemented yet, can be auto-generated if necessary */
   return FBK_TRUE;
}

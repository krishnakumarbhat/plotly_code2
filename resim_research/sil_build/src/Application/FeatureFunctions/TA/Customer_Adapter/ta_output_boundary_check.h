#ifndef TA_OUTPUT_BOUNDARY_CHECK_H
#define TA_OUTPUT_BOUNDARY_CHECK_H

/**
 * @file ta_output_boundary_check.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Declares the interface function for output range checks of TA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/**************************************************
 * Includes
 ***************************************************/

#include "pa_reuse.h"
#include "ta_output_t.h"

/**************************************************
 * Global function definition
 ***************************************************/

/**
 * @brief Checks whether all outputs are in given boundaries
 *
 * @return True when containing outputs are within their boundaries
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-8622}
 * @verification{Create a superordinate test to check whether all outputs are in given boundaries}
 **/
boolean_T Ta_Are_Outputs_In_Boundary(const Ta_Output_T *p_output);

#endif /* TA_OUTPUT_BOUNDARY_CHECK_H */

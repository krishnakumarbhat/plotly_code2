#ifndef TA_INPUT_BOUNDARY_CHECK_H
#define TA_INPUT_BOUNDARY_CHECK_H

/**
 * @file ta_input_boundary_check.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Declares the interface function for input range checks of TA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/**************************************************
 * Includes
 ***************************************************/

#include "pa_reuse.h"
#include "ta_input_t.h"

/**************************************************
 * Global function definition
 ***************************************************/

/**
 * @brief Checks whether all inputs are in given boundaries
 *
 * @return True when containing inputs are within their boundaries
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-8623}
 * @verification{Create a superordinate test to check whether all inputs are in given boundaries}
 **/
boolean_T Ta_Are_Inputs_In_Boundary(const Ta_Input_T *p_input);

#endif /* TA_INPUT_BOUNDARY_CHECK_H */

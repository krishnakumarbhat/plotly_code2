#ifndef LCDA_TEST_HELPERS_H
#define LCDA_TEST_HELPERS_H
/**
 * @file c_testing_main.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for LCDA test helper functions.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "lcda_core_output_t.h"
#include "pa_reuse.h"

boolean_T Lcda_Is_Core_Output_Default(Lcda_Core_Output_T *p_core_output);
boolean_T Lcda_Is_Bsw_Output_Default(Lcda_Bsw_Core_Output_T *p_bsw_core_output);
boolean_T Lcda_Is_Cvw_Output_Default(Lcda_Cvw_Core_Output_T *p_cvw_core_output);
boolean_T Lcda_Is_Slc_Output_Default(Lcda_Slc_Core_Output_T *p_slc_core_output);
boolean_T Lcda_Is_Elc_Output_Default(Lcda_Elc_Core_Output_T *p_elc_core_output);

#endif /* LCDA_TEST_HELPERS_H */

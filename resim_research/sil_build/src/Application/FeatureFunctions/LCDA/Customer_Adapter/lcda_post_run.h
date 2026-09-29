#ifndef LCDA_POST_RUN_H
#define LCDA_POST_RUN_H

/**
 * @file lcda_post_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for all customer specific LCDA post run files.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */
#include "fbk_output.h"
#include "lcda_input_t.h"
#include "lcda_instance.h"
#include "lcda_output_t.h"

/**
 * @brief Customer specific LCDA for reset of customer specific output.
 *
 * @return void
 *
 * @SRS{SF-1071}
 * @SAE{SF-2781}
 * @SDD{CSCSA-196458}
 * @verification{Create a test to check that the data is initalized properly}
 */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Lcda_Init_Output(Lcda_Output_T *p_lcda_output);
/**
 * @brief Customer specific LCDA for reset of customer specific output.
 *
 * @return void
 *
 * @SRS{SF-1071}
 * @SAE{SF-2781}
 * @SDD{SF-6987}
 * @verification{Create a test to check that the data is initalized properly}
 */
void Lcda_Post_Run_Init(void);

/**
 * @brief Customer specific LCDA post-run function.
 *
 * @return void
 *
 * @SRS{SF-1071}
 * @SAE{SF-2781}
 * @SDD{SF-6838}
 * @verification{Create a test to check that the data is mapped properly}
 */
/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_lcda_instance" points to a non-constant type.] */
void Lcda_Post_Run(Lcda_Instance_T *p_lcda_instance, const Lcda_Input_T *p_lcda_input, Lcda_Output_T *p_lcda_output, const Fbk_Output_T *p_fbk_output);
/* clang-format on */

#endif /* LCDA_POST_RUN_H */

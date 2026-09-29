/**
 * @file tracker_post_run_rna.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Mock file for RNA_SWEET400 specific tracker postrun
 *
 * @copyright Copyright (C) 2018 Aptiv. All rights reserved.
 */

#include "tracker_post_run_rna.h"

/* coverity[misra_c_2012_rule_10_3_violation][Struct initialization with all zeros]  */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static Tracker_Output_Rna_T rna_tracker_output_mock = {0};
/* coverity[misra_c_2012_rule_10_3_violation][Struct initialization with all zeros]  */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static Tracker_Output_Rna_Buffer_T rna_tracker_output_buffer_mock = {0};

Tracker_Output_Rna_T *get_tracker_output_rna_pointer(void)
{
   return &rna_tracker_output_mock;
}

/* coverity[misra_c_2012_rule_8_7_violation] */
Tracker_Output_Rna_Buffer_T *get_tracker_output_rna_buffer_pointer(void)
{
   return &rna_tracker_output_buffer_mock;
}

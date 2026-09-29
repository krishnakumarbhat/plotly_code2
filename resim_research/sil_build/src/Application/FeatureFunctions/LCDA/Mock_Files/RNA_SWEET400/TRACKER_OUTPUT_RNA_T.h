#ifndef TRACKER_OUTPUT_RNA_T_H
#define TRACKER_OUTPUT_RNA_T_H

/**
 * @file TRACKER_OUTPUT_RNA_T.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Mock file for RNA specific tracker output
 *
 * @copyright Copyright (C) 2018 Aptiv. All rights reserved.
 */

#include "ml_vector_2d_t.h"
#include "pa_reuse.h"

/**
 * This structure holds data overwritten by the post run process (only reason not to store
 * the full content of the original tracker_output is that  tracker output is huge and
 * having it twice does consume too much RAM).
 */
/* coverity[misra_c_2012_rule_2_4_violation] */
typedef struct Tracker_Output_Rna_Tag
{
   Vector_2d_T snail_trail_point_vcs[20];
   boolean_T f_US_truck_exist[64]; /**< Flag indicating if detection coming from trailer truck is found in BSW zone.
                                    */
} Tracker_Output_Rna_T;

/**
 * This structure holds data overwritten by the post run process (only reason not to store
 * the full content of the original tracker_output is that  tracker output is huge and
 * having it twice does consume too much RAM). In order to ensure that the array size is
 * available for the caller of get_tracker_output_rna_buffer_pointer a sructure is holding
 * the array.
 */
/* coverity[misra_c_2012_rule_2_4_violation] */
typedef struct Tracker_Output_Rna_Buffer_Tag
{
   Tracker_Output_Rna_T data[64];
} Tracker_Output_Rna_Buffer_T;

#endif /* TRACKER_OUTPUT_RNA_T_H */

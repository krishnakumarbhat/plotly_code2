#ifndef TRACKER_POST_RUN_RNA_H
#define TRACKER_POST_RUN_RNA_H

/**
 * @file tracker_post_run_rna.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Mock file for RNA_SWEET400 specific tracker postrun
 *
 * @copyright Copyright (C) 2018 Aptiv. All rights reserved.
 */

#include "TRACKER_OUTPUT_RNA_T.h"

/**
 * This function only exists for backwards compatibility. Please use get_tracker_output_rna_buffer_pointer instead.
 * \return         Pointer to Tracker_Output_Rna_T array
 */
Tracker_Output_Rna_T *get_tracker_output_rna_pointer(void);

/**
 * Returns a pointer to a Tracker_Output_Rna_Buffer_T used to store RNA_SWEET400 specific tracked object data.
 * \return         Pointer to Tracker_Output_Rna_Buffer_T
 */
Tracker_Output_Rna_Buffer_T *get_tracker_output_rna_buffer_pointer(void);

#endif

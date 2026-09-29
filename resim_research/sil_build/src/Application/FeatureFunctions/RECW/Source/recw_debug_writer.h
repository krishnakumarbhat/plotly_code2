#ifndef RECW_DEBUG_WRITER_H
#define RECW_DEBUG_WRITER_H

/**
 * @file recw_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for RECW bin writer functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#ifdef BINARY_DEBUG

#include "AS_bin_writer_wrapper.h"

#ifdef __GNUC__
static const char *Recw_Bin_Writer_Suffix __attribute__((unused)) = "RecwOutput";
#else
static const char *Recw_Bin_Writer_Suffix = "RecwOutput";
#endif

/* Declare function used to write bin files. */
void Recw_Write_Bin_File(void);

/* clang-format off */
#define RECW_STORE_VAL_MGR_WPR(var_name, value)                                  STORE_VAL_MGR_WPR(Recw_Bin_Writer_Suffix, var_name, value)
#define RECW_STORE_VAL_MGR_WPR_INDEXED(var_name, value, index)                   STORE_VAL_MGR_WPR_INDEXED(Recw_Bin_Writer_Suffix, var_name, value, index)
#define RECW_STORE_ARRAY_ELEM_MGR_WPR(var_name, value, arr_index)                STORE_ARRAY_ELEM_MGR_WPR(Recw_Bin_Writer_Suffix, var_name, value, arr_index)
#define RECW_STORE_ARRAY_ELEM_MGR_WPR_INDEXED(var_name, value, arr_index, index) STORE_ARRAY_ELEM_MGR_WPR_INDEXED(Recw_Bin_Writer_Suffix, var_name, value, arr_index, index)
/* clang-format on */

#define Binary_Recw_Write_Bin_File() Recw_Write_Bin_File()

#else

#define Binary_Recw_Write_Bin_File()

#endif /* BINARY_DEBUG */

#endif /* RECW_DEBUG_WRITER_H */

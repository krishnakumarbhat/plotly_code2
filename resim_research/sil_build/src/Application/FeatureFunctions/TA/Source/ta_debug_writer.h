#ifndef TA_DEBUG_WRITER_H
#define TA_DEBUG_WRITER_H

/**
 * @file ta_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for TA bin writer functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#ifdef BINARY_DEBUG

#include "AS_bin_writer_wrapper.h"

#ifdef __GNUC__
static const char *Ta_Bin_Writer_Suffix __attribute__((unused)) = "TaOutput";
#else
static const char *Ta_Bin_Writer_Suffix = "TaOutput";
#endif

/* Declare function used to write bin files. */
void Ta_Write_Bin_File(void);

/* clang-format off */
#define TA_STORE_VAL_MGR_WPR(var_name, value)                                  STORE_VAL_MGR_WPR(Ta_Bin_Writer_Suffix, var_name, value)
#define TA_STORE_VAL_MGR_WPR_INDEXED(var_name, value, index)                   STORE_VAL_MGR_WPR_INDEXED(Ta_Bin_Writer_Suffix, var_name, value, index)
#define TA_STORE_ARRAY_ELEM_MGR_WPR(var_name, value, arr_index)                STORE_ARRAY_ELEM_MGR_WPR(Ta_Bin_Writer_Suffix, var_name, value, arr_index)
#define TA_STORE_ARRAY_ELEM_MGR_WPR_INDEXED(var_name, value, arr_index, index) STORE_ARRAY_ELEM_MGR_WPR_INDEXED(Ta_Bin_Writer_Suffix, var_name, value, arr_index, index)
/* clang-format on */

#define Binary_Ta_Write_Bin_File() Ta_Write_Bin_File()

#else

#define Binary_Ta_Write_Bin_File()

#endif /* BINARY_DEBUG */

#endif /* TA_DEBUG_WRITER_H */

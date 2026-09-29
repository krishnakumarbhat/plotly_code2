#ifndef SCW_DEBUG_WRITER_H
#define SCW_DEBUG_WRITER_H

/**
 * @file scw_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for SCW bin writer functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#ifdef BINARY_DEBUG

#include "AS_bin_writer_wrapper.h"

#ifdef __GNUC__
static const char *Scw_Bin_Writer_Suffix __attribute__((unused)) = "ScwOutput";
#else
static const char *Scw_Bin_Writer_Suffix = "ScwOutput";
#endif

/* Declare function used to write bin files. */
void Scw_Write_Bin_File(void);

/* clang-format off */
#define SCW_STORE_VAL_MGR_WPR(var_name, value)                                  STORE_VAL_MGR_WPR(Scw_Bin_Writer_Suffix, var_name, value)
#define SCW_STORE_VAL_MGR_WPR_INDEXED(var_name, value, index)                   STORE_VAL_MGR_WPR_INDEXED(Scw_Bin_Writer_Suffix, var_name, value, index)
#define SCW_STORE_ARRAY_ELEM_MGR_WPR(var_name, value, arr_index)                STORE_ARRAY_ELEM_MGR_WPR(Scw_Bin_Writer_Suffix, var_name, value, arr_index)
#define SCW_STORE_ARRAY_ELEM_MGR_WPR_INDEXED(var_name, value, arr_index, index) STORE_ARRAY_ELEM_MGR_WPR_INDEXED(Scw_Bin_Writer_Suffix, var_name, value, arr_index, index)
/* clang-format on */

#define Binary_Scw_Write_Bin_File() Scw_Write_Bin_File()

#else

#define Binary_Scw_Write_Bin_File()

#endif /* BINARY_DEBUG */

#endif /* SCW_DEBUG_WRITER_H */

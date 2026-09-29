#ifndef CED_DEBUG_WRITER_H
#define CED_DEBUG_WRITER_H

/**
 * @file ced_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for CED bin writer functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#ifdef BINARY_DEBUG

#include "AS_bin_writer_wrapper.h"

#ifdef __GNUC__
static const char *Ced_Bin_Writer_Suffix __attribute__((unused)) = "CedOutput";
#else
static const char *Ced_Bin_Writer_Suffix = "CedOutput";
#endif

/* Declare function used to write bin files. */
void Ced_Write_Bin_File(void);

/* clang-format off */
#define CED_STORE_VAL_MGR_WPR(var_name, value)                                  STORE_VAL_MGR_WPR(Ced_Bin_Writer_Suffix, var_name, value)
#define CED_STORE_VAL_MGR_WPR_INDEXED(var_name, value, index)                   STORE_VAL_MGR_WPR_INDEXED(Ced_Bin_Writer_Suffix, var_name, value, index)
#define CED_STORE_ARRAY_ELEM_MGR_WPR(var_name, value, arr_index)                STORE_ARRAY_ELEM_MGR_WPR(Ced_Bin_Writer_Suffix, var_name, value, arr_index)
#define CED_STORE_ARRAY_ELEM_MGR_WPR_INDEXED(var_name, value, arr_index, index) STORE_ARRAY_ELEM_MGR_WPR_INDEXED(Ced_Bin_Writer_Suffix, var_name, value, arr_index, index)
/* clang-format on */

#define Binary_Ced_Write_Bin_File() Ced_Write_Bin_File()

#else

#define Binary_Ced_Write_Bin_File()

#endif /* BINARY_DEBUG */

#endif /* CED_DEBUG_WRITER_H */

#ifndef LTB_DEBUG_WRITER_H
#define LTB_DEBUG_WRITER_H

/**
 * @file ltb_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for LTB bin writer functions.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#ifdef BINARY_DEBUG

#include "AS_bin_writer_wrapper.h"

#ifdef __GNUC__
static const char *Ltb_Bin_Writer_Suffix __attribute__((unused)) = "LtbOutput";
#else
static const char *Ltb_Bin_Writer_Suffix = "LtbOutput";
#endif

/* Declare function used to write bin files. */
void Ltb_Write_Bin_File(void);

/* clang-format off */
#define LTB_STORE_VAL_MGR_WPR(var_name, value)                                  STORE_VAL_MGR_WPR(Ltb_Bin_Writer_Suffix, var_name, value)
#define LTB_STORE_VAL_MGR_WPR_INDEXED(var_name, value, index)                   STORE_VAL_MGR_WPR_INDEXED(Ltb_Bin_Writer_Suffix, var_name, value, index)
#define LTB_STORE_ARRAY_ELEM_MGR_WPR(var_name, value, arr_index)                STORE_ARRAY_ELEM_MGR_WPR(Ltb_Bin_Writer_Suffix, var_name, value, arr_index)
#define LTB_STORE_ARRAY_ELEM_MGR_WPR_INDEXED(var_name, value, arr_index, index) STORE_ARRAY_ELEM_MGR_WPR_INDEXED(Ltb_Bin_Writer_Suffix, var_name, value, arr_index, index)
/* clang-format on */

#define Binary_Ltb_Write_Bin_File() Ltb_Write_Bin_File()

#else

#define Binary_Ltb_Write_Bin_File()

#endif /* BINARY_DEBUG */

#endif /* LTB_DEBUG_WRITER_H */

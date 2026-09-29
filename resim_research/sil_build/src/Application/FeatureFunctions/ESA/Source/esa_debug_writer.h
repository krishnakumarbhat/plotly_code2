#ifndef ESA_DEBUG_WRITER_H
#define ESA_DEBUG_WRITER_H

/**
 * @file esa_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for ESA bin writer functions.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#ifdef BINARY_DEBUG

#include "AS_bin_writer_wrapper.h"

#ifdef __GNUC__
static const char *Esa_Bin_Writer_Suffix __attribute__((unused)) = "EsaOutput";
#else
static const char *Esa_Bin_Writer_Suffix = "EsaOutput";
#endif

/* Declare function used to write bin files. */
void Esa_Write_Bin_File(void);

/* clang-format off */
#define ESA_STORE_VAL_MGR_WPR(var_name, value)                                  STORE_VAL_MGR_WPR(Esa_Bin_Writer_Suffix, var_name, value)
#define ESA_STORE_VAL_MGR_WPR_INDEXED(var_name, value, index)                   STORE_VAL_MGR_WPR_INDEXED(Esa_Bin_Writer_Suffix, var_name, value, index)
#define ESA_STORE_ARRAY_ELEM_MGR_WPR(var_name, value, arr_index)                STORE_ARRAY_ELEM_MGR_WPR(Esa_Bin_Writer_Suffix, var_name, value, arr_index)
#define ESA_STORE_ARRAY_ELEM_MGR_WPR_INDEXED(var_name, value, arr_index, index) STORE_ARRAY_ELEM_MGR_WPR_INDEXED(Esa_Bin_Writer_Suffix, var_name, value, arr_index, index)
/* clang-format on */

#define Binary_Esa_Write_Bin_File() Esa_Write_Bin_File()

#else

#define Binary_Esa_Write_Bin_File()

#endif /* BINARY_DEBUG */

#endif /* ESA_DEBUG_WRITER_H */

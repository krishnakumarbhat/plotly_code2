#ifndef CTA_DEBUG_WRITER_H
#define CTA_DEBUG_WRITER_H

/**
 * @file cta_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for CTA bin writer functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#ifdef BINARY_DEBUG

#include "AS_bin_writer_wrapper.h"

#ifdef __GNUC__
static const char *Cta_Bin_Writer_Suffix __attribute__((unused)) = "CtaOutput";
#else
static const char *Cta_Bin_Writer_Suffix = "CtaOutput";
#endif

/* Declare function used to write bin files. */
void Cta_Write_Bin_File(void);

/* clang-format off */
#define CTA_STORE_VAL_MGR_WPR(var_name, value)                                  STORE_VAL_MGR_WPR(Cta_Bin_Writer_Suffix, var_name, value)
#define CTA_STORE_VAL_MGR_WPR_INDEXED(var_name, value, index)                   STORE_VAL_MGR_WPR_INDEXED(Cta_Bin_Writer_Suffix, var_name, value, index)
#define CTA_STORE_ARRAY_ELEM_MGR_WPR(var_name, value, arr_index)                STORE_ARRAY_ELEM_MGR_WPR(Cta_Bin_Writer_Suffix, var_name, value, arr_index)
#define CTA_STORE_ARRAY_ELEM_MGR_WPR_INDEXED(var_name, value, arr_index, index) STORE_ARRAY_ELEM_MGR_WPR_INDEXED(Cta_Bin_Writer_Suffix, var_name, value, arr_index, index)
/* clang-format on */

#define Binary_Cta_Write_Bin_File() Cta_Write_Bin_File()

#else

#define Binary_Cta_Write_Bin_File()

#endif /* BINARY_DEBUG */

#endif /* CTA_DEBUG_WRITER_H */

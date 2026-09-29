#ifndef FBK_DEBUG_WRITER_H
#define FBK_DEBUG_WRITER_H

/**
 * @file fbk_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function to debug FBK output.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

#ifdef BINARY_DEBUG

#include "AS_bin_writer_wrapper.h"

#ifdef __GNUC__
static const char *AS_bww_FBK_OutputString __attribute__((unused)) = "FbkOutput";
static const char *As_Bww_Vehicle_String __attribute__((unused))   = "Vehicle";
static const char *As_Bww_Tracker_String __attribute__((unused))   = "TrackerOutput";
#else
static const char *AS_bww_FBK_OutputString = "FbkOutput";
static const char *As_Bww_Vehicle_String   = "Vehicle";
static const char *As_Bww_Tracker_String   = "TrackerOutput";
#endif

/* Declare function used to write bin files. */
void Fbk_Write_Bin_File(void);

/* clang-format off */
#define FBK_STORE_VAL_MGR_WPR(var_name, value)                   STORE_VAL_MGR_WPR(AS_bww_FBK_OutputString , var_name, value)
#define FBK_STORE_ARRAY_ELEM_MGR_WPR(var_name, value, arr_index) STORE_ARRAY_ELEM_MGR_WPR(AS_bww_FBK_OutputString , var_name, value, arr_index)
/* clang-format on */

#define Binary_Fbk_Write_Bin_File() Fbk_Write_Bin_File()

#else

#define Binary_Fbk_Write_Bin_File()

#endif /* BINARY_DEBUG */

#endif /* FBK_DEBUG_WRITER_H */

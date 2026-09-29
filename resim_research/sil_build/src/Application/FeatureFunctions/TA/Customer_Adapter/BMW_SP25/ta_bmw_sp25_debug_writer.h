#ifndef TA_BMW_SP25_DEBUG_WRITER_H
#define TA_BMW_SP25_DEBUG_WRITER_H

/**
 * @file ta_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for TA bin writer functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/* Declare function used to write bin files. */
void Ta_Bmw_Sp25_Write_Bin_File(void);

#ifdef BINARY_DEBUG

#define Binary_Ta_Bmw_Sp25_Write_Bin_File() Ta_Bmw_Sp25_Write_Bin_File()

#else

#define Binary_Ta_Bmw_Sp25_Write_Bin_File()

#endif /* BINARY_DEBUG */

#endif /* TA_BMW_SP25_DEBUG_WRITER_H */

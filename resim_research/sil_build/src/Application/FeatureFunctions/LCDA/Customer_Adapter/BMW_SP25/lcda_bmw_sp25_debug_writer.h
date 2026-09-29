#ifndef LCDA_BMW_SP25_DEBUG_WRITER_H
#define LCDA_BMW_SP25_DEBUG_WRITER_H

/**
 * @file lcda_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for LCDA bin writer functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/* Declare function used to write bin files. */
void Lcda_Bmw_Sp25_Write_Bin_File(void);

#ifdef BINARY_DEBUG

#define Binary_Lcda_Bmw_Sp25_Write_Bin_File() Lcda_Bmw_Sp25_Write_Bin_File()

#else

#define Binary_Lcda_Bmw_Sp25_Write_Bin_File()

#endif /* BINARY_DEBUG */

#endif /* LCDA_BMW_SP25_DEBUG_WRITER_H */

#ifndef LCDA_RNA_SWEET400_DEBUG_WRITER_H
#define LCDA_RNA_SWEET400_DEBUG_WRITER_H

/**
 * @file lcda_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for LCDA bin writer functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/* Declare function used to write bin files. */
void Lcda_Rna_Sweet400_Write_Bin_File(void);

#ifdef BINARY_DEBUG

#define Binary_Lcda_Rna_Sweet400_Write_Bin_File() Lcda_Rna_Sweet400_Write_Bin_File()

#else

#define Binary_Lcda_Rna_Sweet400_Write_Bin_File()

#endif /* BINARY_DEBUG */

#endif /* LCDA_RNA_SWEET400_DEBUG_WRITER_H */

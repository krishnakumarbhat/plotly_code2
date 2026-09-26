#ifndef FUSA_BBE_SELF_TEST_H
#define FUSA_BBE_SELF_TEST_H
/**
 * @file fusa_bbe_self_test.h
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2025 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 * FUSA BBE Self Test.
 *
 * @note This file should only contain constant definitions and NO INCLUDE files.
 *
 * @section ABBR ABBREVIATIONS:
 *   - @todo List any abbreviations, precede each with a dash ('-').
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - @todo Update list of design document(s).
 *
 *   - Requirements Document(s):
 *     - @todo Update list of requirements document(s)
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - ESGW_4-2_PE-SWx_00-01-A02_EN - C Coding Standards [20120506]
 *     - @todo Update list of other applicable standards
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - @todo List of deviations from standards in this file, or "None".
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include "reuse.h"
/*===========================================================================*
 * Other Header Files
 *===========================================================================*/

#ifdef __cplusplus
extern "C"
{      /* ! Inclusion of header files should NOT be inside the extern "C" block */
#endif /* __cplusplus */

   /*===========================================================================*
    * Exported Preprocessor #define Constants
    *===========================================================================*/

   /*===========================================================================*
    * Exported Preprocessor #define MACROS
    *===========================================================================*/

   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/
   typedef enum Fusa_BBE_Check_Error_Tag
   {
      Fusa_BBE_Check_CHECK_NO_ERROR = 0,
      Fusa_BBE_Check_FAIL_ERROR
   } Fusa_BBE_Check_Error_T;
   /*===========================================================================*
    * Exported Object Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Inline Function Definitions and #define Function-Like Macros
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Prototypes
    *===========================================================================*/
   Fusa_BBE_Check_Error_T FUSA_BBE_Check_Execute(void);
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/** @} doxygen end group */
#endif /* FUSA_BBE_SELF_TEST_H_ */

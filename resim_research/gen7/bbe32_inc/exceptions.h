#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
/*===========================================================================*/
/**
 * @file exceptions.h
 *
 * This file contains interfaces for exception configuration
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2023 Aptiv. All rights reserved.
 * Aptiv Sensitive Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 * This file contains interfaces for exception configuration.
 *
 * @section ABBR ABBREVIATIONS:
 *   - @todo List any abbreviations, precede each with a dash ('-').
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - @todo Update list of design document(s).
 *
 * Add Polarion Work Item Link to the intended line (if using Resource Link
 * for traceability)
 * Syntax:
 * @wi.<LinkRoleAsSingleWord> <PolarionProjectID>/<workitemID>
 * example:
 * @wi.implemented PDP2.0_playground/WI-5001
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
 *
 * @defgroup template Provide API description and define/delete next line
 * @ingroup <parent_API> (OPTIONAL USE if part of another API, else delete)
 * @{
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif

   /*==================================================================================================
   *                                          CONSTANTS
   ==================================================================================================*/

   /*==================================================================================================
   *                                      DEFINES AND MACROS
   ==================================================================================================*/

   /*==================================================================================================
   *                                             ENUMS
   ==================================================================================================*/

   /*==================================================================================================
   *                                STRUCTURES AND OTHER TYPEDEFS
   ==================================================================================================*/

   /*==================================================================================================
   *                                GLOBAL VARIABLE DECLARATIONS
   ==================================================================================================*/

   /*==================================================================================================
   *                                    FUNCTION PROTOTYPES
   ==================================================================================================*/
   void Init_Exception_Handlers(void);

#ifdef __cplusplus
}
#endif

#endif /* EXCEPTIONS_H */

#ifndef CDC_IF_H
#define CDC_IF_H
#ifdef CDC_ENABLE
   /*===========================================================================*/
   /**
    * @file cdc_if.h
    *
    * @todo application CDC interface header file.
    *
    *------------------------------------------------------------------------------
    *
    * Copyright (C) 2024 Aptiv. All rights reserved.
    *Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
    *
    *------------------------------------------------------------------------------
    *
    * @section DESC DESCRIPTION:
    *
    * @todo Add full description here
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
   #include "spbb_typedefs.h"
   #ifdef __cplusplus
extern "C"
{         /* ! Inclusion of header files should NOT be inside the extern "C" block */
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

   /*===========================================================================*
    * Exported Const Object Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Prototypes
    *===========================================================================*/

   bool Appl_Cdc_Process_Execute(uint16_t range_idx, rdop_avg_t **rdop_avg_data_ptr);
   bool Cdc_Cfg_Init(Radar_Look_T look_id);
      /*===========================================================================*
       * Exported Inline Function Definitions and #define Function-Like Macros
       *===========================================================================*/

   #ifdef __cplusplus
} /* extern "C" */
   #endif /* __cplusplus */
#endif    /* End of CDC_ENABLE */
#endif    /* Endif CDC_IF_H*/

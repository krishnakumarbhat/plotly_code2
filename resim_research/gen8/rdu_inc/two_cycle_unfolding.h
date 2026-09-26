#ifndef TWO_CYCLE_UNFOLDING_H
   #define TWO_CYCLE_UNFOLDING_H
   /*===========================================================================*/
   /**
    * @file two_cycle_unfolding.h
    * @brief Header file for two-cycle unfolding stage of RDU.
    *
    *------------------------------------------------------------------------------
    *
    * Copyright (C) 2026 Aptiv. All rights reserved.
    * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
    *
    *------------------------------------------------------------------------------
    *
    * @section DESC DESCRIPTION:
    *
    *
    * @section ABBR ABBREVIATIONS:
    *   - RDU - Range Doppler Unfolding
    *
    * @section TRACE TRACEABILITY INFO:
    *   - Design Document(s):
    *
    *   - Requirements Document(s):
    *
    *   - Applicable Standards (in order of precedence: highest first):
    *     - ESGW_4-2_PE-SWx_00-01-A02_EN - C Coding Standards [20120506]
    *
    * @defgroup two_cycle_unfolding Provide API description and define/delete next line
    * @{
    */
   /*==========================================================================*/

   /*===========================================================================*
    * Standard Header Files
    *===========================================================================*/
   #include <stdint.h>

   /*===========================================================================*
    * Other Header Files
    *===========================================================================*/
   #include "doppler_unfolding.h"

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
   void Two_Cycle_Unfolding_Process(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals,
                                    RDU_Buffer_T *p_rdu_buffer);
   /*===========================================================================*
    * Exported Inline Function Definitions and #define Function-Like Macros
    *===========================================================================*/

   #ifdef __cplusplus
} /* extern "C" */
   #endif /* __cplusplus */

/** @} doxygen end group */
#endif /* TWO_CYCLE_UNFOLDING_H */

/* END OF FILE -------------------------------------------------------------- */

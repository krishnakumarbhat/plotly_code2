#ifndef MAIN_APPLICATION_H
#define MAIN_APPLICATION_H
/**
 * @file main_application.h
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2023 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 * Main application definitions.
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

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "reuse.h"

#ifdef __cplusplus
extern "C"
{      /* ! Inclusion of header files should NOT be inside the extern "C" block */
#endif /* __cplusplus */

/*===========================================================================*
 * Exported Preprocessor #define Constants
 *===========================================================================*/
#define FRAME_TIME_PERIOD_US (50000U) /*Total Time for RDD + AF */
#define BUFFER_TIME_IPC_US   (1000U)  /* Buffer time for Fasica correction + ISR time*/
/** Fixed trigger count from BBE to m7
 * @note: FIXED_BBE_TRIGGER_TIME should be greater than (FRAME_TIME_PERIOD_US - BUFFER_TIME_IPC_US).
 */
#define FIXED_BBE_TRIGGER_TIME (46) /* ms */
#define AF_TIMEOUT_MS          ((FIXED_BBE_TRIGGER_TIME * 1000) - BUFFER_TIME_IPC_US)
/*===========================================================================*
 * Exported Preprocessor #define MACROS
 *===========================================================================*/
#define ENABLE_PROFILE_TIMING_OUTPUTS

   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Const Object Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Object Declarations
    *===========================================================================*/
   extern volatile uint32_t BBE32_Init_Error_Status;

   /*===========================================================================*
    * Exported Function Prototypes
    *===========================================================================*/
   void Run_State_Machine(void);
   void Create_Delay(uint8_t req_time);
   /*===========================================================================*
    * Exported Inline Function Definitions and #define Function-Like Macros
    *===========================================================================*/

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/** @} doxygen end group */
#endif /* MAIN_APPLICATION_H_ */

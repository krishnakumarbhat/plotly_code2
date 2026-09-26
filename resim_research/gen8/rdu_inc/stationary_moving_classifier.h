#ifndef STATIONARY_MOVING_CLASSIFIER_H
#define STATIONARY_MOVING_CLASSIFIER_H
/*===========================================================================*/
/**
 * @file stationary_moving_classifier.h
 *
 *      Header file for stationary_moving_classifier.c
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
 *   -
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - ESGW_4-2_PE-SWx_00-01-A02_EN - C Coding Standards [20120506]
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 *
 * @defgroup stationary_moving_classifier Stationary Moving Classifier API
 * @{
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "doppler_unfolding.h"

#ifdef __cplusplus
extern "C"
{      /* ! Inclusion of header files should NOT be inside the extern "C" block */
#endif /* __cplusplus */

   /*===========================================================================*
    * Exported Preprocessor #define Constants
    *===========================================================================*/
#ifndef M_PI
   #define M_PI (3.14159265358979323846F)
#endif

   /*===========================================================================*
    * Exported Preprocessor #define MACROS
    *===========================================================================*/
/**
 * @brief Convert degrees to radians
 *
 * @param degrees Angle in degrees
 * @return Angle in radians
 */
#define RDU_DEG2RAD(x) ((x)*M_PI / 180.0)

/**
 * @brief Convert radians to degrees
 *
 * @param radians Angle in radians
 * @return Angle in degrees
 */
#define RDU_RAD2DEG(x) ((x)*180.0 / M_PI)
   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Const Object Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Prototypes
    *===========================================================================*/

   /* Main entry point for this module */
#ifdef ENABLE_RDU_TESTING
   bool __attribute__((used))
   Stationary_Moving_Classifier_Process(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals);
#else
bool Stationary_Moving_Classifier_Process(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals);
#endif
   /*===========================================================================*
    * Exported Inline Function Definitions and #define Function-Like Macros
    *===========================================================================*/

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/** @} doxygen end group */

#endif /* STATIONARY_MOVING_CLASSIFIER_H */

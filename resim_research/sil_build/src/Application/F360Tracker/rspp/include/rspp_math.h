#ifndef RSPP_MATH_H
#define RSPP_MATH_H
/*===========================================================================*/
/**
 * @file rspp_math.h
 *
 * @brief Mathematical Utility Functions
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Mathematical constants and wrapper functions for RSPP module.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
 *
 *   - Requirements Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/51-SoftwareRequirementsSpecifications/CMP_SRS_TrackerCore
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *     - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
  *   - None.
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 *
 * @defgroup rspp_math Math Utilities
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include "rspp_reuse.h"
#include <cmath>

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/

/*===========================================================================*
 * Exported Enum Class Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/
static constexpr float32_t RSPP_EPSILON = 1.19e-07F;
static constexpr float32_t RSPP_PI = 3.14159265358979323846F;   // pi

/*===========================================================================*
 * Exported Class Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Function Declarations
 *===========================================================================*/
inline float32_t RSPP_deg2rad(const float32_t x) { return x * 0.0174532925F; }

#ifdef __TASKING__
inline float32_t RSPP_Acosf(const float32_t x) { return static_cast<float>(acosf(x)); }
inline float32_t RSPP_Asinf(const float32_t x) { return static_cast<float>(asinf(x)); }
inline float32_t RSPP_Atan2f(const float32_t y, const float32_t x) { return static_cast<float>(atan2f(y, x)); }
inline float32_t RSPP_Atanf(const float32_t x) { return static_cast<float>(atanf(x)); }
inline float32_t RSPP_Ceilf(const float32_t x) { return static_cast<float>(ceilf(x)); }
inline float32_t RSPP_Copysignf(const float32_t f, const float32_t sign) { return static_cast<float>(copysignf(f, sign)); }
inline float32_t RSPP_Cosf(const float32_t x) { return static_cast<float>(cosf(x)); }
inline float32_t RSPP_Expf(const float32_t x) { return static_cast<float>(expf(x)); }
inline float32_t RSPP_Floorf(const float32_t x) { return static_cast<float>(floorf(x)); }
inline float32_t RSPP_Fmodf(const float32_t x, const float32_t y) { return static_cast<float>(fmodf(x, y)); }
inline float32_t RSPP_Logf(const float32_t x) { return static_cast<float>(logf(x)); }
inline float32_t RSPP_Powf(const float32_t x, const float32_t y) { return static_cast<float>(powf(x, y)); }
inline float32_t RSPP_Roundf(const float32_t x) { return static_cast<float>(roundf(x)); }
inline float32_t RSPP_Sinf(const float32_t x) { return static_cast<float>(sinf(x)); }
inline float32_t RSPP_Sqrtf(const float32_t x) { return static_cast<float>(sqrtf(x)); }
inline float32_t RSPP_Tanf(const float32_t x) { return static_cast<float>(tanf(x)); }

#else
inline float32_t RSPP_Acosf(const float32_t x) { return static_cast<float>(acosf(x)); }
inline float32_t RSPP_Asinf(const float32_t x) { return static_cast<float>(asinf(x)); }
inline float32_t RSPP_Atan2f(const float32_t y, const float32_t x) { return static_cast<float>(atan2f(y, x)); }
inline float32_t RSPP_Atanf(const float32_t x) { return static_cast<float>(atanf(x)); }
inline float32_t RSPP_Ceilf(const float32_t x) { return static_cast<float>(ceilf(x)); }
inline float32_t RSPP_Copysignf(const float32_t f, const float32_t sign) { return static_cast<float>(copysignf(f, sign)); }
inline float32_t RSPP_Cosf(const float32_t x) { return static_cast<float>(cosf(x)); }
inline float32_t RSPP_Expf(const float32_t x) { return static_cast<float>(expf(x)); }
inline float32_t RSPP_Floorf(const float32_t x) { return static_cast<float>(floorf(x)); }
inline float32_t RSPP_Fmodf(const float32_t x, const float32_t y) { return static_cast<float>(fmodf(x, y)); }
inline float32_t RSPP_Logf(const float32_t x) { return static_cast<float>(logf(x)); }
inline float32_t RSPP_Powf(const float32_t x, const float32_t y) { return static_cast<float>(powf(x, y)); }
inline float32_t RSPP_Roundf(const float32_t x) { return static_cast<float>(roundf(x)); }
inline float32_t RSPP_Sinf(const float32_t x) { return static_cast<float>(sinf(x)); }
inline float32_t RSPP_Sqrtf(const float32_t x) { return static_cast<float>(sqrtf(x)); }
inline float32_t RSPP_Tanf(const float32_t x) { return static_cast<float>(tanf(x)); }

#endif // F360MATH

/** @} doxygen end group */
#endif // F360_MATH_H

/*============================================================================*\
 * AUTHOR(S) IDENTITY (AID)
 *-----------------------------------------------------------------------------
 *
 *  AID         NAME
 *  ---------------------------------------------------------------------------
 *  wzfkqj      Tobias Almroth
\*============================================================================*/

/*============================================================================*\
 * FILE REVISION HISTORY
 *-----------------------------------------------------------------------------
 *
 *  File history can be traced by URL:
 *  "https://gitgerrit.asux.aptiv.com/q/project:CORECOMP%252FALSW%252FOT_ObjectTracking"
\*============================================================================*/

/* END OF FILE -------------------------------------------------------------- */

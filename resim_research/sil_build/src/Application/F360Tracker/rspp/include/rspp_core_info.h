#ifndef RSPP_CORE_INFO_H
#define RSPP_CORE_INFO_H
/*===========================================================================*/
/**
 * @file rspp_core_info.h
 *
 * @brief RSPP Core Information
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Core timing and execution information structure for RSPP module.
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
 * @defgroup rspp_core_info Core Info
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include "rspp_reuse.h"

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
/******************************************************************************
 * Name:  RSPP_Core_Info_T
 *   Structure containing core timing and execution information.
 *
 * Struct Members:
 *   time_us            - [us] Current time stamp in microseconds.
 *   prev_time_us       - [us] Previous time stamp in microseconds.
 *   cnt_loops          - Number of times the core processing function has been executed.
 *   elapsed_time_s     - [s] Elapsed time since last core processing function call in seconds.
 ******************************************************************************/
struct RSPP_Core_Info_T
{
   uint64_t time_us;
   uint64_t prev_time_us;
   uint32_t cnt_loops;
   float32_t elapsed_time_s;
};

/*===========================================================================*
 * Exported Class Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Function Declarations
 *===========================================================================*/

static_assert(24 == sizeof(RSPP_Core_Info_T),
              "sizeof(RSPP_Core_Info_T) not as expected. Remember to align padding if needed");

/** @} doxygen end group */
#endif

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

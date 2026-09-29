#ifndef RSPP_LOOK_ID_H
#define RSPP_LOOK_ID_H
/*===========================================================================*/
/**
 * @file rspp_look_ID.h
 *
 * @brief Detection Look ID Enumeration
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Look identifier enumeration for radar detection data.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *   - LR: Long Range
 *   - MR: Medium Range
 *   - LL: Long Look
 *   - ML: Medium Look
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
 * @defgroup rspp_look_ID Look ID
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
/******************************************************************************
 * Name:  RSPP_Det_Look_ID_T
 *   Enumeration for detection look ID.
 *
 * Enum Values:
 *   RSPP_DET_LOOK_ID_INVALID - Invalid look ID
 *   RSPP_DET_LOOK_ID_0       - Look ID 0 (LR, LL)
 *   RSPP_DET_LOOK_ID_1       - Look ID 1 (LR, ML)
 *   RSPP_DET_LOOK_ID_2       - Look ID 2 (MR, LL)
 *   RSPP_DET_LOOK_ID_3       - Look ID 3 (MR, ML)
 ******************************************************************************/
typedef enum RSPP_Det_Look_ID_Tag : int8_t
{
   RSPP_DET_LOOK_ID_INVALID = (-1),
   RSPP_DET_LOOK_ID_0 = (0),
   RSPP_DET_LOOK_ID_1 = (1),
   RSPP_DET_LOOK_ID_2 = (2),
   RSPP_DET_LOOK_ID_3 = (3),
   RSPP_DET_NUM_LOOK_ID = (4)
} RSPP_Det_Look_ID_T;

/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Class Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Function Declarations
 *===========================================================================*/

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

#ifndef RSPP_COMPONENT_VARIANT_TYPE_H
#define RSPP_COMPONENT_VARIANT_TYPE_H
/*===========================================================================*/
/**
 * @file rspp_variant_type.h
 *
 * @brief RSPP Variant Type Enumeration
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Variant type enumeration for different RSPP configurations.
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
 * @defgroup rspp_variant_type Variant Type
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

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
 * Name:  RSPP_Variant_Type_T
 *   Enum for different RSPP variant types.
 ******************************************************************************/
typedef enum RSPP_Variant_Type_Tag : uint8_t
{
   RSPP_VARIANT_TYPE_A = (0),
   RSPP_VARIANT_TYPE_B = (1),
   RSPP_VARIANT_TYPE_C = (2),
   RSPP_VARIANT_TYPE_D = (3),
   RSPP_VARIANT_TYPE_E = (4),
   RSPP_VARIANT_TYPE_F = (5),
   RSPP_VARIANT_TYPE_G = (6),
   RSPP_VARIANT_TYPE_H = (7),
   RSPP_VARIANT_TYPE_I = (8),
   RSPP_VARIANT_TYPE_J = (9),
   RSPP_VARIANT_TYPE_K = (10),
   RSPP_VARIANT_TYPE_L = (11),
   RSPP_VARIANT_TYPE_M = (12),
   RSPP_VARIANT_TYPE_N = (13),
   RSPP_VARIANT_TYPE_O = (14),
   RSPP_VARIANT_TYPE_P = (15),
   RSPP_VARIANT_TYPE_Q = (16),
   RSPP_VARIANT_TYPE_R = (17),
   RSPP_VARIANT_TYPE_S = (18),
} RSPP_Variant_Type_T;

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

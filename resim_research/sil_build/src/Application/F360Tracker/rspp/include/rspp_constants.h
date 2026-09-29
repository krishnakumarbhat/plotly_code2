#ifndef RSPP_CONSTANTS_VARIANT_A_H
#define RSPP_CONSTANTS_VARIANT_A_H
/*===========================================================================*/
/**
 * @file rspp_constants.h
 *
 * @brief RSPP Constants
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Constant definitions for RSPP module.
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
 * @defgroup rspp_constants Constants
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
#include "rspp_variant_definition.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace rspp_variant_A
{
   /*===========================================================================*
    * Exported Enum Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/
   static constexpr uint16_t MAX_NUMBER_OF_DETECTIONS =
       ((MAX_NUMBER_OF_SRR_SENSORS * NUMBER_OF_SRR_DETECTIONS) + (MAX_NUMBER_OF_MRR_SENSORS * NUMBER_OF_MRR_DETECTIONS));
   static constexpr uint8_t MAX_NUMBER_OF_SENSORS = (MAX_NUMBER_OF_SRR_SENSORS + MAX_NUMBER_OF_MRR_SENSORS);
   static constexpr uint16_t MAX_DETS_FOR_SINGLE_SENSOR = (NUMBER_OF_SRR_DETECTIONS > NUMBER_OF_MRR_DETECTIONS)
                                                              ? NUMBER_OF_SRR_DETECTIONS
                                                              : NUMBER_OF_MRR_DETECTIONS;

   // Number of reference points for detections in vcs long sorted order
   static constexpr uint8_t MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS = 31U;

   // Array of reference detection indexes is appended with smallest VCS-long position at first element and largst
   // VCS-long position at last invalid element
   static constexpr uint8_t MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS = (MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS + 2U);

   /*===========================================================================*
    * Exported Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Declarations
    *===========================================================================*/
}

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

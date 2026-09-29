#ifndef RSPP_DETECTION_LIST_VARIANT_A_H
#define RSPP_DETECTION_LIST_VARIANT_A_H
/*===========================================================================*/
/**
 * @file rspp_detection_list.h
 *
 * @brief Detection List Structure
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Detection list container structure with sorted reference indices.
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
 * @defgroup rspp_detection_list Detection List
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
#include "rspp_detection.h"
#include "rspp_constants.h"

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
   /******************************************************************************
    * Name:  RSPP_Detection_List_T
    *   Structure containing a list of detections along with sorted reference detection indices.
    *
    * Struct Members:
    *   detections                      - An array of detections.
    *   number_of_valid_detections      - Number of valid detections at in the detection array
    *   vcslong_det_idx_min             - Sorted vcs-long index of detection with most negative vcs-long position
    *   vcslong_det_idx_max             - Sorted vcs-long index of detection with most positive vcs-long position
    *   vcslong_sorted_ref_det_idx     - An array containing reference detection indicies sorted in vcs longitudinal order.
    ******************************************************************************/
   typedef struct RSPP_Detection_List_Tag
   {
      RSPP_Detection_T detections[MAX_NUMBER_OF_DETECTIONS];
      uint32_t number_of_valid_detections;
      int16_t vcslong_det_idx_min;
      int16_t vcslong_det_idx_max;
      int16_t vcslong_sorted_ref_det_idx[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS];
   } RSPP_Detection_List_T;

   /*===========================================================================*
    * Exported Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Declarations
    *===========================================================================*/

   static_assert((MAX_NUMBER_OF_DETECTIONS * sizeof(RSPP_Detection_T) + 76) == sizeof(RSPP_Detection_List_T),
                 "sizeof(RSPP_Detection_T) not as expected. Remember to align padding if needed");

} // namespace rspp_variant_A

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

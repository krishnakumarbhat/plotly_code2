#ifndef RSPP_VCS_LONG_SORTED_DETS_SUPPORT_FUNCTIONS_H
#define RSPP_VCS_LONG_SORTED_DETS_SUPPORT_FUNCTIONS_H
/*===========================================================================*/
/**
 * @file rspp_vcs_long_sorted_dets_support_functions.h
 *
 * @brief VCS Longitudinal Detection Sorting Functions
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Vehicle Coordinate System (VCS) longitudinal detection sorting and reference
 * point management functions for RSPP module.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *   - VCS: Vehicle Coordinate System
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
 * @defgroup rspp_vcs_long VCS Long Sorted
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_detection_list.h"

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

   /*===========================================================================*
    * Exported Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Declarations
    *===========================================================================*/

   /******************************************************************************
    * Name: RSPP_Sort_Detections_Vcs_Long
    *   This function sorts detections in longitudinal ascending order and stores
    *   reference indices at predefined longitudinal positions. It updates min/max
    *   VCS longitudinal detection indices for efficient lookup operations.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   raw_detections  - Reference to detection list to be sorted (input/output)
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Sort_Detections_Vcs_Long(
       RSPP_Detection_List_T &raw_detections);

   /******************************************************************************
    * Name: RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info
    *   This function updates reference data for VCS longitudinally sorted
    *   detections. It also updates which reference point to start from
    *   next time this function is called for optimization purposes.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   det_vcs_long           - Longitudinal position of current detection
    *   det_idx                - Index of current detection
    *   vcs_long_ref_start_idx - Reference to VCS longitudinal start index (input/output)
    *   raw_detections         - Reference to detection list (input/output)
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       const float32_t det_vcs_long,
       const uint32_t det_idx,
       uint32_t &vcs_long_ref_start_idx,
       RSPP_Detection_List_T &raw_detections);

   /******************************************************************************
    * Name: RSPP_Clear_Dets_Vcs_Long_Sorted_Info
    *   This function resets VCS longitudinal sorting data in detection list. It
    *   sets all reference indices to invalid values and resets min/max indices to
    *   their initial state.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   Reference to detection list to reset (input/output)
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Clear_Dets_Vcs_Long_Sorted_Info(
       RSPP_Detection_List_T &raw_detections);

   /******************************************************************************
    * Name: RSPP_Get_First_Relevant_Long_Sorted_Det_Idx
    *   This function returns the detection index closest to the specified
    *   longitudinal value. It returns the index of detection that is the closest
    *   reference point with value less than vcs_long_value.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   vcs_long_value - Longitudinal value to find closest reference for
    *   raw_detections - Reference to sorted detection list
    *
    * Return Value:
    *   Index of detection closest to specified value, or invalid value
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   int32_t RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(
       const float32_t vcs_long_value,
       const RSPP_Detection_List_T &raw_detections);
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

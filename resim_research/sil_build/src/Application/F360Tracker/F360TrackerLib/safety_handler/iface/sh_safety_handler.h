
#ifndef SH_SAFETY_HANDLER_VARIANT_A_H
#define SH_SAFETY_HANDLER_VARIANT_A_H

/*===========================================================================*/
/**
 * @file sh_safety_handler.h
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2025 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *   This file declares the public API for the F360 safety handler module used
 *   in radar object tracking validation. It provides initialization and
 *   acceptance checking interfaces to verify tracker output plausibility,
 *   manage safety state transitions, and report fault status for safety-critical
 *   monitoring of object-detection associations.
 *
 * @section ABBR ABBREVIATIONS:
 *   - NA
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/53-SoftwareDetailedDesigns/AAU_SDD_Safety_Handler
 *
 * Add Polarion Work Item Link to the intended line (if using Resource Link
 * for traceability)
 * @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8371
 * @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8399
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
 * @defgroup template Provide API description and define/delete next line
 * @{
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "f360_rot_object_log.h"
#include "f360_detection_log.h"
#include "rspp_detection_list.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace f360_variant_A
{
   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/
   typedef enum Safety_Handler_Return_Type_Tag
   {
      SH_E_OK = 0U,             // safety handler initialized
      SH_E_NOT_INITIALIZED = 1U // safety handler not initialized
   } Safety_Handler_Return_Type_T;

   /*===========================================================================*
    * Exported Enum Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Class Declarations
    *===========================================================================*/

   /******************************************************************************
    * Name: Safety_Handler_Acceptance_Check
    *       This function call performs tracker output object list plausible checks basing on
    *       the tracker input data, and update the tracker overall fault in rot_object_list_info struct
    *
    * Shared Variables: none
    *
    * Parameters: processed_det_ref - reference detections which is tracker input
    *             associated_detections - object associated detections
    *             number_of_valid_det - number of valid detections of the reference detections
    *             radar_object_data- tracker generated radar object data
    *             overall_fault- overall fault status output
    *
    * Return Value:
    *    SH_E_NOT_INITIALIZED - when safety handler is not initialized.
    *    SH_E_OK - when safety handler is initialized.
    *
    * Design Information:
    *  - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
    *    wiki/53-SoftwareDetailedDesigns/AAU_SDD_Safety_Handler
    *
    * Change References:
    *  - NA
    *
    * Add Polarion Work Item Link to the intended line (if using Resource Link
    * for traceability) - refer to module header comment block.
    *
    ******************************************************************************/
   Safety_Handler_Return_Type_T Safety_Handler_Acceptance_Check(
       const rspp_variant_A::RSPP_Detection_T (&processed_det_ref)[MAX_NUMBER_OF_DETECTIONS],
       const F360_Detection_Log_T (&associated_detections)[MAX_NUMBER_OF_DETECTIONS],
       const uint32_t number_of_valid_det,
       const ROT_Object_Output_T (&radar_object_data)[NUMBER_OF_REDUCED_OBJECT_TRACKS],
       uint8_t &overall_fault);

   /******************************************************************************
    * Name: Safety_Handler_Initialize
    *       This function initialize the safety handler internal state.
    *
    * Shared Variables: none
    *
    * Parameters: None
    *
    * Return Value: None
    *
    * Design Information:
    *  - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
    *    wiki/53-SoftwareDetailedDesigns/AAU_SDD_Safety_Handler
    *
    * Change References:
    *  - NA
    *
    * Add Polarion Work Item Link to the intended line (if using Resource Link
    * for traceability) - refer to module header comment block.
    *
    ******************************************************************************/
   void Safety_Handler_Initialize();
}
// namespace f360_variant_A
/** @} doxygen end group */
#endif /*F360_SAFETY_HANDLER_H */

/*============================================================================*\
 * AUTHOR(S) IDENTITY (AID)
 *-----------------------------------------------------------------------------
 *
 *  AID         NAME
 *  ---------------------------------------------------------------------------
 *  fjzzjn      Wenbo Xu
\*============================================================================*/

/*============================================================================*\
 * FILE REVISION HISTORY
 File history can be traced by URL:
 "https://gitgerrit.asux.aptiv.com/q/project:CORECOMP%252FALSW%252FOT_ObjectTracking"

\*============================================================================*/

/* END OF FILE -------------------------------------------------------------- */

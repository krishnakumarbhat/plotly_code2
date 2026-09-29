#ifndef SH_SAFETY_HANDLER_INTERNAL_H
#define SH_SAFETY_HANDLER_INTERNAL_H
/*===========================================================================*/
/**
 * @file sh_safety_handler_internal.h
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2025 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *   This file declares internal state management functions for the F360 safety
 *   handler module. It defines the safety state enumeration and provides APIs
 *   to query and update the handler's operational state (uninitialized, fault-free,
 *   or safe mode) and evaluate plausibility check results to determine fault
 *   status transitions for safety-critical failure handling.
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
 * @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8375
 *
 *   - Requirements Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/51-SoftwareRequirementsSpecifications/CMP_SRS_TrackerCore
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *      - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *      - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
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
   typedef enum Safety_Handler_Safety_State_Tag
   {
      STATE_UNINITIALIZED = 255U, // safety handler not initialized
      STATE_FAULT_FREE = 195U,    // safety handler initialized and no fault detected
      STATE_SAFE = 60U            // safety handler safe state when fault detected
   } Safety_Handler_Safety_State_T;

   /*===========================================================================*
    * Exported Enum Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Class Declarations
    *===========================================================================*/

   /******************************************************************************
    * Name:  Set_Safety_Handler_State
    *   Set the current state of the safety handler.
    *
    * Shared Variables: none
    *
    * Parameters: new_state - new safe state to set
    *
    * Return Value: None
    *
    * Design Information:
    *  - NA
    *
    * Change References:
    *  - NA
    *
    * Add Polarion Work Item Link to the intended line (if using Resource Link
    * for traceability) - refer to module header comment block.
    *
    ******************************************************************************/
   void Set_Safety_Handler_State(const Safety_Handler_Safety_State_T new_state);

   /******************************************************************************
    * Name:  Get_Safety_Handler_State
    *   Get the current state of the safety handler.
    *
    * Shared Variables: none
    *
    * Parameters: None
    *
    * Return Value:
    *    STATE_UNINITIALIZED - if safety handler is not initialized
    *    STATE_FAULT_FREE - if safety handler is initialized and no fault detected
    *    STATE_SAFE - if safety handler is in safe state
    *
    * Design Information:
    *  - NA
    *
    * Change References:
    *  - NA
    *
    * Add Polarion Work Item Link to the intended line (if using Resource Link
    * for traceability) - refer to module header comment block.
    *
    ******************************************************************************/
   Safety_Handler_Safety_State_T Get_Safety_Handler_State();

   /******************************************************************************
    * Name: Evaluate_Safe_State
    *       This funtion checks if safety handler state is STATE_SAFE in the life cycle
    *       to decide either bypass the overall_fault_status or set it to fault status
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
    ******************************************************************************/
   void Evaluate_Safe_State(const bool f_plausible, uint8_t &overall_fault_status);

} // namespace f360_variant_A
/** @} doxygen end group */
#endif /*F360_SAFETY_HANDLER_INTERNAL_H */

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

#ifndef SH_OBJECTS_PLAUSIBLE_CHECKS_H
#define SH_OBJECTS_PLAUSIBLE_CHECKS_H
/*===========================================================================*/
/**
 * @file f360_object_plausible_checks.h
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2025 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *   This file declares plausibility checking functions for F360 radar object
 *   tracking validation. It provides APIs to verify that tracked objects have
 *   associated detections within acceptable geometric bounds by comparing
 *   detection positions against extended bounding boxes with calibrated
 *   tolerances. These checks ensure object-detection correspondence for
 *   safety-critical tracking applications.
 *
 * @section ABBR ABBREVIATIONS:
 *   - NA
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *      - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *        wiki/53-SoftwareDetailedDesigns/AAU_SDD_Safety_Handler
 *
 * Add Polarion Work Item Link to the intended line (if using Resource Link
 * for traceability)
 * @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8372
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
#include "f360_reuse.h"
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

   /*===========================================================================*
    * Exported Enum Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Class Declarations
    *===========================================================================*/

   /******************************************************************************
    * Name:  Object_Position_Plausible_Check
    *        In this function it loops over the number of objects to check if their
    *        associated detections raw values matches to the object position values in a certain range.
    *
    * Shared Variables: none
    *
    * Parameters: processed_det_ref - reference detections which is tracker input
    *             associated_detections - object associated detections
    *             number_of_valid_det - number of valid detections of the reference detections
    *             object - single object to perform position plausible check
    *
    * Return Value:
    *    true - if object position plausible check pass
    *    false - if object position plausible check fail
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
   bool Object_Position_Plausible_Check(const rspp_variant_A::RSPP_Detection_T (&processed_det_ref)[MAX_NUMBER_OF_DETECTIONS],
                                        const F360_Detection_Log_T (&associated_detections)[MAX_NUMBER_OF_DETECTIONS],
                                        const uint32_t number_of_valid_det,
                                        const ROT_Object_Output_T &object);

   /******************************************************************************
    * Name:  Object_Plausible_Checks
    *   Perform plausible checks on the object.
    *
    * Shared Variables: none
    *
    * Parameters: processed_det_ref - reference detections which is tracker input
    *             associated_detections - object associated detections
    *             number_of_valid_det - number of valid detections of the reference detections
    *             radar_object_data - tracker generated radar object data
    *
    * Return Value:
    *    true - if all plausible checks pass
    *    false - if any plausible check fails
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
   bool Object_Plausible_Checks(const rspp_variant_A::RSPP_Detection_T (&processed_det_ref)[MAX_NUMBER_OF_DETECTIONS],
                                const F360_Detection_Log_T (&associated_detections)[MAX_NUMBER_OF_DETECTIONS],
                                const uint32_t number_of_valid_det,
                                const ROT_Object_Output_T (&radar_object_data)[NUMBER_OF_REDUCED_OBJECT_TRACKS]);

} // namespace f360_variant_A
/** @} doxygen end group */
#endif /*F360_OBJECTS_PLAUSIBLE_CHECKS_H */

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

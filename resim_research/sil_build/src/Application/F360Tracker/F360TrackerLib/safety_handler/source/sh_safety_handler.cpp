/*===========================================================================*/
/**
 * @file sh_safety_handler.cpp
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2025 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *   This file implements the F360 safety handler state machine and acceptance
 *   checking logic for radar object tracking validation. It manages safety
 *   state transitions (uninitialized, fault-free, safe mode), orchestrates
 *   plausibility checks on tracked objects and detections, and evaluates
 *   results to determine overall fault status for safety-critical failure
 *   detection and response.
 *
 * @section ABBR ABBREVIATIONS:
 *   - NA
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/53-SoftwareDetailedDesigns/AAU_SDD_Safety_Handler
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
 *   - [C77];Compiler warning is generated if typedef enum type is used; No risk locally used with enum only.
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "sh_safety_handler.h"
#include "sh_safety_handler_internal.h"
#include "sh_object_plausible_checks.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Using Namespaces
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace f360_variant_A
{

   /*===========================================================================*
    * Local Type Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Local Enum Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Local Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Static Variable Definitions
    *===========================================================================*/
   static Safety_Handler_Safety_State_T Sh_Current_State = STATE_UNINITIALIZED; // safety handler current state
                                                                                // initialized to STATE_UNINITIALIZED

   /*===========================================================================*
    * Function Definitions
    *===========================================================================*/

   void Safety_Handler_Initialize()
   {
      Set_Safety_Handler_State(STATE_FAULT_FREE); // Initialize safety handler current state to STATE_FAULT_FREE
   }

   void Set_Safety_Handler_State(const Safety_Handler_Safety_State_T new_state)
   {
      Sh_Current_State = new_state;
   }

   Safety_Handler_Safety_State_T Get_Safety_Handler_State()
   {
      return Sh_Current_State;
   }

   Safety_Handler_Return_Type_T Safety_Handler_Acceptance_Check(
       const rspp_variant_A::RSPP_Detection_T (&processed_det_ref)[MAX_NUMBER_OF_DETECTIONS],
       const F360_Detection_Log_T (&associated_detections)[MAX_NUMBER_OF_DETECTIONS],
       const uint32_t number_of_valid_det,
       const ROT_Object_Output_T (&radar_object_data)[NUMBER_OF_REDUCED_OBJECT_TRACKS],
       uint8_t &overall_fault)
   {
      Safety_Handler_Return_Type_T return_status = SH_E_OK;
      if (STATE_UNINITIALIZED == Sh_Current_State)
      {
         return_status = SH_E_NOT_INITIALIZED;
      }
      else
      {
         bool f_plausible = true;
         if (STATE_FAULT_FREE == Sh_Current_State)
         {
            // Check if any object position is implausible. When more implausible checks are added, we need to create a new function to capature all
            // those checks. group thoses properties checks under the same loop of per object, per detection
            f_plausible = Object_Plausible_Checks(processed_det_ref, associated_detections, number_of_valid_det, radar_object_data);
         }
         else
         {
            // already in safe state(sh_current_state == STATE_SAFE), no need to do any checks
         }
         Evaluate_Safe_State(f_plausible, overall_fault);
      }
      return return_status;
   }

   void Evaluate_Safe_State(const bool f_plausible, uint8_t &overall_fault_status)
   {
      if (!f_plausible)
      {
         Set_Safety_Handler_State(STATE_SAFE);
      }

      if (STATE_SAFE == Sh_Current_State)
      {
         enum
         {
            STATUS_FAULT_PRESENT = 60U
         };
         overall_fault_status = static_cast<uint8_t>(STATUS_FAULT_PRESENT);
      }
   }
} // namespace f360_variant_A

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

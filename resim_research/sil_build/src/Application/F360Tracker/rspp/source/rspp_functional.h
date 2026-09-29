#ifndef RSPP_FUNCTIONAL_H
#define RSPP_FUNCTIONAL_H
/*===========================================================================*/
/**
 * @file rspp_functional.h
 *
 * @brief Functional Programming Predicates
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Comparison predicates for sorting and functional programming operations.
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
 * @defgroup rspp_functional Functional
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
namespace rspp_variant_A
{
   namespace cmn
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
       * Name:  f360_less
       *   This template predicate determines if the first element is less than the
       *   second element. Can be used with sorting algorithms to sort collections
       *   in ascending order.
       *
       * Shared Variables: None
       *
       * Parameters:
       *   first_elem  - First element to compare
       *   second_elem - Second element to compare
       *
       * Return Value:
       *   true - If first_elem < second_elem
       *   false - Otherwise
       *
       * Design Information: None
       *
       * Change References: None
       *
       ******************************************************************************/
      template <typename T>
      struct f360_less
      {
         bool operator()(const T &first_elem, const T &second_elem) const
         {
            return first_elem < second_elem;
         }
      };

      /******************************************************************************
       * Name:  f360_greater
       *   This template predicate determines if the first element is greater than
       *   the second element. Can be used with sorting algorithms to sort
       *   collections in descending order.
       *
       * Shared Variables: None
       *
       * Parameters:
       *   first_elem  - First element to compare
       *   second_elem - Second element to compare
       *
       * Return Value:
       *   true - If first_elem > second_elem
       *   false - Otherwise
       *
       * Design Information: None
       *
       * Change References: None
       *
       ******************************************************************************/
      template <typename T>
      struct f360_greater
      {
         bool operator()(const T &first_elem, const T &second_elem) const
         {
            return first_elem > second_elem;
         }
      };

   }
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

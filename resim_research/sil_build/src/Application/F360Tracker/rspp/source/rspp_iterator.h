#ifndef RSPP_ITERATOR_H
#define RSPP_ITERATOR_H
/*===========================================================================*/
/**
 * @file rspp_iterator.h
 *
 * @brief Iterator Utilities
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Iterator utilities for RSPP module providing begin/end functions for arrays.
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
 * @defgroup rspp_iterator Iterator
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_iterator_detail.h"

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
       * Name:  begin
       *   This template function returns an iterator to the first element in a
       *   container or array, supporting multi-dimensional arrays.
       *
       * Shared Variables: None
       *
       * Parameters:
       *   array - Reference to C style array (any number of dimensions)
       *
       * Return Value:
       *   Iterator to first element
       *
       * Design Information: None
       *
       * Change References: None
       *
       ******************************************************************************/
      template <class T, std::size_t N>
      typename detail::Iterator_For<T>::type begin(T (&array)[N])
      {
         return detail::begin_impl(array);
      }

      /******************************************************************************
       * Name:  end
       *   This template function returns an iterator to the end (past-the-last
       *   element) of a container or array, supporting multi-dimensional arrays.
       *
       * Shared Variables: None
       *
       * Parameters:
       *   array - Reference to C style array (any number of dimensions)
       *
       * Return Value:
       *   Iterator to past-the-last element
       *
       * Design Information: None
       *
       * Change References: None
       *
       ******************************************************************************/
      template <class T, std::size_t N>
      typename detail::Iterator_For<T>::type end(T (&array)[N])
      {
         return detail::end_impl(array);
      }
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

#ifndef RSPP_ITERATOR_DETAIL_H
#define RSPP_ITERATOR_DETAIL_H
/*===========================================================================*/
/**
 * @file rspp_iterator_detail.h
 *
 * @brief Iterator Implementation Details
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Implementation details for iterator utilities.
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
 * @defgroup rspp_iterator_detail Iterator Detail
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include <cstddef>

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
      namespace detail
      {
         /*===========================================================================*
          * Exported Enum Class Declarations
          *===========================================================================*/

         /*===========================================================================*
          * Exported Type Declarations
          *===========================================================================*/
         template <class T>
         struct Iterator_For
         {
            using type = T *;
         };

         /*===========================================================================*
          * Exported Class Declarations
          *===========================================================================*/

         /*===========================================================================*
          * Exported Function Declarations
          *===========================================================================*/

         /******************************************************************************
          * Name:  begin_impl
          *   This template function returns a pointer to the first element of an
          *   array, serving as the implementation for the begin() iterator function.
          *
          * Shared Variables: None
          *
          * Parameters:
          *   array - Reference to C style array of size N
          *
          * Return Value:
          *   Pointer to first element
          *
          * Design Information: None
          *
          * Change References: None
          *
          ******************************************************************************/
         template <class T, std::size_t N>
         typename Iterator_For<T>::type begin_impl(T (&array)[N])
         {
            return reinterpret_cast<typename Iterator_For<T>::type>(&array[0]);
         }

         /******************************************************************************
          * Name:  end_impl
          *   This template function returns a pointer past the last element of an
          *   array, serving as the implementation for the end() iterator function.
          *
          * Shared Variables: None
          *
          * Parameters:
          *   array - Reference to C style array of size N
          *
          * Return Value:
          *   Pointer past the last element
          *
          * Design Information: None
          *
          * Change References: None
          *
          ******************************************************************************/
         template <class T, std::size_t N>
         typename Iterator_For<T>::type end_impl(T (&array)[N])
         {
            return reinterpret_cast<typename Iterator_For<T>::type>(&array[N]);
         }

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

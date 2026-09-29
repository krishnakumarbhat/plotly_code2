#ifndef RSPP_SORT_DATA_TYPE_H
#define RSPP_SORT_DATA_TYPE_H
/*===========================================================================*/
/**
 * @file rspp_sort_data_type.h
 *
 * @brief Sort Data Type Structure
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Sort data type structure for RSPP module. Provides data structure with
 * comparison operators for sorting algorithms.
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
 * @defgroup rspp_sort_data Sort Data
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
   struct RSPP_Sort_Data_T
   {
      float32_t data;
      uint32_t index;

      /******************************************************************************
       * Name:  operator<
       *   This function compares if this data value is less than another data
       *   value, enabling sorting algorithms to compare RSPP_Sort_Data_T objects
       *   based on their data field values.
       *
       * Shared Variables: None
       *
       * Parameters:
       *   other  - Reference to another RSPP_Sort_Data_T for comparison
       *
       * Return Value:
       *   true - If this->data < other.data
       *   false - Otherwise
       *
       * Design Information: None
       *
       * Change References: None
       *
       ******************************************************************************/
      bool operator<(const RSPP_Sort_Data_T &other) const
      {
         return (this->data < other.data);
      }

      /******************************************************************************
       * Name:  operator>
       *   This function compares if this data value is greater than another data
       *   value, enabling sorting algorithms to compare RSPP_Sort_Data_T objects
       *   based on their data field values.
       *
       * Shared Variables: None
       *
       * Parameters:
       *   other  - Reference to another RSPP_Sort_Data_T for comparison
       *
       * Return Value:
       *   true - If this->data > other.data
       *   false - Otherwise
       *
       * Design Information: None
       *
       * Change References: None
       *
       ******************************************************************************/
      bool operator>(const RSPP_Sort_Data_T &other) const
      {
         return (this->data > other.data);
      }
   };

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

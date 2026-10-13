#ifndef DETECTION_PROPS_UPDATE_H
   #define DETECTION_PROPS_UPDATE_H
   /*===========================================================================*/
   /**
    * @file f360_detection_props_update.h
    *
    * @brief F360 detection properties update declarations
    *
    *------------------------------------------------------------------------------
    *
    * Copyright (C) 2026 Aptiv. All rights reserved.
    * Confidential - Restricted Aptiv information. Do not disclose
    *
    *------------------------------------------------------------------------------
    *
    * @section DESC DESCRIPTION:
    * Implementation of F360 detection properties update function which calculates refined
    * properties including VCS coordinates and range rate compensation.
    *
    * @section ABBR ABBREVIATIONS:
    *   - FOV: Field of View
    *
    * @section TRACE TRACEABILITY INFO:
    *   - Design Document(s):
    *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
    *       wiki/52-SoftwareArchitecture/CMP_SAD_Interface_Library_TrackerCore
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
    * @defgroup detection_props_update Detection_Properties_Update
    * @{
    */
   /*===========================================================================*/

   /*===========================================================================*
    * Standard Header Files
    *===========================================================================*/

   /*===========================================================================*
    * Other Header Files
    *===========================================================================*/
   #include "f360_constants.h"
   #include "f360_detection_list.h"
   #include "f360_detection_props_list.h"
   #include "f360_host.h"
   #include "f360_radar_sensor.h"
   #include "f360_radar_sensor_props.h"
   #include "f360_reuse.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace f360_variant_A
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
 * Name:  Detection_Props_Update
 *   This function updates detection properties
 *
 * Shared Variables: None
 *
 * Parameters:
 *   host              - Reference to the host structure
 *   sensors           - Array of radar sensors to update
 *   sensors_props     - Array of radar sensor properties to use for update
 *   detect_list       - Reference to the raw detection list
 *   detect_props_list - Reference to the detection properties list to be updated
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
void Detection_Props_Update(const F360_Host_T &host, const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
                            const F360_Radar_Sensor_Props_T (&sensors_props)[MAX_NUMBER_OF_SENSORS],
                            const F360_Detection_List_T &detect_list, F360_Detection_Props_List_T &detect_props_list);

} // namespace f360_variant_A

/** @} doxygen end group */
#endif // F360_DETECTION_PROPS_UPDATE_H

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

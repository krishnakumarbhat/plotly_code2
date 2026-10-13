#ifndef F360_MARK_DETECTION_WITH_LOW_DETECTION_CONF_H
   #define F360_MARK_DETECTION_WITH_LOW_DETECTION_CONF_H
   /*===========================================================================*/
   /**
    * @file f360_mark_detection_with_low_detection_conf.h
    *
    * @brief Detection Confidence Classification
    *
    *------------------------------------------------------------------------------
    *
    * Copyright (C) 2026 Aptiv. All rights reserved.
    * Confidential - Restricted Aptiv information. Do not disclose
    *
    *------------------------------------------------------------------------------
    *
    * @section DESC DESCRIPTION:
    * Functions to mark detections with low confidence based on sensor parameters
    * and detection properties for motion status classification.
    *
    * @section ABBR ABBREVIATIONS:
    *   - RSPP: Radar Signal Pre-Processing
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
    * @defgroup f360_mark_detection Mark Detection
    * @{
    */
   /*===========================================================================*/

   /*===========================================================================*
    * Standard Header Files
    *===========================================================================*/

   /*===========================================================================*
    * Other Header Files
    *===========================================================================*/
   #include "f360_detection.h"
   #include "f360_radar_sensor.h"

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
 * Name:  Mark_Detection_With_Low_Detection_Confidence
 *   This function evaluates detection confidence and marks detections with low
 *   confidence for azimuth error in stationary moving scenarios. Checks
 *   elevation limits, azimuth confidence, range rate compensation, and super
 *   resolution azimuth limits to determine low confidence.
 *
 * Shared Variables: None
 *
 * Parameters:
 *   sensor_type            - Type of radar sensor
 *   host_vcs_speed         - Host vehicle speed in VCS coordinates
 *   raw_detection          - Raw detection data to evaluate
 *   range_rate_compensated - Compensated range rate of the detection
 *
 * Return Value:
 *   true - If detection should be marked with azimuth error flag
 *   false - Otherwise
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
bool Mark_Detection_With_Low_Detection_Confidence(const F360_Sensor_Type_T sensor_type, const float32_t host_vcs_speed,
                                                  const F360_Detection_T &raw_detection, const float32_t range_rate_compensated);
} // namespace f360_variant_A

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

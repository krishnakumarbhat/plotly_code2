/*===========================================================================*\
* FILE: f360_filter_detections_with_bad_azimuth_confidence.cpp
*============================================================================
* Copyright (C) 2023 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "f360_filter_detections_with_bad_azimuth_confidence.h"
#include "f360_detection_multipath_filter.h"

namespace f360_variant_A
{

/*===========================================================================*\
   * FUNCTION: Filter_Detections_With_Bad_Azimuth_Confidence()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *   const F360_Host_T&host,
   *   const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]
   *   const F360_Calibrations_T &f360_calib
   *   const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list
   *   F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function filters detections that have bad azimuth confidence.
   *
   * PRECONDITIONS:
   * That detections are grouped by sensor ID in raw_detection_list
   * (detections from sensor 1 first, then sensor 2, etc.)
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Filter_Detections_With_Bad_Azimuth_Confidence(
      const F360_Host_T& host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& f360_calib,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      uint32_t current_det_idx = 0U;
      for (uint32_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
      {
         if (0U < sensors[sensor_idx].variable.number_of_valid_detections)
         {
            Filter_Out_Multipath_Detections(host, sensors[sensor_idx], f360_calib,
               current_det_idx, raw_detection_list.detections, detection_props);
            current_det_idx += sensors[sensor_idx].variable.number_of_valid_detections;
         }
      }
   }
}

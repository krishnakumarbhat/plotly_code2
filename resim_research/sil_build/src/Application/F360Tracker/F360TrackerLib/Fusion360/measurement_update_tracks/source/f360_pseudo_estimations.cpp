/*===================================================================================*\
* FILE: f360_pseudo_estimations.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    Contains function definition of Pseudo_Estimations()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "f360_pseudo_estimations.h"
#include "f360_pseudo_position_estimation.h"
#include "f360_pseudo_heading_estimation.h"
#include "f360_get_wall_time.h"

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Pseudo_Estimations()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calibrations                               - Calibration struct
   * const F360_Host_T& host                                                - Host vehicle structure
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]    - Detection properties
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]            - Sensor
   * const F360_Globals_T& globals                                          - Globals
   * F360_Object_Track_T & obj                                              - Reference to an object
   * F360_TRKR_TIMING_INFO_T & timing_info                                  - Timing info structure
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
   * Performs all pseudo estimations in Target Coordinate System after
   * that the object have been predicted in the current tracker cycle.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Pseudo_Estimations(
      const F360_Calibrations_T & calibrations,
      const F360_Host_T& host,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      F360_Object_Track_T& obj,
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      const float32_t start_time = get_wall_time();     

      // Run pseudo estimation functions
      if (obj.ndets > 0U)
      {
         Pseudo_Position_Estimation(calibrations, host, det_props, sensors, globals, obj);
      }
      else
      {
         // There are no detections so no estimate of reference point position is available
         obj.pseudo_vcs_position.x = obj.vcs_position.x;
         obj.pseudo_vcs_position.y = obj.vcs_position.y;
         obj.meascov[0][0] = INFTY;
         obj.meascov[0][1] = 0.0F;
         obj.meascov[1][1] = INFTY;
         obj.meascov[1][0] = 0.0F;
      }

      Pseudo_Heading_Estimation(det_props, obj);
      
      timing_info.pseudo_estimations += get_wall_time() - start_time;
   }
}

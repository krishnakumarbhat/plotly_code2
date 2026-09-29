#include "ocg_underdrivability.h"
/*===================================================================================*\
* FILE: ocg_underdrivability.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Underdrivability() function definition
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "cmn_math_func.h"
#include "cmn_utilities.h"
#include "ocg_underdrivability.h"
#include "ocg_initialize_underdrivability.h"
#include "ocg_shift_circular_zones.h"
#include "ocg_assign_detections_to_underdrivability_zones.h"
#include "ocg_calc_underdrivability_probabilities.h"
#include "ocg_assign_underdrivability_status_to_zones.h"

namespace ocg
{

   static void Set_Grid_Curvature(
       const RSPP_Host_T &host,
       OCG_Underdrivability_Internal_T &in_underdrivability);

   static void Derive_Time_Stamp_Information(
       const rspp_variant_A::F360_Radar_Sensor_T (&sensors)[rspp_variant_A::MAX_NUMBER_OF_SENSORS],
       OCG_Underdrivability_Internal_T &in_underdrivability,
       const OCG_Calibrations_T &calibrations);

   /*===========================================================================*\
   * FUNCTION: Underdrivability()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp_variant_A::F360_Detection_List_T &detection_list,
   * const rspp_variant_A::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
   * const rspp::RSPP_Host_T &host,
   * const OCG_Calibrations_T &calibrations,
   * OCG_Underdrivability_Internal_T &underdrivability)
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
   * Main function of underdrivability definition.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Underdrivability(
       const rspp_variant_A::RSPP_Detection_List_T &detection_list,
       const rspp_variant_A::F360_Radar_Sensor_T (&sensors)[rspp_variant_A::MAX_NUMBER_OF_SENSORS],
       const RSPP_Host_T &host,
       const OCG_Calibrations_T &calibrations,
       OCG_Underdrivability_Internal_T &in_underdrivability)
   {

      Derive_Time_Stamp_Information(sensors, in_underdrivability, calibrations);

      if (host.speed < 0.0F)
      {
         Initialize_Underdrivability(host, in_underdrivability);
      }
      else
      {
         Shift_Circular_Zones(in_underdrivability, host);
         if (host.speed >= calibrations.underdrive_slow_moving_host)
         {
            OCG_Zones_Innovation_T zones_innovation[NUM_CELLS_X]{};
            Set_Grid_Curvature(host, in_underdrivability);
            Assign_Detections_To_Underdrivability_Zones(in_underdrivability, zones_innovation, detection_list, calibrations, sensors);
            Calc_Underdrivability_Probabilities(in_underdrivability, zones_innovation, calibrations);
            Assign_Underdrivability_Status_To_Zones(in_underdrivability, host, calibrations);
         }
      }
   }

   static void Set_Grid_Curvature(
       const RSPP_Host_T &host,
       OCG_Underdrivability_Internal_T &in_underdrivability)
   {
      in_underdrivability.props.grid_curvature = host.curvature_rear;
   }

   static void Derive_Time_Stamp_Information(
       const rspp_variant_A::F360_Radar_Sensor_T (&sensors)[rspp_variant_A::MAX_NUMBER_OF_SENSORS],
       OCG_Underdrivability_Internal_T &in_underdrivability,
       const OCG_Calibrations_T &calibrations)
   {
      in_underdrivability.props.prev_timestamp_us = in_underdrivability.props.timestamp_us;
      for (uint8_t idx_sen = 0U; idx_sen < rspp_variant_A::MAX_NUMBER_OF_SENSORS; idx_sen++)
      {
         if (is_sensor_valid(sensors[idx_sen], calibrations))
         {
            in_underdrivability.props.timestamp_us = sensors[idx_sen].variable.timestamp_us;
         }
      }

      float elapsed_time = static_cast<float>((in_underdrivability.props.timestamp_us - in_underdrivability.props.prev_timestamp_us)) * 1e-6F;
      if ((elapsed_time > 0.15F))
      {
         elapsed_time = 0.05F;
      }
      in_underdrivability.props.timestamp_delta_s = elapsed_time;
   }
}

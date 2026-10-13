/*===================================================================================*\
* FILE: ocg_assign_detections_to_underdrivability_zones.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Assign_Detections_To_Underdrivability_Zones() function definition
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifdef _MSC_VER
   #pragma warning(push)
   #pragma warning(disable:5045)
#endif

#include "ocg_assign_detections_to_underdrivability_zones.h"
#include "cmn_math_func.h"
#include "ocg_assign_detections_to_underdrivability_zones_helpers.h"
#include "ocg_initialize_underdrivability.h"

namespace ocg
{
/*===========================================================================*\
* FUNCTION: Assign_Detections_To_Underdrivability_Zones()
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* const OCG_Underdrivability_Internal_T& in_underdrivability,
* OCG_Zones_Innovation_T(&zones_innovation)[NUM_CELLS_X],
* const OCG_Detection_List_T& detection_list,
* const OCG_Detection_Properties_T(&det_props)[OCG_NUM_DETS],
* const OCG_Calibrations_T& calib
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
* This function assigns detections that meet specific conditions to underdrivability zones.
*
* PRECONDITIONS:
*
* POSTCONDITIONS:
* None
*
\*===========================================================================*/
void Assign_Detections_To_Underdrivability_Zones(const OCG_Underdrivability_Internal_T &in_underdrivability,
                                                 OCG_Zones_Innovation_T (&zones_innovation)[NUM_CELLS_X],
                                                 const OCG_Detection_List_T &detection_list,
                                                 const OCG_Detection_Properties_T (&det_props)[OCG_NUM_DETS],
                                                 const OCG_Calibrations_T &calib)
{
   for (int16_t det_idx = 0; det_idx < detection_list.number_of_valid_detections; det_idx++)
   {
      const OCG_Detection_T &det                 = detection_list.detections[det_idx];
      const OCG_Detection_Properties_T &det_prop = det_props[det_idx];

      const float closest_dist_to_host_path =
         Calc_Lateral_Distance_To_Curved_Host_Path(in_underdrivability.props.grid_curvature, calib.underdrive_small_curvature_th,
                                                   det_prop.vcs_position_x, det_prop.vcs_position_y);
      (void)closest_dist_to_host_path; // MISRA

      const float max_lat_offset_from_host_curv =
         cmn::Linear_Equation_With_Saturation(det_prop.vcs_position_x, static_cast<float>(NUM_CELLS_X_CLOSE) * CELL_LENGTH,
                                              (static_cast<float>(NUM_CELLS_X_CLOSE + NUM_CELLS_X_MID)) * CELL_LENGTH,
                                              CELL_WIDTH / 2.0F, CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR / 2.0F);
      (void)max_lat_offset_from_host_curv; // MISRA

      bool filter = false;
      if (calib.underdrive_filter_duplicate_detections)
      {
         bool duplicate_filter = false;
         if (det_idx != 0)
         {
            duplicate_filter = ((det.azimuth == detection_list.detections[det_idx - 1].azimuth) &&
                                (det.range == detection_list.detections[det_idx - 1].range));
         }
         if (det_idx != (detection_list.number_of_valid_detections - 1))
         {
            duplicate_filter = duplicate_filter || ((det.azimuth == detection_list.detections[det_idx + 1].azimuth) &&
                                                    (det.range == detection_list.detections[det_idx + 1].range));
         }
         filter = duplicate_filter;
      }
      if (calib.underdrive_filter_negative_elevation)
      {
         filter = filter || (det.elevation < 0.0F);
      }

      if ((!filter) && (Is_Detection_Valid(det, det_prop, calib, closest_dist_to_host_path, max_lat_offset_from_host_curv)))
      {
         bool is_ground_detection = false;
         float det_vcs_elev       = 0.0F;
         float det_vcs_rng        = 0.0F;
         float det_vcs_height     = 0.0F;

         const float detection_lateral_weight =
            (1.0F - calib.underdrive_lateral_weight_factor * (closest_dist_to_host_path / max_lat_offset_from_host_curv));

         Calc_Det_Elevation_Params(det_prop, det_vcs_rng, det_vcs_height, det_vcs_elev);

         Compensate_Ground_Detection(calib, det_vcs_height, det_vcs_elev, is_ground_detection, det_vcs_rng);

         const uint32_t circ_buff_zone_idx = Calc_In_Which_Zone_Det_Is_Located(in_underdrivability, det_prop.vcs_position_x);
         Update_Zone_Innovation(zones_innovation[circ_buff_zone_idx], calib, det_vcs_rng, det_vcs_elev, det, is_ground_detection,
                                detection_lateral_weight);
      }
   }
}

} // namespace ocg

#ifdef _MSC_VER
   #pragma warning(pop)
#endif

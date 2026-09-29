/*===========================================================================*\
* FILE: f360_mark_host_water_spray_detections.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains functions for flagging detections that stems from water spray from host
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*============================================================================*/

#include "f360_mark_host_water_spray_detections.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Mark_Host_Water_Spray_Detections()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list
   * const F360_Host_T &host
   * const F360_Calibrations_T &calibs
   * F360_Detection_Props_T (&dets_props)[MAX_NUMBER_OF_DETECTIONS]
   *
   * EXTERNAL REFERENCES:
   * For more details, see DFD-395.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This functions flags detections if they are suspected to be waterspray from host's rear wheels
   * and marks them as not ok to use
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Mark_Host_Water_Spray_Detections(
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      const F360_Host_T &host,
      const F360_Calibrations_T &calibs,
      F360_Detection_Props_T (&dets_props)[MAX_NUMBER_OF_DETECTIONS])
   {

      // Don't check for water spray at low speeds
      if (calibs.k_ws_min_speed < host.speed)
      {
         const float32_t host_half_width = calibs.host_vehicle_width * 0.5F;

         // Calculate min and max values for the area where possible host water spray detections could be found
         // Larger area with higher host speed
         const float32_t vcs_x_min = -(host.speed * calibs.k_hws_para_box_host_speed_factor + host.dist_rear_axle_to_vcs_m);
         const float32_t vcs_x_max = -host.dist_rear_axle_to_vcs_m * 0.5F;
         const float32_t vcs_y = host.speed * calibs.k_hws_ortho_box_host_speed_factor + host_half_width;

         uint32_t num_dets_of_interest;
         int32_t det_idx_of_interest[MAX_NUMBER_OF_DETECTIONS] = {};

         Get_Det_Indexes_In_Vcs_Zone(
            vcs_x_min,
            vcs_x_max,
            -vcs_y,
            vcs_y,
            raw_detection_list,
            dets_props,
            det_idx_of_interest,
            num_dets_of_interest);

         // Loop over interesting detections
         for (uint32_t k = 0U; k < num_dets_of_interest; k++)
         {
            const int32_t det_idx = det_idx_of_interest[k];
            if (Is_Det_Valid_For_Host_Water_Spray(raw_detection_list.detections[det_idx], dets_props[det_idx], calibs, host))
            {
               dets_props[det_idx].f_water_spray = true;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Det_Valid_For_Host_Water_Spray()
   *===========================================================================
   * RETURN VALUE:
   * bool f_valid_det
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T &det
   * const F360_Detection_Props_T & det_p
   * const F360_Calibrations_T &calibs
   * const F360_Host_T &host
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
   * This function evaluates if a detection can be water spray based on the
   * detection properties. Returns true if the detection properties fulfills
   * the conditions as a suspicious water spray detection.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Is_Det_Valid_For_Host_Water_Spray(
      const rspp_variant_A::RSPP_Detection_T &det,
      const F360_Detection_Props_T &det_p,
      const F360_Calibrations_T &calibs,
      const F360_Host_T &host)
   {
      bool f_valid_det;

      if ((det_p.f_ok_to_use) &&
         (det.raw.rcs < calibs.k_ws_max_det_rcs) &&
         (0.0F < det.raw.range_rate) &&
         (host.speed > det.raw.range_rate))
      {
         f_valid_det = true;
      }
      else
      {
         f_valid_det = false;
      }

      return f_valid_det;
   }
}

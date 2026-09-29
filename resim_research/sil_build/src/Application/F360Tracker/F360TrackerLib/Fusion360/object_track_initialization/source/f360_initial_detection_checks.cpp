/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_initial_detection_checks.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Initial_Detection_Checks()
* 
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include "f360_initial_detection_checks.h"

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Initial_Detection_Checks()
   *===========================================================================
   * RETURN VALUE:
   * bool f_continue_init
   *
   * PARAMETERS:
   * const F360_Detection_Hist_T& det_hist,
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * const F360_Cluster_T& cluster)
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
   * This function iterates through all historical and current detections of a cluster and based on chekcs for
   * angle ambiguity, angle jump and low azimuth confidence determins if cluster is valid for initialization.
   * Returns f_continue_init flag that means that cluster is valid for further initialization.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   bool Initial_Detection_Checks(
      const F360_Detection_Hist_T& det_hist,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Cluster_T& cluster)
   {
      int16_t num_potential_angle_jump = 0;
      int16_t num_low_az_conf = 0;
      bool f_continue_init;
      bool f_any_angle_amb = false;

      /* Collect data */
      for (int16_t i = 0; i < cluster.ndets; i++)
      {
         const int16_t det_idx = cluster.detids[i] - 1;
         const F360_Detection_Props_T& det = det_props[det_idx];

         if (det.f_potential_angle_jump)
         {
            num_potential_angle_jump++;
         }
         if (raw_detections.detections[det_idx].raw.confid_azimuth >= rspp_variant_A::RSPP_CONF_AZIMUTH_LOW)
         {
            num_low_az_conf++;
         }
         f_any_angle_amb = (f_any_angle_amb || det_props[det_idx].f_angle_amb);
      }

      for (int16_t i = 0; i < cluster.num_old_dets; i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i];
         const F360_Detection_Hist_Data_T& det = det_hist.det_data[det_idx];

         if (det.f_potential_angle_jump)
         {
            num_potential_angle_jump++;
         }
         if (det.az_conf >= rspp_variant_A::RSPP_CONF_AZIMUTH_LOW)
         {
            num_low_az_conf++;
         }
      }

      /* Bail out of initialization early if there are too many bad detections */
      constexpr int16_t bad_det_bias_coeff = 2;
      const int16_t bad_det_thresh =((cluster.ndets + cluster.num_old_dets)) / 2 + bad_det_bias_coeff;
      if ((!f_any_angle_amb) &&
         (num_potential_angle_jump <= bad_det_thresh) &&
         (num_low_az_conf <= bad_det_thresh))
      {
         f_continue_init = true;
      }
      else
      {
         f_continue_init = false;
      }

      return f_continue_init;
   }
}

/*===================================================================================*\
* FILE: dc_features.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file implements Features_T class related to DC FeaturesCalculator calculation.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef DC_FEATURES_H
#define DC_FEATURES_H

namespace sg
{
   namespace dc
   {
      struct Features_T
      {
         float detections_number;      // number of valid detections up to current time
         float rcs_recur_mean;         // recursive mean of radar cross section values
         float z_scs_abs_ewma025_mean; // exponentially weighted mean of absolute values of all detection z coordinates in sensor
                                       // coordinate system (with factor 0.25)
         float z_scs_abs_recur_mean; // recursive mean of absolute values of all detection z coordinates in sensor coordinate system
         float z_scs_abs_max;        // the biggest absolute value of all detection z coordinates in sensor coordnate system
         float height_under_nondr_bins_proportion; // feature based on recursive means of frequencies of detections falling into
                                                   // two different height ranges: underdrivable and nondrivable
         float height_over_nondr_bins_proportion; // feature based on recursive means of frequencies of detections falling into two
                                                  // different height ranges: overdrivable and nondrivable
         float detection_density;                 // number of detections divided by subsegment age
      };
   }
}
#endif
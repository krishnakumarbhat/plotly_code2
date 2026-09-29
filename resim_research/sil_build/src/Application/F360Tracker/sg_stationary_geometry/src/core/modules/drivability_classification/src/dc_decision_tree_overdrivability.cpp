/*===================================================================================*\
* FILE: dc_decision_tree_overdrivability.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*  This file contains implementation of the DC_Decision_Tree_Overdrivability class.
*
*  Applicable Standards (in order of precedence: highest first):
*    ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*    ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_decision_tree_overdrivability.h"

namespace sg
{
   namespace dc
   {
      float DC_Decision_Tree_Overdrivability::predict_obstacle_probability_overdrivable(const Features_T &features)
      {
         float probability_of_obstacle = 0.0F;
         if ((features.height_over_nondr_bins_proportion >= 0.38F) || (features.rcs_recur_mean <= -20.0F)
             || (features.detection_density <= 0.5))
         {
            if ((features.rcs_recur_mean <= -20.0F)
                || ((features.detections_number < 20.0F) && (features.height_over_nondr_bins_proportion > 0.6))
                || (features.height_over_nondr_bins_proportion > 0.85F))
            {
               probability_of_obstacle = 0.48F;
            }
            else if ((features.height_over_nondr_bins_proportion >= 0.38F) && (features.detection_density >= 2.0F)) // front of a
                                                                                                                    // car
            {
               probability_of_obstacle = 0.9F;
            }
            else
            {
               probability_of_obstacle = 0.48F;
            }
         }
         else
         {
            probability_of_obstacle = 0.9F;
         }
         return probability_of_obstacle;
      }
   }
}
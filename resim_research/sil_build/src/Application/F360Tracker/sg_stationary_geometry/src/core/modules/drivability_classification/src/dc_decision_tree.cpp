/*===================================================================================*\
* FILE: dc_decision_tree.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*  This file contains implementation of the DC_Decision_Tree class.
*
*  Applicable Standards (in order of precedence: highest first):
*    ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*    ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_decision_tree.h"

namespace sg
{
   namespace dc
   {
      float DC_Decision_Tree::predict_obstacle_prob(const Features_T &features)
      {
         float probability_of_obstacle = 0.0F;
         if (features.height_under_nondr_bins_proportion <= 0.25821212F)
         {
            if (features.z_scs_abs_recur_mean <= 4.8424253F)
            {
               if (features.z_scs_abs_recur_mean <= 4.2944326F)
               {
                  if (features.z_scs_abs_max <= 8.194763F)
                  {
                     probability_of_obstacle = 0.65643543F;
                  }
                  else
                  { // if z_scs_abs_max > 8.194763
                     probability_of_obstacle = 0.9295965F;
                  }
               }
               else
               { // if z_scs_abs_recur_mean > 4.2944326
                  if (features.z_scs_abs_max <= 10.5651F)
                  {
                     probability_of_obstacle = 0.33087388F;
                  }
                  else
                  { // if z_scs_abs_max > 10.5651
                     probability_of_obstacle = 0.6837436F;
                  }
               }
            }
            else
            { // if z_scs_abs_recur_mean > 4.8424253
               if (features.z_scs_abs_ewma025_mean <= 4.8080444F)
               {
                  if (features.height_under_nondr_bins_proportion <= 0.18660378F)
                  {
                     probability_of_obstacle = 0.14328523F;
                  }
                  else
                  { // if hist_density_feature_3 > 0.18660378
                     probability_of_obstacle = 0.53896546F;
                  }
               }
               else
               { // if z_scs_abs_ewma025_mean > 4.8080444
                  if (features.z_scs_abs_max <= 28.9371F)
                  {
                     probability_of_obstacle = 0.033073492F;
                  }
                  else
                  { // if z_scs_abs_max > 28.9371
                     probability_of_obstacle = 0.29226997F;
                  }
               }
            }
         }
         else
         { // if hist_density_feature_3 > 0.25821212
            if (features.z_scs_abs_ewma025_mean <= 4.7083735F)
            {
               if (features.height_under_nondr_bins_proportion <= 0.37500006F)
               {
                  if (features.z_scs_abs_max <= 9.493257F)
                  {
                     probability_of_obstacle = 0.7722333F;
                  }
                  else
                  { // if z_scs_abs_max > 9.493257
                     probability_of_obstacle = 0.9201702F;
                  }
               }
               else
               { // if hist_density_feature_3 > 0.37500006
                  if (features.height_under_nondr_bins_proportion <= 0.4736842F)
                  {
                     probability_of_obstacle = 0.9559169F;
                  }
                  else
                  { // if hist_density_feature_3 > 0.4736842
                     probability_of_obstacle = 0.9963938F;
                  }
               }
            }
            else
            { // if z_scs_abs_ewma025_mean > 4.7083735
               if (features.z_scs_abs_recur_mean <= 5.313952F)
               {
                  if (features.height_under_nondr_bins_proportion <= 0.28571433F)
                  {
                     probability_of_obstacle = 0.497114F;
                  }
                  else
                  { // if hist_density_feature_3 > 0.28571433
                     probability_of_obstacle = 0.90808344F;
                  }
               }
               else
               { // if z_scs_abs_recur_mean > 5.313952
                  if (features.z_scs_abs_ewma025_mean <= 5.2519846F)
                  {
                     probability_of_obstacle = 0.6262362F;
                  }
                  else
                  { // if z_scs_abs_ewma025_mean > 5.2519846
                     probability_of_obstacle = 0.20208415F;
                  }
               }
            }
         }
         return probability_of_obstacle;
      }
   }
}
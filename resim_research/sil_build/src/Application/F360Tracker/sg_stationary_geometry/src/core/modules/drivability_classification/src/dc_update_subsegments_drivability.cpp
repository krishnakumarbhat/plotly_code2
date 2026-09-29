/*===================================================================================*\
* FILE: dc_update_subsegments_drivability.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*  This file contains update_subsegments_drivability function implementation.
*
*  Applicable Standards (in order of precedence: highest first):
*    ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*    ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_update_subsegments_drivability.h"

namespace sg
{
   namespace dc
   {
      float DC_Update_Subsegments_Drivability::calculate_drivability_confidence(const float probability_of_obstacle,
                                                                                const float obstacle_probability_threshold,
                                                                                const float probability_of_obstacle_overdrivability,
                                                                                const float overdrivability_obstacle_probability_threshold,
                                                                                const float classification_uncertainty)
      {
         float confidence                                   = 0.0F;
         const float probability_of_obstacle_over_threshold = probability_of_obstacle - obstacle_probability_threshold;

         if (std::fabs(probability_of_obstacle_over_threshold) < classification_uncertainty)
         {
            confidence = 50.0F;
         }
         else if (probability_of_obstacle_over_threshold < 0.0F)
         {
            confidence = 75.0F + 25.0F * (-probability_of_obstacle_over_threshold) / obstacle_probability_threshold;
         }
         else
         {
            const float overdrivability_probability_of_obstacle_over_threshold =
               probability_of_obstacle_overdrivability - overdrivability_obstacle_probability_threshold;
            if (std::fabs(overdrivability_probability_of_obstacle_over_threshold) < classification_uncertainty)
            {
               confidence = 50.0F;
            }
            else if (overdrivability_probability_of_obstacle_over_threshold < 0.0F)
            {
               confidence = 75.0F
                            + 25.0F * (-overdrivability_probability_of_obstacle_over_threshold)
                                 / overdrivability_obstacle_probability_threshold;
            }
            else
            {
               confidence =
                  75.0F
                  + 25.0F
                       * sqrtf(
                          std::min(probability_of_obstacle_over_threshold, overdrivability_probability_of_obstacle_over_threshold)
                          / std::max(probability_of_obstacle_over_threshold, overdrivability_probability_of_obstacle_over_threshold));
            }
         }
         return confidence;
      }

      void DC_Update_Subsegments_Drivability::classify_subsegment(Subsegment_T &subsegment,
                                                                  const Drivability_Classification_Calibrations_T &cfg)
      {
         if (subsegment.features.detections_number < cfg.min_num_dets_for_classification)
         {
            subsegment.drivability            = SG_Drivability_Class_T::UNCLASSIFIED;
            subsegment.drivability_confidence = 25.0F;
         }
         else
         {
            const float classification_uncertainty        = 0.01F;
            const float probability_of_obstacle           = DC_Decision_Tree::predict_obstacle_prob(subsegment.features);
            float probability_of_obstacle_overdrivability = 0.0F;

            // in (0.49, 0.51) - unsure
            if (std::fabs(probability_of_obstacle - cfg.decision_tree_obstacle_prob_thr) < classification_uncertainty)
            {
               subsegment.drivability = SG_Drivability_Class_T::UNCLASSIFIED;
            }
            else if (probability_of_obstacle < cfg.decision_tree_obstacle_prob_thr)
            {
               subsegment.drivability = SG_Drivability_Class_T::UNDERDRIVABLE;
            }
            else
            {
               probability_of_obstacle_overdrivability =
                  DC_Decision_Tree_Overdrivability::predict_obstacle_probability_overdrivable(subsegment.features);
               if (std::fabs(probability_of_obstacle_overdrivability - cfg.decision_tree_overdrivability_obstacle_prob_thr)
                   < classification_uncertainty)
               {
                  subsegment.drivability = SG_Drivability_Class_T::UNCLASSIFIED;
               }
               else if (probability_of_obstacle_overdrivability < cfg.decision_tree_overdrivability_obstacle_prob_thr)
               {
                  subsegment.drivability = SG_Drivability_Class_T::OVERDRIVABLE;
               }
               else
               {
                  subsegment.drivability = SG_Drivability_Class_T::NONDRIVABLE;
               }
            }
            subsegment.drivability_confidence = calculate_drivability_confidence(
               probability_of_obstacle, cfg.decision_tree_obstacle_prob_thr, probability_of_obstacle_overdrivability,
               cfg.decision_tree_overdrivability_obstacle_prob_thr, classification_uncertainty);
         }
      }

      void DC_Update_Subsegments_Drivability::update_subsegments_drivability(DCContourStorage &contour_list,
                                                                             const Drivability_Classification_Calibrations_T &cfg)
      {
         for (auto &contour : contour_list)
         {
            for (auto &subsegment : contour.subsegments)
            {
               if (subsegment.begin_vertex.f_critical && subsegment.end_vertex.f_critical)
               {
                  classify_subsegment(subsegment, cfg);
               }
            }
         }
      }
   }
}
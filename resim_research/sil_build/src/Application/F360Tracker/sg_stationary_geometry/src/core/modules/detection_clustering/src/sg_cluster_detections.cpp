/*=============================================================================================*\
* FILE: sg_cluster_detections.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for cluster_detections.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_cluster_detections.h"

#include <limits>

#include "sg_cluster_detections_helpers.h"
#include "sg_temporal_dbscan.h"
#include "sg_update_detections_properties.h"

namespace sg
{
   void cluster_detections(DetectionStorage &detections, const Cluster_Detections_Calibrations_T &calibrations)
   {
      for (uint8_t i_drivability = 0U; i_drivability < static_cast<uint8_t>(SG_Drivability_Class_T::COUNT); ++i_drivability)
      {
         const SG_Drivability_Class_T drivability_class = static_cast<SG_Drivability_Class_T>(i_drivability);
         const bool f_any_detection_to_cluster          = mark_detections_with_drivability_class(detections, drivability_class);
         auto current_cluster_radius                    = calibrations.cluster_radius_nondrivable;

         if (f_any_detection_to_cluster)
         {
            uint8_t current_min_cluster_points = calibrations.min_cluster_points_nondrivable;
            if (drivability_class == SG_Drivability_Class_T::UNDERDRIVABLE)
            {
               current_min_cluster_points = calibrations.min_cluster_points_underdrivable;
               current_cluster_radius     = calibrations.cluster_radius_underdrivable;
            }

            temporal_dbscan(detections, current_cluster_radius, calibrations.historical_num_neighbors_forgetting_factor,
                            current_min_cluster_points);

            update_detections_properties(detections, calibrations.existence_probability);
         }
      }
      detections.update_cluster_ages();
   }

   bool mark_detections_with_drivability_class(const DetectionStorage &detections, const SG_Drivability_Class_T &drivability_class)
   {
      bool any_subset = false;
      for (auto &detection : detections)
      {
         if (detection.drivability == drivability_class)
         {
            detection.f_subset = true;
            any_subset         = true;
         }
         else
         {
            detection.f_subset = false;
         }
      }
      return any_subset;
   }

   std::tuple<float, float> get_relevant_x_position_interval(const Detection_T &current_det, const float cluster_radius)
   {
      float inverse_squeezing_factor = 1.0F;
      if ((std::abs(current_det.position.x) > std::abs(current_det.position_squeezed.x))
          && (std::abs(current_det.position_squeezed.x) >= std::numeric_limits<float>::epsilon()))
      {
         inverse_squeezing_factor = std::abs(current_det.position.x / current_det.position_squeezed.x);
      }
      const float inverse_squeezed_cluster_radius = cluster_radius * inverse_squeezing_factor;

      return std::make_tuple((current_det.position.x - inverse_squeezed_cluster_radius),
                             (current_det.position.x + inverse_squeezed_cluster_radius));
   }
}

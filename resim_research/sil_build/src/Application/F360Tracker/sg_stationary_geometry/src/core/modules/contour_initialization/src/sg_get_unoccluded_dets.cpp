/*=============================================================================================*\
* FILE: sg_get_unoccluded_dets.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for get_unoccluded_dets and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#include "sg_get_unoccluded_dets.h"

#include <algorithm>

#include "sg_math.h"

namespace sg
{
   std::size_t get_unoccluded_dets(const Cluster *const current_cluster_ptr,
                                   const ContourStorage &contours,
                                   const Contour_Initialization_Calibrations_T &calibrations,
                                   const std::bitset<SG_MAX_NUM_CONTOURS> &contours_mask,
                                   const float azimuth_epsilon)
   {
      float dets_azimuth[SG_MAX_NUM_INTERNAL_DETS]{};
      float dets_range[SG_MAX_NUM_INTERNAL_DETS]{};

      calc_detections_azimuth_and_range(dets_azimuth, dets_range, current_cluster_ptr);

      std::bitset<SG_MAX_NUM_INTERNAL_DETS> dets_azimuth_within_limits_mask{};
      std::bitset<SG_MAX_NUM_INTERNAL_DETS> temp_dets_azimuth_within_limits_mask{};
      std::bitset<SG_MAX_NUM_INTERNAL_DETS> dets_range_within_limits_mask{};
      std::bitset<SG_MAX_NUM_INTERNAL_DETS> temp_dets_range_within_limits_mask{};


      auto contour_it = contours.begin();

      for (auto i = 0U; i < contours.size(); ++i)
      {
         if (contours_mask[i])
         {
            float vertices_azimuth[SG_MAX_NUM_VERTICES]{};
            float vertices_range[SG_MAX_NUM_VERTICES]{};

            calc_vertices_azimuth_and_range(contour_it->vertices, vertices_azimuth, vertices_range);

            const float delta_azimuth = DEG2RAD(calibrations.init_azimuth_margin) + azimuth_epsilon;

            // filter dets which are occluded by each of segments
            for (auto j = 0U; j < contour_it->vertices.size() - 1U; ++j)
            {
               // get segment boundary values
               // min value is in pair.first, max value in pair.second
               const std::pair<float, float> azimuth_limits = std::minmax(vertices_azimuth[j], vertices_azimuth[j + 1U]);

               const float range_limit = std::min(vertices_range[j], vertices_range[j + 1U]);

               dets_azimuth_within_limit(temp_dets_azimuth_within_limits_mask, current_cluster_ptr, dets_azimuth, azimuth_limits,
                                         delta_azimuth);
               dets_azimuth_within_limits_mask = dets_azimuth_within_limits_mask | temp_dets_azimuth_within_limits_mask;

               dets_range_within_limit(current_cluster_ptr, temp_dets_range_within_limits_mask, dets_range, range_limit,
                                       calibrations.range_margin);
               dets_range_within_limits_mask = dets_range_within_limits_mask | temp_dets_range_within_limits_mask;
            }
            (void) delta_azimuth; // MISRA
         }
         ++contour_it;
      }

      // get unoccluded detections, i.e.complement of occluded detections
      std::size_t i                      = 0U;
      std::size_t num_of_unoccluded_dets = 0U;
      const auto current_cluster_end     = current_cluster_ptr->end();
      for (auto det_it = current_cluster_ptr->begin(); det_it != current_cluster_end; ++det_it)
      {
         if ((det_it->contour_id == INVALID_CONTOUR_ID) && (!(dets_azimuth_within_limits_mask[i] && dets_range_within_limits_mask[i])))
         {
            det_it->f_subset = true;
            ++num_of_unoccluded_dets;
         }
         else
         {
            det_it->f_subset = false;
         }
         ++i;
      }
      (void) i;                               // MISRA
      (void) dets_azimuth_within_limits_mask; // MISRA
      (void) dets_range_within_limits_mask;   // MISRA
      return num_of_unoccluded_dets;
   }


   void calc_detections_azimuth_and_range(float (&dets_azimuth)[SG_MAX_NUM_INTERNAL_DETS],
                                          float (&dets_range)[SG_MAX_NUM_INTERNAL_DETS],
                                          const Cluster *const current_cluster_ptr)
   {
      const auto current_cluster_end = current_cluster_ptr->end();
      std::size_t i                  = 0U;
      for (auto det_it = current_cluster_ptr->begin(); det_it != current_cluster_end; ++det_it)
      {
         if (det_it->contour_id == INVALID_CONTOUR_ID)
         {
            dets_azimuth[i] = calculate_azimuth(det_it->position.x, det_it->position.y);
            dets_range[i]   = calculate_point_distance(det_it->position.x, det_it->position.y);
         }
         ++i;
      }
      (void) i; // MISRA
   }


   void calc_vertices_azimuth_and_range(const Contour_T::VertexList &vertices,
                                        float (&vertices_azimuth)[SG_MAX_NUM_VERTICES],
                                        float (&vertices_range)[SG_MAX_NUM_VERTICES])
   {
      assert(static_cast<uint16_t>(vertices.capacity()) == SG_MAX_NUM_VERTICES); // vertices_azimuth and vertices_range must be the
                                                                                 // same size as VertexList
      std::size_t idx = 0U;
      for (const auto &vtx : vertices)
      {
         if (idx < SG_MAX_NUM_VERTICES) // MISRA ("Guarantee that container indices and iterators are within the valid range.")
         {
            vertices_azimuth[idx] = calculate_azimuth(vtx.position.x, vtx.position.y);
            vertices_range[idx]   = calculate_point_distance(vtx.position.x, vtx.position.y);
            idx++;
         }
         (void) vtx; // MISRA
      }
      (void) idx; // MISRA
   }


   void dets_azimuth_within_limit(std::bitset<SG_MAX_NUM_INTERNAL_DETS> &dets_azimuth_within_limits_mask,
                                  const Cluster *const current_cluster_ptr,
                                  const float (&dets_azimuth)[SG_MAX_NUM_INTERNAL_DETS],
                                  const std::pair<float, float> azimuth_limits,
                                  const float azimuth_margin_threshold)
   {
      // get dets that are within segments vertices azimuths
      const auto current_cluster_end = current_cluster_ptr->end();
      if ((azimuth_limits.second - azimuth_limits.first) < PI)
      {
         const float min_azimuth_threshold_low  = azimuth_limits.first - azimuth_margin_threshold;
         const float max_azimuth_threshold_high = azimuth_limits.second + azimuth_margin_threshold;
         std::size_t i                          = 0U;
         for (auto det_it = current_cluster_ptr->begin(); det_it != current_cluster_end; ++det_it)
         {
            dets_azimuth_within_limits_mask[i] =
               (det_it->contour_id == INVALID_CONTOUR_ID)
               && ((min_azimuth_threshold_low < dets_azimuth[i]) && (dets_azimuth[i] < max_azimuth_threshold_high));
            ++i;
         }
         (void) min_azimuth_threshold_low;  // MISRA
         (void) max_azimuth_threshold_high; // MISRA
         (void) i;                          // MISRA
      }
      else
      {
         const float max_azimuth_threshold_low  = azimuth_limits.second - azimuth_margin_threshold;
         const float min_azimuth_threshold_high = azimuth_limits.first + azimuth_margin_threshold;
         std::size_t i                          = 0U;
         for (auto det_it = current_cluster_ptr->begin(); det_it != current_cluster_end; ++det_it)
         {
            dets_azimuth_within_limits_mask[i] =
               (det_it->contour_id == INVALID_CONTOUR_ID)
               && ((max_azimuth_threshold_low < dets_azimuth[i]) || (dets_azimuth[i] < min_azimuth_threshold_high));
            ++i;
         }
         (void) max_azimuth_threshold_low;  // MISRA
         (void) min_azimuth_threshold_high; // MISRA
         (void) i;                          // MISRA
      }
   }


   void dets_range_within_limit(const Cluster *const current_cluster_ptr,
                                std::bitset<SG_MAX_NUM_INTERNAL_DETS> &dets_range_within_limits_mask,
                                const float (&dets_range)[SG_MAX_NUM_INTERNAL_DETS],
                                const float range_limit,
                                const float range_margin)
   {
      const auto range_difference    = range_limit - range_margin;
      std::size_t i                  = 0U;
      const auto current_cluster_end = current_cluster_ptr->end();
      for (auto det_it = current_cluster_ptr->begin(); det_it != current_cluster_end; ++det_it)
      {
         if ((det_it->contour_id == INVALID_CONTOUR_ID) && (range_difference < dets_range[i]))
         {
            dets_range_within_limits_mask[i] = true;
         }
         ++i;
      }
      (void) range_difference; // MISRA
      (void) i;                // MISRA
   }
}

/*===================================================================================*\
* FILE: dc_consolidate_subsegment_drivability.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains implementation of consolidate_subsegment_drivability function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_consolidate_subsegment_drivability.h"

#include <bitset>
#include <numeric>

#include "dc_array_wrapper.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      static void append_fused_vertex_data(Fused_Contour_T::FusedVertexList &vertices,
                                           uint16_t &num_of_vertices,
                                           const geometry::Point2D_T &position,
                                           const SG_Drivability_Class_T drivability,
                                           const float drivability_confidence,
                                           const uint16_t sg_age,
                                           const uint16_t sg_cycles_since_coasted);

      void consolidate_subsegment_drivability(FusedContourStorage &fused_contour_storage,
                                              const ContourStorage &sg_contour_storage,
                                              const DCContourStorage &dc_contour_storage)
      {
         uint16_t num_all_vertices = 0U;
         (void) num_all_vertices; // MISRA

         for (const auto &dc_contour : dc_contour_storage)
         {
            const auto &subsegments = dc_contour.subsegments;
            Fused_Contour_T::FusedVertexList vertices;
            uint16_t num_vertices = 0U;
            (void) num_vertices; // MISRA

            const auto fused_contour_id = dc_contour.get_id();
            const auto sg_contour_it    = std::find_if(sg_contour_storage.begin(), sg_contour_storage.end(),
                                                       [&](const Contour_T &sg_contour)
                                                       { return sg_contour.unique_id() == fused_contour_id; });

            for (auto subsegment_it = subsegments.begin(), next_subsegment_it = subsegment_it; subsegment_it != subsegments.end();
                 subsegment_it++)
            {
               if (subsegment_it == next_subsegment_it)
               {
                  next_subsegment_it =
                     std::find_if(std::next(subsegment_it), subsegments.end(),
                                  [&](const Subsegment_T &subsegment)
                                  {
                                     return subsegment.begin_vertex.f_primary
                                            || (subsegment_it->drivability != subsegment.drivability)
                                            || (subsegment.begin_vertex.f_critical != subsegment.end_vertex.f_critical);
                                  });

                  auto first_element_to_sum_it = subsegment_it;
                  if (!subsegment_it->begin_vertex.f_critical && (subsegment_it == subsegments.begin()))
                  {
                     first_element_to_sum_it = std::next(subsegment_it); // when first vertex is not critical, drivability of that
                                                                         // vertex should not be calculated
                  }

                  constexpr auto initial_confidence = 0.0F;

                  const auto accumulated_confidence = std::accumulate(first_element_to_sum_it, next_subsegment_it, initial_confidence,
                                                                      [&](const float confidence, const Subsegment_T &subsegment)
                                                                      { return confidence + subsegment.drivability_confidence; });

                  const auto num_processed = static_cast<uint16_t>(std::distance(first_element_to_sum_it, next_subsegment_it));
                  (void) num_processed; // MISRA

                  const bool f_is_next_subsegment_last = (next_subsegment_it == subsegments.end());

                  const bool f_is_next_drivability_same =
                     f_is_next_subsegment_last ? true : (subsegment_it->drivability == next_subsegment_it->drivability);

                  const bool f_is_criticality_the_same =
                     f_is_next_subsegment_last
                        ? (subsegment_it->begin_vertex.f_critical == subsegments.rbegin()->end_vertex.f_critical)
                        : (subsegment_it->begin_vertex.f_critical == next_subsegment_it->end_vertex.f_critical);

                  if (f_is_next_subsegment_last || (!f_is_next_drivability_same) || (!f_is_criticality_the_same)
                      || next_subsegment_it->begin_vertex.f_primary)
                  {
                     // default values if we are out of critical region
                     float fused_segment_confidence                       = SG_DEFAULT_DRIVABILITY_CONFIDENCE;
                     sg::SG_Drivability_Class_T fused_segment_drivability = sg_contour_it->drivability;

                     if (subsegment_it->is_critical())
                     {
                        fused_segment_confidence  = subsegment_it->drivability_confidence;
                        fused_segment_drivability = subsegment_it->drivability;
                        if (num_processed > 0U)
                        {
                           fused_segment_confidence = accumulated_confidence / static_cast<float>(num_processed);
                        }
                     }
                     append_fused_vertex_data(vertices, num_vertices, subsegment_it->begin_vertex.position,
                                              fused_segment_drivability, fused_segment_confidence, subsegment_it->sg_age,
                                              subsegment_it->sg_cycles_since_coasted);
                  }

                  if (f_is_next_subsegment_last)
                  {
                     const auto last_it = subsegments.rbegin();
                     append_fused_vertex_data(vertices, num_vertices, last_it->end_vertex.position,
                                              SG_Drivability_Class_T::UNCLASSIFIED, 0.0F, 0U, 0U);
                  }
               }
            }

            if (num_vertices > SG_MAX_NUM_FUSED_VERTICES_PER_CONTOUR)
            {
               continue;
            }
            if (num_all_vertices + num_vertices > SG_MAX_NUM_FUSED_VERTICES)
            {
               continue;
            }

            num_all_vertices += num_vertices;
            if (num_vertices > 0U)
            {
               const auto selected_for_output = sg_contour_it->f_selected_for_output;
               Fused_Contour_T fused_contour(vertices, fused_contour_id, num_vertices, selected_for_output,
                                             sg_contour_it->drivability);
               (void) fused_contour_storage.push_back(std::move(fused_contour));
            }
         }
      }

      static void append_fused_vertex_data(Fused_Contour_T::FusedVertexList &vertices,
                                           uint16_t &num_of_vertices,
                                           const geometry::Point2D_T &position,
                                           const SG_Drivability_Class_T drivability,
                                           const float drivability_confidence,
                                           const uint16_t sg_age,
                                           const uint16_t sg_cycles_since_coasted)
      {
         Fused_Vertex_T sg_dc_vertex;
         sg_dc_vertex.position                = position;
         sg_dc_vertex.drivability             = drivability;
         sg_dc_vertex.drivability_confidence  = drivability_confidence;
         sg_dc_vertex.sg_age                  = sg_age;
         sg_dc_vertex.sg_cycles_since_coasted = sg_cycles_since_coasted;
         (void) vertices.push_back(sg_dc_vertex);
         num_of_vertices++;
      }
   }
}

/*===================================================================================*\
* FILE: dc_append_remaining_sg_contours.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file implements dc_append_remaining_sg_contours function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_append_remaining_sg_contours.h"

#include <numeric>

namespace sg
{
   namespace dc
   {
      void append_remaining_sg_contours(FusedContourStorage &fused_contour_storage, const ContourStorage &contour_list)
      {
         uint32_t number_of_fused_contours = fused_contour_storage.size();
         (void) number_of_fused_contours; // MISRA

         auto number_of_fused_verticies = std::accumulate(fused_contour_storage.begin(), fused_contour_storage.end(), 0U,
                                                          [](const uint32_t num_of_vertices, const Fused_Contour_T &fused_contour)
                                                          { return num_of_vertices + fused_contour.num_of_vertices; });
         (void) number_of_fused_verticies; // MISRA

         ArrayWrapper<uint32_t, SG_MAX_NUM_FUSED_CONTOURS> fused_contour_ids;
         get_fused_contour_ids(fused_contour_ids, fused_contour_storage);

         for (const auto &sg_contour : contour_list)
         {
            const auto number_of_fused_verticies_after_addition = number_of_fused_verticies + sg_contour.vertices.size();
            if (sg_contour_can_be_added(sg_contour, fused_contour_ids, number_of_fused_contours,
                                        number_of_fused_verticies_after_addition))
            {
               append_fused_contour(fused_contour_storage, sg_contour);
               number_of_fused_contours++;
               number_of_fused_verticies = number_of_fused_verticies_after_addition;
            }
         }
      }

      bool sg_contour_can_be_added(const Contour_T &sg_contour,
                                   const ArrayWrapper<uint32_t, SG_MAX_NUM_FUSED_CONTOURS> &fused_contour_ids,
                                   const uint32_t number_of_fused_contours,
                                   const uint32_t number_of_fused_verticies_after_addition)
      {
         return (number_of_fused_contours < SG_MAX_NUM_FUSED_CONTOURS)
                && (number_of_fused_verticies_after_addition <= SG_MAX_NUM_FUSED_VERTICES)
                && !std::binary_search(fused_contour_ids.cbegin(), fused_contour_ids.cend(), sg_contour.unique_id());
      }

      void append_fused_contour(FusedContourStorage &fused_contour_storage, const Contour_T &sg_contour)
      {
         uint16_t num_of_vertices = 0U;

         Fused_Contour_T::FusedVertexList fused_vertices;
         for (const auto &vertex : sg_contour.vertices)
         {
            Fused_Vertex_T fused_vertex;

            fused_vertex.position               = vertex.position;
            fused_vertex.pos_cross_cov          = vertex.pos_cross_cov;
            fused_vertex.pos_cov                = vertex.pos_cov;
            fused_vertex.drivability_confidence = SG_DEFAULT_DRIVABILITY_CONFIDENCE;

            fused_vertex.sg_age                  = vertex.age;
            fused_vertex.sg_cycles_since_coasted = vertex.num_cycles_no_update;

            fused_vertex.drivability = sg_contour.drivability;

            (void) fused_vertices.push_back(fused_vertex);

            num_of_vertices++;
         }

         auto fused_contour = Fused_Contour_T{fused_vertices, sg_contour.unique_id(), num_of_vertices, false, sg_contour.drivability};
         (void) fused_contour_storage.push_back(std::move(fused_contour));
      }

      void get_fused_contour_ids(ArrayWrapper<uint32_t, SG_MAX_NUM_FUSED_CONTOURS> &fused_contour_ids,
                                 const FusedContourStorage &fused_contour_storage)
      {
         for (const auto &fused_contour : fused_contour_storage)
         {
            fused_contour_ids.add_element(fused_contour.get_id());
         }
         std::sort(fused_contour_ids.begin(), fused_contour_ids.end());
      }
   }
}

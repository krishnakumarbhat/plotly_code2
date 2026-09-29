/*=============================================================================================*\
* FILE: sg_try_initialize_contour.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for try_initialize_contour and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_try_initialize_contour.h"

#include "sg_init_contour_bbox_stitch.h"
#include "sg_initialize_contours_helpers.h"

namespace sg
{
   std::pair<InitializationResult, Contour_T>
   try_initialize_contour(ContourStorage &contours,
                          const Cluster *const current_cluster_ptr,
                          const Contour_Initialization_Calibrations_T &contour_initialization_calibrations,
                          const Common_Calibrations_T &common_calibrations)
   {
      std::pair<InitializationResult, Contour_T> contour{};

      const bool any_det_f_subset = collect_dets_for_a_new_contour(contours, current_cluster_ptr, contour_initialization_calibrations,
                                                                   common_calibrations.azimuth_epsilon);

      if (any_det_f_subset)
      {
         Contour_T::VertexList vertices{};
         const SG_Drivability_Class_T cluster_drivability = current_cluster_ptr->begin()->drivability;

         const float look_distance = (cluster_drivability == SG_Drivability_Class_T::UNDERDRIVABLE)
                                        ? contour_initialization_calibrations.init_look_distance_underdrivable
                                        : contour_initialization_calibrations.init_look_distance;

         init_bbox_stitch(vertices, current_cluster_ptr, look_distance, common_calibrations.min_segment_length,
                          common_calibrations.host_position);

         const uint16_t num_free_slots_for_vertices = SG_MAX_NUM_VERTICES - static_cast<uint16_t>(contours.total_vertices_number());

         contour = create_contour(contour_initialization_calibrations, common_calibrations, num_free_slots_for_vertices,
                                  current_cluster_ptr->unique_id(), cluster_drivability, vertices, contours.segment_id_handler);
      }
      return contour;
   }

   std::pair<InitializationResult, Contour_T> create_contour(const Contour_Initialization_Calibrations_T &contour_initialization_calibrations,
                                                             const Common_Calibrations_T &common_calibrations,
                                                             const uint16_t num_free_slots_for_vertices,
                                                             const uint16_t cluster_id,
                                                             const SG_Drivability_Class_T drivability_class,
                                                             Contour_T::VertexList &vertices,
                                                             IdHandlerIncremental<uint32_t> &segment_id_handler)
   {
      InitializationResult result{};
      Contour_T output_contour{};

      if (vertices.size() < 2U)
      {
         result = InitializationResult::EMPTY; // jump to next cluster
      }
      else if ((num_free_slots_for_vertices < vertices.size()) && (num_free_slots_for_vertices < SG_MAX_NUM_VERTICES_PER_CONTOUR))
      {
         // not enough free slots
         result = InitializationResult::OUT_OF_MAX_NUM_VERTICES; // break contour initalization
      }
      else if ((SG_MAX_NUM_VERTICES_PER_CONTOUR < vertices.size()) && (SG_MAX_NUM_VERTICES_PER_CONTOUR <= num_free_slots_for_vertices))
      {
         cut_off_far_vertices(vertices, SG_MAX_NUM_VERTICES_PER_CONTOUR, common_calibrations.host_position);
         assign_segment_ids(vertices, segment_id_handler);
         output_contour = Contour_T(vertices, cluster_id, common_calibrations.initial_reliability,
                                    contour_initialization_calibrations.init_vertex_covariance, drivability_class);
         result         = InitializationResult::CREATED;
      }
      else
      {
         assign_segment_ids(vertices, segment_id_handler);
         output_contour = Contour_T(vertices, cluster_id, common_calibrations.initial_reliability,
                                    contour_initialization_calibrations.init_vertex_covariance, drivability_class);
         result         = InitializationResult::CREATED;
      }

      return std::pair<InitializationResult, Contour_T>(result, std::move(output_contour));
   }
}

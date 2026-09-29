/*=============================================================================================*\
* FILE: sg_contour_downselection.cpp
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains methods definitions for ContourDownselection class
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_contour_downselection.h"

#include "sg_constants.h"
#include "sg_math.h"

namespace sg
{
   void ContourDownselection::run(const Contour_Downselection_Calibrations_T &calibrations, const RSPP_Host_T &host)
   {
      reset_contour_iterators();
      calculate_contours_priority(calibrations, host);
      mark_contours_for_reduced_output(calibrations);
   }

   void ContourDownselection::mark_contours_for_reduced_output(const Contour_Downselection_Calibrations_T &calibrations)
   {
      modify_priorities(calibrations);
      select_contours_for_output();
   }

   void ContourDownselection::select_contours_for_output()
   {
      uint16_t num_reduced_output_vertices = 0U;
      (void) num_reduced_output_vertices;
      // select contours starting from the highest priority ones. It iterates up to smaller of following two numbers: max number of
      // reduced contours and current number of fused contours.
      auto idx = 0U;
      for (; (idx < SG_MAX_NUM_REDUCED_OUTPUT_CONTOURS) && (idx < m_number_of_contours); ++idx)
      {
         const auto &priority = m_contour_priorities[idx].priority;
         (void) priority; // MISRA
         const auto contour_it = m_contour_priorities[idx].it;
         num_reduced_output_vertices += contour_it->num_of_vertices;

         if ((num_reduced_output_vertices > SG_MAX_NUM_REDUCED_OUTPUT_VERTICES)
             || (std::fabs(priority) < std::numeric_limits<float>::epsilon()))
         {
            // stop when overall number of vertices exceeds max number of reduced vertices or contour priority is below significant
            break;
         }
         else
         {
            contour_it->f_selected_for_output = true;
            // store ids of the contours selected for reduced output
            m_previously_selected_contour_ids[idx] = contour_it->get_id();
         }
      }
      m_number_of_selected_contour_ids = idx;

      // sort currently chosen ids in ascending order, so for the next scan index can use binary search
      const auto end_contour_ids_it =
         std::next(m_previously_selected_contour_ids.begin(),
                   static_cast<SelectedContourIdsArray::difference_type>(m_number_of_selected_contour_ids));
      std::sort(m_previously_selected_contour_ids.begin(), end_contour_ids_it);
   }

   void ContourDownselection::reset()
   {
      std::fill(m_previously_selected_contour_ids.begin(), m_previously_selected_contour_ids.end(), 0U);
      for (auto &contour_priority : m_contour_priorities)
      {
         contour_priority.it       = nullptr;
         contour_priority.priority = {};
      }

      m_number_of_contours             = {};
      m_number_of_selected_contour_ids = {};
   }

   void ContourDownselection::modify_priorities(const Contour_Downselection_Calibrations_T &calibrations)
   {
      for (auto idx = 0U; idx < m_number_of_contours; ++idx)
      {
         auto &priority        = m_contour_priorities[idx].priority;
         const auto contour_it = m_contour_priorities[idx].it;

         const auto end_contour_ids_it =
            std::next(m_previously_selected_contour_ids.begin(),
                      static_cast<SelectedContourIdsArray::difference_type>(m_number_of_selected_contour_ids));
         const bool f_contour_previously_selected =
            std::binary_search(m_previously_selected_contour_ids.begin(), end_contour_ids_it, contour_it->get_id());
         (void) f_contour_previously_selected; // MISRA

         const bool f_current_priority_sufficient_to_select = (priority >= calibrations.min_required_contour_priority_to_downselect);
         (void) f_current_priority_sufficient_to_select; // MISRA
         if (f_current_priority_sufficient_to_select)
         {
            // keep already selected contour with higher priority
            if (f_contour_previously_selected)
            {
               priority = 0.5F + 0.5F * priority;
            }
            else
            {
               priority = 0.5F * priority;
            }
         }
      }
      // sorting in descending order
      const auto is_priority_greater = [](const ContourToPriority &lhs, const ContourToPriority &rhs)
      { return (lhs.priority) > (rhs.priority); };
      const auto end_contour_prioity_it =
         std::next(m_contour_priorities.begin(), static_cast<ContourToPriorityArray::difference_type>(m_number_of_contours));
      std::sort(m_contour_priorities.begin(), end_contour_prioity_it, is_priority_greater);
   }

   void ContourDownselection::calculate_contours_priority(const Contour_Downselection_Calibrations_T &calibrations,
                                                          const RSPP_Host_T &host)
   {
      for (auto idx = 0U; idx < m_number_of_contours; ++idx)
      {
         const auto &contour = *m_contour_priorities[idx].it;
         auto &priority      = m_contour_priorities[idx].priority;
         if (contour.num_of_vertices < calibrations.min_number_of_valid_vertices)
         {
            priority = 0.0F;
            continue;
         }

         if (!is_contour_drivability_ok_to_downselect(calibrations, contour))
         {
            priority = 0.0F;
            continue;
         }

         float position_importance{};
         float position_importance_weight{};
         std::tie(position_importance, position_importance_weight) =
            calculate_contour_position_importance(contour, calibrations, host);

         // find maximal contour position weight for normalization
         if (calibrations.position_importance_saturation < position_importance_weight)
         {
            position_importance_weight = calibrations.position_importance_saturation;
         }

         // assign actual priority
         priority = position_importance * position_importance_weight;
      }

      // loop through the contours to normalize priorities to [0..<=1]
      for (auto idx = 0U; idx < m_number_of_contours; ++idx)
      {
         auto &priority = m_contour_priorities[idx].priority;
         priority /= calibrations.position_importance_saturation;

         // reject contours with a very small priorities
         if (priority < calibrations.min_required_contour_priority_to_downselect)
         {
            priority = 0.0F;
            continue;
         }
         assert((0.0F <= priority && priority <= 1.0F) && "Contour priority should be within: [0, 1]");
      }
   }


   bool ContourDownselection::is_contour_drivability_ok_to_downselect(const Contour_Downselection_Calibrations_T &calibrations,
                                                                      const sg::dc::Fused_Contour_T &contour)
   {
      float drivability_stats[all_class_idx + 1U]{};
      auto it_prev = contour.vertices.begin();
      (void) it_prev;
      for (auto it = std::next(it_prev); it != contour.vertices.end(); ++it)
      {
         const auto x_pos_1             = it_prev->position.x;
         const auto y_pos_1             = it_prev->position.y;
         const auto x_pos_2             = it->position.x;
         const auto y_pos_2             = it->position.y;
         const float manhattan_distance = std::fabs(x_pos_1 - x_pos_2) + std::fabs(y_pos_1 - y_pos_2);

         assert((it->drivability < SG_Drivability_Class_T::COUNT) && "Drivability has got a wrong value.");
         drivability_stats[static_cast<std::uint8_t>(it_prev->drivability)] += manhattan_distance;
         drivability_stats[all_class_idx] += manhattan_distance;
         ++it_prev;
         assert((it_prev == it) && "Iterators should be equal at this stage");
      }

      calculate_contour_drivability_shares(drivability_stats);

      const float underdrivable_share = drivability_stats[underdrivable_idx];
      const float overdrivable_share  = drivability_stats[overdrivable_idx];
      const float unclassified_share  = drivability_stats[unclassified_idx];

      const bool f_likely_underdrivable = underdrivable_share > calibrations.min_required_underdrivable_share_to_reject_contour; // 70%
      const bool f_likely_overdrivable = overdrivable_share > calibrations.min_required_overdrivable_share_to_reject_contour; // 70%
      (void) f_likely_overdrivable;
      const bool f_underdrivable_and_unclassified =
         (underdrivable_share > calibrations.min_underdrivable_share_along_with_unclassified)
         && (unclassified_share > calibrations.min_unclassified_share_along_with_underdrivable); // UNDERDRIVABLE > 40% and
                                                                                                 // UNCLASSIFIED > 40%
      (void) f_underdrivable_and_unclassified;
      const bool f_sg_underdrivable = (contour.sg_drivability == SG_Drivability_Class_T::UNDERDRIVABLE);
      (void) f_sg_underdrivable;

      const bool f_reject_contour =
         (f_likely_underdrivable || f_likely_overdrivable || f_underdrivable_and_unclassified || f_sg_underdrivable);
      return !f_reject_contour;
   }


   void ContourDownselection::calculate_contour_drivability_shares(float (&drivability_stats)[all_class_idx + 1U])
   {
      assert((drivability_stats[all_class_idx] > 0.0F) && "The contour should have some segments with a length greater than zero.");
      const float test_pass_th = 1e-4F;
      (void) test_pass_th;
      assert(((std::fabs(drivability_stats[all_class_idx]
                         - (drivability_stats[unclassified_idx] + drivability_stats[overdrivable_idx]
                            + drivability_stats[nondrivable_idx] + drivability_stats[underdrivable_idx])))
              <= test_pass_th)
             && "Total number of vertices should equal to the components' sum.");

      drivability_stats[unclassified_idx]  = drivability_stats[unclassified_idx] / drivability_stats[all_class_idx];
      drivability_stats[overdrivable_idx]  = drivability_stats[overdrivable_idx] / drivability_stats[all_class_idx];
      drivability_stats[nondrivable_idx]   = drivability_stats[nondrivable_idx] / drivability_stats[all_class_idx];
      drivability_stats[underdrivable_idx] = drivability_stats[underdrivable_idx] / drivability_stats[all_class_idx];
   }


   std::pair<float, float> ContourDownselection::calculate_contour_position_importance(
      const sg::dc::Fused_Contour_T &contour, const Contour_Downselection_Calibrations_T &calibrations, const RSPP_Host_T &host)
   {
      float contours_max_x = std::numeric_limits<float>::infinity() * -1.0F;
      (void) contours_max_x;
      float contours_min_x = std::numeric_limits<float>::infinity();
      (void) contours_min_x;
      float contour_importance = 0.0F;
      int vertex_cnt           = 0;

      for (const auto &vertex : contour.vertices)
      {
         if (contours_max_x < vertex.position.x)
         {
            contours_max_x = vertex.position.x;
            (void) contours_max_x;
         }

         if (vertex.position.x < contours_min_x)
         {
            contours_min_x = vertex.position.x;
            (void) contours_min_x;
         }

         if (vertex.position.x >= host.dist_rear_axle_to_vcs_m)
         {
            const auto x_transformed =
               (vertex.position.x - calibrations.contour_priority_shift_x) / calibrations.contour_priority_compaction_factor_x;
            auto y_transformed =
               calculate_curvi_lat_pos(-host.curvature_rear, sg::geometry::Point2D_T({vertex.position.x, vertex.position.y}));
            y_transformed /= calibrations.contour_priority_compaction_factor_y;
            const float square_distance = x_transformed * x_transformed + y_transformed * y_transformed;
            const float vtx_priority    = 1.0F / (1.0F + calibrations.importance_decay_coef * square_distance);
            contour_importance += vtx_priority;
            vertex_cnt++;
         }
      }
      const float x_span_factor = std::sqrt(contours_max_x - contours_min_x);
      if (vertex_cnt > 0)
      {
         contour_importance /= static_cast<float>(vertex_cnt);
      }
      else
      {
         contour_importance = 0.0F;
      }

      assert((contour_importance >= 0.0F && contour_importance <= 1.0F) && "contour importance should be within: [0, 1]");

      if (contours_max_x < calibrations.min_x_to_downselect_contour)
      {
         contour_importance = 0.0F;
      }

      return std::make_pair(contour_importance, x_span_factor);
   }

   void ContourDownselection::reset_contour_iterators()
   {
      std::size_t idx = 0U;
      (void) idx; // MISRA
      for (auto it = m_fused_contours.begin(); it != m_fused_contours.end(); ++it)
      {
         m_contour_priorities[idx++] = ContourToPriority{it, 0.0F};
      }
      m_number_of_contours = m_fused_contours.size();
   }
}

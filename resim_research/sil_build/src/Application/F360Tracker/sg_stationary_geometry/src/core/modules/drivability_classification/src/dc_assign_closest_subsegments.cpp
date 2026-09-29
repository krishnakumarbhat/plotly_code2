/*===================================================================================*\
* FILE: dc_assign_closest_subsegments.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains implementation of assign_closest_subsegments function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_assign_closest_subsegments.h"

#include <algorithm>
#include <array>
#include <limits>

#include "dc_past_data.h"
#include "geometry/geo_distance.h"

namespace sg
{
   namespace dc
   {
      void assign_closest_subsegments(std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &new_leftovers_assigned_mask,
                                      const NewLeftovers &new_contour_leftovers,
                                      const OldLeftovers &old_contour_leftovers)
      {
         const uint16_t num_rows = new_contour_leftovers.num_new_leftovers;
         const uint16_t num_cols = std::min(SG_MAX_NUM_SUBVERTICES_PER_CONTOUR, old_contour_leftovers.num_old_leftovers);

         if ((num_rows > 0U) && (num_cols > 0U))
         {
            std::array<std::array<float, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR>, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> dist_squared{};

            // distance calculation to be changed in the future -> use mid points of subsegments
            for (uint8_t row_idx{0U}; row_idx < num_rows; ++row_idx)
            {
               for (uint8_t col_idx{0U}; col_idx < num_cols; ++col_idx)
               {
                  dist_squared[row_idx][col_idx] = geometry::squared_euclidean_distance(
                     new_contour_leftovers.subsegment_leftovers[row_idx]->begin_vertex.position,
                     old_contour_leftovers.subsegment_leftovers[col_idx]->begin_vertex.position);
               }
            }

            // for each new subsegment choose three smallest squared distances, replace other with max_value
            simplify_distance_array(dist_squared, num_cols, num_rows);

            // associate subsegments
            std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> assigned_rows_mask(0U);
            std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> assigned_cols_mask(0U);
            uint16_t num_unassigned_rows   = num_rows;
            uint16_t num_unassigned_cols   = num_cols;
            const uint16_t num_assignments = std::min(num_unassigned_rows, num_unassigned_cols);
            for (uint16_t i{0U}; i < num_assignments; ++i)
            {
               if ((num_unassigned_cols == 0U) || (num_unassigned_rows == 0U))
               {
                  break;
               }
               uint16_t row_idx_min          = 0U;
               uint16_t col_idx_min          = 0U;
               const float smallest_distance = get_the_smallest_matrix_element(col_idx_min, row_idx_min, dist_squared, num_cols,
                                                                               num_rows, assigned_cols_mask, assigned_rows_mask);
               if ((smallest_distance < std::numeric_limits<float>::max()) && (col_idx_min < SG_MAX_NUM_SUBVERTICES_PER_CONTOUR)
                   && (row_idx_min < SG_MAX_NUM_SUBVERTICES_PER_CONTOUR))
               {
                  const auto &new_subsegment    = new_contour_leftovers.subsegment_leftovers[row_idx_min];
                  new_subsegment->subsegment_id = old_contour_leftovers.subsegment_leftovers[col_idx_min]->subsegment_id;
                  new_subsegment->past_data     = old_contour_leftovers.subsegment_leftovers[col_idx_min]->past_data;
                  (void) new_leftovers_assigned_mask.set(static_cast<size_t>(row_idx_min));
                  (void) assigned_rows_mask.set(static_cast<size_t>(row_idx_min));
                  (void) assigned_cols_mask.set(static_cast<size_t>(col_idx_min));
                  --num_unassigned_cols;
                  --num_unassigned_rows;
               }
            }
            (void) num_unassigned_rows; // MISRA
            (void) num_unassigned_cols; // MISRA
            (void) assigned_rows_mask;  // MISRA
            (void) assigned_cols_mask;  // MISRA
         }
      }

      float get_the_smallest_matrix_element(
         uint16_t &col_idx_min,
         uint16_t &row_idx_min,
         const std::array<std::array<float, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR>, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &input_matrix,
         const uint16_t num_cols,
         const uint16_t num_rows,
         const std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &cols_to_exclude_mask,
         const std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &rows_to_exclude_mask)
      {
         float smallest_value = std::numeric_limits<float>::max();
         for (uint16_t row_idx{0U}; row_idx < num_rows; ++row_idx)
         {
            if (!rows_to_exclude_mask[row_idx])
            {
               for (uint16_t col_idx{0U}; col_idx < num_cols; ++col_idx)
               {
                  if (!cols_to_exclude_mask[col_idx])
                  {
                     if (input_matrix[row_idx][col_idx] < smallest_value)
                     {
                        smallest_value = input_matrix[row_idx][col_idx];
                        row_idx_min    = row_idx;
                        col_idx_min    = col_idx;
                     }
                  }
               }
            }
         }
         return smallest_value;
      }

      void simplify_distance_array(
         std::array<std::array<float, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR>, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &distances,
         const uint16_t num_cols,
         const uint16_t num_rows)
      {
         const uint16_t max_num_elements{3U};
         if (num_rows > max_num_elements)
         {
            for (uint16_t col_idx{0U}; col_idx < num_cols; ++col_idx)
            {
               std::array<float, max_num_elements> smallest_distances{
                  {distances[0U][col_idx], distances[1U][col_idx], distances[2U][col_idx]}};
               std::sort(smallest_distances.begin(), smallest_distances.end());
               for (uint16_t row_idx = max_num_elements; row_idx < num_rows; ++row_idx)
               {
                  if (distances[row_idx][col_idx] < smallest_distances[max_num_elements - 1U])
                  {
                     smallest_distances[max_num_elements - 1U] = distances[row_idx][col_idx];
                     std::sort(smallest_distances.begin(), smallest_distances.end());
                  }
               }
               uint16_t num_smallest_values{0U};
               (void) num_smallest_values; // MISRA
               for (uint16_t row_idx{0U}; row_idx < num_rows; ++row_idx)
               {
                  if ((distances[row_idx][col_idx] > smallest_distances[max_num_elements - 1U])
                      || (num_smallest_values == max_num_elements))
                  {
                     distances[row_idx][col_idx] = std::numeric_limits<float>::max();
                  }
                  else
                  {
                     ++num_smallest_values;
                     (void) num_smallest_values; // MISRA
                  }
               }
            }
         }
      }
   }
}

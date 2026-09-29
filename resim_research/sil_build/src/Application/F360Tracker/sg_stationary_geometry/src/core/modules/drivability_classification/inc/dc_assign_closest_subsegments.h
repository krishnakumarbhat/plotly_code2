/*===================================================================================*\
* FILE: dc_assign_closest_subsegments.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of assign_closest_subsegments function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_ASSIGN_CLOSEST_SUBSEGMENTS_H
#define DC_ASSIGN_CLOSEST_SUBSEGMENTS_H

#include <bitset>

#include "dc_common.h"
#include "dc_contour.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      /**
       * @brief            Assigns closest subsegments from old_leftovers to new_leftovers.
       *
       * @param[out]       new_leftovers_assgined_mask - mask of new leftovers that have been assigned with a subsegment and don't
       *need new subsegment_id assignment
       * @param[in]        new_contour_leftovers - strucuture with leftovers in new dc_contour
       * @param[in]        old_contour_leftovers - structure with leftovers in old dc_contour
       **/
      void assign_closest_subsegments(std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &new_leftovers_assigned_mask,
                                      const NewLeftovers &new_contour_leftovers,
                                      const OldLeftovers &old_contour_leftovers);

      /**
       * @brief            Function to get the smallest value in the matrix and indices of this element exluding given rows and
       *columns.
       *
       * @param[out]       col_idx_min - the column index of the smallest element
       * @param[out]       row_idx_min - the row index of the smallest element
       * @param[in]        input_matrix - matrix of values
       * @param[in]        num_cols - number of columns in the matrix
       * @param[in]        num_rows - number of rows in the matrix
       * @param[in]        cols_to_exclude_mask - idicates which of columns should be omitted
       * @param[in]        rows_to_exclude_mask - idicates which of rows should be omitted
       *
       * @return           smallest_number - smallest number in the matrix
       **/
      float get_the_smallest_matrix_element(
         uint16_t &col_idx_min,
         uint16_t &row_idx_min,
         const std::array<std::array<float, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR>, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &input_matrix,
         const uint16_t num_cols,
         const uint16_t num_rows,
         const std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &cols_to_exclude_mask,
         const std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &rows_to_exclude_mask);

      /**
       * @brief            Function to simplify an array of distances (keep only three smallest values in each column).
       *
       * @param[out]   distances - array of distances
       * @param[in]        num_cols - number of columns in the array
       * @param[in]        num_rows - number of rows in the array
       **/
      void simplify_distance_array(
         std::array<std::array<float, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR>, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &distances,
         const uint16_t num_cols,
         const uint16_t num_rows);
   }
}
#endif

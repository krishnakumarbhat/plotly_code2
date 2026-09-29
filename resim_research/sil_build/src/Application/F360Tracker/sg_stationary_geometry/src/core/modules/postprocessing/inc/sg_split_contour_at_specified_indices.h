/*=============================================================================================*\
* FILE: sg_split_contour_at_specified_indices.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains functions used to split the contour
*           given an array of indices of vertices to be removed.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_SPLIT_CONTOUR_AT_SPECIFIED_INDICES_H
#define SG_SPLIT_CONTOUR_AT_SPECIFIED_INDICES_H

#include "sg_contour.h"

namespace sg
{
   /**
    * @brief         Function splits the contour given an array of indices of vertices to be removed.
    *
    * @param[in]     contour
    * @param[in]     new_contours
    * @param[in]     split_indices
    * @param[in]     num_split_indices
    * @param[in]     max_num_new_contours
    *
    * @return        num_new_contours
    **/
   uint16_t split_contour_at_specified_indices(const Contour_T &contour,
                                               std::array<Contour_T, SG_MAX_NUM_CONTOURS> &new_contours,
                                               std::array<uint16_t, SG_MAX_NUM_VERTICES_PER_CONTOUR> &split_indices,
                                               const uint16_t num_split_indices,
                                               const uint16_t max_num_new_contours);

   /**
    * @brief         Function defines the array of indices of contour vertices that are out of the given
    *                   split indices array and returns the number of element of this new array.
    *
    * @param[in]     split_indices
    * @param[in]     non_split_indices
    * @param[in]     num_split_indices
    * @param[in]     num_vertices
    *
    * @return        num_non_split_indices
    **/
   uint16_t get_non_split_indices(std::array<uint16_t, SG_MAX_NUM_VERTICES_PER_CONTOUR> &split_indices,
                                  std::array<uint16_t, SG_MAX_NUM_VERTICES_PER_CONTOUR> &non_split_indices,
                                  const uint16_t num_split_indices,
                                  const uint16_t num_vertices);

   /**
    * @brief         Function returns the index of the first element of the array of numbers after start_idx
    *                   that is not greater by 1 from the previous element of this array. If there's no such
    *                   index (start_idx is incorrect) it returns num_elements. If num_elements is greater
    *                   than SG_MAX_NUM_VERTICES_PER_CONTOUR it returns SG_MAX_NUM_VERTICES_PER_CONTOUR
    *
    * @param[in]     numbers
    * @param[in]     num_elements
    * @param[in]     start_idx
    *
    * @return        idx
    **/
   uint16_t get_first_nonconsecutive_idx(const std::array<uint16_t, SG_MAX_NUM_VERTICES_PER_CONTOUR> &numbers,
                                         const uint16_t num_elements,
                                         const uint16_t start_idx);
}
#endif

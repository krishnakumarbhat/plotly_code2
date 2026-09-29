/*=============================================================================================*\
* FILE: sg_remove_vertices_helpers.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the helper functions used by the remove_vertices algorithm.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_REMOVE_VERTICES_HELPERS_H
#define SG_REMOVE_VERTICES_HELPERS_H

#include <bitset>

#include "sg_contour.h"

namespace sg
{
   /**
    * @brief             Function checks if vertex coordinates are out of range, and if that is the case,
    *                       it returns true; otherwise, it returns false.
    *
    * @param[in]         vertex
    * @param[in]         rear_limit
    * @param[in]         front_limit
    * @param[in]         left_limit
    * @param[in]         right_limit
    *
    * @return            f_vertex_outside_range
    **/
   inline bool vertex_position_out_of_range(
      const Vertex_T &vertex, const float rear_limit, const float front_limit, const float left_limit, const float right_limit)
   {
      return (front_limit <= vertex.position.x) || (vertex.position.x <= rear_limit) || (right_limit <= vertex.position.y)
             || (vertex.position.y <= left_limit);
   }

   /**
    * @brief             Function checks if vertex covariances, respectively x and y, exceed the uncertainty level,
    *                       and if that is the case, it sets the corresponding vertices_to_remove_mask[idx] flag to true;
    *otherwise, it does nothing.
    *
    * @param[in]         vertices
    * @param[in]         accepted_uncertainty,
    * @param[in, out]    vertices_to_remove_mask
    **/
   void mark_uncertain_vertices(const Contour_T::VertexList &vertices,
                                const float accepted_uncertainty,
                                std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_to_remove_mask);

   /**
    * @brief             Function iterates over vertices and checks if the reliability of the vertex has dropped to an unacceptable
    *level or the number of cycles the vertex has existed with no update has exceeded the maximum acceptable number. If that is the
    *case, it sets the corresponding vertices_to_remove_mask[idx] flag to true; otherwise, it does nothing.
    *
    * @param[in]         vertices
    * @param[in]         max_num_cycles_no_update,
    * @param[in, out]    vertices_to_remove_mask
    *
    **/
   void mark_unreliable_vertices(const Contour_T::VertexList &vertices,
                                 const uint16_t max_num_cycles_no_update,
                                 std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_to_remove_mask);

   /**
    * @brief            Function determines if contour should be splitted or shortened.
    *
    * @param[in]        num_of_vertices
    * @param[in]        vertices_to_remove_mask
    *
    * @return           f_split_scenario
    **/
   bool is_split_scenario(const size_t num_of_vertices, const std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_to_remove_mask);

   /**
    * @brief            Function splits or shortens the contour, based on various conditions.
    *
    * @param[in]        contour_idx
    * @param[in]        vertices_to_remove_mask
    * @param[in]        contour
    * @param[in, out]   num_empty_contour_slots
    * @param[in, out]   contours_to_remove_mask
    * @param[in, out]   num_new_contours
    * @param[in, out]   all_new_contours
    **/
   void split_or_shorten_contour(const uint8_t contour_idx,
                                 const std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_to_remove_mask,
                                 std::array<Contour_T, SG_MAX_NUM_CONTOURS> &all_new_contours,
                                 Contour_T &contour,
                                 std::bitset<SG_MAX_NUM_CONTOURS> &contours_to_remove_mask,
                                 uint16_t &num_empty_contour_slots,
                                 uint16_t &num_new_contours);

   /**
    * @brief            Function marks out of range vertices in vertices_to_remove_mask.
    *
    * @param[in]        vertices
    * @param[in]        rear_limit
    * @param[in]        front_limit
    * @param[in]        left_limit
    * @param[in]        right_limit
    * @param[in, out]   vertices_to_remove_mask
    **/
   void mark_out_of_range_vertices(const Contour_T::VertexList &vertices,
                                   const float rear_limit,
                                   const float front_limit,
                                   const float left_limit,
                                   const float right_limit,
                                   std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_to_remove_mask);

   /**
    * @brief            Gets indices of true values of a mask and their number.
    *
    * @param[in]        mask
    * @param[in, out]   indices
    *
    * @return           num_of_indices - number of indices
    *=============================================================================================*/
   uint16_t get_indices_of_mask(const std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &mask,
                                std::array<uint16_t, SG_MAX_NUM_VERTICES_PER_CONTOUR> &indices);
}
#endif

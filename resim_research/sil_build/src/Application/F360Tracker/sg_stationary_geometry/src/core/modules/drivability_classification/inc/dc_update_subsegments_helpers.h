/*===================================================================================*\
* FILE: dc_update_subsegments_helpers.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of update_subsegments helper functions.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_UPDATE_SUBSEGMENTS_HELPERS
#define DC_UPDATE_SUBSEGMENTS_HELPERS

#include <bitset>

#include "dc_array_wrapper.h"
#include "dc_common.h"
#include "dc_contour_storage.h"
#include "sg_calibrations.h"
#include "sg_contour_storage.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      /**
       * @brief          Adds subsegments of old contour that have not been projected to old leftovers.
       *
       * @param[out]     leftovers - old contour leftovers
       * @param[in]      dc_contour_it - iterator to the contour
       * @param[in]      f_projected_old - mask indicating which subsegments of old contour have been projected
       **/
      void add_not_projected_subsegments_to_leftovers(OldLeftovers &leftovers,
                                                      const DCContourStorage::ContourList::iterator dc_contour_it,
                                                      const std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &f_projected_old);

      /**
       * @brief         Assigns the last vertex to the contour.
       *
       * @param[out]    dc_contour - DC contour
       * @param[in]     current_contour - SG contour
       * @param[in]     f_critical - is last vertex critical
       *
       * @details       This function handles problem with skipping the last vertex position assignment to the contour.
       **/
      void assign_last_vertex_to_contour(DC_Contour_T &dc_contour, const Contour_T &current_contour, const bool f_critical);

      /**
       * @brief         Assigns vertex to the contour.
       *
       * @param[out]    dc_contour - reference to the DC contour
       * @param[in]     segment_beg - reference to the begin vertex of subsegment
       * @param[in]     segment_end - reference to the end vertex of subsegment
       * @param[in]     f_critical - is segment_end critical
       **/
      void assign_vertex_to_contour(DC_Contour_T &dc_contour,
                                    const Vertex_T &segment_beg,
                                    const Vertex_T &segment_end,
                                    const bool f_critical);

      /**
       * @brief         Finds DC segment and subsegment corresponding to given SG segment.
       *
       * @param[in]     dc_contour_list - DC contour storage
       * @param[in]     segment_id - id of a segment in SG contour
       *
       * @return        pair<DCContourStorage::ContourList::iterator, DC_Contour_T::SubsegmentList::iterator>
       **/
      std::pair<DCContourStorage::ContourList::iterator, DC_Contour_T::SubsegmentList::iterator>
      find_corresponding_segment(const DCContourStorage &dc_contour_list, const uint32_t segment_id);

      /**
       * @brief         Assigns subsegment ids to subsegments of a new contour, that have not been assigned so far.
       *
       * @param[in]     dc_contour_list - DC contour storage
       * @param[in]     new_contour_leftovers - leftovers of new contour
       * @param[in]     new_leftovers_assigned_mask - indicated leftovers of new contour that have been already assigned
       **/
      void populate_not_assigned_leftovers(DCContourStorage &dc_contour_list,
                                           const NewLeftovers &new_contour_leftovers,
                                           const std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &new_leftovers_assigned_mask);

      /**
       * @brief         Updates DC contours container with data calculated for current scan index.
       *
       * @param[out]    dc_contours_list -  DC contour storage
       * @param[in]     dc_contours_array_temp - temporary DC contour array
       **/
      void reassign_DC_contours(DCContourStorage &dc_contours_list,
                                ArrayWrapper<DC_Contour_T, SG_MAX_NUM_CONTOURS> &dc_contours_array_temp);

      /**
       * @brief         Updates leftovers of new contour.
       *
       * @param[out]    leftovers - leftovers of the new contour
       * @param[in]     segment_start - iterator to the begining of the segment
       * @param[in]     leftovers_mask - mask indicating which subsegments of the current segment are leftovers
       * @param[in]     curr_num_vertices - current number of vertices in DC contour
       * @param[in]     num_subvertices_to_add_safe - number of subsegments that may be added to DC contour
       * @param[in]     f_next_critical - flag indicating if the end vertex of the segment is critical
       **/
      void update_new_leftovers(NewLeftovers &leftovers,
                                const DC_Contour_T::SubsegmentList::iterator segment_start,
                                const std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &leftovers_mask,
                                const uint16_t curr_num_vertices,
                                const uint16_t num_subvertices_to_add_safe,
                                const bool f_next_critical);
   }
}
#endif

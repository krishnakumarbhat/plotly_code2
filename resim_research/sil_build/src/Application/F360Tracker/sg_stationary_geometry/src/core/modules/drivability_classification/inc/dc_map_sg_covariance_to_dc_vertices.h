/*===================================================================================*\
* FILE: dc_map_sg_covariance_to_dc_vertices.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of map_sg_covariance_to_dc_vertices function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_MAP_SG_COVARIANCE_TO_DC_VERTICES
#define DC_MAP_SG_COVARIANCE_TO_DC_VERTICES

#include "dc_fused_contour_storage.h"
#include "sg_contour.h"
#include "sg_contour_storage.h"

namespace sg
{
   namespace dc
   {
      /**
       * @brief         This function assigns appropriate covariance and cross covariance values
       * @brief         to fused contours based on SG contours. If direct assignment is not possible
       * @brief         interpolation is used.
       *
       * @param[out]    contour_storage - reference to list of fused SG and DC contours
       * @param[in]     sg_contours - reference to list of SG contours
       * @param[in]     in_init_segment_length - minimal length of segment
       **/
      void map_sg_covariance_to_dc_vertices(FusedContourStorage &contour_storage,
                                            const ContourStorage &sg_contours,
                                            const float min_segment_length);

      /**
       * @brief         This function checks if two points are close.
       *
       * @param[in]     point1 - position of the first point
       * @param[in]     point2 - position of the second point
       * @param[in]     tolerance - max difference of points' coordinates
       *
       * @return        true if points are close, otherwise false
       **/
      bool are_points_close(const geometry::Point2D_T point1, const geometry::Point2D_T point2, const float tolerance);

      /**
       * @brief         This function finds iterator to SG contour with id equal to the given value.
       *
       * @param[in]     contour_id - id of the contour to be found
       * @param[in]     sg_contours - reference to list of SG contours
       *
       * @return        iterator to SG contour if contour is found or nullptr otherwise
       **/
      ContourStorage::ContourList::iterator find_matching_contour(const uint32_t contour_id, const ContourStorage &sg_contours);

      /**
       * @brief         This function interpolates covariance of fused vertex using two adjacent SG vertices.
       *
       * @param[out]    vertex - vertex of a fused contour
       * @param[in]     sg_vertex - the first SG vertex of a segment
       * @param[in]     next_sg_vertex - the second SG vertex of a segment
       * @param[in]     min_segment_length - if sement length is below this value we use covariance data of the first vertex
       **/
      void interpolate_vertex_covariance(Fused_Vertex_T &vertex,
                                         const Vertex_T &sg_vertex,
                                         const Vertex_T &next_sg_vertex,
                                         const float min_segment_length);
   }
}

#endif

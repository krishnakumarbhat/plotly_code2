/*===================================================================================*\
* FILE: dc_get_subvertices_positions.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file declares method that performs [primary_start_vertex, primary_end_vertex] segment interpolation in a straight line
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_GET_SUBVERTICES_POSITIONS_H
#define DC_GET_SUBVERTICES_POSITIONS_H

#include <utility>

#include "dc_array_wrapper.h"
#include "geometry/geo_point.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {

      /**
       * @brief             This function performs [primary_start_vertex, primary_end_vertex] segment interpolation.
       *
       * @param[out]        subvertices_positions_array - array of subvertices positions
       * @param[in]         first_vertex_position - first vertex position of the segment that is to be interpolated
       * @param[in]         last_vertex_position - last vertex position of the segment that is to be interpolated
       * @param[in]         subsegment_length - const from configuration, length of subsegment
       * @param[in]         include_first_vertex - bool, true - first vertex added to array of subvertices, false - first vertex
       *not added to array of subvertices
       * @param[in]         include_last_vertex  - bool, true - last vertex added to array of subvertices, false - last vertex not
       *added to array of subvertices
       *
       **/
      void get_subvertices_positions(ArrayWrapper<geometry::Point2D_T> &subvertices_positions_array,
                                     const geometry::Point2D_T &first_vertex_position,
                                     const geometry::Point2D_T &last_vertex_position,
                                     const float subsegment_length,
                                     const bool include_first_vertex,
                                     const bool include_last_vertex);
   }
}
#endif
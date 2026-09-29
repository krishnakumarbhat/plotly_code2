/*=============================================================================================*\
* FILE: sg_init_contour_bbox_stitch.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for init_bbox_stitch and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_INIT_BBOX_STITCH_H
#define SG_INIT_BBOX_STITCH_H

#include <bitset>

#include "geometry/geo_rectangle.h"
#include "sg_contour_storage.h"
#include "sg_detection_storage.h"

namespace sg
{
   /**
    * @brief   Initializes a contour along the cluster's length.
    *          A rectangular bounding box encloses detections which mean position constitutes
    *          a single vertex position. By shifting the bounding box further and computing
    *          vertices along the way, the new contour is initialized.
    *
    * @param   vertices
    * @param   current_cluster_ptr
    * @param   init_look_distance
    * @param   min_segment_length
    * @param   host_position
    *
    * @return  List of Vertices
    **/
   void init_bbox_stitch(Contour_T::VertexList &vertices,
                         const Cluster *const current_cluster_ptr,
                         const float init_look_distance,
                         const float min_segment_length,
                         const geometry::Point2D_T host_position);

   /**
    * @brief    Filters detections inside ROI and calculates a vertex based on a scaled mean of chosen detection positions.
    *
    * @param   current_cluster_ptr
    * @param   region_of_interest
    * @param   min_segment_length
    * @param   host_position
    *
    * @return  pair - 1. selected vetex 2. flag that determines if algorithm should be terminated
    **/
   std::pair<Vertex_T, bool> calculate_vertex_inside_roi(const Cluster *const current_cluster_ptr,
                                                         const geometry::Rectangle_T &region_of_interest,
                                                         const float min_segment_length,
                                                         const geometry::Point2D_T host_position);

   /**
    * @brief    Choose contour starting position.
    *
    * @param   current_cluster_ptr
    *
    * @return  2D point
    **/
   geometry::Point2D_T determine_starting_position(const Cluster *const current_cluster_ptr);

   /**
    * @brief   Scales vertex coordinates.
    *
    * @param   next_vertex
    * @param   previous_vertex_position
    * @param   scale_factor
    * @param   min_segment_length
    * @param   host_position
    *
    * @return  N/A
    **/
   void scale_to_max_distant_detection(Vertex_T &next_vertex,
                                       const geometry::Point2D_T previous_vertex_position,
                                       float scale_factor,
                                       const float min_segment_length,
                                       const geometry::Point2D_T host_position);

   /**
    * @brief   Calculates next vertex position.
    *
    * @param   next_vertex
    * @param   previous_vertex_position
    * @param   min_segment_length
    * @param   host_position
    *
    * @return  N/A
    **/
   void calc_min_length_segment_vertex(Vertex_T &next_vertex,
                                       const geometry::Point2D_T previous_vertex_position,
                                       const float min_segment_length,
                                       const geometry::Point2D_T host_position);
}
#endif

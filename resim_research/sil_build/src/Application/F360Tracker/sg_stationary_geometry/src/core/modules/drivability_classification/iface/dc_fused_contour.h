/*===================================================================================*\
* FILE: dc_fused_contour.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of Fused_Contour type.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_FUSED_CONTOUR_H
#define DC_FUSED_CONTOUR_H

#include "dc_fused_vertex.h"
#include "embedded_list.h"
#include "sg_constants.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      class Fused_Contour_T
      {
        public:
         using FusedVertexList = EmbeddedList<Fused_Vertex_T, SG_MAX_NUM_FUSED_VERTICES_PER_CONTOUR * SG_MAX_NUM_FUSED_CONTOURS>;
         FusedVertexList vertices;
         uint16_t num_of_vertices;
         bool f_selected_for_output;
         SG_Drivability_Class_T sg_drivability; // drivability classification that was set by SG

         /**
          * @brief    Default constructor for fused contours.
          **/
         Fused_Contour_T();

         /**
          * @brief   Initilializer list constructor of Fused_Vertex_T class. Initializes the contour with elements from source.
          *
          * @param[in]    init_list    list of fused subvertices
          **/
         Fused_Contour_T(const std::initializer_list<Fused_Vertex_T> init_list);

         /**
          * @brief    Constructor of Fused_Contour_T class. Initializes the contour with elements from source.
          *
          * @param[in]    _vertices           list of subsegments, number of subsegments is number of vertices - 1
          * @param[in]    _id                 unique_id of DC_Contour
          * @param[in]    _num_of_vertices    number of vertices
          * @param[in]    _f_selected_for_output    flag indicating if contour was selected for output
          * @param[in]    _sg_drivability        drivability classification that was set by SG
          **/
         Fused_Contour_T(const FusedVertexList &_vertices,
                         const uint32_t _id,
                         const uint16_t _num_of_vertices,
                         const bool _f_selected_for_output,
                         const SG_Drivability_Class_T _sg_drivability);

         /**
          * @brief    Get unique_id of Fused_Contour_T
          *
          * @return   unique_id of fused contour
          **/
         uint32_t get_id() const;

         /**
          * @brief    Set unique_id of Fused_Contour_T
          *
          * @param[in]    id   -   unique_id of fused contour
          **/
         void set_id(const uint32_t id);

        private:
         uint32_t m_id;
      };
   }
}

#endif

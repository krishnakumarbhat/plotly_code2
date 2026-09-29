/*===================================================================================*\
* FILE: sg_dumping.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains dumping functionality
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_DUMPING_H
#define SG_DUMPING_H

#include "dc_fused_contour.h"
#include "dc_fused_vertex.h"
#include "sg_contour.h"
#include "sg_contour_dump.h"
#include "sg_contour_out.h"
#include "sg_detection.h"
#include "sg_detection_dump.h"
#include "sg_vertex.h"
#include "sg_vertex_dump.h"
#include "sg_vertex_out.h"

namespace sg
{
   namespace dumping
   {
      /*============================================================================================*\
       * Function      dump(SG_Detection_Dump_T& dumped_detection, const Detection_T& detection)
       *
       * Description   dumps internal type Detection_T to interface type SG_Detection_Dump_T
       *
       * Parameters
       *               detection - (in) detection to dump
       *               dumped_detection - (out) dumped detection
       *
       * Returns       N/A
       *=============================================================================================*/
      void dump(SG_Detection_Dump_T &dumped_detection, const Detection_T &detection);

      /*============================================================================================*\
       * Function      dump(SG_Vertex_Dump_T& dumped_vertex, const Vertex_T& vertex)
       *
       * Description   dumps internal type Vertex_T to interface type SG_Vertex_Dump_T
       *
       * Parameters
       *               vertex - (in) vertex to dump
       *               dumped_vertex - (out) dumped vertex
       *
       * Returns       N/A
       *=============================================================================================*/
      void dump(SG_Vertex_Dump_T &dumped_vertex, const Vertex_T &vertex);

      /**
       * @brief          dumps internal vertices
       *
       * @param[out]     dumped_vertices
       * @param[in]      vertices
       * @param[in]      pos_offset
       *
       * @return         position offset of the next free index in output vertices list
       **/
      std::size_t dump(SG_Vertex_Dump_T (&dumped_vertices)[SG_MAX_NUM_VERTICES],
                       const Contour_T::VertexList &vertices,
                       std::size_t pos_offset);

      /**
       * @brief          dumps internal contour data
       *
       * @param[out]     dumped_contour
       * @param[in]      contour
       *
       * @return         None
       **/
      void dump(SG_Contour_Dump_T &dumped_contour, const Contour_T &contour);

      /**
       * @brief          dumps output vertex data
       *
       * @param[out]     dumped_vertex
       * @param[in]      vertex
       *
       * @return         None
       **/
      void dump(SG_Vertex_Out_T &dumped_vertex, const dc::Fused_Vertex_T &vertex);

      /**
       * @brief          dumps output contour data
       *
       * @param[out]     dumped_contour
       * @param[in]      contour
       *
       * @return         None
       **/
      void dump(SG_Contour_Out_T &dumped_contour, const dc::Fused_Contour_T &contour);
   }
}

#endif

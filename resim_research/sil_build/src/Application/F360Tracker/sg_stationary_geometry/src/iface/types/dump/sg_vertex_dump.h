/*===================================================================================*\
* FILE: sg_vertex_dump.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains mandatory SG datatypes i.e. vertex dump type.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_VERTEX_DUMP_H
#define SG_VERTEX_DUMP_H
#include "sg_reuse.h"

namespace sg
{
   struct SG_Vertex_Dump_T
   {
      struct Position_2D_T
      {
         float x;
         float y;
      } position; // [m]

      struct B_Box_Center
      {
         float x;
         float y;
      } b_box_center; // [m]

      struct Covariance_T
      {
         float xx;
         float yy;
         float xy;
      } pos_cov; // [m^2] position covariance

      struct Cross_Covariance_T
      {
         float x1x2;
         float y1y2;
         float x1y2;
         float y1x2;
      } pos_cross_cov; // [m^2] position cross-covariance

      float b_box_width;          // [m] rectangle width
      float b_box_length;         // [m] rectangle length
      float b_box_rotation_angle; // [rad] rectangle rotation angle
      float reliability{};        // [-] measure of how reliable vertex information is
      uint32_t segment_id{}; // [-] gloablly unique id of segment starting with this vertex (0 for the last vertex of a contour)
      uint16_t age{};        // [-] numer of SG cycles this vertex is tracked (from its creation)
      uint16_t num_cycles_no_update{}; // [-] numer of SG cycles by which this vertex was not update by measurements
      uint8_t drivability{};
   };
}

#endif

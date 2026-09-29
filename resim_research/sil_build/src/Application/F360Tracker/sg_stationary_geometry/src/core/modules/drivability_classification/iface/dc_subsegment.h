/*===================================================================================*\
* FILE: dc_subsegment.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file implements Subsegment_T class related to DC subsegments.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_SUBSEGMENT_H
#define DC_SUBSEGMENT_H

#include <utility>

#include "dc_features.h"
#include "dc_past_data.h"
#include "dc_subsegment_vertex.h"
#include "geometry/geo_point.h"
#include "sg_drivability_class.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      class Subsegment_T
      {
        public:
         Subsegment_T() = default;
         Subsegment_T(const Subsegment_Vertex_T &vertex_begin, const Subsegment_Vertex_T &vertex_end);

         Past_Data_T past_data{};                        // accumulated data from past scans used for feature calculation
         Features_T features{};                          // features from current window
         Subsegment_Vertex_T begin_vertex{{0.0F, 0.0F}}; // [m] position of the beginning of the segment
         Subsegment_Vertex_T end_vertex{{0.0F, 0.0F}};   // [m] position of the end of the segment
         float drivability_confidence{0.0F};             // confidence of the estimation of drivability
         uint32_t segment_id{0U};
         uint32_t subsegment_id{0U};
         uint8_t num_of_dets_associated_last_scan{0U}; // [-] number of dets associated with a subsegment in the last scan
         uint16_t sg_age{0U};                          // equivalent of age in Vertex_T
         uint16_t sg_cycles_since_coasted{0U};         // equivalent of num_cycles_no_update in Vertex_T
         SG_Drivability_Class_T drivability{SG_Drivability_Class_T::UNCLASSIFIED};

         bool is_critical() const;
      };
   }
}

#endif

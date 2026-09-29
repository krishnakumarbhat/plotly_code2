/*===================================================================================*\
* FILE: dc_common.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of auxiliary structures used within DC.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_COMMON
#define DC_COMMON

#include <bitset>

#include "dc_contour_storage.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      struct NewLeftovers
      {
         std::array<DC_Contour_T::SubsegmentList::iterator, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> subsegment_leftovers;
         uint16_t num_new_leftovers{0U};
      };

      struct OldLeftovers
      {
         std::array<DC_Contour_T::SubsegmentList::iterator, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> subsegment_leftovers;

         std::array<EmbeddedList<DC_Contour_T, SG_MAX_NUM_CONTOURS>::iterator, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> contour_leftovers;
         uint16_t num_old_leftovers{0U};
      };

      struct UpdatedSegment
      {
         UpdatedSegment() : state{}, assigned_old_subsegment{}, f_critical{}, f_leftover{}, num_elements{0U}
         {
            (void) std::fill_n(assigned_old_subsegment.begin(), SG_MAX_NUM_SUBVERTICES_PER_CONTOUR, nullptr);
         };
         std::array<geometry::Point2D_T, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> state;
         std::array<DC_Contour_T::SubsegmentList::iterator, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> assigned_old_subsegment;
         std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> f_critical;
         std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> f_leftover;
         uint16_t num_elements;
      };
   }
}
#endif

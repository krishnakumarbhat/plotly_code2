/*===================================================================================*\
* FILE: dc_contour_storage.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definitions of DCContourStorage methods.
*
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_contour_storage.h"

#include <numeric>

namespace sg
{
   namespace dc
   {
      const uint32_t DCContourStorage::MAX_ID_RESET_VALUE{1'000'000U};

#ifdef SG_SAVE_DETECTIONS_ASSIGNED_TO_SUBSEGMENTS
      std::unordered_map<uint32_t, SubsegmentDetections_T> DCContourStorage::assigned_detections;
#endif

      DCContourStorage::ContourList::iterator DCContourStorage::push_back(DC_Contour_T &&contour)
      {
         const auto result = m_contours.push_back(std::move(contour));

         if (result != m_contours.end())
         {
            m_contours.back().m_id = get_available_contour_id();
         }
         return result;
      }

      DCContourStorage::ContourList::iterator DCContourStorage::push_back(DC_Contour_T &&contour, const uint32_t id)
      {
         const auto result = m_contours.push_back(std::move(contour));

         if (result != m_contours.end())
         {
            m_contours.back().m_id = id;
         }
         return result;
      }

      void DCContourStorage::clear()
      {
         set_available_contour_id(0U);
         set_available_subsegment_id(0U);
         return m_contours.clear();
      }

      void DCContourStorage::reset_contour_list()
      {
         set_available_contour_id(0U);
         m_contours.clear();
      }

      std::size_t DCContourStorage::total_subsegments_number() const
      {
         return std::accumulate(m_contours.begin(), m_contours.end(), static_cast<size_t>(0U),
                                [](const size_t accumulator, const DC_Contour_T &contour) { return accumulator + contour.size(); });
      }

      uint32_t DCContourStorage::get_available_subsegment_id()
      {
         if (available_subsegment_id >= MAX_ID_RESET_VALUE)
         {
            available_subsegment_id = 0U;
         }
         return ++available_subsegment_id;
      }

      void DCContourStorage::set_available_subsegment_id(const uint32_t id)
      {
         available_subsegment_id = id;
      }

      uint32_t DCContourStorage::get_available_contour_id()
      {
         if (available_contour_id >= MAX_ID_RESET_VALUE)
         {
            available_contour_id = 0U;
         }
         return ++available_contour_id;
      }

      void DCContourStorage::set_available_contour_id(const uint32_t id)
      {
         available_contour_id = id;
      }

      void DCContourStorage::initialize(const DC_Dump_T &dumped_dc)
      {
         (void) dumped_dc; // TODO: FZD-1863
      }
   }
}

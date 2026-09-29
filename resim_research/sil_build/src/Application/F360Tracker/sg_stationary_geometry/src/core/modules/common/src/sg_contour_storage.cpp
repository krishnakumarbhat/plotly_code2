/*===================================================================================*\
* FILE: sg_contour_storage.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definitions of ContourStorage methods.
*
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "sg_contour_storage.h"

namespace sg
{
   ContourStorage::ContourList::iterator ContourStorage::push_back(Contour_T &&contour)
   {
      const auto result = m_contours.push_back(std::move(contour));

      if (result != m_contours.end())
      {
         m_contours.back().m_unique_id = m_id_handler.get_id();
      }
      return result;
   }

   ContourStorage::ContourList::iterator ContourStorage::push_front(Contour_T &&contour)
   {
      const auto result = m_contours.push_front(std::move(contour));

      if (result != m_contours.end())
      {
         m_contours.front().m_unique_id = m_id_handler.get_id();
      }

      return result;
   }

   ContourStorage::ContourList::iterator ContourStorage::erase(const ContourList::iterator current)
   {
      auto id = (*current).unique_id();
      assert(id > 0);
      (void) m_id_handler.return_id(id);
      return m_contours.erase(current);
   }

   std::size_t ContourStorage::total_vertices_number() const
   {
      std::size_t num_vertices = 0U;

      if (!empty())
      {
         for (const auto &contour : m_contours)
         {
            num_vertices += contour.size();
         }
      }

      return num_vertices;
   }

   void ContourStorage::initialize(const SG_Contour_Storage_Dump_T &dumped_internal_contours)
   {
      (void) dumped_internal_contours; // TODO: FZD-1862
   }
}

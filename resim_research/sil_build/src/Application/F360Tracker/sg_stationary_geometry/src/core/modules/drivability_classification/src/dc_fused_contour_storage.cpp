/*===================================================================================*\
* FILE: dc_fused_contour_storage.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definition of FusedContourStorage class - the container for Fused_Contour_T objects.
*   Fused_Contour_T objects are stored in FusedContourList which has type of EmbeddedList.
*   Single Fused_Contour_T object contains list of vertices (of type EmbeddedList) belonging to it.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_fused_contour_storage.h"

namespace sg
{
   namespace dc
   {
      FusedContourStorage::FusedContourList::iterator FusedContourStorage::push_back(Fused_Contour_T &&contour)
      {
         return m_fused_contours.push_back(std::move(contour));
      }

      FusedContourStorage::FusedContourList::iterator FusedContourStorage::begin() const
      {
         return m_fused_contours.begin();
      }

      FusedContourStorage::FusedContourList::iterator FusedContourStorage::end() const
      {
         return m_fused_contours.end();
      }

      std::size_t FusedContourStorage::size() const
      {
         return m_fused_contours.size();
      }

      void FusedContourStorage::clear()
      {
         return m_fused_contours.clear();
      }
   }
}

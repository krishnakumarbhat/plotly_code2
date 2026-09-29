/*===================================================================================*\
* FILE: dc_contour.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file implements DC_Contour_T class which holds subsegments.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_contour.h"

namespace sg
{
   namespace dc
   {
      DC_Contour_T::DC_Contour_T() : subsegments{}, drivability{SG_Drivability_Class_T::UNCLASSIFIED}, m_id{INVALID_SEGMENT_ID}
      {
      }

      DC_Contour_T::DC_Contour_T(const std::initializer_list<Subsegment_T> init_list)
          : subsegments{init_list}, drivability{SG_Drivability_Class_T::UNCLASSIFIED}, m_id{INVALID_CONTOUR_ID}
      {
      }

      DC_Contour_T::DC_Contour_T(const SubsegmentList &_subsegments, const uint32_t _id, const SG_Drivability_Class_T _drivability)
          : drivability{_drivability}, m_id{_id}
      {
         subsegments = _subsegments;
      }

      DC_Contour_T::DC_Contour_T(const DC_Contour_T &in_contour)
      {
         subsegments = in_contour.subsegments;
         copy_fields(in_contour);
      }

      DC_Contour_T::DC_Contour_T(DC_Contour_T &&in_contour) noexcept
      {
         subsegments = std::forward<SubsegmentList>(in_contour.subsegments); // Call move assignment operator of EmbeddedList
         copy_fields(in_contour);
         reset_fields(in_contour);
      }

      DC_Contour_T &DC_Contour_T::operator=(const DC_Contour_T &source) noexcept
      {
         subsegments = source.subsegments;
         copy_fields(source);
         return *this;
      }

      DC_Contour_T &DC_Contour_T::operator=(DC_Contour_T &&source) noexcept
      {
         subsegments = std::forward<SubsegmentList>(source.subsegments); // Call move assignment operator of EmbeddedList
         copy_fields(source);
         reset_fields(source);
         return *this;
      }

      size_t DC_Contour_T::size() const
      {
         return static_cast<size_t>(subsegments.size());
      }

      void DC_Contour_T::clear()
      {
         subsegments.clear();
         reset_fields(*this);
      }

      uint32_t DC_Contour_T::get_id() const
      {
         return m_id;
      }

      void DC_Contour_T::set_id(const uint32_t id)
      {
         m_id = id;
      }

      uint16_t DC_Contour_T::get_num_of_vertices() const
      {
         if (subsegments.empty())
         {
            return 0U;
         }
         else
         {
            return static_cast<uint16_t>(subsegments.size() + 1U);
         }
      }

      SG_Drivability_Class_T DC_Contour_T::get_drivability() const
      {
         return drivability;
      }

      void DC_Contour_T::set_drivability(const SG_Drivability_Class_T _drivability)
      {
         drivability = _drivability;
      }

      void DC_Contour_T::copy_fields(const DC_Contour_T &source)
      {
         m_id        = source.m_id;
         drivability = source.drivability;
      }

      void DC_Contour_T::reset_fields(DC_Contour_T &in_contour)
      {
         in_contour.m_id        = INVALID_SEGMENT_ID;
         in_contour.drivability = SG_Drivability_Class_T::UNCLASSIFIED;
      }
   }
}

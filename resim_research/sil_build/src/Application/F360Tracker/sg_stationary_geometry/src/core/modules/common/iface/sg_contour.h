/*===================================================================================*\
* FILE: sg_contour.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of Contour_T.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_CONTOUR_H
#define SG_CONTOUR_H

#include "embedded_list.h"
#include "sg_constants.h"
#include "sg_drivability_class.h"
#include "sg_vertex.h"

namespace sg
{
   // class for storage metadata information needed for effective iterating over contours and vertices
   class Contour_T
   {
     public:
      using VertexList = EmbeddedList<Vertex_T, SG_MAX_NUM_VERTICES>;
      friend class ContourStorage;

      VertexList vertices;
      float priority;
      uint16_t cluster_id;
      SG_Drivability_Class_T drivability;
      bool f_selected_for_output;

      Contour_T()
          : vertices{},
            priority{},
            cluster_id{INVALID_CLUSTER_ID},
            drivability{SG_Drivability_Class_T::UNCLASSIFIED},
            f_selected_for_output{false},
            m_unique_id{INVALID_CONTOUR_ID}
      {
         (void) m_padding; // unsued variable
      };

      Contour_T(std::initializer_list<Vertex_T> init_list)
          : vertices{init_list},
            priority{},
            cluster_id{INVALID_CLUSTER_ID},
            drivability{SG_Drivability_Class_T::UNCLASSIFIED},
            f_selected_for_output{false},
            m_unique_id{INVALID_CONTOUR_ID} {};

      Contour_T(VertexList &_vertices,
                const uint16_t _cluster_id,
                const float _reliability,
                const float _position_covariance,
                const SG_Drivability_Class_T _drivability)
      {
         this->vertices = std::move(_vertices);

         for (auto &vertex : this->vertices)
         {
            vertex.pos_cov.x   = _position_covariance;
            vertex.pos_cov.y   = _position_covariance;
            vertex.reliability = _reliability;
         }

         this->priority              = {};
         this->cluster_id            = _cluster_id;
         this->drivability           = _drivability;
         this->f_selected_for_output = false;
         this->m_unique_id           = INVALID_CONTOUR_ID;
      }

      // Copy constructor
      Contour_T(const Contour_T &in_contour)
      {
         this->vertices = in_contour.vertices;
         copy_fields(in_contour);
      }

      // Move constructor
      Contour_T(Contour_T &&in_contour) noexcept
      {
         this->vertices = std::move(in_contour.vertices); // Call move assignment operator of EmbeddedList
         copy_fields(in_contour);
         reset_fields(in_contour);
      }

      // Assignment operator
      Contour_T &operator=(const Contour_T &source) noexcept
      {
         this->vertices = source.vertices;
         copy_fields(source);
         return *this;
      }

      // Move assignment operator
      Contour_T &operator=(Contour_T &&source) noexcept
      {
         this->vertices = std::move(source.vertices); // Call move assignment operator of EmbeddedList
         copy_fields(source);
         reset_fields(source);
         return *this;
      }

      uint16_t size() const
      {
         return static_cast<uint16_t>(vertices.size());
      }

      void clear()
      {
         vertices.clear();
         reset_fields(*this);
      }

      uint32_t unique_id() const
      {
         return m_unique_id;
      }

     private:
      uint32_t m_unique_id;
      uint8_t m_padding[4]{}; // to get the constant size for all compilers

      void copy_fields(const Contour_T &source)
      {
         this->priority              = source.priority;
         this->cluster_id            = source.cluster_id;
         this->drivability           = source.drivability;
         this->f_selected_for_output = source.f_selected_for_output;
         this->m_unique_id           = source.m_unique_id;
      }

      void reset_fields(Contour_T &in_contour)
      {
         in_contour.priority              = 0.0F;
         in_contour.cluster_id            = INVALID_CLUSTER_ID;
         in_contour.drivability           = SG_Drivability_Class_T::UNCLASSIFIED;
         in_contour.f_selected_for_output = false;
         in_contour.m_unique_id           = INVALID_CONTOUR_ID;
      }
   };
}

#endif

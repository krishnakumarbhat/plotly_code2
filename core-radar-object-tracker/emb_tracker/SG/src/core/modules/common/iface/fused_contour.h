/*===================================================================================*\
* FILE: fused_contour.h
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

#ifndef FUSED_CONTOUR_H
#define FUSED_CONTOUR_H

#include "embedded_list.h"
#include "fused_vertex.h"
#include "sg_reuse.h"
#include "sg_variant.h"

namespace sg
{
class Fused_Contour_T
{
 public:
   using FusedVertexList = EmbeddedList<Fused_Vertex_T, SG_MAX_NUM_FUSED_VERTICES_PER_CONTOUR * SG_MAX_NUM_FUSED_CONTOURS>;
   FusedVertexList vertices;

 private:
   uint32_t m_id;

 public:
   uint16_t num_of_vertices;
   uint16_t cycles_since_created;
   SG_Drivability_Class_T sg_drivability; // drivability classification that was set by SG
   bool f_selected_for_output;

   // Default constructor for fused contours
   Fused_Contour_T();

   // Destructor for fused contours
   ~Fused_Contour_T();

   // Move constructor
   Fused_Contour_T(Fused_Contour_T &&in_contour) noexcept;

   // Copy constructor
   Fused_Contour_T(Fused_Contour_T &in_contour) noexcept;

   // Move assignment operator
   Fused_Contour_T &operator=(Fused_Contour_T &&source) noexcept;

   // Assignment operator
   Fused_Contour_T &operator=(const Fused_Contour_T &source) noexcept;
   /**
    * @brief   Initilializer list constructor of Fused_Vertex_T class. Initializes the contour with elements from source.
    *
    * @param[in]    init_list    list of fused subvertices
    **/
   Fused_Contour_T(const std::initializer_list<Fused_Vertex_T> init_list);

   /**
    * @brief    Constructor of Fused_Contour_T class. Initializes the contour with elements from source.
    *
    * @param[in]    in_vertices           list of subsegments, number of subsegments is number of vertices - 1
    * @param[in]    in_id                 unique_id of DC_Contour
    * @param[in]    in_num_of_vertices    number of vertices
    * @param[in]    in_cycles_since_created  number of cycles since contour have been created
    * @param[in]    in_sg_drivability        drivability classification that was set by SG
    * @param[in]    in_f_selected_for_output    flag indicating if contour was selected for output
    **/
   Fused_Contour_T(const FusedVertexList &in_vertices, const uint32_t in_id, const uint16_t in_num_of_vertices,
                   const uint16_t in_cycles_since_created, const SG_Drivability_Class_T in_sg_drivability,
                   const bool in_f_selected_for_output);

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
   void copy_fields(const Fused_Contour_T &source)
   {
      this->m_id                  = source.m_id;
      this->num_of_vertices       = source.num_of_vertices;
      this->cycles_since_created  = source.cycles_since_created;
      this->sg_drivability        = source.sg_drivability;
      this->f_selected_for_output = source.f_selected_for_output;
   }

   void reset_fields(Fused_Contour_T &in_contour)
   {
      in_contour.m_id                  = INVALID_CONTOUR_ID;
      in_contour.num_of_vertices       = 0U;
      in_contour.cycles_since_created  = 0U;
      in_contour.sg_drivability        = SG_Drivability_Class_T::NONDRIVABLE;
      in_contour.f_selected_for_output = false;
   }
};
} // namespace sg

#endif

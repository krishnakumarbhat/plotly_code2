/*===================================================================================*\
* FILE: dc_contour.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of DC_Contour_T class which holds subsegments.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_CONTOUR_H
#define DC_CONTOUR_H

#include "dc_subsegment.h"
#include "embedded_list.h"
#include "sg_constants.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      class DC_Contour_T
      {
        public:
         using SubsegmentList = EmbeddedList<Subsegment_T, SG_MAX_NUM_CONTOURS *(SG_MAX_NUM_SUBVERTICES_PER_CONTOUR - 1U)>;
         SubsegmentList subsegments;

         friend class DCContourStorage;

         DC_Contour_T();

         /**
          * @brief      Initilializer list constructor of DC_Contour_T class. Initializes the contour with elements from source.
          *
          * @param[in]  init_list     list of subsegments
          **/
         DC_Contour_T(const std::initializer_list<Subsegment_T> init_list);

         /**
          * @brief      Constructor of DC_Contour_T class. Initializes the contour with elements from source.
          *
          * @param[in]  _subsegments        list of subsegments, number of subsegments is number of vertices - 1
          * @param[in]  _id                 unique_id of DC_Contour
          * @param[in]  _drivability        drivability class
          **/
         DC_Contour_T(const SubsegmentList &_subsegments, const uint32_t _id, const SG_Drivability_Class_T _drivability);

         /**
          * @brief      Copy Constructor. Initializes the contour with elements from source.
          *
          * @param[in]  in_contour    DC_Contour
          **/
         DC_Contour_T(const DC_Contour_T &in_contour);

         /**
          * @brief      Move Constructor. Initializes the contour with elements from source.
          *
          * @param[in]  in_contour    DC_Contour
          **/
         DC_Contour_T(DC_Contour_T &&in_contour) noexcept;

         /**
          * @brief      Assignment operator. Assign the contour elements from source.
          *
          * @param[in]  in_contour    DC_Contour
          **/
         DC_Contour_T &operator=(const DC_Contour_T &source) noexcept;

         /**
          * @brief      Move assignment operator. Move the contour elements from source.
          *
          * @param[in]  in_contour    DC_Contour
          **/
         DC_Contour_T &operator=(DC_Contour_T &&source) noexcept;

         /**
          * @brief      Get size of storage list.
          *
          * @return     size of EmbeddedList of subsegments
          **/
         size_t size() const;

         /**
          * @brief      Clear all fields.
          **/
         void clear();

         /**
          * @brief      Get unique_id of DC_Contour
          *
          * @return     unique_id of DC_Contour
          **/
         uint32_t get_id() const;

         /**
          * @brief      Set unique_id of DC_Contour
          *
          * @param[in]  id new contour id
          **/
         void set_id(const uint32_t id);

         /**
          * @brief      Get number of vertices
          *
          * @return     number of vertices
          **/
         uint16_t get_num_of_vertices() const;

         /**
          * @brief      Get drivability
          *
          * @return     drivability
          **/
         SG_Drivability_Class_T get_drivability() const;

         /**
          * @brief      Set drivability
          *
          * @param[in]  _drivability drivability
          **/
         void set_drivability(const SG_Drivability_Class_T _drivability);

        private:
         SG_Drivability_Class_T drivability;
         uint32_t m_id;
         /**
          * @brief      Copy all fields from source.
          **/
         void copy_fields(const DC_Contour_T &source);

         /**
          * @brief      Resets all fields from in_contour.
          **/
         static void reset_fields(DC_Contour_T &in_contour);
      };
   }
}
#endif

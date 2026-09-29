/*===================================================================================*\
* FILE: dc_fused_contour_storage.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of FusedContourStorage class - the container for Fused_Contour_T objects.
*   Fused_Contour_T objects are stored in FusedContourList which has type of EmbeddedList.
*   Single Fused_Contour_T object contains list of vertices (of type EmbeddedList) belonging to it.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_FUSED_CONTOUR_STORAGE_H
#define DC_FUSED_CONTOUR_STORAGE_H

#include "dc_fused_contour.h"
#include "embedded_list.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      class FusedContourStorage
      {
        public:
         using FusedContourList = EmbeddedList<Fused_Contour_T, SG_MAX_NUM_FUSED_CONTOURS>;

         /**
          * @brief       Adds fused contour to the end of the container.
          *
          * @param[in]   contour - contour that will be added to FusedContourStorage
          *
          * @return      iterator to last added contour
          **/
         FusedContourList::iterator push_back(Fused_Contour_T &&contour);

         /**
          * @brief    Provides bidirectional iterator to first element of m_fused_contours list
          *
          * @return   bidirectional iterator pointing to the first element of fused contour list.
          **/
         FusedContourList::iterator begin() const;

         /**
          * @brief    Provides bidirectional iterator to place after last element of list
          *
          * @return   iterator pointing to the one after last element of fused contour list.
          **/
         FusedContourList::iterator end() const;

         /**
          * @brief    Returns total number of fused contours
          *
          * @return   size_t
          **/
         std::size_t size() const;

         /**
          * @brief   Removes all items from contour list.
          **/
         void clear();

        private:
         /// Double linked list of fused contours managed by contour storage.
         FusedContourList m_fused_contours;
      };
   }
}

#endif
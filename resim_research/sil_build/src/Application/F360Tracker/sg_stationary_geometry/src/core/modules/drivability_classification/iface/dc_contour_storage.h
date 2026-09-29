/*===================================================================================*\
* FILE: dc_contour_storage.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of DCContourStorage class - the container for DC_Contour_T objects.
*   DC_Contour_T objects are stored in ContourList which has type of EmbeddedList.
*   Single DC_Contour_T object contains list of subsegments (of type EmbeddedList) belonging to it.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_CONTOUR_STORAGE_H
#define DC_CONTOUR_STORAGE_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4820)
#endif

#ifdef SG_SAVE_DETECTIONS_ASSIGNED_TO_SUBSEGMENTS
#include <unordered_map>
#endif

#include "dc_contour.h"
#include "dc_dump.h"
#include "dc_subsegment_detections.h"
#include "embedded_list.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      class DCContourStorage
      {
        public:
         using ContourList = EmbeddedList<DC_Contour_T, SG_MAX_NUM_CONTOURS>;

         static const uint32_t MAX_ID_RESET_VALUE;

#ifdef SG_SAVE_DETECTIONS_ASSIGNED_TO_SUBSEGMENTS
         static std::unordered_map<uint32_t, SubsegmentDetections_T> assigned_detections;
#endif

         /**
          * @brief       Adds contour to the end of the container and assigns a unique id.
          *
          * @param[in]   contour - contour that will be added to DCContourStorage
          *
          * @return      bool indicating whether contour was added to DCContourStorage or not
          *
          * @retval      true - contour added to DCContourStorage
          * @retval      false - contour not added to DCContourStorage, because it was full
          **/
         DCContourStorage::ContourList::iterator push_back(DC_Contour_T &&contour);

         /**
          * @brief    Copying contours is not allowed.
          *
          * @return   None
          **/
         DCContourStorage::ContourList::iterator push_back(DC_Contour_T &contour) = delete;

         /**
          * @brief       Adds contour to the end of the container and assigns provided id.
          *
          * @param[in]   contour - contour that will be added to DCContourStorage
          * @param[in]   id - id to be assigned to contour
          *
          * @return      iterator to the added element
          **/
         DCContourStorage::ContourList::iterator push_back(DC_Contour_T &&contour, const uint32_t id);

         /**
          * @brief    Copying contours is not allowed.
          *
          * @return   None
          **/
         DCContourStorage::ContourList::iterator push_front(DC_Contour_T &contour) = delete;

         /**
          * @brief    Get number of elements currently stored in contour list
          *
          * @return   number of elements currently stored in list
          **/
         inline std::size_t size() const
         {
            return m_contours.size();
         }

         /**
          * @brief    Get maximum number of contours that can be stored in contour list.
          *
          * @return   maximum number of contours that can be stored in contour list
          **/
         inline std::size_t capacity() const
         {
            return m_contours.capacity();
         }

         /**
          * @brief    Check if contour container is empty (doesn't contain any elements).
          *
          * @return   Returns true if no items in m_contours, otherwise returns false.
          **/
         inline bool empty() const
         {
            return m_contours.empty();
         }

         /**
          * @brief   Removes all items from contour list and resets available ids.
          **/
         void clear();


         /**
          * @brief   Removes all items from contour list.
          **/
         void reset_contour_list();

         /**
          * @brief   Returns total number of verices contained in all contours in DCContourStorage.
          *
          * @return  total number of subsegments
          **/
         std::size_t total_subsegments_number() const;

         /**
          * @brief   Returns the first available subsegment_id in DCContourStorage
          *
          * @return  id - the first available subsegment_id
          **/
         uint32_t get_available_subsegment_id();

         /**
          * @brief      Sets the first available subsegment_id in DCContourStorage
          *
          * @param[in]  id - available subsegment id
          **/
         void set_available_subsegment_id(const uint32_t id);

         /**
          * @brief   Returns the first available contour_id in DCContourStorage
          *
          * @return  id - the first available contour_id
          **/
         uint32_t get_available_contour_id();

         /**
          * @brief      Sets the first available contour_id in DCContourStorage
          *
          * @param[in]  id - available contour id
          **/
         void set_available_contour_id(const uint32_t id);

         /// Adds sort functionality
         template <typename Compare>
         void sort(Compare comp = Compare{});

         /**
          * @brief    Provides bidirectional iterator to first element of m_contours list
          *
          * @return   bidirectional iterator pointing to the first element of contour list.
          **/
         inline DCContourStorage::ContourList::iterator begin() const
         {
            return m_contours.begin();
         }

         /**
          * @brief    Provides bidirectional iterator to place after last element of list
          *
          * @return   iterator pointing to the one after last element of contour list.
          **/
         inline DCContourStorage::ContourList::iterator end() const
         {
            return m_contours.end();
         }

         /**
          * @brief   Returns reverse bidirectional iterator pointing to the last element in DCContourStorage.
          *
          * @return  reverse bidirectional iterator pointing to the last element in DCContourStorage
          **/
         inline DCContourStorage::ContourList::reverse_iterator rbegin() const
         {
            return m_contours.rbegin();
         }

         /**
          * @brief   Reverse iterator pointing to the one before first element in DCContourStorage.
          *
          * @return  reverse iterator pointing to the one before first element in DCContourStorage
          **/
         inline DCContourStorage::ContourList::reverse_iterator rend() const
         {
            return m_contours.rend();
         }

         /**
          * @brief       Initialize object's state from dumped data.
          *
          * @param[in]   dumped_dc
          **/
         void initialize(const DC_Dump_T &dumped_dc);

        private:
         /// Double linked list of contours managed by contour storage.
         ContourList m_contours;

         /// Handles unique id of contour.
         uint32_t available_contour_id{0U};

         /// Handles unique id of subsegments.
         uint32_t available_subsegment_id{0U};
      };

      /**
       * @brief   Sorts ContourList with given compare statement.
       *
       * @param   comp - compare statement
       **/
      template <typename Compare>
      inline void DCContourStorage::sort(Compare comp)
      {
         m_contours.sort(comp);
      }
   }
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif

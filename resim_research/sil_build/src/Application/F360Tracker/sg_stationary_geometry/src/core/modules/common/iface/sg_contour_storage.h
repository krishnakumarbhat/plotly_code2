/*===================================================================================*\
* FILE: sg_contour_storage.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of ContourStorage class - the container for Contour_T objects.
*   Contous_T objects are stored in ContourList which has type of EmbeddedList.
*   Single Contour_T object contains list of vertices (of type EmbeddedList) belonging to it.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_CONTOUR_STORAGE_H
#define SG_CONTOUR_STORAGE_H

#include "embedded_list.h"
#include "id_handler_incremental.h"
#include "sg_contour.h"
#include "sg_contour_storage_dump.h"
#include "sg_reuse.h"
#include "unique_id_handler.h"

namespace sg
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4820)
#endif

   class ContourStorage
   {
     public:
      using ContourList      = EmbeddedList<Contour_T, SG_MAX_NUM_CONTOURS>;
      using ContourIdHandler = UniqueIdHandler<uint32_t, 1U, SG_MAX_NUM_CONTOURS>;

      /**
       * @brief    Constructor
       *
       * @param    None
       * @return   None
       **/
      ContourStorage() = default;

      /**
       * @brief    Copy construction is not allowed.
       *
       * @param    const ContourStorage&
       *
       * @return   None
       **/
      ContourStorage(const ContourStorage &) = delete;

      /**
       * @brief    Copy assignment is not allowed.
       *
       * @param    ContourStorage&
       *
       * @return   None
       **/
      ContourStorage &operator=(ContourStorage &) = delete;

      /**
       * @brief       Adds contour to the end of the container and assigns a unique id.
       *
       * @param[in]   contour - contour that will be added to ContourStorage
       *
       * @return      bool indicating whether contour was added to ContourStorage or not
       *
       * @retval      iterator pointing to contour
       * @retval      iterator pointing to dummy end, indicating that contour was not added to ContourStorage
       **/
      ContourStorage::ContourList::iterator push_back(Contour_T &&contour);

      /**
       * @brief    Copying contours is not allowed.
       *
       * @return   None
       **/
      ContourStorage::ContourList::iterator push_back(Contour_T &contour) = delete;

      /**
       * @brief       Adds contour to the front of the container and assigns its unique id.
       *
       * @param[in]   contour - contour that will be added to ContourStorage
       *
       * @return      iterator pointing to contour or nullptr depending on whether contour was added to ContourStorage or not
       *
       * @retval      iterator pointing to contour
       * @retval      nullptr - null pointer, indicating that contour was not added to ContourStorage
       **/
      ContourStorage::ContourList::iterator push_front(Contour_T &&contour);

      /**
       * @brief    Copying contours is not allowed.
       *
       * @return   None
       **/
      ContourStorage::ContourList::iterator push_front(Contour_T &contour) = delete;

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
       * @brief   Check if contour container is empty (doesn't contain any elements).
       *
       * @return   Returns true if no items in m_contours, otherwise returns false.
       **/
      inline bool empty() const
      {
         return m_contours.empty();
      }

      /**
       * @brief   Removes all items from contour list.
       **/
      inline void clear()
      {
         m_id_handler.reset();
         segment_id_handler.reset();
         m_contours.clear();
      }

      /**
       * @brief   Returns total number of verices contained in all contours in ContourStorage.
       *
       * @return  total number of verices
       **/
      std::size_t total_vertices_number() const;

      /**
       * @brief    Erase specific element from node in m_contours. After node has been erased double linked list keeps integrity.
       *
       * @param    current - forward iterator to element to erase
       *
       * @return   bidirectional iterator to position of erased node.
       **/
      ContourList::iterator erase(const ContourList::iterator current);

      /// Adds sort functionality
      template <typename Compare>
      void sort(Compare comp = Compare{});

      /**
       * @brief    Provides bidirectional iterator to first element of m_contours list
       *
       * @return   bidirectional iterator pointing to the first element of contour list.
       **/
      inline ContourStorage::ContourList::iterator begin() const
      {
         return m_contours.begin();
      }

      /**
       * @brief    Provides bidirectional iterator to place after last element of list
       *
       * @return   iterator pointing to the one after last element of contour list.
       **/
      inline ContourStorage::ContourList::iterator end() const
      {
         return m_contours.end();
      }

      /**
       * @brief   Returns reverse bidirectional iterator pointing to the last element in ContourStorage.
       *
       * @return  reverse bidirectional iterator pointing to the last element in ContourStorage
       **/
      inline ContourStorage::ContourList::reverse_iterator rbegin() const
      {
         return m_contours.rbegin();
      }

      /**
       * @brief   Reverse iterator pointing to the one before first element in ContourStorage.
       *
       * @return  reverse iterator pointing to the one before first element in ContourStorage
       **/
      inline ContourStorage::ContourList::reverse_iterator rend() const
      {
         return m_contours.rend();
      }

      /**
       * @brief       Initialize object's state from dumped data.
       *
       * @param[in]   dumped_internal_contours
       **/
      void initialize(const SG_Contour_Storage_Dump_T &dumped_internal_contours);

      /// Handles unique ids of contours subsegments
      IdHandlerIncremental<uint32_t> segment_id_handler;

     private:
      /// Double linked list of contours managed by contour storage.
      ContourList m_contours;

      /// Handles unique ids of ContourStorage.
      ContourIdHandler m_id_handler;
   };

   /**
    * @brief   Sorts ContourList with given compare statement.
    *
    * @param   comp - compare statement
    **/
   template <typename Compare>
   inline void ContourStorage::sort(Compare comp)
   {
      m_contours.sort(comp);
   }
#ifdef _MSC_VER
#pragma warning(pop)
#endif
}
#endif

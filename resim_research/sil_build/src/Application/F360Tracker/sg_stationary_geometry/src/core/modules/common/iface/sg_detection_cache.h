/*===================================================================================*\
* FILE: sg_detection_cache.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definitions of DetectionCache class.
*
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef SG_DETECTION_CACHE
#define SG_DETECTION_CACHE

#include <algorithm>

#include "sg_constants.h"
#include "sg_detection.h"

namespace sg
{

   class DetectionCache
   {
     public:
      using collection_data_type = std::array<Detection_T *, SG_MAX_NUM_INTERNAL_DETS>;

      /**
       * @brief    Constructor.
       *
       **/
      DetectionCache() = default;

      /**
       * @brief       Adds detection to m_cache
       *
       * @param[in]   det_ptr - detection pointer
       *
       * @return      N/A
       **/
      void add_detection(Detection_T *const det_ptr)
      {
         if (det_ptr != nullptr)
         {
            if (m_size < SG_MAX_NUM_INTERNAL_DETS)
            {
               m_cache[m_size] = det_ptr;
               ++m_size;
            }
            else
            {
               assert(false);
            }
         }
      }

      /**
       * @brief    Removes detection from m_cache
       *
       * @param    detection pointer
       *
       * @return   None
       **/
      void remove_detection(const Detection_T *const det_ptr);

      /**
       * @brief    const overloaded operator[]
       *
       * @param    position
       *
       * @return   detection pointer
       **/
      Detection_T *const &operator[](const std::size_t _Pos) const
      {
         return m_cache[_Pos];
      }

      /**
       * @brief    Returns iterator pointing to the first element in m_cache
       *
       * @param    None
       *
       * @return   iterator
       **/
      collection_data_type::iterator begin()
      {
         return m_cache.begin();
      }

      /**
       * @brief    Returns iterator pointing to the one after last element in m_cache
       *
       * @param    None
       *
       * @return   iterator
       **/
      collection_data_type::iterator end()
      {
         return std::next(m_cache.begin(), static_cast<std::ptrdiff_t>(m_size));
      }

      /**
       * @brief    Returns const iterator pointing to the first element in m_cache
       *
       * @param    None
       *
       * @return   const iterator
       **/
      collection_data_type::const_iterator cbegin() const
      {
         return m_cache.cbegin();
      }

      /**
       * @brief    Returns const iterator pointing to the one after last element in m_cache
       *
       * @param    None
       *
       * @return   const iterator
       **/
      collection_data_type::const_iterator cend() const
      {
         return std::next(m_cache.cbegin(), static_cast<std::ptrdiff_t>(m_size));
      }

      /**
       * @brief    Clears the m_cache
       *
       * @param    None
       *
       **/
      void clear()
      {
         std::fill_n(m_cache.begin(), m_size, nullptr);
         m_size = 0U;
      }

      /**
       * @brief    Returns number of elements in m_cache
       *
       * @param    None
       *
       * @return   number of elements
       **/
      std::size_t size() const
      {
         return m_size;
      }

     private:
      collection_data_type m_cache{};
      std::size_t m_size{0U};
   };
}

#endif /* SG_DETECTION_CACHE */

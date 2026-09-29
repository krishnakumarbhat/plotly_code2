/*===================================================================================*\
* FILE: dc_array_wrapper.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file implements class that holds elements of generic type in the array
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_array_wrapper.h"

#include <iterator>

#include "dc_contour.h"
#include "geometry/geo_point.h"

namespace sg
{
   namespace dc
   {
      template <typename T, std::size_t N>
      ArrayWrapper<T, N>::ArrayWrapper() : m_array{}, m_num_elements{0U}
      {
      }

      template <typename T, std::size_t N>
      void ArrayWrapper<T, N>::add_element(const T &element)
      {
         if (!is_full())
         {
            m_array[m_num_elements++] = element;
         }
      }

      template <typename T, std::size_t N>
      typename ArrayWrapper<T, N>::ArrayWrapperType::iterator ArrayWrapper<T, N>::begin()
      {
         return m_array.begin();
      }

      template <typename T, std::size_t N>
      typename ArrayWrapper<T, N>::ArrayWrapperType::iterator ArrayWrapper<T, N>::end()
      {
         return std::next(m_array.begin(), m_num_elements);
      }

      template <typename T, std::size_t N>
      typename ArrayWrapper<T, N>::ArrayWrapperType::const_iterator ArrayWrapper<T, N>::cbegin() const
      {
         return m_array.cbegin();
      }

      template <typename T, std::size_t N>
      typename ArrayWrapper<T, N>::ArrayWrapperType::const_iterator ArrayWrapper<T, N>::cend() const
      {
         return std::next(m_array.cbegin(), m_num_elements);
      }

      template <typename T, std::size_t N>
      typename ArrayWrapper<T, N>::ArrayWrapperType::reverse_iterator ArrayWrapper<T, N>::rbegin()
      {
         return std::make_reverse_iterator(end());
      }

      template <typename T, std::size_t N>
      typename ArrayWrapper<T, N>::ArrayWrapperType::reverse_iterator ArrayWrapper<T, N>::rend()
      {
         return m_array.rend();
      }

      template <typename T, std::size_t N>
      const T &ArrayWrapper<T, N>::operator[](const uint16_t position) const
      {
         return m_array[position];
      }

      template <typename T, std::size_t N>
      uint16_t ArrayWrapper<T, N>::get_num_elements() const
      {
         return m_num_elements;
      }

      template <typename T, std::size_t N>
      void ArrayWrapper<T, N>::clear()
      {
         std::fill_n(begin(), m_num_elements, T{});
         m_num_elements = 0U;
      }

      template <typename T, std::size_t N>
      bool ArrayWrapper<T, N>::is_full() const
      {
         return m_num_elements == N;
      }

      template class ArrayWrapper<geometry::Point2D_T>;

      template class ArrayWrapper<uint32_t>;

      template class ArrayWrapper<DC_Contour_T>;
   }
}
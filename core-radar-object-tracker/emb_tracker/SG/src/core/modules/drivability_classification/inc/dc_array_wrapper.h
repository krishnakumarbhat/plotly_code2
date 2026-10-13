/*===================================================================================*\
* FILE: dc_array_wrapper.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file declares class that holds elements of generic type in the array
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_ARRAY_WRAPPER_H
#define DC_ARRAY_WRAPPER_H

#include <algorithm>
#include <array>

#include "sg_variant.h"

namespace sg
{
namespace dc
{
template <typename T, std::size_t N = SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> class ArrayWrapper
{
   using ArrayWrapperType = std::array<T, N>;
   std::array<T, N> m_array{};
   uint16_t m_num_elements{};

 public:
   ArrayWrapper() = default;

   /**
    * @brief            This function adds generic type T to the m_array.
    *
    * @param[in]        element - generic type T to be hold in the m_array.
    **/
   void add_element(const T &element)
   {
      if (!is_full())
      {
         m_array[m_num_elements++] = element;
      }
   }

   auto begin()
   {
      return m_array.begin();
   }

   auto end()
   {
      return std::next(m_array.begin(), static_cast<std::ptrdiff_t>(m_num_elements));
   }

   auto cbegin() const
   {
      return m_array.cbegin();
   }

   auto cend() const
   {
      return std::next(m_array.cbegin(), static_cast<std::ptrdiff_t>(m_num_elements));
   }

   auto rbegin()
   {
      return std::make_reverse_iterator(end());
   }

   auto rend()
   {
      return m_array.rend();
   }

   const T &operator[](const uint16_t position) const
   {
      return m_array[position];
   }

   /**
    * @brief            This function checks if number of elements in array is equal to its max size
    *
    * @return           is number of elements in array equal to its max size
    **/
   bool is_full() const
   {
      return m_num_elements == N;
   }

   /**
    * @brief            This function returns number of elements in m_array.
    *
    * @return           number of elements in m_array.
    **/
   uint16_t get_num_elements() const
   {
      return m_num_elements;
   }

   /**
    * @brief            This function clears m_array to the default state.
    **/
   void clear()
   {
      std::fill_n(begin(), m_num_elements, T{});
      m_num_elements = 0U;
   }
};
} // namespace dc
} // namespace sg
#endif

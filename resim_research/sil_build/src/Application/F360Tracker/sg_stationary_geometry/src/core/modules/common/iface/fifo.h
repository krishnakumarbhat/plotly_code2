/*===================================================================================*\
* FILE: Fifo.h
*====================================================================================
*Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of Fifo class
* It provides Fifo queue functionality for C++ embedded types eg. int, float
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef FIFO_H
#define FIFO_H

#include <cstddef>

#include "sg_reuse.h"

template <typename DataType, std::size_t N>
class Fifo
{
  public:
   using size_type = std::size_t;

   /**
    * @brief    Default constructor
    *
    * @return   N/A
    **/
   Fifo() : m_size(), m_begin_index(), m_end_index(), m_buffer()
   {
   }

   /**
    * @brief    Returns top item in Fifo buffer
    *
    * @return   DataType
    **/
   const DataType &front() const
   {
      return m_buffer[m_begin_index];
   }

   /**
    * @brief    Removes top element from Fifo buffer. In place of removed element set 0. If size == 0 does nothing;
    *
    * @return   N/A
    **/
   void pop()
   {
      if ((size() > 0U) && (m_begin_index < N))
      {
         m_buffer[m_begin_index] = {};
         increment(m_begin_index);
         --m_size;
      }
   }

   /**
    * @brief    Adds element to end of Fifo buffer. If size == capacity does nothing
    *
    * @param[in]    DataType&
    *
    * @return   N/A
    **/
   void push(const DataType &item)
   {
      if ((size() < capacity()) && (m_end_index < N))
      {
         m_buffer[m_end_index] = item;
         increment(m_end_index);
         ++m_size;
      }
   }

   /**
    * @brief    Returns number of elements stored currently in Fifo buffer
    *
    * @return   size of the container
    **/
   size_type size() const
   {
      return m_size;
   }

   /**
    * @brief    Returns max number of elements that can be stored in Fifo buffer
    *
    * @return   capacity
    **/
   constexpr size_type capacity() const
   {
      return N;
   }

   /**
    * @brief    Returns TRUE if Fifo buffer is empty
    *
    * @return   is buffer empty
    **/
   bool empty() const
   {
      return 0U == m_size;
   }

  private:
   /**
    * @brief    Increments index mod N
    *
    * @param[in, out]  index
    *
    * @return   N/A
    **/
   static void increment(size_type &index)
   {
      index = (index == (N - 1U)) ? 0U : (index + 1U);
   }

   size_type m_size;
   size_type m_begin_index; // index of the first element in buffer
   size_type m_end_index;   // index of the first 'past the end' element in buffer
   DataType m_buffer[N];
};

#endif

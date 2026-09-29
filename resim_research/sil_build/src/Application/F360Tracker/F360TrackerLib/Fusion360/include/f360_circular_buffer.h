/*===================================================================================*\
* FILE: f360_circular_buffer.h
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of Circular_Buffer class
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_CIRCULAR_BUFFER_H
#define F360_CIRCULAR_BUFFER_H

#include <cstddef>
#include <algorithm>

#include "f360_iterator.h"

namespace f360_variant_A
{
namespace cmn
{

template<class T, std::size_t N>
struct Circular_Buffer
{
   using value_type = T;
   using size_type = std::size_t;
   using reference = value_type&;
   using const_reference = const value_type&;

   Circular_Buffer() :
      m_size(),
      m_begin_index(),
      m_end_index(),
      m_buffer()
   {}

   /*=========================================================================
   * Method         Circular_Buffer::size
   *
   * Description    returns number of elements currently in buffer
   *
   * Parameters     None
   * Returns        size_type - number of elements in buffer
   *
   * Externals:     None.
   *
   * Precondition   None.
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   size_type size() const
   {
      return m_size;
   }


   /*=========================================================================
   * Method         Circular_Buffer::capacity
   *
   * Description    returns max number of elements in buffer
   *
   * Parameters     None
   * Returns        size_type - max number of elements in buffer
   *
   * Externals:     None.
   *
   * Precondition   None.
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   size_type capacity() const
   {
      return N;
   }

private:
   /*=========================================================================
   * Method         Circular_Buffer::increment
   *
   * Description    increments index mod N
   *
   * Parameters     size_type index - index to be incremented
   * Returns        None
   *
   * Externals:     None.
   *
   * Precondition   None.
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   static void increment(size_type& index)
   {
      index = (index == (N - 1U)) ? 0U : index + 1U;
   }

public:

   /*=========================================================================
   * Method         Circular_Buffer::push 
   *
   * Description    adds element to the buffer. If size == capacity the oldest element is overwritten with new value
   *
   * Parameters     const_reference value - element to be added to the buffer
   * Returns        None
   *
   * Externals:     None.
   *
   * Precondition   None.
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void push(const_reference value)
   {
      m_buffer[m_end_index] = value;
      increment(m_end_index);
      if (this->size() == this->capacity())
      {
         increment(m_begin_index);
      }
      else
      {
         ++m_size;
      }
   }

   /*=========================================================================
   * Method         Circular_Buffer::operator[]
   *
   * Description    enables access to elements in the buffer. Oldest element is under index 0 and newest under index 'size - 1'
   *
   * Parameters     size_type index - index of element in the buffer
   * Returns        None
   *
   * Externals:     None.
   *
   * Precondition   Index has to be less than buffer size.
   * Postcondition  None.
   *
   * Note           If index is greater or equal to the buffer size behaviour is undefined.
   *========================================================================*/
   reference operator[](size_type index)
   {
      index = (m_begin_index + index) % N;
      return m_buffer[index];
   }

private:

   size_type m_size;
   size_type m_begin_index; //index of the first element in buffer
   size_type m_end_index;   //index of the first 'past the end' element in buffer
   value_type m_buffer[N];
};

}
}


#endif

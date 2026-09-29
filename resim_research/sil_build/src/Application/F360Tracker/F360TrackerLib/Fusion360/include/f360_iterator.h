/*===================================================================================*\
* FILE: f360_iterator.h
*====================================================================================
*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions of functions:
* cmn::begin()
* cmn::end()
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_ITERATOR_H
#define F360_ITERATOR_H

#include "f360_iterator_detail.h"


namespace f360_variant_A
{
   namespace cmn
   {
      /*===========================================================================*\
      * FUNCTION: begin()
      *===========================================================================
      * RETURN VALUE:
      * Iterator to first element in container or array
      *
      * PARAMETERS:
      * Reference to C style array. Array can have any number of dimensions.
      *
      * --------------------------------------------------------------------------
      * ABSTRACT:
      * --------------------------------------------------------------------------
      * This function returns iterator to the first element of the container or array.
      *
      \*===========================================================================*/

      template<class T, std::size_t N>
      typename detail::Iterator_For<T>::type begin(T(&array)[N])
      {
         return detail::begin_impl(array);
      }

      /*===========================================================================*\
      * FUNCTION: end()
      *===========================================================================
      * RETURN VALUE:
      * iterator to the end(i.e.the element after the last element) of the given container or array
      *
      * PARAMETERS:
      * Reference to C style array. Array can have any number of dimensions.
      *
      * --------------------------------------------------------------------------
      * ABSTRACT:
      * --------------------------------------------------------------------------
      * This function returns an iterator to the end(i.e.the element after the last element) of the given container or array.
      *
      \*===========================================================================*/

      template<class T, std::size_t N>
      typename detail::Iterator_For<T>::type end(T(&array)[N])
      {
         return detail::end_impl(array);
      }

   }
}

#endif

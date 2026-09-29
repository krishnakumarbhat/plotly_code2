/*===================================================================================*\
* FILE: f360_iterator_detail.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains implementation details for file f360_iterator.h
* Fonctions Located in this file should not be used outside of f360_iterator.h
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_ITERATOR_DETAIL_H
#define F360_ITERATOR_DETAIL_H

#include <cstddef>

namespace f360_variant_A
{
   namespace cmn
   {
      namespace detail
      {

         template<class T>
         struct Iterator_For
         {
            using type = T*;
         };

         template<class T, std::size_t N>
         struct Iterator_For<T[N]>
         {
            using type = typename Iterator_For<T>::type;
         };

         template<class T, std::size_t N>
         typename Iterator_For<T>::type begin_impl(T(&array)[N])
         {
            return reinterpret_cast<typename Iterator_For<T>::type>(&array[0]);
         }

         template<class T, std::size_t N>
         typename Iterator_For<T>::type end_impl(T(&array)[N])
         {
            return reinterpret_cast<typename Iterator_For<T>::type>(&array[N]);
         }

      }
   }
}

#endif

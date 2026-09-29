/*===================================================================================*\
* FILE: sg_interpolator.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Interpolator class declaration.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_INTERPOLATOR_H
#define SG_INTERPOLATOR_H

#include <cassert>
#include <cmath>
#include <limits>
#include <type_traits>

namespace sg
{
   class Interpolator
   {
     public:
      Interpolator(const float arg_min, const float arg_max, const float arg_current)
          : m_arg_difference(arg_max - arg_min), m_arg_current_difference(arg_current - arg_min)
      {
         assert((arg_min < arg_max) && (arg_min <= arg_current) && (arg_current <= arg_max));
      }

      template <typename ValueType>
      ValueType interpolate(const ValueType value_min, const ValueType value_max) const;

     private:
      const float m_arg_difference;
      const float m_arg_current_difference;
   };

   template <>
   inline float Interpolator::interpolate<float>(const float value_min, const float value_max) const
   {
      assert(std::fabs(m_arg_difference) >= std::numeric_limits<float>::epsilon());

      const float value_diff = value_max - value_min;
      return ((m_arg_current_difference / m_arg_difference) * value_diff) + value_min;
   }

   template <typename ValueType>
   inline ValueType Interpolator::interpolate(const ValueType value_min, const ValueType value_max) const
   {
      static_assert(std::is_integral<ValueType>::value, "ValueType must be integral");
      assert(std::fabs(m_arg_difference) >= std::numeric_limits<float>::epsilon());

      const float value_max_f  = static_cast<float>(value_max);
      const float value_min_f  = static_cast<float>(value_min);
      const float value_diff_f = value_max_f - value_min_f;
      float return_value_tmp   = ((m_arg_current_difference / m_arg_difference) * value_diff_f) + value_min_f;
      return_value_tmp         = floorf(return_value_tmp + 0.5F);
      return static_cast<ValueType>(return_value_tmp);
   }
}
#endif
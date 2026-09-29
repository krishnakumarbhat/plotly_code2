/*===================================================================================*\
* FILE: f360_interval.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of Interval struct.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#ifndef F360_INTERVAL_H
#define F360_INTERVAL_H

#include "f360_reuse.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   template<typename value_type>
   class Interval
   {
   public:
      Interval() : lower(), upper() {};
      Interval(const value_type min_val, const value_type max_val) : lower(min_val), upper(max_val) {};

      value_type lower;
      value_type upper;

      bool Contains(const value_type value) const;
   };

   /*=========================================================================
   * Method         Interval<value_type>::Contains(const value_type value)
   *
   * Description    Determine if value given as input parameter is inside interval
   *                (lower bound inclusive, upper bound exclusive)
   *
   * Parameters:
   * const value_type value - value to be compared with interval bounds
   *
   * Returns:
   * bool - boolean indicating if value is inside interval or not
   *
   * Externals:      None.
   *
   * Precondition    None.
   *
   * Postcondition   None.
   *
   * Note            None.
   *========================================================================*/
   template<typename value_type>
   inline bool Interval<value_type>::Contains(const value_type value) const
   {
      const bool f_in_bounds = Is_In_Bounds(value, this->lower, this->upper);
      return f_in_bounds;
   }
}

#endif

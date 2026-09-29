/*===================================================================================*\
* FILE: f360_angle.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of Angle class.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#ifndef F360_ANGLE_H
#define F360_ANGLE_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_math.h"

namespace f360_variant_A
{
   class Angle
   {
   private:
      float32_t raw_value; // [rad] raw_value of angle
      float32_t sine;     // [-] sine of this->raw_value
      float32_t cosine;   // [-] cosine of this->raw_value

   public:

      // Constructors
      Angle();
      explicit Angle(const float32_t);
      Angle(const float32_t, const float32_t, const float32_t);

      // Destructor
      ~Angle() = default;

      // Setters
      Angle& Value(const float32_t);

      // Getters
      inline float32_t Value() const { return this->raw_value; }
      inline float32_t Value_Deg() const { return F360_RAD2DEG(this->raw_value); }
      inline float32_t Sin() const { return this->sine; } // Returns sine of angle value.
      inline float32_t Cos() const { return this->cosine; } // Returns cosine of angle value.

      // Transformations
      Angle& Negate();
      Angle& Normalize(const float32_t interval_center = 0.0F);

      // Operators
      Angle& operator-=(const Angle& right);
      Angle& operator+=(const Angle& right);
      Angle operator+(const Angle& right) const;
      Angle operator-(const Angle& right) const;
      Angle operator-() const;

   private:
      static inline float32_t Sin_Sum(const Angle& left, const Angle& right) { return left.sine * right.cosine + left.cosine * right.sine; }
      static inline float32_t Cos_Sum(const Angle& left, const Angle& right) { return left.cosine * right.cosine - left.sine * right.sine; }
      static inline float32_t Sin_Diff(const Angle& left, const Angle& right) { return left.sine * right.cosine - left.cosine * right.sine; }
      static inline float32_t Cos_Diff(const Angle& left, const Angle& right) { return left.cosine * right.cosine + left.sine * right.sine; }

   };

   inline Angle operator-(const Angle& left, const float32_t right) { return Angle{ left.Value() - right }; }
   inline Angle operator-(const float32_t left, const Angle& right) { return Angle{ left - right.Value() }; }
   inline Angle operator+(const Angle& left, const float32_t right) { return Angle{ left.Value() + right }; }
   inline Angle operator+(const float32_t left, const Angle& right) { return Angle{ left + right.Value() }; }

   // Angle vs Angle operators
   inline bool operator<(const Angle& left, const Angle& right) { return left.Value() < right.Value(); }
   inline bool operator<=(const Angle& left, const Angle& right) { return left.Value() <= right.Value(); }
   inline bool operator>(const Angle& left, const Angle& right) { return left.Value() > right.Value(); }
   inline bool operator>=(const Angle& left, const Angle& right) { return left.Value() >= right.Value(); }
   inline bool operator==(const Angle& left, const Angle& right) { return std::abs(left.Value() - right.Value()) < F360_EPSILON; }
   inline bool operator!=(const Angle& left, const Angle& right) { return !(left == right); }

   // Angle vs float operators
   inline bool operator<(const Angle& left, const float32_t right) { return left.Value() < right; }
   inline bool operator<=(const Angle& left, const float32_t right) { return left.Value() <= right; }
   inline bool operator>(const Angle& left, const float32_t right) { return left.Value() > right; }
   inline bool operator>=(const Angle& left, const float32_t right) { return left.Value() >= right; }
   inline bool operator==(const Angle& left, const float32_t right) { return std::abs(left.Value() - right) < F360_EPSILON; }
   inline bool operator!=(const Angle& left, const float32_t right) { return !(left == right); }

   // float vs Angle operators
   inline bool operator<(const float32_t left, const Angle& right) { return left < right.Value(); }
   inline bool operator<=(const float32_t left, const Angle& right) { return left <= right.Value(); }
   inline bool operator>(const float32_t left, const Angle& right) { return left > right.Value(); }
   inline bool operator>=(const float32_t left, const Angle& right) { return left >= right.Value(); }
   inline bool operator==(const float32_t left, const Angle& right) { return std::abs(left - right.Value()) < F360_EPSILON; }
   inline bool operator!=(const float32_t left, const Angle& right) { return !(left == right); }

}

#endif

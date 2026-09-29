/*===================================================================================*\
* FILE: f360_angle.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of Angle class methods.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/

#include "f360_angle.h"
#include <limits>

namespace f360_variant_A
{
   /*=========================================================================
   * Method          Angle::Angle()
   *
   * Description     Default constructor. Creates angle with value 0.0. 
   *
   * Parameters      None.
   *
   * Returns
   * Angle           Instance of Angle class.
   *
   * Externals:      None.
   *
   * Precondition    None.
   *
   * Postcondition   None.
   *
   * Note            None.
   *========================================================================*/
   Angle::Angle() : raw_value(0.0F), sine(0.0F), cosine(1.0F)
   {}

   /*=========================================================================
   * Method          Angle::Angle(const float32_t)
   *
   * Description     Constructor.
   *
   * Parameters:
   * const float32_t angle_value   angle value given in radians.
   *
   * Returns
   * Angle           Instance of Angle class.
   *
   * Externals:      None.
   *
   * Precondition    None.
   *
   * Postcondition   None.
   *
   * Note            None.
   *========================================================================*/
   Angle::Angle(const float32_t angle_value) : 
      raw_value(angle_value),
      sine(F360_Sinf(angle_value)), 
      cosine(F360_Cosf(angle_value))
   {
   }

   /*=========================================================================
   * Method          Angle::Angle(const float32_t, const float32_t, const float32_t)
   *
   * Description     Constructor.
   *
   * Parameters:
   * const float32_t angle_value   angle value given in radians.
   * const float32_t sin_val       sine value of given angle.
   * const float32_t cos_val       cosine value of given angle.
   *
   * Returns
   * Angle           Instance of Angle class.
   *
   * Externals:      None.
   *
   * Precondition    None.
   *
   * Postcondition   None.
   *
   * Note            TODO: This constructor is temporarily public due to roll out usage of this class.
                     It should be made private in future. It is assumed that given sine and
                     cosine values are correct. There is no check within this method.
                     Jira task for it: https://jiraprod.aptiv.com/browse/DFT-1522
   *========================================================================*/
   Angle::Angle(const float32_t val, const float32_t sin_val, const float32_t cos_val) 
      : raw_value(val), 
        sine(sin_val), 
        cosine(cos_val)
   {
   }

   /*=========================================================================
   * Method          Angle::Value(const float32_t)
   *
   * Description     Set angle value in radians.
   *
   * Parameters:
   * const float32_t angle_value   angle value given in radians.
   *
   * Returns         None.
   *
   * Externals:      None.
   *
   * Precondition    None.
   *
   * Postcondition   None.
   *
   * Note            None.
   *========================================================================*/
   Angle& Angle::Value(const float32_t angle_value)
   {
      this->raw_value = angle_value;
      this->sine = F360_Sinf(angle_value);
      this->cosine = F360_Cosf(angle_value);

      return *this;
   }

   /*=========================================================================
   * Method          Angle::Negate()
   *
   * Description     Transforms angle value to negative.
   *
   * Parameters:     None.
   *
   * Returns         A reference to the negated angle.
   *
   * Externals:      None.
   *
   * Precondition    None.
   *
   * Postcondition   None.
   *
   * Note            None.
   *========================================================================*/
   Angle& Angle::Negate()
   {
      this->raw_value = -this->raw_value;
      this->sine = -this->sine;

      return *this;
   }

   /*=========================================================================
   * Method          Angle::Normalize()
   *
   * Description     Normalize angle value to region [interval_center - PI, interval_center + PI].
   *
   * Parameters:
   * const float32_t interval_center - center of normalization region. Default = 0.0.
   *
   * Returns         A reference to the normalized angle.
   *
   * Externals:      None.
   *
   * Precondition    None.
   *
   * Postcondition   None.
   *
   * Note            None
   *========================================================================*/
   Angle& Angle::Normalize(const float32_t interval_center)
   {
      const float32_t temp_val = (this->raw_value + (F360_PI - interval_center)) * (0.5F * F360_1_PI);
      const float32_t mod_val = (temp_val - F360_Floorf(temp_val)) * (F360_2PI);
      this->raw_value = mod_val - (F360_PI - interval_center);
      
      return *this;
   }

   /*=========================================================================
   * Method          Angle::operator+(const Angle &)
   *
   * Description     Returns Angle instance which represents sum of two input angles.
   *
   * Parameters:
   * const Angle & right - angle which is added to *this.
   *
   * Returns         None.
   *
   * Externals:      None.
   *
   * Precondition    None.
   *
   * Postcondition   None.
   *
   * Note            None
   *========================================================================*/
   Angle Angle::operator+(const Angle & right) const
   {
      const float32_t new_val = this->raw_value + right.raw_value;
      const float32_t new_sine = Angle::Sin_Sum(*this, right);
      const float32_t new_cosine = Angle::Cos_Sum(*this, right);

      const Angle new_angle = Angle(new_val, new_sine, new_cosine);

      return new_angle;
   }

   /*=========================================================================
   * Method          Angle::operator+=(const Angle &)
   *
   * Description     Add given angle.
   *
   * Parameters:
   * const Angle & right - angle which is added to *this.
   *
   * Returns         None.
   *
   * Externals:      None.
   *
   * Precondition    None.
   *
   * Postcondition   None.
   *
   * Note            None
   *========================================================================*/
   Angle & Angle::operator+=(const Angle & right)
   {
      this->raw_value += right.raw_value;

      const float32_t new_sin = Angle::Sin_Sum(*this, right);
      const float32_t new_cos = Angle::Cos_Sum(*this, right);

      this->sine = new_sin;
      this->cosine = new_cos;

      return *this;
   }

   /*=========================================================================
   * Method          Angle::operator-(const Angle &)
   *
   * Description     Return angle which is difference of given angles.
   *
   * Parameters:
   * const Angle & right - angle which is subtracted from *this.
   *
   * Returns         None.
   *
   * Externals:      None.
   *
   * Precondition    None.
   *
   * Postcondition   None.
   *
   * Note            None
   *========================================================================*/
   Angle Angle::operator-(const Angle & right) const
   {
      const float32_t new_val = this->raw_value - right.raw_value;
      const float32_t new_sine = Angle::Sin_Diff(*this, right);
      const float32_t new_cosine = Angle::Cos_Diff(*this, right);

      const Angle new_angle = Angle(new_val, new_sine, new_cosine);

      return new_angle;
   }

   /*=========================================================================
   * Method          Angle::operator-()
   *
   * Description     Returns angle with negated value.
   *
   * Parameters:     None.
   *
   * Returns         None.
   *
   * Externals:      None.
   *
   * Precondition    None.
   *
   * Postcondition   None.
   *
   * Note            None.
   *========================================================================*/
   Angle Angle::operator-() const
   {
      const float32_t new_sine = -this->sine;

      return Angle{-this->raw_value, new_sine , this->cosine};
   }


   /*=========================================================================
   * Method          Angle::operator-=(const Angle &)
   *
   * Description     Subtract given angle.
   *
   * Parameters:
   * const Angle & right - angle which is subtracted from *this.
   *
   * Returns         Reference to the angle.
   *
   * Externals:      None.
   *
   * Precondition    None.
   *
   * Postcondition   None.
   *
   * Note            None
   *========================================================================*/
   Angle& Angle::operator-=(const Angle& right)
   {
      this->raw_value -= right.raw_value;

      const float32_t new_sin = Angle::Sin_Diff(*this, right);
      const float32_t new_cos = Angle::Cos_Diff(*this, right);
      this->sine = new_sin;
      this->cosine = new_cos;

      return *this;
   }
}

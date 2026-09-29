#ifndef LCDA_FORCE_CHANGE_CONSTANT_CALIBRATION
#define LCDA_FORCE_CHANGE_CONSTANT_CALIBRATION

/**
 * @file lcda_force_change_constant_calibration.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Helper method for editing calibration
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

extern "C"
{
#include "lcda_core_calibration.h"
}

#include <type_traits>

template <typename T, typename Q>
void Lcda_Force_Change_Constant_Calibration(Lcda_Core_Calibration_T *calibration, T Lcda_Core_Calibration_T::*member, const Q &new_value)
{

   static_assert(std::is_const<T>::value, "Removing const from no const obiect");
   using NoConstT                               = typename std::remove_const<T>::type;
   const_cast<NoConstT &>(calibration->*member) = static_cast<const T>(new_value);
}

template <typename T, typename Q, size_t N>
void Lcda_Force_Change_Constant_1_D_Array_Calibration(Lcda_Core_Calibration_T *calibration,
                                                      T (Lcda_Core_Calibration_T::*member)[N],
                                                      const size_t x,
                                                      const Q &new_value)
{
   static_assert(std::is_const<T>::value, "Removing const from no const obiect");
   using NoConstT                                    = typename std::remove_const<T>::type;
   const_cast<NoConstT &>((calibration->*member)[x]) = static_cast<const T>(new_value);
}

template <typename T, typename Q, size_t N, size_t M>
void Lcda_Lcda_Force_Change_Constant_2_D_Array_Calibration(Lcda_Core_Calibration_T *calibration,
                                                           const T (Lcda_Core_Calibration_T::*member)[N][M],
                                                           const size_t x,
                                                           const size_t y,
                                                           const Q &new_value)
{
   static_assert(std::is_const<T>::value, "Removing const from no const obiect");
   using NoConstT                                       = typename std::remove_const<T>::type;
   const_cast<NoConstT &>((calibration->*member)[x][y]) = static_cast<const T>(new_value);
}

#endif
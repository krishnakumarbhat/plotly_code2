#ifndef ST_INT_RANGE_HELPER_HPP
#define ST_INT_RANGE_HELPER_HPP

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "as_type.hpp"
#include <gtest/gtest.h>
#include "custom_cmp_helper.hpp"

#include <math.h>
#include "ml_int_range_t.h"

extern "C" {
}

namespace testing {
namespace as {

   /**
   * Specialization of the IsEqual for the Int_Range_T structure.
   * \return true if the Int_Range_T are exactly identical
   * Does compare each element of the Int_Range_T.
   */
template<>
inline bool AS_Type<Int_Range_T>::IsEqual(const AS_Type<Int_Range_T>& rhs) const
{
   const auto left_x(obj.min);
   const auto left_y(obj.max);
   const auto  right_x(rhs.obj.min);
   const auto  right_y(rhs.obj.max);

   return((left_x == right_x) && (left_y == right_y));
}

/**
* Specialization of the IsNear for the Int_Range_T structure.
* \return true if the Int_Range_T are identical within given abs_err
* Does compare each element of the Int_Range_T.
*/
template<>
inline bool AS_Type<Int_Range_T>::IsNear(
   const AS_Type<Int_Range_T>& rhs,
   double                      abs_err) const
{
   bool x_err = (abs(obj.min - rhs.obj.min) > abs_err);
   bool y_err = (abs(obj.max - rhs.obj.max) > abs_err);


   return !(x_err || y_err);
}

/**
* Specialization of the ToString for the Int_Range_T structure.
* \return A string representing the Int_Range_T, e.g: "(22, 44)"
*/
template<>
inline std::string AS_Type<Int_Range_T>::ToString() const
{
   std::stringstream sstream;

   sstream << "(" << std::setprecision(std::numeric_limits<float32_T>::digits10 + 2)
      << obj.min << ", " << obj.max << ")";

   return testing::internal::StringStreamToString(&sstream);
}
}
}

/** Macro to expect that the given Int_Range_T are identical
* \param val1 Int_Range_T to be compared
* \param val2 Int_Range_T to be compared
*/
#define EXPECT_INT_RANGE_EQ(val1, val2)                               \
   EXPECT_PRED_FORMAT2(::testing::as::CustomCmpHelperEQ<Int_Range_T>, \
                       val1, val2)

/** Macro to expect that the given Int_Range_T are identical within given margin
* \param val1 Int_Range_T to be compared
* \param val2 Int_Range_T to be compared
* \param abs_error margin for the vectors to be within
*/
#define EXPECT_FLOAT_RANGE_NEAR(val1, val2, abs_error)                    \
   EXPECT_PRED_FORMAT3(::testing::as::CustomCmpHelperNear<Int_Range_T>, \
                       val1, val2, abs_error)
#endif


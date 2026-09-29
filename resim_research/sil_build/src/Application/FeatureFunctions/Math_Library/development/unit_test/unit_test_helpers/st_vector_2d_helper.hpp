#ifndef ST_VECTOR_2D_HELPER_HPP
#define ST_VECTOR_2D_HELPER_HPP

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "custom_cmp_helper.hpp"
#include "as_type.hpp"

#include <math.h>
#include "ml_vector_2d_t.h"

extern "C" {
}

namespace testing {
namespace as {

/**
 * Specialization of the IsEqual for the Vector_2d_T structure.
 * \return true if the vectors are identical within float tolerances
 * Does compare each element of the vectors.
 */
template<>
inline bool AS_Type<Vector_2d_T>::IsEqual(const AS_Type<Vector_2d_T>& rhs) const
{
   const testing::internal::FloatingPoint<float32_T> left_x(obj.x);
   const testing::internal::FloatingPoint<float32_T> left_y(obj.y);
   const testing::internal::FloatingPoint<float32_T> right_x(rhs.obj.x);
   const testing::internal::FloatingPoint<float32_T> right_y(rhs.obj.y);

   return(left_x.AlmostEquals(right_x) && left_y.AlmostEquals(right_y));
}


/**
* Specialization of the IsNear for the Vector_2d_T structure.
* \return true if the vectors are identical within given abs_err
* Does compare each element of the vectors.
*/
template<>
inline bool AS_Type<Vector_2d_T>::IsNear(
   const AS_Type<Vector_2d_T>& rhs,
   double                      abs_err) const
{
   bool x_err = (fabs(obj.x - rhs.obj.x) > abs_err);
   bool y_err = (fabs(obj.y - rhs.obj.y) > abs_err);


   return !(x_err || y_err);
}

/**
* Specialization of the ToString for the Vector_2d_T structure.
* \return A string representing the vector, e.g: "(0.123, 1.2345)"
*/
template<>
inline std::string AS_Type<Vector_2d_T>::ToString() const
{
   std::stringstream ss;

   ss << "(" << std::setprecision(std::numeric_limits<float32_T>::digits10 + 2)
      << obj.x << ", " << obj.y << ")";

   return testing::internal::StringStreamToString(&ss);
}
}
}

/** Macro to expect that the given vectors are identical
 * \param val1 vector to be compared
 * \param val2 vector to be compared
 */
#define EXPECT_VECTOR_2D_EQ(val1, val2)                               \
   EXPECT_PRED_FORMAT2(::testing::as::CustomCmpHelperEQ<Vector_2d_T>, \
                       val1, val2)

 /** Macro to expect that the given vectors are identical within given margin
 * \param val1 vector to be compared
 * \param val2 vector to be compared
 * \param abs_error margin for the vectors to be within
 */
#define EXPECT_VECTOR_2D_NEAR(val1, val2, abs_error)                    \
   EXPECT_PRED_FORMAT3(::testing::as::CustomCmpHelperNear<Vector_2d_T>, \
                       val1, val2, abs_error)
#endif


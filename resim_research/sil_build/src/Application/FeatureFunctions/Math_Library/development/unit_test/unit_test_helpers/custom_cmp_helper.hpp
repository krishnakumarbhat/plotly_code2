#ifndef CUSTOM_CMP_HELPER_HPP
#define CUSTOM_CMP_HELPER_HPP

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "as_type.hpp"

namespace testing {
namespace as {
template<typename RawType>
AssertionResult CustomCmpHelperEQ(
   const char *lhs_expression,
   const char *rhs_expression,
   RawType     lhs_value,
   RawType     rhs_value)
{
   const AS_Type<RawType> lhs(lhs_value);
   const AS_Type<RawType> rhs(rhs_value);

   if (lhs.IsEqual(rhs))
   {
      return AssertionSuccess();
   }
   ::std::string lhs_str = lhs.ToString();
   ::std::string rhs_str = rhs.ToString();
   return testing::internal::EqFailure(lhs_expression,
                                       rhs_expression,
                                       lhs_str,
                                       rhs_str,
                                       false);
}


template<typename RawType>
AssertionResult CustomCmpHelperNear(
   const char *lhs_expression,
   const char *rhs_expression,
   const char *abs_error_expression,
   RawType     lhs_value,
   RawType     rhs_value,
   double      abs_error)
{
   const AS_Type<RawType> lhs(lhs_value), rhs(rhs_value);

   if (lhs.IsNear(rhs, abs_error))
   {
      return AssertionSuccess();
   }

   ::std::string lhs_str = lhs.ToString();
   ::std::string rhs_str = rhs.ToString();

   return AssertionFailure()
          << "The difference between " << lhs_expression << " and " << rhs_expression
          << " exceeds " << abs_error_expression << ", where\n"
          << lhs_expression << " evaluates to " << lhs_str << ",\n"
          << rhs_expression << " evaluates to " << rhs_str << ", and\n"
          << abs_error_expression << " evaluates to " << abs_error << ".";
}
}
}


#endif


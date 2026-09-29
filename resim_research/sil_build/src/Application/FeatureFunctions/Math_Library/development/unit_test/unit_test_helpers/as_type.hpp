#ifndef AS_TYPE_HPP
#define AS_TYPE_HPP

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include <gtest/gtest.h>


namespace testing {
namespace as {
   /**
    * Base class for custom compare helpers.
    */
template<typename RawType>
class AS_Type
{
protected:
   RawType& obj;
public:
   /** Initializer */
   AS_Type(RawType& T) : obj(T) {}

   /** Comparison for equality 
    * \return true if the given value matches */
   bool IsEqual(
      const AS_Type<RawType>& /**< Value to compare to */
   ) const;

   /** Comparison for equality
   * \return true if the given value matches within given abs_err*/
   bool IsNear(
      const AS_Type<RawType>&, /**< Value to compare to */
      double abs_err /**< margin */
   ) const;

   /** Conversion to string
   * \return A string representing the value of AS_Type */
   std::string ToString() const;
};

}
}

#endif


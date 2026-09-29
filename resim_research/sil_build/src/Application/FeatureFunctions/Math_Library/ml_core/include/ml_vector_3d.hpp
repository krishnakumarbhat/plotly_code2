/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#ifndef ST_VECTOR_3D_HPP
#define ST_VECTOR_3D_HPP

 /**
 * \defgroup Vector_3d_algebra_cpp 3D Vector Algebra C++
 * Functions that allow to work with \ref Vector_3d_T structure in C++.
 *
 * Functionality that allows to conveniently use the shared toolbox Vector_3d_T structure in C++ code.
 * The goal is bi directional interoperability. This means that C++ code shall be able to pass a
 * Vector_3d_T to C code and C code shall be able to return a Vector_3d_T to C++ code.
 * \section ST_VECTOR_3D_HPP_design_considerations Design Considerations
 * Since the Vector_3d_T structure shall be usable in C code it can not have member functions. It
 * might be possible under certain conditions to have member functions but doing so brings the code
 * awfully close to implementation defined behaviour. Most operator overloads can be implemented as
 * member functions, therefore the main limitation is the absence of custom constructors from
 * various types. These as well can be implemented as non member functions. As a developer the lost
 * convenience is little.
 *
 * The C-version of Vector_3d_T needs to live in the global namespace. To ease usage the definition
 * of Vector_3d_T for C++ is in the namespace 'st'. A typedef pulls st::Vector_3d_T into the global
 * namespace. This behaviour is in line with e.g. size_t which is present as size_t as well as
 * std::size_t
 *
 * This has the advantage that e.g.
 * \code
 * Vector_3d_T v0 = C_Function();
 * Vector_3d_T v1 = C_Function();
 * v0 = v0 + v1;
 * \endcode
 * will 'just work' even if auto is used:
 * \code
 * auto v0 = C_Function();
 * auto v1 = C_Function();
 * v0 = v0 + v1;
 * \endcode
 */

#include "ml_vector_3d_t.h"
#include "ml_vector_3d_tag.h"

extern "C" {
#include "reuse.h"
}

#include "ml_math.h"

#include <cassert>
#include <cmath>

/**
 * Functionality to conveniently manipulate Vector_3d_T in C++
 */
namespace ml {
   /**
    * Since the Vector_3d_T is used to share data between C and C++ bi-directionally the struct Vector_3d_T
    * can not have any member functions. In particular Vector_3d_T can not have any custom constructors.
    * To mitigate this limitation the namespace st::vector3d contains a bunch of functions that construct a
    * Vector_3d_T from various parameters.
    */

   namespace vector3d
   {
      /** \return Vector with given coordinates.
       * If no coordinates are given the origin vector is returned.
       * \sa Create_2d_Vector_Coordinates()
       * \sa Create_2d_Vector_Origin()
       * \ingroup Vector_3d_algebra_cpp
       */
      inline Vector_3d_T Vector(
         const float32_T x = 0.0f, /**< Longitudinal component */
         const float32_T y = 0.0f,  /**< Lateral component */
         const float32_T z = 0.0f  /**< Elevation component */
      );

      /** \return a vector in longitudinal direction of given length, a unit vector in this direction if no length is given
      * \sa Create_2d_Vector_X_Normal()
      * \ingroup Vector_3d_algebra_cpp
      */
      inline Vector_3d_T X(
         float32_T x = 1.0f /**< Length of vector to be created */
      );

      /** \return a vector in lateral direction of given length, a unit vector in this direction if no length is given
      * \sa Create_2d_Vector_Y_Normal
      * \ingroup Vector_3d_algebra_cpp
      */
      inline Vector_3d_T Y(
         float32_T y = 1.0f  /**< Length of vector to be created */
      );

      /** \return a vector in lateral direction of given length, a unit vector in this direction if no length is given
      * \sa Create_2d_Vector_Y_Normal
      * \ingroup Vector_3d_algebra_cpp
      */
      inline Vector_3d_T Z(
         float32_T z = 1.0f  /**< Length of vector to be created */
      );
   }
   /** Add two vectors
   * \sa Vector_2d_Alg_Add()
   * \ingroup Vector_3d_algebra_cpp
   */
   inline Vector_3d_T operator+(
      const Vector_3d_T &lhs, /**< Vector to be added*/
      const Vector_3d_T &rhs  /**< Vector to be added*/
      );

   /** Assigning addition of two vectors
   * \sa Vector_2d_Alg_Add()
   * \ingroup Vector_3d_algebra_cpp
   */
   inline Vector_3d_T operator+=(
      Vector_3d_T &lhs, /**< Vector to be added*/
      const Vector_3d_T &rhs  /**< Vector to be added*/
      );

   /** Subtract two vectors
   * Computes the subtraction of two vectors.
   * \return   Difference between given vectors
   * \sa http:\\mathworld.wolfram.com/VectorDifference.html
   * \ingroup Vector_3d_algebra_cpp
   * \sa Vector_2d_Alg_Diff()
   * */
   inline Vector_3d_T operator-(
      const Vector_3d_T &lhs, /**< [in] minuend */
      const Vector_3d_T &rhs  /**< [in] Subtrahend */
      );

     /** Subtract two vectors
   * Computes the subtraction of two vectors.
   * \return   Difference between given vectors
   * \sa http:\\mathworld.wolfram.com/VectorDifference.html
   * \ingroup Vector_3d_algebra_cpp
   * \sa Vector_2d_Alg_Diff()
   * */
   inline Vector_3d_T operator-(
      const Vector_3d_T*lhs, /**< [in] minuend */
      const Vector_3d_T&rhs  /**< [in] Subtrahend */
      );

   /**
   * Unary minus
   * \return Negated version of given vector
   * \sa http:\\mathworld.wolfram.com/VectorDifference.html
   * \ingroup Vector_3d_algebra_cpp*/
   inline Vector_3d_T operator-(
      const Vector_3d_T &vector /**< Vector to negate*/
      );

   /**
   * Unary plus
   * \return Given vector
   * \sa http:\\mathworld.wolfram.com/VectorDifference.html
   * \ingroup Vector_3d_algebra_cpp*/
   inline Vector_3d_T operator+(
      const Vector_3d_T &vector /**< Vector to be added*/
      );

   /** Assigning subtraction two vectors
   * Computes the subtraction of two vectors.
   * \return   Difference between given vectors
   * \sa http:\\mathworld.wolfram.com/VectorDifference.html
   * \ingroup Vector_3d_algebra_cpp
   * \sa Vector_2d_Alg_Diff()
   */
   inline Vector_3d_T operator-=(
      Vector_3d_T &lhs, /**< [in] minuend */
      const Vector_3d_T &rhs  /**< [in] Subtrahend */
      );

   /** Multiply a vector with a scalar
   * \sa Vector_2d_Alg_Multiply_Scalar()
   * \ingroup Vector_3d_algebra_cpp*/
   inline Vector_3d_T operator*(
      const Vector_3d_T &lhs, /**< Vector to be multiplied */
      const float32_T   rhs        /**< Scalar to be multiplied */
      );

   /** Multiply a scalar with a vector
   * \sa Vector_2d_Alg_Multiply_Scalar()
   * \ingroup Vector_3d_algebra_cpp*/
   inline Vector_3d_T    operator*(
      const float32_T    lhs,      /**< Scalar to be multiplied */
      const Vector_3d_T &rhs /**< Vector to be multiplied */
      );

   /** Assigning multiplication of vector and scalar
   * \sa Vector_2d_Alg_Multiply_Scalar()
   * \ingroup Vector_3d_algebra_cpp*/
   inline Vector_3d_T operator*=(
      Vector_3d_T     &lhs, /**< Vector to be multiplied */
      const float32_T &rhs  /**< Scalar to be multiplied */
      );

   /** Index a 2d vector
   * \ingroup Vector_3d_algebra_cpp*/
   inline float32_T at(
      const Vector_3d_T &vector, /**< Vector to be indexed */
      const size_t       index   /**< Index */
   );

   /**
   * Computes the 2d scalar product between two vectors
   * \return           2d scalar product between two vectors
   * \sa http:\\mathworld.wolfram.com/DotProduct.html
   * Since multiplying vectors component by component is seldom useful the
   * operator* has been overloaded to be the dot product.
   * \ingroup Vector_3d_algebra_cpp
   */
   inline float32_T operator*(
      const Vector_3d_T &lhs,
      const Vector_3d_T &rhs
      );

   /**
   * Returns a Vector that points to the middle point of the provided vectors
   * \return           Vector that points to the middle point of the provided vectors
   * \sa Vector_2d_Alg_Middle()
   * \ingroup Vector_3d_algebra_cpp
   */
   inline Vector_3d_T Middle(
      const Vector_3d_T &vec_0, /**< [in] Vector to find a middle point for */
      const Vector_3d_T &vec_1  /**< [in] Vector to find a middle point for */
   );

   /**
   * Returns the squared absolute value (length^2) of a vector.
   * \return           squared absolute value (length^2) of a vector.
   * \sa Vector_2d_Alg_Abs_Squared()
   * \ingroup Vector_3d_algebra_cpp
   */
   inline float32_T Norm_Squared(
      const Vector_3d_T &vector /**< [in] Vector to compute the squared absolute value (squared length) for */
   );

   /**
   * Returns the absolute value (length) of a vector.
   * \return the absolute value (length) of a vector.
   * \sa https:\\en.wikipedia.org/wiki/Absolute_value
   * \sa Vector_2d_Alg_Abs()
   * \ingroup Vector_3d_algebra_cpp
   */
   inline float32_T Norm(
      const Vector_3d_T &vector
   );

   /**
   * Computes the normalized version of the given vector
   * \sa http:\\mathworld.wolfram.com/NormalizedVector.html
   * \return           normalized vector
   * \sa Vector_2d_Alg_Normalize_Vector()
   * \ingroup Vector_3d_algebra_cpp
   */
   inline Vector_3d_T Normalize(
      const Vector_3d_T &vector /**< [in] Vector to be normalized */
   );

   /**
   * Computes the absolute value of each component in the input vector.
   * \return   Given vector with each component in its absolute (positive) value.
   * \sa Vector_2d_Alg_Abs_Component_Wise
   * \ingroup Vector_3d_algebra_cpp
   */
   inline Vector_3d_T Absolute(
      const Vector_3d_T &vector /**< [in] Vector to build the component wise absolute form of */
   );


   namespace vector3d
   {

      inline Vector_3d_T Vector(
         const float32_T x,
         const float32_T y,
         const float32_T z
      )
      {
         Vector_3d_T ret;
         ret.x = x;
         ret.y = y;
         ret.z = z;
         return ret;
      }

      inline Vector_3d_T X(
         float32_T x /* default is 1.0f */
      )
      {
         Vector_3d_T ret = Vector();
         ret.x = x;
         return ret;
      }

      inline Vector_3d_T Y(
         float32_T y /* default is 1.0f */
      )
      {
         Vector_3d_T ret = Vector();
         ret.y = y;
         return ret;
      }

      inline Vector_3d_T Z(
         float32_T z /* default is 1.0f */
      )
      {
         Vector_3d_T ret = Vector();
         ret.z = z;
         return ret;
      }
   }

   inline Vector_3d_T operator+(
      const Vector_3d_T &lhs,
      const Vector_3d_T &rhs
      )
   {
      Vector_3d_T out;
      out.x = lhs.x + rhs.x;
      out.y = lhs.y + rhs.y;
      out.z = lhs.z + rhs.z;
      return out;
   }

   inline Vector_3d_T operator+=(
            Vector_3d_T &lhs,
      const Vector_3d_T &rhs
      )
   {
      lhs = lhs + rhs;
      return lhs;
   }

   inline Vector_3d_T operator-(
      const Vector_3d_T &lhs,
      const Vector_3d_T &rhs
      )
   {
      Vector_3d_T out;
      out.x = lhs.x - rhs.x;
      out.y = lhs.y - rhs.y;
      out.z = lhs.z - rhs.z;
      return out;
   }

   inline Vector_3d_T operator-(
      const Vector_3d_T &vector
      )
   {
      Vector_3d_T out;
      out.x = -vector.x;
      out.y = -vector.y;
      out.z = -vector.z;
      return out;
   }

   inline Vector_3d_T operator+(
      const Vector_3d_T &vector
      )
   {
      return vector;
   }

   inline Vector_3d_T operator-=(
            Vector_3d_T &lhs,
      const Vector_3d_T &rhs
      )
   {
      lhs = lhs - rhs;
      return lhs;
   }

   inline Vector_3d_T operator*(
      const Vector_3d_T &lhs,
      const float32_T        rhs
      )
   {
      Vector_3d_T out;
      out.x = lhs.x * rhs;
      out.y = lhs.y * rhs;
      out.z = lhs.z * rhs;
      return out;
   }

   inline Vector_3d_T operator*(
      const float32_T        lhs,
      const Vector_3d_T &rhs
      )
   {
      return rhs * lhs;
   }

   inline Vector_3d_T operator*=(
            Vector_3d_T &lhs,
      const float32_T   &rhs
      )
   {
      lhs = lhs * rhs;
      return lhs;
   }

   inline float32_T at(
      const Vector_3d_T &vector,
      const size_t       index
      )
   {
       float32_T ret = vector.x;
       if(index == 1)
       {
           ret = vector.y;
       }
       else if (index == 2)
       {
           ret = vector.z;
       }
       else
       {
           /* Nothing to do */
       }
      return ret;
   }

   inline float32_T operator*(
      const Vector_3d_T &lhs,
      const Vector_3d_T &rhs
      )
   {
      float32_T out;
      out = (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
      return out;
   }

   inline Vector_3d_T Middle(
      const Vector_3d_T &vec_0,
      const Vector_3d_T &vec_1
   )
   {
      return (vec_0 * 0.5f) + (vec_1 * 0.5f);
   }

   inline float32_T Norm_Squared(
      const Vector_3d_T &vector
   )
   {
      float32_T ret_val;

      ret_val = (vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z);
      return ret_val;
   }


   inline float32_T Norm(
      const Vector_3d_T &vector
   )
   {
      return Fast_Sqrt(Norm_Squared(vector));
   }

   inline Vector_3d_T Normalize(
      const Vector_3d_T &vector
   )
   {
      Vector_3d_T ret_value;

      assert(vector.x != 0 || vector.y != 0 || vector.z != 0);      /* This prevents division by zero */

      ret_value = vector * (1.0f / Norm(vector));

      return ret_value;
   }

   inline Vector_3d_T Absolute(
      const Vector_3d_T &vector
   )
   {
      Vector_3d_T ret_vec;

      ret_vec.x = std::abs(vector.x);
      ret_vec.y = std::abs(vector.y);
      ret_vec.z = std::abs(vector.z);

      return ret_vec;
   }

}
#endif

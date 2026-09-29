/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#ifndef ML_VECTOR_2D_HPP
#define ML_VECTOR_2D_HPP

 /**
 * \defgroup Vector_2d_algebra_cpp 2D Vector Algebra C++
 * Functions that allow to work with \ref Vector_2d_T structure in C++.
 *
 * Functionality that allows to conveniently use the shared toolbox Vector_2d_T structure in C++ code.
 * The goal is bi directional interoperability. This means that C++ code shall be able to pass a
 * Vector_2d_T to C code and C code shall be able to return a Vector_2d_T to C++ code.
 * \section ST_VECTOR_2D_HPP_design_considerations Design Considerations
 * Since the Vector_2d_T structure shall be usable in C code it can not have member functions. It
 * might be possible under certain conditions to have member functions but doing so brings the code
 * awfully close to implementation defined behaviour. Most operator overloads can be implemented as
 * member functions, therefore the main limitation is the absence of custom constructors from
 * various types. These as well can be implemented as non member functions. As a developer the lost
 * convenience is little.
 *
 * The C-version of Vector_2d_T needs to live in the global namespace. To ease usage the definition
 * of Vector_2d_T for C++ is in the namespace 'st'. A typedef pulls st::Vector_2d_T into the global
 * namespace. This behaviour is in line with e.g. size_t which is present as size_t as well as
 * std::size_t
 *
 * This has the advantage that e.g.
 * \code
 * Vector_2d_T v0 = C_Function();
 * Vector_2d_T v1 = C_Function();
 * v0 = v0 + v1;
 * \endcode
 * will 'just work' even if auto is used:
 * \code
 * auto v0 = C_Function();
 * auto v1 = C_Function();
 * v0 = v0 + v1;
 * \endcode
 */

 /***
 * There is functionality that needs the structures Vector_2d_T and Angle_T.
 * Because user expectations for where these functions can be found may differ
 * these reside in a separate header st_vector_2d_angle.h
 * Both st_vector_2d.h and st_angle.h include this combined header to ensure that
 * users always find the corresponding functions.
 */
#include "ml_vector_2d_angle.hpp"


#include "ml_vector_2d_t.h"
#include "ml_trigonometry.h"
#include "ml_interval.h"  // for Enforce_Range()
#include "ml_angle_t.h"
extern "C" {
#include "reuse.h"
}

#include <cassert>

/**
 * Functionality to conveniently manipulate Vector_2d_T in C++
 */
namespace ml {
   /**
    * Since the Vector_2d_T is used to share data between C and C++ bi-directionally the struct Vector_2d_T
    * can not have any member functions. In particular Vector_2d_T can not have any custom constructors.
    * To mitigate this limitation the namespace ml::vector2d contains a bunch of functions that construct a
    * Vector_2d_T from various parameters.
    */

    /**
    * Functions calculating a cosine will return this if calculation fails.
    * \sa Cos(Vector_2d_T&, Vector_2d_T&)
    * \ingroup Vector_2d_algebra_cpp
    * \sdd{WI-13943}
    */
   static float32_T vector_2d_algebra_not_a_cosine = -2.0f;

   namespace vector2d
   {
      /** \return Vector with given coordinates.
       * If no coordinates are given the origin vector is returned.
       * \sa Create_2d_Vector_Coordinates()
       * \sdd{WI-13925}
       * \sa Create_2d_Vector_Origin()
       * \sdd{WI-13934}
       * \ingroup Vector_2d_algebra_cpp
       */
      inline Vector_2d_T Vector(
         const float32_T x = 0.0f, /**< Longitudinal component */
         const float32_T y = 0.0f  /**< Lateral component */
      );

      /** \return a unit vector in the direction of given angle.
      * \ingroup Vector_2d_algebra_cpp*/
      inline Vector_2d_T Vector(
         const Angle_T angle /**< Angle to create a unit vector from */
      );

      /** \return a vector in longitudinal direction of given length, a unit vector in this direction if no length is given
      * \sa Create_2d_Vector_X_Normal()
      * \sdd{WI-13933}
      * \ingroup Vector_2d_algebra_cpp
      */
      inline Vector_2d_T X(
         float32_T x = 1.0f /**< Length of vector to be created */
      );

      /** \return a vector in lateral direction of given length, a unit vector in this direction if no length is given
      * \sa Create_2d_Vector_Y_Normal
      * \sdd{WI-13930}
      * \ingroup Vector_2d_algebra_cpp
      */
      inline Vector_2d_T Y(
         float32_T y = 1.0f  /**< Length of vector to be created */
      );
   }
   /** Add two vectors
   * \sa Vector_2d_Alg_Add()
   * \sdd{WI-13919}
   * \ingroup Vector_2d_algebra_cpp
   */
   inline Vector_2d_T operator+(
      const Vector_2d_T &lhs, /**< Vector to be added*/
      const Vector_2d_T &rhs  /**< Vector to be added*/
      );

   /** Assigning addition of two vectors
   * \sa Vector_2d_Alg_Add()
   * \sdd{WI-13919}
   * \ingroup Vector_2d_algebra_cpp
   */
   inline Vector_2d_T operator+=(
      Vector_2d_T &lhs, /**< Vector to be added*/
      const Vector_2d_T &rhs  /**< Vector to be added*/
      );

   /** Subtract two vectors
   * Computes the subtraction of two vectors.
   * \return   Difference between given vectors
   * \sa http:\\mathworld.wolfram.com/VectorDifference.html
   * \ingroup Vector_2d_algebra_cpp
   * \sa Vector_2d_Alg_Diff()
   * \sdd{WI-13915}*/
   inline Vector_2d_T operator-(
      const Vector_2d_T &lhs, /**< [in] minuend */
      const Vector_2d_T &rhs  /**< [in] Subtrahend */
      );

   /**
   * Unary minus
   * \return Negated version of given vector
   * \sa http:\\mathworld.wolfram.com/VectorDifference.html
   * \ingroup Vector_2d_algebra_cpp*/
   inline Vector_2d_T operator-(
      const Vector_2d_T &vector /**< Vector to negate*/
      );

   /**
   * Unary plus
   * \return Given vector
   * \sa http:\\mathworld.wolfram.com/VectorDifference.html
   * \ingroup Vector_2d_algebra_cpp*/
   inline Vector_2d_T operator+(
      const Vector_2d_T &vector /**< Vector to be added*/
      );

   /** Assigning subtraction two vectors
   * Computes the subtraction of two vectors.
   * \return   Difference between given vectors
   * \sa http:\\mathworld.wolfram.com/VectorDifference.html
   * \ingroup Vector_2d_algebra_cpp
   * \sa Vector_2d_Alg_Diff()
   * \sdd{WI-13915}*/
   inline Vector_2d_T operator-=(
      Vector_2d_T &lhs, /**< [in] minuend */
      const Vector_2d_T &rhs  /**< [in] Subtrahend */
      );

   /** Multiply a vector with a scalar
   * \sa Vector_2d_Alg_Multiply_Scalar()
   * \sdd{WI-13917}
   * \ingroup Vector_2d_algebra_cpp*/
   inline Vector_2d_T operator*(
      const Vector_2d_T &lhs, /**< Vector to be multiplied */
      const float32_T        rhs        /**< Scalar to be multiplied */
      );

   /** Multiply a scalar with a vector
   * \sa Vector_2d_Alg_Multiply_Scalar()
   * \sdd{WI-13917}
   * \ingroup Vector_2d_algebra_cpp*/
   inline Vector_2d_T operator*(
      const float32_T        lhs,      /**< Scalar to be multiplied */
      const Vector_2d_T &rhs /**< Vector to be multiplied */
      );

   /** Assigning multiplication of vector and scalar
   * \sa Vector_2d_Alg_Multiply_Scalar()
   * \sdd{WI-13917}
   * \ingroup Vector_2d_algebra_cpp*/
   inline Vector_2d_T operator*=(
      Vector_2d_T &lhs, /**< Vector to be multiplied */
      const float32_T       &rhs  /**< Scalar to be multiplied */
      );

   /** Index a 2d vector
   * \ingroup Vector_2d_algebra_cpp*/
   inline float32_T at(
      Vector_2d_T &vector, /**< Vector to be indexed */
      const size_t index   /**< Index */
   );

   /**
   * Computes the 2d scalar product between two vectors
   * \return           2d scalar product between two vectors
   * \sa http:\\mathworld.wolfram.com/DotProduct.html
   * \sdd{WI-13924}
   * Since multiplying vectors component by component is seldom useful the
   * operator* has been overloaded to be the dot product.
   * \ingroup Vector_2d_algebra_cpp
   */
   inline float32_T operator*(
      const Vector_2d_T &lhs,
      const Vector_2d_T &rhs
      );

   /**
   * Returns a Vector that points to the middle point of the provided vectors
   * \return           Vector that points to the middle point of the provided vectors
   * \sa Vector_2d_Alg_Middle()
   * \sdd{WI-13923}
   * \ingroup Vector_2d_algebra_cpp
   */
   inline Vector_2d_T Middle(
      const Vector_2d_T &vec_0, /**< [in] Vector to find a middle point for */
      const Vector_2d_T &vec_1  /**< [in] Vector to find a middle point for */
   );

   /**
   * Calculates the distance between two points defined by two 2d vectors
   * The function is commutative and the result is always bigger then zero
   * \return           distance between given two points defined by two 2d vectors
   * \sa Vector_2d_Alg_Distance()
   * \sdd{WI-13921}
   * \ingroup Vector_2d_algebra_cpp
   */
   inline float32_T Distance(
      const Vector_2d_T &vector_a,
      const Vector_2d_T &vector_b
   );

   /**
   * \return a vector perpendicular to the given vector, rotated in positive direction
   * \ingroup Vector_2d_algebra_cpp
   */
   inline Vector_2d_T Perpendicular_Positive(
      const Vector_2d_T& vector /**< Vector to create a perpendicular vector from */
   );

   /**
   * \return a vector perpendicular to the given vector, rotated in negative direction
   * \ingroup Vector_2d_algebra_cpp
   */
   inline Vector_2d_T Perpendicular_Negative(
      const Vector_2d_T& vector /**< Vector to create a perpendicular vector from */
   );

   /**
   * Returns the squared absolute value (length^2) of a vector.
   * \return           squared absolute value (length^2) of a vector.
   * \sa Vector_2d_Alg_Abs_Squared()
   * \sdd{WI-13935}
   * \ingroup Vector_2d_algebra_cpp
   */
   inline float32_T Norm_Squared(
      const Vector_2d_T &vector /**< [in] Vector to compute the squared absolute value (squared length) for */
   );

   /**
   * Returns the absolute value (length) of a vector.
   * \return the absolute value (length) of a vector.
   * \sa https:\\en.wikipedia.org/wiki/Absolute_value
   * \sa Vector_2d_Alg_Abs()
   * \sdd{WI-13937}
   * \ingroup Vector_2d_algebra_cpp
   */
   inline float32_T Norm(
      const Vector_2d_T &vector
   );

   /**
   * Computes the normalized version of the given vector
   * \sa http:\\mathworld.wolfram.com/NormalizedVector.html
   * \return           normalized vector
   * \sa Vector_2d_Alg_Normalize_Vector()
   * \ingroup Vector_2d_algebra_cpp
   * \sdd{WI-13914}
   */
   inline Vector_2d_T Normalize(
      const Vector_2d_T &vector /**< [in] Vector to be normalized */
   );

   /**
   * Computes the absolute value of each component in the input vector.
   * \return   Given vector with each component in its absolute (positive) value.
   * \sa Vector_2d_Alg_Abs_Component_Wise
   * \ingroup Vector_2d_algebra_cpp
   * \sdd{WI-13938}
   */
   inline Vector_2d_T Absolute(
      const Vector_2d_T &vector /**< [in] Vector to build the component wise absolute form of */
   );

   /**
   * Computes the cosine of the included angle between the two input vectors.
   * To do the calculation a division by both vector length is needed. Therefore the vector lengths need to be nonzero
   * \return   Cosine between given two vectors or \ref VECTOR_2D_ALGEBRA_NOT_A_COSINE if calculation is not possible
   * \Caveats
   * To do the calculation a division by both vector length is needed. Therefore the vector lengths need to be nonzero (longer than a threshold).
   * If this isn't the case and the calculation can't be done a default value of \ref VECTOR_2D_ALGEBRA_NOT_A_COSINE is returned.
   * \ingroup Vector_2d_algebra_cpp
   * \sdd{WI-13931}
   * \sdd{WI-13943}
   * \sa Vector_2d_Alg_Calculate_Cos_Between_Two_Vec
   */
   inline float32_T Cos(
      const Vector_2d_T &vector_a, /**< [in] First vector to find the cosine of the enclosed angle for */
      const Vector_2d_T &vector_b  /**< [in] Second vector to find the cosine of the enclosed angle for */
   );

   /**
   * Returns a vector that is clipped to be within a square defined by given max and given min
   * \return   vector that is clipped to be within a square defined by given max and given min
   * \ingroup Vector_2d_algebra_cpp
   * \sdd{WI-13927}
   */
   inline Vector_2d_T Clip(
      const Vector_2d_T &vector, /**< [in] Vector to be limited */
      const Vector_2d_T &max,    /**< [in] Maximal vector */
      const Vector_2d_T &min     /**< [in] Minimal vector */
   );


   namespace vector2d
   {

      inline Vector_2d_T Vector(
         const float32_T x,
         const float32_T y
      )
      {
         Vector_2d_T ret;
         ret.x = x;
         ret.y = y;
         return ret;
      }

      inline Vector_2d_T Vector(
         const Angle_T angle
      )
      {
         Vector_2d_T ret;
         ret.x = angle.cos;
         ret.y = angle.sin;
         return ret;
      }

      inline Vector_2d_T X(
         float32_T x /* default is 1.0f */
      )
      {
         Vector_2d_T ret = Vector();
         ret.x = x;
         return ret;
      }

      inline Vector_2d_T Y(
         float32_T y /* default is 1.0f */
      )
      {
         Vector_2d_T ret = Vector();
         ret.y = y;
         return ret;
      }
   }

   inline Vector_2d_T operator+(
      const Vector_2d_T &lhs,
      const Vector_2d_T &rhs
      )
   {
      Vector_2d_T out;
      out.x = lhs.x + rhs.x;
      out.y = lhs.y + rhs.y;
      return out;
   }

   inline Vector_2d_T operator+=(
            Vector_2d_T &lhs,
      const Vector_2d_T &rhs
      )
   {
      lhs = lhs + rhs;
      return lhs;
   }

   inline Vector_2d_T operator-(
      const Vector_2d_T &lhs,
      const Vector_2d_T &rhs
      )
   {
      Vector_2d_T out;
      out.x = lhs.x - rhs.x;
      out.y = lhs.y - rhs.y;
      return out;
   }

   inline Vector_2d_T operator-(
      const Vector_2d_T &vector
      )
   {
      Vector_2d_T out;
      out.x = -vector.x;
      out.y = -vector.y;
      return out;
   }

   inline Vector_2d_T operator+(
      const Vector_2d_T &vector
      )
   {
      return vector;
   }

   inline Vector_2d_T operator-=(
            Vector_2d_T &lhs,
      const Vector_2d_T &rhs
      )
   {
      lhs = lhs - rhs;
      return lhs;
   }

   inline Vector_2d_T operator*(
      const Vector_2d_T &lhs,
      const float32_T        rhs
      )
   {
      Vector_2d_T out;
      out.x = lhs.x * rhs;
      out.y = lhs.y * rhs;
      return out;
   }

   inline Vector_2d_T operator*(
      const float32_T        lhs,
      const Vector_2d_T &rhs
      )
   {
      return rhs * lhs;
   }

   inline Vector_2d_T operator*=(
            Vector_2d_T &lhs,
      const float32_T       &rhs
      )
   {
      lhs = lhs * rhs;
      return lhs;
   }

   inline float32_T at(
      Vector_2d_T &vector,
      const size_t index
      )
   {
      return index == 0 ? vector.x : vector.y;
   }

   inline float32_T operator*(
      const Vector_2d_T &lhs,
      const Vector_2d_T &rhs
      )
   {
      float32_T out;
      out = (lhs.x * rhs.x) + (lhs.y * rhs.y);
      return out;
   }

   inline Vector_2d_T Middle(
      const Vector_2d_T &vec_0,
      const Vector_2d_T &vec_1
   )
   {
      return (vec_0 * 0.5f) + (vec_1 * 0.5f);
   }

   inline float32_T Distance(
      const Vector_2d_T &vector_a,
      const Vector_2d_T &vector_b
   )
   {
      float32_T ret_value;

      ret_value = Fast_Hypot(vector_a.x - vector_b.x, vector_a.y - vector_b.y);

      return ret_value;
   }

   inline Vector_2d_T Perpendicular_Positive(
      const Vector_2d_T& vector
   )
   {
      return ml::vector2d::Vector(vector.y, -vector.x);
   }

   inline Vector_2d_T Perpendicular_Negative(
      const Vector_2d_T& vector
   )
   {
      return ml::vector2d::Vector(-vector.y, vector.x);
   }

   inline float32_T Norm_Squared(
      const Vector_2d_T &vector
   )
   {
      float32_T ret_val;

      ret_val = (vector.x * vector.x) + (vector.y * vector.y);
      return ret_val;
   }


   inline float32_T Norm(
      const Vector_2d_T &vector
   )
   {
      return Fast_Sqrt(Norm_Squared(vector));
   }

   inline Vector_2d_T Normalize(
      const Vector_2d_T &vector
   )
   {
      Vector_2d_T ret_value;

      assert(vector.x != 0 || vector.y != 0);      /* This prevents division by zero */

      ret_value = vector * (1.0f / Norm(vector));

      return ret_value;
   }

   inline Vector_2d_T Absolute(
      const Vector_2d_T &vector
   )
   {
      Vector_2d_T ret_vec;

      ret_vec.x = std::abs(vector.x);
      ret_vec.y = std::abs(vector.y);

      return ret_vec;
   }

   inline float32_T Cos(
      const Vector_2d_T &vector_a,
      const Vector_2d_T &vector_b
   )
   {
      float32_T length_vector_a;
      float32_T length_vector_b;
      float32_T cos_theta = vector_2d_algebra_not_a_cosine;

      length_vector_a = Norm(vector_a);
      length_vector_b = Norm(vector_b);
      if ((length_vector_a > THRESHOLD_IS_ZERO) && (length_vector_b > THRESHOLD_IS_ZERO))
      {
         /* cos(angle_between(a,b)) =a*b / (|a|*|b|)*/
         cos_theta = (vector_a * vector_b) / (length_vector_a * length_vector_b);
      }
      else
      {
         /*To do the calculation a division by both vector length is needed. Therefore the vector lengths need to be nonzero
         * This is in this else cause not the case. Throw an assertion */
         assert(false);
      }

      return cos_theta;
   }

   inline Vector_2d_T Clip(
      const Vector_2d_T &vector,
      const Vector_2d_T &max,
      const Vector_2d_T &min
   )
   {
      Vector_2d_T vect_out;

      vect_out.x = Enforce_Range(vector.x, min.x, max.x);
      vect_out.y = Enforce_Range(vector.y, min.y, max.y);

      return vect_out;
   }
}

#endif

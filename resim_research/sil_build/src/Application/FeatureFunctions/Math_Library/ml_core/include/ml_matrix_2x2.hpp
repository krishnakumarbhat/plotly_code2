#ifndef ML_MATRIX_2X2_HPP
#define ML_MATRIX_2X2_HPP

/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

extern "C"
{
#include "reuse.h"
}
#include "ml_vector_2d_t.h"
#include "ml_matrix_2x2_t.h"

#include "ml_matrix_tools.hpp"
#include <algorithm>
#include <cassert>

/** \file
 * Functions to do \f$ \Re^{2 * 2 } \f$ matrix operations.
 * \defgroup Matrix Matrix
 * Matrix types and operations. Currently only \f$ \Re^{2 * 2 } \f$ and \f$ \Re^{3 x 3 } \f$ matrices are supported.
 * See [Wikipedia: Matrix_(mathematics) => Basic operations](https:\\en.wikipedia.org/wiki/Matrix_(mathematics)#Basic_operations)
 */

 /**
 * \defgroup Matrix_2x2_cpp Matrix 2x2 C++
 * Functions that allow to work with \ref Matrix_2X2_T structures in C++.
 */

namespace ml {
   namespace matrix_2x2 {
      /**
       * Create an identity \f$ \Re^{2 * 2 } \f$ matrix. See [Wikipedia: Identity Matrix](https:\\en.wikipedia.org/wiki/Identity_matrix)
       *
       * \ingroup Matrix_2x2_cpp
       * \sdd{WI-13902}
       */
      inline Matrix_2X2_T Identity();

      /**
         * Create a diagonal \f$ \Re^{ 2 * 2 } \f$ matrix based on given vector.
         See[Wikipedia:Identity Matrix](https:\\en.wikipedia.org/wiki/Diagonal_matrix)
         *
         * \ingroup Matrix_2x2_cpp
         * \sdd{ WI - 13902 }
         */
      inline Matrix_2X2_T Diagonal(
         Vector_2d_T v /**< vector to take diagonal elements from*/
      );

      /**
      * Create a diagonal \f$ \Re^{ 2 * 2 } \f$ matrix based on given vector.
      See[Wikipedia:Identity Matrix](https:\\en.wikipedia.org/wiki/Diagonal_matrix)
      *
      * \ingroup Matrix_2x2_cpp
      * \sdd{ WI - 13902 }
      */
      inline Matrix_2X2_T Diagonal(
         float32_T elem_0_0,
         float32_T elem_1_1
      );

      /**
       * Create a zero \f$ \Re^{2 * 2 } \f$ matrix. See [Wikipedia: Zero Matrix](https:\\en.wikipedia.org/wiki/Zero_matrix)
       *
       * \ingroup Matrix_2x2_cpp
       * \sdd{WI-13901}
       */
      inline Matrix_2X2_T Zero();

      /**
       * Create a zero \f$ \Re^{2 * 2 } \f$ matrix based on column vectors.
       *
       * \ingroup Matrix_2x2_cpp
       * \sdd{WI-13901}
       */
      inline Matrix_2X2_T Column(
         Vector_2d_T& column0,
         Vector_2d_T& column1);

      /**
       * Create a zero \f$ \Re^{2 * 2 } \f$ matrix based on column vectors.
       *
       * \ingroup Matrix_2x2_cpp
       * \sdd{WI-13901}
       */
      inline Matrix_2X2_T Row(
         Vector_2d_T& row0,
         Vector_2d_T& row1);
   }

   /**
    * Multiplication of two \f$ \Re^{2 * 2 } \f$ matrices: \f[ res\_mat = mat\_A * mat\_B \f] See [Wikipedia: Matrix_multiplication](https:\\en.wikipedia.org/wiki/Matrix_multiplication)
    *
    * \ingroup Matrix_2x2_cpp
    * \sdd{WI-13900}
    */
   inline Matrix_2X2_T operator*(
      const Matrix_2X2_T& mat_a,    /**< [in] input matrix A */
      const Matrix_2X2_T& mat_b     /**< [in] input matrix B */
      );

   /**
    * Addition of two \f$ \Re^{2 * 2 } \f$ matrices: \f[ res\_mat = mat\_A + mat\_B \f] See [Wikipedia: Matrix_addition](https:\\en.wikipedia.org/wiki/Matrix_addition)
    *
    * \ingroup Matrix_2x2_cpp
    * \sdd{WI-13896}
    */
   inline Matrix_2X2_T operator+(
      const Matrix_2X2_T& mat_a,    /**< [in] input matrix A */
      const Matrix_2X2_T& mat_b     /**< [in] input matrix B */
      );

   /**
    * Unary minus
    */
   inline Matrix_2X2_T operator-(
      const Matrix_2X2_T& mat /**< [in] input matrix */
      );

   inline Matrix_2X2_T operator+(
      Matrix_2X2_T& mat /**< [in] input matrix */
      );

   inline Matrix_2X2_T operator-(
      const Matrix_2X2_T& mat_a,    /**< [in] input matrix A */
      const Matrix_2X2_T& mat_b     /**< [in] input matrix B */
      );

   inline Matrix_2X2_T operator-=(
      Matrix_2X2_T&       mat_a, /**< [in] input matrix A */
      const Matrix_2X2_T& mat_b  /**< [in] input matrix B */
      );

   /**
    * Addition of two \f$ \Re^{2 * 2 } \f$ matrices: \f[ res\_mat = mat\_A + mat\_B \f] See [Wikipedia: Matrix_addition](https:\\en.wikipedia.org/wiki/Matrix_addition)
    *
    * \ingroup Matrix_2x2_cpp
    * \sdd{WI-13896}
    */
   inline Matrix_2X2_T operator+=(
      Matrix_2X2_T&       mat_a, /**< [in] input matrix A */
      const Matrix_2X2_T& mat_b  /**< [in] input matrix B */
      );

   /**
    * Multiplication of a \f$ \Re^{2 * 2 } \f$ matrix with a scalar: \f[ res\_mat = mat * scalar \f] See [Wikipedia: Scalar_multiplication](https:\\en.wikipedia.org/wiki/Scalar_multiplication)
    *
    * \ingroup Matrix_2x2_cpp
    * \sdd{WI-13894}
    */
   inline Matrix_2X2_T operator*(
      const Matrix_2X2_T& mat,      /**< [in] input matrix */
      const float32_T     scalar    /**< [in] scalar */
      );

   /**
    * Multiplication of a \f$ \Re^{2 * 2 } \f$ matrix with a scalar: \f[ res\_mat = mat * scalar \f] See [Wikipedia: Scalar_multiplication](https:\\en.wikipedia.org/wiki/Scalar_multiplication)
    *
    * \ingroup Matrix_2x2_cpp
    * \sdd{WI-13894}
    */
   inline Matrix_2X2_T operator*=(
      Matrix_2X2_T&   mat,      /**< [in] input matrix */
      const float32_T scalar    /**< [in] scalar */
      );

   /**
    * Multiplication of a \f$ \Re^{2 * 2 } \f$ matrix with a scalar: \f[ res\_mat = mat * scalar \f] See [Wikipedia: Scalar_multiplication](https:\\en.wikipedia.org/wiki/Scalar_multiplication)
    *
    * \ingroup Matrix_2x2_cpp
    * \sdd{WI-13894}
    */
   inline Matrix_2X2_T operator*(
      const float32_T     scalar, /**< [in] scalar */
      const Matrix_2X2_T& mat     /**< [in] input matrix */
      );

   /**
    * Multiplication of the given \f$ \Re^{2 * 2 } \f$ matrix and the given vector: \f[ vector\_res = matrix * \vec{vector} \f] See [Wikipedia:
    *Matrix_multiplication](https:\\en.wikipedia.org/wiki/Matrix_multiplication)
    *
    * \ingroup Matrix_2x2_cpp
    * \sdd{WI-13904}
    */
   inline Vector_2d_T operator*(
      const Matrix_2X2_T& matrix, /**< [in] input matrix */
      const Vector_2d_T&  vector  /**< [in] input vector */
      );

   /**
   * Multiplication of the given vector and the given \f$ \Re^{2 * 2 } \f$ matrix: \f[ vector\_res = \vec{vector} * matrix \f] See [Wikipedia:
   *Matrix_multiplication](https:\\en.wikipedia.org/wiki/Matrix_multiplication)
   *
   * \ingroup Matrix_2x2_cpp
   * \sdd{WI-13904}
   */
   inline Vector_2d_T operator*(
      const Vector_2d_T&  vector, /**< [in] input vector */
      const Matrix_2X2_T& matrix  /**< [in] input matrix */

      );

   /**
    * Transposes the given \f$ \Re^{2 * 2 } \f$  matrix: \f[ matrix^T \f] See [Wikipedia: Transpose](https:\\en.wikipedia.org/wiki/Transpose)
    *
    * \ingroup Matrix_2x2_cpp
    * \sdd{WI-13908}
    */
   inline Matrix_2X2_T Transpose(const Matrix_2X2_T& matrix /**< [in] matrix */
   );

   /**
    * Returns the determinant of the given \f$ \Re^{2 * 2 } \f$  matrix: \f[ det = \left | matrix \right | \f] See [Wikipedia: Determinant](https:\\en.wikipedia.org/wiki/Determinant)
    *
    * \ingroup Matrix_2x2_cpp
    * \sdd{WI-13898}
    */
   inline float32_T Determinant(const Matrix_2X2_T& matrix /**< [in] input matrix */
   );

   /**
    * Calculates the inverse of the given \f$ \Re^{2 * 2 } \f$ matrix when determinate is given: \f[ matrix^{-1} =
    * \left [ \begin{array}{cc}
    *   a & b \\ c & d \\
    * \end{array}\right ] ^{-1} =
    * \frac{1}{\det matrix} \left [ \begin{array}{cc}
    *  \,\,\,d & \!\!-b \\ -c & \,a \\
    *  \end{array} \right ] \f]
    * See [Wikipedia: Inversion_of_2_x_2_matrices](https:\\en.wikipedia.org/wiki/Invertible_matrix#Inversion_of_2_%C3%97_2_matrices)
    * \return Inverted matrix if inversion is possible. Call \ref st::Matrix_Inversion_Possible() to find out beforehand.
    *
    * \ingroup Matrix_2x2_cpp
    * \Caveats        This function should not be called with zero determinant.
    *                 If determinant is ~0, all the elements of inverse matrix will be INFINITY
    * \sdd{WI-13910}
    * \sdd{WI-13903}
    */
   inline Matrix_2X2_T Inverse(
      const Matrix_2X2_T& matrix, /**< [in] input matrix */
      const float32_T     det     /**< [in] determinant of given matrix */
   );

   /**
    * \return a 2d vector representing the given row_index in given matrix
    *
    * \ingroup Matrix_2x2_cpp
    */
   inline Vector_2d_T Row(
      const Matrix_2X2_T& matrix,      /**< [in] matrix */
      const size_t        row_index    /**< [in] index of row */
   );

   /**
    * \return a 2d vector representing the given column_index in given matrix
    *
    * \ingroup Matrix_2x2_cpp
    */
   inline Vector_2d_T Column(
      const Matrix_2X2_T& matrix,         /**< [in] matrix */
      const size_t        column_index    /**< [in] index of column_index */
   );



   namespace matrix_2x2 {
      inline Matrix_2X2_T Identity()
      {
         Matrix_2X2_T res_mat;

         res_mat.elements[0][0] = 1.0f;
         res_mat.elements[0][1] = 0.0f;

         res_mat.elements[1][0] = 0.0f;
         res_mat.elements[1][1] = 1.0f;
         return res_mat;
      }

      inline Matrix_2X2_T Diagonal(
         Vector_2d_T v
      )
      {
         Matrix_2X2_T res_mat;

         res_mat.elements[0][0] = v.x;
         res_mat.elements[0][1] = 0.0f;

         res_mat.elements[1][0] = 0.0f;
         res_mat.elements[1][1] = v.y;
         return res_mat;
      }

      inline Matrix_2X2_T Diagonal(
         float32_T elem_0_0,
         float32_T elem_1_1
         )
      {
         Matrix_2X2_T res_mat;

         res_mat.elements[0][0] = elem_0_0;
         res_mat.elements[0][1] = 0.0f;

         res_mat.elements[1][0] = 0.0f;
         res_mat.elements[1][1] = elem_1_1;
         return res_mat;
      }


      inline Matrix_2X2_T Zero()
      {
         Matrix_2X2_T res_mat;

         res_mat.elements[0][0] = 0.0f;
         res_mat.elements[0][1] = 0.0f;

         res_mat.elements[1][0] = 0.0f;
         res_mat.elements[1][1] = 0.0f;

         return res_mat;
      }


      inline Matrix_2X2_T Column(
         Vector_2d_T& column0,
         Vector_2d_T& column1)
      {
         Matrix_2X2_T res_mat;

         res_mat.elements[0][0] = column0.x;
         res_mat.elements[1][0] = column0.y;

         res_mat.elements[0][1] = column1.x;
         res_mat.elements[1][1] = column1.y;

         return res_mat;
      }


      inline Matrix_2X2_T Row(
         Vector_2d_T& row0,
         Vector_2d_T& row1)
      {
         Matrix_2X2_T res_mat;

         res_mat.elements[0][0] = row0.x;
         res_mat.elements[0][1] = row0.y;

         res_mat.elements[1][0] = row1.x;
         res_mat.elements[1][1] = row1.y;

         return res_mat;
      }
   }

   inline Matrix_2X2_T operator*(
      const Matrix_2X2_T& mat_a,
      const Matrix_2X2_T& mat_b
      )
   {
      Matrix_2X2_T res_mat;

      for (size_t i = 0; i < 2; i++)
      {
         for (size_t j = 0; j < 2; j++)
         {
            uint8_t   k;
            float32_T element = 0.0f;
            for (k = 0; k < 2; k++)
            {
               element += mat_a.elements[i][k] * mat_b.elements[k][j];
            }
            res_mat.elements[i][j] = element;
         }
      }
      return res_mat;
   }


   inline Matrix_2X2_T operator+(
      const Matrix_2X2_T& mat_a,
      const Matrix_2X2_T& mat_b
      )
   {
      Matrix_2X2_T res_mat;

      for (size_t i = 0; i < 2; i++)
      {
         for (size_t j = 0; j < 2; j++)
         {
            res_mat.elements[i][j] = mat_a.elements[i][j] + mat_b.elements[i][j];
         }
      }
      return res_mat;
   }


   inline Matrix_2X2_T operator-(
      const Matrix_2X2_T& mat
      )
   {
      Matrix_2X2_T mat_res;
      for (size_t i = 0; i < 2; i++)
      {
         for (size_t j = 0; j < 2; j++)
         {
            mat_res.elements[i][j] = mat.elements[i][j] * -1.0f;
         }
      }
      return mat_res;
   }


   inline Matrix_2X2_T operator+(
      Matrix_2X2_T& mat
      )
   {
      return mat;
   }


   inline Matrix_2X2_T operator-(
      const Matrix_2X2_T& mat_a,
      const Matrix_2X2_T& mat_b
      )
   {
      Matrix_2X2_T helper = mat_b;

      return mat_a + (-helper);
   }


   inline Matrix_2X2_T operator-=(
      Matrix_2X2_T&       mat_a,
      const Matrix_2X2_T& mat_b
      )
   {
      Matrix_2X2_T helper = mat_b;

      mat_a = mat_a + (-helper);
      return mat_a;
   }


   inline Matrix_2X2_T operator+=(
      Matrix_2X2_T&       mat_a,
      const Matrix_2X2_T& mat_b
      )
   {
      mat_a = mat_a + mat_b;
      return mat_a;
   }


   inline Matrix_2X2_T operator*(
      const Matrix_2X2_T& mat,
      const float32_T     scalar
      )
   {
      Matrix_2X2_T res_mat;

      for (size_t i = 0; i < 2; i++)
      {
         for (size_t j = 0; j < 2; j++)
         {
            res_mat.elements[i][j] = mat.elements[i][j] * scalar;
         }
      }
      return res_mat;
   }


   inline Matrix_2X2_T operator*=(
      Matrix_2X2_T&   mat,
      const float32_T scalar
      )
   {
      for (size_t i = 0; i < 2; i++)
      {
         for (size_t j = 0; j < 2; j++)
         {
            mat.elements[i][j] = mat.elements[i][j] * scalar;
         }
      }
      return mat;
   }


   inline Matrix_2X2_T operator*(
      const float32_T     scalar,
      const Matrix_2X2_T& mat
      )
   {
      return mat * scalar;
   }


   inline Vector_2d_T operator*(
      const Matrix_2X2_T& matrix,
      const Vector_2d_T&  vector
      )
   {
      Vector_2d_T vector_res;

      vector_res.x = (matrix.elements[0][0] * vector.x) + (matrix.elements[0][1] * vector.y);
      vector_res.y = (matrix.elements[1][0] * vector.x) + (matrix.elements[1][1] * vector.y);
      return vector_res;
   }

   inline Vector_2d_T operator*(
      const Vector_2d_T&  vector,
      const Matrix_2X2_T& matrix
      )
   {
      Vector_2d_T vector_res;

      vector_res.x = (vector.x * matrix.elements[0][0]) + (vector.y * matrix.elements[1][0]);
      vector_res.y = (vector.x * matrix.elements[0][1]) + (vector.y * matrix.elements[1][1]);
      return vector_res;
   }


   inline Matrix_2X2_T Transpose(
      const Matrix_2X2_T& matrix
   )
   {
      Matrix_2X2_T ret = matrix;

      std::swap(ret.elements[0][1], ret.elements[1][0]);
      return ret;
   }


   inline float32_T Determinant(
      const Matrix_2X2_T& matrix
   )
   {
      return (matrix.elements[0][0] * matrix.elements[1][1]) -
             (matrix.elements[1][0] * matrix.elements[0][1]);
   }


   inline Matrix_2X2_T Inverse(
      const Matrix_2X2_T& matrix,
      const float32_T     det
   )
   {
      /* Find the inverse if determinant is non-zero, otherwise set the elements of inverse matrix to INFINITY
       * so that program can continue in RELEASE or embedded build */
      if (Matrix_Inversion_Possible(det))
      {
         float32_T det_inv;
         Matrix_2X2_T mat_inv;
         det_inv = 1.f / det;
         mat_inv.elements[0][0] =  matrix.elements[1][1] * det_inv;
         mat_inv.elements[0][1] = -matrix.elements[0][1] * det_inv;
         mat_inv.elements[1][0] = -matrix.elements[1][0] * det_inv;
         mat_inv.elements[1][1] =  matrix.elements[0][0] * det_inv;

         return mat_inv;
      }
      else
      {
         /* Set assert if absolute value of determinant is ~0
         * as matrix with zero determinant will not have an inverse*/
         /** \throws Assertion if given det is smaller than THRESHOLD_IS_ZERO */
         assert(false);

         return ml::matrix_2x2::Zero();
      }
   }


   inline Vector_2d_T Row(
      const Matrix_2X2_T& matrix,
      const size_t        row_index
   )
   {
      Vector_2d_T ret;

      ret.x = matrix.elements[row_index][0];
      ret.y = matrix.elements[row_index][1];
      return ret;
   }


   inline Vector_2d_T Column(
      const Matrix_2X2_T& matrix,
      const size_t        column_index
   )
   {
      Vector_2d_T ret;

      ret.x = matrix.elements[0][column_index];
      ret.y = matrix.elements[1][column_index];
      return ret;
   }
}
#endif


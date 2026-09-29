#ifndef ML_MATRIX_3X3_HPP
#define ML_MATRIX_3X3_HPP
/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

extern "C"
{
#include "reuse.h"
}
#include "st_matrix_3x3_t.h"
#include "ml_vector_3d_t.h"
#include "ml_vector_3d_tag.h"

#include "ml_matrix_tools.hpp"
#include <algorithm>
#include <cassert>

/** \file
 * Functions to do \f$ \Re^{3 x 3 } \f$ matrix operations.
 * \defgroup Matrix Matrix
 * Matrix types and operations. Currently only \f$ \Re^{2 * 2 } \f$ and \f$ \Re^{3 x 3 } \f$ matrices are supported.
 * See [Wikipedia: Matrix_(mathematics) => Basic operations](https:\\en.wikipedia.org/wiki/Matrix_(mathematics)#Basic_operations)
 */

 /**
 * \defgroup Matrix_3x3_cpp Matrix 3x3 C++
 * Functions that allow to work with \ref Matrix_3X3_T structures in C++.
 */

namespace ml {
   namespace matrix_3x3 {
      /**
      * Create an identity \f$ \Re^{3 x 3 } \f$ matrix. See [Wikipedia: Identity Matrix](https:\\en.wikipedia.org/wiki/Identity_matrix)
      *
      * \ingroup Matrix_3x3_cpp
      * \sdd{WI-13909}
      */
      inline Matrix_3X3_T Identity();


      /**
      * Create a diagonal \f$ \Re^{ 3 * 3 } \f$ matrix based on given vector.
      See[Wikipedia:Identity Matrix](https:\\en.wikipedia.org/wiki/Diagonal_matrix)
      *
      * \ingroup Matrix_3x3_cpp
      * \sdd{ WI - 13902 }
      */
      inline Matrix_3X3_T Diagonal(
         float32_T elem_0_0,
         float32_T elem_1_1,
         float32_T elem_2_2
      );

      /**
      * Create a zero \f$ \Re^{3 x 3 } \f$ matrix. See [Wikipedia: Zero Matrix](https:\\en.wikipedia.org/wiki/Zero_matrix)
      *
      * \ingroup Matrix_3x3_cpp
      * \sdd{WI-13897}
      */
      inline Matrix_3X3_T Zero();
   }
   /**
   * Multiplication of two \f$ \Re^{3 x 3 } \f$ matrices: \f[ res\_mat = mat\_A * mat\_B \f] See [Wikipedia: Matrix_multiplication](https:\\en.wikipedia.org/wiki/Matrix_multiplication)
   *
   * \ingroup Matrix_3x3_cpp
   * \sdd{WI-13899}
   */
   inline Matrix_3X3_T operator*(
      const Matrix_3X3_T &mat_a, /**< [in] input matrix A */
      const Matrix_3X3_T &mat_b  /**< [in] input matrix B */
      );

   /**
   * Multiplication of two \f$ \Re^{3 x 3 } \f$ matrices: \f[ res\_mat = mat\_A + mat\_B \f] See [Wikipedia: Matrix_addition](https:\\en.wikipedia.org/wiki/Matrix_addition)
   *
   * \ingroup Matrix_3x3_cpp
   * \sdd{WI-13893}
   */
   inline Matrix_3X3_T operator+(
      const Matrix_3X3_T &mat_a, /**< [in] input matrix A */
      const Matrix_3X3_T &mat_b  /**< [in] input matrix B */
      );

   inline Matrix_3X3_T operator-(
      const Matrix_3X3_T &mat   /**< [in] input matrix */
      );

   inline Matrix_3X3_T operator+(
      Matrix_3X3_T &mat   /**< [in] input matrix */
      );

   inline Matrix_3X3_T operator-(
      const Matrix_3X3_T &mat_a, /**< [in] input matrix A */
      const Matrix_3X3_T &mat_b  /**< [in] input matrix B */
      );

   inline Matrix_3X3_T operator-=(
            Matrix_3X3_T &mat_a, /**< [in] input matrix A */
      const Matrix_3X3_T &mat_b  /**< [in] input matrix B */
      );

   /**
   * Multiplication of two \f$ \Re^{3 x 3 } \f$ matrices: \f[ res\_mat = mat\_A + mat\_B \f] See [Wikipedia: Matrix_addition](https:\\en.wikipedia.org/wiki/Matrix_addition)
   *
   * \ingroup Matrix_3x3_cpp
   * \sdd{WI-13893}
   */
   inline Matrix_3X3_T operator+=(
            Matrix_3X3_T &mat_a,   /**< [in] input matrix A */
      const Matrix_3X3_T &mat_b /**< [in] input matrix B */
      );

   /**
   * Multiplication of a \f$ \Re^{3 x 3 } \f$ matrix with a scalar: \f[ res\_mat = mat * scalar \f] See [Wikipedia: Scalar_multiplication](https:\\en.wikipedia.org/wiki/Scalar_multiplication)
   *
   * \ingroup Matrix_3x3_cpp
   * \sdd{WI-13895}
   */
   inline Matrix_3X3_T operator*(
      const Matrix_3X3_T &mat,   /**< [in] input matrix */
      const float32_T     scalar /**< [in] scalar */
      );

   /**
   * Multiplication of a \f$ \Re^{3 x 3 } \f$ matrix with a scalar: \f[ res\_mat = mat * scalar \f] See [Wikipedia: Scalar_multiplication](https:\\en.wikipedia.org/wiki/Scalar_multiplication)
   *
   * \ingroup Matrix_3x3_cpp
   * \sdd{WI-13895}
   */
   inline Matrix_3X3_T operator*=(
            Matrix_3X3_T &mat,   /**< [in] input matrix */
      const float32_T     scalar /**< [in] scalar */
      );

   /**
   * Multiplication of a \f$ \Re^{3 x 3 } \f$ matrix with a scalar: \f[ res\_mat = mat * scalar \f] See [Wikipedia: Scalar_multiplication](https:\\en.wikipedia.org/wiki/Scalar_multiplication)
   *
   * \ingroup Matrix_3x3_cpp
   * \sdd{WI-13895}
   */
   inline Matrix_3X3_T operator*(
      const float32_T    scalar, /**< [in] scalar */
      const Matrix_3X3_T &mat   /**< [in] input matrix */
      );

   /**
   * Multiplication of a \f$ \Re^{3 x 3 } \f$ matrix with a 3d-vector: \f[ res\_mat = mat * \vec{vector} \f]
   * * See [Wikipedia:Matrix_multiplication](https:\\en.wikipedia.org/wiki/Matrix_multiplication)
   *
   * \ingroup Matrix_3x3_cpp
   * \sdd{WI-13895}
   */
   inline Vector_3d_T operator*(
      const Matrix_3X3_T &matrix, /**< [in] input matrix */
      const Vector_3d_T  &vector  /**< [in] vector */
      );

   /**
   * Multiplication of a \f$ \Re^{3 x 3 } \f$ matrix with a 3d-vector: \f[ res\_mat = mat * \vec{vector} \f]
   * * See [Wikipedia:Matrix_multiplication](https:\\en.wikipedia.org/wiki/Matrix_multiplication)
   *
   * \ingroup Matrix_3x3_cpp
   * \sdd{WI-13895}
   */
   inline Vector_3d_T operator*(
      const Vector_3d_T  &vector, /**< [in] vector */
      const Matrix_3X3_T &matrix  /**< [in] input matrix */
      );

   /**
   * Transposes the given \f$ \Re^{3 x 3 } \f$ matrix: \f[ matrix^T \f] See [Wikipedia: Transpose](https:\\en.wikipedia.org/wiki/Transpose)
   *
   * \ingroup Matrix_3x3_cpp
   * \sdd{WI-13913}
   */
   inline Matrix_3X3_T Transpose(
      const Matrix_3X3_T &matrix /**< [in] matrix to be transposed*/
   );

   /**
   * Returns the determinant of a \f$ \Re^{3 x 3 } \f$ matrix: \f[ det = \left | matrix \right | \f] See [Wikipedia: Determinant](https:\\en.wikipedia.org/wiki/Determinant)
   * \return Determinant of given matrix
   * \ingroup Matrix_3x3_cpp
   * \sdd{WI-13905}
   */
   inline float32_T Determinant(
      const Matrix_3X3_T &matrix /**< [in] input matrix */
   );

   /**
   * Calculates the inverse of the given \f$ \Re^{2 * 2 } \f$ matrix when determinate is given: \f[ matrix^{-1} =
   * \left [ \begin{array}{ccc}
   *   a & b & c \\ d & e & f \\ g & h & i
   * \end{array}\right ] ^{-1} =
   * \frac{1}{\det matrix} \left [ \begin{array}{ccc}
   *   a & b & c \\ d & e & f \\ g & h & i
   * \end{array}\right ] ^{T} \f]
   * See [Wikipedia: Inversion_of_3_x_3_matrices](https:\\en.wikipedia.org/wiki/Invertible_matrix#Inversion_of_3_%C3%97_3_matrices)
   * \return Inverted matrix if inversion is possible. Call \ref st::Matrix_Inversion_Possible() to find out beforehand.
   *
   * \ingroup Matrix_3x3_cpp
   * \Caveats        This function should not be called with zero determinant.
   *                 If determinant is ~0, all the elements of inverse matrix will be INFINITY
   * \sdd{WI-13910}
   * \sdd{WI-13903}
   */
   inline Matrix_3X3_T Inverse(
      const Matrix_3X3_T    &matrix, /**< [in] input matrix */
      const float32_T  det /**< [in] determinant of given matrix */
   );

   namespace matrix_3x3 {
      inline Matrix_3X3_T Identity()
      {
         Matrix_3X3_T res_mat;
         res_mat.elements[0][0] = 1.0f;
         res_mat.elements[0][1] = 0.0f;
         res_mat.elements[0][2] = 0.0f;

         res_mat.elements[1][0] = 0.0f;
         res_mat.elements[1][1] = 1.0f;
         res_mat.elements[1][2] = 0.0f;

         res_mat.elements[2][0] = 0.0f;
         res_mat.elements[2][1] = 0.0f;
         res_mat.elements[2][2] = 1.0f;
         return res_mat;
      }

      inline Matrix_3X3_T Diagonal(
         float32_T elem_0_0,
         float32_T elem_1_1,
         float32_T elem_2_2
      )
      {
         Matrix_3X3_T res_mat;
         res_mat.elements[0][0] = elem_0_0;
         res_mat.elements[0][1] = 0.0f;
         res_mat.elements[0][2] = 0.0f;

         res_mat.elements[1][0] = 0.0f;
         res_mat.elements[1][1] = elem_1_1;
         res_mat.elements[1][2] = 0.0f;

         res_mat.elements[2][0] = 0.0f;
         res_mat.elements[2][1] = 0.0f;
         res_mat.elements[2][2] = elem_2_2;
         return res_mat;
      }


      inline Matrix_3X3_T Zero()
      {
         Matrix_3X3_T res_mat;

         res_mat.elements[0][0] = 0.0f;
         res_mat.elements[0][1] = 0.0f;
         res_mat.elements[0][2] = 0.0f;

         res_mat.elements[1][0] = 0.0f;
         res_mat.elements[1][1] = 0.0f;
         res_mat.elements[1][2] = 0.0f;

         res_mat.elements[2][0] = 0.0f;
         res_mat.elements[2][1] = 0.0f;
         res_mat.elements[2][2] = 0.0f;

         return res_mat;
      }
   }

   inline Matrix_3X3_T operator*(
      const Matrix_3X3_T &mat_a,
      const Matrix_3X3_T &mat_b
   )
   {
      Matrix_3X3_T res_mat;

      for (size_t i = 0; i < 3; i++)
      {
         for (size_t j = 0; j < 3; j++)
         {
            float32_T element = 0.0f;
            for (size_t k = 0; k < 3; k++)
            {
               element += mat_a.elements[i][k] * mat_b.elements[k][j];
            }
            res_mat.elements[i][j] = element;
         }
      }
      return res_mat;
   }

   inline Matrix_3X3_T operator+(
      const Matrix_3X3_T &mat_a,
      const Matrix_3X3_T &mat_b
      )
   {
      Matrix_3X3_T res_mat;

      for (size_t i = 0; i < 3; i++)
      {
         for (size_t j = 0; j < 3; j++)
         {
            res_mat.elements[i][j] = mat_a.elements[i][j] + mat_b.elements[i][j];
         }
      }
      return res_mat;
   }

   inline Matrix_3X3_T operator-(
      const Matrix_3X3_T &mat
      )
   {
      Matrix_3X3_T mat_ret;
      for (size_t i = 0; i < 3; i++)
      {
         for (size_t j = 0; j < 3; j++)
         {
            mat_ret.elements[i][j] = mat.elements[i][j] * -1.0f;
         }
      }
      return mat_ret;
   }

   inline Matrix_3X3_T operator+(
      Matrix_3X3_T &mat
      )
   {
      return mat;
   }

   inline Matrix_3X3_T operator-(
      const Matrix_3X3_T &mat_a,
      const Matrix_3X3_T &mat_b
      )
   {
      Matrix_3X3_T helper = mat_b;
      return mat_a + (-helper);
   }

   inline Matrix_3X3_T operator-=(
      Matrix_3X3_T &mat_a,
      const Matrix_3X3_T &mat_b
      )
   {
      Matrix_3X3_T helper = mat_b;
      mat_a = mat_a + (-helper);
      return mat_a;
   }

   inline Matrix_3X3_T operator+=(
            Matrix_3X3_T &mat_a,
      const Matrix_3X3_T &mat_b
      )
   {
      mat_a = mat_a + mat_b;
      return mat_a;
   }

   inline Matrix_3X3_T operator*(
      const Matrix_3X3_T &mat,
      const float32_T     scalar
   )
   {
      Matrix_3X3_T res_mat;
      for (size_t i = 0; i < 3; i++)
      {
         for (size_t j = 0; j < 3; j++)
         {
            res_mat.elements[i][j] = mat.elements[i][j] * scalar;
         }
      }
      return res_mat;
   }

   inline Matrix_3X3_T operator*=(
            Matrix_3X3_T &mat,
      const float32_T     scalar
      )
   {
      for (size_t i = 0; i < 3; i++)
      {
         for (size_t j = 0; j < 3; j++)
         {
            mat.elements[i][j] = mat.elements[i][j] * scalar;
         }
      }
      return mat;
   }

   inline Matrix_3X3_T operator*(
      const float32_T    scalar,
      const Matrix_3X3_T &mat
      )
   {
      Matrix_3X3_T res_mat;
      for (size_t i = 0; i < 3; i++)
      {
         for (size_t j = 0; j < 3; j++)
         {
            res_mat.elements[i][j] = mat.elements[i][j] * scalar;
         }
      }
      return res_mat;
   }

   inline Vector_3d_T operator*(
      const Matrix_3X3_T &matrix,
      const Vector_3d_T  &vector
      )
   {
      Vector_3d_T vector_res;

      vector_res.x = (matrix.elements[0][0] * vector.x) + (matrix.elements[0][1] * vector.y) + (matrix.elements[0][2] * vector.z);
      vector_res.y = (matrix.elements[1][0] * vector.x) + (matrix.elements[1][1] * vector.y) + (matrix.elements[1][2] * vector.z);
      vector_res.z = (matrix.elements[2][0] * vector.x) + (matrix.elements[2][1] * vector.y) + (matrix.elements[2][2] * vector.z);
      return vector_res;
   }

   inline Vector_3d_T operator*(
      const Vector_3d_T&  vector,
      const Matrix_3X3_T& matrix
      )
   {
      Vector_3d_T vector_res;

      vector_res.x = (vector.x * matrix.elements[0][0]) + (vector.y * matrix.elements[1][0]) + + (vector.z * matrix.elements[2][0]);
      vector_res.y = (vector.x * matrix.elements[0][1]) + (vector.y * matrix.elements[1][1]) + + (vector.z * matrix.elements[2][1]);
      vector_res.z = (vector.x * matrix.elements[0][2]) + (vector.y * matrix.elements[1][2]) + + (vector.z * matrix.elements[2][2]);
      return vector_res;
   }

   inline Matrix_3X3_T Transpose(
      const Matrix_3X3_T &matrix
   )
   {
      Matrix_3X3_T ret = matrix;
      std::swap(ret.elements[0][1], ret.elements[1][0]);
      std::swap(ret.elements[0][2], ret.elements[2][0]);
      std::swap(ret.elements[1][2], ret.elements[2][1]);
      return ret;
   }

   inline float32_T Determinant(
      const Matrix_3X3_T &matrix
   )
   {
      return ((matrix.elements[0][0] * matrix.elements[1][1] * matrix.elements[2][2]) +
              (matrix.elements[0][1] * matrix.elements[1][2] * matrix.elements[2][0]) +
              (matrix.elements[0][2] * matrix.elements[1][0] * matrix.elements[2][1])) -
              ((matrix.elements[0][2] * matrix.elements[1][1] * matrix.elements[2][0]) +
              (matrix.elements[0][0] * matrix.elements[1][2] * matrix.elements[2][1]) +
              (matrix.elements[0][1] * matrix.elements[1][0] * matrix.elements[2][2]));
   }

   inline Matrix_3X3_T Inverse(
      const Matrix_3X3_T &matrix,
      const float32_T     det
   )
   {
      /* Find the inverse if determinant is non-zero, otherwise set the elements of inverse matrix to INFINITY
      * so that program can continue in RELEASE or embedded build */
      if (Matrix_Inversion_Possible(det))
      {
         float32_T det_inv;
         Matrix_3X3_T mat_inv;
         det_inv = 1 / det;
         Matrix_3X3_T trans = Transpose(matrix);
         Matrix_3X3_T cof;
         float32_T fact = 1.0f;
         for(size_t row_index=0; row_index<3; row_index++)
         {
            for(size_t column_index=0; column_index<3; column_index++)
            {
               Matrix_2X2_T sub = Submatrix(trans, row_index, column_index);
               float32_T det = Determinant(sub);
               cof.elements[row_index][column_index] = det * fact;
               fact *= -1.0f;
            }
         }
         mat_inv = det_inv * cof;
         return mat_inv;
      }
      else
      {
         /* Set assert if absolute value of determinant is ~0
         * as matrix with zero determinant will not have an inverse*/
         /** \throws Assertion if given det is smaller than THRESHOLD_IS_ZERO */
         assert(false);
         return ml::matrix_3x3::Zero();
      }
   }

   inline Vector_3d_T Row(
      const Matrix_3X3_T& matrix,
      const size_t        row_index
   )
   {
      Vector_3d_T ret;

      ret.x = matrix.elements[row_index][0];
      ret.y = matrix.elements[row_index][1];
      ret.z = matrix.elements[row_index][2];
      return ret;
   }


   inline Vector_3d_T Column(
      const Matrix_3X3_T& matrix,
      const size_t        column_index
   )
   {
      Vector_3d_T ret;

      ret.x = matrix.elements[0][column_index];
      ret.y = matrix.elements[1][column_index];
      ret.z = matrix.elements[2][column_index];
      return ret;
   }
}
#endif


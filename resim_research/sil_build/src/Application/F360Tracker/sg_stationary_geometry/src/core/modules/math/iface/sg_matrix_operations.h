/*=============================================================================================*\
* FILE: sg_matrix_operatios.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
*  This file contains functionalities for matrices operations like:
*      - multiplying matrix by matrix
*      - multiplying matrix by scalar
*      - dividing matrix by scalar
*      - adding matrices
*      - subtracting matrices
*      - transposing
*      - inverse 2x2 matrix
*      - identity matrix generation
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_MATRIX_OPERATIONS_H
#define SG_MATRIX_OPERATIONS_H

#include <array>
#include <cmath>
#include <limits>
#include <type_traits>

namespace sg
{
   constexpr static float matrix_epsilon = std::numeric_limits<float>::epsilon() * 10.0F;
   template <typename T, std::size_t NUM_ROW, std::size_t NUM_COL>
   using Matrix = std::array<std::array<T, NUM_COL>, NUM_ROW>;

   /**
    * @brief    operator*() for multiplying two matrices: [ NUM_ROW_LHS x NUM_ROW_COL ] * [ NUM_ROW_COL x NUM_COL_RHS ]
    *
    * @param    const Matrix<T, NUM_ROW_LHS, NUM_ROW_COL>& matrix_1
    *           const Matrix<T, NUM_ROW_COL, NUM_COL_RHS>& matrix_2
    *
    * @return   Matrix<T, NUM_ROW_LHS, NUM_COL_RHS>
    **/
   template <typename T,
             std::size_t NUM_ROW_LHS,
             std::size_t NUM_ROW_COL,
             std::size_t NUM_COL_RHS,
             typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   Matrix<T, NUM_ROW_LHS, NUM_COL_RHS> operator*(const Matrix<T, NUM_ROW_LHS, NUM_ROW_COL> &matrix_lhs,
                                                 const Matrix<T, NUM_ROW_COL, NUM_COL_RHS> &matrix_rhs)
   {
      Matrix<T, NUM_ROW_LHS, NUM_COL_RHS> result{};

      for (std::size_t i = 0U; i < NUM_ROW_LHS; i++)
      {
         for (std::size_t j = 0U; j < NUM_COL_RHS; j++)
         {
            for (std::size_t k = 0U; k < NUM_ROW_COL; k++)
            {
               result[i][j] += matrix_lhs[i][k] * matrix_rhs[k][j];
            }
         }
      }
      return result;
   }

   /**
    * @brief    operator*=() for multiplying matrix by number: [ NUM_ROW x NUM_COL ] * k
    *           and store results in same matrix
    *
    * @param    const Matrix<T, NUM_ROW, NUM_COL>& matrix
    *           const T k
    *
    * @return   N/A
    **/
   template <typename T, std::size_t NUM_ROW, std::size_t NUM_COL, typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
   void operator*=(Matrix<T, NUM_ROW, NUM_COL> &matrix, const T k)
   {
      for (std::size_t i = 0U; i < NUM_ROW; i++)
      {
         for (std::size_t j = 0U; j < NUM_COL; j++)
         {
            matrix[i][j] *= k;
         }
      }
   }

   /**
    * @brief    operator*() for multiplying matrix by number: [ M x N ] * k;
    *           and store results in new matrix.
    *
    * @param    const Matrix<T, NUM_ROW, NUM_COL>& matrix
    *           const T k
    *
    * @return   Matrix<T, NUM_ROW, NUM_COL>
    **/
   template <typename T, std::size_t NUM_ROW, std::size_t NUM_COL, typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
   Matrix<T, NUM_ROW, NUM_COL> operator*(const Matrix<T, NUM_ROW, NUM_COL> &matrix, const T k)
   {
      Matrix<T, NUM_ROW, NUM_COL> result{};

      for (std::size_t i = 0U; i < NUM_ROW; i++)
      {
         for (std::size_t j = 0U; j < NUM_COL; j++)
         {
            result[i][j] = matrix[i][j] * k;
         }
      }
      return result;
   }

   /**
    * @brief    operator+() for adding number to matrix: [ M x N ] + k;
    *           and store results in new matrix.
    *
    * @param    const Matrix<T, NUM_ROW, NUM_COL>& matrix
    *           const T k
    *
    * @return   Matrix<T, NUM_ROW, NUM_COL>
    **/
   template <typename T, std::size_t NUM_ROW, std::size_t NUM_COL, typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
   Matrix<T, NUM_ROW, NUM_COL> operator+(const Matrix<T, NUM_ROW, NUM_COL> &matrix, const T k)
   {
      Matrix<T, NUM_ROW, NUM_COL> result{};

      for (std::size_t i = 0U; i < NUM_ROW; i++)
      {
         for (std::size_t j = 0U; j < NUM_COL; j++)
         {
            result[i][j] = matrix[i][j] + k;
         }
      }
      return result;
   }

   /**
    * @brief    operator*() for multiplying scalar by matrix: k * [ M x N ];
    *           and store results in new matrix.
    *
    * @param    const T k
    *           const Matrix<T, NUM_ROW, NUM_COL>& matrix
    *
    * @return   Matrix<T, NUM_ROW, NUM_COL>
    **/
   template <typename T, std::size_t NUM_ROW, std::size_t NUM_COL, typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
   Matrix<T, NUM_ROW, NUM_COL> operator*(const T k, const Matrix<T, NUM_ROW, NUM_COL> &matrix)
   {
      return matrix * k;
   }

   /**
    * @brief    operator/() for dividing matrix by number: [ M x N ] / k;
    *           and store results in new matrix.
    *
    * @param    const Matrix<T, NUM_ROW, NUM_COL>& matrix
    *           const T k
    *
    * @return   Matrix<T, NUM_ROW, NUM_COL>
    **/
   template <typename T, std::size_t NUM_ROW, std::size_t NUM_COL, typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
   Matrix<T, NUM_ROW, NUM_COL> operator/(const Matrix<T, NUM_ROW, NUM_COL> &matrix, const T k)
   {
      Matrix<T, NUM_ROW, NUM_COL> result{};

      for (std::size_t i = 0U; i < NUM_ROW; i++)
      {
         for (std::size_t j = 0U; j < NUM_COL; j++)
         {
            result[i][j] = matrix[i][j] / k;
         }
      }
      return result;
   }

   /**
    * @brief    operator+() for adding two matrices: [ NUM_ROW x NUM_COL ] + [ NUM_ROW x NUM_COL ]
    *
    * @param    const Matrix<T, NUM_ROW, NUM_COL>& matrix_lhs
    *           const Matrix<T, NUM_ROW, NUM_COL>& matrix_rhs
    *
    * @return   Matrix<T, NUM_ROW, NUM_COL>
    **/
   template <typename T, std::size_t NUM_ROW, std::size_t NUM_COL, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   Matrix<T, NUM_ROW, NUM_COL> operator+(const Matrix<T, NUM_ROW, NUM_COL> &matrix_lhs, const Matrix<T, NUM_ROW, NUM_COL> &matrix_rhs)
   {
      Matrix<T, NUM_ROW, NUM_COL> result{};

      for (std::size_t i = 0U; i < NUM_ROW; i++)
      {
         for (std::size_t j = 0U; j < NUM_COL; j++)
         {
            result[i][j] = matrix_lhs[i][j] + matrix_rhs[i][j];
         }
      }
      return result;
   }

   /**
    * @brief    operator-() for subtraction two matrices: [ NUM_ROW x NUM_COL ] - [ NUM_ROW x NUM_COL ]
    *
    * @param    const Matrix<T, NUM_ROW, NUM_COL>& matrix_lhs
    *           const Matrix<T, NUM_ROW, NUM_COL>& matrix_rhs
    *
    * @return   Matrix<T, NUM_ROW, NUM_COL>
    **/
   template <typename T, std::size_t NUM_ROW, std::size_t NUM_COL, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   Matrix<T, NUM_ROW, NUM_COL> operator-(const Matrix<T, NUM_ROW, NUM_COL> &matrix_lhs, const Matrix<T, NUM_ROW, NUM_COL> &matrix_rhs)
   {
      Matrix<T, NUM_ROW, NUM_COL> result{};

      for (std::size_t i = 0U; i < NUM_ROW; i++)
      {
         for (std::size_t j = 0U; j < NUM_COL; j++)
         {
            result[i][j] = matrix_lhs[i][j] - matrix_rhs[i][j];
         }
      }
      return result;
   }

   /**
    * @brief    transpose matrix [ NUM_ROW x NUM_COL ]
    *
    * @param    const Matrix<T, NUM_ROW, NUM_COL>& matrix
    *
    * @return   Matrix<T, NUM_COL, NUM_ROW>
    **/
   template <typename T, std::size_t NUM_ROW, std::size_t NUM_COL, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
   Matrix<T, NUM_COL, NUM_ROW> transpose(const Matrix<T, NUM_ROW, NUM_COL> &matrix)
   {
      Matrix<T, NUM_COL, NUM_ROW> transposed_matrix{};

      for (std::size_t i = 0U; i < NUM_ROW; i++)
      {
         for (std::size_t j = 0U; j < NUM_COL; j++)
         {
            transposed_matrix[j][i] = matrix[i][j];
         }
      }
      return transposed_matrix;
   }

   /**
    * @brief    operation inverse of matrix 2x2
    *
    * @param    const Matrix<T, 2U, 2U>& matrix
    *
    * @return   Matrix<T, 2U, 2U>
    **/
   template <typename T, typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
   Matrix<T, 2U, 2U> inverse_matrix(const Matrix<T, 2U, 2U> &matrix)
   {
      Matrix<T, 2U, 2U> inv_matrix{};
      T determinant = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];

      if (std::fabs(determinant) > static_cast<T>(matrix_epsilon))
      {
         // make adjoint matrix from input matrix
         inv_matrix[0][0] = matrix[1][1];
         inv_matrix[1][1] = matrix[0][0];
         inv_matrix[0][1] = matrix[0][1] * static_cast<T>(-1);
         inv_matrix[1][0] = matrix[1][0] * static_cast<T>(-1);

         // calc inverted matrix
         inv_matrix *= static_cast<T>(1) / determinant;
      }
      else
      {
         inv_matrix[0][0] = std::numeric_limits<T>::infinity();
         inv_matrix[0][1] = std::numeric_limits<T>::infinity();
         inv_matrix[1][0] = std::numeric_limits<T>::infinity();
         inv_matrix[1][1] = std::numeric_limits<T>::infinity();
      }

      return inv_matrix;
   }

   /**
    * @brief    operation inverse of matrix 1x1
    *
    * @param    const Matrix<T, 1U, 1U>& matrix
    *
    * @return   float
    **/
   template <typename T, typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
   Matrix<T, 1U, 1U> inverse_matrix(const Matrix<T, 1U, 1U> &matrix)
   {
      Matrix<T, 1U, 1U> inv_matrix{};

      if (std::fabs(matrix[0U][0U]) > static_cast<T>(matrix_epsilon))
      {
         inv_matrix[0][0] = static_cast<T>(1) / matrix[0U][0U];
      }
      else
      {
         inv_matrix[0][0] = std::numeric_limits<T>::infinity();
      }

      return inv_matrix;
   }

   /**
    * @brief    returns identity matrix
    *
    * @param    N/A
    *
    * @return   Matrix<T, size, size>
    **/
   template <typename T, std::size_t size, typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
   Matrix<T, size, size> eye()
   {
      Matrix<T, size, size> out{};
      for (std::size_t idx = 0U; idx < size; ++idx)
      {
         out[idx][idx] = static_cast<T>(1);
      }
      return out;
   }

}
#endif

// f360_LinearSolvers.cpp : main project file.

#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_linear_solver_data_types.h"
#include "f360_matrix_vector_Init_real32_T.h"
#include "f360_LinearSolvers.h"

namespace f360_variant_A
{

   /****************************************************
   // name: qr_modified_gram_schimdt
   // in: Reference to Q,
   //     Reference to R,
   //     Reference to A
   // out:   Reference to Q
   //        Reference to R
   *****************************************************/
   void qr_modified_gram_schimdt(
      F360_matrix_real32_LSC_T& Q,
      F360_matrix_real32_LSC_T& R,
      const F360_matrix_real32_LSC_T& A)
   {
      const int m = A.m_size[0];
      const int n = A.m_size[1];

      static F360_matrix_real32_LSC_T VM;

      F360_matrix_Init_real32_T(Q, m, n);
      F360_matrix_Init_real32_T(R, n, n);
      F360_matrix_Copy_real32_T(VM, A);

      for (int i = 0; i < n; i++)
      {
         R.data[i][i] = euclid_norm(VM, m, i);
         my_rdivide(Q, VM, R, m, i);
         for (int j = i + 1; j < n; j++)
         {
            scalarProduct(R, Q, VM, i, j, m);
            updateVM(VM, Q, R, i, j, m);
         }
      }
   }

   /****************************************************
   // name: scalarProduct
   // in: Reference to R,
   //     Reference to Q,
   //     VM,
   //     col1 & col2
   //     number of rows,
   // out:   Reference to Q
   //        Reference to R
   *****************************************************/
   void scalarProduct(
      F360_matrix_real32_LSC_T& R,
      const F360_matrix_real32_LSC_T& Q,
      const F360_matrix_real32_LSC_T& VM,
      const int col1,
      const int col2,
      const int num_rows)
   {
      int i;
      float total = 0.0F;
      for (i = 0; i < num_rows; i++)
      {
         total = total + Q.data[i][col1] * VM.data[i][col2];
      }
      R.data[col1][col2] = total;
   }

   /****************************************************
   // name: my_rdivide
   // in: Reference to Q,
   //     VM,
   //     Reference to R,
   //     number of rows,
   //     number of cols
   // out:   Reference to Q
   //        Reference to R
   *****************************************************/
   void my_rdivide(
      F360_matrix_real32_LSC_T& Q,
      const F360_matrix_real32_LSC_T& VM,
      const F360_matrix_real32_LSC_T& R,
      const int num_rows,
      const int col)
   {
      for (int i = 0; i < num_rows; i++)
      {
         Q.data[i][col] = VM.data[i][col] / R.data[col][col];
      }
   }

   /****************************************************
   // name: euclid_norm
   // in: VM,
   //     number of rows,
   //     number of column
   // out:   Pointer to Q
   //        Pointer to R
   *****************************************************/
   float euclid_norm(
      const F360_matrix_real32_LSC_T& VM,
      const int num_rows,
      const int col)
   {
      float total = 0.0F;

      for (int i = 0; i < num_rows; i++)
      {
         total = total + VM.data[i][col] * VM.data[i][col];
      }
      return F360_Sqrtf(total);
   }

   void updateVM(
      F360_matrix_real32_LSC_T& VM,
      const F360_matrix_real32_LSC_T& Q,
      const F360_matrix_real32_LSC_T& R,
      const int row,
      const int col,
      const int num_rows)
   {
      for (int i = 0; i < num_rows; i++)
      {
         VM.data[i][col] = VM.data[i][col] - R.data[row][col] * Q.data[i][row];
      }
   }

   /*===========================================================================*\
    * FUNCTION: F360_Fit_Second_Degree_Polynomial()
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const F360_matrix_real32_LSC_T& A
    * const F360_vector_real32_LSC_T& B
    * float32_t& p2
    * float32_t& p1
    * float32_t& p0
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * Find the coefficients to a polynomial on the form y(x) = ax^2 + bx + c.
    * Function uses modified grahm smith (QR) factorisation and then solve by 
    * cholesky decomposition for numerical stability and efficiency
    * Solve for x in 
    * Ax = B
    *
    * PRECONDITIONS:
    * - Number of rows in A must be equal to number of rows in B.
    * - Number of columns in A must be equal to 3
    * - Number of rows in A and B must be equal or greater than 3
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   bool F360_Fit_Second_Degree_Polynomial(
      const F360_matrix_real32_LSC_T& A,
      const F360_vector_real32_LSC_T& B,
      float32_t& p2,
      float32_t& p1,
      float32_t& p0)
   {
      const uint32_t nr_coeffs = 3U;
      const int32_t nr_coeffs_signed = static_cast<int32_t>(nr_coeffs);

      bool f_poly_fit_ok;

      if ((A.m_size[0] == B.m_size) && (A.m_size[1] == nr_coeffs_signed) && (A.m_size[0] >= nr_coeffs_signed))
      {

         // Factorise A in to Q and R
         F360_matrix_real32_LSC_T Q;
         F360_matrix_real32_LSC_T R;
         qr_modified_gram_schimdt(Q, R, A);

         // Now solve for x
         // R * x = Q' * B
         // Transpose Q
         float32_t q_transpose[nr_coeffs][NUMBER_OF_OBJECT_TRACKS];
         F360_Transpose_2D(
            Q.data,
            q_transpose,
            static_cast<uint32_t>(Q.m_size[0]),
            nr_coeffs);

         // Multply Q' * b
         float32_t q_transposed_b[nr_coeffs];
         F360_Mat_Vec_Mul_MxN_N(
            q_transpose,
            B.data,
            q_transposed_b,
            nr_coeffs,
            static_cast<uint32_t>(Q.m_size[0]));

         if ((std::abs(R.data[0][0]) > F360_EPSILON) && (std::abs(R.data[1][1]) > F360_EPSILON) && (std::abs(R.data[2][2]) > F360_EPSILON))
         {
            f_poly_fit_ok = true;

            // Simple solve since R is upper triangular matrix
            p0 = q_transposed_b[2] / R.data[2][2];
            p1 = (q_transposed_b[1] - R.data[1][2] * p0) / R.data[1][1];
            p2 = (q_transposed_b[0] - R.data[0][1] * p1 - R.data[0][2] * p0) / R.data[0][0];
         }
         else
         {
            f_poly_fit_ok = false;
         }
         
      }
      else
      {
         f_poly_fit_ok = false;
      }

      return f_poly_fit_ok;
   }
}
